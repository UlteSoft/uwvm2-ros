/** @file
 * @brief Guest-local Context, Future, CallbackGroup and explicit-time Timer.
 *
 * Deferred work runs on a caller-driven Executor. These types create no host
 * thread, event loop, random source or clock grant. Deferring an RPC changes its
 * guest scheduling only: the existing bounded host RPC still executes once and
 * synchronously. A cancellation cannot undo an admitted external side effect.
 */
#pragma once
#include "roswasmc.h"
#include <functional>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <limits>
namespace roswasmcpp {
/** @brief Explicit local application context; backend selection confers no rights.
 * Shutdown prevents new Context-associated dispatch, notifies no native node,
 * and does not revoke host aliases. Close native owners separately. Context must
 * outlive its associated Node/Executor/Timer. No thread synchronization exists.
 */
class Context {
    roswasmc_backend const* backend_;bool running_{true};
public:
    /** @brief Select one immutable backend without opening a resource or reading time. */
    explicit Context(roswasmc_backend const& backend):backend_{&backend} {}
    Context(Context const&)=delete;Context& operator=(Context const&)=delete;
    /** @brief Inspect local dispatch state; this makes no import call. */
    bool running() const noexcept {return running_;}
    /** @brief Stop local dispatch. Existing owners remain explicitly closeable. */
    void shutdown() noexcept {running_=false;}
    /** @brief Return selected dispatch metadata, never a grant or native node pointer. */
    roswasmc_backend const& backend() const noexcept {return *backend_;}
};
/** @brief Single-guest-thread callback grouping; it is not a native mutex/thread. */
class CallbackGroup {
    bool exclusive_;size_t active_{};
public:
    /** @brief Exclusive refuses nested entry; reentrant permits direct nested callbacks.
     * Executor itself always refuses recursive spin, independently of this mode.
     */
    explicit CallbackGroup(bool mutually_exclusive=true):exclusive_{mutually_exclusive} {}
    /** @brief Attempt local entry without calling ROS. Pair every success with leave. */
    bool enter() noexcept {if((exclusive_&&active_)||active_==std::numeric_limits<size_t>::max()) return false;++active_;return true;}
    /** @brief Release an acquired local borrow after callback completion/unwinding. */
    void leave() noexcept {if(active_) --active_;}
    /** @brief Inspect whether this group currently has an executing callback. */
    bool busy() const noexcept {return active_!=0;}
};
/** @brief Named local future phases; cancellation is not native alias revocation. */
enum class FutureState {pending,running,ready,cancelled,taken};
/** @brief Owned completion value and actual SDK/RPC code. Nonzero never means success. */
template<class T> struct DeferredResult {uint32_t error{};T value{};};
/** @brief Void task completion carries only its actual error code. */
template<> struct DeferredResult<void> {uint32_t error{};};
namespace detail {
/** @brief Value storage for a void completion; no user-visible payload is fabricated. */
struct Unit {};
template<class T> using DeferredStorage=std::conditional_t<std::is_void_v<T>,Unit,T>;
/** @brief Shared local scheduling state; the Executor invokes no host callback. */
template<class T> struct FutureData {
    FutureState phase{FutureState::pending};uint32_t error{};std::optional<DeferredStorage<T>> value;
};
/** @brief Ensure callback group entry releases on return or native C++ unwinding. */
struct GroupLease {
    CallbackGroup* group;explicit GroupLease(CallbackGroup* value):group{value} {}
    ~GroupLease() {if(group) group->leave();}
    GroupLease(GroupLease const&)=delete;GroupLease& operator=(GroupLease const&)=delete;
};
/** @brief A thrown deferred callable locally cancels an unfinished future.
 * The exception still propagates and may follow an admitted native effect.
 * No-exception Wasm profiles require nonthrowing task/callback functions.
 */
template<class T> struct RunningLease {
    std::shared_ptr<FutureData<T>> data;
    ~RunningLease() {if(data->phase==FutureState::running) {data->phase=FutureState::cancelled;data->error=UWVM_ROS_REVOKED;}}
};
}
/** @brief Copyable guest-local future; all copies share one result consumption state.
 * It never blocks, pumps a loop, reads a clock or performs a native cancellation.
 * Pending result11 and local cancellation125 are distinguishable via state().
 */
template<class T> class Future {
    std::shared_ptr<detail::FutureData<T>> data_;
    explicit Future(std::shared_ptr<detail::FutureData<T>> data):data_{std::move(data)} {}
    friend class Executor;
    uint32_t consume_error() {
        if(!data_) return UWVM_ROS_INVALID;
        if(data_->phase==FutureState::pending||data_->phase==FutureState::running) return UWVM_ROS_AGAIN;
        if(data_->phase==FutureState::cancelled) return UWVM_ROS_REVOKED;
        if(data_->phase==FutureState::taken) return UWVM_ROS_BAD_HANDLE;
        return data_->error;
    }
public:
    /** @brief Empty future grants nothing and rejects result access22. */
    Future()=default;
    /** @brief Inspect the local phase without performing an import. */
    FutureState state() const noexcept {return data_?data_->phase:FutureState::taken;}
    /** @brief True when a completion or local cancellation can be inspected. */
    bool ready() const noexcept {return data_&&(data_->phase==FutureState::ready||data_->phase==FutureState::cancelled);}
    /** @brief Cancel a pending local task. A running RPC returns17 and continues once. */
    uint32_t cancel() noexcept {
        if(!data_) return UWVM_ROS_INVALID;
        if(data_->phase==FutureState::running) return UWVM_ROS_CONFLICT;
        if(data_->phase!=FutureState::pending) return UWVM_ROS_BAD_HANDLE;
        data_->phase=FutureState::cancelled;data_->error=UWVM_ROS_REVOKED;return 0;
    }
    /** @brief Consume one successful owned value, preserving output on every error.
     * A failed RPC completion exposes its real code once, with no replay. Taking
     * a value invalidates result access through all copies of this future.
     */
    template<class U=T> requires (!std::is_void_v<U>&&std::is_same_v<U,T>) uint32_t take(U& output) {
        auto code=consume_error();if(!data_||data_->phase!=FutureState::ready) return code;
        if(!code) output=std::move(*data_->value);data_->value.reset();data_->phase=FutureState::taken;return code;
    }
    /** @brief Consume one void completion, exposing its actual operation code. */
    uint32_t take() requires std::is_void_v<T> {
        auto code=consume_error();if(data_&&data_->phase==FutureState::ready) {data_->value.reset();data_->phase=FutureState::taken;}return code;
    }
};
/** @brief Coalescing local timer driven solely by explicit caller nanoseconds.
 * Supply now yourself or through a separately granted Clock::now call. There
 * is no default clock, sleep, chrono query, implicit ROS time or timer thread.
 * Each tick executes at most once; missed periods are coalesced without looping.
 */
class Timer {
    uint64_t period_{},due_{},last_{};std::function<void()> callback_;CallbackGroup* group_{};Context* context_{};
    uint32_t error_{};bool cancelled_{},active_{};
public:
    /** @brief Schedule from explicit initial_now. period0 or overflow is rejected locally. */
    Timer(uint64_t period,uint64_t initial_now,std::function<void()> callback,CallbackGroup* group=nullptr,Context* context=nullptr)
        :period_{period},last_{initial_now},callback_{std::move(callback)},group_{group},context_{context} {
        if(!period_||!callback_) error_=UWVM_ROS_INVALID;
        else if(period_>std::numeric_limits<uint64_t>::max()-initial_now) error_=UWVM_ROS_TOO_LARGE;
        else due_=initial_now+period_;
    }
    /** @brief Inspect setup status; this is a local scheduling result. */
    uint32_t error() const noexcept {return error_;}
    /** @brief Stop this local timer, leaving every native alias untouched. */
    void cancel() noexcept {cancelled_=true;}
    /** @brief Inspect cancellation and period without reading a clock. */
    bool cancelled() const noexcept {return cancelled_;}
    uint64_t period() const noexcept {return period_;}
    /** @brief Inspect the scheduled deadline; zero is meaningful only after valid setup. */
    uint64_t next_deadline() const noexcept {return due_;}
    /** @brief Explicitly restart from caller time, without querying any clock.
     * Reject restart during a callback17 or after Context shutdown125. An
     * overflow/malformed callback leaves prior timer state unchanged.
     */
    uint32_t reset(uint64_t now) {
        if(active_) return UWVM_ROS_CONFLICT;if(context_&&!context_->running()) return UWVM_ROS_REVOKED;
        if(!period_||!callback_) return UWVM_ROS_INVALID;
        if(period_>std::numeric_limits<uint64_t>::max()-now) return UWVM_ROS_TOO_LARGE;
        last_=now;due_=now+period_;cancelled_=false;error_=0;return 0;
    }
    /** @brief Tick at most once.11 means not due,125 local cancellation/shutdown.
     * Backwards time22 and next-deadline overflow90 perform no callback. A nested
     * tick returns17. The next due time commits before callback execution, and
     * local group/busy borrows release on normal return or exception unwinding.
     */
    uint32_t tick(uint64_t now) {
        if(error_) return error_;if(cancelled_||(context_&&!context_->running())) return UWVM_ROS_REVOKED;
        if(active_) return UWVM_ROS_CONFLICT;if(now<last_) return UWVM_ROS_INVALID;last_=now;
        if(now<due_) return UWVM_ROS_AGAIN;
        if(period_>std::numeric_limits<uint64_t>::max()-now) return UWVM_ROS_TOO_LARGE;
        if(group_&&!group_->enter()) return UWVM_ROS_CONFLICT;detail::GroupLease lease{group_};
        struct BusyLease {bool& active;~BusyLease(){active=false;}} busy{active_};active_=true;due_=now+period_;
        callback_();return 0;
    }
};
}
