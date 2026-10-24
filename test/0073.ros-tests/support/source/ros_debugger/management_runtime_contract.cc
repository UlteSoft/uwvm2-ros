/** Genuine owning-source LLVM-full management/ROS lifecycle witness.
 *
 * Unlike management_contract.cc/host_retirement_contract.cc, every principal
 * here is created by an actual generated Wasm business import. All three
 * modules are parsed and initialized by prepare_owned_full_cli_source(true).
 * Replacement uses the production controller's real prepare/commit/cleanup
 * path, never a test entry pointer, synthetic call identity or direct hook.
 *
 * The Unix peer and queued event are explicitly LOCAL Host fixtures installed
 * only after the real Guest created its registration. They prove retirement
 * closes/discards existing local resources before the actual controller reply;
 * they are not a ROS Gateway, DDS/TCPROS delivery or official-suite witness.
 *
 * Root must link the genuine current LLVM-only Runtime/provider/Host closure,
 * retain full MD/link/ELF evidence and execute in a bounded owned Linux process.
 * Source authoring alone is not a compile, replacement or active-reset result.
 */
// These feature macros have balanced, header-local push/pop scopes. Keep an
// outer fixture scope so the requirements below observe the real configured
// backend, thread and exception capabilities after those headers return.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/run/owned_source.h>
#include <uwvm2/uwvm/debugger/controller.h>
#include <uwvm2/uwvm/imported/ros/host.h>
#include "management_test_policy.h"
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <thread>
#include <fast_io.h>
#include <fast_io_unit/string.h>

#if !defined(UWVM_RUNTIME_LLVM_JIT) || !defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) || !defined(UWVM_CPP_EXCEPTIONS)
#error genuine LLVM-full native-thread Runtime with C++ exception support required
#endif
#if !defined(UWVM_IMPORT_ROS1) && !defined(UWVM_IMPORT_ROS2)
#error actual enabled builtin ROS business registry required
#endif

