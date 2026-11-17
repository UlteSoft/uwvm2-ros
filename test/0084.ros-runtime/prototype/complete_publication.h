/** @file
 * @brief Unregistered complete-object publication algorithm prototype.
 * @details This is test-only design code, not a VM provider or authorization
 * implementation. It deliberately exposes no import, CLI option or SDK API.
 * A future integration must share ordered_receipt, object storage and the
 * existing authority lease with all legacy output mutators. Duplicating this
 * prototype beside the production registry would create two unsafe streams.
 *
 * The carrier requests [key:u64, payload_size:u64, payload]. A fixed finite
 * scratch reservation is acquired before its owned copy; object capacity is
 * acquired only after the entire copy, final authority and cancellation checks.
 * Publication and receipt replacement are atomic under one mutex. Payload
 * ownership is moved into a preallocated object slot, so nothing allocates
 * after the final admission/commit point. No reader or sink callback runs
 * under that mutex. Every public data operation rejects inherited fork state
 * before locking. A production VM integration must retain its complete FP
 * guard until allocation handlers and every callback path are separately audited.
 */
#pragma once
#include <uwvm2/imported/ros/scene/outputs.h>
#include <chrono>
#include <cstring>
#include <optional>
#include <stop_token>
#include <utility>

namespace uwvm2::test::complete_publication
{
    namespace scene=::uwvm2::imported::ros::scene;
    using error=scene::error;
    using receipt=scene::outputs::receipt;
    using bytes=std::span<std::uint8_t const>;
    inline constexpr std::uint64_t publish_operation=8;
    enum action : unsigned { begin=1,append=2,commit=4,publish=8,read=16,remove=32 };
    inline constexpr unsigned required_publish=begin|append|commit|publish;
    inline constexpr unsigned all_actions=required_publish|read|remove;
    inline std::uint64_t word(bytes input,std::size_t index) noexcept
    {std::uint64_t value{};for(unsigned n{};n<8;++n)value|=std::uint64_t(input[index*8+n])<<(n*8);return value;}

    /** @brief One bounded exact replay stream shared by every mutation kind.
     * @details Its owner must hold the same registry/authority lock for matching
     * and committing. No hash substitutes for exact request-byte comparison.
     * Storage is reserved during trusted configuration. A next accepted key
     * acknowledges the previous receipt; errors before acceptance do not.
     */
    struct ordered_receipt
    {
        mutable std::mutex mutex;
        std::vector<std::uint8_t> request;
        std::size_t size{};
        receipt last;
        explicit ordered_receipt(std::size_t maximum_request):request(maximum_request){}
        error match(std::uint64_t operation,bytes input,receipt& output,bool& replay) const noexcept
        {
            replay=false;
            if(input.size()<8||input.size()>request.size())return error::invalid;
            auto key=word(input,0);
            if(!key)return error::conflict;
            if(key==last.key)
            {
                if(last.operation!=operation||input.size()!=size||!std::equal(input.begin(),input.end(),request.begin()))return error::conflict;
                output=last;replay=true;return error::success;
            }
            return last.key!=UINT64_MAX&&key==last.key+1?error::success:error::conflict;
        }
        /** @brief Nonallocating replacement after all rejectable work succeeds. */
        void remember(bytes input,receipt const& value) noexcept
        {std::copy(input.begin(),input.end(),request.begin());size=input.size();last=value;}
    };

    /** @brief Explicit finite capacities; zero never means unlimited.
     * @details Payload capacity excludes fixed framing overhead: live object
     * allocation is bounded by payload_bytes + 16*object_slots. The one replay
     * buffer reserves max_payload+24 bytes, retaining the larger legacy append
     * framing even though publish needs only 16. In-flight requests independently
     * reserve their full framing+payload extent and a slot before allocation.
     */
    struct plan
    {
        std::uint64_t payload_bytes{},object_slots{},object_bytes{},max_payload{},max_handles{},scratch_bytes{},scratch_calls{};
    };
    /** @brief Prototype of a single exact deployed grant, default denied.
     * @details All four publish/dependency actions must be both exposed and
     * granted on this same capability and owner generation. Optional limits
     * express proposed logical-work accounting, NOT the current CLI contract.
     * Publish always charges three logical units and payload+280 bytes, even
     * for empty data. That empty-object charge conservatively exceeds the old
     * two-call begin/commit path. Each replay pays again. Limits are atomic:
     * an insufficient allowance never charges only part of the compound call.
     */
    struct policy
    {
        unsigned exposed{},granted{};
        std::optional<std::uint64_t> logical_calls,bytes_per_window;
        std::chrono::nanoseconds window{std::chrono::seconds{1}};
    };
    struct counters
    {
        std::uint64_t retained_bytes{},objects{},scratch_bytes{},scratch_calls{},admissions{},logical_work{},window_calls{},window_bytes{},last_key{};
    };

