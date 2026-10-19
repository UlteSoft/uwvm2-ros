#include "../../sdk/common/codec.hpp"
#include <cassert>
#include <cstdlib>
#include <new>
#include <fast_io.h>
#include <fast_io_device.h>
#if defined(__linux__)
#include <unistd.h>
#endif

static bool forbid_allocation;
void* operator new(std::size_t size) {
    if(forbid_allocation) throw std::bad_alloc{};
    if(auto p=std::malloc(size?size:1)) return p;
    throw std::bad_alloc{};
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p,std::size_t) noexcept { std::free(p); }

namespace sdk=uwvm_ros;
using tag=sdk::parameter_event_value_tag;
static sdk::bytes raw(unsigned type,sdk::bytes const& payload,unsigned kind=2) {
    sdk::bytes result;sdk::append_u32(result,123);sdk::append_u32(result,456);
    result.insert(result.end(),{static_cast<unsigned char>(kind),2,static_cast<unsigned char>(type)});
    assert(sdk::append_string(result,"/typed_events/native"));assert(sdk::append_string(result,"value"));
    sdk::append_u32(result,payload.size());result.insert(result.end(),payload.begin(),payload.end());return result;
}
static sdk::typed_parameter_event sample(tag type) {
    sdk::typed_parameter_event result;result.seconds=123;result.nanoseconds=456;result.kind=2;
    result.node="/typed_events/native";result.name="value";result.tag=type;
    result.bool_value=true;result.integer_value=9007199254740993LL;result.double_value=-0.0;
    result.string_value=std::string{"\xe4\xb8\xad\0value",9};
    result.byte_array_value={0,127,128,255};result.bool_array_value={0,1,0};
    result.integer_array_value={INT64_MIN,9007199254740993LL,INT64_MAX};
    result.double_array_value={-0.0,std::bit_cast<double>(UINT64_C(0x3ff0000000000001)),std::numeric_limits<double>::min()};
    result.string_array_value={"","\xe4\xb8\xad","tail"};return result;
}
static sdk::bytes payload(sdk::typed_parameter_event const& value) {
    switch(value.tag) {
    case tag::boolean:return {1};
    case tag::integer:return {1,0,0,0,0,0,32,0};
    case tag::float64:return {0,0,0,0,0,0,0,128};
    case tag::string:return {9,0,0,0,0xe4,0xb8,0xad,0,'v','a','l','u','e'};
    case tag::byte_array:return {4,0,0,0,0,127,128,255};
    case tag::boolean_array:return {3,0,0,0,0,1,0};
    case tag::integer_array:return {3,0,0,0,0,0,0,0,0,0,0,128,1,0,0,0,0,0,32,0,255,255,255,255,255,255,255,127};
    case tag::float64_array:return {3,0,0,0,0,0,0,0,0,0,0,128,1,0,0,0,0,0,240,63,0,0,0,0,0,0,16,0};
    case tag::string_array:return {3,0,0,0,0,0,0,0,3,0,0,0,0xe4,0xb8,0xad,4,0,0,0,'t','a','i','l'};
    }
    std::abort();
}
static void reject(sdk::bytes const& wire) {
    auto out=sample(tag::integer_array);out.name="sentinel";auto before=sdk::encode(out);
    assert(!sdk::decode_typed_parameter_event(wire,out));assert(sdk::encode(out)==before);
}
static void reject(sdk::typed_parameter_event const& value,bool no_allocate=false) {
    sdk::bytes out{42,43};bool result{};forbid_allocation=no_allocate;
    try { result=sdk::encode_typed_parameter_event(value,out); }
    catch(...) { forbid_allocation=false;assert(false&&"preflight rejection allocated"); }
    forbid_allocation=false;assert(!result);assert((out==sdk::bytes{42,43}));
}
int main() {
#if defined(__linux__)
    char actual_exe[4096];auto length=::readlink("/proc/self/exe",actual_exe,sizeof(actual_exe));assert(length>0&&length<sizeof(actual_exe));
    fast_io::ibuf_file cgroup_input{"/proc/self/cgroup"};std::string actual_cgroup;
    fast_io::scan(cgroup_input,fast_io::mnp::whole_get(actual_cgroup));
    fast_io::io::println("SDK_MAIN pid=",::getpid()," exe=",fast_io::mnp::strvw(actual_exe,actual_exe+length)," cgroup=",actual_cgroup);
#endif
    unsigned groups{};
    for(unsigned number=1;number<=9;++number) {
        auto value=sample(static_cast<tag>(number));
        for(unsigned kind=1;kind<=3;++kind) {
            value.kind=kind;sdk::bytes encoded{42};assert(sdk::encode_typed_parameter_event(value,encoded));
            auto expected=raw(number,kind==3?sdk::bytes{}:payload(value),kind);assert(encoded==expected);
            sdk::typed_parameter_event decoded;assert(sdk::decode_typed_parameter_event(expected,decoded));assert(sdk::encode(decoded)==expected);
            if(kind==3) { assert(decoded.tag==value.tag&&decoded.string_value.empty()&&decoded.integer_array_value.empty()); }
        }
        if(number>=4) {
            value.kind=2;value.string_value.clear();value.byte_array_value.clear();value.bool_array_value.clear();
            value.integer_array_value.clear();value.double_array_value.clear();value.string_array_value.clear();
            auto expected=raw(number,{0,0,0,0});assert(sdk::encode(value)==expected);
            sdk::typed_parameter_event decoded;assert(sdk::decode(expected,decoded)&&sdk::encode(decoded)==expected);
        }
        ++groups;
    }
    for(unsigned number:{0u,10u,255u}) reject(raw(number,{}));
    for(unsigned number=1;number<=9;++number) {
        auto valid=raw(number,payload(sample(static_cast<tag>(number))));
        auto short_wire=valid;short_wire.pop_back();reject(short_wire);
        auto trailing=valid;trailing.push_back(0);reject(trailing);
        reject(raw(number,{0},3));
    }
    reject(raw(6,{1,0,0,0,2}));reject(raw(8,{1,0,0,0,0,0,0,0,0,0,240,127}));
    reject(raw(9,{1,0,0,0,1,0,0,0,0}));reject(raw(4,{1,0,0,0,0x80}));
    reject(raw(9,{1,0,0,0,2,0,0,0,0xc0,0x80}));reject(raw(5,{255,255,255,255}));++groups;
    auto invalid=sample(tag::string);invalid.string_value="\x80";reject(invalid);
    invalid=sample(tag::boolean_array);invalid.bool_array_value={2};reject(invalid,true);
    invalid=sample(tag::float64_array);invalid.double_array_value={std::numeric_limits<double>::infinity()};reject(invalid,true);
    invalid=sample(tag::string_array);invalid.string_array_value={std::string{"a\0b",3}};reject(invalid,true);
    invalid=sample(tag::byte_array);invalid.byte_array_value.resize(sdk::parameter_array_detail::maximum_elements+1);reject(invalid,true);++groups;
    auto boundary=sample(tag::string);boundary.string_value.assign(sdk::parameter_array_detail::maximum_bytes-23-boundary.node.size()-boundary.name.size()-4,'x');
    sdk::bytes exact;assert(sdk::encode_typed_parameter_event(boundary,exact)&&exact.size()==sdk::parameter_array_detail::maximum_bytes);
    sdk::typed_parameter_event decoded;assert(sdk::decode(exact,decoded)&&decoded.string_value==boundary.string_value);
    boundary.string_value.push_back('x');reject(boundary,true);
    auto oversized=exact;oversized.push_back(0);forbid_allocation=true;
    bool accepted=sdk::decode(oversized,decoded);forbid_allocation=false;assert(!accepted);++groups;
    auto deletion=sample(tag::string_array);deletion.kind=3;deletion.string_array_value={std::string{"\x80"}};
    assert(sdk::encode(deletion)==raw(9,{},3));++groups;
    fast_io::io::println("PASS typed ParameterEvent SDK ",groups," groups: nine exact tags/new/change/delete, empty values, transactional malformed rejection, UTF8/NUL semantics, allocation-free envelope/element preflight");
}
