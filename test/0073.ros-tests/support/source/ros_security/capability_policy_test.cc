#include <uwvm2/imported/ros/capability/manager.h>
#include "../../../../../src/ros/gateway/common/config.h"
#include <atomic>
#include <array>
#include <cassert>
#include <future>
#include <fast_io.h>
#include <fast_io_unit/string.h>
#include <limits>
#include <thread>

using namespace uwvm2::imported::ros;

static capability_spec topic(version v = version::ros1, ::std::string alias = "talk", ::std::string operation = "topic.publish")
{
    capability_spec cap{::std::move(alias), ::std::move(operation), "/test/data", v == version::ros1 ? "std_msgs/String" : "std_msgs/msg/String", {}, {}};
    cap.type_identity = builtin_type_identity(v, cap.type);
    cap.limits = {{"max-message-bytes", "64"}, {"max-rate-hz", "0.001"}, {"burst", "4"}, {"max-bytes-per-second", "128"}, {"max-queue-bytes", "256"}, {"overflow-policy", "reject"}, {"validator", "none"}, {"audit", "allow-and-deny"}, {"audit-max-rate-hz", "10"}, {"audit-burst", "10"}, {"audit-max-records", "128"}, {"audit-max-bytes", "65536"}};
    cap.limits.emplace(cap.operation == "topic.publish" ? "max-publishers" : "max-subscribers", "4");
    if(v == version::ros1)
    {
        cap.limits.emplace("queue-depth", "4"); cap.limits.emplace("transport", "tcpros");
        if(cap.operation == "topic.publish") { cap.limits.emplace("latch", "false"); }
    }
    else
    {
        cap.limits.insert({{"max-inflight", "4"}, {"qos.reliability", "reliable"}, {"qos.durability", "volatile"}, {"qos.history", "keep-last"}, {"qos.depth", "4"}, {"qos.deadline", "infinite"}, {"qos.lifespan", "infinite"}, {"qos.liveliness", "automatic"}, {"qos.liveliness-lease-duration", "infinite"}});
    }
    return cap;
}

static target_policy target(version v = version::ros1)
{
    target_policy t{v, scope::global, "", true, {{"gateway-socket", "/tmp/uwvm-test.sock"}, {"gateway-uid", "1000"}, {"node-name", "test_node"}, {"namespace", "/"}, {"connect-timeout-ms", "1000"}, {"audit-deny-max-rate-hz", "10"}, {"audit-deny-burst", "10"}, {"audit-deny-max-records", "128"}, {"audit-deny-max-bytes", "65536"}}, {}};
    if(v == version::ros1) { t.connection.insert({{"master-uri", "http://127.0.0.1:11311"}, {"advertise-host", "127.0.0.1"}, {"transport", "tcpros"}}); }
    else { t.connection.insert({{"domain-id", "1"}, {"rmw-implementation", "rmw_fastrtps_cpp"}, {"security-mode", "disabled-test"}, {"keystore", "not-applicable"}, {"enclave", "not-applicable"}}); }
    auto cap{topic(v)};
    t.capabilities.emplace(cap.alias, ::std::move(cap));
    return t;
}

static void require(result r, errc expected = errc::success)
{
    if(r.code != expected) { ::fast_io::io::perrln("expected ", static_cast<unsigned>(expected), " got ", static_cast<unsigned>(r.code), ": ", r.detail); ::std::abort(); }
}

static capability_spec operation_cap(version v, ::std::string operation)
{
    capability_spec c{"operation", operation, "/test/data", "builtin/string-v1", {}, {}};
    for(auto field : operation_schema(v, operation)) { c.limits.emplace(field, "1"); }
    for(auto key : {"max-message-bytes", "max-request-bytes", "max-response-bytes", "max-parameter-bytes", "max-graph-result-bytes", "max-goal-bytes", "max-feedback-bytes", "max-result-bytes"})
    { if(c.limits.contains(key)) { c.limits[key] = "512"; } }
    for(auto key : {"max-queue-bytes", "max-event-queue-bytes", "max-bytes-per-second", "audit-max-bytes"})
    { if(c.limits.contains(key)) { c.limits[key] = "65536"; } }
    c.limits["audit"] = "allow-and-deny";
    if(c.limits.contains("validator")) { c.limits["validator"] = "none"; }
    if(c.limits.contains("overflow-policy")) { c.limits["overflow-policy"] = "reject"; }
    if(c.limits.contains("transport")) { c.limits["transport"] = "tcpros"; }
    if(c.limits.contains("latch")) { c.limits["latch"] = "false"; }
    if(c.limits.contains("qos.reliability"))
    {
        c.limits["qos.reliability"] = "reliable"; c.limits["qos.durability"] = "volatile"; c.limits["qos.history"] = "keep-last";
        c.limits["qos.liveliness"] = "automatic";
        for(auto key : {"qos.deadline", "qos.lifespan", "qos.liveliness-lease-duration"}) { c.limits[key] = "infinite"; }
    }
    if(operation.starts_with("graph.")) { c.type = "builtin/graph-list-v1"; c.limits["allowed-name-prefix"] = "/test"; }
    if(operation.starts_with("time.")) { c.type = "builtin/time-v1"; }
    if(operation == "log.write") { c.type = "builtin/log-v1"; c.limits["severity"] = "info"; }
    if(operation.starts_with("service.")) { c.type = v == version::ros1 ? "std_srvs/Trigger" : "std_srvs/srv/Trigger"; }
    if(operation.starts_with("tf."))
    {
        c.type = v == version::ros1 ? "geometry_msgs/TransformStamped" : "geometry_msgs/msg/TransformStamped";
        c.resource = "/tf"; c.limits["source-frame"] = "base_link"; c.limits["target-frame"] = "map";
        c.limits["tf.max-translation-abs"] = "10"; c.limits["tf.quaternion-norm-tolerance"] = "0.000001"; c.limits["validator"] = "transform-bounds";
    }
    if(v == version::ros2 && operation.starts_with("param."))
    {
        c.limits["parameter-node"] = "/test"; c.limits["parameter-name"] = operation == "param.list" ? "not-applicable" : "data";
        if(operation == "param.list") { c.resource = "/test"; }
    }
    if(operation == "param.list" || operation == "param.search") { c.limits["allowed-name-prefix"] = "/test"; }
    if(operation == "param.subscribe-events") { c.type = "rcl_interfaces/msg/ParameterEvent"; }
    if(operation.starts_with("lifecycle."))
    { c.type = operation == "lifecycle.get-state" ? "lifecycle_msgs/srv/GetState" : "lifecycle_msgs/srv/ChangeState"; if(operation == "lifecycle.change-state") { c.limits["allowed-transition"] = "activate"; } }
    if(operation.starts_with("action."))
    { c.type = v == version::ros1 ? "actionlib_tutorials/Fibonacci" : "action_tutorials_interfaces/action/Fibonacci";c.limits["fibonacci.max-order"]="46";c.limits["cancel-scope"]=operation=="action.cancel-own-goal"?"own":operation=="action.cancel-any-goal"?"any":"not-applicable";if(v==version::ros2) { for(auto channel:{"goal-service","cancel-service","result-service","feedback","status"}) { auto prefix{std::string{"qos."}+channel+"."};c.limits[prefix+"reliability"]="reliable";c.limits[prefix+"durability"]=std::string{channel}=="status"?"transient-local":"volatile";c.limits[prefix+"history"]="keep-last";c.limits[prefix+"liveliness"]="automatic";for(auto property:{"deadline","lifespan","liveliness-lease-duration"}) { c.limits[prefix+property]="infinite"; } } } }
    c.type_identity = builtin_type_identity(v, c.type); return c;
}

