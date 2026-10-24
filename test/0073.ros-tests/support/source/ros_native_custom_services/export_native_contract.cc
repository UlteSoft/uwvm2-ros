/* SPDX-License-Identifier: Apache-2.0 */
/** Independent actual-original-SDK metadata producer SOURCE, not executed.
 * No Node/entity/capability exists. TypeSupport dispatch may load original
 * native libraries: Root MUST invoke only in its retained loader/MD/maps phase.
 * Output full source bytes plus ACTUAL hashes, never a catalog hash/default.
 * This producer does not include or use independent_native_exports: it cannot
 * validate its own output as expected identity. Root separately reviews the
 * complete selected SDK/producer/MD/ELF/maps and associates that receipt.
 */
#include "custom_owned_services.h"
#include "custom_owned_service_identity.h"
#include <map>
#include <set>
#include "src/ros/gateway/common/native_io.h"
#include <stdexcept>
using namespace uwvm2::ros_gateway;
namespace cs=custom_owned_services;
#if UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API && UWVM_ROS2_HAVE_TYPE_DESCRIPTION && UWVM_ROS2_HAVE_OPENSSL
namespace {
struct row{std::string identity,encoding,source;bool message{};std::vector<std::string> fields;};
std::string text(rosidl_runtime_c__String const&s){if(s.size>4194304||s.capacity<s.size||(!s.data&&s.size))throw std::runtime_error("native source extent");return s.size?std::string(s.data,s.size):std::string{};}
std::string hex(unsigned char const*p,std::size_t n){return native_io::hexadecimal(p,n,"");}
std::string quoted(std::string_view s){std::string out{"\""};for(unsigned char c:s){if(c<0x20||c=='"'||c=='\\')throw std::runtime_error("unrepresentable metadata name");out.push_back(char(c));}out.push_back('"');return out;}
template<class H>std::string add(H const*h,std::map<std::string,row>& rows){
 if(!h||!h->get_type_hash_func||!h->get_type_description_func||!h->get_type_description_sources_func)throw std::runtime_error("complete metadata API absent");
 auto hash=h->get_type_hash_func(h);auto d=h->get_type_description_func(h);auto sources=h->get_type_description_sources_func(h);
 if(!hash||hash->version!=1||!d||!sources||!sources->data||!sources->size||sources->size>256||sources->capacity<sources->size)throw std::runtime_error("complete native metadata absent");
 auto name=text(d->type_description.type_name);if(name.empty()||name.size()>255)throw std::runtime_error("native type name");
 row result;result.identity=native_io::hexadecimal(hash->value,sizeof hash->value,"RIHS01_");unsigned found{};
 for(std::size_t i{};i<sources->size;++i)if(text(sources->data[i].type_name)==name){++found;result.encoding=text(sources->data[i].encoding);result.source=text(sources->data[i].raw_file_contents);}
 if(found!=1)throw std::runtime_error("one original full TypeSource required");
 if constexpr(std::is_same_v<H,rosidl_message_type_support_t>){
  result.message=true;auto native=h->func?h->func(h,rosidl_typesupport_introspection_cpp::typesupport_identifier):nullptr;
  if(!native||!native->data)throw std::runtime_error("actual Message layout unavailable");auto m=static_cast<rosidl_typesupport_introspection_cpp::MessageMembers const*>(native->data);
  if(m->member_count_>4096||(m->member_count_&&!m->members_))throw std::runtime_error("actual member extent");
  for(std::size_t i{};i<m->member_count_;++i){auto p=m->members_[i].name_;if(!p)throw std::runtime_error("actual member name absent");std::size_t n{};while(n<256&&p[n])++n;if(n==256)throw std::runtime_error("actual member name bound");result.fields.emplace_back(p,n);}
 }
 auto old=rows.find(name);if(old!=rows.end()&&(old->second.identity!=result.identity||old->second.encoding!=result.encoding||old->second.source!=result.source||old->second.message!=result.message||old->second.fields!=result.fields))throw std::runtime_error("ambiguous actual native identity/source");
 if(old==rows.end()){std::size_t total{};for(auto const& value:rows){if(value.second.source.size()>16777216-total)throw std::runtime_error("native source total bound");total+=value.second.source.size();}if(result.source.size()>16777216-total)throw std::runtime_error("native source total bound");}
 rows.emplace(name,std::move(result));if(rows.size()>256)throw std::runtime_error("native graph bound");return name;
}
void message(rosidl_message_type_support_t const*h,std::map<std::string,row>& rows,std::set<std::string>& visited,unsigned depth){
 if(depth>16)throw std::runtime_error("native graph depth");auto name=add(h,rows);if(!visited.emplace(name).second)return;
 auto actual=h->func?h->func(h,rosidl_typesupport_introspection_cpp::typesupport_identifier):nullptr;
 if(!actual||!actual->data)throw std::runtime_error("actual introspection unavailable");if(add(actual,rows)!=name)throw std::runtime_error("native/generic mismatch");
 auto m=static_cast<rosidl_typesupport_introspection_cpp::MessageMembers const*>(actual->data);
 if(m->member_count_>4096||(m->member_count_&&!m->members_))throw std::runtime_error("complete members required");
 for(std::size_t i{};i<m->member_count_;++i)if(m->members_[i].members_)message(m->members_[i].members_,rows,visited,depth+1);
}
std::string report;std::size_t roots{};
template<class Service>void service(std::string_view selected){
 auto h=rosidl_typesupport_cpp::get_service_type_support_handle<Service>();std::map<std::string,row> records;auto actual=add(h,records);if(actual!=selected)throw std::runtime_error("selected original service mismatch");
 auto native=h->func?h->func(h,rosidl_typesupport_introspection_cpp::typesupport_identifier):nullptr;if(add(native,records)!=actual)throw std::runtime_error("native root mismatch");
 std::set<std::string> visited;message(rosidl_typesupport_cpp::get_message_type_support_handle<typename Service::Request>(),records,visited,1);
 message(rosidl_typesupport_cpp::get_message_type_support_handle<typename Service::Response>(),records,visited,1);
 message(rosidl_typesupport_cpp::get_message_type_support_handle<typename Service::Event>(),records,visited,1);
 if(roots++)report+=",";report+="{\"type\":"+quoted(actual)+",\"records\":[";std::size_t n{},total{};
 for(auto const& [name,value]:records){if(value.source.size()>16777216-total)throw std::runtime_error("native source total bound");total+=value.source.size();unsigned char digest[32];static unsigned char empty{};
 if(!::SHA256(value.source.empty()?&empty:reinterpret_cast<unsigned char const*>(value.source.data()),value.source.size(),digest))throw std::runtime_error("source hash unavailable");
 if(n++)report+=",";report+="{\"type\":"+quoted(name)+",\"native_identity\":"+quoted(value.identity)+",\"encoding\":"+quoted(value.encoding)+",\"source_sha256\":"+quoted(hex(digest,32))+",\"source_hex\":"+quoted(hex(reinterpret_cast<unsigned char const*>(value.source.data()),value.source.size()))+"}";
 report.pop_back();report+=",\"members\":";
 if(value.message){report+="[";for(std::size_t field{};field<value.fields.size();++field){if(field)report+=",";report+=quoted(value.fields[field]);}report+="]";}else report+="null";report+="}";
 if(report.size()>67108864)throw std::runtime_error("whole independent native report bound");}
 report+="]}";
}
}
#endif
int main(){
#if !UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API || !UWVM_ROS2_HAVE_TYPE_DESCRIPTION || !UWVM_ROS2_HAVE_OPENSSL
 return 95;
#else
 try{report="{\"format_version\":1,\"kind\":\"actual-original-native-custom-service-export\",\"services\":[";
#define UWVM_EXPORT_ACTUAL_SERVICE(Native,Name,NativeId,CodecId,Request,Response) service<Native>(Name);
 UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(UWVM_EXPORT_ACTUAL_SERVICE)
#undef UWVM_EXPORT_ACTUAL_SERVICE
 report+="]}\n";if(!roots)return 95;if(report.size()>67108864)return 90;return native_io::write_all(STDOUT_FILENO,report)?0:5;
 }catch(std::bad_alloc const&){return 12;}catch(std::exception const&e){native_io::diagnostic("independent native export refused: ",e.what());return 22;}catch(...){return 5;}
#endif
}