namespace lib=uwvm2::runtime::lib;
namespace mode=uwvm2::uwvm::runtime::runtime_mode;
namespace store=uwvm2::uwvm::wasm::storage;
namespace runtime_store=uwvm2::uwvm::runtime::storage;
namespace dbg=uwvm2::uwvm::debugger;
namespace control=uwvm2::utils::control;
namespace core=uwvm2::imported::ros;
namespace ros=uwvm2::uwvm::imported::ros;
namespace host=ros::host;
namespace ipc=core::ipc;
namespace management=core::management;
using clock_type=std::chrono::steady_clock;
static unsigned checks{},commits{},rejections{};
static void check(bool ok,char const* text) noexcept
{
    ++checks;
    if(!ok) {fast_io::io::perrln("FAIL_ROS_LLVM_MANAGEMENT ",fast_io::mnp::os_c_str(text));fast_io::fast_terminate();}
}
#if defined(__linux__)
namespace
{
    struct owned_directory
    {
        std::string path;
        owned_directory()
        {
            auto root=std::getenv("TMPDIR");check(root&&root[0]=='/',"Root supplies owned absolute TMPDIR");
            path=fast_io::concat_std(fast_io::mnp::os_c_str(root),"/llvm-management-XXXXXX");
            check(::mkdtemp(path.data())!=nullptr,"fresh bounded native fixture directory");
        }
        owned_directory(owned_directory const&)=delete;
        ~owned_directory()
        {
            for(auto name:{"value9.body","invalid.body","active.body","socket"})
            {auto item=fast_io::concat_std(path,"/",fast_io::mnp::os_c_str(name));::unlink(item.c_str());}
            ::rmdir(path.c_str());
        }
        template<std::size_t N> std::string body(char const* name,std::array<unsigned char,N> const& bytes)
        {
            auto file=fast_io::concat_std(path,"/",fast_io::mnp::os_c_str(name));
            int fd=::open(file.c_str(),O_WRONLY|O_CREAT|O_EXCL|O_NOFOLLOW|O_CLOEXEC,0400);
            check(fd>=0,"exclusive replacement input");std::size_t offset{};
            while(offset!=N)
            {
                // [fixed native-owned source body][offset,N) writable file
                // cursor. No input-sized allocation or Guest address is used.
                auto wrote=::write(fd,bytes.data()+offset,N-offset);
                if(wrote<0&&errno==EINTR) {continue;}
                check(wrote>0&&static_cast<std::size_t>(wrote)<=N-offset,"bounded replacement body write");
                offset+=static_cast<std::size_t>(wrote);
            }
            check(::fsync(fd)==0&&::close(fd)==0,"stable closed replacement body");return file;
        }
    };
    struct eof_peer
    {
        int listener{-1};std::string path;std::atomic_bool eof{};std::thread thread;
        explicit eof_peer(std::string directory,unsigned backend):path(fast_io::concat_std(directory,"/socket"))
        {
            listener=::socket(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0);sockaddr_un address{};address.sun_family=AF_UNIX;
            check(listener>=0&&path.size()<sizeof(address.sun_path),"bounded actual Unix listener");
            std::memcpy(address.sun_path,path.c_str(),path.size()+1);
            auto mask=::umask(0177);auto bound=::bind(listener,reinterpret_cast<sockaddr*>(&address),sizeof(address));::umask(mask);
            check(bound==0&&::chmod(path.c_str(),0600)==0&&::listen(listener,1)==0,"private owner-only local endpoint");
            thread=std::thread{[this,backend]
            {
                // A peer-thread failure terminates this bounded child; it never
                // races the main thread's diagnostic assertion counter.
                auto need=[](bool value) {if(!value) {fast_io::fast_terminate();}};
                pollfd ready{listener,POLLIN,0};need(::poll(&ready,1,60000)==1);
                int accepted=::accept4(listener,nullptr,nullptr,SOCK_CLOEXEC|SOCK_NONBLOCK);need(accepted>=0);
                ipc::frame hello;need(ipc::read_frame(accepted,hello,ipc::after(60000))&&hello.type==ipc::kind::hello&&
                    hello.backend==backend&&hello.request_id==1&&hello.session_id==0&&hello.payload.empty());
                ipc::frame reply;reply.type=ipc::kind::response;reply.backend=static_cast<std::uint8_t>(backend);
                reply.response_id=hello.request_id;reply.session_id=400+backend;reply.timeout_ms=60000;
                need(ipc::write_frame(accepted,reply,ipc::after(60000)));
                pollfd closed{accepted,POLLIN,0};need(::poll(&closed,1,60000)==1);
                unsigned char forbidden{};eof.store(::recv(accepted,&forbidden,1,MSG_DONTWAIT)==0,std::memory_order_release);
                ::close(accepted);
            }};
        }
        void join() {thread.join();check(eof.load(std::memory_order_acquire),"real session peer saw EOF, not an RPC");}
        ~eof_peer() {if(thread.joinable()) {fast_io::fast_terminate();}if(listener>=0) {::close(listener);}::unlink(path.c_str());}
    };
    dbg::controller_reply command(dbg::controller& owner,std::string const& text)
    {
        return owner.execute(dbg::parse_console_command(fast_io::string_view{text.data(),text.size()}),std::chrono::seconds{20});
    }
    std::uint32_t invoke(uwvm2::utils::container::u8string_view name,std::size_t function,
                         std::optional<std::uint32_t> argument={})
    {
        std::uint32_t result{0xffffffffu};lib::full_compile_run_config run{};run.entry_function_index=function;
        // Exact native ABI buffers remain owned until this synchronous real
        // generated entry returns. They do not supply a Runtime identity.
        run.entry_abi_buffers.result_buffer=reinterpret_cast<std::byte*>(&result);run.entry_abi_buffers.result_bytes=sizeof(result);
        if(argument) {run.entry_abi_buffers.param_buffer=reinterpret_cast<std::byte const*>(&*argument);run.entry_abi_buffers.param_bytes=sizeof(*argument);}
        lib::full_compile_and_run_main_module(name,run);return result;
    }
    void outside_denied()
    {
        lib::ros_caller_identity actual{};
        check(!lib::ros_caller_identity_host_api(actual)&&!actual.runtime_instance&&!actual.generation,
              "native manager is not a Guest principal");
    }
    void configure_policy(core::version selected)
    {
        check(host::instances.empty(),"fresh process has no inherited Host state");
        auto policy=ros_management_test::policy(selected);
        for(auto name:{"management-main","management-other","management-cold"})
        {
            check(bool(ros::runtime_policy.create_target(selected,core::scope::single,name)),"explicit native launch target");
            for(auto const& row:policy.connection)
            {check(bool(ros::runtime_policy.set_connection(selected,core::scope::single,name,row.first,row.second)),"actual finite connection schema");}
            check(bool(ros::runtime_policy.grant(selected,core::scope::single,name,policy.capabilities.at("sub")))&&
                  bool(ros::runtime_policy.set_enabled(selected,core::scope::single,name,true)),"finite explicit business grant only");
        }
        std::array<std::string,3> names{"management-main","management-other","management-cold"};
        check(bool(ros::runtime_policy.validate(names)),"real launch policy validates; global/default targets remain denied");
    }
    struct prepared
    {
        std::shared_ptr<dbg::controller> owner;void const* main{};void const* other{};void const* cold{};
        std::size_t main_id{},other_id{},cold_id{};std::uint64_t epoch{};
    };
    prepared prepare(std::u8string const& input)
    {
        mode::global_runtime_mode=mode::runtime_mode_t::full_compile;
        mode::global_runtime_compiler=mode::runtime_compiler_t::llvm_jit_only;
        mode::global_runtime_llvm_jit_exception_dispatch=mode::runtime_llvm_jit_exception_dispatch_t::native_unwind;
        mode::global_runtime_llvm_jit_call_stack=mode::runtime_llvm_jit_call_stack_t::instruction;
        mode::global_runtime_compile_threads=0;mode::runtime_compile_threads_existed=true;
        mode::global_runtime_llvm_jit_cache_path_mode=mode::runtime_llvm_jit_cache_path_mode_t::disabled;
        auto& args=uwvm2::uwvm::cmdline::parsing_result;uwvm2::uwvm::cmdline::wasm_file_ppos=nullptr;args.clear();
        args.emplace_back(uwvm2::utils::cmdline::parameter_parsing_results{u8"actual-llvm-management",nullptr,
            uwvm2::utils::cmdline::parameter_parsing_results_type::dir});
        args.emplace_back(uwvm2::utils::cmdline::parameter_parsing_results{
            uwvm2::utils::container::u8cstring_view{fast_io::mnp::os_c_str(input.c_str())},nullptr,
            uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg});
        // Retire the old cursor before vector mutation above; publish only
        // the final owning argv allocation. Input lives through both epochs.
        uwvm2::uwvm::cmdline::wasm_file_ppos=std::addressof(args.back());
        store::execute_wasm.module_name=u8"management-main";store::preloaded_wasm.clear();store::preload_local_imported.clear();
        store::preloaded_wasm.reserve(2);
        for(std::u8string_view name:{std::u8string_view{u8"management-other"},std::u8string_view{u8"management-cold"}})
        {
            auto& file=store::preloaded_wasm.emplace_back();
            file.file_name=uwvm2::utils::container::u8cstring_view{fast_io::mnp::os_c_str(input.c_str())};file.module_name=uwvm2::utils::container::u8string_view{name.data(),name.size()};
            file.wasm_parameter=store::wasm_parameter;
        }
        check(uwvm2::uwvm::run::prepare_owned_full_cli_source(true)==static_cast<int>(uwvm2::uwvm::run::retval::ok),
              "genuine immutable main plus two independently parsed owning preloads");
        control::launch_config config{};config.debug_enabled=true;config.replacement_enabled=true;
        config.compiler=control::backend::llvm;config.mode=control::compile_mode::full;config.origin=control::launch_origin::console;
        config.instance[0]=1;config.vm_process=static_cast<std::uint64_t>(::getpid());
        prepared result{};result.owner=dbg::controller::create(config,1);
        check(result.owner&&result.owner->status()==control::error::none,"actual trusted native controller, no Guest control ABI");
        check(lib::llvm_jit_configure_debug_session_host_api(result.owner->domain(),result.owner->observer(),
            lib::llvm_jit_debug_safe_point_granularity::instruction)==lib::llvm_jit_debug_configure_result::ok,
            "actual controller observer installed before publication");
        check(lib::llvm_jit_prepare_debug_host_api()&&lib::ros_execution_backend_supported_host_api(),"actual LLVM-only full native publication");
        // The owning CLI deferred active segments during initialize_runtime(true).
        // Full validation/publication above must precede their real installation,
        // in the same preload-then-main order as run_initialized_module_graph.
        // These source-bound fixtures have no Wasm start sections to invoke.
        for(auto const& module:store::active_preloaded_wasm())
        { ::uwvm2::uwvm::runtime::initializer::apply_runtime_active_segments(module.module_name); }
        ::uwvm2::uwvm::runtime::initializer::apply_runtime_active_segments(store::active_execute_wasm().module_name);
        auto& registry=runtime_store::active_runtime_registry();check(registry.size()==3,"exact actual three-module initialized registry");
        result.main=std::addressof(registry.at(u8"management-main"));result.other=std::addressof(registry.at(u8"management-other"));
        result.cold=std::addressof(registry.at(u8"management-cold"));result.epoch=lib::observe_compiler_runtime_generation_host_api();
        unsigned matched{};
        for(std::size_t id{};id!=registry.size();++id)
        {
            auto view=lib::llvm_jit_full_source_publication_host_api(id);
            check(view.canonical_source&&view.initialized_source&&view.ready&&view.engine_address&&view.runtime_epoch==result.epoch,
                  "genuine current owning LLVM engine/source observation for each assigned module");
            // Diagnostic addresses only match actual native registry members.
            // They never authorize an import, frame, mutation or publication.
            if(view.module_address==reinterpret_cast<std::uintptr_t>(result.main)) {result.main_id=id;++matched;}
            if(view.module_address==reinterpret_cast<std::uintptr_t>(result.other)) {result.other_id=id;++matched;}
            if(view.module_address==reinterpret_cast<std::uintptr_t>(result.cold)) {result.cold_id=id;++matched;}
        }
        check(matched==3&&result.main_id!=result.other_id&&result.main_id!=result.cold_id&&result.other_id!=result.cold_id,
              "actual assigned module IDs, never assumed preload ordering");
        check(result.owner->arm_initial_pause()&&command(*result.owner,"continue").status==control::error::none,"open genuine launch pause");
        outside_denied();return result;
    }
    dbg::controller_reply wait_for_stop(prepared const& p,std::atomic_bool const& finished,std::size_t function)
    {
        auto deadline=clock_type::now()+std::chrono::seconds{20};
        for(;;)
        {
            auto value=p.owner->inspect();
            if(value.execution==dbg::execution_status::stopped&&value.reason==dbg::stop_reason::breakpoint)
            {
                check(value.threads.size()==1&&value.threads[0].location.code_unit==p.main_id&&
                    value.threads[0].location.function==function&&value.threads[0].location.offset==0&&value.stop_identifier,
                    "same actual all-participant emitted before-nop stop");return value;
            }
            check(!finished.load(std::memory_order_acquire)&&clock_type::now()<deadline,"finite actual Guest stop");std::this_thread::yield();
        }
    }
    std::uint64_t handle(prepared const& p,std::size_t offset)
    {
        std::array<std::byte,8> raw{};check(lib::llvm_jit_debug_read_memory_host_api(p.main_id,0,offset,raw.data(),raw.size()),
            "actual current stopped Guest memory handle");std::uint64_t result{};
        for(unsigned i{};i!=8;++i) {result|=static_cast<std::uint64_t>(std::to_integer<unsigned>(raw[i]))<<(8*i);}
        check(result!=0,"actual Guest-created handle is nonzero");return result;
    }
    std::shared_ptr<host::instance> state(void const* object,std::uint64_t epoch)
    {
        std::lock_guard lock{host::mutex};auto found=host::instances.find(object);
        check(found!=host::instances.end()&&found->second->generation==epoch&&!found->second->retired,
              "real business caller created current Host registration");return found->second;
    }
    bool authorized(void const* object,std::uint64_t epoch,core::version backend,std::uint64_t old)
    {core::authorization lease;return bool(host::capabilities.describe(object,epoch,backend,old,lease));}
    void queue_fixture(host::instance& owner,void const* key,std::uint64_t epoch,core::version backend,std::uint64_t old)
    {
        auto& session=owner.backends[static_cast<unsigned>(backend)-1];std::lock_guard lock{session.mutex};
        core::debug::trace_record identity{};check(host::capabilities.debug_identity(key,epoch,backend,"sub",identity),"actual capability diagnostic label");
        ipc::event_header binding{};binding.backend=static_cast<std::uint8_t>(backend);binding.instance=identity.instance;
        binding.session=session.connection.session_id();binding.source_handle=old;
        host::watched_event event{};event.source=session.target->capabilities.at("sub");event.observer=event.source;
        event.observer.alias="local_test_observer";event.source_handle=old;event.instance=identity.instance;event.requests.insert(17);
        event.local=std::make_shared<ipc::event_stream>(ipc::event_stream_settings{3,196,68,1,68,ipc::event_overflow::backpressure},
            binding,ipc::event_bit(ipc::event_kind::topic_message));
        check(event.local->enqueue({ipc::event_kind::topic_message,0,0,0,{1,0,0,0}})==0&&
              event.local->enqueue({ipc::event_kind::topic_message,0,0,0,{2,0,0,0}})==0,"real local data and pending queue fixtures");
        auto data=event.local->statistics();
        check(data.depth>0&&data.bytes>0&&data.pending_depth>0&&data.pending_bytes>0,
              "actual queued and pending payload storage are nonempty before retirement");
        check(session.observations.emplace(91,std::move(event)).second,"local queue attached only to existing real Guest owner");
        // A separate stream is essential: the backpressure fixture above has
        // already consumed its one data slot and owns pending data, so reserve
        // must refuse there. This real empty, configured response stream owns
        // one nonzero asynchronous reservation without completing/cancelling it.
        host::watched_event reserved{};reserved.source=session.target->capabilities.at("sub");reserved.observer=reserved.source;
        reserved.observer.alias="local_reserved_observer";reserved.source_handle=old;reserved.instance=identity.instance;
        reserved.requests.insert(23);
        reserved.local=std::make_shared<ipc::event_stream>(ipc::event_stream_settings{3,512,128,0,0,ipc::event_overflow::reject},
            binding,ipc::event_bit(ipc::event_kind::service_response));
        check(reserved.local->configured()&&reserved.local->reserve(23,128,32)==0,
              "actual response reservation owns nonzero bytes before asynchronous effects");
        auto credit=reserved.local->statistics();
        check(credit.reserved_depth==1&&credit.reserved_bytes==160&&credit.depth==0&&credit.pending_depth==0,
              "genuine uncompleted reservation is distinct from data and pending storage");
        check(session.observations.emplace(92,std::move(reserved)).second,"reservation belongs to the same real Guest-created owner");
    }
    struct observation
    {
        void const* target{};std::uint64_t epoch{},first{},second{};core::version backend{};
        unsigned replacements{},resets{};bool old_epoch{},target_retired{},cleared{},drained_business_close{},resources_retired{};
    };
    observation observed{};
    void observe_retirement(management::transition reason,void const* object,std::uint64_t epoch) noexcept
    {
        // This static native observer holds no Runtime lease/publication/domain
        // lock and forwards the production hook unconditionally. Host locks and
        // authorization leases are released before invoking the real cleanup.
        observed.old_epoch=lib::observe_compiler_runtime_generation_host_api()==epoch&&epoch==observed.epoch;
        if(reason==management::transition::runtime_retirement)
        {
            ++observed.resets;
            observed.drained_business_close=object==nullptr&&authorized(observed.target,epoch,observed.backend,observed.first)&&
                !authorized(observed.target,epoch,observed.backend,observed.second);
        }
        else {++observed.replacements;}
        host::retire_authority(reason,object,epoch);
        {
            std::lock_guard lock{host::mutex};
            if(reason==management::transition::runtime_retirement) {observed.cleared=host::instances.empty();}
            else if(object==observed.target)
            {
                auto found=host::instances.find(object);observed.target_retired=found!=host::instances.end()&&found->second->retired;
                if(found!=host::instances.end())
                {
                    auto& session=found->second->backends[static_cast<unsigned>(observed.backend)-1];std::lock_guard backend_lock{session.mutex};
                    observed.resources_retired=!session.connected&&session.connection.session_id()==0&&session.observations.size()==2;
                    for(auto const& row:session.observations)
                    {
                        if(!row.second.local) {observed.resources_retired=false;continue;}
                        auto value=row.second.local->statistics();
                        observed.resources_retired=observed.resources_retired&&row.second.retired&&row.second.requests.empty()&&
                            !row.second.local->active()&&value.depth==0&&value.bytes==0&&value.pending_depth==0&&value.pending_bytes==0&&
                            value.reserved_depth==0&&value.reserved_bytes==0;
                    }
                }
            }
        }
        observed.old_epoch=observed.old_epoch&&lib::observe_compiler_runtime_generation_host_api()==epoch;
    }
    void finite_reset()
    {
        // The native management thread owns NO execution entry, imported call,
        // stopped-domain guard, Host lock or authorization lease. A real active
        // Guest is released by close_runtime_debug_control then actually drained
        // by the central Runtime before our old-epoch callback is reached.
        std::mutex mutex;std::condition_variable changed;bool returned{};
        std::thread watchdog{[&]
        {std::unique_lock lock{mutex};if(!changed.wait_for(lock,std::chrono::seconds{30},[&]{return returned;})) {fast_io::fast_terminate();}}};
        lib::reset_runtime_state_host_api();{std::lock_guard lock{mutex};returned=true;}changed.notify_one();watchdog.join();
    }
}
#endif

