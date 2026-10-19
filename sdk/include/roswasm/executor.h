/** @file
 * @brief Allocation-free caller-driven C Future/deferred queue and Timer.
 *
 * No native thread, clock, sleep, random source or ambient permission is used.
 * A deferred callable executes an existing bounded synchronous SDK call once;
 * its local Future is ready only after that callable returns. Cancellation and
 * Context shutdown affect local dispatch, never undo an external native effect.
 */
#ifndef ROSWASMC_EXECUTOR_H
#define ROSWASMC_EXECUTOR_H
#include "session.h"
/** @brief Local scheduling phases, distinct from native capability lifecycle. */
typedef enum roswasmc_future_state {
    ROSWASMC_FUTURE_PENDING,ROSWASMC_FUTURE_RUNNING,ROSWASMC_FUTURE_READY,
    ROSWASMC_FUTURE_CANCELLED,ROSWASMC_FUTURE_TAKEN
} roswasmc_future_state;
/** @brief Caller-owned result status. Typed result storage belongs to task.user.
 * Keep this Future and typed user storage alive until task completion/cancellation.
 * A callback receives a separate status snapshot after the callable has returned.
 */
typedef struct roswasmc_future {roswasmc_future_state state;uint32_t error;int queued;} roswasmc_future;
/** @brief Initialize a fresh unqueued Future. Never reinitialize one still borrowed by a queue. */
static inline void roswasmc_future_init(roswasmc_future* future) {
    if(future) {future->state=ROSWASMC_FUTURE_TAKEN;future->error=UWVM_ROS_BAD_HANDLE;future->queued=0;}
}
/** @brief Cancel only pending local work;17 means a synchronous call is running. */
static inline uint32_t roswasmc_future_cancel(roswasmc_future* future) {
    if(!future) return UWVM_ROS_INVALID;if(future->state==ROSWASMC_FUTURE_RUNNING) return UWVM_ROS_CONFLICT;
    if(future->state!=ROSWASMC_FUTURE_PENDING) return UWVM_ROS_BAD_HANDLE;
    future->state=ROSWASMC_FUTURE_CANCELLED;future->error=UWVM_ROS_REVOKED;return 0;
}
/** @brief Consume a ready operation code once, with no waiting/pumping/retry.
 * Return11 for pending/running and125 for local cancellation. Inspect state to
 * distinguish local cancellation from an actual completed native errno125.
 */
static inline uint32_t roswasmc_future_take(roswasmc_future* future) {
    uint32_t code;if(!future) return UWVM_ROS_INVALID;
    if(future->state==ROSWASMC_FUTURE_PENDING||future->state==ROSWASMC_FUTURE_RUNNING) return UWVM_ROS_AGAIN;
    if(future->state==ROSWASMC_FUTURE_CANCELLED) return UWVM_ROS_REVOKED;
    if(future->state==ROSWASMC_FUTURE_TAKEN) return UWVM_ROS_BAD_HANDLE;
    code=future->error;future->state=ROSWASMC_FUTURE_TAKEN;return code;
}
/** @brief Single-caller callback group; this is not an OS mutex or native thread. */
typedef struct roswasmc_callback_group {int exclusive;size_t active;} roswasmc_callback_group;
/** @brief Configure local mutually-exclusive or directly reentrant callbacks. */
static inline void roswasmc_callback_group_init(roswasmc_callback_group* group,int exclusive) {if(group) {group->exclusive=exclusive!=0;group->active=0;}}
/** @brief Enter only when allowed; a recursive Executor spin is always refused separately. */
static inline uint32_t roswasmc_callback_group_enter(roswasmc_callback_group* group) {
    if(!group) return 0;if((group->exclusive&&group->active)||group->active==SIZE_MAX) return UWVM_ROS_CONFLICT;
    ++group->active;return 0;
}
/** @brief Release the local callback borrow after normal C return. */
static inline void roswasmc_callback_group_leave(roswasmc_callback_group* group) {if(group&&group->active) --group->active;}
/** @brief Deferred operation executes once and returns the actual SDK/RPC code. */
typedef uint32_t (*roswasmc_deferred_call)(void* user);
/** @brief Called after the operation's stack/scratch borrows have been released.
 * status is a local immutable snapshot valid for this callback only. Copy it
 * if retained. Typed results must already be copied into caller-owned user data.
 */
