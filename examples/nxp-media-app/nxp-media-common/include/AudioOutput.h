#pragma once
#include <map>
#include <string>
#include <vector>
#include <cstdint>

class AudioOutput {
    public:
        int Init();
        int Select(uint32_t number);

        const std::vector<std::string>& GetSinks() {
            return sink_list;
        }

    private:
        std::vector<std::string> sink_list;
        std::map<std::string, int> sinks_;
};
