/** @file
 * @brief Frozen, hashed recording definitions and explicit finite deployment capacities.
 * @details A definition contains the ROS container profile, ordered channels,
 * exact source buffer aliases/profiles and names from a trusted compiled schema
 * registry. A configuration cannot load a native symbol or select a path/socket.
 * Registry validators must be pure, bounded and noexcept; their type, encoding
 * and complete definition digest must match the file. Unknown codecs fail closed.
 * No codec is enabled by default and this parser makes no generic IDL claim.
 *
 * File layout is text magic, profile(identity, SHA256), then body: text ROS
 * profile and array<channel>. Each channel is id, schema id, topic, codec name,
 * type, schema encoding, message encoding, QoS text, schema-definition bytes,
 * maximum message bytes, buffer alias and buffer profile. The logical profile
 * SHA256 covers that entire body. A separate transport SHA covers the whole
 * file when read through configuration_file. All capacities must be specified
 * exactly once before parsing; no implicit unbounded reservation exists.
 */
#pragma once
#include "recording_dispatch.h"

namespace uwvm2::imported::ros::scene::recording
{
    inline constexpr std::string_view definition_magic="uwvm.ros.recording.definition";
    /** @brief Immutable compiled serializer identity, never supplied as an address by configuration. */
    struct codec
    {
        std::string_view name,ros_profile,type,schema_encoding,message_encoding;
        buffers::digest definition_sha256{};
        bool(*validate)(wire::bytes) noexcept{};
    };
    struct owned_channel
    {
        std::uint16_t id{},schema{};
        ::uwvm2::utils::container::std_string topic,type,schema_encoding,message_encoding,qos;
        ::uwvm2::utils::container::std_vector<std::uint8_t> definition;
        bool(*validate)(wire::bytes) noexcept{};
        std::uint64_t maximum_message{};
        recording_dispatch::channel_source source;
        mcap::channel view()const noexcept
        {return {id,schema,topic,type,schema_encoding,message_encoding,qos,definition,validate,maximum_message};}
    };
    /** @brief Copyable owned deployment data; views are constructed only after the final copy. */
    struct definition
    {
        buffers::profile selected;buffers::text<4> ros_profile;plan capacity;
        ::uwvm2::utils::container::std_vector<owned_channel> channels;
        /** @brief Pure runtime store reservation, including exact ownership arrays. */
        bool storage(std::uint64_t& output)const noexcept
        {
            if(channels.empty()||channels.size()>mcap::maximum_channels||ros_profile.size>ros_profile.bytes.size())return false;
            std::array<mcap::channel,mcap::maximum_channels> views;
            for(std::size_t i{};i<channels.size();++i)views[i]=channels[i].view();
            return recording_dispatch::recording_storage(capacity,ros_profile.view(),{views.data(),channels.size()},output);
        }
        /** @brief Conservative retained configuration size, including string capacities and fixed metadata. */
        bool retained(std::uint64_t& output)const noexcept
        {
            std::uint64_t slots=channels.capacity();
            if(slots>(INT64_MAX-sizeof(definition))/sizeof(owned_channel))return false;
            std::uint64_t total=sizeof(definition)+slots*sizeof(owned_channel);
            for(auto const& channel:channels)
                for(auto bytes:{channel.topic.capacity(),channel.type.capacity(),channel.schema_encoding.capacity(),
                                channel.message_encoding.capacity(),channel.qos.capacity(),channel.definition.capacity()})
                {if(bytes>INT64_MAX-total)return false;total+=bytes;}
            output=total;return true;
        }
        /** @brief Bind all owned schema views synchronously; the dispatcher copies them before returning. */
        error bind(recording_dispatch& dispatcher,std::string_view alias)const
        {
            if(channels.empty()||channels.size()>mcap::maximum_channels||ros_profile.size>ros_profile.bytes.size())return error::invalid;
            std::array<mcap::channel,mcap::maximum_channels> views;
            std::array<recording_dispatch::channel_source,mcap::maximum_channels> sources;
            for(std::size_t i{};i<channels.size();++i){views[i]=channels[i].view();sources[i]=channels[i].source;}
            return dispatcher.bind_recording(alias,selected,capacity,ros_profile.view(),{views.data(),channels.size()},{sources.data(),channels.size()});
        }
    };
    /** @brief Explicit replay scope selecting a single immutable recording alias. */
    struct replay_definition
    {buffers::text<255> source;recording_dispatch::replay_plan policy;};
    class configuration
    {
        plan capacity_;std::uint64_t definition_bytes_{},channel_limit_{},assigned_{};
        buffers::profile selected_;definition prepared_;bool profiled_{},defined_{},frozen_{};
        static bool valid_profile(buffers::profile const& value)noexcept
        {return value.identity.size&&value.identity.size<=value.identity.bytes.size()&&value.identity.view().find('\0')==std::string_view::npos&&
            wire::utf8({reinterpret_cast<std::uint8_t const*>(value.identity.bytes.data()),value.identity.size});}
        static bool read_profile(wire::reader& in,buffers::profile& output)noexcept
        {return recording_contract::profile(in,output);}
    public:
        inline static constexpr std::array<std::string_view,9> limit_names{
            "segments","segment.bytes","segment.messages","replays","request.bytes","read.items","storage.bytes","definition.bytes","channels"};
        /** @brief Select a complete immutable logical descriptor once, before definition parsing. */
        error profile(buffers::profile selected)noexcept
        {
            if(profiled_||defined_||frozen_)return error::conflict;
            if(!valid_profile(selected))return error::invalid;
            selected_=selected;profiled_=true;return error::success;
        }
        /** @brief Assign every independent finite storage/count ceiling explicitly once. */
        error limit(std::string_view field,std::uint64_t value)noexcept
        {
            if(defined_||frozen_)return error::conflict;
            auto found=std::find(limit_names.begin(),limit_names.end(),field);if(found==limit_names.end()||value==UINT64_MAX)return error::invalid;
            auto index=static_cast<std::size_t>(found-limit_names.begin());auto bit=std::uint64_t{1}<<index;if(assigned_&bit)return error::conflict;
            constexpr std::array<std::uint64_t,9> maxima{4096,64u*1024u*1024u,65536,256,buffers::maximum_transfer,maximum_batch,INT64_MAX,4u*1024u*1024u,mcap::maximum_channels};
            if(value>maxima[index])return error::too_large;
            std::array fields{&capacity_.segments,&capacity_.segment_bytes,&capacity_.segment_messages,&capacity_.replays,&capacity_.request_bytes,
                &capacity_.read_items,&capacity_.reserved_bytes,&definition_bytes_,&channel_limit_};
            *fields[index]=value;assigned_|=bit;return error::success;
        }
        /** @brief Expose the configured input ceiling before any trusted file read or allocation. */
        error definition_limit(std::uint64_t& output)const noexcept
        {
            if(defined_||frozen_)return error::conflict;
            if(!profiled_||assigned_!=511)return error::denied;
            if(!definition_bytes_)return error::too_large;
            output=definition_bytes_;return error::success;
        }
        /** @brief Parse a complete canonical owned definition against an immutable compiled registry.
         * @details The registry is bounded to 4096 unique exact names. A codec
         * name cannot substitute a schema, encoding or ROS family. Logical hash,
         * all field bounds and whole MCAP definition validation precede commit.
         * Recoverable allocation failure leaves the parser ready for exact retry.
         */
        error parse(wire::bytes input,std::span<codec const> registry)noexcept
        {
            if(defined_||frozen_)return error::conflict;
            if(!profiled_||assigned_!=511)return error::denied;
            if(!definition_bytes_||input.size()>definition_bytes_||registry.size()>4096)return error::too_large;
            for(std::size_t i{};i<registry.size();++i)
            {
                if(!identifier(registry[i].name)||!registry[i].validate)return error::invalid;
                for(std::size_t j{};j<i;++j)if(registry[i].name==registry[j].name)return error::conflict;
            }
            try
            {
                wire::reader in{input};std::string_view marker;buffers::profile selected;
                if(!in.text(marker)||marker!=definition_magic||!read_profile(in,selected))return error::invalid;
                if(selected!=selected_)return error::conflict;
                if(buffers::hash(input.subspan(in.position()))!=selected_.sha256)return error::conflict;
                std::string_view ros;std::uint64_t count{};
                if(!in.text(ros)||(ros!="ros1"&&ros!="ros2")||!in.u64(count)||!count||count>channel_limit_||count>mcap::maximum_channels)return error::invalid;
                definition value;value.selected=selected_;value.ros_profile.assign(ros);value.capacity=capacity_;
                value.channels.reserve(static_cast<std::size_t>(count));
                for(std::size_t i{};i<count;++i)
                {
                    owned_channel channel;std::uint64_t id{},schema{};std::string_view topic,codec_name,type,schema_encoding,message_encoding,qos,alias;wire::bytes definition;
                    if(!in.u64(id)||!id||id>UINT16_MAX||!in.u64(schema)||!schema||schema>UINT16_MAX||
                       !in.text(topic)||!mcap::text(topic,255)||!in.text(codec_name)||!identifier(codec_name)||
                       !in.text(type)||!mcap::text(type,255)||!in.text(schema_encoding)||!mcap::text(schema_encoding,32)||
                       !in.text(message_encoding)||!mcap::text(message_encoding,32)||!in.text(qos)||!mcap::text(qos,16384,ros=="ros1")||
                       !in.blob(definition)||definition.empty()||definition.size()>1024u*1024u||!wire::utf8(definition)||
                       !in.u64(channel.maximum_message)||!channel.maximum_message||channel.maximum_message>buffers::maximum_transfer||
                       !in.text(alias)||!identifier(alias)||!channel.source.alias.assign(alias)||!read_profile(in,channel.source.schema))return error::invalid;
                    codec const* selected_codec{};for(auto const& c:registry)if(c.name==codec_name)selected_codec=&c;
                    if(!selected_codec||selected_codec->ros_profile!=ros||selected_codec->type!=type||selected_codec->schema_encoding!=schema_encoding||
                       selected_codec->message_encoding!=message_encoding||selected_codec->definition_sha256!=buffers::hash(definition))return error::denied;
                    channel.id=channel.source.channel=static_cast<std::uint16_t>(id);channel.schema=static_cast<std::uint16_t>(schema);
                    channel.topic.assign(topic);channel.type.assign(type);channel.schema_encoding.assign(schema_encoding);channel.message_encoding.assign(message_encoding);
                    channel.qos.assign(qos);channel.definition.assign(definition.begin(),definition.end());channel.validate=selected_codec->validate;
                    value.channels.push_back(std::move(channel));
                }
                if(!in.done())return error::invalid;
                std::array<mcap::channel,mcap::maximum_channels> views;for(std::size_t i{};i<count;++i)views[i]=value.channels[i].view();
                std::uint64_t needed{},definitions{};if(!store::required_storage(capacity_,ros,{views.data(),static_cast<std::size_t>(count)},needed,definitions))return error::invalid;
                if(needed>capacity_.reserved_bytes)return error::too_large;
                prepared_=std::move(value);defined_=true;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}
        }
        /** @brief Publish an owned immutable copy; allocation failure does not freeze the parser. */
        error freeze(definition& output)noexcept
        {
            if(!defined_)return error::denied;
            try{auto next=prepared_;output=std::move(next);frozen_=true;return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}
        }
    };
}

