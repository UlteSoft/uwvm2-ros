/** @file
 * @brief Digest-bound device/control definitions and explicit finite startup resources.
 * @details Definitions contain closed SI fields, named profiles and directional
 * source aliases. Descriptors are supplied separately by trusted CLI policy and
 * are never accepted from a Wasm program. Parsing/freeze performs no descriptor
 * inspection, allocation of a provider, native I/O, thread creation or grant
 * elevation. The activation stage checks actual endpoint identity and raw mode.
 */
#pragma once
#include "device_dispatch.h"

namespace uwvm2::imported::ros::scene
{
    class device_configuration
    {
        device_dispatch::device_plan policy_;std::array<int,4> endpoints_{};
        std::uint64_t assigned_{};bool selected_{},defined_{},endpoints_set_{};
    public:
        inline static constexpr std::string_view magic="uwvm.ros.device.scalar";
        inline static constexpr std::array<std::string_view,14> names{"handles","requests","command.bytes","observations","observations.minimum",
            "observation.age.ns","command.deadline.ns","ledger.bytes","retained.bytes","transport.write.bytes","stop.ack.ns",
            "controller.lease.ns","controller.prepare.ns","controller.priority"};
        /** @brief Select exact deployment content identity once; no ambient name fallback. */
        error profile(std::string_view alias,buffers::profile selected)noexcept
        {
            if(selected_)return error::conflict;
            if(!identifier(alias)||!proposals::envelope_profile(selected))return error::invalid;
            policy_.alias.assign(alias);policy_.ledger.profile=selected;selected_=true;return error::success;
        }
        bool definition_pending()const noexcept{return selected_&&!defined_;}
        /** @brief Authenticate the complete scalar schema and fixed stop/watchdog semantics.
         * @details Field names, SI units and finite inclusive min/max bounds are
         * part of the profile hash. Duplicate fields, unknown units, nonfinite
         * bounds, trailing data and absent frame identity are refused.
         */
        error definition(wire::bytes bytes)noexcept
        {
            if(!selected_)return error::denied;
            if(defined_)return error::conflict;
            if(bytes.size()>16384||buffers::hash(bytes)!=policy_.ledger.profile.sha256)return error::invalid;
            auto value=policy_;wire::reader in{bytes};std::string_view kind,identity,frame;std::uint64_t count{};
            if(!in.text(kind)||kind!=magic||!in.text(identity)||identity!=value.ledger.profile.identity.view()||
                !proposals::read_profile(in,value.mode)||!proposals::read_profile(in,value.watchdog)||!proposals::read_profile(in,value.ledger.stop_profile)||
                !proposals::read_profile(in,value.ledger.schema.profile)||!in.text(frame)||!value.ledger.schema.frame.assign(frame)||
                !in.u64(count)||!count||count>devices::maximum_fields)return error::invalid;
            auto& schema=value.ledger.schema;schema.count=static_cast<std::size_t>(count);
            for(std::size_t i{};i<schema.count;++i)
            {
                std::string_view name,quantity;auto& field=schema.fields[i];
                if(!in.text(name)||!field.name.assign(name)||!in.text(quantity)||!in.f64(field.minimum)||!in.f64(field.maximum))return error::invalid;
                auto unit=std::find(devices::unit_names.begin(),devices::unit_names.end(),quantity);if(unit==devices::unit_names.end())return error::invalid;
                field.quantity=static_cast<devices::unit>(unit-devices::unit_names.begin());
            }
            if(!in.done()||!devices::valid(schema))return error::invalid;
            policy_=value;defined_=true;return error::success;
        }
        /** @brief Retain four distinct startup descriptor numbers without touching them. */
        error endpoints(std::array<std::uint64_t,4> values)noexcept
        {
            if(!selected_)return error::denied;
            if(endpoints_set_)return error::conflict;
            for(std::size_t i{};i<values.size();++i)
            {if(values[i]<3||values[i]>INT32_MAX)return error::invalid;for(std::size_t j{};j<i;++j)if(values[i]==values[j])return error::conflict;}
            for(std::size_t i{};i<values.size();++i){endpoints_[i]=static_cast<int>(values[i]);}
            endpoints_set_=true;return error::success;
        }
        /** @brief Assign each capacity exactly once; zero is never expanded into an unlimited sentinel. */
        error limit(std::string_view name,std::uint64_t value)noexcept
        {
            if(!defined_)return error::denied;
            auto found=std::find(names.begin(),names.end(),name);if(found==names.end())return error::invalid;
            auto index=static_cast<unsigned>(found-names.begin());if(assigned_&(UINT64_C(1)<<index))return error::conflict;
            constexpr std::array<std::uint64_t,14> maxima{16,16,8192,8,8,INT64_MAX,INT64_MAX,INT64_MAX,INT64_MAX,4096,INT64_MAX,INT64_MAX,INT64_MAX,UINT32_MAX};
            if(value>maxima[index])return error::too_large;
            auto& p=policy_;
            if(index==9)p.transport.maximum_write=static_cast<std::size_t>(value);
            else
            {
                std::array fields{&p.ledger.handles,&p.ledger.requests,&p.ledger.body_bytes,&p.ledger.observations,&p.ledger.minimum_observations,
                    &p.ledger.maximum_age_ns,&p.ledger.maximum_deadline_ns,&p.ledger.storage_bytes,&p.retained_bytes,
                    &p.transport.stop_ack_ns,&p.transport.stop_ack_ns,&p.arbitration.maximum_lease_ns,&p.arbitration.prepare_ns,&p.arbitration.maximum_priority};
                *fields[index]=value;
            }
            assigned_|=UINT64_C(1)<<index;return error::success;
        }
        /** @brief Validate complete reservations; actual OS endpoint support is checked only during activation. */
        error freeze(device_dispatch::device_plan& output,std::array<int,4>& endpoints)const noexcept
        {
            if(!defined_||!endpoints_set_||assigned_!=(UINT64_C(1)<<names.size())-1)return error::invalid;
            std::uint64_t required{};if(!device_dispatch::required_storage(policy_,required)||required>policy_.retained_bytes)return error::invalid;
            output=policy_;endpoints=endpoints_;return error::success;
        }
    };
    class control_configuration
    {
        device_dispatch::control_plan policy_;unsigned assigned_{};bool selected_{},defined_{};
    public:
        /** @brief Read the cached common device alias without resolving authority or starting work. */
        std::string_view device_alias()const noexcept{return policy_.device_alias.view();}
        inline static constexpr std::string_view magic="uwvm.ros.control.scalar";
        inline static constexpr std::array<std::string_view,4> names{"lease.ns","priority","write.window.ns","retained.bytes"};
        error profile(std::string_view alias,buffers::profile selected)noexcept
        {
            if(selected_)return error::conflict;
            if(!identifier(alias)||!proposals::envelope_profile(selected))return error::invalid;
            policy_.alias.assign(alias);policy_.selected=selected;selected_=true;return error::success;
        }
        bool definition_pending()const noexcept{return selected_&&!defined_;}
        /** @brief Authenticate exact device and ordered sensor/buffer source rules without creating capabilities. */
        error definition(wire::bytes bytes)noexcept
        {
            if(!selected_)return error::denied;
            if(defined_)return error::conflict;
            if(bytes.size()>16384||buffers::hash(bytes)!=policy_.selected.sha256)return error::invalid;
            auto value=policy_;wire::reader in{bytes};std::string_view kind,identity,device;std::uint64_t count{};
            if(!in.text(kind)||kind!=magic||!in.text(identity)||identity!=value.selected.identity.view()||!in.text(device)||!identifier(device)||
                !proposals::read_profile(in,value.stop)||!in.u64(count)||!count||count>value.sources.size())return error::invalid;
            value.device_alias.assign(device);value.source_count=static_cast<std::size_t>(count);
            for(std::size_t i{};i<value.source_count;++i)
            {
                auto& source=value.sources[i];std::string_view alias,buffer,frame;
                if(!in.text(alias)||!identifier(alias)||!proposals::read_profile(in,source.profile)||!in.text(buffer)||!identifier(buffer)||
                    !proposals::read_profile(in,source.buffer_profile)||!in.text(frame)||!proposals::envelope_text(frame,96))return error::invalid;
                source.alias.assign(alias);source.buffer_alias.assign(buffer);source.frame.assign(frame);
                for(std::size_t j{};j<i;++j)if(value.sources[j].alias==source.alias)return error::invalid;
            }
            if(!in.done())return error::invalid;
            policy_=value;defined_=true;return error::success;
        }
        error limit(std::string_view name,std::uint64_t value)noexcept
        {
            if(!defined_)return error::denied;
            auto found=std::find(names.begin(),names.end(),name);if(found==names.end())return error::invalid;
            auto index=static_cast<unsigned>(found-names.begin());if(assigned_&(1u<<index))return error::conflict;
            if(value>INT64_MAX||(index==1&&value>UINT32_MAX))return error::too_large;
            std::array fields{&policy_.lease_ns,&policy_.priority,&policy_.write_window_ns,&policy_.retained_bytes};*fields[index]=value;assigned_|=1u<<index;return error::success;
        }
        error freeze(device_dispatch::control_plan& output)const noexcept
        {
            if(!defined_||assigned_!=(1u<<names.size())-1)return error::invalid;
            std::uint64_t required{};if(!device_dispatch::required_storage(policy_,required)||required>policy_.retained_bytes)return error::invalid;
            output=policy_;return error::success;
        }
    };
}
