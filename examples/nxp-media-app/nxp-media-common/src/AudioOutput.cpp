#include "AudioOutput.h"

#include <fstream>
#include <iostream>
#include <json/json.h>
#include <lib/core/CHIPCore.h>
#include <string>

int AudioOutput::Init() {
    int ret = system("systemctl --user status pipewire wireplumber pipewire-pulse > /dev/null");
    if (ret != 0) {
        int retry = 3;
        while (retry--) {
            ChipLogError(NotSpecified, "pipewire, wireplumber and pipewire-pulse not started. Enabling...");
            ret = system("systemctl --user start pipewire wireplumber pipewire-pulse && sleep 2");
            if (ret != 0) {
                ChipLogError(NotSpecified, "Failed to enable pipewire ,wireplumber and pipewire-pulse.");
            }
            ret = system("systemctl --user status pipewire wireplumber pipewire-pulse > /dev/null");
            if (ret != 0) {
                ChipLogError(NotSpecified, "manual enabling pipewire failed...");
            } else {
                break;
            }
        }
        if (ret != 0) {
            ChipLogError(NotSpecified, "With multiple retry, failed to kick pipewire, wireplumber and pipewire-pulse, return");
            return ret;
        }
    }
    std::string cmd = "pw-dump > /tmp/pw_dump.json";
    ret = system(cmd.c_str());
    if (ret != 0) {
        ChipLogError(NotSpecified, "Failed to get pw dump.");
        return ret;
    }

    //start to parse json
    std::ifstream input("/tmp/pw_dump.json");
    Json::Value root;
    Json::Reader reader;
    bool parsedSuccess = reader.parse(input, root);

    if (!parsedSuccess) {
        ChipLogError(NotSpecified, "Failed to parse JSON");
        return 1;
    }

    for (unsigned int i = 0; i < root.size(); i++) {
        Json::Value item = root[i];
        int id = item["id"].asInt();
        if (item["info"].isMember("props")) {
            if (item["info"]["props"].isMember("factory.name")) {
                if (item["info"]["props"]["factory.name"].asString().find("sink") != std::string::npos) {
                    std::string cardName = item["info"]["props"]["alsa.long_card_name"].asString();
                    sink_list.push_back(cardName);
                    sinks_[cardName] = id;
                }
            }
        }
    }

    return 0;
}

int AudioOutput::Select(uint32_t number) {
    if(number >= sinks_.size()) {
        ChipLogError(NotSpecified, "Invalid sink number");
        return -1;
    }

    std::string cmd = "wpctl set-default " + std::to_string(sinks_[sink_list[number]]);

    return system(cmd.c_str());

}
