#include <uwvm2/imported/ros/capability/manager.h>
#include "../../gateways/common/config.h"
#include <fast_io.h>
#include <array>
#include <atomic>
#include <cassert>
#include <cstdlib>
#include <new>
#include <thread>

using namespace uwvm2::imported::ros;
static thread_local bool record_allocations{};
static thread_local std::size_t allocations{};
void* operator new(std::size_t size)
{
    if(record_allocations) { ++allocations; }
    if(auto p=std::malloc(size?size:1)) { return p; }
    throw std::bad_alloc{};
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p,std::size_t) noexcept { std::free(p); }

static std::size_t checks{};
static void check(bool value) { ++checks;if(!value) { std::abort(); } }
static capability_spec grant(std::string const& suffix)
{
    capability_spec cap{"event","param.subscribe-events","/test/data",::fast_io::concat_std("builtin/param-event-",suffix,"-v1"),{}, {}};
    cap.type_identity=builtin_type_identity(version::ros2,cap.type);
    for(auto name:operation_schema(version::ros2,cap.operation)) { cap.limits.emplace(name,"8"); }
    cap.limits["audit"]="allow-and-deny";cap.limits["audit-max-bytes"]="65536";
    cap.limits["max-rate-hz"]="100";cap.limits["audit-max-rate-hz"]="100";
    cap.limits["timeout-ms"]="2000";cap.limits["max-parameter-bytes"]="4096";
    cap.limits["max-event-queue-bytes"]="32768";cap.limits["max-bytes-per-second"]="1048576";
    cap.limits["parameter-node"]="/test";cap.limits["parameter-name"]="data";
    cap.limits["validator"]="none";cap.limits["overflow-policy"]="reject";
    cap.limits["qos.reliability"]="reliable";cap.limits["qos.durability"]="volatile";
    cap.limits["qos.history"]="keep-last";cap.limits["qos.liveliness"]="automatic";
    for(auto name:{"qos.deadline","qos.lifespan","qos.liveliness-lease-duration"}) { cap.limits[name]="infinite"; }
    auto type=ipc::parameter_event_type(version::ros2,cap.type);
    if(ipc::array_parameter(type)) { cap.limits["max-parameter-elements"]="3"; }
    if(ipc::numeric_parameter(type))
    {
        cap.limits["parameter-min-value"]=ipc::float64_parameter(type)?"-2":"-9223372036854775808";
        cap.limits["parameter-max-value"]=ipc::float64_parameter(type)?"2":"9223372036854775807";
    }
    return cap;
}
static ipc::parameter_value value(ipc::parameter_kind type)
{
    ipc::parameter_value result;result.type=type;result.bool_value=true;result.integer_value=9007199254740993LL;
    result.double_value=-0.;result.string_value=std::string{"hello\0world",11};
    result.byte_array_value={0,128,255};result.bool_array_value={0,1,0};
    result.int64_array_value={std::numeric_limits<std::int64_t>::min(),9007199254740993LL,std::numeric_limits<std::int64_t>::max()};
    result.double_array_value={-0.,std::nextafter(1.,2.),-2.};result.string_array_value={"","hello","\xe4\xb8\xad"};
    return result;
}
int main()
{
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    constexpr std::array names{"bool","int64","float64","string","byte-array","bool-array","int64-array","float64-array","string-array"};
    for(std::size_t index{};index<names.size();++index)
    {
        auto cap=grant(names[index]);auto type=ipc::parameter_event_type(version::ros2,cap.type);
        check(static_cast<bool>(validate_capability(version::ros2,cap)));
        uwvm2::ros_gateway::validate(version::ros2,cap);check(true);
        check(ipc::parameter_event_tag(type)==index+1);
        check(ipc::parameter_event_kind(static_cast<std::uint8_t>(index+1))==type);
        check(ipc::parameter_event_type(version::ros1,cap.type)==ipc::parameter_kind::unsupported);
        auto wrong=cap;wrong.type_identity="builtin:param-string-v1";check(validate_capability(version::ros2,wrong).code==errc::permission_denied);
        for(auto field:{"max-event-queue-bytes","max-bytes-per-second","burst"})
        {
            wrong=cap;wrong.limits[field]=field==std::string_view{"burst"}?"9":"4095";
            check(validate_capability(version::ros2,wrong).code==errc::invalid_argument);
            bool rejected{};
            try { uwvm2::ros_gateway::validate(version::ros2,wrong); }
            catch(std::runtime_error const&) { rejected=true; }
            check(rejected);
        }
        for(auto operation:{"param.get","param.set","param.declare","param.undeclare","param.describe","param.list"})
        { wrong=cap;wrong.operation=operation;check(!backend_supported(version::ros2,operation,wrong.type)); }
        if(ipc::array_parameter(type))
        {
            wrong=cap;wrong.limits.erase("max-parameter-elements");check(validate_capability(version::ros2,wrong).code==errc::invalid_argument);
            for(auto count:{"0","+1","-1","1048577","1.5"})
            { wrong=cap;wrong.limits["max-parameter-elements"]=count;check(validate_capability(version::ros2,wrong).code==errc::invalid_argument); }
        }
        if(ipc::numeric_parameter(type))
        {
            for(auto key:{"parameter-min-value","parameter-max-value"})
            { wrong=cap;wrong.limits.erase(key);check(validate_capability(version::ros2,wrong).code==errc::invalid_argument); }
        }
        else
        { wrong=cap;wrong.limits["parameter-min-value"]="0";wrong.limits["parameter-max-value"]="1";check(validate_capability(version::ros2,wrong).code==errc::invalid_argument); }
        if(ipc::float64_parameter(type))
        {
            wrong=cap;wrong.limits["validator"]="finite";check(static_cast<bool>(validate_capability(version::ros2,wrong)));
            uwvm2::ros_gateway::validate(version::ros2,wrong);check(true);
        }
        auto selected=value(type);
        for(auto kind:{1,2,3})
        {
            ipc::typed_parameter_event event{123,456,static_cast<std::uint8_t>(kind),"/test","data",selected};
            std::vector<std::uint8_t> encoded{7};check(ipc::encode_typed_parameter_event(event,encoded)==0);
            check(encoded[9]==2&&encoded[10]==index+1);
            std::vector<std::uint8_t> payload;check(ipc::encode_parameter(selected,payload)==0);
            check(encoded.size()==32+(kind==3?0:payload.size()));
            if(kind!=3) { check(std::equal(payload.begin(),payload.end(),encoded.begin()+32)); }
            else { check(ipc::get(encoded.data()+28,4)==0); }
            ipc::typed_parameter_event decoded;check(ipc::decode_typed_parameter_event(encoded,decoded)==0);
            check(decoded.value.type==type&&decoded.kind==kind&&decoded.node=="/test"&&decoded.name=="data");
            std::vector<std::uint8_t> roundtrip;check(ipc::encode_typed_parameter_event(decoded,roundtrip)==0&&roundtrip==encoded);
            check(ipc::validate_typed_parameter_event(version::ros2,cap,encoded)==0);
            auto quota=cap;quota.limits["max-parameter-bytes"]=::fast_io::concat_std(encoded.size()-1);
            decoded.node="sentinel";check(ipc::validate_typed_parameter_event(version::ros2,quota,encoded,&decoded)==90&&decoded.node=="sentinel");
            for(auto bad:{0,10,255})
            { auto bytes=encoded;bytes[10]=static_cast<std::uint8_t>(bad);check(ipc::decode_typed_parameter_event(bytes,decoded)==22&&decoded.node=="sentinel"); }
            auto bytes=encoded;bytes.push_back(0);check(ipc::decode_typed_parameter_event(bytes,decoded)==22&&decoded.node=="sentinel");
            if(kind!=3&&ipc::array_parameter(type))
            {
                quota=cap;quota.limits["max-parameter-elements"]="2";
                check(ipc::validate_typed_parameter_event(version::ros2,quota,encoded,&decoded)==90&&decoded.node=="sentinel");
                allocations=0;record_allocations=true;
                auto error=ipc::decode_typed_parameter_event(encoded,decoded,2);
                record_allocations=false;
                check(error==90&&decoded.node=="sentinel"&&allocations==0);
            }
        }
        std::vector<std::uint8_t> encoded;
        allocations=0;record_allocations=true;
        auto error=ipc::encode_typed_parameter_event_view(123,456,2,"/test","data",selected,encoded);
        record_allocations=false;
        check(error==0&&allocations==1);
        if(ipc::array_parameter(type))
        {
            ipc::parameter_value too_many=selected;
            if(type==ipc::parameter_kind::byte_array) { too_many.byte_array_value.push_back(1); }
            if(type==ipc::parameter_kind::boolean_array) { too_many.bool_array_value.push_back(1); }
            if(type==ipc::parameter_kind::int64_array) { too_many.int64_array_value.push_back(1); }
            if(type==ipc::parameter_kind::float64_array) { too_many.double_array_value.push_back(1); }
            if(type==ipc::parameter_kind::string_array) { too_many.string_array_value.emplace_back("x"); }
            check(ipc::validate_parameter_event_value(version::ros2,cap,too_many)==90);
        }
    }
    auto cap=grant("int64-array");cap.limits["parameter-min-value"]="9007199254740993";cap.limits["parameter-max-value"]="9007199254740993";
    auto selected=value(ipc::parameter_kind::int64_array);selected.int64_array_value={9007199254740993LL};
    check(ipc::validate_parameter_event_value(version::ros2,cap,selected)==0);
    selected.int64_array_value={9007199254740992LL};check(ipc::validate_parameter_event_value(version::ros2,cap,selected)==13);
    cap=grant("float64-array");selected=value(ipc::parameter_kind::float64_array);
    selected.double_array_value={std::numeric_limits<double>::quiet_NaN()};check(ipc::validate_parameter_event_value(version::ros2,cap,selected)==22);
    selected.double_array_value={std::nextafter(2.,3.)};check(ipc::validate_parameter_event_value(version::ros2,cap,selected)==13);
    cap=grant("bool-array");selected=value(ipc::parameter_kind::boolean_array);selected.bool_array_value={2};check(ipc::validate_parameter_event_value(version::ros2,cap,selected)==22);
    cap=grant("string-array");selected=value(ipc::parameter_kind::string_array);selected.string_array_value={std::string{"x\0y",3}};check(ipc::validate_parameter_event_value(version::ros2,cap,selected)==22);
    cap=grant("string");selected=value(ipc::parameter_kind::string);selected.string_value="\xc0\x80";check(ipc::validate_parameter_event_value(version::ros2,cap,selected)==22);
    selected=value(ipc::parameter_kind::string);selected.string_value.assign(ipc::parameter_wire_limit-4,'x');
    std::vector<std::uint8_t> sentinel{7};allocations=0;record_allocations=true;
    auto error=ipc::encode_typed_parameter_event_view(123,456,2,"/test","data",selected,sentinel);
    record_allocations=false;check(error==90&&sentinel==std::vector<std::uint8_t>{7}&&allocations==0);
    auto legacy=ipc::encode_parameter_event({123,456,2,"/test","data","hello"});
    check(legacy==std::vector<std::uint8_t>({123,0,0,0,200,1,0,0,2,5,0,0,0,'/','t','e','s','t',4,0,0,0,'d','a','t','a',5,0,0,0,'h','e','l','l','o'}));
    ::fast_io::println("PASS typed parameter events IPC contract checks=",checks," nine tags; complete envelope; explicit elements; zero-allocation rejection; one-allocation encoding");
}
