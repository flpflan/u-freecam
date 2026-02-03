#pragma once

namespace umod::utils::event
{
    template <typename... Args>
    class Event
    {
    public:
        void emit();
    };
}
