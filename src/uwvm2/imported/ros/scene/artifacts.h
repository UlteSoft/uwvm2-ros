/** @file
 * @brief Authority-independent lifetimes for deployment-pinned immutable inputs.
 * @details The scene broker holds an exact authority lease across every method.
 * Handles share the local scheduling identity allocator, preventing cross-kind
 * confusion in resource.close. Inputs stay charged and retained after all guest
 * handles close. Inspection reads cached metadata only; it performs no I/O.
 */
#pragma once
#include <uwvm2/utils/container/standard.h>
#include "storage.h"
#include "artifact_contract.h"

namespace uwvm2::imported::ros::scene::artifacts
{
    using digest=std::array<std::uint8_t,32>;
    inline constexpr std::uint64_t maximum_transfer=4u*1024u*1024u;
    /** @brief Inert trusted launch plan; never accepts guest-selected paths.
     * @details Exact byte length and digest authenticate one generation, not a
     * decoder/schema. Native provisioning happens before guest activation.
     */
    struct input_plan
    {
        ::uwvm2::utils::container::std_string path;
        std::array<std::uint8_t,32> digest{};
        std::uint64_t generation{},bytes{},transfer{};
    };
    /** @brief Portable external purpose codes independent of internal enum order. */
    inline std::uint64_t purpose_code(purpose use) noexcept
    {
        switch(use)
        {
        case purpose::package:return 1;
        case purpose::configuration:return 2;
        case purpose::calibration:return 3;
        case purpose::map:return 4;
        case purpose::model_weights:return 5;
        default:return 0;
        }
    }
    inline bool input_action(std::string_view name) noexcept
    {return name=="artifact.open"||name=="artifact.read"||name=="artifact.describe"||name=="resource.close";}
    inline bool valid(input_plan const& plan) noexcept
    {return !plan.path.empty()&&plan.path.size()<=4095&&plan.path.front()=='/'&&plan.path.find('\0')==plan.path.npos&&
        plan.generation&&plan.bytes<=static_cast<std::uint64_t>(INT64_MAX)&&plan.transfer<=maximum_transfer;}
    /** @brief Parse an exact lowercase deployment SHA256 without accepting prefixes. */
    inline bool parse_digest(std::string_view text,digest& output) noexcept
    {
        if(text.size()!=64)return false;
        digest parsed{};
        for(std::size_t i{};i<32;++i)for(unsigned j{};j<2;++j)
        {auto c=text[i*2+j];unsigned value;if(c>='0'&&c<='9')value=c-'0';else if(c>='a'&&c<='f')value=c-'a'+10;else return false;
            parsed[i]=static_cast<std::uint8_t>(parsed[i]*16+value);}
        output=parsed;return true;
    }
    /** @brief Detached immutable content identity; no source path or native handle. */
    struct information
    {std::uint64_t generation{},size{},purpose_code{},transfer{};digest sha256{};};
    struct snapshot
    {handle id{};::uwvm2::utils::container::std_string alias,resource,binding;std::uint64_t size_bytes{};bool retired{};};

