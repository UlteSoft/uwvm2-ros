// WasmMTE exposes exactly five CLI controls: parser/runtime off, timing,
// fault action (including its independent report destination), and backend.
#pragma once
#ifndef UWVM_MODULE
# include <memory>
# if defined(__linux__)
#  include <cerrno>
#  include <fcntl.h>
#  include <sys/stat.h>
#  include <unistd.h>
# endif
# include <uwvm2/utils/macro/push_macros.h>
# include <fast_io.h>
# include <uwvm2/utils/cmdline/impl.h>
# include <uwvm2/uwvm/io/impl.h>
# include <uwvm2/extension/memtag/execution_policy.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::cmdline::params
{
    namespace details
    {
        inline bool runtime_memtag_backend_exists{},runtime_memtag_mode_exists{},runtime_memtag_action_exists{};
#if defined(__linux__)
        // This handle belongs exclusively to MTE reporting. Never duplicate or
        // reopen the general logger; later --log-output changes cannot move it.
        inline ::fast_io::u8native_file runtime_memtag_report_file{};
#endif
        inline ::uwvm2::utils::cmdline::parameter_return_type runtime_memtag_mode_callback(
            ::uwvm2::utils::cmdline::parameter_parsing_results*,
            ::uwvm2::utils::cmdline::parameter_parsing_results* current,
            ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
        {
            using result=::uwvm2::utils::cmdline::parameter_return_type;
            namespace mt=::uwvm2::extension::memtag;
            auto* value{current+1u};
            if(value==end || value->type!=::uwvm2::utils::cmdline::parameter_parsing_results_type::arg)
            {
                ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,u8"uwvm: [error] ",current->str,u8" requires a value.");
                return result::return_m1_imme;
            }
            bool valid{};
            if(current->str==u8"--runtime-memtag-backend")
            {
                valid=value->str==u8"software" || value->str==u8"hardware";
                if(valid) { mt::select_backend(value->str==u8"hardware"); }
                else { ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,u8"uwvm: [error] --runtime-memtag-backend requires software or hardware."); }
            }
            else if(current->str==u8"--runtime-memtag-mode")
            {
                valid=value->str==u8"sync" || value->str==u8"async";
                if(valid) { mt::select_delivery(value->str==u8"async"); }
                else { ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,u8"uwvm: [error] --runtime-memtag-mode requires sync or async."); }
            }
            else if(current->str==u8"--runtime-memtag-action")
            {
                if(value->str==u8"crash")
                { mt::selected_report_action.store(mt::report_action::crash,::std::memory_order_relaxed);valid=true; }
                else if(value->str==u8"silent-log")
                {
                    auto* destination{value+1u};
                    if(destination==end || destination->type!=::uwvm2::utils::cmdline::parameter_parsing_results_type::arg)
                    {
                        ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                            u8"uwvm: [error] silent-log requires out, err, or path <PATH> for its independent MTE report stream.");
                        return result::return_m1_imme;
                    }
                    auto* path{destination};
                    int source{-1};
                    if(destination->str==u8"out") { source=1; }
                    else if(destination->str==u8"err") { source=2; }
                    else if(destination->str==u8"path")
                    {
                        path=destination+1u;
                        if(path==end || path->type!=::uwvm2::utils::cmdline::parameter_parsing_results_type::arg || path->str.empty())
                        {
                            ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                                u8"uwvm: [error] --runtime-memtag-action silent-log path requires a nonempty report path.");
                            return result::return_m1_imme;
                        }
                    }
                    else
                    {
                        ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                            u8"uwvm: [error] silent-log output must be out, err, or path <PATH>.");
                        return result::return_m1_imme;
                    }
#if defined(__linux__)
                    // Complete argv entries are NUL-terminated. No path copy,
                    // fixed length ceiling, or alternate long-path algorithm.
                    int const saved_errno{errno};
                    int descriptor{source<0 ? ::open(reinterpret_cast<char const*>(path->str.data()),
                        O_WRONLY|O_CREAT|O_APPEND|O_CLOEXEC,0666) : ::fcntl(source,F_DUPFD_CLOEXEC,3)};
                    if(descriptor>=0 && descriptor<3)
                    {
                        // A closed inherited stdio slot is not a report sink.
                        // Keep this owned file above 2 before WASI stdio is
                        // initialized, so no guest grant can re-export it.
                        int const pinned{::fcntl(descriptor,F_DUPFD_CLOEXEC,3)};
                        int const pin_error{errno};::close(descriptor);
                        descriptor=pinned;errno=pin_error;
                    }
                    int const error{errno};errno=saved_errno;
                    if(descriptor<0)
                    {
                        ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                            u8"uwvm: [error] Unable to open the independent MTE report output (errno ",error,
                            u8"). Use --runtime-memtag-action silent-log out|err|path <PATH> and check the selected descriptor/path permissions.");
                        return result::return_m1_imme;
                    }
                    runtime_memtag_report_file.reset(descriptor);
                    mt::report_descriptor=descriptor;
                    struct stat metadata{};
                    mt::report_regular_file=::fstat(descriptor,&metadata)==0 && S_ISREG(metadata.st_mode);
                    errno=saved_errno;
#else
                    if(source<0)
                    {
                        ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                            u8"uwvm: [error] MTE report paths require the admitted Linux native descriptor backend.");
                        return result::return_m1_imme;
                    }
                    mt::report_descriptor=source;
#endif
                    mt::selected_report_action.store(mt::report_action::silent_log,::std::memory_order_relaxed);
                    destination->type=::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg;
                    if(path!=destination) { path->type=::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg; }
                    valid=true;
                }
                else
                {
                    ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                        u8"uwvm: [error] --runtime-memtag-action requires crash, or silent-log out|err|path <PATH>.");
                }
            }
            if(!valid) { return result::return_m1_imme; }
            value->type=::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg;
            return result::def;
        }
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter runtime_memtag_backend{
        .name{u8"--runtime-memtag-backend"},
        .describe{u8"MTE backend: software (default), or hardware. Unsupported hardware/OS combinations are launch errors. Silent-log diagnostics use precise software detection to preserve every violation."},
        .usage{u8"<software|hardware>"},
        .handle=::std::addressof(details::runtime_memtag_mode_callback),
        .is_exist=::std::addressof(details::runtime_memtag_backend_exists),
        .cate=::uwvm2::utils::cmdline::categorization::runtime};
    inline constexpr ::uwvm2::utils::cmdline::parameter runtime_memtag_mode{
        .name{u8"--runtime-memtag-mode"},
        .describe{u8"MTE timing: sync (default), or async. Independent of backend and fault action; ordinary sandbox bounds remain immediate."},
        .usage{u8"<sync|async>"},
        .handle=::std::addressof(details::runtime_memtag_mode_callback),
        .is_exist=::std::addressof(details::runtime_memtag_mode_exists),
        .cate=::uwvm2::utils::cmdline::categorization::runtime};
    inline constexpr ::uwvm2::utils::cmdline::parameter runtime_memtag_action{
        .name{u8"--runtime-memtag-action"},
        .describe{u8"MTE action: crash (default), or silent-log with an explicit independent out, err, or path destination. Record every violation and continue; path appends JSON records. --log-output never redirects MTE reports. Bounds and host permissions remain enforced; report I/O failure terminates."},
        .usage{u8"<crash|silent-log <out|err|path <PATH>>>"},
        .handle=::std::addressof(details::runtime_memtag_mode_callback),
        .is_exist=::std::addressof(details::runtime_memtag_action_exists),
        .cate=::uwvm2::utils::cmdline::categorization::runtime};
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
