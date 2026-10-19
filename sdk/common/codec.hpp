/* Canonical ABI v1 values; these helpers require C++20 and no native ROS headers. */
#pragma once
#include <bit>
#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>
#include <limits>
#include <cmath>
#include <type_traits>

namespace uwvm_ros {
/** @brief Owning canonical wire bytes; unrelated to native ROS serialization. */
using bytes = std::vector<unsigned char>;
/** @brief Append four little-endian bytes to the owning buffer.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline void append_u32(bytes& out, std::uint32_t value) {
    for(unsigned i=0;i!=4;++i) out.push_back(static_cast<unsigned char>(value>>(i*8)));
}
/** @brief Append eight little-endian bytes to the owning buffer.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline void append_u64(bytes& out, std::uint64_t value) {
    for(unsigned i=0;i!=8;++i) out.push_back(static_cast<unsigned char>(value>>(i*8)));
}
/** @brief Append u32 length and raw String bytes; an excessive u32 length is refused before mutation.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool append_string(bytes& out, std::string_view value) {
    if(value.size()>std::numeric_limits<std::uint32_t>::max()) return false;
    append_u32(out,static_cast<std::uint32_t>(value.size())); out.insert(out.end(),value.begin(),value.end()); return true;
}
/** @brief Pack a u32-length String without UTF-8 admission checks; scalar String wire can include NUL.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes string(std::string_view value) { bytes out; if(!append_string(out,value)) out.clear(); return out; }
/** @brief Encode exactly one canonical Bool byte (0 or1).
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes boolean(bool value) { return {static_cast<unsigned char>(value)}; }
/** @brief Encode one canonical Bool parameter byte.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes parameter_bool(bool value) { return boolean(value); }
/** @brief Encode four two's-complement LE bytes for a ROS1 scalar parameter.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes parameter_int32(std::int32_t value) { bytes out;append_u32(out,std::bit_cast<std::uint32_t>(value));return out; }
/** @brief Encode eight two's-complement LE bytes for a ROS2 scalar parameter.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes parameter_int64(std::int64_t value) { bytes out;append_u64(out,std::bit_cast<std::uint64_t>(value));return out; }
/** @brief Encode finite IEEE754 binary64; NaN/infinity return an empty buffer.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes parameter_float64(double value) {
    static_assert(sizeof(double)==8&&std::numeric_limits<double>::is_iec559);
    if(!std::isfinite(value)) return {};bytes out;append_u64(out,std::bit_cast<std::uint64_t>(value));return out;
}
/** @brief Pack linear XYZ/angular XYZ double bits; low-level packing does not enforce finite/bound rules.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes twist(std::array<double,6> const& value) {
    bytes out; out.reserve(48); for(auto field:value) append_u64(out,std::bit_cast<std::uint64_t>(field)); return out;
}
/** @brief Reader canonical value; see sdk/docs/c-cpp.md for fields and lifetime rules. */
class reader {
    std::span<unsigned char const> value_; std::size_t position_{};
public:
    /** @brief Borrow the exact supplied wire range; the backing buffer must outlive this reader. */
    explicit reader(std::span<unsigned char const> value) : value_(value) {}
    /** @brief Read a bounded LE field of width0..8; caller fixes its ABI width. */
    bool integer(std::uint64_t& value,unsigned width) {
        if(width>8 || width>value_.size()-position_) return false;
        value=0; for(unsigned i=0;i!=width;++i) value|=std::uint64_t(value_[position_++])<<(8*i); return true;
    }
    /** @brief Read one four-byte LE field. */
    bool u32(std::uint32_t& value) { std::uint64_t n{}; if(!integer(n,4)) return false; value=static_cast<std::uint32_t>(n); return true; }
    /** @brief Read an owning u32-length String bounded by maximum; this is structural decoding. */
    bool text(std::string& value,std::size_t maximum=std::numeric_limits<std::uint32_t>::max()) {
        std::uint32_t n{}; if(!u32(n) || n>maximum || n>value_.size()-position_) return false;
        value.assign(reinterpret_cast<char const*>(value_.data()+position_),n); position_+=n; return true;
    }
    /** @brief Read binary64 bit representation; this reader does not enforce finite values. */
    bool real(double& value) { std::uint64_t n{}; if(!integer(n,8)) return false; value=std::bit_cast<double>(n); return true; }
    /** @brief Read a bounded u32-length owning byte sequence. */
    bool blob(bytes& value,std::size_t maximum=std::numeric_limits<std::uint32_t>::max()) {
        std::uint32_t n{}; if(!u32(n) || n>maximum || n>value_.size()-position_) return false;
        value.assign(value_.begin()+position_,value_.begin()+position_+n);position_+=n;return true;
    }
    /** @brief Read a bounded borrowed byte view; do not retain it after the source buffer expires. */
    bool blob_view(std::span<unsigned char const>& value,std::size_t maximum=std::numeric_limits<std::uint32_t>::max()) {
        std::uint32_t n{};if(!u32(n)||n>maximum||n>value_.size()-position_) return false;
        value=value_.subspan(position_,n);position_+=n;return true;
    }
    /** @brief Return whether the entire supplied wire range was consumed. */
    bool done() const noexcept { return position_==value_.size(); }
};
/** @brief Decode an exact u32-length String structurally; UTF-8 admission remains a separate check.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_string(std::span<unsigned char const> value,std::string& out) {
    reader in{value}; std::string next; if(!in.text(next)||!in.done()) return false; out=std::move(next); return true;
}
/** @brief Decode exactly one canonical Bool byte and preserve the destination on failure.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_boolean(std::span<unsigned char const> value,bool& out) {
    if(value.size()!=1||value[0]>1) return false; out=value[0]!=0;return true;
}
/** @brief Decode exact bool fields; refusal preserves the supplied destination.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_parameter_bool(std::span<unsigned char const> value,bool& out) { return decode_boolean(value,out); }
/** @brief Decode exact int32 fields; refusal preserves the supplied destination.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_parameter_int32(std::span<unsigned char const> value,std::int32_t& out) {
    if(value.size()!=4) return false;reader in{value};std::uint32_t bits{};if(!in.u32(bits)||!in.done()) return false;
    out=std::bit_cast<std::int32_t>(bits);return true;
}
/** @brief Decode exact int64 fields; refusal preserves the supplied destination.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_parameter_int64(std::span<unsigned char const> value,std::int64_t& out) {
    if(value.size()!=8) return false;reader in{value};std::uint64_t bits{};if(!in.integer(bits,8)||!in.done()) return false;
    out=std::bit_cast<std::int64_t>(bits);return true;
}
/** @brief Decode one finite binary64 value transactionally.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_parameter_float64(std::span<unsigned char const> value,double& out) {
    if(value.size()!=8) return false;reader in{value};double next{};if(!in.real(next)||!in.done()||!std::isfinite(next)) return false;
    out=next;return true;
}
namespace parameter_array_detail {
/** @brief Canonical bound: bounded canonical support without resource authority.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline constexpr std::size_t maximum_elements=1048576,maximum_bytes=4193786;
/** @brief Text valid: bounded canonical support without resource authority.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool text_valid(std::string_view value,bool allow_nul=false) {
    std::size_t position{};
    while(position<value.size()) {
        auto lead=static_cast<unsigned char>(value[position++]);
        if(lead<128) { if(!lead&&!allow_nul) return false;continue; }
        unsigned continuation{};std::uint32_t codepoint{},minimum{};
        if(lead>=0xc2&&lead<=0xdf) { continuation=1;codepoint=lead&31;minimum=128; }
        else if(lead>=0xe0&&lead<=0xef) { continuation=2;codepoint=lead&15;minimum=2048; }
        else if(lead>=0xf0&&lead<=0xf4) { continuation=3;codepoint=lead&7;minimum=65536; }
        else return false;
        if(continuation>value.size()-position) return false;
        while(continuation--) { auto byte=static_cast<unsigned char>(value[position++]);if((byte&0xc0)!=0x80) return false;codepoint=(codepoint<<6)|(byte&63); }
        if(codepoint<minimum||codepoint>0x10ffff||(codepoint>=0xd800&&codepoint<=0xdfff)) return false;
    }
    return true;
}
/** @brief Integer: bounded canonical support without resource authority.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline std::uint64_t integer(std::span<unsigned char const> value,std::size_t position,unsigned width) {
    std::uint64_t result{};for(unsigned i=0;i!=width;++i) result|=std::uint64_t(value[position+i])<<(8*i);return result;
}
template<class T,unsigned Width,bool Boolean=false>
/** @brief Encode fixed: bounded canonical support without resource authority.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool encode_fixed(std::span<T const> values,bytes& out) {
    if(values.size()>maximum_elements||values.size()>(maximum_bytes-4)/Width) return false;
    for(auto value:values) {
        if constexpr(Boolean) { if(value>1) return false; }
        if constexpr(std::is_same_v<T,double>) { if(!std::isfinite(value)) return false; }
    }
    bytes next;next.reserve(4+values.size()*Width);append_u32(next,static_cast<std::uint32_t>(values.size()));
    for(auto value:values) {
        if constexpr(Width==1) next.push_back(value);
        else if constexpr(Width==4) append_u32(next,std::bit_cast<std::uint32_t>(value));
        else append_u64(next,std::bit_cast<std::uint64_t>(value));
    }
    out=std::move(next);return true;
}
template<class T,unsigned Width,bool Boolean=false>
/** @brief Decode fixed: bounded canonical support without resource authority.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_fixed(std::span<unsigned char const> value,std::vector<T>& out) {
    if(value.size()<4||value.size()>maximum_bytes) return false;
    auto count=integer(value,0,4);
    if(count>(value.size()-4)/Width||count*Width!=value.size()-4) return false;
    for(std::size_t position=4;position<value.size();position+=Width) {
        if constexpr(Boolean) { if(value[position]>1) return false; }
        if constexpr(std::is_same_v<T,double>) { if(!std::isfinite(std::bit_cast<double>(integer(value,position,8)))) return false; }
    }
    if(count>maximum_elements) return false;
    std::vector<T> next;next.reserve(static_cast<std::size_t>(count));
    for(std::size_t position=4;position<value.size();position+=Width) {
        if constexpr(Width==1) next.push_back(value[position]);
        else if constexpr(Width==4) next.push_back(std::bit_cast<T>(static_cast<std::uint32_t>(integer(value,position,4))));
        else next.push_back(std::bit_cast<T>(integer(value,position,8)));
    }
    out=std::move(next);return true;
}
}
/** @brief Validate and encode bool array; refusal preserves the supplied owning output.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool encode_parameter_bool_array(std::span<std::uint8_t const> value,bytes& out) { return parameter_array_detail::encode_fixed<std::uint8_t,1,true>(value,out); }
/** @brief Decode exact bool array fields; refusal preserves the supplied destination.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_parameter_bool_array(std::span<unsigned char const> value,std::vector<std::uint8_t>& out) { return parameter_array_detail::decode_fixed<std::uint8_t,1,true>(value,out); }
/** @brief Validate and encode byte array; refusal preserves the supplied owning output.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool encode_parameter_byte_array(std::span<std::uint8_t const> value,bytes& out) { return parameter_array_detail::encode_fixed<std::uint8_t,1>(value,out); }
/** @brief Decode exact byte array fields; refusal preserves the supplied destination.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_parameter_byte_array(std::span<unsigned char const> value,std::vector<std::uint8_t>& out) { return parameter_array_detail::decode_fixed<std::uint8_t,1>(value,out); }
/** @brief Validate and encode int32 array; refusal preserves the supplied owning output.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool encode_parameter_int32_array(std::span<std::int32_t const> value,bytes& out) { return parameter_array_detail::encode_fixed<std::int32_t,4>(value,out); }
/** @brief Decode exact int32 array fields; refusal preserves the supplied destination.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_parameter_int32_array(std::span<unsigned char const> value,std::vector<std::int32_t>& out) { return parameter_array_detail::decode_fixed<std::int32_t,4>(value,out); }
/** @brief Validate and encode int64 array; refusal preserves the supplied owning output.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool encode_parameter_int64_array(std::span<std::int64_t const> value,bytes& out) { return parameter_array_detail::encode_fixed<std::int64_t,8>(value,out); }
/** @brief Decode exact int64 array fields; refusal preserves the supplied destination.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_parameter_int64_array(std::span<unsigned char const> value,std::vector<std::int64_t>& out) { return parameter_array_detail::decode_fixed<std::int64_t,8>(value,out); }
/** @brief Validate and encode float64 array; refusal preserves the supplied owning output.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool encode_parameter_float64_array(std::span<double const> value,bytes& out) { return parameter_array_detail::encode_fixed<double,8>(value,out); }
/** @brief Decode exact float64 array fields; refusal preserves the supplied destination.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_parameter_float64_array(std::span<unsigned char const> value,std::vector<double>& out) { return parameter_array_detail::decode_fixed<double,8>(value,out); }
/** @brief Validate and encode string array; refusal preserves the supplied owning output.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool encode_parameter_string_array(std::span<std::string const> values,bytes& out) {
    if(values.size()>parameter_array_detail::maximum_elements) return false;
    std::size_t size=4;
    for(auto const& value:values) {
        if(parameter_array_detail::maximum_bytes-size<4||value.size()>parameter_array_detail::maximum_bytes-size-4||!parameter_array_detail::text_valid(value)) return false;
        size+=4+value.size();
    }
    bytes next;next.reserve(size);append_u32(next,static_cast<std::uint32_t>(values.size()));
    for(auto const& value:values) append_string(next,value);
    out=std::move(next);return true;
}
/** @brief Decode exact string array fields; refusal preserves the supplied destination.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_parameter_string_array(std::span<unsigned char const> value,std::vector<std::string>& out) {
    if(value.size()<4||value.size()>parameter_array_detail::maximum_bytes) return false;
    auto count=parameter_array_detail::integer(value,0,4);if(count>(value.size()-4)/4) return false;
    std::size_t position=4;
    for(std::uint64_t i=0;i!=count;++i) {
        if(value.size()-position<4) return false;
        auto length=parameter_array_detail::integer(value,position,4);position+=4;
        if(length>value.size()-position||!parameter_array_detail::text_valid({reinterpret_cast<char const*>(value.data()+position),static_cast<std::size_t>(length)})) return false;
        position+=static_cast<std::size_t>(length);
    }
    if(position!=value.size()||count>parameter_array_detail::maximum_elements) return false;
    std::vector<std::string> next;next.reserve(static_cast<std::size_t>(count));position=4;
    for(std::uint64_t i=0;i!=count;++i) {
        auto length=parameter_array_detail::integer(value,position,4);position+=4;
        next.emplace_back(reinterpret_cast<char const*>(value.data()+position),static_cast<std::size_t>(length));position+=static_cast<std::size_t>(length);
    }
    out=std::move(next);return true;
}
/** @brief Encode bool array with canonical count prefix for arrays.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes parameter_bool_array(std::span<std::uint8_t const> value) { bytes out;encode_parameter_bool_array(value,out);return out; }
/** @brief Encode byte array with canonical count prefix for arrays.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes parameter_byte_array(std::span<std::uint8_t const> value) { bytes out;encode_parameter_byte_array(value,out);return out; }
/** @brief Encode int32 array with canonical count prefix for arrays.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes parameter_int32_array(std::span<std::int32_t const> value) { bytes out;encode_parameter_int32_array(value,out);return out; }
/** @brief Encode int64 array with canonical count prefix for arrays.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes parameter_int64_array(std::span<std::int64_t const> value) { bytes out;encode_parameter_int64_array(value,out);return out; }
/** @brief Encode float64 array with canonical count prefix for arrays.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes parameter_float64_array(std::span<double const> value) { bytes out;encode_parameter_float64_array(value,out);return out; }
/** @brief Encode string array with canonical count prefix for arrays.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes parameter_string_array(std::span<std::string const> value) { bytes out;encode_parameter_string_array(value,out);return out; }
/** @brief Decode six exact double fields transactionally; this low-level helper does not reject NaN/infinity.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_twist(std::span<unsigned char const> value,std::array<double,6>& out) {
    reader in{value}; std::array<double,6> next{}; for(auto& field:next) if(!in.real(field)) return false;
    if(!in.done()) return false;out=next;return true;
}
/** @brief Image canonical value; see sdk/docs/c-cpp.md for fields and lifetime rules. */
struct image {
    std::uint32_t sequence{},seconds{},nanoseconds{},height{},width{},step{};
    std::string frame_id,encoding; bool bigendian{}; bytes data;
};
/** @brief Pack complete Image fields; native stride/encoding/timestamp admission remains a host check.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes encode(image const& value) {
    bytes out; append_u32(out,value.sequence);append_u32(out,value.seconds);append_u32(out,value.nanoseconds);
    if(!append_string(out,value.frame_id)) return {};
    append_u32(out,value.height);append_u32(out,value.width);if(!append_string(out,value.encoding)) return {};
    out.push_back(static_cast<unsigned char>(value.bigendian));append_u32(out,value.step);
    if(value.data.size()>std::numeric_limits<std::uint32_t>::max()) return {};
    append_u32(out,static_cast<std::uint32_t>(value.data.size()));out.insert(out.end(),value.data.begin(),value.data.end());return out;
}
/** @brief Decode the complete image wire transactionally, rejecting truncation/trailing bytes.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode(std::span<unsigned char const> value,image& out) {
    reader in{value};image next;std::uint64_t boolean{};
    if(!in.u32(next.sequence)||!in.u32(next.seconds)||!in.u32(next.nanoseconds)||!in.text(next.frame_id)||!in.u32(next.height)||!in.u32(next.width)||!in.text(next.encoding)||!in.integer(boolean,1)||boolean>1||!in.u32(next.step)||!in.blob(next.data)||!in.done()) return false;
    next.bigendian=boolean!=0;out=std::move(next);return true;
}
/** @brief Transform canonical value; see sdk/docs/c-cpp.md for fields and lifetime rules. */
struct transform {
    std::uint32_t sequence{},seconds{},nanoseconds{};
    std::string parent_frame,child_frame; std::array<double,3> translation{};std::array<double,4> quaternion{{0,0,0,1}};
};
/** @brief Pack complete TransformStamped fields; low-level packing does not enforce finite/frame/bound rules.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes encode(transform const& value) {
    bytes out;append_u32(out,value.sequence);append_u32(out,value.seconds);append_u32(out,value.nanoseconds);
    if(!append_string(out,value.parent_frame)||!append_string(out,value.child_frame)) return {};
    for(auto field:value.translation) append_u64(out,std::bit_cast<std::uint64_t>(field));
    for(auto field:value.quaternion) append_u64(out,std::bit_cast<std::uint64_t>(field));return out;
}
/** @brief Decode the complete transform wire transactionally, rejecting truncation/trailing bytes.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode(std::span<unsigned char const> value,transform& out) {
    reader in{value};transform next;
    if(!in.u32(next.sequence)||!in.u32(next.seconds)||!in.u32(next.nanoseconds)||!in.text(next.parent_frame)||!in.text(next.child_frame)) return false;
    for(auto& field:next.translation) if(!in.real(field)) return false;
    for(auto& field:next.quaternion) if(!in.real(field)) return false;
    if(!in.done()) return false;out=std::move(next);return true;
}
/** @brief Encode u64 nanoseconds for TF lookup; zero selects latest under a separately granted alias.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes lookup_time(std::uint64_t nanoseconds=0) { bytes out;append_u64(out,nanoseconds);return out; }
/** @brief Pack signed i32 order bits; allowed order/quota remains a host-policy check.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes fibonacci_goal(std::int32_t order) { bytes out;append_u32(out,std::bit_cast<std::uint32_t>(order));return out; }
/** @brief Pack a u64 goal ID; nonzero/ownership rules depend on the operation.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes goal_id(std::uint64_t goal) { bytes out;append_u64(out,goal);return out; }
/** @brief Action sequence canonical value; see sdk/docs/c-cpp.md for fields and lifetime rules. */
struct action_sequence { std::uint64_t goal{};unsigned char kind{};std::vector<std::int32_t> values; };
/** @brief Pack an ID/count/i32 sequence with at most48 elements and optional kind byte.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes encode(action_sequence const& value,bool include_kind=true) {
    if(value.values.size()>48) return {};
    auto out=goal_id(value.goal);if(include_kind) out.push_back(value.kind);
    append_u32(out,static_cast<std::uint32_t>(value.values.size()));
    for(auto number:value.values) append_u32(out,std::bit_cast<std::uint32_t>(number));return out;
}
/** @brief Decode the complete action sequence wire transactionally, rejecting truncation/trailing bytes.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode(std::span<unsigned char const> value,action_sequence& out,bool has_kind=true) {
    reader in{value};action_sequence next;std::uint64_t kind{};std::uint32_t count{};
    if(!in.integer(next.goal,8)||!next.goal||(has_kind&&(!in.integer(kind,1)||kind<1||kind>4))||!in.u32(count)||count>48) return false;
    next.kind=static_cast<unsigned char>(kind);
    for(std::uint32_t i=0;i!=count;++i) { std::uint32_t bits{};if(!in.u32(bits)) return false;next.values.push_back(std::bit_cast<std::int32_t>(bits)); }
    if(!in.done()) return false;out=std::move(next);return true;
}
/** @brief Parameter event canonical value; see sdk/docs/c-cpp.md for fields and lifetime rules. */
struct parameter_event {
    std::uint32_t seconds{},nanoseconds{};unsigned char kind{};std::string node,name,value;
};
/** @brief Decode the complete parameter event wire transactionally, rejecting truncation/trailing bytes.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode(std::span<unsigned char const> value,parameter_event& out) {
    reader in{value};parameter_event next;std::uint64_t kind{};
    if(!in.u32(next.seconds)||!in.u32(next.nanoseconds)||!in.integer(kind,1)||kind<1||kind>3||!in.text(next.node)||!in.text(next.name)||!in.text(next.value)||!in.done()) return false;
    next.kind=static_cast<unsigned char>(kind);out=std::move(next);return true;
}
/** @brief Parameter event value tag canonical value; see sdk/docs/c-cpp.md for fields and lifetime rules. */
enum class parameter_event_value_tag : unsigned char {
    deletion=0,boolean=1,integer=2,float64=3,string=4,byte_array=5,boolean_array=6,
    integer_array=7,float64_array=8,string_array=9
};
/** @brief Typed parameter event canonical value; see sdk/docs/c-cpp.md for fields and lifetime rules. */
struct typed_parameter_event {
    std::uint32_t seconds{},nanoseconds{};unsigned char kind{};
    std::string node,name;parameter_event_value_tag tag{parameter_event_value_tag::boolean};
    bool bool_value{};std::int64_t integer_value{};double double_value{};
    std::string string_value;
    std::vector<std::uint8_t> byte_array_value,bool_array_value;
    std::vector<std::int64_t> integer_array_value;
    std::vector<double> double_array_value;
    std::vector<std::string> string_array_value;
};
/** @brief Validate supported ROS2 exact node/parameter name syntax.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool valid_parameter_event_name(std::string_view name,bool node) {
    if(name.empty()||name.size()>(node?1024u:255u)||(node&&(name.size()<2||name.front()!='/'))) return false;
    bool first=true;
    for(std::size_t i=node?1u:0u;i<name.size();++i) {
        char c=name[i];if(c==(node?'/':'.')) { if(first) return false;first=true;continue; }
        bool alpha=(c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_';
        if(!alpha&&!(c>='0'&&c<='9'&&!first)) return false;first=false;
    }
    return !first;
}
/** @brief Validate and transactionally encode the complete event-v2 envelope and selected value.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool encode_typed_parameter_event(typed_parameter_event const& value,bytes& out) {
    auto tag=static_cast<unsigned char>(value.tag);
    if(value.seconds>2147483647u||value.nanoseconds>=1000000000u||value.kind<1||value.kind>3||tag>9||(tag==0&&value.kind!=3)||
       !valid_parameter_event_name(value.node,true)||!valid_parameter_event_name(value.name,false)) return false;
    // Account for the complete envelope before allocating the final wire.
    auto available=parameter_array_detail::maximum_bytes-23-value.node.size()-value.name.size();
    std::size_t payload_size{};
    if(value.kind!=3) {
        switch(value.tag) {
        case parameter_event_value_tag::boolean:payload_size=1;break;
        case parameter_event_value_tag::integer:payload_size=8;break;
        case parameter_event_value_tag::float64:payload_size=8;if(!std::isfinite(value.double_value)) return false;break;
        case parameter_event_value_tag::string:
            if(available<4||value.string_value.size()>available-4||!parameter_array_detail::text_valid(value.string_value,true)) return false;
            payload_size=4+value.string_value.size();break;
        case parameter_event_value_tag::string_array:
            if(value.string_array_value.size()>parameter_array_detail::maximum_elements||available<4) return false;
            payload_size=4;
            for(auto const& text:value.string_array_value) {
                if(available-payload_size<4||text.size()>available-payload_size-4||!parameter_array_detail::text_valid(text)) return false;
                payload_size+=4+text.size();
            }
            break;
        default: {
            auto count=tag==5?value.byte_array_value.size():tag==6?value.bool_array_value.size():tag==7?value.integer_array_value.size():value.double_array_value.size();
            auto width=tag<=6?1u:8u;
            if(available<4||count>parameter_array_detail::maximum_elements||count>(available-4)/width) return false;
            payload_size=4+count*width;
            if(tag==6) for(auto item:value.bool_array_value) { if(item>1) return false; }
            if(tag==8) for(auto item:value.double_array_value) { if(!std::isfinite(item)) return false; }
            break;
        }
        }
        if(payload_size>available) return false;
    }
    bytes next;next.reserve(23+value.node.size()+value.name.size()+payload_size);append_u32(next,value.seconds);append_u32(next,value.nanoseconds);
    next.push_back(value.kind);next.push_back(2);next.push_back(tag);
    if(!append_string(next,value.node)||!append_string(next,value.name)) return false;
    append_u32(next,static_cast<std::uint32_t>(payload_size));
    if(value.kind!=3) {
        // Preflight above fixes the exact size and validates every value. Append
        // directly to the final wire without an owning intermediate payload.
        switch(value.tag) {
        case parameter_event_value_tag::boolean:next.push_back(static_cast<unsigned char>(value.bool_value));break;
        case parameter_event_value_tag::integer:append_u64(next,std::bit_cast<std::uint64_t>(value.integer_value));break;
        case parameter_event_value_tag::float64:append_u64(next,std::bit_cast<std::uint64_t>(value.double_value));break;
        case parameter_event_value_tag::string:append_string(next,value.string_value);break;
        case parameter_event_value_tag::byte_array:
            append_u32(next,static_cast<std::uint32_t>(value.byte_array_value.size()));
            next.insert(next.end(),value.byte_array_value.begin(),value.byte_array_value.end());break;
        case parameter_event_value_tag::boolean_array:
            append_u32(next,static_cast<std::uint32_t>(value.bool_array_value.size()));
            next.insert(next.end(),value.bool_array_value.begin(),value.bool_array_value.end());break;
        case parameter_event_value_tag::integer_array:
            append_u32(next,static_cast<std::uint32_t>(value.integer_array_value.size()));
            for(auto item:value.integer_array_value) append_u64(next,std::bit_cast<std::uint64_t>(item));break;
        case parameter_event_value_tag::float64_array:
            append_u32(next,static_cast<std::uint32_t>(value.double_array_value.size()));
            for(auto item:value.double_array_value) append_u64(next,std::bit_cast<std::uint64_t>(item));break;
        case parameter_event_value_tag::string_array:
            append_u32(next,static_cast<std::uint32_t>(value.string_array_value.size()));
            for(auto const& text:value.string_array_value) append_string(next,text);break;
        case parameter_event_value_tag::deletion:return false;
        }
    }
    out=std::move(next);return true;
}
/** @brief Encode a validated complete event-v2 value; refusal returns an empty buffer.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes encode(typed_parameter_event const& value) {
    bytes out;if(!encode_typed_parameter_event(value,out)) return {};return out;
}
/** @brief Validate and transactionally decode event-v2 timestamps, exact names, tags and selected payload.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode_typed_parameter_event(std::span<unsigned char const> value,typed_parameter_event& out) {
    if(value.size()<23||value.size()>4193786u) return false;
    reader in{value};typed_parameter_event next;std::uint64_t kind{},version{},tag{};std::span<unsigned char const> payload;
    if(!in.u32(next.seconds)||next.seconds>2147483647u||!in.u32(next.nanoseconds)||next.nanoseconds>=1000000000u||
       !in.integer(kind,1)||kind<1||kind>3||!in.integer(version,1)||version!=2||!in.integer(tag,1)||tag>9||(tag==0&&kind!=3)||
       !in.text(next.node,1024)||!in.text(next.name,255)||!in.blob_view(payload,parameter_array_detail::maximum_bytes)||!in.done()||
       !valid_parameter_event_name(next.node,true)||!valid_parameter_event_name(next.name,false)) return false;
    next.kind=static_cast<unsigned char>(kind);next.tag=static_cast<parameter_event_value_tag>(tag);
    if(kind==3) { if(!payload.empty()) return false; }
    else switch(next.tag) {
    case parameter_event_value_tag::boolean:if(!decode_parameter_bool(payload,next.bool_value)) return false;break;
    case parameter_event_value_tag::integer:if(!decode_parameter_int64(payload,next.integer_value)) return false;break;
    case parameter_event_value_tag::float64:if(!decode_parameter_float64(payload,next.double_value)) return false;break;
    case parameter_event_value_tag::string:if(!decode_string(payload,next.string_value)||!parameter_array_detail::text_valid(next.string_value,true)) return false;break;
    case parameter_event_value_tag::byte_array:if(!decode_parameter_byte_array(payload,next.byte_array_value)) return false;break;
    case parameter_event_value_tag::boolean_array:if(!decode_parameter_bool_array(payload,next.bool_array_value)) return false;break;
    case parameter_event_value_tag::integer_array:if(!decode_parameter_int64_array(payload,next.integer_array_value)) return false;break;
    case parameter_event_value_tag::float64_array:if(!decode_parameter_float64_array(payload,next.double_array_value)) return false;break;
    case parameter_event_value_tag::string_array:if(!decode_parameter_string_array(payload,next.string_array_value)) return false;break;
    case parameter_event_value_tag::deletion:return false;
    }
    out=std::move(next);return true;
}
/** @brief Decode the complete typed parameter event wire transactionally, rejecting truncation/trailing bytes.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode(std::span<unsigned char const> value,typed_parameter_event& out) { return decode_typed_parameter_event(value,out); }
/** @brief Service response canonical value; see sdk/docs/c-cpp.md for fields and lifetime rules. */
struct service_response { bool success{};std::string message; };
/** @brief Pack Bool plus response String; host independently validates text and quotas.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes encode(service_response const& value) {
    auto out=boolean(value.success);if(!append_string(out,value.message)) return {};return out;
}
/** @brief Decode the complete service response wire transactionally, rejecting truncation/trailing bytes.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bool decode(std::span<unsigned char const> value,service_response& out) {
    reader in{value};service_response next;std::uint64_t success{};
    if(!in.integer(success,1)||success>1||!in.text(next.message)||!in.done()) return false;
    next.success=success!=0;out=std::move(next);return true;
}
/** @brief Prefix the gateway-owned request ID to the canonical Trigger/SetBool response.
 * ABI v1 fields use LE byte order. Structural helpers do not replace host checks
 * for builtin identity, exact resources, permissions, per-alias limits or native semantics.
 */
inline bytes service_reply(std::uint64_t request_id,service_response const& response) {
    bytes out;append_u64(out,request_id);auto payload=encode(response);out.insert(out.end(),payload.begin(),payload.end());return out;
}
}
