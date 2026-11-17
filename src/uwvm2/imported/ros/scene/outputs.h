/** @file
 * @brief Bounded volatile cache objects with one recoverable mutation receipt.
 * @details This is a trusted resource registry, not an authorization boundary.
 * The broker holds its exact capability lease while calling these methods.
 * Every binding starts with explicit finite data/object/transfer reservations;
 * bind allocates its bounded replay buffer before any guest may use it.
 * A grant has one ordered mutation stream. The next key acknowledges the last
 * receipt, an exact replay returns that receipt, and older/skipped/conflicting
 * keys never execute. Closing a capability cannot reset this stream.
 *
 * Object storage is private heap memory, without paths, descriptors, native
 * callbacks or filesystem effects. Publication makes accepted bytes immutable
 * and readable only within this live deployment; it never implies persistence,
 * schema validation or trust in the content. Full begin reservations remain
 * allocated and charged through publication. Abort/remove release the actual
 * allocation before returning capacity. Retained receipts survive guest-copy
 * failures and terminal object deletion until an explicit next-key operation.
 */
#pragma once
#include <uwvm2/utils/container/function_ref.h>
#include <exception>
#include <uwvm2/utils/container/unique.h>
#include <uwvm2/utils/container/standard.h>
#include "authority.h"
#include "output_contract.h"
#include "byte_buffer.h"
#include <algorithm>
#include <cstddef>
#include <functional>
#include <limits>
#include <span>
#include <stop_token>
#include <stdexcept>
#if defined(__linux__)
#include <unistd.h>
#endif

namespace uwvm2::imported::ros::scene::outputs
{
    using bytes=std::span<std::uint8_t const>;
    inline constexpr std::uint64_t maximum_transfer=4u*1024u*1024u;
    /** @brief Finite per-binding byte, object, object-extent and transfer limits.
     * @details All zero values mean zero allowance. Aggregate bytes count full
     * object allocations, not only initialized content. Replay metadata has a
     * separate fixed reservation returned by receipt_bytes(). Native object
     * extents must fit size_t; no narrowing or implicit unlimited value occurs.
     * Complete publication additionally requires explicit finite in-flight
     * scratch byte/call reservations. These default to zero and do not affect
     * the existing staged API. Scratch charges the fixed inline buffer plus
     * any larger heap request extent; allocator metadata is bounded separately
     * by the call count. Object preparation uses the ordinary object capacity.
     */
    struct plan
    {std::uint64_t bytes{},objects{},object_bytes{},transfer{},publication_scratch_bytes{},publication_scratch_calls{};};
    inline bool valid(plan const& value) noexcept
    {return value.bytes<=static_cast<std::uint64_t>(INT64_MAX)&&value.objects!=UINT64_MAX&&
        value.object_bytes<=value.bytes&&value.object_bytes<=SIZE_MAX&&value.transfer<=maximum_transfer&&
        value.publication_scratch_bytes<=static_cast<std::uint64_t>(INT64_MAX)&&value.publication_scratch_calls!=UINT64_MAX;}
    /** @brief Exact retained request plus fixed nine-word recovery receipt bytes. */
    inline std::uint64_t receipt_bytes(plan const& value) noexcept{return value.transfer+24u+::uwvm2::imported::ros::output_contract::receipt_bytes;}
    /** @brief Stable nine-word result for mutations, status and receipt lookup.
     * @details effect_status is the operation result; a successful carrier only
     * means these words were delivered. States are absent=0, staged=1,
     * published=2, aborted=3, removed=4. Flag bit zero denotes publication;
     * durability bit one is always clear. Unknown-object failures redact IDs.
     */
    struct receipt
    {
        std::uint64_t key{},operation{},object{},effect_status{},accepted_bytes{},
            total_bytes{},retained_bytes{},state{},flags{};
        friend bool operator==(receipt const&,receipt const&)=default;
    };
    /** @brief Cached debugger metadata, without content or native identities.
     * @details A terminal/absent last receipt gets its own receipt_only row;
     * otherwise its retention count is attached to the corresponding live row.
     */
    struct snapshot
    {
        handle id{};::uwvm2::utils::container::std_string alias,resource,binding;
        std::uint64_t bytes{},reserved{},retained_receipts{},state{};
        bool published{},retired{},receipt_only{};
    };

