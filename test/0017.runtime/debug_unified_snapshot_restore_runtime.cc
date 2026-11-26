// Two consecutive unified archive restores of a real stopped LLVM instance.
// GC cycles, EH, memory64, table64, SIMD and segment state are serialized and
// decoded before the old native workers retire and new continuations execute.
#include "mandatory_native_fixture.h"
#include <uwvm2/uwvm/run/owned_source.h>
#include <uwvm2/uwvm/cmdline/callback/impl.h>
#include <uwvm2/utils/control/debug_permissions.h>
#include <uwvm2/utils/control/owned_file_image.h>
#include <uwvm2/utils/allocator/resource_limit.h>
#include <uwvm2/runtime/checkpoint/materialization.h>
#include <uwvm2/runtime/gc/entry_admission.h>
#include <uwvm2/uwvm/debugger/checkpoint_codec.h>
#include <uwvm2/runtime/diagnostic/snapshot_state.h>
#include <uwvm2/utils/container/standard.h>
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
    ::fast_io::print(::fast_io::err(), "debug_unified_snapshot_restore_runtime FAIL line=", ::fast_io::mnp::dec(line), "\n");
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
        if(!self.collecting || location.function != 1u || self.ticket) { return; }
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
        // selected references before the twentieth callback, then executes the delay loop.
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
    if(argc != 2) { return 64; }

    namespace budget = ::uwvm2::utils::allocator::resource;
    constexpr auto initializer_pool{static_cast<unsigned>(budget::pool::initializer)};
    constexpr auto initializer_bytes{1uz << 20u};
    budget::policies[initializer_pool] = {initializer_bytes, initializer_bytes};
    constexpr auto auxiliary_pool{static_cast<unsigned>(budget::pool::auxiliary)};
    constexpr auto auxiliary_bytes{16uz << 20u};
    budget::policies[auxiliary_pool] = {auxiliary_bytes, auxiliary_bytes};
    auto const& auxiliary_live{budget::live_bytes[auxiliary_pool]};
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
    setup.path = ::uwvm2::utils::container::u8concat(::fast_io::mnp::code_cvt(::fast_io::mnp::os_c_str(argv[1])));
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
    // Explicit pure Core policy: no visible WASIp1 environment to omit.
    ::uwvm2::uwvm::wasm::storage::local_preload_wasip1=false;
    auto bytes{image::owned_file_image::read(setup.path, 1048576u)}; REQUIRE(bytes);
    auto const original{bytes.image->bytes()}; REQUIRE(original.size() >= 8u && original.size() <= PTRDIFF_MAX);
    setup.immutable = ::std::move(bytes.image);
    auto const before_schema{auxiliary_live.load()};
    REQUIRE(lib::replace_full_source_after_drain_host_api(source_setup::prepare, ::std::addressof(setup)) &&
        setup.ready && setup.source && setup.source->file().has_owned_source_image());
    auto const schema_bytes{auxiliary_live.load() - before_schema};
    auto state{::std::make_shared<observer>()};

    auto profile{checkpoint::compilation_profile::create_for_trusted_observer()};
    REQUIRE(profile && profile->purpose() == checkpoint::compilation_purpose::observe_values);
    REQUIRE(lib::llvm_jit_configure_debug_session_host_api(state->control,
        {state, observer::point, observer::before_park}, lib::llvm_jit_debug_safe_point_granularity::instruction) ==
        lib::llvm_jit_debug_configure_result::ok);
    REQUIRE(lib::llvm_jit_configure_debug_value_observation_host_api(profile->limits()) == lib::llvm_jit_debug_configure_result::ok);
    REQUIRE(lib::llvm_jit_prepare_debug_host_api());
    lib::full_compile_run_config initialize{}; initialize.entry_function_index = 0u;
    lib::full_compile_and_run_main_module(u8"checkpoint-instance", initialize);
    { ::std::lock_guard lock{state->mutex}; state->collecting = true; }
    using outcome = lib::llvm_jit_checkpoint_native_retirement_status;
    lib::llvm_jit_checkpoint_prepare_request prepare_request{};prepare_request.recording_label[0u]=::std::byte{34u};
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
            lib::full_compile_run_config config{};config.entry_function_index=1u;
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
    auto saved=lib::llvm_jit_checkpoint_capture_instance_host_api(ticket,captures,prepare_request.recording_label,prepare_request.graph_budget);
    ::fast_io::io::perrln("CAPTURE status=",unsigned(saved.status)," data=",unsigned(saved.data_error));
    REQUIRE(saved.status==lib::llvm_jit_checkpoint_instance_capture_status::captured);
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
    // A host caller can bypass archive decoding, but it cannot make the runtime
    // silently ignore an event journal and execute the corresponding real I/O.
    auto journal=::std::make_shared<cp::state>(*prepare_request.saved_graph);
    cp::object pending_event{};pending_event.kind=cp::object_kind::event;
    pending_event.words[0u]=1u;pending_event.words[2u]=1u;pending_event.words[3u]=1u;
    journal->objects.push_back(pending_event);REQUIRE(cp::validate_graph(*journal)==cp::error::none);
    auto unhandled=prepare_request;unhandled.saved_graph=journal;
    auto rehearsal=lib::llvm_jit_checkpoint_prepare_instance_host_api(ticket,captures,unhandled);
    REQUIRE(rehearsal.status==lib::llvm_jit_checkpoint_prepare_status::resource_preparation_declined &&
        rehearsal.data_error==cp::error::non_replayable_import);
    auto refused=lib::llvm_jit_checkpoint_prepare_retire_instance_host_api(ticket,captures,unhandled,deadline());
    ::fast_io::io::perrln("REPLAY_REFUSAL status=",unsigned(refused.status),
        " prepare=",unsigned(refused.preparation.status)," data=",unsigned(refused.preparation.data_error));
    REQUIRE(refused.status==outcome::preparation_declined && !refused.operation &&
        refused.preparation.data_error==cp::error::non_replayable_import);
    REQUIRE(lib::observe_compiler_runtime_generation_host_api()==epoch &&
        state->control->wait_until_paused(ticket,deadline())==threads::cooperative_pause_result::paused);
    auto const predecessor_bytes{budget::live_bytes[initializer_pool].load()};
    unsigned allocation_diagnostic{};
    {
        auto const predecessor{full::selected_full_source_owner_pin()};
        REQUIRE(predecessor && predecessor_bytes < initializer_bytes);
        budget::reservation<budget::pool::initializer> concurrent{};
        REQUIRE(concurrent.resize(initializer_bytes - predecessor_bytes));
        auto const limited{lib::llvm_jit_checkpoint_prepare_retire_instance_host_api(ticket,captures,prepare_request,deadline())};
        REQUIRE(limited.status == outcome::preparation_declined && !limited.operation &&
            limited.preparation.status == lib::llvm_jit_checkpoint_prepare_status::resource_preparation_declined);
        REQUIRE(full::selected_full_source_owner_pin() == predecessor && lib::observe_compiler_runtime_generation_host_api() == epoch);
        REQUIRE(budget::live_bytes[initializer_pool].load() == initializer_bytes);
        allocation_diagnostic = limited.preparation.resource_diagnostic;
        REQUIRE(allocation_diagnostic != 0u);
    }
    REQUIRE(budget::live_bytes[initializer_pool].load() == predecessor_bytes);
    auto const predecessor_auxiliary{auxiliary_live.load()};
    {
        auto const predecessor{full::selected_full_source_owner_pin()};
        // The same schema, bridge and token records fit. Its new 1024-pointer
        // exception index must compete with the still-live original world.
        auto const allowance{schema_bytes + 1024uz};
        REQUIRE(predecessor && predecessor_auxiliary < auxiliary_bytes && allowance < auxiliary_bytes - predecessor_auxiliary);
        budget::reservation<budget::pool::auxiliary> concurrent{};
        REQUIRE(concurrent.resize(auxiliary_bytes - predecessor_auxiliary - allowance));
        auto const held{auxiliary_live.load()};
        auto const limited{lib::llvm_jit_checkpoint_prepare_retire_instance_host_api(ticket,captures,prepare_request,deadline())};
        REQUIRE(limited.status == outcome::preparation_declined && !limited.operation &&
            limited.preparation.status == lib::llvm_jit_checkpoint_prepare_status::resource_preparation_declined &&
            limited.preparation.resource_diagnostic == allocation_diagnostic);
        REQUIRE(auxiliary_live.load() == held && full::selected_full_source_owner_pin() == predecessor &&
            lib::observe_compiler_runtime_generation_host_api() == epoch);
    }
    REQUIRE(auxiliary_live.load() == predecessor_auxiliary);
    { ::std::lock_guard lock{state->mutex};state->collecting=false; }
    auto ready=lib::llvm_jit_checkpoint_prepare_retire_instance_host_api(ticket,captures,prepare_request,deadline());
    ::fast_io::io::perrln("PREPARE status=",unsigned(ready.status)," prepare=",unsigned(ready.preparation.status),
        " resource=",ready.preparation.resource_diagnostic," engine=",ready.preparation.engine_diagnostic," frame=",ready.preparation.frame_diagnostic);
    REQUIRE(ready.status==outcome::prepared_world_ready_closed && ready.preparation.data_error==cp::error::none);
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
    auto recaptured=lib::llvm_jit_checkpoint_capture_instance_host_api(ticket,captures,prepare_request.recording_label,prepare_request.graph_budget);
    ::fast_io::io::perrln("RECAPTURE_INSTANCE status=",unsigned(recaptured.status)," data=",unsigned(recaptured.data_error));
    encoded.clear();
    dumped=lib::runtime_dump_stopped_guest_host_api(ticket,published.runtime_epoch,"checkpoint-instance","second test",
        &encoded,writer,2u,2u,0u,captures);
    REQUIRE(dumped);REQUIRE(archive::decode(encoded,envelope));
    auto again=snapshot::decode(envelope,decoded,prepare_request.graph_budget);
    ::fast_io::io::perrln("REDECODE status=",unsigned(again.status)," state=",unsigned(again.state_error));
    REQUIRE(again && decoded);
    prepare_request.saved_graph=decoded;prepare_request.recording_label=decoded->recording_id;
    { ::std::lock_guard lock{state->mutex};state->collecting=false; }
    ready=lib::llvm_jit_checkpoint_prepare_retire_instance_host_api(ticket,captures,prepare_request,deadline());
    ::fast_io::io::perrln("REPREPARE status=",unsigned(ready.status)," prepare=",unsigned(ready.preparation.status),
        " data=",unsigned(ready.preparation.data_error)," resource=",ready.preparation.resource_diagnostic,
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
    ::std::uint32_t value{};lib::full_compile_run_config run{};run.entry_function_index=1u;
    run.entry_abi_buffers.result_buffer=reinterpret_cast<::std::byte*>(&value);run.entry_abi_buffers.result_bytes=sizeof(value);
    lib::full_compile_and_run_main_module(u8"checkpoint-instance",run);REQUIRE(value==42u);
    lib::reset_runtime_state_host_api();::uwvm2::uwvm::cmdline::wasm_file_ppos=nullptr;
    ::fast_io::io::println("actual GC/EH/memory64 instance restore and two physical worker joins PASS");
}
#include <uwvm2/uwvm/runtime/macro/pop_macros.h>
#include <uwvm2/utils/macro/pop_macros.h>
