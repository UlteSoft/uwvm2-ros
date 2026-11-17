// Licensed under APL-2.0 (see LICENSE).
#pragma once
#include <uwvm2/utils/container/standard.h>
#ifndef UWVM_MODULE
# include "checkpoint_replay.h"
# include <fast_io_crypto.h>
# include <cstring>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::debugger::checkpoint
{
    // These spans are synchronous HOST borrows, never addresses decoded from an
    // event. The runtime adapter must pin a detached replay world's memory and
    // exclude other writers for the entire prepare/commit operation. This codec
    // neither issues that borrow nor admits any host call or external resource.
    struct replay_memory_image
    {
        object_id memory{};
        ::std::span<::std::byte const> bytes{};
    };
    struct replay_memory_view
    {
        object_id memory{};
        ::std::span<::std::byte> bytes{};
    };
    namespace replay_memory_detail
    {
        inline constexpr ::std::array<::std::byte,8u> magic{
            ::std::byte{'U'},::std::byte{'W'},::std::byte{'R'},::std::byte{'M'},
            ::std::byte{'E'},::std::byte{'M'},::std::byte{'1'},::std::byte{}};
        using digest = ::std::array<::std::byte,32u>;
        [[nodiscard]] inline digest finish(::fast_io::sha256_context& hash) noexcept
        {
            hash.do_final();digest out{};
            hash.digest_to_byte_ptr(out.data());return out;
        }
        [[nodiscard]] inline digest hash(::std::span<::std::byte const> bytes) noexcept
        {
            ::fast_io::sha256_context context{};
            if(!bytes.empty())context.update(bytes.data(),bytes.data()+bytes.size());
            return finish(context);
        }
        struct reader
        {
            ::std::span<::std::byte const> remaining{};
            [[nodiscard]] bool take(::std::uint64_t count,::std::span<::std::byte const>& value) noexcept
            {
                if(count>remaining.size())return false;
                value=remaining.first(static_cast<::std::size_t>(count));
                remaining=remaining.subspan(static_cast<::std::size_t>(count));return true;
            }
            [[nodiscard]] bool number(::std::uint64_t& value) noexcept
            {
                ::std::span<::std::byte const> bytes{};
                if(!take(8u,bytes))return false;
                value=0u;
                for(unsigned i{};i!=8u;++i)value|=static_cast<::std::uint64_t>(bytes[i])<<(i*8u);
                return true;
            }
        };
        struct writer
        {
            ::uwvm2::utils::container::std_vector<::std::byte> bytes{};
            ::std::uint64_t maximum{};
            [[nodiscard]] bool append(::std::span<::std::byte const> value)
            {
                if(bytes.size()>maximum || value.size()>maximum-bytes.size() ||
                   value.size()>bytes.max_size()-bytes.size())return false;
                bytes.insert(bytes.end(),value.begin(),value.end());return true;
            }
            [[nodiscard]] bool number(::std::uint64_t value)
            {
                ::std::array<::std::byte,8u> data{};
                for(unsigned i{};i!=8u;++i)data[i]=static_cast<::std::byte>(value>>(i*8u));
                return append(data);
            }
            void replace_number(::std::size_t offset,::std::uint64_t value) noexcept
            {
                // Only a previously emitted fixed-width field reaches here.
                for(unsigned i{};i!=8u;++i)bytes[offset+i]=static_cast<::std::byte>(value>>(i*8u));
            }
        };
        template<typename View>
        [[nodiscard]] inline bool ordered(::std::span<View const> views) noexcept
        {
            object_id previous{};
            for(auto const& view:views)
            {
                if(!view.memory || view.memory<=previous)return false;
                previous=view.memory;
            }
            return true;
        }
        [[nodiscard]] inline bool disjoint(::std::span<replay_memory_view const> views) noexcept
        {
            for(::std::size_t i{};i!=views.size();++i)
            {
                auto const& left=views[i].bytes;
                if(left.empty())continue;
                auto const begin=reinterpret_cast<::std::uintptr_t>(left.data());
                if(!begin || left.size()>UINTPTR_MAX-begin)return false;
                auto const end=begin+left.size();
                for(::std::size_t j{};j!=i;++j)
                {
                    auto const& right=views[j].bytes;
                    if(right.empty())continue;
                    auto const other=reinterpret_cast<::std::uintptr_t>(right.data());
                    // Its extent was checked in the earlier outer iteration.
                    if(begin<other+right.size() && other<end)return false;
                }
            }
            return true;
        }
        struct write
        {
            ::std::span<::std::byte> target{};
            ::std::span<::std::byte const> value{};
        };
        // Nothing in this preparation mutates guest state. Every view, input
        // digest, patch extent/order, output digest and trailing byte is checked
        // before the caller can publish even the first changed memory byte.
        [[nodiscard]] inline error prepare(::std::span<::std::byte const> payload,
            ::std::span<replay_memory_view const> memories,::uwvm2::utils::container::std_vector<write>& writes)
        {
            if(!ordered(memories) || !disjoint(memories))return error::invalid_reference;
            reader input{payload};::std::span<::std::byte const> tag{};
            ::std::uint64_t count{};
            if(!input.take(magic.size(),tag) || !input.number(count))return error::truncated;
            if(!::std::equal(tag.begin(),tag.end(),magic.begin()))return error::unsupported_version;
            if(count!=memories.size())return error::replay_diverged;
            if(count>input.remaining.size()/88u)return error::truncated;
            for(auto const& memory:memories)
            {
                ::std::uint64_t id{},length{},patches{};
                ::std::span<::std::byte const> before{},after{};
                if(!input.number(id) || !input.number(length) || !input.take(32u,before) ||
                   !input.take(32u,after) || !input.number(patches))return error::truncated;
                if(id!=memory.memory || length!=memory.bytes.size())return error::replay_diverged;
                auto const observed=hash(memory.bytes);
                if(!::std::equal(observed.begin(),observed.end(),before.begin()))return error::replay_diverged;
                // Each patch has two u64 fields and at least one payload byte.
                if(patches>input.remaining.size()/17u || patches>writes.max_size()-writes.size())return error::truncated;
                ::fast_io::sha256_context projected{};::std::size_t cursor{};
                for(::std::uint64_t index{};index!=patches;++index)
                {
                    ::std::uint64_t offset{},size{};::std::span<::std::byte const> value{};
                    if(!input.number(offset) || !input.number(size))return error::truncated;
                    if(!size || offset>length || size>length-offset ||
                       (index!=0u && offset<=cursor))return error::invalid_shape;
                    if(!input.take(size,value))return error::truncated;
                    auto const position=static_cast<::std::size_t>(offset);
                    auto const bytes=static_cast<::std::size_t>(size);
                    if(position!=cursor)projected.update(memory.bytes.data()+cursor,memory.bytes.data()+position);
                    projected.update(value.data(),value.data()+value.size());
                    writes.push_back({memory.bytes.subspan(position,bytes),value});
                    cursor=position+bytes; // subtraction proof above precedes sum
                }
                if(cursor!=memory.bytes.size())projected.update(memory.bytes.data()+cursor,memory.bytes.data()+memory.bytes.size());
                auto const result=finish(projected);
                if(!::std::equal(result.begin(),result.end(),after.begin()))return error::digest_mismatch;
            }
            return input.remaining.empty()?error::none:error::trailing_bytes;
        }
    }
    // Portable before/after DATA for one authenticated host call. Different
    // memory objects are explicit even when their module-local indices agree.
    // Extent changes require a resource adapter; this codec never grows/shrinks
    // an allocation or silently drops a second memory. On failure output is
    // unchanged. There is no implicit recording quota; hosts may set one.
    [[nodiscard]] inline error encode_replay_memory_effect(
        ::std::span<replay_memory_image const> before,::std::span<replay_memory_image const> after,
        ::uwvm2::utils::container::std_vector<::std::byte>& output,
        ::std::uint64_t maximum_payload=(::std::numeric_limits<::std::uint64_t>::max)())
    {
        namespace detail=replay_memory_detail;
        if(!detail::ordered(before) || !detail::ordered(after) || before.size()!=after.size())return error::invalid_reference;
        detail::writer wire{{},maximum_payload};
        if(!wire.append(detail::magic) || !wire.number(before.size()))return error::limit_exceeded;
        for(::std::size_t i{};i!=before.size();++i)
        {
            auto const& old=before[i];auto const& next=after[i];
            if(old.memory!=next.memory || old.bytes.size()!=next.bytes.size())return error::replay_diverged;
            auto const original=detail::hash(old.bytes);
            bool const unchanged=::std::equal(old.bytes.begin(),old.bytes.end(),next.bytes.begin());
            auto const updated=unchanged?original:detail::hash(next.bytes);
            if(!wire.number(old.memory) || !wire.number(old.bytes.size()) ||
               !wire.append(original) || !wire.append(updated))return error::limit_exceeded;
            auto const count_offset=wire.bytes.size();
            if(!wire.number(0u))return error::limit_exceeded;
            ::std::uint64_t patches{};
            if(!unchanged)
            {
                ::std::size_t offset{};
                while(offset!=old.bytes.size())
                {
                    auto const difference=::std::mismatch(old.bytes.begin()+offset,old.bytes.end(),next.bytes.begin()+offset);
                    offset=static_cast<::std::size_t>(difference.first-old.bytes.begin());
                    if(offset==old.bytes.size())break;
                    auto end=offset+1u;
                    while(end!=old.bytes.size() && old.bytes[end]!=next.bytes[end])++end;
                    if(!wire.number(offset) || !wire.number(end-offset) ||
                       !wire.append(next.bytes.subspan(offset,end-offset)))return error::limit_exceeded;
                    ++patches;offset=end;
                }
            }
            wire.replace_number(count_offset,patches);
        }
        output=::std::move(wire.bytes);return error::none;
    }
    // The original log owns the immutable payload and serializes matching,
    // preparation and commit. The actual adapter must validate typed results,
    // its detached resources and current authority in Prepare. Commit must be
    // nonthrowing and may update ONLY that detached world. No native provider is
    // invoked, no effect is repeated, and a rejected final patch consumes no
    // event or earlier write. Native spans never survive this synchronous call.
    template<typename Prepare,typename Commit>
    [[nodiscard]] inline replay_result apply_replay_memory_effect(replay_log& log,
        replay_request const& request,::std::span<replay_memory_view const> memories,
        Prepare&& prepare,Commit&& commit)
    {
        static_assert(noexcept(::std::forward<Commit>(commit)(::std::declval<replay_result const&>())));
        ::uwvm2::utils::container::std_vector<replay_memory_detail::write> writes{};
        return log.apply(request,[&](replay_result const& matched)
        {
            // Authorize the actual borrowed world/results BEFORE reading any
            // byte through the supplied views. A wire memory ID cannot select a
            // different capability from this exact matched request.
            for(auto const& memory:memories)
                if(::std::find(request.capability_ids.begin(),request.capability_ids.end(),memory.memory)==request.capability_ids.end())
                    return error::invalid_reference;
            auto const admitted=::std::forward<Prepare>(prepare)(matched);
            if(admitted!=error::none)return admitted;
            return replay_memory_detail::prepare(matched.adapter_payload,memories,writes);
        },[&](replay_result const& matched) noexcept
        {
            for(auto const& entry:writes)::std::memcpy(entry.target.data(),entry.value.data(),entry.value.size());
            ::std::forward<Commit>(commit)(matched);
        });
    }
}
