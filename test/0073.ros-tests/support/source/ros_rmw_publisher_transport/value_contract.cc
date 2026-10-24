/** Independent value/ownership contracts, with no fake ROS/Runtime headers.
 * Receipts below are explicit logical fixtures, never actual native evidence.
 * These tests establish neither native entity lifetime nor Host permissions.
 */
#include <uwvm2/imported/ros/common/rmw_endpoint_history.h>
#include <uwvm2/imported/ros/ipc/rmw_publish.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
namespace core=uwvm2::imported::ros;
namespace wire=core::ipc::rmw_endpoint;
namespace publication=core::ipc::rmw_publish;
using history=core::rmw_endpoint_history;
static void require(bool value){if(!value){std::abort();}}
static wire::binding owner()
{return {2,UINT64_C(0xf1234567a1234567),UINT64_C(0xff11223344556677),UINT64_C(0xaa11223344556677),UINT64_C(0xbb11223344556677)};}
static wire::packet request()
{wire::packet p{};p.owner=owner();return p;}
static wire::packet actual(wire::packet p=request())
{
    p.incarnation=UINT64_C(0xfd11223344556677);p.profiles=1;p.gid_size=24;
    p.actual[0]={1,1,2,1,4,{9223372036,854775807},{0,0},{9223372036,854775807},false};return p;
}
static void reserve(history& h,wire::packet const& p=request(),std::size_t maximum=1)
{history::entry const* row{};require(h.reserve(p,"publisher",maximum,row)==0&&row&&row->state==history::phase::reserved);}
static void activate(history& h,wire::packet const& p=actual())
{require(h.activate(p)==0);}
static void envelope_exact_known_bytes()
{
    auto a{actual()};publication::header h{a.owner,a.incarnation,3};
    std::array<std::uint8_t,publication::header_bytes> b{};require(publication::encode(h,b));
    require(b[0]=='U'&&b[1]=='M'&&b[2]=='R'&&b[3]=='P'&&b[4]==1&&b[6]==2&&b[8]==64);
    require(b[16]==0x67&&b[23]==0xf1&&b[48]==0x77&&b[55]==0xfd&&b[56]==3);
    std::vector<std::uint8_t> all(b.begin(),b.end());all.insert(all.end(),{0,0xff,7});
    publication::header decoded{};require(publication::decode(all.data(),all.size(),decoded));
    require(wire::same(decoded.owner,h.owner)&&decoded.incarnation==h.incarnation&&decoded.payload_bytes==3);
}
static void envelope_length_and_output_atomicity()
{
    auto a{actual()};std::array<std::uint8_t,publication::header_bytes>b{};
    require(publication::encode({a.owner,a.incarnation,0},b));publication::header out{};out.incarnation=73;
    require(!publication::decode(b.data(),63,out)&&out.incarnation==73);
    auto changed{b};changed[56]=1;require(!publication::decode(changed.data(),changed.size(),out)&&out.incarnation==73);
    require(!publication::decode(nullptr,b.size(),out)&&out.incarnation==73);
}
static void envelope_reserved_and_wrong_backend()
{
    auto a{actual()};std::array<std::uint8_t,publication::header_bytes>b{};
    require(publication::encode({a.owner,a.incarnation,0},b));publication::header out{};
    for(unsigned offset:{4u,6u,7u,8u,9u,15u})
    {auto changed{b};changed[offset]=0xff;require(!publication::decode(changed.data(),changed.size(),out));}
}
static void envelope_full_handle_and_zero_logical_body()
{
    auto a{actual()};std::array<std::uint8_t,publication::header_bytes>b{};
    require(publication::encode({a.owner,a.incarnation,0},b));publication::header out{};
    require(publication::decode(b.data(),b.size(),out)&&out.payload_bytes==0);
    auto changed{a.owner};changed.source=UINT64_C(0x100000000);auto previous{b};
    require(!publication::encode({changed,a.incarnation,0},b)&&b==previous);
    changed.source=1;require(!publication::encode({changed,a.incarnation,0},b)&&b==previous);
}
static void envelope_maximum_and_wide_incarnation()
{
    auto a{actual()};std::array<std::uint8_t,publication::header_bytes>b{};
    require(publication::encode({a.owner,UINT64_MAX,0},b));publication::header out{};
    require(publication::decode(b.data(),b.size(),out)&&out.incarnation==UINT64_MAX);
    auto previous{b};require(!publication::encode({a.owner,0,0},b)&&b==previous);
    require(!publication::encode({a.owner,1,core::ipc::max_payload_bytes},b)&&b==previous);
}
static void reserve_is_not_activation()
{
    history h;reserve(h);history::entry const* row{};
    require(h.find(owner(),0,wire::role::publisher,row)==17&&h.active_count("publisher")==1);
    require(h.rows().begin()->second.observed.profiles==0);
}
static void activation_and_same_object_inspection()
{
    history h;reserve(h);activate(h);auto p{actual()};p.operation=wire::command::inspect;
    require(h.inspect(p)==0);history::entry const* row{};
    require(h.find(p.owner,p.incarnation,p.kind,row)==0&&row->observed.incarnation==p.incarnation);
}
static void active_and_reserved_never_reactivate()
{
    history h;reserve(h);history::entry const* row{};require(h.reserve(request(),"publisher",1,row)==17);
    activate(h);require(h.reserve(request(),"publisher",1,row)==17);
    require(h.activate(actual())==17&&h.active_count("publisher")==1);
}
static void failed_activation_is_a_tombstone()
{
    history h;reserve(h);h.abandon(owner().source,95);history::entry const* row{};
    require(h.reserve(request(),"publisher",1,row)==125&&h.active_count("publisher")==0);
    auto next{request()};++next.owner.source;require(h.reserve(next,"publisher",1,row)==0);
}
static void alias_count_released_exactly_once()
{
    history h;reserve(h);auto next{request()};++next.owner.source;history::entry const* row{};
    require(h.reserve(next,"publisher",1,row)==11&&h.rows().size()==1);
    h.abandon(owner().source,125);h.abandon(owner().source,107);
    require(h.active_count("publisher")==0&&h.reserve(next,"publisher",1,row)==0&&h.active_count("publisher")==1);
}
static void exact_binding_refuses_foreign_owner()
{
    history h;reserve(h);activate(h);auto p{actual()};history::entry const* out{};
    for(unsigned field=0;field<5;++field)
    {
        auto wrong{p.owner};if(field==0){++wrong.source;}else if(field==1){++wrong.instance;}
        else if(field==2){++wrong.generation;}else if(field==3){++wrong.session;}else{wrong.backend=1;}
        require(h.find(wrong,p.incarnation,p.kind,out)!=0);
    }
    require(h.find(p.owner,p.incarnation+1,p.kind,out)==13);
    require(h.find(p.owner,p.incarnation,wire::role::subscription,out)==13);
}
static void context_generation_never_rebinds()
{
    history h;reserve(h);auto next{request()};++next.owner.source;++next.owner.generation;history::entry const* out{};
    require(h.reserve(next,"publisher",2,out)==13&&h.rows().size()==1);
    next=request();++next.owner.source;++next.owner.instance;require(h.reserve(next,"publisher",2,out)==13);
}
static void genuine_zero_gid_and_extent_are_values()
{
    history h;reserve(h);activate(h);auto p{actual()};p.operation=wire::command::inspect;
    require(h.inspect(p)==0);p.gid[23]=1;require(h.inspect(p)==13);
    p.gid.fill(0);p.gid_size=16;require(h.inspect(p)==13);
    require(h.rows().begin()->second.observed.gid_size==24&&h.rows().begin()->second.observed.gid[23]==0);
}
static void actual_qos_drift_preserves_snapshot()
{
    history h;reserve(h);activate(h);auto p{actual()};p.operation=wire::command::inspect;
    ++p.actual[0].depth;require(h.inspect(p)==13);p=actual();p.operation=wire::command::inspect;
    p.actual[0].avoid_ros_namespace_conventions=true;require(h.inspect(p)==13);
    require(h.rows().begin()->second.observed.actual[0].depth==4&&!h.rows().begin()->second.observed.actual[0].avoid_ros_namespace_conventions);
}
static void retire_before_cleanup_and_once_only_claim()
{
    history h;reserve(h);activate(h);auto p{actual()};require(!h.claim_cleanup(p.owner.source));
    require(h.retire(p.owner,p.incarnation,p.kind)==0&&h.active_count("publisher")==0);
    require(h.claim_cleanup(p.owner.source)&&!h.claim_cleanup(p.owner.source));
    require(h.rows().begin()->second.cleanup_error==115);h.cleanup_result(p.owner.source,110);
    require(h.rows().begin()->second.cleanup_error==110&&h.retire(p.owner,p.incarnation,p.kind)==125);
}
static void abandoned_session_never_revives_old_source()
{
    history h;reserve(h);activate(h);auto p{actual()};h.retire_session(p.owner.session);history::entry const* out{};
    require(h.find(p.owner,p.incarnation,p.kind,out)==125);
    auto next{request()};++next.owner.source;++next.owner.session;require(h.reserve(next,"publisher",1,out)==0);
    auto stale{request()};++stale.owner.session;require(h.reserve(stale,"publisher",1,out)==13);
}
static void sealed_history_keeps_diagnostics_and_no_authority()
{
    history h;reserve(h);activate(h);h.seal();require(h.rows().size()==1&&h.active_count("publisher")==0);
    history::entry const* out{};auto next{request()};++next.owner.source;
    require(h.reserve(next,"publisher",1,out)==125&&h.reserve_native_incarnation()==0);
    require(h.find(owner(),actual().incarnation,wire::role::publisher,out)==125);
}
static void native_incarnations_do_not_reset_on_retirement()
{
    history h;auto first{h.reserve_native_incarnation()};require(first==1);
    reserve(h);activate(h);h.abandon(owner().source,125);require(h.reserve_native_incarnation()==2);
    h.retire_session(owner().session);require(h.reserve_native_incarnation()==3);
}
static void hard_history_bound_counts_tombstones()
{
    history h;history::entry const* row{};
    for(std::size_t i{};i<history::maximum_entries;++i)
    {
        auto p{request()};p.owner.source+=i;
        require(h.reserve(p,"publisher",1,row)==0);h.abandon(p.owner.source,95);
    }
    auto p{request()};p.owner.source+=history::maximum_entries;
    require(h.reserve(p,"publisher",1,row)==11&&h.rows().size()==history::maximum_entries&&h.active_count("publisher")==0);
}
struct group{char const*name;void(*run)();};
static group const groups[]={
    {"envelope-known-bytes",envelope_exact_known_bytes},{"envelope-atomicity",envelope_length_and_output_atomicity},
    {"envelope-reserved",envelope_reserved_and_wrong_backend},{"envelope-zero-body",envelope_full_handle_and_zero_logical_body},
    {"envelope-wide",envelope_maximum_and_wide_incarnation},{"reserve-not-active",reserve_is_not_activation},
    {"same-object-snapshot",activation_and_same_object_inspection},{"no-reactivation",active_and_reserved_never_reactivate},
    {"failed-tombstone",failed_activation_is_a_tombstone},{"active-count",alias_count_released_exactly_once},
    {"foreign-owner",exact_binding_refuses_foreign_owner},{"generation",context_generation_never_rebinds},
    {"zero-full-gid",genuine_zero_gid_and_extent_are_values},{"qos-drift",actual_qos_drift_preserves_snapshot},
    {"retire-once",retire_before_cleanup_and_once_only_claim},{"session-loss",abandoned_session_never_revives_old_source},
    {"sealed",sealed_history_keeps_diagnostics_and_no_authority},{"nonreuse",native_incarnations_do_not_reset_on_retirement},
    {"bounded-tombstones",hard_history_bound_counts_tombstones}};
int main(int argc,char**argv)
{
    if(argc!=2){return 2;}
    for(auto const& selected:groups)
    {if(std::strcmp(argv[1],selected.name)==0){selected.run();std::puts("PASS_RMW_PUBLISHER_VALUES_ONLY_NOT_NATIVE_ROS");return 0;}}
    return 2;
}
