/** @file
 * @brief Deployment-owned device supervision independent of guest hostcalls.
 * @details Each prepared environment reserves one finite guarded fast_io stack
 * and an explicit poll interval. The thread inherits the VM's CPU affinity,
 * cgroup and NUMA policy; it never broadens placement or requests real-time
 * scheduling. Native receiver execution and its independent lease watchdog
 * remain separate deployment obligations. Polling is a scheduling bound, not
 * a worst-case hardware response guarantee.
 *
 * Stop requests retire normal work immediately. The supervisor continues the
 * reserved stop lanes until acknowledgement, unknown outcome or the explicit
 * shutdown bound, then joins before dispatcher destruction. No detached thread
 * or implicit unlimited budget exists. A stop ACK is not a stopped witness.
 */
#pragma once
#include "device_dispatch.h"
#include "supervisor_stack.h"

namespace uwvm2::imported::ros::scene
{
    class device_supervisor
    {
    public:
        struct plan {std::uint64_t stack_bytes{},poll_ns{},shutdown_ns{};};
    private:
        device_dispatch& dispatcher_;plan policy_;
        std::atomic<bool> stopped_{};std::mutex join_;
#if defined(__linux__)
        model_worker::supervisor_stack stack_;pthread_t thread_{};
        pid_t process_{::getpid()};bool started_{},joined_{};
        static void* run(void* opaque)noexcept
        {
            auto& self=*static_cast<device_supervisor*>(opaque);
            std::uint64_t deadline{};
            for(;;)
            {
                auto now=static_cast<std::uint64_t>(resources::steady_now());
                if(self.stopped_.load(std::memory_order_acquire)&&!deadline)
                {
                    self.dispatcher_.retire();
                    deadline=now>UINT64_MAX-self.policy_.shutdown_ns?UINT64_MAX:now+self.policy_.shutdown_ns;
                }
                self.dispatcher_.pump_all();
                if(deadline&&(self.dispatcher_.drained()||now>=deadline))break;
                auto pause=self.policy_.poll_ns;
                if(deadline)pause=std::min(pause,deadline-now);
                timespec duration{static_cast<time_t>(pause/1000000000),static_cast<long>(pause%1000000000)};
                // An interrupted sleep simply starts the next bounded sweep.
                (void)::nanosleep(&duration,nullptr);
            }
            return nullptr;
        }
#endif
    public:
        device_supervisor(device_dispatch& dispatcher,plan policy):dispatcher_(dispatcher),policy_(policy){}
        device_supervisor(device_supervisor const&)=delete;device_supervisor& operator=(device_supervisor const&)=delete;
        ~device_supervisor(){retire();}
        static bool valid(plan p,std::uint64_t stop_ns)noexcept
        {
            return p.stack_bytes>=65536&&p.stack_bytes<=64u*1024u*1024u&&p.stack_bytes%4096==0&&
                p.poll_ns&&p.poll_ns<=100000000&&p.shutdown_ns<=UINT64_C(5000000000)&&
                stop_ns&&stop_ns<=p.shutdown_ns&&p.poll_ns<=(p.shutdown_ns-stop_ns)/2;
        }
        /** @brief Start after complete static validation and native endpoint preparation. */
        error start()noexcept
        {
#if defined(__linux__)
            if(process_!=::getpid())return error::denied;
            std::lock_guard lock{join_};if(started_||stopped_.load())return error::conflict;
            if(!valid(policy_,dispatcher_.stop_bound()))return error::invalid;
            auto status=stack_.prepare(policy_.stack_bytes);if(status!=error::success)return status;
            pthread_attr_t attr;if(::pthread_attr_init(&attr))return error::io;
            auto result=::pthread_attr_setstack(&attr,stack_.data(),stack_.size());
            if(!result)result=::pthread_create(&thread_,&attr,run,this);
            ::pthread_attr_destroy(&attr);if(result)return error::io;
            started_=true;return error::success;
#else
            return error::unsupported;
#endif
        }
        /** @brief Nonblocking cancellation safe under an authority retirement callback. */
        void request_stop()noexcept
        {
#if defined(__linux__)
            if(process_!=::getpid())return;
#endif
            stopped_.store(true,std::memory_order_release);
        }
        /** @brief Join outside authority locks; never destroy live provider state. */
        void retire()noexcept
        {
#if defined(__linux__)
            if(process_!=::getpid())return;
            request_stop();std::lock_guard lock{join_};
            if(started_&&!joined_){if(!::pthread_join(thread_,nullptr))joined_=true;}
#endif
        }
    };
}
