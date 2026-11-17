// Pending native results are process-local continuations. The copied journal
// prefix describes known guest execution; a random file label selects only an
// independently retained, exact live stop. It is not a bearer capability.
struct retained_snapshot_record
{
    ::std::array<::std::byte,32u> label{},digest{};
    ::uwvm2::runtime::lib::llvm_jit_checkpoint_pending_snapshot_owner operation{};
};
::uwvm2::utils::container::std_vector<retained_snapshot_record> retained_snapshots_{};
[[nodiscard]] static bool has_retained_host(controller_reply const& reply) noexcept
{return ::std::any_of(reply.threads.begin(),reply.threads.end(),[](auto const& thread){return thread.retained_host;});}

// Controller mutex and exact pause remain held. No live environment/provider
// locks are acquired: WASI/ROS records in this archive are explicitly the
// recording origin's historical data, not a fabricated pending result.
[[nodiscard]] controller_reply save_retained_snapshot_locked(console_command const& command,controller_reply reply)
{
    namespace lib=::uwvm2::runtime::lib;
    namespace archive=::uwvm2::runtime::diagnostic::snapshot_archive;
    namespace snapshot=::uwvm2::runtime::diagnostic::snapshot_state;
    namespace crash=::uwvm2::runtime::diagnostic::crash;
    reply.status=control_error::invalid_state;
    if(recording_archive_.empty() || command.dump_path.empty() || !has_retained_host(reply))return reply;
    try
    {
        auto captured=lib::llvm_jit_checkpoint_capture_pending_host_api(pause_);
        if(!captured.retained || !captured.graph)return reply;
        archive::decoded origin{};if(!archive::decode(recording_archive_,origin))return reply;
        snapshot::state_stream state{};if(!state.bind(captured.graph))return reply;
        retained_snapshot_record saved{};
        ::fast_io::native_white_hole entropy{};
        ::fast_io::operations::read_all_bytes(entropy,saved.label.data(),saved.label.data()+saved.label.size());
        ::uwvm2::utils::container::std_vector<archive::stream_record> records{};
        for(auto const& record:origin.records)
        {
            if(record.kind==archive::component::wasm_state){records.push_back(state.record());continue;}
            records.push_back({record.kind,record.capture,record.id,record.schema,record.payload.size(),
                const_cast<archive::record*>(::std::addressof(record)),
                [](void* item,void* output,archive::writer write) noexcept
                {return write(output,static_cast<archive::record const*>(item)->payload);}});
        }
        records.push_back({archive::component::retained_native,archive::completeness::partial,0u,1u,saved.label.size(),
            ::std::addressof(saved.label),[](void* item,void* output,archive::writer write) noexcept
            {return write(output,*static_cast<::std::array<::std::byte,32u>*>(item));}});
        struct sink {crash::dump_file file{};::fast_io::sha256_context hash{}; } output{};
        if(!output.file.open(command.dump_path))return reply;
        auto write=[](void* context,::std::span<::std::byte const> bytes) noexcept
        {
            auto& out=*static_cast<sink*>(context);
            if(!crash::dump_file::writer(::std::addressof(out.file),bytes))return false;
            if(!bytes.empty())out.hash.update(bytes.data(),bytes.data()+bytes.size());return true;
        };
        if(!archive::encode_stream(origin.origin,records,::std::addressof(output),write,{},archive::visibility::full))return reply;
        saved.digest=checkpoint::replay_memory_detail::finish(output.hash);saved.operation=::std::move(captured.retained);
        if(!lib::llvm_jit_checkpoint_validate_pending_host_api(pause_,saved.operation))return reply;
        ::std::erase_if(retained_snapshots_,[&](auto const& old)
            {return !lib::llvm_jit_checkpoint_validate_pending_host_api(pause_,old.operation);});
        // Allocate the registry entry before the atomic file publication. A
        // successful file can never lack its actual original operation owner.
        retained_snapshots_.push_back(::std::move(saved));
        if(!output.file.publish()){retained_snapshots_.pop_back();return reply;}
        reply.retained_snapshot=true;reply.dump_published=true;reply.status=control_error::none;
        reply.replay=lib::llvm_jit_checkpoint_replay_status_host_api();
    }
    catch(...){reply.status=control_error::exhausted;}
    return reply;
}

[[nodiscard]] controller_reply restore_retained_snapshot(console_command const& command,controller_reply reply,
    ::uwvm2::runtime::diagnostic::snapshot_archive::decoded const& archive,::std::span<::std::byte const> input)
{
    namespace ar=::uwvm2::runtime::diagnostic::snapshot_archive;
    namespace lib=::uwvm2::runtime::lib;
    namespace policy=::uwvm2::utils::control::dump_permissions;
    reply.status=control_error::invalid_state;reply.restore_phase=4u;
    reply.restore_state_error=static_cast<unsigned>(checkpoint::error::non_replayable_import);
    if(command.restore_recorded || !command.restore_bindings.empty() || archive.disclosure!=ar::visibility::full ||
       policy::permit(policy::disclosure::full).status!=policy::denial::none)return reply;
    ar::record const* selected{};
    for(auto const& record:archive.records)if(record.kind==ar::component::retained_native)
    {
        if(selected || record.id || record.schema!=1u || record.capture!=ar::completeness::partial || record.payload.size()!=32u)return reply;
        selected=::std::addressof(record);
    }
    if(!selected)return reply;
    auto digest=checkpoint::replay_memory_detail::hash(input);
    ::std::lock_guard lock{mutex_};
    if(restoration_in_progress_ || native_owned_ || exited_ || !pause_ || reply.execution!=execution_status::stopped ||
       command.dump_stop_identifier!=stop_identifier_ || reply.stop_identifier!=stop_identifier_)return reply;
    for(auto const& retained:retained_snapshots_)
    {
        if(!::std::equal(selected->payload.begin(),selected->payload.end(),retained.label.begin()) || retained.digest!=digest)continue;
        if(!lib::llvm_jit_checkpoint_validate_pending_host_api(pause_,retained.operation))return reply;
        reply.status=control_error::none;reply.restore_state_error=0u;reply.restore_phase=10u;
        reply.restore_reattached=true;reply.restore_applied=true;reply.restore_original_retained=true;
        reply.restore_epoch=lib::observe_compiler_runtime_generation_host_api();return reply;
    }
    return reply; // Unknown process, expired stop or altered bytes: never dispatch.
}
