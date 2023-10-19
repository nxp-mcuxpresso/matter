#ifndef MEDIA_IPC_HELPER_H
#define MEDIA_IPC_HELPER_H

class MediaIPCHelper {
public:
  static MediaIPCHelper* GetInstance();

  int Init();

  int Notify(char* str);

  int GetACK();

  void Release();

private:
  MediaIPCHelper();
};

static MediaIPCHelper* gMediaIPCHelper = MediaIPCHelper::GetInstance();

#endif // MEDIA_IPC_HELPER_H
