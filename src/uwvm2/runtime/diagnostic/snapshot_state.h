// Licensed under APL-2.0 (see LICENSE).
#pragma once
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#ifndef UWVM_MODULE
# include "snapshot_archive.h"
# include "snapshot_wasip1.h"
# include "snapshot_ros.h"
# include <uwvm2/uwvm/debugger/checkpoint_codec.h>
# include <uwvm2/uwvm/debugger/checkpoint_execution_replay.h>
# include <uwvm2/runtime/checkpoint/materialization.h>
# include <algorithm>
# include <memory>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::diagnostic::snapshot_state
{
    namespace archive = ::uwvm2::runtime::diagnostic::snapshot_archive;
    namespace cp = ::uwvm2::uwvm::debugger::checkpoint;
    namespace materialization = ::uwvm2::runtime::checkpoint;
    enum class error : unsigned char
    { none, disclosure_denied, missing_state, incomplete_state, incompatible_schema,
      invalid_state, inconsistent_manifest, allocation_failed, unsupported_component };
    struct result
    {
        error status{error::missing_state};
        cp::error state_error{cp::error::none};
        explicit operator bool() const noexcept { return status == error::none; }
        static constexpr bool grants_restore_authority() noexcept { return false; }
    };
    namespace detail
    {
        struct callback_sink
        {
            using char_type = char;
            using output_char_type = char;
            void* context{};
            archive::writer write{};
            bool* failed{};
        };
        inline callback_sink io_stream_ref_define(callback_sink sink) noexcept { return sink; }
        inline void write_all_overflow_define(callback_sink sink, char const* first, char const* last) noexcept
        {
            if(*sink.failed || first == last) { return; }
            // fast_io supplies one synchronous, owned span from the canonical
            // state encoder. No file offset or guest address enters this API.
            *sink.failed = !sink.write(sink.context, {reinterpret_cast<::std::byte const*>(first),
                static_cast<::std::size_t>(last-first)});
        }
    }
    // The collector retains the immutable state and this descriptor through
    // encode_stream(). Canonical graph bytes go directly into the outer hash
    // and output; there is no second memory-sized serialization buffer.
    class state_stream
    {
        ::std::shared_ptr<cp::state const> state_{};
        cp::limits limits_{};
        ::std::uint64_t bytes_{};
        static bool emit(void* opaque, void* output, archive::writer write) noexcept
        {
            auto const& self = *static_cast<state_stream const*>(opaque);
            if(!self.state_ || !write) { return false; }
            bool failed{};
            detail::callback_sink sink{output, write, &failed};
            try { return cp::encode_database(sink, *self.state_, self.limits_) == cp::error::none && !failed; }
            catch(...) { return false; }
        }
    public:
        [[nodiscard]] result bind(::std::shared_ptr<cp::state const> const& state, cp::limits const& cap = {})
        {
            if(!state) { return {}; }
            auto const valid = cp::validate_graph(*state, cap);
            if(valid != cp::error::none) { return {error::invalid_state, valid}; }
            ::std::uint64_t body{};
            if(!cp::codec_details::measure(*state, cap, body))
                { return {error::invalid_state, cp::error::limit_exceeded}; }
            state_ = state; limits_ = cap; bytes_ = body+cp::header_bytes+cp::footer_bytes;
            return {error::none};
        }
        [[nodiscard]] archive::stream_record record() noexcept
        {
            return {archive::component::wasm_state, archive::completeness::complete,
                state_ ? state_->checkpoint_id : 0u, cp::database_format_version, bytes_, this, emit};
        }
        void describe(archive::platform& platform) const noexcept
        {
            if(!state_) { return; }
            platform.logical_abi = materialization::state_schema_revision;
            platform.continuation_abi = materialization::native_resume_abi_revision;
            platform.features = state_->required_features;
        }
    };
    namespace detail
    {
    // Same declaration/environment binding check for executable snapshots and
    // detached historical observations. It neither inspects nor authorizes a
    // native continuation. Avoid copying/serializing whole linear memories just
    // to validate an offline observation's origin.
    [[nodiscard]] inline result validate_manifest(archive::decoded const& archive,cp::state const& graph)
    {
        unsigned present_ros{};
        for(auto const& row:archive.records)
        {
            if(row.kind!=archive::component::ros1 && row.kind!=archive::component::ros2)continue;
            auto const bit=row.kind==archive::component::ros1?1u:2u;
            if(present_ros&bit)return {error::unsupported_component};
            present_ros|=bit;
            if(row.capture==archive::completeness::unavailable &&
               (row.id || row.schema!=1u || !row.payload.empty()))return {error::unsupported_component};
        }
    unsigned required_ros{};
    for(auto const& object:graph.objects)
        if(object.kind==cp::object_kind::host_resource && object.flags==3u && object.words[0u]==cp::builtin_ros_v1_binding_kind)
            required_ros|=object.words[1u]<=2u?1u:2u;
    if(required_ros!=present_ros) return {error::inconsistent_manifest};
    // Original modules are also embedded in the graph. If an archive
    // provides extraction copies, require the complete same ordered set
    // and byte identity. Contradictory code views cannot be restored.
    ::std::size_t module_count{}, copies{}, next_object{};
    for(auto const& object : graph.objects) { module_count += object.kind == cp::object_kind::module; }
    for(auto const& record : archive.records)
    {
        if(record.kind != archive::component::wasm_module) { continue; }
        while(next_object < graph.objects.size() && graph.objects[next_object].kind != cp::object_kind::module) { ++next_object; }
        if(record.capture != archive::completeness::complete || record.schema != 1u ||
           record.id != copies || next_object == graph.objects.size()) { return {error::inconsistent_manifest}; }
        auto const& bytes = graph.objects[next_object++].bytes;
        if(record.payload.size() != bytes.size() || !::std::equal(bytes.begin(),bytes.end(),record.payload.begin()))
            { return {error::inconsistent_manifest}; }
        ++copies;
    }
    if(copies != 0u && copies != module_count) { return {error::inconsistent_manifest}; }
    ::uwvm2::utils::container::std_vector<cp::object const*> modules{};modules.reserve(module_count);
    for(auto const& object:graph.objects)
        if(object.kind==cp::object_kind::module)modules.push_back(::std::addressof(object));
    for(auto const& record:archive.records)
    {
        if((record.kind!=archive::component::ros1 && record.kind!=archive::component::ros2) ||
            record.capture==archive::completeness::unavailable)continue;
        ::uwvm2::runtime::diagnostic::snapshot_ros::environment env;
        if(!::uwvm2::runtime::diagnostic::snapshot_ros::decode(archive.disclosure,record,env) ||
           env.recording!=graph.recording_id)return {error::inconsistent_manifest};
        ::uwvm2::utils::container::std_vector<bool> present(module_count);
        for(auto const& group:env.groups)for(auto const& member:group.members)
        {
            if(member.module>=module_count || present[static_cast<::std::size_t>(member.module)])return {error::inconsistent_manifest};
            present[static_cast<::std::size_t>(member.module)]=true;
        }
        if(::std::find(present.begin(),present.end(),false)!=present.end())return {error::inconsistent_manifest};
    }
    ::uwvm2::utils::container::std_vector<bool> assigned(module_count);
    for(auto const& record:archive.records)
    {
        if(record.kind!=archive::component::wasip1 || record.capture==archive::completeness::unavailable)continue;
        ::uwvm2::runtime::diagnostic::snapshot_wasip1::environment environment{};
        if(!::uwvm2::runtime::diagnostic::snapshot_wasip1::decode(archive.disclosure,record,environment) ||
           environment.recording!=graph.recording_id || environment.module>=modules.size())
            return {error::inconsistent_manifest};
        auto const& original=modules[static_cast<::std::size_t>(environment.module)]->bytes;
        ::fast_io::sha256_context hash{};hash.update(original.data(),original.data()+original.size());hash.do_final();
        ::std::array<::std::byte,32u> digest{};hash.digest_to_byte_ptr(digest.data());
        if(environment.original_wasm!=digest)return {error::inconsistent_manifest};
        for(auto member:environment.members)
        {
            if(member>=assigned.size() || assigned[static_cast<::std::size_t>(member)])return {error::inconsistent_manifest};
            assigned[static_cast<::std::size_t>(member)]=true;
        }
    }
        return {error::none};
    }
    }
    // The outer reader must retain its immutable input during this call. This
    // validates DATA only. Actual restore independently requires full debugger
    // authority, current source/policy binding, a stopped cohort, new generated
    // continuations, closed host admission and physical old-worker retirement.
    [[nodiscard]] inline result decode(archive::decoded const& archive,
        ::std::shared_ptr<cp::state const>& output, cp::limits const& cap = {},
        bool use_current_ros_environment = false, bool execution_replay = false) noexcept
    {
        if(archive.disclosure != archive::visibility::full) { return {error::disclosure_denied}; }
        archive::record const* selected{};
        unsigned current_ros{};
        for(auto const& record : archive.records)
        {
            if(record.kind == archive::component::events || record.kind==archive::component::retained_native)
                { return {error::unsupported_component,cp::error::non_replayable_import}; }
            if(record.kind == archive::component::ros1 || record.kind == archive::component::ros2)
            {
                // Current selection explicitly retains the authorized native
                // environment. Local recorded DATA is validated and disclosed,
                // but cannot reconstruct external peers or confer authority.
                auto const bit=record.kind==archive::component::ros1?1u:2u;
                if(current_ros&bit)return {error::unsupported_component};
                if(record.capture==archive::completeness::unavailable)
                {
                    if(!use_current_ros_environment || record.id || record.schema!=1u || !record.payload.empty())return {error::unsupported_component};
                }
                else
                {
                    ::uwvm2::runtime::diagnostic::snapshot_ros::environment env;
                    if(!::uwvm2::runtime::diagnostic::snapshot_ros::decode(archive.disclosure,record,env))return {error::unsupported_component};
                }
                current_ros|=bit;continue;
            }
            if(record.kind != archive::component::wasm_state) { continue; }
            if(selected) { return {error::inconsistent_manifest}; }
            if(record.capture != archive::completeness::complete) { return {error::incomplete_state}; }
            if(record.schema != cp::database_format_version) { return {error::incompatible_schema}; }
            selected = ::std::addressof(record);
        }
        if(!selected) { return {}; }
        // Inspection-only archives deliberately omit a continuation ABI.
        // Classify unavailable state before comparing executable ABI revisions.
        if(archive.origin.logical_abi != materialization::state_schema_revision ||
           archive.origin.continuation_abi != materialization::native_resume_abi_revision)
            { return {error::incompatible_schema}; }
        try
        {
            auto graph = ::uwvm2::utils::container::make_shared<cp::state>();
            auto const decoded = cp::decode_database(selected->payload, *graph, cap);
            if(decoded != cp::error::none) { return {error::invalid_state, decoded}; }
            if(selected->id != graph->checkpoint_id || archive.origin.features != graph->required_features)
                { return {error::inconsistent_manifest}; }
            // The ordinary restoration path does not consume a deterministic
            // event journal. Refuse embedded events too: checking only the outer
            // events component would silently discard a typed log and run real
            // host effects again. DATA parsing never upgrades replay support.
            if(execution_replay)
            {
                cp::execution_replay::decoded journal{};
                auto valid=cp::execution_replay::decode(*graph,journal,cap);
                if(valid!=cp::error::none)return {error::unsupported_component,valid};
            }
            else for(auto const& object:graph->objects)
                if(object.kind==cp::object_kind::event)return {error::unsupported_component,cp::error::non_replayable_import};
            auto manifest=detail::validate_manifest(archive,*graph);if(!manifest)return manifest;
            output = ::std::move(graph);
            return {error::none};
        }
        catch(...) { return {error::allocation_failed}; }
    }
    // Offline historical inspection of a retained native operation. Return only
    // a validated observation journal; this API cannot prepare or resume code.
    // Reuse the full origin/manifest checks over the validated origin so
    // neither the executable decoder nor its unknown-result refusal is relaxed.
    [[nodiscard]] inline result inspect_pending_history(archive::decoded const& input,
        cp::execution_replay::decoded& output,cp::limits const& cap={}) noexcept
    {
        if(input.disclosure!=archive::visibility::full)return {error::disclosure_denied};
        try
        {
            archive::record const* selected{};bool retained{};
            for(auto const& row:input.records)
            {
                if(row.kind==archive::component::retained_native)
                {
                    if(retained || row.id || row.schema!=1u || row.capture!=archive::completeness::partial || row.payload.size()!=32u)
                        return {error::inconsistent_manifest};
                    retained=true;
                }
                if(row.kind==archive::component::wasm_state)
                {
                    if(selected || row.capture!=archive::completeness::complete || row.schema!=cp::database_format_version)
                        return {error::incomplete_state};
                    selected=::std::addressof(row);
                }
            }
            if(!retained || !selected)return {error::missing_state};
            cp::state graph{};auto valid=cp::decode_database(selected->payload,graph,cap);
            if(valid!=cp::error::none)return {error::invalid_state,valid};
            if(selected->id!=graph.checkpoint_id || input.origin.features!=graph.required_features)
                return {error::inconsistent_manifest};
            cp::execution_replay::decoded candidate{};
            valid=cp::execution_replay::inspect_pending_prefix(graph,candidate,cap);
            if(valid!=cp::error::none)return {error::invalid_state,valid};
            if(input.origin.logical_abi!=materialization::state_schema_revision ||
               input.origin.continuation_abi!=materialization::native_resume_abi_revision)
                return {error::incompatible_schema};
            for(auto const& row:input.records)if(row.kind==archive::component::events)
                return {error::unsupported_component};
            auto checked=detail::validate_manifest(input,candidate.origin);if(!checked)return checked;
            output=::std::move(candidate);return {error::none};
        }
        catch(...){return {error::allocation_failed};}
    }
}