    class registry
    {
        struct binding
        {
            storage::principal owner;
            ::uwvm2::utils::container::std_string resource,name;
            information metadata;
#if defined(__linux__)
            std::shared_ptr<storage::immutable_input> input;
#endif
            std::uint64_t opened{};bool retired{};
        };
        mutable std::mutex mutex;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,binding,std::less<>> bindings;
        ::uwvm2::utils::container::std_map<handle,binding*> objects;
        bool retired{};
        binding* find(std::string_view alias,handle id) noexcept
        {
            auto it=objects.find(id);
            if(retired||it==objects.end()||it->second->retired||it->second->owner.grant!=alias)return nullptr;
            return it->second;
        }
    public:
#if defined(__linux__)
        /** @brief Install one verified sealed input before publishing the environment.
         * @details This is trusted launch glue, not guest API. The provisioner
         * has already verified the digest of the sealed snapshot. Principal,
         * purpose and exact length are checked again; no path is retained here.
         */
        error bind(storage::principal who,purpose use,::uwvm2::utils::container::std_string resource,::uwvm2::utils::container::std_string provider,
                   input_plan const& plan,std::shared_ptr<storage::immutable_input> input)
        {
            if(!storage::valid(who)||!valid(plan)||!purpose_code(use)||!input)return error::invalid;
            std::uint64_t size{};purpose actual{};
            if(input->describe(who,size,actual)!=storage::success||size!=plan.bytes||actual!=use)return error::denied;
            std::lock_guard lock{mutex};if(retired)return error::denied;
            auto alias=who.grant;
            binding value{std::move(who),std::move(resource),std::move(provider),
                {plan.generation,plan.bytes,purpose_code(use),plan.transfer,plan.digest},std::move(input)};
            return bindings.emplace(std::move(alias),std::move(value)).second?error::success:error::conflict;
        }
#endif
        /** @brief Open only the exact approved content identity with bounded handles. */
        error open(std::string_view alias,std::uint64_t generation,digest const& expected,std::uint64_t limit,handle& output)
        {
            std::lock_guard lock{mutex};auto it=bindings.find(alias);
            if(retired||it==bindings.end()||it->second.retired)return error::denied;
            auto& value=it->second;
            if(value.metadata.generation!=generation||value.metadata.sha256!=expected)return error::denied;
            if(value.opened>=limit)return error::again;
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            objects.emplace(id,&value);++value.opened;output=id;return error::success;
        }
        /** @brief Copy cached approved metadata after separate describe authority. */
        error describe(std::string_view alias,handle id,information& output)
        {std::lock_guard lock{mutex};auto value=find(alias,id);if(!value)return error::bad_handle;output=value->metadata;return error::success;}
        /** @brief Read a bounded immutable range with no shared cursor or consumption.
         * @details Caller admits read independently. The actual requested span,
         * not unused guest output capacity, is checked against transfer bounds.
         * Errors preserve output data; the broker stages bytes before delivery.
         */
        error read(std::string_view alias,handle id,std::uint64_t offset,std::span<std::uint8_t> output,std::size_t& count)
        {
            count=0;std::lock_guard lock{mutex};auto value=find(alias,id);if(!value)return error::bad_handle;
            if(offset>value->metadata.size||output.size()>value->metadata.transfer||output.size()>UINT64_MAX-offset)return error::too_large;
#if defined(__linux__)
            auto status=value->input->read(value->owner,offset,{reinterpret_cast<std::byte*>(output.data()),output.size()},count);
            if(status==storage::success)return error::success;
            if(status==storage::denied)return error::denied;
            if(status==storage::too_large)return error::too_large;
            return error::io;
#else
            return error::unsupported;
#endif
        }
        /** @brief Release an exact resource handle without refunding retained inputs. */
        error close(std::string_view alias,handle id)
        {
            std::lock_guard lock{mutex};auto it=objects.find(id);
            if(retired||it==objects.end()||it->second->owner.grant!=alias||it->second->retired)return error::bad_handle;
            --it->second->opened;objects.erase(it);return error::success;
        }
        /** @brief Roll back only an unpublished open, including concurrent retirement. */
        void abandon(std::string_view alias,handle id)
        {std::lock_guard lock{mutex};auto it=objects.find(id);if(it!=objects.end()&&it->second->owner.grant==alias)
            {--it->second->opened;objects.erase(it);}}
        /** @brief Retire a grant's inputs; no new reads or opens can follow. */
        void revoke(std::string_view alias)
        {std::lock_guard lock{mutex};auto it=bindings.find(alias);if(it!=bindings.end())it->second.retired=true;}
        void retire(){std::lock_guard lock{mutex};retired=true;}
        /** @brief Copy bounded stored metadata without calling an input provider. */
        bool inspect(::uwvm2::utils::container::std_vector<snapshot>& output,std::size_t maximum) const
        {
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock()||objects.size()>maximum)return false;
            ::uwvm2::utils::container::std_vector<snapshot> rows;rows.reserve(objects.size());
            for(auto const& [id,value]:objects)
                rows.push_back({id,value->owner.grant,value->resource,value->name,value->metadata.size,retired||value->retired});
            output=std::move(rows);return true;
        }
    };
}
