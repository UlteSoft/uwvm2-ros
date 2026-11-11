#pragma once
#include <array>
#include <atomic>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <memory>
#include <new>
#include <thread>
#include <vector>
#include <uwvm2/runtime/lib/uwvm_runtime_update.h>
#include <uwvm2/utils/thread/cooperative_pause_domain.h>
#if defined(__linux__)
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <poll.h>
#include <uwvm2/utils/control/sealed_input.h>
#include <unistd.h>
#endif
namespace uwvm2::uwvm::run::production_update_cli
{
#if defined(__linux__) && defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS)
    namespace update = ::uwvm2::utils::update;
    struct business_export
    {
        bool enabled{};
        ::std::uint32_t function_index{}, input_bytes{}, output_bytes{};
        ::std::shared_ptr<::uwvm2::utils::thread::cooperative_pause_domain> debug_control{};
    };
    class channel
    {
        int fd_{-1}, peer_pidfd_{-1};
        ::ucred peer_{};
        explicit channel(int fd, int pidfd, ::ucred peer) noexcept : fd_{fd}, peer_pidfd_{pidfd}, peer_{peer} {}
        bool peer_live() const noexcept
        {
            ::pollfd observed{peer_pidfd_, POLLIN, 0};
            int result{};
            do { result = ::poll(&observed, 1, 0); } while(result < 0 && errno == EINTR);
            return result == 0 && observed.revents == 0;
        }
        bool receive(void* data, ::std::size_t size) noexcept
        {
            auto* p{static_cast<unsigned char*>(data)};
            while(size)
            {
                if(!peer_live()) { return false; }
                ::iovec payload{p, size};
                alignas(::cmsghdr) ::std::array<unsigned char, CMSG_SPACE(sizeof(::ucred)) + CMSG_SPACE(16u*sizeof(int))> ancillary{};
                ::msghdr message{};
                message.msg_iov = &payload; message.msg_iovlen = 1;
                message.msg_control = ancillary.data(); message.msg_controllen = ancillary.size();
                auto const count{::recvmsg(fd_, &message, MSG_CMSG_CLOEXEC)};
                if(count < 0 && errno == EINTR) { continue; }
                unsigned credential_count{}; ::ucred sender{}; bool unexpected{};
                for(auto* item{CMSG_FIRSTHDR(&message)}; item; item = CMSG_NXTHDR(&message, item))
                {
                    if(item->cmsg_len < CMSG_LEN(0)) { unexpected = true; continue; }
                    if(item->cmsg_level == SOL_SOCKET && item->cmsg_type == SCM_CREDENTIALS && item->cmsg_len == CMSG_LEN(sizeof(sender)))
                    { ::std::memcpy(&sender, CMSG_DATA(item), sizeof(sender)); ++credential_count; }
                    else
                    {
                        unexpected = true;
                        if(item->cmsg_level == SOL_SOCKET && item->cmsg_type == SCM_RIGHTS)
                        {
                            auto const length{item->cmsg_len - CMSG_LEN(0)};
                            for(::std::size_t offset{}; offset + sizeof(int) <= length; offset += sizeof(int))
                            { int received{}; ::std::memcpy(&received, CMSG_DATA(item)+offset, sizeof(received)); ::close(received); }
                        }
                    }
                }
                if(count <= 0 || unexpected || credential_count != 1 || (message.msg_flags & (MSG_TRUNC | MSG_CTRUNC)) != 0 ||
                   sender.pid != peer_.pid || sender.uid != peer_.uid || sender.gid != peer_.gid || !peer_live()) { return false; }
                p += count; size -= static_cast<::std::size_t>(count);
            }
            return true;
        }
        bool send(::std::span<unsigned char const> bytes) noexcept
        {
            while(!bytes.empty())
            {
                if(!peer_live()) { return false; }
                auto const count{::send(fd_, bytes.data(), bytes.size(), MSG_NOSIGNAL)};
                if(count < 0 && errno == EINTR) { continue; }
                if(count <= 0) { return false; } bytes = bytes.subspan(static_cast<::std::size_t>(count));
            }
            return true;
        }
        static ::std::uint32_t u32(unsigned char const* p) noexcept
        { return p[0] | (static_cast<::std::uint32_t>(p[1])<<8) | (static_cast<::std::uint32_t>(p[2])<<16) | (static_cast<::std::uint32_t>(p[3])<<24); }
        static void put32(::std::vector<unsigned char>& out, ::std::uint32_t n)
        { for(unsigned i{}; i < 4; ++i) { out.push_back(static_cast<unsigned char>(n>>(i*8))); } }
        bool reply(update::status status, ::std::span<unsigned char const> result = {})
        {
            update::facts f{}; (void)::uwvm2::runtime::lib::runtime_update_observe_host_api(f);
            ::std::vector<unsigned char> out{'U','U','R','1'};
            put32(out, static_cast<::std::uint32_t>(296 + result.size()));
            put32(out, static_cast<unsigned>(status)); put32(out, static_cast<unsigned>(f.state));
            put32(out, f.active); put32(out, f.coordinator_connected);
            for(auto n : {f.installed_epoch, f.authenticated_commit_epoch, f.decision_sequence}) { update::append_u64(out, n); }
            for(auto const* d : {&f.nonce, &f.installed, &f.prepared, &f.ready, &f.transaction, &f.contract, &f.scope, &f.issuer}) { out.insert(out.end(), d->begin(), d->end()); }
            out.insert(out.end(), result.begin(), result.end()); return send(out);
        }
        update::status transition_facts(::std::vector<unsigned char>& result)
        {
            using namespace ::uwvm2::runtime::lib;
            update::transition_facts f{}; auto status{runtime_update_observe_transition_host_api(f)};
            ::std::vector<unsigned char> policy;
            if(status == update::status::ok) { status = runtime_update_read_policy_host_api(policy); }
            if(status == update::status::ok)
            {
                result.insert(result.end(), f.pending_contract.begin(), f.pending_contract.end());
                for(auto n : {f.policy_generation, f.revocation_generation, f.pending_policy_generation, f.pending_revocation_generation})
                { update::append_u64(result, n); }
                put32(result, static_cast<::std::uint32_t>(policy.size())); result.insert(result.end(), policy.begin(), policy.end());
            }
            return status;
        }
    public:
        ~channel()
        {
            if(fd_ >= 0) { ::shutdown(fd_, SHUT_RDWR); ::close(fd_); }
            if(peer_pidfd_ >= 0) { ::close(peer_pidfd_); }
        }
        channel(channel const&) = delete;
        channel& operator=(channel const&) = delete;
        static ::std::unique_ptr<channel> adopt(int fd)
        {
            if(fd < 3) { return {}; }
            int passcred{}; ::socklen_t credential_size{sizeof(passcred)};
            if(::getsockopt(fd, SOL_SOCKET, SO_PASSCRED, &passcred, &credential_size) != 0 || passcred != 1) { return {}; }
            int type{}; ::socklen_t size{sizeof(type)};
            ::ucred peer{}; ::socklen_t peer_size{sizeof(peer)};
            ::sockaddr_storage address{}; ::socklen_t address_size{sizeof(address)};
            if(::getsockopt(fd, SOL_SOCKET, SO_TYPE, &type, &size) != 0 || type != SOCK_STREAM ||
               ::getsockname(fd, reinterpret_cast<::sockaddr*>(&address), &address_size) != 0 || address.ss_family != AF_UNIX ||
               ::getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &peer, &peer_size) != 0 || peer.pid != ::getppid() || peer.uid != ::geteuid()) { return {}; }
            struct ::stat control_identity{};
            if(::fstat(fd, &control_identity) != 0) { return {}; }
            for(int stream{}; stream < 3; ++stream)
            {
                struct ::stat stream_identity{};
                if(::fstat(stream, &stream_identity) == 0 && stream_identity.st_dev == control_identity.st_dev &&
                   stream_identity.st_ino == control_identity.st_ino) { return {}; }
            }
            int const owned{::fcntl(fd, F_DUPFD_CLOEXEC, 3)};
            if(owned < 0) { return {}; }
            ::close(fd);
            int const pidfd{static_cast<int>(::syscall(SYS_pidfd_open, peer.pid, 0u))};
            if(pidfd < 0) { ::close(owned); return {}; }
            ::std::unique_ptr<channel> result{new (::std::nothrow) channel(owned, pidfd, peer)};
            if(!result) { ::close(owned); ::close(pidfd); return {}; }
            ::timeval timeout{10, 0}; int enable{1};
            if(!result->peer_live() || ::setsockopt(owned, SOL_SOCKET, SO_PASSCRED, &enable, sizeof(enable)) != 0 ||
               ::setsockopt(owned, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) != 0 ||
               ::uwvm2::utils::control::seal_update_control_host_api(owned) != ::uwvm2::utils::control::sealed_input_status::ok)
            { return {}; }
            // A stream can split a frame arbitrarily. Every recvmsg fragment
            // authenticates the real sender; passing the peer FD does not pass
            // this process-bound management grant. SCM_RIGHTS are never accepted.
            return result;
        }
        int await_launch()
        {
            using namespace ::uwvm2::runtime::lib;
            for(;;)
            {
                ::std::array<unsigned char, 12> header{};
                if(!receive(header.data(), header.size()) || ::std::memcmp(header.data(), "UUP1", 4) != 0) { break; }
                auto const operation{u32(header.data()+4)}, length{u32(header.data()+8)};
                if(length > update::maximum_image_bytes + update::maximum_manifest_bytes + update::permit::wire_size + 4u || operation > 9) { break; }
                ::std::vector<unsigned char> payload(length);
                if(!receive(payload.data(), payload.size())) { break; }
                update::status status{update::status::invalid_state}; ::std::vector<unsigned char> result;
                if(operation == 0 && payload.empty()) { status = update::status::ok; }
                else if(operation == 8 && payload.empty()) { status = transition_facts(result); }
                else if(operation == 9)
                {
                    update::permit p{};
                    status = update::permit::decode(payload, p) ? runtime_update_launch_host_api(p) : update::status::invalid_input;
                }
                // Before authorization there is no worker and no business call,
                // prepare, READY, cancel or shutdown operation can open the gate.
                if(!reply(status, result)) { break; }
                update::facts facts{}; (void)runtime_update_observe_host_api(facts);
                if(operation == 9 && status == update::status::ok && facts.active) { return 0; }
            }
            runtime_update_disconnected_host_api(); return 75;
        }
        int serve(::std::atomic<bool>& entry_finished, void const* runtime_module, business_export const& allowed_call)
        {
            using namespace ::uwvm2::runtime::lib;
            for(;;)
            {
                // Versioned bounded framing. Peer closure never implies ABORT.
                ::std::array<unsigned char, 12> header{};
                if(!receive(header.data(), header.size())) { break; }
                if(::std::memcmp(header.data(), "UUP1", 4) != 0) { break; }
                auto const operation{u32(header.data()+4)}, length{u32(header.data()+8)};
                if(length > update::maximum_image_bytes + update::maximum_manifest_bytes + update::permit::wire_size + 4u || operation > 9) { break; }
                ::std::vector<unsigned char> payload(length);
                if(!receive(payload.data(), payload.size())) { break; }
                update::status status{update::status::invalid_input}; ::std::vector<unsigned char> result;
                if(operation == 0 && payload.empty()) { status = update::status::ok; }
                else if(operation == 1 && payload.size() >= update::permit::wire_size + 4)
                {
                    update::permit p{};
                    if(update::permit::decode(::std::span{payload}.first(update::permit::wire_size), p))
                    {
                        auto const count{u32(payload.data()+update::permit::wire_size)};
                        auto const prefix{update::permit::wire_size + 4u + static_cast<::std::size_t>(count)*8u};
                        if(count > 0 && count <= update::maximum_functions && prefix < payload.size())
                        {
                            ::std::vector<::std::uint64_t> functions(count);
                            for(::std::size_t i{}; i < count; ++i)
                            { for(unsigned j{}; j < 8; ++j) { functions[i] |= static_cast<::std::uint64_t>(payload[update::permit::wire_size+4+i*8+j]) << (j*8); } }
                            status = runtime_update_prepare_host_api(p, ::std::span{payload}.subspan(prefix), functions,
                                ::std::chrono::steady_clock::now()+::std::chrono::seconds{5});
                        }
                    }
                }
                else if(operation == 2 && payload.empty()) { status = runtime_update_ready_host_api(); }
                else if(operation == 3)
                {
                    update::permit p{};
                    if(update::permit::decode(payload, p)) { status = runtime_update_decide_host_api(p); }
                }
                else if(operation == 4 && payload.empty()) { status = runtime_update_cancel_private_host_api(); }
                else if(operation == 7 && payload.size() >= update::permit::wire_size + 4u)
                {
                    update::permit p{};
                    auto const size{u32(payload.data()+update::permit::wire_size)};
                    auto const prefix{update::permit::wire_size + 4u};
                    if(size <= update::maximum_manifest_bytes && size <= payload.size()-prefix && payload.size()-prefix-size >= 8u &&
                       update::permit::decode(::std::span{payload}.first(update::permit::wire_size), p))
                    {
                        status = runtime_update_prepare_transition_host_api(p, ::std::span{payload}.subspan(prefix+size),
                            ::std::span{payload}.subspan(prefix,size), ::std::chrono::steady_clock::now()+::std::chrono::seconds{5});
                    }
                }
                else if(operation == 8 && payload.empty())
                {
                    status = transition_facts(result);
                }
                else if(operation == 9)
                {
                    update::permit p{};
                    if(update::permit::decode(payload, p)) { status = runtime_update_launch_host_api(p); }
                }
                else if(operation == 5 && payload.size() >= 12)
                {
                    update::facts facts{}; (void)runtime_update_observe_host_api(facts);
                    auto const index{u32(payload.data())}, input{u32(payload.data()+4)}, output{u32(payload.data()+8)};
                    if(!allowed_call.enabled || index != allowed_call.function_index) { status = update::status::unauthorized; }
                    else if(!entry_finished.load(::std::memory_order_acquire) || !facts.active) { status = update::status::busy; }
                    else if(input == allowed_call.input_bytes && output == allowed_call.output_bytes && payload.size() == 12u+input)
                    {
                        result.resize(output);
                        bool exited{}, call_failed{};
                        if(allowed_call.debug_control)
                        {
                            // The coordinator keeps its original native thread.
                            // Guest execution runs on a registered worker so an
                            // IDE pause leaves both authenticated channels alive.
                            struct call_state
                            {
                                void const* module{}; ::std::uint32_t index{};
                                ::std::vector<unsigned char> input{}, output{}; bool exited{};
                            };
                            auto state{::std::make_shared<call_state>()};
                            state->module = runtime_module; state->index = index;
                            state->input.assign(payload.begin()+12, payload.end()); state->output.resize(output);
                            auto launched{runtime_launch_llvm_jit_debug_guest_worker_host_api(allowed_call.debug_control, state,
                                +[](void* raw) noexcept
                                {
                                    auto& call{*static_cast<call_state*>(raw)}; guest_exit::clear();
                                    llvm_jit_call_raw_host_api(call.module, call.index, call.output.data(), call.output.size(),
                                        call.input.data(), call.input.size()); call.exited = guest_exit::last.requested;
                                })};
                            if(!launched.worker) { call_failed = true; result.clear(); }
                            else
                            {
                                for(;;)
                                {
                                    auto const joined{runtime_join_llvm_jit_debug_guest_worker_until_host_api(launched.worker,
                                        ::std::chrono::steady_clock::now()+::std::chrono::milliseconds{100})};
                                    if(joined.actual.status == ::fast_io::thread_join_status::joined) { break; }
                                    if(joined.actual.status != ::fast_io::thread_join_status::pending) { ::std::_Exit(75); }
                                }
                                result = ::std::move(state->output); exited = state->exited;
                            }
                        }
                        else
                        {
                            guest_exit::clear();
                            llvm_jit_call_raw_host_api(runtime_module, index, result.data(), result.size(), payload.data()+12, input);
                            exited = guest_exit::last.requested;
                        }
                        update::facts after{}; (void)runtime_update_observe_host_api(after);
                        status = call_failed ? update::status::busy :
                            result.size() != output || exited || !after.active ? update::status::validation_failed : update::status::ok;
                        if(status != update::status::ok) { result.clear(); }
                    }
                }
                else if(operation == 6 && payload.empty())
                {
                    update::facts facts{}; (void)runtime_update_observe_host_api(facts);
                    if(facts.active && entry_finished.load(::std::memory_order_acquire))
                    { (void)reply(update::status::ok); return 0; }
                    status = update::status::busy;
                }
                // Reserved operations, including arbitrary writes, are denied.
                if(!reply(status, result)) { break; }
            }
            runtime_update_disconnected_host_api(); update::facts final{}; (void)runtime_update_observe_host_api(final);
            return final.active ? 0 : 75;
        }
    };
#endif
}
