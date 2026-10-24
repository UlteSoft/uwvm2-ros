/** @file
 * @brief Prepared regression for error90 leaving legacy C++ length slots untouched.
 * All definitions below are pure import models; this is not a Worker/native ROS run.
 */
#include <cassert>
#include <platform/ros1/cpp/roswasm/abi/ros1.hpp>
#include <platform/ros2/cpp/roswasm/abi/ros2.hpp>
#include <platform/ros1/cpp/roswasm/abi/ros1_freestanding.hpp>
#include <platform/ros2/cpp/roswasm/abi/ros2_freestanding.hpp>
static uint32_t cleanup_error;
static unsigned close_calls,revoke_calls;
/** @brief Store a test-owned eight-byte handle without assuming alignment. */
static void store_handle(uint64_t output) {
    unsigned char* bytes=reinterpret_cast<unsigned char*>(static_cast<uintptr_t>(output));
    bytes[0]=1;for(unsigned i=1;i<8;++i) bytes[i]=0;
}
/** @brief Model a successful alias open for ROS1; create no native resource. */
uint32_t uwvm_ros1_capability_open(uint32_t m,uint64_t a,uint64_t l,uint64_t o) {(void)m;(void)a;(void)l;store_handle(o);return 0;}
/** @brief Model a successful local cleanup. */
uint32_t uwvm_ros1_capability_close(uint64_t h) {(void)h;++close_calls;return cleanup_error;}
uint32_t uwvm_ros1_capability_revoke(uint64_t h) {(void)h;++revoke_calls;return cleanup_error;}
/** @brief Error90 before the host writes any output length. */
uint32_t uwvm_ros1_event_poll(uint64_t h,uint32_t m,uint64_t o,uint64_t c,uint64_t l) {(void)h;(void)m;(void)o;(void)c;(void)l;return 90;}
/** @brief Error90 before writing output length for a non-idempotent RPC. */
uint32_t uwvm_ros1_service_call(uint64_t h,uint32_t m,uint64_t i,uint64_t n,uint64_t o,uint64_t c,uint64_t l) {(void)h;(void)m;(void)i;(void)n;(void)o;(void)c;(void)l;return 90;}
/** @brief Model a successful alias open for ROS2; create no native resource. */
uint32_t uwvm_ros2_capability_open(uint32_t m,uint64_t a,uint64_t l,uint64_t o) {(void)m;(void)a;(void)l;store_handle(o);return 0;}
/** @brief Model a successful local cleanup. */
uint32_t uwvm_ros2_capability_close(uint64_t h) {(void)h;++close_calls;return cleanup_error;}
uint32_t uwvm_ros2_capability_revoke(uint64_t h) {(void)h;++revoke_calls;return cleanup_error;}
/** @brief Error90 before the host writes any output length. */
uint32_t uwvm_ros2_event_poll(uint64_t h,uint32_t m,uint64_t o,uint64_t c,uint64_t l) {(void)h;(void)m;(void)o;(void)c;(void)l;return 90;}
/** @brief Error90 before writing output length for a non-idempotent RPC. */
uint32_t uwvm_ros2_service_call(uint64_t h,uint32_t m,uint64_t i,uint64_t n,uint64_t o,uint64_t c,uint64_t l) {(void)h;(void)m;(void)i;(void)n;(void)o;(void)c;(void)l;return 90;}
/** @brief Verify hosted/freestanding ownership helpers preserve caller length on both backends. */
int main() {
    unsigned char output[8]{};uint64_t length=123;
    {uwvm_ros1::capability cap;assert(cap.open("alias")==0);
     assert(cap.poll(output,length)==90&&length==123);
     assert(cap.service_call({},output,length)==90&&length==123);}
    {uwvm_ros1::freestanding_capability cap;assert(cap.open("alias",5)==0);
     assert(cap.poll(output,sizeof output,length)==90&&length==123);
     assert(cap.service_call(nullptr,0,output,sizeof output,length)==90&&length==123);}
    {uwvm_ros2::capability cap;assert(cap.open("alias")==0);
     assert(cap.poll(output,length)==90&&length==123);
     assert(cap.service_call({},output,length)==90&&length==123);}
    {uwvm_ros2::freestanding_capability cap;assert(cap.open("alias",5)==0);
     assert(cap.poll(output,sizeof output,length)==90&&length==123);
     assert(cap.service_call(nullptr,0,output,sizeof output,length)==90&&length==123);}
    cleanup_error=110;unsigned before=close_calls;
    {uwvm_ros1::capability cap;assert(!cap.open("alias"));assert(cap.close()==110&&!cap.handle());}
    {uwvm_ros2::freestanding_capability cap;assert(!cap.open("alias",5));assert(cap.close()==110&&!cap.handle());}
    assert(close_calls==before+2);before=close_calls;
    {uwvm_ros2::capability cap;assert(!cap.open("alias"));assert(cap.revoke()==110&&!cap.handle());}
    {uwvm_ros1::freestanding_capability cap;assert(!cap.open("alias",5));assert(cap.revoke()==110&&!cap.handle());}
    assert(revoke_calls==2&&close_calls==before);
    return 0;
}
