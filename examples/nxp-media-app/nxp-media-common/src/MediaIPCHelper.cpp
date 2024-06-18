#include "MediaIPCHelper.h"

#include <fcntl.h>
#include <lib/core/CHIPCore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <poll.h>
#include <mutex>
#include <unistd.h>
#include <cmath>
#include <app/clusters/media-playback-server/media-playback-server.h>
#include <app-common/zap-generated/cluster-enums.h>

#define FIFO_BUF_SZ 80
#define ACK_TIMEOUT_MS 3000

using namespace std;
using namespace chip::app::Clusters::MediaPlayback;

int MediaIPCHelper::StartPlayer() {
    int ret = system("systemctl start --user gplay_matter");
    if (ret != 0) {
        ChipLogError(NotSpecified,"gplay_matter start failed: %d", ret);
    }
    return ret;
}

int MediaIPCHelper::StopPlayer() {
    int ret = system("systemctl stop --user gplay_matter");
    if (ret != 0) {
        ChipLogError(NotSpecified,"gplay_matter stop failed: %d", ret);
    }
    return ret;
}

ServiceActiveState MediaIPCHelper::PlayerStatus() {
    int ret = system("systemctl is-active --user gplay_matter");
    if (ret == 0) {
        return ServiceActiveState::Active;
    } else if (ret == 768) {
        return ServiceActiveState::Inactive;
    } else {
        ChipLogError(NotSpecified,"gplay_matter is-active failed: %d", ret);
        return ServiceActiveState::Unknown;
    }
}

MediaIPCHelper* MediaIPCHelper::GetInstance() {
    static MediaIPCHelper instance;
    return &instance;
}

MediaIPCHelper::MediaIPCHelper() {
}

int MediaIPCHelper::Init() {
    mkfifo("/tmp/fifo_nxp_media", 0666);
    mkfifo("/tmp/fifo_nxp_media_ack", 0666);
    return 0;
}

int MediaIPCHelper::Notify(const char* str) {
    std::lock_guard<std::mutex> lock(_mtxNotify);
    int fd = open("/tmp/fifo_nxp_media", O_WRONLY);
    if (fd < 0) {
        ChipLogError(NotSpecified,"failed to open input pipe");
        return -1;
    }
    write(fd, str, strlen(str)+1);
    close(fd);
    sleep(1);
    return GetACK();
}

uint64_t MediaIPCHelper::GetDuration() {

    std::string durationStr = Query("q u");
    uint64_t duration = 0;

    try {
        duration = std::stoull(durationStr); 
    } catch(const std::exception& e) {
        return 0;
    }

    ChipLogError(NotSpecified,"GetDuration parsed: %lu", duration);
    return duration/GPlayTimeDivide;

}

uint64_t MediaIPCHelper::GetPosition() {

    std::string positionStr = Query("q p");
    uint64_t position = 0;

    try {
        position = std::stoull(positionStr); 
    } catch(const std::exception& e) {
        return 0;
    }

    ChipLogError(NotSpecified,"GetPosition parsed: %lu", position);
    return position/GPlayTimeDivide;

}

float MediaIPCHelper::GetPlaybackSpeed() {

    std::string speedStr = Query("q c");
    float speed = 0;

    try {
        speed = std::stof(speedStr); 
    } catch(const std::exception& e) {
        return 0;
    }
    speed = std::round(speed  * 100) / 100;

    ChipLogError(NotSpecified,"GetPlaybackSpeed parsed: %f", speed);
    return speed;

}

