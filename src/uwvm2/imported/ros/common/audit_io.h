/*************************************************************
 * UWVM2 ROS Runtime Extension. APL-2.0.                     *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
#include <fast_io.h>
#endif
#include <array>
#include <atomic>
#include <cstdint>
#include <limits>
#include <string_view>
#if defined(__linux__)
#include <cerrno>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#endif

namespace uwvm2::imported::ros::audit_io
{
    // Trusted-runtime diagnostics only: no Guest import, handle or ABI. A
    // delivered record means all bytes were accepted by the selected sink.
    // Counters are monotonic, saturating, and do not add unbudgeted log records.
    inline std::atomic<std::uint64_t> delivered{},dropped{};
    inline void increment(std::atomic<std::uint64_t>& counter) noexcept
    {
        auto value{counter.load(std::memory_order_relaxed)};
        while(value!=std::numeric_limits<std::uint64_t>::max()&&
              !counter.compare_exchange_weak(value,value+1,std::memory_order_relaxed)) {}
    }
    inline std::uint64_t delivered_records() noexcept { return delivered.load(std::memory_order_relaxed); }
    inline std::uint64_t dropped_records() noexcept { return dropped.load(std::memory_order_relaxed); }

#if defined(__linux__)
    namespace details
    {
        struct errno_guard { int value{errno};~errno_guard() { errno=value; } };
        inline bool same_file(struct stat const& a,struct stat const& b) noexcept
        { return a.st_dev==b.st_dev&&a.st_ino==b.st_ino&&(a.st_mode&S_IFMT)==(b.st_mode&S_IFMT); }
        class pipe_signal_guard
        {
            sigset_t set_{},old_{};
            bool active_{},previously_pending_{},consume_{};
        public:
            pipe_signal_guard() noexcept
            {
                if(::sigemptyset(&set_)||::sigaddset(&set_,SIGPIPE)||::pthread_sigmask(SIG_BLOCK,&set_,&old_)) { return; }
                active_=true;sigset_t pending{};
                if(::sigpending(&pending)<0) { (void)::pthread_sigmask(SIG_SETMASK,&old_,nullptr);active_=false;return; }
                previously_pending_=::sigismember(&pending,SIGPIPE)==1;
            }
            pipe_signal_guard(pipe_signal_guard const&)=delete;
            bool valid() const noexcept { return active_; }
            void broken_pipe() noexcept { consume_=true; }
            ~pipe_signal_guard()
            {
                if(!active_) { return; }
                if(consume_&&!previously_pending_)
                {
                    timespec zero{};
                    for(unsigned attempt{};attempt!=16;++attempt)
                    { if(::sigtimedwait(&set_,nullptr,&zero)>=0||errno!=EINTR) { break; } }
                }
                (void)::pthread_sigmask(SIG_SETMASK,&old_,nullptr);
            }
        };
        inline bool regular_write(int fd,std::string_view record)
        {
#if defined(__cpp_exceptions)
            // A pinned duplicate shares the original regular-file OFD, so its
            // position and append semantics are unchanged. No flags are set.
            ::fast_io::io::println(::fast_io::posix_io_observer{fd},record);return true;
#else
            // Error-reporting fast_io device operations terminate without
            // exceptions. Keep best-effort behavior for that build profile.
            for(auto part:{record,std::string_view{"\n"}})
            {
                std::size_t done{};unsigned interrupted{};
                while(done<part.size())
                {
                    auto n{::write(fd,part.data()+done,part.size()-done)};
                    if(n<0&&errno==EINTR&&++interrupted<16) { continue; }
                    if(n<=0) { return false; }done+=static_cast<std::size_t>(n);
                }
            }
            return true;
#endif
        }
        inline bool linux_write(std::string_view record)
        {
            struct stat original{};
            if(::fstat(STDERR_FILENO,&original)<0) { return false; }
            int pinned{::fcntl(STDERR_FILENO,F_DUPFD_CLOEXEC,3)};
            if(pinned<0) { return false; }
            ::fast_io::posix_file owner{pinned};struct stat snapshot{};
            if(::fstat(pinned,&snapshot)<0||!same_file(original,snapshot)) { return false; }
            auto flags{::fcntl(pinned,F_GETFL,0)};
            if(flags<0||(flags&O_ACCMODE)==O_RDONLY) { return false; }
            if(S_ISREG(snapshot.st_mode)) { return regular_write(pinned,record); }

            // Keep one bounded, allocation-free record including its newline.
            // Nonblocking pipes accept <= PIPE_BUF atomically or reject it; an
            // oversized record is dropped before any bytes are written.
            std::array<char,4096> bytes;
            if(record.size()>=bytes.size()) { return false; }
            ::fast_io::obuffer_view buffer{bytes.data(),bytes.data()+bytes.size()};
            ::fast_io::print(buffer,record,"\n");auto length{record.size()+1};
            if(S_ISSOCK(snapshot.st_mode))
            {
                for(unsigned attempt{};attempt!=16;++attempt)
                {
                    auto n{::send(pinned,bytes.data(),length,MSG_DONTWAIT|MSG_NOSIGNAL)};
                    if(n<0&&errno==EINTR) { continue; }
                    return n>=0&&static_cast<std::size_t>(n)==length;
                }
                return false;
            }
            bool pipe{S_ISFIFO(snapshot.st_mode)};
            bool terminal{S_ISCHR(snapshot.st_mode)&&::isatty(pinned)};
            bool null_device{S_ISCHR(snapshot.st_mode)&&snapshot.st_rdev==::makedev(1,3)};
            if(!pipe&&!terminal&&!null_device) { return false; }
            if(pipe)
            { auto atomic{::fpathconf(pinned,_PC_PIPE_BUF)};if(atomic<=0||length>static_cast<std::uint64_t>(atomic)) { return false; } }
            std::array<char,64> path;
            ::fast_io::obuffer_view path_buffer{path.data(),path.data()+path.size()};
            ::fast_io::print(path_buffer,"/proc/self/fd/",::fast_io::mnp::dec(pinned),std::string_view{"\0",1});
            // dup + F_SETFL would also change the original stderr OFD. Opening
            // its pinned proc reference creates an independent nonblocking OFD.
            int independent{::open(path.data(),O_WRONLY|O_CLOEXEC|O_NONBLOCK|O_NOCTTY)};
            if(independent<0) { return false; }
            ::fast_io::posix_file output{independent};struct stat actual{};
            if(::fstat(independent,&actual)<0||!same_file(snapshot,actual)) { return false; }
            auto output_flags{::fcntl(independent,F_GETFL,0)};
            if(output_flags<0||!(output_flags&O_NONBLOCK)) { return false; }
            pipe_signal_guard signal;
            if(!signal.valid()) { return false; }
            for(unsigned attempt{};attempt!=16;++attempt)
            {
                auto n{::write(independent,bytes.data(),length)};
                if(n<0&&errno==EINTR) { continue; }
                if(n<0&&errno==EPIPE) { signal.broken_pipe(); }
                return n>=0&&static_cast<std::size_t>(n)==length;
            }
            return false;
        }
    }
#endif
    inline bool write_record(std::string_view record) noexcept
    {
        bool complete{};
#if defined(__linux__)
        details::errno_guard preserve_errno;
#endif
#if defined(__cpp_exceptions)
        try
#endif
        {
#if defined(__linux__)
            complete=details::linux_write(record);
#else
            ::fast_io::io::println(::fast_io::err(),record);complete=true;
#endif
        }
#if defined(__cpp_exceptions)
        catch(...) {}
#endif
        increment(complete?delivered:dropped);return complete;
    }
}
