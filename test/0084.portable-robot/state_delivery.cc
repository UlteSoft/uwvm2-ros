// SPDX-License-Identifier: Apache-2.0
// Component checks with synthetic native state, not official simulation evidence.
#include <uwvm2/imported/ros/portable/runtime_audit.h>
#include <cstdio>
#include <cstdlib>
namespace p=uwvm2::imported::ros::portable;
namespace a=p::runtime_audit;
static void check(bool condition,char const* message)
{ if(!condition) { std::fprintf(stderr,"FAIL: %s\n",message);std::exit(1); } }
static std::uint64_t now()
{ return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()); }
int main()
{
    p::joint_deployment d{};d.mapping.epoch=11;d.mapping.count=2;d.policy_epoch=12;d.native_generation=13;
    d.mapping.joints[0]={-1000000,1000000,10000,0};d.mapping.joints[1]=d.mapping.joints[0];
    d.wasm_sha256.fill(1);d.adapter_sha256.fill(2);d.policy_sha256.fill(3);d.native_provenance.fill(4);
    p::state state{};state.epoch=11;state.sequence=40;state.count=2;
    state.observed_ns=now();state.valid_until_ns=state.observed_ns+250000000;state.positions[0]=100;state.positions[1]=-100;
    std::array<std::uint8_t,96> bytes{};check(p::encode(state,bytes).status==p::error::ok,"state encoding");
    int owner{};a::channel channel;
    check(!channel.emit_state_delivery({},d,1,4096,bytes)&&channel.attempts()==0,"disabled is inert");
    check(channel.configure("0123456789abcdef0123456789abcdef"),"configure actual channel");
    auto before=channel.enter_call(&owner,42,d.wasm_sha256,3,&d,true);
    check(channel.emit_state_delivery(before,d,7,4096,bytes),"baseline state delivery");
    state.sequence=41;state.positions[0]+=1000;state.positions[1]+=1000;
    state.observed_ns=now();state.valid_until_ns=state.observed_ns+250000000;
    check(p::encode(state,bytes).status==p::error::ok,"final state encoding");
    p::receipt result{11,1,41,state.observed_ns,p::outcome::succeeded,0,{}};result.command_sha256.fill(5);
    std::array<std::uint8_t,16> uuid{};uuid.fill(6);
    auto after=channel.enter_call(&owner,42,d.wasm_sha256,3,&d,true);
    check(channel.emit_state_delivery(after,d,7,4096,bytes,&result,&uuid),"final command-correlated state delivery");
    auto refused=after;refused.allowed=false;
    check(!channel.emit_state_delivery(refused,d,7,4096,bytes),"denial cannot emit delivery");
    refused=after;refused.function_index=4;
    check(!channel.emit_state_delivery(refused,d,7,4096,bytes),"other import cannot claim state copy");
    check(!channel.emit_state_delivery(after,d,7,UINT64_MAX,bytes),"overflow rejected");
    check(!channel.emit_state_delivery(after,d,7,4096,bytes,&result),"command without UUID rejected");
    auto malformed=bytes;malformed[0]=0;
    check(!channel.emit_state_delivery(after,d,7,4096,malformed),"malformed response rejected");
    state.observed_ns=1;state.valid_until_ns=2;check(p::encode(state,bytes).status==p::error::ok,"expired encoding");
    check(!channel.emit_state_delivery(after,d,7,4096,bytes),"expired state rejected");
    check(channel.attempts()==8&&channel.dropped()==6,"invalid inputs consume bounded diagnostic quota only");
    check(!channel.emit_state_delivery(after,d,7,4096,bytes)&&channel.attempts()==8,"quota does not grow");
    std::fprintf(stderr,"PASS state delivery component; real guest-memory execution remains separate\n");
}
