/** @file
 * @brief Finite action goal, proposal and cancellation identity translation.
 * @details Every row belongs to one actual consumer and sealed worker endpoint.
 * A UUID only checks an existing mapping; it cannot confer permission or turn a
 * native handle into a host handle. Capacity is reserved before native work.
 * Unknown or contradictory effects permanently retain their row. Terminal goal
 * reclamation is a separate trusted host-ledger decision after queued delivery
 * and pending operations have drained. This table performs no I/O or admission.
 */
#pragma once
#include "communication_action_reply.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
    class action_correlations
    {
        using error=communication::error;using effect=communication::effect;using handle=communication::handle;
        using consumer=communication::delivery_consumer;using uuid=communication::actions::uuid;
        using role=communication::actions::role;using result=communication::native_result;
    public:
        struct plan
        {communication::owner authority;handle endpoint{},native_endpoint{},generation{};role direction{};std::uint64_t goals{},cancels{};};
        struct translated{handle goal{},request{},native_goal{},native_request{};};
    private:
        struct goal_row
        {consumer who;uuid identity{};handle host{},native{},proposal{},native_proposal{},pending{};bool submitting{},uncertain{};};
        struct cancel_row{handle host{},native{};bool pending{};};
        plan selected;::uwvm2::utils::container::std_vector<goal_row> goals;
        ::uwvm2::utils::container::std_vector<cancel_row> cancels;
        mutable std::mutex mutex;bool configured{},retired{};handle high_host{};
#if defined(__linux__)
        pid_t process{::getpid()};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
        goal_row* host(handle id)noexcept{for(auto& row:goals)if(id&&row.host==id)return &row;return nullptr;}
        goal_row* native(handle id)noexcept{for(auto& row:goals)if(id&&row.native==id)return &row;return nullptr;}
        goal_row* identity(uuid const& id)noexcept{for(auto& row:goals)if(row.host&&row.identity==id)return &row;return nullptr;}
        goal_row* free()noexcept{for(auto& row:goals)if(!row.host)return &row;return nullptr;}
        std::span<cancel_row> cancellations(goal_row& row)noexcept
        {if(!selected.cancels)return {};return {cancels.data()+static_cast<std::size_t>(&row-goals.data())*selected.cancels,static_cast<std::size_t>(selected.cancels)};}
        bool native_correlation_used(handle id)const noexcept
        {for(auto const& row:goals)if(row.host&&row.native_proposal==id)return true;for(auto const& row:cancels)if(row.host&&row.native==id)return true;return false;}
        bool active()const noexcept{return configured&&!retired;}
        bool valid(action_record const& record)const noexcept
        {return record.information.source==selected.native_endpoint&&record.information.generation==selected.generation&&record.information.sequence&&
            record.native_goal&&record.native_request&&std::any_of(record.identity.begin(),record.identity.end(),[](auto b){return b!=0;});}
        bool evidence(result value,handle request)const noexcept
        {return value.request_identity==request&&value.provider_generation==selected.generation&&!value.deferred&&communication::known_status(value.status)&&
            (value.status==error::ok?value.observed==effect::locally_accepted||value.observed==effect::observed_remote:value.observed==effect::none);}
        static translated view(goal_row const& row,handle request,handle native_request)noexcept
        {return {row.host,request,row.native,native_request};}
    public:
        ~action_correlations(){if(!same_process()){std::construct_at(std::addressof(goals));std::construct_at(std::addressof(cancels));}}
        /// @brief Exact row payload reservation; zero or excessive dimensions deny configuration.
        static constexpr std::uint64_t reservation_bytes(std::uint64_t goals,std::uint64_t cancels)noexcept
        {return goals&&goals<=65536&&cancels<=65536&&cancels<=1048576/goals?goals*(sizeof(goal_row)+cancels*sizeof(cancel_row)):0;}
        error configure(plan policy)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(configured||retired)return error::denied;
            if(!policy.authority||!policy.endpoint||!policy.native_endpoint||!policy.generation||policy.direction>role::server||!reservation_bytes(policy.goals,policy.cancels))return error::invalid;
            try
            {
                ::uwvm2::utils::container::std_vector<goal_row> g(policy.goals);
                ::uwvm2::utils::container::std_vector<cancel_row> c(policy.goals*policy.cancels);
                goals=std::move(g);cancels=std::move(c);selected=policy;configured=true;return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::invalid;}
        }
        /** @brief Reserve the actual host goal before admitting client submission. */
        error begin_submit(consumer who,handle goal,uuid const& id)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(!active()||selected.direction!=role::client||!who||!goal||std::none_of(id.begin(),id.end(),[](auto b){return b!=0;}))return error::denied;
            if(auto row=host(goal))return row->who==who&&row->identity==id&&row->submitting&&!row->uncertain?error::ok:error::stale;
            if(goal<=high_host||identity(id))return error::stale;
            auto row=free();if(!row)return error::again;
            *row={who,id,goal,0,goal,0,goal,true,false};high_host=goal;return error::ok;
        }
        error abandon_submit(consumer who,handle goal)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=host(goal);
            if(!row||row->who!=who||!row->submitting||row->uncertain||row->native)return error::denied;
            *row={};return error::ok;
        }
        /** @brief Apply only a complete generation-matched native goal receipt. */
        error submitted(consumer who,handle goal,result value,handle actual_native)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=host(goal);
            if(!row||row->who!=who||!row->submitting||row->uncertain)return error::denied;
            if(!evidence(value,goal)||(value.status==error::ok?(!actual_native||native(actual_native)):actual_native!=0))
            {row->uncertain=true;return error::io;}
            if(value.status!=error::ok){*row={};return error::ok;}
            row->native=row->native_proposal=actual_native;row->pending=0;row->submitting=false;return error::ok;
        }
        /** @brief Reserve both host identities before a server proposal becomes visible.
         * @details Exact retries retain the same host goal and proposal. The
         * receiving core must publish the supplied goal only after enqueueing the
         * complete retained record; its failure cannot recycle either identity.
         */
        error proposal(consumer who,action_record const& event,translated& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(!active()||selected.direction!=role::server||!who||!valid(event)||event.kind!=communication::actions::event_kind::proposal)return error::denied;
            if(auto row=native(event.native_goal))
            {
                if(row->who!=who||row->identity!=event.identity||row->native_proposal!=event.native_request||row->uncertain)return error::stale;
                output=view(*row,row->proposal,row->native_proposal);return error::ok;
            }
            if(identity(event.identity)||native_correlation_used(event.native_request))return error::stale;
            auto row=free();if(!row)return error::again;
            auto goal=resources::allocate_identity(),request=resources::allocate_identity();if(!goal||!request)return error::too_large;
            *row={who,event.identity,goal,event.native_goal,request,event.native_request,0,false,false};high_host=std::max(high_host,request);
            output=view(*row,request,event.native_request);return error::ok;
        }
        /** @brief Translate an exact server cancellation without reusing its goal handle. */
        error cancellation(consumer who,action_record const& event,translated& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=native(event.native_goal);
            if(!active()||selected.direction!=role::server||!who||!valid(event)||event.kind!=communication::actions::event_kind::cancel_request||
                !row||row->who!=who||row->identity!=event.identity||row->uncertain)return error::denied;
            cancel_row* available{};
            for(auto& entry:cancellations(*row))
            {
                if(entry.native==event.native_request){output=view(*row,entry.host,entry.native);return error::ok;}
                if(!entry.host&&!available)available=&entry;
            }
            if(native_correlation_used(event.native_request))return error::stale;
            if(!available)return error::again;
            auto request=resources::allocate_identity();if(!request)return error::too_large;
            *available={request,event.native_request,false};high_host=std::max(high_host,request);
            output=view(*row,request,event.native_request);return error::ok;
        }
        /** @brief Resolve current client event identities; UUID-only matching is forbidden. */
        error event(consumer who,action_record const& value,translated& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=native(value.native_goal);
            if(!active()||selected.direction!=role::client||!who||!valid(value)||!row||row->who!=who||row->identity!=value.identity||row->uncertain)return error::denied;
            if(value.kind==communication::actions::event_kind::proposal||value.kind==communication::actions::event_kind::cancel_request)return error::denied;
            if(value.kind==communication::actions::event_kind::cancel_response)
            {
                for(auto const& entry:cancellations(*row))if(entry.native==value.native_request&&!entry.pending)
                {output=view(*row,entry.host,entry.native);return error::ok;}
                return error::denied;
            }
            if(value.native_request!=row->native_proposal)return error::denied;
            output=view(*row,row->proposal,row->native_proposal);return error::ok;
        }
        /** @brief Pin one server response mapping before queue admission. */
        error begin_response(consumer who,handle goal,handle correlation,handle request,bool cancel_response,translated& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=host(goal);
            if(!active()||selected.direction!=role::server||!who||!row||row->who!=who||!request||row->uncertain)return error::denied;
            if(row->pending)return error::busy;
            handle target{};
            if(!cancel_response){if(correlation!=row->proposal)return error::denied;target=row->native_proposal;}
            else for(auto const& entry:cancellations(*row))if(entry.host==correlation&&!entry.pending){target=entry.native;break;}
            if(!target)return error::denied;
            row->pending=request;output=view(*row,correlation,target);return error::ok;
        }
        error responded(consumer who,handle goal,result value)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=host(goal);
            if(!row||row->who!=who||!row->pending||selected.direction!=role::server||row->uncertain)return error::denied;
            if(!evidence(value,row->pending)){row->uncertain=true;return error::io;}
            row->pending=0;return error::ok;
        }
        /** @brief Reserve an independent client cancellation receipt, never a stop claim. */
        error begin_cancel(consumer who,handle goal,handle request,translated& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=host(goal);
            if(!active()||selected.direction!=role::client||!who||!request||!row||row->who!=who||row->uncertain||!row->native)return error::denied;
            if(row->pending)return error::busy;
            if(request<=high_host)return error::stale;
            cancel_row* available{};
            for(auto& entry:cancellations(*row)){if(entry.host==request)return error::stale;if(!entry.host&&!available)available=&entry;}
            if(!available)return error::again;
            *available={request,0,true};row->pending=request;high_host=request;output=view(*row,request,0);return error::ok;
        }
        error cancelled(consumer who,handle goal,result value,handle actual_native)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=host(goal);
            if(!row||row->who!=who||!row->pending||selected.direction!=role::client||row->uncertain)return error::denied;
            cancel_row* target{};bool duplicate=actual_native&&native_correlation_used(actual_native);
            for(auto& entry:cancellations(*row))if(entry.host==row->pending&&entry.pending)target=&entry;
            if(!target||!evidence(value,row->pending)||(value.status==error::ok?(!actual_native||duplicate):actual_native!=0))
            {row->uncertain=true;return error::io;}
            if(value.status==error::ok){target->native=actual_native;target->pending=false;}else *target={};
            row->pending=0;return error::ok;
        }
        /** @brief Forget only after the host ledger proved terminal expiry and drained events.
         * @details The adapter must also exclude an in-flight native event query.
         * Unknown effects and pending mutations are never erased by reclamation.
         */
        error release_terminal(handle goal)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto row=host(goal);if(!row)return error::bad_handle;
            if(row->uncertain||row->pending||row->submitting)return error::busy;
            for(auto& entry:cancellations(*row)){if(entry.pending)return error::busy;}
            for(auto& entry:cancellations(*row))entry={};
            *row={};return error::ok;
        }
        bool occupied()const noexcept
        {
            if(!same_process())return true;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return true;
            for(auto const& row:goals)if(row.host&&(row.pending||row.submitting||row.uncertain))return true;
            return false;
        }
        void retire()noexcept{if(same_process()){std::lock_guard lock{mutex};retired=true;}}
    };
}
