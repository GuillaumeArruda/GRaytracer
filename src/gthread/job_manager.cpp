#include "gthread\job_manager.hpp"

namespace gthread
{
    job_manager::~job_manager()
    {
        for (auto& queue : m_job_queues)
            queue.done();
    }

    void job_manager::process_job(unsigned index)
    {
        while (true)
        {
            std::function<void()> job;
            for (unsigned i = 0; i < m_job_queues.size(); ++i)
                if (m_job_queues[(static_cast<std::size_t>(i) + index) % m_job_queues.size()].try_dequeue(job)) break;

            if (!job && !m_job_queues[index].dequeue(job)) break;
            job();
        }
    }
}