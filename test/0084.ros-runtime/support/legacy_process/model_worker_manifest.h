// Test-only historical process owner. Never include or link into the VM product.
/** @file
 * @brief Trusted, inert model-worker configuration and exact-file preparation.
 * @details These settings are deployment inputs, never guest imports. Parsing
 * records absolute host paths, semantic named limits, exact sizes and SHA-256
 * digests without opening anything. freeze() requires every limit, one launcher
 * and a complete bootstrap/backend image description. It copies the model/task
 * profiles from the already validated model plan and rejects later changes.
 *
 * prepare() first pins every path with O_PATH|O_NOFOLLOW, verifies a regular
 * file, then reopens that exact descriptor for reading. Thus devices/FIFOs are
 * not activated merely to classify them. O_NOFOLLOW only constrains the final
 * path component; parent directories remain trusted deployment configuration.
 * Immutable sealed copies and exact hashes in model_sandbox establish the
 * actual bytes used, independently of later path replacement. Every failure
 * closes the temporary descriptors and publishes no provider owner.
 */
#pragma once
#include "model_worker.h"
#include <uwvm2/imported/ros/scene/proposal_envelope.h>

namespace uwvm2::imported::ros::scene::model_worker
{
    /** @brief Closed semantic limit names used by the trusted CLI. */
    struct limit_field {std::string_view name;std::uint64_t model_sandbox::limits::*member;};
    inline constexpr std::array limit_fields{
        limit_field{"image-bytes",&model_sandbox::limits::image_bytes},
        limit_field{"address-bytes",&model_sandbox::limits::address_bytes},
        limit_field{"cpu-seconds",&model_sandbox::limits::cpu_seconds},
        limit_field{"file-bytes",&model_sandbox::limits::file_bytes},
        limit_field{"open-files",&model_sandbox::limits::open_files},
        limit_field{"scratch-bytes",&model_sandbox::limits::scratch_bytes},
        limit_field{"scratch-slots",&model_sandbox::limits::scratch_slots},
        limit_field{"request-bytes",&model_sandbox::limits::request_bytes},
        limit_field{"response-bytes",&model_sandbox::limits::response_bytes},
        limit_field{"stderr-bytes",&model_sandbox::limits::stderr_bytes},
        limit_field{"startup-ns",&model_sandbox::limits::startup_ns},
        limit_field{"poll-ns",&model_sandbox::limits::poll_ns},
        limit_field{"supervisor-bytes",&model_sandbox::limits::supervisor_bytes}};

