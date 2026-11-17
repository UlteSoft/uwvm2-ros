/** @file
 * @brief Explicit named configuration for private logs and cached diagnostics.
 * @details Parsers perform no I/O and create no provider, capability or native
 * appender. Every named capacity must be supplied once, including intentional
 * zeroes. Logger names are an exact finite allowlist. Loss mode is explicit;
 * reject is stable finite-capacity admission and drop_oldest is attenuation.
 * Freeze validates complete plans before any store allocation. Optional rates
 * remain independent authority settings, never inferred from these capacities.
 */
#pragma once
#include "log_diagnostic_dispatch.h"
namespace uwvm2::imported::ros::scene
{
    /** @brief Six finite environment reservations; incomplete input stays unusable. */
    class log_diagnostic_budget_configuration
    {
        log_diagnostic_dispatch::limits capacity{};std::uint64_t assigned{};bool frozen{};
    public:
        inline static constexpr std::array<std::string_view,6> names{
            "logs","diagnostics","storage_bytes","request_bytes","workspace_bytes","workspace_calls"};
        error limit(std::string_view name,std::uint64_t value) noexcept
        {
            if(frozen)return error::conflict;
            auto found=std::find(names.begin(),names.end(),name);if(found==names.end()||value==UINT64_MAX)return error::invalid;
            auto index=static_cast<std::size_t>(found-names.begin());auto bit=std::uint64_t{1}<<index;if(assigned&bit)return error::conflict;
            constexpr std::array<std::uint64_t,6> maxima{4096,4096,INT64_MAX,buffers::maximum_transfer,INT64_MAX,INT64_MAX};
            if(value>maxima[index])return error::too_large;
            std::array fields{&capacity.logs,&capacity.diagnostics,&capacity.storage_bytes,&capacity.request_bytes,&capacity.workspace_bytes,&capacity.workspace_calls};
            *fields[index]=value;assigned|=bit;return error::success;
        }
        /** @brief Validate the complete reservation without granting authority or allocating. */
        error freeze(log_diagnostic_dispatch::limits& output) noexcept
        {
            if(assigned!=63)return error::denied;
            if((capacity.logs||capacity.diagnostics)&&(!capacity.storage_bytes||capacity.request_bytes<log_diagnostic_contract::maximum_read_request||
               !capacity.workspace_bytes||!capacity.workspace_calls))return error::invalid;
            output=capacity;frozen=true;return error::success;
        }
    };
}
namespace uwvm2::imported::ros::scene::logs
{
    class configuration
    {
        plan selected{};std::uint64_t assigned{};bool loss_set{},frozen{};
    public:
        inline static constexpr std::array<std::string_view,4> limit_names{"records","record_bytes","storage_bytes","severity_mask"};
        /** @brief Set each explicit finite capacity/closed severity mask once. */
        error limit(std::string_view name,std::uint64_t value) noexcept
        {
            if(frozen)return error::conflict;
            auto found=std::find(limit_names.begin(),limit_names.end(),name);if(found==limit_names.end()||value==UINT64_MAX)return error::invalid;
            auto index=static_cast<std::size_t>(found-limit_names.begin());auto bit=std::uint64_t{1}<<index;if(assigned&bit)return error::conflict;
            constexpr std::array<std::uint64_t,4> maxima{65536,buffers::maximum_transfer,INT64_MAX,31};if(value>maxima[index])return error::too_large;
            std::array fields{&selected.records,&selected.record_bytes,&selected.storage_bytes,&selected.severity_mask};
            *fields[index]=value;assigned|=bit;return error::success;
        }
        /** @brief Add an exact logger identity; no prefix or wildcard grants exist. */
        error logger(std::string_view name) noexcept
        {
            if(frozen)return error::conflict;
            if(!valid_text(name,maximum_name,false))return error::invalid;
            for(std::size_t i{};i<selected.logger_count;++i)if(selected.loggers[i].view()==name)return error::conflict;
            if(selected.logger_count==maximum_loggers)return error::too_large;
            if(!selected.loggers[selected.logger_count].assign(name))return error::invalid;
            ++selected.logger_count;return error::success;
        }
        /** @brief Explicitly choose full-queue refusal or acknowledged loss after authorization. */
        error loss(std::string_view mode) noexcept
        {
            if(frozen||loss_set)return error::conflict;
            if(mode=="reject")selected.full=overflow::reject;
            else if(mode=="drop-oldest")selected.full=overflow::drop_oldest;
            else return error::invalid;
            loss_set=true;return error::success;
        }
        /** @brief Freeze a usable complete record shape and its full fixed allocation.
         * @details Every approved logger/severity must at least fit a record with
         * an empty message and no optional fields. Longer content remains bounded
         * by the complete encoded record ceiling and is never silently truncated.
         */
        error freeze(plan& output) noexcept
        {
            if(assigned!=15||!loss_set)return error::denied;
            std::uint64_t needed{};if(!store::required_storage(selected,needed))return error::invalid;
            if(needed>selected.storage_bytes)return error::too_large;
            for(std::size_t i{};i<selected.logger_count;++i)for(std::size_t level{};level<severity_names.size();++level)
                if(selected.severity_mask&(std::uint64_t{1}<<level))
                {
                    request value;value.session=value.sequence=1;value.level=static_cast<severity>(level);value.logger=selected.loggers[i].view();
                    auto writer=wire::writer::measuring(static_cast<std::size_t>(selected.record_bytes));if(!encode(writer,value))return error::too_large;
                }
            output=selected;frozen=true;return error::success;
        }
    };
}
namespace uwvm2::imported::ros::scene::diagnostics
{
    /** @brief Immutable local-log diagnostic source and closed counter/page limits. */
    class configuration
    {
        plan selected{};logs::name origin{};std::uint64_t assigned{};bool source_set{},frozen{};
    public:
        inline static constexpr std::array<std::string_view,7> limit_names{
            "scope","fields","rows","page_items","storage_bytes","lifetime_ns","capture_ns"};
        error source(std::string_view alias) noexcept
        {
            if(frozen||source_set)return error::conflict;
            if(!identifier(alias)||!origin.assign(alias))return error::invalid;
            source_set=true;return error::success;
        }
        /** @brief Configure one closed counter mask and finite lifetime/capacity value. */
        error limit(std::string_view name,std::uint64_t value) noexcept
        {
            if(frozen)return error::conflict;
            auto found=std::find(limit_names.begin(),limit_names.end(),name);if(found==limit_names.end()||value==UINT64_MAX)return error::invalid;
            auto index=static_cast<std::size_t>(found-limit_names.begin());auto bit=std::uint64_t{1}<<index;if(assigned&bit)return error::conflict;
            constexpr std::array<std::uint64_t,7> maxima{INT64_MAX,2047,64,64,INT64_MAX,INT64_MAX,INT64_MAX};if(value>maxima[index])return error::too_large;
            std::array fields{&selected.scope,&selected.fields,&selected.rows,&selected.page_items,&selected.storage_bytes,&selected.lifetime_ns,&selected.capture_ns};
            *fields[index]=value;assigned|=bit;return error::success;
        }
        /** @brief Publish an owned plan and alias only after every field validates. */
        error freeze(plan& output,logs::name& source_alias) noexcept
        {
            if(assigned!=127||!source_set)return error::denied;
            std::uint64_t needed{};if(!snapshot::required_storage(selected,needed)||std::popcount(selected.fields)>static_cast<int>(selected.rows))return error::invalid;
            if(needed>selected.storage_bytes)return error::too_large;
            output=selected;source_alias=origin;frozen=true;return error::success;
        }
    };
}
