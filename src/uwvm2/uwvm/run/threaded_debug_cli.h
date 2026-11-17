// Included in run's namespace, after its actual source/entry helpers.
// The transport supplies authenticated HOST commands, never Guest imports.
#if UWVM2_HAS_THREADED_PROFILE_HOST_API && defined(UWVM_RUNTIME_LLVM_JIT)
    class threaded_debug_cli final
    {
        using text=::uwvm2::utils::container::std_string;
        using bytes=::uwvm2::utils::container::std_vector<::std::byte>;
        using clock=::std::chrono::steady_clock;
        using worker=::uwvm2::runtime::wasm_threads::debug_stopped_worker;
        ::uwvm2::runtime::wasm_threads::threaded_debug_all_stop guard_;
        ::uwvm2::utils::container::std_vector<worker> rows_;
        bytes selected_history_;
        ::uwvm2::runtime::wasm_threads::threaded_snapshot history_;
        ::uwvm2::runtime::diagnostic::snapshot_archive::decoded environment_;
        ::std::uint64_t serial_{},history_serial_{},sample_{};
        bool stopped_{},closed_{},joined_{},entry_{true};
        ::std::uint32_t result_code_{};
        unsigned result_kind_{};
        bool terminal_observed_{},output_failed_{},output_finished_{};
        threaded_history_cli& selected_output_;
        static constexpr auto wait_slice=::std::chrono::milliseconds{50};
        static bool number(::std::string_view& input,::std::uint64_t& value) noexcept
        {
            auto end=input.find(' ');auto token=input.substr(0,end);if(token.empty())return false;
            value=0u;for(char c:token){if(c<'0'||c>'9'||value>(UINT64_MAX-static_cast<unsigned>(c-'0'))/10u)return false;value=value*10u+static_cast<unsigned>(c-'0');}
            input=end==input.npos?::std::string_view{}:input.substr(end+1u);return true;
        }
        static bool read_file(::std::string_view path,bytes& output)
        {
            if(path.empty()||path.front()!='/')return false;
            for(char c:path)if(static_cast<unsigned char>(c)<32u||c==127)return false;
            ::uwvm2::utils::container::std_string name{path};
            ::fast_io::native_file file{name,::fast_io::open_mode::in|::fast_io::open_mode::no_block};
            auto before=::fast_io::status(file);bytes data;
            if(before.type!=::fast_io::file_type::regular||!before.size||before.size>data.max_size()||before.size>PTRDIFF_MAX)return false;
            data.resize(static_cast<::std::size_t>(before.size));::fast_io::operations::read_all_bytes(file,data.data(),data.data()+data.size());
            ::std::byte extra{};if(::fast_io::operations::read_some_bytes(file,&extra,&extra+1u)!=&extra)return false;
            auto after=::fast_io::status(file);file.close();
            if(before.dev!=after.dev||before.ino!=after.ino||before.size!=after.size||before.mtim!=after.mtim||before.ctim!=after.ctim)return false;
            output=::std::move(data);return true;
        }
        static bool write_file(::std::string_view path,bytes const& data) noexcept
        {
            if(path.empty()||path.front()!='/')return false;
            ::uwvm2::runtime::diagnostic::crash::dump_file out;
            return out.open(path)&&out.write(data)&&out.publish();
        }
        bool refresh_rows()
        {
            namespace t=::uwvm2::runtime::wasm_threads;
            ::std::size_t count{};
            if(guard_.read({},count)!=t::debug_stop_status::insufficient_capacity||!count)return false;
            rows_.resize(count);return guard_.read(rows_,count)==t::debug_stop_status::stopped;
        }
        bool pause()
        {
            namespace api=::uwvm2::runtime::lib;namespace t=::uwvm2::runtime::wasm_threads;
            if(stopped_)return refresh_rows();if(joined_||serial_==UINT64_MAX)return false;
            auto deadline=clock::now()+::std::chrono::seconds{15};
            while(clock::now()<deadline)
            {
                auto session=api::runtime_threaded_profile_debug_control_session_host_api();
                if(session.stop_until(clock::now()+wait_slice,guard_)==t::debug_stop_status::stopped)
                {stopped_=true;++serial_;return refresh_rows();}
                if(terminal())return false;
                ::std::this_thread::yield();
            }
            return false;
        }
        bool terminal()
        {
            namespace api=::uwvm2::runtime::lib;
            if(joined_)return true;
            unsigned kind{};::std::uint32_t code{};
            if(!api::runtime_threaded_profile_terminal_host_api(code,kind))return false;
            if(api::runtime_threaded_profile_unresolved_imports_host_api())
            {
                using termination=::uwvm2::runtime::wasm_threads::termination_kind;
                // A host syscall still owns its original native stack. Do not
                // wait indefinitely for it or unwind it to manufacture a dump.
                // Apply the same trusted terminal policy as the CLI driver.
                if(kind==static_cast<unsigned>(termination::proc_exit))::std::_Exit(static_cast<int>(code&255u));
                namespace crash=::uwvm2::runtime::diagnostic::crash;
                if(crash::first_fault.load(::std::memory_order_acquire)&&crash::report_complete.load(::std::memory_order_acquire))
                {crash::pause_process();crash::finish_unhandled_fault();}
            }
            auto retired=api::runtime_threaded_profile_wait_host_api(clock::now()+::std::chrono::milliseconds{1});
            if(!retired.permits(api::threaded_management_kind::full_source_reset))return false;
            joined_=true;stopped_=false;guard_.reset();rows_.clear();result_code_=code;result_kind_=kind;terminal_observed_=true;return true;
        }
        text status()
        {
            namespace api=::uwvm2::runtime::lib;
            text result;
            if(terminal())result=::uwvm2::utils::container::concat("guest exited: ",result_code_,"\n");
            else if(stopped_ && refresh_rows())
            {
                result=entry_?"stopped: first Wasm instruction\n":"stopped: private thread checkpoint\n";
                result+=::uwvm2::utils::container::concat("stop-id ",serial_,"\n");
                for(auto const& row:rows_)
                {
                    // Only actual logical locations; native boundaries have no
                    // invented PC, frame, locals, registers or machine address.
                    if(row.module==SIZE_MAX||row.function==SIZE_MAX||row.wasm_offset==UINT64_MAX)continue;
                    result+=::uwvm2::utils::container::concat("thread ",row.participant," module=",row.module,
                        " function=",row.function," byte-offset=",row.wasm_offset," generation=",row.source_generation,"\n");
                    if(row.retained_host)result+="  native-operation=pending; Wasm call retained; native stack and assembly unavailable\n";
                }
            }
            else result="running\n";
            auto journal=api::runtime_threaded_profile_record_status_host_api();
            result+=::uwvm2::utils::container::concat("threaded-profile events=",journal.events," cursor=",journal.cursor,
                " journal=",static_cast<unsigned>(journal.status),"\n");return result;
        }
        bool same_sources(::uwvm2::runtime::wasm_threads::threaded_snapshot const& saved)
        {
            auto source=::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin();
            if(!source||source->registry().size()!=saved.modules.size())return false;
            for(auto const& module:saved.modules)
            {
                auto found=source->registry().find(::uwvm2::utils::container::u8string_view{reinterpret_cast<char8_t const*>(module.name.data()),module.name.size()});
                if(found==source->registry().end())return false;
                auto id=source->bound_initialized_module_id(&found->second);auto file=source->bound_initialized_file(id,&found->second);
                if(!file||!file->has_owned_source_image()||file->source_size()!=module.wasm.size()||
                    ::std::memcmp(file->source_cbegin(),module.wasm.data(),module.wasm.size()))return false;
            }
            return true;
        }
        bool retire()
        {
            namespace api=::uwvm2::runtime::lib;
            auto deadline=clock::now()+::std::chrono::seconds{15};
            do
            {
                auto done=api::runtime_threaded_profile_retire_host_api(clock::now()+wait_slice,api::threaded_management_kind::full_source_reset);
                if(done.permits(api::threaded_management_kind::full_source_reset))
                {guard_.reset();stopped_=false;joined_=true;rows_.clear();return true;}
            }while(clock::now()<deadline);
            return false;
        }
        bool start()
        {
            namespace api=::uwvm2::runtime::lib;
            auto name=::uwvm2::uwvm::wasm::storage::active_execute_wasm().module_name;
            auto entry=resolve_runtime_entry_invocation(name);api::full_compile_run_config config{};
            configure_runtime_entry_buffers(config,entry);auto const& abi=config.entry_abi_buffers;
            if(!api::full_compile_prepare_host_api())return false;
            joined_=false;terminal_observed_=false;result_code_=0;result_kind_=0;
            return api::runtime_threaded_profile_start_main_host_api(name,config.entry_function_index,
                {abi.param_buffer,abi.param_bytes},abi.result_bytes)==api::threaded_main_status::running;
        }
        static void hex(text& out,::std::span<::std::byte const> value)
        {constexpr char digits[]{"0123456789abcdef"};for(auto b:value){auto n=::std::to_integer<unsigned>(b);out+=digits[n>>4u];out+=digits[n&15u];}}
        text history_command(::std::string_view raw)
        {
            namespace ar=::uwvm2::runtime::diagnostic::snapshot_archive;
            namespace t=::uwvm2::runtime::wasm_threads;
            if(raw.starts_with("open "))
            {
                raw.remove_prefix(5u);::std::uint64_t selected{};bytes input;t::threaded_snapshot decoded;
                if(!number(raw,selected)||!read_file(raw,input)||!t::decode_threaded_snapshot(input,decoded)||
                   selected>=decoded.environments.size()||history_serial_==UINT64_MAX)return "error: invalid thread environment sample\n";
                ar::decoded env;if(!ar::decode(decoded.environments[selected].archive,env))return "error: invalid environment data\n";
                history_=::std::move(decoded);selected_history_=::std::move(input);environment_=::std::move(env);sample_=selected;++history_serial_;raw={};
            }
            if(!history_serial_)return "error: open a saved thread history first\n";
            if(raw.empty())
            {
                ::std::uint64_t instruction{};for(::std::size_t i{};i<history_.environments[sample_].cursor;++i)
                    instruction+=history_.history.events[i].kind==t::journal_kind::instruction;
                text out=::uwvm2::utils::container::concat("snapshot-history serial=",history_serial_," position=",
                    ::fast_io::mnp::os_c_str(sample_?"checkpoint":"origin")," event=",sample_," instruction=",instruction," recording=");
                hex(out,t::threaded_history_label(history_.history));
                out+=::uwvm2::utils::container::concat(" samples=",history_.environments.size()," cursor=",history_.environments[sample_].cursor);
                out+="; historical data, no live resource access\n";
                for(auto const& record:environment_.records)
                {out+=::uwvm2::utils::container::concat("environment kind=",static_cast<unsigned>(record.kind)," id=",record.id," bytes=",record.payload.size(),"\n");}
                return out;
            }
            ::std::uint64_t serial{},kind{},id{},offset{},count{};
            if(!number(raw,serial)||!number(raw,kind)||!number(raw,id)||!number(raw,offset)||!number(raw,count)||!raw.empty()||
                serial!=history_serial_||!count||count>4096u)return "error: stale history transfer\n";
            for(auto const& record:environment_.records)
                if(static_cast<unsigned>(record.kind)==kind&&record.id==id&&offset<=record.payload.size()&&count<=record.payload.size()-offset)
                {
                    auto out=::uwvm2::utils::container::concat("snapshot-history-bytes serial=",serial," kind=",kind," id=",id,
                        " offset=",offset," count=",count," hex=");hex(out,record.payload.subspan(offset,count));out+='\n';return out;
                }
            return "error: unavailable environment range\n";
        }
    public:
        explicit threaded_debug_cli(threaded_history_cli& output) noexcept:selected_output_{output}{}
        [[nodiscard]] bool launch()
        {
            if(!start())return false;
            auto deadline=clock::now()+::std::chrono::seconds{30};
            while(clock::now()<deadline)
            {
                if(!pause())return joined_;
                auto observed=::uwvm2::runtime::lib::runtime_threaded_profile_record_status_host_api();
                if(observed.status==::uwvm2::runtime::wasm_threads::journal_status::endpoint)
                {entry_=false;return true;}
                if(observed.status==::uwvm2::runtime::wasm_threads::journal_status::recording)
                {
                    if(!rows_.empty()&&::std::all_of(rows_.begin(),rows_.end(),[](auto const& row)
                        {return row.site==::uwvm2::runtime::wasm_threads::debug_stop_site::before_instruction;}))return true;
                }
                else if(observed.status!=::uwvm2::runtime::wasm_threads::journal_status::replaying)return false;
                // A managed replay can be paused before its journal observes
                // the selected endpoint. Let only the already-checked prefix
                // advance to that boundary; an ordinary entry stop must not be
                // mistaken for either a held recording or a replay endpoint.
                guard_.reset();stopped_=false;::std::this_thread::yield();
            }
            return false;
        }
        [[nodiscard]] bool closed() const noexcept{return closed_;}
        void detach() noexcept
        {
            if(!output_finished_)
            {
                output_finished_=true;
                if(selected_output_.selected()&&!selected_output_.finish(stopped_?&guard_:nullptr))
                {
                    output_failed_=true;
                    ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                        u8"uwvm: [error] Thread history was not complete or could not be published; no partial archive was published.");
                }
            }
            if(closed_&&joined_)return;if(!retire())::std::_Exit(125);closed_=true;
        }
        [[nodiscard]] int result() const noexcept
        {
            namespace crash=::uwvm2::runtime::diagnostic::crash;
            using kind=::uwvm2::runtime::wasm_threads::termination_kind;
            // A debugger connection does not suppress the trusted launch fault
            // policy or turn a Guest failure into a successful process exit.
            if(terminal_observed_&&result_kind_==static_cast<unsigned>(kind::native_failure))::fast_io::fast_terminate();
            if(crash::first_fault.load(::std::memory_order_acquire))
            {crash::crash_unless_stable_exit();return static_cast<int>(crash::exit_status());}
            if(output_failed_)return 125;
            if(!terminal_observed_)return 0;
            if(result_kind_==static_cast<unsigned>(kind::proc_exit))return static_cast<int>(result_code_&255u);
            if(result_kind_==static_cast<unsigned>(kind::preparation_rejected))return 126;
            if(result_kind_==static_cast<unsigned>(kind::replay_refused))return 125;
            return result_kind_==static_cast<unsigned>(kind::main_return)?0:121;
        }
        [[nodiscard]] text command(::std::string_view raw) noexcept
        {
            namespace api=::uwvm2::runtime::lib;namespace t=::uwvm2::runtime::wasm_threads;namespace p=::uwvm2::utils::control;
            try
            {
                if(raw=="permissions")return ::uwvm2::utils::container::concat("permissions ",text{p::debug_permissions::name()},"\n");
                if(raw=="info host-interfaces"||raw.starts_with("info host-interfaces "))
                {
                    namespace dbg=::uwvm2::uwvm::debugger;
                    auto request=dbg::parse_console_command(::fast_io::string_view{raw.data(),raw.size()});
                    if(request.kind!=dbg::console_command_kind::host_interfaces)return "error: invalid host interface inventory request\n";
                    if(!dbg::command_permission_allowed(request))return "error: debugger permission denies this operation\n";
                    return dbg::host_interfaces::inventory(request.module_help_page);
                }
                if(!p::debug_permissions::full_allowed())return "error: private execution control requires full permission\n";
                if(raw=="status")return status();
                if(raw=="quit"){closed_=true;return "private thread debugger detached\n";}
                if(raw=="pause")return pause()?status():text{"error: private all-stop unavailable\n"};
                if(raw=="continue")
                {
                    if(!stopped_||!refresh_rows())return "error: no current thread stop\n";
                    auto observed=api::runtime_threaded_profile_record_status_host_api();
                    if(entry_||observed.status==t::journal_status::endpoint)
                        if(!api::runtime_threaded_profile_replay_continue_host_api(guard_))return "error: no recorded continuation; live I/O fallback forbidden\n";
                    entry_=false;guard_.reset();stopped_=false;rows_.clear();return status();
                }
                if(raw=="dump policy")
                {
                    auto selection=p::dump_permissions::runtime_policy.selected();auto allowed=p::dump_permissions::permit();
                    return ::uwvm2::utils::container::concat("dump-policy ",::fast_io::mnp::os_c_str(selection.kind==p::dump_permissions::constraint::maximum?"max":"force"),
                        "=",::fast_io::mnp::os_c_str(p::dump_permissions::name(selection.level))," effective=",
                        ::fast_io::mnp::os_c_str(p::dump_permissions::name(allowed.level)),"\n");
                }
                if(p::dump_permissions::permit(p::dump_permissions::disclosure::full).status!=p::dump_permissions::denial::none)
                    return "error: full dump disclosure is not authorized\n";
                if(raw=="dump history")return history_command({});
                if(raw.starts_with("dump history "))return history_command(raw.substr(13u));
                if(raw=="thread-history status")
                {
                    auto s=api::runtime_threaded_profile_record_status_host_api();return ::uwvm2::utils::container::concat(
                        "thread-history mode=",static_cast<unsigned>(s.status)," events=",s.events," cursor=",s.cursor," stop=",stopped_?serial_:0u,"\n");
                }
                bool const save=raw.starts_with("thread-history save "),replay=raw.starts_with("thread-history replay "),mark=raw.starts_with("thread-history mark ");
                if(!save&&!replay&&!mark)return "error: operation unavailable on the private thread execution driver\n";
                raw.remove_prefix(replay?22u:20u);::std::uint64_t stop{};
                if(!number(raw,stop)||(!joined_&&(!stopped_||stop!=serial_||!refresh_rows()))||(joined_&&stop))return "error: stale thread stop\n";
                if(save||mark)
                {
                    bytes data;if(!api::runtime_threaded_profile_snapshot_capture_host_api(joined_?nullptr:&guard_,data))return "error: incomplete coherent thread snapshot\n";
                    if(save&&!write_file(raw,data))return "error: cannot publish thread snapshot\n";
                    if(mark&&!raw.empty())return "error: mark takes only a stop\n";
                    t::threaded_snapshot snapshot;if(!t::decode_threaded_snapshot(data,snapshot))return "error: invalid captured archive\n";
                    return ::uwvm2::utils::container::concat("thread-history saved=",save?1u:0u," samples=",snapshot.environments.size()," retained=",snapshot.history.retained_native?1u:0u,"\n");
                }
                ::std::uint64_t cursor{};bytes data;t::threaded_snapshot snapshot;
                if(!number(raw,cursor)||!read_file(raw,data)||!t::decode_threaded_snapshot(data,snapshot)||!same_sources(snapshot)||cursor>snapshot.history.events.size())
                    return "error: invalid thread replay source or cursor; current world retained\n";
                if(snapshot.history.retained_native)
                    return !cursor&&stopped_&&api::runtime_threaded_profile_snapshot_reattach_host_api(guard_,data)?
                        status():text{"error: unknown native result requires the original live stop\n"};
                // Validation precedes irreversible retirement. A pending native
                // operation cannot be cancelled and re-issued as a known result.
                if(api::runtime_threaded_profile_unresolved_imports_host_api())return "error: current native operation is unresolved\n";
                snapshot.history.replay_until=cursor?cursor:snapshot.history.replay_until;
                if(!t::encode_threaded_snapshot(snapshot.history,snapshot.modules,data,snapshot.environments))return "error: invalid replay target\n";
                if(!retire())return "error: original workers have not physically joined\n";
                api::reset_runtime_state_host_api();
                if(!api::runtime_threaded_profile_snapshot_replay_configure_host_api(data)||prepare_owned_full_cli_source(true,true)!=0||!start())
                {closed_=true;return "error: replacement refused; old world retired\n";}
                entry_=false;
                auto deadline=clock::now()+::std::chrono::seconds{30};
                while(clock::now()<deadline)
                {
                    if(terminal())return status();
                    if(api::runtime_threaded_profile_record_status_host_api().status==t::journal_status::endpoint)
                        return pause()?status():text{"error: reconstructed stop unavailable\n"};
                    ::std::this_thread::yield();
                }
                return "error: replay target pending; query status or pause\n";
            }
            catch(...){return "error: thread control allocation or input failure\n";}
        }
    };
#endif
