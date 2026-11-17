/** @file
 * @brief Owned bounded tensor/image bytes with immutable sealed-buffer leases.
 * @details The broker independently admits exact buffer actions and holds its
 * authority lease across every effect. This provider grants no authority. Each
 * alias binds one immutable schema profile and finite capacities; default zero
 * permits nothing. Guest creation always has guest provenance. No layout field
 * selects a path, native descriptor, plugin, sensor identity or device address.
 *
 * Data and object capacity is reserved before allocation. Writes are contiguous,
 * bounds checked and complete; unwritten allocation bytes are never readable.
 * Seal checks full initialization and SHA256, then the bytes become immutable.
 * Move-only host leases pin that actual allocation. Closing a guest handle does
 * not refund bytes/objects while a lease or active read still retains it.
 * Revocation rejects new lease visits, reads and operations; it cannot undo a
 * borrow that was already authorized. Providers must hold source authority
 * throughout a borrow and retain leases for asynchronous work.
 *
 * Lock order is broker authority -> registry. No delivery sink runs while the
 * registry mutex is held. Create reserves an unpublished object while copying
 * its handle; failure rolls it back. Read stages initialized bytes with a
 * separate bounded workspace reservation and never consumes the source. Failed
 * write/seal result delivery is recovered by exact retry, not a second effect.
 * Lifetime accounting is independently shared and outlives registry teardown.
 */
#pragma once
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/container/unique.h>
#include "../runtime/resources.h"
#include "byte_buffer.h"
#include "wire.h"
#ifndef UWVM_MODULE
#include <fast_io_crypto.h>
#endif
#include <algorithm>
#include <array>
#include <atomic>
#include <cstdint>
#include <limits>
#include <memory>
#include <mutex>
#include <span>
#include <string_view>
#if defined(__linux__)
#include <unistd.h>
#endif

