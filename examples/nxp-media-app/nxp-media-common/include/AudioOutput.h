#pragma once
#include <string>
#include <vector>

class AudioOutput {
    public:
        int Init();
        int Select(uint32_t number);

        const std::vector<std::string>& GetSinks() {
            return sinks_;
        }

    private:
        std::vector<std::string> sinks_;
};