    /** @brief Minimal coordinator for the publication proof, not a second registry.
     * @details Fixed slots model the future provider's already configured
     * object index. It implements only publish/read/remove and borrows a replay
     * stream; it does not reimplement begin/append/commit. Integration must use
     * the production object index and shared mutation lock instead of keeping
     * this test coordinator as an alternate object store.
     */
    class coordinator
    {
        struct object
        {std::unique_ptr<std::uint8_t[]> owned;std::uint64_t id{},size{};bool live{};};
        plan bounds;
        policy access;
        scene::caller owner;
        scene::handle capability;
        ordered_receipt& stream;
        std::vector<object> objects;
        std::mutex& mutex;
        counters count;
        std::chrono::steady_clock::time_point began{std::chrono::steady_clock::now()};
        ::pid_t process{::getpid()};
        bool revoked{};
        bool same_process() const noexcept{return process==::getpid();}
        static void add_saturated(std::uint64_t& value,std::uint64_t amount) noexcept
        {value=amount>UINT64_MAX-value?UINT64_MAX:value+amount;}
        error authorized(scene::caller who,scene::handle cap,unsigned needed) const noexcept
        {return !revoked&&who.instance==owner.instance&&who.generation==owner.generation&&cap==capability&&
            (access.exposed&needed)==needed&&(access.granted&needed)==needed?error::success:error::denied;}
        error budget(std::uint64_t calls,std::uint64_t amount,bool charge)
        {
            auto used_calls=count.window_calls,used_bytes=count.window_bytes;
            bool elapsed{};auto now=began;
            if(access.logical_calls||access.bytes_per_window)
            {
                now=std::chrono::steady_clock::now();elapsed=now-began>=access.window;
                if(elapsed)used_calls=used_bytes=0;
                if((access.logical_calls&&(used_calls>*access.logical_calls||calls>*access.logical_calls-used_calls))||
                   (access.bytes_per_window&&(used_bytes>*access.bytes_per_window||amount>*access.bytes_per_window-used_bytes)))return error::again;
            }
            if(charge)
            {
                if(elapsed){began=now;count.window_calls=count.window_bytes=0;}
                if(access.logical_calls)count.window_calls+=calls;
                if(access.bytes_per_window)count.window_bytes+=amount;
                add_saturated(count.admissions,1);add_saturated(count.logical_work,calls);
            }
            return error::success;
        }
        struct scratch_lease
        {
            coordinator* value;std::uint64_t size{};bool active{};
            ~scratch_lease()
            {
                if(!active||!value->same_process())return;
                std::lock_guard lock{value->mutex};value->count.scratch_bytes-=size;--value->count.scratch_calls;
            }
        };
    public:
        /** @brief Configure finite storage once, before any guest-equivalent call. */
        coordinator(plan limits,policy authority,scene::caller who,scene::handle cap,ordered_receipt& shared_stream):
            bounds(limits),access(authority),owner(who),capability(cap),stream(shared_stream),mutex(shared_stream.mutex)
        {
            if(!who.instance||!who.generation||!cap||((authority.exposed|authority.granted)&~all_actions)||limits.max_payload>scene::outputs::maximum_transfer||
               limits.object_bytes>limits.payload_bytes||limits.payload_bytes>INT64_MAX||
               limits.object_slots>SIZE_MAX/sizeof(object)||limits.object_slots>(UINT64_MAX-limits.payload_bytes)/16||
               limits.scratch_bytes>INT64_MAX||limits.scratch_calls==UINT64_MAX||limits.max_handles>limits.object_slots||
               shared_stream.request.size()<limits.max_payload+24||authority.window.count()<=0)throw std::invalid_argument{"invalid prototype plan"};
            objects.resize(static_cast<std::size_t>(limits.object_slots));
        }
        coordinator(coordinator const&)=delete;
        /** @brief Checked-copy complete object, revalidate, atomically publish and deliver.
         * @param reader Trusted checked-copy adapter: true means the entire span
         * was initialized. Partial/failed/throwing readers never reach parsing.
         * @param sink Receipt delivery after the lock is released. Failure or
         * exception leaves the exact retained receipt for an authorized retry.
         * @param stop Trusted runtime cancellation token, checked under the
         * final mutation lock after authority and budget admission. This pure
         * atomic observation invokes no callback. It is the cancellation
         * linearization point: later stop requests cannot undo publication.
         * Admitted cancellation keeps its work charge but changes no object,
         * capacity or replay key. An empty token explicitly models a trusted
         * noncancellable caller; guest data must never choose this token.
         */
        template<class Reader,class Sink>
        error invoke(scene::caller who,scene::handle cap,std::size_t request_bytes,std::size_t output_capacity,
                     std::size_t& required,Reader&& reader,Sink&& sink,std::stop_token stop)
        {
            required=0;if(!same_process())return error::denied;
            if(request_bytes<16)return error::invalid;
            auto payload=request_bytes-16;
            scratch_lease scratch{this};
            {
                std::lock_guard lock{mutex};auto checked=authorized(who,cap,required_publish);if(checked!=error::success)return checked;
                if(payload>bounds.max_payload)return error::too_large;
                if(output_capacity<72){required=72;return error::too_large;}
                if(auto checked_budget=budget(3,payload+280,false);checked_budget!=error::success)return checked_budget;
                if(count.scratch_calls>=bounds.scratch_calls||request_bytes>bounds.scratch_bytes-count.scratch_bytes)return error::again;
                count.scratch_bytes+=request_bytes;++count.scratch_calls;scratch.size=request_bytes;scratch.active=true;
            }
            try
            {
                auto owned=std::make_unique_for_overwrite<std::uint8_t[]>(request_bytes);
                if(!reader(std::span{owned.get(),request_bytes}))return error::invalid;
                bytes request{owned.get(),request_bytes};
                if(word(request,1)!=payload)return error::invalid;
                if(!same_process())return error::denied;
                receipt next;
                {
                    std::lock_guard lock{mutex};auto checked=authorized(who,cap,required_publish);if(checked!=error::success)return checked;
                    if(auto checked_budget=budget(3,payload+280,true);checked_budget!=error::success)return checked_budget;
                    if(stop.stop_requested())return error::cancelled;
                    bool replay{};auto matched=stream.match(publish_operation,request,next,replay);if(matched!=error::success)return matched;
                    if(!replay)
                    {
                        next={};next.key=word(request,0);next.operation=publish_operation;
                        if(payload>bounds.object_bytes||payload>bounds.payload_bytes-count.retained_bytes||
                           count.objects>=bounds.object_slots||count.objects>=bounds.max_handles)
                            next.effect_status=static_cast<unsigned>(error::again);
                        else
                        {
                            auto id=::uwvm2::imported::ros::resources::allocate_identity();if(!id)return error::too_large;
                            auto slot=std::find_if(objects.begin(),objects.end(),[](object const& value){return !value.live;});
                            // The slot index and replay storage were allocated at
                            // configuration. Everything below is nonallocating.
                            next.object=id;next.accepted_bytes=payload;next.total_bytes=payload;next.retained_bytes=payload;
                            next.state=2;next.flags=1;
                            stream.remember(request,next);
                            slot->owned=std::move(owned);slot->id=id;slot->size=payload;slot->live=true;
                            count.retained_bytes+=payload;++count.objects;
                        }
                        if(next.effect_status)stream.remember(request,next);
                    }
                }
                required=72;return sink(next)?error::success:error::invalid;
            }
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
            catch(...){return error::invalid;}
        }
        /** @brief Independent authorized read into already bounded caller storage. */
        error read_owned(scene::caller who,scene::handle cap,scene::handle id,std::span<std::uint8_t> output,std::size_t& size)
        {
            size=0;if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto checked=authorized(who,cap,read);if(checked!=error::success)return checked;
            auto slot=std::find_if(objects.begin(),objects.end(),[&](object const& value){return value.live&&value.id==id;});
            if(slot==objects.end())return error::bad_handle;
            if(output.size()<slot->size)return error::too_large;
            if(auto charged=budget(1,slot->size+24,true);charged!=error::success)return charged;
            size=static_cast<std::size_t>(slot->size);if(size)std::copy_n(slot->owned.get()+16,size,output.data());return error::success;
        }
        /** @brief Remove using the same ordered stream and an independent action. */
        error remove_owned(scene::caller who,scene::handle cap,bytes request,receipt& result)
        {
            if(!same_process())return error::denied;
            if(request.size()!=16)return error::invalid;
            std::lock_guard lock{mutex};auto checked=authorized(who,cap,remove);if(checked!=error::success)return checked;
            if(auto charged=budget(1,88,true);charged!=error::success)return charged;
            bool replay{};auto matched=stream.match(5,request,result,replay);if(matched!=error::success||replay)return matched;
            receipt next;next.key=word(request,0);next.operation=5;
            auto id=word(request,1);auto slot=std::find_if(objects.begin(),objects.end(),[&](object const& value){return value.live&&value.id==id;});
            if(slot==objects.end())next.effect_status=static_cast<unsigned>(error::bad_handle);
            else
            {
                next.object=id;next.total_bytes=slot->size;next.state=4;
                count.retained_bytes-=slot->size;--count.objects;*slot={};
            }
            stream.remember(request,next);result=next;return error::success;
        }
        /** @brief Revoke before the next linearization; committed effects remain committed. */
        void revoke(){if(!same_process())return;std::lock_guard lock{mutex};revoked=true;}
        /** @brief Inspect bounded counters only; no payload, mutation or clock advancement. */
        counters inspect() const
        {if(!same_process())return {};std::lock_guard lock{mutex};auto result=count;result.last_key=stream.last.key;return result;}
    };
}
