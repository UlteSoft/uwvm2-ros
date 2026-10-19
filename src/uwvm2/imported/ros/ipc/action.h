/*************************************************************
 * UWVM2 canonical generated Fibonacci Action IPC. APL-2.0. *
 *************************************************************/
#pragma once
#include "protocol.h"
#include <cstring>
namespace uwvm2::imported::ros::ipc
{
    inline bool fibonacci_order(std::uint8_t const* data,std::size_t size,std::int32_t& order) noexcept
    {
        if(!data||size!=4) { return false; }auto bits{static_cast<std::uint32_t>(get(data,4))};std::memcpy(&order,&bits,4);return order>=0&&order<=46;
    }
    inline bool fibonacci_order(std::vector<std::uint8_t> const& data,std::int32_t& order)
    {return fibonacci_order(data.data(),data.size(),order);}
    // Admission checks inspect the existing canonical bytes directly. They do
    // not need a copied byte vector or an allocated decoded sequence merely to
    // prove the bounded shape and nonnegative generated Fibonacci values.
    inline bool validate_action_sequence(std::uint8_t const* data,std::size_t size,bool has_kind) noexcept
    {
        auto base{std::size_t{12}+(has_kind?1:0)};
        if(!data||size<base||size>max_payload_bytes-518||!get(data,8)) {return false;}
        if(has_kind&&(data[8]<1||data[8]>4)) {return false;}
        auto count{get(data+(has_kind?9:8),4)};
        if(count>48||(size-base)%4||count!=(size-base)/4) {return false;}
        for(std::size_t position{base};position<size;position+=4)
        {if(get(data+position,4)>0x7fffffffU) {return false;}}
        return true;
    }
    struct action_sequence { std::uint64_t goal{};std::uint8_t kind{};std::vector<std::int32_t> values{}; };
    inline bool decode_action_sequence(std::vector<std::uint8_t> const& data,bool has_kind,action_sequence& sequence)
    {
        auto base{std::size_t{12}+(has_kind?1:0)};if(!validate_action_sequence(data.data(),data.size(),has_kind)) { return false; }
        action_sequence parsed;parsed.goal=get(data.data(),8);
        if(has_kind) { parsed.kind=data[8]; }
        auto n{get(data.data()+(has_kind?9:8),4)};
        parsed.values.reserve(static_cast<std::size_t>(n));
        for(std::size_t p{base};p<data.size();p+=4) { auto bits{static_cast<std::uint32_t>(get(data.data()+p,4))};std::int32_t value{};std::memcpy(&value,&bits,4);if(value<0) { return false; }parsed.values.push_back(value); }
        sequence=std::move(parsed);return true;
    }
    inline std::vector<std::uint8_t> encode_action_sequence(action_sequence const& sequence,bool has_kind)
    {
        std::vector<std::uint8_t> data;put(data,sequence.goal,8);if(has_kind) { put(data,sequence.kind,1); }put(data,sequence.values.size(),4);
        for(auto value:sequence.values) { std::uint32_t bits{};std::memcpy(&bits,&value,4);put(data,bits,4); }return data;
    }
    inline std::vector<std::uint8_t> encode_action_goal(std::uint64_t goal,std::int32_t order)
    { std::vector<std::uint8_t> data;put(data,goal,8);std::uint32_t bits{};std::memcpy(&bits,&order,4);put(data,bits,4);return data; }
}