int main()
{
    unsigned cases{};
    auto run = [&](char const* name, auto fn) { fn(); ++cases; ::fast_io::io::println("PASS ", ::fast_io::mnp::os_c_str(name)); };
    constexpr ::std::array<::std::byte, 7> message{::std::byte{3}, ::std::byte{0}, ::std::byte{0}, ::std::byte{0}, ::std::byte{'a'}, ::std::byte{'b'}, ::std::byte{'c'}};

    auto scalar_cap=[](version v,::std::string operation,::std::string type) {
        auto c{operation_cap(v,::std::move(operation))};c.type=::std::move(type);c.type_identity=builtin_type_identity(v,c.type);
        auto kind{ipc::parameter_type(v,c.type)};
        if(ipc::numeric_parameter(kind))
        { c.limits["parameter-min-value"]=kind==ipc::parameter_kind::int32?"-2147483648":kind==ipc::parameter_kind::int64?"-9223372036854775808":"-1e308";c.limits["parameter-max-value"]=kind==ipc::parameter_kind::int32?"2147483647":kind==ipc::parameter_kind::int64?"9223372036854775807":"1e308"; }
        return c;
    };
    auto gateway_accepts=[](version v,capability_spec const& c) {
        try { uwvm2::ros_gateway::validate(v,c);return true; }catch(::std::runtime_error const&) { return false; }
    };
    auto event_cap=[](::std::string type) {
        auto c{operation_cap(version::ros2,"param.subscribe-events")};c.type=::std::move(type);c.type_identity=builtin_type_identity(version::ros2,c.type);
        auto kind{ipc::parameter_event_type(version::ros2,c.type)};
        if(ipc::numeric_parameter(kind))
        { c.limits["parameter-min-value"]=kind==ipc::parameter_kind::int64?"-9223372036854775808":"-1e308";c.limits["parameter-max-value"]=kind==ipc::parameter_kind::int64?"9223372036854775807":"1e308"; }
        return c;
    };
    auto array_cap=[](version v,::std::string operation,::std::string type) {
        auto c{operation_cap(v,::std::move(operation))};c.type=::std::move(type);c.type_identity=builtin_type_identity(v,c.type);
        auto kind{ipc::parameter_element_kind(ipc::parameter_type(v,c.type))};c.limits["max-parameter-elements"]="8";
        if(ipc::numeric_parameter(kind))
        { c.limits["parameter-min-value"]=kind==ipc::parameter_kind::int32?"-2147483648":kind==ipc::parameter_kind::int64?"-9223372036854775808":"-1e308";c.limits["parameter-max-value"]=kind==ipc::parameter_kind::int32?"2147483647":kind==ipc::parameter_kind::int64?"9223372036854775807":"1e308"; }
        return c;
    };
    run("parameter-arrays-closed-backend-operation-and-trusted-identity", [&] {
        for(auto v:{version::ros1,version::ros2})
        {
            for(auto type:{"builtin/bool-array-v1","builtin/float64-array-v1","builtin/string-array-v1","builtin/int32-array-v1","builtin/int64-array-v1","builtin/byte-array-v1"})
            {
                auto supported{ipc::parameter_type(v,type)!=ipc::parameter_kind::unsupported};
                for(auto operation:{"param.get","param.set","param.declare","param.undeclare"})
                {
                    if(operation_schema(v,operation).empty()) { continue; }
                    auto c{array_cap(v,operation,type)};require(validate_capability(v,c),supported?errc::success:errc::unsupported);
                    assert(gateway_accepts(v,c)==supported);
                    if(supported) { c.type_identity="builtin:param-string-v1";require(validate_capability(v,c),errc::permission_denied);assert(!gateway_accepts(v,c)); }
                }
                for(auto operation:{"param.delete","param.exists","param.search","param.list","param.describe","param.subscribe-events"})
                { if(operation_schema(v,operation).empty()) { continue; }auto c{array_cap(v,operation,type)};require(validate_capability(v,c),errc::unsupported);assert(!gateway_accepts(v,c)); }
            }
        }
        assert(ipc::parameter_type(static_cast<version>(0),"builtin/bool-array-v1")==ipc::parameter_kind::unsupported);
        assert(ipc::parameter_type(version::ros2,"builtin/byte-v1")==ipc::parameter_kind::unsupported);
    });
    run("parameter-array-explicit-element-quota-and-numeric-contract", [&] {
        for(auto v:{version::ros1,version::ros2})
        {
            for(auto type:{"builtin/bool-array-v1","builtin/float64-array-v1","builtin/string-array-v1",v==version::ros1?"builtin/int32-array-v1":"builtin/int64-array-v1"})
            {
                auto c{array_cap(v,"param.get",type)};c.limits.erase("max-parameter-elements");require(validate_capability(v,c),errc::invalid_argument);assert(!gateway_accepts(v,c));
                for(auto limit:{"0","-1","+1","1.5","1048577","18446744073709551616"})
                { c.limits["max-parameter-elements"]=limit;require(validate_capability(v,c),errc::invalid_argument);assert(!gateway_accepts(v,c)); }
                c.limits["max-parameter-elements"]="1048576";c.limits["max-parameter-bytes"]="4";require(validate_capability(v,c));assert(gateway_accepts(v,c));
                c.limits["max-parameter-bytes"]="3";require(validate_capability(v,c),errc::invalid_argument);assert(!gateway_accepts(v,c));
            }
        }
        for(auto type:{"builtin/bool-array-v1","builtin/string-array-v1","builtin/byte-array-v1"})
        { auto c{array_cap(version::ros2,"param.set",type)};c.limits["parameter-min-value"]="0";c.limits["parameter-max-value"]="1";require(validate_capability(version::ros2,c),errc::invalid_argument);assert(!gateway_accepts(version::ros2,c)); }
        for(auto type:{"builtin/int64-array-v1","builtin/float64-array-v1"})
        {
            auto c{array_cap(version::ros2,"param.set",type)};c.limits.erase("parameter-min-value");require(validate_capability(version::ros2,c),errc::invalid_argument);assert(!gateway_accepts(version::ros2,c));
            c.limits["parameter-min-value"]="2";c.limits["parameter-max-value"]="1";require(validate_capability(version::ros2,c),errc::invalid_argument);assert(!gateway_accepts(version::ros2,c));
        }
        auto c{array_cap(version::ros1,"param.set","builtin/int32-array-v1")};c.limits["parameter-min-value"]="-2147483649";require(validate_capability(version::ros1,c),errc::invalid_argument);
        c=array_cap(version::ros2,"param.set","builtin/int64-array-v1");c.limits["parameter-max-value"]="9223372036854775808";require(validate_capability(version::ros2,c),errc::invalid_argument);
        c=array_cap(version::ros2,"param.set","builtin/float64-array-v1");c.limits["validator"]="finite";require(validate_capability(version::ros2,c));assert(gateway_accepts(version::ros2,c));
        c.limits["parameter-max-value"]="inf";require(validate_capability(version::ros2,c),errc::invalid_argument);
    });
    run("parameter-array-wire-empty-and-all-closed-kinds-roundtrip", [&] {
        for(auto type:{ipc::parameter_kind::boolean_array,ipc::parameter_kind::int32_array,ipc::parameter_kind::int64_array,ipc::parameter_kind::float64_array,ipc::parameter_kind::string_array,ipc::parameter_kind::byte_array})
        {
            ipc::parameter_value value;value.type=type;::std::vector<::std::uint8_t> encoded{7};assert(!ipc::encode_parameter(value,encoded));assert(encoded==::std::vector<::std::uint8_t>({0,0,0,0}));
            ipc::parameter_value output;output.type=ipc::parameter_kind::string;output.string_value="sentinel";assert(!ipc::decode_parameter(type,encoded.data(),encoded.size(),output));assert(output.type==type&&ipc::parameter_array_size(output)==0);
            value.bool_array_value={0,1,0};value.int32_array_value={::std::numeric_limits<::std::int32_t>::min(),0,::std::numeric_limits<::std::int32_t>::max()};
            value.int64_array_value={::std::numeric_limits<::std::int64_t>::min(),9007199254740993LL,::std::numeric_limits<::std::int64_t>::max()};
            value.double_array_value={-0.,::std::nextafter(1.,2.),::std::numeric_limits<double>::min()};value.string_array_value={"","unicode \xe4\xb8\xad\xf0\x9f\x98\x80","\t\n\r"};value.byte_array_value={0,128,255};
            assert(!ipc::encode_parameter(value,encoded)&&!ipc::decode_parameter(type,encoded.data(),encoded.size(),output));assert(ipc::parameter_array_size(output)==3);
            ::std::vector<::std::uint8_t> again;assert(!ipc::encode_parameter(output,again)&&again==encoded);
            if(type==ipc::parameter_kind::int64_array) { assert(output.int64_array_value==value.int64_array_value); }
            if(type==ipc::parameter_kind::float64_array) { for(::std::size_t i{};i<3;++i) { assert(::std::bit_cast<::std::uint64_t>(output.double_array_value[i])==::std::bit_cast<::std::uint64_t>(value.double_array_value[i])); } }
        }
    });
    run("parameter-array-malformed-preflight-preserves-output-before-quota", [&] {
        ipc::parameter_value sentinel;sentinel.type=ipc::parameter_kind::string;sentinel.string_value="sentinel";sentinel.int64_array_value={42};
        auto check=[&](ipc::parameter_kind type,::std::vector<::std::uint8_t> bytes,unsigned error,::std::size_t quota=8) {
            auto output{sentinel};assert(ipc::decode_parameter(type,bytes.data(),bytes.size(),output,quota)==error);
            assert(output.type==sentinel.type&&output.string_value=="sentinel"&&output.int64_array_value==::std::vector<::std::int64_t>{42});
            ::std::uint32_t count{42};assert(ipc::preflight_parameter_array(type,bytes.data(),bytes.size(),quota,count)==error&&count==42);
        };
        check(ipc::parameter_kind::int64_array,{255,255,255,255},22,1);
        check(ipc::parameter_kind::boolean_array,{1,0,0},22);
        check(ipc::parameter_kind::boolean_array,{0,0,0,0,1},22);
        check(ipc::parameter_kind::boolean_array,{1,0,0,0,2},22,0);
        check(ipc::parameter_kind::int64_array,{1,0,0,0,0},22);
        check(ipc::parameter_kind::string_array,{1,0,0,0,255,255,255,255},22);
        check(ipc::parameter_kind::string_array,{1,0,0,0,2,0,0,0,0xc0,0x80},22);
        check(ipc::parameter_kind::string_array,{1,0,0,0,1,0,0,0,0},22);
        check(ipc::parameter_kind::boolean_array,{3,0,0,0,0,1,0},90,2);
        auto output{sentinel};assert(ipc::decode_parameter(ipc::parameter_kind::byte_array,nullptr,4,output)==22&&output.string_value=="sentinel");
        assert(ipc::decode_parameter(static_cast<ipc::parameter_kind>(255),nullptr,0,output)==95&&output.string_value=="sentinel");
    });
    run("parameter-array-string-transport-is-explicit-without-changing-scalar", [&] {
        for(auto v:{version::ros1,version::ros2})
        {
            assert(ipc::parameter_array_string_valid(v,"\t\n\r\xe4\xb8\xad"));
            assert(!ipc::parameter_array_string_valid(v,::std::string_view{"a\0b",3}));
            assert(!ipc::parameter_array_string_valid(v,"\xed\xa0\x80"));
            for(auto text:{"\x01","\xef\xbf\xbe","\xef\xbf\xbf"})
            {
                assert(ipc::parameter_array_string_valid(v,text)==(v==version::ros2));
                auto c{array_cap(v,"param.set","builtin/string-array-v1")};ipc::parameter_value value;value.type=ipc::parameter_kind::string_array;value.string_array_value={"valid",text};::std::vector<::std::uint8_t> bytes;assert(!ipc::encode_parameter(value,bytes));
                ipc::parameter_value output;output.string_value="sentinel";assert(ipc::validate_parameter_value(v,c,bytes.data(),bytes.size(),&output)==(v==version::ros1?22u:0u));if(v==version::ros1) { assert(output.string_value=="sentinel"); }
            }
        }
        ipc::parameter_value value;value.type=ipc::parameter_kind::string_array;value.string_array_value={::std::string{"a\0b",3}};::std::vector<::std::uint8_t> bytes{7};assert(ipc::encode_parameter(value,bytes)==22&&bytes==::std::vector<::std::uint8_t>{7});
        value.type=ipc::parameter_kind::string;value.string_value=::std::string{"a\0b",3};assert(!ipc::encode_parameter(value,bytes));ipc::parameter_value output;assert(!ipc::decode_parameter(value.type,bytes.data(),bytes.size(),output)&&output.string_value==value.string_value);
    });
    run("parameter-array-every-numeric-element-exact-bounds-and-finite", [&] {
        auto c{array_cap(version::ros2,"param.set","builtin/int64-array-v1")};c.limits["parameter-min-value"]="9007199254740993";c.limits["parameter-max-value"]="9223372036854775807";
        ipc::parameter_value value;value.type=ipc::parameter_kind::int64_array;value.int64_array_value={9007199254740993LL,::std::numeric_limits<::std::int64_t>::max()};::std::vector<::std::uint8_t> bytes;assert(!ipc::encode_parameter(value,bytes));assert(!ipc::validate_parameter_value(version::ros2,c,bytes.data(),bytes.size()));
        value.int64_array_value[1]=9007199254740992LL;assert(!ipc::encode_parameter(value,bytes));ipc::parameter_value output;output.int64_array_value={42};assert(ipc::validate_parameter_value(version::ros2,c,bytes.data(),bytes.size(),&output)==13&&output.int64_array_value==::std::vector<::std::int64_t>{42});
        auto span=[&] { return ::std::span<::std::byte const>{reinterpret_cast<::std::byte const*>(bytes.data()),bytes.size()}; };require(validate_message(version::ros2,c,span()),errc::permission_denied);c.operation="param.get";require(validate_response_payload(version::ros2,c,span()),errc::permission_denied);
        c=array_cap(version::ros1,"param.set","builtin/int32-array-v1");c.limits["parameter-min-value"]="-1";c.limits["parameter-max-value"]="1";value.type=ipc::parameter_kind::int32_array;value.int32_array_value={0,-2};assert(!ipc::encode_parameter(value,bytes)&&ipc::validate_parameter_value(version::ros1,c,bytes.data(),bytes.size())==13);
        c=array_cap(version::ros2,"param.set","builtin/float64-array-v1");c.limits["parameter-min-value"]="-0.5";c.limits["parameter-max-value"]="0.5";value.type=ipc::parameter_kind::float64_array;value.double_array_value={-0.,0.5};assert(!ipc::encode_parameter(value,bytes)&&!ipc::validate_parameter_value(version::ros2,c,bytes.data(),bytes.size()));
        value.double_array_value[1]=::std::nextafter(0.5,1.);assert(!ipc::encode_parameter(value,bytes)&&ipc::validate_parameter_value(version::ros2,c,bytes.data(),bytes.size())==13);
        for(auto nonfinite:{::std::numeric_limits<double>::infinity(),-::std::numeric_limits<double>::infinity(),::std::numeric_limits<double>::quiet_NaN()})
        { value.double_array_value[1]=nonfinite;auto prior{bytes};assert(ipc::encode_parameter(value,bytes)==22&&bytes==prior);bytes.clear();ipc::put(bytes,1,4);ipc::put(bytes,::std::bit_cast<::std::uint64_t>(nonfinite),8);assert(ipc::decode_parameter(value.type,bytes.data(),bytes.size(),output)==22&&output.int64_array_value==::std::vector<::std::int64_t>{42}); }
    });
    run("parameter-array-full-envelope-and-element-quotas-independent", [&] {
        auto c{array_cap(version::ros2,"param.set","builtin/byte-array-v1")};c.limits["max-parameter-bytes"]="4";ipc::parameter_value value;value.type=ipc::parameter_kind::byte_array;::std::vector<::std::uint8_t> bytes;assert(!ipc::encode_parameter(value,bytes)&&!ipc::validate_parameter_value(version::ros2,c,bytes.data(),bytes.size()));
        value.byte_array_value={0};assert(!ipc::encode_parameter(value,bytes));ipc::parameter_value output;output.byte_array_value={42};assert(ipc::validate_parameter_value(version::ros2,c,bytes.data(),bytes.size(),&output)==90&&output.byte_array_value==::std::vector<::std::uint8_t>{42});
        c.limits["max-parameter-bytes"]="8";c.limits["max-parameter-elements"]="2";value.byte_array_value={0,128,255};assert(!ipc::encode_parameter(value,bytes)&&ipc::validate_parameter_value(version::ros2,c,bytes.data(),bytes.size(),&output)==90&&output.byte_array_value==::std::vector<::std::uint8_t>{42});
        value.byte_array_value.resize(ipc::parameter_max_elements+1);auto prior{bytes};assert(ipc::encode_parameter(value,bytes)==90&&bytes==prior);
        value.type=ipc::parameter_kind::string_array;value.string_array_value={::std::string(ipc::parameter_wire_limit,'x')};assert(ipc::encode_parameter(value,bytes)==90&&bytes==prior);
        c=array_cap(version::ros2,"param.set","builtin/string-array-v1");c.limits["max-parameter-bytes"]="8";value.string_array_value={""};assert(!ipc::encode_parameter(value,bytes)&&!ipc::validate_parameter_value(version::ros2,c,bytes.data(),bytes.size()));value.string_array_value={"x"};assert(!ipc::encode_parameter(value,bytes)&&ipc::validate_parameter_value(version::ros2,c,bytes.data(),bytes.size())==90);
    });
    run("array-only-quota-rejected-on-scalars-events-and-all-six-targets", [&] {
        for(auto v:{version::ros1,version::ros2})
        {
            for(auto type:{"builtin/string-v1","builtin/bool-v1","builtin/float64-v1"})
            { auto c{scalar_cap(v,"param.get",type)};c.limits["max-parameter-elements"]="8";require(validate_capability(v,c),errc::invalid_argument);assert(!gateway_accepts(v,c)); }
            for(auto target_scope:{scope::global,scope::single,scope::group})
            {
                policy_store p;::std::string name{target_scope==scope::global?"":"owner"};if(target_scope!=scope::global) { require(p.create_target(v,target_scope,name)); }if(target_scope==scope::group) { require(p.bind_module(v,name,"owner")); }
                auto c{array_cap(v,"param.get","builtin/string-array-v1")};c.limits.erase("max-parameter-elements");require(p.grant(v,target_scope,name,c));require(p.set_capability(v,target_scope,name,c.alias,"max-parameter-elements","8"));require(p.set_capability(v,target_scope,name,c.alias,"max-parameter-elements","8"),errc::conflict);
                ::std::array<::std::string,1> modules{"owner"};require(p.validate(modules));
            }
        }
        for(auto type:{"rcl_interfaces/msg/ParameterEvent","builtin/param-event-bool-v1","builtin/param-event-int64-v1","builtin/param-event-float64-v1"})
        { auto c{type==::std::string_view{"rcl_interfaces/msg/ParameterEvent"}?operation_cap(version::ros2,"param.subscribe-events"):event_cap(type)};c.limits["max-parameter-elements"]="8";require(validate_capability(version::ros2,c),errc::invalid_argument);assert(!gateway_accepts(version::ros2,c)); }
    });
    run("array-read-capability-does-not-grant-write-another-kind-or-event", [&] {
        auto t{target(version::ros2)};t.capabilities.clear();auto c{array_cap(version::ros2,"param.get","builtin/int64-array-v1")};t.capabilities.emplace(c.alias,c);
        capability_manager manager;int instance{};require(manager.register_instance(&instance,1,"array",nullptr,&t));::std::uint64_t handle{};require(manager.open(&instance,1,version::ros2,c.alias,handle));
        authorization lease;require(manager.authorize(&instance,1,version::ros2,handle,"param.set",{},lease),errc::permission_denied);require(manager.authorize(&instance,1,version::ros2,handle,"param.subscribe-events",{},lease),errc::permission_denied);require(manager.authorize(&instance,1,version::ros2,handle,"param.get",{},lease));
        c.type="builtin/float64-array-v1";require(validate_capability(version::ros2,c),errc::permission_denied);
    });
    run("typed-event-contract-closed-to-exact-ros2-subscription", [&] {
        for(auto type:{"builtin/param-event-bool-v1","builtin/param-event-int64-v1","builtin/param-event-float64-v1"})
        {
            auto c{event_cap(type)};require(validate_capability(version::ros2,c));assert(gateway_accepts(version::ros2,c));
            assert(ipc::parameter_event_identity(version::ros2,type)==c.type_identity);
            c.limits["max-parameter-bytes"]="1";require(validate_capability(version::ros2,c));assert(gateway_accepts(version::ros2,c));
            c.type_identity="builtin:param-string-v1";require(validate_capability(version::ros2,c),errc::permission_denied);assert(!gateway_accepts(version::ros2,c));
            for(auto operation:{"param.get","param.set","param.declare","param.undeclare","param.describe","param.list"})
            {
                auto wrong{operation_cap(version::ros2,operation)};wrong.type=type;wrong.type_identity=builtin_type_identity(version::ros2,type);
                require(validate_capability(version::ros2,wrong),errc::unsupported);assert(!gateway_accepts(version::ros2,wrong));
            }
            auto wrong{operation_cap(version::ros1,"param.get")};wrong.type=type;wrong.type_identity=builtin_type_identity(version::ros2,type);
            require(validate_capability(version::ros1,wrong),errc::unsupported);assert(!gateway_accepts(version::ros1,wrong));
        }
        auto c{event_cap("builtin/param-event-int32-v1")};require(validate_capability(version::ros2,c),errc::unsupported);assert(!gateway_accepts(version::ros2,c));
    });
    run("typed-event-numeric-bounds-mandatory-exact-and-bool-inapplicable", [&] {
        for(auto type:{"builtin/param-event-int64-v1","builtin/param-event-float64-v1"})
        {
            auto good{event_cap(type)};
            for(auto key:{"parameter-min-value","parameter-max-value"})
            { auto c{good};c.limits.erase(key);require(validate_capability(version::ros2,c),errc::invalid_argument);assert(!gateway_accepts(version::ros2,c)); }
            for(auto bad:{"+1"," 1","1 ","0x1","NaN","Infinity","1x","","1e9999"})
            { auto c{good};c.limits["parameter-max-value"]=bad;require(validate_capability(version::ros2,c),errc::invalid_argument);assert(!gateway_accepts(version::ros2,c)); }
            auto c{good};c.limits["parameter-min-value"]="2";c.limits["parameter-max-value"]="1";require(validate_capability(version::ros2,c),errc::invalid_argument);assert(!gateway_accepts(version::ros2,c));
        }
        for(auto type:{"builtin/param-event-bool-v1","rcl_interfaces/msg/ParameterEvent"})
        { auto c{event_cap(type)};c.limits["parameter-min-value"]="0";c.limits["parameter-max-value"]="1";require(validate_capability(version::ros2,c),errc::invalid_argument);assert(!gateway_accepts(version::ros2,c)); }
        auto c{event_cap("builtin/param-event-int64-v1")};c.limits["parameter-max-value"]="9223372036854775808";require(validate_capability(version::ros2,c),errc::invalid_argument);assert(!gateway_accepts(version::ros2,c));
        c=event_cap("builtin/param-event-float64-v1");c.limits["validator"]="finite";require(validate_capability(version::ros2,c));assert(gateway_accepts(version::ros2,c));
    });
    run("typed-event-codec-roundtrip-and-deletion-tag", [&] {
        for(auto type:{ipc::parameter_kind::boolean,ipc::parameter_kind::int64,ipc::parameter_kind::float64})
        {
            for(auto kind:{1,2,3})
            {
                ipc::typed_parameter_event event{123,456,static_cast<::std::uint8_t>(kind),"/test","data",{}};
                event.value.type=type;event.value.bool_value=true;event.value.integer_value=9007199254740993LL;event.value.double_value=-0.0;
                ::std::vector<::std::uint8_t> bytes;assert(!ipc::encode_typed_parameter_event(event,bytes));
                assert(bytes.size()==23+event.node.size()+event.name.size()+(kind==3?0:ipc::parameter_width(type)));
                assert(bytes[9]==2&&bytes[10]==ipc::parameter_event_tag(type));
                ipc::typed_parameter_event output;assert(!ipc::decode_typed_parameter_event(bytes,output)&&output.value.type==type&&output.kind==kind);
                if(kind!=3)
                { if(type==ipc::parameter_kind::boolean) { assert(output.value.bool_value); }else if(type==ipc::parameter_kind::int64) { assert(output.value.integer_value==9007199254740993LL); }else { assert(::std::bit_cast<::std::uint64_t>(output.value.double_value)==::std::bit_cast<::std::uint64_t>(-0.0)); } }
                ::std::vector<::std::uint8_t> encoded;assert(!ipc::encode_typed_parameter_event(output,encoded)&&encoded==bytes);
            }
        }
    });
    run("typed-event-malformed-wire-is-transactional", [&] {
        ipc::typed_parameter_event event{123,456,2,"/test","data",{}};event.value.type=ipc::parameter_kind::boolean;event.value.bool_value=true;
        ::std::vector<::std::uint8_t> good;assert(!ipc::encode_typed_parameter_event(event,good));
        auto denied=[&](::std::vector<::std::uint8_t> const& bytes) { ipc::typed_parameter_event output;output.node="sentinel";output.value.integer_value=42;assert(ipc::decode_typed_parameter_event(bytes,output)==22&&output.node=="sentinel"&&output.value.integer_value==42); };
        for(auto index:{8u,9u,10u}) { auto bytes{good};bytes[index]=0;denied(bytes); }
        auto bytes{good};bytes.back()=2;denied(bytes);bytes=good;bytes.pop_back();denied(bytes);bytes=good;bytes.push_back(0);denied(bytes);
        bytes=good;bytes[8]=3;denied(bytes);bytes=good;bytes[0]=255;bytes[1]=255;bytes[2]=255;bytes[3]=255;denied(bytes);
        bytes=good;bytes[4]=0;bytes[5]=202;bytes[6]=154;bytes[7]=59;denied(bytes);
        bytes=good;bytes[11]=255;bytes[12]=255;bytes[13]=255;bytes[14]=255;denied(bytes);bytes=good;bytes[15]='x';denied(bytes);
        event.value.type=ipc::parameter_kind::float64;event.value.double_value=::std::numeric_limits<double>::quiet_NaN();bytes={7};assert(ipc::encode_typed_parameter_event(event,bytes)==22&&bytes==::std::vector<::std::uint8_t>{7});
        event.value.double_value=1.;assert(!ipc::encode_typed_parameter_event(event,bytes));auto bits{::std::bit_cast<::std::uint64_t>(::std::numeric_limits<double>::infinity())};
        for(unsigned i{};i!=8;++i) { bytes[bytes.size()-8+i]=static_cast<::std::uint8_t>(bits>>(8*i)); }denied(bytes);
        event.node="/bad//node";bytes={7};assert(ipc::encode_typed_parameter_event(event,bytes)==22&&bytes==::std::vector<::std::uint8_t>{7});
    });
    run("typed-event-selection-envelope-quota-and-exact-int64-bounds", [&] {
        auto c{event_cap("builtin/param-event-int64-v1")};c.limits["parameter-min-value"]="9007199254740993";c.limits["parameter-max-value"]="9007199254740993";
        ipc::typed_parameter_event event{123,456,2,"/test","data",{}};event.value.type=ipc::parameter_kind::int64;event.value.integer_value=9007199254740993LL;
        ::std::vector<::std::uint8_t> bytes;
        auto check=[&](errc code) { assert(!ipc::encode_typed_parameter_event(event,bytes));require(validate_message(version::ros2,c,{reinterpret_cast<::std::byte const*>(bytes.data()),bytes.size()}),code);ipc::typed_parameter_event output;output.node="sentinel";auto error{ipc::validate_typed_parameter_event(version::ros2,c,bytes,&output)};assert(error==static_cast<unsigned>(code));if(error) { assert(output.node=="sentinel"); } };
        check(errc::success);event.value.integer_value=9007199254740992LL;check(errc::permission_denied);event.value.integer_value=9007199254740994LL;check(errc::permission_denied);
        event.value.integer_value=9007199254740993LL;event.node="/other";check(errc::permission_denied);event.node="/test";event.name="other";check(errc::permission_denied);event.name="data";
        event.value.type=ipc::parameter_kind::boolean;check(errc::permission_denied);event.value.type=ipc::parameter_kind::int64;
        c.limits["max-parameter-bytes"]="1";require(validate_capability(version::ros2,c));check(errc::too_large);
    });
    run("typed-event-native-deletion-prior-value-contract", [&] {
        auto c{event_cap("builtin/param-event-float64-v1")};c.limits["parameter-min-value"]="-0.5";c.limits["parameter-max-value"]="0.5";
        ipc::parameter_value value;value.type=ipc::parameter_kind::float64;
        for(auto number:{-0.5,-0.0,0.5,::std::nextafter(0.5,1.)})
        { value.double_value=number;assert(ipc::validate_parameter_event_value(version::ros2,c,value)==(number>0.5?13u:0u)); }
        value.double_value=::std::numeric_limits<double>::infinity();assert(ipc::validate_parameter_event_value(version::ros2,c,value)==22);
        ipc::typed_parameter_event event{123,456,3,"/test","data",value};::std::vector<::std::uint8_t> bytes;
        assert(!ipc::encode_typed_parameter_event(event,bytes));assert(!ipc::validate_typed_parameter_event(version::ros2,c,bytes));
        value.type=ipc::parameter_kind::int64;assert(ipc::validate_parameter_event_value(version::ros2,c,value)==13);
        c.operation="param.get";assert(ipc::validate_parameter_event_value(version::ros2,c,value)==95);
    });
    run("typed-event-grant-no-read-or-write-and-legacy-wire-unchanged", [&] {
        auto t{target(version::ros2)};t.capabilities.clear();auto c{event_cap("builtin/param-event-bool-v1")};t.capabilities.emplace(c.alias,c);
        capability_manager manager;int instance{};require(manager.register_instance(&instance,1,"event",nullptr,&t));::std::uint64_t handle{};require(manager.open(&instance,1,version::ros2,c.alias,handle));
        authorization lease;require(manager.authorize(&instance,1,version::ros2,handle,"param.get",{},lease),errc::permission_denied);require(manager.authorize(&instance,1,version::ros2,handle,"param.set",{},lease),errc::permission_denied);
        auto legacy{ipc::encode_parameter_event({123,456,2,"/test","data","hello"})};
        assert(legacy.size()==35&&legacy[8]==2&&legacy[9]==5&&legacy[13]=='/');ipc::parameter_event output;assert(ipc::decode_parameter_event(legacy,output)&&output.value=="hello");
        ipc::typed_parameter_event typed;assert(ipc::decode_typed_parameter_event(legacy,typed)==22);
        auto lc{operation_cap(version::ros2,"param.subscribe-events")};require(validate_message(version::ros2,lc,{reinterpret_cast<::std::byte const*>(legacy.data()),legacy.size()}));
        typed={123,456,2,"/test","data",{}};typed.value.type=ipc::parameter_kind::boolean;::std::vector<::std::uint8_t> bytes;assert(!ipc::encode_typed_parameter_event(typed,bytes));
        require(validate_message(version::ros2,lc,{reinterpret_cast<::std::byte const*>(bytes.data()),bytes.size()}),errc::invalid_argument);
    });
    run("scalar-parameter-types-are-explicit-per-backend-and-operation", [&] {
        for(auto v:{version::ros1,version::ros2})
        {
            for(auto type:{"builtin/bool-v1",v==version::ros1?"builtin/int32-v1":"builtin/int64-v1","builtin/float64-v1"})
            {
                for(auto operation:{"param.get","param.set","param.declare","param.undeclare"})
                {
                    if(v==version::ros1&&(operation==::std::string_view{"param.declare"}||operation==::std::string_view{"param.undeclare"})) { continue; }
                    auto c{scalar_cap(v,operation,type)};require(validate_capability(v,c));assert(gateway_accepts(v,c));
                    c.limits["max-parameter-bytes"]=::std::to_string(ipc::parameter_width(ipc::parameter_type(v,type))-1);
                    require(validate_capability(v,c),errc::invalid_argument);assert(!gateway_accepts(v,c));
                }
            }
            for(auto operation:{"param.list","param.describe","param.subscribe-events","param.delete","param.exists","param.search"})
            {
                if(operation_schema(v,operation).empty()) { continue; }auto c{scalar_cap(v,operation,"builtin/bool-v1")};
                require(validate_capability(v,c),errc::unsupported);assert(!gateway_accepts(v,c));
            }
            auto wrong{scalar_cap(v,"param.get",v==version::ros1?"builtin/int64-v1":"builtin/int32-v1")};
            wrong.type_identity=v==version::ros1?"builtin:param-int64-v1":"builtin:param-int32-v1";
            require(validate_capability(v,wrong),errc::unsupported);assert(!gateway_accepts(v,wrong));
        }
    });
    run("numeric-parameter-bounds-mandatory-exact-and-not-expanded", [&] {
        for(auto type:{"builtin/int64-v1","builtin/float64-v1"})
        {
            auto good{scalar_cap(version::ros2,"param.set",type)};
            for(auto key:{"parameter-min-value","parameter-max-value"})
            { auto c{good};c.limits.erase(key);require(validate_capability(version::ros2,c),errc::invalid_argument);assert(!gateway_accepts(version::ros2,c)); }
            for(auto bad:{"+1"," 1","1 ","0x1","NaN","Infinity","1x","","1e9999"})
            { auto c{good};c.limits["parameter-max-value"]=bad;require(validate_capability(version::ros2,c),errc::invalid_argument);assert(!gateway_accepts(version::ros2,c)); }
            auto c{good};c.limits["parameter-min-value"]="2";c.limits["parameter-max-value"]="1";require(validate_capability(version::ros2,c),errc::invalid_argument);assert(!gateway_accepts(version::ros2,c));
        }
        auto i{scalar_cap(version::ros2,"param.set","builtin/int64-v1")};i.limits["parameter-max-value"]="9223372036854775808";require(validate_capability(version::ros2,i),errc::invalid_argument);assert(!gateway_accepts(version::ros2,i));
        auto i32{scalar_cap(version::ros1,"param.set","builtin/int32-v1")};i32.limits["parameter-min-value"]="-2147483649";require(validate_capability(version::ros1,i32),errc::invalid_argument);assert(!gateway_accepts(version::ros1,i32));
        for(auto type:{"builtin/bool-v1","builtin/string-v1"})
        { auto c{scalar_cap(version::ros2,"param.set",type)};c.limits["parameter-min-value"]="0";c.limits["parameter-max-value"]="1";require(validate_capability(version::ros2,c),errc::invalid_argument);assert(!gateway_accepts(version::ros2,c)); }
        double unchanged{42};assert(!ipc::parse_parameter_real("1e-9999",unchanged)&&unchanged==42);
        ::std::int64_t exact{};assert(ipc::parse_parameter_integer("9007199254740993",exact)&&exact==9007199254740993LL);
    });
    run("scalar-parameter-payload-errors-preserve-output-and-core-admission", [&] {
        auto c{scalar_cap(version::ros2,"param.set","builtin/int64-v1")};c.limits["parameter-min-value"]="9007199254740993";c.limits["parameter-max-value"]="9007199254740993";
        ipc::parameter_value value;value.type=ipc::parameter_kind::int64;value.integer_value=9007199254740993LL;
        ::std::vector<::std::uint8_t> bytes;assert(!ipc::encode_parameter(value,bytes));
        auto span=[&] { return ::std::span<::std::byte const>{reinterpret_cast<::std::byte const*>(bytes.data()),bytes.size()}; };
        require(validate_message(version::ros2,c,span()));c.operation="param.get";require(validate_response_payload(version::ros2,c,span()));
        value.integer_value=9007199254740992LL;assert(!ipc::encode_parameter(value,bytes));ipc::parameter_value output;output.integer_value=42;
        assert(ipc::validate_parameter_value(version::ros2,c,bytes.data(),bytes.size(),&output)==13&&output.integer_value==42);require(validate_response_payload(version::ros2,c,span()),errc::permission_denied);
        bytes.pop_back();assert(ipc::validate_parameter_value(version::ros2,c,bytes.data(),bytes.size(),&output)==22&&output.integer_value==42);require(validate_response_payload(version::ros2,c,span()),errc::invalid_argument);
        bytes.resize(513);assert(ipc::validate_parameter_value(version::ros2,c,bytes.data(),bytes.size(),&output)==90&&output.integer_value==42);
        c=scalar_cap(version::ros2,"param.set","builtin/bool-v1");bytes={2};assert(ipc::validate_parameter_value(version::ros2,c,bytes.data(),bytes.size())==22);require(validate_message(version::ros2,c,span()),errc::invalid_argument);
    });
    run("float-parameter-rejects-nonfinite-and-enforces-inclusive-bounds", [&] {
        auto c{scalar_cap(version::ros1,"param.set","builtin/float64-v1")};c.limits["parameter-min-value"]="-0.5";c.limits["parameter-max-value"]="0.5";c.limits["validator"]="finite";
        require(validate_capability(version::ros1,c));assert(gateway_accepts(version::ros1,c));
        for(auto n:{-0.5,-0.0,0.5,::std::nextafter(0.5,1.)})
        { ipc::parameter_value value;value.type=ipc::parameter_kind::float64;value.double_value=n;::std::vector<::std::uint8_t> bytes;assert(!ipc::encode_parameter(value,bytes));assert(ipc::validate_parameter_value(version::ros1,c,bytes.data(),bytes.size())==(n>0.5?13u:0u)); }
        for(auto n:{::std::numeric_limits<double>::quiet_NaN(),::std::numeric_limits<double>::infinity(),-::std::numeric_limits<double>::infinity()})
        {
            auto bits{::std::bit_cast<::std::uint64_t>(n)};::std::vector<::std::uint8_t> bytes;ipc::put(bytes,bits,8);ipc::parameter_value output;output.double_value=42;
            assert(ipc::validate_parameter_value(version::ros1,c,bytes.data(),bytes.size(),&output)==22&&output.double_value==42);
            output.type=ipc::parameter_kind::float64;output.double_value=n;::std::vector<::std::uint8_t> encoded{7};assert(ipc::encode_parameter(output,encoded)==22&&encoded==::std::vector<::std::uint8_t>{7});
        }
    });
    run("scalar-read-grant-does-not-grant-write-or-another-type", [&] {
        auto t{target(version::ros2)};t.capabilities.clear();auto c{scalar_cap(version::ros2,"param.get","builtin/bool-v1")};t.capabilities.emplace(c.alias,c);
        capability_manager manager;int instance{};require(manager.register_instance(&instance,1,"scalar",nullptr,&t));::std::uint64_t handle{};require(manager.open(&instance,1,version::ros2,c.alias,handle));
        authorization lease;require(manager.authorize(&instance,1,version::ros2,handle,"param.set",{},lease),errc::permission_denied);
        require(manager.authorize(&instance,1,version::ros2,handle,"param.get",{},lease));
        c.type="builtin/int64-v1";c.type_identity="builtin:param-bool-v1";c.limits["parameter-min-value"]="0";c.limits["parameter-max-value"]="1";require(validate_capability(version::ros2,c),errc::permission_denied);
    });

    run("default-deny-independent-versions", [] {
        policy_store p{};
        assert(!p.effective(version::ros1, "camera_a")->enabled);
        assert(!p.effective(version::ros2, "camera_a")->enabled);
        require(p.validate({}));
    });
    run("group-no-global-inheritance", [] {
        policy_store p{};
        require(p.grant(version::ros1, scope::global, "", topic()));
        require(p.create_target(version::ros1, scope::group, "sensors"));
        require(p.bind_module(version::ros1, "sensors", "camera_a"));
        assert(p.effective(version::ros1, "camera_a")->capabilities.empty());
        assert(p.effective(version::ros1, "other")->capabilities.size() == 1);
        assert(p.effective(version::ros2, "camera_a")->target_scope == scope::global);
        ::std::array<::std::string, 1> modules{"camera_a"}; require(p.validate(modules));
    });
    run("same-named-ros-groups-independent", [] {
        policy_store p{};
        require(p.create_target(version::ros1, scope::group, "sensors")); require(p.create_target(version::ros2, scope::group, "sensors"));
        require(p.bind_module(version::ros1, "sensors", "camera_a"));
        assert(p.effective(version::ros2, "camera_a")->target_scope == scope::global);
    });
    run("single-group-and-multiple-group-conflict", [] {
        policy_store p{};
        require(p.create_target(version::ros1, scope::single, "camera_a")); require(p.create_target(version::ros1, scope::group, "sensors"));
        require(p.bind_module(version::ros1, "sensors", "camera_a"), errc::conflict);
        require(p.bind_module(version::ros1, "sensors", "camera_b")); require(p.create_target(version::ros1, scope::group, "other"));
        require(p.bind_module(version::ros1, "other", "camera_b"), errc::conflict);
        require(p.create_target(version::ros1, scope::single, "camera_b"), errc::conflict);
        require(p.create_target(version::ros2, scope::single, "camera_a"));
    });
    run("bindings-validated-against-real-loaded-names", [] {
        policy_store p{}; require(p.create_target(version::ros1, scope::group, "sensors")); require(p.bind_module(version::ros1, "sensors", "absent"));
        require(p.validate({}), errc::unknown_module); require(p.validate({}, false));
        ::std::array<::std::string, 2> modules{"absent", "absent"}; require(p.validate(modules), errc::conflict);
    });
    run("every-connection-field-explicit", [] {
        for(auto v : {version::ros1, version::ros2})
        { auto original{target(v)}; require(validate_target(original)); for(auto const& [field, value] : original.connection) { auto missing{original}; missing.connection.erase(field); require(validate_target(missing), errc::invalid_argument); } }
    });
    run("every-operation-limit-explicit", [] {
        for(auto v : {version::ros1, version::ros2})
        { auto original{topic(v)}; require(validate_capability(v, original)); for(auto const& [field, value] : original.limits) { auto missing{original}; missing.limits.erase(field); require(validate_capability(v, missing), errc::invalid_argument); } }
    });
    run("names-cannot-normalize-or-remap-around-policy", [] {
        for(auto name : {"relative", "~private", "/a//b", "/a/../b", "/a/./b", "/a/", "/a:=/b", "/a*", "/0bad", "/a b"}) { assert(!canonical_resource(name)); }
        assert(canonical_resource("/robot/cmd_vel")); assert(!canonical_resource("/")); assert(canonical_resource("/", true));
        require(validate_connection_value(version::ros1, "node-name", "nested/node"), errc::invalid_argument);
        require(validate_connection_value(version::ros1, "master-uri", "http://127.0.0.1"), errc::invalid_argument);
    });
    run("duplicate-grants-and-type-conflicts-rejected", [] {
        auto t{target()}; auto duplicate{topic(version::ros1, "dup")}; t.capabilities.emplace("dup", duplicate); require(validate_target(t), errc::conflict);
        t = target(); auto subscribe{topic(version::ros1, "sub", "topic.subscribe")}; subscribe.type = "std_msgs/Bool"; subscribe.type_identity = builtin_type_identity(version::ros1, subscribe.type); t.capabilities.emplace("sub", subscribe); require(validate_target(t), errc::conflict);
    });
    run("unknown-unimplemented-operation-never-active", [] {
        auto t{target()}; auto cap{topic()}; cap.operation = "action.send-goal"; cap.limits.clear();
        for(auto field : operation_schema(version::ros1, cap.operation)) { cap.limits.emplace(field, "1"); }
        cap.limits["audit"] = "allow-and-deny"; cap.limits["cancel-scope"] = "not-applicable"; cap.limits["validator"] = "none"; cap.limits["overflow-policy"] = "reject";
        t.capabilities.clear(); t.capabilities.emplace("talk", cap); require(validate_target(t), errc::unsupported);
        assert(operation_schema(version::ros1, "lifecycle.get-state").empty()); assert(!operation_schema(version::ros2, "lifecycle.get-state").empty());
    });
    run("trusted-type-identity-and-version-binding", [&] {
        auto cap{topic()}; cap.type_identity = "00000000000000000000000000000000";
        require(validate_capability(version::ros1, cap), errc::permission_denied); require(validate_message(version::ros1, cap, message), errc::permission_denied);
        cap = topic(); require(validate_message(version::ros2, cap, message), errc::unsupported);
    });
    run("canonical-message-length-and-utf8", [&] {
        auto cap{topic()}; require(validate_message(version::ros1, cap, message));
        auto bad{message}; bad[0] = ::std::byte{4}; require(validate_message(version::ros1, cap, bad), errc::invalid_argument);
        bad = message; bad[4] = ::std::byte{0xc0}; bad[5] = ::std::byte{0x80}; require(validate_message(version::ros1, cap, bad), errc::invalid_argument);
        ::std::array<::std::byte, 65> large{}; require(validate_message(version::ros1, cap, large), errc::too_large);
    });
    run("boolean-structure-is-validated", [] {
        auto cap{topic()}; cap.type = "std_msgs/Bool"; cap.type_identity = builtin_type_identity(version::ros1, cap.type);
        ::std::array<::std::byte, 1> bytes{::std::byte{1}}; require(validate_message(version::ros1, cap, bytes));
        bytes[0] = ::std::byte{2}; require(validate_message(version::ros1, cap, bytes), errc::invalid_argument); require(validate_message(version::ros1, cap, {}), errc::invalid_argument);
    });
    run("twist-rejects-nan-infinity-and-enforces-bounds", [] {
        auto cap{topic()}; cap.type = "geometry_msgs/Twist"; cap.type_identity = builtin_type_identity(version::ros1, cap.type);
        ::std::array<::std::byte, 48> bytes{}; require(validate_message(version::ros1, cap, bytes));
        for(double value : {::std::numeric_limits<double>::quiet_NaN(), ::std::numeric_limits<double>::infinity(), -::std::numeric_limits<double>::infinity()})
        { auto bits{::std::bit_cast<::std::uint64_t>(value)}; for(unsigned j{}; j < 8; ++j) { bytes[j] = ::std::byte((bits >> (8 * j)) & 255); } require(validate_message(version::ros1, cap, bytes), errc::invalid_argument); }
        cap.limits["validator"] = "twist-bounds"; cap.limits["twist.linear-max-abs"] = "1"; cap.limits["twist.angular-max-abs"] = "2";
        auto bits{::std::bit_cast<::std::uint64_t>(1.5)}; for(unsigned j{}; j < 8; ++j) { bytes[j] = ::std::byte((bits >> (8 * j)) & 255); } require(validate_message(version::ros1, cap, bytes), errc::permission_denied);
    });
    run("qos-and-resource-conflicts-fail-closed", [] {
        auto cap{topic(version::ros2)}; cap.limits["qos.history"] = "keep-all"; require(validate_capability(version::ros2, cap), errc::unsupported);
        cap = topic(); cap.limits["overflow-policy"] = "backpressure"; require(validate_capability(version::ros1, cap), errc::unsupported);
        cap = topic(); cap.limits["max-queue-bytes"] = "1"; require(validate_capability(version::ros1, cap), errc::invalid_argument);
        cap = topic(); cap.limits["max-rate-hz"] = "nan"; require(validate_capability(version::ros1, cap), errc::invalid_argument);
    });
    run("ros2-dds-service-parameter-lifecycle-qos-all-eight-explicit", [] {
        for(auto operation : {"service.call", "service.advertise", "service.respond", "param.get", "param.set", "param.describe", "param.list", "lifecycle.get-state", "lifecycle.change-state"})
        {
            auto original{operation_cap(version::ros2, operation)};
            require(validate_capability(version::ros2, original));
            ::uwvm2::ros_gateway::validate(version::ros2, original);
            auto core_fields{operation_schema(version::ros2, operation)};
            auto gateway_fields{::uwvm2::ros_gateway::schema(version::ros2, operation)};
            ::std::sort(core_fields.begin(), core_fields.end()); ::std::sort(gateway_fields.begin(), gateway_fields.end());
            assert(core_fields.size() == gateway_fields.size());
            for(::std::size_t i{}; i != core_fields.size(); ++i) { assert(core_fields[i] == gateway_fields[i]); }
            assert(::std::adjacent_find(core_fields.begin(), core_fields.end()) == core_fields.end());
            for(auto field : {"qos.reliability", "qos.durability", "qos.history", "qos.depth", "qos.deadline", "qos.lifespan", "qos.liveliness", "qos.liveliness-lease-duration"})
            {
                assert(original.limits.contains(field)); auto missing{original}; missing.limits.erase(field);
                require(validate_capability(version::ros2, missing), errc::invalid_argument);
                bool rejected{}; try { ::uwvm2::ros_gateway::validate(version::ros2, missing); } catch(::std::runtime_error const&) { rejected = true; } assert(rejected);
            }
        }
    });
    run("dds-qos-values-have-no-default-or-unknown-field-fallback", [] {
        auto original{operation_cap(version::ros2, "service.call")};
        for(auto item : {::std::pair{"qos.reliability", "system-default"}, {"qos.durability", "system-default"}, {"qos.history", "unknown"}, {"qos.depth", "0"}, {"qos.deadline", "0"}, {"qos.lifespan", "nan"}, {"qos.liveliness", "system-default"}, {"qos.liveliness-lease-duration", "86400000000001"}})
        {
            auto invalid{original}; invalid.limits[item.first] = item.second;
            assert(!validate_capability(version::ros2, invalid));
            bool rejected{}; try { ::uwvm2::ros_gateway::validate(version::ros2, invalid); } catch(::std::runtime_error const&) { rejected = true; } assert(rejected);
        }
        auto explicit_profile{original}; explicit_profile.limits["qos.reliability"] = "best-effort"; explicit_profile.limits["qos.durability"] = "transient-local";
        explicit_profile.limits["qos.liveliness"] = "manual-by-topic";
        for(auto field : {"qos.deadline", "qos.lifespan", "qos.liveliness-lease-duration"}) { explicit_profile.limits[field] = "1000000000"; }
        require(validate_capability(version::ros2, explicit_profile)); ::uwvm2::ros_gateway::validate(version::ros2, explicit_profile);
        auto policy{target(version::ros2)}; policy.capabilities.clear(); policy.capabilities.emplace(original.alias, original);
        auto manifest{binding_manifest(policy)}; policy.capabilities.at(original.alias).limits["qos.depth"] = "2"; assert(manifest != binding_manifest(policy));
    });
    run("local-parameter-declare-and-ros1-reject-inapplicable-dds-qos", [] {
        for(auto operation : {"param.declare", "param.undeclare"})
        {
            auto local{operation_cap(version::ros2, operation)}; require(validate_capability(version::ros2, local));
            assert(!local.limits.contains("qos.reliability")); local.limits["qos.reliability"] = "reliable";
            require(validate_capability(version::ros2, local), errc::invalid_argument);
            bool rejected{}; try { ::uwvm2::ros_gateway::validate(version::ros2, local); } catch(::std::runtime_error const&) { rejected = true; } assert(rejected);
        }
        auto ros1{operation_cap(version::ros1, "service.call")}; assert(!ros1.limits.contains("qos.reliability"));
        ros1.limits["qos.reliability"] = "reliable"; require(validate_capability(version::ros1, ros1), errc::invalid_argument);
    });
    run("service-response-qos-is-exact-explicit-server-contract", [] {
        auto policy{target(version::ros2)}; policy.capabilities.clear();
        auto response{operation_cap(version::ros2, "service.respond")}; response.alias = "response"; policy.capabilities.emplace(response.alias, response);
        require(validate_target(policy)); ::uwvm2::ros_gateway::validate_service_qos_contract(policy);
        auto server{operation_cap(version::ros2, "service.advertise")}; server.alias = "server"; policy.capabilities.emplace(server.alias, server);
        require(validate_target(policy)); ::uwvm2::ros_gateway::validate_service_qos_contract(policy);
        for(auto item : {::std::pair{"qos.reliability", "best-effort"}, {"qos.durability", "transient-local"}, {"qos.depth", "2"}, {"qos.deadline", "1"}, {"qos.lifespan", "1"}, {"qos.liveliness", "manual-by-topic"}, {"qos.liveliness-lease-duration", "1"}})
        {
            auto mismatch{policy}; mismatch.capabilities.at("response").limits[item.first] = item.second;
            require(validate_target(mismatch), errc::conflict);
            bool rejected{}; try { ::uwvm2::ros_gateway::validate_service_qos_contract(mismatch); } catch(::std::runtime_error const&) { rejected = true; } assert(rejected);
        }
        auto separate{policy}; separate.capabilities.at("response").resource = "/test/other"; separate.capabilities.at("response").limits["qos.depth"] = "2";
        require(validate_target(separate)); ::uwvm2::ros_gateway::validate_service_qos_contract(separate);
    });
    run("ros2-enforce-cannot-use-test-security-material", [] {
        auto t{target(version::ros2)}; t.connection["security-mode"] = "enforce"; require(validate_target(t), errc::invalid_argument);
        t.connection["keystore"] = "/tmp/keys"; t.connection["enclave"] = "/uwvm/test"; require(validate_target(t));
        t.connection["security-mode"] = "disabled-test"; require(validate_target(t), errc::invalid_argument);
    });
    run("stable-machine-readable-effective-dump", [] {
        policy_store p{}; require(p.create_target(version::ros1, scope::group, "sensors")); require(p.bind_module(version::ros1, "sensors", "camera_a"));
        ::std::array<::std::string, 2> modules{"camera_b", "camera_a"}; auto first{p.canonical_dump(version::ros1, modules)};
        ::std::swap(modules[0], modules[1]); assert(first == p.canonical_dump(version::ros1, modules)); assert(first.find("\"module\":\"camera_a\"") < first.find("\"module\":\"camera_b\""));
        assert(json_quote("a\n\"b") == "\"a\\u000a\\\"b\"");
    });

    auto ros1{target()}, ros2{target(version::ros2)};
    int a{}, b{};
    run("independent-instance-and-version-handle-tables", [&] {
        capability_manager manager{}; require(manager.register_instance(&a, 1, "camera_a", &ros1, &ros2)); require(manager.register_instance(&b, 1, "camera_b", &ros1, &ros2));
        ::std::uint64_t ha{}, hb{}, h2{}; require(manager.open(&a, 1, version::ros1, "talk", ha)); require(manager.open(&b, 1, version::ros1, "talk", hb)); require(manager.open(&a, 1, version::ros2, "talk", h2)); assert(ha != hb && ha != h2);
        authorization lease{}; require(manager.authorize(&b, 1, version::ros1, ha, "topic.publish", message, lease), errc::bad_handle);
        require(manager.authorize(&a, 1, version::ros2, ha, "topic.publish", message, lease), errc::bad_handle);
        require(manager.authorize(&a, 1, version::ros1, h2, "topic.publish", message, lease), errc::bad_handle);
        require(manager.authorize(&a, 1, version::ros1, ha ^ (1ULL << 32), "topic.publish", message, lease), errc::bad_handle);
        require(manager.authorize(&a, 1, version::ros1, ha, "topic.subscribe", message, lease), errc::permission_denied);
        require(manager.authorize(&a, 1, version::ros1, ha, "topic.publish", message, lease)); assert(lease.capability()->alias == "talk");
    });
    run("close-generation-and-runtime-generation-reject-old-handles", [&] {
        capability_manager manager{}; require(manager.register_instance(&a, 1, "camera_a", &ros1, nullptr)); ::std::uint64_t old{}, fresh{}; require(manager.open(&a, 1, version::ros1, "talk", old)); require(manager.close(&a, 1, version::ros1, old)); require(manager.open(&a, 1, version::ros1, "talk", fresh)); assert(old != fresh);
        authorization lease{}; require(manager.describe(&a, 1, version::ros1, old, lease), errc::bad_handle); require(manager.unregister_instance(&a, 1)); require(manager.register_instance(&a, 2, "camera_a", &ros1, nullptr)); require(manager.describe(&a, 2, version::ros1, fresh, lease), errc::bad_handle); require(manager.describe(&a, 1, version::ros1, fresh, lease), errc::permission_denied);
    });
    run("multiple-handles-cannot-multiply-rate-quota", [&] {
        capability_manager manager{}; require(manager.register_instance(&a, 1, "camera_a", &ros1, nullptr)); ::std::uint64_t h1{}, h2{}; require(manager.open(&a, 1, version::ros1, "talk", h1)); require(manager.open(&a, 1, version::ros1, "talk", h2));
        for(unsigned i{}; i != 4; ++i) { authorization lease{}; require(manager.authorize(&a, 1, version::ros1, i % 2 ? h1 : h2, "topic.publish", message, lease)); }
        authorization lease{}; require(manager.authorize(&a, 1, version::ros1, h1, "topic.publish", message, lease), errc::rate_limited);
        require(manager.close(&a, 1, version::ros1, h1)); require(manager.open(&a, 1, version::ros1, "talk", h1)); require(manager.authorize(&a, 1, version::ros1, h1, "topic.publish", message, lease), errc::rate_limited);
    });
    run("invalid-data-does-not-consume-publication-quota", [&] {
        capability_manager manager{}; require(manager.register_instance(&a, 1, "camera_a", &ros1, nullptr)); ::std::uint64_t handle{}; require(manager.open(&a, 1, version::ros1, "talk", handle));
        for(unsigned i{}; i != 8; ++i) { authorization lease{}; require(manager.authorize(&a, 1, version::ros1, handle, "topic.publish", {}, lease), errc::invalid_argument); }
        for(unsigned i{}; i != 4; ++i) { authorization lease{}; require(manager.authorize(&a, 1, version::ros1, handle, "topic.publish", message, lease)); }
    });
    run("resource-count-and-creation-flood-bounded", [&] {
        capability_manager manager{}; auto limited{ros1}; limited.capabilities.at("talk").limits["max-publishers"] = "1"; require(manager.register_instance(&a, 1, "camera_a", &limited, nullptr));
        ::std::uint64_t handle{}, extra{}; require(manager.open(&a, 1, version::ros1, "talk", handle)); require(manager.open(&a, 1, version::ros1, "talk", extra), errc::rate_limited); assert(extra == 0);
        for(unsigned i{}; i != 3; ++i) { require(manager.close(&a, 1, version::ros1, handle)); require(manager.open(&a, 1, version::ros1, "talk", handle)); }
        require(manager.close(&a, 1, version::ros1, handle)); require(manager.open(&a, 1, version::ros1, "talk", handle), errc::rate_limited);
    });
    run("revocation-is-permanent-and-independent-per-instance", [&] {
        capability_manager manager{}; require(manager.register_instance(&a, 1, "camera_a", &ros1, nullptr)); require(manager.register_instance(&b, 1, "camera_b", &ros1, nullptr));
        ::std::uint64_t ha{}, hb{}; require(manager.open(&a, 1, version::ros1, "talk", ha)); require(manager.open(&b, 1, version::ros1, "talk", hb)); require(manager.revoke(&a, 1, version::ros1, "talk"));
        authorization lease{}; require(manager.describe(&a, 1, version::ros1, ha, lease), errc::bad_handle); require(manager.open(&a, 1, version::ros1, "talk", ha), errc::revoked); require(manager.authorize(&b, 1, version::ros1, hb, "topic.publish", message, lease));
    });
    run("revocation-linearizes-after-admitted-side-effect", [&] {
        capability_manager manager{}; require(manager.register_instance(&a, 1, "camera_a", &ros1, nullptr)); ::std::uint64_t handle{}; require(manager.open(&a, 1, version::ros1, "talk", handle));
        authorization lease{}; require(manager.authorize(&a, 1, version::ros1, handle, "topic.publish", message, lease));
        ::std::atomic<bool> started{}, completed{};
        auto future{::std::async(::std::launch::async, [&] { started = true; auto r{manager.revoke(&a, 1, version::ros1, "talk")}; completed = true; return r; })};
        while(!started.load()) { ::std::this_thread::yield(); }
        assert(future.wait_for(::std::chrono::milliseconds(20)) == ::std::future_status::timeout); assert(!completed.load()); lease.release();
        assert(future.wait_for(::std::chrono::seconds(1)) == ::std::future_status::ready); require(future.get()); require(manager.describe(&a, 1, version::ros1, handle, lease), errc::bad_handle);
    });
    run("empty-poll-preflight-does-not-consume-delivery-quota", [&] {
        auto t{target()}; t.capabilities.clear(); auto cap{topic(version::ros1, "listen", "topic.subscribe")}; t.capabilities.emplace("listen", cap);
        capability_manager manager{}; require(manager.register_instance(&a, 1, "camera_a", &t, nullptr)); ::std::uint64_t handle{}; require(manager.open(&a, 1, version::ros1, "listen", handle));
        for(unsigned i{}; i != 10; ++i) { authorization lease{}; require(manager.authorize(&a, 1, version::ros1, handle, "topic.subscribe", {}, lease, false, false)); }
        for(unsigned i{}; i != 4; ++i) { authorization lease{}; require(manager.authorize(&a, 1, version::ros1, handle, "topic.subscribe", message, lease)); }
    });
    run("service-request-and-response-structure-validated", [] {
        capability_spec cap{"trigger", "service.call", "/test/trigger", "std_srvs/Trigger", "937c9679a518e3a18d831e57125ea522", {{"timeout-ms", "1000"}, {"max-inflight", "1"}, {"max-request-bytes", "1"}, {"max-response-bytes", "64"}, {"max-rate-hz", "1"}, {"burst", "1"}, {"max-bytes-per-second", "128"}, {"validator", "none"}, {"audit", "allow-and-deny"}, {"audit-max-rate-hz", "10"}, {"audit-burst", "10"}, {"audit-max-records", "128"}, {"audit-max-bytes", "65536"}}};
        require(validate_capability(version::ros1, cap)); require(validate_message(version::ros1, cap, {}));
        ::std::array<::std::byte, 5> response{::std::byte{1}}; require(validate_service_response(version::ros1, cap, response)); response[0] = ::std::byte{2}; require(validate_service_response(version::ros1, cap, response), errc::invalid_argument);
    });
    run("manifest-binds-exact-backend-policy-before-resources", [&] {
        auto t{ros1}; auto original{binding_manifest(t)};
        t.target_scope = scope::group; t.name = "sensors"; t.connection["gateway-socket"] = "/var/run/mounted.sock"; t.connection["gateway-uid"] = "1001"; t.connection["connect-timeout-ms"] = "2000";
        assert(original == binding_manifest(t));
        t.capabilities.at("talk").resource = "/other"; assert(original != binding_manifest(t));
        t = ros1; t.capabilities.at("talk").limits["burst"] = "3"; assert(original != binding_manifest(t));
        t = ros1; t.connection["master-uri"] = "http://other:11311"; assert(original != binding_manifest(t));
    });
    run("unicode-module-identities-are-exact-utf8", [] {
        ::std::string module{"camera_摄像机"}; assert(valid_module_name(module)); assert(!valid_module_name(::std::string("bad\0name", 8)));
        policy_store p{}; require(p.create_target(version::ros1, scope::single, module)); ::std::array<::std::string, 1> names{module}; require(p.validate(names));
        assert(p.effective(version::ros1, module)->target_scope == scope::single);
    });
    run("backend-failure-lock-cannot-block-other-ros-version", [&] {
        capability_manager manager{}; require(manager.register_instance(&a, 1, "hybrid", &ros1, &ros2)); ::std::uint64_t h1{}, h2{}; require(manager.open(&a, 1, version::ros1, "talk", h1)); require(manager.open(&a, 1, version::ros2, "talk", h2));
        authorization ros1_pending{}; require(manager.describe(&a, 1, version::ros1, h1, ros1_pending));
        auto future{::std::async(::std::launch::async, [&] { authorization lease{}; return manager.authorize(&a, 1, version::ros2, h2, "topic.publish", message, lease); })};
        auto status{future.wait_for(::std::chrono::milliseconds(100))}; ros1_pending.release(); assert(status == ::std::future_status::ready); require(future.get());
    });
    run("image-structure-timestamp-stride-and-length-validated", [] {
        auto cap{topic()}; cap.type = "sensor_msgs/Image"; cap.type_identity = builtin_type_identity(version::ros1, cap.type); cap.limits["max-message-bytes"] = "512";
        ipc::image image{}; image.seconds = 1; image.frame_id = "camera"; image.height = 1; image.width = 2; image.encoding = "rgb8"; image.step = 6; image.data = {1,2,3,4,5,6};
        auto encode = [&](version v) { auto bytes{ipc::encode_image(image)}; return validate_message(v, cap, {reinterpret_cast<::std::byte const*>(bytes.data()), bytes.size()}); };
        require(encode(version::ros1)); image.step = 5; require(encode(version::ros1), errc::invalid_argument); image.step = 6; image.nanoseconds = 1000000000; require(encode(version::ros1), errc::invalid_argument);
        image.nanoseconds = 0; image.data.pop_back(); require(encode(version::ros1), errc::invalid_argument); image.data.push_back(6); image.encoding = "arbitrary"; require(encode(version::ros1), errc::invalid_argument);
    });
    run("audit-record-rate-count-byte-quotas-enforced", [&] {
        auto cap{topic()}; cap.limits["audit-max-rate-hz"] = "0.001"; cap.limits["audit-burst"] = "3"; cap.limits["audit-max-records"] = "2"; cap.limits["audit-max-bytes"] = "8";
        bounded_audit audit{}; require(audit.configure(cap)); assert(audit.record("abc")); assert(audit.record("def")); assert(audit.record("ghi")); assert(!audit.record("jkl")); assert(audit.retained_records() == 2 && audit.retained_bytes() == 6);
        ::std::vector<::std::string> records{}; ::std::uint64_t dropped{}; audit.drain(records, dropped); assert(records.size() == 2 && records[0] == "def" && records[1] == "ghi" && dropped == 2);
        capability_manager manager{}; require(manager.register_instance(&a, 1, "camera_a", &ros1, nullptr)); ::std::uint64_t handle{}; require(manager.open(&a, 1, version::ros1, "talk", handle));
        authorization lease{}; require(manager.authorize(&a, 1, version::ros1, handle, "topic.subscribe", message, lease), errc::permission_denied); require(manager.record_result(&a, 1, version::ros1, "talk", "gateway-error", errc::disconnected)); require(manager.revoke(&a, 1, version::ros1, "talk")); require(manager.drain_audit(&a, 1, version::ros1, "talk", records, dropped));
        assert(records.size() == 4); assert(records[0].find("resource-create") != ::std::string::npos); assert(records[1].find("\"error\":13") != ::std::string::npos); assert(records[2].find("gateway-error") != ::std::string::npos); assert(records[3].find("capability-revoke") != ::std::string::npos);
    });
    run("every-implemented-resource-field-is-explicit", [] {
        for(auto v : {version::ros1, version::ros2})
        {
            for(auto operation : {"service.call", "service.advertise", "service.respond", "param.get", "param.set", "param.list", "graph.list-topics", "graph.inspect-topic", "graph.inspect-node", "graph.inspect-service", "tf.lookup", "tf.broadcast", "time.read-ros", "log.write", "action.send-goal", "action.cancel-own-goal", "action.cancel-any-goal", "action.receive-feedback", "action.receive-result", "action.advertise", "action.respond"})
            {
                auto original{operation_cap(v, operation)}; require(validate_capability(v, original));
                for(auto const& item : original.limits) { auto missing{original}; missing.limits.erase(item.first); require(validate_capability(v, missing), errc::invalid_argument); }
            }
        }
        for(auto operation : {"param.declare", "param.undeclare", "param.describe", "param.subscribe-events", "graph.inspect-action", "lifecycle.get-state", "lifecycle.change-state"})
        { auto original{operation_cap(version::ros2, operation)}; require(validate_capability(version::ros2, original)); for(auto const& item : original.limits) { auto missing{original}; missing.limits.erase(item.first); require(validate_capability(version::ros2, missing), errc::invalid_argument); } }
    });
    run("transform-generated-type-frame-pair-and-field-bounds", [] {
        for(auto v : {version::ros1, version::ros2})
        {
            auto c{operation_cap(v, "tf.broadcast")}; ipc::transform_data transform{0,123,456,"map","base_link",{{1,2,3,0,0,0,1}}};
            auto check=[&](errc expected) { auto encoded{ipc::encode_transform(transform)}; require(validate_message(v, c, {reinterpret_cast<::std::byte const*>(encoded.data()), encoded.size()}), expected); };
            check(errc::success); transform.child = "other"; check(errc::permission_denied); transform.child = "base_link";
            transform.values[0] = 11; check(errc::permission_denied); transform.values[0] = ::std::numeric_limits<double>::infinity(); check(errc::invalid_argument); transform.values[0] = 1;
            transform.values[6] = 1.0000001; c.limits["tf.quaternion-norm-tolerance"] = "0.00000001"; check(errc::permission_denied);
            c.operation = "topic.publish"; assert(!backend_supported(v, c.operation, c.type));
        }
    });
    run("selected-parameter-events-filter-node-name-and-canonical-content", [] {
        auto c{operation_cap(version::ros2, "param.subscribe-events")}; ipc::parameter_event event{123,456,2,"/test","data","selected"};
        auto check=[&](errc expected) { auto encoded{ipc::encode_parameter_event(event)}; require(validate_message(version::ros2, c, {reinterpret_cast<::std::byte const*>(encoded.data()), encoded.size()}), expected); };
        check(errc::success);event.node="/other";check(errc::permission_denied);event.node="/test";event.name="other";check(errc::permission_denied);event.name="data";
        event.kind=3;check(errc::invalid_argument);event.value.clear();check(errc::success);event.nanoseconds=1000000000;check(errc::invalid_argument);
    });
    run("service-server-request-id-and-response-body-validated", [] {
        auto advertised{operation_cap(version::ros1, "service.advertise")}; auto responded{operation_cap(version::ros1, "service.respond")};
        ::std::array<::std::byte,8> request{};request[0]=::std::byte{1};require(validate_message(version::ros1, advertised, request));request[0]=::std::byte{0};require(validate_message(version::ros1, advertised, request),errc::invalid_argument);
        ::std::array<::std::byte,13> response{};response[0]=::std::byte{1};response[8]=::std::byte{1};require(validate_message(version::ros1, responded, response));response[8]=::std::byte{2};require(validate_message(version::ros1, responded, response),errc::invalid_argument);
    });
    run("lifecycle-transition-is-policy-only-and-response-bounded", [] {
        auto c{operation_cap(version::ros2, "lifecycle.get-state")};require(validate_request_payload(version::ros2,c,{}));::std::array<::std::byte,1> injection{::std::byte{1}};require(validate_request_payload(version::ros2,c,injection),errc::invalid_argument);
        ::std::array<::std::byte,5> state{::std::byte{3}};require(validate_response_payload(version::ros2,c,state));state[1]=::std::byte{1};require(validate_response_payload(version::ros2,c,state),errc::invalid_argument);
        c=operation_cap(version::ros2,"lifecycle.change-state");require(validate_response_payload(version::ros2,c,injection));injection[0]=::std::byte{2};require(validate_response_payload(version::ros2,c,injection),errc::invalid_argument);
    });
    run("action-canonical-goal-feedback-result-and-independent-scope", [] {
        for(auto v : {version::ros1,version::ros2})
        {
            auto bytes=[](auto const& encoded) { return ::std::span<::std::byte const>{reinterpret_cast<::std::byte const*>(encoded.data()),encoded.size()}; };
            auto c{operation_cap(v,"action.send-goal")};::std::vector<::std::uint8_t> order;ipc::put(order,5,4);require(validate_request_payload(v,c,bytes(order)));
            c.limits["fibonacci.max-order"]="4";require(validate_request_payload(v,c,bytes(order)),errc::permission_denied);order.assign(4,255);require(validate_request_payload(v,c,bytes(order)),errc::invalid_argument);
            c=operation_cap(v,"action.receive-result");auto encoded{ipc::encode_action_sequence({123,2,{0,1,1,2,3,5}},true)};require(validate_response_payload(v,c,bytes(encoded)));encoded[8]=1;require(validate_response_payload(v,c,bytes(encoded)),errc::invalid_argument);
            c=operation_cap(v,"action.respond");encoded=ipc::encode_action_sequence({123,1,{0,1}},true);require(validate_request_payload(v,c,bytes(encoded)));encoded[0]=0;encoded[1]=0;require(validate_request_payload(v,c,bytes(encoded)),errc::invalid_argument);
            c=operation_cap(v,"action.cancel-own-goal");c.limits["cancel-scope"]="any";require(validate_capability(v,c),errc::invalid_argument);
            c=operation_cap(v,"action.advertise");encoded=ipc::encode_action_goal(123,5);require(validate_request_payload(v,c,bytes(encoded)));encoded.push_back(0);require(validate_request_payload(v,c,bytes(encoded)),errc::invalid_argument);
        }
    });
    ::fast_io::io::println("PASS ", cases, " policy/capability tests; ROS side effects were not tested by this unit suite");
}
