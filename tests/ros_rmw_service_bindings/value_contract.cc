/** Pure VALUE contracts, authored NOT RUN. The labels/native identity strings
 * are explicitly synthetic test data, not a fake native TypeSupport, current
 * granted capability or native/RMW/Host acceptance. No effects are implemented.
 * Checks stay active in Release. Actual source allocation and standard library
 * behavior still require a separately admitted real build/finite test owner.
 */
#include <uwvm2/imported/ros/common/rmw_service_bindings.h>
#include <cstdio>
#include <cstdint>
namespace ros=uwvm2::imported::ros;
namespace wire=ros::ipc::rmw_endpoint;
using history=ros::rmw_service_bindings;using role=ros::rmw_service_role;
static unsigned checks{},failures{};
#define CHECK(c) do{++checks;if(!(c)){++failures;std::fprintf(stderr,"FAIL_SERVICE_BINDINGS line=%u\n",unsigned(__LINE__));}}while(false)
static ros::capability_spec primary(bool server=false)
{
 ros::capability_spec c;c.alias=server?"advertise":"call";c.operation=server?"service.advertise":"service.call";
 c.resource="/explicit_value_fixture";c.type="fixture_msgs/srv/OnlySyntheticValue";c.type_identity="synthetic-not-native-proof";
 for(auto k:{"qos.reliability","qos.durability","qos.history","qos.depth","qos.deadline","qos.lifespan","qos.liveliness","qos.liveliness-lease-duration"})c.limits[k]="same-explicit-value";
 return c;
}
static ros::capability_spec auxiliary(bool server=false)
{
 if(server){auto c=primary(true);c.alias="respond";c.operation="service.respond";return c;}
 ros::capability_spec c;c.alias="observer";c.operation="event.observe";c.resource="/explicit_value_fixture";
 c.type=ros::ipc::event_type;c.type_identity=ros::ipc::event_type_identity;
 c.limits={{"event-source-alias","call"},{"event-kinds","service-response"}};return c;
}
static wire::binding owner(std::uint64_t low=1,std::uint64_t session=31)
{return {2,UINT64_C(0xf123456700000000)|low,17,23,session};}
static std::uint64_t peer(){return UINT64_C(0xc123456780000009);}
static void client_response_only()
{auto c=primary(),a=auxiliary();CHECK(ros::service_endpoint_pair(c,a,role::client)==0);std::uint32_t mask=999;CHECK(ros::service_observer_kinds(a,mask)==0&&mask==ros::ipc::event_bit(ros::ipc::event_kind::service_response));CHECK(a.limits.at("event-kinds")=="service-response");}
static void explicit_optional_control_masks()
{auto c=primary(),a=auxiliary();for(auto text:{"service-response,resource-error","service-response,resource-closed","service-response,resource-error,resource-closed"}){a.limits["event-kinds"]=text;CHECK(ros::service_endpoint_pair(c,a,role::client)==0&&a.limits.at("event-kinds")==text);}}
static void no_other_or_implicit_event_kinds()
{auto c=primary(),a=auxiliary();for(auto text:{"resource-error","resource-closed","topic-message,service-response","service-request,service-response"}){a.limits["event-kinds"]=text;CHECK(ros::service_endpoint_pair(c,a,role::client)==13);}a.limits["event-kinds"]="service-response,service-response";std::uint32_t unchanged=123;CHECK(ros::service_observer_kinds(a,unchanged)==22&&unchanged==123);a.limits.erase("event-kinds");CHECK(ros::service_endpoint_pair(c,a,role::client)==22);}
static void observer_operation_and_own_type()
{auto c=primary(),a=auxiliary();a.operation="event.poll";CHECK(ros::service_endpoint_pair(c,a,role::client)==13);a=auxiliary();a.type=c.type;CHECK(ros::service_endpoint_pair(c,a,role::client)==13);a=auxiliary();a.type_identity=c.type_identity;CHECK(ros::service_endpoint_pair(c,a,role::client)==13);}
static void no_observer_native_qos_fabrication()
{auto c=primary(),a=auxiliary();CHECK(a.limits.size()==2&&a.type!=c.type&&ros::service_endpoint_pair(c,a,role::client)==0);c.type="another_fixture_msgs/srv/SyntheticValue";c.type_identity="another-synthetic-value-not-native";CHECK(ros::service_endpoint_pair(c,a,role::client)==0);}
static void exact_source_alias_resource()
{auto c=primary(),a=auxiliary();a.limits["event-source-alias"]="other";CHECK(ros::service_endpoint_pair(c,a,role::client)==13);a=auxiliary();a.resource="/other";CHECK(ros::service_endpoint_pair(c,a,role::client)==13);a=auxiliary();a.alias=c.alias;CHECK(ros::service_endpoint_pair(c,a,role::client)==22);c=primary();c.alias="..";CHECK(ros::service_endpoint_pair(c,auxiliary(),role::client)==22);}
static void complete_server_pair()
{auto c=primary(true),a=auxiliary(true);CHECK(ros::service_endpoint_pair(c,a,role::service)==0);a.type="other/srv/Type";CHECK(ros::service_endpoint_pair(c,a,role::service)==13);a=auxiliary(true);a.type_identity="other";CHECK(ros::service_endpoint_pair(c,a,role::service)==13);a=auxiliary(true);a.operation="service.call";CHECK(ros::service_endpoint_pair(c,a,role::service)==13);}
static void all_eight_original_qos_values()
{auto c=primary(true);for(auto k:{"qos.reliability","qos.durability","qos.history","qos.depth","qos.deadline","qos.lifespan","qos.liveliness","qos.liveliness-lease-duration"}){auto a=auxiliary(true);a.limits.erase(k);CHECK(ros::service_endpoint_pair(c,a,role::service)==13);a=auxiliary(true);a.limits[k]="different";CHECK(ros::service_endpoint_pair(c,a,role::service)==13);}}
static void explicit_optional_codec_presence()
{auto c=primary(true),a=auxiliary(true);c.limits["adapter-codec-identity"]="explicit-synthetic-codec";CHECK(ros::service_endpoint_pair(c,a,role::service)==13);a.limits["adapter-codec-identity"]="different";CHECK(ros::service_endpoint_pair(c,a,role::service)==13);a.limits["adapter-codec-identity"]=c.limits.at("adapter-codec-identity");CHECK(ros::service_endpoint_pair(c,a,role::service)==0);c.limits["adapter-codec-identity"]="";a.limits["adapter-codec-identity"]="";CHECK(ros::service_endpoint_pair(c,a,role::service)==13);}
static void no_default_unknown_or_malformed_handles()
{history h;history::entry sentinel;history::entry const*out=&sentinel;auto c=primary(),a=auxiliary();CHECK(ros::service_endpoint_pair(c,a,role::publisher)==22);CHECK(h.reserve(owner(),role::client,owner().source,c,a,out)==22&&out==&sentinel&&h.rows().empty());CHECK(h.reserve(owner(),role::client,9,c,a,out)==22&&h.rows().empty());auto bad=owner();bad.backend=1;CHECK(h.reserve(bad,role::client,peer(),c,a,out)==22&&h.rows().empty());}
static void once_only_and_complete_wrong_binding()
{history h;history::entry const*out=nullptr;auto c=primary(),a=auxiliary();CHECK(h.reserve(owner(),role::client,peer(),c,a,out)==0&&out&&h.rows().size()==1);auto saved=out;CHECK(h.reserve(owner(),role::client,peer(),c,a,out)==17&&out==saved);CHECK(h.reserve(owner(),role::client,peer()+1,c,a,out)==13);auto other=owner();++other.session;CHECK(h.reserve(other,role::client,peer(),c,a,out)==13);auto server=primary(true),respond=auxiliary(true);CHECK(h.reserve(owner(),role::service,peer(),server,respond,out)==13);CHECK(h.rows().size()==1);}
static void lookup_preserves_failed_output()
{history h;history::entry sentinel;history::entry const*out=&sentinel;CHECK(h.lookup(owner(),role::client,out)==9&&out==&sentinel);CHECK(h.reserve(owner(),role::client,peer(),primary(),auxiliary(),out)==0);auto saved=out;auto bad=owner();++bad.generation;CHECK(h.lookup(bad,role::client,out)==13&&out==saved);CHECK(h.find_live(owner(),role::service,out)==13&&out==saved);CHECK(h.find_live(owner(),role::client,out)==0&&out==saved);}
static void retired_rows_remain_and_first_reason_wins()
{history h;history::entry const*out=nullptr;CHECK(h.reserve(owner(),role::client,peer(),primary(),auxiliary(),out)==0);auto saved=out;CHECK(h.retire(owner(),role::client,107)==0);CHECK(h.lookup(owner(),role::client,out)==0&&out==saved&&out->retired&&out->retirement_reason==107);CHECK(h.find_live(owner(),role::client,out)==125&&out==saved);CHECK(h.reserve(owner(),role::client,peer(),primary(),auxiliary(),out)==125);CHECK(h.retire(owner(),role::client,5)==125&&out->retirement_reason==107);auto wrong=owner();++wrong.session;CHECK(h.lookup(wrong,role::client,out)==13);}
static void both_full_handles_and_saved_session()
{history h;history::entry const*out=nullptr;CHECK(h.reserve(owner(),role::client,peer(),primary(),auxiliary(),out)==0);auto domain=owner();domain.source=0;CHECK(history::matches_peer(*out,domain,owner().source,"call",false));CHECK(history::matches_peer(*out,domain,peer(),"observer",false));CHECK(!history::matches_peer(*out,domain,peer(),"call",false));CHECK(!history::matches_peer(*out,domain,9,"observer",false));auto disconnected=domain;disconnected.session=0;CHECK(!history::matches_peer(*out,disconnected,peer(),"observer",false));auto other=domain;++other.generation;CHECK(!history::matches_peer(*out,other,peer(),"observer",false));CHECK(h.retire(owner(),role::client,107)==0&&history::matches_peer(*out,domain,peer(),"observer",false));}
static void either_alias_revocation_without_handle()
{history h;history::entry const*out=nullptr;CHECK(h.reserve(owner(),role::client,peer(),primary(),auxiliary(),out)==0);auto domain=owner();domain.source=0;CHECK(history::matches_peer(*out,domain,0,"call",true));CHECK(history::matches_peer(*out,domain,0,"observer",true));CHECK(!history::matches_peer(*out,domain,0,"other",true));auto wrong=domain;++wrong.session;CHECK(!history::matches_peer(*out,wrong,0,"observer",true));CHECK(h.retire_matching(domain,0,"observer",true,125)==1);CHECK(h.retire_matching(domain,0,"observer",true,107)==0&&out->retirement_reason==125);}
static void sweep_shared_peer_exact_domains()
{history h;history::entry const*out=nullptr;CHECK(h.reserve(owner(1),role::client,peer(),primary(),auxiliary(),out)==0);CHECK(h.reserve(owner(2),role::client,peer(),primary(),auxiliary(),out)==0);CHECK(h.reserve(owner(3,32),role::client,peer(),primary(),auxiliary(),out)==0);CHECK(h.retire_matching(owner(),peer(),"observer",false,107)==2);CHECK(h.find_live(owner(1),role::client,out)==125);CHECK(h.find_live(owner(3,32),role::client,out)==0);CHECK(h.rows().size()==3);}
static void finite_capacity_never_evicts()
{history h{1};history::entry const*out=nullptr;CHECK(h.reserve(owner(),role::client,peer(),primary(),auxiliary(),out)==0);CHECK(h.retire(owner(),role::client,107)==0);CHECK(h.reserve(owner(2),role::client,peer(),primary(),auxiliary(),out)==11&&h.rows().size()==1);history invalid{0};CHECK(invalid.reserve(owner(),role::client,peer(),primary(),auxiliary(),out)==22);history oversized{history::maximum_entries+1};CHECK(oversized.capacity()==0);}
static void seal_keeps_cleanup_rows()
{history h;history::entry const*out=nullptr;CHECK(h.reserve(owner(),role::client,peer(),primary(),auxiliary(),out)==0);CHECK(h.retire(owner(),role::client,5)==0);CHECK(h.reserve(owner(2),role::client,peer(),primary(),auxiliary(),out)==0);h.seal(107);CHECK(h.sealed()&&h.lookup(owner(),role::client,out)==0&&out->retirement_reason==5);CHECK(h.lookup(owner(2),role::client,out)==0&&out->retirement_reason==107);CHECK(h.find_live(owner(2),role::client,out)==125);CHECK(h.reserve(owner(3),role::client,peer(),primary(),auxiliary(),out)==125&&h.rows().size()==2);}
int main()
{
 client_response_only();explicit_optional_control_masks();no_other_or_implicit_event_kinds();observer_operation_and_own_type();no_observer_native_qos_fabrication();exact_source_alias_resource();complete_server_pair();all_eight_original_qos_values();explicit_optional_codec_presence();no_default_unknown_or_malformed_handles();once_only_and_complete_wrong_binding();lookup_preserves_failed_output();retired_rows_remain_and_first_reason_wins();both_full_handles_and_saved_session();either_alias_revocation_without_handle();sweep_shared_peer_exact_domains();finite_capacity_never_evicts();seal_keeps_cleanup_rows();
 std::printf("%s_SERVICE_BINDINGS_VALUE_CONTRACT groups=18 checks=%u failures=%u\n",failures?"FAIL":"PASS",checks,failures);return failures?1:0;
}
