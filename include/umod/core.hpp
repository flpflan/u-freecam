#pragma once

#include "umod/feature.hpp"

#include <span>
#include <string>

namespace umod::core
{
    namespace player_loop
    {
        void init(void *);
        enum class Index
        {
            Update = 0,
            LateUpdate = 1,
            FixedUpdate = 2
        };
        struct Handle
        {
            using ID = void (*)();
            ID id;
            Index index;
            void dettach();
        };

        const bool isMockLoop();

        // Handle attach(Index, void (*)());
        Handle attach(void (*)());
    }

    namespace feature
    {
        std::span<const ::umod::feature::Module> getFeatures();
        void enable(const std::string &name);
        void disable(const std::string &name);
    }

    void run(void *playerLoop, std::span<::umod::feature::Module> modules);
    void shutdown();
}
