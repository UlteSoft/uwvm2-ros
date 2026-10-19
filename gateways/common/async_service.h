/*************************************************************
 * UWVM2 ROS Runtime Extension. APL-2.0.                     *
 *************************************************************/
#pragma once
#include "event_stream.h"
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <memory>
#include <chrono>

namespace uwvm2::ros_gateway
{
    namespace service_ipc=::uwvm2::imported::ros::ipc;
    // This gate is a native-side admission lease. Closing a source prevents all
    // queued work from entering its native call. A call admitted before close
    // may still cause an external ROS side effect; its bounded completion is
    // discarded by the sealed observer. No cancellation claim is made for DDS
    // peers or effects which have already crossed the native transport.
    struct async_service_source
    {
        std::mutex mutex;
        bool active{true};
        std::uint64_t inflight{};
        void retire() noexcept { std::lock_guard<std::mutex> guard{mutex};active=false; }
    };
    // Exactly one owned worker per authenticated session. Every job first owns
    // a stream reservation, including its copied input and task accounting.
    // Thus queue depth/bytes and source max-inflight are finite explicit grants,
    // not an unbounded std::async/detached-thread facility. The adapter's native
    // service call MUST honor the supplied deadline; both production adapters
    // have bounded implementations. Session teardown joins this owned thread.
    class async_service_pool
    {
    public:
        struct task
        {
            std::shared_ptr<async_service_source> source;
            std::shared_ptr<event_stream> output;
            std::uint64_t request{};
            std::chrono::steady_clock::time_point deadline;
            std::vector<std::uint8_t> input;
            std::function<service_ipc::call_result(std::vector<std::uint8_t> const&,std::uint32_t)> invoke;
        };
        inline static constexpr std::size_t accounting_bytes{sizeof(task)+64};
    private:
        std::mutex mutex_;
        std::condition_variable ready_;
        std::deque<task> tasks_;
        std::thread worker_;
        bool stopping_{};
        static void finish_count(std::shared_ptr<async_service_source> const& source) noexcept
        { std::lock_guard<std::mutex> guard{source->mutex};if(source->inflight) { --source->inflight; } }
        void run() noexcept
        {
            for(;;)
            {
                task work;
                { std::unique_lock<std::mutex> guard{mutex_};ready_.wait(guard,[&]{return stopping_||!tasks_.empty();});if(stopping_&&tasks_.empty()) { return; }work=std::move(tasks_.front());tasks_.pop_front(); }
                bool admitted{};
                { std::lock_guard<std::mutex> guard{work.source->mutex};admitted=work.source->active&&work.output->active(); }
                if(!admitted) { work.output->cancel_reservation(work.request);finish_count(work.source);continue; }
                service_ipc::call_result response;bool attempted{};
                try
                {
                    auto now{std::chrono::steady_clock::now()};
                    auto budget{std::chrono::duration_cast<std::chrono::milliseconds>(work.deadline-now).count()};
                    if(budget<=0) {response={110,{}};}else {attempted=true;response=work.invoke(work.input,static_cast<std::uint32_t>(budget));}
                    if(std::chrono::steady_clock::now()>=work.deadline) { response={110,{}}; }
                }
                catch(...) { response={5,{}}; }
                if(response.error) { (void)work.output->complete(work.request,{service_ipc::event_kind::resource_error,static_cast<std::uint8_t>(attempted?service_ipc::event_external_effects_may_remain:0),response.error,work.request,{}}); }
                else { (void)work.output->complete(work.request,{service_ipc::event_kind::service_response,0,0,work.request,std::move(response.payload)}); }
                finish_count(work.source);
            }
        }
    public:
        async_service_pool()=default;
        async_service_pool(async_service_pool const&)=delete;async_service_pool& operator=(async_service_pool const&)=delete;
        ~async_service_pool() { stop(); }
        std::uint32_t submit(task work,std::uint64_t maximum)
        {
            { std::lock_guard<std::mutex> source_guard{work.source->mutex};if(!work.source->active) { return 125; }if(work.source->inflight>=maximum) { return 11; }++work.source->inflight; }
            auto source{work.source};
            try
            {
                std::lock_guard<std::mutex> guard{mutex_};
                if(stopping_) { finish_count(source);return 125; }
                // Start before moving the job so thread-creation failure retains
                // all source accounting and the caller can cancel its credit.
                if(!worker_.joinable()) { worker_=std::thread{[this]{run();}}; }
                tasks_.push_back(std::move(work));ready_.notify_one();return 0;
            }
            catch(...) { finish_count(source);return 12; }
        }
        void stop() noexcept
        {
            { std::lock_guard<std::mutex> guard{mutex_};stopping_=true;for(auto& task:tasks_) { task.output->cancel_reservation(task.request);finish_count(task.source); }tasks_.clear(); }
            ready_.notify_one();if(worker_.joinable()) { worker_.join(); }
        }
    };
}
