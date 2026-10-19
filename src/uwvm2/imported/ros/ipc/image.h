/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include "protocol.h"
#include <exception>
#include <stdexcept>
#include <string_view>
namespace uwvm2::imported::ros::ipc
{
    struct image
    {
        std::uint32_t sequence{}, seconds{}, nanoseconds{}, height{}, width{}, step{};
        std::string frame_id{}, encoding{};
        bool bigendian{};
        std::vector<std::uint8_t> data{};
    };
    // The caller keeps these fields alive for the encoding expression. Native
    // subscription callbacks can borrow their generated Image without first
    // copying its strings and complete pixel buffer into an owning image.
    struct image_view
    {
        std::uint32_t sequence{}, seconds{}, nanoseconds{}, height{}, width{}, step{};
        std::string_view frame_id{}, encoding{};
        bool bigendian{};
        std::uint8_t const* data{};
        std::size_t data_size{};
    };
    // Shared allocation-free admission for a borrowed canonical/native Image.
    // Callers retain the strings/pixels for this expression. This is the exact
    // existing bounded Image policy, not an expansion of native encoding rights.
    inline bool valid_image_view(image_view const& value,std::uint8_t backend) noexcept
    {
        if(backend<1||backend>2||value.nanoseconds>=1000000000||
           (backend==2&&value.seconds>2147483647)||!value.height||!value.width||
           value.frame_id.empty()||value.frame_id.size()>255||value.encoding.size()>255||
           value.data_size>UINT32_MAX||(!value.data&&value.data_size)) {return false;}
        for(char c:value.frame_id) { if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='/')) { return false; } }
        if(value.frame_id.front()=='/'||value.frame_id.back()=='/'||value.frame_id.find("//")!=std::string_view::npos) { return false; }
        std::uint64_t stride{};
        if(value.encoding=="mono8") { stride=1; }
        else if(value.encoding=="mono16") { stride=2; }
        else if(value.encoding=="rgb8"||value.encoding=="bgr8") { stride=3; }
        else if(value.encoding=="rgba8"||value.encoding=="bgra8") { stride=4; }
        else { return false; }
        return std::uint64_t{value.width}*stride<=value.step&&std::uint64_t{value.step}*value.height==value.data_size;
    }
    // Admission borrows strings/pixels from the canonical input. It does not
    // allocate another multi-megabyte Image merely to validate its shape.
    // The caller must keep bytes alive while using the returned view.
    inline bool decode_image_view(std::uint8_t const* bytes,std::size_t size,std::uint8_t backend,image_view& out) noexcept
    {
        if(!bytes||!size||backend<1||backend>2) {return false;}
        image_view value;
        std::size_t p{};
        auto number=[&](std::uint32_t& n) { if(p>size||size-p<4) { return false; } n=static_cast<std::uint32_t>(get(bytes+p,4)); p+=4; return true; };
        auto string=[&](std::string_view& s) { std::uint32_t n{}; if(!number(n)||n>255||p>size||n>size-p) { return false; } s={reinterpret_cast<char const*>(bytes+p),n}; p+=n; return true; };
        if(!number(value.sequence)||!number(value.seconds)||!number(value.nanoseconds)||!string(value.frame_id)||!number(value.height)||!number(value.width)||!string(value.encoding)||p==size) { return false; }
        auto e{bytes[p++]}; if(e>1) { return false; } value.bigendian=e!=0;
        std::uint32_t n{};
        if(!number(value.step)||!number(n)||p>size||n!=size-p) { return false; }
        value.data=bytes+p;value.data_size=n;if(!valid_image_view(value,backend)) {return false;}out=value;return true;
    }
    inline bool decode_image(std::vector<std::uint8_t> const& bytes,std::uint8_t backend,image& out)
    {
        image_view view;if(!decode_image_view(bytes.data(),bytes.size(),backend,view)) {return false;}
        image value;value.sequence=view.sequence;value.seconds=view.seconds;value.nanoseconds=view.nanoseconds;
        value.height=view.height;value.width=view.width;value.step=view.step;value.bigendian=view.bigendian;
        value.frame_id=view.frame_id;value.encoding=view.encoding;
        value.data.assign(view.data,view.data+view.data_size);out=std::move(value);return true;
    }
    inline std::vector<std::uint8_t> encode_image_view(image_view const& in)
    {
        std::vector<std::uint8_t> b;
        std::size_t total{37};
        for(auto length:{in.frame_id.size(),in.encoding.size(),in.data_size})
        {
            if(total>b.max_size()||length>b.max_size()-total)
            {
#if defined(__cpp_exceptions)
                throw std::length_error("canonical Image exceeds representable output size");
#else
                std::terminate();
#endif
            }
            total+=length;
        }
        b.reserve(total);
        put(b,in.sequence,4); put(b,in.seconds,4); put(b,in.nanoseconds,4); put(b,in.frame_id.size(),4);
        if(!in.frame_id.empty()) { b.insert(b.end(),in.frame_id.begin(),in.frame_id.end()); }
        put(b,in.height,4); put(b,in.width,4); put(b,in.encoding.size(),4);
        if(!in.encoding.empty()) { b.insert(b.end(),in.encoding.begin(),in.encoding.end()); }
        put(b,in.bigendian?1:0,1); put(b,in.step,4); put(b,in.data_size,4);
        if(in.data_size) { b.insert(b.end(),in.data,in.data+in.data_size); }return b;
    }
    inline std::vector<std::uint8_t> encode_image(image const& in)
    {
        return encode_image_view({in.sequence,in.seconds,in.nanoseconds,in.height,in.width,in.step,
                                  in.frame_id,in.encoding,in.bigendian,in.data.data(),in.data.size()});
    }
}