    class registry
    {
        struct object
        {
            ::uwvm2::utils::container::std_unique_ptr<std::uint8_t[]> data;
            std::uint64_t bytes{},reserved{},state{1},last_key{},last_operation{};
        };
        using object_map=::uwvm2::utils::container::std_map<handle,object>;
        struct binding
        {
            ::uwvm2::utils::container::std_string resource;
            plan bounds;
            ::uwvm2::utils::container::std_vector<std::uint8_t> last_request;
            std::size_t last_size{};
            receipt last;
            object_map objects;
            std::uint64_t reserved{},pending_objects{},scratch_bytes{},scratch_calls{};
            bool retired{};
        };
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
        mutable std::mutex mutex;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,binding,std::less<>> bindings;
        bool retired{};
        static std::uint64_t word(bytes input,std::size_t index) noexcept
        {std::uint64_t value{};for(unsigned n{};n<8;++n)value|=std::uint64_t(input[index*8+n])<<(n*8);return value;}
        static void metadata(receipt& result,handle id,object const& value) noexcept
        {result.object=id;result.total_bytes=value.bytes;result.retained_bytes=value.reserved;
            result.state=value.state;result.flags=value.state==2?1:0;}
        binding* find(std::string_view alias) noexcept
        {auto it=bindings.find(alias);return retired||it==bindings.end()||it->second.retired?nullptr:&it->second;}
        void release_scratch(binding* selected,std::uint64_t bytes) noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};
            if(!selected||!selected->scratch_calls||selected->scratch_bytes<bytes)std::terminate();
            selected->scratch_bytes-=bytes;--selected->scratch_calls;
        }
        void release_pending(binding* selected,std::uint64_t bytes) noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};
            if(!selected||!selected->pending_objects||selected->reserved<bytes)std::terminate();
            selected->reserved-=bytes;--selected->pending_objects;
        }
        /** @brief Private rollback for capacity and a prepared, unpublished map node.
         * @details Release allocation before making its capacity available to
         * another operation. The owning broker keeps this registry alive.
         */
        struct pending_publication
        {
            registry* owner;binding* selected{};std::uint64_t bytes{};bool held{};
            object_map::node_type node{};
            ~pending_publication()
            {
                if(!held)return;
                node={};owner->release_pending(selected,bytes);
            }
        };
        static bool object_capacity(binding const& value,std::uint64_t max_resources) noexcept
        {
            auto count=static_cast<std::uint64_t>(value.objects.size());
            return count<value.bounds.objects&&value.pending_objects<value.bounds.objects-count&&
                   count<max_resources&&value.pending_objects<max_resources-count;
        }
    public:
        /** @brief Move-only in-flight publication reservation, not guest authority.
         * @details reserve_publication() is called only after exact authority
         * preflight and before any owned request allocation/copy. Its broker
         * owner must outlive the reservation. Revocation denies later effects
         * but does not prevent rollback. Forked destruction touches no inherited
         * mutex. Moving or resetting the reservation releases its old charge.
         */
        class publication_scratch
        {
            friend class registry;
            registry* owner{};binding* selected{};std::size_t extent{};std::uint64_t charge{};
        public:
            publication_scratch()=default;
            publication_scratch(publication_scratch const&)=delete;
            publication_scratch& operator=(publication_scratch const&)=delete;
            publication_scratch(publication_scratch&& other) noexcept:
                owner(std::exchange(other.owner,nullptr)),selected(other.selected),extent(other.extent),charge(other.charge){}
            publication_scratch& operator=(publication_scratch&& other) noexcept
            {
                if(this!=&other){reset();owner=std::exchange(other.owner,nullptr);selected=other.selected;extent=other.extent;charge=other.charge;}return *this;
            }
            ~publication_scratch(){reset();}
            void reset() noexcept
            {if(auto* previous=std::exchange(owner,nullptr))previous->release_scratch(selected,charge);}
            explicit operator bool() const noexcept{return owner!=nullptr;}
            std::uint64_t reserved_bytes() const noexcept{return owner?charge:0;}
        };
        /** @brief Reserve bounded carrier storage before complete publication copying.
         * @details This is a capacity operation, not rate attenuation or an
         * authorization decision. It requires a finite configured call slot
         * and the inline buffer plus any heap request storage. No quota is
         * silently made unlimited by zero. The shared replay reservation keeps
         * transfer+24 framing so the old append stream remains fully usable.
         */
        error reserve_publication(std::string_view alias,std::size_t request_bytes,publication_scratch& output)
        {
            output.reset();if(!same_process())return error::denied;
            if(request_bytes<output_contract::publication_framing)return error::invalid;
            std::lock_guard lock{mutex};auto selected=find(alias);if(!selected)return error::denied;
            if(request_bytes-output_contract::publication_framing>selected->bounds.transfer)return error::too_large;
            auto charge=static_cast<std::uint64_t>(byte_buffer::inline_capacity)+(request_bytes>byte_buffer::inline_capacity?request_bytes:0);
            if(selected->scratch_calls>=selected->bounds.publication_scratch_calls||charge>selected->bounds.publication_scratch_bytes-selected->scratch_bytes)return error::again;
            selected->scratch_bytes+=charge;++selected->scratch_calls;
            output.owner=this;output.selected=selected;output.extent=request_bytes;output.charge=charge;return error::success;
        }
        /** @brief Check a publication extent for an uncharged short-receipt query.
         * @details Performs no scratch reservation or allocation. Publication's
         * two-word framing must not inherit append's larger 24-byte allowance.
         */
        error publication_size(std::string_view alias,std::size_t request_bytes)
        {
            if(!same_process())return error::denied;
            if(request_bytes<output_contract::publication_framing)return error::invalid;
            std::lock_guard lock{mutex};auto selected=find(alias);if(!selected)return error::denied;
            return request_bytes-output_contract::publication_framing<=selected->bounds.transfer?error::success:error::too_large;
        }
        /** @brief Install one inert cache binding with its replay memory reserved.
         * @details Called only by trusted deployment configuration, before guest
         * activation. No I/O or user callback runs. Allocation failure leaves
         * the registry unchanged; unsupported platforms do not install a grant.
         */
        error bind(::uwvm2::utils::container::std_string alias,::uwvm2::utils::container::std_string resource,plan limits)
        {
#if defined(__linux__)
            if(!same_process())return error::denied;
            if(!identifier(alias)||!identifier(resource)||!valid(limits))return error::invalid;
            std::lock_guard lock{mutex};if(retired)return error::denied;
            if(bindings.contains(alias))return error::conflict;
            binding pending;pending.resource=std::move(resource);pending.bounds=limits;
            pending.last_request.resize(static_cast<std::size_t>(limits.transfer+24u));
            bindings.emplace(std::move(alias),std::move(pending));return error::success;
#else
            (void)alias;(void)resource;(void)limits;return error::unsupported;
#endif
        }
        /** @brief Check the live binding's request bound before host allocation.
         * @details This allocation-free check accepts at most the reserved
         * transfer plus 24 framing bytes. It grants no authority, charges no
         * admission and does not validate operation shape or acknowledge a
         * receipt. The broker rechecks authority and the bound before effects.
         */
        error request_size(std::string_view alias,std::size_t size)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=find(alias);if(!value)return error::denied;
            return size<=value->last_request.size()?error::success:error::too_large;
        }
        /** @brief Execute or replay one exactly ordered mutation without a callback.
         * @details Shape/sequence errors preserve the previous receipt and output.
         * Admission, caller identity and optional attenuation are broker duties.
         * A next-key request explicitly acknowledges the previous receipt. Once
         * accepted, effect failures also produce a retained receipt and consume
         * that key; a retry of those exact bytes returns the same outcome.
         * Begin allocation failures happen before any effect/receipt replacement
         * and return no_memory directly. All post-effect bookkeeping is fixed
         * storage, so publication cannot lose its recovery record to allocation.
         */
        error mutate(std::string_view alias,::uwvm2::imported::ros::output_contract::operation action,
                     bytes request,std::uint64_t max_resources,receipt& output)
        {
            if(!same_process())return error::denied;
            using op=::uwvm2::imported::ros::output_contract::operation;
            if(action>op::remove)return error::unsupported;
            auto fixed=action==op::commit?24u:16u;
            if(action==op::append?request.size()<24:request.size()!=fixed)return error::invalid;
            std::lock_guard lock{mutex};auto selected=find(alias);if(!selected)return error::denied;
            auto& value=*selected;
            if(request.size()>value.last_request.size())return error::too_large;
            auto key=word(request,0);if(!key)return error::conflict;
            auto operation=static_cast<std::uint64_t>(action)+1;
            if(key==value.last.key)
            {
                if(value.last.operation!=operation||value.last_size!=request.size()||
                   !std::equal(request.begin(),request.end(),value.last_request.begin()))return error::conflict;
                output=value.last;return error::success;
            }
            if(value.last.key==UINT64_MAX||key!=value.last.key+1)return error::conflict;
            receipt next;next.key=key;next.operation=operation;
            if(action==op::begin)
            {
                auto size=word(request,1);
                if(size>value.bounds.object_bytes||size>value.bounds.bytes-value.reserved||
                   !object_capacity(value,max_resources))
                    next.effect_status=static_cast<unsigned>(error::again);
                else
                {
                    try
                    {
                        object pending;pending.reserved=size;
                        if(size)pending.data=::uwvm2::utils::container::make_unique_for_overwrite<std::uint8_t[]>(static_cast<std::size_t>(size));
                        auto id=resources::allocate_identity();if(!id)return error::too_large;
                        auto [entry,inserted]=value.objects.emplace(id,std::move(pending));
                        if(!inserted)return error::conflict;
                        value.reserved+=size;metadata(next,id,entry->second);
                    }
                    catch(std::bad_alloc const&){return error::no_memory;}
                    catch(std::length_error const&){return error::too_large;}
                }
            }
            else
            {
                auto id=word(request,1);auto entry=value.objects.find(id);
                if(entry==value.objects.end())next.effect_status=static_cast<unsigned>(error::bad_handle);
                else
                {
                    auto& item=entry->second;metadata(next,id,item);
                    if(action==op::append)
                    {
                        auto size=request.size()-24;
                        if(item.state!=1)next.effect_status=static_cast<unsigned>(error::denied);
                        else if(word(request,2)!=item.bytes)next.effect_status=static_cast<unsigned>(error::conflict);
                        else if(size>item.reserved-item.bytes)next.effect_status=static_cast<unsigned>(error::too_large);
                        else
                        {
                            if(size)std::copy_n(request.data()+24,size,item.data.get()+static_cast<std::size_t>(item.bytes));
                            item.bytes+=size;next.accepted_bytes=size;metadata(next,id,item);
                        }
                    }
                    else if(action==op::commit)
                    {
                        if(item.state!=1)next.effect_status=static_cast<unsigned>(error::denied);
                        else if(word(request,2)!=item.bytes)next.effect_status=static_cast<unsigned>(error::conflict);
                        else{item.state=2;metadata(next,id,item);}
                    }
                    else if((action==op::abort&&item.state!=1)||(action==op::remove&&item.state!=2))
                        next.effect_status=static_cast<unsigned>(error::denied);
                    else
                    {
                        auto released=item.reserved;item.data.reset();value.objects.erase(entry);
                        value.reserved-=released;next.retained_bytes=0;
                        next.state=action==op::abort?3:4;next.flags=0;
                    }
                }
            }
            if(next.effect_status==0&&next.object)
                if(auto it=value.objects.find(next.object);it!=value.objects.end())
                {it->second.last_key=key;it->second.last_operation=operation;}
            // These stores cannot allocate or fail after a resource effect.
            std::copy(request.begin(),request.end(),value.last_request.begin());
            value.last_size=request.size();value.last=next;output=next;return error::success;
        }
        /** @brief Publish one completely owned bounded object in the existing stream.
         * @param request Immutable complete host-owned [key,size,payload] copy;
         * the caller must keep this private copy unchanged through return.
         * @param scratch Exact in-flight reservation acquired before that copy.
         * @param final_check Trusted cancellation and authority revalidation,
         * executed outside the registry mutex after all rejectable preparation.
         * The broker keeps its original authority lease and may acquire a
         * second recursive lease here; it must not release the original lease.
         * @param stop Optional continuously linked native stop token; the real
         * VM cancellation predicate must still run in final_check when it has
         * no equivalent linked token. A one-time sampled stop_source is not an
         * equivalent runtime cancellation adapter.
         * @details Preparation reserves ordinary payload/object capacity and
         * creates a private map node. Existing begin operations count these
         * pending slots. Allocation failure restores all capacity without
         * consuming a key or replacing a receipt. Next, final_check runs with
         * no provider lock. Commit attempts the mutex only with try_lock: it
         * never waits after that cancellation observation, and contention
         * returns again without a receipt/key effect. Under the acquired lock
         * it rechecks binding, exact stream position, reservation and stop.
         * Moving the prepared node and replacing the receipt cannot allocate.
         *
         * final_check is the actual predicate cancellation decision point;
         * a later cancellation cannot be claimed to withdraw an accepted
         * publication. A linked stop token adds a final atomic observation.
         * Failed result delivery is handled by the broker and exact replay.
         * Empty objects still consume a slot and require the full compound
         * authority. Capacity refusals receive a retained effect receipt;
         * transient commit-lock contention does not consume a key.
         */
        error publish(std::string_view alias,bytes request,std::uint64_t max_resources,receipt& output,
                      publication_scratch const& scratch,::uwvm2::utils::container::function_ref<error()> final_check,std::stop_token stop={})
        {
            if(!same_process())return error::denied;
            if(request.size()<output_contract::publication_framing||!final_check)return error::invalid;
            auto size=request.size()-output_contract::publication_framing;
            if(word(request,1)!=size)return error::invalid;
            if(scratch.owner!=this||scratch.extent!=request.size())return error::denied;
            auto key=word(request,0);constexpr auto operation=static_cast<std::uint64_t>(output_contract::operation::publish)+1;
            pending_publication pending{this};receipt next;next.key=key;next.operation=operation;
            {
                std::lock_guard lock{mutex};auto selected=find(alias);
                if(!selected||selected!=scratch.selected)return error::denied;
                auto& value=*selected;
                if(size>value.bounds.transfer||request.size()>value.last_request.size())return error::too_large;
                if(!key)return error::conflict;
                if(key==value.last.key)
                {
                    if(value.last.operation!=operation||value.last_size!=request.size()||
                       !std::equal(request.begin(),request.end(),value.last_request.begin()))return error::conflict;
                }
                else
                {
                    if(value.last.key==UINT64_MAX||key!=value.last.key+1)return error::conflict;
                    if(size>value.bounds.object_bytes||size>value.bounds.bytes-value.reserved||!object_capacity(value,max_resources))
                        next.effect_status=static_cast<unsigned>(error::again);
                    else
                    {
                        value.reserved+=size;++value.pending_objects;
                        pending.selected=selected;pending.bytes=size;pending.held=true;
                        try
                        {
                            object candidate;candidate.bytes=candidate.reserved=size;candidate.state=2;
                            candidate.last_key=key;candidate.last_operation=operation;
                            if(size)
                            {
                                candidate.data=::uwvm2::utils::container::make_unique_for_overwrite<std::uint8_t[]>(size);
                                std::copy_n(request.data()+output_contract::publication_framing,size,candidate.data.get());
                            }
                            object_map staging;staging.emplace(0,std::move(candidate));pending.node=staging.extract(staging.begin());
                        }
                        catch(std::bad_alloc const&){return error::no_memory;}
                        catch(std::length_error const&){return error::too_large;}
                    }
                }
            }
            // This callback may observe runtime predicates or reentrantly
            // revoke/close the capability. It never runs under the provider lock.
            if(auto checked=final_check();checked!=error::success)return checked;
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            auto selected=find(alias);if(!selected||selected!=scratch.selected)return error::denied;
            auto& value=*selected;
            if(stop.stop_requested())return error::cancelled;
            if(key==value.last.key)
            {
                if(value.last.operation!=operation||value.last_size!=request.size()||
                   !std::equal(request.begin(),request.end(),value.last_request.begin()))return error::conflict;
                output=value.last;return error::success;
            }
            if(value.last.key==UINT64_MAX||key!=value.last.key+1)return error::conflict;
            if(pending.held)
            {
                if(pending.selected!=selected||pending.bytes!=size||!value.pending_objects||value.reserved<size||pending.node.empty())return error::denied;
                auto id=resources::allocate_identity();if(!id)return error::too_large;
                pending.node.key()=id;
                auto inserted=value.objects.insert(std::move(pending.node));if(!inserted.inserted)return error::conflict;
                --value.pending_objects;pending.held=false;
                metadata(next,id,inserted.position->second);next.accepted_bytes=size;
            }
            else if(!next.effect_status)return error::conflict;
            std::copy(request.begin(),request.end(),value.last_request.begin());
            value.last_size=request.size();value.last=next;output=next;return error::success;
        }
        /** @brief Validate and bound a published read before allocating its copy.
         * @details This allocation-free check verifies live alias ownership,
         * publication, offset and the original requested transfer, including
         * requests at EOF. It returns only the available initialized extent.
         * No authority, admission or object lifetime is acquired here; read()
         * repeats validation while holding the registry lock before copying.
         */
        error read_size(std::string_view alias,handle id,std::uint64_t offset,std::uint64_t requested,std::size_t& actual)
        {
            actual=0;if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=find(alias);if(!value)return error::denied;
            auto it=value->objects.find(id);if(it==value->objects.end())return error::bad_handle;
            auto const& item=it->second;if(item.state!=2)return error::denied;
            if(offset>item.bytes||requested>value->bounds.transfer)return error::too_large;
            actual=static_cast<std::size_t>(std::min(requested,item.bytes-offset));return error::success;
        }
        /** @brief Produce an owned initialized read copy with one locked lookup.
         * @details Checks the original requested extent even at EOF, before
         * allocating scratch memory. Publication and alias ownership remain
         * protected through the copy. No callback or guest memory access runs
         * under this lock; the caller delivers the detached copy afterward.
         * The original object's full reservation remains charged throughout.
         */
        error copy_read(std::string_view alias,handle id,std::uint64_t offset,std::uint64_t requested,byte_buffer& output)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=find(alias);if(!value)return error::denied;
            auto it=value->objects.find(id);if(it==value->objects.end())return error::bad_handle;
            auto const& item=it->second;if(item.state!=2)return error::denied;
            if(offset>item.bytes||requested>value->bounds.transfer)return error::too_large;
            auto size=static_cast<std::size_t>(std::min(requested,item.bytes-offset));
            output.resize_for_overwrite(size);
            if(size)std::copy_n(item.data.get()+static_cast<std::size_t>(offset),size,output.writable().data());
            return error::success;
        }
        /** @brief Read only published bytes, independently of mutation authority.
         * @details No mutable view or backing allocation escapes the lock.
         * Offsets beyond EOF and oversized transfers fail before copying. A
         * final short read is allowed; staging and unpublished bytes stay hidden.
         */
        error read(std::string_view alias,handle id,std::uint64_t offset,std::span<std::uint8_t> output,std::size_t& count)
        {
            count=0;if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=find(alias);if(!value)return error::denied;
            auto it=value->objects.find(id);if(it==value->objects.end())return error::bad_handle;
            auto const& item=it->second;if(item.state!=2)return error::denied;
            if(offset>item.bytes||output.size()>value->bounds.transfer)return error::too_large;
            auto amount=static_cast<std::size_t>(std::min<std::uint64_t>(output.size(),item.bytes-offset));
            if(amount)std::copy_n(item.data.get()+static_cast<std::size_t>(offset),amount,output.data());
            count=amount;return error::success;
        }
        /** @brief Observe current owned metadata without advancing the mutation key.
         * @details Status contains no content and is not an acknowledgement.
         * key/op identify this object's last successful mutation, even after
         * that recovery receipt was acknowledged. accepted_bytes is always zero.
         */
        error status(std::string_view alias,handle id,receipt& output)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=find(alias);if(!value)return error::denied;
            auto it=value->objects.find(id);if(it==value->objects.end())return error::bad_handle;
            receipt next;next.key=it->second.last_key;next.operation=it->second.last_operation;
            metadata(next,id,it->second);output=next;return error::success;
        }
        /** @brief Recover the retained mutation outcome without executing or ACKing.
         * @details Unknown, zero, acknowledged or future keys return conflict.
         * Grant revocation and environment retirement deny even receipt access.
         */
        error receipt_for(std::string_view alias,std::uint64_t key,receipt& output)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=find(alias);if(!value)return error::denied;
            if(!key||key!=value->last.key)return error::conflict;
            output=value->last;return error::success;
        }
        /** @brief Revoke one binding without resetting its allocations or receipt. */
        void revoke(std::string_view alias)
        {if(!same_process())return;std::lock_guard lock{mutex};auto it=bindings.find(alias);if(it!=bindings.end())it->second.retired=true;}
        /** @brief Retire all operations while retaining bounded diagnostic state. */
        void retire(){if(!same_process())return;std::lock_guard lock{mutex};retired=true;}
        /** @brief Copy cached bounded rows; never read data or consume a receipt. */
        bool inspect(::uwvm2::utils::container::std_vector<snapshot>& output,std::size_t maximum) const
        {
            if(!same_process())return false;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return false;
            ::uwvm2::utils::container::std_vector<snapshot> rows;
            for(auto const& [alias,value]:bindings)
            {
                for(auto const& [id,item]:value.objects)
                {
                    if(rows.size()==maximum)return false;
                    rows.push_back({id,alias,value.resource,"volatile-output",item.bytes,item.reserved,
                        value.last.key&&value.last.object==id?1u:0u,item.state,item.state==2,retired||value.retired,false});
                }
                if(value.last.key&&!value.objects.contains(value.last.object))
                {
                    if(rows.size()==maximum)return false;
                    rows.push_back({value.last.object,alias,value.resource,"volatile-output",value.last.total_bytes,
                        value.last.retained_bytes,1,value.last.state,(value.last.flags&1)!=0,retired||value.retired,true});
                }
            }
            output=std::move(rows);return true;
        }
    };
}
