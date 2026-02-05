#include "umod/core.hpp"
#include "umod/debug/logger.hpp"
#include "umod/feature.hpp"

#include "user/config.hpp"

#include <algorithm>
#include <span>

namespace umod::core
{
    namespace
    {
        static std::span<::umod::feature::Module> modules;

        static void enableModules()
        {
            for (auto &module : modules)
            {
                // Enable only if exist in user defined EnabledModules
                if (std::ranges::find(user_config::core::EnabledModules, module.name()) !=
                    std::ranges::end(user_config::core::EnabledModules))
                {
                    debug::logger::info("Feature enabled: {}", module.name());
                    module.load();
                }
            }
        }

        static ::umod::feature::Module *findModule(std::string_view name)
        {
            for (auto &module : modules)
            {
                if (module.name() == name) return &module;
            }
            return nullptr;
        }
    }

    void run(void *playerLoop, std::span<::umod::feature::Module> features)
    {
        player_loop::init(playerLoop);
        modules = features;
        enableModules();
    }

    void shutdown()
    {
        for (auto &module : modules)
            module.unload();
        //TODO: UnHook PlayerLoop

        debug::logger::shutdown();
    }

    namespace feature
    {
        std::span<const ::umod::feature::Module> getFeatures() { return modules; }

        void enable(const std::string &name)
        {
            debug::logger::info("Feature enabled: {}", name);
            if (auto *module = findModule(name)) module->load();
        }
        void disable(const std::string &name)
        {
            debug::logger::info("Feature disabled: {}", name);
            if (auto *module = findModule(name)) module->unload();
        }
    }
}
