/** @file
 * @brief Owned single-node worker manifest with no paths, FDs or guest authority.
 * @details One independently sealed worker represents exactly one approved
 * context/node pair and one participant. Decode is inert and transactional.
 * Context open may retain a logical private context; only node open holding the
 * complete context/node authority group may attach transport pipes. Preparation
 * can warm the closed codecs while no external FD is present. It must not create
 * another node or leave warm-up endpoints visible when attachment occurs.
 * Native closures, image digests and syscall restrictions belong to the separate
 * communication sandbox manifest. This file does not grant or start a worker.
 */
#pragma once
#include "communication_configuration.h"
#include "communication_transport_protocol.h"
namespace uwvm2::imported::ros::scene::communication_worker_configuration
{
    namespace configuration=communication_configuration;
    namespace transport=communication_transport;
    inline constexpr std::string_view magic="uwvm.ros.communication.worker";
    /** @brief Resource ceilings for one worker, independent of registered codecs.
     * @details The deployment must still reserve every actual type, endpoint and
     * compiled schema byte. These ceilings grant no default capacity and do not
     * register a native factory. They bound inert parsing work and agree with
     * the host deployment's aggregate type ceiling. A zero type reservation is
     * valid for a context/node-only worker.
     */
    inline constexpr std::uint64_t maximum_types=1024;
    inline constexpr std::uint64_t maximum_definitions=2+maximum_types+64+32+16+8;
    struct specification
    {
        transport::plan network;
        configuration::budget capacity;
        std::uint64_t request_bytes{},response_bytes{};
        ::uwvm2::utils::container::std_vector<configuration::definition> definitions;
    };
    inline communication::profile const& profile_of(configuration::definition const& definition) noexcept
    {
        return std::visit([](auto const& plan)->communication::profile const&
        {if constexpr(requires{plan.selected;})return plan.selected;else return plan.policy.selected;},definition.policy);
    }
    inline configuration::definition const* find(specification const& input,configuration::family family,communication::profile const& profile) noexcept
    {for(auto const& item:input.definitions)if(item.kind==family&&profile_of(item)==profile)return &item;return nullptr;}
    inline bool read_u16(wire::reader& in,std::uint16_t& out) noexcept
    {std::uint64_t value{};if(!in.u64(value)||value>UINT16_MAX)return false;out=static_cast<std::uint16_t>(value);return true;}
    inline bool read_u32(wire::reader& in,std::uint32_t& out) noexcept
    {return configuration::read_small(in,out);}
    inline bool read_network(wire::reader& in,transport::plan& plan) noexcept
    {
        wire::bytes local,peer;
        if(!in.u64(plan.generation)||!read_u32(in,plan.domain)||!read_u32(in,plan.local_participant)||!read_u32(in,plan.peer_participant)||
           !read_u16(in,plan.local_discovery)||!read_u16(in,plan.local_user)||!read_u16(in,plan.peer_discovery)||!read_u16(in,plan.peer_user)||
           !read_u16(in,plan.local_discovery_source)||!read_u16(in,plan.local_user_source)||!read_u16(in,plan.peer_source)||
           !in.blob(local)||local.size()!=12||!in.blob(peer)||peer.size()!=12)return false;
        std::copy(local.begin(),local.end(),plan.local_prefix.begin());std::copy(peer.begin(),peer.end(),plan.peer_prefix.begin());
        for(auto& allowed:plan.allowed)if(!in.boolean(allowed))return false;
        if(!in.boolean(plan.user_data_receive)||!in.boolean(plan.user_data_transmit)||!in.boolean(plan.allow_uncertain_attenuation)||
           !in.u64(plan.frame_deadline_ns)||!read_u32(in,plan.max_datagram_bytes)||!read_u32(in,plan.max_sample_bytes)||
           !read_u32(in,plan.max_submessages)||!read_u32(in,plan.socket_buffer_bytes))return false;
        for(auto& quota:plan.queues)if(!in.u64(quota.packets)||!in.u64(quota.bytes)||!in.boolean(quota.attenuated)||
            !in.u64(quota.packets_per_second)||!in.u64(quota.bytes_per_second))return false;
        return plan.valid();
    }
    inline bool read_capacity(wire::reader& in,configuration::budget& capacity) noexcept
    {
        for(auto* value:{&capacity.contexts,&capacity.nodes,&capacity.types,&capacity.topics,&capacity.services,&capacity.actions,&capacity.graphs,
                        &capacity.streams,&capacity.queue_bytes,&capacity.definition_bytes,&capacity.observers})if(!in.u64(*value))return false;
        return capacity.contexts==1&&capacity.nodes==1&&capacity.types<=maximum_types&&capacity.topics<=64&&capacity.services<=32&&capacity.actions<=16&&capacity.graphs<=8&&
            capacity.streams==0&&capacity.queue_bytes<=(1ull<<30)&&capacity.definition_bytes>0&&capacity.definition_bytes<=(1ull<<24)&&capacity.observers<=65536;
    }
    /** @brief Validate every profile dependency against this single-node manifest.
     * @details Owners remain zero until the trusted host dispatcher mints them.
     * Neither equal integer resource IDs nor a matching profile name alone can
     * supply an origin. Type references compare complete closed codec schemas,
     * or accept a frozen bounded CDR schema for separate native type approval.
     * This structural check does not register or qualify a native factory.
     */
    inline bool origins(specification const& input) noexcept
    {
        auto compiled_type=[](communication::type_plan const& value,std::string_view codec)noexcept
        {
            auto expected=communication::codecs::find(codec);
            return expected&&value.type_identity==expected->identity&&value.encoding==expected->encoding&&value.validate==expected->validate&&
                std::equal(value.descriptor.begin(),value.descriptor.end(),expected->schema.begin(),expected->schema.end());
        };
        communication::context_plan const* context{};communication::node_plan const* node{};
        for(auto const& item:input.definitions)
        {
            if(item.kind==configuration::family::context){if(context)return false;context=&std::get<communication::context_plan>(item.policy);}
            if(item.kind==configuration::family::node){if(node)return false;node=&std::get<communication::node_plan>(item.policy);}
        }
        if(!context||!node||node->context!=context->selected||context->domain!=input.network.domain)return false;
        for(auto const& item:input.definitions)
        {
            switch(item.kind)
            {
                case configuration::family::context:case configuration::family::node:case configuration::family::type:break;
                case configuration::family::topic:
                {
                    auto const& p=std::get<communication::topic_plan>(item.policy);auto type=find(input,configuration::family::type,p.type);
                    if(p.node!=node->selected||!type)return false;
                    auto const& schema=std::get<communication::type_plan>(type->policy);
                    if(!compiled_type(schema,"std-string")&&!compiled_type(schema,"std-bool")&&
                       !(schema.validate.bounded()&&schema.encoding=="cdr"&&schema.validate.bounded()->encoding()==bounded_message::layout::cdr))return false;
                    break;
                }
                case configuration::family::service:
                {
                    auto const& p=std::get<communication::service_plan>(item.policy);
                    auto request=find(input,configuration::family::type,p.request_type),response=find(input,configuration::family::type,p.response_type);
                    if(p.node!=node->selected||!request||!response||!compiled_type(std::get<communication::type_plan>(request->policy),"set-bool-request")||
                       !compiled_type(std::get<communication::type_plan>(response->policy),"set-bool-response"))return false;
                    break;
                }
                case configuration::family::action:
                {
                    auto const& wrapper=std::get<communication_action_dispatch::plan>(item.policy);auto const& p=wrapper.policy;if(wrapper.node_profile!=node->selected)return false;
                    for(auto const* expected:{&p.goal,&p.feedback,&p.result})
                    {auto item=find(input,configuration::family::type,expected->selected);if(!item)return false;auto const& actual=std::get<communication::type_plan>(item->policy);
                     if(actual.type_identity!=expected->type_identity||actual.encoding!=expected->encoding||actual.descriptor!=expected->descriptor||actual.validate!=expected->validate)return false;}
                    break;
                }
                case configuration::family::graph:
                {auto const& p=std::get<communication_graph_dispatch::plan>(item.policy);if(p.context_profile!=context->selected||p.node_profile!=node->selected)return false;break;}
                default:return false; // Event streams remain host-local.
            }
        }
        return true;
    }
    /** @brief Decode complete sealed startup bytes without retaining input views.
     * @details A failed parse leaves output unchanged. Extents, limits and
     * aggregate reservations are checked before allocating the next definition;
     * all owned storage uses recoverable fast_io. Runtime paths and native file
     * descriptors cannot be expressed by this carrier.
     */
    inline error decode(wire::bytes bytes,std::uint64_t maximum,specification& output) noexcept
    {
        if(!maximum||maximum>16u*1024u*1024u||bytes.size()>maximum)return error::too_large;
        try
        {
            specification value;wire::reader in{bytes};std::string_view marker;std::uint64_t count{};
            if(!in.text(marker)||marker!=magic||!read_network(in,value.network)||!read_capacity(in,value.capacity)||
               !in.u64(value.request_bytes)||!value.request_bytes||value.request_bytes>4u*1024u*1024u||
               !in.u64(value.response_bytes)||!value.response_bytes||value.response_bytes>4u*1024u*1024u||!in.u64(count)||count<2||count>maximum_definitions)return error::invalid;
            // read_capacity has bounded every summand. Refuse a declaration
            // count beyond the explicit family reservations before allocating.
            auto const& capacity=value.capacity;
            if(count>capacity.contexts+capacity.nodes+capacity.types+capacity.topics+capacity.services+capacity.actions+capacity.graphs)return error::too_large;
            value.definitions.reserve(static_cast<std::size_t>(count));configuration::budget used{};
            for(std::uint64_t i{};i<count;++i)
            {
                std::uint64_t family{},provider{},limits{};communication::profile selected;
                if(!in.u64(family)||family>static_cast<std::uint64_t>(configuration::family::graph)||!in.u64(provider)||provider>3||
                   !configuration::read_profile(in,selected)||!in.u64(limits)||limits>configuration::limit_fields.size())return error::invalid;
                configuration::configuration config;
                if(config.profile(static_cast<configuration::family>(family),static_cast<configuration::provider>(provider),selected)!=error::success)return error::invalid;
                for(std::uint64_t n{};n<limits;++n){std::string_view key;std::uint64_t number{};if(!in.text(key)||!in.u64(number)||config.limit(key,number)!=error::success)return error::invalid;}
                wire::bytes definition;if(!in.blob(definition))return error::invalid;
                auto status=config.parse(definition);if(status!=error::success)return status;
                status=config.accumulate(value.capacity,used);if(status!=error::success)return status;
                configuration::definition owned;if(config.freeze(owned)!=error::success)return error::invalid;
                for(auto const& previous:value.definitions)if(profile_of(previous)==profile_of(owned))return error::conflict;
                value.definitions.push_back(std::move(owned));
            }
            if(!in.done()||!origins(value))return error::invalid;
            output=std::move(value);return error::success;
        }
        catch(std::bad_alloc const&){return error::no_memory;}
        catch(...){return error::io;}
    }
}
