/* SPDX-License-Identifier: Apache-2.0 */
/** Authored MODEL DATA only. Original rmw structs are used, but none of these
 * assigned IDs/timestamps/GUIDs is an actual received Native record. Checks
 * remain active in every build; no assert side effects or fixture TypeSupport.
 */
#ifdef NDEBUG
#error "Record contract must retain all source checks (-UNDEBUG)"
#endif
#include "gateways/ros2/custom_owned_service_records.h"
#include "gateways/common/native_io.h"
namespace d=uwvm2::ros_gateway::custom_owned_services::detail;
namespace n=uwvm2::ros_gateway::native_rcl_service;
using records=d::records;static unsigned checks,failures;
#define CHECK(x) do{++checks;if(!(x)){++failures;uwvm2::ros_gateway::native_io::fixed_text<96> record;if(record.append("FAIL_CUSTOM_RECORD line=")&&record.number(unsigned(__LINE__))&&record.append("\n"))(void)uwvm2::ros_gateway::native_io::write_audit_text(record.view());}}while(0)
static n::wire::binding owner(){return {2,UINT64_C(0x100000001),2,3,4};}
static n::info::header header(std::size_t n){n::info::header h{};h.kind=n::events::event_kind::service_request;h.flags=n::info::source_time|n::info::received_time|n::info::native_request_id;h.writer_size=16;h.request_sequence=INT64_MIN;h.source_timestamp=-42;h.received_timestamp=INT64_MAX;h.payload_bytes=n;return h;}
static void populate(records& q,std::size_t n){auto i=q.reserve(UINT64_MAX);CHECK(i==0);auto&r=q.at(i);r.identity_known=true;r.identity.sequence_number=INT64_MIN;std::memset(r.identity.writer_guid,0,sizeof r.identity.writer_guid);r.native_info.request_id=r.identity;r.native_info.source_timestamp=-42;r.native_info.received_timestamp=INT64_MAX;for(std::size_t j{};j<n;++j)r.body[j]=static_cast<std::uint8_t>(j);CHECK(q.enqueue(i,header(n),n)==0);}
static void bounds(){CHECK(records::valid_options(256,32768));CHECK(records::valid_options(1,0));CHECK(!records::valid_options(0,0)&&!records::valid_options(257,1)&&!records::valid_options(1,32769));bool threw{};try{records q{257,32768,owner(),7};}catch(std::invalid_argument const&){threw=true;}CHECK(threw);}
static void full_dynamic_body(){records q{2,32768,owner(),7};populate(q,32768);n::snapshot s;CHECK(q.peek(owner(),7,16,s)==0&&s.size==192+32768);for(std::size_t i{};i<32768;++i)CHECK(s.bytes[192+i]==static_cast<std::uint8_t>(i));}
static void zero_guid_signed_facts(){records q{1,32,owner(),7};populate(q,32);n::snapshot s;CHECK(q.peek(owner(),7,16,s)==0);n::info::header h{};CHECK(n::info::decode(s.bytes.get(),192,32,h)&&n::info::complete(h));CHECK(h.correlation==UINT64_MAX&&h.request_sequence==INT64_MIN&&h.source_timestamp==-42&&h.received_timestamp==INT64_MAX);for(auto b:h.writer)CHECK(b==0);}
static void short_capacity_no_borrow(){records q{1,32,owner(),7};populate(q,32);n::snapshot s;s.sequence=19;CHECK(q.peek(owner(),7,16,s,223)==90&&!s.bytes&&s.sequence==19);CHECK(q.peek(owner(),7,16,s,224)==0&&s.reservation==1);}
static void missing_extent_no_consume(){records q{1,32,owner(),7};populate(q,32);n::snapshot s;CHECK(q.peek(owner(),7,24,s)==95&&!s.bytes);CHECK(q.peek(owner(),7,16,s)==0&&s.reservation==1);}
static void same_native_record(){records q{1,0,owner(),7};auto i=q.reserve(9);auto&r=q.at(i);r.identity_known=true;r.identity.sequence_number=INT64_MIN;r.native_info.request_id=r.identity;r.native_info.source_timestamp=-41;r.native_info.received_timestamp=INT64_MAX;CHECK(q.enqueue(i,header(0),0)==22);r.native_info.source_timestamp=-42;CHECK(q.enqueue(i,header(0),0)==0);}
static void aba_commit(){records q{1,0,owner(),7};populate(q,0);n::snapshot a,b;CHECK(q.peek(owner(),7,16,a)==0&&q.cancel(owner(),7,a.sequence,a.reservation)==0);CHECK(q.peek(owner(),7,16,b)==0&&b.reservation>a.reservation);CHECK(q.commit(owner(),7,a.sequence,a.reservation)==13&&q.commit(owner(),7,b.sequence,b.reservation)==0);}
static void aba_cancel(){records q{1,0,owner(),7};populate(q,0);n::snapshot a,b;CHECK(q.peek(owner(),7,16,a)==0&&q.cancel(owner(),7,a.sequence,a.reservation)==0);CHECK(q.peek(owner(),7,16,b)==0);CHECK(q.cancel(owner(),7,a.sequence,a.reservation)==13&&q.cancel(owner(),7,b.sequence,b.reservation)==0);}
static void full_source_binding(){records q{1,0,owner(),7};populate(q,0);n::snapshot s;auto b=owner();++b.source;CHECK(q.peek(b,7,16,s)==13);b=owner();++b.session;CHECK(q.peek(b,7,16,s)==13);CHECK(q.peek(owner(),8,16,s)==13&&!s.bytes);}
static void empty_logical_body(){records q{1,0,owner(),7};populate(q,0);n::snapshot s;CHECK(q.peek(owner(),7,16,s)==0&&s.size==192&&q.commit(owner(),7,s.sequence,s.reservation)==0);}
static void no_history_reuse(){records q{1,0,owner(),7};populate(q,0);n::snapshot s;CHECK(q.peek(owner(),7,16,s)==0&&q.commit(owner(),7,s.sequence,s.reservation)==0);CHECK(!q.can_reserve()&&q.reserve(3)==records::no_slot&&q.ticket(UINT64_MAX)==0);}
static void once_retire_separate_error(){records q{1,0,owner(),7};CHECK(q.control(22)==0);n::snapshot s;CHECK(q.peek(owner(),7,0,s)==0&&q.commit(owner(),7,s.sequence,s.reservation)==0);CHECK(q.retire(5)==0&&q.retire(0)==125);CHECK(q.peek(owner(),7,0,s)==0);n::info::header h{};CHECK(n::info::decode(s.bytes.get(),192,0,h)&&h.error==5&&(h.control_flags&n::events::event_source_retired));}
int main(){bounds();full_dynamic_body();zero_guid_signed_facts();short_capacity_no_borrow();missing_extent_no_consume();same_native_record();aba_commit();aba_cancel();full_source_binding();empty_logical_body();no_history_reuse();once_retire_separate_error();uwvm2::ros_gateway::native_io::fixed_text<256> record;bool written=record.append(failures?"FAIL":"PASS")&&record.append("_CUSTOM_DYNAMIC_RECORDS groups=12 checks=")&&record.number(checks)&&record.append(" failures=")&&record.number(failures)&&record.append("\n")&&uwvm2::ros_gateway::native_io::write_all(STDOUT_FILENO,record.view());return failures?1:(written?0:5);}
