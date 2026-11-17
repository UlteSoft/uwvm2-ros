/** @file
 * @brief Exact-node routing for bounded local ROS1/ROS2 parameter transactions.
 * @details Each configured parameter alias owns a separate store and names one
 * existing communication node grant. A matching profile in another worker does
 * not satisfy that dependency. Current node/context authority is revalidated
 * before each effect and delivery; no node or remote parameter service is
 * created implicitly. Local transactional semantics include retained outcomes,
 * deferred validation and explicit ROS1 search boundaries. They do not mirror
 * changes into a native node's parameter service.
 *
 * Configuration is single-threaded and sealed before guest access. Bindings,
 * value storage, events, cursors, receipts and workspaces have independent finite
 * reservations. All owning allocations use recoverable fast_io storage. This
 * class starts no thread, accepts no guest path and grants no authority.
 */
#pragma once
#include "parameter_broker.h"
#include "communication_host.h"
#include <uwvm2/utils/container/unique.h>
namespace uwvm2::imported::ros::scene
{
    class parameter_host
    {
        using string=::uwvm2::utils::container::std_string;
        struct entry
        {
            std::shared_ptr<authority> permissions;
            communication_host& nodes;
            string node_alias;
            parameter_dispatch dispatch;
            error check(caller who,handle cap)const
            {
                lease granted;auto status=permissions->admit(who,cap,"node.open",0,granted,false);
                if(status!=error::success)return status;
                auto const& value=granted.specification();
                return value.alias==node_alias&&value.kind==purpose::node&&value.binding=="native-communication"?error::success:error::denied;
            }
            entry(std::shared_ptr<authority> owner,communication_host& host,string alias)
                :permissions(std::move(owner)),nodes(host),node_alias(std::move(alias)),dispatch(permissions,
                    parameter_dispatch::node_access{this,
                    [](void* context,caller who,handle cap,handle node,communication::profile const& profile,communication::node_lease& out)
                    {auto& self=*static_cast<entry*>(context);auto status=self.check(who,cap);
                        return status==error::success?self.nodes.attach_node(who,cap,node,profile,out):status;},
                    [](void* context,caller who,handle cap,handle node,communication::profile const& profile,communication::owner owner)
                    {auto& self=*static_cast<entry*>(context);auto status=self.check(who,cap);
                        return status==error::success?self.nodes.validate_node(who,cap,node,profile,owner):status;}}){}
        };
        std::shared_ptr<authority> permissions;
        communication_host& nodes;
        ::uwvm2::utils::container::std_map<string,::uwvm2::utils::container::std_unique_ptr<entry>,std::less<>> entries;
        std::uint64_t maximum{};
        std::atomic<bool> ready{},retired{};
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
        error select(caller who,handle cap,std::string_view action,entry*& output)
        {
            if(!same_process()||!ready.load(std::memory_order_acquire)||retired.load())return error::denied;
            lease actual;auto status=permissions->admit(who,cap,action,0,actual,false);if(status!=error::success)return status;
            auto const& grant=actual.specification();
            if(grant.kind!=purpose::parameter||grant.binding!="local-parameters")return error::denied;
            auto found=entries.find(grant.alias);if(found==entries.end())return error::denied;
            output=found->second.get();return error::success;
        }
    public:
        using operation=parameter_contract::operation;
        using sink=parameters::sink;
        using cancellation=parameters::cancellation;
        parameter_host(std::shared_ptr<authority> actual,communication_host& native,std::uint64_t bindings)
            :permissions(std::move(actual)),nodes(native),maximum(bindings){}
        parameter_host(parameter_host const&)=delete;parameter_host& operator=(parameter_host const&)=delete;
        ~parameter_host(){retire();}
        /** @brief Bind one finite store to an exact node grant before publication. */
        error bind(string alias,string node_alias,parameters::plan const& policy)noexcept
        {
            if(!same_process()||ready.load()||retired.load())return error::denied;
            if(!identifier(alias)||!identifier(node_alias)||!maximum||maximum>128)return error::invalid;
            if(entries.contains(alias))return error::conflict;
            if(entries.size()>=maximum)return error::too_large;
            try
            {
                auto value=::uwvm2::utils::container::make_unique<entry>(permissions,nodes,std::move(node_alias));
                auto status=value->dispatch.bind(alias,policy);if(status!=error::success)return status;
                entries.emplace(std::move(alias),std::move(value));return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        error prepare()noexcept
        {if(!same_process()||retired.load()||ready.load()||entries.empty())return error::denied;ready.store(true,std::memory_order_release);return error::success;}
        error preflight(caller who,operation action,handle cap,std::size_t extent,bool fits=true)
        {
            auto index=static_cast<std::size_t>(action);if(index>=parameter_contract::operations.size())return error::unsupported;
            entry* selected{};auto status=select(who,cap,parameter_contract::operations[index].name,selected);
            return status==error::success?selected->dispatch.preflight(who,action,cap,extent,fits):status;
        }
        error reserve_workspace(caller who,operation action,handle cap,std::size_t extent,parameters::workspace_lease& held,bool complete=true)
        {
            auto status=preflight(who,action,cap,extent,complete);if(status!=error::success)return status;
            entry* selected{};status=select(who,cap,parameter_contract::operations[static_cast<std::size_t>(action)].name,selected);
            return status==error::success?selected->dispatch.reserve_workspace(who,action,cap,extent,held,complete):status;
        }
        error invoke(caller who,operation action,handle cap,wire::bytes input,std::uint64_t capacity,std::uint64_t& required,sink deliver,cancellation cancelled={})
        {
            required=0;auto index=static_cast<std::size_t>(action);if(index>=parameter_contract::operations.size())return error::unsupported;
            entry* selected{};auto status=select(who,cap,parameter_contract::operations[index].name,selected);
            return status==error::success?selected->dispatch.invoke(who,action,cap,input,capacity,required,deliver,cancelled):status;
        }
        error preflight_request(caller who,bool release,handle cap,std::size_t extent,bool fits=true)
        {
            if(extent!=8)return error::invalid;
            auto name=release?"request.release":"request.describe";entry* selected{};auto status=select(who,cap,name,selected);
            if(status!=error::success||!fits)return status;
            lease allowed;return permissions->preflight(who,cap,name,extent,allowed);
        }
        error invoke_request(caller who,bool release,handle cap,wire::bytes input,std::uint64_t capacity,std::uint64_t& required,sink deliver,cancellation cancelled={})
        {
            required=0;entry* selected{};auto status=select(who,cap,release?"request.release":"request.describe",selected);
            return status==error::success?selected->dispatch.invoke_request(who,release?parameters::request_operation::release:parameters::request_operation::describe,
                cap,input,capacity,required,deliver,cancelled):status;
        }
        error close(caller who,handle cap,handle scope)
        {entry* selected{};auto status=select(who,cap,"resource.close",selected);return status==error::success?selected->dispatch.close(who,cap,scope):status;}
        bool try_inspect(std::string_view alias,parameters::usage& output)const noexcept
        {if(!same_process())return false;auto found=entries.find(alias);return found!=entries.end()&&found->second->dispatch.try_inspect(alias,output);}
        /// @brief Fence an exact local store; original node revocation is independently checked on access.
        void revoke(std::string_view alias)noexcept
        {if(same_process()){auto found=entries.find(alias);if(found!=entries.end())found->second->dispatch.revoke(alias);}}
        void retire()noexcept
        {if(same_process()){retired.store(true);for(auto const& [alias,value]:entries){(void)alias;value->dispatch.retire();}}}
    };
}
