/*************************************************************
 * UWVM2 bounded custom canonical wire schema. APL-2.0.       *
 *************************************************************/
#pragma once
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace uwvm2::imported::ros::ipc
{
    /** A closed generated grammar, never supplied by a Guest or discovered ROS
     * peer. Existing strings are strict UTF-8; explicit raw_bytes preserves every
     * counted byte. Sequences count logical elements. bounded_record adds only
     * an explicit full-value wire cap, never a field projection or grant.
     * Records and fixed arrays have no envelope/padding. Scalar widths/signs
     * are explicit, and every integer/count uses little endian. */
    enum class custom_wire_kind : ::std::uint8_t
    {
        boolean,u8,i8,u16,i16,u32,i32,u64,i64,f32,f64,utf8,record,array,sequence,raw_bytes,bounded_record,
        // Trusted generated field opt-in only. Existing numeric enum values stay fixed.
        f32_positive_infinity,f64_positive_infinity
    };
    struct custom_wire_node
    {
        custom_wire_kind kind{};
        ::std::uint32_t bound{};
        custom_wire_node const* children{};
        ::std::size_t child_count{};
    };
    struct custom_wire_extent
    {
        ::std::uint32_t minimum{},maximum{},cells{};
    };
    inline constexpr unsigned custom_wire_width(custom_wire_kind kind) noexcept
    {
        switch(kind)
        {
            case custom_wire_kind::boolean:case custom_wire_kind::u8:case custom_wire_kind::i8:return 1;
            case custom_wire_kind::u16:case custom_wire_kind::i16:return 2;
            case custom_wire_kind::u32:case custom_wire_kind::i32:case custom_wire_kind::f32:case custom_wire_kind::f32_positive_infinity:return 4;
            case custom_wire_kind::u64:case custom_wire_kind::i64:case custom_wire_kind::f64:case custom_wire_kind::f64_positive_infinity:return 8;
            default:return 0;
        }
    }
    /** Bound complete expanded layouts before using their payload grammar.
     * Existing roots retain their original32KiB cartesian rule. Only a genuine
     * bounded_record activates larger INTERMEDIATE cartesian sequence extents;
     * its complete active wire value must still fit its explicit<=32KiB cap.
     * This does not bound inline C sizeof, allocator RSS or native stack frames.
     * Expanded cells/depth are independent and never clipped by wire quota. */
    class custom_wire_extent_walk final
    {
        // A caller cannot choose the expansion mode. Only encountering an
        // actual bounded_record in this private traversal activates it.
        static constexpr bool visit(custom_wire_node const& node,
            custom_wire_extent& out,unsigned depth,bool bounded_parent) noexcept
    {
        if(depth>16) { return false; }
        auto width{custom_wire_width(node.kind)};
        if(width)
        {
            if(node.bound||node.children||node.child_count) { return false; }
            out={width,width,1};return true;
        }
        if(node.kind==custom_wire_kind::utf8||node.kind==custom_wire_kind::raw_bytes)
        {
            if(!node.bound||node.bound>32764||node.children||node.child_count) { return false; }
            out={4,node.bound+4,1};return true;
        }
        bool const capped{node.kind==custom_wire_kind::bounded_record};
        if(node.kind==custom_wire_kind::record||capped)
        {
            if((capped?(!node.bound||node.bound>32768):node.bound)||node.child_count>4096||
                (node.child_count&&!node.children)) { return false; }
            ::std::uint64_t minimum{},maximum{},cells{1};
            for(::std::size_t i{};i!=node.child_count;++i)
            {
                custom_wire_extent child{};
                if(!visit(node.children[i],child,depth+1,bounded_parent||capped)) { return false; }
                if(child.minimum>UINT64_MAX-minimum||child.maximum>UINT64_MAX-maximum||child.cells>UINT64_MAX-cells) { return false; }
                minimum+=child.minimum;maximum+=child.maximum;cells+=child.cells;
                if(cells>4096||maximum>UINT32_MAX||minimum>UINT32_MAX||
                    (!bounded_parent&&!capped&&maximum>32768)) { return false; }
            }
            if(capped)
            {
                if(minimum>node.bound) { return false; }
                if(maximum>node.bound) { maximum=node.bound; }
            }
            out={static_cast<::std::uint32_t>(minimum),static_cast<::std::uint32_t>(maximum),static_cast<::std::uint32_t>(cells)};
            return true;
        }
        if(node.kind!=custom_wire_kind::array&&node.kind!=custom_wire_kind::sequence) { return false; }
        if(!node.bound||node.bound>4096||node.child_count!=1||!node.children) { return false; }
        custom_wire_extent child{};
        if(!visit(*node.children,child,depth+1,bounded_parent)) { return false; }
        if(child.maximum>UINT64_MAX/node.bound||child.minimum>UINT64_MAX/node.bound||
            child.cells>(UINT64_MAX-1u)/node.bound) { return false; }
        ::std::uint64_t maximum{::std::uint64_t{child.maximum}*node.bound};
        ::std::uint64_t cells{::std::uint64_t{child.cells}*node.bound+1};
        ::std::uint64_t minimum{::std::uint64_t{child.minimum}*node.bound};
        if(node.kind==custom_wire_kind::sequence)
        {
            if(maximum>UINT64_MAX-4u) { return false; }
            maximum+=4;minimum=4;
        }
        if((!bounded_parent&&maximum>32768)||maximum>UINT32_MAX||minimum>UINT32_MAX||cells>4096) { return false; }
        out={static_cast<::std::uint32_t>(minimum),static_cast<::std::uint32_t>(maximum),static_cast<::std::uint32_t>(cells)};
        return true;
    }
    public:
        static constexpr bool run(custom_wire_node const& node,custom_wire_extent& out,unsigned depth) noexcept
        { return visit(node,out,depth,false); }
    };
    inline constexpr bool custom_wire_extent_of(custom_wire_node const& node,
        custom_wire_extent& out,unsigned depth=1) noexcept
    { return custom_wire_extent_walk::run(node,out,depth); }
    /** Read at most eight already checked bytes without alignment/aliasing
     * assumptions. This is canonical decoding, never native ROS deserialization. */
    inline ::std::uint64_t custom_wire_bits(::std::uint8_t const* bytes,unsigned width) noexcept
    {
        ::std::uint64_t value{};
        for(unsigned i{};i!=width;++i) { value|=::std::uint64_t{bytes[i]}<<(8u*i); }
        return value;
    }
    /** Strict Unicode scalar UTF-8. Reject overlong encodings, surrogates,
     * truncated sequences, NUL and values above U+10FFFF on both directions.
     * Rejection of NUL is part of this profile and its canonical identity. */
    inline bool custom_wire_utf8(::std::uint8_t const* bytes,::std::size_t size) noexcept
    {
        if(size&&!bytes) { return false; }
        ::std::size_t at{};
        while(at!=size)
        {
            auto first{bytes[at++]};if(!first) { return false; }
            if(first<0x80) { continue; }
            unsigned count{};::std::uint32_t value{},minimum{};
            if(first>=0xc2&&first<=0xdf) { count=1;value=first&0x1f;minimum=0x80; }
            else if(first>=0xe0&&first<=0xef) { count=2;value=first&0xf;minimum=0x800; }
            else if(first>=0xf0&&first<=0xf4) { count=3;value=first&7;minimum=0x10000; }
            else { return false; }
            if(count>size-at) { return false; }
            while(count--)
            { auto byte{bytes[at++]};if((byte&0xc0)!=0x80) { return false; }value=(value<<6)|(byte&0x3f); }
            if(value<minimum||value>0x10ffff||(value>=0xd800&&value<=0xdfff)) { return false; }
        }
        return true;
    }
    /** Consume a validated generated grammar under an independent work bound.
     * A zero-byte child cannot cause an unbounded loop: sequence counts are
     * schema-bounded and the walk has a 65,536-step ceiling. No allocation,
     * callback, syscall, native type lookup or capability acquisition occurs. */
    inline bool custom_wire_consume(custom_wire_node const& node,::std::uint8_t const* bytes,
        ::std::size_t size,::std::size_t& offset,unsigned& work,unsigned depth=1) noexcept
    {
        if(depth>16||!work||offset>size) { return false; }--work;
        auto width{custom_wire_width(node.kind)};
        if(width)
        {
            if(width>size-offset) { return false; }
            auto bits{custom_wire_bits(bytes+offset,width)};
            if(node.kind==custom_wire_kind::boolean&&bits>1) { return false; }
            if(node.kind==custom_wire_kind::f32&&(bits&0x7f800000u)==0x7f800000u) { return false; }
            if(node.kind==custom_wire_kind::f64&&(bits&0x7ff0000000000000ULL)==0x7ff0000000000000ULL) { return false; }
            if(node.kind==custom_wire_kind::f32_positive_infinity&&(bits&0x7f800000u)==0x7f800000u&&bits!=0x7f800000u) { return false; }
            if(node.kind==custom_wire_kind::f64_positive_infinity&&(bits&0x7ff0000000000000ULL)==0x7ff0000000000000ULL&&bits!=0x7ff0000000000000ULL) { return false; }
            offset+=width;return true;
        }
        if(node.kind==custom_wire_kind::utf8||node.kind==custom_wire_kind::raw_bytes)
        {
            if(size-offset<4) { return false; }
            auto count{custom_wire_bits(bytes+offset,4)};offset+=4;
            if(count>node.bound||count>size-offset||
                (node.kind==custom_wire_kind::utf8&&!custom_wire_utf8(bytes+offset,static_cast<::std::size_t>(count)))) { return false; }
            offset+=static_cast<::std::size_t>(count);return true;
        }
        if(node.kind==custom_wire_kind::record||node.kind==custom_wire_kind::bounded_record)
        {
            auto const start{offset};
            for(::std::size_t i{};i!=node.child_count;++i)
            {
                if(!custom_wire_consume(node.children[i],bytes,size,offset,work,depth+1)) { return false; }
                if(node.kind==custom_wire_kind::bounded_record&&offset-start>node.bound) { return false; }
            }
            return true;
        }
        ::std::uint64_t count{node.bound};
        if(node.kind==custom_wire_kind::sequence)
        {
            if(size-offset<4) { return false; }count=custom_wire_bits(bytes+offset,4);offset+=4;
            if(count>node.bound) { return false; }
        }
        else if(node.kind!=custom_wire_kind::array) { return false; }
        for(::std::uint64_t i{};i!=count;++i)
        { if(!custom_wire_consume(*node.children,bytes,size,offset,work,depth+1)) { return false; } }
        return true;
    }
    inline bool validate_custom_wire(custom_wire_node const& node,::std::uint8_t const* bytes,::std::size_t size) noexcept
    {
        custom_wire_extent extent{};
        if(!custom_wire_extent_of(node,extent)||size<extent.minimum||size>extent.maximum||(size&&!bytes)) { return false; }
        ::std::size_t offset{};unsigned work{65536};
        return custom_wire_consume(node,bytes,size,offset,work)&&offset==size;
    }
}
