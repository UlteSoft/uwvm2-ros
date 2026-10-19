#pragma once
/* Bounded DATA parser only. It issues no memory read, stop or authorization. */
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <utility>
#include <vector>
#include "../include/uwvm_language_snapshot.h"
#if !defined(__cpp_exceptions) && !defined(_CPPUNWIND)
#error "The bounded Host reader requires the admitted exception-enabled Host build"
#endif

namespace uwvm_language_debug {
enum class status { ok, malformed, unavailable, allocation_failure };
struct snapshot {
    uwvm_language_snapshot_header header{};
    std::array<uwvm_language_snapshot_frame, UWVM_LS_MAX_FRAMES> frames{};
    std::array<uwvm_language_snapshot_value, UWVM_LS_MAX_VALUES> values{};
    std::vector<std::byte> text{}; // detached owned bytes; includes binary payloads
};
namespace detail {
inline std::uint32_t u32(std::span<std::byte const> b, std::size_t n) noexcept {
    std::uint32_t value{};
    for (unsigned i{}; i != 4; ++i) value |= std::uint32_t(std::to_integer<unsigned char>(b[n+i])) << (i*8);
    return value;
}
inline std::uint64_t u64(std::span<std::byte const> b, std::size_t n) noexcept {
    return u32(b,n) | (std::uint64_t(u32(b,n+4)) << 32);
}
inline bool zero(std::span<std::byte const> b, std::size_t first, std::size_t last) noexcept {
    for (; first != last; ++first) if (b[first] != std::byte{}) return false;
    return true;
}
inline bool range(std::uint32_t length, std::uint32_t offset, std::uint32_t size) noexcept {
    return offset <= length && size <= length-offset;
}
inline bool utf8_count(std::span<std::byte const> b, std::uint64_t& count) noexcept {
    std::uint64_t candidate{};
    for (std::size_t i{}; i < b.size();) {
        ++candidate;
        auto c = std::to_integer<unsigned char>(b[i++]);
        if (c < 0x80) continue;
        unsigned n{}; std::uint32_t v{}, minimum{};
        if (c >= 0xc2 && c <= 0xdf) { n=1; v=c&31; minimum=0x80; }
        else if (c >= 0xe0 && c <= 0xef) { n=2; v=c&15; minimum=0x800; }
        else if (c >= 0xf0 && c <= 0xf4) { n=3; v=c&7; minimum=0x10000; }
        else return false;
        if (n > b.size()-i) return false;
        while (n--) { auto next=std::to_integer<unsigned char>(b[i++]);
            if ((next&0xc0)!=0x80) return false; v=(v<<6)|(next&63); }
        if (v<minimum || v>0x10ffff || (v>=0xd800 && v<=0xdfff)) return false;
    }
    count=candidate;
    return true;
}
inline bool utf8(std::span<std::byte const> b) noexcept {
    std::uint64_t ignored{};
    return utf8_count(b,ignored);
}
}

namespace detail {
inline status parse_unaliased(std::span<std::byte const> bytes, snapshot& out) noexcept {
    out = {};
    if (bytes.size()!=UWVM_LS_TOTAL_BYTES) return status::malformed;
    static constexpr unsigned char magic[]{'U','W','L','F',0,0,1,0};
    for (unsigned i{}; i!=8; ++i) if (std::to_integer<unsigned char>(bytes[i])!=magic[i]) return status::malformed;
    snapshot candidate{}; auto& h=candidate.header;
    std::memcpy(h.magic,magic,8); h.schema=u32(bytes,8); h.total_bytes=u32(bytes,12);
    h.header_bytes=u32(bytes,16); h.flags=u32(bytes,20); h.sequence=u64(bytes,24);
    h.layout_generation=u64(bytes,32); h.language=u32(bytes,40); h.frame_count=u32(bytes,44);
    h.value_count=u32(bytes,48); h.text_bytes=u32(bytes,52); h.frame_offset=u32(bytes,56);
    h.value_offset=u32(bytes,60); h.text_offset=u32(bytes,64); h.status=u32(bytes,68); h.event=u32(bytes,72);
    if (h.schema!=1 || h.total_bytes!=UWVM_LS_TOTAL_BYTES || h.header_bytes!=128 ||
        h.flags>15 || !(h.flags&UWVM_LS_LIVE) || !(h.flags&UWVM_LS_INSTRUMENTED) ||
        !h.sequence || (h.sequence&1) || h.layout_generation!=1 || h.language<1 || h.language>4 ||
        !h.frame_count || h.frame_count>16 || h.value_count>512 || h.text_bytes>24576 ||
        h.frame_offset!=128 || h.value_offset!=1664 || h.text_offset!=34432 ||
        h.status>1 || h.status!=!!(h.flags&UWVM_LS_CAPTURE_ERROR) || !zero(bytes,76,128)) return status::malformed;
    if (h.status) return status::unavailable;
    auto text=bytes.subspan(h.text_offset,h.text_bytes);
    auto string=[&](std::uint32_t offset,std::uint32_t length) noexcept {
        return range(h.text_bytes,offset,length) && utf8(text.subspan(offset,length));
    };
    for (std::uint32_t n{}; n!=h.frame_count; ++n) {
        auto off=128+std::size_t(n)*96; auto& f=candidate.frames[n];
        f.id=u32(bytes,off); f.parent=u32(bytes,off+4); f.line=u32(bytes,off+8); f.flags=u32(bytes,off+12);
        f.instruction=u64(bytes,off+16); f.file_offset=u32(bytes,off+24); f.file_length=u32(bytes,off+28);
        f.function_offset=u32(bytes,off+32); f.function_length=u32(bytes,off+36);
        f.value_first=u32(bytes,off+40); f.value_count=u32(bytes,off+44);
        f.argument_count=u32(bytes,off+48); f.known=u32(bytes,off+52);
        if (f.id!=n+1 || f.parent!=(n?n:UINT32_MAX) || (f.flags&~UWVM_LS_TRUNCATED) ||
            f.known>31 || f.value_count>32 || !range(h.value_count,f.value_first,f.value_count) ||
            f.argument_count>f.value_count || !zero(bytes,off+56,off+96) ||
            !string(f.file_offset,f.file_length) || !string(f.function_offset,f.function_length) ||
            (!(f.known&UWVM_LS_FILE_KNOWN) && (f.file_offset||f.file_length)) ||
            (!(f.known&UWVM_LS_FUNCTION_KNOWN) && (f.function_offset||f.function_length)) ||
            (!(f.known&UWVM_LS_LINE_KNOWN) && f.line) ||
            (!(f.known&UWVM_LS_INSTRUCTION_KNOWN) && f.instruction) ||
            (!(f.known&UWVM_LS_ARGUMENTS_KNOWN) && f.argument_count)) return status::malformed;
    }
    for (std::uint32_t n{}; n!=h.value_count; ++n) {
        auto off=1664+std::size_t(n)*64; auto& v=candidate.values[n];
        v.frame_id=u32(bytes,off); v.parent_value=u32(bytes,off+4); v.name_offset=u32(bytes,off+8);
        v.name_length=u32(bytes,off+12); v.kind=u32(bytes,off+16); v.flags=u32(bytes,off+20);
        v.payload=u64(bytes,off+24); v.bytes_offset=u32(bytes,off+32); v.bytes_length=u32(bytes,off+36);
        v.child_first=u32(bytes,off+40); v.child_count=u32(bytes,off+44); v.known=u32(bytes,off+48);
        bool container=v.kind>=UWVM_LS_LIST && v.kind<=UWVM_LS_DICT;
        bool blob=v.kind==UWVM_LS_UTF8 || v.kind==UWVM_LS_BYTES;
        if (!v.frame_id || v.frame_id>h.frame_count || v.kind>UWVM_LS_U64 || v.flags>7 || v.known>7 ||
            v.child_count>32 || !range(h.value_count,v.child_first,v.child_count) ||
            !range(h.text_bytes,v.bytes_offset,v.bytes_length) || !string(v.name_offset,v.name_length) ||
            !zero(bytes,off+52,off+64) ||
            (!(v.known&UWVM_LS_NAME_KNOWN) && (v.name_offset||v.name_length)) ||
            ((v.flags&UWVM_LS_UNAVAILABLE) && (v.known&~UWVM_LS_NAME_KNOWN)) ||
            (!(v.known&UWVM_LS_DATA_KNOWN) && (v.payload||v.bytes_offset||v.bytes_length)) ||
            (!blob && (v.bytes_offset||v.bytes_length)) ||
            (!container && ((v.known&UWVM_LS_CHILDREN_KNOWN)||v.child_count||v.child_first)) ||
            (!(v.known&UWVM_LS_CHILDREN_KNOWN) && (v.child_count||v.child_first)) ||
            (container && (v.known&UWVM_LS_DATA_KNOWN) &&
                (v.child_count>v.payload || ((v.known&UWVM_LS_CHILDREN_KNOWN) &&
                 !(v.flags&UWVM_LS_VALUE_TRUNCATED) && v.child_count!=v.payload))) ||
            (v.kind==UWVM_LS_BOOL && v.payload>1) || (v.kind==UWVM_LS_NONE && v.payload) ||
            (v.kind==UWVM_LS_UNKNOWN && (v.known&~UWVM_LS_NAME_KNOWN))) return status::malformed;
        if (blob) {
            std::uint64_t copied_count=v.bytes_length;
            if (v.kind==UWVM_LS_UTF8 &&
                !utf8_count(text.subspan(v.bytes_offset,v.bytes_length),copied_count)) return status::malformed;
            if ((v.known&UWVM_LS_DATA_KNOWN) &&
                (copied_count>v.payload || (!(v.flags&UWVM_LS_VALUE_TRUNCATED) && copied_count!=v.payload)))
                return status::malformed;
        }
    }
    for (std::uint32_t n{}; n!=h.value_count; ++n) {
        auto const& v=candidate.values[n];
        if (v.kind!=UWVM_LS_DICT) continue;
        for (std::uint32_t j{}; j!=v.child_count; ++j) {
            auto const& pair=candidate.values[v.child_first+j];
            if (pair.flags&UWVM_LS_UNAVAILABLE) continue;
            if (pair.kind!=UWVM_LS_TUPLE || !(pair.known&UWVM_LS_DATA_KNOWN) || pair.payload!=2 ||
                pair.child_count>2 || (pair.child_count!=2 && !(pair.flags&UWVM_LS_VALUE_TRUNCATED)) ||
                (!(pair.known&UWVM_LS_CHILDREN_KNOWN) && !(pair.flags&UWVM_LS_VALUE_TRUNCATED))) return status::malformed;
        }
    }
    std::array<bool,512> visited{}; std::uint32_t seen{};
    auto visit=[&](auto&& self,std::uint32_t at,std::uint32_t parent,std::uint32_t frame,unsigned depth) noexcept -> bool {
        if (at>=h.value_count || visited[at] || depth>4) return false;
        auto const& v=candidate.values[at];
        if (v.frame_id!=frame || v.parent_value!=parent ||
            ((v.flags&UWVM_LS_ARGUMENT) && parent!=UINT32_MAX)) return false;
        visited[at]=true; ++seen;
        if (v.child_count && v.child_first<=at) return false;
        for (std::uint32_t i{}; i!=v.child_count; ++i) if (!self(self,v.child_first+i,at,frame,depth+1)) return false;
        return true;
    };
    for (std::uint32_t n{}; n!=h.frame_count; ++n) {
        auto const& f=candidate.frames[n]; std::uint32_t arguments{};
        for (std::uint32_t j{}; j!=f.value_count; ++j) {
            auto at=f.value_first+j;
            if (!visit(visit,at,UINT32_MAX,f.id,0)) return status::malformed;
            arguments+=!!(candidate.values[at].flags&UWVM_LS_ARGUMENT);
        }
        if ((f.known&UWVM_LS_ARGUMENTS_KNOWN) && arguments!=f.argument_count) return status::malformed;
    }
    if (seen!=h.value_count || !zero(bytes,128+std::size_t(h.frame_count)*96,1664) ||
        !zero(bytes,1664+std::size_t(h.value_count)*64,34432) ||
        !zero(bytes,34432+h.text_bytes,UWVM_LS_TOTAL_BYTES)) return status::malformed;
    try { candidate.text.assign(text.begin(),text.end()); out=std::move(candidate); return status::ok; }
    catch (...) { out={}; return status::allocation_failure; }
}
}
/* Finish reading into a separate owner before changing out, including when a
 * trusted Host caller supplies out.text as input. No input alias survives. */
inline status parse(std::span<std::byte const> bytes, snapshot& out) noexcept {
    snapshot candidate{};
    auto result=detail::parse_unaliased(bytes,candidate);
    if (result==status::ok) out=std::move(candidate);
    else out={};
    return result;
}
}
