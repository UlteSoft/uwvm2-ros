#include "codec.hpp"
#include <fast_io.h>
#include <cstdlib>
#include <new>
#include <csignal>
#include <cstring>
#include <limits>
#include <utility>
#include <algorithm>
namespace stats { inline thread_local bool enabled{};inline thread_local std::size_t allocations{},bytes{}; }
void* operator new(std::size_t size) { if(stats::enabled) { ++stats::allocations;stats::bytes+=size; }if(auto p=std::malloc(size?size:1)) return p;throw std::bad_alloc{}; }
void* operator new[](std::size_t n) { return ::operator new(n); }
void operator delete(void* p) noexcept { std::free(p); }
void operator delete[](void* p) noexcept { std::free(p); }
void operator delete(void* p,std::size_t) noexcept { std::free(p); }
void operator delete[](void* p,std::size_t) noexcept { std::free(p); }
void* operator new(std::size_t size,std::align_val_t alignment) { if(stats::enabled) { ++stats::allocations;stats::bytes+=size; }void* p{};if(!::posix_memalign(&p,static_cast<std::size_t>(alignment),size?size:1)) return p;throw std::bad_alloc{}; }
void* operator new[](std::size_t n,std::align_val_t a) { return ::operator new(n,a); }
void operator delete(void* p,std::align_val_t) noexcept { std::free(p); }
void operator delete[](void* p,std::align_val_t) noexcept { std::free(p); }
void operator delete(void* p,std::size_t,std::align_val_t) noexcept { std::free(p); }
void operator delete[](void* p,std::size_t,std::align_val_t) noexcept { std::free(p); }
namespace golden { using namespace uwvm_ros;
#include "before-encoder.inc"
}
using uwvm_ros::typed_parameter_event;
using tag=uwvm_ros::parameter_event_value_tag;
std::size_t checks{},cases{};
void need(bool ok,std::string_view reason) { ++checks;if(!ok) { fast_io::println(fast_io::err(),"FAIL ",reason);std::exit(1); } }
typed_parameter_event sample(unsigned number,unsigned kind=1) {
    typed_parameter_event e;e.seconds=2147483647;e.nanoseconds=999999999;e.kind=kind;e.node="/sdk/events_1";e.name="module.item_2";e.tag=static_cast<tag>(number);
    e.bool_value=true;e.integer_value=std::numeric_limits<std::int64_t>::min();e.double_value=-0.0;e.string_value=std::string{"a\0\xcf\x80",4};
    e.byte_array_value={0,1,127,255};e.bool_array_value={0,1,1,0};e.integer_array_value={std::numeric_limits<std::int64_t>::min(),0,std::numeric_limits<std::int64_t>::max()};
    e.double_array_value={-0.0,-1.25,std::numeric_limits<double>::max()};e.string_array_value={"", "plain", std::string{"\xcf\x80",2}};return e;
}
void exercise(std::string const& name,typed_parameter_event const& value,bool expected) {
    uwvm_ros::bytes reference={0xde,0xad,0xbe,0xef},out=reference;
    auto old=golden::encode_typed_parameter_event(value,reference);need(old==expected,"golden acceptance mismatch");
    stats::allocations=stats::bytes=0;stats::enabled=true;auto ok=uwvm_ros::encode_typed_parameter_event(value,out);stats::enabled=false;
    auto allocations=stats::allocations,bytes=stats::bytes;
    need(ok==expected,"candidate acceptance mismatch");need(out==reference,"exact old wire/transactional output mismatch");
    if(expected) {
        need(out.size()>=23&&out.size()<=uwvm_ros::parameter_array_detail::maximum_bytes,"full envelope size");
#if SDK_AFTER
        need(allocations==1,"successful encoder did not allocate exactly one final wire");need(bytes==out.size(),"successful allocation bytes differ from final wire size");
#else
        need(allocations>=1,"baseline has no wire allocation");
#endif
        uwvm_ros::typed_parameter_event decoded;need(uwvm_ros::decode_typed_parameter_event(out,decoded),"candidate wire does not decode");
        need(decoded.tag==value.tag&&decoded.kind==value.kind&&decoded.node==value.node&&decoded.name==value.name,"decoded metadata mismatch");
        if(value.kind==3) {
            need(out.size()==23+value.node.size()+value.name.size(),"deletion unexpectedly contains prior payload");
            need(decoded.string_value.empty()&&decoded.byte_array_value.empty()&&decoded.bool_array_value.empty()&&decoded.integer_array_value.empty()&&decoded.double_array_value.empty()&&decoded.string_array_value.empty(),"deletion decoder has prior array/string data");
        }
    } else { need(allocations==0&&bytes==0,"rejected value allocated");need(out==uwvm_ros::bytes({0xde,0xad,0xbe,0xef}),"rejected output changed"); }
    ++cases;fast_io::println(fast_io::out(),"PASS case=",name," accepted=",ok," allocations=",allocations," allocated_bytes=",bytes," wire_bytes=",out.size()," checks=",checks);
}
int main() {
    ::raise(SIGSTOP);
    for(unsigned kind=1;kind!=4;++kind) for(unsigned number=0;number!=10;++number) {
        auto e=sample(number,kind);exercise("tag"+std::to_string(number)+"_kind"+std::to_string(kind),e,number!=0||kind==3);
    }
    // Explicit golden bytes independently pin the fixed-width LE envelope.
    auto b=sample(1);b.seconds=0x01020304;b.nanoseconds=0x05060708;b.kind=2;b.node="/a";b.name="b";
    uwvm_ros::bytes encoded;need(uwvm_ros::encode_typed_parameter_event(b,encoded),"golden bool encode");
    uwvm_ros::bytes pinned={4,3,2,1,8,7,6,5,2,2,1,2,0,0,0,'/','a',1,0,0,0,'b',1,0,0,0,1};need(encoded==pinned,"fixed LE golden bool bytes");
    auto i=b;i.tag=tag::integer;i.integer_value=std::bit_cast<std::int64_t>(UINT64_C(0x8070605040302010));need(uwvm_ros::encode_typed_parameter_event(i,encoded),"golden int encode");
    pinned[10]=2;pinned[22]=8;pinned.resize(34);std::array<unsigned char,8> integers={0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80};std::copy(integers.begin(),integers.end(),pinned.begin()+26);need(encoded==pinned,"fixed LE golden int bytes");
    for(unsigned number=1;number!=10;++number) { auto e=sample(number);e.string_value.clear();e.byte_array_value.clear();e.bool_array_value.clear();e.integer_array_value.clear();e.double_array_value.clear();e.string_array_value.clear();exercise("empty_tag"+std::to_string(number),e,true); }
    for(unsigned number=0;number!=10;++number) { auto e=sample(number,3);e.double_value=std::numeric_limits<double>::quiet_NaN();e.string_value="\xff";e.bool_array_value={2};e.double_array_value={std::numeric_limits<double>::infinity()};e.string_array_value={std::string{"nul\0value",9},"\xff"};e.byte_array_value.resize(uwvm_ros::parameter_array_detail::maximum_elements+1);exercise("deletion_invalid_prior_tag"+std::to_string(number),e,true); }
    auto e=sample(1);e.node="/"+std::string(1023,'n');e.name=std::string(255,'p');exercise("max_metadata",e,true);
    e.node.push_back('n');exercise("oversize_node",e,false);e=sample(1);e.name.resize(256,'p');exercise("oversize_name",e,false);
    for(auto invalid:{"", "/", "/a/", "/a//b", "/1bad", "/bad-node", "/a.thing", "/\xff"}) { e=sample(1);e.node=invalid;exercise("invalid_node_"+std::to_string(cases),e,false); }
    for(auto invalid:{"", ".", "a.", "a..b", "1bad", "bad-name", "a/b", "\xff"}) { e=sample(1);e.name=invalid;exercise("invalid_name_"+std::to_string(cases),e,false); }
    e=sample(1);e.seconds=2147483648u;exercise("seconds_outside_signed32",e,false);e=sample(1);e.nanoseconds=1000000000;exercise("nanoseconds_outside_range",e,false);
    for(unsigned kind:{0u,4u,255u}) { e=sample(1,kind);exercise("invalid_kind"+std::to_string(kind),e,false); }
    for(unsigned number:{10u,255u}) for(unsigned kind=1;kind!=4;++kind) { e=sample(number,kind);exercise("unknown_tag"+std::to_string(number)+"_kind"+std::to_string(kind),e,false); }
    for(double invalid:{std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity()}) { e=sample(3);e.double_value=invalid;exercise("invalid_float"+std::to_string(cases),e,false);e=sample(8);e.double_array_value={1,invalid};exercise("invalid_float_array"+std::to_string(cases),e,false); }
    e=sample(6);e.bool_array_value={0,1,2};exercise("non_boolean_array",e,false);
    for(std::string text:{std::string{"\xc0\x80",2},std::string{"\xed\xa0\x80",3},std::string{"\xf4\x90\x80\x80",4},std::string{"\xe2\x82",2},std::string{"\xff",1}}) { e=sample(4);e.string_value=text;exercise("invalid_utf8_string_"+std::to_string(cases),e,false);e=sample(9);e.string_array_value={text};exercise("invalid_utf8_array_"+std::to_string(cases),e,false); }
    e=sample(9);e.string_array_value={std::string{"a\0b",3}};exercise("nul_string_array",e,false);
    e=sample(4);auto available=uwvm_ros::parameter_array_detail::maximum_bytes-23-e.node.size()-e.name.size();e.string_value=std::string(available-4,'x');exercise("string_exact_envelope",e,true);e.string_value.push_back('x');exercise("string_over_envelope",e,false);
    e=sample(5);e.byte_array_value.resize(uwvm_ros::parameter_array_detail::maximum_elements,0xff);exercise("byte_max_elements",e,true);e.byte_array_value.push_back(0);exercise("byte_over_elements",e,false);
    e=sample(6);e.bool_array_value.resize(uwvm_ros::parameter_array_detail::maximum_elements,1);exercise("bool_max_elements",e,true);e.bool_array_value.push_back(0);exercise("bool_over_elements",e,false);
    e=sample(7);e.integer_array_value.resize((available-4)/8,42);exercise("int_array_last_envelope_element",e,true);e.integer_array_value.push_back(43);exercise("int_array_over_envelope",e,false);
    e=sample(8);e.double_array_value.resize((available-4)/8,-0.0);exercise("float_array_last_envelope_element",e,true);e.double_array_value.push_back(1);exercise("float_array_over_envelope",e,false);
    e=sample(9);e.string_array_value={std::string(available-8,'x')};exercise("string_array_exact_envelope",e,true);e.string_array_value[0].push_back('x');exercise("string_array_over_envelope",e,false);
    e=sample(9);e.string_array_value.resize(uwvm_ros::parameter_array_detail::maximum_elements+1);exercise("string_array_over_elements",e,false);
    fast_io::println(fast_io::out(),"PASS SDK event encode summary cases=",cases," checks=",checks," phase=",std::string_view{SDK_AFTER?"after":"before"});
}
