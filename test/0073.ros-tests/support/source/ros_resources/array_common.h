#pragma once
#include <cmath>
#include <limits>
#include <functional>

namespace array_probe
{
    inline std::vector<std::string> kinds(core::version backend)
    { return backend==core::version::ros1?std::vector<std::string>{"bool","int32","float64","string"}:std::vector<std::string>{"bool","int64","float64","string","byte"}; }
    inline ipc::parameter_value value(core::version backend,std::string const& kind,bool empty=false)
    {
        ipc::parameter_value result;result.type=ipc::parameter_type(backend,"builtin/"+kind+"-array-v1");
        require(ipc::array_parameter(result.type),"supported exact native array kind");if(empty) { return result; }
        result.bool_array_value={0,1};
        result.int32_array_value={std::numeric_limits<std::int32_t>::min(),-7,std::numeric_limits<std::int32_t>::max()};
        result.int64_array_value={std::numeric_limits<std::int64_t>::min(),9007199254740993LL,std::numeric_limits<std::int64_t>::max()};
        result.double_array_value={-0.0,std::nextafter(1.0,2.0),1.25};
        result.string_array_value={"","hello<&>","你好\nline"};result.byte_array_value={0,255};return result;
    }
    inline ipc::parameter_value invalid(core::version backend,std::string const& kind,std::string const& scenario)
    {
        auto result{value(backend,kind)};
        if(scenario=="bounds")
        { result.int32_array_value={200000001};result.int64_array_value={9007199254740996LL};result.double_array_value={3}; }
        else if(scenario=="nonfinite")
        { require(kind=="float64","native nonfinite array is Float64");result.double_array_value={std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity()}; }
        else if(scenario=="count")
        { result.bool_array_value={0,1,0,1};result.int32_array_value={-7,0,1,2};result.int64_array_value={-7,0,1,2};result.double_array_value={-0.5,0,1,2};result.string_array_value={"a","b","c","d"};result.byte_array_value={0,1,254,255}; }
        else { require(scenario=="bytes"||scenario=="mixed","known native bad-array scenario"); }
        return result;
    }
    // Independent fixture encoding also represents invalid native values, so
    // an observer can compare NaN bits and malformed UTF8 without accepting them.
    inline std::vector<std::uint8_t> raw(ipc::parameter_value const& value)
    {
        std::vector<std::uint8_t> bytes;ipc::put(bytes,ipc::parameter_array_size(value),4);
        if(value.type==ipc::parameter_kind::boolean_array) { bytes.insert(bytes.end(),value.bool_array_value.begin(),value.bool_array_value.end()); }
        else if(value.type==ipc::parameter_kind::byte_array) { bytes.insert(bytes.end(),value.byte_array_value.begin(),value.byte_array_value.end()); }
        else if(value.type==ipc::parameter_kind::int32_array) { for(auto item:value.int32_array_value) { ipc::put(bytes,static_cast<std::uint32_t>(item),4); } }
        else if(value.type==ipc::parameter_kind::int64_array) { for(auto item:value.int64_array_value) { ipc::put(bytes,static_cast<std::uint64_t>(item),8); } }
        else if(value.type==ipc::parameter_kind::float64_array) { for(auto item:value.double_array_value) { std::uint64_t bits{};std::memcpy(&bits,&item,8);ipc::put(bytes,bits,8); } }
        else { for(auto const& item:value.string_array_value) { ipc::put(bytes,item.size(),4);bytes.insert(bytes.end(),item.begin(),item.end()); } }
        return bytes;
    }
    inline std::vector<std::uint8_t> bytes(ipc::parameter_value const& value)
    { std::vector<std::uint8_t> encoded;require(!ipc::encode_parameter(value,encoded)&&encoded==raw(value),"native fixture canonical array encoding");return encoded; }
    inline core::capability_spec cap(core::version backend,std::string alias,std::string operation,std::string const& name,std::string const& kind)
    {
        auto result{resource(backend,std::move(alias),std::move(operation),(backend==core::version::ros1?"/uwvm_resources/":"/uwvm_resources/native/")+name)};
        result.type="builtin/"+kind+"-array-v1";result.type_identity=std::string{ipc::parameter_identity(backend,result.type)};
        result.limits["max-parameter-elements"]="64";result.limits["max-parameter-bytes"]="4096";
        if(backend==core::version::ros2)
        {
            result.limits["parameter-name"]=name;
            if(result.operation!="param.declare"&&result.operation!="param.undeclare") { result.limits["qos.depth"]="6"; }
        }
        if(kind=="int32") { result.limits["parameter-min-value"]="-2147483648";result.limits["parameter-max-value"]="2147483647"; }
        if(kind=="int64") { result.limits["parameter-min-value"]="-9223372036854775808";result.limits["parameter-max-value"]="9223372036854775807"; }
        if(kind=="float64") { result.limits["parameter-min-value"]="-1.7976931348623157e308";result.limits["parameter-max-value"]="1.7976931348623157e308";result.limits["validator"]="finite"; }
        return result;
    }
    inline void caps(gateway::configuration& config,core::version backend)
    {
        config.policy.capabilities.clear();if(backend==core::version::ros2) { config.policy.connection["domain-id"]="75";config.policy.connection["node-name"]="uwvm_array_gateway"; }
        for(auto const& kind:kinds(backend))
        {
            for(std::string const profile:{"value","wrong","count","bytes","mixed","bound","invalid"})
            {
                if(profile=="mixed"&&backend!=core::version::ros1) { continue; }
                if(profile=="bound"&&kind!="int32"&&kind!="int64"&&kind!="float64") { continue; }
                if(profile=="invalid"&&kind!="float64"&&kind!="string") { continue; }
                for(auto const* operation:{"get","set"})
                {
                    auto c{cap(backend,kind+profile+operation,std::string{"param."}+operation,"array_"+profile+"_"+kind,kind)};
                    if(profile=="count") { c.limits["max-parameter-elements"]="1"; }
                    if(profile=="bytes") { c.limits["max-parameter-bytes"]="4"; }
                    if(profile=="bound") { c.limits["parameter-min-value"]="-1";c.limits["parameter-max-value"]="1"; }
                    add(config,std::move(c));
                }
            }
        }
    }
    inline void expect(ipc::client& client,std::string const& operation,std::string const& alias,std::vector<std::uint8_t> const& request,std::uint32_t error)
    { auto reply{client.call(operation,alias,request,2500)};require(reply.error==error&&reply.payload.empty(),"array refusal exact error with no parameter bytes disclosed"); }
    inline std::vector<std::vector<std::uint8_t>> malformed(core::version backend,std::string const& kind)
    {
        auto regular{value(backend,kind)};auto data{bytes(regular)};auto trailing{data};trailing.push_back(0);
        auto short_value{data};short_value.pop_back();std::vector<std::vector<std::uint8_t>> result{{0,0,0},{255,255,255,255},trailing,short_value};
        if(kind=="bool") { auto invalid{data};invalid[4]=2;result.push_back(std::move(invalid)); }
        if(kind=="float64")
        { for(auto bits:{0x7ff0000000000000ULL,0xfff0000000000000ULL,0x7ff8000000000001ULL}) { std::vector<std::uint8_t> invalid;ipc::put(invalid,1,4);ipc::put(invalid,bits,8);result.push_back(std::move(invalid)); } }
        if(kind=="string")
        {
            for(auto const& invalid:std::vector<std::string>{std::string{"a\0b",3},std::string{"\xc0\xaf",2}})
            { auto bad{regular};bad.string_array_value={invalid};result.push_back(raw(bad)); }
            if(backend==core::version::ros1)
            { for(auto const& invalid:std::vector<std::string>{std::string{1,'\1'},std::string{"\xef\xbf\xbe",3},std::string{"\xef\xbf\xbf",3}}) { auto bad{regular};bad.string_array_value={invalid};result.push_back(raw(bad)); } }
        }
        return result;
    }
    inline void roundtrips(ipc::client& client,core::version backend,
        std::function<std::vector<std::uint8_t>(std::string const&,std::string const&)> const& native_read,
        std::function<unsigned()> const& native_writes)
    {
        for(auto const& kind:kinds(backend))
        {
            auto full{value(backend,kind)},empty{value(backend,kind,true)};
            auto read=[&](ipc::parameter_value const& expected) {
                auto reply{client.call("param.get",kind+"valueget",{},2500)};
                require(!reply.error&&reply.payload==bytes(expected),"actual native array getter returns exact canonical elements");
                require(native_read(kind,"value")==raw(expected),"independent native array kind/element bits match canonical result");
            };
            auto write=[&](ipc::parameter_value const& selected) {
                auto reply{client.call("param.set",kind+"valueset",bytes(selected),2500)};
                require(!reply.error&&reply.payload.empty(),"authorized exact native array setter returns no bytes");read(selected);
            };
            auto deny=[&](std::string const& profile,std::string const& operation,std::string const& alias,
                         std::vector<std::uint8_t> const& request,std::uint32_t error) {
                auto before{native_read(kind,profile)};auto writes{native_writes()};
                expect(client,operation,alias,request,error);
                require(native_read(kind,profile)==before&&native_writes()==writes,"denied array operation preserves actual native value and setter callback count");
            };
            read(full);write(empty);write(full);
            if(kind=="float64")
            {
                auto exact{full};exact.double_array_value={-0.0,std::nextafter(1.0,2.0),std::numeric_limits<double>::min(),-std::numeric_limits<double>::min(),1.2345678901234567e300};write(exact);write(full);
            }
            deny("value","param.set",kind+"valueget",bytes(empty),13);
            for(auto const& packet:malformed(backend,kind)) { deny("value","param.set",kind+"valueset",packet,22); }
            deny("value","param.set",kind+"valueset",std::vector<std::uint8_t>(4097),90);
            auto many{full};many.bool_array_value.assign(65,1);many.int32_array_value.assign(65,-7);many.int64_array_value.assign(65,9007199254740993LL);
            many.double_array_value.assign(65,0.5);many.string_array_value.assign(65,"x");many.byte_array_value.assign(65,255);
            deny("value","param.set",kind+"valueset",bytes(many),90);
            for(std::string const profile:{"wrong","count","bytes"})
            {
                auto error{profile=="wrong"?95u:90u};deny(profile,"param.get",kind+profile+"get",{},error);
                deny(profile,"param.set",kind+profile+"set",bytes(full),error);
                if(profile!="wrong")
                {
                    auto reply{client.call("param.set",kind+profile+"set",bytes(empty),2500)};
                    require(!reply.error&&reply.payload.empty()&&native_read(kind,profile)==raw(empty),"empty typed write repairs an over-quota resident array");
                    auto fetched{client.call("param.get",kind+profile+"get",{},2500)};require(!fetched.error&&fetched.payload==bytes(empty),"empty native array retains canonical count0");
                }
            }
            if(backend==core::version::ros1)
            { deny("mixed","param.get",kind+"mixedget",{},95);deny("mixed","param.set",kind+"mixedset",bytes(full),95); }
            if(kind=="int32"||kind=="int64"||kind=="float64")
            {
                deny("bound","param.get",kind+"boundget",{},13);deny("bound","param.set",kind+"boundset",bytes(full),13);
                auto repaired{empty};repaired.int32_array_value={0};repaired.int64_array_value={0};repaired.double_array_value={0.5};
                auto reply{client.call("param.set",kind+"boundset",bytes(repaired),2500)};
                require(!reply.error&&reply.payload.empty()&&native_read(kind,"bound")==raw(repaired),"per-element in-range native array write repairs out-of-range resident value");
                auto fetched{client.call("param.get",kind+"boundget",{},2500)};require(!fetched.error&&fetched.payload==bytes(repaired),"inclusive per-element bounds apply to native getter");
            }
            if(kind=="float64"||(kind=="string"&&backend==core::version::ros2))
            {
                deny("invalid","param.get",kind+"invalidget",{},22);
                auto reply{client.call("param.set",kind+"invalidset",bytes(full),2500)};
                require(!reply.error&&reply.payload.empty()&&native_read(kind,"invalid")==raw(full),"finite/valid UTF8 native array can repair a malformed resident value of the same kind");
            }
        }
        std::cout<<"PASS actual native empty/multi typed array roundtrips, exact extrema/Float64 bits/UTF8/bytes, malformed/count/byte/range/native-kind denials without mutation or disclosure\n";
    }
}
