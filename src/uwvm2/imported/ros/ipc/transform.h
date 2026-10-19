/*************************************************************
 * UWVM2 canonical TransformStamped IPC v1. APL-2.0.          *
 *************************************************************/
#pragma once
#include "protocol.h"
#include <array>
#include <cmath>
#include <cstddef>
#include <cstring>

namespace uwvm2::imported::ros::ipc
{
    struct transform_data
    {
        std::uint32_t sequence{}, seconds{}, nanoseconds{};
        std::string parent{}, child{};
        // translation x/y/z; quaternion x/y/z/w. No implicit normalization.
        std::array<double,7> values{{0,0,0,0,0,0,1}};
    };
    inline bool valid_frame(std::string const& name) noexcept
    {
        if(name.empty()||name.size()>255||name.front()=='/'||name.back()=='/') { return false; }
        bool first{true};
        for(char c:name)
        {
            if(c=='/') { if(first) { return false; } first=true; continue; }
            bool alpha{(c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_'};
            if(!alpha&&!(c>='0'&&c<='9'&&!first)) { return false; }
            first=false;
        }
        return !first;
    }
    inline bool valid_transform(transform_data const& t) noexcept
    {
        if(!valid_frame(t.parent)||!valid_frame(t.child)||t.parent==t.child||t.seconds>2147483647U||t.nanoseconds>=1000000000U) { return false; }
        for(double value:t.values) { if(!std::isfinite(value)) { return false; } }
        double norm{};
        for(unsigned i=3;i!=7;++i) { norm+=t.values[i]*t.values[i]; }
        return std::isfinite(norm)&&std::abs(norm-1.)<=0.001;
    }
    inline std::vector<std::uint8_t> encode_transform(transform_data const& t)
    {
        std::vector<std::uint8_t> out;
        put(out,t.sequence,4); put(out,t.seconds,4); put(out,t.nanoseconds,4);
        put(out,t.parent.size(),4); out.insert(out.end(),t.parent.begin(),t.parent.end());
        put(out,t.child.size(),4); out.insert(out.end(),t.child.begin(),t.child.end());
        for(double value:t.values) { std::uint64_t bits{}; std::memcpy(&bits,&value,8); put(out,bits,8); }
        return out;
    }
    // The existing canonical decoder's complete extent bound: two nonempty
    // frame names of at most255 bytes plus the fixed scalar/length fields.
    // A caller may reject this size before copying, but must still decode and
    // validate the complete owned payload; length alone never admits a frame.
    inline constexpr bool valid_transform_extent(std::size_t size) noexcept
    { return size>=78&&size<=586; }
    inline bool decode_transform(std::vector<std::uint8_t> const& in,transform_data& t)
    {
        if(!valid_transform_extent(in.size())) { return false; }
        transform_data parsed;
        parsed.sequence=static_cast<std::uint32_t>(get(in.data(),4));
        parsed.seconds=static_cast<std::uint32_t>(get(in.data()+4,4));
        parsed.nanoseconds=static_cast<std::uint32_t>(get(in.data()+8,4));
        std::size_t offset{12};
        auto string{[&](std::string& result)
        {
            if(in.size()-offset<4) { return false; }
            auto length{get(in.data()+offset,4)}; offset+=4;
            if(!length||length>255||length>in.size()-offset) { return false; }
            result.assign(reinterpret_cast<char const*>(in.data()+offset),static_cast<std::size_t>(length));
            offset+=static_cast<std::size_t>(length); return true;
        }};
        if(!string(parsed.parent)||!string(parsed.child)||in.size()-offset!=56) { return false; }
        for(double& value:parsed.values) { auto bits{get(in.data()+offset,8)}; std::memcpy(&value,&bits,8); offset+=8; }
        if(!valid_transform(parsed)) { return false; }
        t=std::move(parsed); return true;
    }
}
