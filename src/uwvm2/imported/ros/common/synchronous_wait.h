/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/utils/thread/monotonic_clock.h>
#endif
#include "quota.h"
#ifndef UWVM_MODULE
#include <uwvm2/utils/thread/process_exit.h>
#endif
#include <memory>
#include <new>
#include <chrono>
#include <cerrno>
#if defined(__linux__)
#include <poll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <unistd.h>
#endif
namespace uwvm2::imported::ros
{
    inline ::std::string_view quota_setting(capability_spec const& cap, ::std::string_view key, ::std::string_view suffix) noexcept
    {
        for(auto const& item:cap.limits)
        {
            auto const name{::std::string_view{item.first}};
            if(name.size()==key.size()+suffix.size() && name.substr(0,key.size())==key && name.substr(key.size())==suffix)
            { return item.second; }
        }
        return {};
    }
    inline bool quota_wait_enabled(capability_spec const& cap, ::std::string_view key) noexcept
    { return quota_setting(cap,key,".on-exhausted")=="wait"; }
    inline bool quota_wait_on_exit(capability_spec const& cap, ::std::string_view key) noexcept
    { return quota_setting(cap,key,".on-proc-exit")=="wait"; }

    inline bool native_wait_on_exit(capability_spec const& cap) noexcept
    {
        auto const explicit_action{quota_setting(cap,"native-wait",".on-proc-exit")};
        return explicit_action.empty()?quota_wait_on_exit(cap,"timeout-ms"):explicit_action=="wait";
    }
    class synchronous_quota_wait;
    class synchronous_wait_context
    {
        inline static thread_local synchronous_wait_context* current{};
        synchronous_wait_context* previous{current};
        bool (*check)(void*) noexcept{};
        void* context{};
        ::std::unique_ptr<synchronous_quota_wait> owned_waiter;
        bool native_wait_on_exit{};
        int external_cancellation_fd{-1};
    public:
        synchronous_wait_context(bool (*callback)(void*) noexcept,void* object,int cancellation_fd=-1) noexcept;
        synchronous_wait_context(synchronous_wait_context const&)=delete;
        ~synchronous_wait_context();
        static bool stop_requested() noexcept
        {
            for(auto* scope=current;scope;scope=scope->previous)
            {
                if(scope->external_cancellation_fd< -1 || (scope->check&&scope->check(scope->context))) { return true; }
#if defined(__linux__)
                if(scope->external_cancellation_fd>=0)
                {
                    ::pollfd watched{scope->external_cancellation_fd,POLLIN,0};
                    if(::poll(&watched,1,0)<0 || watched.revents) { return true; }
                }
#endif
            }
            return false;
        }
        static int cancellation_fd() noexcept { return current?current->external_cancellation_fd:-1; }
        static bool present() noexcept { return current!=nullptr; }
        static bool native_exit_policy() noexcept { return current&&current->native_wait_on_exit; }
        static synchronous_quota_wait* waiter() noexcept;
        static bool select_native_exit(bool wait) noexcept;
        static bool process_cancelled() noexcept;
    };
    class native_exit_selection
    {
        bool previous{synchronous_wait_context::native_exit_policy()};
        bool admitted{};
    public:
        explicit native_exit_selection(bool finish) noexcept:admitted{synchronous_wait_context::select_native_exit(finish)} {}
        ~native_exit_selection() { (void)synchronous_wait_context::select_native_exit(previous); }
        explicit operator bool() const noexcept { return admitted; }
    };
    enum class quota_wait_result { ready, cancelled, unavailable };
    // A synchronous native operation keeps this object alive across wakeups.
    // Its exit owner is removed before descriptors close, so an exit notification
    // can never write through a recycled descriptor number.
    class synchronous_quota_wait
    {
#if defined(__linux__)
        // Cache only idle kernel resources, never an operation, permission or
        // cancellation owner. Nested waits allocate independently; this single
        // reuse slot imposes no limit on the number of live waits.
        inline static thread_local unsigned descriptor_cache_state{};
        struct idle_descriptors
        {
            int event{-1}, timer{-1};
            pid_t process{};
            idle_descriptors() noexcept { descriptor_cache_state=1u; }
            ~idle_descriptors()
            {
                descriptor_cache_state=2u; // Later TLS destructors must not borrow a dead cache.
                if(event>=0) { ::close(event); }
                if(timer>=0) { ::close(timer); }
            }
        };
        static idle_descriptors& descriptor_cache() noexcept
        { static thread_local idle_descriptors idle; return idle; }
        struct descriptors
        {
            int event{-1}, timer{-1};
            pid_t process{::getpid()};
            descriptors() noexcept
            {
                if(descriptor_cache_state!=2u)
                {
                    auto& idle{descriptor_cache()};
                    // fork() copies TLS but shares the kernel open-file objects.
                    // Never drain or rearm another process's cached resources.
                    if(idle.process!=process)
                    {
                        if(idle.event>=0) { ::close(idle.event); }
                        if(idle.timer>=0) { ::close(idle.timer); }
                        idle.event=idle.timer=-1; idle.process=process;
                    }
                    event=idle.event; timer=idle.timer; idle.event=idle.timer=-1;
                }
                if(event>=0 && timer>=0)
                {
                    // The previous exit owner was unregistered before release,
                    // and this new owner has not been registered yet. Drain its
                    // old event without racing any writer to the new operation.
                    ::std::uint64_t count{};
                    ssize_t n;
                    do { n=::read(event,&count,sizeof(count)); } while(n<0 && errno==EINTR);
                    if(n==static_cast<ssize_t>(sizeof(count)) || (n<0 && errno==EAGAIN)) { return; }
                }
                if(event>=0) { ::close(event); }
                if(timer>=0) { ::close(timer); }
                event=::eventfd(0u,EFD_NONBLOCK|EFD_CLOEXEC);
                timer=::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
            }
            ~descriptors()
            {
                // Member destruction unregisters exit_owner FIRST. A cached
                // descriptor cannot receive a later notification for this owner.
                // Disarm before caching so a completed operation schedules no
                // later timer wakeup. until() rearms before every actual poll.
                // An inherited live owner may only close its child descriptors;
                // it must never disarm the parent's shared kernel timer.
                if(event>=0 && timer>=0 && descriptor_cache_state!=2u && process==::getpid())
                {
                    auto& idle{descriptor_cache()};
                    if(idle.event<0 && idle.timer<0 && idle.process==process &&
                       ::uwvm2::utils::host_clock::arm_timer(timer,::std::chrono::steady_clock::time_point::max()))
                    { idle.event=event; idle.timer=timer; event=timer=-1; }
                }
                if(event>=0) { ::close(event); }
                if(timer>=0) { ::close(timer); }
            }
            descriptors(descriptors const&)=delete;
        } handles;
#endif
        ::uwvm2::utils::thread::process_exit::operation exit_owner{
            [](void* opaque) noexcept { static_cast<synchronous_quota_wait*>(opaque)->notify(); },this};
    public:
        synchronous_quota_wait() = default;
        synchronous_quota_wait(synchronous_quota_wait const&) = delete;
        [[nodiscard]] bool select_exit(bool wait) noexcept
        { return exit_owner.select(wait ? ::uwvm2::utils::thread::process_exit::wait_action::wait :
                                         ::uwvm2::utils::thread::process_exit::wait_action::cancel); }
        [[nodiscard]] bool exit_cancelled() const noexcept { return exit_owner.cancelled(); }
        [[nodiscard]] bool cancelled() const noexcept { return exit_owner.cancelled() || synchronous_wait_context::stop_requested(); }
        void notify() noexcept
        {
#if defined(__linux__)
            if(handles.event>=0)
            { ::std::uint64_t const one{1u}; while(::write(handles.event,&one,sizeof(one))<0 && errno==EINTR) {} }
#endif
        }
        [[nodiscard]] int notification_fd() const noexcept
        {
#if defined(__linux__)
            return handles.event;
#else
            return -1;
#endif
        }
        [[nodiscard]] quota_wait_result until(::std::chrono::steady_clock::time_point deadline, int cancellation_fd=-1, int native_fd=-1, short events=0) noexcept
        {
#if defined(__linux__)
            if(handles.event<0 || handles.timer<0) { return quota_wait_result::unavailable; }
            if(cancelled()) { return quota_wait_result::cancelled; }
            if(!::uwvm2::utils::host_clock::arm_timer(handles.timer,deadline)) { return quota_wait_result::unavailable; }
            ::pollfd watched[5]{{handles.event,POLLIN,0},{handles.timer,POLLIN,0},{cancellation_fd,POLLIN,0},{native_fd,events,0},
                {synchronous_wait_context::cancellation_fd(),POLLIN,0}};
            int result{};
            do { result=::poll(watched,5,-1); } while(result<0 && errno==EINTR);
            if(result<0) { return quota_wait_result::unavailable; }
            if(watched[2].revents || watched[4].revents || cancelled()) { return quota_wait_result::cancelled; }
            for(unsigned i{};i<2u;++i)
            {
                if(watched[i].revents&(POLLERR|POLLHUP|POLLNVAL)) { return quota_wait_result::unavailable; }
                if(watched[i].revents&POLLIN)
                { ::std::uint64_t count{}; while(::read(watched[i].fd,&count,sizeof(count))<0 && errno==EINTR) {} }
            }
            return quota_wait_result::ready;
#else
            (void)deadline;(void)cancellation_fd;(void)native_fd;(void)events;return quota_wait_result::unavailable;
#endif
        }
    };
    inline synchronous_wait_context::synchronous_wait_context(bool (*callback)(void*) noexcept,void* object,int cancellation_fd) noexcept :
        check{callback},context{object},external_cancellation_fd{cancellation_fd}
    {
        if(previous)
        {
            if(previous->external_cancellation_fd< -1 || cancellation_fd< -1 ||
               (previous->external_cancellation_fd>=0 && cancellation_fd>=0 && previous->external_cancellation_fd!=cancellation_fd))
            { external_cancellation_fd=-2; }
            else if(cancellation_fd==-1) { external_cancellation_fd=previous->external_cancellation_fd; }
        }
        current=this;
    }
    inline synchronous_wait_context::~synchronous_wait_context()
    { owned_waiter.reset(); current=previous; }
    inline synchronous_quota_wait* synchronous_wait_context::waiter() noexcept
    {
        if(!current) { return nullptr; }
        if(!current->owned_waiter)
        {
            current->owned_waiter.reset(new(::std::nothrow) synchronous_quota_wait{});
            if(current->owned_waiter) { (void)current->owned_waiter->select_exit(current->native_wait_on_exit); }
        }
        return current->owned_waiter.get();
    }
    inline bool synchronous_wait_context::select_native_exit(bool wait) noexcept
    {
        if(!current) { return !::uwvm2::utils::thread::process_exit::exit_claimed.load(::std::memory_order_acquire); }
        current->native_wait_on_exit=wait;
        if(wait || current->owned_waiter)
        {
            auto* active{waiter()};
            return active && active->select_exit(wait);
        }
        return !::uwvm2::utils::thread::process_exit::exit_claimed.load(::std::memory_order_acquire);
    }
    inline bool synchronous_wait_context::process_cancelled() noexcept
    {
        if(current && current->owned_waiter) { return current->owned_waiter->exit_cancelled(); }
        return ::uwvm2::utils::thread::process_exit::exit_claimed.load(::std::memory_order_acquire);
    }

}
