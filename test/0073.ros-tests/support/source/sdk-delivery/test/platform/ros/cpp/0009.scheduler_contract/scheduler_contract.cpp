/** @file
 * @brief Pure caller-driven C++ scheduling contracts over a fake C backend.
 * No ROS Runtime, clock, thread or Wasm launches are performed by this model.
 * A timed-out client remains a timed-out client even when its local Future is
 * ready; the fixture tests scheduling/ownership rather than native delivery.
 */
#include <platform/ros/cpp/roswasm/roswasmcpp.hpp>
#include "../support/ownership_backend.h"
#include <cstdio>
static unsigned checks;
#define CHECK(expression) do {++checks;if(!(expression)) {std::printf("FAIL C++ scheduler line=%u\n",(unsigned)__LINE__);return 1;}} while(0)
int main() {
    auto* backend=fixture_backend();roswasmcpp::Context context{*backend};roswasmcpp::Node node{context,"scheduler"};
    roswasmcpp::Executor executor{context,2};size_t delivered=99;unsigned calls=0,callbacks=0;
    auto first=executor.post<int>([&]{++calls;return roswasmcpp::DeferredResult<int>{0,7};},[&](auto value){++callbacks;value.value=999;});
    auto cancelled=executor.post<int>([&]{++calls;return roswasmcpp::DeferredResult<int>{0,8};});
    auto full=executor.post<int>([&]{++calls;return roswasmcpp::DeferredResult<int>{0,9};});int value=123;
    CHECK(calls==0&&first.state()==roswasmcpp::FutureState::pending);
    CHECK(first.take(value)==UWVM_ROS_AGAIN&&value==123);
    CHECK(full.take(value)==UWVM_ROS_TOO_LARGE&&value==123);
    CHECK(cancelled.cancel()==0&&cancelled.take(value)==UWVM_ROS_REVOKED);
    CHECK(executor.spin_some(0,delivered)==0&&delivered==0&&calls==1&&callbacks==1&&executor.completed_requests()==1);
    CHECK(first.take(value)==0&&value==7);
    CHECK(first.take(value)==UWVM_ROS_BAD_HANDLE);
    CHECK(executor.spin_some(0,delivered)==0&&calls==1&&executor.completed_requests()==0);
    roswasmcpp::Future<int> later;uint32_t nested=0;size_t nested_count=77;
    auto posting=executor.post<int>([&]{++calls;return roswasmcpp::DeferredResult<int>{0,10};},[&](auto){
        nested=executor.spin_some(0,nested_count);later=executor.post<int>([&]{++calls;return roswasmcpp::DeferredResult<int>{0,11};});
    });
    CHECK(executor.spin_some(0,delivered)==0&&calls==2&&later.state()==roswasmcpp::FutureState::pending);
    CHECK(nested==UWVM_ROS_INVALID&&nested_count==77);
    CHECK(executor.spin_some(0,delivered)==0&&calls==3);
    CHECK(posting.take(value)==0&&value==10&&later.take(value)==0&&value==11);
    roswasmcpp::CallbackGroup group;CHECK(group.enter());
    auto blocked=executor.post<int>([&]{++calls;return roswasmcpp::DeferredResult<int>{0,12};},{},&group);
    CHECK(executor.spin_some(0,delivered)==UWVM_ROS_CONFLICT&&calls==3&&blocked.state()==roswasmcpp::FutureState::pending);
    group.leave();CHECK(executor.spin_some(0,delivered)==0&&calls==4&&!group.busy());
    CHECK(blocked.take(value)==0&&value==12);
    roswasmcpp::Future<int> running;uint32_t running_cancel=0;
    running=executor.post<int>([&]{running_cancel=running.cancel();return roswasmcpp::DeferredResult<int>{0,13};});
    CHECK(executor.spin_some(0,delivered)==0&&running_cancel==UWVM_ROS_CONFLICT);
    CHECK(running.take(value)==0&&value==13);
    auto client=node.create_client<roswasmcpp::srv::Trigger>("call",64);
    CHECK(client.error()==0);
    fixture_request_error=UWVM_ROS_TIMEOUT;unsigned before=fixture_requests;
    auto rpc=client.async_call(executor,{},[&](auto response){++callbacks;(void)response;});
    CHECK(fixture_requests==before&&rpc.state()==roswasmcpp::FutureState::pending);
    CHECK(executor.spin_some(0,delivered)==0&&fixture_requests==before+1&&executor.completed_requests()==1);
    roswasmcpp::srv::Trigger::Response response{true,"preserved"};
    CHECK(rpc.take(response)==UWVM_ROS_TIMEOUT&&response.message=="preserved");
    CHECK(executor.spin_some(0,delivered)==0&&fixture_requests==before+1);
    CHECK(client.close()==0&&!client.is_open());
    unsigned ticks=0;roswasmcpp::Timer* timer_ptr=nullptr;uint32_t timer_nested=0;
    roswasmcpp::Timer timer{10,100,[&]{++ticks;timer_nested=timer_ptr->tick(110);return 0;},&group,&context};timer_ptr=&timer;
    CHECK(timer.error()==0&&timer.next_deadline()==110);
    auto registration=executor.add_timer(timer);CHECK(registration!=0);
    CHECK(executor.spin_some(1,delivered)==0&&ticks==0&&delivered==0);
    CHECK(executor.spin_some_at(109,1,delivered)==0&&ticks==0);
    CHECK(timer.tick(108)==UWVM_ROS_INVALID&&ticks==0);
    CHECK(executor.spin_some_at(110,1,delivered)==0&&ticks==1&&delivered==1&&timer_nested==UWVM_ROS_CONFLICT&&!group.busy());
    CHECK(timer.tick(160)==0&&ticks==2&&timer.next_deadline()==170);
    CHECK(timer.tick(160)==UWVM_ROS_AGAIN&&ticks==2);
    auto deadline=timer.next_deadline();CHECK(timer.reset(UINT64_MAX)==UWVM_ROS_TOO_LARGE&&timer.next_deadline()==deadline);
    CHECK(timer.tick(UINT64_MAX)==UWVM_ROS_TOO_LARGE&&ticks==2);
    timer.cancel();CHECK(timer.tick(170)==UWVM_ROS_REVOKED);
    CHECK(timer.reset(1000)==0&&timer.next_deadline()==1010);executor.remove(registration);
    auto stopped=executor.post<void>([]{return roswasmcpp::DeferredResult<void>{0};});
    context.shutdown();CHECK(!context.running());
    CHECK(executor.spin_some(0,delivered)==UWVM_ROS_REVOKED&&stopped.take()==UWVM_ROS_REVOKED);
    CHECK(timer.tick(1010)==UWVM_ROS_REVOKED&&timer.reset(1000)==UWVM_ROS_REVOKED);
    auto refused=executor.post<int>([&]{++calls;return roswasmcpp::DeferredResult<int>{0,0};});
    CHECK(refused.take(value)==UWVM_ROS_REVOKED&&calls==4);
    auto stopped_owner=node.create_publisher<roswasmcpp::msg::String>("pub");
    CHECK(stopped_owner.error()==UWVM_ROS_INVALID&&!stopped_owner.is_open());
    std::printf("PASS C++ scheduler models checks=%u\n",checks);return 0;
}
