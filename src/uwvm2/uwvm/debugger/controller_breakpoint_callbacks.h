// Host command callbacks, never Wasm/host-function pointers or a shell.
// Included inside controller. Complete-list admission precedes every install.
#pragma once
        [[nodiscard]] static bool admitted_callback(console_command const& command) noexcept
        {
            if(command.wait_for_event || command.run_from_entry || command.launch_action!=console_launch_action::none)
            { return false; }
            switch(command.kind)
            {
                case console_command_kind::callback_echo:case console_command_kind::wasm_state:
                case console_command_kind::wasm_mutation:case console_command_kind::source_value:
                case console_command_kind::assembly_registers:return true;
                case console_command_kind::breakpoint_control:
                    return command.breakpoint_policy==breakpoint_action::enable || command.breakpoint_policy==breakpoint_action::disable;
                case console_command_kind::protocol:return command.operation==operation::resume;
                default:return false;
            }
        }
        [[nodiscard]] controller_reply breakpoint_commands_locked(console_command const& command,controller_reply result)
        {
            if(command.operation!=operation::status || command.payload_size || command.wait_for_event ||
               command.run_from_entry || command.launch_action!=console_launch_action::none ||
               command.script_size>command.script_text.size())
            { result.status=control_error::malformed;return result; }
            try
            {
                if(!command.breakpoint_id)
                {
                    auto first{command.breakpoint_ignore};auto offset{command.wasm_state_request.first};
                    auto const budget{command.wasm_state_request.index};
                    if(first>callback_history_.size() || budget<8u || budget>32768u || (first==callback_history_.size() && offset))
                    { result.status=control_error::invalid_state;return result; }
                    auto const count{(::std::min)(command.wasm_state_request.count,
                        static_cast<::std::uint64_t>(callback_history_.size())-first)};
                    result.callback_next=first;result.callback_next_byte=offset;result.callback_total=callback_history_.size();
                    for(::std::uint64_t i{};i!=count;++i)
                    {
                        auto const& row{callback_history_[static_cast<::std::size_t>(first+i)]};
                        auto const header{::fast_io::concat_std("callback ",first+i," breakpoint=",row.breakpoint," stop=",row.stop,
                            " status=",static_cast<unsigned>(row.status),"\n")};
                        auto const total{header.size()+row.text.size()};
                        auto byte_at{[&](::std::size_t index)->char { return index<header.size() ? header[index]:row.text[index-header.size()]; }};
                        if(offset>total || (offset<total && (static_cast<unsigned char>(byte_at(static_cast<::std::size_t>(offset)))&0xc0u)==0x80u))
                        { result.status=control_error::invalid_state;return result; }
                        auto end{static_cast<::std::size_t>(offset)+(::std::min)(total-static_cast<::std::size_t>(offset),
                            static_cast<::std::size_t>(budget)-result.callback_output.size())};
                        while(end>offset && end<total && (static_cast<unsigned char>(byte_at(end))&0xc0u)==0x80u) { --end; }
                        for(auto cursor{static_cast<::std::size_t>(offset)};cursor!=end;++cursor) { result.callback_output.push_back(byte_at(cursor)); }
                        if(end!=total) { result.callback_next=first+i;result.callback_next_byte=end;break; }
                        result.callback_next=first+i+1u;result.callback_next_byte=0u;offset=0u;
                        if(result.callback_output.size()==budget) { break; }
                    }
                    return result;
                }
                auto const point{::std::find_if(breakpoints_.begin(),breakpoints_.end(),
                    [&](auto const& value){return value.identifier==command.breakpoint_id;})};
                if(point==breakpoints_.end()) { result.status=control_error::invalid_breakpoint_target;return result; }
                ::fast_io::string_view text{command.script_text.data(),command.script_size};
                if(text=="list" || text.starts_with("list "))
                {
                    ::std::uint64_t first{},count{16u};
                    auto cursor{details::skip_command_space(text,4u)};
                    if(cursor<text.size())
                    {
                        auto const end{details::command_word_end(text,cursor)};
                        if(!details::parse_decimal(text.subview(cursor,end-cursor),first)) { result.status=control_error::malformed;return result; }
                        cursor=details::skip_command_space(text,end);
                        if(cursor<text.size() && !details::parse_decimal(text.subview(cursor),count)) { result.status=control_error::malformed;return result; }
                    }
                    if(first>point->callback_scripts.size() || !count || count>32u) { result.status=control_error::invalid_state;return result; }
                    count=(::std::min)(count,static_cast<::std::uint64_t>(point->callback_scripts.size())-first);
                    ::fast_io::ostring_ref_std output{::std::addressof(result.callback_output)};
                    ::fast_io::io::print(output,"breakpoint ",point->identifier," callback commands=",point->callbacks.size(),"\n");
                    for(::std::uint64_t i{};i!=count;++i)
                    { ::fast_io::io::print(output,first+i,": ",::std::string_view{point->callback_scripts[static_cast<::std::size_t>(first+i)]},"\n"); }
                    ::fast_io::io::print(output,"callback-command-next=",first+count," total=",point->callback_scripts.size(),"\n");
                    return result;
                }
                if(text.empty() || text=="clear") { point->callbacks.clear();point->callback_scripts.clear();return result; }
                bool const append{text.starts_with("append ")};
                if(append) { text=text.subview(7u); }
                auto next{append ? point->callbacks : ::std::vector<console_command>{}};
                auto scripts{append ? point->callback_scripts : ::std::vector<::std::string>{}};
                ::std::size_t begin{};
                for(::std::size_t i{};i<=text.size();++i)
                {
                    if(i!=text.size() && text[i]!=';') { continue; }
                    auto const item{parse_console_command(text.subview(begin,i-begin))};
                    if(!admitted_callback(item) || (!next.empty() && next.back().operation==operation::resume))
                    { result.status=control_error::unsupported_command;return result; }
                    scripts.emplace_back(text.data()+begin,i-begin);next.push_back(item);if(i!=text.size()) { begin=i+1u; }
                }
                point->callbacks=::std::move(next);point->callback_scripts=::std::move(scripts);return result;
            }
            catch(...) { result.status=control_error::exhausted;return result; }
        }
        [[nodiscard]] control_error evaluate_breakpoint_locked(breakpoint const& point,controller_reply const& stopped,bool& matches)
        {
            matches=false;
            if(!point.condition_size) { matches=true;return control_error::none; }
            if(point.condition_size>point.condition.size()) { return control_error::malformed; }
            auto expression{::fast_io::string_view{point.condition.data(),point.condition_size}};
            // Literal truth needs no guessed DWARF scope, also for pure WAT.
            if(expression=="true" || expression=="1") { matches=true;return control_error::none; }
            if(expression=="false" || expression=="0") { return control_error::none; }
            auto const trace{::std::find_if(traces_.begin(),traces_.end(),
                [&](auto const& item){return item.participant==pending_breakpoint_participant_;})};
            if(trace==traces_.end()) { return control_error::source_location_unmapped; }
            console_command query{};query.kind=console_command_kind::source_value;
            query.requested_step_thread=trace->participant;query.disassembly_stop_identifier=stop_identifier_;
            query.source_frame_explicit=true;query.source_frame_ordinal=0u;
            auto const text{::fast_io::concat_fast_io("(",expression,") != false")};
            if(text.size()>query.source_variable_name.size()) { return control_error::exhausted; }
            query.source_variable_name_size=text.size();
            for(::std::size_t i{};i!=text.size();++i) { query.source_variable_name[i]=text[i]; }
            controller_reply copied{};copied.execution=execution_status::stopped;
            copied.stop_identifier=stop_identifier_;copied.threads=stopped.threads;
            source_type_locked(*trace,query,copied);
            if(copied.status!=control_error::none || !copied.source_object_value_available ||
               copied.source_object_type.size()!=1u || !copied.source_object_type.front().value_available ||
               (copied.source_object_type.front().scalar_kind!=source_dwarf::numeric_kind::signed_integer &&
                copied.source_object_type.front().scalar_kind!=source_dwarf::numeric_kind::boolean))
            { return copied.status==control_error::none ? control_error::source_location_unmapped:copied.status; }
            matches=copied.source_object_type.front().bits!=0u;return control_error::none;
        }
        [[nodiscard]] bool run_breakpoint_callbacks_locked(::std::uint64_t identifier,
            ::std::vector<console_command> const& commands,controller_reply& stopped,bool& continuing)
        {
            continuing=false;
            if(commands.empty()) { return true; }
            callback_record event{};event.breakpoint=identifier;event.stop=stop_identifier_;
            bool success{true};::fast_io::ostring_ref_std output{::std::addressof(event.text)};
            // Reserve the durable result first. Allocation failure cannot run
            // a command without retaining its failure/success history.
            callback_history_.reserve(callback_history_.size()+1u);
            try
            {
            for(auto command:commands)
            {
                if(!::uwvm2::utils::control::management::debug_allowed() || !pause_ || exited_ || domain_->is_closed() ||
                   !admitted_callback(command) || domain_->capture(pause_).result!=
                        ::uwvm2::utils::thread::cooperative_pause_result::paused)
                { success=false;event.status=control_error::invalid_state;break; }
                if(command.operation==operation::resume) { continuing=true;break; }
                controller_reply reply{};reply.execution=execution_status::stopped;reply.stop_identifier=stop_identifier_;
                reply.threads=stopped.threads;
                if(command.kind==console_command_kind::callback_echo)
                { ::fast_io::io::print(output,::fast_io::string_view{command.script_text.data(),command.script_size},"\n"); }
                else if(command.kind==console_command_kind::breakpoint_control)
                {
                    bool found{};
                    for(auto& point:breakpoints_)
                    {
                        if(!point.identifier || (command.breakpoint_id && point.identifier!=command.breakpoint_id)) { continue; }
                        point.enabled=command.breakpoint_policy==breakpoint_action::enable;found=true;
                    }
                    success=found || !command.breakpoint_id;rebuild_wasm_breakpoint_index_locked();
                    ::fast_io::io::print(output,"breakpoint enable policy updated\n");
                }
                else if(command.kind==console_command_kind::wasm_state)
                {
                    query_wasm_state_locked(command.wasm_state_request,reply);
                    ::fast_io::io::print(output,::std::string_view{wasm_state::format(reply.wasm_state_values)});
                    success=reply.wasm_state_values.result==wasm_state::status::available;
                }
                else if(command.kind==console_command_kind::wasm_mutation)
                {
                    mutate_wasm_state_locked(command.wasm_mutation_request,reply);
                    ::fast_io::io::print(output,::std::string_view{wasm_mutation::format(reply.wasm_mutation_value)});
                    success=reply.wasm_mutation_value.applied;
                    stopped.stop_identifier=stop_identifier_;condition_checked_stop_=stop_identifier_;
                }
                else if(command.kind==console_command_kind::assembly_registers)
                {
                    command.disassembly_stop_identifier=stop_identifier_;
                    if(!command.requested_step_thread && reply.threads.size()==1u) { command.requested_step_thread=reply.threads.front().identifier; }
                    registers_locked(command,reply);
                    success=reply.status==control_error::none;
                    ::fast_io::io::print(output,"register snapshot status=",static_cast<unsigned>(reply.status),"\n");
                    if(success)
                    {
                        for(::std::size_t i{};i!=reply.registers.size();++i)
                        {
                            auto const name{native_registers::name(reply.registers.machine,i)};
                            if(name==nullptr) { continue; }
                            ::fast_io::io::print(output,::fast_io::mnp::code_cvt(::fast_io::mnp::os_c_str(name)),"=");
                            if(!reply.registers.known_bits[i]) { ::fast_io::io::print(output,"unavailable\n"); }
                            else { ::fast_io::io::print(output,"0x",::fast_io::mnp::hex<false,true>(reply.registers.values[i]),
                                " known-bits=",reply.registers.known_bits[i],"\n"); }
                        }
                        for(::std::size_t i{};i!=native_registers::max_fp_registers;++i)
                        {
                            auto const name{native_registers::fp_name(reply.registers.machine,i)};
                            auto const& value{reply.registers.floating.values[i]};
                            if(name==nullptr || !value.width) { continue; }
                            ::fast_io::io::print(output,::fast_io::mnp::code_cvt(::fast_io::mnp::os_c_str(name)),"=0x");
                            for(::std::size_t j{value.width};j!=0u;--j)
                            { ::fast_io::io::print(output,::fast_io::mnp::hex<false,true>(value.bytes[j-1u])); }
                            ::fast_io::io::print(output,"\n");
                        }
                    }
                }
                else
                {
                    if(!command.requested_step_thread && reply.threads.size()==1u) { command.requested_step_thread=reply.threads.front().identifier; }
                    command.disassembly_stop_identifier=stop_identifier_;command.source_frame_explicit=true;command.source_frame_ordinal=0u;
                    auto const record{::std::find_if(traces_.begin(),traces_.end(),
                        [&](auto const& item){return item.participant==command.requested_step_thread;})};
                    if(record==traces_.end()) { success=false; }
                    else
                    {
                        source_type_locked(*record,command,reply);
                        success=reply.status==control_error::none && reply.source_object_value_available &&
                            reply.source_object_type.size()==1u && reply.source_object_type.front().value_available;
                        if(success) { ::fast_io::io::print(output,"source value bits=0x",
                            ::fast_io::mnp::hex<false,true>(reply.source_object_type.front().bits),"\n"); }
                    }
                }
                if(!success) { event.status=control_error::invalid_state;break; }
            }
            }
            catch(...) { success=false;event.status=control_error::exhausted; }
            if(!success) { continuing=false;::fast_io::io::print(output,"callback failed; execution remains stopped\n"); }
            callback_history_.push_back(::std::move(event));return success;
        }
