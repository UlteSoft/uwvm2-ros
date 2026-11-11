// Included inside controller. Only the authenticated full-permission manager reaches here.
// Software value watchpoints observe real complete stops, never stale snapshots.
#pragma once
        [[nodiscard]] bool sample_watchpoint_locked(data_watchpoint& point, controller_reply const& stopped,
            ::std::vector<::std::byte>& bytes, ::std::string& error)
        {
            if(!pause_ || stopped.stop_identifier!=stop_identifier_ || exited_ || domain_->is_closed())
            { error="current complete stop required";return false; }
            if(point.memory)
            {
                if(native_owned_ && !native_external_parked_) { error="native worker is running";return false; }
                auto const& selection{point.query.wasm_state_request};
                if(selection.index>UINT32_MAX || selection.count>SIZE_MAX || selection.count>UINT64_MAX-selection.first ||
                   read_memory_==nullptr || !read_memory_(selection.module,static_cast<::std::uint32_t>(selection.index),
                    selection.first+selection.count,nullptr,0u))
                { error="Wasm memory range is unavailable";return false; }
                bytes.resize(static_cast<::std::size_t>(selection.count));
                for(::std::size_t offset{};offset!=bytes.size();)
                {
                    auto const size{(::std::min)(::std::size_t{65536u},bytes.size()-offset)};
                    if(!read_memory_(selection.module,static_cast<::std::uint32_t>(selection.index),
                       selection.first+offset,bytes.data()+offset,size))
                    { error="Wasm memory read failed";return false; }
                    offset+=size;
                }
                return true;
            }
            if(native_owned_) { error="typed watch requires a cooperative Wasm stop";return false; }
            auto query{point.query};controller_reply copied{};
            copied.execution=execution_status::stopped;copied.stop_identifier=stop_identifier_;
            copied.threads=stopped.threads;
            auto const participant{query.kind==console_command_kind::wasm_state ? query.wasm_state_request.participant : query.requested_step_thread};
            auto const record{::std::find_if(traces_.begin(),traces_.end(),
                [&](auto const& item){return item.participant==participant;})};
            if(record==traces_.end()) { error="watched participant retired";return false; }
            bool const scoped{query.kind==console_command_kind::source_value ||
                wasm_state::frame_selection(query.wasm_state_request.selected)};
            if(scoped)
            {
                ::uwvm2::runtime::lib::llvm_jit_debug_activation_snapshot actual{};
                if(!::uwvm2::runtime::lib::llvm_jit_debug_query_activation_host_api(record->activation_capture,actual) ||
                   actual.frames.empty()) { error="current activation unavailable";return false; }
                auto frame{query.kind==console_command_kind::wasm_state ? query.wasm_state_request.frame : 0u};
                if(!point.incarnation)
                {
                    if(frame>=actual.frames.size()) { error="frame out of range";return false; }
                    point.incarnation=actual.frames[actual.frames.size()-1u-static_cast<::std::size_t>(frame)].incarnation;
                }
                auto const found{::std::find_if(actual.frames.begin(),actual.frames.end(),
                    [&](auto const& item){return item.incarnation==point.incarnation;})};
                if(found==actual.frames.end()) { error="watched activation retired";point.enabled=false;return false; }
                frame=static_cast<::std::uint64_t>(actual.frames.end()-found-1);
                if(query.kind==console_command_kind::wasm_state) { query.wasm_state_request.frame=frame; }
                else
                {
                    console_command frames{};frames.kind=console_command_kind::source_frame;
                    frames.frame_action=source_frame_action::list;frames.requested_step_thread=participant;
                    frames.disassembly_stop_identifier=stop_identifier_;frames.frame_page_explicit=true;frames.frame_page_count=128u;
                    bool selected{};
                    for(;;)
                    {
                        controller_reply page{};page.threads=stopped.threads;page.stop_identifier=stop_identifier_;
                        source_frames_locked(*record,frames,page);
                        if(page.status!=control_error::none || !page.source_frames_available) { break; }
                        for(::std::size_t i{};i!=page.source_frames.size();++i)
                        {
                            if(page.source_frames[i].incarnation==point.incarnation)
                            { query.source_frame_ordinal=page.source_frame_first+i;selected=true;break; }
                        }
                        if(selected || page.source_frame_first+page.source_frames.size()>=page.source_frame_total || page.source_frames.empty()) { break; }
                        frames.frame_page_first=page.source_frame_first+page.source_frames.size();
                    }
                    if(!selected) { error="watched source frame unavailable";return false; }
                }
            }
            if(query.kind==console_command_kind::wasm_state)
            {
                query_wasm_state_locked(query.wasm_state_request,copied);
                auto const& values{copied.wasm_state_values};
                if(values.result!=wasm_state::status::available || values.rows.empty() ||
                   query.wasm_state_request.first>values.total_values ||
                   values.rows.size()!=(::std::min)(query.wasm_state_request.count,
                       values.total_values-query.wasm_state_request.first))
                { error="typed watched value unavailable";return false; }
                auto const* rows{::std::addressof(values.rows)};
                if(query.wasm_state_request.member_page())
                {
                    auto const object{values.selected_object};
                    if(object==0u || object>values.objects.size())
                    { error="watched GC object unavailable";return false; }
                    auto const& selected{values.objects[static_cast<::std::size_t>(object)-1u]};
                    auto const first{query.wasm_state_request.member_first};
                    auto const count{query.wasm_state_request.member_count};
                    if((selected.kind!=wasm_state::object_kind::structure && selected.kind!=wasm_state::object_kind::array) ||
                       selected.first_member!=first || first>selected.total_members ||
                       count>selected.total_members-first || selected.members.size()!=count)
                    { error="complete watched GC member range unavailable";return false; }
                    rows=::std::addressof(selected.members);
                }
                for(auto const& row:*rows)
                {
                    if(!row.data.available || row.data.type.kind==wasm_state::value_kind::reference)
                    { error="watch requires complete numeric/vector values or a linear-memory range";return false; }
                    bytes.push_back(static_cast<::std::byte>(row.data.type.kind));
                    bytes.insert(bytes.end(),row.data.bits.begin(),row.data.bits.end());
                }
                return true;
            }
            query.disassembly_stop_identifier=stop_identifier_;
            query.source_frame_explicit=true;
            source_type_locked(*record,query,copied);
            if(copied.status!=control_error::none || !copied.source_object_value_available ||
               copied.source_object_type.size()!=1u || !copied.source_object_type.front().value_available)
            { error="source watch requires an available scalar expression";return false; }
            auto const& scalar{copied.source_object_type.front()};
            bytes.push_back(static_cast<::std::byte>(scalar.scalar_kind));
            for(unsigned i{};i!=8u;++i) { bytes.push_back(static_cast<::std::byte>((scalar.bits>>(8u*i))&0xffu)); }
            return true;
        }
        [[nodiscard]] controller_reply watch_command_locked(console_command const& command, controller_reply result)
        {
            refresh_observation_hints_on_return refresh{*this};
            result.watchpoint_identifier=0u;result.watchpoint_error.clear();
            if(command.operation!=operation::status || command.payload_size || command.wait_for_event ||
               command.run_from_entry || command.launch_action!=console_launch_action::none)
            { result.status=control_error::malformed;return result; }
            try
            {
                if(command.watchpoint_policy==watchpoint_action::list)
                {
                    auto const first{command.wasm_state_request.first};
                    if(first>watchpoints_.size()) { result.status=control_error::invalid_state;return result; }
                    auto const count{(::std::min)(command.wasm_state_request.count,static_cast<::std::uint64_t>(watchpoints_.size())-first)};
                    result.watchpoint_next=first+count;result.watchpoint_total=watchpoints_.size();
                    result.watchpoints.reserve(static_cast<::std::size_t>(count));
                    for(::std::uint64_t i{};i!=count;++i)
                    {
                        auto const& point{watchpoints_[static_cast<::std::size_t>(first+i)]};
                        data_watchpoint display{};display.identifier=point.identifier;display.hits=point.hits;
                        display.enabled=point.enabled;display.memory=point.memory;display.description=point.description;
                        result.watchpoints.push_back(::std::move(display));
                    }
                    return result;
                }
                if(command.watchpoint_policy==watchpoint_action::read_old || command.watchpoint_policy==watchpoint_action::read_new)
                {
                    auto const& value{command.watchpoint_policy==watchpoint_action::read_old ? watch_before_:watch_after_};
                    auto const first{command.wasm_state_request.first};
                    if(result.execution!=execution_status::stopped || !pause_ || watch_hit_!=command.watchpoint_id ||
                       first>value.size() || !command.wasm_state_request.count || command.wasm_state_request.count>256u)
                    { result.status=control_error::invalid_state;return result; }
                    auto const count{(::std::min)(command.wasm_state_request.count,static_cast<::std::uint64_t>(value.size())-first)};
                    result.watchpoint_identifier=watch_hit_;result.watchpoint_value_first=first;result.watchpoint_value_total=value.size();
                    result.watchpoint_before.assign(value.begin()+static_cast<::std::size_t>(first),value.begin()+static_cast<::std::size_t>(first+count));
                    result.watchpoint_after.clear();return result;
                }
                if(command.watchpoint_policy!=watchpoint_action::create)
                {
                    auto const point{::std::find_if(watchpoints_.begin(),watchpoints_.end(),
                        [&](auto const& item){return item.identifier==command.watchpoint_id;})};
                    if(point==watchpoints_.end()) { result.status=control_error::invalid_state;return result; }
                    if(command.watchpoint_policy==watchpoint_action::erase) { watchpoints_.erase(point); }
                    else if(command.watchpoint_policy==watchpoint_action::disable) { point->enabled=false; }
                    else if(command.watchpoint_policy==watchpoint_action::enable)
                    {
                        if(result.execution!=execution_status::stopped) { result.status=control_error::invalid_state;return result; }
                        ::std::vector<::std::byte> value;::std::string error;
                        if(!sample_watchpoint_locked(*point,result,value,error))
                        { result.watchpoint_error=::std::move(error);result.status=control_error::invalid_state;return result; }
                        point->value=::std::move(value);point->enabled=true;
                    }
                    else { result.status=control_error::malformed; }
                    return result;
                }
                if(result.execution!=execution_status::stopped || !next_watchpoint_ ||
                   result.stop_identifier!=stop_identifier_ || !pause_)
                { result.status=control_error::invalid_state;return result; }
                data_watchpoint point{};point.memory=command.watchpoint_memory;
                if(point.memory)
                {
                    point.query=command;
                    point.description=::fast_io::concat_std("memory module=",command.wasm_state_request.module,
                        " memory=",command.wasm_state_request.index," offset=",command.wasm_state_request.first,
                        " bytes=",command.wasm_state_request.count);
                }
                else
                {
                    if(!command.script_size || command.script_size>command.script_text.size())
                    { result.status=control_error::malformed;return result; }
                    point.query=parse_console_command(::fast_io::string_view{command.script_text.data(),command.script_size});
                    auto& query{point.query};
                    if(query.kind==console_command_kind::invalid)
                    {
                        auto const text{::fast_io::concat_fast_io("print ",::fast_io::string_view{command.script_text.data(),command.script_size})};
                        query=parse_console_command(::fast_io::string_view{text.data(),text.size()});
                    }
                    if(query.kind==console_command_kind::wasm_path)
                    {
                        if(query.wasm_path_request.action!=wasm_path::operation::members)
                        { result.status=control_error::unsupported_command;return result; }
                        // Resolve the original root/path at this exact real stop.
                        // The watch keeps copied indices, not a live GC pointer or
                        // a reusable ledger capability. Every future sample again
                        // obtains a fresh complete cohort and canonical root borrow.
                        controller_reply copied{};copied.execution=result.execution;
                        copied.stop_identifier=result.stop_identifier;copied.threads=result.threads;
                        query_wasm_path_locked(query.wasm_path_request,copied);
                        if(copied.wasm_path_values.result!=wasm_state::status::available)
                        { result.status=control_error::invalid_state;result.watchpoint_error="current GC member path required";return result; }
                        query.kind=console_command_kind::wasm_state;
                        query.wasm_state_request=::std::move(copied.wasm_state_values.requested);
                    }
                    if(query.kind!=console_command_kind::wasm_state && query.kind!=console_command_kind::source_value)
                    { result.status=control_error::unsupported_command;return result; }
                    if(query.kind==console_command_kind::wasm_state &&
                       !wasm_state::value_selection(query.wasm_state_request.selected))
                    { result.status=control_error::unsupported_command;return result; }
                    if(query.kind==console_command_kind::source_value && !query.requested_step_thread && result.threads.size()==1u)
                    { query.requested_step_thread=result.threads.front().identifier; }
                    point.description.assign(command.script_text.data(),command.script_size);
                }
                ::std::string error;
                if(!sample_watchpoint_locked(point,result,point.value,error))
                { result.status=control_error::invalid_state;result.watchpoint_error=::std::move(error);return result; }
                point.identifier=next_watchpoint_;watchpoints_.push_back(::std::move(point));
                result.watchpoint_identifier=next_watchpoint_++;watch_checked_stop_=stop_identifier_;
                return result;
            }
            catch(...) { result.status=control_error::exhausted;return result; }
        }
        void rebase_watchpoints_locked(controller_reply const& stopped) noexcept
        {
            if(!watches_active_locked()) { return; }
            try
            {
                for(auto& point:watchpoints_)
                {
                    if(!point.enabled) { continue; }
                    ::std::vector<::std::byte> value;::std::string error;
                    if(sample_watchpoint_locked(point,stopped,value,error)) { point.value=::std::move(value); }
                    else { watch_error_=::std::move(error); }
                }
                watch_checked_stop_=stop_identifier_;
            }
            catch(...)
            {
                for(auto& point:watchpoints_) { point.enabled=false; }
                condition_error_=control_error::exhausted;
            }
        }
        [[nodiscard]] bool filter_watchpoints_locked()
        {
            if(!watches_active_locked() || !pause_ || watch_checked_stop_==stop_identifier_) { return true; }
            auto const cohort{domain_->capture(pause_)};
            if(cohort.result!=::uwvm2::utils::thread::cooperative_pause_result::paused) { return true; }
            refresh_observation_hints_on_return refresh{*this};
            watch_checked_stop_=stop_identifier_;watch_hit_=0u;
            watch_before_.clear();watch_after_.clear();watch_error_.clear();
            try
            {
                controller_reply stopped{};stopped.execution=execution_status::stopped;stopped.stop_identifier=stop_identifier_;
                for(auto const& member:cohort.participants) { stopped.threads.push_back({member.id,member.location,{}}); }
                for(auto& point:watchpoints_)
                {
                    if(!point.enabled) { continue; }
                    ::std::vector<::std::byte> current;::std::string error;
                    if(!sample_watchpoint_locked(point,stopped,current,error))
                    { watch_error_=::std::move(error);watch_hit_=point.identifier;break; }
                    if(current!=point.value)
                    {
                        if(point.hits!=UINT64_MAX) { ++point.hits; }
                        if(!watch_hit_)
                        { watch_hit_=point.identifier;watch_before_=point.value;watch_after_=current; }
                        point.value=::std::move(current);
                    }
                }
                if(watch_hit_)
                { reason_=stop_reason::watchpoint;cancel_step_locked();return true; }
                if(reason_==stop_reason::watchpoint_probe)
                {
                    auto const retired{stop_identifier_};
                    if(resume_locked()) { condition_resumed_stop_=retired;return false; }
                    watch_error_="could not retire watch probe";reason_=stop_reason::watchpoint;
                }
                return true;
            }
            catch(...)
            { watch_error_="watchpoint allocation failed";reason_=stop_reason::watchpoint;cancel_step_locked();return true; }
        }
