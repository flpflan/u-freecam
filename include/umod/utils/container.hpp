#pragma once

#include <vector>

namespace umod::utils::container
{
    template <typename T>
    class CowSeq
    {
    public:
        using Seq = std::vector<T>;

        std::shared_ptr<const Seq> snapshot() const
        {
            return std::atomic_load_explicit(&data_, std::memory_order_acquire);
        }

        void push(const T &v)
        {
            modify([&](Seq &s) { s.push_back(v); });
        }

        void remove(const T &v)
        {
            modify([&](Seq &s) { s.erase(std::remove(s.begin(), s.end(), v), s.end()); });
        }

        bool empty() const { return data_->empty(); }

    private:
        template <typename F>
        void modify(F &&f)
        {
            const auto old = std::atomic_load_explicit(&data_, std::memory_order_acquire);

            const auto copy = std::make_shared<Seq>(*old);
            f(*copy);

            std::atomic_store_explicit(&data_, copy, std::memory_order_release);
        }

        std::shared_ptr<Seq> data_ = std::make_shared<Seq>();
    };
}
