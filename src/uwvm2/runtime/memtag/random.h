/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <array>
# include <cerrno>
# include <cstddef>
# include <cstdint>
# include <uwvm2/utils/macro/push_macros.h>
# if defined(__linux__)
#  include <pthread.h>
#  include <uwvm2/utils/control/posix_abi.h>
# endif
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::memtag
{
    namespace details
    {
        struct tag_entropy_buffer
        {
            ::std::array<::std::uint8_t,256u> bytes{};
            ::std::size_t position{bytes.size()};
        };
        // VM-private OS entropy; no guest interface permission or address controls
        // this state. Buffering avoids a syscall per random-tag instruction.
        inline constinit thread_local tag_entropy_buffer entropy_buffer{};
#if defined(UWVM_TEST) && defined(__linux__)
        inline ::ssize_t (*entropy_read_test_hook)(void*, ::std::size_t, unsigned){};
#endif
#if defined(__linux__)
        inline void discard_entropy_after_fork() noexcept
        { entropy_buffer.position = entropy_buffer.bytes.size(); }
#endif
    }

    // Called before borrowing a native memory, never while holding its grow pin.
    // Entropy failure is explicit: no time/address/counter fallback can turn a
    // failed host entropy source into predictable allocation tags.
    [[nodiscard]] inline bool next_random_tag(::std::uint8_t& tag) noexcept
    {
#if defined(__linux__)
        auto& buffer{details::entropy_buffer};
        if(buffer.position == buffer.bytes.size()) [[unlikely]]
        {
            static bool const fork_hook_registered{
                ::pthread_atfork(nullptr,nullptr,details::discard_entropy_after_fork) == 0};
            if(!fork_hook_registered) { return false; }
            ::std::size_t read{};
            while(read != buffer.bytes.size())
            {
                auto const result{
#if defined(UWVM_TEST)
                    details::entropy_read_test_hook != nullptr ?
                        details::entropy_read_test_hook(buffer.bytes.data()+read,buffer.bytes.size()-read,0u) :
#endif
                        ::uwvm2::utils::control::posix_abi::getrandom_noexcept(
                            buffer.bytes.data()+read,buffer.bytes.size()-read,0u)};
                if(result < 0)
                {
                    if(errno == EINTR) { continue; }
                    return false;
                }
                if(result == 0 || static_cast<::std::size_t>(result) > buffer.bytes.size()-read) { return false; }
                read += static_cast<::std::size_t>(result);
            }
            buffer.position = 0u;
        }
        tag = buffer.bytes[buffer.position++];
        return true;
#else
        static_cast<void>(tag);
        return false;
#endif
    }
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
