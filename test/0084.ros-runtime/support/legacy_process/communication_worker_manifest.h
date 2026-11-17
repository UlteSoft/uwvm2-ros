// Test-only historical process owner. Never include or link into the VM product.
/** @file
 * @brief Inert deployment settings and pinned images for isolated ROS workers.
 * @details Trusted CLI configuration supplies semantic limits, exact image
 * sizes/digests, CPU/NUMA placement and one startup-profile identity. Parsing
 * never opens a source, starts a process or issues a communication capability.
 * freeze() validates the complete finite description. prepare() pins regular
 * files without activating devices/FIFOs; the worker factory subsequently seals
 * and verifies their exact bytes before any admitted node startup.
 *
 * Host paths are deployment inputs, never guest resources. Final symlinks are
 * refused; ancestor directories remain trusted deployment configuration. The
 * pinned inode and later sealed digest are authoritative if a path is replaced.
 * All owning containers use recoverable fast_io allocation. Absent limits do
 * not become unlimited, and no fallback weakens an unavailable isolation policy.
 */
#pragma once
#include "communication_worker_sandbox.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
    struct image_limit_field{std::string_view name;std::uint64_t communication_sandbox::limits::*member;};
    inline constexpr std::array image_limit_fields{
        image_limit_field{"image-bytes",&communication_sandbox::limits::image_bytes},
        image_limit_field{"address-bytes",&communication_sandbox::limits::address_bytes},
        image_limit_field{"cpu-seconds",&communication_sandbox::limits::cpu_seconds},
        image_limit_field{"file-bytes",&communication_sandbox::limits::file_bytes},
        image_limit_field{"open-files",&communication_sandbox::limits::open_files},
        image_limit_field{"scratch-bytes",&communication_sandbox::limits::scratch_bytes},
        image_limit_field{"scratch-slots",&communication_sandbox::limits::scratch_slots},
        image_limit_field{"request-bytes",&communication_sandbox::limits::request_bytes},
        image_limit_field{"response-bytes",&communication_sandbox::limits::response_bytes},
        image_limit_field{"stderr-bytes",&communication_sandbox::limits::stderr_bytes},
        image_limit_field{"startup-ns",&communication_sandbox::limits::startup_ns},
        image_limit_field{"poll-ns",&communication_sandbox::limits::poll_ns},
        image_limit_field{"supervisor-bytes",&communication_sandbox::limits::supervisor_bytes},
        image_limit_field{"profile-bytes",&communication_sandbox::limits::profile_bytes},
        image_limit_field{"threads",&communication_sandbox::limits::thread_count}};
    class image_configuration
    {
        using error=communication_sandbox::error;
        struct setting
        {
            buffers::text<4095> path;
            buffers::text<255> destination;
            std::uint64_t bytes{};
            buffers::digest digest{};
            communication_sandbox::image_role role{};
        };
        communication_sandbox::limits bounds;
        execution_placement::policy placement_policy;
        buffers::profile startup;
        buffers::text<255> prefix;
        setting launch;
        ::uwvm2::utils::container::std_vector<setting> images;
        std::uint64_t assigned{};
        bool launcher_set{},profile_set{},placement_set{},frozen_{};
        static bool path(std::string_view value)noexcept
        {return value.size()>1&&value.size()<=4095&&value.front()=='/'&&value.back()!='/'&&value.find('\0')==value.npos;}
        static communication_sandbox::image_spec specification(setting const& value,int descriptor)noexcept
        {return {descriptor,value.bytes,value.digest,value.destination,value.role};}
        void describe(communication_sandbox::manifest& output)const
        {
            output.bound=bounds;output.placement=placement_policy;output.configuration=startup;output.ros_prefix=prefix;
            // Placeholder descriptors are only used by syntax validation.
            output.launcher=specification(launch,0);output.files.reserve(images.size());
            for(auto const& image:images)output.files.push_back(specification(image,0));
        }
#if defined(__linux__)
        static error open_regular(setting const& value,communication_sandbox::descriptor& output)noexcept
        {
            if(output)return error::conflict;
            std::array<char,4096> name{};std::copy_n(value.path.bytes.begin(),value.path.size,name.begin());
            communication_sandbox::descriptor pinned{::open(name.data(),O_PATH|O_NOFOLLOW|O_CLOEXEC)};
            if(!pinned)return error::io;
            struct stat before{};
            if(::fstat(pinned.get(),&before)||!S_ISREG(before.st_mode)||before.st_size<0||static_cast<std::uint64_t>(before.st_size)!=value.bytes)return error::invalid;
            std::array<char,64> route{};constexpr std::string_view head="/proc/self/fd/";std::copy(head.begin(),head.end(),route.begin());
            auto rendered=std::to_chars(route.data()+head.size(),route.data()+route.size()-1,pinned.get());if(rendered.ec!=std::errc{})return error::invalid;
            communication_sandbox::descriptor readable{::open(route.data(),O_RDONLY|O_NONBLOCK|O_CLOEXEC)};if(!readable)return error::io;
            struct stat after{};
            if(::fstat(readable.get(),&after)||!S_ISREG(after.st_mode)||before.st_dev!=after.st_dev||before.st_ino!=after.st_ino||before.st_size!=after.st_size)return error::conflict;
            output=std::move(readable);return error::success;
        }
#endif
    public:
        /** @brief Set an exact startup definition and logical ROS installation prefix.
         * @details The complete startup image digest is verified by the worker
         * factory. This profile grants no node, peer, socket or schema authority.
         */
        error profile(buffers::profile selected,std::string_view ros_prefix)noexcept
        {
            if(frozen_)return error::denied;
            if(profile_set)return error::conflict;
            if(!selected.identity.size||selected.identity.size>selected.identity.bytes.size()||
                selected.identity.view().find('\0')!=std::string_view::npos||
                !wire::utf8({reinterpret_cast<std::uint8_t const*>(selected.identity.bytes.data()),selected.identity.size})||
                !ros_prefix.starts_with("/opt/ros/")||ros_prefix.size()<=9||ros_prefix.size()>255||ros_prefix.back()=='/'||
                ros_prefix.find('\0')!=std::string_view::npos)return error::invalid;
#if defined(__linux__)
            if(!communication_sandbox::destination(ros_prefix))return error::invalid;
#endif
            buffers::text<255> destination;if(!destination.assign(ros_prefix))return error::invalid;
            startup=selected;prefix=destination;profile_set=true;return error::success;
        }
        /// @brief Assign a finite named limit once; explicit zero remains zero.
        error limit(std::string_view name,std::uint64_t number)noexcept
        {
            if(frozen_)return error::denied;
            if(number==UINT64_MAX)return error::invalid;
            for(std::size_t i{};i<image_limit_fields.size();++i)if(image_limit_fields[i].name==name)
            {auto bit=std::uint64_t{1}<<i;if(assigned&bit)return error::conflict;bounds.*image_limit_fields[i].member=number;assigned|=bit;return error::success;}
            return error::invalid;
        }
        /** @brief Store explicit or inherited placement without probing system state.
         * @details Actual allowed CPU and memory-node sets are checked during
         * process preparation. No exclusive or real-time scheduling is implied.
         */
        error placement(std::string_view cpus,std::string_view memory,std::string_view nodes)noexcept
        {
            if(frozen_)return error::denied;
            if(placement_set)return error::conflict;
            execution_placement::policy selected;auto status=execution_placement::parse(cpus,memory,nodes,selected);
            if(status!=error::success)return status;
            placement_policy=std::move(selected);placement_set=true;return error::success;
        }
        /// @brief Record the exact trusted launcher image without opening its path.
        error launcher(std::string_view host_path,std::uint64_t bytes,buffers::digest digest)noexcept
        {
            if(frozen_)return error::denied;
            if(launcher_set)return error::conflict;
            if(!path(host_path)||!bytes||bytes>INT64_MAX)return error::invalid;
            setting value;value.bytes=bytes;value.digest=digest;value.role=communication_sandbox::image_role::runtime;
            if(!value.path.assign(host_path))return error::invalid;
            launch=value;launcher_set=true;return error::success;
        }
        /// @brief Record one bounded logical image with no in-process library load.
        error image(communication_sandbox::image_role role,std::string_view destination,std::string_view host_path,std::uint64_t bytes,buffers::digest digest)noexcept
        {
            if(frozen_)return error::denied;
            if(role<communication_sandbox::image_role::bootstrap||role>communication_sandbox::image_role::configuration||
                !path(host_path)||bytes>INT64_MAX||destination.empty()||destination.size()>255||images.size()>=256)return error::invalid;
#if defined(__linux__)
            if(!communication_sandbox::destination(destination))return error::invalid;
#else
            return error::unsupported;
#endif
            for(auto const& current:images)if(current.destination.view()==destination)return error::conflict;
            setting value;value.role=role;value.bytes=bytes;value.digest=digest;
            if(!value.path.assign(host_path)||!value.destination.assign(destination))return error::invalid;
            try{images.push_back(value);return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
        }
        /** @brief Validate all capacities and image roles without file access or launch.
         * @details A failed validation leaves the configuration editable. An
         * unavailable platform isolation backend refuses this configuration.
         */
        error freeze()noexcept
        {
            if(frozen_)return error::conflict;
            if(!launcher_set||!profile_set||assigned!=((std::uint64_t{1}<<image_limit_fields.size())-1))return error::invalid;
#if defined(__linux__)
            try{communication_sandbox::manifest value;describe(value);if(!communication_sandbox::valid(value))return error::invalid;frozen_=true;return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
#else
            return error::unsupported;
#endif
        }
        bool frozen()const noexcept{return frozen_;}
        communication_sandbox::limits limits()const noexcept{return bounds;}
#if defined(__linux__)
        /** @brief Descriptors outlive the manifest consumed by factory configuration. */
        struct prepared
        {
            ::uwvm2::utils::container::std_vector<communication_sandbox::descriptor> descriptors;
            communication_sandbox::manifest manifest;
        };
        /** @brief Pin only regular exact-size images, publishing all or none.
         * @details No worker is launched and no dynamic library is loaded here.
         * The caller next passes manifest to paired_factory::configure, which
         * verifies sealed image digests and resource placement before node use.
         */
        error prepare(prepared& output)const noexcept
        {
            if(!frozen_)return error::denied;
            if(!output.descriptors.empty()||!output.manifest.files.empty())return error::conflict;
            try
            {
                prepared next;next.descriptors.resize(images.size()+1);describe(next.manifest);
                auto status=open_regular(launch,next.descriptors[0]);if(status!=error::success)return status;
                next.manifest.launcher.source=next.descriptors[0].get();
                for(std::size_t i{};i<images.size();++i)
                {status=open_regular(images[i],next.descriptors[i+1]);if(status!=error::success)return status;next.manifest.files[i].source=next.descriptors[i+1].get();}
                output=std::move(next);return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
#endif
    };
}
