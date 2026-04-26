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

        enum class Type
        {
            None,
            Internal,
            Mock
        };

        const Type currentState();

        // Handle attach(Index, void (*)());
        [[nodiscard]] Handle attach(void (*)());

        void reload(Type ty);
    }

    namespace feature
    {
        std::span<const ::umod::feature::Module> getFeatures();
        void enable(const std::string &name);
        void disable(const std::string &name);
    }

    void init(void *playerLoop, std::span<::umod::feature::Module> modules);
    void shutdown();
}
