/* SPDX-License-Identifier: Apache-2.0
 * Authored18 groups, unexecuted. Uses ACTUAL generated native C++/C objects,
 * TypeSupport and original RMW functions. Only the no-open transport callbacks
 * below are local counters. No fake generated struct/hash/introspection exists.
 * This is not a DDS, Gateway, Wasm, unchanged-rclcpp or native ROS route result.
 */
#include "published_cpp_identity_fixture.hpp"
#include <uwvm_rmw/bridge.h>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/string.h>
#include <std_srvs/srv/empty.hpp>
#include <example_interfaces/srv/add_two_ints.hpp>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp>
#include <rmw/error_handling.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
using Registry=uwvm_rmw_cpp::Registry;using Kind=uwvm_rmw_cpp::Codec;
using String=std_msgs::msg::String;using Empty=std_srvs::srv::Empty;using Add=example_interfaces::srv::AddTwoInts;
static unsigned checks,failures,opens,closes;
#define CHECK(x) do{++checks;if(!(x)){++failures;std::fprintf(stderr,"FAIL_RMW_CPP_CODEC line=%u\n",unsigned(__LINE__));}}while(0)
static uwvm_rmw_codec const& codec(Registry const& r,Kind k){auto p=r.codec(k);CHECK(p);if(!p)std::abort();return *p;}
static std::uint32_t abi(){return 1;}
static std::uint32_t open(std::uint32_t,std::uint64_t,std::uint64_t,std::uint64_t){++opens;return 13;}
static std::uint32_t close(std::uint64_t){++closes;return 9;}
static void all_native_graphs_and_handles(){Registry r{cpp_fixture::published(),128};CHECK(!r.error()&&r.codecs().size()==7);
 for(auto const& c:r.codecs())CHECK(c.validation_state&&c.validate(&c,c.generated_support)==0);
 auto const& s=codec(r,Kind::string);CHECK(s.generated_support==rosidl_typesupport_cpp::get_message_type_support_handle<String>());
 CHECK(s.validate(&s,rosidl_typesupport_introspection_cpp::get_message_type_support_handle<String>())==0);}
static void no_implicit_selection(){CHECK(uwvm_rmw_install_profile(nullptr)==RMW_RET_OK);CHECK(opens==0&&closes==0);}
static void complete_profile_no_open(){Registry r{cpp_fixture::published(),128};CHECK(!r.error());roswasmc_backend b{};b.version=2;b.abi_version=abi;b.open=open;b.close=close;
 uwvm_rmw_profile p{};p.version=1;p.backend=&b;p.codecs=r.codecs().data();p.codec_count=r.codecs().size();
 CHECK(uwvm_rmw_install_profile(&p)==RMW_RET_OK&&opens==0&&closes==0);CHECK(uwvm_rmw_install_profile(nullptr)==RMW_RET_OK);}
static void wrong_hash_refused(){auto ids=cpp_fixture::published();ids.hashes[5][0]^=1;Registry r{ids,128};CHECK(r.error()==22&&r.codecs().empty());}
static void missing_hash_refused(){auto ids=cpp_fixture::published();ids.hashes[1]={};Registry r{ids,128};CHECK(r.error()==22&&r.codecs().empty());}
static void bounds_refused(){auto ids=cpp_fixture::published();Registry small{ids,3},large{ids,4194305};CHECK(small.error()==22&&large.error()==22);}
static void string_roundtrip_utf8_nul(){Registry r{cpp_fixture::published(),128};auto const& c=codec(r,Kind::string);String value;value.data=std::string{"A\0B",3}+"\xef\xbb\xbf\xf0\x9f\x98\x80";
 unsigned char bytes[128]{};std::size_t length=99;CHECK(c.encode(&value,bytes,sizeof(bytes),&length)==0&&length==value.data.size()+4);
 String out;out.data="old";CHECK(c.decode(bytes,length,&out)==0&&out.data==value.data);}
static void string_bad_native_utf8(){Registry r{cpp_fixture::published(),128};auto const& c=codec(r,Kind::string);String value;value.data="\xc0\xaf";unsigned char bytes[128];std::memset(bytes,0x55,sizeof(bytes));std::size_t n=77;
 CHECK(c.encode(&value,bytes,sizeof(bytes),&n)==22&&n==77&&bytes[0]==0x55);}
static void string_short_output_atomic(){Registry r{cpp_fixture::published(),128};auto const& c=codec(r,Kind::string);String value;value.data="abc";unsigned char bytes[8];std::memset(bytes,0x55,sizeof(bytes));std::size_t n=77;
 CHECK(c.encode(&value,bytes,6,&n)==90&&n==77&&bytes[0]==0x55&&bytes[7]==0x55);}
static void string_bad_wire(){Registry r{cpp_fixture::published(),128};auto const& c=codec(r,Kind::string);unsigned char bytes[]{1,0,0,0,0xc0};String out;out.data="preserved";
 CHECK(c.decode(bytes,sizeof(bytes),&out)==22&&out.data=="preserved");bytes[4]='a';CHECK(c.decode(bytes,4,&out)==22&&out.data=="preserved");}
static void string_empty(){Registry r{cpp_fixture::published(),128};auto const& c=codec(r,Kind::string);String value;unsigned char bytes[4]{55,55,55,55};std::size_t n=77;
 CHECK(c.encode(&value,bytes,4,&n)==0&&n==4&&!bytes[0]&&!bytes[3]);String out;out.data="old";CHECK(c.decode(bytes,4,&out)==0&&out.data.empty());}
