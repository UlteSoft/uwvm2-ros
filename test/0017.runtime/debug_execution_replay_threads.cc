// Record and replay a real native-worker LLVM cohort twice. The supplied pure
// Wasm module selects shared-memory/atomic wait scenarios. All old native
// workers must physically retire before the replacement is published.
#include "mandatory_native_fixture.h"
#if defined(UWVM_TEST_REPLAY_ALLOCATION_FAILURE)
# include "fast_io_allocation_probe.h"
#endif
#include <uwvm2/uwvm/run/owned_source.h>
#include <uwvm2/uwvm/cmdline/callback/impl.h>
#include <uwvm2/utils/control/debug_permissions.h>
#include <uwvm2/utils/control/owned_file_image.h>
#include <uwvm2/runtime/checkpoint/materialization.h>
#include <uwvm2/runtime/gc/entry_admission.h>
#include <uwvm2/uwvm/debugger/checkpoint_codec.h>
#include <uwvm2/uwvm/debugger/checkpoint_execution_replay.h>
#include <uwvm2/runtime/diagnostic/snapshot_state.h>
#include <fast_io.h>
#include <fast_io_unit/string.h>
#include <atomic>
#include <array>
#include <barrier>
#include <cstddef>
#include <cstdint>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#if defined(__linux__)
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
// Test-only access to the actual registry lock. No product API exposes it.
namespace uwvm2::runtime::lib { extern ::std::mutex checkpoint_replay_mutex; }
#endif
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
#include "execution_replay_ros_fixture.h"
#include "execution_replay_wasi_fixture.h"
static auto deadline() { return ::std::chrono::steady_clock::now() + ::std::chrono::seconds{180}; }
static ::std::atomic<unsigned> returned_native_entries{};
static ::std::atomic<unsigned> returned_worker_slot{UINT_MAX};
static ::std::atomic<bool> release_returned_callback{};
static ::std::atomic<bool> returned_tls_entered{},release_returned_tls{};
struct native_tls_return_delay
{
    bool enabled{};
    ~native_tls_return_delay()
    {
        if(!enabled)return;
        returned_tls_entered.store(true);returned_tls_entered.notify_all();
        while(!release_returned_tls.load())release_returned_tls.wait(false);
    }
};
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
            if(loaded != ::uwvm2::uwvm::wasm::loader::load_wasm_file_rtl::ok)return false;
            if(replay_ros_fixture || replay_wasi_fixture)
            {
                if(!::uwvm2::uwvm::imported::ros::validate_loaded_policy(true) ||
                   !::uwvm2::uwvm::imported::ros::host::preflight() ||
                   ::uwvm2::uwvm::run::load_local_modules()!=0)return false;
            }
            if(::uwvm2::uwvm::wasm::loader::construct_all_module_and_check_duplicate_module() !=
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
    ::std::size_t worker_count;
    ::std::shared_ptr<domain> control;
    explicit observer(::std::size_t count):worker_count{count},control{::std::make_shared<domain>(count)}{}
    ::std::mutex mutex{};::std::condition_variable changed{};
    domain::pause_ticket ticket{};
    ::std::vector<::std::uint64_t> participants{};
    ::std::vector<::std::size_t> participant_points{};
    ::std::vector<lib::llvm_jit_checkpoint_thread_capture_owner> captures{};
    ::std::vector<::std::uint_least64_t> capture_participants{};
    ::std::size_t points{},threshold{200u};bool replay{},intermediate_done{},await_return{},require_both_progress{};
    bool intermediate{::std::getenv("UWVM_TEST_INTERMEDIATE_PAUSE")!=nullptr};
    bool import_pause{::std::getenv("UWVM_TEST_IMPORT_PAUSE")!=nullptr};
    ::std::uint64_t import_pause_instruction{};
    ::std::size_t unavailable_import_captures{};
    static void point(void* opaque,::std::uint_least64_t id,threads::cooperative_pause_location location) noexcept
    {
        auto& self=*static_cast<observer*>(opaque);::std::lock_guard lock{self.mutex};
        if(location.function!=replay_worker_function || self.ticket)return;
        auto found=::std::find(self.participants.begin(),self.participants.end(),id);
        if(found==self.participants.end())
        {self.participants.push_back(id);self.participant_points.push_back(1u);}
        else ++self.participant_points[static_cast<::std::size_t>(found-self.participants.begin())];
        ++self.points;
        bool const progress=!self.require_both_progress || ::std::all_of(self.participant_points.begin(),self.participant_points.end(),[](auto count){return count>=100u;});
        auto const journal=lib::llvm_jit_checkpoint_replay_status_host_api();
        bool ready=self.replay?(journal.mode==4u || (!self.intermediate_done &&
            ((self.import_pause && self.import_pause_instruction && journal.instructions>=self.import_pause_instruction) ||
             (self.intermediate && self.points>=25u)))) : self.points>=self.threshold && self.participants.size()==self.worker_count && progress && (!self.await_return || returned_native_entries.load()==1u);
        if(ready)
        {
            if(self.replay)self.intermediate_done=true;
            if(!self.replay)replay_wasi_input_drain.store(true,::std::memory_order_release);
            self.ticket=self.control->request_pause();REQUIRE(self.ticket);self.changed.notify_all();
        }
    }
    static void before_park(void* opaque,::std::uint_least64_t participant,threads::cooperative_pause_location,lib::llvm_jit_debug_local_view) noexcept
    {
        auto& self=*static_cast<observer*>(opaque);::std::lock_guard lock{self.mutex};REQUIRE(self.ticket);
        auto saved=lib::llvm_jit_checkpoint_capture_thread_host_api(self.ticket);
        if(saved.status!=lib::llvm_jit_checkpoint_capture_status::captured)
        {
            REQUIRE(self.import_pause && self.replay && self.intermediate_done && !saved.capture);
            ++self.unavailable_import_captures;
        }
        // A real participant may re-park in the same requested pause. Retain
        // its latest authentic capture instead of counting the callback twice.
        auto found=::std::find(self.capture_participants.begin(),self.capture_participants.end(),participant);
        if(found==self.capture_participants.end())
        {self.capture_participants.push_back(participant);self.captures.push_back(saved.capture);}
        else self.captures[static_cast<::std::size_t>(found-self.capture_participants.begin())]=saved.capture;
        self.changed.notify_all();
    }
    static bool restored(void* opaque,domain const* control,domain::pause_ticket const& ticket,::std::uint_least64_t) noexcept
    {
        auto& self=*static_cast<observer*>(opaque);::std::lock_guard lock{self.mutex};
        if(control!=self.control.get() || !ticket)return false;
        self.ticket=ticket;self.captures.clear();self.capture_participants.clear();self.participants.clear();self.participant_points.clear();self.points=0u;self.intermediate_done=false;self.changed.notify_all();return true;
    }
    auto stopped(::std::size_t expected=0u)
    {
        if(!expected)expected=worker_count;
        domain::pause_ticket saved{};
        {::std::unique_lock lock{mutex};REQUIRE(changed.wait_until(lock,deadline(),[&]{return bool(ticket);}));saved=ticket;}
        REQUIRE(control->wait_until_paused(saved,deadline())==threads::cooperative_pause_result::paused);
        ::std::lock_guard lock{mutex};REQUIRE(captures.size()==expected);
        return ::std::pair{saved,captures};
    }
    void resume(bool playing)
    {
        domain::pause_ticket saved{};
        {::std::lock_guard lock{mutex};saved=ticket;ticket={};captures.clear();capture_participants.clear();points=0u;::std::fill(participant_points.begin(),participant_points.end(),0u);replay=playing;}
        replay_wasi_input_drain.store(false,::std::memory_order_release);
        REQUIRE(control->resume(saved));
    }
};
int main(int argc, char** argv)
{
    if(!::uwvm2test::mandatory_native::arguments(argc, argv)) { return 64; }
    if(argc != 2 && argc != 3) { return 64; }
    bool const worker_exit=::std::getenv("UWVM_TEST_WORKER_EXIT")!=nullptr;
    ::std::size_t worker_count{2u};
    if(auto text=::std::getenv("UWVM_TEST_WORKERS"))
    {
        char* end{};auto const parsed=::std::strtoul(text,&end,10);
        // This is a stress-fixture bound, not a product worker-count limit.
        REQUIRE(text!=end && !*end && parsed>=2u && parsed<=32u);
        worker_count=parsed;REQUIRE(!worker_exit || worker_count==2u);
    }
    if(::std::getenv("UWVM_TEST_EXPECT_ALLOCATOR"))
    {
        REQUIRE(!::uwvm2::object::memory::linear::native_memory_t::can_mmap);
        ::fast_io::io::perrln("MEMORY actual allocator backend; no mmap");
    }

    REQUIRE(::uwvm2::utils::control::debug_permissions::configure_trusted_launch(
        ::uwvm2::utils::control::debug_permissions::permission_level::full));
    if(auto value=::std::getenv("UWVM_TEST_MEMTAG_BACKEND"))
    {
        REQUIRE(!::std::strcmp(value,"software") || !::std::strcmp(value,"hardware"));
        ::uwvm2::extension::memtag::select_backend(!::std::strcmp(value,"hardware"));
    }
    if(auto value=::std::getenv("UWVM_TEST_MEMTAG_DELIVERY"))
    {
        REQUIRE(!::std::strcmp(value,"sync") || !::std::strcmp(value,"async"));
        ::uwvm2::extension::memtag::select_delivery(!::std::strcmp(value,"async"));
    }
    if(auto value=::std::getenv("UWVM_TEST_MEMTAG_REPORT"))
    {
        REQUIRE(!::std::strcmp(value,"crash") || !::std::strcmp(value,"log"));
        ::uwvm2::extension::memtag::selected_report_action.store(!::std::strcmp(value,"log")?
            ::uwvm2::extension::memtag::report_action::silent_log:
            ::uwvm2::extension::memtag::report_action::crash,::std::memory_order_relaxed);
    }
    configure_replay_ros_fixture();
    configure_replay_wasi_fixture();
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
    auto native_path = ::fast_io::u8concat_std(::fast_io::mnp::code_cvt(::fast_io::mnp::os_c_str(argv[1])));
    setup.path.assign(native_path.data(),native_path.size());
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
    REQUIRE(lib::replace_full_source_after_drain_host_api(source_setup::prepare, ::std::addressof(setup)) &&
        setup.ready && setup.source && setup.source->file().has_owned_source_image());
    if(auto backend=::std::getenv("UWVM_TEST_MEMTAG_BACKEND");backend && !::std::strcmp(backend,"hardware"))
    {
        ::std::size_t checked{};
        for(auto const& entry:setup.source->registry())for(auto const& local:entry.second.local_defined_memory_vec_storage)
        {
            if(!local.memory.memtag_shadow.enabled())continue;
            // Log-only policy must permit the diagnosed access to continue.
            // It keeps the hardware backend selection but does not arm a
            // synchronous native tag fault on each known mismatch.
            REQUIRE(local.memory.memtag_shadow.hardware_active()==
                ::uwvm2::extension::memtag::hardware_checked_access_requested());++checked;
        }
        REQUIRE(checked!=0u);
        if(::uwvm2::extension::memtag::hardware_checked_access_requested())
            ::fast_io::io::perrln("MTE actual hardware mappings=",checked);
        else ::fast_io::io::perrln("MTE hardware log-only policy mappings=",checked);
    }
    replay_worker_function=setup.source->registry().begin()->second.imported_function_vec_storage.size()+1u;
    if(::std::getenv("UWVM_TEST_REQUIRE_MTE"))
    {
        ::std::size_t tagged{};
        for(auto const& entry:setup.source->registry())for(auto const& local:entry.second.local_defined_memory_vec_storage)
            if(local.memory.memtag_shadow.enabled())++tagged;
        REQUIRE(tagged);::fast_io::io::perrln("MTE actual active mappings=",tagged);
    }
    auto state{::std::make_shared<observer>(worker_count)};

    auto profile{checkpoint::compilation_profile::create_for_trusted_observer()};
    REQUIRE(profile && profile->purpose() == checkpoint::compilation_purpose::observe_values);
    REQUIRE(lib::llvm_jit_configure_debug_session_host_api(state->control,
        {.context=state, .on_safe_point=observer::point, .on_before_park=observer::before_park, .on_restored_pause=observer::restored}, lib::llvm_jit_debug_safe_point_granularity::instruction) ==
        lib::llvm_jit_debug_configure_result::ok);
    REQUIRE(lib::llvm_jit_configure_debug_value_observation_host_api(profile->limits()) == lib::llvm_jit_debug_configure_result::ok);
    REQUIRE(lib::llvm_jit_prepare_debug_host_api());
    lib::full_compile_run_config initialize{}; initialize.entry_function_index = replay_worker_function-1u;
    lib::full_compile_and_run_main_module(u8"checkpoint-instance", initialize);

    using outcome = lib::llvm_jit_checkpoint_native_retirement_status;
    lib::llvm_jit_checkpoint_prepare_request prepare_request{};prepare_request.recording_label[0u]=::std::byte{34u};
    if(replay_wasi_fixture)
    {
        // The native API requires the same explicit joint-environment choice
        // as the console. These tests bind the actual current stdin/stdout;
        // they must not omit the environment or pretend pipes are managed files.
        prepare_request.include_wasip1=true;
        prepare_request.require_managed_wasip1_resources=false;
    }
    auto const epoch{lib::observe_compiler_runtime_generation_host_api()};
    REQUIRE(epoch != 0u);


    auto start=::std::make_shared<::std::barrier<>>(worker_count+1u);
    struct launch
    {
        ::std::shared_ptr<::std::barrier<>> start;
        unsigned slot{};
        static void body(void* p) noexcept
        {
            auto& self=*static_cast<launch*>(p);self.start->arrive_and_wait();
            ::std::uint32_t value{};lib::full_compile_run_config config{};config.entry_function_index=replay_worker_function;
            config.entry_abi_buffers.result_buffer=reinterpret_cast<::std::byte*>(&value);config.entry_abi_buffers.result_bytes=sizeof(value);
            lib::full_compile_and_run_main_module(u8"checkpoint-instance",config);
            returned_worker_slot.store(self.slot);returned_native_entries.fetch_add(1u);
            if(::std::getenv("UWVM_TEST_BETWEEN_ENTRIES"))
                while(!release_returned_callback.load())release_returned_callback.wait(false);
            if(::std::getenv("UWVM_TEST_WORKER_TLS"))
            {
                static thread_local native_tls_return_delay delay{};
                delay.enabled=true;
            }
        }
    };
    ::std::vector<lib::llvm_jit_debug_guest_worker_owner> workers(worker_count);
    for(unsigned n{};n<worker_count;++n)
    {
        auto body=::std::make_shared<launch>();body->start=start;body->slot=n;
        auto actual=lib::runtime_launch_llvm_jit_debug_guest_worker_host_api(state->control,body,launch::body);
        REQUIRE(actual.status==lib::llvm_jit_debug_guest_worker_launch_status::started);workers[n]=actual.worker;
    }
    start_replay_wasi_input();start->arrive_and_wait();
    auto [ticket,captures]=state->stopped();
    auto report_count=[]() noexcept
    {
        namespace reports=::uwvm2::runtime::memtag::report;
        ::std::lock_guard lock{reports::output_mutex};return reports::next_sequence;
    };
    auto const reports_origin=report_count();
    auto begin=lib::llvm_jit_checkpoint_record_host_api(ticket,captures,prepare_request.recording_label,1u);
    ::fast_io::io::perrln("RECORD start=",unsigned(begin.status)," data=",unsigned(begin.data_error));
    REQUIRE(begin.status==lib::llvm_jit_checkpoint_instance_capture_status::captured);
#if defined(__linux__)
    if(::std::getenv("UWVM_TEST_REPLAY_FORK"))
    {
        // A fork child inherits locked mutex bytes, never the parent's native
        // workers. Rejection must precede every journal or pause-domain lock.
        lib::checkpoint_replay_mutex.lock();
        auto child=::fork();
        if(child==0)
        {
            ::signal(SIGALRM,[](int){::_exit(124);});::alarm(10);
            auto observed=lib::llvm_jit_checkpoint_replay_status_host_api();
            auto pending=lib::llvm_jit_checkpoint_capture_pending_host_api(ticket);
            bool accepted=lib::llvm_jit_checkpoint_validate_pending_host_api(ticket,{});
            ::_exit(observed.mode || pending.retained || pending.graph || accepted ? 1 : 0);
        }
        lib::checkpoint_replay_mutex.unlock();
        REQUIRE(child>=0);
        int child_status{};REQUIRE(::waitpid(child,&child_status,0)==child);
        REQUIRE(WIFEXITED(child_status)&&WEXITSTATUS(child_status)==0);
        REQUIRE(lib::llvm_jit_checkpoint_replay_status_host_api().mode==1u);
    }
#endif
    if(argc==3)state->threshold=::std::strtoul(argv[2],nullptr,10);
    state->await_return=worker_exit;
    state->require_both_progress=replay_ros_fixture || replay_wasi_fixture || worker_count>2u ||
        ::std::getenv("UWVM_TEST_ALL_WORKERS_PROGRESS")!=nullptr;
#if defined(UWVM_TEST_REPLAY_ALLOCATION_FAILURE)
    // The allocation probe uses non-blocking worker loops. Ensure each real
    // worker contributed to the journal before testing seal/retry; OS fairness
    // is not a Wasm guarantee and one worker can reach the threshold alone.
    if(::std::getenv("UWVM_TEST_SEAL_FAILURE"))state->require_both_progress=true;
#endif
    state->resume(false);::std::tie(ticket,captures)=state->stopped(worker_exit?1u:worker_count);
    if(::std::getenv("UWVM_TEST_RECORD_STEP"))
    {
        // Real debugger stepping while all peer workers stay parked. A queued
        // recording turn must not remain owned by a non-running peer.
        REQUIRE(!worker_exit && !replay_wasi_fixture && !replay_ros_fixture);
        auto const identifiers=state->capture_participants;
        for(auto participant:identifiers)
        {
            auto const before=lib::llvm_jit_checkpoint_replay_status_host_api();
            unsigned attempts{};
            for(;;)
            {
                REQUIRE(state->control->release_one_to_next_poll(ticket,participant));
                REQUIRE(state->control->wait_until_paused(ticket,::std::chrono::steady_clock::now()+::std::chrono::seconds{10})==
                    threads::cooperative_pause_result::paused);
                auto const after=lib::llvm_jit_checkpoint_replay_status_host_api();
                REQUIRE(after.mode==1u && after.instructions>=before.instructions);
                if(after.instructions>before.instructions)break;
                REQUIRE(++attempts<64u);
            }
        }
        ::std::tie(ticket,captures)=state->stopped();
        ::fast_io::io::perrln("RECORD individual-step participants=",identifiers.size()," peers parked PASS");
    }
#if defined(UWVM_TEST_REPLAY_ALLOCATION_FAILURE)
    // Link the test with --wrap=malloc,--wrap=calloc,--wrap=realloc. The real
    // runtime allocator is exercised; no injection branch is added to the VM.
    if(auto selection=::std::getenv("UWVM_TEST_SEAL_FAILURE"))
    {
        auto const before=lib::llvm_jit_checkpoint_replay_status_host_api();
        fast_io_allocation_probe::remaining=::std::strtoul(selection,nullptr,10);
        fast_io_allocation_probe::once=true;fast_io_allocation_probe::armed=true;
        auto attempted=lib::llvm_jit_checkpoint_record_host_api(ticket,captures,prepare_request.recording_label,2u);
        fast_io_allocation_probe::armed=false;
        auto const after=lib::llvm_jit_checkpoint_replay_status_host_api();
        ::fast_io::io::perrln("SEAL allocation probe=",::fast_io::mnp::os_c_str(selection)," failures=",fast_io_allocation_probe::failures,
            " result=",unsigned(attempted.status)," mode-before=",before.mode," mode-after=",after.mode,
            " events-before=",before.events," events-after=",after.events);
        if(attempted.status!=lib::llvm_jit_checkpoint_instance_capture_status::captured)
        {
            REQUIRE(!attempted.graph && before.mode==after.mode && before.instructions==after.instructions &&
                before.events==after.events && before.cursor==after.cursor && before.error==after.error);
        }
        // A rejected capture must retry normally. The remaining fixture then
        // decodes and replays that exact successfully sealed journal twice.
    }
#endif
    auto end=lib::llvm_jit_checkpoint_record_host_api(ticket,captures,prepare_request.recording_label,2u);
    ::fast_io::io::perrln("RECORD end=",unsigned(end.status)," data=",unsigned(end.data_error));
    REQUIRE(end.status==lib::llvm_jit_checkpoint_instance_capture_status::captured && end.graph);
    auto const reports_recorded=report_count()-reports_origin;
    if(::std::getenv("UWVM_TEST_REQUIRE_REPORTS"))
    {
        REQUIRE(::uwvm2::extension::memtag::logging_requested() && reports_recorded);
        ::fast_io::io::perrln("MTE recorded violations=",reports_recorded);
    }
    stop_replay_wasi_input();
    cp::execution_replay::decoded decoded{};REQUIRE(cp::execution_replay::decode(*end.graph,decoded)==cp::error::none);
    REQUIRE(decoded.thread_count==worker_count);
    if(state->import_pause)
    {
        // Choose an actual recorded interval where one worker remains inside
        // a host call while another executes at least 16 real Wasm opcodes.
        // Stop in its middle, well before its recorded return can be consumed.
        ::std::vector<::std::uint64_t> entered(worker_count,UINT64_MAX);
        ::std::uint64_t longest{};
        for(auto const& event:decoded.events)
        {
            auto const index=event.words[2]-1u;
            if(event.flags==cp::execution_replay::import_begin_kind)entered[index]=event.words[1];
            if(event.flags==cp::execution_replay::import_finish_kind && entered[index]!=UINT64_MAX)
            {
                auto const span=event.words[1]-entered[index];
                if(span>longest){longest=span;state->import_pause_instruction=entered[index]+span/2u;}
                entered[index]=UINT64_MAX;
            }
        }
        REQUIRE(longest>=16u && state->import_pause_instruction);
        ::fast_io::io::perrln("IMPORT pause instruction=",state->import_pause_instruction," interval=",longest);
    }
    if(::std::getenv("UWVM_TEST_REQUIRE_REPORTS"))
    {
        auto const journaled=::std::count_if(decoded.events.begin(),decoded.events.end(),[](auto const& event)
            {return event.flags==cp::execution_replay::import_memtag_report_kind;});
        ::fast_io::io::perrln("MTE journaled host violations=",journaled);
        REQUIRE(static_cast<::std::uint64_t>(journaled)==reports_recorded);
    }
    if(worker_exit)
    {
        REQUIRE(returned_native_entries.load()==1u);
        REQUIRE(::std::count_if(decoded.events.begin(),decoded.events.end(),[](auto const& event)
            {return event.flags==cp::execution_replay::thread_exit_kind;})==1);
        ::fast_io::io::perrln("EXIT genuine native entry returned; one survivor captured");
    }
    ::std::size_t pending{},wait_begins{},wait_ends{},interleaved{};
    for(auto const& item:decoded.origin.objects)if(item.kind==cp::object_kind::atomic_wait)++pending;
    for(auto const& event:decoded.events)
    {
        if(event.flags==cp::execution_replay::wait_begin_kind)++wait_begins;
        if(event.flags==cp::execution_replay::wait_end_kind)
        {++wait_ends;if(event.words[7]!=UINT64_MAX && event.words[0]>event.words[7]+1u)++interleaved;}
    }
    ::fast_io::io::perrln("WAITS origin=",pending," began=",wait_begins," completed=",wait_ends," interleaved=",interleaved);

    ::std::vector<::std::size_t> turns(worker_count);
    for(auto const& event:decoded.events)if(event.flags==cp::execution_replay::instruction_kind)++turns[event.words[2]-1u];
    for(auto const& item:decoded.origin.objects)if(item.kind==cp::object_kind::thread)
        REQUIRE(turns[item.words[0]-1u] || item.flags==2u);
    if(::std::getenv("UWVM_TEST_REQUIRE_ENTROPY"))
    {
        ::std::vector<::std::size_t> samples(worker_count);
        for(auto const& event:decoded.events)
            if(event.flags==cp::execution_replay::entropy_kind)++samples[event.words[2]-1u];
        REQUIRE(::std::all_of(samples.begin(),samples.end(),[](auto count){return count!=0u;}));
        ::fast_io::io::perrln("MTE actual random-tag entropy recorded for all ",worker_count," workers");
    }
    auto expected=lib::llvm_jit_checkpoint_capture_instance_host_api(ticket,captures,prepare_request.recording_label,prepare_request.graph_budget);
    REQUIRE(expected.graph);
    auto memory=[](cp::state const& graph)
    {
        struct memory_image
        {
            ::std::uint64_t address_bits{};
            ::std::vector<::std::byte> bytes{},tags{};
            bool operator==(memory_image const&) const=default;
        };
        ::std::vector<memory_image> images;
        for(::std::size_t index{};index<graph.objects.size();++index)
        {
            auto const& owner=graph.objects[index];
            if(owner.kind!=cp::object_kind::memory)continue;
            REQUIRE(owner.words[1]<=SIZE_MAX/65536u);
            memory_image image{owner.words[0],::std::vector<::std::byte>(owner.words[1]*65536u),
                ::std::vector<::std::byte>(owner.bytes.begin(),owner.bytes.end())};
            // The archive intentionally omits zero pages. Reconstruct their
            // real extent and preserve each memory's identity/order instead of
            // concatenating only stored pages (which would miss zero growth).
            for(auto const& chunk:graph.objects)
                if(chunk.kind==cp::object_kind::memory_chunk && chunk.links.size()==1u && chunk.links[0]==index+1u)
                {
                    REQUIRE(chunk.words[0]<=image.bytes.size() && chunk.bytes.size()<=image.bytes.size()-chunk.words[0]);
                    ::std::copy(chunk.bytes.begin(),chunk.bytes.end(),image.bytes.begin()+chunk.words[0]);
                }
            images.push_back(::std::move(image));
        }
        return images;
    };
    auto expected_bytes=memory(*expected.graph);REQUIRE(!expected_bytes.empty());
    if(::std::getenv("UWVM_TEST_WASI_GROW"))
    {
        auto const original=memory(decoded.origin);
        REQUIRE(original.size()==1u && expected_bytes.size()==1u);
        ::fast_io::io::perrln("GROW observed origin bytes=",original[0].bytes.size()," endpoint bytes=",expected_bytes[0].bytes.size());
        REQUIRE(original[0].bytes.size()==65536u && expected_bytes[0].bytes.size()==3u*65536u);
        ::fast_io::io::perrln("GROW recorded shared memory grew from one to three pages");
    }
    prepare_request.saved_graph=::std::make_shared<cp::state>(decoded.origin);
    prepare_request.replay_graph=end.graph;prepare_request.pause_after_restore=true;
    if(pending)
    {
        for(auto field:{0u,2u,4u,5u,6u,7u})
        {
            auto tampered=::std::make_shared<cp::state>(decoded.origin);
            auto wait=::std::find_if(tampered->objects.begin(),tampered->objects.end(),[](auto const& object){return object.kind==cp::object_kind::atomic_wait;});
            REQUIRE(wait!=tampered->objects.end());
            wait->words[field]+=field==0u?4u:1u;
            auto request=prepare_request;request.saved_graph=tampered;request.replay_graph.reset();
            auto refused=lib::llvm_jit_checkpoint_prepare_retire_instance_host_api(ticket,captures,request,deadline());
            REQUIRE(refused.status==outcome::preparation_declined && !refused.operation);
            REQUIRE(state->control->capture(ticket).result==threads::cooperative_pause_result::paused);
        }
        ::fast_io::io::perrln("WAITS tampered opcode/queue bindings refused before retirement=6");
    }
    if(auto tamper=::std::getenv("UWVM_TEST_TAMPER_WAIT"))
    {
        bool const queue=!::std::strcmp(tamper,"queue"),offset=!::std::strcmp(tamper,"offset");
        REQUIRE(queue || offset || !::std::strcmp(tamper,"byte") || !::std::strcmp(tamper,"tag"));
        auto changed=::std::make_shared<cp::state>(*end.graph);
        auto event=::std::find_if(changed->objects.begin(),changed->objects.end(),[&](auto const& item)
        {
            return item.kind==cp::object_kind::event &&
                ((queue || offset)?item.flags==cp::execution_replay::wait_endpoint_kind:
                    (item.flags==cp::execution_replay::wait_begin_kind || item.flags==cp::execution_replay::wait_endpoint_kind)) &&
                item.bytes.size()==40u && (::std::strcmp(tamper,"tag") || (item.bytes[24]&::std::byte{1})!=::std::byte{});
        });
        REQUIRE(event!=changed->objects.end());
        if(queue)++event->values[4].low_bits;
        else if(offset)event->values[0].low_bits+=4u;
        else event->bytes[!::std::strcmp(tamper,"byte")?16u:25u]^=::std::byte{1};
        cp::execution_replay::decoded validated{};
        REQUIRE(cp::execution_replay::decode(*changed,validated)==cp::error::none);
        prepare_request.replay_graph=changed;
        // Structurally valid hostile DATA must reach the real execution guard
        // and exit 125 before consuming this wait, not fail a file checksum.
    }
    auto audit_counts=replay_ros_audit_counts();
    auto wasi_audit_counts=replay_wasi_audit_counts();
    auto const random_calls=::uwvm2::imported::wasi::wasip1::environment::process_budget_ledger.native_random_calls.load();
    auto const random_budget_calls=replay_wasi_random_budget_calls();
    if(replay_wasi_scalar_fixture)REQUIRE(random_calls && random_budget_calls);
    if(replay_wasi_io_fixture && !::std::getenv("UWVM_TEST_WASI_EMPTY"))REQUIRE(wasi_audit_counts.first);
    if(replay_ros_fixture || replay_wasi_fixture)
    {
        ::std::size_t began{},finished{},accesses{},overlaps{};
        for(auto const& event:decoded.events)
        {
            if(event.flags==cp::execution_replay::import_begin_kind)++began;
            if(event.flags==cp::execution_replay::import_memory_kind)++accesses;
            if(event.flags!=cp::execution_replay::import_finish_kind)continue;
            ++finished;
            for(auto n=event.words[7];n+1u<event.words[0];++n)
                if(decoded.events[n].flags==cp::execution_replay::instruction_kind && decoded.events[n].words[2]!=event.words[2]){++overlaps;break;}
        }
        ::fast_io::io::perrln(::fast_io::mnp::os_c_str(replay_ros_fixture?"ROS phases began=":"WASI phases began="),
            began," finished=",finished," memory=",accesses," concurrent=",overlaps);
        REQUIRE(began && began==finished && accesses && overlaps);
        if(replay_ros_fixture)REQUIRE(audit_counts.first && audit_counts.second);
    }
    if(auto tamper=::std::getenv("UWVM_TEST_TAMPER_HOST"))
    {
        REQUIRE(!::std::strcmp(tamper,"readonly") || !::std::strcmp(tamper,"failed") || !::std::strcmp(tamper,"begin"));
        auto changed=::std::make_shared<cp::state>(*end.graph);
        auto write=::std::find_if(changed->objects.begin(),changed->objects.end(),[](auto const& item)
        {return item.kind==cp::object_kind::event && item.flags==cp::execution_replay::import_memory_kind &&
            item.values.size()==5u && item.values[0].low_bits==2u && item.values[4].low_bits && item.bytes.size()>112u;});
        REQUIRE(write!=changed->objects.end());
        if(!::std::strcmp(tamper,"readonly"))write->values[0].low_bits=1u;
        else if(!::std::strcmp(tamper,"failed"))write->values[4].low_bits=0u;
        else
        {
            auto begin=::std::find_if(changed->objects.begin(),changed->objects.end(),[](auto const& item)
            {return item.kind==cp::object_kind::event && item.flags==cp::execution_replay::import_begin_kind;});
            REQUIRE(begin!=changed->objects.end() && begin->bytes.size()>=8u && write->bytes.size()>=8u);
            // Keep the real begin timestamp. Copying the later transaction's
            // timestamp would create an invalid journal order, testing only
            // the decoder instead of the immutable-begin execution boundary.
            ::std::array<::std::byte,8u> timestamp{};
            ::std::copy(begin->bytes.end()-8u,begin->bytes.end(),timestamp.begin());
            begin->bytes=write->bytes;
            ::std::copy(timestamp.begin(),timestamp.end(),begin->bytes.end()-8u);
        }
        cp::execution_replay::decoded validated{};REQUIRE(cp::execution_replay::decode(*changed,validated)==cp::error::none);
        prepare_request.replay_graph=changed;
        ::fast_io::io::perrln("TAMPER typed host-memory journal accepted for actual execution guard");
    }
    if(::std::getenv("UWVM_TEST_REQUIRE_REPORTS"))
    {
        namespace mt=::uwvm2::extension::memtag;
        // Simulate an archive selected on a crash-policy host while every Guest
        // is parked. The file must not change the host policy or retire workers.
        mt::selected_report_action.store(mt::report_action::crash);
        auto refused=lib::llvm_jit_checkpoint_prepare_retire_instance_host_api(ticket,captures,prepare_request,deadline());
        mt::selected_report_action.store(mt::report_action::silent_log);
        REQUIRE(refused.status==outcome::preparation_declined && !refused.operation &&
            refused.preparation.data_error==cp::error::unavailable_capability);
        REQUIRE(lib::observe_compiler_runtime_generation_host_api()==epoch);
        REQUIRE(state->control->capture(ticket).result==threads::cooperative_pause_result::paused);
        ::fast_io::io::perrln("MTE crash-policy replay refusal preserved original stop and generation");
    }
    decltype(prepare_request.saved_graph) middle_saved{};
    bool const between_entries=::std::getenv("UWVM_TEST_BETWEEN_ENTRIES")!=nullptr;
    bool const delayed_tls=::std::getenv("UWVM_TEST_WORKER_TLS")!=nullptr;
    REQUIRE(!between_entries || !delayed_tls);
    if(between_entries)
    {
        REQUIRE(worker_exit);
        // A returned guest entry does NOT attest the end of its enclosing host
        // callback. The actual callback is held above and may enter Wasm again;
        // it must prevent retirement rather than be mistaken for a dead worker.
        auto refused=lib::llvm_jit_checkpoint_prepare_retire_instance_host_api(ticket,captures,prepare_request,deadline());
        REQUIRE(refused.status==outcome::preparation_declined && !refused.operation);
        REQUIRE(lib::observe_compiler_runtime_generation_host_api()==epoch);
        REQUIRE(state->control->capture(ticket).result==threads::cooperative_pause_result::paused);
        release_returned_callback.store(true);release_returned_callback.notify_all();
        auto const slot=returned_worker_slot.load();REQUIRE(slot<workers.size());
        auto joined=lib::runtime_join_llvm_jit_debug_guest_worker_until_host_api(workers[slot],deadline());
        REQUIRE(joined.actual.status==::fast_io::thread_join_status::joined);
        ::fast_io::io::perrln("EXIT between-entry callback refused without retiring; genuine native join completed");
    }
    for(unsigned round{};round<2u;++round)
    {
        if(delayed_tls && !round)
            while(!returned_tls_entered.load())returned_tls_entered.wait(false);
        auto ready=lib::llvm_jit_checkpoint_prepare_retire_instance_host_api(ticket,captures,prepare_request,
            delayed_tls && !round?::std::chrono::steady_clock::now()+::std::chrono::milliseconds{250}:deadline());
        if(delayed_tls && !round)
        {
            REQUIRE(worker_exit && ready.operation);
            if(ready.status==outcome::pending_execution)
                ready=lib::llvm_jit_checkpoint_continue_native_retirement_host_api(ready.operation,
                    ::std::chrono::steady_clock::now()+::std::chrono::seconds{2});
            REQUIRE(ready.status==outcome::pending_native_join && ready.workers==2u);
            REQUIRE(lib::observe_compiler_runtime_generation_host_api()==epoch);
            release_returned_tls.store(true);release_returned_tls.notify_all();
            ready=lib::llvm_jit_checkpoint_continue_native_retirement_host_api(ready.operation,deadline());
            ::fast_io::io::perrln("EXIT pending native TLS join prevented publication until destructor returned");
        }
        ::fast_io::io::perrln("PREPARE round=",round," status=",unsigned(ready.status)," data=",unsigned(ready.preparation.data_error),
            " preparation=",unsigned(ready.preparation.status)," wasip1=",unsigned(ready.preparation.wasip1_status));
        REQUIRE(ready.status==outcome::prepared_world_ready_closed);
        if(worker_exit)REQUIRE(ready.workers==(between_entries && !round?1u:2u));
        auto published=lib::llvm_jit_checkpoint_publish_prepared_instance_host_api(ready.operation,deadline());
        REQUIRE(published.status==outcome::restored_execution_started);
        ::std::tie(ticket,captures)=state->stopped();
        auto restored_origin=lib::llvm_jit_checkpoint_capture_instance_host_api(ticket,captures,prepare_request.recording_label,prepare_request.graph_budget);
        REQUIRE(restored_origin.graph);
        auto actual_pending=::std::count_if(restored_origin.graph->objects.begin(),restored_origin.graph->objects.end(),
            [](auto const& object){return object.kind==cp::object_kind::atomic_wait;});
        REQUIRE(static_cast<::std::size_t>(actual_pending)==pending);
        auto const replay_reports_origin=report_count();
        state->resume(true);::std::tie(ticket,captures)=state->stopped(worker_exit?1u:worker_count);
        if(lib::llvm_jit_checkpoint_replay_status_host_api().mode!=4u)
        {
            REQUIRE(state->intermediate || state->import_pause);
            auto middle=lib::llvm_jit_checkpoint_capture_instance_host_api(ticket,captures,prepare_request.recording_label,prepare_request.graph_budget);
            if(state->import_pause)
            {
                // A parked virtual import is observable, but cannot be exported
                // as an ordinary pre-call continuation that repeats native I/O.
                REQUIRE(!middle.graph && middle.status!=lib::llvm_jit_checkpoint_instance_capture_status::captured);
                ::fast_io::io::perrln("IMPORT virtual call paused; ordinary executable capture refused");
            }
            else
            {
                REQUIRE(middle.graph);middle_saved=middle.graph;
                auto request=prepare_request;request.saved_graph=middle.graph;request.replay_graph.reset();
                auto candidate=lib::llvm_jit_checkpoint_prepare_instance_host_api(ticket,captures,request);
                REQUIRE(candidate.status==lib::llvm_jit_checkpoint_prepare_status::prepared_and_discarded);
                ::fast_io::io::perrln("INTERMEDIATE replay pause captured and candidate prepared");
            }
            state->resume(true);::std::tie(ticket,captures)=state->stopped();
        }
        auto actual=lib::llvm_jit_checkpoint_capture_instance_host_api(ticket,captures,prepare_request.recording_label,prepare_request.graph_budget);
        REQUIRE(actual.graph && memory(*actual.graph)==expected_bytes);
        auto status=lib::llvm_jit_checkpoint_replay_status_host_api();REQUIRE(status.mode==4u);
        if(::std::getenv("UWVM_TEST_REQUIRE_REPORTS"))
        {
            auto const delivered=report_count()-replay_reports_origin;
            ::fast_io::io::perrln("MTE replay round=",round," violations=",delivered," expected=",reports_recorded);
            REQUIRE(delivered==reports_recorded);
        }
        REQUIRE(replay_ros_audit_counts()==audit_counts);
        REQUIRE(replay_wasi_audit_counts()==wasi_audit_counts);
        REQUIRE(::uwvm2::imported::wasi::wasip1::environment::process_budget_ledger.native_random_calls.load()==random_calls);
        REQUIRE(replay_wasi_random_budget_calls()==random_budget_calls);
        ::fast_io::io::perrln("REPLAY round=",round," instructions=",status.instructions," events=",status.events," memory_equal=1");
    }
    if(middle_saved)
    {
        // A separate, explicit ordinary restore of the intermediate DATA must
        // rebuild its real native pending/completed wait; this continuation is
        // intentionally not described as deterministic journal replay.
        auto request=prepare_request;request.saved_graph=middle_saved;request.replay_graph.reset();
        auto ready=lib::llvm_jit_checkpoint_prepare_retire_instance_host_api(ticket,captures,request,deadline());
        REQUIRE(ready.status==outcome::prepared_world_ready_closed);
        REQUIRE(lib::llvm_jit_checkpoint_publish_prepared_instance_host_api(ready.operation,deadline()).status==outcome::restored_execution_started);
        ::std::tie(ticket,captures)=state->stopped();
        auto restored=lib::llvm_jit_checkpoint_capture_instance_host_api(ticket,captures,prepare_request.recording_label,prepare_request.graph_budget);
        REQUIRE(restored.graph && memory(*restored.graph)==memory(*middle_saved));
        state->resume(false);::std::tie(ticket,captures)=state->stopped();
        REQUIRE(lib::llvm_jit_checkpoint_replay_status_host_api().mode==0u);
        if(::std::getenv("UWVM_TEST_REQUIRE_PENDING_RETIRE"))
        {
            bool found{};
            for(unsigned attempt{};attempt<16u;++attempt)
            {
                auto live=lib::llvm_jit_checkpoint_capture_instance_host_api(ticket,captures,prepare_request.recording_label,prepare_request.graph_budget);
                REQUIRE(live.graph);
                found=::std::any_of(live.graph->objects.begin(),live.graph->objects.end(),
                    [](auto const& object){return object.kind==cp::object_kind::atomic_wait;});
                if(found)break;
                state->threshold=25u;state->resume(false);::std::tie(ticket,captures)=state->stopped();
            }
            REQUIRE(found);
            ::fast_io::io::perrln("RETIRE genuine native pending wait present in final all-stop");
        }
        ::fast_io::io::perrln("INTERMEDIATE native wait state restored and live continuation resumed");
    }
    // Physical retirement is separate from replay and must still join every OS
    // worker at the sealed endpoint, without executing another guest opcode.
    auto retired=lib::llvm_jit_checkpoint_retire_saved_native_workers_host_api(ticket,captures,deadline());
    REQUIRE(retired.status==outcome::retired_and_joined);
    if(worker_exit)REQUIRE(retired.workers==2u);
    lib::reset_runtime_state_host_api();::uwvm2::uwvm::cmdline::wasm_file_ppos=nullptr;
    if(worker_count==2u)::fast_io::io::println("two-worker shared-memory instruction replay PASS");
    else ::fast_io::io::println(worker_count,"-worker shared-memory instruction replay PASS");
}
#include <uwvm2/uwvm/runtime/macro/pop_macros.h>
#include <uwvm2/utils/macro/pop_macros.h>
