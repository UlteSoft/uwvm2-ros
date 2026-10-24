/** @file C++20 ownership and public correlation behavior. No ROS imports.
 * Compile separately from the C11 contract: the noncopyable wrapper must retain
 * the same ID lifetime and checked error behavior without exposing its storage.
 */
#include <platform/unitree/cpp/roswasm_unitree/topic_rpc.hpp>
#include <type_traits>

using roswasm_unitree::TopicRequests;
static_assert(!std::is_copy_constructible_v<TopicRequests>);
static_assert(!std::is_copy_assignable_v<TopicRequests>);
static_assert(!std::is_move_constructible_v<TopicRequests>);
static_assert(!std::is_move_assignable_v<TopicRequests>);
static_assert(std::is_nothrow_destructible_v<TopicRequests>);
#define CHECK(condition) do { if(!(condition))return __LINE__; } while(0)

int main() {
    TopicRequests requests;
    int64_t id=-1;
    CHECK(requests.begin_next(1008,1,10,id)==0);
    CHECK(id==1);
    CHECK(requests.forget(id,1008)==0);
    CHECK(requests.begin(1,1008,2,10)==17);
    CHECK(requests.begin_next(1008,2,10,id)==0);
    CHECK(id==2);
    CHECK(requests.accept(1,1008,3)==9);
    CHECK(requests.accept(2,1008,3)==0);
    CHECK(requests.begin_next(1008,3,10,id)==0);
    CHECK(id==3);
    CHECK(requests.expire(id,1008,10)==110);
    CHECK(requests.begin(3,1008,11,20)==17);
    CHECK(requests.begin_next(1008,11,20,id)==0);
    CHECK(id==4);
    CHECK(requests.accept(3,1008,12)==9);
    CHECK(requests.accept(4,1008,12)==0);
    CHECK(requests.begin(INT64_MAX,1008,12,20)==0);
    CHECK(requests.begin_next(1008,12,20,id)==75);
    CHECK(id==4);
    requests.close();
    CHECK(requests.accept(INT64_MAX,1008,13)==9);
    CHECK(requests.begin_next(1008,13,20,id)==9);
    CHECK(id==4);
    requests.close();
    return 0;
}
