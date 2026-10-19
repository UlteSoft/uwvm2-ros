// UNEXECUTED SOURCE. Link as the actual embedding main with the integrated
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
// Usage: embedding none|control|race int|llvm -- <normal finite threaded options>
// Fixtures: actual env memory provider and b1_wait_and_loop.wat compiled on the
// authorized Linux host only. --run selects its real _start. No cache override.
int main(int argc,char** argv)
{
    namespace f=b1;namespace t=f::threads;
    if(argc<6||::std::strcmp(argv[3],"--")!=0)return 2;
    bool const race{::std::strcmp(argv[1],"race")==0};
    bool const enabled{race||::std::strcmp(argv[1],"control")==0};
    if(!enabled&&::std::strcmp(argv[1],"none")!=0)return 2;
    bool const llvm{::std::strcmp(argv[2],"llvm")==0};
    if(!llvm&&::std::strcmp(argv[2],"int")!=0)return 2;
    t::threaded_debug_control_limits explicit_control{t::threaded_debug_control::cooperative_all_stop,4u*1024u*1024u};
    if(f::api::runtime_threaded_profile_debug_control_configure_host_api(explicit_control))return f::fail("control-without-metadata");
    if(enabled&&(!f::api::runtime_threaded_profile_debug_observation_configure_host_api(t::threaded_debug_observation::readonly_metadata)||
        !f::api::runtime_threaded_profile_debug_control_configure_host_api(explicit_control)))return f::fail("explicit-opt-in");
    if(enabled&&f::api::runtime_threaded_profile_debug_observation_configure_host_api(t::threaded_debug_observation::none))
        return f::fail("silent-metadata-removal");
    ::std::vector<char8_t const*> cli{reinterpret_cast<char8_t const*>(argv[0])};
    for(int i{4};i<argc;++i)cli.push_back(reinterpret_cast<char8_t const*>(argv[i]));
    if(::uwvm2::uwvm::cmdline::parsing(cli.size(),cli.data())!=::uwvm2::uwvm::cmdline::parsing_return_val::def)return 2;
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
    ::std::vector<t::debug_stopped_worker> rows(limits.max_active_children+1u);
    ::std::array<::std::uint64_t,2u> participants{},incarnations{};
    for(unsigned cycle{};cycle!=100u;++cycle)
    {
        auto const outer{f::clock::now()+::std::chrono::seconds{5}};bool accepted{};
        do
        {
            auto const result{session.stop_until(f::clock::now()+::std::chrono::milliseconds{100},guard)};
            if(f::clock::now()>outer){guard.reset();return f::fail("elapsed-outer-stop-budget");}
            if(result==t::debug_stop_status::stopped)
            {
                ::std::size_t needed{};bool wait{},loop{};
                if(guard.read(rows,needed)!=t::debug_stop_status::stopped||needed!=2u)return f::fail("actual-two-worker-roster");
                for(::std::size_t i{};i!=needed;++i)
                {
                    auto const& row{rows[i]};
                    if(row.participant==0u||row.incarnation==0u||row.native_tid==0u||row.source_generation==0u||
                        row.validation_epoch!=row.source_generation||
                        row.backend!=(llvm?t::debug_backend::llvm:t::debug_backend::interpreter))return f::fail("actual-worker-proof");
                    if(row.site==t::debug_stop_site::atomic_wait)
                    {wait=true;if(row.instruction!=SIZE_MAX||row.wasm_offset!=UINT64_MAX)return f::fail("wait-fake-logical-pc");}
                    if(row.site==(llvm?t::debug_stop_site::function_boundary:t::debug_stop_site::before_instruction))
                    {
                        loop=true;
                        if(llvm?(row.instruction!=SIZE_MAX||row.wasm_offset!=UINT64_MAX):
                            (row.instruction==SIZE_MAX||row.wasm_offset==UINT64_MAX))return f::fail("backend-position-contract");
                    }
                }
                accepted=wait&&loop;
                if(accepted)
                {
                    for(::std::size_t i{};i!=needed;++i)
                    {
                        if(cycle==0u){participants[i]=rows[i].participant;incarnations[i]=rows[i].incarnation;}
                        else if(rows[i].participant!=participants[i]||rows[i].incarnation!=incarnations[i])return f::fail("unstable-fixed-roster");
                    }
                }
                if(!accepted)guard.reset();
            }
            else if(result!=t::debug_stop_status::busy&&result!=t::debug_stop_status::timeout)return f::fail("unexpected-stop-refusal");
            if(!accepted)::std::this_thread::sleep_for(::std::chrono::milliseconds{1});
        }while(!accepted&&f::clock::now()<outer);
        if(!accepted)return f::fail("no-real-wait-plus-loop-cohort");
        if(cycle!=99u)guard.reset(); // Immediate S1->S2 must use real drain ACK.
    }
    if(race)
    {
        // Real independent native caller races ordinary guard.reset/P.resume
        // against real terminal request_stop/physical retirement. This barrier
        // chooses no fake token/park state and does not prove the precise narrow
        // token-publication interleaving happened. Repeat with an owned Linux
        // process deadline; no detached thread or successful timeout is allowed.
        ::std::atomic<bool> begin{};bool actually_joined{};
        ::std::thread stopper{[&]
        {
            while(!begin.load(::std::memory_order_acquire))::std::this_thread::yield();
            actually_joined=f::join(f::clock::now()+::std::chrono::seconds{5});
        }};
        begin.store(true,::std::memory_order_release);guard.reset();
        stopper.join(); // Real native thread completion, never a synthetic ACK.
        if(!actually_joined)return f::fail("resume-concurrent-terminal-physical-join");
    }
    else
    {
        // Keep the genuine issued observer alive through real stop AND join.
        if(!f::join(f::clock::now()+::std::chrono::seconds{5}))return f::fail("terminal-stop-with-live-guard");
    }
    ::std::uint64_t ready{},progress{},continuation{};
    if(!f::read_after_join(retained,60000u,ready)||ready!=1u||
        !f::read_after_join(retained,60004u,progress)||progress==0u||
        !f::read_after_join(retained,60008u,continuation)||continuation!=0u)return f::fail("real-progress-or-post-stop-sentinel");
    ::std::uint32_t code{};unsigned kind{};
    if(f::api::runtime_threaded_profile_result_host_api({abi.result_buffer,abi.result_bytes},&code,&kind)!=
        f::api::threaded_main_status::finished||kind!=static_cast<unsigned>(t::termination_kind::cancelled)||code!=0u)
        return f::fail("actual-terminal-outcome");
    ::std::size_t needed{};
    if(guard.read(rows,needed)!=(race?t::debug_stop_status::unavailable:t::debug_stop_status::stale))return f::fail("terminal-guard-not-invalidated");
    if(!race&&session.stop_until(f::clock::now()+::std::chrono::milliseconds{100},guard)!=t::debug_stop_status::busy)
        return f::fail("issued-expired-output-overwritten");
    // The ordinary entry descriptor/config contain only byte vectors/raw ABI
    // buffers (run.h runtime_entry_invocation, API full_compile_run_config).
    // Explicitly drop the external Source pin used ONLY for post-join reads;
    // never keep a testing Group/admission/execution lease across public drain.
    retained.reset();
    auto const drain_started{f::clock::now()};
    f::api::runtime_stop_and_drain_host_api(); // ACTUAL public management path.
    if(f::clock::now()-drain_started>::std::chrono::seconds{5})return f::fail("actual-public-drain-elapsed-budget");
    // control mode still has its original stale issued guard here. Weak identity
    // must not turn expiry into fresh/unavailable or resume a retired Group.
    if(!race&&guard.read(rows,needed)!=t::debug_stop_status::stale)return f::fail("post-drain-issued-expired-guard");
    guard.reset();
    if(guard.read(rows,needed)!=t::debug_stop_status::unavailable)return f::fail("reset-empty-guard");
    if(session.stop_until(f::clock::now()+::std::chrono::milliseconds{100},guard)!=t::debug_stop_status::stale)
        return f::fail("terminal-session-reused");
    ::std::puts(race?
        "B1 SOURCE CASE race: real resume/terminal concurrency attempted; precise token gap unobserved; physical join/drain":
        "B1 SOURCE CASE control: real wait/loop, 100 serials, held stale observer across physical join and public drain");
    return 0;
}
