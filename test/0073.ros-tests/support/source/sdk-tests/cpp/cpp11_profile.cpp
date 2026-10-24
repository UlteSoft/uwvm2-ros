/** @file Actual C++11/C++20 macro and library contract using a fake backend.
 * Covers copied codecs, default denial, moved ownership, cleanup failure and
 * bounded local scheduling. This is not native ROS/DDS or robot evidence.
 */
#include <platform/ros/cpp/roswasm/roswasmcpp.hpp>
#include <platform/ros/cpp/roswasm/library_profile.hpp>
#include "../c/mock_backend.h"
#include <cassert>
#include <utility>
static uint32_t once(void* user){++*static_cast<unsigned*>(user);return 110;}
int main(){
    assert(roswasmcpp::library_profile()==ROSWASM_CPP_STANDARD);
    roswasmcpp::Node node(*model_backend(),"explicit-profile");
    roswasmcpp::msg::String value;value.data="canonical";
    roswasmcpp::wire::bytes bytes;
    assert(roswasmcpp::Codec<roswasmcpp::msg::String>::encode(value,bytes));
    roswasmcpp::msg::String decoded;assert(roswasmcpp::Codec<roswasmcpp::msg::String>::decode(bytes,decoded));
    assert(decoded.data==value.data);bytes.pop_back();decoded.data="preserved";
    assert(!roswasmcpp::Codec<roswasmcpp::msg::String>::decode(bytes,decoded)&&decoded.data=="preserved");
    auto denied=node.create_publisher<roswasmcpp::msg::String>("absent");assert(denied.error()==13);
    auto publisher=node.create_publisher<roswasmcpp::msg::String>("allowed");
    assert(!publisher.error()&&!publisher.publish(value)&&model_publish_calls==1);
    roswasmcpp::Capability owner(node.context(),"allowed");uint64_t handle=owner.handle();
    roswasmcpp::Capability moved(std::move(owner));assert(!owner.handle()&&moved.handle()==handle);
    model_error=110;assert(moved.close()==110&&!moved.handle());model_error=0;
#if ROSWASM_CPP_STANDARD == 11
    roswasmcpp::Context context(*model_backend());roswasmcpp::Executor executor(context,1);
    roswasmc_future future;roswasmc_future_init(&future);unsigned calls=0;size_t completed=0;
    assert(!executor.post(future,once,&calls));assert(!calls);
    assert(!executor.spin_some(completed)&&completed==1&&calls==1);
    assert(roswasmc_future_take(&future)==110);
    assert(!executor.spin_some(completed)&&completed==0&&calls==1);
    context.shutdown();assert(executor.spin_some(completed)==125);
#endif
    return 0;
}
