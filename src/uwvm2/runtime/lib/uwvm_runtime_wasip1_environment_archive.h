// Private immutable archive projection, called in the SAME host-closed
// capture scope as the retained resources. No later live handle lookup.
[[nodiscard]] status detach_archive_environment(wasip1_env_type const& actual_environment)
{
    namespace wire=::uwvm2::runtime::diagnostic::snapshot_wasip1;
    auto saved=::uwvm2::utils::container::make_shared<wire::environment>();
    saved->recording=data_.recording_label;saved->original_wasm=data_.original_wasm;
    saved->builtin_interface=data_.builtin_interface;saved->module=data_.module;saved->opens_size=opens_size_;
    auto copy=[](auto const& text,wire::bytes& bytes)
    {
        if(text.empty()) { return; }
        auto const* first=reinterpret_cast<::std::byte const*>(text.data());bytes.assign(first,first+text.size());
    };
    for(::std::size_t n{};n<g_runtime.modules.size();++n)
    {
        if(is_wasip1_import_visible_for_runtime_module_id_slow(n) &&
           ::std::addressof(resolve_wasip1_env_for_runtime_module_id(n))==::std::addressof(actual_environment))
        { saved->members.push_back(n); }
    }
    for(auto const& text:data_.arguments) { copy(text,saved->arguments.emplace_back()); }
    for(auto const& text:data_.environment) { copy(text,saved->variables.emplace_back()); }
    saved->resources.reserve(resources_.size());
    for(auto const& pinned:resources_)
    {
        wire::resource value{};
        if(!pinned.actual.ptr) { return status::stale_environment; }
        auto const& actual=*pinned.actual.ptr;
        if(pinned.managed_identity)
        {
            if(pinned.managed_offset<0) { return status::unavailable_environment; }
            value.kind=wire::resource_kind::managed_file;value.offset_known=value.flags_known=true;
            value.offset=static_cast<::std::uint64_t>(pinned.managed_offset);value.content=pinned.managed_content;
            value.flags=::uwvm2::runtime::lib::wasip1_native_file::wasi_flags(pinned.managed_flags);
        }
        else if(actual.checkpoint_stdio_index>=0 && actual.checkpoint_stdio_index<=2)
        { value.kind=wire::resource_kind::stdio;value.stdio_index=static_cast<::std::uint8_t>(actual.checkpoint_stdio_index); }
        else if(pinned.original_kind==fd_kind::dir)
        {
            value.kind=wire::resource_kind::directory;value.follow=actual.checkpoint_follow;
            if(pinned.directories.empty()) { return status::unavailable_environment; }
            copy(pinned.directories.front().original_name,value.mount);
            for(::std::size_t n{1u};n<pinned.directories.size();++n)
            {
                auto const& text=pinned.directories[n].original_name;
                if(n!=1u) { value.path.push_back(::std::byte{'/'}); }
                if(!text.empty())
                { auto const* first=reinterpret_cast<::std::byte const*>(text.data());value.path.insert(value.path.end(),first,first+text.size()); }
            }
        }
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS) && defined(__linux__)
        else if(actual.socket_kind!=::uwvm2::imported::wasi::wasip1::func::socket_capability::socket_type::none)
        { value.kind=wire::resource_kind::socket; }
#endif
#if defined(_WIN32) && !defined(__CYGWIN__)
        else if(pinned.original_kind==fd_kind::socket || pinned.original_kind==fd_kind::socket_observer)
        { value.kind=wire::resource_kind::socket; }
#endif
        else
        {
            // Ordinary host files stay external. The recorded guest route is
            // descriptive; no importer may interpret it as a native path.
            value.kind=wire::resource_kind::external_file;
            if(actual.checkpoint_reopenable)
            { copy(actual.checkpoint_mount,value.mount);copy(actual.checkpoint_path,value.path);value.follow=actual.checkpoint_follow; }
        }
        saved->resources.push_back(::std::move(value));
    }
    for(::std::size_t n{};n<bindings_.size();++n)
    {
        auto const& link=bindings_[n];auto const& row=data_.descriptors[n];
        if(link.guest_number>INT32_MAX || link.resource>UINT32_MAX) { return status::unavailable_environment; }
        saved->bindings.push_back({static_cast<::std::uint32_t>(link.guest_number),static_cast<::std::uint32_t>(link.resource),row.base_rights,row.inheriting_rights});
    }
    for(auto const& entry:reserved_empty_)
    {
        if(entry.number>INT32_MAX) { return status::unavailable_environment; }
        saved->reserved.push_back({static_cast<::std::uint32_t>(entry.number),entry.null_resource,entry.protected_slot,entry.base,entry.inheriting});
    }
    for(auto number:closed_order_)
    { if(number>INT32_MAX) { return status::unavailable_environment; }saved->closed.push_back(static_cast<::std::uint32_t>(number)); }
    if(!wire::valid(*saved)) { return status::invalid_portable_snapshot; }
    data_.archive_environment=::std::move(saved);return status::captured;
}
