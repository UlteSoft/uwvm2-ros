/** @file Real C++ ROS1 legacy SDK application; the command entry links in-module.
 * Complete typed Guest records are distinct from native ROS ABI/layouts.
 * Every decoded state field and reserved byte is compared through the actual
 * codec. One full command is submitted only after the parent's native arm.
 * No native ROS, thread, timer, firmware address or permission bridge exists.
 */
#include <roswasm/ros1.h>
#include "bounded_records.hpp"
#include "values_c.h"
#define EXPORT(N) extern "C" __attribute__((export_name(N)))
namespace {
using State=uwvm_custom::Ros1Unitree_legged_msgsLowState;
using Command=uwvm_custom::Ros1Unitree_legged_msgsLowCmd;
roswasmc_context context{};
roswasmcpp::Publisher<Command>* publisher{};
roswasmcpp::Subscription<State>* subscription{};
bool started{},closed{},submitted{};uint32_t status{},polls{},received{},denials{};
unsigned char observed[1536]{};uint32_t observed_size{};
uint32_t close_all(uint32_t first=0) {
    if(closed)return first?first:9;closed=true;
    if(subscription&&subscription->handle()){auto code=subscription->close();if(!first)first=code;}
    if(publisher&&publisher->handle()){auto code=publisher->close();if(!first)first=code;}
    delete subscription;subscription=nullptr;delete publisher;publisher=nullptr;status=first;return first;
}
}
EXPORT("initialize_case") uint32_t initialize_case(uint32_t mode,uint32_t family,uint32_t reserved) {
    if(started||closed)return 9;if(mode>1||family!=4||reserved)return close_all(22);started=true;
    if(uwvm_ros1_abi_version()!=1)return close_all(95);context.backend=roswasmc_ros1_backend();
    publisher=new roswasmcpp::Publisher<Command>(context,"legacy-lowcmd");auto code=publisher->error();
    if(!mode){
        if(code!=13||publisher->handle())return close_all(code?code:22);++denials;
        roswasmcpp::Capability probe(context,"legacy-lowstate");code=probe.error();
        if(code!=13||probe.handle()){if(probe.handle())(void)probe.close();return close_all(code?code:22);}++denials;return close_all();
    }
    if(code)return close_all(code);
    // Constructor calls the actual topic_subscribe import before READY/M.
    subscription=new roswasmcpp::Subscription<State>(context,"legacy-lowstate",roswasmcpp::Codec<State>::max_wire_bytes);
    if((code=subscription->error()))return close_all(code);
    subscription->on_message([](State const& value){
        if(received){status=22;return;}roswasmcpp::wire::bytes actual,expected;
        if(!roswasmcpp::Codec<State>::encode(value,actual)||!roswasmcpp::Codec<State>::encode(legacy_expected_state(),expected)||
           actual!=expected||actual.empty()||actual.size()>sizeof observed){status=22;return;}
        observed_size=static_cast<uint32_t>(actual.size());for(uint32_t i=0;i<observed_size;++i)observed[i]=actual[i];++received;
    });return 0;
}
EXPORT("poll_once") uint32_t poll_once(){
    if(!started||closed||received)return 9;if(polls++>=4096)return close_all(110);
    auto code=subscription->poll_once();if(code==11)return 11;if(code)return close_all(code);if(status)return close_all(status);
    return received==1?0:close_all(22);
}
EXPORT("submit_once") uint32_t submit_once(){
    if(!started||closed||received!=1||submitted)return 9;auto code=publisher->publish(legacy_expected_command());
    if(code)return close_all(code);submitted=true;return 0;
}
EXPORT("observed_length") uint32_t observed_length(){return received?observed_size:0;}
EXPORT("observed_byte") uint32_t observed_byte(uint32_t i){if(!received||i>=observed_size){status=22;return 0;}return observed[i];}
EXPORT("received_count") uint32_t received_count(){return received;}
EXPORT("denied_count") uint32_t denied_count(){return denials;}
EXPORT("case_status") uint32_t case_status(){return status;}
EXPORT("close_case") uint32_t close_case(){return close_all();}
