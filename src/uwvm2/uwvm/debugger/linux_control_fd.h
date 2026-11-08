/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/push_macros.h>
# if defined(__linux__) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
#  include <array>
#  include <cerrno>
#  include <cstddef>
#  include <cstdint>
#  include <cstring>
#  include <memory>
#  include <new>
#  include <fast_io.h>
#  include <string>
#  include <string_view>
#  include <type_traits>
#  include <utility>
#  include <fcntl.h>
#  include <poll.h>
#  include <sys/socket.h>
#  include <sys/syscall.h>
#  include <unistd.h>
#  include "posix_abi.h"
#  include <uwvm2/utils/control/sealed_input.h>
#  include <uwvm2/utils/control/owned_file_image.h>
#  include "managed_cli_shutdown.h"
#  include "console.h"
# endif
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::debugger
{
#if defined(__linux__) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(SYS_pidfd_open) && defined(SCM_CREDENTIALS)
    // The launcher creates SOCK_SEQPACKET socketpair before exec and passes only
    // its VM endpoint by explicit command-line opt-in. There is no listener,
    // pathname, environment token, or arbitrary FD adoption after guest entry.
    // Kernel credentials on every packet bind commands to the original launcher
    // process. A guest that reopens either endpoint through /proc has its own PID
    // and cannot impersonate that process.
    class linux_control_fd final
    {
        // native_file owns every inherited and kernel-created capability.
        // Syscall native_handle() arguments below are synchronous borrows.
        ::fast_io::native_file descriptor_{}, peer_pidfd_{};
        ::pid_t peer_pid_{};
        ::uid_t peer_uid_{};
        ::gid_t peer_gid_{};
        static constexpr ::std::size_t maximum_command_bytes{max_input_command_bytes};
        static constexpr ::std::size_t maximum_reply_bytes{65536u};

        linux_control_fd(::fast_io::native_file&& descriptor, ::fast_io::native_file&& pidfd, ::ucred peer) noexcept
            : descriptor_{::std::move(descriptor)}, peer_pidfd_{::std::move(pidfd)}, peer_pid_{peer.pid}, peer_uid_{peer.uid}, peer_gid_{peer.gid} {}
        [[nodiscard]] bool peer_live() const noexcept
        {
            ::pollfd observed{peer_pidfd_.native_handle(), POLLIN, 0};
            int result{};
            do { result = posix_abi::poll_noexcept(&observed, 1, 0); } while(result == -1 && errno == EINTR);
            return result == 0 && observed.revents == 0;
        }
        [[nodiscard]] bool send_text(char const* bytes, ::std::size_t size) const noexcept
        {
            if(size > maximum_reply_bytes || (size != 0u && bytes == nullptr) || !peer_live()) { return false; }
            ::ssize_t written{};
            do { written = posix_abi::send_noexcept(descriptor_.native_handle(), bytes, size, MSG_DONTWAIT | MSG_NOSIGNAL); }
            while(written == -1 && errno == EINTR);
            return written >= 0 && static_cast<::std::size_t>(written) == size;
        }
        [[nodiscard]] bool send_text(::std::string const& text) const noexcept
        {
            if(text.size() > maximum_reply_bytes)
            { return send_literal("error: bounded debugger response exceeded 65536 bytes\n"); }
            return send_text(text.data(), text.size());
        }
        template <::std::size_t N>
        [[nodiscard]] bool send_literal(char const (&literal)[N]) const noexcept
        { return send_text(literal, N - 1u); }

        struct received_packet
        {
            ::std::array<char, maximum_command_bytes> bytes{};
            ::std::size_t size{};
            bool valid{}, closed{};
        };
        [[nodiscard]] received_packet receive_packet() noexcept
        {
            received_packet packet{};
            ::iovec payload{packet.bytes.data(), packet.bytes.size()};
            alignas(::cmsghdr) ::std::array<unsigned char,
                CMSG_SPACE(sizeof(::ucred)) + CMSG_SPACE(16u * sizeof(int))> ancillary{};
            ::msghdr message{};
            message.msg_iov = &payload; message.msg_iovlen = 1u;
            message.msg_control = ancillary.data(); message.msg_controllen = ancillary.size();
            ::ssize_t count{};
            do { count = posix_abi::recvmsg_noexcept(descriptor_.native_handle(), &message, MSG_DONTWAIT | MSG_CMSG_CLOEXEC); }
            while(count == -1 && errno == EINTR);
            if(count == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) { return packet; }
            if(count == -1) { packet.closed = true; return packet; }

            ::ucred credentials{};
            unsigned credentials_count{};
            bool unexpected{};
            // recvmsg bounds each cmsghdr. Copy CMSG_DATA rather than assuming
            // its alignment, and close all rights even on rejected packets.
            for(auto* item{CMSG_FIRSTHDR(&message)}; item != nullptr; item = CMSG_NXTHDR(&message, item))
            {
                if(item->cmsg_len < CMSG_LEN(0)) { unexpected = true; continue; }
                if(item->cmsg_level == SOL_SOCKET && item->cmsg_type == SCM_CREDENTIALS &&
                   item->cmsg_len == CMSG_LEN(sizeof(::ucred)))
                { ::std::memcpy(&credentials, CMSG_DATA(item), sizeof(credentials)); ++credentials_count; }
                else
                {
                    unexpected = true;
                    if(item->cmsg_level == SOL_SOCKET && item->cmsg_type == SCM_RIGHTS)
                    {
                        auto const length{item->cmsg_len - CMSG_LEN(0)};
                        for(::std::size_t offset{}; offset + sizeof(int) <= length; offset += sizeof(int))
                        {
                            int received_fd{};
                            // [ancillary rights bytes ...] offset+sizeof(int) <= length.
                            // [safe                      ] copy cannot read past cmsg.
                            ::std::memcpy(&received_fd, CMSG_DATA(item) + offset, sizeof(received_fd));
                            ::fast_io::native_file rejected_right{received_fd};
                        }
                    }
                }
            }
            // An AF_UNIX SOCK_SEQPACKET sender may attach SCM_RIGHTS to a
            // zero-byte record. The loop above must close those descriptors
            // before EOF is handled; otherwise each rejected record leaks an
            // inherited host capability into the management process.
            if(count == 0) { packet.closed = true; return packet; }
            if(unexpected || credentials_count != 1u || (message.msg_flags & (MSG_TRUNC | MSG_CTRUNC)) != 0 ||
               credentials.pid != peer_pid_ || credentials.uid != peer_uid_ || credentials.gid != peer_gid_ ||
               credentials.pid == posix_abi::getpid_noexcept() || !peer_live())
            { packet.closed = true; return packet; }
            packet.size = static_cast<::std::size_t>(count); // recvmsg wrote <= packet.bytes.size(), else MSG_TRUNC above.
            packet.valid = true;
            return packet;
        }
        void close_endpoint() noexcept
        {
            if(descriptor_)
            {
                // The sealed CLOEXEC duplicate pins this socket until guest
                // drain. Shutdown applies to that duplicate too, so a detached
                // or rejected management session really becomes unusable now.
                posix_abi::shutdown_noexcept(descriptor_.native_handle(), SHUT_RDWR);
                // reset() is the nonthrowing RAII retirement; no retry can
                // accidentally close a reused POSIX descriptor.
                descriptor_.reset();
            }
        }
        struct close_on_serve_exit
        {
            linux_control_fd& channel;
            controller& control;
            managed_cli_shutdown* shutdown{};
            bool handoff{};
            ~close_on_serve_exit()
            {
                if(handoff) { return; }
                // A partially retired world cannot be resumed or abandoned.
                if(shutdown && shutdown->requested()) { posix_abi::_exit_noexcept(75); }
                // A detached manager cannot leave a selected guest parked in
                // the SIGTRAP futex. If the host cannot safely reopen that gate,
                // fail closed instead of hanging the inferior indefinitely.
                if(!control.detach_resume()) { posix_abi::_exit_noexcept(1); }
                channel.close_endpoint();
            }
        };
    public:
        linux_control_fd(linux_control_fd const&) = delete;
        linux_control_fd& operator=(linux_control_fd const&) = delete;
        ~linux_control_fd()
        {
            close_endpoint();
            // peer_pidfd_ retires exactly once with its native_file member.
        }
        [[nodiscard]] static ::std::unique_ptr<linux_control_fd> adopt(int fd, bool diagnostic = false) noexcept
        {
            if(fd < 3) { return {}; }
            // The VM takes ownership at entry, including every rejected launch.
            // A rejected endpoint must never survive into any later guest setup.
            ::fast_io::native_file supplied{fd};
            int type{}, domain{};
            ::socklen_t length{sizeof(type)};
            if(posix_abi::getsockopt_noexcept(fd, SOL_SOCKET, SO_TYPE, &type, &length) != 0 || length != sizeof(type) || type != SOCK_SEQPACKET)
            { return {}; }
            length = sizeof(domain);
            if(posix_abi::getsockopt_noexcept(fd, SOL_SOCKET, SO_DOMAIN, &domain, &length) != 0 || length != sizeof(domain) || domain != AF_UNIX)
            { return {}; }
            ::ucred peer{};
            length = sizeof(peer);
            if(posix_abi::getsockopt_noexcept(fd, SOL_SOCKET, SO_PEERCRED, &peer, &length) != 0 || length != sizeof(peer) || peer.pid <= 0 ||
               peer.pid == posix_abi::getpid_noexcept() || peer.uid != posix_abi::geteuid_noexcept() || peer.gid != posix_abi::getegid_noexcept()) { return {}; }
            auto const opened_pidfd{::fast_io::system_call<SYS_pidfd_open, int>(peer.pid, 0u)};
            // pidfd_open succeeds with a nonnegative FD. Reject raw -errno
            // BEFORE adopting a capability; keep the old libc failure errno.
            if(opened_pidfd < 0)
            {
                errno = -opened_pidfd;
                return {};
            }
            ::fast_io::native_file pidfd{opened_pidfd};
            // A failed allocation leaves both guards owned by this scope.
            auto result{::std::unique_ptr<linux_control_fd>{new (::std::nothrow) linux_control_fd(
                ::std::move(supplied), ::std::move(pidfd), peer)}};
            if(!result) { return {}; }
            if(!result->peer_live()) { return {}; }
            int enable{1};
            int const flags{posix_abi::fcntl_noexcept(fd, F_GETFD)};
            // This host-startup socket opt-in follows peer/type/pidfd checks;
            // every received packet must still authenticate the original peer.
            auto const sealed{diagnostic ? ::uwvm2::utils::control::seal_diagnostic_control_host_api(fd) :
                ::uwvm2::utils::control::seal_console_input_host_api(fd, true)};
            if(posix_abi::setsockopt_noexcept(fd, SOL_SOCKET, SO_PASSCRED, &enable, sizeof(enable)) != 0 ||
               flags == -1 || posix_abi::fcntl_noexcept(fd, F_SETFD, flags | FD_CLOEXEC) == -1 ||
               (sealed != ::uwvm2::utils::control::sealed_input_status::ok &&
                !(sealed == ::uwvm2::utils::control::sealed_input_status::already_sealed &&
                  ::uwvm2::utils::control::inspect_guest_file_host_api(fd) == ::uwvm2::utils::control::sealed_input_decision::denied)))
            { return {}; }
            for(int guest_stdio{}; guest_stdio != 3; ++guest_stdio)
            {
                // WASI installs 0..2 independently of preopens. Reject a launch
                // whose guest stdio is another alias of the management endpoint.
                if(::uwvm2::utils::control::inspect_guest_file_host_api(guest_stdio) !=
                   ::uwvm2::utils::control::sealed_input_decision::allow)
                { return {}; }
            }
            return result;
        }
        // A separate launch grant selects this immutable read-only dispatcher.
        // It never constructs a controller or interprets console/DAP commands.
        void serve_diagnostics(::std::atomic<bool> const& finished, ::std::string (*snapshot)() noexcept) noexcept
        {
            struct close_when_done { linux_control_fd& channel; ~close_when_done() { channel.close_endpoint(); } } close{*this};
            while(!finished.load(::std::memory_order_acquire) && peer_live())
            {
                ::pollfd input{descriptor_.native_handle(), POLLIN, 0};
                int const ready{posix_abi::poll_noexcept(&input, 1, 50)};
                if(ready < 0) { if(errno == EINTR) continue; return; }
                if(ready == 0) continue;
                auto packet{receive_packet()};
                if(packet.closed) return;
                if(!packet.valid) continue;
                ::std::string_view raw{packet.bytes.data(),packet.size};
                if(raw == "close") return; // Disconnects observation only, never the Guest.
                if(raw == "help")
                { if(!send_literal("status | modules | health | crash | update | close; non-sensitive diagnostics\n")) return; }
                else if(raw == "status" || raw == "modules" || raw == "health" || raw == "crash" || raw == "update")
                { auto text{snapshot()}; if(text.empty() || !send_text(text)) return; }
                else if(!send_literal("error: diagnostic grant denies this operation\n")) return;
            }
            // One bounded post-execution report; a blocked/disconnected reader
            // cannot prevent Guest completion or make this thread wait on send.
            auto text{snapshot()}; if(!text.empty()) (void)send_text(text);
        }
        struct rebuild_request
        {
            ::std::u8string path{};
            ::uwvm2::utils::control::owned_file_image::owner image{};
            ::std::size_t module_index{}, preload_count{};
            bool allowed{}, ready{};
        };
        [[nodiscard]] bool notify_rebuild_result(bool success) const noexcept
        { return success ? send_literal("rebuild complete\n") : send_literal("rebuild failed; execution stopped\n"); }
        void serve(controller& control, managed_cli_shutdown* shutdown = nullptr, rebuild_request* rebuild = nullptr) noexcept
        {
            close_on_serve_exit close_after_guest_detach{*this, control, shutdown};
            using command_view = ::fast_io::string_view;
            for(;;)
            {
                auto const state{control.inspect().execution};
                bool const guest_faulted{::uwvm2::runtime::lib::runtime_guest_fault_detected_host_api()};
                if(((state == execution_status::exited || state == execution_status::closed) &&
                    !guest_faulted && !(shutdown && shutdown->requested())) || !peer_live()) { break; }
                ::pollfd wait_for[2]{{descriptor_.native_handle(), POLLIN, 0}, {peer_pidfd_.native_handle(), POLLIN, 0}};
                int ready{};
                do { ready = posix_abi::poll_noexcept(wait_for, 2u, 50); } while(ready == -1 && errno == EINTR);
                if(ready == -1 || (wait_for[1].revents != 0) || (wait_for[0].revents & (POLLERR | POLLHUP | POLLNVAL)) != 0)
                { break; }
                if((wait_for[0].revents & POLLIN) == 0) { continue; }
                auto const packet{receive_packet()};
                if(packet.closed) { break; }
                if(!packet.valid) { continue; }
                ::std::string_view const raw{packet.bytes.data(), packet.size};
                // A known Guest fault has already unwound and closed admission.
                // Keep the authenticated status/quit channel alive, but never
                // resume or mutate this stopped world through the DEV transport.
                if(::uwvm2::runtime::lib::runtime_guest_fault_detected_host_api() &&
                   raw != "status" && raw != "quit" && raw != "help" && raw != "permissions")
                { if(!send_literal("error: Guest faulted; execution stopped\n")) { return; } continue; }
                if(raw == "help rebuild")
                {
                    if(!send_literal("rebuild <path>  Full-permission complete guest-world reconstruction; host grants/resources/quotas persist\n"
                                     "rebuild-module <index> <path>  replace main (0) or a preload (1..N startup order)\n"
                                     "rebuild         retry the same pending retirement; old world cannot resume\n")) { return; }
                    continue;
                }
                if(raw == "rebuild" || raw.starts_with("rebuild ") || raw.starts_with("rebuild-module "))
                {
                    if(!rebuild || !rebuild->allowed || !shutdown || !shutdown->matches_controller(control) ||
                       !::uwvm2::utils::control::debug_permissions::full_allowed())
                    { if(!send_literal("error: rebuild unsupported for this launch\n")) { return; } continue; }
                    if(!rebuild->image)
                    {
                        ::std::size_t path_begin{8u}, selected{};
                        bool valid{raw.find('\0') == ::std::string_view::npos};
                        if(raw.starts_with("rebuild-module "))
                        {
                            path_begin = 15u; auto const first{path_begin};
                            for(; path_begin < raw.size() && raw[path_begin] != ' '; ++path_begin)
                            {
                                auto c{raw[path_begin]};
                                if(c < '0' || c > '9' || selected > rebuild->preload_count / 10u)
                                { valid = false; break; }
                                selected = selected * 10u + static_cast<unsigned>(c - '0');
                            }
                            valid = valid && path_begin > first && selected <= rebuild->preload_count;
                            ++path_begin;
                        }
                        if(!valid || path_begin >= raw.size())
                        { if(!send_literal("error: rebuild requires an existing module index and path\n")) { return; } continue; }
                        rebuild->module_index = selected;
#ifdef UWVM_CPP_EXCEPTIONS
                        try
#endif
                        {
                            rebuild->path.assign(reinterpret_cast<char8_t const*>(raw.data() + path_begin), raw.size() - path_begin);
                            auto input{::uwvm2::utils::control::owned_file_image::read(rebuild->path,
                                ::uwvm2::utils::control::owned_file_image::maximum_bytes)};
                            if(input) { rebuild->image = ::std::move(input.image); }
                        }
#ifdef UWVM_CPP_EXCEPTIONS
                        catch(...) {}
#endif
                        if(!rebuild->image)
                        { if(!send_literal("error: rebuild input unavailable\n")) { return; } continue; }
                    }
                    else if(raw != "rebuild")
                    { if(!send_literal("error: rebuild already pending; retry rebuild\n")) { return; } continue; }
                    auto const retired{shutdown->attempt_until(::std::chrono::steady_clock::now() + ::std::chrono::seconds{2})};
                    if(retired.phase == managed_cli_shutdown_phase::completed)
                    {
                        rebuild->ready = true;
                        close_after_guest_detach.handoff = true;
                        return; // Same authenticated descriptor/pidfd retained by the launcher.
                    }
                    switch(retired.phase)
                    {
                        case managed_cli_shutdown_phase::pending_native:
                        case managed_cli_shutdown_phase::pending_execution:
                        case managed_cli_shutdown_phase::pending_producers:
                        case managed_cli_shutdown_phase::pending_guest_thread:
                        case managed_cli_shutdown_phase::pending_release:
                        case managed_cli_shutdown_phase::busy:
                            if(!send_literal("rebuild pending\n")) { return; }
                            break;
                        default:
                            static_cast<void>(send_literal("rebuild failed; execution stopped\n"));
                            posix_abi::_exit_noexcept(75); // Retain every incomplete owner until process containment.
                    }
                    continue;
                }
                if(shutdown && shutdown->requested())
                { if(!send_literal("error: rebuild pending; retry rebuild\n")) { return; } continue; }
                if(command_view{packet.bytes.data(), packet.size} == "permissions")
                {
                    auto const text=::fast_io::concat_std("permissions ",::std::string{::uwvm2::utils::control::debug_permissions::name()},"\n");
                    if(!send_text(text.data(),text.size()))return;
                    continue;
                }
                auto const command{parse_console_command(command_view{packet.bytes.data(), packet.size})};
                switch(command.kind)
                {
                    case console_command_kind::empty: if(!send_literal("error: empty command\n")) { return; } continue;
                    case console_command_kind::help:
                        {
                            if(!::uwvm2::utils::control::debug_permissions::inspection_allowed())
                            { if(!send_literal("permissions | status | dump wasm <stop-id> <path> | continue (terminal fault only) | quit; limited diagnostics\n"))return;continue; }

                            auto const text{console_help::format(command_view{command.help_topic.data(), command.help_topic_size})};
                            if(!send_text(text.data(), text.size())) { return; }
                             if(rebuild && rebuild->allowed && !send_literal("rebuild <path>  replace the guest world with full debugger permissions; retain host grants and state\n")) { return; }
                        }
                        continue;
                    case console_command_kind::quit:
                        if(::uwvm2::runtime::lib::runtime_guest_fault_detected_host_api())
                        { static_cast<void>(send_literal("debug control detached; Guest remains stopped\n")); }
                        else { static_cast<void>(send_literal("debug control detached; guest continues\n")); }
                        return;
                    case console_command_kind::unsupported:
                        {
                            auto const message{unsupported_step_message(command)};
                            if(!send_text(message.data(), message.size())) { return; }
                        }
                        continue;
                    case console_command_kind::invalid:
                        if(!send_literal("error: invalid command; type help\n")) { return; }
                        continue;
                    case console_command_kind::source_step:
                    case console_command_kind::wasm_step:
                    case console_command_kind::wasm_state:
                    case console_command_kind::wasm_path:
                    case console_command_kind::wasm_mutation:
                    case console_command_kind::assembly_breakpoint:
                    case console_command_kind::execution_relocate:
                    case console_command_kind::source_breakpoint:
                    case console_command_kind::source_locals:
                    case console_command_kind::source_type:
                    case console_command_kind::source_value:
                    case console_command_kind::source_frame:
                    case console_command_kind::wasip1_state:
                    case console_command_kind::host_trace:
                    case console_command_kind::ros_state:
                    case console_command_kind::watchpoint:
                case console_command_kind::breakpoint_commands:
                case console_command_kind::callback_echo:
                case console_command_kind::breakpoint_control:
                    case console_command_kind::assembly_next:
                    case console_command_kind::assembly_finish:
                    case console_command_kind::assembly_step:
                    case console_command_kind::assembly_disassemble:
                    case console_command_kind::assembly_disassemble_range:
                    case console_command_kind::assembly_registers:
                    case console_command_kind::wasm_event:
                    case console_command_kind::wasm_script:
                    case console_command_kind::replacement_file:
                    case console_command_kind::source_print_options:
                    case console_command_kind::source_printer_config:
                    case console_command_kind::wasm_disassemble:
                    case console_command_kind::wasm_dump:
                    case console_command_kind::module_list:
                    case console_command_kind::protocol: break;
                }
                if(!send_text(details::execute_control_command(control, command))) { return; }
            }
        }
    };
#endif
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
