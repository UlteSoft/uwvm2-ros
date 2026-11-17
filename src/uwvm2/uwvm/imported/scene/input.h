/** @file
 * @brief Provision trusted scene inputs as digest-verified sealed snapshots.
 * @details Only the deployment preparation phase calls this host-side helper,
 * after reserving its finite object/byte budgets and before guest activation.
 * CLI parsing, --ros-scene-check and guest callbacks never invoke it. The plan
 * path is trusted deployment input; it is not accepted from guest memory.
 * Opening/reading an artifact later requires separate exact scene authority.
 */
#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/scene/artifacts.h>
#include <uwvm2/imported/ros/scene/storage.h>
#include <algorithm>
#include <array>
#include <cerrno>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <new>
#include <span>
#include <stdexcept>
#include <string_view>
#include <utility>
#if defined(__linux__)
#include <fast_io_crypto.h>
#include <fcntl.h>
#include <linux/memfd.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif
#endif

namespace uwvm2::uwvm::imported::scene
{
#if defined(__linux__)
    namespace input_details
    {
        namespace model=::uwvm2::imported::ros::scene;
        using descriptor=model::storage::detail::descriptor;
        /** @brief Preserve Linux provisioning failures without exposing source names. */
        inline model::error failure() noexcept
        {return static_cast<model::error>(errno>0?errno:5);}

        /** @brief Resolve only nonsymlink components using metadata-only descriptors.
         * @details O_PATH cannot activate a FIFO/device through a read-open.
         * Every intermediate component must be an actual directory; the final
         * component remains pinned even if another process replaces its name.
         * Parent traversal, embedded NULs and oversized components are refused.
         * Only absolute paths are accepted; no cwd or environment expansion runs.
         */
        inline model::error pin_source(std::string_view path,descriptor& output) noexcept
        {
            if(path.empty()||path.front()!='/'||path.size()>4095||path.find('\0')!=path.npos||path.back()=='/')return model::error::invalid;
            descriptor current{::open("/",O_PATH|O_DIRECTORY|O_NOFOLLOW|O_CLOEXEC)};
            if(current.value<0)return failure();
            std::size_t cursor=1;
            for(;;)
            {
                while(cursor<path.size()&&path[cursor]=='/')++cursor;
                if(cursor==path.size())return model::error::invalid;
                auto end=path.find('/',cursor);bool final=end==path.npos;
                if(final)end=path.size();
                auto component=path.substr(cursor,end-cursor);
                if(component==".."||component.size()>255)return model::error::invalid;
                std::array<char,256> name{};
                for(std::size_t index=0;index<component.size();++index)name[index]=component[index];
                descriptor next{::openat(current.value,name.data(),O_PATH|O_NOFOLLOW|O_CLOEXEC|(final?0:O_DIRECTORY))};
                if(next.value<0)return failure();
                struct ::stat metadata{};if(::fstat(next.value,&metadata))return failure();
                if((final&&!S_ISREG(metadata.st_mode))||(!final&&!S_ISDIR(metadata.st_mode)))return model::error::denied;
                if(final){output=std::move(next);return model::error::success;}
                current=std::move(next);cursor=end+1;
            }
        }

        /** @brief Read a finite extent, retrying interruptions but not EOF drift. */
        inline model::error read_chunk(int fd,std::uint64_t offset,std::span<std::byte> bytes,std::size_t& count) noexcept
        {
            count=0;
            if(!model::storage::detail::native_range(offset,bytes.size()))return model::error::too_large;
            for(;;)
            {
                auto done=::pread(fd,bytes.data(),bytes.size(),static_cast<::off_t>(offset));
                if(done<0){if(errno==EINTR)continue;return failure();}
                if(!done)return model::error::conflict;
                count=static_cast<std::size_t>(done);return model::error::success;
            }
        }
    }
#endif

