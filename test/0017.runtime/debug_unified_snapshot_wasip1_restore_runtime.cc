// Real unified archive export, decode and two consecutive whole-instance
// restorations. Each retirement physically joins both original workers.
// Run with group or global to verify shared environments and a separate single.
#include "mandatory_native_fixture.h"
#include <uwvm2/uwvm/run/owned_source.h>
#include <uwvm2/uwvm/cmdline/callback/impl.h>
#include <uwvm2/utils/control/debug_permissions.h>
#include <uwvm2/utils/control/owned_file_image.h>
#include <uwvm2/runtime/checkpoint/materialization.h>
#include <uwvm2/runtime/gc/entry_admission.h>
#include <uwvm2/uwvm/debugger/checkpoint_codec.h>
#include <uwvm2/runtime/diagnostic/snapshot_state.h>
#include <fast_io.h>
#include <fast_io_unit/string.h>
#include <atomic>
#include <array>
#include <barrier>
#include <cstddef>
#include <cstdint>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
// Derive actual target/backend capabilities in this fixture's own balanced scope.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#if !defined(UWVM_RUNTIME_LLVM_JIT) || !defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) || !defined(UWVM_CPP_EXCEPTIONS)
# error Actual checkpoint census fixture requires LLVM full/native threads/C++ EH
#endif
namespace cp = ::uwvm2::uwvm::debugger::checkpoint;
namespace lib = ::uwvm2::runtime::lib;
namespace mode = ::uwvm2::uwvm::runtime::runtime_mode;
namespace threads = ::uwvm2::utils::thread;
namespace checkpoint = ::uwvm2::runtime::checkpoint;
namespace image = ::uwvm2::utils::control;
namespace full = ::uwvm2::uwvm::runtime::full;
using domain = threads::cooperative_pause_domain;
static void require(bool valid, unsigned line)
{
    if(valid) { return; }
    ::fast_io::print(::fast_io::err(), "debug_unified_snapshot_wasip1_restore_runtime FAIL line=", ::fast_io::mnp::dec(line), "\n");
    ::fast_io::fast_terminate();
}
#define REQUIRE(x) require(bool(x), __LINE__)
static auto deadline() { return ::std::chrono::steady_clock::now() + ::std::chrono::seconds{180}; }
struct source_setup
{
    ::uwvm2::utils::container::std_u8string path{};
    image::owned_file_image::owner immutable{};
    full::full_source_instance::mutable_owner source{};
    bool ready{};
    static bool prepare(void* pointer) noexcept
    {
        // [actual main-owned synchronous drained setup context] owner_end
        // [safe] retained until replace_full_source_after_drain callback returns.
        auto& state{*static_cast<source_setup*>(pointer)};
        try
        {
            auto candidate{full::full_source_instance::create_unparsed(state.path, u8"checkpoint-instance")};
            if(!full::select_unparsed_full_source_after_drain(candidate)) { return false; }
            // SAME exclusive source image is adopted BEFORE magic/section parse,
            // not copied from a mutable file mapping after compiler validation.
            auto const loaded{::uwvm2::uwvm::wasm::loader::load_wasm_file(candidate->file_for_native_initialization(),
                candidate->owned_file_name(), candidate->owned_rename(), ::uwvm2::uwvm::wasm::storage::wasm_parameter,
                ::std::move(state.immutable))};
            if(loaded != ::uwvm2::uwvm::wasm::loader::load_wasm_file_rtl::ok ||
               ::uwvm2::uwvm::run::load_local_modules(false)!=static_cast<int>(::uwvm2::uwvm::run::retval::ok) ||
               ::uwvm2::uwvm::wasm::loader::construct_all_module_and_check_duplicate_module() !=
                   ::uwvm2::uwvm::wasm::loader::load_and_check_modules_rtl::ok ||
               ::uwvm2::uwvm::wasm::loader::check_import_exist_and_detect_cycles() !=
                   ::uwvm2::uwvm::wasm::loader::load_and_check_modules_rtl::ok) { return false; }
            // These modules have no start functions. Complete ordinary segment
            // instantiation, including passive expression payloads, BEFORE any
            // setup guest entry or source seal; true would leave it deferred.
            if(!::uwvm2::uwvm::runtime::initializer::initialize_runtime(false)) { return false; }
            if(!candidate->seal_actual_initializer()) { return false; }
            state.source = ::std::move(candidate); state.ready = true; return true;
        }
        catch(...) { return false; }
    }
};
struct observer
{
    ::std::shared_ptr<domain> control{::std::make_shared<domain>(2u)};
    ::std::mutex mutex{}; ::std::condition_variable changed{};
    ::std::array<::std::uint_least64_t, 2u> participants{};
    ::std::array<threads::cooperative_pause_location, 2u> locations{};
    ::std::array<lib::llvm_jit_checkpoint_capture_result, 2u> captures{};
    domain::pause_ticket ticket{}; threads::cooperative_pause_location request_location{};
    ::std::size_t seen{}, done{};
    ::std::array<::std::size_t, 2u> points{};
    bool collecting{};
    ::std::atomic<unsigned long long> restored_points{};
    ::std::uint_least64_t original_epoch{};
    void prepare_attempt()
    {
        // Both actual guest threads have joined BEFORE state is reused.
        // No private producer, participant or ticket constructor is used.
        ::std::lock_guard lock{mutex};
        REQUIRE(done == 0u || done == 2u);
        participants = {}; locations = {}; captures = {}; ticket = {};
        request_location = {}; seen = 0u; done = 0u; points = {};
    }
    static void point(void* pointer, ::std::uint_least64_t participant,
        threads::cooperative_pause_location location) noexcept
    {
        auto& self{*static_cast<observer*>(pointer)};
        ::std::lock_guard lock{self.mutex};
        REQUIRE(participant != 0u);
        if(lib::observe_compiler_runtime_generation_host_api()!=self.original_epoch) { ++self.restored_points; }
        if(!self.collecting || location.function != 4u || self.ticket) { return; }
        ::std::size_t index{};
        for(; index != self.seen; ++index) { if(self.participants[index] == participant) { break; } }
        if(index == self.seen)
        {
            REQUIRE(self.seen < self.participants.size());
            self.participants[self.seen++] = participant;
        }
        ++self.points[index];
        // Each threshold counts genuine original safe-point callbacks, never
        // an invented opcode address/value. The WAT prefix initializes all
        // selected references before the twentieth callback, then runs nops.
        if(self.seen != 2u || self.points[0u] < 20u || self.points[1u] < 20u) { return; }
        // This only requests a pause. Two previously observed IDs alone are
        // NOT a census proof: a first entrant may already have returned.
        self.ticket = self.control->request_pause(); REQUIRE(self.ticket);
        self.request_location = location;
        self.changed.notify_all();
        // No observer waits, yields, reenters or blocks for the other guest.
    }
    static void before_park(void* pointer, ::std::uint_least64_t participant,
        threads::cooperative_pause_location location, lib::llvm_jit_debug_local_view) noexcept
    {
        auto& self{*static_cast<observer*>(pointer)};
        ::std::lock_guard lock{self.mutex}; REQUIRE(self.ticket);
        ::std::size_t index{};
        for(; index != self.seen; ++index)
        { if(self.participants[index] == participant) { break; } }
        if(index == self.seen)
        { REQUIRE(self.seen < self.participants.size()); self.participants[self.seen++] = participant; }
        REQUIRE(index < self.participants.size() && !self.captures[index].capture);
        // Whichever actual participant requested the pause owns this location.
        // Both locations are independently matched to the actual domain below.
        // Both owners come ONLY from the real generated before-park callback.
        // The actual ticket/control/lease/source/plan checks remain in producer.
        self.locations[index] = location;
        self.captures[index] = lib::llvm_jit_checkpoint_capture_thread_host_api(self.ticket);
        self.changed.notify_all();
    }
};


