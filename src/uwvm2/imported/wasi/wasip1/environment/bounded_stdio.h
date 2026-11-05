/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS) && defined(__linux__)
#ifndef UWVM_MODULE
#include <array>
#include <climits>
#include <cstring>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <pthread.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::imported::wasi::wasip1::environment
{
    // Synchronous Linux standard-stream adapter. Byte-stream pipe writes and
    // stream sockets use nonblocking operations plus cancellable readiness
    // waits. Reads preserve native pipe packet/drop semantics. An in-flight
    // device/filesystem syscall is not reported as cancelled or drained early.
    // No inherited open-file-description flags are ever modified.
    inline ::uwvm2::imported::wasi::wasip1::abi::errno_t bounded_stdio(
        ::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_t& object,
        ::fast_io::io_scatter_t const* vectors, ::std::size_t count,
        ::std::uint64_t total, bool writing, ::std::size_t& completed) noexcept
    {
        using error=::uwvm2::imported::wasi::wasip1::abi::errno_t;
        using kind=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e;
        auto const& storage{object.wasi_fd.ptr->wasi_fd_storage};
        int descriptor{-1};
        if(storage.type==kind::file) descriptor=storage.storage.file_fd.native_handle();
        else if(storage.type==kind::file_observer) descriptor=storage.storage.file_observer.native_handle();
        else return error::enotsup;
        if(host_wait_cancelled()) return error::ecanceled;
        auto const flags{::fcntl(descriptor,F_GETFL)};
        if(flags<0) return error::ebadf;
        int socket_type{};::socklen_t extent{sizeof(socket_type)};
        bool const socket{::getsockopt(descriptor,SOL_SOCKET,SO_TYPE,&socket_type,&extent)==0};
        if(socket)
        { if(extent!=sizeof(socket_type)||socket_type!=SOCK_STREAM) return error::enotsup; }
        else if(errno!=ENOTSOCK) return error::eio;
        bool const pipe_descriptor{!socket && ::fcntl(descriptor,1032)>0}; // F_GETPIPE_SZ
        // Packet-mode writes must retain their packet boundaries: splice does
        // not provide the write(2) contract for that descriptor mode.
        bool const splice_pipe{pipe_descriptor && writing && (flags&O_DIRECT)==0};
        completed=0u;
        if(total==0u) return error::esuccess;
        auto const wanted{static_cast<::std::size_t>(total<PIPE_BUF?total:PIPE_BUF)};
        ::std::array<::std::byte,PIPE_BUF> bounce{};
        if(writing)
        {
            ::std::size_t at{};
            for(::std::size_t i{};i<count && at<wanted;++i)
            { auto n{vectors[i].len<wanted-at?vectors[i].len:wanted-at};::std::memcpy(bounce.data()+at,vectors[i].base,n);at+=n; }
        }
        struct staging
        {
            int ends[2]{-1,-1};
            ~staging(){for(int fd:ends)if(fd>=0)static_cast<void>(::close(fd));}
        } pipe;
        if(splice_pipe)
        {
            if(::syscall(SYS_pipe2,pipe.ends,O_NONBLOCK|O_CLOEXEC)!=0) return error::eio;
            if(writing && ::write(pipe.ends[1],bounce.data(),wanted)!=static_cast<::ssize_t>(wanted)) return error::eio;
        }
        struct signal_scope
        {
            ::sigset_t mask{},old{};bool active{},pending{};
            signal_scope() noexcept
            {
                ::sigemptyset(&mask);::sigaddset(&mask,SIGPIPE);
                active=::pthread_sigmask(SIG_BLOCK,&mask,&old)==0;
                if(active){::sigset_t before{};if(::sigpending(&before)==0)pending=::sigismember(&before,SIGPIPE)==1;}
            }
            void consume() noexcept
            {if(active&&!pending){::timespec zero{};static_cast<void>(::sigtimedwait(&mask,nullptr,&zero));}}
            ~signal_scope(){if(active)static_cast<void>(::pthread_sigmask(SIG_SETMASK,&old,nullptr));}
        } signal;
        if(!signal.active) return error::eio;
        for(;;)
        {
            if(host_wait_cancelled()) return error::ecanceled;
            if(::fcntl(descriptor,F_GETFL)!=flags) return error::eio;
            ::ssize_t transferred{};
            if(socket) transferred=writing ? ::send(descriptor,bounce.data(),wanted,MSG_DONTWAIT|MSG_NOSIGNAL) : ::recv(descriptor,bounce.data(),wanted,MSG_DONTWAIT);
            else if(splice_pipe)
                transferred=static_cast<::ssize_t>(::syscall(SYS_splice,pipe.ends[0],nullptr,descriptor,nullptr,wanted,2u)); // SPLICE_F_NONBLOCK
            else transferred=writing ? ::write(descriptor,bounce.data(),wanted) : ::read(descriptor,bounce.data(),wanted);
            if(transferred>=0)
            {
                completed=static_cast<::std::size_t>(transferred);
                if(!writing)
                {
                    ::std::size_t at{};
                    for(::std::size_t i{};i<count && at<completed;++i)
                    {auto n{vectors[i].len<completed-at?vectors[i].len:completed-at};::std::memcpy(const_cast<void*>(vectors[i].base),bounce.data()+at,n);at+=n;}
                }
                return error::esuccess;
            }
            auto const failure{errno};
            if(failure==EPIPE){signal.consume();return error::epipe;}
            if(failure==EINTR) continue;
            if(failure!=EAGAIN && failure!=EWOULDBLOCK)
            {
                switch(failure)
                {
                    case EBADF: return error::ebadf;
                    case EACCES: return error::eacces;
                    case EPERM: return error::eperm;
                    case EINVAL: return error::einval;
                    case EISDIR: return error::eisdir;
                    case ENOSPC: return error::enospc;
                    case EDQUOT: return error::edquot;
                    case EFBIG: return error::efbig;
                    case EROFS: return error::erofs;
                    case ENOMEM: return error::enomem;
                    case ENOSYS: return error::enosys;
                    case ENOTSUP: return error::enotsup;
                    case ECONNRESET: return error::econnreset;
                    default: return error::eio;
                }
            }
            if((flags&O_NONBLOCK)!=0) return error::eagain;
            auto const ready{wait_for_native_io(descriptor,static_cast<short>(writing?POLLOUT:POLLIN))};
            if(ready == quota_call_result::cancelled) return error::ecanceled;
            if(ready == quota_call_result::wait_unavailable) return error::eio;
        }
    }
}
#endif