    /** @brief Prepare exactly one approved immutable input before guest execution.
     * @details The caller reserves aggregate bytes/objects and validates the
     * sealed-input binding and input purpose before calling. Generation must
     * be positive, declared bytes must fit native offsets, and guest transfers
     * are bounded to 4 MiB. Zero-byte input and zero transfer allowance are real
     * values. No default authority or rate limit is installed here.
     *
     * Linux pins an O_PATH regular inode before a readable open, then reopens
     * that exact descriptor through /proc/self/fd and checks inode/type/size.
     * Symlinks, FIFOs, devices, directories and parent traversal are rejected.
     * Copying uses a fixed 64 KiB workspace and exact declared size. A private
     * memfd receives WRITE/GROW/SHRINK/SEAL seals before its bytes are hashed;
     * only the expected complete SHA256 is admitted. The original may change
     * afterward without altering the snapshot. A digest authenticates selected
     * bytes, not their decoder/schema or publisher; those remain deployment
     * responsibilities. No source path/FD is exposed by the resulting object.
     *
     * The output is replaced only after all checks and backend admission pass.
     * Failures close every temporary FD and preserve any previous output. The
     * caller must release its pending budget reservation on failure. Platforms
     * without this reviewed sealing implementation return unsupported.
     */
    inline ::uwvm2::imported::ros::scene::error prepare_input(
        ::uwvm2::imported::ros::scene::artifacts::input_plan const& plan,
        ::uwvm2::imported::ros::scene::storage::principal who,
        ::uwvm2::imported::ros::scene::purpose use,
        std::shared_ptr<::uwvm2::imported::ros::scene::storage::immutable_input>& output) noexcept
    {
        namespace model=::uwvm2::imported::ros::scene;
#if defined(__linux__)
        try
        {
            if(!plan.generation||!model::storage::valid(who)||!model::storage::input_purpose(use))return model::error::invalid;
            if(plan.bytes>model::storage::detail::offset_limit||plan.transfer>4u*1024u*1024u||
               plan.transfer>model::storage::detail::transfer_limit)return model::error::too_large;
            input_details::descriptor pinned;
            if(auto status=input_details::pin_source(plan.path,pinned);status!=model::error::success)return status;
            struct ::stat identity{};if(::fstat(pinned.value,&identity))return input_details::failure();
            if(identity.st_size<0||static_cast<std::uint64_t>(identity.st_size)!=plan.bytes)return model::error::conflict;
            std::array<char,64> descriptor_path{};
            constexpr std::string_view prefix="/proc/self/fd/";
            for(std::size_t index=0;index<prefix.size();++index)descriptor_path[index]=prefix[index];
            auto converted=std::to_chars(descriptor_path.data()+prefix.size(),descriptor_path.data()+descriptor_path.size()-1,pinned.value);
            if(converted.ec!=std::errc{})return model::error::invalid;
            input_details::descriptor source{::open(descriptor_path.data(),O_RDONLY|O_CLOEXEC|O_NONBLOCK|O_NOCTTY)};
            if(source.value<0)return input_details::failure();
            struct ::stat reopened{};if(::fstat(source.value,&reopened))return input_details::failure();
            if(!S_ISREG(reopened.st_mode)||!model::storage::detail::same_file(identity,reopened)||
               reopened.st_size!=identity.st_size)return model::error::conflict;
            input_details::descriptor snapshot{static_cast<int>(::syscall(SYS_memfd_create,"uwvm-scene-input",MFD_CLOEXEC|MFD_ALLOW_SEALING))};
            if(snapshot.value<0)return errno==ENOSYS?model::error::unsupported:input_details::failure();
            std::array<std::byte,64u*1024u> buffer{};
            for(std::uint64_t offset{};offset<plan.bytes;)
            {
                auto size=static_cast<std::size_t>(std::min<std::uint64_t>(buffer.size(),plan.bytes-offset));std::size_t count{};
                if(auto status=input_details::read_chunk(source.value,offset,{buffer.data(),size},count);status!=model::error::success)return status;
                for(std::size_t written{};written<count;)
                {
                    auto done=::pwrite(snapshot.value,buffer.data()+written,count-written,static_cast<::off_t>(offset+written));
                    if(done<0){if(errno==EINTR)continue;return input_details::failure();}
                    if(!done)return static_cast<model::error>(5);
                    written+=static_cast<std::size_t>(done);
                }
                offset+=count;
            }
            struct ::stat final_source{};
            if(::fstat(source.value,&final_source))return input_details::failure();
            if(final_source.st_size!=identity.st_size||!model::storage::detail::same_file(identity,final_source))return model::error::conflict;
            constexpr int seals=F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL;
            if(::fcntl(snapshot.value,F_ADD_SEALS,seals))return input_details::failure();
            auto actual_seals=::fcntl(snapshot.value,F_GET_SEALS);
            if(actual_seals<0)return input_details::failure();
            if((actual_seals&seals)!=seals)return model::error::denied;
            struct ::stat sealed{};if(::fstat(snapshot.value,&sealed))return input_details::failure();
            if(sealed.st_size<0||static_cast<std::uint64_t>(sealed.st_size)!=plan.bytes)return model::error::conflict;
            ::fast_io::sha256_context digest{};
            for(std::uint64_t offset{};offset<plan.bytes;)
            {
                auto size=static_cast<std::size_t>(std::min<std::uint64_t>(buffer.size(),plan.bytes-offset));std::size_t count{};
                if(auto status=input_details::read_chunk(snapshot.value,offset,{buffer.data(),size},count);status!=model::error::success)return status;
                digest.update(buffer.data(),buffer.data()+count);offset+=count;
            }
            digest.do_final();std::array<std::byte,32> actual{};digest.digest_to_byte_ptr(actual.data());
            unsigned difference{};
            for(std::size_t index=0;index<actual.size();++index)difference|=std::to_integer<unsigned>(actual[index])^plan.digest[index];
            if(difference)return model::error::denied;
            std::shared_ptr<model::storage::immutable_input> prepared;
            auto status=model::storage::immutable_input::create(snapshot.value,std::move(who),use,plan.bytes,plan.transfer,prepared);
            if(status!=model::storage::success)return static_cast<model::error>(status);
            output=std::move(prepared);return model::error::success;
        }
        catch(std::bad_alloc const&){return static_cast<model::error>(12);}
        catch(std::length_error const&){return model::error::too_large;}
#else
        (void)plan;(void)who;(void)use;(void)output;return model::error::unsupported;
#endif
    }
}