typedef void (*roswasmc_completion_callback)(void* user,roswasmc_future const* status);
/** @brief Bounded ring queue slot; all pointers remain caller-owned. */
typedef struct roswasmc_deferred_task {
    roswasmc_future* future;roswasmc_deferred_call call;roswasmc_completion_callback completion;
    void* user;roswasmc_callback_group* group;
} roswasmc_deferred_task;
/** @brief Caller-buffer Executor. Poll queues remain separately usable via spin_some.
 * The context, slots, Futures, groups and captured resource/user data must remain
 * valid until completion/cancellation. Do not copy/reinitialize a live executor.
 * Callback posts wait for the next outer pass; ring storage avoids moving live
 * closures while callbacks run. No blocking wait or implicit time source exists.
 */
typedef struct roswasmc_executor {
    roswasmc_local_context const* context;roswasmc_deferred_task* slots;
    size_t capacity,head,count,last_completions;int spinning;
} roswasmc_executor;
/** @brief Initialize a fresh bounded caller-storage queue, with no native action. */
static inline uint32_t roswasmc_executor_init(roswasmc_executor* executor,roswasmc_local_context const* context,
    roswasmc_deferred_task* slots,size_t capacity) {
    if(!executor||!context||(!slots&&capacity)||capacity>1048576u) return UWVM_ROS_INVALID;
    executor->context=context;executor->slots=slots;executor->capacity=capacity;
    executor->head=executor->count=executor->last_completions=0;executor->spinning=0;return 0;
}
/** @brief Queue one task into a fresh Future. No callable/import runs here.
 * A rejected post records a ready22/90/125 result. The previous Future must be
 * retired/not queued before reuse. Captured native owners remain the caller's
 * responsibility; cancel before closing/destroying their borrowed references.
 */
static inline uint32_t roswasmc_executor_post(roswasmc_executor* executor,roswasmc_future* future,
    roswasmc_deferred_call call,void* user,roswasmc_completion_callback completion,roswasmc_callback_group* group) {
    uint32_t code=0;size_t index;
    if(!future) return UWVM_ROS_INVALID;
    if(future->queued||future->state==ROSWASMC_FUTURE_RUNNING) return UWVM_ROS_CONFLICT;
    if(!executor||!executor->context||!call) code=UWVM_ROS_INVALID;
    else if(!executor->context->active) code=UWVM_ROS_REVOKED;
    else if(executor->count>=executor->capacity) code=UWVM_ROS_TOO_LARGE;
    future->state=code?ROSWASMC_FUTURE_READY:ROSWASMC_FUTURE_PENDING;future->error=code;future->queued=0;if(code) return code;
    index=(executor->head+executor->count)%executor->capacity;
    {roswasmc_deferred_task task={future,call,completion,user,group};executor->slots[index]=task;}future->queued=1;++executor->count;return 0;
}
/** @brief Cancel every unexecuted local task, never an admitted external RPC. */
static inline void roswasmc_executor_cancel_pending(roswasmc_executor* executor) {
    size_t n;if(!executor||!executor->slots) return;
    for(n=0;n<executor->count;++n) {roswasmc_future* f=executor->slots[(executor->head+n)%executor->capacity].future;if(f) {if(f->state==ROSWASMC_FUTURE_PENDING) roswasmc_future_cancel(f);f->queued=0;}}
    executor->head=executor->count=0;
}
/** @brief Run only tasks present at pass entry, at most once each.
 * Nested spin22 leaves completed unchanged. A task's native errno is stored in
 * its Future and is never retried. completed counts local callable completions,
 * including error completions; it is not successful ROS delivery evidence.
 * C callbacks must return normally; longjmp through borrowed scheduler state is
 * unsupported. A blocked callback group17 leaves the task pending for a later
 * explicit pass without calling its operation.
 */
