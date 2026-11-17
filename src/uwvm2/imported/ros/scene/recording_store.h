/** @file
 * @brief Fixed-reservation recording segments and transactional private replay.
 * @details This is a provider core, not an authority issuer, disk writer or ROS
 * publisher. The dispatcher must retain exact source observation/export and
 * destination storage authority throughout each call. Source identities below
 * are correlation supplied by that trusted dispatcher, never proof by themselves.
 * Configuration owns every schema/name/definition and reserves all segment,
 * index, replay and retry storage with recoverable fast_io allocation. After
 * configuration, every method is allocation-free. Default zero cannot configure.
 *
 * One writer per binding uses strictly increasing request keys and exact last
 * request replay. A failed result copy must retry the same key and full content.
 * Rejection never advances the key. Checkpoints are immutable volatile MCAP
 * objects with SHA256, not fsync or durable commits. All retained segments count
 * against the fixed reservation; capacity is never extended or silently evicted.
 * Explicit erase can recycle only an unpinned sealed slot, never its identity.
 *
 * Replay pins an ordered, explicit object set and maintains a message ordinal,
 * not a clock. Read copies no partial record and advances only after successful
 * delivery and a live-owner recheck. Sinks run outside the mutex. Close/retire
 * during a sink fences final consumption, but cannot recall already seen bytes.
 * Sinks may not retain spans beyond the call. Destruction requires joined calls.
 */
#pragma once
#include "buffers.h"
#include "recording_mcap.h"