namespace uwvm2::imported::ros::scene::buffers
{
    using error=resources::error;
    using handle=resources::handle;
    using digest=std::array<std::uint8_t,32>;
    inline constexpr std::size_t maximum_rank=8,maximum_identity=96;
    inline constexpr std::uint64_t maximum_transfer=4u*1024u*1024u;
    /** @brief Bounded owned profile text, with no allocation or native meaning. */
    template<std::size_t Capacity> struct text
    {
        std::array<char,Capacity> bytes{};std::size_t size{};
        bool assign(std::string_view value) noexcept
        {
            if(value.size()>Capacity||value.find('\0')!=value.npos)return false;
            std::copy(value.begin(),value.end(),bytes.begin());size=value.size();return true;
        }
        std::string_view view() const noexcept{return {bytes.data(),size};}
        friend bool operator==(text const& left,text const& right) noexcept{return left.view()==right.view();}
    };
    /** @brief Exact canonical registry identity and SHA256 profile digest. */
    struct profile
    {text<maximum_identity> identity;digest sha256{};friend bool operator==(profile const&,profile const&)=default;};
    enum class element_type : std::uint8_t {uint8,int8,uint16,int16,uint32,int32,uint64,int64,float16,bfloat16,float32,float64,boolean};
    inline constexpr std::array element_names{
        std::string_view{"uint8"},std::string_view{"int8"},std::string_view{"uint16"},std::string_view{"int16"},
        std::string_view{"uint32"},std::string_view{"int32"},std::string_view{"uint64"},std::string_view{"int64"},
        std::string_view{"float16"},std::string_view{"bfloat16"},std::string_view{"float32"},std::string_view{"float64"},std::string_view{"bool"}};
    inline constexpr std::array<std::uint64_t,13> element_sizes{1,1,2,2,4,4,8,8,2,2,4,8,1};
    enum class byte_order : std::uint8_t {little,big};
    enum class color_space : std::uint8_t {absent,mono,rgb,bgr,rgba,bgra,depth,yuv};
    inline constexpr std::array color_names{
        std::string_view{},std::string_view{"mono"},std::string_view{"rgb"},std::string_view{"bgr"},
        std::string_view{"rgba"},std::string_view{"bgra"},std::string_view{"depth"},std::string_view{"yuv"}};
    /** @brief Owned canonical layout; strides are byte strides.
     * @details Rank zero is one scalar. Any zero shape dimension is an empty
     * tensor with bytes zero. Nonempty layouts require nonoverlapping strides;
     * padded and transposed axes are supported, broadcast/overlap is rejected.
     */
    struct layout
    {
        profile schema;
        element_type element{};
        std::array<std::uint64_t,maximum_rank> shape{},strides{};
        std::size_t rank{};
        byte_order order{};color_space color{};
        std::uint64_t bytes{};
        friend bool operator==(layout const&,layout const&)=default;
    };
    /** @brief Stable capacity, independent of optional broker rate attenuation.
     * @details All fields default to zero. Limits count retained closed objects
     * and leases; workspace limits aggregate active detached read copies. Schema,
     * dtype/order/color masks and rank/elements are trusted deployment bindings.
     */
    struct plan
    {
        profile schema;
        std::uint64_t objects{},bytes{},object_bytes{},transfer_bytes{},workspace_bytes{},leases{},elements{};
        std::uint64_t element_mask{},order_mask{},color_mask{};
        std::size_t rank{};
    };
    /** @brief Cached public description; provenance is always guest for create(). */
    struct information
    {layout value;bool sealed{};std::string_view provenance{"guest"};digest sha256{};std::uint64_t written{};};
    /** @brief Retained data/object/lease/workspace charges, including closed handles. */
    struct usage
    {std::uint64_t bytes{},objects{},leases{},workspace{},active_handles{};};
    inline bool valid(plan const& value) noexcept
    {
        return value.schema.identity.size&&value.schema.identity.size<=maximum_identity&&
            value.schema.identity.view().find('\0')==std::string_view::npos&&
            wire::utf8({reinterpret_cast<std::uint8_t const*>(value.schema.identity.bytes.data()),value.schema.identity.size})&&
            value.objects!=UINT64_MAX&&value.bytes<=INT64_MAX&&value.object_bytes<=value.bytes&&
            value.transfer_bytes<=maximum_transfer&&value.workspace_bytes<=INT64_MAX&&value.leases!=UINT64_MAX&&
            value.elements<=INT64_MAX&&value.rank<=maximum_rank&&!(value.element_mask&~((std::uint64_t{1}<<element_names.size())-1))&&
            !(value.order_mask&~std::uint64_t{3})&&!(value.color_mask&~std::uint64_t{255});
    }
    /** @brief Reject unknown types, overflow, overlap and unapproved profiles before allocation. */
    inline error validate(layout const& value,plan const& policy) noexcept
    {
        auto element=static_cast<unsigned>(value.element),order=static_cast<unsigned>(value.order),color=static_cast<unsigned>(value.color);
        if(element>=element_names.size()||order>1||color>=color_names.size()||value.rank>maximum_rank)return error::invalid;
        if(value.schema.identity.size>maximum_identity||value.schema!=policy.schema||
           !(policy.element_mask&(std::uint64_t{1}<<element))||!(policy.order_mask&(std::uint64_t{1}<<order))||
           !(policy.color_mask&(std::uint64_t{1}<<color)))return error::denied;
        if(value.rank>policy.rank||value.bytes>policy.object_bytes||value.bytes>SIZE_MAX)return error::too_large;
        std::uint64_t count{1};bool empty{};
        for(std::size_t i{};i!=value.rank;++i)
        {
            if(value.shape[i]>policy.elements)return error::too_large;
            if(!value.shape[i])empty=true;
            if(!empty&&value.shape[i]>policy.elements/count)return error::too_large;
            if(!empty)count*=value.shape[i];
        }
        if(empty)return value.bytes?error::invalid:error::success;
        if(count>policy.elements)return error::too_large;
        std::array<std::size_t,maximum_rank> axes{};
        for(std::size_t i{};i!=value.rank;++i)axes[i]=i;
        std::sort(axes.begin(),axes.begin()+value.rank,[&](auto left,auto right){return value.strides[left]<value.strides[right];});
        auto extent=element_sizes[element];
        for(std::size_t k{};k!=value.rank;++k)
        {
            auto i=axes[k];if(value.shape[i]<=1)continue;
            if(value.strides[i]<extent)return error::invalid;
            auto repeats=value.shape[i]-1;
            if(value.strides[i]>(UINT64_MAX-extent)/repeats)return error::too_large;
            extent+=value.strides[i]*repeats;
        }
        return extent<=value.bytes?error::success:error::invalid;
    }
    /** @brief Portable SHA256 over fully initialized content only. */
    inline digest hash(std::span<std::uint8_t const> bytes) noexcept
    {
        ::fast_io::sha256_context context{};digest output{};
        auto first=reinterpret_cast<std::byte const*>(bytes.data());
        if(!bytes.empty())context.update(first,first+bytes.size());
        context.do_final();context.digest_to_byte_ptr(reinterpret_cast<std::byte*>(output.data()));return output;
    }

