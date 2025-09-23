#pragma once

#include <fstream>
#include <optional>
#include <sstream>
#include <unistd.h>
#include <vector>

struct ModuleInfo
{
    uintptr_t base;
    size_t size;
};

static std::optional<ModuleInfo> GetModuleInfo(const char *const moduleName)
{
    std::ifstream maps("/proc/self/maps");
    std::string line;
    uintptr_t min_addr = -1;
    uintptr_t max_addr = 0;
    bool found = false;

    while (std::getline(maps, line))
    {
        if (line.find(moduleName) != std::string::npos)
        {
            found = true;
            std::istringstream iss(line);
            std::string addr_range;
            iss >> addr_range;

            size_t pos = addr_range.find("-");
            if (pos != std::string::npos)
            {
                uintptr_t start = std::stoull(addr_range.substr(0, pos), nullptr, 16);
                uintptr_t end = std::stoull(addr_range.substr(pos + 1), nullptr, 16);

                if (start < min_addr) min_addr = start;
                if (end > max_addr) max_addr = end;
            }
        }
    }

    std::optional<ModuleInfo> info{};
    if (found)
    {
        info = {.base = min_addr, .size = max_addr - min_addr};
    }
    return info;
}

static uintptr_t PatternScan(const char *moduleName, const char *signature)
{
    static auto patternToByte = [](const char *pattern)
    {
        auto bytes = std::vector<int>{};
        const auto start = const_cast<char *>(pattern);
        const auto end = const_cast<char *>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current)
        {
            if (*current == '?')
            {
                ++current;
                if (*current == '?' && current < end) ++current;
                bytes.push_back(-1);
            }
            else
            {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

#ifdef __ANDROID__
    const auto info = GetModuleInfo(moduleName);
    if (!info.has_value()) return 0;
    const auto module = reinterpret_cast<uintptr_t>(info->base);
    const auto moduleSize = info->size;
#else
    // TODO:
#endif
    const auto header = reinterpret_cast<uint8_t *>(module);
    auto patternBytes = patternToByte(signature);
    const auto scanBytes = reinterpret_cast<uint8_t *>(module);

    const auto s = patternBytes.size();
    const auto d = patternBytes.data();

    for (auto i = 0ul; i < moduleSize - s; ++i)
    {
        bool found = true;
        for (auto j = 0ul; j < s; ++j)
        {
            if (scanBytes[i + j] != d[j] && d[j] != -1)
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return module + i;
        }
    }
    return 0;
}
