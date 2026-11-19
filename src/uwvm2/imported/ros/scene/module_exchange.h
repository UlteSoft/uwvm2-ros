/** @file
 * @brief Strictly registered, bounded, copied messages between Wasm principals.
 * @details Only trusted deployment can add a route. The route fixes both module
 * names, operation, request/response schema identities, byte ceilings, slots
 * and deadline. A guest cannot supply an export name, address, callback or new
 * route. The VM authenticates the immediate caller separately from its grant.
 * Payloads are bounded opaque records: application schema interpretation stays
 * in the receiving Wasm sandbox; the host never executes a decoder or native
 * function supplied by either endpoint.
 *
 * Requests and responses have separate, preallocated storage. Each slot starts fully zeroed. Consumption
 * overwrites every byte written to that buffer. The unused suffix stays zero. A delivered guest exchange region
 * is registered by trusted runtime glue and retained until respond/acknowledge.
 * Cancellation, observed expiry, revocation and retirement erase both sides.
 * A failed erasure quarantines the slot; it cannot be reported complete or
 * reused. Expiry is checked on every access and trusted sweep, not represented
 * as a hard real-time timer. Independent private guest copies are outside this
 * exchange ownership contract. No raw guest pointer is retained.
 */
#pragma once
#include "module_exchange_contract.h"
#ifndef UWVM_SCENE_MODULE_EXCHANGE_MACROS_ONLY
#include "authority.h"
#include "erased_buffer.h"
#include "module_buffer_audit.h"
#include "wire.h"
#include <uwvm2/utils/container/unique.h>
#include <cstring>
namespace uwvm2::imported::ros::scene::module_exchange
{
    inline constexpr std::uint64_t maximum_bytes=4u*1024u*1024u,maximum_slots=1024;
    /** @brief VM-sealed erasure descriptor. No guest can install the eraser.
     * @details Identity is compared with the runtime epoch, never dereferenced.
     * The runtime resolves logical memory afresh under its lifetime gate. It
     * must use concurrent-safe memory access for remotely retained regions.
     */
    struct region
    {
        caller owner{};std::uint64_t memory{},offset{},size{};
        bool (*erase)(caller,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t)noexcept{};
        std::uint64_t length_offset{},length_size{};
        bool clear()noexcept
        {
            if(!erase)return true;
            // One owner/backend pin covers both registered extents. The eraser
            // must attempt each extent even when the other is invalid.
            if(!erase(owner,memory,offset,size,length_offset,length_size))return false;
            *this={};return true;
        }
        bool matches(caller value)const noexcept
        {return erase&&owner.instance==value.instance&&owner.generation==value.generation;}
        /** @brief Compare normalized spans without pointer arithmetic or overflow. */
        bool overlaps(region const& other)const noexcept
        {
            if(!erase||!other.erase||!other.matches(owner)||memory!=other.memory)return false;
            auto intersects=[](std::uint64_t a,std::uint64_t n,std::uint64_t b,std::uint64_t m)noexcept
            {return n&&m&&(a<=b?b-a<n:a-b<m);};
            return intersects(offset,size,other.offset,other.size)||intersects(offset,size,other.length_offset,other.length_size)||
                intersects(length_offset,length_size,other.offset,other.size)||intersects(length_offset,length_size,other.length_offset,other.length_size);
        }
    };
    struct plan
    {
        ::uwvm2::utils::container::std_string route,client,server,semantic,request_schema,response_schema;
        std::uint64_t request_bytes{},response_bytes{},slots{},deadline_ns{};
        /** @brief Default-on input-carrier erasure; false retains sender bytes.
         * @details Trusted immutable route configuration only. Response-region
         * and broker-storage erasure, quarantine and authority are unaffected.
         */
        bool clear_after_copy{true};
        // Host deployment subscription only. An empty mask takes the ordinary
        // path without touching the process-wide diagnostic writer.
        std::uint8_t capture_stages{};
        ::uwvm2::utils::container::std_string capture_environment{};
    };
    /** @brief Dispatch only the successfully committed extent of an operator-selected stage. */
    inline void capture_committed(plan const& p,module_buffer_audit::stage stage,caller who,
                                  std::span<std::uint8_t const> body) noexcept
    {
        if(p.capture_stages & module_buffer_audit::bit(stage))
            module_buffer_audit::output.capture(p.capture_environment,p.route,stage,who.generation,body);
    }
    inline bool valid(plan const& value)noexcept
    {
        return ((value.capture_stages==0 && value.capture_environment.empty()) ||
                (value.capture_stages && value.capture_stages<=15 && identifier(value.capture_environment))) &&
            identifier(value.route)&&identifier(value.client)&&identifier(value.server)&&value.client!=value.server&&
            identifier(value.semantic)&&identifier(value.request_schema)&&identifier(value.response_schema)&&
            value.request_bytes<=maximum_bytes&&value.response_bytes<=maximum_bytes&&value.slots&&value.slots<=maximum_slots&&
            value.deadline_ns&&value.deadline_ns<=static_cast<std::uint64_t>(INT64_MAX);
    }
    inline bool same(caller left,caller right)noexcept{return left.instance==right.instance&&left.generation==right.generation;}
    enum class phase : std::uint8_t {free,queued,received,responded,delivered,quarantined};
    struct slot
    {
        erased_buffer request,response;
        std::uint64_t request_size{},response_size{},token{};
        caller client{},server{};phase state{};
        resources::nanoseconds deadline{};
        region request_region,response_region;
        /** @brief Allocate and zero both capacities before publishing the route. */
        void prepare(std::size_t request_capacity,std::size_t response_capacity)
        {
            request.resize_for_overwrite(request_capacity);request.erase();
            response.resize_for_overwrite(response_capacity);response.erase();
        }
        /** @brief Erase the complete written prefix. Only submit and respond write
         * to these buffers, both from offset zero under the router lock. They set
         * the corresponding size before any callback. The suffix stays zero from
         * prepare or a previous erase, including after short writes and retries.
         * Guest regions have independent lengths and still require full erasure.
         */
        void erase_request()noexcept
        {erase_bytes(request.writable().first(static_cast<std::size_t>(request_size)));request_size=0;}
        void erase_response()noexcept
        {erase_bytes(response.writable().first(static_cast<std::size_t>(response_size)));response_size=0;}
        bool erase()noexcept
        {
            erase_request();erase_response();
            // Always attempt both regions, even if the first owner is unavailable.
            bool left=request_region.clear(),right=response_region.clear();
            if(!left||!right){state=phase::quarantined;return false;}
            state=phase::free;return true;
        }
        ~slot(){(void)erase();}
    };
    struct route
    {
        plan specification;
        ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_unique_ptr<slot>> slots;
        caller client{},server{};
        bool retired{};
    };
    inline std::uint64_t reservation(plan const& value)noexcept
    {return (value.request_bytes+value.response_bytes+sizeof(slot)+sizeof(void*))*value.slots+sizeof(route)+6u*256u;}
    using sink=::uwvm2::utils::container::function_ref<bool(std::uint64_t,std::span<std::uint8_t const>)>;
    /** @brief Detached counts only; no payload, token, address or endpoint identity. */
    struct inspection
    {
        ::uwvm2::utils::container::std_string route;
        std::uint64_t slots{},occupied{},delivered{},quarantined{},reserved{};
        bool retired{};
        bool clear_after_copy{true};
    };
    /** @brief Environment-owned router; fixed storage before guest execution. */
    class router
    {
        mutable std::mutex mutex;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,route,std::less<>> routes;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,::uwvm2::utils::container::std_string,std::less<>> aliases;
        std::uint64_t reserved{};
        bool retired{};
        bool available_locked(region const& candidate)const noexcept
        {
            for(auto const& [name,route]:routes)
            {
                (void)name;
                for(auto const& item:route.slots)
                    if(candidate.overlaps(item->request_region)||candidate.overlaps(item->response_region))return false;
            }
            return true;
        }
    public:
        /** @brief Copy cached debugger counters without expiring or consuming work. */
        bool inspect(::uwvm2::utils::container::std_vector<inspection>& output,std::size_t maximum)const
        {
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock()||routes.size()>maximum)return false;
            for(auto const& [name,route]:routes)
            {
                inspection row;row.route=name;row.slots=route.slots.size();row.reserved=reservation(route.specification);
                row.retired=retired||route.retired;
                row.clear_after_copy=route.specification.clear_after_copy;
                for(auto const& item:route.slots)
                {
                    row.occupied+=item->state!=phase::free;
                    row.delivered+=item->state==phase::received||item->state==phase::delivered;
                    row.quarantined+=item->state==phase::quarantined;
                }
                output.emplace_back(std::move(row));
            }
            return true;
        }
        /** @brief Refuse aliasing before reading or erasing a new request carrier.
         * @details The caller retains the environment authority lease through
         * this check and the input copy/erase. All guest operations share that
         * lease lock. Delivery is rechecked under the router lock after final
         * admission, so release/reacquisition cannot create an aliasing race.
         * Even receipt and cleanup output spans must be disjoint from retained
         * regions. This prevents one call from clearing another call's bytes.
         */
        bool available(region const& source,region const& destination)const noexcept
        {
            std::lock_guard lock{mutex};
            // Both spans share the same authority lease and router lock. Walk
            // retained descriptors once; every source/destination intersection
            // is still checked before any carrier byte can be read or erased.
            for(auto const& [name,route]:routes)
            {
                (void)name;
                for(auto const& item:route.slots)
                    if(source.overlaps(item->request_region)||source.overlaps(item->response_region)||
                       destination.overlaps(item->request_region)||destination.overlaps(item->response_region))return false;
            }
            return true;
        }
        void bind_grant(grant const& permission)
        {std::lock_guard lock{mutex};aliases.emplace(permission.alias,permission.resource);}
        /** @brief Atomically reserve an exact route; allocation failure publishes nothing. */
        error bind(plan const& value,std::uint64_t budget)
        {
            std::lock_guard lock{mutex};if(retired||!valid(value))return error::invalid;
            if(routes.contains(value.route))return error::conflict;
            auto bytes=reservation(value);
            if(bytes>budget||reserved>budget-bytes)return error::too_large;
            route next;next.specification=value;next.slots.reserve(static_cast<std::size_t>(value.slots));
            for(std::uint64_t i{};i<value.slots;++i)
            {
                auto item=::uwvm2::utils::container::make_unique<slot>();
                item->prepare(static_cast<std::size_t>(value.request_bytes),static_cast<std::size_t>(value.response_bytes));
                next.slots.emplace_back(std::move(item));
            }
            routes.emplace(value.route,std::move(next));reserved+=bytes;return error::success;
        }
        /** @brief Admit a fixed role after capability admission; no caller-selected target.
         * @details The enclosing authority lease must remain held throughout
         * invocation. Runtime erasure regions are checked against the actual
         * caller. Failed output cannot consume a request or disclose a result
         * twice. Byte buffers never resize during an invocation.
         */
        error invoke(caller who,std::string_view module,grant const& permission,operation action,std::uint64_t token,
            std::span<std::uint8_t const> input,region delivery,std::uint64_t capacity,resources::nanoseconds now,sink deliver)
        {
            std::lock_guard lock{mutex};
            if(static_cast<std::size_t>(action)>=names.size())return error::unsupported;
            if(retired||!who.instance||!who.generation||permission.kind!=purpose::module_call||permission.binding!="registered-message"||
               !permission.actions.contains(names[static_cast<std::size_t>(action)]))return error::denied;
            auto found=routes.find(permission.resource);if(found==routes.end()||found->second.retired)return error::denied;
            auto& route=found->second;auto const& plan=route.specification;
            bool client=module==plan.client,server=module==plan.server;
            if((action==operation::submit||action==operation::result||action==operation::acknowledge)&&!client)return error::denied;
            if((action==operation::take||action==operation::respond)&&!server)return error::denied;
            if(action==operation::cancel&&!client&&!server)return error::denied;
            if(!available_locked(delivery))return error::conflict;
            if(server)
            {
                if(!route.server.instance)
                {if(action!=operation::take||token||!input.empty()||!delivery.matches(who))return error::denied;route.server=who;}
                else if(!same(route.server,who))return error::denied;
            }
            if(client)
            {
                // The callee must first enter its registered receive endpoint.
                // A queued request never targets an unregistered incarnation.
                if(!route.server.instance)return error::again;
                if(!route.client.instance)
                {if(action!=operation::submit||token||input.size()>plan.request_bytes)return error::denied;route.client=who;}
                else if(!same(route.client,who))return error::denied;
            }
            bool expired{};
            for(auto& item:route.slots)
                if(item->state!=phase::free&&now>=item->deadline)
                {expired|=item->token==token&&(same(item->client,who)||same(item->server,who));if(!item->erase())return error::io;}
            if(expired)return error::timeout;
            slot* selected{};
            if(action==operation::submit)
            {
                if(token||input.size()>plan.request_bytes)return error::invalid;
                for(auto& item:route.slots)if(item->state==phase::free){selected=item.get();break;}
                if(!selected)return error::again;
                resources::nanoseconds deadline;
                if(!resources::add(now,static_cast<resources::nanoseconds>(plan.deadline_ns),deadline))return error::too_large;
                auto id=resources::allocate_identity();if(!id)return error::too_large;
                if(!input.empty())std::memcpy(selected->request.writable().data(),input.data(),input.size());
                selected->request_size=input.size();selected->token=id;selected->client=who;selected->server=route.server;selected->deadline=deadline;
                selected->state=phase::queued;
                if(!deliver(id,{})){selected->erase();return error::invalid;}
                capture_committed(plan,module_buffer_audit::stage::submit,who,selected->request.view().first(input.size()));
                return error::success;
            }
            if(action==operation::take)
            {
                if(token||!input.empty()||!delivery.matches(who))return error::invalid;
                for(auto& item:route.slots)if(item->state==phase::queued){selected=item.get();break;}
                if(!selected)return error::again;
                if(selected->request_size>capacity)return error::too_large;
                // Install the cleanup owner before any guest write.
                selected->request_region=delivery;
                if(!deliver(selected->token,selected->request.view().first(static_cast<std::size_t>(selected->request_size))))
                {if(!selected->request_region.clear()){selected->state=phase::quarantined;return error::io;}return error::invalid;}
                capture_committed(plan,module_buffer_audit::stage::take,who,selected->request.view().first(static_cast<std::size_t>(selected->request_size)));
                selected->erase_request();selected->server=who;selected->state=phase::received;return error::success;
            }
            for(auto& item:route.slots)if(item->token==token&&token){selected=item.get();break;}
            if(!selected)return error::bad_handle;
            if(client?!same(selected->client,who):!same(selected->server,who))return error::denied;
            if(action==operation::cancel)
            {
                if(!input.empty())return error::invalid;
                if(!selected->erase())return error::io;
                return deliver(token,{})?error::success:error::invalid;
            }
            if(selected->state==phase::free)return action==operation::acknowledge&&deliver(token,{})?error::success:error::bad_handle;
            if(selected->state==phase::quarantined)return error::io;
            if(action==operation::respond)
            {
                if(selected->state!=phase::received)return error::conflict;
                if(input.size()>plan.response_bytes)return error::too_large;
                if(!input.empty())std::memcpy(selected->response.writable().data(),input.data(),input.size());
                selected->response_size=input.size();
                if(!selected->request_region.clear()){selected->erase();return error::io;}
                selected->state=phase::responded;
                // A lost receipt retains the response. Replaying a response is
                // refused; the client can still consume the committed result.
                if(!deliver(token,{}))return error::invalid;
                capture_committed(plan,module_buffer_audit::stage::respond,who,selected->response.view().first(input.size()));
                return error::success;
            }
            if(action==operation::result)
            {
                if(!input.empty()||!delivery.matches(who))return error::invalid;
                if(selected->state==phase::queued||selected->state==phase::received)return error::again;
                if(selected->state!=phase::responded)return error::conflict;
                if(selected->response_size>capacity)return error::too_large;
                selected->response_region=delivery;
                if(!deliver(token,selected->response.view().first(static_cast<std::size_t>(selected->response_size))))
                {if(!selected->response_region.clear()){selected->state=phase::quarantined;return error::io;}return error::invalid;}
                capture_committed(plan,module_buffer_audit::stage::result,who,selected->response.view().first(static_cast<std::size_t>(selected->response_size)));
                selected->erase_response();selected->state=phase::delivered;return error::success;
            }
            if(action==operation::acknowledge)
            {
                if(!input.empty()||selected->state!=phase::delivered)return error::conflict;
                if(!selected->erase())return error::io;
                return deliver(token,{})?error::success:error::invalid;
            }
            return error::unsupported;
        }
        /** @brief Revoke a route atomically with its authority lease, erasing all slots. */
        void revoke(std::string_view alias)noexcept
        {std::lock_guard lock{mutex};auto owner=aliases.find(alias);if(owner==aliases.end())return;
            auto it=routes.find(owner->second);if(it==routes.end())return;it->second.retired=true;for(auto& item:it->second.slots)(void)item->erase();}
        void retire_member(caller who)noexcept
        {std::lock_guard lock{mutex};for(auto& [name,route]:routes){(void)name;
            if(same(route.client,who)||same(route.server,who)){route.retired=true;for(auto& item:route.slots)(void)item->erase();}}}
        void retire()noexcept
        {std::lock_guard lock{mutex};retired=true;for(auto& [name,route]:routes){(void)name;for(auto& item:route.slots)(void)item->erase();}}
        ~router(){retire();}
    };
}
#endif
