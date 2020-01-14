#pragma once

#include <thread>
#include <vector>

namespace gthread
{
    struct thread_group
    {
        template<class T, class ... Args>
        thread_group(unsigned int number_of_thread, T&& callable, Args&& ... args)
        {
            for (unsigned int i = 0; i < number_of_thread; ++i)
            {
                m_threads.emplace_back(std::forward<T>(callable), std::forward<Args>(args)...);
            }
        }
        thread_group(thread_group const&) = delete;
        thread_group(thread_group&&) noexcept = default;

        thread_group& operator=(thread_group const&) = delete;
        thread_group& operator=(thread_group&&) noexcept = default;

        ~thread_group()
        {
            for (std::thread& thread : m_threads)
            {
                thread.join();
            }
        }

        std::size_t size() const noexcept { return m_threads.size(); }
    private:
        std::vector<std::thread> m_threads;
    };
}