/* SPDX-License-Identifier: Apache-2.0
 * Real original generated-object companion, UNEXECUTED. All11 exact independent
 * native identities are mandatory. It neither computes RIHS nor creates fake
 * TypeSupport. All B/C/CPP10/provider/application compile with the same codec.h.
 */
#include "native_objects.h"
#include <uwvm_rmw/cpp_codecs.hpp>
#include <std_srvs/srv/empty.hpp>
#include <example_interfaces/srv/add_two_ints.hpp>
#include <memory>
#include <new>
#include <utility>
namespace {
struct objects {
 uwvm_rmw_cpp::Registry registry;
 std_srvs::srv::Empty::Request empty_request{};
 std_srvs::srv::Empty::Response empty_response{};
 example_interfaces::srv::AddTwoInts::Request add_request{};
 example_interfaces::srv::AddTwoInts::Response add_response{};
 bool add{};
 objects(uwvm_rmw_cpp::NativeIdentities const&ids,bool selected):registry{ids,128},add{selected}{}
};
std::unique_ptr<objects> selected;
bool parse(char const*t,uwvm_rmw_cpp::Hash&out){
 if(!t)return false;unsigned nonzero{};
 for(unsigned i{};i!=64;++i){auto c=static_cast<unsigned char>(t[i]);if(!c)return false;unsigned n;
 if(c>='0'&&c<='9')n=c-'0';else if(c>='a'&&c<='f')n=c-'a'+10;else if(c>='A'&&c<='F')n=c-'A'+10;else return false;
 if(i%2)out[i/2]|=static_cast<std::uint8_t>(n);else out[i/2]=static_cast<std::uint8_t>(n<<4);nonzero|=n;}
 return !t[64]&&nonzero;
}
}
extern "C" uint32_t example_native_prepare(char const*const text[11],int add,uwvm_rmw_codec const**table,size_t*count,size_t*index){
 if(selected)return 17;if(!text||!table||!count||!index||(add!=0&&add!=1))return 22;
 uwvm_rmw_cpp::NativeIdentities ids{};for(unsigned i{};i!=11;++i)if(!parse(text[i],ids.hashes[i]))return 22;
 try{auto next=std::make_unique<objects>(ids,add!=0);if(next->registry.error())return next->registry.error();
 auto codecs=next->registry.codecs();if(codecs.size()!=7)return 22;
 *table=codecs.data();*count=codecs.size();*index=add?6:3;selected=std::move(next);return 0;
 }catch(std::bad_alloc const&){return 12;}catch(...){return 5;}
}
extern "C" void*example_native_request(void){if(!selected)return nullptr;return selected->add?static_cast<void*>(&selected->add_request):static_cast<void*>(&selected->empty_request);}
extern "C" void*example_native_response(void){if(!selected)return nullptr;return selected->add?static_cast<void*>(&selected->add_response):static_cast<void*>(&selected->empty_response);}
extern "C" uint32_t example_native_set_request(void){if(!selected)return 22;if(selected->add){selected->add_request.a=17;selected->add_request.b=-9;}return 0;}
extern "C" uint32_t example_native_check_request(void){if(!selected)return 22;return selected->add?(selected->add_request.a==17&&selected->add_request.b==-9?0:22):(selected->empty_request.structure_needs_at_least_one_member==0?0:22);}
extern "C" uint32_t example_native_set_response(void){if(!selected)return 22;if(selected->add)selected->add_response.sum=8;return 0;}
extern "C" uint32_t example_native_check_response(void){if(!selected)return 22;return selected->add?(selected->add_response.sum==8?0:22):(selected->empty_response.structure_needs_at_least_one_member==0?0:22);}
