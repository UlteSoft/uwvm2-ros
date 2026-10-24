/** Pure actual helper VALUE contracts, authored and NOT RUN.
 * Synthetic binding/header bytes are value fixtures, not native TypeSupport,
 * actual capabilities, Runtime caller identity or evidence of ROS acceptance.
 * No network, syscall provider, Native factory or substitute Runtime is used.
 */
#include <uwvm2/uwvm/imported/ros/rmw_service_state.h>
#include <cstdio>
namespace host=uwvm2::uwvm::imported::ros::host;
namespace ipc=uwvm2::imported::ros::ipc;
namespace srv=ipc::rmw_service;
static unsigned checks{},failures{};
#define CHECK(x) do{++checks;if(!(x)){++failures;std::fprintf(stderr,"FAIL_RMW_SERVICE_HOST_VALUE line=%u\n",unsigned(__LINE__));}}while(false)
static ipc::rmw_metadata::header response()
{
 ipc::rmw_metadata::header h{};h.kind=ipc::event_kind::service_response;
 h.owner={2,UINT64_C(0xf123456700000001),17,23,31};h.incarnation=41;h.sequence=43;h.correlation=47;
 h.flags=ipc::rmw_metadata::source_time|ipc::rmw_metadata::received_time|ipc::rmw_metadata::native_request_id;
 h.writer_size=16;h.source_timestamp=-7;h.received_timestamp=0;h.request_sequence=INT64_MIN;return h;
}
static void pending_is_not_success()
{host::rmw_service_journal j;j.api_result(115);CHECK(j.api_returned&&j.api_error==115&&j.first_primary==0);j.api_result(22);CHECK(j.api_error==22&&j.first_primary==22&&j.last_primary==22);}
static void preserve_first_and_last_primary()
{host::rmw_service_journal j;j.primary(0);j.primary(11);CHECK(j.first_primary==0);j.primary(13);j.primary(90);CHECK(j.first_primary==13&&j.last_primary==90);}
static void cleanup_cannot_overwrite_primary()
{host::rmw_service_journal j;j.api_result(22);j.cancelled(5);j.committed(107);j.cleaned(110);CHECK(j.first_primary==22&&j.last_primary==22&&j.cancel==5&&j.commit==107&&j.cleanup==110);}
static void every_cleanup_pending_slot_separate()
{host::rmw_service_journal j;j.cancelled(115);j.committed(115);j.cleaned(115);CHECK(j.cancel_attempted&&j.commit_attempted&&j.cleanup_attempted&&j.cancel==115&&j.commit==115&&j.cleanup==115);j.cancelled(0);CHECK(j.cancel==0&&j.commit==115&&j.cleanup==115);}
static void clear_borrow_preserves_history()
{host::rmw_service_journal j;j.pending_sequence=3;j.pending_reservation=5;j.last_sequence=3;j.last_reservation=5;j.last_committed=2;j.clear_borrow();CHECK(!j.pending_sequence&&!j.pending_reservation&&j.last_sequence==3&&j.last_reservation==5&&j.last_committed==2);}
static void only_peek_idle_without_visible_borrow()
{CHECK(host::rmw_service_values::peek_idle(11,true));CHECK(!host::rmw_service_values::peek_idle(11,false));for(auto e:{0u,5u,12u,13u,17u,22u,90u,95u,107u,110u,115u,125u})CHECK(!host::rmw_service_values::peek_idle(e,true));}
static void prefix_exact64_bits_and_restored()
{std::array<std::uint8_t,12> b{1,2,3,4,5,6,7,8,9,10,11,12};auto original=b;{host::rmw_service_values::ticket_prefix p{b.data(),UINT64_C(0xffeeddccbbaa9988)};CHECK(ipc::get(b.data(),8)==UINT64_C(0xffeeddccbbaa9988));CHECK(b[8]==9&&b[11]==12);}CHECK(b==original);}
static void prefix_restored_on_early_scope_return()
{std::array<std::uint8_t,8>b{1,2,3,4,5,6,7,8};auto old=b;auto reject=[&](){host::rmw_service_values::ticket_prefix p{b.data(),1};return 22;};CHECK(reject()==22&&b==old);}
static void logical_response_uses_same_identity()
{auto h=response();ipc::event_header e{};CHECK(host::rmw_service_values::event(h,e));CHECK(e.kind==h.kind&&e.source_handle==h.owner.source&&e.instance==h.owner.instance&&e.session==h.owner.session&&e.sequence==h.sequence&&e.correlation==h.correlation&&!e.flags&&!e.payload_bytes);CHECK(h.source_timestamp==-7&&h.request_sequence==INT64_MIN);}
static void no_missing_metadata_becomes_complete()
{auto h=response();h.flags&=~ipc::rmw_metadata::received_time;ipc::event_header e{};e.sequence=999;CHECK(!host::rmw_service_values::event(h,e)&&e.sequence==999);}
static void zero_guid_and_zero_time_are_actual_values()
{auto h=response();CHECK(ipc::rmw_metadata::complete(h));for(auto b:h.writer)CHECK(b==0);ipc::event_header e{};CHECK(host::rmw_service_values::event(h,e));}
static void request_topic_not_response_projection()
{auto h=response();ipc::event_header e{};e.sequence=999;h.kind=ipc::event_kind::service_request;CHECK(!host::rmw_service_values::event(h,e)&&e.sequence==999);h.kind=ipc::event_kind::topic_message;CHECK(!host::rmw_service_values::event(h,e));}
static void error_control_has_no_native_values()
{auto h=response();h.kind=ipc::event_kind::resource_error;h.flags=0;h.writer_size=0;h.source_timestamp=h.request_sequence=0;h.error=95;ipc::event_header e{};CHECK(host::rmw_service_values::event(h,e)&&e.error==95&&e.payload_bytes==0&&e.correlation==h.correlation);h.payload_bytes=1;CHECK(!host::rmw_service_values::event(h,e));}
static void closed_control_requires_real_retired_bit()
{auto h=response();h.kind=ipc::event_kind::resource_closed;h.flags=0;h.writer_size=0;h.source_timestamp=h.request_sequence=0;ipc::event_header e{};CHECK(!host::rmw_service_values::event(h,e));h.control_flags=ipc::event_source_retired;CHECK(host::rmw_service_values::event(h,e)&&e.flags==ipc::event_source_retired);}
static void malformed_native_header_keeps_output()
{auto h=response();h.incarnation=0;ipc::event_header e{};e.error=77;CHECK(!host::rmw_service_values::event(h,e)&&e.error==77);}
static void empty_request_not_absent_umrc_body()
{srv::control c{};c.owner=response().owner;c.incarnation=41;c.peer=UINT64_C(0xf123456700000002);c.operation=srv::command::submit;std::array<std::uint8_t,srv::header_bytes>b{};CHECK(srv::encode(c,false,b));srv::control out{};CHECK(srv::request(b.data(),b.size(),out)&&out.body_bytes==0);CHECK(!srv::request(b.data(),b.size()-1,out));}
static void available_independent_third_peer()
{srv::control c{};c.owner=response().owner;c.incarnation=41;c.peer=UINT64_C(0xf123456700000003);c.operation=srv::command::available;c.wait_ns=60000000000ULL;std::array<std::uint8_t,srv::header_bytes>b{};CHECK(srv::encode(c,false,b));c.peer=c.owner.source;CHECK(!srv::encode(c,false,b));}
static void ack_exact_sequence_and_nonce()
{srv::control c{};c.owner=response().owner;c.incarnation=41;c.peer=UINT64_C(0xf123456700000002);c.operation=srv::command::commit;c.sequence=43;c.reservation=59;auto r=c;std::array<std::uint8_t,srv::header_bytes>b{};CHECK(srv::encode(r,true,b));srv::control out{};CHECK(srv::associated_ack(b.data(),b.size(),c,out));++c.reservation;CHECK(!srv::associated_ack(b.data(),b.size(),c,out));}
static void service_bound_keys_are_actual_schema()
{uwvm2::imported::ros::capability_spec cap;cap.limits={{"max-request-bytes","16"},{"max-response-bytes","8"}};CHECK(cap.limits.at(host::rmw_service_values::send_limit_key(false))=="16");CHECK(cap.limits.at(host::rmw_service_values::send_limit_key(true))=="8");CHECK(!cap.limits.contains("max-message-bytes"));}
static void source_deadline_clamps_connection_entry()
{using namespace std::chrono;host::rmw_service_values::deadline entry{},out=entry+hours(1);CHECK(host::rmw_service_values::entry_deadline(entry,3000,50,out)==0&&out==entry+milliseconds(50));auto saved=out;CHECK(host::rmw_service_values::entry_deadline(entry,3000,0,out)==22&&out==saved);CHECK(host::rmw_service_values::entry_deadline(entry,60001,50,out)==22&&out==saved);}
static void responder_deadline_cannot_restart_budget()
{using namespace std::chrono;host::rmw_service_values::deadline entry{},out=entry+milliseconds(50);CHECK(host::rmw_service_values::clamp_deadline(entry,7,out)==0&&out==entry+milliseconds(7));CHECK(host::rmw_service_values::clamp_deadline(entry,20,out)==0&&out==entry+milliseconds(7));}
static void query_deadline_is_independent_and_finite()
{using namespace std::chrono;host::rmw_service_values::deadline entry{},out=entry+milliseconds(50);CHECK(host::rmw_service_values::clamp_deadline(entry,3,out)==0&&out==entry+milliseconds(3));auto old=out;CHECK(host::rmw_service_values::clamp_deadline(entry,0,out)==22&&out==old);}
static void rpc_zero_survives_host_copy_error()
{host::rmw_service_journal j;j.begin_rpc();CHECK(j.rpc_attempted&&!j.rpc_returned&&j.rpc_error==115);j.end_rpc(0);j.api_result(22);CHECK(j.rpc_returned&&j.rpc_error==0&&j.api_error==22&&j.first_primary==22);}
static void local_preflight_is_not_rpc_attempt()
{host::rmw_service_journal j;j.begin_api();j.begin_rpc();j.end_rpc(0);j.api_result(0);j.begin_api();j.api_result(90);CHECK(j.api_returned&&j.api_error==90&&!j.api_rpc_attempted&&j.rpc_attempted&&j.rpc_returned&&j.rpc_error==0);j.cancelled(0);j.cleaned(107);CHECK(!j.api_rpc_attempted&&j.api_error==90&&j.cancel==0&&j.cleanup==107&&j.rpc_error==0);}
static void actual_borrow_helper_compensates_allocation_fault()
{unsigned compensation=0;auto error=host::rmw_service_values::guarded_borrow([]()->std::uint32_t{throw std::bad_alloc{};},[&](std::uint32_t primary)noexcept{++compensation;return primary;});CHECK(error==12&&compensation==1);}
static void actual_borrow_helper_compensates_other_fault()
{unsigned compensation=0;auto error=host::rmw_service_values::guarded_borrow([]()->std::uint32_t{throw 7;},[&](std::uint32_t primary)noexcept{++compensation;return primary;});CHECK(error==5&&compensation==1);}
static void early_entry_before_invalid_deadline_is_not_old_rpc()
{host::rmw_service_journal j;j.begin_api();j.begin_rpc();j.end_rpc(0);j.api_result(0);j.begin_api();host::rmw_service_values::deadline entry{},out{};auto error=host::rmw_service_values::entry_deadline(entry,3000,0,out);j.api_result(error);CHECK(error==22&&j.api_returned&&j.api_error==22&&!j.api_rpc_attempted&&j.rpc_attempted&&j.rpc_returned&&j.rpc_error==0);}
int main()
{
 pending_is_not_success();preserve_first_and_last_primary();cleanup_cannot_overwrite_primary();every_cleanup_pending_slot_separate();clear_borrow_preserves_history();only_peek_idle_without_visible_borrow();prefix_exact64_bits_and_restored();prefix_restored_on_early_scope_return();logical_response_uses_same_identity();no_missing_metadata_becomes_complete();zero_guid_and_zero_time_are_actual_values();request_topic_not_response_projection();error_control_has_no_native_values();closed_control_requires_real_retired_bit();malformed_native_header_keeps_output();empty_request_not_absent_umrc_body();available_independent_third_peer();ack_exact_sequence_and_nonce();
 service_bound_keys_are_actual_schema();source_deadline_clamps_connection_entry();responder_deadline_cannot_restart_budget();query_deadline_is_independent_and_finite();rpc_zero_survives_host_copy_error();local_preflight_is_not_rpc_attempt();actual_borrow_helper_compensates_allocation_fault();actual_borrow_helper_compensates_other_fault();
 early_entry_before_invalid_deadline_is_not_old_rpc();
 std::printf("%s_RMW_SERVICE_HOST_VALUES groups=27 checks=%u failures=%u\n",failures?"FAIL":"PASS",checks,failures);return failures?1:0;
}
