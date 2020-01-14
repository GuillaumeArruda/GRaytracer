#pragma once

#include <mutex>
#include <atomic>

namespace gthread 
{
    // Replacement for c++20 latch
    struct latch
    {
        explicit latch(int expected) : m_count(expected), m_waiting(0) {}
        ~latch() { while (m_waiting > 0); };

        latch(latch const&) = delete;
        latch(latch&&) = delete;
        latch& operator=(latch const&) = delete;
        latch& operator=(latch&&) = delete;

        void count_down(int update = 1)
        {
            if (m_count -= update) m_cond_var.notify_all();
        }

        void count_down_and_wait()
        {
            ++m_waiting;
            {
                std::unique_lock lock(m_lock);
                if (--m_count == 0)
                    m_cond_var.notify_all();
                else
                    while (m_count > 0)
                        m_cond_var.wait(lock);
            }
            --m_waiting;
        }

        void wait()
        {
            ++m_waiting;
            {
                std::unique_lock lock(m_lock);
                while (m_count > 0) {
                    m_cond_var.wait(lock);
                }
            }
            --m_waiting;
        }

        bool is_ready() const noexcept { return m_count == 0; }

    private:
        std::mutex m_lock;
        std::condition_variable m_cond_var;
        std::atomic_int m_count;
        std::atomic_int m_waiting;
    };
}