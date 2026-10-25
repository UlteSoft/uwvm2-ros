/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include "protocol.h"
#include "cancellation_scope.h"
#include <chrono>
#include <mutex>
#include <utility>
#include <cstring>
#if defined(__linux__)
# include <sys/socket.h>
# include <sys/un.h>
# include <sys/stat.h>
# include <sys/random.h>
# include <unistd.h>
# include <poll.h>
# include <fcntl.h>
# include <errno.h>
#endif

namespace uwvm2::imported::ros::ipc
{
#if defined(__linux__)
    using deadline=std::chrono::steady_clock::time_point;
    inline deadline after(std::uint32_t ms) { return std::chrono::steady_clock::now()+std::chrono::milliseconds(ms); }
    /**
     * @brief Transfer a bounded stream extent under the original absolute deadline.
     * @details Attempt nonblocking I/O before waiting: ready sockets need no
     * redundant readiness syscall. Cancellation is still checked before and
     * after every successful transfer, and wins over simultaneous readiness.
     * A failed transfer may have changed the peer; callers must retain their
     * existing possible-peer-effects handling and discard incomplete frames.
     */
    inline bool transfer(int fd, std::uint8_t* bytes, std::size_t n, bool writing, deadline until, int cancellation_fd = -1)
    {
        if(cancellation_requested(cancellation_fd)) { return false; }
        while(n)
        {
            if(cancellation_requested(cancellation_fd)) { return false; }
            auto left{std::chrono::duration_cast<std::chrono::milliseconds>(until - std::chrono::steady_clock::now()).count()};
            if(left <= 0) { errno = ETIMEDOUT; return false; }
            ssize_t const count{writing ? ::send(fd, bytes, n, MSG_NOSIGNAL | MSG_DONTWAIT) : ::recv(fd, bytes, n, MSG_DONTWAIT)};
            if(count < 0)
            {
                if(errno == EINTR) { continue; }
                if(errno != EAGAIN && errno != EWOULDBLOCK) { return false; }
                // The attempted syscall and a scheduling interruption consume
                // the same deadline; never restart its budget before waiting.
                if(cancellation_requested(cancellation_fd)) { return false; }
                left = std::chrono::duration_cast<std::chrono::milliseconds>(until - std::chrono::steady_clock::now()).count();
                if(left <= 0) { errno = ETIMEDOUT; return false; }
                pollfd watched[2]{{fd, static_cast<short>(writing ? POLLOUT : POLLIN), 0}, {cancellation_fd, POLLIN, 0}};
                int const ready{::poll(watched, cancellation_fd < 0 ? 1 : 2, static_cast<int>(left))};
                if(ready < 0 && errno == EINTR) { continue; }
                if(ready == 0) { errno = ETIMEDOUT; return false; }
                if(ready < 0) { return false; }
                if(cancellation_fd >= 0 && watched[1].revents) { errno = ECANCELED; return false; }
                continue;
            }
            if(count == 0) { errno = ECONNRESET; return false; }
            bytes += count;
            n -= static_cast<std::size_t>(count);
            // A failed whole transfer may already have sent/received a prefix.
            // Cancellation is observed without draining the level eventfd.
            if(cancellation_requested(cancellation_fd)) { return false; }
        }
        return !cancellation_requested(cancellation_fd);
    }
    /**
     * @brief Write one unchanged wire frame with a bounded small-message buffer.
     * @details Small control requests and replies need one stream transfer instead
     * of separate header and payload writes. The complete extent still uses the
     * same deadline, nonblocking I/O and cancellation checks. Larger payloads use
     * the existing segmented path; no allocation scales with an untrusted length.
     */
    inline bool write_frame(int fd,frame const& f,deadline until,int cancellation_fd=-1)
    {
        if(cancellation_requested(cancellation_fd)) { return false; }
        if(f.payload.size()>max_payload_bytes||!f.timeout_ms||f.timeout_ms>max_timeout_ms) { errno=EINVAL; return false; }
        auto b{encode_header_fixed(f)};
        if(!f.payload.empty() && f.payload.size() <= 1024u)
        {
            ::std::array<::std::uint8_t, header_bytes + 1024u> bytes;
            ::std::memcpy(bytes.data(), b.data(), b.size());
            ::std::memcpy(bytes.data() + b.size(), f.payload.data(), f.payload.size());
            // Only the initialized header and payload prefix can reach send().
            return transfer(fd, bytes.data(), b.size() + f.payload.size(), true, until, cancellation_fd);
        }
        return transfer(fd,b.data(),b.size(),true,until,cancellation_fd)&&transfer(fd,const_cast<std::uint8_t*>(f.payload.data()),f.payload.size(),true,until,cancellation_fd);
    }
    inline bool read_frame(int fd,frame& f,deadline until,int cancellation_fd=-1)
    {
        if(cancellation_requested(cancellation_fd)) { return false; }
        std::uint8_t b[header_bytes];
        std::uint32_t length{};
        if(!transfer(fd,b,sizeof b,false,until,cancellation_fd)) { return false; }
        if(!decode_header(b,sizeof b,f,length)) { errno=EPROTO; return false; }
        if(cancellation_requested(cancellation_fd)) { return false; }
        f.payload.resize(length);
        return transfer(fd,f.payload.data(),length,false,until,cancellation_fd);
    }
    inline bool wait_connected(int fd,deadline until,int cancellation_fd)
    {
        for(;;)
        {
            if(cancellation_requested(cancellation_fd)) { return false; }
            auto left{std::chrono::duration_cast<std::chrono::milliseconds>(until-std::chrono::steady_clock::now()).count()};
            if(left<=0) { errno=ETIMEDOUT;return false; }
            pollfd watched[2]{{fd,POLLOUT,0},{cancellation_fd,POLLIN,0}};
            auto result{::poll(watched,cancellation_fd<0?1:2,static_cast<int>(left))};
            if(result<0&&errno==EINTR) { continue; }
            if(result<0) { return false; }
            if(!result) { errno=ETIMEDOUT;return false; }
            if(cancellation_fd>=0&&watched[1].revents) { errno=ECANCELED;return false; }
            if(cancellation_requested(cancellation_fd)) { return false; }
            int error{};socklen_t size{sizeof error};
            if(::getsockopt(fd,SOL_SOCKET,SO_ERROR,&error,&size)<0||size!=sizeof error) { return false; }
            if(error) { errno=error;return false; }
            return !cancellation_requested(cancellation_fd);
        }
    }
    inline bool peer_credentials(int fd,std::uint32_t& uid,std::uint32_t& pid)
    {
        ucred c{}; socklen_t n{sizeof c};
        if(::getsockopt(fd,SOL_SOCKET,SO_PEERCRED,&c,&n)<0||n!=sizeof c||c.pid<=0) { return false; }
        uid=static_cast<std::uint32_t>(c.uid); pid=static_cast<std::uint32_t>(c.pid); return true;
    }
    inline std::uint64_t random_session()
    {
        std::uint64_t id{};
        do { if(::getrandom(&id,sizeof id,0)!=sizeof id) { return 0; } } while(!id);
        return id;
    }
#endif
    class client
    {
        int fd_{-1};
        [[maybe_unused]] std::uint8_t backend_{};
        [[maybe_unused]] std::uint64_t session_{}, next_{1}, retired_session_{};
        std::mutex mutex_{};
        cancellation_observation cancellation_{};
        void observe_cancellation(int stop,cancellation_phase phase,bool possible,std::uint64_t old_session)noexcept
        {
            cancellation_.observed=true;cancellation_.possible_peer_effects|=possible;
            if(old_session) { if(!cancellation_.first_session)cancellation_.first_session=old_session;cancellation_.old_session=old_session; }
            if(cancellation_.observations==UINT64_MAX)cancellation_.observations_saturated=true;
            else ++cancellation_.observations;
            cancellation_.phase=phase;cancellation_.cause=probe_cancellation(stop);
            if(cancellation_.cause==cancellation_probe::clear)cancellation_.cause=cancellation_probe::probe_failed;
        }
        std::uint32_t cancelled(int stop,cancellation_phase phase,bool possible,std::uint64_t old_session)noexcept
        { observe_cancellation(stop,phase,possible,old_session?old_session:retired_session_);disconnect();return 125; }
        void disconnect() noexcept
        {
#if defined(__linux__)
            if(fd_>=0) { ::close(fd_); }
#endif
            if(session_) { retired_session_=session_; }
            fd_=-1; session_=0;
        }
    public:
        client()=default;
        client(client const&)=delete;
        client& operator=(client const&)=delete;
        ~client() { disconnect(); }
        std::uint64_t session_id() noexcept { std::lock_guard<std::mutex> lock{mutex_};return session_; }
        void close() noexcept { std::lock_guard<std::mutex> lock{mutex_}; disconnect(); }
        // Trusted Native summary only; no Guest import or descriptor ownership.
        // Mutex acquisition itself is not claimed interruptible. Read after the
        // controller's real worker drain when a bounded nonwaiting view matters.
        cancellation_observation cancellation_status()noexcept
        { std::lock_guard<std::mutex> lock{mutex_};return cancellation_; }
        std::uint32_t connect(std::string const& path,std::uint8_t backend,std::uint32_t expected_uid,std::uint32_t timeout_ms)
        {
            auto stop{current_cancellation_fd()};
#if defined(__linux__)
            auto until{after(timeout_ms)};
#endif
            // These real mutex waits cannot be interrupted by poll. Controller
            // group stop first wakes the owner RPC, then joins every borrower.
            std::lock_guard<std::mutex> lock{mutex_};auto old_session{session_};
            if(cancellation_requested(stop)) { return cancelled(stop,cancellation_phase::connect_admission,false,old_session); }
            disconnect();
#if defined(__linux__)
            sockaddr_un a{}; a.sun_family=AF_UNIX;
            if(path.empty()||path[0]!='/'||path.size()>=sizeof a.sun_path||backend<1||backend>2||!timeout_ms||timeout_ms>max_timeout_ms) { return 22; }
            if(std::chrono::steady_clock::now()>=until) { return 110; }
            struct stat st{};
            if(::lstat(path.c_str(),&st)<0||!S_ISSOCK(st.st_mode)||st.st_uid!=expected_uid||(st.st_mode&0007)) { return 13; }
            if(cancellation_requested(stop)) { return cancelled(stop,cancellation_phase::connect_admission,false,old_session); }
            std::memcpy(a.sun_path,path.c_str(),path.size()+1);
            fd_=::socket(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,0);
            if(fd_<0) { return 107; }
            if(cancellation_requested(stop)) { return cancelled(stop,cancellation_phase::connect_wait,false,old_session); }
            if(::connect(fd_,reinterpret_cast<sockaddr*>(&a),sizeof a)<0)
            {
                if(errno!=EINPROGRESS) { disconnect(); return 107; }
                if(!wait_connected(fd_,until,stop))
                {
                    auto error{errno};
                    if(error==ECANCELED) { return cancelled(stop,cancellation_phase::connect_wait,false,old_session); }
                    disconnect();return error==ETIMEDOUT?110u:107u;
                }
            }
            if(cancellation_requested(stop)) { return cancelled(stop,cancellation_phase::connect_wait,false,old_session); }
            std::uint32_t uid{},pid{};
            if(!peer_credentials(fd_,uid,pid)||uid!=expected_uid) { disconnect(); return 13; }
            if(cancellation_requested(stop)) { return cancelled(stop,cancellation_phase::connect_wait,false,old_session); }
            backend_=backend; next_=1;
            frame hello; hello.type=kind::hello; hello.backend=backend_; hello.request_id=next_++; hello.timeout_ms=timeout_ms;
            frame reply;
            if(cancellation_requested(stop)) { return cancelled(stop,cancellation_phase::hello,false,old_session); }
            // From this point a false write can have sent a prefix or the whole
            // hello. It is never used as a zero-peer-effect cancellation proof.
            if(!write_frame(fd_,hello,until,stop)||!read_frame(fd_,reply,until,stop))
            {
                auto error{errno};
                if(error==ECANCELED) { return cancelled(stop,cancellation_phase::hello,true,old_session); }
                disconnect();return error==ETIMEDOUT?110u:107u;
            }
            if(cancellation_requested(stop)) { return cancelled(stop,cancellation_phase::hello,true,old_session); }
            // Retain the original complete hello correlation rule.
            if(reply.type!=kind::response||reply.backend!=backend_||reply.request_id!=0||reply.response_id!=hello.request_id||!reply.session_id||!reply.payload.empty()) { disconnect(); return 13; }
            if(reply.error) { disconnect(); return reply.error; }
            session_=reply.session_id;
            if(cancellation_requested(stop)) { return cancelled(stop,cancellation_phase::hello,true,session_); }
            return 0;
#else
            (void)path; (void)backend; (void)expected_uid; (void)timeout_ms; return 95;
#endif
        }
        // Metadata is borrowed only until encode_request_view synchronously
        // copies it into the owned frame; no view survives this call.
        call_result call(std::string_view operation,std::string_view alias,std::vector<std::uint8_t> payload,std::uint32_t timeout_ms)
        {
            auto stop{current_cancellation_fd()};
#if defined(__linux__)
            auto until{after(timeout_ms)};
#endif
            std::lock_guard<std::mutex> lock{mutex_};
            if(cancellation_requested(stop)) { return {cancelled(stop,cancellation_phase::call_admission,false,session_),{}}; }
#if defined(__linux__)
            if(fd_<0) { return {107,{}}; }
            if(!timeout_ms||timeout_ms>max_timeout_ms||next_==std::numeric_limits<std::uint64_t>::max()) { return {22,{}}; }
            if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }
            frame f; f.type=kind::request; f.backend=backend_; f.session_id=session_; f.request_id=next_++; f.timeout_ms=timeout_ms;
            if(!encode_request_view({operation,alias,payload},f.payload)) { return {22,{}}; }
            auto remaining{std::chrono::duration_cast<std::chrono::milliseconds>(until-std::chrono::steady_clock::now()).count()};
            // Serialization and request preparation consume the entry budget.
            // An unwritten expired request cannot affect the peer, so it need
            // not disconnect a still-live session or retire its resources.
            if(remaining<=0) { return {110,{}}; }
            f.timeout_ms=static_cast<std::uint32_t>(remaining); frame reply;
            if(cancellation_requested(stop)) { return {cancelled(stop,cancellation_phase::request_transfer,false,session_),{}}; }
            // A failing write_frame is not evidence that no request prefix was
            // sent. Any cancellation from here retains possible-peer-effects.
            if(!write_frame(fd_,f,until,stop))
            {
                auto e{errno};if(e==ECANCELED) { return {cancelled(stop,cancellation_phase::request_transfer,true,session_),{}}; }
                disconnect();return {e==ETIMEDOUT?110u:107u,{}};
            }
            if(!read_frame(fd_,reply,until,stop))
            {
                auto e{errno};if(e==ECANCELED) { return {cancelled(stop,cancellation_phase::response_transfer,true,session_),{}}; }
                disconnect();return {e==ETIMEDOUT?110u:107u,{}};
            }
            if(cancellation_requested(stop)) { return {cancelled(stop,cancellation_phase::response_transfer,true,session_),{}}; }
            if(reply.type!=kind::response||reply.backend!=backend_||reply.session_id!=session_||reply.response_id!=f.request_id||reply.request_id!=0)
            { disconnect(); return {13,{}}; }
            // session.bind acknowledges an exact policy manifest with an empty
            // body. Do not silently accept a future/foreign binding protocol or
            // expose its data as a successful authorization acknowledgement.
            if(operation=="session.bind"&&!reply.payload.empty())
            { disconnect(); return {13,{}}; }
            if(cancellation_requested(stop)) { return {cancelled(stop,cancellation_phase::response_transfer,true,session_),{}}; }
            return {reply.error,std::move(reply.payload)};
#else
            (void)operation; (void)alias; (void)payload; (void)timeout_ms; return {95,{}};
#endif
        }
    };
}
