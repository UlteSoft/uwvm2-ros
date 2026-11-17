/** @file
 * @brief Frozen, bounded ROS message descriptions and allocation-free wire validation.
 * @details A description grants no communication, storage or device authority.
 * Trusted deployment pins its complete bytes and selects explicit message/work
 * bounds before any endpoint is exposed. Nodes are topologically ordered: every
 * member refers to an earlier node. Cycles, forward references, duplicate field
 * names, unsupported kinds and implicit sequence/string bounds are rejected.
 *
 * Supported layouts are packed little-endian ROS1 messages and final/plain
 * XCDR1 with a four-byte encapsulation header (either byte order). This is not
 * a TCPROS packet, DDS/RTPS packet, XCDR2 parser or a native object layout.
 * Parameter lists, optional/mutable IDL members and nonzero encapsulation
 * options are refused. Padding is not interpreted as data. Empty ROS2 records
 * use the generated one-byte dummy member; ROS1 empty records use no bytes.
 * Strings have explicit byte/code-unit limits. Narrow CDR strings require one
 * final NUL and no interior NUL. UTF-8 checking and finite-float checking are
 * explicit per-node restrictions; unselected float restrictions preserve all
 * IEEE bit patterns, including sensor NaNs, without touching the host FP state.
 *
 * Parsing allocates only through recoverable fast_io-backed containers and
 * commits atomically. Validation and conversion allocate nothing, use at most
 * 64 traversal frames and retain no input pointer. Every possible traversal is
 * bounded at preparation time, including sequences of empty records. Conversion
 * preflights the complete input and exact output extent before writing; input
 * must remain immutable for the call and overlap is refused. Conversion never
 * invents ROS1/ROS2 Header fields, clock semantics, type hashes or authority.
 */
#pragma once
#include "wire.h"
#include <uwvm2/utils/container/standard.h>
#include <algorithm>
#include <array>
#include <bit>
#include <limits>
#include <stdexcept>

