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
#include "after_join_shared_memory.h"
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
        bool const joined{join(clock::now()+::std::chrono::seconds{5})};
        ::std::fprintf(stderr,"B1 FAIL phase=%s physically_joined=%u\n",phase,unsigned(joined));return 1;
    }
}
// Usage: embedding control int|llvm -- <normal finite threaded options>
// Fixtures: actual env memory provider and b1_wait_and_loop.wat compiled on the
// authorized Linux host only. --run selects its real _start. No cache override.
int main(int argc,char** argv)
{
    namespace f=b1;namespace t=f::threads;
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
    if(f::run::prepare_owned_full_cli_source(true)!=0)return f::fail("owned-cli-source");
    f::run::resolve_runtime_compile_threads();
    auto const name{::uwvm2::uwvm::wasm::storage::active_execute_wasm().module_name};
    auto entry{f::run::resolve_runtime_entry_invocation(name)};f::api::full_compile_run_config config{};
    f::run::configure_runtime_entry_buffers(config,entry);
    if(!f::api::full_compile_prepare_host_api())return f::fail("real-full-validator");
    auto retained{::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin()};
    if(!retained||!::uwvm2::uwvm::runtime::full::full_source_instance::has_canonical_owner(retained))return f::fail("actual-source-pin");
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
        ::std::puts("B1 SOURCE CASE none: actual control refusal and physical join");return 0;
    }

    auto metadata=f::api::runtime_threaded_profile_debug_session_host_api();
    std::array<t::threaded_debug_worker_ticket,4> tickets;
    auto deadline=f::clock::now()+std::chrono::seconds{20};bool departed{};
    while(f::clock::now()<deadline)
    {
        std::size_t count{};
        if(metadata.workers(tickets,count)==t::debug_read_status::ok)
        {
            for(std::size_t i{};i<count;++i)
            {
                t::debug_worker_snapshot snapshot{};
                auto status=metadata.read(tickets[i],snapshot,{},{},{});
                if(status!=t::debug_read_status::insufficient_capacity&&status!=t::debug_read_status::ok)continue;
                std::vector<t::debug_function_row> funcs(snapshot.metadata.functions);
                std::vector<t::debug_logical_pc_row> pcs(snapshot.metadata.logical_pcs);
                std::vector<t::debug_cfi_row> cfi(snapshot.metadata.cfi_ranges);
                if(metadata.read(tickets[i],snapshot,funcs,pcs,cfi)==t::debug_read_status::ok&&
                   snapshot.state==t::debug_worker_state::body_left)departed=true;
            }
        }
        if(departed)break;
        std::this_thread::yield();
    }
    if(!departed)return f::fail("actual-child-not-departed");
    std::puts("actual child has returned; its native work is retained before join");
    t::debug_stop_status stopped{};
    do {stopped=session.stop_until(f::clock::now()+std::chrono::milliseconds{100},guard);}
    while(stopped!=t::debug_stop_status::stopped&&f::clock::now()<deadline);
    if(stopped!=t::debug_stop_status::stopped)return f::fail("stop-after-child-return");
    std::array<t::debug_stopped_worker,4> stopped_rows{};std::size_t stopped_count{};
    if(guard.read(stopped_rows,stopped_count)!=t::debug_stop_status::stopped||stopped_count!=1)
        return f::fail("actual-live-cohort");
    std::size_t retained_count{};
    if(guard.read_retained({},retained_count)!=t::debug_stop_status::insufficient_capacity||retained_count!=2)
        return f::fail("real-retained-count");
    std::array<t::debug_retained_worker,2> retained_rows{};
    if(guard.read_retained(retained_rows,retained_count)!=t::debug_stop_status::stopped||retained_count!=2)
        return f::fail("real-retained-worlds");
    auto const& parent=retained_rows[0];auto const& child=retained_rows[1];
    if(!parent.initial_main||parent.tid||parent.body_returned||child.initial_main||child.tid!=1||!child.body_returned||
       !parent.incarnation||!child.incarnation||parent.incarnation==child.incarnation||
       parent.source_generation!=child.source_generation||parent.validation_epoch!=child.validation_epoch||
       parent.backend!=child.backend||parent.backend!=(llvm?t::debug_backend::llvm:t::debug_backend::interpreter))
        return f::fail("retained-lifecycle-identities");
    // An ordinary reap must not dispose of a retained child while an all-stop
    // reader owns this actual generation. Terminal retirement still joins below.
    auto reap=f::api::runtime_threaded_profile_wait_host_api(f::clock::now()+std::chrono::milliseconds{10});
    if(reap.physically_joined||reap.native_error!=EBUSY||
       guard.read_retained(retained_rows,retained_count)!=t::debug_stop_status::stopped||retained_count!=2)
        return f::fail("reap-invalidated-live-snapshot");
    auto pid=::fork();
    if(pid<0)return f::fail("native-fork");
    if(pid==0)
    {
        ::alarm(10);std::size_t count{99};t::threaded_debug_all_stop new_guard;
        bool good=guard.read(stopped_rows,count)==t::debug_stop_status::stale&&count==0;
        count=99;good=good&&guard.read_retained(retained_rows,count)==t::debug_stop_status::stale&&count==0;
        good=good&&guard.advance_until(stopped_rows[0].participant,f::clock::now()+std::chrono::seconds{1})==t::debug_stop_status::stale;
        good=good&&session.stop_until(f::clock::now()+std::chrono::seconds{1},new_guard)==t::debug_stop_status::stale;
        count=99;good=good&&metadata.workers(tickets,count)==t::debug_read_status::stale&&count==0;
        auto fresh=f::api::runtime_threaded_profile_debug_session_host_api();
        count=99;good=good&&fresh.workers(tickets,count)==t::debug_read_status::unavailable&&count==0;
        auto fresh_control=f::api::runtime_threaded_profile_debug_control_session_host_api();
        good=good&&fresh_control.stop_until(f::clock::now()+std::chrono::seconds{1},new_guard)==t::debug_stop_status::unavailable;
        guard.reset();:: _exit(good?0:1);
    }
    int child_status{};
    if(::waitpid(pid,&child_status,0)!=pid||!WIFEXITED(child_status)||WEXITSTATUS(child_status)!=0)
        return f::fail("fork-inherited-private-authority");
    if(guard.read(stopped_rows,stopped_count)!=t::debug_stop_status::stopped||stopped_count!=1)
        return f::fail("fork-affected-original-stop");
    for(unsigned i{};i<32;++i)
    {
        if(guard.advance_until(stopped_rows[0].participant,f::clock::now()+std::chrono::seconds{5})!=t::debug_stop_status::stopped)
            return f::fail("advance-after-child-return");
        if(guard.read(stopped_rows,stopped_count)!=t::debug_stop_status::stopped||stopped_count!=1)
            return f::fail("cohort-after-advance");
    }
    if(!f::join(f::clock::now()+std::chrono::seconds{10}))return f::fail("actual-physical-join");
    std::size_t stale_count{99};
    if(guard.read_retained(retained_rows,stale_count)!=t::debug_stop_status::stale||stale_count)
        return f::fail("retired-lifecycle-still-readable");
    retained.reset();f::api::runtime_stop_and_drain_host_api();guard.reset();
    if(guard.read_retained(retained_rows,stale_count)!=t::debug_stop_status::unavailable||stale_count)
        return f::fail("reset-lifecycle-still-readable");
    std::puts("PASS: live parent stopped/advanced after dynamic child return; actual native group joined");
    return 0;
}
