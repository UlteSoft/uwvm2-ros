/** @file
 * @brief Allocation-free MCAP segments for explicitly admitted ROS recordings.
 * @details This is a container codec, not a recording provider or authority
 * issuer. The caller owns immutable schema/channel definitions, a finite byte
 * arena and a finite message index. No file, socket, clock, plugin loader or
 * native allocator is used here. The provider must independently admit source
 * observation/export and destination storage, authenticate whole-object SHA256,
 * and account for the actual arena/index/retained segments before calling it.
 *
 * The supported closed format is uncompressed, non-chunked MCAP with exact
 * deployment schemas/channels and no auxiliary attachments or summary. ROS1
 * and ROS2 use their public encoding names. Unknown records, metadata keys,
 * extension tails, compression and nonzero CRC fields fail closed; supporting
 * them requires a separately bounded codec. This intentionally does not claim
 * acceptance of every MCAP variant. Complete segments interoperate with native
 * readers. CRC is omitted; the outer immutable object's SHA256 is mandatory.
 *
 * Single-owner writers reserve their final trailer before every append, so
 * capacity exhaustion cannot make an accepted prefix impossible to finish.
 * A failed append leaves the written extent unchanged. Views and indexes are
 * valid only while their original admitted immutable bytes remain retained.
 * Reading/seek does not publish a message or move a simulation clock.
 */
#pragma once
#include "wire.h"
#include <algorithm>
#include <array>

