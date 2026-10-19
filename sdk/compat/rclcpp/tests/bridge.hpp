/** Pure host model backend over REAL SDK wrappers, not ROS native evidence.
 * Only this test binding is mutable. No generated/native hash or provider is
 * fabricated and no production import is replaced in actual Wasm recipes.
 */
#pragma once
#include <string>
#include <vector>
#include <array>
#include <cstring>
namespace bridge {
inline std::uint64_t now{},next{1};inline unsigned steps{},opens{},closes{},publishes{},polls{},logs{},times{};
inline bool deny{},malformed{},clock_fail{},log_deny{},publish_fail{};inline std::vector<unsigned char> bytes;inline std::vector<std::string> output;
inline std::array<std::string,64> aliases;inline std::array<bool,64> live{};
inline void slot(std::uint64_t address,std::uint64_t value){auto p=reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(address));for(unsigned i=0;i!=8;++i)p[i]=static_cast<unsigned char>(value>>(8*i));}
inline std::uint32_t abi(){return 1;}
inline std::uint32_t open(std::uint32_t,std::uint64_t p,std::uint64_t n,std::uint64_t out){
 ++opens;if(deny)return 13;std::string alias(reinterpret_cast<char const*>(static_cast<std::uintptr_t>(p)),static_cast<std::size_t>(n));if(log_deny&&alias.find("log")!=std::string::npos)return 13;
 if(next>=live.size())return 90;auto id=next++;aliases[id]=alias;live[id]=true;slot(out,id);return 0;
}
inline std::uint32_t close(std::uint64_t id){if(!id||id>=live.size()||!live[id])return 9;++closes;live[id]=false;return 0;}
inline std::uint32_t subscribe(std::uint64_t id){return id<live.size()&&live[id]?0:9;}
inline std::uint32_t publish(std::uint64_t id,std::uint32_t,std::uint64_t data,std::uint64_t n){
 if(id>=live.size()||!live[id])return 9;++publishes;if(publish_fail)return 110;auto p=reinterpret_cast<unsigned char const*>(static_cast<std::uintptr_t>(data));bytes.assign(p,p+n);return 0;
}
inline std::uint32_t poll(std::uint64_t id,std::uint32_t,std::uint64_t out,std::uint64_t cap,std::uint64_t length){
 if(id>=live.size()||!live[id])return 9;++polls;if(bytes.empty())return 11;if(cap<bytes.size())return 90;
 auto p=reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(out));std::memcpy(p,bytes.data(),bytes.size());
 if(malformed&&bytes.size()>=4)uwvm_ros_store_u32(p,1000);
 slot(length,bytes.size());return 0;
}
inline std::uint32_t log(std::uint64_t id,std::uint32_t,std::uint64_t data,std::uint64_t n){
 if(id>=live.size()||!live[id])return 9;++logs;std::string text;if(!::uwvm_ros::decode_string(std::span<unsigned char const>{reinterpret_cast<unsigned char const*>(static_cast<std::uintptr_t>(data)),static_cast<std::size_t>(n)},text))return 22;output.push_back(text);return 0;
}
inline std::uint32_t time(std::uint64_t id,std::uint32_t,std::uint64_t,std::uint64_t,std::uint64_t out,std::uint64_t cap,std::uint64_t length){
 if(id>=live.size()||!live[id])return 9;++times;if(clock_fail)return 110;if(cap<8)return 90;slot(out,now);slot(length,8);return 0;
}
inline std::uint32_t step(void*){if(steps++)return 125;now=500000000;return 0;}
inline roswasmc_backend const* backend(){static roswasmc_backend value=[] {roswasmc_backend b{};b.version=2;b.abi_version=abi;b.open=open;b.close=close;b.publish=publish;b.subscribe=subscribe;b.poll=poll;b.log=log;b.request[ROSWASMC_TIME_READ_STEADY]=time;return b;}();return &value;}
inline roswasm_rclcpp::TopicBinding topics[]{
 {"minimal_publisher","topic","compat-publish",roswasm_rclcpp::Role::publish,10,1028},
 {"minimal_subscriber","topic","compat-subscribe",roswasm_rclcpp::Role::subscribe,10,1028}};
inline roswasm_rclcpp::NodeBinding const nodes[]{
 {"minimal_publisher","compat-steady","compat-pub-log"},{"minimal_subscriber","","compat-sub-log"}};
inline void reset(){now=0;next=1;steps=opens=closes=publishes=polls=logs=times=0;deny=malformed=clock_fail=log_deny=publish_fail=false;live.fill(false);output.clear();topics[0].keep_last_depth=10;}
inline bool empty(){for(auto value:live)if(value)return false;return true;}
}
namespace roswasm_rclcpp {inline Selection const* selected_configuration()noexcept{static Selection const s{bridge::backend(),bridge::topics,bridge::nodes,bridge::step,nullptr,4};return &s;}}
