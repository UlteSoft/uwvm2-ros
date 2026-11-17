/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include <uwvm2/utils/container/shared.h>
#include <atomic>
#include <condition_variable>
#include <concepts>
#include <functional>
#include <mutex>
#include <stop_token>
#include <thread>
#include <utility>

namespace uwvm2::utils::thread
{
    // The standard stop_source has no allocator-taking constructor. Keep its
    // synchronous callback contract, but allocate the shared state/control block
    // through fast_io. Callback nodes live in their registration, never a heap.
    namespace cancellation_details
    {
        struct callback_node
        {
            callback_node* previous{};
            callback_node* next{};
            void (*invoke)(callback_node*) noexcept{};
            bool linked{};
        };
        struct state
        {
            ::std::atomic_bool requested{};
            ::std::atomic_size_t sources{1u};
            ::std::mutex mutex;
            ::std::condition_variable finished;
            callback_node* head{};
            callback_node* executing{};
            ::std::thread::id executor{};
            void unlink(callback_node& node) noexcept
            {
                if(node.previous) { node.previous->next=node.next; }
                else { head=node.next; }
                if(node.next) { node.next->previous=node.previous; }
                node.linked=false;
            }
            void attach(callback_node& node) noexcept
            {
                ::std::unique_lock lock{mutex};
                if(requested.load(::std::memory_order_acquire))
                {
                    lock.unlock();
                    node.invoke(&node); // Already stopped: synchronous on the registering thread.
                }
                else if(sources.load(::std::memory_order_acquire)!=0u)
                {
                    node.next=head;
                    if(head) { head->previous=&node; }
                    head=&node;node.linked=true;
                }
            }
            void detach(callback_node& node) noexcept
            {
                ::std::unique_lock lock{mutex};
                if(node.linked) { unlink(node); }
                else if(executing==&node && executor!=::std::this_thread::get_id())
                {
                    // A foreign destructor must not return while its callable
                    // is still running. Self-destruction cannot wait for itself.
                    finished.wait(lock,[&]{return executing!=&node;});
                }
            }
            bool request() noexcept
            {
                if(requested.exchange(true,::std::memory_order_acq_rel)) { return false; }
                ::std::unique_lock lock{mutex};
                executor=::std::this_thread::get_id();
                while(head)
                {
                    auto* node=head;
                    auto invoke=node->invoke;
                    unlink(*node);executing=node;
                    lock.unlock();
                    invoke(node);
                    // The callback may have destroyed its registration. Never
                    // dereference node again; only the pinned state survives.
                    lock.lock();executing=nullptr;
                    finished.notify_all();
                }
                executor={};
                return true;
            }
        };
    }
    class stop_source;
    template<typename Callback> class stop_callback;
    class stop_token
    {
        friend class stop_source;
        template<typename> friend class stop_callback;
        ::std::shared_ptr<cancellation_details::state> state_{};
        explicit stop_token(::std::shared_ptr<cancellation_details::state> state) noexcept : state_(::std::move(state)) {}
    public:
        stop_token() noexcept=default;
        [[nodiscard]] bool stop_requested() const noexcept
        { return state_ && state_->requested.load(::std::memory_order_acquire); }
        [[nodiscard]] bool stop_possible() const noexcept
        { return state_ && (state_->requested.load(::std::memory_order_acquire) || state_->sources.load(::std::memory_order_acquire)!=0u); }
        void swap(stop_token& other) noexcept { state_.swap(other.state_); }
        friend bool operator==(stop_token const&,stop_token const&) noexcept=default;
    };
    class stop_source
    {
        ::std::shared_ptr<cancellation_details::state> state_{};
    public:
        stop_source() : state_(::uwvm2::utils::container::make_shared<cancellation_details::state>()) {}
        explicit stop_source(::std::nostopstate_t) noexcept {}
        stop_source(stop_source const& other) noexcept : state_(other.state_)
        { if(state_) { state_->sources.fetch_add(1u,::std::memory_order_relaxed); } }
        stop_source(stop_source&& other) noexcept=default;
        stop_source& operator=(stop_source other) noexcept { swap(other);return *this; }
        ~stop_source() { if(state_) { state_->sources.fetch_sub(1u,::std::memory_order_acq_rel); } }
        void swap(stop_source& other) noexcept { state_.swap(other.state_); }
        [[nodiscard]] stop_token get_token() const noexcept { return stop_token{state_}; }
        [[nodiscard]] bool stop_possible() const noexcept { return state_!=nullptr; }
        [[nodiscard]] bool stop_requested() const noexcept
        { return state_ && state_->requested.load(::std::memory_order_acquire); }
        bool request_stop() const noexcept
        {
            // A callback is allowed to destroy the requesting source itself.
            // Keep all bookkeeping alive independently of that object's lifetime.
            auto state=state_;
            return state && state->request();
        }
        friend bool operator==(stop_source const&,stop_source const&) noexcept=default;
    };
    template<typename Callback> class stop_callback final : private cancellation_details::callback_node
    {
        ::std::shared_ptr<cancellation_details::state> state_;
        [[no_unique_address]] Callback callback_;
    public:
        using callback_type=Callback;
        template<typename C> requires ::std::constructible_from<Callback,C>
        explicit stop_callback(stop_token token,C&& callback) noexcept(::std::is_nothrow_constructible_v<Callback,C>)
            : state_(::std::move(token.state_)),callback_(::std::forward<C>(callback))
        {
            this->invoke=[](cancellation_details::callback_node* node) noexcept
            { ::std::invoke(static_cast<stop_callback*>(node)->callback_); };
            if(state_) { state_->attach(*this); }
        }
        stop_callback(stop_callback const&)=delete;
        stop_callback& operator=(stop_callback const&)=delete;
        ~stop_callback() { if(state_) { state_->detach(*this); } }
    };
    template<typename Callback> stop_callback(stop_token,Callback) -> stop_callback<Callback>;
}
