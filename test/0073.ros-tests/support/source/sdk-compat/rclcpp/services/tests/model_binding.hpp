/** PURE HOST MODEL. It tests the real SDK/adapter code, never ROS/native delivery. */
#pragma once
#include <array>
#include <bit>
#include <cstdint>
#include <vector>
#include <string>
#include <cstring>
namespace service_model {
/** Private model byte writer, using the actual common ABI u32 primitive twice.
 * It is NOT a new public SDK API. Byte addressing is explicit and unaligned
 * writes retain every bit of the actual8-byte little-endian ID/value.
 */
inline void store_u64(unsigned char* p,std::uint64_t value){
 uwvm_ros_store_u32(p,static_cast<std::uint32_t>(value));
 uwvm_ros_store_u32(p+4,static_cast<std::uint32_t>(value>>32));
}
struct Owner {std::uint64_t id{};std::string alias;bool live{};};
inline std::array<Owner,32> owners;inline std::uint64_t next_handle{1},next_request{1},source{},observer{},sequence{};
inline unsigned steps{},stop_after{UINT32_MAX};inline std::uint64_t clock_now{100};
inline unsigned opens{},closes{},submits{},sync_calls{},responds{},watches{},logs{};
inline std::uint32_t submit_error{},close_error{};inline std::string deny_alias;inline bool deny_all{};
inline std::vector<unsigned char> incoming,event,response,last_request;
inline void slot(std::uint64_t p,std::uint64_t v){store_u64(reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(p)),v);}
inline Owner* find(std::uint64_t id){for(auto& o:owners)if(o.live&&o.id==id)return &o;return nullptr;}
inline std::uint32_t abi(){return 1;}
inline std::uint32_t open(std::uint32_t,std::uint64_t p,std::uint64_t n,std::uint64_t out){
 ++opens;std::string name(reinterpret_cast<char const*>(static_cast<std::uintptr_t>(p)),static_cast<std::size_t>(n));if(deny_all||name==deny_alias)return 13;
 for(auto& o:owners)if(!o.live){o={next_handle++,name,true};slot(out,o.id);return 0;}return 90;
}
inline std::uint32_t close(std::uint64_t id){auto p=find(id);if(!p)return 9;p->live=false;++closes;return close_error;}
inline std::uint32_t advertise(std::uint64_t id){return find(id)?0:9;}
inline std::uint32_t poll(std::uint64_t id,std::uint32_t,std::uint64_t out,std::uint64_t cap,std::uint64_t len){
 if(!find(id))return 9;if(incoming.empty())return 11;if(cap<incoming.size())return 90;std::memcpy(reinterpret_cast<void*>(static_cast<std::uintptr_t>(out)),incoming.data(),incoming.size());slot(len,incoming.size());incoming.clear();return 0;
}
inline std::uint32_t sync(std::uint64_t,std::uint32_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t){++sync_calls;return 95;}
inline std::uint32_t respond(std::uint64_t id,std::uint32_t,std::uint64_t in,std::uint64_t n,std::uint64_t,std::uint64_t,std::uint64_t length_out){if(!find(id))return 9;++responds;auto p=reinterpret_cast<unsigned char const*>(static_cast<std::uintptr_t>(in));response.assign(p,p+n);slot(length_out,0);return 0;}
inline std::uint32_t time_read(std::uint64_t id,std::uint32_t,std::uint64_t,std::uint64_t,std::uint64_t out,std::uint64_t cap,std::uint64_t len){if(!find(id))return 9;if(cap!=8)return 90;slot(out,clock_now);slot(len,8);return 0;}
inline std::uint32_t log(std::uint64_t id,std::uint32_t,std::uint64_t,std::uint64_t){if(!find(id))return 9;++logs;return 0;}
inline std::uint32_t watch(std::uint64_t o,std::uint64_t s,std::uint64_t correlation){if(!find(o)||!find(s)||o==s||correlation)return 22;observer=o;source=s;++watches;return 0;}
inline std::uint32_t submit(std::uint64_t s,std::uint64_t o,std::uint32_t,std::uint64_t p,std::uint64_t n,std::uint64_t id){
 if(s!=source||o!=observer||!find(o)||!find(s))return 9;++submits;if(submit_error)return submit_error;if(n!=16)return 22;
 auto raw=reinterpret_cast<unsigned char const*>(static_cast<std::uintptr_t>(p));last_request.assign(raw,raw+n);slot(id,next_request++);return 0;
}
inline std::uint32_t event_poll(std::uint64_t o,std::uint32_t,std::uint64_t out,std::uint64_t cap,std::uint64_t len){
 if(o!=observer||!find(o))return 9;if(event.empty())return 11;slot(len,event.size());if(cap<event.size())return 90;
 std::memcpy(reinterpret_cast<void*>(static_cast<std::uintptr_t>(out)),event.data(),event.size());event.clear();return 0;
}
inline void queue(std::uint16_t kind,std::uint64_t id,std::int64_t value=42,std::uint32_t error=0,std::uint8_t flags=0){
 event.assign(kind==3?72:64,0);uwvm_ros_store_u32(event.data(),0x45525755u);event[4]=1;event[6]=static_cast<unsigned char>(kind);event[8]=2;event[9]=flags;uwvm_ros_store_u32(event.data()+12,error);
 store_u64(event.data()+16,source);store_u64(event.data()+24,id);store_u64(event.data()+32,++sequence);store_u64(event.data()+40,77);store_u64(event.data()+48,88);store_u64(event.data()+56,event.size()-64);if(kind==3)store_u64(event.data()+64,std::bit_cast<std::uint64_t>(value));
}
inline roswasmc_backend const* backend(){static roswasmc_backend b=[] {roswasmc_backend v{};v.version=2;v.abi_version=abi;v.open=open;v.close=close;v.poll=poll;v.advertise_service=advertise;v.log=log;v.request[ROSWASMC_TIME_READ_STEADY]=time_read;v.request[ROSWASMC_SERVICE_CALL]=sync;v.request[ROSWASMC_SERVICE_RESPOND]=respond;return v;}();return &b;}
inline roswasmc_event_backend const api{2,watch,event_poll,nullptr,submit};
inline roswasm_rclcpp::NodeBinding const nodes[]{{"minimal_service","","log"},{"minimal_client","","log"}};
inline roswasm_rclcpp::ServiceBinding rows[]{
 {"minimal_service","add_two_ints","example_interfaces/srv/AddTwoInts","official-default-service-profile",roswasm_rclcpp::ServiceRole::server,"","","advertise","respond",2},
 {"minimal_client","add_two_ints","example_interfaces/srv/AddTwoInts","official-default-service-profile",roswasm_rclcpp::ServiceRole::client,"call","observe","","",2}};
inline std::uint32_t step(void*){return steps++>=stop_after?125:0;}
inline void reset(){for(auto& p:owners)p={};next_handle=next_request=1;source=observer=sequence=0;steps=0;stop_after=UINT32_MAX;clock_now=100;opens=closes=submits=sync_calls=responds=watches=logs=0;submit_error=close_error=0;deny_all=false;deny_alias.clear();incoming.clear();event.clear();response.clear();last_request.clear();rows[1].type="example_interfaces/srv/AddTwoInts";}
inline bool empty(){for(auto const& p:owners)if(p.live)return false;return true;}
}
namespace roswasm_rclcpp {inline Selection const* selected_configuration()noexcept{static ServiceProfile const p{&service_model::api,service_model::rows};static Selection const s{service_model::backend(),{},service_model::nodes,service_model::step,nullptr,4,&p};return &s;}}
