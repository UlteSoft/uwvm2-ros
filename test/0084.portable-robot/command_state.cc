// SPDX-License-Identifier: Apache-2.0
// Deterministic component observations only. No real native result is claimed.
#include <uwvm2/imported/ros/portable/command_state.h>
#include <cstdio>
#include <cstdlib>
#include <type_traits>

namespace p = uwvm2::imported::ros::portable;
using journal = p::command_state;
static unsigned checks{};
static unsigned owner_token{},other_owner_token{};
static void check(bool value,char const* name)
{
    ++checks;
    if(!value) { ::std::fprintf(stderr,"FAIL %u %s\n",checks,name);::std::exit(1); }
}
struct fixture
{
    p::operation_binding binding{};
    p::descriptor mapping{};
    p::state sample{},confirmation{};
    p::command request{};
    p::digest hash{};
    p::terminal_observation terminal{91,300,p::native_terminal::succeeded,0};
    fixture()
    {
        binding.owner={&owner_token,4,{}};binding.owner.wasm_sha256[0]=1;
        binding.mapping_epoch=7;binding.policy_epoch=8;binding.native_generation=9;
        binding.adapter_sha256[0]=2;binding.mapping_sha256[0]=3;binding.native_provenance[0]=4;
        mapping.epoch=7;mapping.rights=15;mapping.count=1;mapping.mapping=binding.mapping_sha256;
        mapping.joints[0]={-1000000,1000000,10000,100};
        sample.epoch=7;sample.sequence=10;sample.observed_ns=100;sample.valid_until_ns=1000;sample.count=1;
        request.epoch=7;request.id=1;request.based_on_sequence=10;request.duration_ns=1000000000;
        request.count=1;request.mapping=mapping.mapping;request.targets[0]=5000;hash[0]=5;
        confirmation=sample;confirmation.sequence=11;confirmation.observed_ns=350;
        confirmation.valid_until_ns=600;confirmation.positions[0]=5000;
    }
    void reserve(journal& j) const
    { check(j.reserve(binding,request,mapping,sample,hash,150,500)==p::error::ok,"reserve one bounded step"); }
    void accepted(journal& j) const
    { reserve(j);check(j.accepted(binding,91,200)==p::error::ok,"actual acceptance observation"); }
};
static_assert(!::std::is_copy_constructible_v<journal> && !::std::is_move_constructible_v<journal>);
int main()
{
    fixture f;
    {
        journal j;p::receipt out{};out.command_id=99;
        check(j.snapshot(f.binding,160,out)==p::error::bad_handle && out.command_id==99,"empty journal preserves output");
        f.reserve(j);check(j.snapshot(f.binding,160,out)==p::error::no_fresh_state && out.command_id==99,"reservation is not acceptance");
        check(j.accepted(f.binding,91,200)==p::error::ok,"goal accepted");
        check(j.snapshot(f.binding,220,out)==p::error::ok && out.result==p::outcome::accepted && out.command_sha256==f.hash,"acceptance is not completion");
        check(j.finish(f.binding,f.terminal,f.confirmation,400)==p::error::ok,"native result plus fresh confirmed state");
        check(j.snapshot(f.binding,800,out)==p::error::ok && out.result==p::outcome::succeeded && out.observed_sequence==11 && out.command_sha256==f.hash,"terminal receipt remains bound");
        auto different=f.request;different.id=2;
        check(j.reserve(f.binding,different,f.mapping,f.sample,f.hash,500,500)==p::error::bad_handle,"second ID cannot replay owner generation");
        check(j.close(f.binding.owner)==p::error::ok && j.consumed() && j.closed(),"close preserves consumption");
        check(j.reserve(f.binding,f.request,f.mapping,f.sample,f.hash,500,500)==p::error::bad_handle,"close does not reset journal");
    }
    for(unsigned field{};field!=10;++field)
    {
        journal j;f.accepted(j);auto binding=f.binding;
        switch(field)
        {
            case 0:binding.owner.instance=&other_owner_token;break;
            case 1:++binding.owner.generation;break;
            case 2:binding.owner.wasm_sha256[0]^=1;break;
            case 3:++binding.mapping_epoch;break;
            case 4:++binding.policy_epoch;break;
            case 5:++binding.native_generation;break;
            case 6:binding.adapter_sha256[0]^=1;break;
            case 7:binding.mapping_sha256[0]^=1;break;
            default:binding.native_provenance[field-8]^=1;break;
        }
        p::receipt out{};out.command_id=99;auto expected=field<3 ? p::error::bad_handle : p::error::stale;
        check(j.snapshot(binding,400,out)==expected && out.command_id==99,"each independent binding field is retained");
        check(j.finish(binding,f.terminal,f.confirmation,400)==expected,"changed binding cannot complete");
    }
    for(auto offset : {-101,-100,0,100,101})
    {
        journal j;f.accepted(j);auto state=f.confirmation;state.positions[0]+=offset;
        auto expected=(offset<-100 || offset>100) ? p::error::no_fresh_state : p::error::ok;
        check(j.finish(f.binding,f.terminal,state,400)==expected,"confirmation tolerance boundary");
    }
    for(unsigned field{};field!=8;++field)
    {
        journal j;f.accepted(j);auto state=f.confirmation;auto terminal=f.terminal;p::error expected{};
        switch(field)
        {
            case 0:state.sequence=10;expected=p::error::no_fresh_state;break;
            case 1:state.observed_ns=299;expected=p::error::no_fresh_state;break;
            case 2:state.observed_ns=401;expected=p::error::no_fresh_state;break;
            case 3:state.valid_until_ns=400;expected=p::error::no_fresh_state;break;
            case 4:++state.epoch;expected=p::error::stale;break;
            case 5:++terminal.goal_id;expected=p::error::malformed;break;
            case 6:terminal.status=1;expected=p::error::malformed;break;
            default:terminal.observed_ns=199;expected=p::error::malformed;break;
        }
        check(j.finish(f.binding,terminal,state,400)==expected,"native and state witnesses must agree");
        check(j.current_phase()==journal::phase::accepted,"invalid witness is never success");
    }
    for(auto kind : {p::native_terminal::rejected,p::native_terminal::cancelled})
    {
        journal j;f.accepted(j);auto terminal=f.terminal;terminal.result=kind;terminal.status=-5;
        check(j.finish(f.binding,terminal,{},400)==p::error::ok,"native negative terminal needs no invented success state");
        p::receipt out{};check(j.snapshot(f.binding,400,out)==p::error::ok && out.result==(kind==p::native_terminal::rejected ? p::outcome::rejected : p::outcome::cancelled) && out.native_status==-5,"negative terminal preserved");
    }
    {
        journal j;f.accepted(j);p::receipt out{};out.command_id=99;
        check(j.snapshot(f.binding,650,out)==p::error::deadline && out.command_id==99,"exact deadline is indeterminate without output");
        check(j.finish(f.binding,f.terminal,f.confirmation,651)==p::error::bad_handle,"late success cannot undo deadline");
        check(j.reserve(f.binding,f.request,f.mapping,f.sample,f.hash,700,500)==p::error::bad_handle,"deadline never resends");
    }
    {
        journal j;f.reserve(j);check(j.accepted(f.binding,91,650)==p::error::deadline,"late acceptance remains uncertain");
        check(j.current_phase()==journal::phase::indeterminate && j.consumed(),"late accepted goal remains consumed");
    }
    {
        journal j;f.reserve(j);check(j.mark_indeterminate(f.binding)==p::error::deadline,"transport loss after possible send");
        check(j.reserve(f.binding,f.request,f.mapping,f.sample,f.hash,200,500)==p::error::bad_handle,"transport loss never resets reservation");
    }
    {
        journal j;f.accepted(j);auto other=f.binding.owner;other.instance=&other_owner_token;
        check(j.revoke(other)==p::error::bad_handle && j.close(other)==p::error::bad_handle,"foreign cleanup cannot select owner");
        check(j.revoke(f.binding.owner)==p::error::ok,"native owner retirement");
        p::receipt out{};out.command_id=99;
        check(j.snapshot(f.binding,400,out)==p::error::bad_handle && out.command_id==99,"old matching binding cannot restore revoked state");
        check(j.close(f.binding.owner)==p::error::ok,"owned cleanup after revocation");
        check(j.close(f.binding.owner)==p::error::bad_handle,"duplicate close refused");
    }
    for(auto wait : {0ULL,2000000001ULL,~0ULL})
    {
        journal j;check(j.reserve(f.binding,f.request,f.mapping,f.sample,f.hash,150,wait)==p::error::malformed && !j.consumed(),"invalid deadline refuses before reservation");
    }
    {
        journal j;f.reserve(j);check(j.accepted(f.binding,91,149)==p::error::malformed && j.current_phase()==journal::phase::indeterminate,"backwards clock never creates acceptance");
    }
    ::std::printf("{\"passed\":true,\"checks\":%u,\"scope\":\"portable command journal data contracts; no actual native endpoint\"}\n",checks);
}
