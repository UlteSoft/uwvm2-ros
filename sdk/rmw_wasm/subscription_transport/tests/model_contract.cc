/* SPDX-License-Identifier: Apache-2.0
 * Local RAW-PROTOCOL MODELS ONLY, UNEXECUTED. Real original SDK message-info
 * layout, allocator and generated C++ TypeSupport/complete codec are used. The
 * seven functions below supply explicitly synthetic wire/effect results; they
 * are NOT linked into production and are NOT native/Host qualification.
 */
#ifdef NDEBUG
#error "All authored model checks must remain enabled"
#endif
#include <uwvm_rmw/subscription_transport.h>
#include <roswasm/rmw_subscription_v1.h>
#include <uwvm_rmw/cpp_codecs.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_srvs/srv/empty.hpp>
#include "published_cpp_identity_fixture.hpp"
#include <array>
#include <cstdio>
#include <cstring>
#include <limits>
using Bytes=std::array<unsigned char,416>;
namespace model {
static unsigned failures,checks;
#define CHECK(v) do{++model::checks;if(!(v)){++model::failures;std::fprintf(stderr,"FAIL_SUB_TRANSPORT_MODEL line=%u\n",unsigned(__LINE__));}}while(false)
static void put(unsigned char*p,uint64_t n,size_t width){for(size_t i=0;i<width;++i)p[i]=static_cast<unsigned char>(n>>(8*i));}
struct Host {
 uint32_t peek_error{},commit_error{},cancel_error{},retire_error{};unsigned activations{},inspections{},peeks{},commits{},cancels{},retires{};
 bool corrupt_envelope{},corrupt_metadata{},wrong_gid{},sequences{},empty{},wrong_binding{};
 uint64_t source{},sequence{1},nonce{},incarnation{71};
};
static Host*host;
static void binding(unsigned char*p){p[7]=2;put(p+16,host->source,8);put(p+24,7,8);put(p+32,11,8);put(p+40,13,8);put(p+48,host->incarnation,8);}
static void receipt(unsigned char*p,unsigned command){std::memset(p,0,256);put(p,0x45524d55,4);put(p+4,1,2);p[6]=static_cast<unsigned char>(command);put(p+8,256,4);p[12]=2;p[13]=1;binding(p);
 put(p+64,1,4);put(p+68,1,4);put(p+72,2,4);put(p+76,1,4);put(p+80,4,8);
 if(host->wrong_binding)put(p+40,14,8);
}
static void delivery(unsigned char*p,size_t capacity){std::memset(p,0,capacity);put(p,0x53524d55,4);put(p+4,1,2);p[6]=1;put(p+8,96,4);p[12]=RMW_GID_STORAGE_SIZE;binding(p);put(p+56,host->sequence,8);put(p+64,++host->nonce,8);put(p+72,capacity,8);
 size_t body=host->empty?0:5;put(p+80,192+body,8);unsigned char*m=p+96;put(m,0x494d5255,4);put(m+4,1,2);m[6]=1;put(m+8,192,4);put(m+12,host->sequences?31:7,4);binding(m);put(m+56,host->sequence,8);put(m+72,UINT64_MAX-6,8);put(m+80,0,8);
 put(m+88,host->sequences?17:UINT64_MAX,8);put(m+96,host->sequences?19:UINT64_MAX,8);put(m+112,body,8);m[120]=RMW_GID_STORAGE_SIZE;
 // All-zero model GID value bytes are legal when real ABI extent is present.
 if(body){put(p+288,1,4);p[292]='x';}
 if(host->corrupt_envelope)p[13]=1;if(host->corrupt_metadata)m[176]=1;if(host->wrong_gid)m[120]=RMW_GID_STORAGE_SIZE==32?31:RMW_GID_STORAGE_SIZE+1;
}
struct Fixture {
 Host h;roswasmc_backend backend{};roswasmc_context context{};roswasmc_capability source{};
 uwvm_rmw_subscription_transport*state{};uwvm_rmw_c_activation_provider activation{};uwvm_rmw_c_native_metadata_provider metadata{};uwvm_rmw_c_native_endpoint native{};
 Fixture(uwvm_rmw_codec const*codec,bool empty=false){host=&h;backend.version=2;context.backend=&backend;source={&context,UINT64_C(0xa123000180000001)};h.source=source.handle;h.empty=empty;
 auto a=rcutils_get_default_allocator();CHECK(uwvm_rmw_subscription_transport_create(2,128,&a,&state)==0);CHECK(uwvm_rmw_subscription_transport_callbacks(state,&activation,&metadata)==0);
 rmw_qos_profile_t q={};CHECK(activation.activate(activation.state,UWVM_RMW_SUBSCRIPTION,&source,nullptr,codec,&q,&native)==0);CHECK(native.qos_count==1&&!native.native_gid_bytes);}
 ~Fixture(){host=&h;CHECK(activation.retire(activation.state,&source,nullptr,&native)==h.retire_error);CHECK(h.retires==1);CHECK(uwvm_rmw_subscription_transport_dispose_unpublished(&state)==17);}
 uint32_t take(void*p,size_t n,size_t*length,rmw_message_info_t*info){host=&h;return metadata.take_topic(metadata.state,&native,&source,p,n,length,info);}
 uwvm_rmw_subscription_transport_diagnostic facts(){uwvm_rmw_subscription_transport_diagnostic d{};CHECK(uwvm_rmw_subscription_transport_diagnostics(state,0,&d)==0);return d;}
};
static void no_effects_before_export(){auto a=rcutils_get_default_allocator();uwvm_rmw_subscription_transport*s=nullptr;CHECK(uwvm_rmw_subscription_transport_create(0,1,&a,&s)==22&&!s);CHECK(uwvm_rmw_subscription_transport_create(1,0,&a,&s)==0);CHECK(uwvm_rmw_subscription_transport_dispose_unpublished(&s)==0&&!s);}
static void successful_whole_record(uwvm_rmw_codec const*c){Fixture f{c};unsigned char bytes[128]{};size_t n=999;rmw_message_info_t info{};CHECK(f.take(bytes,sizeof bytes,&n,&info)==0&&n==5&&bytes[4]=='x');CHECK(info.source_timestamp==-7&&info.received_timestamp==0&&info.publication_sequence_number==UINT64_MAX&&info.reception_sequence_number==UINT64_MAX&&!info.from_intra_process);for(auto b:info.publisher_gid.data)CHECK(b==0);CHECK(f.h.commits==1&&!f.h.cancels);std_msgs::msg::String value;CHECK(c->decode(bytes,n,&value)==0&&value.data=="x");}
static void actual_sequence_fields(uwvm_rmw_codec const*c){Fixture f{c};f.h.sequences=true;unsigned char b[128]{};size_t n=99;rmw_message_info_t i{};CHECK(!f.take(b,sizeof b,&n,&i)&&i.publication_sequence_number==17&&i.reception_sequence_number==19);}
static void idle_preserves_outputs(uwvm_rmw_codec const*c){Fixture f{c};f.h.peek_error=11;unsigned char b[128];std::memset(b,0x66,sizeof b);size_t n=999;rmw_message_info_t i{};i.source_timestamp=123;CHECK(f.take(b,sizeof b,&n,&i)==11&&n==999&&i.source_timestamp==123&&b[0]==0x66&&!f.h.commits&&!f.h.cancels&&!f.h.retires);CHECK(f.facts().first_primary==0);}
static void short_capacity_before_borrow(uwvm_rmw_codec const*c){Fixture f{c};unsigned char b[4]{};size_t n=999;rmw_message_info_t i{};CHECK(f.take(b,sizeof b,&n,&i)==90&&n==999&&!f.h.peeks);}
static void wrong_owner_before_borrow(uwvm_rmw_codec const*c){Fixture f{c};auto stale=f.native;++stale.runtime_generation;unsigned char b[128]{};size_t n=999;rmw_message_info_t i{};CHECK(f.metadata.take_topic(f.metadata.state,&stale,&f.source,b,sizeof b,&n,&i)==13&&n==999&&!f.h.peeks);}
static void inspect_drift_preserves_outputs(uwvm_rmw_codec const*c){Fixture f{c};f.h.wrong_binding=true;unsigned char b[128]{};size_t n=999;rmw_message_info_t i{};CHECK(f.take(b,sizeof b,&n,&i)==13&&n==999&&!f.h.peeks);f.h.wrong_binding=false;}
static void malformed_metadata_cancels(uwvm_rmw_codec const*c){Fixture f{c};f.h.corrupt_metadata=true;unsigned char b[128]{};size_t n=999;rmw_message_info_t i{};CHECK(f.take(b,sizeof b,&n,&i)==22&&n==999&&f.h.cancels==1&&!f.h.commits&&!f.h.retires);}
static void incompatible_native_width_cancels(uwvm_rmw_codec const*c){Fixture f{c};f.h.wrong_gid=true;unsigned char b[128]{};size_t n=999;rmw_message_info_t i{};CHECK(f.take(b,sizeof b,&n,&i)==95&&n==999&&f.h.cancels==1&&!f.h.commits);}
static void malformed_envelope_retires(uwvm_rmw_codec const*c){Fixture f{c};f.h.corrupt_envelope=true;unsigned char b[128]{};size_t n=999;rmw_message_info_t i{};CHECK(f.take(b,sizeof b,&n,&i)==22&&n==999&&f.h.retires==1&&!f.h.cancels&&!f.h.commits);CHECK(f.take(b,sizeof b,&n,&i)==22&&f.h.peeks==1&&f.h.retires==1);}
static void failed_commit_no_retry(uwvm_rmw_codec const*c){Fixture f{c};f.h.commit_error=110;f.h.retire_error=5;unsigned char b[128]{};size_t n=999;rmw_message_info_t i{};CHECK(f.take(b,sizeof b,&n,&i)==110&&n==999&&f.h.commits==1&&!f.h.cancels&&f.h.retires==1);CHECK(f.take(b,sizeof b,&n,&i)==110&&f.h.commits==1&&f.h.peeks==1);auto d=f.facts();CHECK(d.first_primary==110&&d.commit_attempted&&d.commit_code==110&&!d.commit_succeeded&&d.retire_attempted&&d.retire_code==5);}
static void independent_three_errors(uwvm_rmw_codec const*c){Fixture f{c};f.h.corrupt_metadata=true;f.h.cancel_error=17;f.h.retire_error=5;unsigned char b[128]{};size_t n=999;rmw_message_info_t i{};CHECK(f.take(b,sizeof b,&n,&i)==22);auto d=f.facts();CHECK(d.first_primary==22&&d.cancel_code==17&&d.retire_code==5&&d.cancel_attempted&&d.retire_attempted&&!d.commit_attempted&&n==999);}
static void local_close_attachment(uwvm_rmw_codec const*c){Fixture f{c};CHECK(uwvm_rmw_subscription_transport_record_raw_close(f.state,0,107)==0);auto d=f.facts();CHECK(d.raw_close_reported&&d.raw_close_code==107&&!f.h.retires);CHECK(uwvm_rmw_subscription_transport_record_raw_close(f.state,0,0)==22);}
static void non_subscription_refuses(uwvm_rmw_codec const*c){Fixture f{c};auto before=f.h.activations;uwvm_rmw_c_native_endpoint out{};rmw_qos_profile_t q{};CHECK(f.activation.activate(f.activation.state,UWVM_RMW_SERVICE,&f.source,nullptr,c,&q,&out)==95&&f.h.activations==before&&!out.native_incarnation);}
static void logical_zero(uwvm_rmw_codec const*c){Fixture f{c,true};size_t n=999;rmw_message_info_t i{};CHECK(f.take(nullptr,0,&n,&i)==0&&n==0&&f.h.commits==1);std_srvs::srv::Empty::Request value;value.structure_needs_at_least_one_member=99;CHECK(c->decode(nullptr,n,&value)==0&&value.structure_needs_at_least_one_member==0);}
}
extern "C" uint32_t uwvm_ros2_rmw_sub_v1_abi_version(void){return 1;}
extern "C" uint32_t uwvm_ros2_rmw_sub_v1_subscription_activate(uint64_t source,uint32_t memory,uint64_t receipt){CHECK(source==model::host->source&&!memory);++model::host->activations;model::receipt(reinterpret_cast<unsigned char*>(static_cast<uintptr_t>(receipt)),1);return 0;}
extern "C" uint32_t uwvm_ros2_rmw_sub_v1_subscription_inspect(uint64_t source,uint64_t incarnation,uint32_t memory,uint64_t receipt){CHECK(source==model::host->source&&incarnation==model::host->incarnation&&!memory);++model::host->inspections;model::receipt(reinterpret_cast<unsigned char*>(static_cast<uintptr_t>(receipt)),2);return 0;}
extern "C" uint32_t uwvm_ros2_rmw_sub_v1_subscription_peek(uint64_t source,uint64_t incarnation,uint32_t gid,uint32_t memory,uint64_t packet,uint64_t capacity){CHECK(source==model::host->source&&incarnation==model::host->incarnation&&gid==RMW_GID_STORAGE_SIZE&&!memory);++model::host->peeks;if(model::host->peek_error)return model::host->peek_error;model::delivery(reinterpret_cast<unsigned char*>(static_cast<uintptr_t>(packet)),static_cast<size_t>(capacity));return 0;}
extern "C" uint32_t uwvm_ros2_rmw_sub_v1_subscription_commit(uint64_t,uint64_t,uint64_t sequence,uint64_t reservation){CHECK(sequence==model::host->sequence&&reservation==model::host->nonce);++model::host->commits;return model::host->commit_error;}
extern "C" uint32_t uwvm_ros2_rmw_sub_v1_subscription_cancel(uint64_t,uint64_t,uint64_t sequence,uint64_t reservation){CHECK(sequence==model::host->sequence&&reservation==model::host->nonce);++model::host->cancels;return model::host->cancel_error;}
extern "C" uint32_t uwvm_ros2_rmw_sub_v1_subscription_retire(uint64_t,uint64_t){++model::host->retires;return model::host->retire_error;}
int main(){uwvm_rmw_cpp::Registry registry{cpp_fixture::published(),128};if(registry.error())return static_cast<int>(registry.error());auto string=registry.codec(uwvm_rmw_cpp::Codec::string),empty=registry.codec(uwvm_rmw_cpp::Codec::empty_request);CHECK(string&&empty);if(!string||!empty)return 1;
 model::no_effects_before_export();model::successful_whole_record(string);model::actual_sequence_fields(string);model::idle_preserves_outputs(string);model::short_capacity_before_borrow(string);model::wrong_owner_before_borrow(string);model::inspect_drift_preserves_outputs(string);model::malformed_metadata_cancels(string);model::incompatible_native_width_cancels(string);model::malformed_envelope_retires(string);model::failed_commit_no_retry(string);model::independent_three_errors(string);model::local_close_attachment(string);model::non_subscription_refuses(string);model::logical_zero(empty);
 std::printf("%s_GUEST_SUBSCRIPTION_TRANSPORT_MODELS groups=15 checks=%u failures=%u\n",model::failures?"FAIL":"PASS",model::checks,model::failures);return model::failures?1:0;}
