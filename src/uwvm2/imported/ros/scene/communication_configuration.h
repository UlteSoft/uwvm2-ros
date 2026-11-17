/** @file
 * @brief Owned canonical deployment definitions for all communication families.
 * @details Parsing is inert: no native I/O, path opening, function loading or
 * implicit authority. The closed provider names describe required deployment
 * mechanisms; accepting ros2-loopback-worker does not start or qualify a worker.
 * Every applicable named limit must be explicitly supplied once. Zero stays zero;
 * required positive capacities fail validation instead of acquiring defaults.
 *
 * Definition carrier: text magic, profile identity, blob logical SHA256, then
 * body {text family,text provider,family-specific semantic fields}. The logical
 * hash covers the exact complete body, including provider selection. The caller
 * independently pins and hashes the complete configuration file. Capacity limits
 * are explicit deployment restrictions outside the semantic body. Profiles and
 * definitions are immutable after freeze. Native owners are assigned later by
 * the capability dispatcher; no owner/address/function pointer appears on wire.
 */
#pragma once
#include "communication_action_dispatch.h"
#include "communication_graph_dispatch.h"
#include "communication_event_dispatch.h"
#include "communication_codecs.h"
namespace uwvm2::imported::ros::scene::communication_configuration
{
    inline constexpr std::string_view magic="uwvm.ros.communication.definition";
    enum class family:std::uint8_t{context,node,type,topic,service,action,graph,event};
    enum class provider:std::uint8_t{unconfigured,ros2_loopback_worker,canonical_schema,local_event_stream};
    inline constexpr std::array<std::string_view,8> family_names{"context","node","type","topic","service","action","graph","event"};
    inline constexpr std::array<std::string_view,4> provider_names{"unconfigured","ros2-loopback-worker","canonical-schema","local-event-stream"};
    using owned_plan=std::variant<communication::context_plan,communication::node_plan,communication::type_plan,communication::topic_plan,
        communication::service_plan,communication_action_dispatch::plan,communication_graph_dispatch::plan,communication_event_dispatch::plan>;
    struct definition{family kind{};provider backend{};owned_plan policy;};
    struct limit_field{std::string_view name;std::uint64_t families,maximum;};
    constexpr std::uint64_t mask(family value) noexcept{return std::uint64_t{1}<<static_cast<unsigned>(value);}
    inline constexpr std::uint64_t context=mask(family::context),node=mask(family::node),type=mask(family::type),topic=mask(family::topic),
        service=mask(family::service),action=mask(family::action),graph=mask(family::graph),event=mask(family::event);
    inline constexpr std::array limit_fields{
        limit_field{"definition.bytes",255,4u*1024u*1024u},limit_field{"instances",context|node|topic|service|action|graph,1},
        limit_field{"observers",node,65536},limit_field{"descriptor.bytes",type,4u*1024u*1024u},limit_field{"message.bytes",topic,16u*1024u*1024u},
        limit_field{"queue.items",topic,65536},limit_field{"queue.bytes",topic|service|action,1ull<<30},limit_field{"deadline.ns",topic|service|action|event,INT64_MAX},
        limit_field{"pending",service,65536},limit_field{"request.bytes",service,16u*1024u*1024u},limit_field{"response.bytes",service,16u*1024u*1024u},
        limit_field{"goals",action,65536},limit_field{"cancels_per_goal",action,65536},limit_field{"feedback.items",action,65536},
        limit_field{"goal.bytes",action,16u*1024u*1024u},limit_field{"feedback.bytes",action,16u*1024u*1024u},limit_field{"result.bytes",action|graph|event,1ull<<30},
        limit_field{"transitions",action,510},limit_field{"retention.ns",action,INT64_MAX},limit_field{"rows",graph,65536},
        limit_field{"page.items",graph,65536},limit_field{"cursor.ns",graph,INT64_MAX},limit_field{"capture.ns",graph,INT64_MAX},
        limit_field{"streams",event,65536},limit_field{"sources",event,64},limit_field{"events",event,65536},limit_field{"workspace.bytes",event,1ull<<30}};
    /** @brief Common globally bounded deployment totals, separate from rate attenuation. */
    struct budget
    {std::uint64_t contexts{},nodes{},types{},topics{},services{},actions{},graphs{},streams{},queue_bytes{},definition_bytes{},observers{};};
    inline bool add(std::uint64_t left,std::uint64_t right,std::uint64_t& result) noexcept
    {if(right>UINT64_MAX-left)return false;result=left+right;return true;}
    inline bool selected_profile(communication::profile const& p) noexcept
    {return communication::valid(p)&&p.identity.find('\0')==p.identity.npos&&wire::utf8({reinterpret_cast<std::uint8_t const*>(p.identity.data()),p.identity.size()});}
    inline bool read_text(wire::reader& in,::uwvm2::utils::container::std_string& out,std::size_t maximum=255)
    {std::string_view value;if(!in.text(value)||value.size()>maximum||value.find('\0')!=value.npos)return false;out.assign(value);return true;}
    inline bool read_profile(wire::reader& in,communication::profile& out)
    {wire::bytes digest;if(!read_text(in,out.identity)||!in.blob(digest)||digest.size()!=32)return false;std::copy(digest.begin(),digest.end(),out.digest.begin());return selected_profile(out);}
    inline bool read_small(wire::reader& in,std::uint32_t& out) noexcept
    {std::uint64_t value{};if(!in.u64(value)||value>UINT32_MAX)return false;out=static_cast<std::uint32_t>(value);return true;}
    inline bool read_qos(wire::reader& in,communication::qos& out) noexcept
    {return read_small(in,out.history)&&read_small(in,out.reliability)&&read_small(in,out.durability)&&read_small(in,out.liveliness)&&in.u64(out.depth)&&
        in.u64(out.deadline.seconds)&&in.u64(out.deadline.nanoseconds)&&in.u64(out.lifespan.seconds)&&in.u64(out.lifespan.nanoseconds)&&
        in.u64(out.lease.seconds)&&in.u64(out.lease.nanoseconds)&&communication::valid(out)&&out.depth<=65536;}
    inline bool schema(communication::profile selected,std::string_view codec,communication::type_plan& output)
    {
        auto const* descriptor=communication::codecs::find(codec);if(!descriptor)return false;
        communication::type_plan value;value.selected=std::move(selected);value.type_identity=descriptor->identity;value.encoding=descriptor->encoding;
        value.descriptor.assign(descriptor->schema.begin(),descriptor->schema.end());value.validate=descriptor->validate;output=std::move(value);return true;
    }
    class configuration
    {
        family kind_{};provider backend_{};communication::profile selected_;
        std::array<std::uint64_t,limit_fields.size()> limits_{};
        std::uint64_t assigned_{},required_{},definition_size_{},compiled_schema_bytes_{};
        owned_plan plan_;
        bool profiled_{},defined_{},frozen_{};
        std::uint64_t value(std::string_view key) const noexcept
        {for(std::size_t i{};i<limit_fields.size();++i)if(limit_fields[i].name==key)return limits_[i];return 0;}
        bool read_type(wire::reader& in,communication::type_plan& out)
        {communication::profile selected;std::string_view codec;return read_profile(in,selected)&&in.text(codec)&&schema(std::move(selected),codec,out);}
    public:
        /** @brief Select one exact family/provider/profile before supplying limits. */
        error profile(family kind,provider backend,communication::profile selected)
        {
            if(profiled_||defined_||frozen_)return error::conflict;
            if(static_cast<std::size_t>(kind)>=family_names.size()||!selected_profile(selected)||
                (kind==family::type?backend!=provider::canonical_schema:kind==family::event?backend!=provider::local_event_stream:backend!=provider::ros2_loopback_worker))return error::invalid;
            std::uint64_t required{};for(std::size_t i{};i<limit_fields.size();++i)if(limit_fields[i].families&mask(kind))required|=std::uint64_t{1}<<i;
            kind_=kind;backend_=backend;selected_=std::move(selected);required_=required;profiled_=true;return error::success;
        }
        /** @brief Assign one applicable finite capacity exactly once. */
        error limit(std::string_view name,std::uint64_t number) noexcept
        {
            if(!profiled_)return error::denied;
            if(defined_||frozen_)return error::conflict;
            for(std::size_t i{};i<limit_fields.size();++i)if(limit_fields[i].name==name)
            {
                auto bit=std::uint64_t{1}<<i;if(!(required_&bit)||number==UINT64_MAX)return error::invalid;if(assigned_&bit)return error::conflict;
                if(number>limit_fields[i].maximum)return error::too_large;
                limits_[i]=number;assigned_|=bit;return error::success;
            }
            return error::invalid;
        }
        /** @brief Decode a complete hashed owned definition without native effects. */
        error parse(wire::bytes input) noexcept
        {
            if(defined_||frozen_)return error::conflict;
            if(!profiled_||assigned_!=required_)return error::denied;
            if(input.size()>value("definition.bytes")||!value("definition.bytes"))return error::too_large;
            try
            {
                wire::reader in{input};std::string_view marker;communication::profile selected;
                if(!in.text(marker)||marker!=magic||!read_profile(in,selected))return error::invalid;
                if(selected!=selected_)return error::denied;
                if(buffers::hash(input.subspan(in.position()))!=selected_.digest)return error::invalid;
                std::string_view kind,backend;if(!in.text(kind)||kind!=family_names[static_cast<std::size_t>(kind_)]||!in.text(backend)||backend!=provider_names[static_cast<std::size_t>(backend_)])return error::denied;
                if(kind_!=family::type&&kind_!=family::event&&value("instances")!=1)return error::invalid;
                owned_plan parsed;std::uint64_t compiled_schema_bytes{};
                switch(kind_)
                {
                case family::context:
                {
                    communication::context_plan p;p.selected=selected_;std::string_view network;
                    if(!read_small(in,p.domain)||p.domain>232||!read_text(in,p.enclave)||!p.enclave.empty()||!in.text(network)||network!="unauthenticated-loopback")return error::invalid;
                    p.network=communication::transport::unauthenticated_loopback;parsed=std::move(p);break;
                }
                case family::node:
                {
                    communication::node_plan p;p.selected=selected_;p.max_observers=value("observers");
                    if(!read_profile(in,p.context)||!read_text(in,p.name)||!read_text(in,p.name_space)||p.name.empty()||p.name.find('/')!=p.name.npos||
                        !communication::absolute_name('/'+p.name)||(p.name_space!="/"&&!communication::absolute_name(p.name_space)))return error::invalid;
                    parsed=std::move(p);break;
                }
                case family::type:
                {
                    std::string_view codec,identity,encoding;wire::bytes descriptor;communication::type_plan p;
                    if(!in.text(codec)||!in.text(identity)||!in.text(encoding)||!in.blob(descriptor)||descriptor.size()>value("descriptor.bytes"))return error::invalid;
                    if(codec=="bounded-message")
                    {
                        if(identity.empty()||identity.size()>255||identity.find('\0')!=identity.npos||
                            (encoding!="ros1-message"&&encoding!="cdr"))return error::invalid;
                        auto status=p.validate.assign(descriptor);if(status!=bounded_message::status::ok)return static_cast<error>(status);
                        if((encoding=="cdr")!=(p.validate.bounded()->encoding()==bounded_message::layout::cdr))return error::invalid;
                        compiled_schema_bytes=p.validate.bounded()->storage_bytes();
                        p.selected=selected_;p.type_identity.assign(identity);p.encoding.assign(encoding);p.descriptor.assign(descriptor.begin(),descriptor.end());
                    }
                    else if(!schema(selected_,codec,p)||identity!=p.type_identity||encoding!=p.encoding||
                        !std::equal(descriptor.begin(),descriptor.end(),p.descriptor.begin(),p.descriptor.end()))return error::invalid;
                    parsed=std::move(p);break;
                }
                case family::topic:
                {
                    communication::topic_plan p;p.selected=selected_;std::string_view role,overflow;
                    if(!read_profile(in,p.node)||!read_profile(in,p.type)||!read_text(in,p.resolved_name)||!communication::absolute_name(p.resolved_name)||
                        !in.text(role)||(role!="publisher"&&role!="subscription")||!read_qos(in,p.quality)||!in.text(overflow)||(overflow!="reject"&&overflow!="drop-oldest"))return error::invalid;
                    p.direction=role=="publisher"?communication::role::publisher:communication::role::subscription;p.overflow_policy=overflow=="reject"?communication::overflow::reject:communication::overflow::drop_oldest;
                    p.max_message_bytes=value("message.bytes");p.queue_depth=value("queue.items");p.queue_bytes=value("queue.bytes");p.max_deadline_ns=value("deadline.ns");
                    if(!p.max_message_bytes||!p.max_deadline_ns||(p.direction==communication::role::publisher?(p.queue_depth||p.queue_bytes):(!p.queue_depth||p.queue_depth>p.queue_bytes/p.max_message_bytes)))return error::invalid;
                    parsed=std::move(p);break;
                }
                case family::service:
                {
                    communication::service_plan p;p.selected=selected_;std::string_view role;
                    if(!read_profile(in,p.node)||!read_profile(in,p.request_type)||!read_profile(in,p.response_type)||!read_text(in,p.resolved_name)||!communication::absolute_name(p.resolved_name)||
                        !read_text(in,p.service_type)||p.service_type!="std_srvs/srv/SetBool"||!in.text(role)||(role!="client"&&role!="server")||!read_qos(in,p.quality))return error::invalid;
                    p.direction=role=="client"?communication::service_role::client:communication::service_role::server;p.pending=value("pending");p.max_request_bytes=value("request.bytes");
                    p.max_response_bytes=value("response.bytes");p.reserved_bytes=value("queue.bytes");p.max_deadline_ns=value("deadline.ns");
                    if(!p.pending||!p.max_request_bytes||!p.max_response_bytes||!p.max_deadline_ns||p.pending>p.reserved_bytes/std::max(p.max_request_bytes,p.max_response_bytes))return error::invalid;
                    parsed=std::move(p);break;
                }
                case family::action:
                {
                    communication_action_dispatch::plan wrapper;auto& p=wrapper.policy;p.selected=selected_;std::string_view role;
                    if(!read_profile(in,wrapper.node_profile)||!read_type(in,p.goal)||!read_type(in,p.feedback)||!read_type(in,p.result)||!read_text(in,p.resolved_name)||
                        !communication::absolute_name(p.resolved_name)||!read_text(in,p.action_type)||p.action_type!="tf2_msgs/action/LookupTransform"||!in.text(role)||(role!="client"&&role!="server")||
                        !read_qos(in,p.goal_quality)||!read_qos(in,p.result_quality)||!read_qos(in,p.cancel_quality)||!read_qos(in,p.feedback_quality)||!read_qos(in,p.status_quality)||
                        p.goal.validate!=communication::codecs::lookup_goal::validate||p.feedback.validate!=communication::codecs::lookup_feedback::validate||p.result.validate!=communication::codecs::lookup_result::validate)return error::invalid;
                    p.direction=role=="client"?communication::actions::role::client:communication::actions::role::server;p.max_goals=value("goals");p.max_cancels_per_goal=value("cancels_per_goal");p.feedback_slots=value("feedback.items");
                    p.max_goal_bytes=value("goal.bytes");p.max_feedback_bytes=value("feedback.bytes");p.max_result_bytes=value("result.bytes");p.reserved_bytes=value("queue.bytes");p.transition_mask=value("transitions");p.max_deadline_ns=value("deadline.ns");p.retention_ns=value("retention.ns");
                    if(!p.max_goals||!p.max_goal_bytes||!p.max_result_bytes||!p.max_deadline_ns||!p.retention_ns||(p.transition_mask&~std::uint64_t{510}))return error::invalid;
                    auto maximum=std::max({p.max_goal_bytes,p.max_feedback_bytes,p.max_result_bytes,std::size_t{8}});std::uint64_t depth=std::uint64_t(p.max_goals)*(4+2*std::uint64_t(p.max_cancels_per_goal))+p.feedback_slots;
                    if(depth>UINT64_MAX/(maximum+48))return error::too_large;
                    auto needed=depth*(maximum+48);std::uint64_t goals=std::uint64_t(p.max_goals)*p.max_goal_bytes;
                    if(!add(needed,goals,needed)||!add(needed,2*maximum+48,needed)||needed>p.reserved_bytes)return error::too_large;
                    parsed=std::move(wrapper);break;
                }
                case family::graph:
                {
                    communication_graph_dispatch::plan wrapper;auto& p=wrapper.policy;p.selected=selected_;std::uint64_t count{};
                    if(!read_profile(in,wrapper.context_profile)||!read_profile(in,wrapper.node_profile)||!in.u64(count)||!count||count>65536)return error::invalid;
                    p.nodes.reserve(static_cast<std::size_t>(count));for(std::uint64_t i{};i<count;++i){::uwvm2::utils::container::std_string name;if(!read_text(in,name)||!communication::absolute_name(name)||(!p.nodes.empty()&&p.nodes.back()>=name))return error::invalid;p.nodes.push_back(std::move(name));}
                    if(!in.u64(p.fields)||!p.fields||(p.fields&~std::uint64_t{63})||!(p.fields&31))return error::invalid;
                    p.max_rows=value("rows");p.max_bytes=value("result.bytes");p.max_page_items=value("page.items");p.cursor_lifetime_ns=value("cursor.ns");p.capture_deadline_ns=value("capture.ns");
                    if(!p.max_rows||!p.max_bytes||!p.max_page_items||p.max_page_items>p.max_rows||!p.cursor_lifetime_ns||!p.capture_deadline_ns)return error::invalid;
                    parsed=std::move(wrapper);break;
                }
                case family::event:
                {
                    communication_event_dispatch::plan wrapper;auto& p=wrapper.policy;p.selected=selected_;if(!in.u64(p.fields)||!p.fields||(p.fields&~std::uint64_t{3}))return error::invalid;
                    p.max_sources=value("sources");p.max_events=value("events");p.max_deadline_ns=value("deadline.ns");wrapper.max_streams=value("streams");wrapper.max_bytes=value("result.bytes");wrapper.max_workspace=value("workspace.bytes");
                    std::uint64_t minimum{};if(!wrapper.max_streams||!p.max_deadline_ns||!wrapper.max_bytes||!events::stream::required_workspace(p.max_sources,p.max_events,minimum)||minimum>wrapper.max_workspace)return error::invalid;
                    parsed=std::move(wrapper);break;
                }
                }
                if(!in.done())return error::invalid;
                plan_=std::move(parsed);definition_size_=input.size();compiled_schema_bytes_=compiled_schema_bytes;defined_=true;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::invalid;}
        }
        /** @brief Return one complete immutable owned plan; never activate a provider. */
        error freeze(definition& out) noexcept
        {
            if(frozen_)return error::conflict;
            if(!defined_||assigned_!=required_)return error::denied;
            out={kind_,backend_,std::move(plan_)};frozen_=true;return error::success;
        }
        /** @brief Accumulate a configured profile against independent global capacities. */
        error accumulate(budget const& maximum,budget& total) const noexcept
        {
            if(!defined_)return error::denied;
            budget cost;if(!add(definition_size_,compiled_schema_bytes_,cost.definition_bytes))return error::too_large;
            switch(kind_){case family::context:cost.contexts=1;break;case family::node:cost.nodes=1;cost.observers=value("observers");break;case family::type:cost.types=1;break;
                case family::topic:cost.topics=1;cost.queue_bytes=value("queue.bytes");break;case family::service:cost.services=1;cost.queue_bytes=value("queue.bytes");break;
                case family::action:cost.actions=1;cost.queue_bytes=value("queue.bytes");break;case family::graph:cost.graphs=1;cost.queue_bytes=value("result.bytes");break;
                case family::event:cost.streams=value("streams");if(value("workspace.bytes")&&cost.streams>UINT64_MAX/value("workspace.bytes"))return error::too_large;cost.queue_bytes=cost.streams*value("workspace.bytes");break;}
            constexpr std::array fields{&budget::contexts,&budget::nodes,&budget::types,&budget::topics,&budget::services,&budget::actions,&budget::graphs,&budget::streams,&budget::queue_bytes,&budget::definition_bytes,&budget::observers};
            auto next=total;for(auto member:fields){if(next.*member>maximum.*member||cost.*member>maximum.*member-next.*member)return error::too_large;next.*member+=cost.*member;}total=next;return error::success;
        }
    };
}
