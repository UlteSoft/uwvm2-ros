// Detached historical DATA for the authenticated debugger. Never a resource
// binding or a restore credential. The same handler serves local/remote hosts.
struct snapshot_history_record
{
    ::std::uint64_t kind{},id{};
    ::uwvm2::utils::container::std_vector<::std::byte> bytes{};
};
::uwvm2::utils::container::std_vector<snapshot_history_record> snapshot_history_{};
::std::array<::std::byte,16u> snapshot_history_label_{};
::std::uint64_t snapshot_history_serial_{};
bool snapshot_history_exhausted_{};
bool snapshot_history_follow_runtime_{true};
::std::uint64_t snapshot_history_event_{},snapshot_history_instruction_{},snapshot_history_runtime_epoch_{};
::std::uint64_t snapshot_history_sample_count_{},snapshot_history_cursor_{};
[[nodiscard]] static auto select_snapshot_history(
    ::uwvm2::runtime::diagnostic::snapshot_archive::decoded const& archive,
    ::std::array<::std::byte,16u> const& label)
{
    namespace format=::uwvm2::runtime::diagnostic::snapshot_archive;
    namespace wasi=::uwvm2::runtime::diagnostic::snapshot_wasip1;
    namespace ros=::uwvm2::runtime::diagnostic::snapshot_ros;
    ::uwvm2::utils::container::std_vector<snapshot_history_record> result{};
    for(auto const& record:archive.records)
    {
        if(record.kind==format::component::wasip1)
        {
            wasi::environment value{};
            if(!wasi::decode(archive.disclosure,record,value) || value.recording!=label)
                throw checkpoint::error::invalid_shape;
        }
        else if(record.kind==format::component::ros1 || record.kind==format::component::ros2)
        {
            ros::environment value{};
            if(!ros::decode(archive.disclosure,record,value) || value.recording!=label)
                throw checkpoint::error::invalid_shape;
        }
        else continue;
        snapshot_history_record row{};row.kind=static_cast<::std::uint64_t>(record.kind);row.id=record.id;
        row.bytes.assign(record.payload.begin(),record.payload.end());result.push_back(::std::move(row));
    }
    return result;
}
void install_snapshot_history_locked(::uwvm2::utils::container::std_vector<snapshot_history_record>&& rows,
    ::std::array<::std::byte,16u> const& label) noexcept
{
    // Tokens never wrap or become valid again for another archive. An exhausted
    // debugger session can still run; only history transfer becomes unavailable.
    if(snapshot_history_exhausted_ || snapshot_history_serial_==UINT64_MAX)
    {snapshot_history_exhausted_=true;snapshot_history_.clear();return;}
    snapshot_history_=::std::move(rows);snapshot_history_label_=label;++snapshot_history_serial_;
    snapshot_history_follow_runtime_=true;
    snapshot_history_event_=snapshot_history_instruction_=snapshot_history_runtime_epoch_=0u;
    snapshot_history_sample_count_=snapshot_history_cursor_=0u;
}
static void append_history_hex(::uwvm2::utils::container::std_string& text,::std::span<::std::byte const> bytes)
{
    constexpr char digits[]{"0123456789abcdef"};
    for(auto byte:bytes){auto value=::std::to_integer<unsigned>(byte);text.push_back(digits[value>>4u]);text.push_back(digits[value&15u]);}
}
[[nodiscard]] controller_reply inspect_snapshot_history(console_command const& command,controller_reply reply)
{
    namespace permissions=::uwvm2::utils::control::debug_permissions;
    namespace dump=::uwvm2::utils::control::dump_permissions;
    reply.status=control_error::unavailable_capability;
    if(!permissions::inspection_allowed() || dump::permit(dump::disclosure::full).status!=dump::denial::none)return reply;
    ::std::uint64_t selected_serial{};
    if(command.recording_action==7u)
    {
        try
        {
            namespace archive=::uwvm2::runtime::diagnostic::snapshot_archive;
            namespace snapshot=::uwvm2::runtime::diagnostic::snapshot_state;
            ::uwvm2::utils::container::std_vector<::std::byte> input{};archive::decoded saved{};
            ::std::shared_ptr<checkpoint::state const> graph{};
            if(!read_snapshot_file(command.dump_path,input) || !archive::decode(input,saved))return reply;
            checkpoint::execution_replay::decoded journal{};
            ::std::array<::std::byte,16> threaded_label{};
            ::std::uint64_t threaded_instruction{};
            ::std::uint64_t threaded_samples{},threaded_cursor{};
            bool threaded{};
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
            // decoded records are spans. Keep the nested sample owner alive
            // through typed validation and the owning history-row copy below;
            // a block-local history would free those spans at the closing if.
            ::uwvm2::runtime::wasm_threads::threaded_snapshot threaded_owner;
            if(saved.origin.logical_abi==7u)
            {
                namespace threads=::uwvm2::runtime::wasm_threads;
                auto& history=threaded_owner;
                if(!threads::decode_threaded_snapshot(input,history) || command.history_id>=history.environments.size())return reply;
                auto const& sample=history.environments[static_cast<::std::size_t>(command.history_id)];
                threaded_samples=history.environments.size();threaded_cursor=sample.cursor;
                if(!archive::decode(sample.archive,saved))return reply;
                threaded_label=threads::threaded_history_label(history.history);threaded=true;
                for(::std::size_t i{};i<sample.cursor;++i)
                    threaded_instruction+=history.history.events[i].kind==threads::journal_kind::instruction;
            }
#endif
            bool const pending=::std::any_of(saved.records.begin(),saved.records.end(),
                [](auto const& row){return row.kind==archive::component::retained_native;});
            if(threaded){}
            else if(pending)
            {
                if(!snapshot::inspect_pending_history(saved,journal))return reply;
                // The inspected journal already owns the complete origin. History
                // only needs its label/environment; do not clone linear memories.
            }
            else
            {
                auto decoded=snapshot::decode(saved,graph,{},true);
                if(!decoded)
                {
                    // A plain full snapshot has no event log. A journal must pass
                    // the stricter replay decoder; do not relax either format.
                    if(decoded.status!=snapshot::error::unsupported_component ||
                       decoded.state_error!=checkpoint::error::non_replayable_import ||
                       !snapshot::decode(saved,graph,{},true,true))return reply;
                }
            }
            auto label=threaded?threaded_label:pending?journal.origin.recording_id:graph->recording_id;
            ::std::uint64_t instruction{threaded_instruction};
            // File DATA is decoded and checked without any runtime preparation,
            // current permission transfer, provider lookup or resource binding.
            // The optional event must be an actual coherent environment sample.
            if(command.history_id && !threaded)
            {
                if((!pending && checkpoint::execution_replay::decode(*graph,journal)!=checkpoint::error::none) ||
                   command.history_id>journal.events.size())return reply;
                auto const& event=journal.events[static_cast<::std::size_t>(command.history_id-1u)];
                if(event.flags!=checkpoint::execution_replay::environment_kind || !archive::decode(event.bytes,saved))return reply;
                instruction=event.words[1];
            }
            auto selected=select_snapshot_history(saved,label);
            ::std::lock_guard lock{mutex_};
            if(restoration_in_progress_ || snapshot_history_exhausted_)return reply;
            install_snapshot_history_locked(::std::move(selected),label);
            snapshot_history_follow_runtime_=false;snapshot_history_event_=command.history_id;
            snapshot_history_instruction_=instruction;
            snapshot_history_sample_count_=threaded_samples;snapshot_history_cursor_=threaded_cursor;
            selected_serial=snapshot_history_serial_;
        }
        catch(...){reply.status=control_error::invalid_state;return reply;}
    }
    bool follow_runtime{};
    {
        ::std::lock_guard lock{mutex_};
        if(restoration_in_progress_ || snapshot_history_exhausted_ || !snapshot_history_serial_)return reply;
        if(selected_serial && selected_serial!=snapshot_history_serial_)return reply;
        selected_serial=snapshot_history_serial_;
        follow_runtime=snapshot_history_follow_runtime_;
    }
    // A detached file selection must not acquire the live journal lock or copy
    // its environment on every page. Recheck selection under mutex below if a
    // concurrent command replaced it while the live observation was copied.
    auto observed=follow_runtime?::uwvm2::runtime::lib::llvm_jit_checkpoint_replay_history_host_api():
        ::uwvm2::runtime::lib::llvm_jit_checkpoint_replay_history{};
    ::std::lock_guard lock{mutex_};
    if(restoration_in_progress_ || snapshot_history_exhausted_ || !snapshot_history_serial_ ||
       selected_serial!=snapshot_history_serial_)return reply;
    try
    {
        if(snapshot_history_follow_runtime_ && observed.sequence && (observed.epoch!=snapshot_history_runtime_epoch_ || observed.sequence!=snapshot_history_event_))
        {
            namespace archive=::uwvm2::runtime::diagnostic::snapshot_archive;
            archive::decoded saved{};if(!archive::decode(observed.archive,saved))return reply;
            auto records=select_snapshot_history(saved,snapshot_history_label_);
            install_snapshot_history_locked(::std::move(records),snapshot_history_label_);
            if(snapshot_history_exhausted_)return reply;
            snapshot_history_runtime_epoch_=observed.epoch;snapshot_history_event_=observed.sequence;
            snapshot_history_instruction_=observed.instruction;
        }
        if(command.recording_action==4u || command.recording_action==7u)
        {
            auto& text=reply.historical_environment;
            text=::uwvm2::utils::container::concat("snapshot-history serial=",snapshot_history_serial_,
                " position=",::fast_io::mnp::os_c_str(snapshot_history_event_?"checkpoint":"origin"),
                " event=",snapshot_history_event_," instruction=",snapshot_history_instruction_," recording=");
            append_history_hex(text,snapshot_history_label_);
            if(snapshot_history_sample_count_)text+=::uwvm2::utils::container::concat(" samples=",snapshot_history_sample_count_," cursor=",snapshot_history_cursor_);
            text+="; historical data, no live resource access\n";
            for(auto const& row:snapshot_history_)
                text+=::uwvm2::utils::container::concat("environment kind=",row.kind," id=",row.id," bytes=",row.bytes.size(),"\n");
        }
        else
        {
            if(command.recording_action!=5u || command.history_serial!=snapshot_history_serial_ ||
               !command.history_count || command.history_count>4096u)return reply;
            auto found=::std::find_if(snapshot_history_.begin(),snapshot_history_.end(),[&](auto const& row)
                {return row.kind==command.history_kind && row.id==command.history_id;});
            if(found==snapshot_history_.end() || command.history_offset>found->bytes.size() ||
               command.history_count>found->bytes.size()-command.history_offset)return reply;
            auto& text=reply.historical_environment;
            text=::uwvm2::utils::container::concat("snapshot-history-bytes serial=",snapshot_history_serial_,
                " kind=",found->kind," id=",found->id," offset=",command.history_offset," count=",command.history_count," hex=");
            append_history_hex(text,{found->bytes.data()+static_cast<::std::size_t>(command.history_offset),
                static_cast<::std::size_t>(command.history_count)});text+="\n";
        }
        reply.status=control_error::none;
    }
    catch(...){reply.historical_environment.clear();reply.status=control_error::invalid_state;}
    return reply;
}
