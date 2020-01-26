#include "gthread\latch.hpp"

namespace gthread
{
    void latch::count_down(int update)
    {
        if (m_count -= update) m_cond_var.notify_all();
    }

    void latch::count_down_and_wait()
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

    void latch::wait()
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
}