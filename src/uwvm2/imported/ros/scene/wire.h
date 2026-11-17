/** @file
 * @brief Allocation-free canonical records for purpose-bound scene providers.
 * @details These codecs only move validated bounded bytes; they grant no
 * authority and never retain guest addresses. The carrier owns the complete
 * request before parsing and validates the complete result span before effects.
 * Scalars are little endian, bool/presence are u64 0/1, and text/bytes have a
 * u64 byte length. Text must be canonical UTF-8. A failed operation latches the
 * codec invalid and never modifies its output argument or writes partial data.
 * Schema validation separately bounds nesting, array elements and total nodes.
 * Semantic checks (identity, profile digest, physical units and finite numeric
 * constraints) remain mandatory in the selected provider.
 */
#pragma once
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <span>
#include <string_view>

namespace uwvm2::imported::ros::scene::wire
{
    using bytes=std::span<std::uint8_t const>;
    /** @brief Reject overlong UTF-8, bad continuations, surrogates and >U+10FFFF.
     * @details NUL and Unicode noncharacters are valid encoded text; providers
     * must additionally enforce their name/schema rules before accepting them.
     */
    inline bool utf8(bytes input) noexcept
    {
        for(std::size_t i{};i<input.size();)
        {
            auto first=input[i++];if(first<0x80)continue;
            unsigned count{};std::uint32_t value{},minimum{};
            if(first>=0xc2&&first<=0xdf){count=1;value=first&0x1fu;minimum=0x80;}
            else if(first>=0xe0&&first<=0xef){count=2;value=first&0x0fu;minimum=0x800;}
            else if(first>=0xf0&&first<=0xf4){count=3;value=first&7u;minimum=0x10000;}
            else return false;
            if(count>input.size()-i)return false;
            for(unsigned n{};n<count;++n)
            {auto next=input[i++];if((next&0xc0u)!=0x80u)return false;value=(value<<6)|(next&0x3fu);}
            if(value<minimum||value>0x10ffffu||(value>=0xd800u&&value<=0xdfffu))return false;
        }
        return true;
    }
    /** @brief Borrow a caller-owned immutable request for one synchronous decode. */
    class reader
    {
        bytes input;std::size_t offset{};bool valid{true};
        bool scalar(unsigned width,std::uint64_t& output) noexcept
        {
            if(!valid||width>input.size()-offset){valid=false;return false;}
            std::uint64_t value{};
            for(unsigned n{};n<width;++n)value|=std::uint64_t(input[offset+n])<<(8*n);
            offset+=width;output=value;return true;
        }
    public:
        explicit reader(bytes value) noexcept:input(value){}
        bool good() const noexcept{return valid;}
        bool done() const noexcept{return valid&&offset==input.size();}
        std::size_t position() const noexcept{return offset;}
        std::size_t remaining() const noexcept{return input.size()-offset;}
        bool u64(std::uint64_t& output) noexcept{return scalar(8,output);}
        bool u32(std::uint32_t& output) noexcept
        {std::uint64_t value{};if(!scalar(4,value))return false;output=static_cast<std::uint32_t>(value);return true;}
        bool i64(std::int64_t& output) noexcept
        {std::uint64_t value{};if(!u64(value))return false;output=std::bit_cast<std::int64_t>(value);return true;}
        bool f64(double& output) noexcept
        {std::uint64_t value{};if(!u64(value))return false;output=std::bit_cast<double>(value);return true;}
        bool boolean(bool& output) noexcept
        {std::uint64_t value{};if(!u64(value)||value>1){valid=false;return false;}output=value!=0;return true;}
        /** @brief Read one bounded length-prefixed byte view without allocation. */
        bool blob(bytes& output) noexcept
        {
            std::uint64_t size{};if(!u64(size)||size>remaining()){valid=false;return false;}
            auto result=input.subspan(offset,static_cast<std::size_t>(size));offset+=static_cast<std::size_t>(size);
            output=result;return true;
        }
        bool text(std::string_view& output) noexcept
        {
            bytes value;if(!blob(value)||!utf8(value)){valid=false;return false;}
            output=value.empty()?std::string_view{}:std::string_view{reinterpret_cast<char const*>(value.data()),value.size()};return true;
        }
        /** @brief Mark a provider-specific shape rejection without reading more. */
        bool reject() noexcept{valid=false;return false;}
    };
    /** @brief Write canonical bytes or measure an exactly bounded result.
     * @details Measuring performs the same overflow, text and extent checks,
     * with no storage and no effects. A provider must reserve the measured
     * result capacity before any externally visible action. A writer retains
     * no callback and cannot publish bytes by itself.
     */
    class writer
    {
        std::span<std::uint8_t> output;std::size_t offset{},maximum{};bool valid{true},count_only{};
        bool room(std::size_t amount) noexcept
        {if(!valid||amount>maximum-offset){valid=false;return false;}return true;}
        bool scalar(std::uint64_t value,unsigned width) noexcept
        {
            if(!room(width))return false;
            if(!count_only)for(unsigned n{};n<width;++n)output[offset+n]=static_cast<std::uint8_t>(value>>(n*8));
            offset+=width;return true;
        }
        writer(std::size_t bound,bool measure) noexcept:maximum(bound),count_only(measure){}
    public:
        explicit writer(std::span<std::uint8_t> value) noexcept:output(value),maximum(value.size()){}
        static writer measuring(std::size_t maximum) noexcept{return writer{maximum,true};}
        bool good() const noexcept{return valid;}
        std::size_t size() const noexcept{return offset;}
        std::size_t remaining() const noexcept{return maximum-offset;}
        bytes written() const noexcept{return count_only?bytes{}:bytes{output.data(),offset};}
        bool u64(std::uint64_t value) noexcept{return scalar(value,8);}
        bool u32(std::uint32_t value) noexcept{return scalar(value,4);}
        bool i64(std::int64_t value) noexcept{return u64(std::bit_cast<std::uint64_t>(value));}
        bool f64(double value) noexcept{return u64(std::bit_cast<std::uint64_t>(value));}
        bool boolean(bool value) noexcept{return u64(value?1:0);}
        bool blob(bytes value) noexcept
        {
            if(value.size()>std::numeric_limits<std::size_t>::max()-8){valid=false;return false;}
            if(!room(value.size()+8))return false;
            // Move payload first so a caller's in-place prefix insertion cannot
            // overwrite source bytes before they have been copied.
            if(!count_only&&!value.empty())std::memmove(output.data()+offset+8,value.data(),value.size());
            if(!u64(static_cast<std::uint64_t>(value.size())))return false;
            offset+=value.size();return true;
        }
        bool text(std::string_view value) noexcept
        {
            bytes data{reinterpret_cast<std::uint8_t const*>(value.data()),value.size()};
            if(!utf8(data)){valid=false;return false;}return blob(data);
        }
        /** @brief Embed one already validated canonical field without a prefix.
         * @details This is a record composition primitive, not a validator.
         * The caller must own and completely validate the encoded field under
         * its selected schema before composing it. Measuring counts precisely
         * the same extent; insufficient capacity latches failure without any
         * write. Overlapping owned input/output is supported by memmove.
         */
        bool encoded(bytes value) noexcept
        {
            if(!room(value.size()))return false;
            if(!count_only&&!value.empty())std::memmove(output.data()+offset,value.data(),value.size());
            offset+=value.size();return true;
        }
    };
}
