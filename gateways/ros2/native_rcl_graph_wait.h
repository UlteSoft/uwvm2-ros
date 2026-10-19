/* SPDX-License-Identifier: Apache-2.0 */
/** Real original-SDK graph-event wait storage and borrowed cancellation facts.
 *
 * No Guest wire contains these fields. The authenticated native session owner
 * lends its connected socket, pinned worker pidfd and stable stop flag for ONE
 * synchronous call. No descriptor is adopted, closed or read here. This is not
 * a capability constructor. All original source/peer/query leases are required.
 *
 * One domain is allocated per explicitly selected actual Node/profile. Its one
 * event loan is registered lazily after an authorized positive query actually
 * needs to wait. Keeping that loan until the Node/profile dies avoids an
 * unbounded series of expired NodeGraph weak loans during quiet reconnects.
 * An initial registration/start exception permanently poisons this domain;
 * partial original GraphListener infrastructure stays owned by the actual
 * Node/Context until their real destruction. We invent no successful cleanup.
 */
#pragma once
#include "compat/node.h"
#if __has_include("native_rcl_service_features.h")
#include "native_rcl_service_features.h"
#endif
#ifndef UWVM_NATIVE_RCL_FINITE_GRAPH_WAIT_API
#define UWVM_NATIVE_RCL_FINITE_GRAPH_WAIT_API 0
#endif
#include <rclcpp/node.hpp>
#if UWVM_NATIVE_RCL_FINITE_GRAPH_WAIT_API
#include <rclcpp/node_interfaces/node_base_interface.hpp>
#include <rclcpp/utilities.hpp>
#include <rclcpp/event.hpp>
#endif
#include <chrono>
#include <algorithm>
#include <new>
#include <utility>
#include <memory>
#include <atomic>
#include <csignal>
#include <cstdint>
#include <cerrno>
#if defined(__linux__)
#include <poll.h>
#endif

