/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/utils/thread/monotonic_clock.h>
#endif
#include "protocol.h"
#include "cancellation_scope.h"
#include <chrono>
#include <mutex>
#include <utility>
#include <cstring>
#include <optional>
#include <climits>
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
    // Zero is the wire representation of an explicitly unlimited deadline.
    // A finite configured zero is rejected as expired before encoding a frame.
    inline deadline after(std::uint32_t ms)
    { return ms ? ::uwvm2::utils::host_clock::now()+std::chrono::milliseconds(ms) : deadline::max(); }
    inline std::uint32_t min_timeout(std::uint32_t left,std::uint32_t right) noexcept
    { return !left ? right : !right ? left : (left<right?left:right); }
    inline std::optional<std::uint32_t> remaining_ms(deadline until) noexcept
    {
        if(until==deadline::max()) { return std::uint32_t{}; }
        auto const now{::uwvm2::utils::host_clock::now()};
        if(!::uwvm2::utils::host_clock::available() || now>=until) { return std::nullopt; }
        auto const ns{std::chrono::duration_cast<std::chrono::nanoseconds>(until-now).count()};
        auto const millis{static_cast<std::uint64_t>(ns/1000000+(ns%1000000!=0))};
        return static_cast<std::uint32_t>(millis>UINT32_MAX?UINT32_MAX:millis);
    }
    inline int poll_timeout(deadline until) noexcept
    {
        auto const left{remaining_ms(until)};
        if(!left) { return 0; }
        if(!*left) { return -1; }
        return *left>INT_MAX?INT_MAX:static_cast<int>(*left);
    }
    inline bool wait_ready(int fd,short events,deadline until,int cancellation_fd) noexcept
    {
        // The provider owns this waiter through the entire RPC, including its
        // reply. A quota wait selected to finish on proc_exit keeps that owner.
        auto* waiter{synchronous_wait_context::present()?synchronous_wait_context::waiter():nullptr};
        if(synchronous_wait_context::present() && !waiter) { errno=ENOMEM;return false; }
        for(;;)
        {
            if(cancellation_requested(cancellation_fd)) { return false; }
            auto const now{::uwvm2::utils::host_clock::now()};
            if(!::uwvm2::utils::host_clock::available()) { errno=EIO;return false; }
            if(now>=until) { errno=ETIMEDOUT;return false; }
            if(waiter)
            {
                short native_revents{};
                auto result{waiter->until(until,cancellation_fd,fd,events,&native_revents)};
                if(result==quota_wait_result::cancelled) { errno=ECANCELED;return false; }
                if(result!=quota_wait_result::ready) { errno=EIO;return false; }
                if(cancellation_requested(cancellation_fd)) { return false; }
                // until() already sampled this descriptor together with the
                // timer and cancellation sources. A fresh readiness syscall
                // adds no ownership proof; the caller's nonblocking transfer
                // handles readiness consumed by another reader.
                if(native_revents) { return true; }
            }
            else
            {
                pollfd watched[2]{{fd,events,0},{cancellation_fd,POLLIN,0}};
                auto const count{::poll(watched,2,poll_timeout(until))};
                if(count<0 && errno==EINTR) { continue; }
                if(count<0) { return false; }
                if(watched[1].revents || cancellation_requested(cancellation_fd)) { errno=ECANCELED;return false; }
                if(watched[0].revents) { return true; }
            }
        }
    }
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
            auto left{poll_timeout(until)};
            if(left == 0) { errno = ETIMEDOUT; return false; }
            ssize_t const count{writing ? ::send(fd, bytes, n, MSG_NOSIGNAL | MSG_DONTWAIT) : ::recv(fd, bytes, n, MSG_DONTWAIT)};
            if(count < 0)
            {
                if(errno == EINTR) { continue; }
                if(errno != EAGAIN && errno != EWOULDBLOCK) { return false; }
                // The attempted syscall and a scheduling interruption consume
                // the same deadline; never restart its budget before waiting.
                if(cancellation_requested(cancellation_fd)) { return false; }
                left = poll_timeout(until);
                if(left == 0) { errno = ETIMEDOUT; return false; }
                if(!wait_ready(fd,static_cast<short>(writing?POLLOUT:POLLIN),until,cancellation_fd)) { return false; }
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
        if(f.payload.size()>max_payload_bytes) { errno=EINVAL; return false; }
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
        // A claimed length alone cannot reserve the complete body. Grow only
        // as the authenticated stream supplies it, under the same deadline.
        f.payload.clear();
        while(f.payload.size()<length)
        {
            auto const offset=f.payload.size();
            auto const chunk=(std::min)(static_cast<std::size_t>(length)-offset,std::size_t{65536});
            f.payload.resize(offset+chunk);
            if(!transfer(fd,f.payload.data()+offset,chunk,false,until,cancellation_fd))return false;
        }
        return !cancellation_requested(cancellation_fd);
    }
    // A connection-local read buffer avoids a second recv for small replies.
    // This is a chunk size, not a protocol length limit. Larger frames are
    // streamed; surplus bytes remain attached to this exact authenticated
    // connection and are discarded on disconnect. Headers are validated before
    // their payload length can allocate memory.
    class buffered_frame_reader
    {
        ::std::array<::std::uint8_t,1024> bytes_{};
        ::std::size_t begin_{},end_{};
        bool fill(int fd,deadline until,int stop)
        {
            for(;;)
            {
                if(cancellation_requested(stop)) { return false; }
                if(poll_timeout(until)==0) {errno=ETIMEDOUT;return false;}
                auto n=::recv(fd,bytes_.data(),bytes_.size(),MSG_DONTWAIT);
                if(n>0)
                {
                    begin_=0;end_=static_cast<::std::size_t>(n);
                    return !cancellation_requested(stop);
                }
                if(!n) {errno=ECONNRESET;return false;}
                if(errno==EINTR) {continue;}
                if(errno!=EAGAIN&&errno!=EWOULDBLOCK) {return false;}
                if(!wait_ready(fd,POLLIN,until,stop)) {return false;}
            }
        }
        bool take(int fd,::std::uint8_t* out,::std::size_t n,deadline until,int stop)
        {
            if(cancellation_requested(stop)) {return false;}
            while(n)
            {
                if(poll_timeout(until)==0) {errno=ETIMEDOUT;return false;}
                if(begin_==end_&&n>=bytes_.size()) {return transfer(fd,out,n,false,until,stop);}
                if(begin_==end_&&!fill(fd,until,stop)) {return false;}
                auto count=(::std::min)(n,end_-begin_);
                ::std::memcpy(out,bytes_.data()+begin_,count);
                begin_+=count;out+=count;n-=count;
                if(cancellation_requested(stop)) {return false;}
            }
            return !cancellation_requested(stop);
        }
    public:
        void reset() noexcept {begin_=end_=0;}
        bool read(int fd,frame& result,deadline until,int stop=-1)
        {
            ::std::array<::std::uint8_t,header_bytes> header;
            ::std::uint32_t size{};
            if(!take(fd,header.data(),header.size(),until,stop)) {return false;}
            if(!decode_header(header.data(),header.size(),result,size)) {errno=EPROTO;return false;}
            if(cancellation_requested(stop)) {return false;}
            if(size<=bytes_.size())
            {
                result.payload.resize(size);
                return take(fd,result.payload.data(),size,until,stop);
            }
            result.payload.clear();
            while(result.payload.size()<size)
            {
                auto const offset=result.payload.size();
                auto const chunk=(::std::min)(static_cast<::std::size_t>(size)-offset,::std::size_t{65536});
                result.payload.resize(offset+chunk);
                if(!take(fd,result.payload.data()+offset,chunk,until,stop))return false;
            }
            return !cancellation_requested(stop);
        }
    };
    inline bool wait_connected(int fd,deadline until,int cancellation_fd)
    {
        for(;;)
        {
            if(cancellation_requested(cancellation_fd)) { return false; }
            auto left{poll_timeout(until)};
            if(left==0) { errno=ETIMEDOUT;return false; }
            if(!wait_ready(fd,POLLOUT,until,cancellation_fd)) { return false; }
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
#if defined(__linux__)
        buffered_frame_reader reader_{};
#endif
        [[maybe_unused]] std::uint8_t backend_{};
        [[maybe_unused]] std::uint64_t session_{}, next_{1}, retired_session_{};
        std::uint32_t peer_pid_{},peer_uid_{};
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
            reader_.reset();
#endif
            if(session_) { retired_session_=session_; }
            fd_=-1; session_=0;peer_pid_=0;peer_uid_=0;
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
            if(path.empty()||path[0]!='/'||path.size()>=sizeof a.sun_path||backend<1||backend>2) { return 22; }
            if(::uwvm2::utils::host_clock::now()>=until) { return 110; }
            if(!::uwvm2::utils::host_clock::available()) { return 5; }
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
            peer_pid_=pid;peer_uid_=uid;
            if(cancellation_requested(stop)) { return cancelled(stop,cancellation_phase::connect_wait,false,old_session); }
            backend_=backend; next_=1;
            frame hello; hello.type=kind::hello; hello.backend=backend_; hello.request_id=next_++; hello.timeout_ms=timeout_ms;
            frame reply;
            if(cancellation_requested(stop)) { return cancelled(stop,cancellation_phase::hello,false,old_session); }
            // From this point a false write can have sent a prefix or the whole
            // hello. It is never used as a zero-peer-effect cancellation proof.
            if(!write_frame(fd_,hello,until,stop)||!reader_.read(fd_,reply,until,stop))
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
            if(next_==std::numeric_limits<std::uint64_t>::max()) { return {22,{}}; }
            if(::uwvm2::utils::host_clock::now()>=until) { return {110,{}}; }
            if(!::uwvm2::utils::host_clock::available()) { return {5,{}}; }
            frame f; f.type=kind::request; f.backend=backend_; f.session_id=session_; f.request_id=next_++; f.timeout_ms=timeout_ms;
            if(!encode_request_view({operation,alias,payload},f.payload)) { return {22,{}}; }
            auto remaining{remaining_ms(until)};
            // Serialization and request preparation consume the entry budget.
            // An unwritten expired request cannot affect the peer, so it need
            // not disconnect a still-live session or retire its resources.
            if(!remaining) { return {110,{}}; }
            f.timeout_ms=*remaining; frame reply;
            if(cancellation_requested(stop)) { return {cancelled(stop,cancellation_phase::request_transfer,false,session_),{}}; }
            checked_call checked{f.request_id,f.session_id,peer_pid_,peer_uid_,true,false};
            // A failing write_frame is not evidence that no request prefix was
            // sent. Any cancellation from here retains possible-peer-effects.
            if(!write_frame(fd_,f,until,stop))
            {
                auto e{errno};if(e==ECANCELED) { return {cancelled(stop,cancellation_phase::request_transfer,true,session_),{},checked}; }
                disconnect();return {e==ETIMEDOUT?110u:107u,{},checked};
            }
            if(!reader_.read(fd_,reply,until,stop))
            {
                auto e{errno};if(e==ECANCELED) { return {cancelled(stop,cancellation_phase::response_transfer,true,session_),{},checked}; }
                disconnect();return {e==ETIMEDOUT?110u:107u,{},checked};
            }
            if(cancellation_requested(stop)) { return {cancelled(stop,cancellation_phase::response_transfer,true,session_),{},checked}; }
            if(reply.type!=kind::response||reply.backend!=backend_||reply.session_id!=session_||reply.response_id!=f.request_id||reply.request_id!=0)
            { disconnect(); return {13,{},checked}; }
            // session.bind acknowledges an exact policy manifest with an empty
            // body. Do not silently accept a future/foreign binding protocol or
            // expose its data as a successful authorization acknowledgement.
            if(operation=="session.bind"&&!reply.payload.empty())
            { disconnect(); return {13,{},checked}; }
            if(cancellation_requested(stop)) { return {cancelled(stop,cancellation_phase::response_transfer,true,session_),{},checked}; }
            checked.response_checked=true;
            return {reply.error,std::move(reply.payload),checked};
#else
            (void)operation; (void)alias; (void)payload; (void)timeout_ms; return {95,{}};
#endif
        }
    };
}
