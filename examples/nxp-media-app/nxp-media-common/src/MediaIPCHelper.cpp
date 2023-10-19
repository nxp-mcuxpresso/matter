#include "MediaIPCHelper.h"

#include <fcntl.h>
#include <lib/core/CHIPCore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

MediaIPCHelper* MediaIPCHelper::GetInstance() {
	static MediaIPCHelper instance;
	return &instance;
}

MediaIPCHelper::MediaIPCHelper() {
}

int MediaIPCHelper::Init() {
	mkfifo("/tmp/fifo_nxp_media", 0666);
	return 0;
}

int MediaIPCHelper::Notify(char* str) {
	int fd = open("/tmp/fifo_nxp_media", O_WRONLY);
	write(fd, str, strlen(str)+1);
	close(fd);
	return 0;
}

int MediaIPCHelper::GetACK() {
	int fd = open("/tmp/fifo_nxp_media_ack", O_RDONLY);
	char buf[80];
	read(fd, buf, sizeof(buf));
	close(fd);
	return 0;
}

void MediaIPCHelper::Release() {
	unlink("/tmp/fifo_nxp_media");
}
