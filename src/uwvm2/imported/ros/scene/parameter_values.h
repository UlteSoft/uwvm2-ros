/** @file
 * @brief Allocation-free ROS parameter algebra and numeric constraint validation.
 * @details ROS1 XML-RPC and ROS2 parameter variants are separate explicit
 * deployment choices. Limits default to zero; parsing cannot widen them.
 * This layer performs no declaration, mutation, native call or authorization.
 * A provider must also check the parameter name, descriptor, live owner,
 * revision and independent action grant before storing the validated bytes.
 */
#pragma once
#include "wire.h"
#include <array>
#include <cmath>
#include <limits>
#include <string_view>

namespace uwvm2::imported::ros::scene::parameter_values
{
    enum class algebra : std::uint8_t {disabled,ros1,ros2};
    enum class kind : std::uint64_t
    {not_set,boolean,integer,floating,string,bytes,booleans,integers,floats,strings,xmlrpc_array=32,xmlrpc_struct=33,xmlrpc_datetime=34};
    inline constexpr std::size_t maximum_depth=16,maximum_struct_members=128;
    /** @brief Finite whole-record parsing reservations, independent of rate limits.
     * @details Nodes count the root and each nested/array value. Elements are
     * cumulative across arrays and dictionaries, not a fresh limit per level.
     * Nonfinite floating values require an explicit profile choice and are
     * never allowed as range endpoints or inside a constrained numeric value.
     */
    struct limits
    {std::uint64_t bytes{},nodes{},elements{},depth{};algebra selected{};bool nonfinite{};};
    struct budget {std::uint64_t nodes{},elements{};};
    /** @brief Decoded root scalar summary; raw values remain in owned canonical bytes. */
    struct scalar {kind type{};std::int64_t integer{};double floating{};};
    inline bool valid(limits const& value) noexcept
    {return value.bytes&&value.bytes<=4u*1024u*1024u&&value.nodes&&value.nodes<=65536&&value.elements<=65536&&
        value.depth&&value.depth<=maximum_depth&&(value.selected==algebra::ros1||value.selected==algebra::ros2);}
    inline bool consume(std::uint64_t& available,std::uint64_t count) noexcept
    {if(count>available)return false;available-=count;return true;}
    /** @brief Validate the XML-RPC calendar spelling without inventing a timezone.
     * @details The portable spelling is YYYYMMDDThh:mm:ss, with valid Gregorian
     * calendar fields and seconds 0..59. It is a lexical value, not a security
     * deadline or an implicit system/ROS clock timestamp.
     */
    inline bool datetime(std::string_view text) noexcept
    {
        if(text.size()!=17||text[8]!='T'||text[11]!=':'||text[14]!=':')return false;
        for(std::size_t n{};n<text.size();++n)if(n!=8&&n!=11&&n!=14&&(text[n]<'0'||text[n]>'9'))return false;
        auto pair=[&](std::size_t offset){return unsigned(text[offset]-'0')*10+unsigned(text[offset+1]-'0');};
        auto year=pair(0)*100+pair(2),month=pair(4),day=pair(6);
        if(!year||!month||month>12||pair(9)>23||pair(12)>59||pair(15)>59)return false;
        constexpr std::array<unsigned,12> days{31,28,31,30,31,30,31,31,30,31,30,31};
        auto maximum=days[month-1]+(month==2&&year%4==0&&(year%100!=0||year%400==0));
        return day&&day<=maximum;
    }
    /** @brief Reject characters that cannot be represented by XML 1.0 text.
     * @details UTF-8 validity is already enforced by wire::reader. No escaping
     * or normalization is performed; the native XML encoder must still escape
     * markup. Strings remain data and cannot select a provider or endpoint.
     */
    inline bool xml_text(std::string_view value) noexcept
    {
        for(std::size_t n{};n<value.size();++n)
        {
            auto byte=static_cast<unsigned char>(value[n]);
            if(byte<0x20&&byte!=9&&byte!=10&&byte!=13)return false;
            if(byte==0xef&&n+2<value.size()&&static_cast<unsigned char>(value[n+1])==0xbf&&
               (static_cast<unsigned char>(value[n+2])==0xbe||static_cast<unsigned char>(value[n+2])==0xbf))return false;
        }
        return true;
    }
    /** @brief Parse one value with shared depth/node/element accounting.
     * @details A caller parsing an enclosing transaction passes one budget to
     * every value. Unknown variants and cross-algebra values fail before any
     * allocation. The output scalar is changed only after a complete value.
     */
    inline bool read(wire::reader& input,limits const& policy,budget& remaining,scalar& output,std::uint64_t depth=1) noexcept
    {
        if(!valid(policy)||depth>policy.depth||!consume(remaining.nodes,1))return input.reject();
        std::uint64_t raw{};if(!input.u64(raw))return false;
        bool ros1=policy.selected==algebra::ros1;
        if(ros1?(raw<1||raw>5)&&(raw<32||raw>34):raw>9)return input.reject();
        scalar result;result.type=static_cast<kind>(raw);bool boolean{};std::string_view text;std::span<std::uint8_t const> blob;
        auto string=[&]{return input.text(text)&&(!ros1||xml_text(text));};
        auto floating=[&](double& value){return input.f64(value)&&(policy.nonfinite||std::isfinite(value));};
        std::uint64_t count{};
        switch(result.type)
        {
        case kind::not_set:break;
        case kind::boolean:if(!input.boolean(boolean))return false;break;
        case kind::integer:
            if(!input.i64(result.integer)||(ros1&&(result.integer<INT32_MIN||result.integer>INT32_MAX)))return input.reject();
            break;
        case kind::floating:if(!floating(result.floating))return input.reject();break;
        case kind::string:if(!string())return input.reject();break;
        case kind::bytes:if(!input.blob(blob)||!consume(remaining.elements,blob.size()))return input.reject();break;
        case kind::xmlrpc_datetime:if(!string()||!datetime(text))return input.reject();break;
        default:
            if(!input.u64(count)||!consume(remaining.elements,count))return input.reject();
            if(result.type==kind::xmlrpc_struct)
            {
                if(count>maximum_struct_members)return input.reject();
                std::array<std::string_view,maximum_struct_members> names{};
                for(std::uint64_t i{};i<count;++i)
                {
                    if(!string())return input.reject();
                    for(std::uint64_t j{};j<i;++j)if(names[j]==text)return input.reject();
                    names[i]=text;scalar nested;if(!read(input,policy,remaining,nested,depth+1))return false;
                }
            }
            else for(std::uint64_t i{};i<count;++i)
            {
                if(result.type==kind::xmlrpc_array)
                {scalar nested;if(!read(input,policy,remaining,nested,depth+1))return false;continue;}
                if(!consume(remaining.nodes,1))return input.reject();
                switch(result.type)
                {
                case kind::booleans:if(!input.boolean(boolean))return false;break;
                case kind::integers:{std::int64_t value{};if(!input.i64(value))return false;break;}
                case kind::floats:{double value{};if(!floating(value))return input.reject();break;}
                case kind::strings:if(!string())return input.reject();break;
                default:return input.reject();
                }
            }
        }
        output=result;return true;
    }
    /** @brief Validate one entire owned value; trailing bytes are rejected. */
    inline bool validate(std::span<std::uint8_t const> bytes,limits const& policy,scalar& output) noexcept
    {
        if(!valid(policy)||bytes.size()>policy.bytes)return false;
        wire::reader input{bytes};budget remaining{policy.nodes,policy.elements};scalar result;
        if(!read(input,policy,remaining,result)||!input.done())return false;
        output=result;return true;
    }
    /** @brief Preserve ROS integer range semantics including the full unsigned step.
     * @details The upper endpoint is accepted even when not a step multiple.
     * Unsigned distance avoids signed overflow across INT64_MIN..INT64_MAX.
     * The u64 step is independent of the signed parameter value algebra.
     */
    struct integer_range
    {
        std::int64_t minimum{},maximum{};std::uint64_t step{};
        bool valid() const noexcept{return minimum<=maximum;}
        bool contains(std::int64_t value) const noexcept
        {
            if(!valid()||value<minimum||value>maximum)return false;
            if(value==maximum||!step)return true;
            auto distance=static_cast<std::uint64_t>(value)-static_cast<std::uint64_t>(minimum);
            return distance%step==0;
        }
    };
    /** @brief Finite floating range with explicit bounded comparison tolerance.
     * @details Native ROS accepts endpoints and treats the step as magnitude.
     * The trusted profile supplies absolute tolerance (default exact zero),
     * never a guest command. Endpoints remain hard bounds; tolerance applies
     * only to a step reconstruction inside them. Nonfinite intermediate math
     * refuses the value instead of silently disabling the constraint.
     */
    struct floating_range
    {
        double minimum{},maximum{},step{},absolute_tolerance{};
        bool valid() const noexcept
        {return std::isfinite(minimum)&&std::isfinite(maximum)&&std::isfinite(step)&&std::isfinite(absolute_tolerance)&&
            minimum<=maximum&&absolute_tolerance>=0;}
        bool contains(double value) const noexcept
        {
            if(!valid()||!std::isfinite(value)||value<minimum||value>maximum)return false;
            if(value==minimum||value==maximum||step==0)return true;
            auto magnitude=std::abs(static_cast<long double>(step));
            auto distance=static_cast<long double>(value)-static_cast<long double>(minimum);
            auto extent=static_cast<long double>(maximum)-static_cast<long double>(minimum);
            if(!std::isfinite(extent)||magnitude>=extent)return false;
            auto ratio=distance/magnitude;if(!std::isfinite(ratio))return false;
            auto reconstructed=std::round(ratio)*magnitude;
            return std::isfinite(reconstructed)&&std::abs(reconstructed-distance)<=static_cast<long double>(absolute_tolerance);
        }
    };
}