    class configuration
    {
        struct image_setting
        {
            buffers::text<4095> path;
            buffers::text<255> destination;
            std::uint64_t bytes{};
            buffers::digest digest{};
            model_sandbox::image_role role{};
        };
        model_sandbox::limits bounds_{};
        execution_placement::policy placement_;
        std::uint64_t assigned_{};
        image_setting launcher_{};
        ::uwvm2::utils::container::std_vector<image_setting> images_;
        buffers::profile model_,task_;
        bool launcher_set_{},placement_set_{},frozen_{};
        static bool path(std::string_view value) noexcept
        {return value.size()>1&&value.size()<=4095&&value.front()=='/'&&value.back()!='/'&&value.find('\0')==value.npos;}
        static model_sandbox::image_spec specification(image_setting const& value,int fd) noexcept
        {return {fd,value.bytes,value.digest,value.destination,value.role};}
        error describe(model_sandbox::manifest& output) const
        {
            // Synthetic nonnegative descriptors are used only for inert syntax
            // validation; no syscall or sealing operation sees this manifest.
            output.bound=bounds_;output.model=model_;output.task=task_;output.placement=placement_;
            output.launcher=specification(launcher_,0);output.files.reserve(images_.size());
            for(auto const& file:images_)output.files.push_back(specification(file,0));
            return error::success;
        }
#if defined(__linux__)
        static error open_regular(image_setting const& setting,model_sandbox::descriptor& output) noexcept
        {
            if(output)return error::conflict;
            // Path text was bounded and copied; append a NUL in fixed storage.
            std::array<char,4096> text{};std::copy_n(setting.path.bytes.begin(),setting.path.size,text.begin());
            model_sandbox::descriptor pinned{::open(text.data(),O_PATH|O_NOFOLLOW|O_CLOEXEC)};
            if(!pinned)return error::io;
            struct stat before{};
            if(::fstat(pinned.get(),&before)||!S_ISREG(before.st_mode)||before.st_size<0||static_cast<std::uint64_t>(before.st_size)!=setting.bytes)return error::invalid;
            std::array<char,64> descriptor_path{};constexpr std::string_view prefix="/proc/self/fd/";
            std::copy(prefix.begin(),prefix.end(),descriptor_path.begin());
            auto rendered=std::to_chars(descriptor_path.data()+prefix.size(),descriptor_path.data()+descriptor_path.size()-1,pinned.get());
            if(rendered.ec!=std::errc{})return error::invalid;
            model_sandbox::descriptor readable{::open(descriptor_path.data(),O_RDONLY|O_CLOEXEC|O_NONBLOCK)};
            if(!readable)return error::io;
            struct stat after{};
            if(::fstat(readable.get(),&after)||!S_ISREG(after.st_mode)||before.st_dev!=after.st_dev||before.st_ino!=after.st_ino||
               after.st_size!=before.st_size)return error::conflict;
            output=std::move(readable);return error::success;
        }
#endif
    public:
        /** @brief Select CPU/NUMA launch placement once, without probing the host.
         * @details Missing placement inherits the preparing thread's current CPU
         * and memory policy. Explicit IDs are checked against actual allowed sets
         * during provider preparation; an unavailable guarantee fails startup.
         * Neither option grants model authority or requests exclusive/real-time CPUs.
         */
        error placement(std::string_view cpus,std::string_view memory,std::string_view nodes)noexcept
        {
            if(frozen_)return error::denied;
            if(placement_set_)return error::conflict;
            execution_placement::policy value;auto status=execution_placement::parse(cpus,memory,nodes,value);
            if(status!=error::success)return status;
            placement_=std::move(value);placement_set_=true;return error::success;
        }
        /** @brief Set exactly one named limit; explicit zero is retained, duplicates fail. */
        error limit(std::string_view name,std::uint64_t value) noexcept
        {
            if(frozen_)return error::denied;
            for(std::size_t i{};i<limit_fields.size();++i)if(limit_fields[i].name==name)
            {
                auto bit=std::uint64_t{1}<<i;if(assigned_&bit)return error::conflict;
                bounds_.*limit_fields[i].member=value;assigned_|=bit;return error::success;
            }
            return error::invalid;
        }
        /** @brief Select one exact launcher image without performing I/O. */
        error launcher(std::string_view host_path,std::uint64_t bytes,buffers::digest digest) noexcept
        {
            if(frozen_)return error::denied;
            if(launcher_set_)return error::conflict;
            if(!path(host_path)||!bytes||bytes>INT64_MAX)return error::invalid;
            image_setting value;value.bytes=bytes;value.digest=digest;value.role=model_sandbox::image_role::runtime;
            if(!value.path.assign(host_path))return error::invalid;
            launcher_=value;launcher_set_=true;return error::success;
        }
        /** @brief Add an exact logical image role/destination, rejecting duplicate destinations. */
        error image(model_sandbox::image_role role,std::string_view destination,std::string_view host_path,std::uint64_t bytes,buffers::digest digest) noexcept
        {
            if(frozen_)return error::denied;
            if(role<model_sandbox::image_role::bootstrap||role>model_sandbox::image_role::weights||!path(host_path)||bytes>INT64_MAX||
               !model_sandbox::destination(destination)||images_.size()>=64)return error::invalid;
            for(auto const& file:images_)if(file.destination.view()==destination)return error::conflict;
            image_setting value;value.role=role;value.bytes=bytes;value.digest=digest;
            if(!value.path.assign(host_path)||!value.destination.assign(destination))return error::invalid;
            try{images_.push_back(value);return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
        }
        /** @brief Check complete finite configuration against the exact selected model plan.
         * @details No source path is opened here. On refusal configuration
         * remains editable so a trusted caller can supply missing fields.
         */
        error freeze(models::plan const& selected) noexcept
        {
            if(frozen_)return error::conflict;
            if(!models::valid(selected)||!launcher_set_||assigned_!=((std::uint64_t{1}<<limit_fields.size())-1))return error::invalid;
            try
            {
                model_sandbox::manifest manifest;describe(manifest);manifest.model=selected.model;manifest.task=selected.task;
                if(!model_sandbox::valid(manifest))return error::invalid;
                model_=selected.model;task_=selected.task;frozen_=true;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
        }
        /** @brief Prepared limits contain no paths, contents or live descriptors. */
        /** @brief Freeze an isolated component image closure with no data descriptors.
         * @details This shares only the fixed-image launcher and resource checks
         * with inference. It does not select a model backend or grant model
         * access. Component configuration is sent later through its own protocol.
         */
        error freeze_component(buffers::profile selected,buffers::profile stop)noexcept
        {
            if(frozen_)return error::conflict;
            if(!proposals::envelope_profile(selected)||!proposals::envelope_profile(stop)||!launcher_set_||
                assigned_!=((std::uint64_t{1}<<limit_fields.size())-1))return error::invalid;
            for(auto const& image:images_)if(image.role==model_sandbox::image_role::weights)return error::denied;
            try
            {
                model_sandbox::manifest manifest;describe(manifest);manifest.model=selected;manifest.task=stop;
                if(!model_sandbox::valid(manifest))return error::invalid;
                model_=selected;task_=stop;frozen_=true;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
        }
#if defined(__linux__)
        /** @brief Seal an inert approved component process; this method never launches it. */
        error prepare_component(model_sandbox::process& process)const noexcept
        {
            if(!frozen_)return error::denied;
            for(auto const& image:images_)if(image.role==model_sandbox::image_role::weights)return error::denied;
            try
            {
                std::array<model_sandbox::descriptor,65> opened;
                model_sandbox::manifest manifest;manifest.bound=bounds_;manifest.model=model_;manifest.task=task_;manifest.placement=placement_;
                auto status=open_regular(launcher_,opened[0]);if(status!=error::success)return status;
                manifest.launcher=specification(launcher_,opened[0].get());manifest.files.reserve(images_.size());
                for(std::size_t i{};i<images_.size();++i)
                {status=open_regular(images_[i],opened[i+1]);if(status!=error::success)return status;manifest.files.push_back(specification(images_[i],opened[i+1].get()));}
                return process.prepare(manifest);
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
#endif
        model_sandbox::limits limits() const noexcept{return bounds_;}
        bool frozen() const noexcept{return frozen_;}
        /** @brief Open, hash/seal, isolate and attach one exact worker transactionally.
         * @details The supplied plan is checked again by service::prepare via
         * the dispatcher. No backend owner is published on partial startup.
         */
        error prepare(model_dispatch& dispatcher,std::string_view alias,std::uint64_t generation,std::shared_ptr<models::provider_lifetime>& output) const noexcept
        {
            if(!frozen_||output)return error::invalid;
#if defined(__linux__)
            try
            {
                std::array<model_sandbox::descriptor,65> opened;
                model_sandbox::manifest manifest;manifest.bound=bounds_;manifest.model=model_;manifest.task=task_;manifest.placement=placement_;
                auto status=open_regular(launcher_,opened[0]);if(status!=error::success)return status;
                manifest.launcher=specification(launcher_,opened[0].get());manifest.files.reserve(images_.size());
                for(std::size_t i{};i<images_.size();++i)
                {
                    status=open_regular(images_[i],opened[i+1]);if(status!=error::success)return status;
                    manifest.files.push_back(specification(images_[i],opened[i+1].get()));
                }
                return service::prepare(dispatcher,alias,generation,manifest,output);
            }
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
            catch(...){return error::io;}
#else
            (void)dispatcher;(void)alias;(void)generation;return error::unsupported;
#endif
        }
    };
}
