/** Value/history contracts against real production headers. Logical receipt
 * fixtures are explicitly not native evidence. No copied Host/queue algorithm,
 * fake ROS headers, stub Native entities or Runtime execution appears here.
 */
#include <uwvm2/imported/ros/common/rmw_endpoint_history.h>
#include <cstdlib>
namespace core=uwvm2::imported::ros;
namespace wire=core::ipc::rmw_endpoint;
using history=core::rmw_endpoint_history;
static void require(bool value){if(!value){std::abort();}}
static wire::packet request(wire::role kind=wire::role::subscription)
{wire::packet p{};p.kind=kind;p.owner={2,UINT64_C(0xf1234567a1234567),41,43,47};return p;}
static wire::packet actual(wire::packet p)
{
    p.incarnation=UINT64_C(0xfb11223344556677);p.profiles=1;
    if(p.kind==wire::role::publisher){p.gid_size=24;}
    p.actual[0]={1,1,2,1,4,{9223372036,854775807},{0,0},{9223372036,854775807},false};return p;
}
static void activate(history& h,wire::packet p=request())
{history::entry const* row{};require(h.reserve(p,"topic",2,row)==0&&h.activate(actual(p))==0);}
static void independent_primary_cancel_retire()
{
    history h;activate(h);auto p{actual(request())};
    h.transfer_result(p.owner.source,90);h.cancel_result(p.owner.source,110);
    h.abandon(p.owner.source,110);require(h.claim_cleanup(p.owner.source));h.cleanup_result(p.owner.source,95);
    auto const& e{h.rows().begin()->second};
    require(e.last_transfer_error==90&&e.cancel_attempted&&e.last_cancel_error==110&&e.cleanup_error==95&&e.retirement_reason==110);
}
static void cancellation_survives_later_primary_observation()
{
    history h;activate(h);auto p{request()};h.cancel_result(p.owner.source,5);h.transfer_result(p.owner.source,0);
    auto const& e{h.rows().begin()->second};require(e.last_transfer_error==0&&e.last_cancel_error==5&&e.cancel_attempted);
    h.retire_session(p.owner.session,107);require(e.last_cancel_error==5&&e.state==history::phase::retired);
}
static void pending_cleanup_not_false_success()
{
    history h;activate(h);auto p{request()};h.transfer_result(p.owner.source,115);h.abandon(p.owner.source,13);
    require(h.claim_cleanup(p.owner.source)&&!h.claim_cleanup(p.owner.source));
    auto const& e{h.rows().begin()->second};require(e.last_transfer_error==115&&e.cleanup_error==115&&!e.cancel_attempted);
}
static void pending_cancel_survives_transport_retirement()
{
    history h;activate(h);auto p{request()};h.cancel_result(p.owner.source,0);
    h.transfer_result(p.owner.source,115);h.cancel_result(p.owner.source,115);
    h.retire_session(p.owner.session,13);auto const& e{h.rows().begin()->second};
    require(e.cancel_attempted&&e.last_cancel_error==115&&e.last_transfer_error==115&&e.retirement_reason==13);
}
static void unknown_diagnostics_do_not_mint_authority()
{
    history h;h.transfer_result(9,0);h.cancel_result(9,0);require(h.rows().empty());
    history::entry const* row{};require(h.find(request().owner,1,wire::role::subscription,row)==9);
}
static void same_handle_cannot_switch_native_role()
{
    history h;activate(h);auto p{request(wire::role::publisher)};history::entry const* row{};
    require(h.reserve(p,"topic",2,row)==13&&h.find(p.owner,actual(request()).incarnation,p.kind,row)==13);
    require(h.rows().size()==1&&h.active_count("topic")==1);
}
static void mixed_roles_share_session_drain()
{
    history h;activate(h);auto p{request(wire::role::publisher)};++p.owner.source;activate(h,p);
    h.retire_session(p.owner.session,107);require(h.active_count("topic")==0);
    for(auto const& item:h.rows()){require(item.second.state==history::phase::retired&&item.second.retirement_reason==107);}
}
static void retired_source_cannot_borrow_again()
{
    history h;activate(h);auto p{actual(request())};h.abandon(p.owner.source,125);history::entry const* row{};
    require(h.find(p.owner,p.incarnation,p.kind,row)==125&&h.reserve(request(),"topic",2,row)==125);
    require(h.rows().begin()->second.observed.incarnation==p.incarnation);
}
int main()
{
    independent_primary_cancel_retire();cancellation_survives_later_primary_observation();
    pending_cleanup_not_false_success();pending_cancel_survives_transport_retirement();unknown_diagnostics_do_not_mint_authority();
    same_handle_cannot_switch_native_role();mixed_roles_share_session_drain();retired_source_cannot_borrow_again();
}
