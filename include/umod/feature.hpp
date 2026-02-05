#pragma once

namespace umod::feature
{
    struct Module
    {
        struct Desc
        {
            friend struct Module;

            const char *name;
            void (*load)();
            void (*unload)();
        };

        bool loaded() const { return loaded_; }
        auto name() const { return desc_.name; }
        void load()
        {
            if (loaded_) return;
            desc_.load();
            loaded_ = true;
        }
        void unload()
        {
            if (!loaded_) return;
            desc_.unload();
            loaded_ = false;
        }

        Module(Desc desc) : desc_(desc) {}

    private:
        Desc desc_;
        bool loaded_{};
    };
}
