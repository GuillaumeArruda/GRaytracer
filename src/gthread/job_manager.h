#pragma once

#include <functional>
#include <atomic>
#include <future>
#include <type_traits>

#include "gthread/concurrent_queue.h"
#include "gthread/thread_group.h"
#include "gthread/latch.h"

namespace gthread
{
    struct job_manager
    {
        job_manager(unsigned int number_of_thread) : m_job_queues(number_of_thread), m_threads(number_of_thread, [&](unsigned index) { process_job(index); }) {}
        ~job_manager();

        job_manager(job_manager const&) = delete;
        job_manager(job_manager&&) = delete;
        job_manager& operator=(job_manager const&) = delete;
        job_manager& operator=(job_manager&&) = delete;
         
        template<class Invocable>
        void submit(Invocable&& invocable)
        {
            const std::size_t number_of_try_enqueue = 20 * m_job_queues.size();
            const unsigned i = m_index++;
            for (std::size_t n = 0; n < number_of_try_enqueue; ++n)
                if (m_job_queues[(i + n) % m_job_queues.size()].try_enqueue(std::forward<Invocable>(invocable))) return;

            m_job_queues[i % m_job_queues.size()].enqueue(std::forward<Invocable>(invocable));
        }

        template<class Invocable>
        [[nodiscard]] auto submit_with_future(Invocable&& invocable) -> std::future<std::decay_t<std::invoke_result_t<Invocable>>>
        {
            using ReturnType = std::decay_t<std::invoke_result_t<Invocable>>;
            std::shared_ptr<std::promise<ReturnType>> promise = std::make_shared<std::promise<ReturnType>>();
            std::future<ReturnType> returnValue = promise->get_future();
            if constexpr (std::is_same_v<std::invoke_result_t<Invocable>, void>)
            {
                submit([i = std::forward<Invocable>(invocable), p = std::move(promise)]() mutable { i(); p->set_value(); });
            }
            else
            {
                submit([i = std::forward<Invocable>(invocable), p = std::move(promise)]() mutable { p->set_value(i()); });
            }
            return returnValue;
        }

    private:
        void process_job(unsigned i);
        std::atomic<bool> m_stop_flag = false;
        unsigned m_index = 0;
        std::vector<concurrent_queue<std::function<void()>>> m_job_queues;
        thread_group m_threads;
    };
}