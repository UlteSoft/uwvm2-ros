// Private controller fragment. Archive data cannot acquire a stop, source,
// capability, execution lease or native address. Every restore uses the actual
// current cohort and the runtime's closed preparation/OS-join/publication path.
bool restoration_in_progress_{}, snapshot_rebinding_{}, snapshot_published_{};
::std::uint_least64_t snapshot_epoch_{};
::uwvm2::runtime::lib::llvm_jit_checkpoint_native_retirement_owner snapshot_operation_{};
::uwvm2::runtime::lib::llvm_jit_checkpoint_native_retirement_result snapshot_result_{};
bool (*snapshot_source_refresher_)(controller&){};
::uwvm2::utils::container::std_vector<snapshot_history_record> pending_snapshot_history_{};
::std::array<::std::byte,16u> pending_snapshot_history_label_{};

[[nodiscard]] bool source_installation_permitted_locked() const noexcept
{
    if(exited_)return false;
    if(!source_installation_closed_ && !pause_)return true;
    return restoration_in_progress_ && snapshot_rebinding_ && pause_ && snapshot_epoch_ &&
        snapshot_epoch_==::uwvm2::runtime::lib::observe_compiler_runtime_generation_host_api();
}
static bool restored_pause(void* context,domain_type const* domain,
    domain_type::pause_ticket const& ticket,::std::uint_least64_t epoch) noexcept
{
    auto& self=*static_cast<controller*>(context);
    ::std::lock_guard lock{self.mutex_};
    if(!self.restoration_in_progress_ || self.native_owned_ || self.exited_ ||
       domain!=self.domain_.get() || !ticket || !epoch)return false;
    self.cancel_step_locked();self.clear_traces_locked();self.native_code_capture_={};
    self.source_frame_cursor_={};self.source_modules_.clear();
    self.pause_=ticket;self.snapshot_epoch_=epoch;self.snapshot_published_=true;
    self.install_snapshot_history_locked(::std::move(self.pending_snapshot_history_),self.pending_snapshot_history_label_);
    self.reason_=stop_reason::requested;self.source_statement_stop_=false;
    self.first_wasm_instruction_pending_=false;self.step_thread_=0u;
    self.stopped_wasm_catchpoint_=0u;self.terminal_wasm_participant_=0u;
    // Native breakpoints and value watches belong to old code/activations.
    // Keep logical source/Wasm breakpoints; revalidate them before continuation.
    for(auto& point:self.breakpoints_)if(point.assembly)point={};
    for(auto& point:self.watchpoints_){point.enabled=false;point.value.clear();point.incarnation=0u;}
    for(auto& saved:self.wasip1_checkpoints_)saved.reset();
    self.rebuild_wasm_breakpoint_index_locked();self.advance_stop_identifier_locked();
    ::uwvm2::runtime::checkpoint::observation_probe_hints::require_all(::std::addressof(self));
    self.changed_.notify_all();return true;
}
static void restored_complete(void* context,domain_type const* domain,
    ::std::uint_least64_t epoch,::std::uint_least32_t code) noexcept
{
    auto& self=*static_cast<controller*>(context);
    ::std::lock_guard lock{self.mutex_};
    if(domain!=self.domain_.get() || epoch!=self.snapshot_epoch_)return;
    self.exited_=true;self.exit_code_=code;self.cancel_step_locked();
    self.source_installation_closed_=true;self.native_code_capture_={};self.changed_.notify_all();
}
[[nodiscard]] static bool read_snapshot_file(::uwvm2::utils::container::std_string const& path,::uwvm2::utils::container::std_vector<::std::byte>& bytes)
{
    if(path.empty() || path.find('\0')!=::uwvm2::utils::container::std_string::npos)return false;
    ::uwvm2::utils::container::std_u8string native_path{reinterpret_cast<char8_t const*>(path.data()),path.size()};
    // Linux FIFO/device names are not archives. Opening nonblocking lets the
    // descriptor's real file type be checked without hanging the management
    // thread on a FIFO that has no writer. Regular-file reads stay synchronous.
#if defined(__linux__)
    ::fast_io::native_file file{native_path,::fast_io::open_mode::in|::fast_io::open_mode::no_block};
#else
    ::fast_io::native_file file{native_path,::fast_io::open_mode::in};
#endif
    auto before=::fast_io::status(file);
    if(before.type!=::fast_io::file_type::regular || !before.size || before.size>bytes.max_size() ||
       before.size>static_cast<::std::uintmax_t>(PTRDIFF_MAX))return false;
    bytes.resize(static_cast<::std::size_t>(before.size));
    ::fast_io::operations::read_all_bytes(file,bytes.data(),bytes.data()+bytes.size());
    ::std::byte extra{};
    if(::fast_io::operations::read_some_bytes(file,::std::addressof(extra),::std::addressof(extra)+1u)!=::std::addressof(extra))return false;
    auto after=::fast_io::status(file);file.close();
    return before.dev==after.dev && before.ino==after.ino && before.size==after.size &&
        before.mtim==after.mtim && before.ctim==after.ctim;
}
[[nodiscard]] controller_reply restore_snapshot(console_command const& command,controller_reply initial,
    ::std::chrono::milliseconds timeout,management_wait_interrupt interrupt)
{
    namespace lib=::uwvm2::runtime::lib;
    namespace archive=::uwvm2::runtime::diagnostic::snapshot_archive;
    namespace snapshot=::uwvm2::runtime::diagnostic::snapshot_state;
    using state=lib::llvm_jit_checkpoint_native_retirement_status;
    initial.status=control_error::invalid_state;
    auto deadline=clock_type::now()+(::std::max)(timeout,::std::chrono::milliseconds{30000});
    try
    {
        if(command.operation!=operation::status || command.payload_size || command.wait_for_event ||
           command.run_from_entry || command.launch_action!=console_launch_action::none ||
           command.dump_disclosure || command.dump_policy_query)
        {initial.status=control_error::malformed;return initial;}
        lib::llvm_jit_checkpoint_prepare_request requested{};
        ::uwvm2::utils::container::std_vector<lib::llvm_jit_checkpoint_thread_capture_owner> captures{};
        domain_type::pause_ticket ticket{};
        bool published{};
        lib::llvm_jit_checkpoint_native_retirement_owner pending{};
        if(!command.restore_continue)
        {
            {
                ::std::lock_guard lock{mutex_};
                initial.restore_original_retained=!restoration_in_progress_ &&
                    initial.execution==execution_status::stopped && pause_ && !native_owned_ &&
                    !exited_ && !domain_->is_closed();
                if(restoration_in_progress_ || !snapshot_source_refresher_ || initial.execution!=execution_status::stopped ||
                   initial.threads.empty() || !pause_ || native_owned_ || exited_ || domain_->is_closed() ||
                   initial.stop_identifier!=stop_identifier_ || command.dump_stop_identifier!=stop_identifier_)
                {initial.restore_phase=1u;return initial;}
            }
            ::uwvm2::utils::container::std_vector<::std::byte> input{};archive::decoded decoded{};
            if(!read_snapshot_file(command.dump_path,input)) {initial.restore_phase=2u;return initial;}
            auto envelope=archive::decode(input,decoded);
            if(!envelope) {initial.restore_phase=3u;initial.restore_detail=static_cast<unsigned>(envelope.status);return initial;}
            if(::std::any_of(decoded.records.begin(),decoded.records.end(),[](auto const& record)
                {return record.kind==archive::component::retained_native;}))
                return restore_retained_snapshot(command,::std::move(initial),decoded,input);
            auto graph=snapshot::decode(decoded,requested.saved_graph,requested.graph_budget,!command.restore_recorded,command.restore_replay);
            if(!graph) {initial.restore_phase=4u;initial.restore_detail=static_cast<unsigned>(graph.status);
                initial.restore_state_error=static_cast<unsigned>(graph.state_error);return initial;}
            if(command.restore_replay)
            {
                namespace cp=::uwvm2::uwvm::debugger::checkpoint;
                cp::execution_replay::decoded journal{};
                auto valid=cp::execution_replay::decode(*requested.saved_graph,journal,requested.graph_budget);
                if(valid!=cp::error::none){initial.restore_phase=4u;initial.restore_state_error=static_cast<unsigned>(valid);return initial;}
                requested.replay_graph=requested.saved_graph;
                requested.saved_graph=::uwvm2::utils::container::make_shared<cp::state const>(::std::move(journal.origin));
            }
            requested.recording_label=requested.saved_graph->recording_id;
            auto history=select_snapshot_history(decoded,requested.recording_label);
            requested.include_wasip1=true;requested.require_managed_wasip1_resources=false;requested.pause_after_restore=true;
            if(command.restore_recorded)
            {
                requested.environment_source=lib::llvm_jit_checkpoint_environment_source::recorded;
                namespace wasi=::uwvm2::runtime::diagnostic::snapshot_wasip1;
                for(auto const& record:decoded.records)
                {
                    if(record.kind==archive::component::ros1 || record.kind==archive::component::ros2)
                    {
                        namespace ros=::uwvm2::runtime::diagnostic::snapshot_ros;
                        auto environment=::uwvm2::utils::container::make_shared<ros::environment>();
                        if(!ros::decode(decoded.disclosure,record,*environment)){initial.restore_phase=5u;return initial;}
                        requested.recorded_ros_environments.push_back(::std::move(environment));continue;
                    }
                    if(record.kind!=archive::component::wasip1)continue;
                    auto environment=::uwvm2::utils::container::make_shared<wasi::environment>();
                    if(!wasi::decode(decoded.disclosure,record,*environment)) {initial.restore_phase=5u;return initial;}
                    requested.recorded_wasip1_environments.push_back(::std::move(environment));
                }
                for(auto const& binding:command.restore_bindings)
                    requested.wasip1_resource_rebindings.push_back({binding.module,binding.resource,binding.descriptor});
            }
            else if(!command.restore_bindings.empty()) {initial.status=control_error::malformed;return initial;}
            {
                ::std::lock_guard lock{mutex_};::std::size_t count{};
                if(restoration_in_progress_ || !pause_ || native_owned_ || exited_ ||
                   initial.stop_identifier!=stop_identifier_ || command.dump_stop_identifier!=stop_identifier_ ||
                   !source_capture_cohort_locked(initial,initial.threads.front().identifier,captures,count))
                {initial.restore_phase=1u;initial.restore_original_retained=false;return initial;}
                captures.resize(count);ticket=pause_;restoration_in_progress_=true;snapshot_published_=false;
                pending_snapshot_history_=::std::move(history);pending_snapshot_history_label_=requested.recording_label;
                initial.restore_original_retained=false;
            }
            // Never retain mutex_ over worker retirement: native completion and
            // the new before-park callback must acquire this same controller.
            auto result=lib::llvm_jit_checkpoint_prepare_retire_instance_host_api(ticket,captures,requested,deadline);
            {::std::lock_guard lock{mutex_};snapshot_result_=result;snapshot_operation_=result.operation;}
        }
        else
        {
            if(!command.dump_path.empty() || command.dump_stop_identifier || command.restore_recorded || !command.restore_bindings.empty())
            {initial.status=control_error::malformed;return initial;}
            {::std::lock_guard lock{mutex_};
                if(!restoration_in_progress_){initial.restore_phase=1u;return initial;}
                published=snapshot_published_;pending=snapshot_operation_;
            }
            if(!published && pending)
            {
                auto result=lib::llvm_jit_checkpoint_continue_native_retirement_host_api(pending,deadline);
                ::std::lock_guard lock{mutex_};snapshot_result_=result;
            }
        }
        lib::llvm_jit_checkpoint_native_retirement_result result{};
        {::std::lock_guard lock{mutex_};result=snapshot_result_;published=snapshot_published_;}
        if(!published && result.status==state::prepared_world_ready_closed)
        {
            result=lib::llvm_jit_checkpoint_publish_prepared_instance_host_api(result.operation,deadline);
            ::std::lock_guard lock{mutex_};snapshot_result_=result;published=snapshot_published_;
        }
        initial.restore_detail=static_cast<unsigned>(result.status);
        initial.restore_preparation=static_cast<unsigned>(result.preparation.status);
        initial.restore_state_error=static_cast<unsigned>(result.preparation.data_error);
        if(result.status==state::pending_execution || result.status==state::pending_native_join || result.status==state::busy)
        {initial.restore_phase=7u;initial.restore_pending=true;return initial;}
        if(result.status!=state::restored_execution_started)
        {
            initial.restore_phase=6u;initial.restore_applied=published;
            ::std::lock_guard lock{mutex_};
            if(!result.operation && !published)
            {
                restoration_in_progress_=false;snapshot_operation_.reset();
                // This explicit native outcome is issued before retirement.
                // A missing operation token alone is not proof that an unknown
                // or failed publication preserved the original stopped world.
                initial.restore_original_retained=result.status==state::preparation_declined;
            }
            return initial;
        }
        if(!wait_for_stop(deadline,interrupt))
        {initial.restore_phase=8u;initial.restore_applied=true;initial.restore_pending=true;return initial;}
        auto stopped=inspect();
        {
            ::std::lock_guard lock{mutex_};
            if(stopped.execution!=execution_status::stopped || !pause_ || stopped.threads.empty() ||
               stopped.stop_identifier!=stop_identifier_ || exited_ || !snapshot_published_ || !snapshot_source_refresher_)
            {initial.restore_phase=8u;initial.restore_applied=true;initial.restore_pending=true;return initial;}
            snapshot_rebinding_=true;
        }
        auto const rebound=snapshot_source_refresher_(*this);
        {
            ::std::lock_guard lock{mutex_};
            snapshot_rebinding_=false;source_installation_closed_=true;
            // Recheck retained logical breakpoints against the newly published
            // compiler's exact safe-point inventory, never archived native PCs.
            for(auto& point:breakpoints_)
            {
                if(!point.identifier)continue;
                auto sites=lib::llvm_jit_debug_safe_points_host_api(point.module,point.function);
                if(!sites || !sites.contains(point.offset))point={};
            }
            rebuild_wasm_breakpoint_index_locked();snapshot_operation_.reset();restoration_in_progress_=false;
        }
        auto output=inspect();output.restore_applied=true;
        {::std::lock_guard lock{mutex_};output.restore_epoch=snapshot_epoch_;}
        output.restore_phase=rebound?10u:9u;output.status=rebound?control_error::none:control_error::source_debug_info_unavailable;
        return output;
    }
    catch(...)
    {
        ::std::lock_guard lock{mutex_};snapshot_rebinding_=false;
        if(!snapshot_operation_ && !snapshot_published_)restoration_in_progress_=false;
        initial.restore_phase=11u;initial.restore_applied=snapshot_published_;initial.status=control_error::exhausted;return initial;
    }
}
