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
#include <sys/uio.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#endif

namespace uwvm2::imported::ros::audit_io
{
    enum class stream : unsigned char { error, output };
    enum class failure : unsigned char
    { none, bad_descriptor, replaced_descriptor, unsupported_sink, oversized, would_block, partial_write, peer_closed, io_error, signal_guard, exception, count };
    // Trusted-runtime diagnostics only: no Guest import, handle or ABI. A
    // delivered record means all bytes were accepted by the selected sink.
    // Counters are monotonic, saturating, and do not add unbudgeted log records.
    inline std::atomic<std::uint64_t> delivered{},dropped{};
    inline std::array<std::atomic<std::uint64_t>,static_cast<std::size_t>(failure::count)> failures{};
    inline void increment(std::atomic<std::uint64_t>& counter) noexcept
    {
        auto value{counter.load(std::memory_order_relaxed)};
        while(value!=std::numeric_limits<std::uint64_t>::max()&&
              !counter.compare_exchange_weak(value,value+1,std::memory_order_relaxed)) {}
    }
    inline std::uint64_t delivered_records() noexcept { return delivered.load(std::memory_order_relaxed); }
    inline std::uint64_t dropped_records() noexcept { return dropped.load(std::memory_order_relaxed); }
    inline std::uint64_t failed_records(failure reason) noexcept
    {
        auto index{static_cast<std::size_t>(reason)};
        return index<failures.size() ? failures[index].load(std::memory_order_relaxed) : 0;
    }

#if defined(__linux__)
    namespace details
    {
        struct errno_guard { int value{errno};~errno_guard() { errno=value; } };
        inline bool same_file(struct stat const& a,struct stat const& b) noexcept
        { return a.st_dev==b.st_dev&&a.st_ino==b.st_ino&&(a.st_mode&S_IFMT)==(b.st_mode&S_IFMT); }
        class write_signal_guard
        {
            sigset_t set_{},old_{};
            bool active_{},previously_pending_{},consume_{};
        public:
            explicit write_signal_guard(int signo) noexcept
            {
                if(::sigemptyset(&set_)||::sigaddset(&set_,signo)||::pthread_sigmask(SIG_BLOCK,&set_,&old_)) { return; }
                active_=true;sigset_t pending{};
                if(::sigpending(&pending)<0) { (void)::pthread_sigmask(SIG_SETMASK,&old_,nullptr);active_=false;return; }
                previously_pending_=::sigismember(&pending,signo)==1;
            }
            write_signal_guard(write_signal_guard const&)=delete;
            bool valid() const noexcept { return active_; }
            void consume_generated_signal() noexcept { consume_=true; }
            ~write_signal_guard()
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
        /** @brief Write the record and newline to the pinned regular file.
         * @details Keep the shared file offset and append mode intact. Linux
         * can signal the writing thread on its file-size limit. Consume only
         * the signal associated with an EFBIG write when none was pending on
         * entry. No process-wide signal disposition is changed. A short write
         * followed by a failure is recorded as a partial delivery.
         */
        inline bool regular_write(int fd,std::string_view record,failure& reason) noexcept
        {
            if(record.size()>=static_cast<std::size_t>(std::numeric_limits<ssize_t>::max()))
            { reason=failure::oversized;return false; }
            write_signal_guard signal{SIGXFSZ};
            if(!signal.valid()) { reason=failure::signal_guard;return false; }
            char newline='\n';
            iovec parts[2]{{const_cast<char*>(record.data()),record.size()},{&newline,1}};
            unsigned index=record.empty()?1u:0u,interrupted{};
            bool started{};
            while(index<2)
            {
                auto written=::writev(fd,parts+index,static_cast<int>(2-index));
                if(written<0 && errno==EINTR && ++interrupted<16) continue;
                if(written<=0)
                {
                    if(written<0 && errno==EFBIG) signal.consume_generated_signal();
                    reason=started?failure::partial_write:failure::io_error;
                    return false;
                }
                started=true;
                auto consumed=static_cast<std::size_t>(written);
                while(index<2 && consumed>=parts[index].iov_len)
                { consumed-=parts[index].iov_len;++index; }
                if(consumed)
                {
                    parts[index].iov_base=static_cast<char*>(parts[index].iov_base)+consumed;
                    parts[index].iov_len-=consumed;
                }
            }
            return true;
        }
        inline bool write_complete(ssize_t count,std::size_t expected,failure& reason) noexcept
        {
            if(count>=0 && static_cast<std::size_t>(count)==expected) { return true; }
            if(count>=0) { reason=failure::partial_write; }
            else if(errno==EAGAIN || errno==EWOULDBLOCK) { reason=failure::would_block; }
            else if(errno==EPIPE || errno==ECONNRESET) { reason=failure::peer_closed; }
            return false;
        }
        inline bool linux_write(std::string_view record,failure& reason,int descriptor)
        {
            reason=failure::io_error;
            struct stat original{};
            if(::fstat(descriptor,&original)<0) { reason=failure::bad_descriptor;return false; }
            int pinned{::fcntl(descriptor,F_DUPFD_CLOEXEC,3)};
            if(pinned<0) { return false; }
            ::fast_io::posix_file owner{pinned};struct stat snapshot{};
            if(::fstat(pinned,&snapshot)<0||!same_file(original,snapshot)) { reason=failure::replaced_descriptor;return false; }
            auto flags{::fcntl(pinned,F_GETFL,0)};
            if(flags<0||(flags&O_ACCMODE)==O_RDONLY) { reason=failure::bad_descriptor;return false; }
            if(S_ISREG(snapshot.st_mode)) { return regular_write(pinned,record,reason); }

            // Keep one bounded, allocation-free record including its newline.
            // Nonblocking pipes accept <= PIPE_BUF atomically or reject it; an
            // oversized record is dropped before any bytes are written.
            std::array<char,4096> bytes;
            if(record.size()>=bytes.size()) { reason=failure::oversized;return false; }
            ::fast_io::obuffer_view buffer{bytes.data(),bytes.data()+bytes.size()};
            ::fast_io::print(buffer,record,"\n");auto length{record.size()+1};
            if(S_ISSOCK(snapshot.st_mode))
            {
                for(unsigned attempt{};attempt!=16;++attempt)
                {
                    auto n{::send(pinned,bytes.data(),length,MSG_DONTWAIT|MSG_NOSIGNAL)};
                    if(n<0&&errno==EINTR) { continue; }
                    return write_complete(n,length,reason);
                }
                return false;
            }
            bool pipe{S_ISFIFO(snapshot.st_mode)};
            bool terminal{S_ISCHR(snapshot.st_mode)&&::isatty(pinned)};
            bool null_device{S_ISCHR(snapshot.st_mode)&&snapshot.st_rdev==::makedev(1,3)};
            if(!pipe&&!terminal&&!null_device) { reason=failure::unsupported_sink;return false; }
            if(pipe)
            { auto atomic{::fpathconf(pinned,_PC_PIPE_BUF)};if(atomic<=0||length>static_cast<std::uint64_t>(atomic)) { reason=failure::oversized;return false; } }
            std::array<char,64> path;
            ::fast_io::obuffer_view path_buffer{path.data(),path.data()+path.size()};
            ::fast_io::print(path_buffer,"/proc/self/fd/",::fast_io::mnp::dec(pinned),std::string_view{"\0",1});
            // dup + F_SETFL would also change the original selected OFD. Opening
            // its pinned proc reference creates an independent nonblocking OFD.
            int independent{::open(path.data(),O_WRONLY|O_CLOEXEC|O_NONBLOCK|O_NOCTTY)};
            if(independent<0) { if(errno==ENXIO || errno==EPIPE) { reason=failure::peer_closed; }return false; }
            ::fast_io::posix_file output{independent};struct stat actual{};
            if(::fstat(independent,&actual)<0||!same_file(snapshot,actual)) { reason=failure::replaced_descriptor;return false; }
            auto output_flags{::fcntl(independent,F_GETFL,0)};
            if(output_flags<0||!(output_flags&O_NONBLOCK)) { return false; }
            write_signal_guard signal{SIGPIPE};
            if(!signal.valid()) { reason=failure::signal_guard;return false; }
            for(unsigned attempt{};attempt!=16;++attempt)
            {
                auto n{::write(independent,bytes.data(),length)};
                if(n<0&&errno==EINTR) { continue; }
                if(n<0&&errno==EPIPE) { signal.consume_generated_signal(); }
                return write_complete(n,length,reason);
            }
            return false;
        }
    }
#endif
    inline bool write_record_to(stream destination,std::string_view record) noexcept
    {
        bool complete{};
        failure reason{failure::io_error};
#if defined(__linux__)
        details::errno_guard preserve_errno;
#endif
#if defined(__cpp_exceptions)
        try
#endif
        {
#if defined(__linux__)
            complete=details::linux_write(record,reason,destination==stream::output?STDOUT_FILENO:STDERR_FILENO);
#else
            if(destination==stream::output) { ::fast_io::io::println(::fast_io::out(),record); }
            else { ::fast_io::io::println(::fast_io::err(),record); }complete=true;
#endif
        }
#if defined(__cpp_exceptions)
        catch(...) { reason=failure::exception; }
#endif
        increment(complete?delivered:dropped);
        if(!complete) { increment(failures[static_cast<std::size_t>(reason)]); }
        return complete;
    }
    inline bool write_record(std::string_view record) noexcept
    { return write_record_to(stream::error,record); }
}
