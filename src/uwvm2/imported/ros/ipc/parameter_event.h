/*************************************************************
 * UWVM2 canonical selected ROS2 parameter event. APL-2.0.    *
 *************************************************************/
#pragma once
#include "protocol.h"
#include "parameter.h"
namespace uwvm2::imported::ros::ipc
{
    struct parameter_event
    {
        std::uint32_t seconds{},nanoseconds{};
        std::uint8_t kind{};
        std::string node{},name{},value{};
    };
    inline bool parameter_node(std::string_view name) noexcept
    {
        if(name.size()<2||name.size()>1024||name.front()!='/'||name.back()=='/') { return false; }
        bool first{true};
        for(std::size_t i{1};i<name.size();++i)
        {
            char c{name[i]}; if(c=='/') { if(first) { return false; } first=true; continue; }
            bool alpha{(c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_'};
            if(!alpha&&!(c>='0'&&c<='9'&&!first)) { return false; } first=false;
        }
        return !first;
    }
    inline bool parameter_name(std::string_view name) noexcept
    {
        if(name.empty()||name.size()>255) { return false; }
        bool first{true};
        for(char c:name)
        {
            if(c=='.') { if(first) { return false; } first=true; continue; }
            bool alpha{(c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_'};
            if(!alpha&&!(c>='0'&&c<='9'&&!first)) { return false; } first=false;
        }
        return !first;
    }
    inline bool parameter_utf8(std::string const& text) noexcept
    {
        std::size_t p{};
        while(p<text.size())
        {
            auto lead{static_cast<unsigned char>(text[p++])}; if(lead<128) { continue; }
            unsigned n{}; std::uint32_t cp{},minimum{};
            if(lead>=0xc2&&lead<=0xdf) { n=1;cp=lead&31;minimum=128; }
            else if(lead>=0xe0&&lead<=0xef) { n=2;cp=lead&15;minimum=2048; }
            else if(lead>=0xf0&&lead<=0xf4) { n=3;cp=lead&7;minimum=65536; }
            else { return false; }
            if(n>text.size()-p) { return false; }
            while(n--) { auto byte{static_cast<unsigned char>(text[p++])}; if((byte&0xc0)!=0x80) { return false; } cp=(cp<<6)|(byte&63); }
            if(cp<minimum||cp>0x10ffff||(cp>=0xd800&&cp<=0xdfff)) { return false; }
        }
        return true;
    }
    inline bool decode_parameter_event(std::vector<std::uint8_t> const& bytes,parameter_event& result)
    {
        if(bytes.size()<24||bytes.size()>max_payload_bytes-518) { return false; }
        parameter_event event;
        event.seconds=static_cast<std::uint32_t>(get(bytes.data(),4)); event.nanoseconds=static_cast<std::uint32_t>(get(bytes.data()+4,4)); event.kind=bytes[8];
        if(event.seconds>2147483647U||event.nanoseconds>=1000000000U||event.kind<1||event.kind>3) { return false; }
        std::size_t p{9};
        auto string=[&](std::string& output,std::size_t maximum) {
            if(bytes.size()-p<4) { return false; } auto n{get(bytes.data()+p,4)}; p+=4;
            if(n>maximum||n>bytes.size()-p) { return false; }
            output.assign(reinterpret_cast<char const*>(bytes.data()+p),static_cast<std::size_t>(n));p+=static_cast<std::size_t>(n);return true;
        };
        if(!string(event.node,1024)||!string(event.name,255)||!string(event.value,max_payload_bytes-518)||p!=bytes.size()||!parameter_node(event.node)||!parameter_name(event.name)||!parameter_utf8(event.value)||(event.kind==3&&!event.value.empty())) { return false; }
        result=std::move(event);return true;
    }
    inline std::vector<std::uint8_t> encode_parameter_event_view(std::uint32_t seconds,std::uint32_t nanoseconds,std::uint8_t kind,
        std::string_view node,std::string_view name,std::string_view value)
    {
        std::vector<std::uint8_t> bytes;bytes.reserve(21+node.size()+name.size()+value.size());
        put(bytes,seconds,4);put(bytes,nanoseconds,4);put(bytes,kind,1);
        for(auto text:{node,name,value}) { put(bytes,text.size(),4);bytes.insert(bytes.end(),text.begin(),text.end()); }
        return bytes;
    }
    inline std::vector<std::uint8_t> encode_parameter_event(parameter_event const& event)
    { return encode_parameter_event_view(event.seconds,event.nanoseconds,event.kind,event.node,event.name,event.value); }
    struct typed_parameter_event
    {
        std::uint32_t seconds{},nanoseconds{};
        std::uint8_t kind{};
        std::string node{},name{};
        parameter_value value{};
    };
    inline std::uint32_t encode_typed_parameter_event_view(std::uint32_t seconds,std::uint32_t nanoseconds,std::uint8_t kind,
        std::string_view node,std::string_view name,parameter_value const& value,std::vector<std::uint8_t>& output)
    {
        auto tag{parameter_event_tag(value.type)};
        if(value.type==parameter_kind::unsupported||(!tag&&value.type!=parameter_kind::deletion)) { return 95; }
        if(seconds>2147483647U||nanoseconds>=1000000000U||kind<1||kind>3||!parameter_node(node)||!parameter_name(name)) { return 22; }
        if(value.type==parameter_kind::deletion&&kind!=3) { return 22; }
        std::size_t payload_size{};
        if(kind!=3) { auto error{parameter_encoded_size(value,payload_size)};if(error) { return error; } }
        auto base{23+node.size()+name.size()};
        if(payload_size>parameter_wire_limit-base) { return 90; }
        std::vector<std::uint8_t> bytes;
        bytes.reserve(base+payload_size);
        put(bytes,seconds,4);put(bytes,nanoseconds,4);put(bytes,kind,1);put(bytes,2,1);put(bytes,tag,1);
        for(auto text:{node,name}) { put(bytes,text.size(),4);bytes.insert(bytes.end(),text.begin(),text.end()); }
        put(bytes,payload_size,4);
        if(kind!=3) { parameter_detail::append_checked(value,bytes); }
        output=std::move(bytes);return 0;
    }
    inline std::uint32_t encode_typed_parameter_event(typed_parameter_event const& event,std::vector<std::uint8_t>& output)
    { return encode_typed_parameter_event_view(event.seconds,event.nanoseconds,event.kind,event.node,event.name,event.value,output); }
    inline std::uint32_t decode_typed_parameter_event(std::vector<std::uint8_t> const& bytes,typed_parameter_event& output,std::size_t max_elements=parameter_max_elements)
    {
        if(bytes.size()>max_payload_bytes-518) { return 90; }
        if(bytes.size()<23) { return 22; }
        typed_parameter_event event;
        event.seconds=static_cast<std::uint32_t>(get(bytes.data(),4));event.nanoseconds=static_cast<std::uint32_t>(get(bytes.data()+4,4));event.kind=bytes[8];
        event.value.type=parameter_event_kind(bytes[10]);
        if(event.seconds>2147483647U||event.nanoseconds>=1000000000U||event.kind<1||event.kind>3||bytes[9]!=2||event.value.type==parameter_kind::unsupported||(event.value.type==parameter_kind::deletion&&event.kind!=3)) { return 22; }
        std::size_t p{11};
        auto string=[&](std::string& text,std::size_t maximum) {
            if(bytes.size()-p<4) { return false; }auto size{get(bytes.data()+p,4)};p+=4;
            if(size>maximum||size>bytes.size()-p) { return false; }
            text.assign(reinterpret_cast<char const*>(bytes.data()+p),static_cast<std::size_t>(size));p+=static_cast<std::size_t>(size);return true;
        };
        if(!string(event.node,1024)||!string(event.name,255)||!parameter_node(event.node)||!parameter_name(event.name)||bytes.size()-p<4) { return 22; }
        auto size{get(bytes.data()+p,4)};p+=4;
        if(size!=bytes.size()-p||(event.kind==3&&size!=0)) { return 22; }
        if(event.kind!=3) { auto error{decode_parameter(event.value.type,bytes.data()+p,static_cast<std::size_t>(size),event.value,max_elements)};if(error) { return error; } }
        output=std::move(event);return 0;
    }
    inline std::uint32_t validate_typed_parameter_event(version backend,capability_spec const& cap,std::vector<std::uint8_t> const& bytes,typed_parameter_event* output=nullptr)
    {
        if(backend!=version::ros2||cap.operation!="param.subscribe-events"||parameter_event_type(backend,cap.type)==parameter_kind::unsupported) { return 95; }
        auto error{validate_parameter_contract(backend,cap)};if(error) { return error; }
        std::int64_t maximum{};parse_parameter_integer(cap.limits.at("max-parameter-bytes"),maximum);
        if(bytes.size()>static_cast<std::uint64_t>(maximum)) { return 90; }
        std::size_t elements{parameter_max_elements};
        if(array_parameter(parameter_event_type(backend,cap.type)))
        { std::int64_t count{};parse_parameter_integer(cap.limits.at("max-parameter-elements"),count);elements=static_cast<std::size_t>(count); }
        typed_parameter_event event;error=decode_typed_parameter_event(bytes,event,elements);if(error) { return error; }
        auto node{cap.limits.find("parameter-node")},name{cap.limits.find("parameter-name")};
        if(node==cap.limits.end()||name==cap.limits.end()||event.node!=node->second||event.name!=name->second||event.value.type!=parameter_event_type(backend,cap.type)) { return 13; }
        // Deleted wire values carry no old value. Tags1..9 attest a prior
        // concrete native value checked by the trusted Gateway. Tag0 is an
        // explicitly granted NOT_SET deletion notification; its prior kind and
        // value are unknown and are never inferred from the selected grant.
        if(event.kind!=3) { error=validate_parameter_event_value(backend,cap,event.value);if(error) { return error; } }
        if(output) { *output=std::move(event); }return 0;
    }
}
