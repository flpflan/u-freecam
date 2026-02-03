#include "umod/core.hpp"
#include "umod/feature.hpp"
#include "user/config.hpp"

#include <algorithm>
#include <span>

namespace umod::core
{
    namespace
    {
        static std::span<::umod::feature::Module> modules;
        static void shutdown()
        {
            for (auto &module : modules)
                module.unload();
            //TODO: UnHook PlayerLoop
        }

        static void enableModules()
        {
            for (auto &module : modules)
            {
                // Enable only if exist in user defined EnabledModules
                if (std::ranges::find(user::config::core::EnabledModules, module.name()) !=
                    std::ranges::end(user::config::core::EnabledModules))
                {
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

    void run(std::atomic<bool> &stopToken, void *playerLoop, std::span<::umod::feature::Module> features)
    {
        modules = features;
        enableModules();
        while (!stopToken.load(std::memory_order_relaxed))
        {
        }
        shutdown();
    }

    namespace feature
    {
        std::span<const ::umod::feature::Module> getFeatures() { return modules; }

        void enable(const std::string &name)
        {
            if (auto *module = findModule(name)) module->load();
        }
        void disable(const std::string &name)
        {
            if (auto *module = findModule(name)) module->unload();
        }
    }
}
