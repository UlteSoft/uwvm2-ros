// Private world transaction fragment. Only CURRENT stable v1 environments can
// be handed off here. A file-local handle never becomes a native registration.
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
::uwvm2::imported::ros::checkpoint::owner ros_handoff_{};
#endif
[[nodiscard]] bool prepare_current_ros_handoff(llvm_jit_checkpoint_prepare_request const& request)
{
    bool required{};
    for(auto const& object:saved_.objects)
        required=required || (object.kind==checkpoint_world_data::cp::object_kind::host_resource && object.flags==3u &&
            object.words[0u]==checkpoint_world_data::cp::builtin_ros_v1_binding_kind);
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
    namespace ros=::uwvm2::imported::ros::checkpoint;
    ::uwvm2::utils::container::std_vector<ros::member> members;members.reserve(modules_.size());
    auto const epoch=current_runtime_generation();
    if(epoch==0u || epoch==UINT64_MAX)return fail(preparation_status::source_mismatch);
    for(auto const& bound:modules_)
    {
        if(bound.old_actual_id>=g_runtime.modules.size() || bound.new_actual_id>=indirect_dispatch_modules_.size())
            return fail(preparation_status::source_mismatch);
        auto const& old=g_runtime.modules.index_unchecked(bound.old_actual_id);
        auto& next=indirect_dispatch_modules_.index_unchecked(bound.new_actual_id);
        if(!old.runtime_module || next.runtime_module!=bound.actual || !old.llvm_jit_full_publication ||
           old.llvm_jit_full_publication->source->actual_validated_file(bound.old_actual_id,epoch,old.runtime_module)==nullptr)
            return fail(preparation_status::source_mismatch);
        // Permanent retirement survives every world replacement, even if the
        // module never created a ROS owner and has nothing in the host registry.
        next.ros_authority_retired=old.ros_authority_retired;
        members.push_back({old.runtime_module,bound.actual,epoch,epoch+1u,
            {reinterpret_cast<char const*>(bound.file->module_name.data()),bound.file->module_name.size()},old.ros_authority_retired,bound.old_actual_id});
    }
    if(!required)return request.recorded_ros_environments.empty() || fail(preparation_status::unavailable_adapter);
    if(request.environment_source==llvm_jit_checkpoint_environment_source::recorded)
    {
        auto prepare=ros::prepare_recorded_hook.load(::std::memory_order_acquire);
        unsigned expected{},present{};
        for(auto const& object:saved_.objects)
            if(object.kind==checkpoint_world_data::cp::object_kind::host_resource && object.flags==3u &&
               object.words[0u]==checkpoint_world_data::cp::builtin_ros_v1_binding_kind)
                expected|=object.words[1u]<=2u?1u:2u;
        for(auto const& env:request.recorded_ros_environments)
        {
            if(!env || env->recording!=saved_.recording_id || env->backend<1 || env->backend>2 ||
               !::uwvm2::runtime::diagnostic::snapshot_ros::valid(*env))return fail(preparation_status::unavailable_adapter);
            auto bit=1u<<(env->backend-1u);if(present&bit)return fail(preparation_status::unavailable_adapter);present|=bit;
        }
        if(present!=expected || !prepare || prepare(members,request.recorded_ros_environments,ros_handoff_)!=ros::status::ready)
            return fail(preparation_status::unavailable_adapter);
    }
    else
    {
        if(request.environment_source!=llvm_jit_checkpoint_environment_source::current)
            return fail(preparation_status::unavailable_adapter);
        auto prepare=ros::prepare_hook.load(::std::memory_order_acquire);
        if(!prepare || prepare(members,ros_handoff_)!=ros::status::ready)return fail(preparation_status::unavailable_adapter);
    }
    if(!ros_handoff_ || !ros_handoff_->current())return fail(preparation_status::unavailable_adapter);
    return true;
#else
# if defined(UWVM_IMPORT_ROSV2)
    // Shared robot authority retirement is independent of the deprecated
    // checkpoint adapter. Disabling ROSv1 must never restore this bit when a
    // debugger publishes replacement Wasm. Scene resources are not replayed.
    auto const epoch=current_runtime_generation();
    if(epoch==0u || epoch==UINT64_MAX)return fail(preparation_status::source_mismatch);
    for(auto const& bound:modules_)
    {
        if(bound.old_actual_id>=g_runtime.modules.size() || bound.new_actual_id>=indirect_dispatch_modules_.size())
            return fail(preparation_status::source_mismatch);
        auto const& old=g_runtime.modules.index_unchecked(bound.old_actual_id);
        auto& next=indirect_dispatch_modules_.index_unchecked(bound.new_actual_id);
        if(!old.runtime_module || next.runtime_module!=bound.actual || !old.llvm_jit_full_publication ||
           old.llvm_jit_full_publication->source->actual_validated_file(bound.old_actual_id,epoch,old.runtime_module)==nullptr)
            return fail(preparation_status::source_mismatch);
        next.ros_authority_retired=old.ros_authority_retired;
    }
# endif
    return (!required && request.recorded_ros_environments.empty()) || fail(preparation_status::unavailable_adapter);
#endif
}
[[nodiscard]] bool current_ros_handoff() const noexcept
{
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
    return !ros_handoff_ || ros_handoff_->current();
#else
    return true;
#endif
}
[[nodiscard]] bool publish_current_ros_handoff() noexcept
{
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
    return !ros_handoff_ || ros_handoff_->publish();
#else
    return true;
#endif
}