PlaybackStateEnum MediaIPCHelper::GetCurrentStatus() {

    std::string sState = Query("q s");
    const char *strState = sState.c_str();
    PlaybackStateEnum state = chip::app::Clusters::MediaPlayback::PlaybackStateEnum::kUnknownEnumValue;
    if(strcmp(strState, "Playing") == 0) {
        state = chip::app::Clusters::MediaPlayback::PlaybackStateEnum::kPlaying;
    } else if(strcmp(strState, "Paused") == 0) {
        state = chip::app::Clusters::MediaPlayback::PlaybackStateEnum::kPaused;
    } else if(strcmp(strState, "Buffering") == 0) {
        state = chip::app::Clusters::MediaPlayback::PlaybackStateEnum::kBuffering;
    } else if(strcmp(strState, "Stopped") == 0) {
        state = chip::app::Clusters::MediaPlayback::PlaybackStateEnum::kNotPlaying;
    } else {
        ChipLogError(NotSpecified,"GetCurrentStatus unexpected value: %s", strState);
    }

    ChipLogError(NotSpecified,"GetCurrentStatus parsed: %s", strState);
    return state;

}
std::string MediaIPCHelper::Query(const char* str) {
    std::lock_guard<std::mutex> lock(_mtxQuery);
    if (Notify(str)) {
        return std::string("");
    }
    sleep(1);

    char buf[FIFO_BUF_SZ] = {0};
    ssize_t len = 0;

    int ret = 0;
    int fd = open("/tmp/fifo_nxp_media_ack", O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        ChipLogError(NotSpecified, "failed to open query pipe");
        return std::string("");
    }
    struct pollfd waiter = {.fd = fd, .events = POLLIN};
    int pret = poll(&waiter, 1, ACK_TIMEOUT_MS);
    if (pret != 1) {
        ChipLogError(NotSpecified, "ACK pipe exception, poll result: %d", ret);
        goto exit;
    }
    if (!(waiter.revents & POLLIN)) {
        ChipLogError(NotSpecified, "polling event incorrect: %d", waiter.revents);
        goto exit;
    }
    len = read(fd, buf, sizeof(buf));
    if (len >= FIFO_BUF_SZ) {
        ChipLogError(NotSpecified, "query size unexcepted: %u", len);
        goto exit;
    }
    if (buf[len-1] != '\0') {
        ChipLogError(NotSpecified, "unexcepted data with non-string data buf=%s buf[%d]=0x%x", buf, len-1, buf[len-1]);
        goto exit;
    }
    ChipLogError(NotSpecified, "Query returned: %s", buf);

    return std::string(buf);;
exit:
    close(fd);
    ChipLogError(NotSpecified, "Query exception restart player");
    StopPlayer();
    StartPlayer();
    return std::string("");

}

int MediaIPCHelper::GetACK() {
    int ret = 0;
    char buf[FIFO_BUF_SZ] = {0};
    ssize_t len = 0;
    int fd = open("/tmp/fifo_nxp_media_ack", O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        ChipLogError(NotSpecified, "failed to open ACK pipe");
        return -1;
    }
    struct pollfd waiter = {.fd = fd, .events = POLLIN};
    int pret = poll(&waiter, 1, ACK_TIMEOUT_MS);
    if (pret != 1) {
        ChipLogError(NotSpecified, "ACK pipe exception, poll result: %d", ret);
        ret = -1;
        goto exit;
    }
    if (!(waiter.revents & POLLIN)) {
        ChipLogError(NotSpecified, "polling event incorrect: %d", waiter.revents);
        ret = -1;
        goto exit;
    }
    len = read(fd, buf, sizeof(buf));
    if (len >= FIFO_BUF_SZ) {
        ChipLogError(NotSpecified, "ack size unexcepted: %u", len);
        ret = -1;
        goto exit;
    }
    if (buf[len] != '\0') {
        ChipLogError(NotSpecified, "unexcepted data with non-string data");
        ret = -1;
        goto exit;
    }
    if (strcmp(buf, "ack") != 0) {
        ChipLogError(NotSpecified, "Unexpected ACK: %s", buf);
        ret = -1;
        goto exit;
    }
exit:
    close(fd);
    return ret;
}

void MediaIPCHelper::Release() {
    unlink("/tmp/fifo_nxp_media");
    unlink("/tmp/fifo_nxp_media_nxp");
}
