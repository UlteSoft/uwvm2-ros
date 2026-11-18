/** @file
 * @brief Reject channel aliases across every ROSv2 environment and provider kind.
 * @details Static deployment freeze uses one registry for driver, model and ROS
 * channels. A different integer fd, alias, environment or protocol cannot make
 * the same kernel endpoint an independent capability. Both pipe directions
 * share one identity and cannot be registered to distinct consumers. Session
 * identities are globally unique within the VM deployment. No data is read or
 * sent, no descriptor is opened and no provider is started. This check does not
 * replace authenticated endpoint preparation. The trusted provisioner must keep
 * registered endpoints stable until their private duplicates have been taken.
 * Recoverable fast_io allocation is confined to static freeze, never dispatch.
 */
#pragma once
#include "driver_channel.h"
#include <uwvm2/utils/container/standard.h>
namespace uwvm2::imported::ros::scene
{
    class attachment_registry
    {
        using error=resources::error;
        ::uwvm2::utils::container::std_set<std::pair<std::uint64_t,std::uint64_t>> endpoints_;
        ::uwvm2::utils::container::std_set<drivers::protocol::identity> sessions_;
        bool declarations_only_{};
    public:
        explicit attachment_registry(bool declarations_only=false):declarations_only_{declarations_only}{}
        error endpoint(std::pair<std::uint64_t,std::uint64_t> key)noexcept
        {
            if(!key.second)return error::bad_handle;
            try{return endpoints_.insert(key).second?error::success:error::conflict;}
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
        }
        error descriptor(int fd)noexcept
        {
            // Offline declarations can detect reused numbers. Only a live
            // launch can prove that distinct numbers refer to distinct objects.
            if(declarations_only_)return fd<0?error::bad_handle:endpoint({0,static_cast<std::uint64_t>(fd)+1});
#if defined(__linux__)
            struct stat info{};if(fd<0||::fstat(fd,&info))return error::bad_handle;
            if(!S_ISSOCK(info.st_mode)&&!S_ISFIFO(info.st_mode))return error::denied;
            return endpoint({static_cast<std::uint64_t>(info.st_dev),static_cast<std::uint64_t>(info.st_ino)});
#else
            (void)fd;return error::unsupported;
#endif
        }
        error session(drivers::protocol::identity value)noexcept
        {
            if(!drivers::protocol::present(value))return error::invalid;
            try{return sessions_.insert(value).second?error::success:error::conflict;}
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
        }
    };
}
