#pragma once

#include <functional>
#include <atomic>
#include <future>
#include <type_traits>

#include "gthread\concurrent_bounded_queue.h"
#include "gthread\thread_group.h"
#include "gthread\latch.h"

namespace gthread
{
    struct job_manager
    {
        job_manager(unsigned int number_of_thread) : m_threads(number_of_thread, [&] { process_job(); }) {}
        ~job_manager();

        job_manager(job_manager const&) = delete;
        job_manager(job_manager&&) = delete;
        job_manager& operator=(job_manager const&) = delete;
        job_manager& operator=(job_manager&&) = delete;
         
        template<class Invocable>
        void submit(Invocable&& invocable)
        {
            m_jobs.enqueue(std::forward<Invocable>(invocable));
        }

        template<class Invocable>
        [[nodiscard]] auto submit_with_future(Invocable&& invocable) -> std::future<std::invoke_result_t<Invocable>>
        {
            using ReturnType = std::invoke_result_t<Invocable>;
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
        void process_job();
        std::atomic<bool> m_stop_flag = false;
        concurrent_bonded_queue<std::function<void()>, 64> m_jobs;
        thread_group m_threads;
    };
}