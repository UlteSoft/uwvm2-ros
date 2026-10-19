/* SPDX-License-Identifier: Apache-2.0
 * Authored LOCAL transport contracts over the actual C/C++ SDK. No Host/native
 * query is performed by this file's authorship. All tests remain unexecuted.
 */
#include <roswasm/service_client.hpp>
#include <cassert>
#include <array>
#include <cstring>
#include <limits>
#include <string>
#include <type_traits>
namespace m {
struct Owner {std::uint64_t handle{};std::string name;bool live{};};
static std::array<Owner,16> owners;
static std::uint64_t next{UINT64_C(0xf123456780000001)},features_bits{7},last_available{},last_source{},last_wait{};
static unsigned opens{},closes{},features_calls{},query_calls{},abi_value{1};
static std::uint32_t failure{},close_failure{};static unsigned char answer{};static bool write_output{true};
static std::string deny;
static void put(std::uint64_t p,std::uint64_t v){auto out=reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(p));for(unsigned i{};i<8;++i)out[i]=static_cast<unsigned char>(v>>(8*i));}
static Owner* find(std::uint64_t handle){for(auto& o:owners)if(o.live&&o.handle==handle)return &o;return nullptr;}
static std::uint32_t abi(){return abi_value;}
static std::uint32_t open(std::uint32_t memory,std::uint64_t p,std::uint64_t n,std::uint64_t out){
 assert(memory==0);++opens;std::string name{reinterpret_cast<char const*>(static_cast<std::uintptr_t>(p)),static_cast<std::size_t>(n)};
 if(name==deny)return 13;for(auto& owner:owners)if(!owner.live){owner={next++,name,true};put(out,owner.handle);return 0;}return 90;
}
static std::uint32_t close(std::uint64_t handle){auto owner=find(handle);if(!owner)return 9;owner->live=false;++closes;return close_failure;}
static std::uint32_t feature(std::uint64_t a,std::uint64_t s,std::uint32_t memory,std::uint64_t out){
 assert(memory==0);++features_calls;last_available=a;last_source=s;
 if(failure)return failure;if(write_output)put(out,features_bits);return 0;
}
static std::uint32_t query(std::uint64_t a,std::uint64_t s,std::uint64_t wait,std::uint32_t memory,std::uint64_t out){
 assert(memory==0);++query_calls;last_available=a;last_source=s;last_wait=wait;
 if(failure)return failure;if(write_output)*reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(out))=answer;return 0;
}
static roswasmc_backend business;
static roswasmc_service_client_backend extension;
static roswasmc_context context;
static void reset(unsigned backend=2){
 for(auto const& o:owners)assert(!o.live);owners={};opens=closes=features_calls=query_calls=0;
 next=UINT64_C(0xf123456780000001);features_bits=backend==1?11:7;last_available=last_source=last_wait=0;
 abi_value=1;failure=close_failure=0;answer=0;write_output=true;deny.clear();business={};business.version=backend;business.abi_version=abi;business.open=open;business.close=close;
 extension={backend,abi,feature,query};context={&business};
}
struct Raw {
 roswasmc_capability source{},available{};
 explicit Raw(){roswasmc_capability_init(&source,&context);roswasmc_capability_init(&available,&context);assert(!roswasmc_open(&source,"call",4));assert(!roswasmc_open(&available,"available",9));}
 ~Raw(){if(available.handle)(void)roswasmc_close(&available);if(source.handle)(void)roswasmc_close(&source);}
};
}
static void full_width_pair_and_real_false(){m::reset();m::Raw r;int out=99;
 assert(!roswasmc_service_client_query(&m::extension,&r.available,&r.source,0,&out)&&out==0&&m::query_calls==1);
 assert(m::last_source==r.source.handle&&m::last_source>UINT64_C(9007199254740992)&&m::last_available==r.available.handle&&m::last_wait==0);}
static void finite_true_and_maximum(){m::reset();m::Raw r;int out=0;m::answer=1;
 assert(!roswasmc_service_client_query(&m::extension,&r.available,&r.source,UINT64_C(60000000000),&out)&&out==1&&m::last_wait==UINT64_C(60000000000));
 assert(roswasmc_service_client_query(&m::extension,&r.available,&r.source,UINT64_C(60000000001),&out)==22&&m::query_calls==1&&out==1);}
static void failures_preserve_output(){m::reset();m::Raw r;
 for(auto code:{11u,13u,95u,110u,125u}){m::failure=code;int out=83;std::uint64_t bits=UINT64_C(0x123456789abcdef0);
  assert(roswasmc_service_client_query(&m::extension,&r.available,&r.source,0,&out)==code&&out==83);
  assert(roswasmc_service_client_features(&m::extension,&r.available,&r.source,&bits)==code&&bits==UINT64_C(0x123456789abcdef0));}}
static void canonical_bool_and_missing_write(){m::reset();m::Raw r;int out=37;m::answer=2;
 assert(roswasmc_service_client_query(&m::extension,&r.available,&r.source,0,&out)==22&&out==37);
 m::write_output=false;assert(roswasmc_service_client_query(&m::extension,&r.available,&r.source,0,&out)==22&&out==37);}
