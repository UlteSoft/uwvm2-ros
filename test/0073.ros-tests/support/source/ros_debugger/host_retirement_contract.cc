/** Actual Host retirement, queue and Unix-client component.
 *
 * Link the genuine current Runtime closure, as for host_contract.cc; do not
 * stub a Runtime identity function. This native fixture explicitly installs
 * Host registry records and event streams, and uses actual local Unix sockets.
 * It never fabricates an active Runtime import context, Wasm memory or a ROS
 * Gateway. The accepting threads acknowledge only hello, then require EOF:
 * lifecycle retirement must close descriptors without issuing an RPC.
 *
 * The final fixture calls the genuine central Runtime reset twice. A static
 * native observation callback forwards the real Host hook and records that its
 * cleanup occurs before the real Runtime epoch advances. No active Guest or
 * compiler/provider callback is installed. Successful/failed function-body
 * publication and stopped-domain ordering still need the real integration lane.
 * Run only in a fresh bounded Linux process
 * with TMPDIR inside its owned output; external deadline/RSS/cleanup mandatory.
 */
#include <uwvm2/uwvm/imported/ros/host.h>
#include "management_test_policy.h"
#include <atomic>
#include <cstdlib>
#include <functional>
#include <thread>

#if !defined(UWVM_IMPORT_ROS1) && !defined(UWVM_IMPORT_ROS2)
# error select an actual ROS Host header profile
#endif
namespace host=uwvm2::uwvm::imported::ros::host;
namespace ros=uwvm2::imported::ros;
namespace ipc=ros::ipc;
namespace diagnostic=ros::debug;
namespace management=ros::management;
namespace runtime=uwvm2::runtime::lib;
static unsigned checks{};
static void check(bool condition,char const* detail)
{
    ++checks;
    if(!condition) {fast_io::io::perrln("FAIL_ROS_HOST_RETIREMENT ",fast_io::mnp::os_c_str(detail));fast_io::fast_terminate();}
}
#if defined(__linux__)
namespace
{
    struct endpoint
    {
        int descriptor{-1};std::string directory,path;
        endpoint()
        {
            auto temporary=std::getenv("TMPDIR");check(temporary&&temporary[0]=='/',"owned absolute TMPDIR required");
            directory=fast_io::concat_std(fast_io::mnp::os_c_str(temporary),"/host-retirement-XXXXXX");
            check(::mkdtemp(directory.data())!=nullptr,"create private socket directory");path=fast_io::concat_std(directory,"/socket");
            descriptor=::socket(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0);
            sockaddr_un address{};address.sun_family=AF_UNIX;
            check(descriptor>=0&&path.size()<sizeof address.sun_path,"bounded real Unix listener");
            std::memcpy(address.sun_path,path.c_str(),path.size()+1);
            // Both endpoints are constructed before either peer thread starts.
            // No process-wide umask change races a live fixture thread.
            auto previous=::umask(0177);auto result=::bind(descriptor,reinterpret_cast<sockaddr*>(&address),sizeof address);
            ::umask(previous);check(result==0&&::chmod(path.c_str(),0600)==0&&::listen(descriptor,1)==0,"owner-only bound socket");
        }
        endpoint(endpoint const&)=delete;
        ~endpoint() {if(descriptor>=0) {::close(descriptor);}::unlink(path.c_str());::rmdir(directory.c_str());}
    };
    void hello_then_eof(endpoint const& endpoint,unsigned backend,std::atomic<bool>& eof)
    {
        // Never modify the shared assertion counter from peer threads.
        auto fail=[] {fast_io::fast_terminate();};
        pollfd ready{endpoint.descriptor,POLLIN,0};if(::poll(&ready,1,3000)!=1) {fail();}
        int connection=::accept4(endpoint.descriptor,nullptr,nullptr,SOCK_CLOEXEC|SOCK_NONBLOCK);
        if(connection<0) {fail();}
        ipc::frame hello;
        if(!ipc::read_frame(connection,hello,ipc::after(3000))||hello.type!=ipc::kind::hello||
           hello.backend!=backend||hello.request_id!=1||hello.session_id!=0) {fail();}
        ipc::frame reply;reply.type=ipc::kind::response;reply.backend=static_cast<std::uint8_t>(backend);
        reply.response_id=hello.request_id;reply.session_id=40+backend;reply.timeout_ms=3000;
        if(!ipc::write_frame(connection,reply,ipc::after(3000))) {fail();}
        pollfd closed{connection,POLLIN,0};if(::poll(&closed,1,3000)!=1) {fail();}
        std::uint8_t forbidden_rpc{};auto received=::recv(connection,&forbidden_rpc,1,MSG_DONTWAIT);
        eof.store(received==0,std::memory_order_release);::close(connection);
    }
    std::shared_ptr<host::instance> install(void const* key,std::uint64_t epoch,char const* name,
        ros::target_policy const& one,ros::target_policy const& two,std::uint64_t (&handles)[2])
    {
        check(bool(host::capabilities.register_instance(key,epoch,name,&one,&two)),"register explicit native fixture owner");
        auto value=std::make_shared<host::instance>();value->generation=epoch;
        value->backends[0].target=&one;value->backends[1].target=&two;
        for(unsigned index{};index!=2;++index)
        {
            auto version=static_cast<ros::version>(index+1);
            check(bool(host::capabilities.open(key,epoch,version,"sub",handles[index])),"open explicit fixture source");
            diagnostic::trace_record identity{};
            check(host::capabilities.debug_identity(key,epoch,version,"sub",identity),"obtain actual registry instance label");
            ipc::event_header binding{};binding.backend=static_cast<std::uint8_t>(index+1);
            binding.instance=identity.instance;binding.session=40+index+1;binding.source_handle=handles[index];
            host::watched_event observation{};
            observation.source=value->backends[index].target->capabilities.at("sub");
            observation.observer=observation.source;observation.observer.alias="fixture_observer";
            observation.source_handle=handles[index];observation.instance=identity.instance;observation.requests.insert(17);observation.delivered=5;
            observation.local=std::make_shared<ipc::event_stream>(
                ipc::event_stream_settings{3,196,68,1,68,ipc::event_overflow::backpressure},binding,
                ipc::event_bit(ipc::event_kind::topic_message));
            check(observation.local->enqueue({ipc::event_kind::topic_message,0,0,0,{1,0,0,0}})==0&&
                  observation.local->enqueue({ipc::event_kind::topic_message,0,0,0,{2,0,0,0}})==0,
                  "actual queue contains one data record and one pending record");
            value->backends[index].observations.emplace(91+index,std::move(observation));
        }
        check(host::instances.emplace(key,value).second,"install explicit Host metadata, not a Runtime identity");return value;
    }