static inline uint32_t roswasmc_executor_spin(roswasmc_executor* executor,size_t* completed) {
    size_t initial,n;if(!executor||!completed||!executor->context) return UWVM_ROS_INVALID;
    if(executor->spinning) return UWVM_ROS_INVALID;
    if(!executor->context->active) {roswasmc_executor_cancel_pending(executor);return UWVM_ROS_REVOKED;}
    executor->spinning=1;*completed=executor->last_completions=0;initial=executor->count;
    for(n=0;n<initial&&executor->count;++n) {
        roswasmc_deferred_task task=executor->slots[executor->head];uint32_t code;
        if(task.future->state==ROSWASMC_FUTURE_PENDING) {
            code=roswasmc_callback_group_enter(task.group);if(code) {executor->spinning=0;return code;}
            executor->head=(executor->head+1)%executor->capacity;--executor->count;
            task.future->state=ROSWASMC_FUTURE_RUNNING;task.future->error=task.call(task.user);
            task.future->state=ROSWASMC_FUTURE_READY;task.future->queued=0;++executor->last_completions;*completed=executor->last_completions;
            if(task.completion) {roswasmc_future snapshot=*task.future;task.completion(task.user,&snapshot);}
            roswasmc_callback_group_leave(task.group);
        } else {task.future->queued=0;executor->head=(executor->head+1)%executor->capacity;--executor->count;}
        if(!executor->context->active) {roswasmc_executor_cancel_pending(executor);executor->spinning=0;return UWVM_ROS_REVOKED;}
    }
    executor->spinning=0;return 0;
}
/** @brief Explicit-time coalescing timer. No clock is ever read by this struct. */
typedef struct roswasmc_timer {
    uint64_t period,due,last;roswasmc_deferred_call callback;void* user;
    roswasmc_callback_group* group;roswasmc_local_context const* context;int cancelled,active;
} roswasmc_timer;
/** @brief Configure period and initial caller time; reject0/overflow before mutation. */
static inline uint32_t roswasmc_timer_init(roswasmc_timer* timer,uint64_t period,uint64_t now,
    roswasmc_deferred_call callback,void* user,roswasmc_callback_group* group,roswasmc_local_context const* context) {
    if(!timer||!period||!callback) return UWVM_ROS_INVALID;if(period>UINT64_MAX-now) return UWVM_ROS_TOO_LARGE;
    {roswasmc_timer next={period,now+period,now,callback,user,group,context,0,0};*timer=next;}return 0;
}
/** @brief Cancel local timer scheduling only; no native alias is revoked. */
static inline void roswasmc_timer_cancel(roswasmc_timer* timer) {if(timer) timer->cancelled=1;}
/** @brief Restart using caller time, never a clock query or native transition. */
static inline uint32_t roswasmc_timer_reset(roswasmc_timer* timer,uint64_t now) {
    if(!timer||!timer->period||!timer->callback) return UWVM_ROS_INVALID;if(timer->active) return UWVM_ROS_CONFLICT;
    if(timer->context&&!timer->context->active) return UWVM_ROS_REVOKED;
    if(timer->period>UINT64_MAX-now) return UWVM_ROS_TOO_LARGE;
    timer->last=now;timer->due=now+timer->period;timer->cancelled=0;return 0;
}
/** @brief Tick once at explicit time;11 not due,125 local cancellation/shutdown.
 * Time rollback22, nesting17 and overflow90 perform no callback. Missed periods
 * coalesce into one callback and next due=now+period, never a catch-up busy loop.
 */
static inline uint32_t roswasmc_timer_tick(roswasmc_timer* timer,uint64_t now) {
    uint32_t code;if(!timer||!timer->period||!timer->callback) return UWVM_ROS_INVALID;
    if(timer->cancelled||(timer->context&&!timer->context->active)) return UWVM_ROS_REVOKED;
    if(timer->active) return UWVM_ROS_CONFLICT;if(now<timer->last) return UWVM_ROS_INVALID;timer->last=now;
    if(now<timer->due) return UWVM_ROS_AGAIN;if(timer->period>UINT64_MAX-now) return UWVM_ROS_TOO_LARGE;
    code=roswasmc_callback_group_enter(timer->group);if(code) return code;
    timer->due=now+timer->period;timer->active=1;code=timer->callback(timer->user);
    timer->active=0;roswasmc_callback_group_leave(timer->group);return code;
}
#endif
