#include "AudioOutput.h"
#include <iostream>

int AudioOutput::Init() {
    int ret = system("pulseaudio --check");
    if (ret != 0) {
        ret = system("pulseaudio --start");
        if (ret != 0) {
            return -1;
        }
    }
    std::string cmd = "pacmd list-sinks | grep -oP 'name: \\K\\S+' | sed 's/<//g' | sed 's/>//g'";

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return -1;
    }

    char buffer[128];
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL) {
            sinks_.push_back(buffer);
        }
    }
    pclose(pipe);

    return 0;
}

int AudioOutput::Select(uint32_t number) {
    if(number >= sinks_.size()) {
        std::cout << "Invalid sink number" << std::endl;
        return -1;
    }

    std::string cmd = "pacmd set-default-sink " + sinks_[number];

    return system(cmd.c_str());

}
