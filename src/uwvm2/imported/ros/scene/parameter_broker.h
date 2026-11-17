/** @file
 * @brief Parameter transactions with explicit originating-node authority.
 * @details No ambient node, parameter service, YAML file or ROS1 namespace is
 * consulted. A trusted parameter plan fixes its key policy and node profile.
 * Open names both an already-open node capability and a node resource. The
 * communication dispatcher resolves the actual node/context grant generations;
 * guest identifiers never manufacture those owners.
 *
 * Every operation holds one authority lock for its primary parameter action
 * and the original node action. Existing-node checks use validation_only: they
 * neither create another node nor debit its creation rate. The counted
 * node_lease supplies independent observer capacity. Only the parameter
 * carrier and its measured reply are charged, after the reply fits, through
 * the store's final_admission callback. Missing rates remain unlimited inside
 * an explicit grant; explicit zero rejects admission. Capacity and rate costs
 * remain separate. Cleanup needs only the parameter close grant, so a revoked
 * parent cannot make a child scope impossible to release.
 *
 * All guest bytes have already been copied into bounded host-owned storage.
 * Large carrier allocations first reserve workspace under the same authority.
 * Callback adapters are borrowed synchronous references to a trusted live
 * communication dispatcher; the deployment must retire this object and join
 * callers before destroying that dispatcher or the shared authority.
 */
#pragma once
#include "authority.h"
#include "parameter_store.h"