static void closed_and_same_owners_before_import(){m::reset();m::Raw r;int out=13;
 assert(roswasmc_service_client_query(&m::extension,&r.source,&r.source,0,&out)==22&&m::query_calls==0&&out==13);
 assert(!roswasmc_close(&r.source));assert(roswasmc_service_client_query(&m::extension,&r.available,&r.source,0,&out)==9&&m::query_calls==0&&out==13);}
static void context_and_backend_are_exact(){m::reset();m::Raw r;roswasmc_context other{&m::business};auto source=r.source;source.context=&other;int out=91;
 assert(roswasmc_service_client_query(&m::extension,&r.available,&source,0,&out)==22&&m::query_calls==0&&out==91);
 m::extension.version=1;assert(roswasmc_service_client_query(&m::extension,&r.available,&r.source,0,&out)==22&&m::query_calls==0);}
static void optional_missing_or_wrong_version(){m::reset();m::Raw r;int out=5;m::extension.query=nullptr;
 assert(roswasmc_service_client_query(&m::extension,&r.available,&r.source,0,&out)==95&&out==5);
 m::extension.query=m::query;m::abi_value=2;assert(roswasmc_service_client_query(&m::extension,&r.available,&r.source,0,&out)==95&&m::query_calls==0);}
static void complete_feature_masks(){for(auto backend:{1u,2u}){m::reset(backend);m::Raw r;std::uint64_t bits=91;
 assert(!roswasmc_service_client_features(&m::extension,&r.available,&r.source,&bits)&&bits==(backend==1?11u:7u));
 m::features_bits=0;assert(!roswasmc_service_client_features(&m::extension,&r.available,&r.source,&bits)&&bits==0);
 m::features_bits=1;bits=33;assert(roswasmc_service_client_features(&m::extension,&r.available,&r.source,&bits)==22&&bits==33);
 m::write_output=false;assert(roswasmc_service_client_features(&m::extension,&r.available,&r.source,&bits)==22&&bits==33);}}
static void null_outputs_before_import(){m::reset();m::Raw r;
 assert(roswasmc_service_client_query(&m::extension,&r.available,&r.source,0,nullptr)==22);
 assert(roswasmc_service_client_features(&m::extension,&r.available,&r.source,nullptr)==22&&m::query_calls==0&&m::features_calls==0);}
static void cpp_checked_source_and_false(){m::reset();roswasmcpp::Capability source{m::context,"call"};
 {roswasmcpp::ServiceAvailability available{m::context,m::extension,source,"available"};assert(!available.error());bool out=true;
  assert(!available.query(0,out)&&!out&&m::query_calls==1);std::uint64_t bits{};assert(!available.features(bits)&&bits==7);
  assert(!available.close()&&available.cleanup_attempted()&&available.cleanup_status()==0&&source.is_open());}
 assert(m::closes==1);assert(!source.close());}
static void cpp_abi_before_open(){m::reset();roswasmcpp::Capability source{m::context,"call"};m::abi_value=2;auto before=m::opens;
 {roswasmcpp::ServiceAvailability available{m::context,m::extension,source,"available"};assert(available.error()==95&&!available.is_open()&&m::opens==before);}
 assert(!source.close());}
static void cpp_denied_query_owner_no_source_retirement(){m::reset();roswasmcpp::Capability source{m::context,"call"};m::deny="available";
 {roswasmcpp::ServiceAvailability available{m::context,m::extension,source,"available"};bool out=true;assert(available.error()==13&&!available.is_open());
  assert(available.query(0,out)==13&&out&&m::query_calls==0&&!available.cleanup_attempted());}
 assert(source.is_open()&&m::closes==0);assert(!source.close());}
static void cpp_opaque_source_cannot_be_rebound(){m::reset();roswasmcpp::Capability source{m::context,"call"};
 {roswasmcpp::ServiceAvailability available{m::context,m::extension,source,"available"};auto first=source.handle();assert(!source.close());
  source=roswasmcpp::Capability{m::context,"call"};assert(first!=source.handle());bool out=true;
  assert(available.query(0,out)==9&&out&&m::query_calls==0);assert(!available.close());}
 assert(!source.close());}
static void cpp_cleanup_error_is_separate_and_not_replayed(){m::reset();roswasmcpp::Capability source{m::context,"call"};
 {roswasmcpp::ServiceAvailability available{m::context,m::extension,source,"available"};m::close_failure=110;
  assert(available.close()==110&&!available.is_open()&&available.cleanup_status()==110&&available.cleanup_attempted());
  auto before=m::closes;assert(!available.close()&&m::closes==before&&available.cleanup_status()==110);}
 m::close_failure=0;assert(!source.close());}
static void cpp_reverse_stack_destruction_once(){m::reset();{roswasmcpp::Capability source{m::context,"call"};
 {roswasmcpp::ServiceAvailability available{m::context,m::extension,source,"available"};assert(available.is_open());}assert(m::closes==1&&source.is_open());}
 assert(m::closes==2);}
int main(){full_width_pair_and_real_false();finite_true_and_maximum();failures_preserve_output();canonical_bool_and_missing_write();closed_and_same_owners_before_import();
 context_and_backend_are_exact();optional_missing_or_wrong_version();complete_feature_masks();null_outputs_before_import();cpp_checked_source_and_false();cpp_abi_before_open();
 cpp_denied_query_owner_no_source_retirement();cpp_opaque_source_cannot_be_rebound();cpp_cleanup_error_is_separate_and_not_replayed();cpp_reverse_stack_destruction_once();m::reset();return 0;}
