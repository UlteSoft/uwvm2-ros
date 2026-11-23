// Link as the actual embedding main with the integrated
// Runtime objects/flags; never link this beside the ordinary CLI main.
// No Group/Admission/World/participant constructors, friends or fake providers.
#include <uwvm2/uwvm/crtmain/uwvm.h>
#include <uwvm2/uwvm/runtime/storage/full.h>
#include <uwvm2/runtime/lib/uwvm_runtime_threaded_profile_api.h>
#include <chrono>
#include <array>
#include <atomic>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <thread>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#if !defined(__linux__) || !UWVM2_HAS_THREADED_PROFILE_HOST_API
#error This source requires the actual Linux threaded Runtime Host API.
#endif
namespace b1
{
    namespace api=::uwvm2::runtime::lib;
    namespace run=::uwvm2::uwvm::run;
    namespace threads=::uwvm2::runtime::wasm_threads;
    using clock=::std::chrono::steady_clock;
    bool complete(api::threaded_retirement const& value) noexcept
    {return value.physically_joined&&value.native_error==0&&value.preparing==0u&&value.native_pending==0u&&
        value.compiler_tickets_pending==0u&&value.native_cleanup_pending==0u;}
    bool join(clock::time_point deadline) noexcept
    {
        do
        {
            auto next{clock::now()+::std::chrono::milliseconds{50}};
            auto actual{api::runtime_threaded_profile_retire_host_api(next<deadline?next:deadline,
                api::threaded_management_kind::stop_only)};
            if(complete(actual))return true;
            if(actual.native_error!=0&&actual.native_error!=EBUSY)return false;
        }while(clock::now()<deadline);
        return false;
    }
    int fail(char const* phase) noexcept
    {
        auto journal=api::runtime_threaded_profile_record_status_host_api();
        bool const joined{join(clock::now()+::std::chrono::seconds{5})};
        ::std::fprintf(stderr,"B1 FAIL phase=%s physically_joined=%u journal=%u cursor=%llu events=%llu running=%llu\n",phase,unsigned(joined),unsigned(journal.status),(unsigned long long)journal.cursor,(unsigned long long)journal.events,(unsigned long long)journal.running);return 1;
    }
    bool retire_runtime() noexcept
    {
        // Stop/join retains reusable compiled code. Explicitly reset while LLVM
        // globals are alive; process-exit teardown deliberately retains MCJIT
        // owners to avoid calling already-destroyed LLVM global registries.
        // Verify reset was admitted instead of silently accepting a busy owner.
        api::runtime_stop_and_drain_host_api();
        auto const before{api::observe_compiler_runtime_generation_host_api()};
        api::reset_runtime_state_host_api();
        return api::observe_compiler_runtime_generation_host_api()!=before;
    }
}
// Usage: native control int|llvm -- <authorized finite threaded options>.
// Compiled and linked as a real host embedding; no fake child/provider APIs.
int main(int argc,char** argv)
{
    namespace f=b1;namespace t=f::threads;
    auto* exit_text=::std::getenv("UWVM_TEST_EXIT_CODE");unsigned const expected_exit=exit_text?static_cast<unsigned>(::std::strtoul(exit_text,nullptr,10)):0u;
    bool const expect_trap=::std::getenv("UWVM_TEST_TRAP")!=nullptr;
    bool const pending_wasi=::std::getenv("UWVM_TEST_PENDING")!=nullptr;
    auto* ros_pause_file=::std::getenv("UWVM_TEST_PENDING_ROS_FILE");
    bool const pending_test=pending_wasi||ros_pause_file;int ros_peer{};
    int input_pipe[2]{-1,-1};
    if(pending_wasi){if(::pipe(input_pipe)||::dup2(input_pipe[0],0)<0)return 2;::close(input_pipe[0]);}
    if(argc<6||::std::strcmp(argv[3],"--")!=0)return 2;
    bool const race{::std::strcmp(argv[1],"race")==0};
    bool const transfer_timeout{::std::strcmp(argv[1],"turn-timeout")==0};
    bool const branch_trace{::std::strcmp(argv[1],"branches")==0};
    bool const mte_stop{::std::strcmp(argv[1],"mte-stop")==0};
    bool const enabled{race||transfer_timeout||branch_trace||mte_stop||::std::strcmp(argv[1],"control")==0};
    if(!enabled&&::std::strcmp(argv[1],"none")!=0)return 2;
    bool const llvm{::std::strcmp(argv[2],"llvm")==0};
    if(!llvm&&::std::strcmp(argv[2],"int")!=0)return 2;
    ::std::vector<char8_t const*> cli{reinterpret_cast<char8_t const*>(argv[0])};
    for(int i{4};i<argc;++i)cli.push_back(reinterpret_cast<char8_t const*>(argv[i]));
    if(::uwvm2::uwvm::cmdline::parsing(cli.size(),cli.data())!=::uwvm2::uwvm::cmdline::parsing_return_val::def)return 2;
    // This embedding deliberately skips run(); seal the actual host launch
    // selection before calling protected APIs, exactly as the CLI run path does.
    namespace permissions=::uwvm2::utils::control::debug_permissions;
    namespace options=::uwvm2::uwvm::cmdline::params::details;
    if(!::uwvm2::runtime::diagnostic::crash::process_policy.seal(static_cast<::uwvm2::runtime::diagnostic::crash::action>(options::debug_crash_action)))return f::fail("crash-launch-policy");
    namespace dumps=::uwvm2::utils::control::dump_permissions;
    if(!dumps::runtime_policy.configure_trusted_launch({static_cast<dumps::constraint>(options::debug_dump_constraint),static_cast<dumps::disclosure>(options::debug_dump_disclosure)}))return f::fail("dump-launch-policy");
    if(!options::debug_permissions_is_exist||!permissions::configure_trusted_launch(
        static_cast<permissions::permission_level>(options::debug_permission_selection)))return f::fail("explicit-launch-permissions");
    t::threaded_debug_control_limits explicit_control{t::threaded_debug_control::cooperative_all_stop,4u*1024u*1024u};
    if(f::api::runtime_threaded_profile_debug_control_configure_host_api(explicit_control))return f::fail("control-without-metadata");
    if(enabled&&(!f::api::runtime_threaded_profile_debug_observation_configure_host_api(t::threaded_debug_observation::readonly_metadata)||
        !f::api::runtime_threaded_profile_debug_control_configure_host_api(explicit_control)))return f::fail("explicit-opt-in");
    if(enabled&&f::api::runtime_threaded_profile_debug_observation_configure_host_api(t::threaded_debug_observation::none))
        return f::fail("silent-metadata-removal");
    t::threaded_profile_limits limits{};
    if(f::api::runtime_threaded_profile_policy_host_api(limits)!=f::api::threaded_policy_status::configured||
        limits.max_active_children<1u)return f::fail("explicit-finite-profile");

    if(::std::getenv("UWVM_TEST_REPLAY"))
    {
        auto* file=::std::fopen(::std::getenv("UWVM_TEST_HISTORY"),"rb");if(!file)return f::fail("read-image");
        ::std::fseek(file,0,SEEK_END);auto length=::std::ftell(file);::std::rewind(file);
        if(length<0)return f::fail("file-length");::uwvm2::utils::container::std_vector<::std::byte> bytes(length);
        bool read=::std::fread(bytes.data(),1u,bytes.size(),file)==bytes.size();::std::fclose(file);
        t::threaded_snapshot archive;
        if(!read||!t::decode_threaded_snapshot(bytes,archive))return f::fail("decode-replay");
        auto image=::std::move(archive.history);
        if(::std::getenv("UWVM_TEST_PREFIX"))
        {
            ::std::size_t entered{};
            for(::std::size_t i=1;i<image.events.size();++i)
            {
                entered+=image.events[i].kind==t::journal_kind::enter;
                if(entered>=3u&&image.events[i].kind==t::journal_kind::instruction){image.replay_until=i;break;}
            }
            if(auto const* requested=::std::getenv("UWVM_TEST_PREFIX_AT"))
            {
                char* end{};auto const value=::std::strtoull(requested,&end,10);
                if(end==requested||*end||value<2u||value>=image.events.size())return f::fail("invalid-prefix-offset");
                image.replay_until=static_cast<::std::size_t>(value);
            }
            if(!image.replay_until)return f::fail("no-prefix");
        }
        if(!f::api::runtime_threaded_profile_replay_configure_host_api(image))return f::fail("configure-replay");
    }
    else if(!f::api::runtime_threaded_profile_record_configure_host_api())return f::fail("configure-recording");
    if(f::run::prepare_owned_full_cli_source(true)!=0)return f::fail("owned-cli-source");
    f::run::resolve_runtime_compile_threads();
    auto const name{::uwvm2::uwvm::wasm::storage::active_execute_wasm().module_name};
    auto entry{f::run::resolve_runtime_entry_invocation(name)};f::api::full_compile_run_config config{};
    f::run::configure_runtime_entry_buffers(config,entry);
    if(!f::api::full_compile_prepare_host_api())return f::fail("real-full-validator");
    auto retained{::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin()};
    if(!retained||!::uwvm2::uwvm::runtime::full::full_source_instance::has_canonical_owner(retained))return f::fail("actual-source-pin");
    if(ros_pause_file)
    {
        auto* input=::std::fopen(ros_pause_file,"r");
        if(!input)return f::fail("native-ros-peer-file");
        auto parsed=::std::fscanf(input,"%d",&ros_peer);::std::fclose(input);
        if(parsed!=1||ros_peer<=1||::kill(ros_peer,SIGSTOP))return f::fail("native-ros-peer-pause");
    }
    auto const& abi{config.entry_abi_buffers};
    if(f::api::runtime_threaded_profile_start_main_host_api(name,config.entry_function_index,
        {abi.param_buffer,abi.param_bytes},abi.result_bytes)!=f::api::threaded_main_status::running)return f::fail("real-main-start");
    auto session{f::api::runtime_threaded_profile_debug_control_session_host_api()};
    t::threaded_debug_all_stop guard;
    ::std::size_t initially_needed{123u};
    if(guard.read({},initially_needed)!=t::debug_stop_status::unavailable||initially_needed!=0u)return f::fail("default-empty-guard");
    if(!enabled)
    {
        if(session.stop_until(f::clock::now()+::std::chrono::milliseconds{100},guard)!=t::debug_stop_status::unavailable)
            return f::fail("none-exposed-control");
        if(!f::join(f::clock::now()+::std::chrono::seconds{5}))return f::fail("none-physical-join");
        retained.reset();
        if(!f::retire_runtime())return f::fail("none-runtime-retirement");
        ::std::puts("B1 SOURCE CASE none: actual control refusal and physical join");return 0;
    }


    if(::std::getenv("UWVM_TEST_CHECKPOINT"))
    {
        auto until=f::clock::now()+::std::chrono::seconds{20};bool stable{};
        ::std::vector<t::debug_stopped_worker> original_rows;
        while(f::clock::now()<until)
        {
            if(session.stop_until(f::clock::now()+::std::chrono::milliseconds{100},guard)==t::debug_stop_status::stopped)
            {
                ::std::size_t count{};(void)guard.read({},count);original_rows.resize(count);
                if(guard.read(original_rows,count)==t::debug_stop_status::stopped&&count==3u&&
                    ::std::all_of(original_rows.begin(),original_rows.end(),[](auto const& row){return !row.retained_host&&row.site==t::debug_stop_site::before_instruction;}))
                {stable=true;break;}
            }
            guard.reset();::std::this_thread::sleep_for(::std::chrono::milliseconds{1});
        }
        if(!stable)return f::fail("checkpoint-real-cohort");
        auto read_memory=[&](auto const& owner,auto& image)
        {
            ::std::size_t count{};if(guard.read({},count)!=t::debug_stop_status::insufficient_capacity||count!=3)return false;
            auto const* module=owner->initialized_main_module();if(!module||module->imported_memory_vec_storage.size()!=1)return false;
            auto const& link=module->imported_memory_vec_storage.index_unchecked(0);
            using K=::uwvm2::uwvm::runtime::storage::imported_memory_storage_t::imported_memory_link_kind;
            if(link.link_kind!=K::defined||!link.target.defined_ptr)return false;
            bool owned{};for(auto const& item:owner->registry())for(auto const& memory:item.second.local_defined_memory_vec_storage)
                owned|=::std::addressof(memory)==link.target.defined_ptr;
            if(!owned)return false;
            auto const& memory=link.target.defined_ptr->memory;image.resize(t::threaded_memory_length(memory));
            return t::copy_threaded_memory(memory,image,false);
        };
        ::uwvm2::utils::container::std_vector<::std::byte> before,after,archive;
        if(!read_memory(retained,before)||!f::api::runtime_threaded_profile_snapshot_capture_host_api(&guard,archive))return f::fail("checkpoint-capture");
        t::threaded_snapshot checked;
        if(!t::decode_threaded_snapshot(archive,checked)||checked.history.retained_native||checked.history.terminal)return f::fail("checkpoint-prefix-kind");
        auto* file=::std::fopen(::std::getenv("UWVM_TEST_HISTORY"),"wb");if(!file)return f::fail("checkpoint-file");
        bool saved=::std::fwrite(archive.data(),1,archive.size(),file)==archive.size();::std::fclose(file);if(!saved)return f::fail("checkpoint-save");
        if(!f::join(f::clock::now()+::std::chrono::seconds{10}))return f::fail("checkpoint-retire-real-workers");
        guard.reset();retained.reset();f::api::reset_runtime_state_host_api();
        if(!f::api::runtime_threaded_profile_snapshot_replay_configure_host_api(archive)||f::run::prepare_owned_full_cli_source(true,true)!=0)return f::fail("checkpoint-reconfigure");
        auto current_name=::uwvm2::uwvm::wasm::storage::active_execute_wasm().module_name;
        auto next_entry=f::run::resolve_runtime_entry_invocation(current_name);f::api::full_compile_run_config next_config{};
        f::run::configure_runtime_entry_buffers(next_config,next_entry);
        if(!f::api::full_compile_prepare_host_api())return f::fail("checkpoint-full-validate");
        auto const& buffers=next_config.entry_abi_buffers;
        if(f::api::runtime_threaded_profile_start_main_host_api(current_name,next_config.entry_function_index,{buffers.param_buffer,buffers.param_bytes},buffers.result_bytes)!=f::api::threaded_main_status::running)return f::fail("checkpoint-start-rebuilt");
        auto current=f::api::runtime_threaded_profile_debug_control_session_host_api();until=f::clock::now()+::std::chrono::seconds{15};
        while(f::api::runtime_threaded_profile_record_status_host_api().status!=t::journal_status::endpoint&&f::clock::now()<until)::std::this_thread::sleep_for(::std::chrono::milliseconds{1});
        if(current.stop_until(f::clock::now()+::std::chrono::seconds{3},guard)!=t::debug_stop_status::stopped)return f::fail("checkpoint-rebuilt-stop");
        auto rebuilt=::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin();
        if(!read_memory(rebuilt,after)||before!=after)return f::fail("checkpoint-memory-diff");
        ::std::size_t count{};(void)guard.read({},count);::std::vector<t::debug_stopped_worker> restored_rows(count);
        if(count!=original_rows.size()||guard.read(restored_rows,count)!=t::debug_stop_status::stopped)return f::fail("checkpoint-roster");
        auto key=[](auto const& row){return ::std::tuple{row.module,row.function,row.wasm_offset,row.site};};
        auto order=[&](auto const& a,auto const& b){return key(a)<key(b);};
        ::std::sort(original_rows.begin(),original_rows.end(),order);::std::sort(restored_rows.begin(),restored_rows.end(),order);
        for(::std::size_t i{};i<count;++i)if(key(original_rows[i])!=key(restored_rows[i]))
        {::std::fprintf(stderr,"checkpoint location original=%zu:%zu:%llu restored=%zu:%zu:%llu\n",original_rows[i].module,original_rows[i].function,(unsigned long long)original_rows[i].wasm_offset,restored_rows[i].module,restored_rows[i].function,(unsigned long long)restored_rows[i].wasm_offset);return f::fail("checkpoint-location-diff");}
        if(f::api::runtime_threaded_profile_replay_continue_host_api(guard))return f::fail("checkpoint-no-unrecorded-future");
        if(!f::join(f::clock::now()+::std::chrono::seconds{10}))return f::fail("checkpoint-final-join");
        guard.reset();rebuilt.reset();
        if(!f::retire_runtime())return f::fail("checkpoint-runtime-retirement");
        ::std::puts("PASS nonterminal checkpoint: actual thread destruction/reconstruction, identical memory/PCs, unknown future refused");return 0;
    }

    if(pending_test)
    {
        bool stopped{};auto until=f::clock::now()+::std::chrono::seconds{15};
        while(f::clock::now()<until)
        {
            if(session.stop_until(f::clock::now()+::std::chrono::milliseconds{100},guard)==t::debug_stop_status::stopped)
            {
                ::std::size_t count{};(void)guard.read({},count);::std::vector<t::debug_stopped_worker> rows(count);
                if(guard.read(rows,count)==t::debug_stop_status::stopped)
                {
                    unsigned retained{};for(auto const& row:rows)retained+=row.retained_host;
                    // Retention can precede native call admission while the
                    // worker waits for its recorded turn. Require two admitted,
                    // unresolved calls, not merely two retained participants.
                    t::threaded_execution_history candidate;
                    if(retained>=2&&f::api::runtime_threaded_profile_record_capture_host_api(&guard,candidate)&&candidate.retained_native)
                    {
                        ::std::size_t in_flight{};
                        for(auto const& event:candidate.events)
                        {
                            if(event.kind==t::journal_kind::host_begin)++in_flight;
                            else if(event.kind==t::journal_kind::host_end){if(!in_flight)return f::fail("pending-history-order");--in_flight;}
                        }
                        if(in_flight>=2){stopped=true;break;}
                    }
                }
            }
            guard.reset();::std::this_thread::sleep_for(::std::chrono::milliseconds{2});
        }
        if(!stopped)return f::fail("pending-all-stop");
        t::threaded_execution_history original;
        if(!f::api::runtime_threaded_profile_record_capture_host_api(&guard,original)||!original.retained_native)return f::fail("pending-capture");
        if(!f::api::runtime_threaded_profile_record_reattach_host_api(guard,original))return f::fail("pending-reattach");
        ::uwvm2::utils::container::std_vector<::std::byte> pending_archive;
        if(!f::api::runtime_threaded_profile_snapshot_capture_host_api(&guard,pending_archive)||
           !f::api::runtime_threaded_profile_snapshot_reattach_host_api(guard,pending_archive))return f::fail("pending-archive");
        ::std::string pending_path=::std::getenv("UWVM_TEST_HISTORY");pending_path+=".pending";
        auto* pending_file=::std::fopen(pending_path.c_str(),"wb");if(!pending_file)return f::fail("pending-file");
        bool pending_saved=::std::fwrite(pending_archive.data(),1,pending_archive.size(),pending_file)==pending_archive.size();::std::fclose(pending_file);
        if(!pending_saved)return f::fail("pending-write");
        auto forged=original;forged.events.front().input[0]^=::std::byte{1};
        if(f::api::runtime_threaded_profile_record_reattach_host_api(guard,forged))return f::fail("pending-forgery");
        auto child=::fork();
        if(child==0){bool denied=!f::api::runtime_threaded_profile_record_reattach_host_api(guard,original);::_exit(denied?0:9);}
        int code{};if(child<0||::waitpid(child,&code,0)!=child||!WIFEXITED(code)||WEXITSTATUS(code))return f::fail("pending-foreign");
        if(pending_wasi){if(::write(input_pipe[1],"AB",2)!=2)return f::fail("pending-input");::close(input_pipe[1]);}
        else if(::kill(ros_peer,SIGCONT))return f::fail("pending-ros-peer-resume");
        ::std::this_thread::sleep_for(::std::chrono::milliseconds{20});
        t::threaded_execution_history unchanged;
        if(!f::api::runtime_threaded_profile_record_capture_host_api(&guard,unchanged)||unchanged.events.size()!=original.events.size())return f::fail("pending-published-while-paused");
        guard.reset();
        if(f::api::runtime_threaded_profile_record_reattach_host_api(guard,original))return f::fail("pending-expired");
        ::std::puts(pending_wasi?"PASS actual in-flight fd_read: retained native, no paused delivery, same-process only":
            "PASS actual in-flight stable-v1 ROS RPC: retained native, no paused delivery, same-process only");
    }
    if(::std::getenv("UWVM_TEST_PREFIX"))
    {
        auto until=f::clock::now()+::std::chrono::seconds{15};
        while(f::api::runtime_threaded_profile_record_status_host_api().status!=t::journal_status::endpoint&&f::clock::now()<until)
            ::std::this_thread::sleep_for(::std::chrono::milliseconds{1});
        bool stable{};auto stop_deadline=f::clock::now()+::std::chrono::seconds{2};
        while(f::clock::now()<stop_deadline)
        {if(session.stop_until(stop_deadline,guard)==t::debug_stop_status::stopped){stable=true;break;} ::std::this_thread::yield();}
        if(!stable)return f::fail("prefix-all-stop");
        t::threaded_execution_history prefix;
        if(!f::api::runtime_threaded_profile_record_capture_host_api(&guard,prefix)||!prefix.replay_until)return f::fail("prefix-capture");
        ::std::size_t count{};if(guard.read({},count)!=t::debug_stop_status::insufficient_capacity||count<1u)return f::fail("prefix-workers");
        if(!f::api::runtime_threaded_profile_replay_continue_host_api(guard))return f::fail("prefix-continue");
        guard.reset();::std::printf("PASS real prefix checkpoint cursor=%llu workers=%zu\n",(unsigned long long)prefix.replay_until,count);
    }
    bool const replay=::std::getenv("UWVM_TEST_REPLAY")!=nullptr;
    // Configuration below is performed BEFORE main startup by the insertion
    // above; this part only observes real native completion and exports data.
    auto deadline=f::clock::now()+::std::chrono::seconds{40};bool joined{};
    while(f::clock::now()<deadline)
    {
        auto result=f::api::runtime_threaded_profile_wait_host_api(f::clock::now()+::std::chrono::milliseconds{50});
        if(f::complete(result)){joined=true;break;}
        auto status=f::api::runtime_threaded_profile_record_status_host_api();
        if(status.status==t::journal_status::diverged){return f::fail("replay-diverged");}
    }
    if(!joined)return f::fail("natural-completion");
    ::std::uint32_t code{};unsigned kind{};
    auto completed=f::api::runtime_threaded_profile_result_host_api({},&code,&kind);
    auto status=f::api::runtime_threaded_profile_record_status_host_api();
    ::std::printf("status=%u events=%llu cursor=%llu kind=%u code=%u\n",unsigned(status.status),
        (unsigned long long)status.events,(unsigned long long)status.cursor,kind,code);
    if(completed!=f::api::threaded_main_status::finished||kind!=static_cast<unsigned>(expect_trap?t::termination_kind::trap:exit_text?t::termination_kind::proc_exit:t::termination_kind::main_return)||(!expect_trap&&code!=expected_exit))return f::fail("main-result");
    t::threaded_execution_history completed_record;
    if(!replay)
    {
        t::threaded_execution_history image;::uwvm2::utils::container::std_vector<::std::byte> encoded;
        if(!f::api::runtime_threaded_profile_record_capture_host_api(nullptr,image)||!f::api::runtime_threaded_profile_snapshot_capture_host_api(nullptr,encoded))return f::fail("capture");
        completed_record=image;
        t::threaded_snapshot checked;bool archive_decoded=t::decode_threaded_snapshot(encoded,checked);
        auto source=::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin();
        if(!archive_decoded||!source||checked.modules.size()!=source->registry().size())
        {::std::fprintf(stderr,"archive decoded=%u modules=%zu bytes=%zu\n",unsigned(archive_decoded),checked.modules.size(),encoded.size());return f::fail("archive-source-roundtrip");}
        for(auto const& module:checked.modules)
        {
            auto found=source->registry().find(::uwvm2::utils::container::u8string_view{reinterpret_cast<char8_t const*>(module.name.data()),module.name.size()});
            if(found==source->registry().end())return f::fail("archive-module-name");
            auto const* file=source->bound_initialized_file(source->bound_initialized_module_id(&found->second),&found->second);
            if(!file||file->source_size()!=module.wasm.size()||::std::memcmp(file->source_cbegin(),module.wasm.data(),module.wasm.size()))return f::fail("archive-module-bytes");
        }
        auto* file=::std::fopen(::std::getenv("UWVM_TEST_HISTORY"),"wb");if(!file)return f::fail("file");
        bool written=::std::fwrite(encoded.data(),1u,encoded.size(),file)==encoded.size();::std::fclose(file);if(!written)return f::fail("write");
        ::std::size_t born{},departed{};for(auto const& event:image.events){born+=event.kind==t::journal_kind::enter;departed+=event.kind==t::journal_kind::leave;}
        ::std::printf("actual private workers entered=%zu departed=%zu\n",born,departed);
        if(born<3u)return f::fail("missing-dynamic-workers");
    }
    else if(status.cursor!=status.events)return f::fail("journal-not-consumed");
    retained.reset();
    if(!replay && ::std::getenv("UWVM_TEST_SAME_PROCESS"))
    {
        f::api::reset_runtime_state_host_api();
        if(!f::api::runtime_threaded_profile_replay_configure_host_api(completed_record))return f::fail("same-process-reconfigure");
        if(f::run::prepare_owned_full_cli_source(true,true)!=0)return f::fail("same-process-source");
        auto const current_name=::uwvm2::uwvm::wasm::storage::active_execute_wasm().module_name;
        auto next_entry=f::run::resolve_runtime_entry_invocation(current_name);f::api::full_compile_run_config next_config{};
        f::run::configure_runtime_entry_buffers(next_config,next_entry);
        if(!f::api::full_compile_prepare_host_api())return f::fail("same-process-validation");
        auto const& next_abi=next_config.entry_abi_buffers;
        if(f::api::runtime_threaded_profile_start_main_host_api(current_name,next_config.entry_function_index,
            {next_abi.param_buffer,next_abi.param_bytes},next_abi.result_bytes)!=f::api::threaded_main_status::running)return f::fail("same-process-start");
        bool done{};auto until=f::clock::now()+::std::chrono::seconds{30};
        while(f::clock::now()<until)
        {if(f::complete(f::api::runtime_threaded_profile_wait_host_api(f::clock::now()+::std::chrono::milliseconds{50}))){done=true;break;}}
        auto next=f::api::runtime_threaded_profile_record_status_host_api();
        ::std::fprintf(stderr,"rebuilt status=%u events=%llu cursor=%llu running=%llu\n",unsigned(next.status),(unsigned long long)next.events,(unsigned long long)next.cursor,(unsigned long long)next.running);
        unsigned next_kind{};::std::uint32_t next_code{};
        if(!done||next.cursor!=next.events||f::api::runtime_threaded_profile_result_host_api({},&next_code,&next_kind)!=f::api::threaded_main_status::finished||
           next_kind!=static_cast<unsigned>(expect_trap?t::termination_kind::trap:exit_text?t::termination_kind::proc_exit:t::termination_kind::main_return)||next_code!=code)return f::fail("same-process-result");
        ::std::puts("PASS actual retirement, full Source reset, fresh dynamic reconstruction in same process");
    }
    if(!f::retire_runtime())return f::fail("runtime-retirement");
    ::std::puts("PASS real dynamic group execution journal");return 0;
}