namespace uwvm2::imported::ros::scene::recording::mcap
{
    using bytes=wire::bytes;
    enum class result:std::uint8_t {success,invalid,unsupported,too_large,conflict};
    inline constexpr std::array<std::uint8_t,8> magic{137,77,67,65,80,48,13,10};
    inline constexpr std::size_t trailer_size=13+29+8,maximum_channels=64;
    struct channel
    {
        std::uint16_t id{},schema{};
        std::string_view topic,type,schema_encoding,message_encoding,qos;
        bytes definition;
        /** @brief Trusted bounded payload validator; never supplied by guest data. */
        bool (*validate)(bytes) noexcept{};
        std::uint64_t maximum_message{};
    };
    /** @brief Finite record scan and message capacity; zero permits no records. */
    struct limits{std::uint64_t bytes{},records{},messages{};};
    struct message
    {std::uint16_t channel{};std::uint32_t sequence{};std::uint64_t log_time{},publish_time{};bytes data;};
    struct position
    {std::uint64_t offset{},size{},log_time{},publish_time{};std::uint32_t sequence{};std::uint16_t channel{};};
    inline bytes raw(std::string_view value) noexcept
    {return {reinterpret_cast<std::uint8_t const*>(value.data()),value.size()};}
    inline bool same(bytes a,bytes b) noexcept{return std::ranges::equal(a,b);}
    inline bool text(std::string_view value,std::size_t maximum,bool empty=false) noexcept
    {return (empty||!value.empty())&&value.size()<=maximum&&value.find('\0')==std::string_view::npos&&wire::utf8(raw(value));}
    /** @brief Validate immutable ROS profile definitions, not message authority. */
    inline bool valid(std::string_view profile,std::span<channel const> channels) noexcept
    {
        if((profile!="ros1"&&profile!="ros2")||channels.empty()||channels.size()>maximum_channels)return false;
        for(std::size_t i{};i<channels.size();++i)
        {
            auto const& c=channels[i];
            if(!c.id||!c.schema||!text(c.topic,255)||!text(c.type,255)||!c.validate||!c.maximum_message||c.maximum_message>4u*1024u*1024u||
               c.definition.empty()||c.definition.size()>1024u*1024u||!wire::utf8(c.definition)||!text(c.qos,16384,profile=="ros1"))return false;
            if(profile=="ros1"?(c.schema_encoding!="ros1msg"||c.message_encoding!="ros1"||!c.qos.empty()):
               ((c.schema_encoding!="ros2msg"&&c.schema_encoding!="ros2idl")||c.message_encoding!="cdr"))return false;
            for(std::size_t j{};j<i;++j)
            {
                auto const& other=channels[j];if(c.id==other.id)return false;
                if(c.schema==other.schema&&(c.type!=other.type||c.schema_encoding!=other.schema_encoding||!same(c.definition,other.definition)))return false;
            }
        }
        return true;
    }
    namespace details
    {
        class writer
        {
            std::span<std::uint8_t> data;std::size_t used{},bound{};bool measuring{},good_{true};
        public:
            explicit writer(std::span<std::uint8_t> output):data(output),bound(output.size()){}
            explicit writer(std::size_t maximum):bound(maximum),measuring(true){}
            bool good() const noexcept{return good_;}std::size_t size()const noexcept{return used;}
            bool scalar(std::uint64_t value,unsigned count) noexcept
            {
                if(!good_||count>bound-used){good_=false;return false;}
                if(!measuring)for(unsigned i{};i<count;++i)data[used+i]=static_cast<std::uint8_t>(value>>(8*i));
                used+=count;return true;
            }
            bool block(bytes value) noexcept
            {
                if(!good_||value.size()>bound-used){good_=false;return false;}
                if(!measuring&&!value.empty())std::memmove(data.data()+used,value.data(),value.size());
                used+=value.size();return true;
            }
            bool blob(bytes value) noexcept{return value.size()<=UINT32_MAX&&scalar(value.size(),4)&&block(value);}
            bool string(std::string_view value) noexcept{return blob(raw(value));}
            template<typename Body> bool record(std::uint8_t code,Body body) noexcept
            {
                writer measured{bound-used};if(!body(measured)||!measured.good()||!scalar(code,1)||!scalar(measured.size(),8))return false;
                return body(*this);
            }
        };
        class reader
        {
            bytes data;std::size_t used{};
        public:
            explicit reader(bytes input):data(input){}
            std::size_t size()const noexcept{return used;}bool done()const noexcept{return used==data.size();}
            bool scalar(std::uint64_t& output,unsigned count) noexcept
            {
                if(count>data.size()-used)return false;
                std::uint64_t value{};for(unsigned i{};i<count;++i)value|=std::uint64_t{data[used+i]}<<(8*i);
                used+=count;output=value;return true;
            }
            bool block(std::uint64_t count,bytes& output) noexcept
            {if(count>data.size()-used)return false;output=data.subspan(used,static_cast<std::size_t>(count));used+=static_cast<std::size_t>(count);return true;}
            bool blob(bytes& output) noexcept{std::uint64_t count{};return scalar(count,4)&&block(count,output);}
            bool string(std::string_view& output) noexcept
            {bytes value;if(!blob(value)||!wire::utf8(value))return false;output=value.empty()?std::string_view{}:std::string_view{reinterpret_cast<char const*>(value.data()),value.size()};return true;}
        };
        inline std::size_t find(std::span<channel const> channels,std::uint64_t id) noexcept
        {for(std::size_t i{};i<channels.size();++i)if(channels[i].id==id)return i;return channels.size();}
        inline bool prefix(writer& output,std::string_view profile,std::span<channel const> channels) noexcept
        {
            if(!output.block(magic)||!output.record(1,[&](writer& w){return w.string(profile)&&w.string("uwvm2-ros");}))return false;
            for(auto const& c:channels)
            {
                if(!output.record(3,[&](writer& w){return w.scalar(c.schema,2)&&w.string(c.type)&&w.string(c.schema_encoding)&&w.blob(c.definition);}))return false;
                if(!output.record(4,[&](writer& w)
                {
                    if(!w.scalar(c.id,2)||!w.scalar(c.schema,2)||!w.string(c.topic)||!w.string(c.message_encoding))return false;
                    if(profile=="ros1")return w.scalar(0,4);
                    return w.scalar(8+20+c.qos.size(),4)&&w.string("offered_qos_profiles")&&w.string(c.qos);
                }))return false;
            }
            return true;
        }
    }
    /** @brief Single-owner bounded segment assembly; references must remain immutable. */
    class segment
    {
        std::span<std::uint8_t> output;std::span<channel const> channels;
        std::size_t used{};std::uint64_t messages{},maximum_messages{};bool opened{},finished{};
    public:
        result open(std::span<std::uint8_t> arena,std::string_view profile,std::span<channel const> selected,std::uint64_t limit) noexcept
        {
            if(opened)return result::conflict;
            if(!valid(profile,selected)||arena.size()<trailer_size||limit==UINT64_MAX)return result::invalid;
            details::writer measured{arena.size()-trailer_size};if(!details::prefix(measured,profile,selected))return result::too_large;
            details::writer writer{arena};if(!details::prefix(writer,profile,selected)||writer.size()!=measured.size())return result::invalid;
            output=arena;channels=selected;used=writer.size();maximum_messages=limit;opened=true;return result::success;
        }
        /** @brief Append one complete validated message, preserving trailer capacity.
         * @details Input must not alias the output arena or mutable definitions.
         * The provider owns input bytes and handles request replay separately.
         * Timestamp/sequence are preserved; the codec never invents freshness.
         */
        result append(message const& input) noexcept
        {return append(std::span<message const>{&input,1});}
        /** @brief Append a whole bounded batch or leave the previous extent unchanged.
         * @details Validate every payload and reserve the combined encoded size
         * before writing any record. Validators are trusted pure predicates over
         * immutable inputs; they must not reenter or mutate this segment.
         */
        result append(std::span<message const> batch) noexcept
        {
            if(!opened||finished)return result::conflict;
            if(batch.empty())return result::invalid;
            if(batch.size()>maximum_messages-messages)return result::too_large;
            auto remaining=output.size()-used-trailer_size;
            for(auto const& input:batch)
            {
                auto index=details::find(channels,input.channel);if(index==channels.size())return result::invalid;
                auto const& c=channels[index];if(input.data.size()>c.maximum_message)return result::too_large;
                if(!input.data.empty())
                {
                    auto from=reinterpret_cast<std::uintptr_t>(input.data.data()),to=reinterpret_cast<std::uintptr_t>(output.data());
                    if(from>=to?from-to<output.size():to-from<input.data.size())return result::invalid;
                }
                if(remaining<31||input.data.size()>remaining-31)return result::too_large;
                remaining-=31+input.data.size();
                if(!c.validate(input.data))return result::invalid;
            }
            details::writer writer{output.subspan(used)};
            for(auto const& input:batch)
                if(!writer.record(5,[&](details::writer& w){return w.scalar(input.channel,2)&&w.scalar(input.sequence,4)&&w.scalar(input.log_time,8)&&w.scalar(input.publish_time,8)&&w.block(input.data);}))return result::invalid;
            used+=writer.size();messages+=batch.size();return result::success;
        }
        /** @brief Seal the reserved trailer; this is not durable storage publication. */
        result finish(bytes& complete) noexcept
        {
            if(!opened)return result::conflict;
            if(!finished)
            {
                details::writer writer{output.subspan(used)};
                if(!writer.record(15,[](details::writer& w){return w.scalar(0,4);})||
                   !writer.record(2,[](details::writer& w){return w.scalar(0,8)&&w.scalar(0,8)&&w.scalar(0,4);})||!writer.block(magic))return result::invalid;
                used+=writer.size();finished=true;
            }
            complete=output.first(used);return result::success;
        }
        std::size_t size()const noexcept{return used;}
        std::uint64_t count()const noexcept{return messages;}
    };
    /** @brief Validate/index a whole admitted immutable segment without allocation.
     * @details On failure count stays unchanged; scratch index entries are not
     * published and must be ignored. Record, byte and message bounds stop hostile
     * repeated/oversized fields. Exact definitions constrain both schema IDs and
     * channel metadata. No native parser/codec is selected by container strings.
     */
    inline result index(bytes input,std::string_view profile,std::span<channel const> channels,limits bound,
                        std::span<position> output,std::size_t& count) noexcept
    {
        if(!valid(profile,channels)||bound.bytes==UINT64_MAX||bound.records==UINT64_MAX||bound.messages==UINT64_MAX)return result::invalid;
        if(input.size()>bound.bytes)return result::too_large;
        if(input.size()<16||!same(input.first(8),magic)||!same(input.last(8),magic))return result::invalid;
        details::reader scan{input.subspan(8,input.size()-16)};std::uint64_t records{},schema_seen{},channel_seen{};std::size_t messages{};bool header{},ended{},footer{};
        while(!scan.done())
        {
            if(records==bound.records)return result::too_large;
            ++records;auto record_offset=8+scan.size();std::uint64_t opcode{},extent{};bytes payload;
            if(!scan.scalar(opcode,1)||!scan.scalar(extent,8)||!scan.block(extent,payload)||footer)return result::invalid;
            details::reader fields{payload};
            if(!header)
            {
                std::string_view p,library;if(opcode!=1||!fields.string(p)||p!=profile||!fields.string(library)||!text(library,1024,true)||!fields.done())return result::invalid;
                header=true;continue;
            }
            if(ended&&opcode!=2)return result::unsupported;
            switch(opcode)
            {
            case 3:
            {
                std::uint64_t id{};std::string_view name,encoding;bytes definition;
                if(!fields.scalar(id,2)||!id||!fields.string(name)||!fields.string(encoding)||!fields.blob(definition)||!fields.done())return result::invalid;
                bool found{};for(std::size_t i{};i<channels.size();++i)if(channels[i].schema==id)
                {auto const& c=channels[i];if(name!=c.type||encoding!=c.schema_encoding||!same(definition,c.definition))return result::invalid;schema_seen|=std::uint64_t{1}<<i;found=true;}
                if(!found)return result::invalid;
                break;
            }
            case 4:
            {
                std::uint64_t id{},schema{};std::string_view topic,encoding;bytes metadata;
                if(!fields.scalar(id,2)||!fields.scalar(schema,2)||!fields.string(topic)||!fields.string(encoding)||!fields.blob(metadata)||!fields.done())return result::invalid;
                auto i=details::find(channels,id);if(i==channels.size()||!(schema_seen&(std::uint64_t{1}<<i)))return result::invalid;
                auto const& c=channels[i];if(schema!=c.schema||topic!=c.topic||encoding!=c.message_encoding)return result::invalid;
                if(profile=="ros1"){if(!metadata.empty())return result::unsupported;}
                else {details::reader meta{metadata};std::string_view key,qos;if(!meta.string(key)||key!="offered_qos_profiles"||!meta.string(qos)||qos!=c.qos||!meta.done())return result::invalid;}
                channel_seen|=std::uint64_t{1}<<i;break;
            }
            case 5:
            {
                if(messages==bound.messages||messages==output.size())return result::too_large;
                std::uint64_t id{},sequence{},log{},publish{};
                if(!fields.scalar(id,2)||!fields.scalar(sequence,4)||!fields.scalar(log,8)||!fields.scalar(publish,8))return result::invalid;
                auto i=details::find(channels,id);if(i==channels.size()||!(channel_seen&(std::uint64_t{1}<<i)))return result::invalid;
                bytes data=payload.subspan(fields.size());if(data.size()>channels[i].maximum_message)return result::too_large;
                if(!channels[i].validate(data))return result::invalid;
                output[messages++]={record_offset+31,data.size(),log,publish,static_cast<std::uint32_t>(sequence),static_cast<std::uint16_t>(id)};break;
            }
            case 15:
            {std::uint64_t crc{};if(!fields.scalar(crc,4)||!fields.done())return result::invalid;if(crc)return result::unsupported;ended=true;break;}
            case 2:
            {std::uint64_t summary{},offset{},crc{};if(!ended||!fields.scalar(summary,8)||!fields.scalar(offset,8)||!fields.scalar(crc,4)||!fields.done())return result::invalid;if(summary||offset||crc)return result::unsupported;footer=true;break;}
            default:return result::unsupported;
            }
        }
        if(!header||!ended||!footer)return result::invalid;
        count=messages;return result::success;
    }
}
