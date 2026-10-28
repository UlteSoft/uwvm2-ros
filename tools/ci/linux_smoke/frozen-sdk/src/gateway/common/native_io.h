#pragma once
#include <array>
#include <charconv>
#include <cstring>
#include <cstdint>
#include <string>
#include <string_view>
#include <stdexcept>
#include <system_error>
#include <type_traits>
#if defined(__linux__)
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/sysmacros.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <cerrno>
#endif

namespace uwvm2::ros_gateway::native_io
{
    // The native ROS builds are C++17; vendored fast_io needs C++20 concepts.
    // Keep this adapter independent of fast_io and ROS implementation types.
    template<std::size_t Capacity> class fixed_text
    {
        static_assert(Capacity>0,"fixed_text needs nonempty storage");
        std::array<char,Capacity> storage_{};
        std::size_t used_{};
    public:
        bool append(std::string_view value) noexcept
        {
            if(value.size()>Capacity-used_) { return false; }
            if(!value.empty()) { std::memcpy(storage_.data()+used_,value.data(),value.size()); }
            used_+=value.size();return true;
        }
        template<typename Integer> bool number(Integer value) noexcept
        {
            static_assert(std::is_integral_v<Integer>&&!std::is_same_v<std::remove_cv_t<Integer>,bool>,"non-bool integer formatting only");
            auto result{std::to_chars(storage_.data()+used_,storage_.data()+Capacity,value)};
            if(result.ec!=std::errc{}) { return false; }
            used_=static_cast<std::size_t>(result.ptr-storage_.data());return true;
        }
        std::string_view view() const noexcept { return {storage_.data(),used_}; }
    };
    /** Build a bounded numeric native identity in one final string allocation.
     * C++17 ROS deployments cannot include the C++20 fast_io headers; to_chars
     * is the locale-independent compatibility path used by this narrow shim. */
    inline std::string goal_identity(std::uint64_t nonce,std::uint64_t id)
    {
        fixed_text<64> text;
        if(!text.append("uwvm-")||!text.number(nonce)||!text.append("-")||!text.number(id))
        { throw std::length_error("native goal identity exceeded its fixed bound"); }
        return std::string{text.view()};
    }
    inline std::string hexadecimal(std::uint8_t const* values,std::size_t count,std::string_view prefix={})
    {
        if(prefix.size()>std::string{}.max_size()||count>(std::string{}.max_size()-prefix.size())/2) { throw std::length_error("hexadecimal result size"); }
        static constexpr char digits[]{"0123456789abcdef"};
        std::string result(prefix);result.resize(prefix.size()+count*2);
        for(std::size_t i{};i<count;++i)
        { result[prefix.size()+i*2]=digits[values[i]>>4];result[prefix.size()+i*2+1]=digits[values[i]&15]; }
        return result;
    }
    inline bool matches_hexadecimal(std::uint8_t const* values,std::size_t count,std::string_view expected,std::string_view prefix={}) noexcept
    {
        if(expected.size()<prefix.size()||(expected.size()-prefix.size())/2!=count||(expected.size()-prefix.size())%2||expected.substr(0,prefix.size())!=prefix) { return false; }
        static constexpr char digits[]{"0123456789abcdef"};
        for(std::size_t i{};i<count;++i)
        { if(expected[prefix.size()+i*2]!=digits[values[i]>>4]||expected[prefix.size()+i*2+1]!=digits[values[i]&15]) { return false; } }
        return true;
    }
    inline bool ascii_space(char c) noexcept
    { return c==' '||c=='\t'||c=='\n'||c=='\r'||c=='\f'||c=='\v'; }
    inline std::size_t split(std::string_view line,std::array<std::string_view,7>& tokens) noexcept
    {
        std::size_t count{},p{};
        while(p<line.size())
        {
            while(p<line.size()&&ascii_space(line[p])) { ++p; }
            if(p==line.size()) { break; }
            if(count==tokens.size()) { return tokens.size(); }
            auto first{p};while(p<line.size()&&!ascii_space(line[p])) { ++p; }
            tokens[count++]={line.data()+first,p-first};
        }
        return count;
    }
#if defined(__linux__)
    class owned_fd
    {
        int fd_{-1};
    public:
        explicit owned_fd(int fd=-1) noexcept:fd_{fd} {}
        owned_fd(owned_fd const&)=delete;owned_fd& operator=(owned_fd const&)=delete;
        ~owned_fd() { if(fd_>=0) { (void)::close(fd_); } }
        int get() const noexcept { return fd_; }
    };
    class config_lines
    {
        owned_fd file_;
        std::array<char,4096> input_{},line_{};
        std::size_t cursor_{},available_{};
    public:
        explicit config_lines(std::string const& path):file_{::open(path.c_str(),O_RDONLY|O_CLOEXEC|O_NOFOLLOW|O_NONBLOCK)}
        {
            struct stat st{};
            if(file_.get()<0) { throw std::runtime_error("cannot open trusted config"); }
            if(::fstat(file_.get(),&st)<0||!S_ISREG(st.st_mode)||st.st_uid!=::geteuid()||(st.st_mode&0022))
            { throw std::runtime_error("config must be owned regular file, not group/world writable"); }
            // A FIFO must not block before fstat can reject it. Restore the
            // regular file's original blocking flags after checking the FD.
            auto flags{::fcntl(file_.get(),F_GETFL,0)};
            if(flags<0||::fcntl(file_.get(),F_SETFL,flags&~O_NONBLOCK)<0)
            { throw std::runtime_error("cannot configure trusted config descriptor"); }
        }
        bool next(std::string_view& line)
        {
            std::size_t used{};
            for(;;)
            {
                if(cursor_==available_)
                {
                    ssize_t count;
                    do { count=::read(file_.get(),input_.data(),input_.size()); }while(count<0&&errno==EINTR);
                    if(count<0) { throw std::runtime_error("cannot read trusted config"); }
                    if(!count) { if(!used) { return false; }line={line_.data(),used};return true; }
                    cursor_=0;available_=static_cast<std::size_t>(count);
                }
                auto c{input_[cursor_++]};
                if(c=='\n') { line={line_.data(),used};return true; }
                if(used==line_.size()) { throw std::runtime_error("oversized config line"); }
                line_[used++]=c;
            }
        }
    };
    inline bool write_all(int fd,std::string_view bytes) noexcept
    {
        std::size_t done{};
        while(done<bytes.size())
        {
            auto count{::write(fd,bytes.data()+done,bytes.size()-done)};
            if(count<0&&errno==EINTR) { continue; }
            if(count<=0) { return false; }done+=static_cast<std::size_t>(count);
        }
        return true;
    }
    namespace audit_details
    {
        struct errno_guard { int saved{errno};~errno_guard() { errno=saved; } };
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
    }
    // The caller supplies the complete record, including its newline. Native
    // Noetic/Jazzy builds are C++17: use to_chars/owned_fd rather than importing
    // the vendored fast_io concepts API. No original stderr OFD flags are set.
    // A complete pipe record <= PIPE_BUF is accepted atomically or dropped.
    // TTY/socket and regular-file failures may leave partial output; regular
    // files retain the shared original offset/append and have no hard deadline.
    inline bool write_audit_text(std::string_view bytes) noexcept
    {
        audit_details::errno_guard preserve_errno;
        struct stat original{};
        if(::fstat(STDERR_FILENO,&original)<0) { return false; }
        int pinned{::fcntl(STDERR_FILENO,F_DUPFD_CLOEXEC,3)};
        if(pinned<0) { return false; }
        owned_fd owner{pinned};struct stat snapshot{};
        if(::fstat(pinned,&snapshot)<0||!audit_details::same_file(original,snapshot)) { return false; }
        auto flags{::fcntl(pinned,F_GETFL,0)};
        if(flags<0||(flags&O_ACCMODE)==O_RDONLY) { return false; }
        if(S_ISREG(snapshot.st_mode)) { return write_all(pinned,bytes); }
        if(bytes.size()>4096) { return false; }
        if(S_ISSOCK(snapshot.st_mode))
        {
            for(unsigned attempt{};attempt!=16;++attempt)
            {
                auto count{::send(pinned,bytes.data(),bytes.size(),MSG_DONTWAIT|MSG_NOSIGNAL)};
                if(count<0&&errno==EINTR) { continue; }
                return count>=0&&static_cast<std::size_t>(count)==bytes.size();
            }
            return false;
        }
        bool pipe{S_ISFIFO(snapshot.st_mode)};
        bool terminal{S_ISCHR(snapshot.st_mode)&&::isatty(pinned)};
        bool null_device{S_ISCHR(snapshot.st_mode)&&snapshot.st_rdev==::makedev(1,3)};
        if(!pipe&&!terminal&&!null_device) { return false; }
        if(pipe)
        { auto atomic{::fpathconf(pinned,_PC_PIPE_BUF)};if(atomic<=0||bytes.size()>static_cast<std::uint64_t>(atomic)) { return false; } }
        fixed_text<64> path;
        if(!path.append("/proc/self/fd/")||!path.number(pinned)||!path.append(std::string_view{"\0",1})) { return false; }
        // A dup alone shares file-status flags. Reopening the pinned proc FD
        // creates an independent nonblocking OFD, verified against the pin.
        int independent{::open(path.view().data(),O_WRONLY|O_CLOEXEC|O_NONBLOCK|O_NOCTTY)};
        if(independent<0) { return false; }
        owned_fd output{independent};struct stat actual{};
        if(::fstat(independent,&actual)<0||!audit_details::same_file(snapshot,actual)) { return false; }
        auto output_flags{::fcntl(independent,F_GETFL,0)};
        if(output_flags<0||!(output_flags&O_NONBLOCK)) { return false; }
        audit_details::pipe_signal_guard signal;
        if(!signal.valid()) { return false; }
        for(unsigned attempt{};attempt!=16;++attempt)
        {
            auto count{::write(independent,bytes.data(),bytes.size())};
            if(count<0&&errno==EINTR) { continue; }
            if(count<0&&errno==EPIPE) { signal.broken_pipe(); }
            return count>=0&&static_cast<std::size_t>(count)==bytes.size();
        }
        return false;
    }

    /** Emit one bounded diagnostic through the same checked descriptor path as
     * audit records. Exception messages cannot grow an unbounded stream buffer
     * or leave interleaved iostream fragments from concurrent session threads.
     * No guest permission is created by this trusted Gateway diagnostic. */
    inline void diagnostic(std::string_view prefix,char const* detail) noexcept
    {
        fixed_text<3072> record;
        if(prefix.size()>128||!record.append(prefix)) { return; }
        std::size_t length{};
        if(detail) { while(length<2900&&detail[length]) { ++length; } }
        if(!record.append({detail?detail:"",length})) { return; }
        if(length==2900&&!record.append(" [truncated]")) { return; }
        if(record.append("\n")) { (void)write_audit_text(record.view()); }
    }

#endif
}