namespace uwvm2::imported::ros::scene::bounded_message
{
    enum class status : std::uint32_t {ok=0,no_memory=12,denied=13,invalid=22,conflict=17,too_large=90,unsupported=95};
    enum class layout : std::uint8_t {ros1,cdr};
    enum class kind : std::uint8_t
    {boolean,int8,uint8,int16,uint16,int32,uint32,int64,uint64,float32,float64,string,wide_string,record,array,sequence};
    inline constexpr std::array<std::string_view,16> kind_names{
        "bool","int8","uint8","int16","uint16","int32","uint32","int64","uint64","float32","float64",
        "string","wstring","record","array","sequence"};
    inline constexpr std::string_view magic="uwvm.ros.bounded-message";
    inline constexpr std::uint64_t finite=1,unicode=2,no_nul=4;
    struct limits {std::uint64_t message_bytes{},work{};std::uint32_t depth{};};
    struct member {::uwvm2::utils::container::std_string name;std::uint32_t type{};};
    struct node
    {
        kind type{};std::uint64_t bound{},flags{};
        ::uwvm2::utils::container::std_vector<member> members;
    };
    /** @brief ASCII schema identifiers, independent of resource names and paths. */
    inline bool field_name(std::string_view name)noexcept
    {
        if(name.empty()||name.size()>255)return false;
        auto alpha=[](unsigned char c){return (c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_';};
        if(!alpha(name.front()))return false;
        for(unsigned char c:name)if(!alpha(c)&&(c<'0'||c>'9'))return false;
        return true;
    }
    namespace details
    {
        constexpr unsigned width(kind value)noexcept
        {
            switch(value)
            {
            case kind::boolean:case kind::int8:case kind::uint8:return 1;
            case kind::int16:case kind::uint16:return 2;
            case kind::int32:case kind::uint32:case kind::float32:return 4;
            case kind::int64:case kind::uint64:case kind::float64:return 8;
            default:return 0;
            }
        }
        struct reader
        {
            wire::bytes bytes;std::size_t offset{};layout format;bool little{true};
            bool align(unsigned width)noexcept
            {auto count=format==layout::cdr?(-offset)&(width-1u):0;return skip(count);}
            bool skip(std::size_t count)noexcept
            {if(count>bytes.size()-offset)return false;offset+=count;return true;}
            bool number(unsigned width,std::uint64_t& output)noexcept
            {
                if(!align(width)||width>bytes.size()-offset)return false;
                std::uint64_t value{};for(unsigned i{};i<width;++i)value|=std::uint64_t(bytes[offset+i])<<(8*(little?i:width-1-i));
                offset+=width;output=value;return true;
            }
            bool take(std::size_t count,wire::bytes& output)noexcept
            {if(count>bytes.size()-offset)return false;output=bytes.subspan(offset,count);offset+=count;return true;}
        };
        struct writer
        {
            std::span<std::uint8_t> bytes;std::size_t offset{},maximum{};layout format;bool little{true},measuring{true};
            bool skip(std::size_t count)noexcept
            {
                if(count>maximum-offset)return false;
                if(!measuring)std::fill_n(bytes.data()+offset,count,std::uint8_t{});
                offset+=count;return true;
            }
            bool number(unsigned width,std::uint64_t value)noexcept
            {
                if(!skip(format==layout::cdr?(-offset)&(width-1u):0)||width>maximum-offset)return false;
                if(!measuring)for(unsigned i{};i<width;++i)bytes[offset+i]=static_cast<std::uint8_t>(value>>(8*(little?i:width-1-i)));
                offset+=width;return true;
            }
            bool copy(wire::bytes input)noexcept
            {
                if(input.size()>maximum-offset)return false;
                if(!measuring&&!input.empty())std::memcpy(bytes.data()+offset,input.data(),input.size());
                offset+=input.size();return true;
            }
        };
        inline bool utf16(wire::bytes bytes,bool little,bool without_nul)noexcept
        {
            auto code=[&](std::size_t i){return static_cast<std::uint16_t>(bytes[i+(little?0:1)]|(std::uint16_t(bytes[i+(little?1:0)])<<8));};
            for(std::size_t i{};i<bytes.size();i+=2)
            {
                auto value=code(i);if(without_nul&&!value)return false;
                if(value>=0xd800&&value<=0xdbff)
                {if(bytes.size()-i<4)return false;auto next=code(i+2);if(next<0xdc00||next>0xdfff)return false;i+=2;}
                else if(value>=0xdc00&&value<=0xdfff)return false;
            }
            return true;
        }
        /** @brief Scan complete scalar extents using integer bits without touching FP state.
         * @details The caller proves a whole number of elements and exact input
         * bounds first. memcpy permits unaligned input on every supported ABI;
         * masks follow wire byte order. Reduction permits compiler SIMD without
         * speculative out-of-range loads, NaN arithmetic or architecture-specific
         * intrinsics. A signaling NaN is data and cannot raise an FP exception.
         */
        template<typename UInt>inline bool finite_array(wire::bytes bytes,bool little)noexcept
        {
            constexpr UInt exponent=sizeof(UInt)==4?UInt{0x7f800000u}:static_cast<UInt>(UINT64_C(0x7ff0000000000000));
            UInt invalid{};
            if(little==(std::endian::native==std::endian::little))
            {
                // After clearing sign/fraction, adding one exponent unit sets
                // the sign bit exactly for the all-ones exponent. Integer
                // add/OR reduction avoids a synthesized 64-bit SIMD comparison
                // on baseline SSE2, with the same scalar semantics on ARM.
                constexpr UInt unit=sizeof(UInt)==4?UInt{0x00800000u}:static_cast<UInt>(UINT64_C(0x0010000000000000));
                constexpr UInt sign=UInt{1}<<(sizeof(UInt)*8-1);
                for(std::size_t offset{};offset<bytes.size();offset+=sizeof(UInt))
                {UInt value;std::memcpy(&value,bytes.data()+offset,sizeof(value));invalid|=(value&exponent)+unit;}
                return !(invalid&sign);
            }
            auto mask=std::byteswap(exponent);
            for(std::size_t offset{};offset<bytes.size();offset+=sizeof(UInt))
            {UInt value;std::memcpy(&value,bytes.data()+offset,sizeof(value));invalid|=static_cast<UInt>((value&mask)==mask);}
            return !invalid;
        }
        inline bool boolean_array(wire::bytes bytes)noexcept
        {std::uint8_t bits{};for(auto value:bytes)bits|=value;return !(bits&0xfeu);}
    }
    class schema
    {
        ::uwvm2::utils::container::std_vector<node> nodes_;
        ::uwvm2::utils::container::std_vector<std::uint8_t> description_;
        limits bounds_{};layout format_{};bool ready_{},wide_{};
        /** @brief Bounded iterative interpreter; the output cursor is either measuring or private.
         * @details verified is private to conversion's second pass, following
         * complete preflight of immutable input. It skips only repeated scalar
         * array value scans; extents and output capacity remain checked. There
         * is no public unchecked-validation or unchecked-conversion interface.
         */
        bool visit(details::reader& input,details::writer* output,bool verified=false)const noexcept
        {
            struct frame {std::uint32_t node;std::uint64_t remaining,index;bool entered;};
            // Every pushed frame is fully assigned before it becomes reachable.
            // Avoid clearing all 64 frames for each small message/two-pass copy.
            std::array<frame,64> stack;std::size_t depth=1;
            stack[0]={static_cast<std::uint32_t>(nodes_.size()-1),0,0,false};
            while(depth)
            {
                auto& current=stack[depth-1];auto const& value=nodes_[current.node];
                if(!current.entered)
                {
                    current.entered=true;
                    if(auto width=details::width(value.type))
                    {
                        std::uint64_t bits{};if(!input.number(width,bits))return false;
                        if(value.type==kind::boolean&&bits>1)return false;
                        if(value.flags&finite)
                        {auto mask=value.type==kind::float32?UINT64_C(0x7f800000):UINT64_C(0x7ff0000000000000);if((bits&mask)==mask)return false;}
                        if(output&&!output->number(width,bits))return false;
                    }
                    else if(value.type==kind::string||value.type==kind::wide_string)
                    {
                        bool wide=value.type==kind::wide_string;std::uint64_t count{};
                        if(wide&&(input.format==layout::ros1||(output&&output->format==layout::ros1)))return false;
                        if(!input.number(4,count))return false;
                        bool terminator=!wide&&input.format==layout::cdr;
                        if(terminator){if(!count)return false;--count;}
                        if(count>value.bound||count>(input.bytes.size()-input.offset)/(wide?2u:1u))return false;
                        wire::bytes text;if(!input.take(static_cast<std::size_t>(count)*(wide?2u:1u),text))return false;
                        if(terminator){std::uint64_t nul{};if(!input.number(1,nul)||nul)return false;}
                        if(wide)
                        {if((value.flags&unicode)&&!details::utf16(text,input.little,value.flags&no_nul))return false;
                         if(!(value.flags&unicode)&&(value.flags&no_nul))for(std::size_t i{};i<text.size();i+=2)if(!text[i]&&!text[i+1])return false;}
                        else if(((value.flags&unicode)&&!wire::utf8(text))||
                                ((terminator||(value.flags&no_nul))&&std::find(text.begin(),text.end(),0)!=text.end()))return false;
                        if(output)
                        {
                            bool emit_nul=!wide&&output->format==layout::cdr;
                            if(emit_nul&&std::find(text.begin(),text.end(),0)!=text.end())return false;
                            if(!output->number(4,count+(emit_nul?1u:0u)))return false;
                            if(wide&&input.little!=output->little)
                            {for(std::size_t i{};i<text.size();i+=2){auto bits=std::uint64_t(text[i+(input.little?0:1)])|(std::uint64_t(text[i+(input.little?1:0)])<<8);if(!output->number(2,bits))return false;}}
                            else if(!output->copy(text))return false;
                            if(emit_nul&&!output->number(1,0))return false;
                        }
                    }
                    else if(value.type==kind::record)
                    {
                        current.remaining=value.members.size();
                        if(!current.remaining)
                        {
                            std::uint64_t dummy{};if(input.format==layout::cdr&&(!input.number(1,dummy)||dummy))return false;
                            if(output&&output->format==layout::cdr&&!output->number(1,0))return false;
                        }
                    }
                    else
                    {
                        std::uint64_t count=value.bound;
                        if(value.type==kind::sequence)
                        {if(!input.number(4,count)||count>value.bound||(output&&!output->number(4,count)))return false;}
                        current.remaining=count;
                        // Primitive collections are contiguous after one alignment.
                        // Value restrictions use bounded integer scans, then the
                        // writer copies the complete range without per-element frames.
                        auto const& child=nodes_[value.members[0].type];auto width=details::width(child.type);
                        if(count&&width&&(!output||input.little==output->little||width==1))
                        {
                            if(!input.align(width)||count>(input.bytes.size()-input.offset)/width)return false;
                            wire::bytes bytes;if(!input.take(static_cast<std::size_t>(count)*width,bytes))return false;
                            if(!verified)
                            {
                                if(child.type==kind::boolean&&!details::boolean_array(bytes))return false;
                                if((child.flags&finite)&&!(width==4?details::finite_array<std::uint32_t>(bytes,input.little):
                                                                 details::finite_array<std::uint64_t>(bytes,input.little)))return false;
                            }
                            if(output&&(!output->skip(output->format==layout::cdr?(-output->offset)&(width-1u):0)||!output->copy(bytes)))return false;
                            current.remaining=0;
                        }
                    }
                }
                if(!current.remaining){--depth;continue;}
                if(depth==stack.size())return false;
                auto child=value.members[value.type==kind::record?static_cast<std::size_t>(current.index++):0].type;
                --current.remaining;stack[depth++]={child,0,0,false};
            }
            return input.offset==input.bytes.size();
        }
        bool input(wire::bytes bytes,layout format,details::reader& result)const noexcept
        {
            if(!ready_||bytes.size()>bounds_.message_bytes||format>layout::cdr||(wide_&&format==layout::ros1))return false;
            result={bytes,0,format,true};
            if(format==layout::cdr)
            {if(bytes.size()<4||bytes[0]||bytes[1]>1||bytes[2]||bytes[3])return false;result.bytes=bytes.subspan(4);result.little=bytes[1]==1;}
            return true;
        }
    public:
        /** @brief Parse and freeze a complete canonical description with no partial publication.
         * @details Carrier: text magic, text layout (ros1 or cdr), u64 byte/work/
         * depth bounds, u64 node count, then nodes {text kind,u64 bound,u64 flags,
         * u64 member count,{text name,u64 prior-node-index}...}. The last node is
         * the root record. Array/sequence members have the single name "element".
         * Limits are at most 16 MiB/message, 64 Mi work units, 64 levels, 4096
         * nodes and 65536 member edges; descriptor bytes are at most 4 MiB.
         * All nodes must be reachable. Bounds are deployment restrictions, not
         * upstream type declarations. Zero-length strings/sequences are legal.
         */
        status parse(wire::bytes bytes)noexcept
        {
            if(ready_)return status::conflict;
            if(bytes.size()>4u*1024u*1024u)return status::too_large;
            try
            {
                wire::reader in{bytes};std::string_view marker,format;std::uint64_t depth{},count{};schema candidate;
                if(!in.text(marker)||marker!=magic||!in.text(format)||(format!="ros1"&&format!="cdr")||
                    !in.u64(candidate.bounds_.message_bytes)||!in.u64(candidate.bounds_.work)||!in.u64(depth)||!in.u64(count))return status::invalid;
                if(!candidate.bounds_.message_bytes||candidate.bounds_.message_bytes>16u*1024u*1024u||!candidate.bounds_.work||
                    candidate.bounds_.work>64u*1024u*1024u||!depth||depth>64||!count||count>4096)return status::too_large;
                candidate.bounds_.depth=static_cast<std::uint32_t>(depth);candidate.format_=format=="ros1"?layout::ros1:layout::cdr;
                candidate.nodes_.reserve(static_cast<std::size_t>(count));
                ::uwvm2::utils::container::std_vector<std::uint64_t> work;work.reserve(static_cast<std::size_t>(count));
                ::uwvm2::utils::container::std_vector<std::uint32_t> depths;depths.reserve(static_cast<std::size_t>(count));
                std::uint64_t edges{};
                for(std::uint64_t index{};index<count;++index)
                {
                    node value;std::string_view name;std::uint64_t members{};
                    if(!in.text(name))return status::invalid;
                    auto found=std::find(kind_names.begin(),kind_names.end(),name);if(found==kind_names.end())return status::unsupported;
                    value.type=static_cast<kind>(found-kind_names.begin());
                    if(!in.u64(value.bound)||!in.u64(value.flags)||!in.u64(members))return status::invalid;
                    if(members>65536-edges)return status::too_large;
                    edges+=members;
                    auto width=details::width(value.type);bool text=value.type==kind::string||value.type==kind::wide_string;
                    if((width&&(value.bound||members||(value.flags&~((value.type==kind::float32||value.type==kind::float64)?finite:0))))||
                       (text&&(members||(value.flags&~(unicode|no_nul))||value.bound>UINT32_MAX-1u))||
                       (value.type==kind::record&&(value.bound||value.flags))||
                       ((value.type==kind::array||value.type==kind::sequence)&&(members!=1||value.flags||value.bound>UINT32_MAX||
                           (value.type==kind::array&&!value.bound))))return status::invalid;
                    if(value.type==kind::wide_string){if(candidate.format_==layout::ros1)return status::unsupported;candidate.wide_=true;}
                    value.members.reserve(static_cast<std::size_t>(members));
                    ::uwvm2::utils::container::std_vector<std::string_view> names;names.reserve(static_cast<std::size_t>(members));
                    std::uint64_t cost=1;std::uint32_t level=1;
                    for(std::uint64_t i{};i<members;++i)
                    {
                        std::uint64_t child{};if(!in.text(name)||!field_name(name)||!in.u64(child)||child>=index)return status::invalid;
                        names.push_back(name);
                        if(value.type!=kind::record&&name!="element")return status::invalid;
                        auto multiplier=value.type==kind::record?1:value.bound;
                        if(multiplier&&(work[child]>(candidate.bounds_.work-cost)/multiplier))return status::too_large;
                        cost+=work[child]*multiplier;level=std::max(level,depths[child]+1);
                        value.members.push_back({::uwvm2::utils::container::std_string(name),static_cast<std::uint32_t>(child)});
                    }
                    std::sort(names.begin(),names.end());if(std::adjacent_find(names.begin(),names.end())!=names.end())return status::invalid;
                    if(text)
                    {auto multiplier=value.type==kind::wide_string?2u:1u;if(value.bound>(candidate.bounds_.work-cost)/multiplier)return status::too_large;cost+=value.bound*multiplier;}
                    if(cost>candidate.bounds_.work||level>depth)return status::too_large;
                    work.push_back(cost);depths.push_back(level);candidate.nodes_.push_back(std::move(value));
                }
                if(!in.done()||candidate.nodes_.back().type!=kind::record)return status::invalid;
                ::uwvm2::utils::container::std_vector<std::uint8_t> reached(static_cast<std::size_t>(count));reached.back()=1;
                for(std::size_t i=reached.size();i--;)if(reached[i])for(auto const& field:candidate.nodes_[i].members)reached[field.type]=1;
                if(std::find(reached.begin(),reached.end(),0)!=reached.end())return status::invalid;
                candidate.description_.assign(bytes.begin(),bytes.end());candidate.ready_=true;*this=std::move(candidate);return status::ok;
            }
            catch(std::bad_alloc const&){return status::no_memory;}catch(std::length_error const&){return status::too_large;}
        }
        bool ready()const noexcept{return ready_;}
        layout encoding()const noexcept{return format_;}
        limits bounds()const noexcept{return bounds_;}
        wire::bytes description()const noexcept{return description_;}
        /** @brief Account logical retained storage in the deployment definition budget.
         * @details Includes reserved container capacities and owned names, not
         * allocator metadata. Transient preparation storage remains bounded by
         * the independent node/edge/description caps. Shared plans may charge
         * this conservatively more than once; copies do not duplicate the graph.
         */
        std::uint64_t storage_bytes()const noexcept
        {
            if(!ready_)return 0;
            std::uint64_t total=sizeof(schema)+nodes_.capacity()*sizeof(node)+description_.capacity();
            for(auto const& value:nodes_)
            {total+=value.members.capacity()*sizeof(member);for(auto const& field:value.members)total+=field.name.capacity()+1;}
            return total;
        }
        /** @brief Validate exactly one complete message under the selected frozen layout. */
        bool validate(wire::bytes bytes)const noexcept
        {details::reader reader{{},0,format_};return input(bytes,format_,reader)&&visit(reader,nullptr);}
        /** @brief Convert the same structural schema without native objects or implicit type mapping.
         * @details Output byte order applies only to CDR; ROS1 is always little
         * endian. `required` is changed only after a successful complete
         * preflight, including the output message budget. Insufficient output
         * returns too_large with exact required bytes, leaving storage intact.
         * A successful call writes only that prefix. Any overlapping input and
         * output spans are invalid, including same-layout conversion. Foreign
         * schemas, protocol options and partial messages are never forwarded.
         */
        status convert(wire::bytes bytes,layout target,bool little,std::span<std::uint8_t> output,std::size_t& required)const noexcept
        {
            if(!ready_)return status::denied;
            if(target>layout::cdr||(wide_&&target==layout::ros1))return status::unsupported;
            auto a=reinterpret_cast<std::uintptr_t>(bytes.data()),b=reinterpret_cast<std::uintptr_t>(output.data());
            if(!bytes.empty()&&!output.empty()&&(a<=b?b-a<bytes.size():a-b<output.size()))return status::invalid;
            details::reader reader{{},0,format_};if(!input(bytes,format_,reader))return status::invalid;
            auto header=target==layout::cdr?4u:0u;if(bounds_.message_bytes<header)return status::too_large;
            details::writer measure{{},0,static_cast<std::size_t>(bounds_.message_bytes)-header,target,target==layout::ros1||little,true};
            if(!visit(reader,&measure))return status::invalid;
            auto size=measure.offset+header;required=size;if(size>output.size())return status::too_large;
            input(bytes,format_,reader);
            details::writer writer{output.subspan(header,size-header),0,size-header,target,target==layout::ros1||little,false};
            if(header){output[0]=0;output[1]=little?1:0;output[2]=output[3]=0;}
            return visit(reader,&writer,true)&&writer.offset==size-header?status::ok:status::invalid;
        }
    };
}
