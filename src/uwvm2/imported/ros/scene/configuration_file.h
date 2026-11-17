/** @file
 * @brief Bounded owned configuration loading from an explicit trusted host path.
 * @details This is a deployment/CLI helper, never a guest import. A caller
 * supplies a finite exact byte count, hard parser ceiling and expected SHA256.
 * Linux opens O_PATH first to classify a pinned inode without opening a device
 * or FIFO for I/O, then reopens that already pinned regular file for reading.
 * O_NOFOLLOW rejects a final symlink; parent-directory selection remains the
 * trusted deployer's responsibility. A complete verified owned copy is the
 * result, not a reusable host file capability. Commit briefly retains two
 * exact-size copies (at most twice the parser ceiling), so failed allocation
 * leaves the previous output untouched. No socket/provider is activated.
 * Other hosts refuse this file helper; pure owned-byte configuration remains
 * portable. Allocations use the recoverable fast_io byte buffer and strings.
 *
 * The trusted path may traverse symlinked parent directories; this helper is
 * not a directory sandbox. Resolution and reads on a trusted filesystem may
 * block despite O_NONBLOCK on the regular-file descriptor. This helper does
 * not claim a storage deadline or durability guarantee. The expected digest
 * authenticates the detached bytes even when a writer changes the source;
 * size and timestamp comparisons additionally reject observed concurrent
 * mutation. A mutation after the final read cannot alter the owned result.
 */
#pragma once
#include "buffers.h"
#include <charconv>
#if defined(__linux__)
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#endif
namespace uwvm2::imported::ros::scene::configuration_file
{
    /** @brief Decode a finite trusted configuration after the caller checks its exact scope.
     * @details Empty, odd, excessive or non-lowercase input is rejected before
     * allocation. The caller must reject missing grants and duplicate or frozen
     * definitions before invoking this helper. A silent fast_io allocation
     * failure returns no_memory and preserves the previous output. No native
     * endpoint or file is opened, and successful decoding creates no authority.
     */
    inline resources::error decode_hex(std::string_view hex,std::size_t ceiling,byte_buffer& output) noexcept
    {
        using error=resources::error;
        if(hex.empty()||hex.size()%2)return error::invalid;
        if(hex.size()/2>ceiling)return error::too_large;
        auto nibble=[](char c)noexcept->int{return c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:-1;};
        for(char c:hex)if(nibble(c)<0)return error::invalid;
        try
        {
            output.resize_for_overwrite(hex.size()/2);auto destination=output.writable();
            for(std::size_t i{};i<destination.size();++i)
                destination[i]=static_cast<std::uint8_t>((nibble(hex[i*2])<<4)|nibble(hex[i*2+1]));
            return error::success;
        }
        catch(std::bad_alloc const&){return error::no_memory;}
        catch(std::length_error const&){return error::too_large;}
    }
    /** @brief Read exactly one pinned regular file and atomically publish verified bytes.
     * @param path Explicit trusted host path, nonempty, without NUL, at most 4096 bytes.
     * @param expected Nonzero exact encoded length; checked before path allocation or I/O.
     * @param digest SHA256 of the entire expected file, distinct from an embedded logical profile digest.
     * @param ceiling Caller's fixed parser ceiling; not read from the file being loaded.
     * @param output Previous content remains unchanged on every failure, including final allocation.
     * @details Linux obtains an O_PATH|O_NOFOLLOW|O_CLOEXEC identity first and
     * refuses directories, symlinks, FIFOs, sockets and devices without opening
     * them for data I/O. /proc/self/fd reopens only that retained identity;
     * fstat verifies device/inode/type and observed revision before allocation.
     * pread advances by actual short-read counts, retries EINTR and checks one
     * extra byte to reject growth. Final metadata and full digest checks precede
     * output replacement. Both descriptors close on every return or exception.
     * Allocation failure maps to no_memory; syscall errors map to io; observed
     * length/content/revision mismatches map to conflict. No provider is started.
     */
    inline resources::error read(std::string_view path,std::uint64_t expected,buffers::digest const& digest,std::size_t ceiling,byte_buffer& output) noexcept
    {
        using error=resources::error;
        if(path.empty()||path.size()>4096||path.find('\0')!=std::string_view::npos||!expected||expected>ceiling||expected>SIZE_MAX)return error::invalid;
#if defined(__linux__)
        if(expected>static_cast<std::uint64_t>(std::numeric_limits<off_t>::max()))return error::too_large;
        struct descriptor
        {
            int fd{-1};explicit descriptor(int value) noexcept:fd(value){}
            descriptor(descriptor const&)=delete;descriptor& operator=(descriptor const&)=delete;
            ~descriptor(){if(fd>=0)::close(fd);}
        };
        auto same_revision=[](struct stat const& left,struct stat const& right) noexcept
        {return left.st_dev==right.st_dev&&left.st_ino==right.st_ino&&left.st_size==right.st_size&&
            left.st_mtim.tv_sec==right.st_mtim.tv_sec&&left.st_mtim.tv_nsec==right.st_mtim.tv_nsec&&
            left.st_ctim.tv_sec==right.st_ctim.tv_sec&&left.st_ctim.tv_nsec==right.st_ctim.tv_nsec;};
        try
        {
            ::uwvm2::utils::container::std_string owned{path};
            descriptor identity{::open(owned.c_str(),O_PATH|O_NOFOLLOW|O_CLOEXEC)};if(identity.fd<0)return error::io;
            struct stat initial{};if(::fstat(identity.fd,&initial))return error::io;
            if(!S_ISREG(initial.st_mode))return error::invalid;
            if(initial.st_size<0||static_cast<std::uint64_t>(initial.st_size)!=expected)return error::conflict;
            std::array<char,64> name{};constexpr std::string_view prefix="/proc/self/fd/";
            std::copy(prefix.begin(),prefix.end(),name.begin());auto printed=std::to_chars(name.data()+prefix.size(),name.data()+name.size()-1,identity.fd);
            if(printed.ec!=std::errc{})return error::invalid;
            *printed.ptr='\0';
            descriptor source{::open(name.data(),O_RDONLY|O_CLOEXEC|O_NONBLOCK)};if(source.fd<0)return error::io;
            struct stat opened{};if(::fstat(source.fd,&opened))return error::io;
            if(!S_ISREG(opened.st_mode)||!same_revision(initial,opened))return error::conflict;
            byte_buffer bytes;bytes.resize_for_overwrite(static_cast<std::size_t>(expected));auto destination=bytes.writable();std::size_t offset{};
            while(offset<destination.size())
            {
                auto count=::pread(source.fd,destination.data()+offset,destination.size()-offset,static_cast<off_t>(offset));
                if(count<0&&errno==EINTR)continue;
                if(count<=0)return error::io;
                offset+=static_cast<std::size_t>(count);
            }
            std::uint8_t extra{};ssize_t beyond{};
            do{beyond=::pread(source.fd,&extra,1,static_cast<off_t>(expected));}while(beyond<0&&errno==EINTR);
            if(beyond<0)return error::io;
            if(beyond!=0)return error::conflict;
            struct stat after{};if(::fstat(source.fd,&after))return error::io;
            if(!same_revision(initial,after))return error::conflict;
            if(buffers::hash(bytes.view())!=digest)return error::conflict;
            output.resize_for_overwrite(bytes.view().size());
            std::copy(bytes.view().begin(),bytes.view().end(),output.writable().begin());return error::success;
        }
        catch(std::bad_alloc const&){return error::no_memory;}
        catch(std::length_error const&){return error::too_large;}
#else
        (void)digest;(void)output;return error::unsupported;
#endif
    }
}