namespace uwvm2::ros_gateway::native_rcl_graph_wait
{
    static_assert(std::atomic<std::sig_atomic_t>::is_always_lock_free,
        "Native signal and cross-thread cancellation require actual always-lock-free atomic sig_atomic_t");
    using clock=std::chrono::steady_clock;
    using deadline=clock::time_point;
    struct lifetime
    {
        int session_fd{-1},worker_pidfd{-1};
        std::atomic<std::sig_atomic_t> const* stopping{};
        bool valid()const noexcept{return session_fd>=0&&worker_pidfd>=0&&stopping;}
    };
    /** Real time, never poll counts, bounds every graph-CV slice. Native RMW
     * predicate calls and foreign mutex acquisition still need the independent
     * Root process deadline; a post-return check cannot preempt middleware. */
    inline std::chrono::nanoseconds slice(deadline now,deadline requested,deadline transport)noexcept
    {
        if(now>=requested||now>=transport)return std::chrono::nanoseconds{0};
        auto a=std::chrono::duration_cast<std::chrono::nanoseconds>(requested-now);
        auto b=std::chrono::duration_cast<std::chrono::nanoseconds>(transport-now);
        auto cap=std::chrono::nanoseconds{100000000};return std::min(a,std::min(b,cap));
    }
    /** HUP/ERR/dead worker and shutdown are errors, not an unavailable server.
     * POLLIN on the socket is deliberately ignored: this helper consumes no
     * IPC data and does not reinterpret a queued request as cancellation. */
    inline std::uint32_t transport_check(lifetime const& life,deadline until)noexcept
    {
        if(!life.valid())return 95;if(life.stopping->load(std::memory_order_relaxed))return 125;
        if(clock::now()>=until)return 110;
#if defined(__linux__) && defined(POLLRDHUP)
        pollfd watched[2]{{life.session_fd,POLLRDHUP,0},{life.worker_pidfd,POLLIN,0}};
        int result=::poll(watched,2,0);
        if(result<0)return life.stopping->load(std::memory_order_relaxed)?125u:(errno==EINTR?4u:107u);
        if(watched[0].revents&(POLLHUP|POLLRDHUP|POLLERR|POLLNVAL))return 107;
        if(watched[1].revents)return 107;
        if(life.stopping->load(std::memory_order_relaxed))return 125;return clock::now()>=until?110u:0u;
#else
        return 95;
#endif
    }
    inline std::uint32_t check(lifetime const& life,::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,deadline until)noexcept
    {
#if UWVM_NATIVE_RCL_FINITE_GRAPH_WAIT_API
        auto error=transport_check(life,until);if(error)return error;if(!node)return 95;
        try{if(!rclcpp::ok(node->get_node_base_interface()->get_context()))return 125;}
        catch(...){return 5;}
        if(life.stopping->load(std::memory_order_relaxed))return 125;return clock::now()>=until?110u:0u;
#else
        // No optional NodeBase/Context/utilities dependency on old SDKs.
        (void)life;(void)node;(void)until;return 95;
#endif
    }
    struct observation
    {
        std::uint64_t registration_attempts{},registration_completed{},wait_attempts{},wait_completed{};
        std::uint32_t first_error{};bool poisoned{};
    };
    class domain
    {
        ::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node_;
        // One writer initializes event_ before publishing ready with release.
        // The immutable shared_ptr is only READ after an acquire-ready load.
        // No Native call or graph wait holds a shared domain mutex. Contending
        // first preparations return17 instead of waiting on a hidden mutex.
        enum phase:unsigned {virgin,preparing,ready,poisoned};
        std::atomic<unsigned> phase_{virgin};
        std::atomic<std::uint32_t> first_error_{};
        std::atomic<std::uint64_t> registrations_{},registered_{},waits_{},waited_{};
#if UWVM_NATIVE_RCL_FINITE_GRAPH_WAIT_API
        rclcpp::Event::SharedPtr event_;
#endif
        static void bump(std::atomic<std::uint64_t>& n)noexcept
        {auto old=n.load(std::memory_order_relaxed);while(old!=UINT64_MAX&&!n.compare_exchange_weak(old,old+1,std::memory_order_relaxed)){};}
        void poison(std::uint32_t error)noexcept
        {std::uint32_t zero{};(void)first_error_.compare_exchange_strong(zero,error?error:5);phase_.store(poisoned,std::memory_order_release);}
    public:
        explicit domain(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node):node_{std::move(node)}{}
        domain(domain const&)=delete;domain& operator=(domain const&)=delete;
        bool same_node(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node)const noexcept
        {return node&&node_.get()==node.get();}
        /** Monotonic concurrent counters; obtain a complete terminal snapshot
         * only after all actual calls have quiesced. They are not fake results. */
        observation observe()const noexcept
        {auto state=phase_.load(std::memory_order_acquire);return {registrations_.load(),registered_.load(),waits_.load(),waited_.load(),first_error_.load(),state==poisoned};}
        std::uint32_t status()const noexcept
        {auto state=phase_.load(std::memory_order_acquire);return state==poisoned?first_error_.load():state==preparing?17u:0u;}
#if UWVM_NATIVE_RCL_FINITE_GRAPH_WAIT_API
        std::uint32_t loan(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,rclcpp::Event::SharedPtr& output)noexcept
        {
            if(!same_node(node))return 13;auto state=phase_.load(std::memory_order_acquire);
            if(state==poisoned)return first_error_.load();if(state==preparing)return 17;
            if(state==virgin)
            {
                unsigned expected=virgin;
                if(!phase_.compare_exchange_strong(expected,preparing,std::memory_order_acq_rel))
                    return expected==poisoned?first_error_.load():17u;
                // State/counters exist BEFORE entering the original SDK. Even
                // a throw after its weak-loan insertion poisons this same domain.
                bump(registrations_);
                try{auto next=node_->get_graph_event();if(!next){poison(5);return 5;}
                    event_=std::move(next);bump(registered_);phase_.store(ready,std::memory_order_release);}
                catch(std::bad_alloc const&){poison(12);return 12;}
                catch(...){poison(5);return 5;}
            }
            // No writer resets/replaces this event before the whole domain dies.
            output=event_;return 0;
        }
        std::uint32_t wait(rclcpp::Event::SharedPtr const& event,std::chrono::nanoseconds maximum)noexcept
        {
            if(maximum.count()<0||maximum>std::chrono::nanoseconds{100000000})return 22;
            auto state=phase_.load(std::memory_order_acquire);
            if(state==poisoned)return first_error_.load();if(state!=ready)return 17;
            if(!event||event!=event_)return 13;bump(waits_);
            try
            {
                // This is the REAL NodeGraph condition-variable timed wait.
                // Another endpoint can clear the shared event: a missed wake
                // only delays the next actual predicate by this <=100ms slice.
                node_->wait_for_graph_change(event,maximum);event->check_and_clear();
                bump(waited_);return status();
            }
            catch(std::bad_alloc const&){poison(12);return 12;}
            catch(...){poison(5);return 5;}
        }
#endif
    };
    struct admission
    {
        domain* graph{};lifetime cancel{};
        // Borrowed ONLY while the actual profile/shared Node and authenticated
        // session remain retained by the synchronous caller. Not serialized.
    };
}
