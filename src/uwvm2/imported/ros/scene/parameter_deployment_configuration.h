/** @file
 * @brief Explicit aggregate parameter budgets and exact-node deployment profiles.
 * @details Semantic configuration and resource reservations are separate from
 * capability grants. Every aggregate field is assigned once, including zero;
 * every alias selects one immutable parameter definition and one exact existing
 * native-node grant. Freeze checks the complete graph without opening a path,
 * starting a worker or activating a parameter service. Definition reads are an
 * explicit trusted configuration operation bounded before allocation. No field
 * provides an unlimited sentinel or an ambient ROS namespace.
 */
#pragma once
#include "parameter_configuration.h"
#include "parameter_host.h"
#include "communication_deployment_configuration.h"
namespace uwvm2::imported::ros::scene
{
    class parameter_deployment_configuration
    {
    public:
        using string=::uwvm2::utils::container::std_string;
        struct entry{string node_alias;parameters::configuration setting;};
        using table=::uwvm2::utils::container::std_map<string,entry,std::less<>>;
        struct field{std::string_view name;std::uint64_t parameters::budget::*member;};
        inline static constexpr std::array fields{
            field{"scopes",&parameters::budget::scopes},field{"parameters",&parameters::budget::parameters},
            field{"value-bytes",&parameters::budget::value_bytes},field{"event-slots",&parameters::budget::event_slots},
            field{"event-bytes",&parameters::budget::event_bytes},field{"cursors",&parameters::budget::cursors},
            field{"workspace-bytes",&parameters::budget::workspace_bytes},field{"request-bytes",&parameters::budget::request_bytes},
            field{"receipt-bytes",&parameters::budget::receipt_bytes},field{"pending-bytes",&parameters::budget::pending_bytes},
            field{"cursor-bytes",&parameters::budget::cursor_bytes},field{"definition-bytes",&parameters::budget::definition_bytes}};
    private:
        parameters::budget maximum;
        std::uint64_t assigned{},bindings_{};
        bool bindings_set{};
        table entries_;
    public:
        bool empty()const noexcept{return !bindings_set&&!assigned&&entries_.empty();}
        bool contains(std::string_view alias)const noexcept{return entries_.contains(alias);}
        std::uint64_t bindings()const noexcept{return bindings_;}
        table const& entries()const noexcept{return entries_;}
        error budget_limit(std::string_view name,std::uint64_t value)noexcept
        {
            if(value>INT64_MAX||value>SIZE_MAX)return error::too_large;
            if(name=="bindings")
            {if(bindings_set)return error::conflict;if(value>128)return error::too_large;bindings_=value;bindings_set=true;return error::success;}
            for(std::size_t i{};i<fields.size();++i)if(fields[i].name==name)
            {auto bit=std::uint64_t{1}<<i;if(assigned&bit)return error::conflict;maximum.*fields[i].member=value;assigned|=bit;return error::success;}
            return error::invalid;
        }
        error profile(string alias,string node_alias,buffers::profile selected,buffers::profile node,parameter_values::algebra algebra)noexcept
        {
            if(!identifier(alias)||!identifier(node_alias))return error::invalid;
            if(!bindings_set||assigned!=((std::uint64_t{1}<<fields.size())-1))return error::denied;
            if(entries_.contains(alias))return error::conflict;
            if(entries_.size()>=bindings_)return error::too_large;
            try{entry next{std::move(node_alias),{}};auto status=next.setting.profile(selected,node,algebra);if(status!=error::success)return status;
                entries_.emplace(std::move(alias),std::move(next));return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}
        }
        /** @brief Apply one setting transactionally to an already selected alias. */
        template<class Change>error change(std::string_view alias,Change&& apply)
        {
            auto found=entries_.find(alias);if(found==entries_.end())return error::denied;
            try{auto next=found->second.setting;auto status=apply(next);if(status==error::success)found->second.setting=std::move(next);return status;}
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
        }
        /** @brief Validate immutable node profiles and every aggregate reservation without side effects. */
        error validate(communication_deployment_configuration const& communication,std::uint64_t& bytes)const
        {
            bytes=0;if(empty())return error::success;
            if(!bindings_set||assigned!=((std::uint64_t{1}<<fields.size())-1)||entries_.size()>bindings_)return error::invalid;
            parameters::budget total;
            for(auto const& [alias,value]:entries_)
            {
                (void)alias;auto setting=value.setting;parameters::plan plan;
                auto status=setting.freeze(plan,maximum,total);if(status!=error::success)return status;
                auto origin=communication.definition(value.node_alias);
                if(!origin||origin->kind!=communication_configuration::family::node)return error::denied;
                auto const& node=std::get<communication::node_plan>(origin->policy);
                if(plan.node_profile.identity.view()!=node.selected.identity||plan.node_profile.sha256!=node.selected.digest)return error::denied;
            }
            for(auto count:{total.value_bytes,total.event_bytes,total.workspace_bytes,total.request_bytes,total.receipt_bytes,
                total.pending_bytes,total.cursor_bytes,total.definition_bytes})
            {if(count>static_cast<std::uint64_t>(INT64_MAX)-bytes)return error::too_large;bytes+=count;}
            return error::success;
        }
        error bind(parameter_host& host)const
        {
            for(auto const& [alias,value]:entries_)
            {auto setting=value.setting;parameters::plan plan;auto status=setting.freeze(plan);if(status!=error::success)return status;
                status=host.bind(alias,value.node_alias,plan);if(status!=error::success)return status;}
            return host.prepare();
        }
    };
}
