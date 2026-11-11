// Explicit diagnostic mode: preserve every detected violation and continue.
// Ordinary bounds, owner identity and host capabilities are never bypassed.
#pragma once
#ifndef UWVM_MODULE
# include <mutex>
# include <cstdio>
# include <cstdlib>
# include <uwvm2/extension/memtag/execution_policy.h>
# include <uwvm2/extension/memtag/shadow.h>
# include <uwvm2/runtime/diagnostic/crash_policy.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::memtag::report
{
    inline ::std::mutex output_mutex{};
    inline ::std::uint64_t next_sequence{};
    [[nodiscard]] inline bool write(char const* bytes,::std::size_t count) noexcept
    {
#if defined(__linux__)
        if(::uwvm2::extension::memtag::report_regular_file)
        {
            // A pinned regular-file description cannot produce a pipe's
            // SIGPIPE. Keep exact partial-write/EINTR/error handling without
            // three signal-mask syscalls for every JSON record.
            int const saved_errno{errno};::std::size_t sent{};unsigned interrupted{};
            while(sent<count)
            {
                auto const written{::write(::uwvm2::extension::memtag::report_descriptor,bytes+sent,count-sent)};
                if(written>0) { sent+=static_cast<::std::size_t>(written); }
                else if(written<0 && errno==EINTR && ++interrupted<16u) { continue; }
                else { break; }
            }
            errno=saved_errno;return sent==count;
        }
        return ::uwvm2::runtime::diagnostic::crash::write_descriptor(
            ::uwvm2::extension::memtag::report_descriptor,bytes,count);
#else
        // Native descriptor diagnostics are Linux-specific. The portable cold
        // logger still supports explicitly selected out/err on libc hosts.
        auto const descriptor{::uwvm2::extension::memtag::report_descriptor};
        auto* output{descriptor==1 ? stdout:descriptor==2 ? stderr:nullptr};
        return output && ::std::fwrite(bytes,1u,count,output)==count && ::std::fflush(output)==0;
#endif
    }
    // No owner pointers, guest text or lazy views reach this sink. Serializing
    // complete records prevents interleaved output from admitted Wasm threads.
    [[gnu::noinline,gnu::cold]] inline void violation(::uwvm2::extension::memtag::access_result const& access,
        ::std::uint64_t raw,::std::uint64_t memory,::std::uint64_t module=UINT64_MAX,
        ::std::uint64_t function=UINT64_MAX,::std::uint64_t offset=UINT64_MAX,
        bool asynchronous=false) noexcept
    {
        namespace crash=::uwvm2::runtime::diagnostic::crash;
        ::std::lock_guard lock{output_mutex};
        // No unused stack bytes leave this sink: append/number/optional write
        // the entire [0,size) prefix; write() receives that prefix only.
        crash::text_record out;
        out.append("{\"schema\":\"uwvm.memtag.violation.v1\",\"sequence\":");out.number(next_sequence++);
        out.append(",\"action\":\"silent-log\",\"module\":");out.optional(module,true);
        out.append(",\"function\":");out.optional(function,true);
        out.append(",\"wasm_offset\":");out.optional(offset,true);
        out.append(",\"memory\":");out.number(memory);
        out.append(",\"pointer\":");out.number(raw);
        out.append(",\"granule\":");out.number(access.mismatch_granule);
        out.append(",\"pointer_tag\":");out.number(access.pointer_tag);
        out.append(",\"allocation_tag\":");out.number(access.allocation_tag);
        out.append(",\"asynchronous_delivery\":");out.append(asynchronous ? "true":"false");out.append("}\n");
        if(!write(out.bytes,out.size))
        {
            // Silent log loss would make this diagnostic contract untrue.
            crash::report(crash::category::internal,"cannot write the MTE violation report");
            crash::pause_process();::fast_io::fast_terminate(); // Host reporting failure is not a suppressible Guest fault.
        }
    }
}