static void empty_zero_wire(){Registry r{cpp_fixture::published(),128};Empty::Request request;Empty::Response response;unsigned char physical_private=0x66;std::size_t n=77;
 auto const& a=codec(r,Kind::empty_request);auto const& b=codec(r,Kind::empty_response);CHECK(!a.maximum_wire_bytes&&!b.maximum_wire_bytes);
 CHECK(a.encode(&request,&physical_private,0,&n)==0&&n==0&&physical_private==0x66);n=77;CHECK(b.encode(&response,&physical_private,0,&n)==0&&n==0);
 request.structure_needs_at_least_one_member=77;CHECK(a.decode(nullptr,0,&request)==0&&request.structure_needs_at_least_one_member==0);
 response.structure_needs_at_least_one_member=77;CHECK(b.decode(&physical_private,1,&response)==22&&response.structure_needs_at_least_one_member==77);
 n=77;CHECK(b.encode(&response,&physical_private,0,&n)==22&&n==77&&physical_private==0x66);
 request.structure_needs_at_least_one_member=33;CHECK(a.encode(&request,&physical_private,0,&n)==22&&n==77&&physical_private==0x66);}
static void add_request_complete(){Registry r{cpp_fixture::published(),128};auto const& c=codec(r,Kind::add_request);Add::Request q;q.a=std::numeric_limits<std::int64_t>::min();q.b=std::numeric_limits<std::int64_t>::max();unsigned char bytes[16]{};std::size_t n=77;
 CHECK(c.encode(&q,bytes,16,&n)==0&&n==16&&bytes[7]==0x80&&bytes[8]==0xff&&bytes[15]==0x7f);Add::Request out;
 CHECK(c.decode(bytes,16,&out)==0&&out.a==q.a&&out.b==q.b);}
static void add_response_complete(){Registry r{cpp_fixture::published(),128};auto const& c=codec(r,Kind::add_response);Add::Response v;v.sum=-42;unsigned char bytes[8]{};std::size_t n=77;
 CHECK(c.encode(&v,bytes,8,&n)==0&&n==8);Add::Response out;CHECK(c.decode(bytes,8,&out)==0&&out.sum==-42);}
static void add_malformed_atomic(){Registry r{cpp_fixture::published(),128};auto const& a=codec(r,Kind::add_request);auto const& b=codec(r,Kind::add_response);unsigned char bytes[16]{};Add::Request q;q.a=123;q.b=456;Add::Response v;v.sum=789;
 CHECK(a.decode(bytes,15,&q)==22&&q.a==123&&q.b==456);CHECK(b.decode(bytes,7,&v)==22&&v.sum==789);std::size_t n=77;std::memset(bytes,0x55,sizeof(bytes));CHECK(b.encode(&v,bytes,7,&n)==90&&n==77&&bytes[0]==0x55);}
static void descriptor_state_and_names(){Registry r{cpp_fixture::published(),128};auto c=codec(r,Kind::string);CHECK(c.validate(&c,c.generated_support)==22);c.validation_state=nullptr;CHECK(c.validate(&c,c.generated_support)==22);
 // The allocated test RegistryState is nonconst. Deliberately corrupt only its
 // own descriptor, then restore it; no native TypeSupport object is fabricated.
 auto real=const_cast<uwvm_rmw_codec*>(r.codec(Kind::string));auto saved=*real;
 real->native_type="std_msgs/msg/Bool";CHECK(real->validate(real,saved.generated_support)==22);*real=saved;
 real->kind=UWVM_RMW_SERVICE_CODEC;CHECK(real->validate(real,saved.generated_support)==22);*real=saved;
 real->maximum_wire_bytes=3;CHECK(real->validate(real,saved.generated_support)==22);*real=saved;}
static void native_cpp_rejects_c_typesupport(){Registry r{cpp_fixture::published(),128};auto const& c=codec(r,Kind::string);auto actual_c=ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String);CHECK(actual_c&&actual_c!=c.generated_support&&c.validate(&c,actual_c)==22);}
static void cross_service_components(){Registry r{cpp_fixture::published(),128};auto const& e=codec(r,Kind::empty_service);auto const& a=codec(r,Kind::add_service);
 CHECK(e.kind==UWVM_RMW_SERVICE_CODEC&&!e.encode&&!e.decode&&e.request==r.codec(Kind::empty_request)&&e.response==r.codec(Kind::empty_response)&&e.maximum_wire_bytes==0);
 CHECK(a.generated_support==rosidl_typesupport_cpp::get_service_type_support_handle<Add>()&&a.request==r.codec(Kind::add_request)&&a.response==r.codec(Kind::add_response)&&a.maximum_wire_bytes==16);
 auto real=const_cast<uwvm_rmw_codec*>(r.codec(Kind::empty_service));auto saved=*real;
 real->request=r.codec(Kind::add_request);CHECK(real->validate(real,saved.generated_support)==22);*real=saved;}
int main(){Registry admitted{cpp_fixture::published(),128};if(admitted.error()){
 std::fprintf(stderr,"FAIL_RMW_CPP_CODEC actual installed native graph/identity refusal=%u\n",admitted.error());return 1;}
 all_native_graphs_and_handles();no_implicit_selection();complete_profile_no_open();wrong_hash_refused();missing_hash_refused();bounds_refused();string_roundtrip_utf8_nul();string_bad_native_utf8();string_short_output_atomic();string_bad_wire();string_empty();empty_zero_wire();add_request_complete();add_response_complete();add_malformed_atomic();descriptor_state_and_names();native_cpp_rejects_c_typesupport();cross_service_components();rmw_reset_error();
 std::printf("%s_UPSTREAM_RMW_CPP_CODEC_CONTRACT groups=18 checks=%u failures=%u\n",failures?"FAIL":"PASS",checks,failures);return failures?1:0;}
