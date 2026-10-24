/* Genuine generated C++ facade plus the actual shared canonical grammar.
 * This is wire DATA, not a replacement ROS typesupport/Native layout. */
#include "bounded_records.hpp"
#include "control_wire.hpp"
#include <cassert>
#include <cstring>
#include <memory>
#include <vector>
using namespace uwvm2::imported::ros::ipc;
using Source=uwvm_custom::Ros2Type_description_interfacesMsgTypeSource;
using Response=uwvm_custom::Ros2Type_description_interfacesSrvGetTypeDescription_Response;
static_assert(static_cast<unsigned>(custom_wire_kind::sequence)==14);
static_assert(static_cast<unsigned>(custom_wire_kind::raw_bytes)==15);
static_assert(static_cast<unsigned>(custom_wire_kind::bounded_record)==16);
static void raw_and_typed_atomicity(){
  auto source=std::make_unique<Source>();source->raw_file_contents.size=4;source->raw_file_contents.data[1]=255;source->raw_file_contents.data[2]=192;
  roswasmcpp::wire::bytes bytes{42};assert(roswasmcpp::Codec<Source>::encode(*source,bytes));
  assert(validate_custom_wire(uwvm_control_wire::schema_Ros2Type_description_interfacesMsgTypeSource,bytes.data(),bytes.size()));
  Source decoded{};assert(roswasmcpp::Codec<Source>::decode(bytes,decoded));assert(decoded.raw_file_contents.size==4&&decoded.raw_file_contents.data[1]==255);
  source->type_name.size=1;source->type_name.data[0]=0;auto before=bytes;assert(!roswasmcpp::Codec<Source>::encode(*source,bytes)&&bytes==before);
}
static void bounded_extent_is_explicit(){
  constexpr custom_wire_node raw{custom_wire_kind::raw_bytes,6162,nullptr,0};
  constexpr custom_wire_node text{custom_wire_kind::utf8,6162,nullptr,0};
  custom_wire_node seq{custom_wire_kind::sequence,12,&raw,1};custom_wire_extent extent{};
  assert(!custom_wire_extent_of(seq,extent)); /* Old root32KiB cartesian rule. */
  custom_wire_node cap{custom_wire_kind::bounded_record,32768,&seq,1};
  assert(custom_wire_extent_of(cap,extent)&&extent.maximum==32768&&extent.minimum==4);
  std::vector<unsigned char> bytes(4);uwvm_bounded_put(bytes.data(),0,4);assert(validate_custom_wire(cap,bytes.data(),bytes.size()));
  /* One raw logical byte including NUL succeeds only in the raw domain. */
  bytes.assign(5,0);uwvm_bounded_put(bytes.data(),1,4);
  assert(validate_custom_wire(raw,bytes.data(),bytes.size())&&!validate_custom_wire(text,bytes.data(),bytes.size()));
  cap.bound=3;assert(!custom_wire_extent_of(cap,extent));
}
static void nested_caps_and_work(){
  custom_wire_node raw{custom_wire_kind::raw_bytes,100,nullptr,0};custom_wire_node inner{custom_wire_kind::bounded_record,8,&raw,1};
  custom_wire_node outer{custom_wire_kind::bounded_record,512,&inner,1};
  unsigned char bytes[9]{};uwvm_bounded_put(bytes,5,4);assert(!validate_custom_wire(outer,bytes,sizeof bytes));
  uwvm_bounded_put(bytes,4,4);assert(validate_custom_wire(outer,bytes,8));
  custom_wire_node cycle{custom_wire_kind::array,1,nullptr,1};cycle.children=&cycle;custom_wire_extent e{};assert(!custom_wire_extent_of(cycle,e));
  custom_wire_node many{custom_wire_kind::array,4096,&raw,1};assert(!custom_wire_extent_of(many,e));
  unsigned work=0;size_t at=0;assert(!custom_wire_consume(raw,bytes,sizeof bytes,at,work));
}
static void typed_aggregate_refusal(){
  auto response=std::make_unique<Response>();response->type_sources.size=6;
  for(unsigned i=0;i<6;++i)response->type_sources.data[i].raw_file_contents.size=6162;
  roswasmcpp::wire::bytes bytes{0xa5,0x5a};auto before=bytes;assert(!roswasmcpp::Codec<Response>::encode(*response,bytes)&&bytes==before);
}
int main(){raw_and_typed_atomicity();bounded_extent_is_explicit();nested_caps_and_work();typed_aggregate_refusal();}