namespace uwvm2::imported::ros::scene
{
    class recording_budget_configuration
    {
        recording_dispatch::limits value_;std::uint64_t assigned_{};bool frozen_{};
    public:
        inline static constexpr std::array<std::string_view,5> names{"recordings","replays","storage.bytes","workspace.bytes","workspace.calls"};
        error limit(std::string_view field,std::uint64_t value)noexcept
        {
            if(frozen_)return error::conflict;
            auto found=std::find(names.begin(),names.end(),field);if(found==names.end()||value==UINT64_MAX)return error::invalid;
            auto index=static_cast<std::size_t>(found-names.begin());auto bit=std::uint64_t{1}<<index;if(assigned_&bit)return error::conflict;
            if(value>(index<2?256u:static_cast<std::uint64_t>(INT64_MAX)))return error::too_large;
            std::array fields{&value_.recordings,&value_.replays,&value_.storage_bytes,&value_.workspace_bytes,&value_.workspace_calls};
            *fields[index]=value;assigned_|=bit;return error::success;
        }
        error freeze(recording_dispatch::limits& output)noexcept
        {
            if(assigned_!=31)return error::denied;
            if((value_.recordings||value_.replays)&&(!value_.recordings||!value_.storage_bytes||!value_.workspace_bytes||!value_.workspace_calls))return error::invalid;
            output=value_;frozen_=true;return error::success;
        }
    };
}
