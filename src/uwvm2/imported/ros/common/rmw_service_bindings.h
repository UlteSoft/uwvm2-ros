/** @file
 * Structural association of two ALREADY AUTHORIZED service capabilities.
 * This header confers no rights, authenticates no packet and creates no native
 * endpoint. Real Host/IPC callers must first lease both current full handles,
 * validate their actual instance/generation/session, policy/revocation/quotas,
 * and prove the call/advertise codec against complete original native metadata.
 * An event.observe grant has its OWN builtin envelope type; it does not have
 * the native service's type or QoS. A structural pair is never a native receipt.
 *
 * History is a SOURCE-RECOMPILE value seam, not stable binary ABI. Owners must
 * serialize every access. Reserve/copy all entries before native effects; rows
 * are never erased/reused, including failed activation and disconnected rows.
 */
#pragma once
#include "types.h"
#include "../ipc/event.h"
#include "../ipc/rmw_endpoint.h"
#include <cstddef>
#include <initializer_list>
#include <map>
#include <new>
#include <string>
#include <string_view>
#include <utility>

namespace uwvm2::imported::ros
{
    using rmw_service_role=ipc::rmw_endpoint::role;
    using rmw_service_owner=ipc::rmw_endpoint::binding;
    inline bool service_endpoint_role(rmw_service_role role) noexcept
    {return role==rmw_service_role::client||role==rmw_service_role::service;}
    inline bool service_endpoint_alias(std::string_view alias) noexcept
    {return ipc::valid_token_view(alias)&&alias!="."&&alias!="..";}
    /** Complete domain/session equality. source is deliberately not part of a
     * domain: matches_peer receives the independently known closing handle.
     * In particular, use the row's SAVED old native session after disconnect;
     * connection.session_id()==0 is not permission to skip local retirement.
     */
    inline bool service_endpoint_domain(rmw_service_owner const& a,rmw_service_owner const& b) noexcept
    {return a.backend==2&&b.backend==2&&a.instance&&a.generation&&a.session&&
        a.instance==b.instance&&a.generation==b.generation&&a.session==b.session;}
    /** Parse ONLY this observer's explicit mask. Error/closed are optional,
     * never silently added; service_response is mandatory. No other source
     * event kind is accepted. Failed parsing preserves output unchanged.
     */
    inline std::uint32_t service_observer_kinds(capability_spec const& observer,std::uint32_t& output) noexcept
    {
        if(observer.operation!="event.observe"||observer.type!=ipc::event_type||observer.type_identity!=ipc::event_type_identity){return 13;}
        auto value=observer.limits.find("event-kinds");if(value==observer.limits.end()){return 22;}
        std::uint32_t mask{};if(!ipc::parse_event_kinds(value->second,mask)){return 22;}
        constexpr auto response=ipc::event_bit(ipc::event_kind::service_response);
        constexpr auto allowed=response|ipc::event_bit(ipc::event_kind::resource_error)|ipc::event_bit(ipc::event_kind::resource_closed);
        if(!(mask&response)||(mask&~allowed)){return 13;}output=mask;return 0;
    }
    /** source is service.call or service.advertise; auxiliary is respectively
     * event.observe or service.respond. Values are ORIGINAL configured grants,
     * not a Guest-supplied alias-to-resource declaration. No scalar handle,
     * field match, native identity text or this return code substitutes leases.
     * This intentionally does not validate quotas/Native schemas or invent an
     * observer native type/QoS; real consumers must perform those validations.
     */
    inline std::uint32_t service_endpoint_pair(capability_spec const& source,
        capability_spec const& auxiliary,rmw_service_role role) noexcept
    {
        if(!service_endpoint_role(role)){return 22;}
        if(!service_endpoint_alias(source.alias)||!service_endpoint_alias(auxiliary.alias)||
            source.alias==auxiliary.alias||source.resource.empty()||auxiliary.resource.empty()){return 22;}
        if(source.resource!=auxiliary.resource){return 13;}
        if(role==rmw_service_role::client)
        {
            if(source.operation!="service.call"){return 13;}
            auto selected=auxiliary.limits.find("event-source-alias");
            if(selected==auxiliary.limits.end()||selected->second!=source.alias){return 13;}
            std::uint32_t mask{};return service_observer_kinds(auxiliary,mask);
        }
        if(source.operation!="service.advertise"||auxiliary.operation!="service.respond"||source.type.empty()||
            source.type_identity.empty()||source.type!=auxiliary.type||source.type_identity!=auxiliary.type_identity){return 13;}
        for(auto key:{"qos.reliability","qos.durability","qos.history","qos.depth","qos.deadline","qos.lifespan","qos.liveliness","qos.liveliness-lease-duration"})
        {
            auto left=source.limits.find(key),right=auxiliary.limits.find(key);
            if(left==source.limits.end()||right==auxiliary.limits.end()||left->second.empty()||left->second!=right->second){return 13;}
        }
        auto left=source.limits.find("adapter-codec-identity"),right=auxiliary.limits.find("adapter-codec-identity");
        if((left==source.limits.end())!=(right==auxiliary.limits.end())||
            (left!=source.limits.end()&&(left->second.empty()||left->second!=right->second))){return 13;}
        return 0;
    }
    class rmw_service_bindings
    {
    public:
        static constexpr std::size_t maximum_entries{65536};
        struct entry
        {
            rmw_service_owner owner{};rmw_service_role kind{};std::uint64_t peer{};
            std::string primary_alias,peer_alias;
            bool retired{};std::uint32_t retirement_reason{};
        };
    private:
        std::map<std::uint64_t,entry> rows_;
        std::size_t capacity_;std::uint64_t instance_{},generation_{};bool sealed_{};
    public:
        /** Invalid capacity is retained as0 and reserve refuses22. No native
         * allocation/effect or ambient authority is acquired by construction.
         */
        explicit rmw_service_bindings(std::size_t capacity=maximum_entries) noexcept:
            capacity_{capacity&&capacity<=maximum_entries?capacity:0}{}
        rmw_service_bindings(rmw_service_bindings const&)=delete;
        rmw_service_bindings& operator=(rmw_service_bindings const&)=delete;
        auto const& rows() const noexcept{return rows_;}
        std::size_t capacity() const noexcept{return capacity_;}
        bool sealed() const noexcept{return sealed_;}
        /** Parameter order is the version1 source contract. peer must be the
         * OTHER current full handle in this same actual domain/session, already
         * verified by the caller. Both aliases are copied BEFORE native work.
         * Same full source can reserve only once in the entire object lifetime.
         * Copy/map allocation failure changes neither history nor output. A
         * local nonretired reservation is not proof native creation succeeded.
         */
        std::uint32_t reserve(rmw_service_owner const& owner,rmw_service_role kind,std::uint64_t peer,
            capability_spec const& primary,capability_spec const& auxiliary,entry const*& output)
        {
            if(!capacity_||!ipc::rmw_endpoint::valid(owner)||!ipc::rmw_endpoint::full_handle(peer)||peer==owner.source){return 22;}
            auto error=service_endpoint_pair(primary,auxiliary,kind);if(error){return error;}
            if(instance_&&(instance_!=owner.instance||generation_!=owner.generation)){return 13;}
            auto old=rows_.find(owner.source);
            if(old!=rows_.end())
            {
                auto const& row=old->second;
                if(!ipc::rmw_endpoint::same(row.owner,owner)||row.kind!=kind||row.peer!=peer||
                    row.primary_alias!=primary.alias||row.peer_alias!=auxiliary.alias){return 13;}
                return row.retired||sealed_?125u:17u;
            }
            if(sealed_){return 125;}if(rows_.size()>=capacity_){return 11;}
#if defined(__cpp_exceptions)
            try
#endif
            {
                entry next{owner,kind,peer,primary.alias,auxiliary.alias,false,0};
                auto stored=rows_.emplace(owner.source,std::move(next));if(!stored.second){return 17;}
                instance_=owner.instance;generation_=owner.generation;output=&stored.first->second;return 0;
            }
#if defined(__cpp_exceptions)
            catch(std::bad_alloc const&){return 12;}catch(...){return 5;}
#endif
        }
        /** Lookup deliberately includes retired/sealed rows, so disconnect and
         * either side's close can still retain the OLD association and cleanup
         * diagnostics. No row/output is returned for a wrong owner or kind.
         */
        std::uint32_t lookup(rmw_service_owner const& owner,rmw_service_role kind,entry const*& output) const noexcept
        {
            if(!service_endpoint_role(kind)||!ipc::rmw_endpoint::valid(owner)){return 22;}
            auto found=rows_.find(owner.source);if(found==rows_.end()){return 9;}
            auto const& row=found->second;if(!ipc::rmw_endpoint::same(row.owner,owner)||row.kind!=kind){return 13;}
            output=&row;return 0;
        }
        std::uint32_t find_live(rmw_service_owner const& owner,rmw_service_role kind,entry const*& output) const noexcept
        {
            entry const* row{};auto error=lookup(owner,kind,row);if(error){return error;}
            if(row->retired||sealed_){return 125;}output=row;return 0;
        }
        /** First tombstone/reason wins; this performs NO native/RPC cleanup.
         * The actual owner must attempt its separately stored native retire
         * once, record every result, and close both raw owners independently.
         */
        std::uint32_t retire(rmw_service_owner const& owner,rmw_service_role kind,std::uint32_t reason=125) noexcept
        {
            entry const* row{};auto error=lookup(owner,kind,row);if(error){return error;}if(row->retired){return 125;}
            auto& selected=rows_.find(owner.source)->second;selected.retired=true;selected.retirement_reason=reason;return 0;
        }
        /** Either actual member handle closes its pair; revoke also covers all
         * rows using that original alias in the SAME actual domain/session.
         * closing_domain.source is not consulted: handle is separately explicit
         * (and must be full for close). revoke can have no still-live handle.
         * Caller derives instance/generation from the truly authorized closing
         * cap and session from each saved row. Never ask a disconnected client
         * for session0 and skip a previously owned row. Native cleanup separately
         * checks its old live connection; this pure match never reconnects.
         * Retired rows STILL match, enabling independent pending cleanup sweep.
         */
        static bool matches_peer(entry const& row,rmw_service_owner const& closing_domain,std::uint64_t handle,
            std::string_view alias,bool revoke) noexcept
        {
            if(!service_endpoint_role(row.kind)||!ipc::rmw_endpoint::valid(row.owner)||
                !ipc::rmw_endpoint::full_handle(row.peer)||!service_endpoint_domain(row.owner,closing_domain)||
                !service_endpoint_alias(alias)){return false;}
            if(revoke){return alias==row.primary_alias||alias==row.peer_alias;}
            return ipc::rmw_endpoint::full_handle(handle)&&
                ((handle==row.owner.source&&alias==row.primary_alias)||(handle==row.peer&&alias==row.peer_alias));
        }
        /** Mark every matching row before callers start native teardown. It is
         * safe to iterate rows() afterward: keys/storage are never erased.
         * Return only the number of NEW tombstones, not native cleanup success.
         */
        std::size_t retire_matching(rmw_service_owner const& domain,std::uint64_t handle,std::string_view alias,
            bool revoke,std::uint32_t reason=125) noexcept
        {
            std::size_t changed{};
            for(auto& item:rows_)
            {
                if(matches_peer(item.second,domain,handle,alias,revoke)&&!item.second.retired)
                {
                    item.second.retired=true;
                    item.second.retirement_reason=reason;
                    ++changed;
                }
            }
            return changed;
        }
        void seal(std::uint32_t reason=107) noexcept
        {sealed_=true;for(auto& item:rows_)if(!item.second.retired){item.second.retired=true;item.second.retirement_reason=reason;}}
    };
}
