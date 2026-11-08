// Licensed under APL-2.0 (see LICENSE).
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
# include <cerrno>
# include <cstddef>
# include <cstdint>
# include <span>
# include <string>
# include <string_view>
# include <utility>
# if defined(__linux__)
#  include <fcntl.h>
#  include <sys/stat.h>
#  include <unistd.h>
# endif
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::diagnostic::crash
{
    // Host-owned output only. No Wasm FD/path or capability lookup reaches this
    // object. Publish a complete 0600 regular file without replacing any name.
    class dump_file final
    {
        int directory_{-1},file_{-1};
        ::std::string destination_{},temporary_{};
        bool published_{};
        inline static ::std::atomic<unsigned long long> serial_{};
    public:
        dump_file() = default;
        dump_file(dump_file const&)=delete;
        dump_file& operator=(dump_file const&)=delete;
        ~dump_file()
        {
#if defined(__linux__)
            if(file_!=-1) ::close(file_);
            if(directory_!=-1)
            {
                if(!temporary_.empty()) ::unlinkat(directory_,temporary_.c_str(),0);
                ::close(directory_);
            }
#endif
        }
        [[nodiscard]] bool open(::std::string_view path) noexcept
        {
#if defined(__linux__)
            if(directory_!=-1 || file_!=-1 || path.empty() || path.find('\0')!=path.npos) return false;
# if defined(__cpp_exceptions)
            try
            {
# endif
                auto const slash{path.find_last_of('/')};
                ::std::string parent{slash==path.npos ? "." : path.substr(0u,slash==0u ? 1u:slash)};
                destination_=path.substr(slash==path.npos ? 0u:slash+1u);
                if(destination_.empty() || destination_=="." || destination_=="..") return false;
                directory_=::open(parent.c_str(),O_RDONLY|O_CLOEXEC|O_DIRECTORY);
                if(directory_==-1) return false;
                struct stat existing{};
                if(::fstatat(directory_,destination_.c_str(),&existing,AT_SYMLINK_NOFOLLOW)==0 || errno!=ENOENT) return false;
                // Distinct non-guest-controlled names. O_EXCL is the authority,
                // not unpredictability; a pre-existing name is never followed.
                temporary_=".uwvm-core-"+::std::to_string(static_cast<unsigned long long>(::getpid()))+"-"+
                    ::std::to_string(serial_.fetch_add(1u,::std::memory_order_relaxed));
                file_=::openat(directory_,temporary_.c_str(),O_WRONLY|O_CREAT|O_EXCL|O_CLOEXEC|O_NOFOLLOW,0600);
                if(file_==-1) { temporary_.clear();return false; }
                struct stat actual{};
                return ::fstat(file_,&actual)==0 && S_ISREG(actual.st_mode) && (actual.st_mode&0777u)==0600u;
# if defined(__cpp_exceptions)
            }
            catch(...) { return false; }
# endif
#else
            (void)path;return false;
#endif
        }
        [[nodiscard]] bool write(::std::span<::std::byte const> bytes) noexcept
        {
#if defined(__linux__)
            if(file_==-1 || published_) return false;
            while(!bytes.empty())
            {
                // The encoder emits bounded transfer chunks. SSIZE_MAX remains
                // the platform syscall boundary, never an aggregate dump cap.
                auto const chunk{bytes.size()>static_cast<::std::size_t>(PTRDIFF_MAX) ?
                    static_cast<::std::size_t>(PTRDIFF_MAX):bytes.size()};
                auto const count{::write(file_,bytes.data(),chunk)};
                if(count<0 && errno==EINTR) continue;
                if(count<=0 || static_cast<::std::size_t>(count)>chunk) return false;
                bytes=bytes.subspan(static_cast<::std::size_t>(count));
            }
            return true;
#else
            (void)bytes;return false;
#endif
        }
        [[nodiscard]] bool publish() noexcept
        {
#if defined(__linux__)
            if(file_==-1 || directory_==-1 || published_ || ::fsync(file_)!=0) return false;
            // linkat cannot overwrite a file or symlink raced into destination.
            // Both names are relative to the same retained parent directory.
            if(::linkat(directory_,temporary_.c_str(),directory_,destination_.c_str(),0)!=0) return false;
            published_=true;
            if(::unlinkat(directory_,temporary_.c_str(),0)==0) temporary_.clear();
            return ::fsync(directory_)==0;
#else
            return false;
#endif
        }
        [[nodiscard]] static bool writer(void* context,::std::span<::std::byte const> bytes) noexcept
        { return context!=nullptr && static_cast<dump_file*>(context)->write(bytes); }
    };
}
