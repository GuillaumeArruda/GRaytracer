#include "gthread/thread_group.hpp"

namespace gthread
{
    thread_group::~thread_group()
    {
        for (std::thread& thread : m_threads)
        {
            thread.join();
        }
    }
}