    struct reset_observation
    {
        unsigned calls{};
        std::uint64_t notified_epoch{};
        std::uint_least64_t epoch_before_cleanup{},epoch_after_cleanup{};
        std::size_t owners_before{},owners_after{};
        bool runtime_retirement{},null_object{},owners_match_notified_epoch{};
    };
    reset_observation actual_reset{};
    void observe_actual_reset(management::transition reason,void const* object,std::uint64_t epoch) noexcept
    {
        // This callback has no captured owner, fabricated Runtime context or
        // replacement API. Both socket peers have already joined. The actual
        // reset calls it synchronously on this native main thread, before taking
        // publication. The epoch observer is an atomic read without TLS, an
        // execution lease, the publication mutex or another reset operation.
        ++actual_reset.calls;actual_reset.notified_epoch=epoch;
        actual_reset.runtime_retirement=reason==management::transition::runtime_retirement;
        actual_reset.null_object=object==nullptr;
        actual_reset.epoch_before_cleanup=runtime::observe_compiler_runtime_generation_host_api();
        {
            std::lock_guard lock{host::mutex};
            actual_reset.owners_before=host::instances.size();actual_reset.owners_match_notified_epoch=true;
            for(auto const& row:host::instances)
            { if(row.second->generation!=epoch) {actual_reset.owners_match_notified_epoch=false;} }
        }
        // Release the observation lock FIRST: production cleanup takes its own
        // Host locks. Forward even an unexpected notification unchanged rather
        // than substituting a fixture implementation of the retirement work.
        host::retire_authority(reason,object,epoch);
        {
            std::lock_guard lock{host::mutex};actual_reset.owners_after=host::instances.size();
        }
        actual_reset.epoch_after_cleanup=runtime::observe_compiler_runtime_generation_host_api();
    }

    void denied_without_runtime_context()
    {
        runtime::ros_caller_identity identity{};
        check(!runtime::ros_caller_identity_host_api(identity)&&identity.runtime_instance==nullptr&&
              identity.generation==0&&identity.module_name.empty()&&!identity.cross_module_ancestry,
              "real primary identity remains absent outside Runtime execution");
    }

