#include "gthread\job_manager.h"

namespace gthread
{
    job_manager::~job_manager()
    {
        gthread::latch l(static_cast<int>(m_threads.size() + 1));
        for (std::size_t i = 0; i < m_threads.size(); ++i)
            submit([&] { l.count_down_and_wait(); });
        m_stop_flag = true;
        l.count_down();
    }

    void job_manager::process_job()
    {
        while (!m_stop_flag)
        {
            m_jobs.dequeue()();
        }
        while (auto job = m_jobs.try_dequeue())
        {
            std::move((*job))();
        }
    }
}