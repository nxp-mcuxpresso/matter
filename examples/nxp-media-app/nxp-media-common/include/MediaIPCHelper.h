#ifndef MEDIA_IPC_HELPER_H
#define MEDIA_IPC_HELPER_H

#include <stdlib.h>
#include <cstdint>
#include <string>
#include <mutex>
#include <app/clusters/media-playback-server/media-playback-server.h>

using namespace chip::app::Clusters::MediaPlayback;

enum class ServiceActiveState {
    Active,
    Inactive,
    Unknown
};
class MediaIPCHelper {
public:
    static MediaIPCHelper* GetInstance();
    int Init();
    int Notify(const char* str);
    int GetACK();
    void Release();
    uint64_t GetDuration();
    uint64_t GetPosition();
    PlaybackStateEnum GetCurrentStatus();
    float GetPlaybackSpeed();

    int StartPlayer();
    int StopPlayer();
    ServiceActiveState PlayerStatus();

private:
    MediaIPCHelper();
    std::string Query(const char *str);
    std::mutex _mtxNotify;
    std::mutex _mtxQuery;
    uint64_t GPlayTimeDivide = 1000000; //GPlay use ns instead of ms
};

static MediaIPCHelper* gMediaIPCHelper = MediaIPCHelper::GetInstance();

#endif // MEDIA_IPC_HELPER_H
