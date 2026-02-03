#pragma once

#include "umod/debug/logger.hpp"

#ifdef __ANDROID__
#include "KittyMemory/KittyInclude.hpp"
#include "KittyMemory/KittyScanner.hpp"

namespace umod::memory
{
    inline auto patternScan(const char *const moduleName, const char *const signature) -> uintptr_t
    {
        debug::logger::debug("Pattern scan {} with pattern {}", moduleName, signature);
        const auto unityELF = ElfScanner::findElf(moduleName);
        for (const auto segment : unityELF.segments())
        {
            if (!segment.readable) continue;
            debug::logger::debug("Using segment {} - {}", segment.startAddress, segment.endAddress);
            if (const auto found =
                    KittyScanner::findIdaPatternFirst(segment.startAddress, segment.endAddress, signature))
            {
                debug::logger::debug("pattern found at: {}", found);
                return found;
            }
        }
        return 0;
    }
}
#else
namespace umod::memory
{
    // Reference: https://github.com/Taiga74164/BA-Cheeto
    inline auto patternScan(const char *const moduleName, const char *const signature) -> uintptr_t
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
        const auto module = reinterpret_cast<uintptr_t>(GetModuleHandleA(moduleName));
        const auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
        const auto ntHeaders =
            reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t *>(module) + dosHeader->e_lfanew);

        const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
        auto patternBytes = patternToByte(signature);
        const auto scanBytes = reinterpret_cast<uint8_t *>(module);

        const auto s = patternBytes.size();
        const auto d = patternBytes.data();

        for (auto i = 0ul; i < sizeOfImage - s; ++i)
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
}
#endif