    void denied_after_reset(void const* key,std::uint64_t original_epoch,std::uint64_t current_epoch,
        std::uint64_t const (&handles)[2])
    {
        for(unsigned index{};index!=2;++index)
        {
            auto version=static_cast<ros::version>(index+1);ros::authorization lease;std::uint64_t fresh{};
            check(!host::capabilities.describe(key,original_epoch,version,handles[index],lease)&&!lease,
                  "actual Runtime reset invalidates the original fixture handle");
            check(!host::capabilities.open(key,original_epoch,version,"sub",fresh)&&fresh==0,
                  "actual Runtime reset cannot reopen the old registration");
            check(!host::capabilities.open(key,current_epoch,version,"sub",fresh)&&fresh==0,
                  "advanced Runtime epoch does not inherit or grant a fixture registration");
        }
        denied_without_runtime_context();
    }
}
#endif
int main()
{
#if !defined(__linux__)
    return 77;
#else
    check(host::instances.empty(),"fresh process has no inherited Host owners");
    check(management::retirement_hook.load()==&host::retire_authority,"real Host native hook is installed");
    auto one=ros_management_test::policy(ros::version::ros1),two=ros_management_test::policy(ros::version::ros2);
    check(bool(ros::validate_target(one))&&bool(ros::validate_target(two)),"explicit bounded fixture policies validate");
    int replaced{},other{},missing{};std::uint64_t handles[2]{},other_handles[2]{};
    auto state=install(&replaced,7,"replaced",one,two,handles);
    auto unaffected=install(&other,9,"other",one,two,other_handles);
    management::retire(management::transition::function_replacement,&missing,7);
    management::retire(management::transition::function_replacement,&replaced,8);
    check(host::instances.size()==2&&!state->retired&&!unaffected->retired,"absent and stale identities neither create nor retire owners");

    endpoint endpoints[2];std::atomic<bool> eof[2]{};
    std::thread first{hello_then_eof,std::cref(endpoints[0]),1,std::ref(eof[0])};
    std::thread second{hello_then_eof,std::cref(endpoints[1]),2,std::ref(eof[1])};
    for(unsigned index{};index!=2;++index)
    {
        auto& backend=state->backends[index];
        check(backend.connection.connect(endpoints[index].path,static_cast<std::uint8_t>(index+1),
              static_cast<std::uint32_t>(::getuid()),3000)==0,"actual client hello establishes a real local descriptor");
        backend.connected=true;backend.timeout=3000;
        check(backend.connection.session_id()==41+index,"real client session identity observed before retirement");
    }
    // This calls the installed production hook; no test callback substitutes
    // for descriptor close, capability invalidation or event-stream discard.
    management::retire(management::transition::function_replacement,&replaced,7);
    first.join();second.join();
    check(eof[0].load(std::memory_order_acquire)&&eof[1].load(std::memory_order_acquire),
          "both real peers observe EOF without a policy/event/cleanup RPC");
    check(state->retired&&host::instances.at(&replaced)==state&&!unaffected->retired,"target registration becomes inert and other owner stays live");
    for(unsigned index{};index!=2;++index)
    {
        auto& backend=state->backends[index];auto& observed=backend.observations.at(91+index);
        auto statistics=observed.local->statistics();auto const original=unaffected->backends[index].observations.at(91+index).local->statistics();
        check(!backend.connected&&backend.connection.session_id()==0&&backend.connection.call("topic.subscribe","sub",{},100).error==107,
              "retired client is disconnected and cannot issue a new call");
        check(observed.retired&&observed.requests.empty()&&!observed.local->active()&&observed.delivered==5,
              "watch metadata stays while pending request identities are removed");
        check(statistics.depth==0&&statistics.pending_depth==0&&statistics.reserved_depth==0&&
              statistics.bytes==0&&statistics.pending_bytes==0&&statistics.reserved_bytes==0,
              "no queued, pending or reserved Guest data survives");
        check(statistics.enqueued==original.enqueued&&statistics.high_water_depth==original.high_water_depth,
              "historical queue counters survive discard");
        check(original.depth==1&&original.pending_depth==1,"other module queue remains untouched");
        ros::authorization lease;auto version=static_cast<ros::version>(index+1);std::uint64_t fresh{};
        check(!host::capabilities.describe(&replaced,7,version,handles[index],lease)&&!lease&&
              !host::capabilities.open(&replaced,7,version,"sub",fresh)&&fresh==0,"old and fresh handles are denied for retired incarnation");
        check(bool(host::capabilities.describe(&other,9,version,other_handles[index],lease)),"other module authorization survives");lease.release();
    }
    diagnostic::request query{};query.module.assign("replaced");query.selected=diagnostic::selection::sessions;
    diagnostic::view view{};diagnostic::snapshot(query,view);
    check(view.result==diagnostic::status::ok&&view.size==2,"both retired session rows remain observable");
    for(std::size_t index{};index!=view.size;++index) {check(view.rows[index].retired&&!view.rows[index].alive&&!view.rows[index].connected,
          "session diagnostics cannot advertise retired authority as live");}
    query.selected=diagnostic::selection::queues;diagnostic::snapshot(query,view);
    check(view.result==diagnostic::status::ok&&view.size==2,"retired watch rows remain observable");
    for(std::size_t index{};index!=view.size;++index) {check(view.rows[index].retired&&view.rows[index].requests==0&&view.rows[index].depth==0&&
          view.rows[index].pending_depth==0&&view.rows[index].reserved_depth==0,"queue snapshots expose only retired empty state");}
    management::retire(management::transition::function_replacement,&replaced,7);
    check(host::instances.at(&replaced)==state&&state->retired,"repeated notification cannot recreate the registration");
    // No active Runtime context is installed: fixture metadata alone cannot
    // make a caller primary identity or a Guest memory range valid.
#if defined(UWVM_IMPORT_ROS1)
    check(host::capability_open<ros::version::ros1>(0,0,1,0)==13,"fixture registry cannot grant an out-of-context ROS1 caller");
#endif
#if defined(UWVM_IMPORT_ROS2)
    check(host::capability_open<ros::version::ros2>(0,0,1,0)==13,"fixture registry cannot grant an out-of-context ROS2 caller");
#endif
    management::retire(management::transition::runtime_retirement,nullptr,7);
    check(host::instances.empty(),"direct local Runtime-retirement notification removes remaining registrations");
    state.reset();unaffected.reset(); // Release only this test's old diagnostic owners.

    // The preceding notification is intentionally retained as a LOCAL hook
    // component. It does not stand in for the following genuine Runtime API.
    // Start a separate fixture at the actual current Runtime epoch, not one of
    // the deliberately mathematical 7/9 epochs used for local isolation above.
    auto const before_reset=runtime::observe_compiler_runtime_generation_host_api();
    check(before_reset>0&&before_reset<(std::numeric_limits<std::uint_least64_t>::max)()-1,
          "actual nonwrapping Runtime epoch leaves room for two genuine resets");
    int reset_owner{};std::uint64_t reset_handles[2]{};
    auto reset_state=install(&reset_owner,static_cast<std::uint64_t>(before_reset),"central_reset",one,two,reset_handles);
    check(host::instances.size()==1&&host::instances.at(&reset_owner)==reset_state&&!reset_state->retired&&
          reset_state->generation==before_reset,"new native fixture belongs to the actual pre-reset epoch");
    for(unsigned index{};index!=2;++index)
    {
        ros::authorization lease;
        check(bool(host::capabilities.describe(&reset_owner,static_cast<std::uint64_t>(before_reset),
              static_cast<ros::version>(index+1),reset_handles[index],lease)),"new fixture handle is live before actual reset");
        lease.release(); // Never hold an authorization lease across reset cleanup.
    }
    denied_without_runtime_context();reset_state.reset();
    // The Host registry is now the sole owner of the new fixture metadata.
    // No Guest, compilation, source/provider callback, controller lock, pause
    // ticket or publication scope has been entered by this executable.
    auto original_hook=management::retirement_hook.exchange(&observe_actual_reset,std::memory_order_acq_rel);
    check(original_hook==&host::retire_authority,"observation wraps the actual installed production hook");
    runtime::reset_runtime_state_host_api();
    auto const after_reset=runtime::observe_compiler_runtime_generation_host_api();
    check(actual_reset.calls==1&&actual_reset.runtime_retirement&&actual_reset.null_object,
          "genuine central reset emitted exactly one native retirement notification");
    check(actual_reset.notified_epoch==before_reset&&actual_reset.epoch_before_cleanup==before_reset&&
          actual_reset.epoch_after_cleanup==before_reset,"actual Host cleanup occurs before Runtime epoch advance");
    check(actual_reset.owners_before==1&&actual_reset.owners_match_notified_epoch&&actual_reset.owners_after==0&&
          host::instances.empty(),"real forwarded Host hook clears the distinct current-epoch native fixture");
    check(after_reset==before_reset+1,"genuine central reset advances the actual Runtime epoch exactly once");
    denied_after_reset(&reset_owner,static_cast<std::uint64_t>(before_reset),static_cast<std::uint64_t>(after_reset),reset_handles);
    auto expected_hook=&observe_actual_reset;
    check(management::retirement_hook.compare_exchange_strong(expected_hook,original_hook,std::memory_order_acq_rel)&&
          management::retirement_hook.load(std::memory_order_acquire)==&host::retire_authority,
          "restore the exact original production hook before the second reset");
    runtime::reset_runtime_state_host_api();
    auto const after_second_reset=runtime::observe_compiler_runtime_generation_host_api();
    check(after_second_reset==after_reset+1&&actual_reset.calls==1&&host::instances.empty()&&
          management::retirement_hook.load(std::memory_order_acquire)==original_hook,
          "second genuine reset uses restored production hook and inherits no Host owners");
    denied_after_reset(&reset_owner,static_cast<std::uint64_t>(before_reset),static_cast<std::uint64_t>(after_second_reset),reset_handles);
    fast_io::io::println("PASS_ROS_HOST_RETIREMENT checks=",checks);
#endif
}
