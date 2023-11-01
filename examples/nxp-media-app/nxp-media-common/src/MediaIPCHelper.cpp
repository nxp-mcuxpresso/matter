#include "MediaIPCHelper.h"

#include <fcntl.h>
#include <lib/core/CHIPCore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cmath>
#include <app/clusters/media-playback-server/media-playback-server.h>
#include <app-common/zap-generated/cluster-enums.h>

using namespace std;
using namespace chip::app::Clusters::MediaPlayback;

int MediaIPCHelper::StartPlayer() {
    int ret = system("systemctl start gplay_matter");
    if (ret != 0) {
        ChipLogError(NotSpecified,"gplay_matter start failed: %d", ret);
    }
    return ret;
}

int MediaIPCHelper::StopPlayer() {
    int ret = system("systemctl stop gplay_matter");
    if (ret != 0) {
        ChipLogError(NotSpecified,"gplay_matter stop failed: %d", ret);
    }
    return ret;
}

ServiceActiveState MediaIPCHelper::PlayerStatus() {
    int ret = system("systemctl is-active gplay_matter");
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
    int fd = open("/tmp/fifo_nxp_media", O_WRONLY);
    write(fd, str, strlen(str)+1);
    close(fd);
    GetACK();
    return 0;
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

    std::string positionStr= Query("q p");
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
    if (Notify(str)) {
        return std::string("");
    }

    int fd = open("/tmp/fifo_nxp_media_ack", O_RDONLY);
    char buf[80];
    read(fd, buf, sizeof(buf));
    close(fd);
    ChipLogError(NotSpecified, "Query returned: %s", buf);

    std::string resp(buf);
    return resp;

}

int MediaIPCHelper::GetACK() {
    int fd = open("/tmp/fifo_nxp_media_ack", O_RDONLY);
    char buf[80];
    read(fd, buf, sizeof(buf));
    if (strcmp(buf, "ack") != 0) {
        ChipLogError(NotSpecified, "Unexpected ACK: %s", buf);
    }
    close(fd);
    return 0;
}

void MediaIPCHelper::Release() {
    unlink("/tmp/fifo_nxp_media");
    unlink("/tmp/fifo_nxp_media_nxp");
}
