// Private controller recording transaction. Input/output files are host-owned;
// dump disclosure remains immutable and independent of execution authority.
::uwvm2::utils::container::std_vector<::std::byte> recording_archive_{};
::std::array<::std::byte,16u> recording_label_{};
[[nodiscard]] controller_reply record_snapshot(console_command const& command,controller_reply reply)
{
    namespace lib=::uwvm2::runtime::lib;
    namespace archive=::uwvm2::runtime::diagnostic::snapshot_archive;
    namespace snapshot=::uwvm2::runtime::diagnostic::snapshot_state;
    namespace policy=::uwvm2::utils::control::dump_permissions;
    namespace crash=::uwvm2::runtime::diagnostic::crash;
    reply.status=control_error::invalid_state;
    if(command.recording_action==4u || command.recording_action==5u || command.recording_action==7u)
        return inspect_snapshot_history(command,::std::move(reply));
    if(command.recording_action==3u)
    {reply.replay=lib::llvm_jit_checkpoint_replay_status_host_api();reply.status=control_error::none;return reply;}
    auto permit=policy::permit(policy::disclosure::full);
    if(permit.status!=policy::denial::none){reply.dump_denial=permit.status;return reply;}
    // A running/stale-stop refusal still reports the actual journal, rather
    // than the default mode=0. This observation conveys no pause authority.
    reply.replay=lib::llvm_jit_checkpoint_replay_status_host_api();
    ::std::lock_guard lock{mutex_};
    if(restoration_in_progress_ || reply.execution!=execution_status::stopped || reply.threads.empty() ||
       !pause_ || native_owned_ || exited_ || domain_->is_closed() || command.dump_stop_identifier!=stop_identifier_ ||
       reply.stop_identifier!=stop_identifier_)return reply;
    try
    {
        if(command.recording_action==2u && has_retained_host(reply))
            return save_retained_snapshot_locked(command,::std::move(reply));
        ::uwvm2::utils::container::std_vector<lib::llvm_jit_checkpoint_thread_capture_owner> captures{};::std::size_t count{};
        if(!source_capture_cohort_locked(reply,reply.threads.front().identifier,captures,count))return reply;
        captures.resize(count);
        if(command.recording_action==1u)
        {
            if(lib::llvm_jit_checkpoint_replay_status_host_api().mode)return reply;
            ::uwvm2::utils::container::std_vector<::std::byte> bytes{};
            auto append=[](void* context,::std::span<::std::byte const> input) noexcept
            {
                try{auto& bytes=*static_cast<::uwvm2::utils::container::std_vector<::std::byte>*>(context);bytes.insert(bytes.end(),input.begin(),input.end());return true;}
                catch(...){return false;}
            };
            auto captured=lib::runtime_dump_stopped_guest_host_api(pause_,reply.threads.front().location.code_generation,
                "uwvm2-ros","execution recording origin",::std::addressof(bytes),append,2u,stop_identifier_,
                static_cast<unsigned>(reply.reason),captures);
            if(!captured)return reply;
            archive::decoded decoded{};::std::shared_ptr<checkpoint::state const> graph{};
            if(!archive::decode(bytes,decoded))return reply;
            auto origin=snapshot::decode(decoded,graph,{},true);
            if(!origin)
            {
                reply.restore_state_error=static_cast<unsigned>(origin.state_error);
                for(auto const& record:decoded.records)
                {
                    if(record.kind==archive::component::wasm_state && record.capture==archive::completeness::unavailable)
                    {
                        reply.recording_origin_unavailable=true;
                        break;
                    }
                }
                return reply;
            }
            auto history=select_snapshot_history(decoded,graph->recording_id);
            auto started=lib::llvm_jit_checkpoint_record_host_api(pause_,captures,graph->recording_id,1u);
            reply.restore_state_error=static_cast<unsigned>(started.data_error);
            reply.replay=lib::llvm_jit_checkpoint_replay_status_host_api();
            if(started.status!=lib::llvm_jit_checkpoint_instance_capture_status::captured)return reply;
            recording_archive_=::std::move(bytes);recording_label_=graph->recording_id;
            install_snapshot_history_locked(::std::move(history),graph->recording_id);
            reply.replay=lib::llvm_jit_checkpoint_replay_status_host_api();reply.status=control_error::none;return reply;
        }
        if(command.recording_action==6u)
        {
            if(recording_archive_.empty())return reply;
            auto marked=lib::llvm_jit_checkpoint_record_host_api(pause_,captures,recording_label_,3u);
            reply.restore_state_error=static_cast<unsigned>(marked.data_error);
            reply.replay=lib::llvm_jit_checkpoint_replay_status_host_api();
            if(marked.status!=lib::llvm_jit_checkpoint_instance_capture_status::captured)return reply;
            reply.replay=lib::llvm_jit_checkpoint_replay_status_host_api();reply.status=control_error::none;return reply;
        }
        if(command.recording_action!=2u || recording_archive_.empty() || command.dump_path.empty())return reply;
        archive::decoded decoded{};if(!archive::decode(recording_archive_,decoded))return reply;
        auto sealed=lib::llvm_jit_checkpoint_record_host_api(pause_,captures,recording_label_,2u);
        reply.restore_state_error=static_cast<unsigned>(sealed.data_error);
        reply.replay=lib::llvm_jit_checkpoint_replay_status_host_api();
        if(sealed.status!=lib::llvm_jit_checkpoint_instance_capture_status::captured || !sealed.graph)return reply;
        snapshot::state_stream state{};if(!state.bind(sealed.graph))return reply;
        ::uwvm2::utils::container::std_vector<archive::stream_record> records{};records.reserve(decoded.records.size());
        for(auto const& record:decoded.records)
        {
            if(record.kind==archive::component::wasm_state){records.push_back(state.record());continue;}
            records.push_back({record.kind,record.capture,record.id,record.schema,record.payload.size(),
                const_cast<archive::record*>(::std::addressof(record)),
                [](void* item,void* output,archive::writer write) noexcept
                {return write(output,static_cast<archive::record const*>(item)->payload);}});
        }
        crash::dump_file output;if(!output.open(command.dump_path))return reply;
        if(!archive::encode_stream(decoded.origin,records,::std::addressof(output),crash::dump_file::writer,{},archive::visibility::full) ||
           !output.publish())return reply;
        reply.dump_published=true;reply.replay=lib::llvm_jit_checkpoint_replay_status_host_api();reply.status=control_error::none;
    }
    catch(...){reply.status=control_error::invalid_state;}
    return reply;
}
