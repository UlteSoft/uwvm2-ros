// Licensed under APL-2.0 (see LICENSE).
#pragma once
#include <uwvm2/utils/container/standard.h>
#ifndef UWVM_MODULE
# include <array>
# include <cstddef>
# include <cstdint>
# include <span>
# include <vector>
# include "snapshot_archive.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::diagnostic::snapshot_diagnostic
{
    // Deliberately no arbitrary text/blob field: diagnostics contain logical
    // fault coordinates only. Original code is a separate static archive record.
    // No PC/SP, register value, guest value, path, environment or payload enters
    // this schema. UINT64_MAX denotes an unavailable Wasm expression offset;
    // generation zero means that exact body generation could not be captured.
    struct frame
    {
        ::std::uint64_t thread{}, ordinal{}, module{}, function{}, generation{}, offset{UINT64_MAX};
    };
    struct view
    {
        ::std::uint64_t stop{}, participants{};
        ::std::uint32_t reason{}, exit_status{};
        ::std::span<frame const> frames{};
    };
    inline constexpr ::std::uint64_t magic{0x0031474149445755ull}; // UWDIAG1\0
    inline constexpr ::std::size_t header_size{64}, frame_size{48};
    [[nodiscard]] inline bool encode(view const& source,::uwvm2::utils::container::std_vector<::std::byte>& output)
    {
        namespace wire=snapshot_archive::detail;
        if(!source.stop || !source.participants || source.frames.empty() ||
           source.frames.size()>(static_cast<::std::size_t>(PTRDIFF_MAX)-header_size)/frame_size)return false;
        ::std::uint64_t participants{},previous{},ordinal{};
        for(auto const& f:source.frames)
        {
            if(!f.thread)return false;
            if(f.thread!=previous)
            {if(f.thread<previous || f.ordinal)return false;++participants;ordinal=0;previous=f.thread;}
            if(f.ordinal!=ordinal++)return false;
        }
        if(participants!=source.participants)return false;
        ::uwvm2::utils::container::std_vector<::std::byte> bytes(header_size+source.frames.size()*frame_size);
        wire::put(bytes,0,magic,8);wire::put(bytes,8,1u,4);wire::put(bytes,12,header_size,4);
        wire::put(bytes,16,source.stop,8);wire::put(bytes,24,source.reason,4);wire::put(bytes,28,source.exit_status,4);
        wire::put(bytes,32,source.participants,8);wire::put(bytes,40,source.frames.size(),8);
        ::std::size_t at{header_size};
        for(auto const& f:source.frames)
        {
            wire::put(bytes,at,f.thread,8);wire::put(bytes,at+8,f.ordinal,8);wire::put(bytes,at+16,f.module,8);
            wire::put(bytes,at+24,f.function,8);wire::put(bytes,at+32,f.generation,8);wire::put(bytes,at+40,f.offset,8);
            at+=frame_size;
        }
        output=::std::move(bytes);return true;
    }
    [[nodiscard]] inline bool validate(::std::span<::std::byte const> bytes) noexcept
    {
        namespace wire=snapshot_archive::detail;
        if(bytes.size()<header_size || wire::get(bytes,0,8)!=magic || wire::get(bytes,8,4)!=1u ||
           wire::get(bytes,12,4)!=header_size || !wire::get(bytes,16,8) ||
           wire::get(bytes,48,8) || wire::get(bytes,56,8))return false;
        auto const count=wire::get(bytes,40,8);
        if(!count || count!=(bytes.size()-header_size)/frame_size || (bytes.size()-header_size)%frame_size)return false;
        ::std::uint64_t participants{},previous{},ordinal{};
        for(::std::size_t at=header_size;at<bytes.size();at+=frame_size)
        {
            auto const thread=wire::get(bytes,at,8), index=wire::get(bytes,at+8,8);
            if(!thread)return false;
            if(thread!=previous)
            {if(thread<previous || index)return false;++participants;ordinal=0;previous=thread;}
            if(index!=ordinal++)return false;
        }
        return participants==wire::get(bytes,32,8);
    }
}
