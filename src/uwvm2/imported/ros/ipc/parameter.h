/*************************************************************
 * UWVM2 canonical typed parameters. APL-2.0.                *
 *************************************************************/
#pragma once
#include "protocol.h"
#include "../common/types.h"
#include <cmath>
#include <charconv>
#include <cstring>
#if defined(__cpp_concepts)
#ifndef UWVM_MODULE
# include <fast_io_freestanding.h>
#endif
#endif
#include <locale>
#include <sstream>
#include <string_view>

namespace uwvm2::imported::ros::ipc
{
    enum class parameter_kind { unsupported, string, boolean, int32, int64, float64, boolean_array, int32_array, int64_array, float64_array, string_array, byte_array, deletion };
    struct parameter_value
    {
        parameter_kind type{parameter_kind::unsupported};
        std::string string_value{};
        bool bool_value{};
        std::int64_t integer_value{};
        double double_value{};
        std::vector<std::uint8_t> bool_array_value{};
        std::vector<std::int32_t> int32_array_value{};
        std::vector<std::int64_t> int64_array_value{};
        std::vector<double> double_array_value{};
        std::vector<std::string> string_array_value{};
        std::vector<std::uint8_t> byte_array_value{};
    };
    inline constexpr parameter_kind parameter_type(version backend,std::string_view type) noexcept
    {
        if(!valid_version(backend)) { return parameter_kind::unsupported; }
        if(type=="builtin/string-v1") { return parameter_kind::string; }
        if(type=="builtin/bool-v1") { return parameter_kind::boolean; }
        if(type=="builtin/float64-v1") { return parameter_kind::float64; }
        if(backend==version::ros1&&type=="builtin/int32-v1") { return parameter_kind::int32; }
        if(backend==version::ros2&&type=="builtin/int64-v1") { return parameter_kind::int64; }
        if(type=="builtin/bool-array-v1") { return parameter_kind::boolean_array; }
        if(type=="builtin/float64-array-v1") { return parameter_kind::float64_array; }
        if(type=="builtin/string-array-v1") { return parameter_kind::string_array; }
        if(backend==version::ros1&&type=="builtin/int32-array-v1") { return parameter_kind::int32_array; }
        if(backend==version::ros2&&type=="builtin/int64-array-v1") { return parameter_kind::int64_array; }
        if(backend==version::ros2&&type=="builtin/byte-array-v1") { return parameter_kind::byte_array; }
        return parameter_kind::unsupported;
    }
    inline constexpr std::string_view parameter_identity(version backend,std::string_view type) noexcept
    {
        switch(parameter_type(backend,type))
        {
        case parameter_kind::string:return "builtin:param-string-v1";
        case parameter_kind::boolean:return "builtin:param-bool-v1";
        case parameter_kind::int32:return "builtin:param-int32-v1";
        case parameter_kind::int64:return "builtin:param-int64-v1";
        case parameter_kind::float64:return "builtin:param-float64-v1";
        case parameter_kind::boolean_array:return "builtin:param-bool-array-v1";
        case parameter_kind::int32_array:return "builtin:param-int32-array-v1";
        case parameter_kind::int64_array:return "builtin:param-int64-array-v1";
        case parameter_kind::float64_array:return "builtin:param-float64-array-v1";
        case parameter_kind::string_array:return "builtin:param-string-array-v1";
        case parameter_kind::byte_array:return "builtin:param-byte-array-v1";
        default:return {};
        }
    }
    inline constexpr bool array_parameter(parameter_kind type) noexcept
    { return type==parameter_kind::boolean_array||type==parameter_kind::int32_array||type==parameter_kind::int64_array||type==parameter_kind::float64_array||type==parameter_kind::string_array||type==parameter_kind::byte_array; }
    inline constexpr parameter_kind parameter_element_kind(parameter_kind type) noexcept
    {
        return type==parameter_kind::boolean_array?parameter_kind::boolean:type==parameter_kind::int32_array?parameter_kind::int32:
            type==parameter_kind::int64_array?parameter_kind::int64:type==parameter_kind::float64_array?parameter_kind::float64:
            type==parameter_kind::string_array?parameter_kind::string:type==parameter_kind::byte_array?parameter_kind::unsupported:type;
    }
    inline constexpr std::size_t parameter_width(parameter_kind type) noexcept
    { return array_parameter(type)?4:type==parameter_kind::boolean?1:type==parameter_kind::int32?4:(type==parameter_kind::int64||type==parameter_kind::float64)?8:type==parameter_kind::string?4:0; }
    inline constexpr std::size_t parameter_element_width(parameter_kind type) noexcept
    { return type==parameter_kind::byte_array?1:parameter_width(parameter_element_kind(type)); }
    inline constexpr bool float64_parameter(parameter_kind type) noexcept
    { return type==parameter_kind::float64||type==parameter_kind::float64_array; }
    inline constexpr bool numeric_parameter(parameter_kind type) noexcept
    { auto element{parameter_element_kind(type)};return element==parameter_kind::int32||element==parameter_kind::int64||element==parameter_kind::float64; }
    inline std::size_t parameter_array_size(parameter_value const& value) noexcept
    {
        switch(value.type)
        {
        case parameter_kind::boolean_array:return value.bool_array_value.size();
        case parameter_kind::int32_array:return value.int32_array_value.size();
        case parameter_kind::int64_array:return value.int64_array_value.size();
        case parameter_kind::float64_array:return value.double_array_value.size();
        case parameter_kind::string_array:return value.string_array_value.size();
        case parameter_kind::byte_array:return value.byte_array_value.size();
        default:return 0;
        }
    }
    inline constexpr std::size_t parameter_max_elements{1048576};
    inline constexpr std::size_t parameter_wire_limit{max_payload_bytes-518};
    inline constexpr parameter_kind parameter_event_type(version backend,std::string_view type) noexcept
    {
        if(backend!=version::ros2) { return parameter_kind::unsupported; }
        if(type=="builtin/param-event-bool-v1") { return parameter_kind::boolean; }
        if(type=="builtin/param-event-int64-v1") { return parameter_kind::int64; }
        if(type=="builtin/param-event-float64-v1") { return parameter_kind::float64; }
        if(type=="builtin/param-event-string-v1") { return parameter_kind::string; }
        if(type=="builtin/param-event-byte-array-v1") { return parameter_kind::byte_array; }
        if(type=="builtin/param-event-bool-array-v1") { return parameter_kind::boolean_array; }
        if(type=="builtin/param-event-int64-array-v1") { return parameter_kind::int64_array; }
        if(type=="builtin/param-event-float64-array-v1") { return parameter_kind::float64_array; }
        if(type=="builtin/param-event-string-array-v1") { return parameter_kind::string_array; }
        if(type=="builtin/param-event-deletion-v1") { return parameter_kind::deletion; }
        return parameter_kind::unsupported;
    }
    inline constexpr std::string_view parameter_event_identity(version backend,std::string_view type) noexcept
    {
        switch(parameter_event_type(backend,type))
        {
        case parameter_kind::boolean:return "builtin:param-event-bool-v1";
        case parameter_kind::int64:return "builtin:param-event-int64-v1";
        case parameter_kind::float64:return "builtin:param-event-float64-v1";
        case parameter_kind::string:return "builtin:param-event-string-v1";
        case parameter_kind::byte_array:return "builtin:param-event-byte-array-v1";
        case parameter_kind::boolean_array:return "builtin:param-event-bool-array-v1";
        case parameter_kind::int64_array:return "builtin:param-event-int64-array-v1";
        case parameter_kind::float64_array:return "builtin:param-event-float64-array-v1";
        case parameter_kind::string_array:return "builtin:param-event-string-array-v1";
        case parameter_kind::deletion:return "builtin:param-event-deletion-v1";
        default:return {};
        }
    }
    inline constexpr std::uint8_t parameter_event_tag(parameter_kind type) noexcept
    {
        switch(type)
        {
        case parameter_kind::boolean:return 1;
        case parameter_kind::int64:return 2;
        case parameter_kind::float64:return 3;
        case parameter_kind::string:return 4;
        case parameter_kind::byte_array:return 5;
        case parameter_kind::boolean_array:return 6;
        case parameter_kind::int64_array:return 7;
        case parameter_kind::float64_array:return 8;
        case parameter_kind::string_array:return 9;
        case parameter_kind::deletion:return 0;
        default:return 0;
        }
    }
    inline constexpr parameter_kind parameter_event_kind(std::uint8_t tag) noexcept
    {
        switch(tag)
        {
        case 0:return parameter_kind::deletion;
        case 1:return parameter_kind::boolean;
        case 2:return parameter_kind::int64;
        case 3:return parameter_kind::float64;
        case 4:return parameter_kind::string;
        case 5:return parameter_kind::byte_array;
        case 6:return parameter_kind::boolean_array;
        case 7:return parameter_kind::int64_array;
        case 8:return parameter_kind::float64_array;
        case 9:return parameter_kind::string_array;
        default:return parameter_kind::unsupported;
        }
    }
    inline constexpr bool scalar_parameter_operation(version backend,std::string_view operation) noexcept
    { return operation=="param.get"||operation=="param.set"||(backend==version::ros2&&(operation=="param.declare"||operation=="param.undeclare")); }
    inline bool parse_parameter_integer(std::string_view text,std::int64_t& output) noexcept
    {
        if(text.empty()||!((text.front()>='0'&&text.front()<='9')||text.front()=='-')) { return false; }
        std::int64_t candidate{};
#if defined(__cpp_concepts)
        auto const [next,error]{::fast_io::parse_by_scan(text.data(),text.data()+text.size(),candidate)};
        if(error!=::fast_io::parse_code::ok||next!=text.data()+text.size()) { return false; }
#else
        // The independent Noetic/Jazzy C++17 binaries cannot include fast_io's
        // C++20 concepts. Their compatibility parser is still allocation-free,
        // locale-independent and requires the complete signed decimal input.
        auto const [next,error]{std::from_chars(text.data(),text.data()+text.size(),candidate)};
        if(error!=std::errc{}||next!=text.data()+text.size()) { return false; }
#endif
        output=candidate;return true;
    }
    inline bool parse_parameter_real(std::string_view text,double& output)
    {
        if(text.empty()) { return false; }std::size_t p{text.front()=='-'?1u:0u};bool nonzero{};
        auto digits=[&](bool mantissa) { auto start{p};while(p<text.size()&&text[p]>='0'&&text[p]<='9') { if(mantissa&&text[p]!='0') { nonzero=true; }++p; }return p!=start; };
        if(!digits(true)) { return false; }
        if(p<text.size()&&text[p]=='.') { ++p;if(!digits(true)) { return false; } }
        if(p<text.size()&&(text[p]=='e'||text[p]=='E')) { ++p;if(p<text.size()&&(text[p]=='-'||text[p]=='+')) { ++p; }if(!digits(false)) { return false; } }
        if(p!=text.size()) { return false; }
        double candidate{};
#if defined(__cpp_concepts)
        auto const [next,error]{::fast_io::parse_by_scan(text.data(),text.data()+text.size(),candidate)};
        if(error!=::fast_io::parse_code::ok||next!=text.data()+text.size()) { return false; }
        // Classic num_get may reject underflow even when rounding produces a
        // nonzero subnormal or the minimum normal. Preserve that platform
        // behavior for this rare boundary; ordinary finite values use fast_io.
        if(nonzero&&std::abs(candidate)<=std::numeric_limits<double>::min())
#endif
        {
            std::istringstream stream{std::string{text}};stream.imbue(std::locale::classic());
            stream>>std::noskipws>>candidate;
            if(stream.fail()||!stream.eof()) { return false; }
        }
        if(!std::isfinite(candidate)||(candidate==0&&nonzero)) { return false; }
        output=candidate;return true;
    }
    inline bool parameter_payload_utf8(std::uint8_t const* bytes,std::size_t size) noexcept
    {
        std::size_t p{};while(p<size)
        {
            auto lead{bytes[p++]};if(lead<128) { continue; }unsigned n{};std::uint32_t cp{},minimum{};
            if(lead>=0xc2&&lead<=0xdf) { n=1;cp=lead&31;minimum=128; }
            else if(lead>=0xe0&&lead<=0xef) { n=2;cp=lead&15;minimum=2048; }
            else if(lead>=0xf0&&lead<=0xf4) { n=3;cp=lead&7;minimum=65536; }else { return false; }
            if(n>size-p) { return false; }while(n--) { auto b{bytes[p++]};if((b&0xc0)!=0x80) { return false; }cp=(cp<<6)|(b&63); }
            if(cp<minimum||cp>0x10ffff||(cp>=0xd800&&cp<=0xdfff)) { return false; }
        }return true;
    }
    inline bool parameter_array_string_valid(version backend,std::string_view text) noexcept
    {
        if(!valid_version(backend)||!parameter_payload_utf8(reinterpret_cast<std::uint8_t const*>(text.data()),text.size())) { return false; }
        for(std::size_t p{};p<text.size();++p)
        {
            auto byte{static_cast<unsigned char>(text[p])};
            if(!byte) { return false; }
            if(backend==version::ros1)
            {
                if(byte<32&&byte!=9&&byte!=10&&byte!=13) { return false; }
                if(byte==0xef&&text.size()-p>=3&&static_cast<unsigned char>(text[p+1])==0xbf&&
                   (static_cast<unsigned char>(text[p+2])==0xbe||static_cast<unsigned char>(text[p+2])==0xbf)) { return false; }
            }
        }
        return true;
    }
    inline std::int64_t parameter_wire_integer(std::uint8_t const* bytes,std::size_t width) noexcept
    {
        auto bits{get(bytes,width)};
        return width==4?((bits&0x80000000ULL)?-1-static_cast<std::int64_t>((~bits)&0xffffffffULL):static_cast<std::int64_t>(bits)):
            ((bits&0x8000000000000000ULL)?-1-static_cast<std::int64_t>(~bits):static_cast<std::int64_t>(bits));
    }
    // No allocation before complete shape/element validation. Malformed arrays
    // return 22 before the element quota is considered; a well-formed count
    // exceeding either the explicit limit or the hard ceiling returns 90.
    inline std::uint32_t preflight_parameter_array(parameter_kind type,std::uint8_t const* bytes,std::size_t size,std::size_t max_elements,std::uint32_t& count)
    {
        if(!array_parameter(type)) { return 95; }
        if((!bytes&&size)||size<4) { return 22; }
        if(size>parameter_wire_limit) { return 90; }
        auto candidate{static_cast<std::uint32_t>(get(bytes,4))};auto width{parameter_element_width(type)};
        if(candidate>(size-4)/width) { return 22; }
        if(type!=parameter_kind::string_array&&static_cast<std::size_t>(candidate)*width!=size-4) { return 22; }
        std::size_t p{4};
        for(std::uint32_t i{};i<candidate;++i)
        {
            if(type==parameter_kind::string_array)
            {
                if(size-p<4) { return 22; }auto length{static_cast<std::size_t>(get(bytes+p,4))};p+=4;
                if(length>size-p||!parameter_array_string_valid(version::ros2,{reinterpret_cast<char const*>(bytes+p),length})) { return 22; }p+=length;
            }
            else
            {
                if(type==parameter_kind::boolean_array&&bytes[p]>1) { return 22; }
                if(type==parameter_kind::float64_array)
                { auto bits{get(bytes+p,8)};double value{};std::memcpy(&value,&bits,8);if(!std::isfinite(value)) { return 22; } }
                p+=width;
            }
        }
        if(p!=size) { return 22; }
        if(candidate>max_elements||candidate>parameter_max_elements) { return 90; }
        count=candidate;return 0;
    }
    inline std::uint32_t decode_parameter(parameter_kind type,std::uint8_t const* bytes,std::size_t size,parameter_value& output,std::size_t max_elements=parameter_max_elements)
    {
        static_assert(sizeof(double)==8&&std::numeric_limits<double>::is_iec559,"canonical parameter float64 requires IEEE754");
        if(!parameter_width(type)) { return 95; }
        if(array_parameter(type))
        {
            std::uint32_t count{};auto error{preflight_parameter_array(type,bytes,size,max_elements,count)};if(error) { return error; }
            parameter_value candidate;candidate.type=type;std::size_t p{4};
            switch(type)
            {
            case parameter_kind::boolean_array:candidate.bool_array_value.assign(bytes+4,bytes+size);break;
            case parameter_kind::byte_array:candidate.byte_array_value.assign(bytes+4,bytes+size);break;
            case parameter_kind::int32_array:
                candidate.int32_array_value.reserve(count);for(std::uint32_t i{};i<count;++i,p+=4) { candidate.int32_array_value.push_back(static_cast<std::int32_t>(parameter_wire_integer(bytes+p,4))); }break;
            case parameter_kind::int64_array:
                candidate.int64_array_value.reserve(count);for(std::uint32_t i{};i<count;++i,p+=8) { candidate.int64_array_value.push_back(parameter_wire_integer(bytes+p,8)); }break;
            case parameter_kind::float64_array:
                candidate.double_array_value.reserve(count);for(std::uint32_t i{};i<count;++i,p+=8) { auto bits{get(bytes+p,8)};double value{};std::memcpy(&value,&bits,8);candidate.double_array_value.push_back(value); }break;
            case parameter_kind::string_array:
                candidate.string_array_value.reserve(count);for(std::uint32_t i{};i<count;++i) { auto length{static_cast<std::size_t>(get(bytes+p,4))};p+=4;candidate.string_array_value.emplace_back(reinterpret_cast<char const*>(bytes+p),length);p+=length; }break;
            default:return 95;
            }
            output=std::move(candidate);return 0;
        }
        if((!bytes&&size)||size<parameter_width(type)||(type!=parameter_kind::string&&size!=parameter_width(type))) { return 22; }
        parameter_value candidate;candidate.type=type;
        if(type==parameter_kind::string)
        { if(get(bytes,4)!=size-4||!parameter_payload_utf8(bytes+4,size-4)) { return 22; }candidate.string_value.assign(reinterpret_cast<char const*>(bytes+4),size-4); }
        else if(type==parameter_kind::boolean) { if(bytes[0]>1) { return 22; }candidate.bool_value=bytes[0]!=0; }
        else if(type==parameter_kind::float64) { auto bits{get(bytes,8)};std::memcpy(&candidate.double_value,&bits,8);if(!std::isfinite(candidate.double_value)) { return 22; } }
        else { candidate.integer_value=parameter_wire_integer(bytes,type==parameter_kind::int32?4:8); }
        output=std::move(candidate);return 0;
    }
    // Complete value/size validation precedes allocation. The same checked
    // payload can then be appended directly into an event's final buffer.
    inline std::uint32_t parameter_encoded_size(parameter_value const& value,std::size_t& output)
    {
        std::size_t total{};
        if(array_parameter(value.type))
        {
            auto count{parameter_array_size(value)};if(count>parameter_max_elements) { return 90; }
            total=4;
            if(value.type==parameter_kind::string_array)
            {
                for(auto const& item:value.string_array_value)
                { if(parameter_wire_limit-total<4||item.size()>parameter_wire_limit-total-4) { return 90; }if(!parameter_array_string_valid(version::ros2,item)) { return 22; }total+=4+item.size(); }
            }
            else
            {
                auto width{parameter_element_width(value.type)};if(count>(parameter_wire_limit-4)/width) { return 90; }total+=count*width;
                if(value.type==parameter_kind::boolean_array) { for(auto item:value.bool_array_value) { if(item>1) { return 22; } } }
                if(value.type==parameter_kind::float64_array) { for(auto item:value.double_array_value) { if(!std::isfinite(item)) { return 22; } } }
            }
        }
        else if(value.type==parameter_kind::string)
        {
            if(value.string_value.size()>parameter_wire_limit-4) { return 90; }
            if(!parameter_payload_utf8(reinterpret_cast<std::uint8_t const*>(value.string_value.data()),value.string_value.size())) { return 22; }
            total=4+value.string_value.size();
        }
        else if(value.type==parameter_kind::boolean) { total=1; }
        else if(value.type==parameter_kind::float64) { if(!std::isfinite(value.double_value)) { return 22; }total=8; }
        else if(value.type==parameter_kind::int32||value.type==parameter_kind::int64)
        {
            if(value.type==parameter_kind::int32&&(value.integer_value<std::numeric_limits<std::int32_t>::min()||value.integer_value>std::numeric_limits<std::int32_t>::max())) { return 13; }
            total=value.type==parameter_kind::int32?4:8;
        }
        else { return 95; }
        output=total;return 0;
    }
    namespace parameter_detail
    {
        // Call only after parameter_encoded_size succeeds.
        inline void append_checked(parameter_value const& value,std::vector<std::uint8_t>& bytes)
        {
            if(array_parameter(value.type))
            {
                put(bytes,parameter_array_size(value),4);
                switch(value.type)
                {
                case parameter_kind::boolean_array:bytes.insert(bytes.end(),value.bool_array_value.begin(),value.bool_array_value.end());break;
                case parameter_kind::byte_array:bytes.insert(bytes.end(),value.byte_array_value.begin(),value.byte_array_value.end());break;
                case parameter_kind::int32_array:for(auto item:value.int32_array_value) { put(bytes,static_cast<std::uint32_t>(item),4); }break;
                case parameter_kind::int64_array:for(auto item:value.int64_array_value) { put(bytes,static_cast<std::uint64_t>(item),8); }break;
                case parameter_kind::float64_array:for(auto item:value.double_array_value) { std::uint64_t bits{};std::memcpy(&bits,&item,8);put(bytes,bits,8); }break;
                case parameter_kind::string_array:for(auto const& item:value.string_array_value) { put(bytes,item.size(),4);bytes.insert(bytes.end(),item.begin(),item.end()); }break;
                default:break;
                }
            }
            else if(value.type==parameter_kind::string)
            { put(bytes,value.string_value.size(),4);bytes.insert(bytes.end(),value.string_value.begin(),value.string_value.end()); }
            else if(value.type==parameter_kind::boolean) { bytes.push_back(value.bool_value?1:0); }
            else if(value.type==parameter_kind::float64)
            { std::uint64_t bits{};std::memcpy(&bits,&value.double_value,8);put(bytes,bits,8); }
            else { put(bytes,static_cast<std::uint64_t>(value.integer_value),value.type==parameter_kind::int32?4:8); }
        }
    }
    inline std::uint32_t encode_parameter(parameter_value const& value,std::vector<std::uint8_t>& output)
    {
        std::size_t size{};auto error{parameter_encoded_size(value,size)};if(error) { return error; }
        std::vector<std::uint8_t> candidate;candidate.reserve(size);parameter_detail::append_checked(value,candidate);
        output=std::move(candidate);return 0;
    }
    inline std::uint32_t validate_parameter_contract(version backend,capability_spec const& cap)
    {
        auto minimum{cap.limits.find("parameter-min-value")},maximum{cap.limits.find("parameter-max-value")};
        auto elements{cap.limits.find("max-parameter-elements")};
        bool bounded{minimum!=cap.limits.end()||maximum!=cap.limits.end()};
        bool event{backend==version::ros2&&cap.operation=="param.subscribe-events"&&parameter_event_type(backend,cap.type)!=parameter_kind::unsupported};
        if(!scalar_parameter_operation(backend,cap.operation)&&!event)
        {
            if(elements!=cap.limits.end()) { return 22; }
            if(cap.operation.compare(0,6,"param.")==0)
            {
                if(cap.operation=="param.subscribe-events")
                { if(backend!=version::ros2||cap.type!="rcl_interfaces/msg/ParameterEvent") { return 95; } }
                else if(cap.type!="builtin/string-v1") { return 95; }
                else if(cap.type_identity!=parameter_identity(backend,cap.type)) { return 13; }
            }
            return bounded?22:0;
        }
        auto type{event?parameter_event_type(backend,cap.type):parameter_type(backend,cap.type)};
        if(type==parameter_kind::unsupported) { return 95; }
        if((event?parameter_event_identity(backend,cap.type):parameter_identity(backend,cap.type))!=cap.type_identity) { return 13; }
        if(array_parameter(type))
        {
            std::int64_t count{};
            if(elements==cap.limits.end()||!parse_parameter_integer(elements->second,count)||count<=0||static_cast<std::uint64_t>(count)>parameter_max_elements) { return 22; }
        }
        else if(elements!=cap.limits.end()) { return 22; }
        auto quota{cap.limits.find("max-parameter-bytes")};std::int64_t bytes{};
        if(quota==cap.limits.end()||!parse_parameter_integer(quota->second,bytes)||
           bytes<(event?1:static_cast<std::int64_t>(parameter_width(type)))) { return 22; }
        if(!numeric_parameter(type)) { return bounded?22:0; }
        if(minimum==cap.limits.end()||maximum==cap.limits.end()) { return 22; }
        if(float64_parameter(type))
        { double lo{},hi{};return !parse_parameter_real(minimum->second,lo)||!parse_parameter_real(maximum->second,hi)||lo>hi?22:0; }
        std::int64_t lo{},hi{};
        if(!parse_parameter_integer(minimum->second,lo)||!parse_parameter_integer(maximum->second,hi)||lo>hi) { return 22; }
        return parameter_element_kind(type)==parameter_kind::int32&&(lo<std::numeric_limits<std::int32_t>::min()||hi>std::numeric_limits<std::int32_t>::max())?22:0;
    }
    inline std::uint32_t validate_parameter_event_value(version backend,capability_spec const& cap,parameter_value const& value)
    {
        if(backend!=version::ros2||cap.operation!="param.subscribe-events"||parameter_event_type(backend,cap.type)==parameter_kind::unsupported) { return 95; }
        auto error{validate_parameter_contract(backend,cap)};if(error) { return error; }
        if(value.type!=parameter_event_type(backend,cap.type)) { return 13; }
        if(array_parameter(value.type))
        {
            std::int64_t count{};parse_parameter_integer(cap.limits.at("max-parameter-elements"),count);
            if(parameter_array_size(value)>static_cast<std::uint64_t>(count)||parameter_array_size(value)>parameter_max_elements) { return 90; }
            if(value.type==parameter_kind::boolean_array)
            { for(auto item:value.bool_array_value) { if(item>1) { return 22; } } }
            else if(value.type==parameter_kind::string_array)
            { for(auto const& item:value.string_array_value) { if(!parameter_array_string_valid(backend,item)) { return 22; } } }
        }
        else if(value.type==parameter_kind::string)
        { if(!parameter_payload_utf8(reinterpret_cast<std::uint8_t const*>(value.string_value.data()),value.string_value.size())) { return 22; } }
        if(float64_parameter(value.type))
        {
            double lo{},hi{};parse_parameter_real(cap.limits.at("parameter-min-value"),lo);parse_parameter_real(cap.limits.at("parameter-max-value"),hi);
            auto check=[&](double item) -> std::uint32_t { return !std::isfinite(item)?22:item<lo||item>hi?13:0; };
            if(value.type==parameter_kind::float64_array)
            { for(auto item:value.double_array_value) { auto result{check(item)};if(result) { return result; } } }
            else { auto result{check(value.double_value)};if(result) { return result; } }
        }
        else if(numeric_parameter(value.type))
        {
            std::int64_t lo{},hi{};parse_parameter_integer(cap.limits.at("parameter-min-value"),lo);parse_parameter_integer(cap.limits.at("parameter-max-value"),hi);
            if(value.type==parameter_kind::int64_array)
            { for(auto item:value.int64_array_value) { if(item<lo||item>hi) { return 13; } } }
            else if(value.integer_value<lo||value.integer_value>hi) { return 13; }
        }
        return 0;
    }
    inline std::uint32_t validate_parameter_value(version backend,capability_spec const& cap,std::uint8_t const* bytes,std::size_t size,parameter_value* output=nullptr)
    {
        auto contract{validate_parameter_contract(backend,cap)};if(contract) { return contract; }
        auto quota{cap.limits.find("max-parameter-bytes")};std::int64_t maximum{};
        if(quota==cap.limits.end()||!parse_parameter_integer(quota->second,maximum)||maximum<0) { return 22; }
        if(size>static_cast<std::uint64_t>(maximum)) { return 90; }
        auto type{parameter_type(backend,cap.type)};std::size_t elements{parameter_max_elements};
        if(array_parameter(type)) { std::int64_t count{};parse_parameter_integer(cap.limits.at("max-parameter-elements"),count);elements=static_cast<std::size_t>(count); }
        if(array_parameter(type))
        {
            // Validate borrowed wire bytes before allocating a decoded value.
            // Shape/finite/Bool/UTF-8 errors still precede the element quota;
            // ROS1 XML compatibility and every numeric bound follow it.
            std::uint32_t count{};auto error{preflight_parameter_array(type,bytes,size,elements,count)};if(error) { return error; }
            if(type==parameter_kind::string_array&&backend==version::ros1)
            {
                std::size_t p{4};
                for(std::uint32_t i{};i<count;++i)
                {
                    auto length{static_cast<std::size_t>(get(bytes+p,4))};p+=4;
                    if(!parameter_array_string_valid(backend,{reinterpret_cast<char const*>(bytes+p),length})) { return 22; }p+=length;
                }
            }
            if(numeric_parameter(type))
            {
                auto width{parameter_element_width(type)};
                if(float64_parameter(type))
                {
                    double lo{},hi{};parse_parameter_real(cap.limits.at("parameter-min-value"),lo);parse_parameter_real(cap.limits.at("parameter-max-value"),hi);
                    for(std::uint32_t i{};i<count;++i)
                    {
                        auto bits{get(bytes+4+static_cast<std::size_t>(i)*width,8)};double value{};std::memcpy(&value,&bits,8);
                        if(value<lo||value>hi) { return 13; }
                    }
                }
                else
                {
                    std::int64_t lo{},hi{};parse_parameter_integer(cap.limits.at("parameter-min-value"),lo);parse_parameter_integer(cap.limits.at("parameter-max-value"),hi);
                    for(std::uint32_t i{};i<count;++i)
                    { auto value{parameter_wire_integer(bytes+4+static_cast<std::size_t>(i)*width,width)};if(value<lo||value>hi) { return 13; } }
                }
            }
            return output?decode_parameter(type,bytes,size,*output,elements):0;
        }
        if(type==parameter_kind::string&&!output)
        {
            // A validation-only caller borrows the canonical bytes. Decoding a
            // String here would allocate a copy that is immediately discarded.
            if((!bytes&&size)||size<4||get(bytes,4)!=size-4||!parameter_payload_utf8(bytes+4,size-4)) { return 22; }
            return 0;
        }
        parameter_value candidate;auto decoded{decode_parameter(type,bytes,size,candidate,elements)};if(decoded) { return decoded; }
        if(numeric_parameter(candidate.type))
        {
            if(float64_parameter(candidate.type))
            {
                double lo{},hi{};parse_parameter_real(cap.limits.at("parameter-min-value"),lo);parse_parameter_real(cap.limits.at("parameter-max-value"),hi);
                if(candidate.double_value<lo||candidate.double_value>hi) { return 13; }
            }
            else
            {
                std::int64_t lo{},hi{};parse_parameter_integer(cap.limits.at("parameter-min-value"),lo);parse_parameter_integer(cap.limits.at("parameter-max-value"),hi);
                if(candidate.integer_value<lo||candidate.integer_value>hi) { return 13; }
            }
        }
        if(output) { *output=std::move(candidate); }return 0;
    }
}