    class registry
    {
        struct account
        {
            plan bounds;
            std::atomic<std::uint64_t> bytes{},objects{},leases{},workspace{};
            std::atomic<bool> retired{};
#if defined(__linux__)
            ::pid_t process{::getpid()};
#endif
            explicit account(plan value):bounds{std::move(value)}{}
        };
        struct object
        {
            std::shared_ptr<account> owner;
            handle id{};layout format;
            ::uwvm2::utils::container::std_unique_ptr<std::uint8_t[]> data;
            ::fast_io::sha256_context hashing{};
            std::uint64_t written{};digest sha256{};bool sealed{},published{};
            object(std::shared_ptr<account> scope,layout value):owner{std::move(scope)},format{std::move(value)}{}
            ~object()
            {data.reset();owner->bytes.fetch_sub(format.bytes,std::memory_order_relaxed);owner->objects.fetch_sub(1,std::memory_order_relaxed);}
        };
        struct reservation
        {
            std::shared_ptr<account> owner;std::uint64_t bytes{};bool active{true};
            ~reservation(){if(active){owner->bytes.fetch_sub(bytes,std::memory_order_relaxed);owner->objects.fetch_sub(1,std::memory_order_relaxed);}}
        };
        struct workspace_reservation
        {
            std::shared_ptr<account> owner;std::uint64_t bytes{};
            ~workspace_reservation(){if(owner)owner->workspace.fetch_sub(bytes,std::memory_order_relaxed);}
        };
        mutable std::mutex mutex;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,std::shared_ptr<account>,std::less<>> bindings;
        ::uwvm2::utils::container::std_map<handle,std::shared_ptr<object>> objects;
        bool retired{};
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
        std::shared_ptr<account> binding(std::string_view alias) const noexcept
        {
            auto it=bindings.find(alias);
            return retired||it==bindings.end()||it->second->retired.load(std::memory_order_acquire)?nullptr:it->second;
        }
        std::shared_ptr<object> owned(std::string_view alias,handle id) const noexcept
        {
            auto scope=binding(alias);auto it=objects.find(id);
            return !scope||it==objects.end()||!it->second->published||it->second->owner!=scope?nullptr:it->second;
        }
        usage sample(std::shared_ptr<account> const& scope) const noexcept
        {
            usage output{scope->bytes.load(),scope->objects.load(),scope->leases.load(),scope->workspace.load()};
            for(auto const& [id,value]:objects)
            {static_cast<void>(id);if(value->owner==scope&&value->published)++output.active_handles;}
            return output;
        }
    public:
        /** @brief Move-only pin of actual sealed bytes and their retained quota.
         * @details Only pin() creates a lease. A lease holds no registry pointer,
         * so it survives handle close and registry destruction safely. Retired
         * authority rejects further visits; already disclosed bytes cannot be
         * recalled. The host callback must not retain its span past the visit.
         */
        class sealed_lease
        {
            friend class registry;
            std::shared_ptr<object> value;
            void release() noexcept
            {if(value){value->owner->leases.fetch_sub(1,std::memory_order_relaxed);value.reset();}}
        public:
            sealed_lease()=default;sealed_lease(sealed_lease const&)=delete;sealed_lease& operator=(sealed_lease const&)=delete;
            sealed_lease(sealed_lease&& other) noexcept:value{std::move(other.value)}{}
            sealed_lease& operator=(sealed_lease&& other) noexcept
            {if(this!=&other){release();value=std::move(other.value);}return *this;}
            ~sealed_lease(){release();}
            explicit operator bool() const noexcept{return static_cast<bool>(value);}
            handle identity() const noexcept{return value?value->id:0;}
            /** @brief Borrow immutable bytes under independently held source authority. */
            template<typename Visitor> error visit(Visitor&& visitor) const
            {
                if(!value||value->owner->retired.load(std::memory_order_acquire))return error::denied;
#if defined(__linux__)
                if(value->owner->process!=::getpid())return error::denied;
#endif
                auto retained=value;
                try
                {
                    if(!visitor(std::span<std::uint8_t const>{retained->data.get(),static_cast<std::size_t>(retained->format.bytes)},
                                std::as_const(retained->format),std::as_const(retained->sha256)))return error::io;
                }
                catch(...){return error::io;}
                return retained->owner->retired.load(std::memory_order_acquire)?error::cancelled:error::success;
            }
        };
        /** @brief Unpublished immutable output, with its bytes, object and pin already reserved.
         * @details This synchronous transaction must not outlive its registry.
         * Ordinary handle operations cannot observe it. Destruction rolls back;
         * publish_sealed() is the sole allocation-free publication point. The
         * caller holds all source and destination authority through publication.
         */
        class sealed_creation
        {
            friend class registry;
            registry* owner{};sealed_lease pin;
        public:
            sealed_creation()=default;sealed_creation(sealed_creation const&)=delete;
            sealed_creation& operator=(sealed_creation const&)=delete;
            ~sealed_creation()
            {
                if(owner&&owner->same_process())
                {std::lock_guard lock{owner->mutex};owner->objects.erase(pin.identity());}
            }
            handle identity()const noexcept{return pin.identity();}
        };
        /** @brief Stage a completely initialized sealed output without disclosing a handle.
         * @details No guest callback runs here. Quotas and exact layout/transfer
         * limits precede recoverable allocation. Failure leaves the transaction
         * empty; all reservations, including the output pin, are rolled back.
         */
        error stage_sealed(std::string_view alias,layout const& format,std::uint64_t maximum,
                           std::span<std::uint8_t const> input,sealed_creation& output)
        {
            if(!same_process())return error::denied;
            if(output.owner||output.pin)return error::conflict;
            std::lock_guard lock{mutex};auto scope=binding(alias);if(!scope)return error::denied;
            auto status=validate(format,scope->bounds);if(status!=error::success)return status;
            if(input.size()!=format.bytes)return error::invalid;
            if(input.size()>scope->bounds.transfer_bytes)return error::too_large;
            auto count=scope->objects.load(),used=scope->bytes.load();
            if(count>=scope->bounds.objects||count>=maximum||used>scope->bounds.bytes||
               format.bytes>scope->bounds.bytes-used||scope->leases.load()>=scope->bounds.leases)return error::again;
            scope->bytes.fetch_add(format.bytes);scope->objects.fetch_add(1);reservation reserved{scope,format.bytes};
            auto value=::uwvm2::utils::container::make_shared<object>(scope,format);reserved.active=false;
            value->data=::uwvm2::utils::container::make_unique_for_overwrite<std::uint8_t[]>(input.size());
            if(!input.empty())std::copy(input.begin(),input.end(),value->data.get());
            value->written=input.size();value->sha256=hash(input);value->sealed=true;
            value->id=resources::allocate_identity();if(!value->id)return error::too_large;
            objects.emplace(value->id,value);scope->leases.fetch_add(1);
            output.pin.value=std::move(value);output.owner=this;return error::success;
        }
        /** @brief Publish staged bytes and transfer the reserved pin without allocation or callbacks. */
        error publish_sealed(sealed_creation& input,sealed_lease& output)noexcept
        {
            if(!same_process())return error::denied;
            if(input.owner!=this||!input.pin||output)return error::invalid;
            std::lock_guard lock{mutex};auto const& value=input.pin.value;
            if(retired||value->owner->retired.load()||!objects.contains(value->id))return error::cancelled;
            value->published=true;output=std::move(input.pin);input.owner=nullptr;return error::success;
        }
        /** @brief Move-only charge for a detached input carrier's heap allocation.
         * @details Holds shared accounting, never a registry pointer. Declare
         * before the owned byte buffer so storage is freed before its charge.
         * Revocation and fork reject live use but do not prevent cleanup. This
         * lease contains no bytes and confers no action or source authority.
         */
        class workspace_lease
        {
            friend class registry;
            std::shared_ptr<account> owner;std::uint64_t bytes{};
        public:
            workspace_lease()=default;workspace_lease(workspace_lease const&)=delete;workspace_lease& operator=(workspace_lease const&)=delete;
            workspace_lease(workspace_lease&& other) noexcept:owner{std::move(other.owner)},bytes{std::exchange(other.bytes,0)}{}
            workspace_lease& operator=(workspace_lease&& other) noexcept
            {if(this!=&other){reset();owner=std::move(other.owner);bytes=std::exchange(other.bytes,0);}return *this;}
            ~workspace_lease(){reset();}
            explicit operator bool() const noexcept{return static_cast<bool>(owner);}
            std::uint64_t reserved_bytes() const noexcept{return owner?bytes:0;}
            bool live() const noexcept
            {
                if(!owner||owner->retired.load(std::memory_order_acquire))return false;
#if defined(__linux__)
                if(owner->process!=::getpid())return false;
#endif
                return true;
            }
            void reset() noexcept
            {if(owner){owner->workspace.fetch_sub(bytes,std::memory_order_relaxed);owner.reset();bytes=0;}}
        };
        /** @brief Reserve complete heap carrier bytes before copying guest input.
         * @details Broker authority and action-specific bounds must already be
         * checked. This charges the same finite workspace as detached reads,
         * so concurrent input/output copies cannot each use the full budget.
         * No allocation or payload access occurs here. Zero, overflow, an
         * already occupied token and retired aliases fail without any charge.
         */
        error reserve_workspace(std::string_view alias,std::uint64_t bytes,workspace_lease& output)
        {
            if(!same_process())return error::denied;
            if(output||!bytes||bytes>SIZE_MAX)return error::invalid;
            std::lock_guard lock{mutex};auto scope=binding(alias);if(!scope)return error::denied;
            auto used=scope->workspace.load(std::memory_order_relaxed);
            if(used>scope->bounds.workspace_bytes||bytes>scope->bounds.workspace_bytes-used)return error::again;
            scope->workspace.fetch_add(bytes,std::memory_order_relaxed);output.owner=std::move(scope);output.bytes=bytes;
            return error::success;
        }
        registry()=default;registry(registry const&)=delete;registry& operator=(registry const&)=delete;
        ~registry(){retire();}
        /** @brief Bind one exact immutable schema and capacity before guest activation. */
        error bind(::uwvm2::utils::container::std_string alias,plan policy)
        {
            if(!same_process())return error::denied;
            if(alias.empty()||alias.size()>255||alias.find('\0')!=alias.npos||!valid(policy))return error::invalid;
            std::lock_guard lock{mutex};if(retired)return error::denied;
            if(bindings.contains(alias))return error::conflict;
            bindings.emplace(std::move(alias),::uwvm2::utils::container::make_shared<account>(std::move(policy)));return error::success;
        }
        /** @brief Reserve and allocate one buffer, publishing only after handle delivery.
         * @details max_resources is the broker grant's independent finite object
         * bound. The sink runs outside the mutex and cannot use the unpublished
         * handle. Failure/throw/revocation rolls back the object and reservations.
         */
        template<typename Sink> error create(std::string_view alias,layout const& format,std::uint64_t max_resources,Sink&& sink)
        {
            if(!same_process())return error::denied;
            std::shared_ptr<object> value;
            {
                std::lock_guard lock{mutex};auto scope=binding(alias);if(!scope)return error::denied;
                auto status=validate(format,scope->bounds);if(status!=error::success)return status;
                auto used=scope->bytes.load(std::memory_order_relaxed),count=scope->objects.load(std::memory_order_relaxed);
                if(count>=scope->bounds.objects||count>=max_resources||format.bytes>scope->bounds.bytes-used)return error::again;
                scope->bytes.fetch_add(format.bytes,std::memory_order_relaxed);scope->objects.fetch_add(1,std::memory_order_relaxed);
                reservation reserved{scope,format.bytes};
                value=::uwvm2::utils::container::make_shared<object>(scope,format);reserved.active=false;
                value->data=::uwvm2::utils::container::make_unique_for_overwrite<std::uint8_t[]>(static_cast<std::size_t>(format.bytes));
                value->id=resources::allocate_identity();if(!value->id)return error::too_large;
                objects.emplace(value->id,value);
            }
            bool delivered{};try{delivered=sink(value->id);}catch(...){delivered=false;}
            std::lock_guard lock{mutex};auto scope=binding(alias);
            if(!delivered||scope!=value->owner)
            {objects.erase(value->id);return delivered?error::cancelled:error::io;}
            value->published=true;return error::success;
        }
        /** @brief Write a complete contiguous range; exact previous bytes are replayable.
         * @details No gaps, partial writes or mutation after seal. Repeating a
         * fully written identical range returns its accepted size for delivery
         * recovery. Different contents or a range crossing the cursor conflict.
         */
        error write(std::string_view alias,handle id,std::uint64_t expected_offset,std::span<std::uint8_t const> input,std::uint64_t& accepted)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=owned(alias,id);if(!value)return error::bad_handle;
            if(value->sealed)return error::denied;
            if(input.size()>value->owner->bounds.transfer_bytes||expected_offset>value->format.bytes||input.size()>value->format.bytes-expected_offset)return error::too_large;
            if(expected_offset!=value->written)
            {
                if(expected_offset>value->written||input.size()>value->written-expected_offset)return error::conflict;
                if(!std::equal(input.begin(),input.end(),value->data.get()+static_cast<std::size_t>(expected_offset)))return error::conflict;
            }
            else
            {
                if(!input.empty())
                {
                    std::copy(input.begin(),input.end(),value->data.get()+static_cast<std::size_t>(expected_offset));
                    auto first=reinterpret_cast<std::byte const*>(input.data());value->hashing.update(first,first+input.size());
                }
                value->written+=input.size();
            }
            accepted=input.size();return error::success;
        }
        /** @brief Publish immutable bytes only after complete initialization and exact SHA256. */
        error seal(std::string_view alias,handle id,digest const& expected)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=owned(alias,id);if(!value)return error::bad_handle;
            if(value->sealed)return value->sha256==expected?error::success:error::conflict;
            if(value->written!=value->format.bytes)return error::conflict;
            auto context=value->hashing;digest actual{};context.do_final();
            context.digest_to_byte_ptr(reinterpret_cast<std::byte*>(actual.data()));
            if(actual!=expected)return error::conflict;
            value->sha256=actual;value->sealed=true;return error::success;
        }
        /** @brief Read an exact initialized span without consuming state or exposing unwritten memory. */
        template<typename Sink> error read(std::string_view alias,handle id,std::uint64_t offset,std::uint64_t bytes,Sink&& sink)
        {
            if(!same_process())return error::denied;
            workspace_reservation workspace;byte_buffer copy;
            {
                std::lock_guard lock{mutex};auto value=owned(alias,id);if(!value)return error::bad_handle;
                if(bytes>value->owner->bounds.transfer_bytes||offset>value->written||bytes>value->written-offset||bytes>SIZE_MAX)return error::too_large;
                auto used=value->owner->workspace.load(std::memory_order_relaxed);
                if(bytes>value->owner->bounds.workspace_bytes-used)return error::again;
                workspace.owner=value->owner;workspace.bytes=bytes;workspace.owner->workspace.fetch_add(bytes,std::memory_order_relaxed);
                copy.resize_for_overwrite(static_cast<std::size_t>(bytes));
                if(bytes)std::copy_n(value->data.get()+static_cast<std::size_t>(offset),static_cast<std::size_t>(bytes),copy.writable().data());
            }
            bool delivered{};try{delivered=sink(copy.view());}catch(...){delivered=false;}
            if(!delivered)return error::io;
            return workspace.owner->retired.load(std::memory_order_acquire)?error::cancelled:error::success;
        }
        /** @brief Return owned bounded metadata; guest provenance cannot be relabelled. */
        error describe(std::string_view alias,handle id,information& output)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=owned(alias,id);if(!value)return error::bad_handle;
            output={value->format,value->sealed,"guest",value->sha256,value->written};return error::success;
        }
        /** @brief Reserve a finite move-only pin after independent buffer.read admission. */
        error pin(std::string_view alias,handle id,sealed_lease& output)
        {
            if(!same_process())return error::denied;
            if(output)return error::conflict;
            std::lock_guard lock{mutex};auto value=owned(alias,id);if(!value)return error::bad_handle;
            if(!value->sealed)return error::denied;
            auto count=value->owner->leases.load(std::memory_order_relaxed);
            if(count>=value->owner->bounds.leases)return error::again;
            value->owner->leases.fetch_add(1,std::memory_order_relaxed);output.value=value;return error::success;
        }
        /** @brief Close the guest handle; outstanding leases keep all object/data charges. */
        error close(std::string_view alias,handle id)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=owned(alias,id);if(!value)return error::bad_handle;
            objects.erase(id);return error::success;
        }
        /** @brief Inspect retained accounting without allocating or reading bytes. */
        error inspect(std::string_view alias,usage& output) const
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto it=bindings.find(alias);if(it==bindings.end())return error::bad_handle;
            output=sample(it->second);return error::success;
        }
        /** @brief Nonblocking debugger accounting; no state advance, payload or identifiers.
         * @details Lock contention returns again and leaves output unchanged.
         * Retired scopes remain inspectable. Atomic counters may reflect a
         * concurrent final lease release; this is diagnosis, not admission.
         */
        error try_inspect(std::string_view alias,usage& output) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            auto it=bindings.find(alias);if(it==bindings.end())return error::bad_handle;
            output=sample(it->second);return error::success;
        }
        /** @brief Read the exact immutable transfer ceiling before host request copying.
         * @details This performs no allocation, reservation, rate charge or I/O.
         * Broker computes per-operation request bounds using buffer_contract.
         */
        error transfer_limit(std::string_view alias,std::uint64_t& output) const noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto scope=binding(alias);if(!scope)return error::denied;
            output=scope->bounds.transfer_bytes;return error::success;
        }
        /** @brief Revoke a scope and close its guest handles; retained pins become unreadable. */
        void revoke(std::string_view alias)
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};auto it=bindings.find(alias);if(it==bindings.end())return;
            it->second->retired.store(true,std::memory_order_release);
            for(auto object=objects.begin();object!=objects.end();)
                if(object->second->owner==it->second)object=objects.erase(object);else ++object;
        }
        /** @brief Retire before destruction; pinned allocations outlive registry storage safely. */
        void retire() noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};retired=true;
            for(auto& [alias,value]:bindings){static_cast<void>(alias);value->retired.store(true,std::memory_order_release);}
            objects.clear();
        }
    };
}
