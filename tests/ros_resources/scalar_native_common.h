#pragma once
#include <cmath>
#include <functional>
inline std::vector<std::string> native_scalar_kinds(core::version v)
{ return {"bool",v==core::version::ros1?"int32":"int64","float64"}; }
inline core::capability_spec native_scalar_cap(core::version v,std::string alias,std::string operation,std::string const& name,std::string const& kind)
{
    auto cap{scalar_cap(v,std::move(alias),std::move(operation),(v==core::version::ros1?"/uwvm_resources/":"/uwvm_resources/native/")+name,kind)};
    if(v==core::version::ros2) { cap.limits["parameter-name"]=name;cap.limits["qos.depth"]="6"; }return cap;
}
inline void native_scalar_caps(gateway::configuration& cfg,core::version v)
{
    cfg.policy.capabilities.clear();if(v==core::version::ros2) { cfg.policy.connection["domain-id"]="75"; }
    for(auto const& kind:native_scalar_kinds(v))
    {
        for(auto const& op:{"get","set"})
        {
            add(cfg,native_scalar_cap(v,kind+op,std::string{"param."}+op,"scalar_"+kind,kind));
            add(cfg,native_scalar_cap(v,kind+"wrong"+op,std::string{"param."}+op,"scalar_wrong",kind));
            if(kind!="bool")
            {
                auto cap{native_scalar_cap(v,kind+"bound"+op,std::string{"param."}+op,"scalar_"+kind,kind)};
                cap.limits["parameter-min-value"]=std::string{op}=="get"?"0":"-1";cap.limits["parameter-max-value"]="1";add(cfg,std::move(cap));
            }
        }
    }
}
inline void native_scalar_roundtrips(ipc::client& client,core::version v,
    std::function<std::vector<std::uint8_t>(std::string const&)> const& native_read,std::function<void()> const& wrong_unchanged)
{
    auto denied=[&](std::string const& operation,std::string const& alias,std::vector<std::uint8_t> const& bytes,std::uint32_t error) {
        auto reply{client.call(operation,alias,bytes,2500)};require(reply.error==error&&reply.payload.empty(),"scalar refusal has exact error and no value disclosure");
    };
    for(auto const& kind:native_scalar_kinds(v))
    {
        auto initial{scalar_value(v,kind)},changed{scalar_value(v,kind,true)};
        auto read=[&](ipc::parameter_value const& expected) { auto reply{client.call("param.get",kind+"get",{},2500)};require(!reply.error&&reply.payload==scalar_bytes(expected),"native scalar canonical read is bit-exact");require(native_read(kind)==scalar_bytes(expected),"independent native scalar kind/value is bit-exact"); };
        auto write=[&](ipc::parameter_value const& expected) { auto reply{client.call("param.set",kind+"set",scalar_bytes(expected),2500)};require(!reply.error&&reply.payload.empty(),"native scalar authorized write has empty response");read(expected); };
        read(initial);write(changed);
        if(kind=="int32"||kind=="int64")
        {
            auto value{changed};value.integer_value=kind=="int32"?std::numeric_limits<std::int32_t>::min():std::numeric_limits<std::int64_t>::min();write(value);
            value.integer_value=kind=="int32"?std::numeric_limits<std::int32_t>::max():std::numeric_limits<std::int64_t>::max();write(value);write(changed);
        }
        if(kind=="float64")
        {
            for(double exact:{std::nextafter(1.0,2.0),-0.0,std::numeric_limits<double>::min(),1.2345678901234567e300}) { auto value{changed};value.double_value=exact;write(value); }
            write(changed);
        }
        auto before{native_read(kind)};
        denied("param.set",kind+"get",scalar_bytes(initial),13);require(native_read(kind)==before,"scalar operation substitution does not mutate native value");
        auto extra{scalar_bytes(changed)};extra.push_back(0);denied("param.set",kind+"set",extra,90);require(native_read(kind)==before,"over-quota scalar payload has no native mutation");
        auto short_value{scalar_bytes(changed)};short_value.pop_back();denied("param.set",kind+"set",short_value,22);require(native_read(kind)==before,"short scalar has no native mutation");
        if(kind=="bool") { denied("param.set",kind+"set",{2},22); }
        if(kind=="float64")
        {
            for(auto bits:{0x7ff0000000000000ULL,0xfff0000000000000ULL,0x7ff8000000000001ULL}) { std::vector<std::uint8_t> bytes;ipc::put(bytes,bits,8);denied("param.set",kind+"set",bytes,22); }
        }
        require(native_read(kind)==before,"malformed scalar preserves native exact value");
        denied("param.get",kind+"wrongget",{},95);denied("param.set",kind+"wrongset",scalar_bytes(changed),95);wrong_unchanged();
        if(kind!="bool")
        {
            write(initial);denied("param.get",kind+"boundget",{},13);denied("param.set",kind+"boundset",scalar_bytes(changed),13);read(initial);
            auto restored{initial};restored.integer_value=0;restored.double_value=0.5;
            auto result{client.call("param.set",kind+"boundset",scalar_bytes(restored),2500)};require(!result.error&&result.payload.empty(),"in-range scalar write repairs existing out-of-range native value");read(restored);
        }
    }
    std::cout<<"PASS native Bool/integer/float64 scalar kinds and exact canonical roundtrips, signed limits, binary64 precision, range/malformed/quota/type denials and no mutation/disclosure\n";
}
