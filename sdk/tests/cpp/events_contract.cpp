/** @file
 * @brief Actual C++ EventStream facade over explicit test-owned fake dispatch.
 * No Runtime/ROS acceptance is claimed. The fake returns exact envelopes and
 * cleanup statuses so ownership/sequence/byte invariants are exercised locally.
 */
#include <roswasm/roswasmcpp.hpp>
#include <cassert>
#include <cstring>
#include <unistd.h>
static unsigned closes,polls;static bool retired;
static void u64(unsigned char* p,uint64_t value){for(unsigned i=0;i<8;++i)p[i]=static_cast<unsigned char>(value>>(8*i));}
static uint32_t open_owner(uint32_t,uint64_t alias,uint64_t length,uint64_t slot){auto name=reinterpret_cast<char const*>(uintptr_t(alias));uint64_t id=length==3&&std::memcmp(name,"sub",3)==0?5:9;u64(reinterpret_cast<unsigned char*>(uintptr_t(slot)),id);return 0;}
static uint32_t close_owner(uint64_t handle){++closes;if(handle==5)retired=true;return 0;}
static uint32_t watch(uint64_t observer,uint64_t source,uint64_t correlation){return observer==9&&source==5&&!correlation?0:13;}
static uint32_t poll(uint64_t,uint32_t,uint64_t out,uint64_t capacity,uint64_t slot){
    ++polls;uint64_t size=retired?64:68;u64(reinterpret_cast<unsigned char*>(uintptr_t(slot)),size);if(capacity<size)return 90;
    auto bytes=reinterpret_cast<unsigned char*>(uintptr_t(out));std::memset(bytes,0,size);bytes[0]='U';bytes[1]='W';bytes[2]='R';bytes[3]='E';bytes[4]=1;bytes[6]=retired?7:1;bytes[8]=1;bytes[9]=retired?1:0;u64(bytes+16,5);u64(bytes+32,retired?2:1);u64(bytes+40,32);u64(bytes+48,64);u64(bytes+56,size-64);return 0;
}
static uint32_t ready(uint64_t,uint32_t,uint64_t output){std::memset(reinterpret_cast<void*>(uintptr_t(output)),0,32);return 0;}
static uint32_t submit(uint64_t,uint64_t,uint32_t,uint64_t,uint64_t,uint64_t slot){u64(reinterpret_cast<unsigned char*>(uintptr_t(slot)),17);return 0;}
int main(){
    roswasmc_backend backend{};backend.version=1;backend.open=open_owner;backend.close=close_owner;roswasmc_context context{&backend};
    roswasmcpp::Capability source{context,"sub"},observer{context,"events"};roswasmc_event_backend extension{1,watch,poll,ready,submit};
    roswasmcpp::EventStream events{observer,source,extension};assert(events.error()==0);unsigned char bytes[68];std::memset(bytes,0xa5,sizeof bytes);
    auto too_small=events.poll(std::span<unsigned char>{bytes,67});assert(too_small.result.error==90&&too_small.result.has_length&&too_small.result.length==68&&bytes[0]==0xa5);
    auto first=events.poll(bytes);assert(first.result.error==0&&first.header.source==5&&first.header.sequence==1&&first.header.instance==32);
    uint64_t request{};assert(events.submit({},request)==0&&request==17);
    assert(source.close()==0);auto last=events.poll(bytes);assert(last.result.error==0&&last.header.kind==ROSWASMC_RESOURCE_CLOSED&&last.header.sequence==2);
    assert(events.close_checked()==0&&closes==2);auto before=polls;assert(events.poll(bytes).result.error==9&&polls==before);
    static constexpr char marker[]="PASS_CPP_UNIFIED_EVENTS\n";assert(::write(1,marker,sizeof marker-1)==ssize_t(sizeof marker-1));
}