int main(int argc, char** argv)
{
    if(!::uwvm2test::mandatory_native::arguments(argc, argv)) { return 64; }
    if(argc < 2 || argc > 4) { return 64; }

    REQUIRE(::uwvm2::utils::control::debug_permissions::configure_trusted_launch(
        ::uwvm2::utils::control::debug_permissions::permission_level::full));
    mode::global_runtime_mode = mode::runtime_mode_t::full_compile;
    mode::global_runtime_compiler = mode::runtime_compiler_t::llvm_jit_only;

    mode::global_runtime_compile_threads = 0u; mode::runtime_compile_threads_existed = true;
    mode::global_runtime_llvm_jit_cache_path_mode = mode::runtime_llvm_jit_cache_path_mode_t::disabled;
    auto& features{::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(::uwvm2::uwvm::wasm::storage::wasm_parameter.binfmt1_para)};
    features.disable_gc = false; features.explicit_enable_gc = true;
    features.disable_function_references = false; features.explicit_enable_function_references = true;
    features.disable_reference_types = false; features.explicit_enable_reference_types = true;
    features.disable_memory64 = false; features.explicit_enable_memory64 = true;
    features.disable_table64 = false; features.explicit_enable_table64 = true;
    features.disable_table_instructions = false; features.explicit_enable_table_instructions = true;
    features.disable_multiple_tables = false; features.explicit_enable_multiple_tables = true;
    features.disable_table_initializer = false; features.explicit_enable_table_initializer = true;
    features.disable_exceptions = false; features.explicit_enable_exceptions = true;
    features.disable_simd = false; features.explicit_enable_simd = true;
    features.disable_bulk_memory = false; features.explicit_enable_bulk_memory = true;
    source_setup setup{};
    setup.path = ::fast_io::u8concat_std(::fast_io::mnp::code_cvt(::fast_io::mnp::os_c_str(argv[1])));
    auto& arguments{::uwvm2::uwvm::cmdline::parsing_result};
    ::uwvm2::uwvm::cmdline::wasm_file_ppos = nullptr; arguments.clear();
    arguments.emplace_back(::uwvm2::utils::cmdline::parameter_parsing_results{
        u8"debug-checkpoint-complete-instance", nullptr, ::uwvm2::utils::cmdline::parameter_parsing_results_type::dir});
    arguments.emplace_back(::uwvm2::utils::cmdline::parameter_parsing_results{
        ::uwvm2::utils::container::u8cstring_view{::fast_io::mnp::os_c_str(setup.path.c_str())}, nullptr,
        ::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg});
    // [actual finalized global CLI argument allocation] end
    // [safe] cursor installed AFTER all vector growth, retained through reset.
    ::uwvm2::uwvm::cmdline::wasm_file_ppos = ::std::addressof(arguments.back());
    namespace ws=::uwvm2::uwvm::imported::wasi::wasip1::storage;
    namespace api=::uwvm2::utils::control::host_interfaces;
    using permission=::uwvm2::imported::wasi::wasip1::environment::permission_t;
    ::uwvm2::uwvm::wasm::storage::local_preload_wasip1=false;
    bool const use_global=argc>=3 && ::std::string_view{argv[2]}=="global";
    auto const group=use_global?ws::ensure_global_wasip1_group_index():ws::try_create_named_wasip1_group_index(u8"restore-shared");
    REQUIRE(group!=ws::invalid_wasip1_group_index);
    if(!use_global)
    {
        REQUIRE(ws::try_add_wasip1_module_to_named_group(u8"checkpoint-instance",group));
        REQUIRE(ws::try_add_wasip1_module_to_named_group(u8"checkpoint-shared",group));
    }
    REQUIRE(ws::try_create_targetless_wasip1_module_override(u8"checkpoint-isolated"));
    for(auto* policy:{ws::find_wasip1_group_state(group),ws::find_targetless_wasip1_module_override(u8"checkpoint-isolated")})
    {
        REQUIRE(policy);policy->enabled=true;policy->enabled_is_set=true;
        policy->non_fd_permissions=permission::args|permission::environ|permission::stdio_metadata|permission::stdout_write;
        policy->permission_presence=policy->non_fd_permissions;
        for(auto name:{"args_sizes_get","environ_sizes_get","fd_fdstat_get"})
        { REQUIRE(policy->interfaces.set(api::find(api::family::wasip1,::std::string_view{name}),api::access::allow)); }
        policy->noinherit_system_environment=true;policy->noinherit_system_environment_is_set=true;
        policy->force_args_is_set=true;policy->force_argument_storage.emplace_back(u8"alpha");policy->force_argument_storage.emplace_back(u8"beta");
        policy->add_or_replace_environment.emplace(u8"MODE",policy==ws::find_wasip1_group_state(group)?::uwvm2::utils::container::u8string_view{u8"restore"}: ::uwvm2::utils::container::u8string_view{u8"separate"});
    }
    ::uwvm2::uwvm::wasm::storage::execute_wasm.module_name=u8"checkpoint-instance";
    for(auto name:{u8"checkpoint-shared",u8"checkpoint-isolated"})
    {
        auto& preload=::uwvm2::uwvm::wasm::storage::preloaded_wasm.emplace_back();
        preload.module_name=::uwvm2::utils::container::u8string_view{::fast_io::mnp::os_c_str(name)};
        preload.file_name=::uwvm2::utils::container::u8cstring_view{::fast_io::mnp::os_c_str(setup.path.c_str())};
        preload.wasm_parameter=::uwvm2::uwvm::wasm::storage::wasm_parameter;
    }
    REQUIRE(::uwvm2::uwvm::run::prepare_owned_full_cli_source(true)==static_cast<int>(::uwvm2::uwvm::run::retval::ok));
    auto state{::std::make_shared<observer>()};

    auto profile{checkpoint::compilation_profile::create_for_trusted_observer()};
    REQUIRE(profile && profile->purpose() == checkpoint::compilation_purpose::observe_values);
    REQUIRE(lib::llvm_jit_configure_debug_session_host_api(state->control,
        {state, observer::point, observer::before_park}, lib::llvm_jit_debug_safe_point_granularity::instruction) ==
        lib::llvm_jit_debug_configure_result::ok);
    REQUIRE(lib::llvm_jit_configure_debug_value_observation_host_api(profile->limits()) == lib::llvm_jit_debug_configure_result::ok);
    REQUIRE(lib::llvm_jit_prepare_debug_host_api());
    lib::full_compile_run_config initialize{}; initialize.entry_function_index = 3u;
    for(auto name:{u8"checkpoint-instance",u8"checkpoint-shared",u8"checkpoint-isolated"})
    {
        auto view=::uwvm2::utils::container::u8string_view{::fast_io::mnp::os_c_str(name)};
        REQUIRE(::uwvm2::uwvm::runtime::initializer::apply_runtime_active_segments(view)==
            ::uwvm2::uwvm::runtime::initializer::details::active_segment_result::ok);
        lib::full_compile_and_run_main_module(view,initialize);
    }
    { ::std::lock_guard lock{state->mutex}; state->collecting = true; }
    using outcome = lib::llvm_jit_checkpoint_native_retirement_status;
    lib::llvm_jit_checkpoint_prepare_request prepare_request{};prepare_request.recording_label[0u]=::std::byte{34u};
    prepare_request.include_wasip1=true;prepare_request.require_managed_wasip1_resources=false;
    auto const epoch{lib::observe_compiler_runtime_generation_host_api()};
    REQUIRE(epoch != 0u);

    state->original_epoch=epoch;
    auto start=::std::make_shared<::std::barrier<>>(3);
    struct launch
    {
        ::std::shared_ptr<observer> state;
        ::std::shared_ptr<::std::barrier<>> start;
        ::std::uint32_t output{0xdeadbeefu};
        static void body(void* p) noexcept
        {
            auto& self=*static_cast<launch*>(p);self.start->arrive_and_wait();
            lib::full_compile_run_config config{};config.entry_function_index=4u;
            config.entry_abi_buffers.result_buffer=reinterpret_cast<::std::byte*>(&self.output);
            config.entry_abi_buffers.result_bytes=sizeof(self.output);
            lib::full_compile_and_run_main_module(u8"checkpoint-instance",config);
            ::std::lock_guard lock{self.state->mutex};++self.state->done;self.state->changed.notify_all();
        }
    };
    ::std::array<::std::shared_ptr<launch>,2u> launches{};
    ::std::array<lib::llvm_jit_debug_guest_worker_owner,2u> workers{};
    for(unsigned n{};n<2;++n)
    {
        launches[n]=::std::make_shared<launch>();launches[n]->state=state;launches[n]->start=start;
        auto actual=lib::runtime_launch_llvm_jit_debug_guest_worker_host_api(state->control,launches[n],launch::body);
        REQUIRE(actual.status==lib::llvm_jit_debug_guest_worker_launch_status::started);workers[n]=actual.worker;
    }
    start->arrive_and_wait();
    domain::pause_ticket ticket{};
    { ::std::unique_lock lock{state->mutex};
      REQUIRE(state->changed.wait_until(lock,deadline(),[&]{return bool(state->ticket)||state->done==2u;}));ticket=state->ticket; }
    REQUIRE(ticket);
    REQUIRE(state->control->wait_until_paused(ticket,deadline())==threads::cooperative_pause_result::paused);
    ::std::array<lib::llvm_jit_checkpoint_thread_capture_owner,2u> captures{};
    { ::std::lock_guard lock{state->mutex};
      for(unsigned n{};n<2;++n) { ::fast_io::io::perrln("THREAD_CAPTURE ",n," status=",unsigned(state->captures[n].status)); REQUIRE(state->captures[n].status==lib::llvm_jit_checkpoint_capture_status::captured);captures[n]=state->captures[n].capture; } }
    auto saved=lib::llvm_jit_checkpoint_capture_instance_with_wasip1_host_api(ticket,captures,prepare_request.recording_label,prepare_request.graph_budget);
    ::fast_io::io::perrln("CAPTURE status=",unsigned(saved.status)," data=",unsigned(saved.data_error));
    REQUIRE(saved.status==lib::llvm_jit_checkpoint_instance_capture_status::captured);
    bool expect_recorded{};
    auto check_environments=[&](auto const& capture)
    {
        REQUIRE(capture.status==lib::llvm_jit_checkpoint_instance_capture_status::captured);
        REQUIRE(capture.wasip1_captured_together && capture.wasip1_environments.size()==2u);
        unsigned shared{},isolated{};
        for(auto const& owner:capture.wasip1_environments)
        {
            auto value=lib::llvm_jit_checkpoint_copy_wasip1_capsule_data_host_api(owner);
            REQUIRE(value.status==lib::llvm_jit_wasip1_environment_capsule_status::captured);
            REQUIRE(value.data.arguments.size()==2u && value.data.environment.size()==1u);
            REQUIRE(value.data.arguments[0]==(expect_recorded?::fast_io::u8string_view{u8"gamma"} : ::fast_io::u8string_view{u8"alpha"}) && value.data.arguments[1]==u8"beta");
            if(value.data.shared_environment) { ++shared;REQUIRE(value.data.environment[0]==u8"MODE=restore"); }
            else { ++isolated;REQUIRE(value.data.environment[0]==u8"MODE=separate"); }
        }
        REQUIRE(shared==1u && isolated==1u);
    };
    check_environments(saved);
    // Retaining earlier captures must not impose the former 16-slot ceiling.
    ::std::vector<lib::llvm_jit_wasip1_environment_capsule_owner> retained_environments{};
    for(unsigned iteration{};iteration<24u;++iteration)
    {
        auto extra=lib::llvm_jit_checkpoint_capture_instance_with_wasip1_host_api(ticket,captures,prepare_request.recording_label,prepare_request.graph_budget);
        REQUIRE(extra.status==lib::llvm_jit_checkpoint_instance_capture_status::captured && extra.wasip1_environments.size()==2u);
        retained_environments.push_back(extra.wasip1_environments.front());
    }
    ::fast_io::io::perrln("CAPSULE live_owners=",retained_environments.size()," no_fixed_registry_ceiling=1");
    // Exercise the actual full dump collector, not an independently assembled
    // envelope. The restored graph must originate from its committed bytes.
    namespace archive=::uwvm2::runtime::diagnostic::snapshot_archive;
    namespace snapshot=::uwvm2::runtime::diagnostic::snapshot_state;
    ::std::vector<::std::byte> encoded;
    auto writer=+[](void* context,::std::span<::std::byte const> bytes) noexcept -> bool
    {
        try { auto& out=*static_cast<::std::vector<::std::byte>*>(context);out.insert(out.end(),bytes.begin(),bytes.end());return true; }
        catch(...) { return false; }
    };
    auto dumped=lib::runtime_dump_stopped_guest_host_api(ticket,epoch,"checkpoint-instance","explicit test",
        &encoded,writer,2u,1u,0u,captures);
    ::fast_io::io::perrln("DUMP status=",unsigned(dumped.status)," bytes=",encoded.size());
    REQUIRE(dumped);
    if(argc>=4)
    {
        ::fast_io::native_file output{::fast_io::mnp::os_c_str(argv[3]),::fast_io::open_mode::out|::fast_io::open_mode::creat|::fast_io::open_mode::excl,
            static_cast<::fast_io::perms>(0600)};
        ::fast_io::operations::write_all_bytes(output,encoded.data(),encoded.data()+encoded.size());output.close();
    }
    archive::decoded envelope;
    REQUIRE(archive::decode(encoded,envelope));
    ::std::shared_ptr<cp::state const> decoded;
    auto state_decoded=snapshot::decode(envelope,decoded,prepare_request.graph_budget);
    ::fast_io::io::perrln("DECODE status=",unsigned(state_decoded.status)," state=",unsigned(state_decoded.state_error));
    REQUIRE(state_decoded && decoded);
    auto comparable=*saved.graph;comparable.recording_id=decoded->recording_id;
    REQUIRE(*decoded==comparable);
    prepare_request.recording_label=decoded->recording_id;
    prepare_request.saved_graph=::std::move(decoded);
    ::fast_io::io::perrln("ARCHIVE bytes=",encoded.size()," exact_roundtrip=1");
    namespace environment_wire=::uwvm2::runtime::diagnostic::snapshot_wasip1;
    auto select_recorded=[&](bool change_argument)
    {
        prepare_request.environment_source=lib::llvm_jit_checkpoint_environment_source::recorded;
        prepare_request.recorded_wasip1_environments.clear();prepare_request.wasip1_resource_rebindings.clear();
        for(auto const& record:envelope.records)
        {
            if(record.kind!=archive::component::wasip1)continue;
            auto data=::std::make_shared<environment_wire::environment>();
            REQUIRE(environment_wire::decode(envelope.disclosure,record,*data));
            REQUIRE(data->resources.size()==1u && data->resources[0].kind==environment_wire::resource_kind::stdio);
            REQUIRE(data->resources[0].stdio_index==1u && data->reserved.size()==2u);
            if(change_argument)data->arguments[0]={::std::byte{'g'},::std::byte{'a'},::std::byte{'m'},::std::byte{'m'},::std::byte{'a'}};
            prepare_request.wasip1_resource_rebindings.push_back({data->module,0u,1u});
            prepare_request.recorded_wasip1_environments.push_back(::std::move(data));
        }
        REQUIRE(prepare_request.recorded_wasip1_environments.size()==2u);
    };
    select_recorded(true);
    auto refused=[&](lib::llvm_jit_checkpoint_prepare_request const& rejected)
    {
        auto result=lib::llvm_jit_checkpoint_prepare_instance_host_api(ticket,captures,rejected);
        REQUIRE(result.status==lib::llvm_jit_checkpoint_prepare_status::wasip1_preparation_declined);
        REQUIRE(lib::observe_compiler_runtime_generation_host_api()==epoch);
        REQUIRE(state->control->wait_until_paused(ticket,deadline())==threads::cooperative_pause_result::paused);
    };
    { auto rejected=prepare_request;rejected.wasip1_resource_rebindings.clear();refused(rejected); }
    { auto rejected=prepare_request;rejected.wasip1_resource_rebindings.push_back(rejected.wasip1_resource_rebindings[0]);refused(rejected); }
    { auto rejected=prepare_request;rejected.wasip1_resource_rebindings[0].target_descriptor=0u;refused(rejected); }
    { auto rejected=prepare_request;rejected.wasip1_resource_rebindings[0].module=999u;refused(rejected); }
    for(unsigned scenario{};scenario<4;++scenario)
    {
        auto rejected=prepare_request;
        auto corrupt=::std::make_shared<environment_wire::environment>(*rejected.recorded_wasip1_environments[0]);
        if(scenario==0)corrupt->members={corrupt->module,999u};
        if(scenario==1)corrupt->original_wasm[0]^=::std::byte{1};
        if(scenario==2)corrupt->bindings[0].base|=2u; // stdin.read is not a stdout capability.
        if(scenario==3)corrupt->recording[0]^=::std::byte{1};
        rejected.recorded_wasip1_environments[0]=corrupt;refused(rejected);
    }
    auto unchanged=lib::llvm_jit_checkpoint_capture_instance_with_wasip1_host_api(ticket,captures,prepare_request.recording_label,prepare_request.graph_budget);
    check_environments(unchanged);
    ::fast_io::io::perrln("REJECTED missing/duplicate/foreign/reserved binding, group merge, source mismatch, rights escalation and label mismatch; old world unchanged");
    expect_recorded=true;

    { ::std::lock_guard lock{state->mutex};state->collecting=false; }
    auto ready=lib::llvm_jit_checkpoint_prepare_retire_instance_host_api(ticket,captures,prepare_request,deadline());
    ::fast_io::io::perrln("PREPARE status=",unsigned(ready.status)," prepare=",unsigned(ready.preparation.status),
        " wasi=",unsigned(ready.preparation.wasip1_status)," resource=",ready.preparation.resource_diagnostic," engine=",ready.preparation.engine_diagnostic," frame=",ready.preparation.frame_diagnostic);
    REQUIRE(ready.status==outcome::prepared_world_ready_closed);
    state->prepare_attempt();
    { ::std::lock_guard lock{state->mutex};state->collecting=true; }
    auto published=lib::llvm_jit_checkpoint_publish_prepared_instance_host_api(ready.operation,deadline());
    ::fast_io::io::perrln("PUBLISH status=",unsigned(published.status)," epoch=",published.runtime_epoch);
    REQUIRE(published.status==outcome::restored_execution_started && published.runtime_epoch==epoch+1u);
    // A restored world must remain capturable and restorable. This second
    // complete cohort is observed at genuine new generated safe points.
    { ::std::unique_lock lock{state->mutex};
      REQUIRE(state->changed.wait_until(lock,deadline(),[&]{return bool(state->ticket);}));ticket=state->ticket; }
    REQUIRE(state->control->wait_until_paused(ticket,deadline())==threads::cooperative_pause_result::paused);
    { ::std::lock_guard lock{state->mutex};
      for(unsigned n{};n<2;++n) { ::fast_io::io::perrln("RECAPTURE ",n," status=",unsigned(state->captures[n].status));
        REQUIRE(state->captures[n].status==lib::llvm_jit_checkpoint_capture_status::captured);captures[n]=state->captures[n].capture; } }
    auto recaptured=lib::llvm_jit_checkpoint_capture_instance_with_wasip1_host_api(ticket,captures,prepare_request.recording_label,prepare_request.graph_budget);
    ::fast_io::io::perrln("RECAPTURE_INSTANCE status=",unsigned(recaptured.status)," data=",unsigned(recaptured.data_error));
    check_environments(recaptured);
    encoded.clear();
    dumped=lib::runtime_dump_stopped_guest_host_api(ticket,published.runtime_epoch,"checkpoint-instance","second test",
        &encoded,writer,2u,2u,0u,captures);
    REQUIRE(dumped);REQUIRE(archive::decode(encoded,envelope));
    auto again=snapshot::decode(envelope,decoded,prepare_request.graph_budget);
    ::fast_io::io::perrln("REDECODE status=",unsigned(again.status)," state=",unsigned(again.state_error));
    REQUIRE(again && decoded);
    prepare_request.saved_graph=decoded;prepare_request.recording_label=decoded->recording_id;
    select_recorded(false);
    { ::std::lock_guard lock{state->mutex};state->collecting=false; }
    ready=lib::llvm_jit_checkpoint_prepare_retire_instance_host_api(ticket,captures,prepare_request,deadline());
    ::fast_io::io::perrln("REPREPARE status=",unsigned(ready.status)," prepare=",unsigned(ready.preparation.status),
        " data=",unsigned(ready.preparation.data_error)," wasi=",unsigned(ready.preparation.wasip1_status)," resource=",ready.preparation.resource_diagnostic,
        " engine=",ready.preparation.engine_diagnostic," frame=",ready.preparation.frame_diagnostic);
    REQUIRE(ready.status==outcome::prepared_world_ready_closed);
    published=lib::llvm_jit_checkpoint_publish_prepared_instance_host_api(ready.operation,deadline());
    REQUIRE(published.status==outcome::restored_execution_started && published.runtime_epoch==epoch+2u);
    auto completed=lib::llvm_jit_checkpoint_join_restored_instance_host_api(published.runtime_epoch,deadline());
    ::fast_io::io::perrln("JOIN status=",unsigned(completed.status)," threads=",completed.threads.size()," points=",state->restored_points.load());
    REQUIRE(completed.status==lib::llvm_jit_checkpoint_restored_join_status::joined && completed.threads.size()==2u);
    for(auto status:completed.threads) { ::fast_io::io::perrln("THREAD outcome=",unsigned(status)); REQUIRE(status==lib::llvm_jit_checkpoint_continuation_status::continued); }
    REQUIRE(state->restored_points.load()!=0u);
    for(unsigned n{};n<2;++n)
    {
        REQUIRE(launches[n]->output==0xdeadbeefu);
        REQUIRE(lib::runtime_join_llvm_jit_debug_guest_worker_until_host_api(workers[n],deadline()).actual.status==::fast_io::thread_join_status::joined);
    }
    // Normal entry also uses the new registry/code and relocated GC graph.
    ::std::uint32_t value{};lib::full_compile_run_config run{};run.entry_function_index=4u;
    run.entry_abi_buffers.result_buffer=reinterpret_cast<::std::byte*>(&value);run.entry_abi_buffers.result_bytes=sizeof(value);
    lib::full_compile_and_run_main_module(u8"checkpoint-instance",run);REQUIRE(value==42u);
    value=0u;run.entry_function_index=6u;
    lib::full_compile_and_run_main_module(u8"checkpoint-instance",run);REQUIRE(value==42u);
    lib::reset_runtime_state_host_api();::uwvm2::uwvm::cmdline::wasm_file_ppos=nullptr;
    ::fast_io::io::println("actual recorded WASI data/rebindings, negative authority tests and group/global sharing after two restores PASS");
}
#include <uwvm2/uwvm/runtime/macro/pop_macros.h>
#include <uwvm2/utils/macro/pop_macros.h>
