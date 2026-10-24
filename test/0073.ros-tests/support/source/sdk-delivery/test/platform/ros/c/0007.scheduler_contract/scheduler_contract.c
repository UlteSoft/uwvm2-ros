/** @file
 * @brief Pure host contract model for local C scheduling; no Runtime/ROS import.
 * The fixture checks actual local control flow, not successful native service
 * delivery. Codes110/125 remain failure/cancellation results, never ROS PASS.
 * Compile as one C11 translation unit with -I sdk/include; run serially.
 */
#include <platform/ros/c/roswasm/node.h>
#include <stdio.h>
static unsigned checks;
#define CHECK(expression) do {++checks;if(!(expression)) {printf("FAIL C scheduler line=%u\n",(unsigned)__LINE__);return 1;}} while(0)
typedef struct state {
    roswasmc_executor* executor;roswasmc_future* future;roswasmc_future* later;
    roswasmc_timer* timer;roswasmc_local_context* context;
    unsigned calls,completions;uint32_t code,nested,cancel,posted;size_t sentinel;
    int repost,shutdown;
} state;
/** @brief Simulate one bounded operation, recording cancellation while running. */
static uint32_t call(void* opaque) {
    state* s=(state*)opaque;++s->calls;
    if(s->future) s->cancel=roswasmc_future_cancel(s->future);
    return s->code;
}
/** @brief Result is copied before completion; callback posts wait for next pass. */
static void completion(void* opaque,roswasmc_future const* snapshot) {
    state* s=(state*)opaque;++s->completions;s->code=snapshot->error;
    s->sentinel=77;s->nested=roswasmc_executor_spin(s->executor,&s->sentinel);
    if(s->repost) {s->repost=0;s->posted=roswasmc_executor_post(s->executor,s->later,call,s,0,0);}
    if(s->shutdown) roswasmc_local_context_shutdown(s->context);
}
/** @brief Explicit-time callback checks recursion without reading a clock. */
static uint32_t tick(void* opaque) {
    state* s=(state*)opaque;++s->calls;s->nested=roswasmc_timer_tick(s->timer,s->timer->last);return s->code;
}
int main(void) {
    roswasmc_local_context context;roswasmc_executor executor;roswasmc_deferred_task slots[2];
    roswasmc_future first,second,third;roswasmc_callback_group group;size_t done=99;
    state s={0};roswasmc_timer timer;uint64_t old_due;
    roswasmc_local_context_init(&context);roswasmc_future_init(&first);roswasmc_future_init(&second);roswasmc_future_init(&third);
    s.executor=&executor;s.future=&first;s.later=&third;s.context=&context;s.code=UWVM_ROS_TIMEOUT;
    CHECK(roswasmc_executor_init(&executor,&context,slots,2)==0);
    CHECK(roswasmc_executor_post(&executor,&first,call,&s,completion,0)==0&&s.calls==0);
    CHECK(roswasmc_future_take(&first)==UWVM_ROS_AGAIN);
    CHECK(roswasmc_executor_post(&executor,&first,call,&s,0,0)==UWVM_ROS_CONFLICT&&executor.count==1);
    CHECK(roswasmc_executor_post(&executor,&second,call,&s,0,0)==0);
    CHECK(roswasmc_executor_post(&executor,&third,call,&s,0,0)==UWVM_ROS_TOO_LARGE&&third.state==ROSWASMC_FUTURE_READY);
    CHECK(roswasmc_future_take(&third)==UWVM_ROS_TOO_LARGE);
    CHECK(roswasmc_future_cancel(&second)==0);
    CHECK(roswasmc_future_take(&second)==UWVM_ROS_REVOKED);
    CHECK(roswasmc_executor_spin(&executor,&done)==0&&done==1&&s.calls==1&&s.completions==1);
    CHECK(s.cancel==UWVM_ROS_CONFLICT&&s.nested==UWVM_ROS_INVALID&&s.sentinel==77);
    CHECK(!first.queued&&!second.queued&&executor.count==0);
    CHECK(roswasmc_future_take(&first)==UWVM_ROS_TIMEOUT);
    CHECK(roswasmc_future_take(&first)==UWVM_ROS_BAD_HANDLE);
    CHECK(roswasmc_executor_spin(&executor,&done)==0&&done==0&&s.calls==1);
    s.future=&first;s.code=0;s.repost=1;
    CHECK(roswasmc_executor_post(&executor,&first,call,&s,completion,0)==0);
    CHECK(roswasmc_executor_spin(&executor,&done)==0&&done==1&&s.calls==2&&s.posted==0&&executor.count==1);
    CHECK(roswasmc_future_take(&third)==UWVM_ROS_AGAIN);
    s.future=&third;
    CHECK(roswasmc_executor_spin(&executor,&done)==0&&done==1&&s.calls==3);
    CHECK(roswasmc_future_take(&first)==0&&roswasmc_future_take(&third)==0);
    roswasmc_callback_group_init(&group,1);
    CHECK(roswasmc_callback_group_enter(&group)==0);
    CHECK(roswasmc_callback_group_enter(&group)==UWVM_ROS_CONFLICT);
    CHECK(roswasmc_executor_post(&executor,&first,call,&s,0,&group)==0);
    CHECK(roswasmc_executor_spin(&executor,&done)==UWVM_ROS_CONFLICT&&done==0&&s.calls==3&&first.queued);
    roswasmc_callback_group_leave(&group);s.future=&first;
    CHECK(roswasmc_executor_spin(&executor,&done)==0&&done==1&&group.active==0&&s.calls==4);
    CHECK(roswasmc_future_take(&first)==0);
    s.timer=&timer;s.future=0;s.calls=0;
    CHECK(roswasmc_timer_init(&timer,10,100,tick,&s,&group,&context)==0);
    CHECK(roswasmc_timer_tick(&timer,109)==UWVM_ROS_AGAIN&&s.calls==0);
    CHECK(roswasmc_timer_tick(&timer,108)==UWVM_ROS_INVALID&&s.calls==0);
    CHECK(roswasmc_timer_tick(&timer,110)==0&&s.calls==1&&s.nested==UWVM_ROS_CONFLICT&&timer.due==120&&group.active==0);
    CHECK(roswasmc_timer_tick(&timer,160)==0&&s.calls==2&&timer.due==170);
    CHECK(roswasmc_timer_tick(&timer,160)==UWVM_ROS_AGAIN&&s.calls==2);
    old_due=timer.due;
    CHECK(roswasmc_timer_reset(&timer,UINT64_MAX)==UWVM_ROS_TOO_LARGE&&timer.due==old_due);
    CHECK(roswasmc_timer_tick(&timer,UINT64_MAX)==UWVM_ROS_TOO_LARGE&&s.calls==2);
    roswasmc_timer_cancel(&timer);
    CHECK(roswasmc_timer_tick(&timer,170)==UWVM_ROS_REVOKED);
    CHECK(roswasmc_timer_reset(&timer,1000)==0&&timer.due==1010);
    s.shutdown=1;s.future=&first;s.code=0;
    CHECK(roswasmc_executor_post(&executor,&first,call,&s,completion,0)==0);
    CHECK(roswasmc_executor_post(&executor,&second,call,&s,0,0)==0);
    CHECK(roswasmc_executor_spin(&executor,&done)==UWVM_ROS_REVOKED&&done==1&&s.calls==3);
    CHECK(roswasmc_future_take(&first)==0&&roswasmc_future_take(&second)==UWVM_ROS_REVOKED&&!second.queued);
    CHECK(roswasmc_timer_tick(&timer,1010)==UWVM_ROS_REVOKED&&roswasmc_timer_reset(&timer,1000)==UWVM_ROS_REVOKED);
    CHECK(roswasmc_executor_post(&executor,&third,call,&s,0,0)==UWVM_ROS_REVOKED&&roswasmc_future_take(&third)==UWVM_ROS_REVOKED);
    CHECK(roswasmc_executor_init(&executor,&context,slots,1048577u)==UWVM_ROS_INVALID);
    printf("PASS C scheduler models checks=%u\n",checks);return 0;
}