namespace uwvm2::imported::ros::scene
{
    class parameter_dispatch
    {
    public:
        using operation=parameter_contract::operation;
        using bytes=wire::bytes;
        using sink=parameters::sink;
        using cancellation=parameters::cancellation;
        /** @brief Nonowning trusted node callbacks; never supplied by guest code. */
        struct node_access
        {
            void* context{};
            error (*attach)(void*,caller,handle,handle,communication::profile const&,communication::node_lease&){};
            error (*validate)(void*,caller,handle,handle,communication::profile const&,communication::owner){};
            explicit operator bool() const noexcept{return context&&attach&&validate;}
            static error convert(error value) noexcept{return value;}
            static error convert(communication::error value) noexcept
            {
                switch(value)
                {
                case communication::error::ok:return error::success;
                case communication::error::io:return error::io;
                case communication::error::bad_handle:return error::bad_handle;
                case communication::error::again:case communication::error::busy:return error::again;
                case communication::error::no_memory:return error::no_memory;
                case communication::error::denied:return error::denied;
                case communication::error::invalid:return error::invalid;
                case communication::error::too_large:return error::too_large;
                case communication::error::unsupported:return error::unsupported;
                case communication::error::timeout:return error::timeout;
                case communication::error::stale:return error::conflict;
                case communication::error::cancelled:return error::cancelled;
                }
                return error::io;
            }
            template<typename Dispatcher> static node_access from(Dispatcher& dispatcher) noexcept
            {
                return {std::addressof(dispatcher),
                    [](void* object,caller who,handle cap,handle node,communication::profile const& profile,communication::node_lease& output)
                    {return convert(static_cast<Dispatcher*>(object)->attach_node(who,cap,node,profile,output));},
                    [](void* object,caller who,handle cap,handle node,communication::profile const& profile,communication::owner origin)
                    {return convert(static_cast<Dispatcher*>(object)->validate_node(who,cap,node,profile,origin));}};
            }
        };
    private:
        std::shared_ptr<authority> permissions_;
        node_access nodes_;
        parameters::registry store_;
#if defined(__linux__)
        ::pid_t process_{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        static bool parameter_grant(grant const& value) noexcept
        {return value.kind==purpose::parameter&&value.binding=="local-parameters";}
        static communication::profile native_profile(buffers::profile const& profile)
        {return {::uwvm2::utils::container::std_string{profile.identity.view()},profile.sha256};}
        error admitted(caller who,operation action,handle cap,lease& output) const
        {
            if(!permissions_||static_cast<std::size_t>(action)>=parameter_contract::operations.size())return error::unsupported;
            auto result=permissions_->admit(who,cap,parameter_contract::operations[static_cast<std::size_t>(action)].name,0,output,false);
            return result==error::success&&!parameter_grant(output.specification())?error::denied:result;
        }
        static bool scope_prefix(operation action,bytes input,handle& scope) noexcept
        {
            auto offset=(action==operation::declare||action==operation::undeclare||action==operation::set||action==operation::set_atomic)?16u:0u;
            if(input.size()<offset)return false;
            wire::reader reader{input.subspan(offset)};return reader.u64(scope)&&scope;
        }
    public:
        explicit parameter_dispatch(std::shared_ptr<authority> permissions,node_access nodes,parameters::clock_source clock=parameters::steady_now)
            :permissions_{std::move(permissions)},nodes_{nodes},store_{clock}{}
        explicit parameter_dispatch(std::shared_ptr<authority> permissions)
            :parameter_dispatch{std::move(permissions),node_access{},parameters::steady_now}{}
        parameter_dispatch(parameter_dispatch const&)=delete;parameter_dispatch& operator=(parameter_dispatch const&)=delete;
        ~parameter_dispatch(){retire();}
        /** @brief Configure inert finite storage; never attach or construct a native node. */
        error bind(std::string_view alias,parameters::plan const& plan) noexcept
        {
            if(!same_process())return error::denied;
            try{return store_.configure(alias,plan);}
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
        }
        /** @brief Exact primary admission and bounded carrier preflight before guest copy. */
        error preflight(caller who,operation action,handle cap,std::size_t extent,bool complete=true)
        {
            if(!same_process())return error::denied;
            if(extent>4u*1024u*1024u)return error::too_large;
            lease allowed;auto result=admitted(who,action,cap,allowed);if(result!=error::success)return result;
            parameter_values::limits parser;std::uint64_t bound{};
            result=store_.parser_policy(allowed.specification().alias,parser,bound);if(result!=error::success)return result;
            if(action==operation::open)bound=std::min<std::uint64_t>(bound,160);
            if(action==operation::take)bound=std::min<std::uint64_t>(bound,8);
            if(extent>bound)return error::too_large;
            if(!complete)return error::success;
            return permissions_->preflight(who,cap,parameter_contract::operations[static_cast<std::size_t>(action)].name,extent+8,allowed);
        }
        /** @brief Reserve copied input from the same binding's finite workspace budget.
         * @details Declare this lease before the owning byte allocation so
         * storage is destroyed before its reservation is refunded.
         */
        error reserve_workspace(caller who,operation action,handle cap,std::size_t extent,parameters::workspace_lease& out,bool complete=true)
        {
            if(!same_process())return error::denied;
            auto result=preflight(who,action,cap,extent,complete);if(result!=error::success)return result;
            lease allowed;result=admitted(who,action,cap,allowed);if(result!=error::success)return result;
            return store_.reserve_workspace(allowed.specification().alias,extent,out);
        }
        /** @brief Route a complete owned canonical record under all origin leases.
         * @details The store measures output before final_admission. It also
         * rechecks revision, reservations and node lifetime after that callback.
         * A later race can charge one attempted operation and return again;
         * it never publishes a partial local mutation. Delivery failures after
         * committed mutations are recovered by their exact request keys.
         */
        error invoke(caller who,operation action,handle cap,bytes input,std::uint64_t capacity,
                     std::uint64_t& required,sink deliver,cancellation cancelled={}) noexcept
        {
            required=0;if(!same_process())return error::denied;
            try
            {
                if(!deliver)return error::invalid;
                if(capacity>4u*1024u*1024u)return error::too_large;
                auto result=preflight(who,action,cap,input.size(),capacity>=8);if(result!=error::success)return result;
                lease allowed;result=admitted(who,action,cap,allowed);if(result!=error::success)return result;
                auto const& spec=allowed.specification();parameter_values::limits parser;std::uint64_t bound{};
                result=store_.parser_policy(spec.alias,parser,bound);if(result!=error::success)return result;
                parameter_contract::request request;
                if(!parameter_contract::decode(action,input,parser,request))return error::invalid;
                buffers::profile selected,node_profile;
                result=store_.profiles(spec.alias,selected,node_profile);if(result!=error::success)return result;
                parameters::dependency dependency;
                if(action==operation::open)
                {
                    if(!request.node_capability||!request.node||request.profile!=selected)return error::denied;
                    required=8;if(capacity<required)return error::too_large;
                    dependency.capability=request.node_capability;dependency.node=request.node;
                }
                else
                {
                    handle scope{};if(!scope_prefix(action,input,scope)||scope!=request.scope)return error::invalid;
                    result=store_.dependencies(spec.alias,scope,dependency);if(result!=error::success)return result;
                }
                if(!nodes_)return error::unsupported;
                std::array claims{admission_claim{cap,parameter_contract::operations[static_cast<std::size_t>(action)].name,0},
                    admission_claim{dependency.capability,"node.open",0,false,true}};
                group_lease sources;result=permissions_->admit_group(who,claims,sources,false);if(result!=error::success)return result;
                auto node_grant=sources.specification(1);if(!node_grant||node_grant->kind!=purpose::node)return error::denied;
                auto native=native_profile(node_profile);communication::node_lease attachment;
                if(action==operation::open)
                {
                    result=nodes_.attach(nodes_.context,who,dependency.capability,dependency.node,native,attachment);
                    if(result!=error::success)return result;
                    if(!attachment||!attachment.live()||attachment.identity()!=dependency.node||!attachment.authority())return error::denied;
                    dependency.authority=attachment.authority();
                }
                else
                {
                    result=nodes_.validate(nodes_.context,who,dependency.capability,dependency.node,native,dependency.authority);
                    if(result!=error::success)return result;
                }
                auto revalidate=[&]()
                {
                    group_lease current;auto status=permissions_->admit_group(who,claims,current,false);if(status!=error::success)return status;
                    return nodes_.validate(nodes_.context,who,dependency.capability,dependency.node,native,dependency.authority);
                };
                auto final=[&](std::uint64_t bytes)
                {
                    auto status=revalidate();if(status!=error::success)return status;
                    if(bytes>UINT64_MAX-input.size())return error::too_large;
                    lease charged;return permissions_->admit(who,cap,claims[0].action,input.size()+bytes,charged);
                };
                auto copy=[&](wire::bytes output)->bool
                {
                    if(cancelled&&cancelled())return false;
                    if(revalidate()!=error::success)return false;
                    if(!deliver(output))return false;
                    return (!cancelled||!cancelled())&&revalidate()==error::success;
                };
                return store_.invoke(action,spec.alias,input,spec.max_resources,capacity,copy,cancelled,final,required,std::move(attachment));
            }
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
            catch(...){return error::io;}
        }
        /** @brief Close an exact owned scope with primary cleanup authority only. */
        error close(caller who,handle cap,handle scope) noexcept
        {
            if(!same_process())return error::denied;
            try
            {
                if(!permissions_)return error::denied;
                lease allowed;
                auto result=permissions_->admit(who,cap,"resource.close",16,allowed);if(result!=error::success)return result;
                if(!parameter_grant(allowed.specification()))return error::denied;
                return store_.close(allowed.specification().alias,scope);
            }
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(...){return error::io;}
        }
        /** @brief Common request operations read only the exact origin ledger.
         * @details No node attachment or parent capability is acquired here.
         * A released request no longer has a public description, while its
         * bounded last-key proof survives to recover the original mutation.
         */
        error invoke_request(caller who,parameters::request_operation action,handle cap,bytes input,std::uint64_t capacity,
                             std::uint64_t& required,sink deliver,cancellation cancelled={}) noexcept
        {
            required=0;if(!same_process())return error::denied;
            try
            {
                if(!permissions_||!deliver)return error::denied;
                if(input.size()!=8||capacity>4u*1024u*1024u)return error::invalid;
                auto name=action==parameters::request_operation::describe?"request.describe":
                    action==parameters::request_operation::release?"request.release":nullptr;
                if(!name)return error::invalid;
                lease origin;auto status=permissions_->admit(who,cap,name,0,origin,false);if(status!=error::success)return status;
                auto const& grant=origin.specification();if(!parameter_grant(grant))return error::denied;
                wire::reader reader{input};handle id{};if(!reader.u64(id)||!id||!reader.done())return error::invalid;
                auto admit=[&](std::uint64_t size)
                {lease current;return permissions_->admit(who,cap,name,input.size()+size,current);};
                auto copy=[&](wire::bytes output)
                {
                    lease current;if(permissions_->admit(who,cap,name,0,current,false)!=error::success||(cancelled&&cancelled()))return false;
                    if(!deliver(output))return false;
                    return (!cancelled||!cancelled())&&permissions_->admit(who,cap,name,0,current,false)==error::success;
                };
                return store_.request_invoke(action,grant.alias,id,capacity,copy,cancelled,admit,required);
            }
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
            catch(...){return error::io;}
        }
        /** @brief Called by trusted broker revocation while authority is already fenced. */
        void revoke(std::string_view alias){if(same_process())store_.revoke(alias);}
        void retire(){if(same_process())store_.retire();}
        bool try_inspect(std::string_view alias,parameters::usage& output) const noexcept
        {return same_process()&&store_.try_inspect(alias,output);}
    };
}