int main(int argc,char** argv)
{
#if !defined(__linux__)
    return 77;
#else
    if(argc!=3||(std::strcmp(argv[1],"ros1")!=0&&std::strcmp(argv[1],"ros2")!=0)) {return 2;}
    auto backend=std::strcmp(argv[1],"ros1")==0?core::version::ros1:core::version::ros2;
#if !defined(UWVM_IMPORT_ROS1)
    check(backend!=core::version::ros1,"matching enabled ROS1 native Runtime required");
#endif
#if !defined(UWVM_IMPORT_ROS2)
    check(backend!=core::version::ros2,"matching enabled ROS2 native Runtime required");
#endif
    auto input=fast_io::u8concat_std(fast_io::mnp::code_cvt(fast_io::mnp::os_c_str(argv[2])));
    check(!input.empty()&&input[0]==u8'/',"actual absolute assembled fixture");configure_policy(backend);
    check(management::retirement_hook.load()==&host::retire_authority,"original genuine production Host hook installed");
    owned_directory work;
    // Complete function bodies, not whole Wasm modules or entry pointers.
    // Root must cross-check the two VALID constants against actual wat2wasm
    // Code-section bodies from management_runtime_replacement.wat first.
    auto valid=work.body("value9.body",std::array<unsigned char,4>{0,0x41,9,0x0b});
    auto invalid=work.body("invalid.body",std::array<unsigned char,3>{0,0xff,0x0b});
    auto active=work.body("active.body",std::array<unsigned char,3>{0,0x01,0x0b});
    auto p=prepare(input);check(invoke(u8"management-main",3)==42&&invoke(u8"management-other",3)==42,"real Guest opens on two distinct instances");
    auto main_state=state(p.main,p.epoch),other_state=state(p.other,p.epoch);
    {std::lock_guard lock{host::mutex};check(host::instances.size()==2&&!host::instances.contains(p.cold),"cold module has never created Host state");}
    for(auto function:{5u,6u})
    {check(command(*p.owner,fast_io::concat_std("breakwasm ",p.main_id," ",function," 0")).status==control::error::none,"actual emitted breakpoint");}
    std::atomic_bool finished{};std::uint32_t result{};
    std::thread guest{[&]{result=invoke(u8"management-main",7);finished.store(true,std::memory_order_release);}};
    auto first=wait_for_stop(p,finished,5);auto original_handle=handle(p,72);
    auto reject=[&](std::size_t function,std::uint64_t generation,std::string const& file)
    {
        auto reply=command(*p.owner,fast_io::concat_std("replace ",p.main_id," ",function," ",generation," ",file));
        check(reply.status!=control::error::none&&p.owner->inspect().stop_identifier==first.stop_identifier,
              "actual rejected replacement does not acknowledge commit or retire the stop");++rejections;
        check(!main_state->retired&&authorized(p.main,p.epoch,backend,original_handle)&&!other_state->retired,
              "rejected preparation/active-stack commit preserves both actual authorities");
    };
    reject(2,1,invalid);reject(2,9,valid);reject(5,1,active);
    check(command(*p.owner,"continue").status==control::error::none,"resume after actual refused publications");
    auto second=wait_for_stop(p,finished,6);
    check(second.stop_identifier!=first.stop_identifier&&authorized(p.main,p.epoch,backend,original_handle),
          "actual unchanged Guest functions and original value7 ran between real stops");
    // Retain a genuine private ready transaction from the SAME original
    // generation. The controller's later successful publication makes this
    // actual token stale. It is not a fabricated pointer/metadata ticket.
    std::array<std::byte,4> const private_body{std::byte{0},std::byte{0x41},std::byte{9},std::byte{0x0b}};
    auto pending=lib::llvm_jit_debug_prepare_function_replacement_host_api(p.main_id,2,1,private_body.data(),private_body.size());
    check(pending.status==lib::llvm_jit_debug_replace_status::replaced&&pending.transaction&&
          !main_state->retired&&authorized(p.main,p.epoch,backend,original_handle),
          "actual private ready compilation changes no published code or authority");
    eof_peer peer(work.path,static_cast<unsigned>(backend));auto& session=main_state->backends[static_cast<unsigned>(backend)-1];
    check(session.connection.connect(peer.path,static_cast<std::uint8_t>(backend),static_cast<std::uint32_t>(::getuid()),60000)==0,
          "real credential-checked local session fixture");session.connected=true;session.timeout=60000;
    queue_fixture(*main_state,p.main,p.epoch,backend,original_handle);
    observed={p.main,p.epoch,0,0,backend};auto production=management::retirement_hook.exchange(&observe_retirement);
    check(production==&host::retire_authority,"native observer wraps only the original real Host hook");
    auto committed=command(*p.owner,fast_io::concat_std("replace ",p.main_id," 2 1 ",valid));
    check(committed.status==control::error::none&&committed.replacement_generation==2&&observed.replacements==1&&
          observed.old_epoch&&observed.target_retired&&observed.resources_retired&&main_state->retired,
          "actual successful publication retires target/resources inside the hook before controller acknowledgement");++commits;
    peer.join();auto stats=session.observations.at(91).local->statistics();
    check(!session.connected&&session.connection.session_id()==0&&session.observations.at(91).retired&&
          session.observations.at(91).requests.empty()&&!session.observations.at(91).local->active()&&
          stats.depth==0&&stats.pending_depth==0&&stats.reserved_depth==0&&stats.bytes==0&&stats.pending_bytes==0&&stats.reserved_bytes==0,
          "old real descriptor/pending identities/local queues are empty before trusted ack is consumed");
    auto& reserved=session.observations.at(92);auto credits=reserved.local->statistics();
    check(reserved.retired&&reserved.requests.empty()&&!reserved.local->active()&&credits.reserved_depth==0&&credits.reserved_bytes==0&&
          credits.depth==0&&credits.bytes==0&&credits.pending_depth==0&&credits.pending_bytes==0,
          "the previously nonzero actual reservation and every data category were discarded");
    check(!authorized(p.main,p.epoch,backend,original_handle)&&!other_state->retired,"stale handle denied, unrelated module untouched");
    auto stale=command(*p.owner,fast_io::concat_std("info ros capabilities backend=all module=management-main if-stop=",second.stop_identifier));
    check(stale.status!=control::error::none||stale.ros_state_values.result==core::debug::status::stale_stop,
          "actual controller refuses the previous stop reference after replacement");
    auto current=p.owner->inspect();check(current.execution==dbg::execution_status::stopped&&current.stop_identifier!=second.stop_identifier,
          "replacement retains actual paused Guest but advances canonical stop identity");
    auto cold=command(*p.owner,fast_io::concat_std("replace ",p.cold_id," 2 1 ",valid));
    check(cold.status==control::error::none&&cold.replacement_generation==2&&observed.replacements==2,"real publication also retires a never-opened module");++commits;
    {std::lock_guard lock{host::mutex};check(!host::instances.contains(p.cold),"cold replacement allocates no Host registration");}
    management::retirement_hook.store(production);
    check(command(*p.owner,"continue").status==control::error::none,"resume real original caller into value9 and denied unchanged ROS functions");
    auto deadline=clock_type::now()+std::chrono::seconds{20};while(!finished.load(std::memory_order_acquire))
    {check(clock_type::now()<deadline,"finite actual post-commit Guest completion");std::this_thread::yield();}guest.join();
    check(result==42,"actual unchanged functions deny old/new authority and execute value9");
    // With the genuine old Guest joined, the actual domain has zero active
    // participants. Acquire a REAL empty-cohort ticket; no controller pause
    // ticket is forged or borrowed. The one bounded commit runs under its
    // actual while_stopped mutex and must reject the prepared old generation.
    auto empty=p.owner->domain()->request_pause();
    check(bool(empty)&&p.owner->domain()->wait_until_paused(empty,clock_type::now()+std::chrono::seconds{20})==
          uwvm2::utils::thread::cooperative_pause_result::paused,"actual empty cohort maintenance admission");
    lib::llvm_jit_debug_replace_result stale_commit{};
    check(p.owner->domain()->while_stopped(empty,[&]{stale_commit=lib::llvm_jit_debug_commit_function_replacement_host_api(pending.transaction);})&&
          stale_commit.status==lib::llvm_jit_debug_replace_status::stale_generation&&stale_commit.generation==2,
          "genuine private transaction fails actual commit after another real publication");
    lib::llvm_jit_debug_discard_function_replacement_host_api(pending.transaction);++rejections;
    check(main_state->retired&&!authorized(p.main,p.epoch,backend,original_handle)&&!other_state->retired&&
          !session.connected&&session.observations.at(91).requests.empty(),
          "failed actual commit neither resurrects target resources nor retires unrelated authority");
    check(p.owner->domain()->resume(empty),"release actual empty maintenance cohort");
    check(invoke(u8"management-other",8)==42,"actual unrelated module keeps existing/new handles and original code after failed commit");
    check(invoke(u8"management-cold",2)==9&&invoke(u8"management-cold",4,13)==42,"cold actual code changed but first ROS open is refused with untouched output");
    {std::lock_guard lock{host::mutex};check(!host::instances.contains(p.cold),"permanent Runtime flag closes first-Host-state loophole");}
    outside_denied();auto replaced_epoch=p.epoch;finite_reset();
    check(lib::observe_compiler_runtime_generation_host_api()==replaced_epoch+1&&host::instances.empty(),"genuine completed incarnation reset clears all old owners");
    main_state.reset();other_state.reset();p.owner.reset();

    // A new explicit trusted native launch owns fresh records and reuses the
    // same finite launch policy. No regrant/reset occurs in the retired record.
    p=prepare(input);check(invoke(u8"management-main",3)==42,"new actual incarnation can use its explicit launch grant");
    main_state=state(p.main,p.epoch);
    check(command(*p.owner,fast_io::concat_std("breakwasm ",p.main_id," 5 0")).status==control::error::none,"actual active-reset breakpoint");
    finished.store(false,std::memory_order_release);result=0;
    guest=std::thread{[&]{result=invoke(u8"management-main",9);finished.store(true,std::memory_order_release);}};
    wait_for_stop(p,finished,5);observed={p.main,p.epoch,handle(p,64),handle(p,72),backend};
    check(authorized(p.main,p.epoch,backend,observed.first)&&authorized(p.main,p.epoch,backend,observed.second),
          "genuine active Guest entered reset with two real authorized handles");
    production=management::retirement_hook.exchange(&observe_retirement);check(production==&host::retire_authority,"real reset hook selected");
    auto reset_epoch=p.epoch;outside_denied();finite_reset();
    deadline=clock_type::now()+std::chrono::seconds{20};while(!finished.load(std::memory_order_acquire))
    {check(clock_type::now()<deadline,"actual entry drained and returned after domain close");std::this_thread::yield();}guest.join();
    check(result==42&&observed.resets==1&&observed.old_epoch&&observed.drained_business_close&&observed.cleared&&
          lib::observe_compiler_runtime_generation_host_api()==reset_epoch+1&&host::instances.empty(),
          "actual active business close completed before old-epoch Host cleanup and epoch publication");
    check(!authorized(p.main,reset_epoch,backend,observed.first)&&!authorized(p.main,reset_epoch,backend,observed.second),
          "old current-runtime handles stay denied after real active drain");
    check(p.owner->domain()->is_closed()&&p.owner->inspect().execution==dbg::execution_status::closed,"real reset closed the old controller domain");
    management::retirement_hook.store(production);main_state.reset();p.owner.reset();outside_denied();
    finite_reset();check(host::instances.empty()&&lib::observe_compiler_runtime_generation_host_api()==reset_epoch+2,
          "restored production hook and repeated genuine reset inherit no authority");
    fast_io::io::println("PASS_ROS_LLVM_MANAGEMENT backend=",static_cast<unsigned>(backend)," commits=",commits,
        " rejections=",rejections," active_resets=1 checks=",checks," scope=native-runtime-local-host-not-ros-network");return 0;
#endif
}
#include <uwvm2/uwvm/runtime/macro/pop_macros.h>
#include <uwvm2/utils/macro/pop_macros.h>
