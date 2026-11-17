/** @file
 * @brief Closed, digest-bound sensor semantics and explicitly assigned finite capacities.
 * @details Definition bytes cannot select a descriptor, route, native symbol or
 * validator. Trusted registration supplies the exact schema/type/encoding and
 * bounded validation function. The definition binds source/clock incarnation,
 * frame/units, raw-buffer profile and overflow policy. Every capacity is named,
 * assigned once and required even when zero. No field is a rate allowance; this
 * parser cannot create authority, perform I/O or allocate provider storage.
 */
#pragma once
#include "sensor_dispatch.h"
namespace uwvm2::imported::ros::scene::sensors
{
    inline constexpr std::string_view definition_magic="uwvm.ros.sensor.definition";
    struct schema_registration
    {buffers::profile schema;buffers::text<96> type;buffers::text<32> encoding;bool(*validate)(bytes)noexcept{};};
    class configuration
    {
        sensor_dispatch::configuration policy_;unsigned assigned_{};bool begun_{},frozen_{};
    public:
        inline static constexpr std::array<std::string_view,11> names{
            "source.handles","source.records","frame.bytes","source.storage.bytes","history.frames","buffer.objects",
            "ingress.frame.bytes","ingress.read.bytes","ingress.retained.bytes","workspace.bytes","workspace.calls"};
        error begin(std::string_view alias,buffers::profile const& selected,bytes definition,schema_registration const& registered)noexcept
        {
            if(begun_||frozen_)return error::conflict;
            if(!identifier(alias)||!proposals::envelope_profile(selected)||definition.size()>4096||buffers::hash(definition)!=selected.sha256||!registered.validate||
                !proposals::envelope_profile(registered.schema)||!proposals::policy_text(registered.type)||!proposals::policy_text(registered.encoding))return error::invalid;
            sensor_dispatch::configuration value;wire::reader input{definition};std::string_view magic,identity,type,encoding,frame,units,buffer_alias,full;
            auto& source=value.source;auto& plan=source.ingress;auto& actual=value.actual;
            if(!input.text(magic)||magic!=definition_magic||!input.text(identity)||identity!=selected.identity.view()||!proposals::read_profile(input,plan.schema)||
                plan.schema!=registered.schema||!input.text(type)||type!=registered.type.view()||!input.text(encoding)||encoding!=registered.encoding.view()||
                !input.text(frame)||!proposals::envelope_text(frame,96)||!input.text(units)||!proposals::envelope_text(units,96)||
                !input.text(buffer_alias)||!identifier(buffer_alias)||!proposals::read_profile(input,source.buffer_profile)||
                !input.u64(actual.resource)||!input.u64(actual.generation)||!input.u64(actual.received_clock)||!input.u64(actual.received_epoch)||
                !input.u64(actual.source_clock)||!input.u64(actual.source_epoch)||!input.boolean(plan.source_sequence)||!input.boolean(plan.source_stamp)||
                !input.text(full)||(full!="reject"&&full!="drop_oldest")||!input.done()||!actual||
                plan.source_stamp!=bool(actual.source_clock)||plan.source_stamp!=bool(actual.source_epoch))return error::invalid;
            source.alias.assign(alias);source.buffer_alias.assign(buffer_alias);plan.profile=selected;plan.type=registered.type;plan.encoding=registered.encoding;
            plan.frame.assign(frame);plan.units.assign(units);plan.validate_body=registered.validate;plan.full=full=="reject"?overflow::reject:overflow::drop_oldest;
            policy_=value;begun_=true;return error::success;
        }
        error limit(std::string_view name,std::uint64_t value)noexcept
        {
            if(!begun_||frozen_)return error::denied;
            auto found=std::find(names.begin(),names.end(),name);if(found==names.end())return error::invalid;
            auto index=static_cast<unsigned>(found-names.begin());if(assigned_&(1u<<index))return error::conflict;
            constexpr std::array<std::uint64_t,11> bounds{64,64,buffers::maximum_transfer,INT64_MAX,16,UINT64_MAX-1,buffers::maximum_transfer,4096,INT64_MAX,INT64_MAX,1024};
            if(value>bounds[index])return error::too_large;
            auto& p=policy_;std::array fields{&p.source.ingress.handles,&p.source.ingress.records,&p.source.ingress.body_bytes,&p.source.ingress.storage_bytes,
                &p.source.history,&p.source.buffer_objects,&p.transport.frame_bytes,&p.transport.read_bytes,&p.transport.retained_bytes,&p.workspace_bytes,&p.workspace_calls};
            *fields[index]=value;assigned_|=1u<<index;return error::success;
        }
        error freeze(sensor_dispatch::configuration& output)noexcept
        {
            if(!begun_||frozen_)return error::denied;
            if(assigned_!=(1u<<names.size())-1)return error::invalid;
            std::uint64_t core{},native{},total{};
            if(!sensor_dispatch::required_storage(policy_,total)||!sensors::core::required_storage(policy_.source.ingress,core)||
                !ingress::required_storage(policy_.transport,native)||core>policy_.source.ingress.storage_bytes||native>policy_.transport.retained_bytes||
                !policy_.source.history||policy_.source.buffer_objects<=policy_.source.history||policy_.transport.frame_bytes>policy_.source.ingress.body_bytes)return error::invalid;
            output=policy_;frozen_=true;return error::success;
        }
    };
}
