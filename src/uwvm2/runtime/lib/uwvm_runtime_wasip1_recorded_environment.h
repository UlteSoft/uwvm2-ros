// Private recorded-data preparation. Every replacement is staged while the
// real manager owns the complete stop, host admission and publication scope.
#if !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1)
[[nodiscard]] static bool recorded_table_matches(
    ::uwvm2::runtime::diagnostic::snapshot_wasip1::environment const& saved,
    ::std::span<fd_ref const> resources, wasip1_env_type& environment,
    runtime_wasip1_debug_environment::strings const& arguments,
    runtime_wasip1_debug_environment::strings const& variables) noexcept
{
    auto text=[](auto const& owners,auto const& borrowed,auto const& data) noexcept
    {
        if(!runtime_wasip1_debug_environment::bound_text(owners,borrowed) || owners.size()!=data.size())return false;
        for(::std::size_t i{};i<owners.size();++i)
        {
            auto const& value=owners.index_unchecked(i);
            if(value.size()!=data[i].size() || (!value.empty() &&
               ::fast_io::freestanding::my_memcmp(value.data(),data[i].data(),value.size())))return false;
        }
        return true;
    };
    auto& table=environment.fd_storage;
    if(!text(arguments,environment.argv,saved.arguments) || !text(variables,environment.envs,saved.variables) ||
       table.opens.size()!=saved.opens_size || table.closes.size()!=saved.closed.size() ||
       table.opens.size()+table.renumber_map.size()!=saved.bindings.size()+saved.reserved.size()+saved.closed.size())return false;
    for(::std::size_t i{};i<saved.closed.size();++i)
    {
        auto const fd=saved.closed[i];
        if(table.closes.index_unchecked(i)!=fd || !table.opens.index_unchecked(fd).fd_p ||
           table.opens.index_unchecked(fd).fd_p->close_pos!=i)return false;
    }
    for(auto const& expected:saved.reserved)
    {
        auto const* slot=find_actual_descriptor(table,expected.descriptor);
        if(!slot || slot->close_pos!=(expected.protected_slot?SIZE_MAX-1u:SIZE_MAX) ||
           (slot->wasi_fd.ptr!=nullptr)!=expected.null_resource ||
           (slot->wasi_fd.ptr && slot->wasi_fd.ptr->wasi_fd_storage.type!=fd_kind::null) ||
           static_cast<::std::uint64_t>(slot->rights_base)!=expected.base ||
           static_cast<::std::uint64_t>(slot->rights_inherit)!=expected.inheriting)return false;
    }
    for(auto const& expected:saved.bindings)
    {
        auto const* slot=find_actual_descriptor(table,expected.descriptor);
        if(!slot || slot->close_pos!=SIZE_MAX || expected.resource_index>=resources.size() ||
           !resources[expected.resource_index].ptr || slot->wasi_fd.ptr!=resources[expected.resource_index].ptr ||
           static_cast<::std::uint64_t>(slot->rights_base)!=expected.base ||
           static_cast<::std::uint64_t>(slot->rights_inherit)!=expected.inheriting)return false;
    }
    return true;
}
[[nodiscard]] static status prepare_recorded_group(
    ::std::span<owner const> current, llvm_jit_checkpoint_prepare_request const& request,
    prepared_environment_group& prepared)
{
    namespace wire=::uwvm2::runtime::diagnostic::snapshot_wasip1;
    namespace fm=::uwvm2::imported::wasi::wasip1::fd_manager;
    if(current.size()!=prepared.size() || current.size()!=request.recorded_wasip1_environments.size())return status::invalid_portable_snapshot;
    ::std::uint64_t remaining=request.maximum_native_payload_bytes;
    auto charge=[&](::std::uint64_t count,::std::uint64_t unit) noexcept
    { if(unit && count>remaining/unit)return false;remaining-=count*unit;return true; };
    ::std::size_t used_bindings{};
    for(::std::size_t index{};index<current.size();++index)
    {
        auto actual=canonical(current[index]);auto& target=*prepared[index];
        if(!actual || !actual->data_.archive_environment || !target.target || !target.preserve_current_resources)return status::invalid_capsule_owner;
        auto const& live=*actual->data_.archive_environment;
        ::std::shared_ptr<wire::environment const> saved{};
        for(auto const& entry:request.recorded_wasip1_environments)
        {
            if(!entry)return status::invalid_portable_snapshot;
            if(entry->module==live.module) { if(saved)return status::invalid_portable_snapshot;saved=entry; }
        }
        if(!saved || !wire::valid(*saved) || saved->recording!=request.recording_label ||
           saved->members!=live.members || saved->original_wasm!=live.original_wasm || saved->builtin_interface!=live.builtin_interface)
            return status::invalid_portable_snapshot;
        // The current grouping is authoritative. An archive cannot merge two
        // single groups, split a shared environment, or move another group's FD.
        if(saved->bindings.size()+saved->reserved.size()>target.target->fd_storage.fd_limit)return status::resource_limit;
        if(!charge(saved->opens_size,sizeof(fm::wasi_fd_unique_ptr_t)+sizeof(fd)) ||
           !charge(saved->bindings.size()+saved->reserved.size(),sizeof(fm::wasi_fd_unique_ptr_t)+sizeof(fd)+64u) ||
           !charge(saved->closed.size(),sizeof(::std::size_t)) || !charge(saved->resources.size(),sizeof(fd_ref)+sizeof(fd)+64u) ||
           !charge(saved->arguments.size()+saved->variables.size(),sizeof(::fast_io::u8string)+sizeof(::fast_io::u8string_view)))
            return status::resource_limit;
        for(auto const& text:saved->arguments)if(!charge(text.size(),1u) || !charge(1u,1u))return status::resource_limit;
        for(auto const& text:saved->variables)if(!charge(text.size(),1u) || !charge(1u,1u))return status::resource_limit;
        for(auto const& resource:saved->resources)if(!charge(resource.content.size(),1u))return status::resource_limit;
        for(auto const& reserved:live.reserved)
        {
            if(!reserved.protected_slot)continue;
            auto found=::std::find_if(saved->reserved.begin(),saved->reserved.end(),[&](auto const& entry){return entry.descriptor==reserved.descriptor;});
            if(found==saved->reserved.end() || !found->protected_slot)return status::capability_denied;
        }
        ::uwvm2::utils::container::std_vector<fd_ref> replacements(saved->resources.size());
        ::uwvm2::utils::container::std_vector<fd const*> selected(saved->resources.size());
        ::uwvm2::utils::container::std_vector<void const*> external_identities{};
        for(auto const& binding:request.wasip1_resource_rebindings)
        {
            if(binding.module!=live.module)continue;
            if(binding.resource>=selected.size() || selected[binding.resource])return status::invalid_portable_snapshot;
            auto const* slot=find_actual_descriptor(target.target->fd_storage,binding.target_descriptor);
            if(!slot || slot->close_pos!=SIZE_MAX || !slot->wasi_fd.ptr || slot->wasi_fd.ptr->wasi_fd_storage.type==fd_kind::null)
                return status::capability_denied;
            selected[binding.resource]=slot;++used_bindings;
        }
        for(::std::size_t n{};n<saved->resources.size();++n)
        {
            auto const* allowed=selected[n];if(!allowed)return status::missing_rebind;
            auto const& resource=saved->resources[n];auto const& current_resource=*allowed->wasi_fd.ptr;
            auto binding=::std::find_if(live.bindings.begin(),live.bindings.end(),[&](auto const& row)
            {return row.descriptor<=INT32_MAX && find_actual_descriptor(target.target->fd_storage,row.descriptor)==allowed;});
            if(binding==live.bindings.end() || binding->resource_index>=live.resources.size() ||
               live.resources[binding->resource_index].kind!=resource.kind)return status::capability_denied;
            if(resource.kind==wire::resource_kind::stdio && current_resource.checkpoint_stdio_index!=resource.stdio_index)
                return status::capability_denied;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            if(resource.kind==wire::resource_kind::stdio && !target.target->grants_stdio(resource.stdio_index))return status::capability_denied;
#endif
            for(auto const& row:saved->bindings)
            {
                if(row.resource_index!=n)continue;
                if(!::uwvm2::uwvm::debugger::wasip1_state::rights_subset(row.base,row.inheriting,
                    static_cast<::std::uint64_t>(allowed->rights_base),static_cast<::std::uint64_t>(allowed->rights_inherit)))
                    return status::capability_denied;
                if(row.descriptor<3u && (resource.kind!=wire::resource_kind::stdio || resource.stdio_index!=row.descriptor))
                    return status::capability_denied;
            }
            if(resource.kind==wire::resource_kind::managed_file)
            {
                if(!current_resource.checkpoint_managed_identity || resource.offset>static_cast<::std::uint64_t>((::std::numeric_limits<::fast_io::intfpos_t>::max)()))
                    return status::capability_denied;
                runtime_wasip1_debug_environment::create_managed_file(replacements[n]);
                auto& file=runtime_wasip1_debug_environment::managed_file(replacements[n]);
                if(!resource.content.empty())::fast_io::operations::pwrite_all_bytes(file,resource.content.data(),resource.content.data()+resource.content.size(),0);
                int flags{};
                if(!::uwvm2::runtime::lib::wasip1_native_file::native_flags(resource.flags,flags))
                    return status::incompatible_flags;
                runtime_wasip1_debug_environment::set_managed_flags(replacements[n],flags);
                if(::uwvm2::runtime::lib::wasip1_native_file::wasi_flags(
                    runtime_wasip1_debug_environment::managed_flags(replacements[n]))!=resource.flags)
                    return status::incompatible_flags;
                if(::fast_io::operations::io_stream_seek_bytes(file,static_cast<::fast_io::intfpos_t>(resource.offset),::fast_io::seekdir::beg)!=static_cast<::fast_io::intfpos_t>(resource.offset) ||
                   ::fast_io::status(file).size!=resource.content.size())return status::native_operation_failed;
            }
            else
            {
                // Kernel streams, sockets and files stay at CURRENT state. A
                // saved path/offset never reopens, rewinds or replays external IO.
                replacements[n]=allowed->wasi_fd;external_identities.push_back(allowed->wasi_fd.ptr);
            }
        }
        ::std::sort(external_identities.begin(),external_identities.end(),::std::less<void const*>{});
        if(::std::adjacent_find(external_identities.begin(),external_identities.end())!=external_identities.end())return status::invalid_portable_snapshot;
        decltype(target.opens) opens{};decltype(target.renumber) renumber{};decltype(target.closes) closes{};
        opens.reserve(static_cast<::std::size_t>(saved->opens_size));
        for(::std::size_t i{};i<saved->opens_size;++i)
        { fm::wasi_fd_unique_ptr_t slot{};slot.fd_p->wasi_fd=fd_ref{fm::wasi_no_construct};opens.push_back(::std::move(slot)); }
        auto place=[&](::std::uint32_t number,fm::wasi_fd_unique_ptr_t slot)
        {
            if(number<opens.size())opens.index_unchecked(number)=::std::move(slot);
            else renumber.emplace(static_cast<::std::int32_t>(number),::std::move(slot));
        };
        for(auto const& row:saved->bindings)
        {
            fm::wasi_fd_unique_ptr_t slot{};slot.fd_p->wasi_fd=replacements[row.resource_index];
            slot.fd_p->rights_base=static_cast<decltype(slot.fd_p->rights_base)>(row.base);
            slot.fd_p->rights_inherit=static_cast<decltype(slot.fd_p->rights_inherit)>(row.inheriting);place(row.descriptor,::std::move(slot));
        }
        for(auto const& row:saved->reserved)
        {
            fm::wasi_fd_unique_ptr_t slot{};if(!row.null_resource)slot.fd_p->wasi_fd=fd_ref{fm::wasi_no_construct};
            slot.fd_p->rights_base=static_cast<decltype(slot.fd_p->rights_base)>(row.base);
            slot.fd_p->rights_inherit=static_cast<decltype(slot.fd_p->rights_inherit)>(row.inheriting);
            if(row.protected_slot)slot.fd_p->close_pos=SIZE_MAX-1u;place(row.descriptor,::std::move(slot));
        }
        for(auto number:saved->closed)
        { opens.index_unchecked(number).fd_p->close_pos=closes.size();closes.push_back(number); }
        prepared_current_environment::strings args{},vars{};prepared_current_environment::views arg_views{},var_views{};
        auto copy=[](auto const& data,auto& owned,auto& views)
        {
            owned.reserve(data.size());views.reserve(data.size());
            for(auto const& bytes:data)owned.emplace_back(::fast_io::u8string_view{reinterpret_cast<char8_t const*>(bytes.data()),bytes.size()});
            for(auto const& value:owned)views.emplace_back(value.data(),value.size());
        };
        copy(saved->arguments,args,arg_views);copy(saved->variables,vars,var_views);
        target.opens.swap(opens);target.renumber.swap(renumber);target.closes.swap(closes);target.resources=::std::move(replacements);
        target.new_arguments.swap(args);target.new_variables.swap(vars);target.argument_views.swap(arg_views);target.variable_views.swap(var_views);
        target.recorded=::std::move(saved);
    }
    if(used_bindings!=request.wasip1_resource_rebindings.size())return status::invalid_portable_snapshot;
    return status::captured;
}
#endif