namespace uwvm2::imported::ros::scene::recording
{
    using error=resources::error;
    using handle=resources::handle;
    struct owner
    {
        handle environment{},grant{},generation{};
        explicit operator bool() const noexcept{return environment&&grant&&generation;}
        friend bool operator==(owner const&,owner const&)=default;
    };
    inline constexpr std::size_t maximum_batch=64;
    /** @brief Independent finite capacity, never an implicit rate attenuation.
     * @details reserved_bytes includes logical arrays and owned definitions;
     * allocator bookkeeping, mutexes and this fixed object are outside that
     * accounting. They remain inside the deployment's process/cgroup bound.
     */
    struct plan
    {std::uint64_t segments{},segment_bytes{},segment_messages{},replays{},request_bytes{},read_items{},reserved_bytes{};};
    /** @brief Immutable local object identity and exact complete MCAP digest. */
    struct object_info
    {
        handle id{};std::uint64_t bytes{},messages{};buffers::digest sha256{};
        friend bool operator==(object_info const&,object_info const&)=default;
    };
    /** @brief Trusted source correlation; it conveys no source authority. */
    struct item
    {mcap::message message;handle source_capability{},source{},source_generation{};};
    enum class mutation:std::uint8_t{append,checkpoint,stop_drain,stop_abort};
    struct receipt
    {
        std::uint64_t key{},accepted_messages{},accepted_bytes{};mutation operation{};object_info object{};bool stopped{};
        friend bool operator==(receipt const&,receipt const&)=default;
    };
    struct usage
    {std::uint64_t reserved_bytes{},sealed_segments{},sealed_bytes{},replays{},pins{},active_messages{};bool recording{},retired{};};
    /** @brief Alias-private store; owner generation prevents cross-incarnation reuse. */
    class store
    {
        struct slot
        {mcap::segment writer;object_info info{};std::uint64_t pins{};bool occupied{},sealed{};};
        struct playback
        {handle id{};std::uint64_t cursor{},total{},objects{};bool delivering{},closing{};};
        mutable std::mutex mutex;
        owner authority{};plan bounds{};bool configured{},retired{},stopped{};
        handle writer_id{};std::size_t active{SIZE_MAX},last_size{};receipt last{};
        std::size_t channel_count{};buffers::text<4> profile;
        ::uwvm2::utils::container::std_unique_ptr<mcap::channel[]> channels;
        ::uwvm2::utils::container::std_unique_ptr<std::uint8_t[]> definitions,data,requests;
        ::uwvm2::utils::container::std_unique_ptr<slot[]> slots;
        ::uwvm2::utils::container::std_unique_ptr<mcap::position[]> indexes;
        ::uwvm2::utils::container::std_unique_ptr<playback[]> playbacks;
        ::uwvm2::utils::container::std_unique_ptr<std::size_t[]> selections;
        ::uwvm2::utils::container::std_unique_ptr<mcap::message[]> read_records;
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
        error admitted(owner caller) const noexcept
        {return !configured||caller!=authority?error::denied:retired?error::cancelled:error::success;}
        std::span<std::uint8_t> arena(std::size_t index) noexcept
        {return {data.get()+index*static_cast<std::size_t>(bounds.segment_bytes),static_cast<std::size_t>(bounds.segment_bytes)};}
        mcap::position* positions(std::size_t index) noexcept{return indexes.get()+index*static_cast<std::size_t>(bounds.segment_messages);}
        std::size_t* selected(std::size_t index) noexcept{return selections.get()+index*static_cast<std::size_t>(bounds.segments);}
        mcap::message* read_scratch(std::size_t index) noexcept{return read_records.get()+index*static_cast<std::size_t>(bounds.read_items);}
        std::size_t find_object(handle id) const noexcept
        {for(std::size_t i{};i<bounds.segments;++i)if(slots[i].sealed&&slots[i].info.id==id)return i;return SIZE_MAX;}
        std::size_t find_replay(handle id) const noexcept
        {for(std::size_t i{};i<bounds.replays;++i)if(playbacks[i].id==id)return i;return SIZE_MAX;}
        std::size_t available() const noexcept
        {for(std::size_t i{};i<bounds.segments;++i)if(!slots[i].occupied)return i;return SIZE_MAX;}
        error begin_segment(std::size_t index) noexcept
        {
            slots[index]=slot{};
            auto result=slots[index].writer.open(arena(index),profile.view(),{channels.get(),channel_count},bounds.segment_messages);
            if(result!=mcap::result::success)return error::too_large;
            slots[index].occupied=true;return error::success;
        }
        static error translate(mcap::result code) noexcept
        {
            switch(code){case mcap::result::success:return error::success;case mcap::result::too_large:return error::too_large;
            case mcap::result::unsupported:return error::unsupported;case mcap::result::conflict:return error::conflict;default:return error::invalid;}
        }
        /** @brief Encode complete retry identity, including admitted source correlation. */
        bool request(wire::writer& out,mutation operation,std::span<item const> batch) const noexcept
        {
            if(!out.u64(static_cast<unsigned>(operation))||!out.u64(batch.size()))return false;
            for(auto const& value:batch)
            {
                auto const& m=value.message;
                if(!value.source_capability||!value.source||!value.source_generation||!out.u64(value.source_capability)||!out.u64(value.source)||
                   !out.u64(value.source_generation)||!out.u64(m.channel)||!out.u64(m.sequence)||!out.u64(m.log_time)||!out.u64(m.publish_time)||!out.blob(m.data))return false;
            }
            return true;
        }
        error prepare_key(handle id,std::uint64_t key,mutation op,std::span<item const> batch,bool& duplicate) noexcept
        {
            if(!id||id!=writer_id)return error::bad_handle;
            if(!key)return error::invalid;
            if(batch.size()>maximum_batch)return error::too_large;
            auto measured=wire::writer::measuring(static_cast<std::size_t>(bounds.request_bytes));
            if(!request(measured,op,batch))return error::too_large;
            wire::writer encoded{{requests.get(),static_cast<std::size_t>(bounds.request_bytes)}};
            if(!request(encoded,op,batch))return error::invalid;
            if(key==last.key)
            {
                if(encoded.size()!=last_size||!std::equal(encoded.written().begin(),encoded.written().end(),requests.get()+bounds.request_bytes))return error::conflict;
                duplicate=true;return error::success;
            }
            if(stopped)return error::cancelled;
            if(last.key==UINT64_MAX||key!=last.key+1)return error::conflict;
            duplicate=false;return error::success;
        }
        void commit(receipt value,std::span<item const> batch,receipt& output) noexcept
        {
            auto measured=wire::writer::measuring(static_cast<std::size_t>(bounds.request_bytes));request(measured,value.operation,batch);
            last_size=measured.size();std::memcpy(requests.get()+bounds.request_bytes,requests.get(),last_size);last=value;output=value;
        }
        error seal(object_info& output) noexcept
        {
            if(active==SIZE_MAX){output={};return error::success;}
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            auto& value=slots[active];wire::bytes complete;
            if(value.writer.finish(complete)!=mcap::result::success){retired=true;return error::io;}
            value.info={id,complete.size(),value.writer.count(),buffers::hash(complete)};value.sealed=true;
            output=value.info;active=SIZE_MAX;return error::success;
        }
        void release_replay(std::size_t index) noexcept
        {
            auto& value=playbacks[index];auto* refs=selected(index);
            for(std::size_t i{};i<value.objects;++i)--slots[refs[i]].pins;
            value=playback{};
        }
    public:
        /** @brief Compute all logical reservations with overflow checks before allocation. */
        static bool required_storage(plan const& value,std::string_view profile,std::span<mcap::channel const> source,
                                     std::uint64_t& total,std::uint64_t& definition_size) noexcept
        {
            if(!mcap::valid(profile,source)||!value.segments||value.segments>4096||!value.segment_bytes||value.segment_bytes>64u*1024u*1024u||
               !value.segment_messages||value.segment_messages>65536||!value.replays||value.replays>256||
               value.request_bytes<16||value.request_bytes>buffers::maximum_transfer||!value.read_items||value.read_items>maximum_batch)return false;
            if(value.segment_bytes<mcap::trailer_size)return false;
            mcap::details::writer prefix{static_cast<std::size_t>(value.segment_bytes-mcap::trailer_size)};
            if(!mcap::details::prefix(prefix,profile,source))return false;
            constexpr auto maximum=std::min<std::uint64_t>(SIZE_MAX,INT64_MAX);
            std::uint64_t sum{},defs{};
            auto add=[&](std::uint64_t count,std::uint64_t bytes)
            {if(bytes&&count>(maximum-sum)/bytes)return false;sum+=count*bytes;return true;};
            for(auto const& c:source)
                for(auto bytes:{c.topic.size(),c.type.size(),c.schema_encoding.size(),c.message_encoding.size(),c.qos.size(),c.definition.size()})
                {if(bytes>maximum-defs)return false;defs+=bytes;}
            if(!add(1,defs)||!add(source.size(),sizeof(mcap::channel))||!add(value.segments,sizeof(slot))||!add(value.segments,value.segment_bytes)||
               !add(value.segments*value.segment_messages,sizeof(mcap::position))||!add(value.replays,sizeof(playback))||
               !add(value.replays*value.segments,sizeof(std::size_t))||!add(value.replays*value.read_items,sizeof(mcap::message))||!add(2,value.request_bytes))return false;
            total=sum;definition_size=defs;return true;
        }
        /** @brief Copy immutable schemas and reserve every slot before exposing a writer.
         * @details The silent fast_io allocation path returns no_memory without
         * publishing partial storage or terminating the process. No throwing
         * convenience factory is used. The original store remains retryable.
         */
        error configure(owner caller,plan const& value,std::string_view selected_profile,std::span<mcap::channel const> source)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(configured||retired)return error::denied;
            std::uint64_t needed{},defs{};
            if(!caller||!required_storage(value,selected_profile,source,needed,defs))return error::invalid;
            if(needed>value.reserved_bytes)return error::too_large;
            namespace c=::uwvm2::utils::container;
            namespace a=::uwvm2::utils::allocator::fast_io_strict;
            static_assert(std::is_nothrow_default_constructible_v<mcap::channel>&&std::is_nothrow_default_constructible_v<slot>&&
                          std::is_nothrow_default_constructible_v<mcap::position>&&std::is_nothrow_default_constructible_v<playback>&&
                          std::is_nothrow_default_constructible_v<mcap::message>);
            c::std_unique_ptr<mcap::channel[]> next_channels{a::try_array<mcap::channel>(source.size(),false)};
            if(!next_channels)return error::no_memory;
            c::std_unique_ptr<std::uint8_t[]> next_definitions{a::try_array<std::uint8_t>(static_cast<std::size_t>(defs),false)};
            if(!next_definitions)return error::no_memory;
            c::std_unique_ptr<std::uint8_t[]> next_data{a::try_array<std::uint8_t>(static_cast<std::size_t>(value.segments*value.segment_bytes),false)};
            if(!next_data)return error::no_memory;
            c::std_unique_ptr<std::uint8_t[]> next_requests{a::try_array<std::uint8_t>(static_cast<std::size_t>(2*value.request_bytes),false)};
            if(!next_requests)return error::no_memory;
            c::std_unique_ptr<slot[]> next_slots{a::try_array<slot>(static_cast<std::size_t>(value.segments))};
            if(!next_slots)return error::no_memory;
            c::std_unique_ptr<mcap::position[]> next_indexes{a::try_array<mcap::position>(static_cast<std::size_t>(value.segments*value.segment_messages),false)};
            if(!next_indexes)return error::no_memory;
            c::std_unique_ptr<playback[]> next_playbacks{a::try_array<playback>(static_cast<std::size_t>(value.replays))};
            if(!next_playbacks)return error::no_memory;
            c::std_unique_ptr<std::size_t[]> next_selections{a::try_array<std::size_t>(static_cast<std::size_t>(value.replays*value.segments),false)};
            if(!next_selections)return error::no_memory;
            c::std_unique_ptr<mcap::message[]> next_read_records{a::try_array<mcap::message>(static_cast<std::size_t>(value.replays*value.read_items),false)};
            if(!next_read_records)return error::no_memory;
            std::size_t offset{};
            auto copy=[&](wire::bytes input)->wire::bytes
            {auto* to=next_definitions.get()+offset;if(!input.empty())std::memcpy(to,input.data(),input.size());offset+=input.size();return {to,input.size()};};
            auto text=[&](std::string_view input)->std::string_view
            {auto value=copy(mcap::raw(input));return {reinterpret_cast<char const*>(value.data()),value.size()};};
            for(std::size_t i{};i<source.size();++i)
            {
                auto const& from=source[i];auto& to=next_channels[i];to=from;
                to.topic=text(from.topic);to.type=text(from.type);to.schema_encoding=text(from.schema_encoding);
                to.message_encoding=text(from.message_encoding);to.qos=text(from.qos);to.definition=copy(from.definition);
            }
            mcap::segment probe;
            if(probe.open({next_data.get(),static_cast<std::size_t>(value.segment_bytes)},selected_profile,{next_channels.get(),source.size()},value.segment_messages)!=mcap::result::success)return error::too_large;
            channels=std::move(next_channels);definitions=std::move(next_definitions);data=std::move(next_data);requests=std::move(next_requests);
            slots=std::move(next_slots);indexes=std::move(next_indexes);playbacks=std::move(next_playbacks);selections=std::move(next_selections);
            read_records=std::move(next_read_records);channel_count=source.size();
            profile.assign(selected_profile);bounds=value;bounds.reserved_bytes=needed;authority=caller;configured=true;return error::success;
        }
        /** @brief Open one writer; rejected or undelivered opens can be closed without publication. */
        error open(owner caller,handle& output) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto code=admitted(caller);if(code!=error::success)return code;
            if(writer_id)return error::again;
            auto free=available();if(free==SIZE_MAX)return error::again;
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            code=begin_segment(free);if(code!=error::success)return code;
            active=free;writer_id=id;stopped=false;last={};last_size=0;output=id;return error::success;
        }
        /** @brief Append a whole batch after validating every payload and the exact retry identity. */
        error append(owner caller,handle writer,std::uint64_t key,std::span<item const> batch,receipt& output) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto code=admitted(caller);if(code!=error::success)return code;
            if(batch.empty())return error::invalid;
            if(batch.size()>maximum_batch)return error::too_large;
            for(auto const& value:batch)if(!value.source_capability||!value.source||!value.source_generation)return error::invalid;
            bool duplicate{};code=prepare_key(writer,key,mutation::append,batch,duplicate);if(code!=error::success)return code;
            if(duplicate){output=last;return error::success;}
            auto index=active;bool created=index==SIZE_MAX;
            if(created){index=available();if(index==SIZE_MAX)return error::again;code=begin_segment(index);if(code!=error::success)return code;}
            auto& target=slots[index];auto before=target.writer.size();auto messages=target.writer.count();
            std::array<mcap::message,maximum_batch> selected_messages;
            for(std::size_t i{};i<batch.size();++i)selected_messages[i]=batch[i].message;
            auto result=target.writer.append({selected_messages.data(),batch.size()});
            if(result!=mcap::result::success){if(created)target=slot{};return translate(result);}
            auto* positions=this->positions(index);auto offset=before;
            for(std::size_t i{};i<batch.size();++i)
            {
                auto const& m=batch[i].message;positions[messages+i]={offset+31,m.data.size(),m.log_time,m.publish_time,m.sequence,m.channel};offset+=31+m.data.size();
            }
            active=index;commit({key,batch.size(),target.writer.size()-before,mutation::append,{},false},batch,output);return error::success;
        }
        /** @brief Publish the complete current segment, including an empty first segment.
         * @details Subsequent append reserves a free slot. No space is evicted to
         * make room. Repeated checkpoints with no current segment yield no object.
         */
        error checkpoint(owner caller,handle writer,std::uint64_t key,receipt& output) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto code=admitted(caller);if(code!=error::success)return code;
            bool duplicate{};code=prepare_key(writer,key,mutation::checkpoint,{},duplicate);if(code!=error::success)return code;
            if(duplicate){output=last;return error::success;}
            object_info info;code=seal(info);if(code!=error::success)return code;
            commit({key,0,0,mutation::checkpoint,info,false},{},output);return error::success;
        }
        /** @brief Explicit drain or abort; neither policy discards previously sealed objects. */
        error stop(owner caller,handle writer,std::uint64_t key,bool drain,receipt& output) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto code=admitted(caller);if(code!=error::success)return code;
            auto op=drain?mutation::stop_drain:mutation::stop_abort;
            bool duplicate{};code=prepare_key(writer,key,op,{},duplicate);if(code!=error::success)return code;
            if(duplicate){output=last;return error::success;}
            object_info info;if(drain){code=seal(info);if(code!=error::success)return code;}
            else if(active!=SIZE_MAX){slots[active]=slot{};active=SIZE_MAX;}
            stopped=true;commit({key,0,0,op,info,true},{},output);return error::success;
        }
        /** @brief Close the writer; active uncheckpointed data is explicitly abandoned. */
        error close_writer(owner caller,handle writer) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(!configured||caller!=authority)return error::denied;
            if(!writer||writer!=writer_id)return error::bad_handle;
            if(active!=SIZE_MAX){slots[active]=slot{};active=SIZE_MAX;}
            writer_id=0;stopped=true;return error::success;
        }
        /** @brief Borrow a complete sealed object under a retained pin and separate export authority. */
        template<typename Sink> error export_object(owner caller,object_info const& expected,Sink&& sink)
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex};auto code=admitted(caller);if(code!=error::success)return code;
            auto index=find_object(expected.id);if(index==SIZE_MAX)return error::bad_handle;
            auto& value=slots[index];if(value.info!=expected)return error::conflict;
            if(value.pins==UINT64_MAX)return error::too_large;
            ++value.pins;auto bytes=wire::bytes{arena(index).first(static_cast<std::size_t>(value.info.bytes))};lock.unlock();bool copied{};
            try{copied=sink(value.info,bytes);}catch(...){lock.lock();--value.pins;throw;}
            lock.lock();--value.pins;if(retired)return error::cancelled;return copied?error::success:error::io;
        }
        /** @brief Explicitly release an unpinned immutable object; retained capacity is never hidden. */
        error erase(owner caller,object_info const& expected) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto code=admitted(caller);if(code!=error::success)return code;
            auto index=find_object(expected.id);if(index==SIZE_MAX)return error::bad_handle;
            auto& value=slots[index];if(value.info!=expected)return error::conflict;
            if(value.pins)return error::again;
            value=slot{};return error::success;
        }
        /** @brief Admit one complete external segment into an already reserved free slot.
         * @details The dispatcher must hold the immutable artifact read lease
         * and verify its exact profile and digest authority. Copy, SHA256 and
         * complete bounded indexing finish before the object is published.
         * Failure leaves no object or consumed slot. This trusted primitive is
         * not a guest import; a failed creation-result copy must erase its new
         * object before retrying. Reading it grants no ROS publish permission.
         */
        error import_segment(owner caller,wire::bytes input,buffers::digest const& expected,object_info& output) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto code=admitted(caller);if(code!=error::success)return code;
            if(input.size()>bounds.segment_bytes)return error::too_large;
            auto index=available();if(index==SIZE_MAX)return error::again;
            auto copied=arena(index).first(input.size());if(!input.empty())std::memmove(copied.data(),input.data(),input.size());
            if(buffers::hash(copied)!=expected)return error::conflict;
            std::size_t count{};
            auto result=mcap::index(copied,profile.view(),{channels.get(),channel_count},
                {bounds.segment_bytes,bounds.segment_messages+2*channel_count+3,bounds.segment_messages},
                {positions(index),static_cast<std::size_t>(bounds.segment_messages)},count);
            if(result!=mcap::result::success)return translate(result);
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            auto& value=slots[index];value=slot{};value.occupied=value.sealed=true;value.info={id,input.size(),count,expected};
            output=value.info;return error::success;
        }
        /** @brief Pin an exact ordered immutable object set; no automatic source discovery. */
        error replay_open(owner caller,std::span<object_info const> objects,handle& output) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto code=admitted(caller);if(code!=error::success)return code;
            if(objects.empty()||objects.size()>bounds.segments)return error::invalid;
            std::size_t index{};while(index<bounds.replays&&playbacks[index].id)++index;if(index==bounds.replays)return error::again;
            auto* refs=selected(index);std::uint64_t total{};
            for(std::size_t i{};i<objects.size();++i)
            {
                auto found=find_object(objects[i].id);if(found==SIZE_MAX)return error::bad_handle;
                if(slots[found].info!=objects[i])return error::conflict;
                for(std::size_t j{};j<i;++j)if(refs[j]==found)return error::invalid;
                if(slots[found].pins==UINT64_MAX||objects[i].messages>UINT64_MAX-total)return error::too_large;
                refs[i]=found;total+=objects[i].messages;
            }
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            for(std::size_t i{};i<objects.size();++i)++slots[refs[i]].pins;
            playbacks[index]={id,0,total,objects.size(),false,false};output=id;return error::success;
        }
        /** @brief Deliver complete records without consuming on failed/throwing/cancelled copies.
         * @details Sinks receive the starting message ordinal and immutable
         * records. EOF is a successful empty batch. A seek is an absolute
         * ordinal; it neither rewrites timestamps nor advances a ROS clock.
         */
        template<typename Sink> error read(owner caller,handle id,std::uint64_t count,Sink&& sink)
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex};auto code=admitted(caller);if(code!=error::success)return code;
            auto index=find_replay(id);if(index==SIZE_MAX)return error::bad_handle;
            auto& value=playbacks[index];if(value.closing)return error::cancelled;
            if(value.delivering)return error::again;
            if(!count||count>bounds.read_items)return error::too_large;
            auto amount=static_cast<std::size_t>(std::min(count,value.total-value.cursor));auto* records=read_scratch(index);
            auto remaining=value.cursor;auto* refs=selected(index);std::size_t written{};
            for(std::size_t i{};i<value.objects&&written<amount;++i)
            {
                auto segment=refs[i];auto const& info=slots[segment].info;
                if(remaining>=info.messages){remaining-=info.messages;continue;}
                auto* offsets=positions(segment);auto bytes=arena(segment);
                for(auto j=remaining;j<info.messages&&written<amount;++j)
                {
                    auto const& p=offsets[j];records[written++]={p.channel,p.sequence,p.log_time,p.publish_time,bytes.subspan(static_cast<std::size_t>(p.offset),static_cast<std::size_t>(p.size))};
                }
                remaining=0;
            }
            auto cursor=value.cursor;value.delivering=true;lock.unlock();bool copied{};
            try{copied=sink(cursor,std::span<mcap::message const>{records,amount});}
            catch(...){lock.lock();value.delivering=false;if(value.closing)release_replay(index);throw;}
            lock.lock();value.delivering=false;
            if(value.closing){release_replay(index);return error::cancelled;}
            if(retired)return error::cancelled;
            if(!copied)return error::io;
            value.cursor+=amount;return error::success;
        }
        /** @brief Absolute message ordinal within the admitted set; no delivery or clock effect. */
        error seek(owner caller,handle id,std::uint64_t position) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto code=admitted(caller);if(code!=error::success)return code;
            auto index=find_replay(id);if(index==SIZE_MAX)return error::bad_handle;
            auto& value=playbacks[index];if(value.closing)return error::cancelled;if(value.delivering)return error::again;
            if(position>value.total)return error::invalid;
            value.cursor=position;return error::success;
        }
        /** @brief Close replay, deferring pin release while a checked delivery is active. */
        error close_replay(owner caller,handle id) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(!configured||caller!=authority)return error::denied;
            auto index=find_replay(id);if(index==SIZE_MAX)return error::bad_handle;
            if(playbacks[index].delivering)playbacks[index].closing=true;else release_replay(index);
            return error::success;
        }
        /** @brief Fence new operations and final consumption; keep accounting and immutable bytes. */
        error retire(owner caller) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(!configured||caller!=authority)return error::denied;
            retired=true;return error::success;
        }
        /** @brief Cached, nonconsuming metadata suitable for separately authorized debugger projection. */
        error inspect(owner caller,usage& output) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!configured||caller!=authority)return error::denied;
            usage value;value.reserved_bytes=bounds.reserved_bytes;value.retired=retired;value.recording=writer_id&&!stopped;
            for(std::size_t i{};i<bounds.segments;++i)
            {auto const& s=slots[i];if(s.sealed){++value.sealed_segments;value.sealed_bytes+=s.info.bytes;}value.pins+=s.pins;}
            for(std::size_t i{};i<bounds.replays;++i)if(playbacks[i].id)++value.replays;
            if(active!=SIZE_MAX)value.active_messages=slots[active].writer.count();
            output=value;return error::success;
        }
    };
}
