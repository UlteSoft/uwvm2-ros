/** Actual ROS Host-header component, linked against the real Runtime closure.
 * Run as a fresh process, outside every Runtime import call context. No fake
 * identity API, WebAssembly Instance, memory, IPC server or ROS RPC is supplied.
 * This proves out-of-context denial and local diagnostic behavior; it does not
 * prove an authenticated Guest/native exchange. Build ROS1/ROS2/dual variants.
 */
#include <uwvm2/uwvm/imported/ros/host.h>
#include <future>
#include <thread>
#if !defined(UWVM_IMPORT_ROS1) && !defined(UWVM_IMPORT_ROS2)
# error select an actual ROS Host backend for this component
#endif
namespace host=uwvm2::uwvm::imported::ros::host;
namespace data=uwvm2::imported::ros::debug;
namespace core=uwvm2::imported::ros;
static unsigned checks{},calls{};
static void check(bool value,char const* message)
{++checks;if(!value) {fast_io::io::perrln("FAIL_ROS_DEBUG_HOST ",fast_io::mnp::os_c_str(message));fast_io::fast_terminate();}}
/** The policy store reserves one inert Global slot per ROS version so later
 * explicit CLI options can configure it. Slot existence grants no authority.
 * Require exactly those two slots, both disabled and without any connection
 * settings or capabilities; an additional target or any implicit grant fails.
 */
static bool default_policy_has_no_authority()
{
    auto const targets=uwvm2::uwvm::imported::ros::runtime_policy.configured_targets();
    if(targets.size()!=2) {return false;}
    unsigned versions{};
    for(auto const* target:targets)
    {
        if(target==nullptr||target->enabled||target->target_scope!=core::scope::global||
           !target->name.empty()||!target->connection.empty()||!target->capabilities.empty()) {return false;}
        if(target->ros_version==core::version::ros1) {versions|=1;}
        else if(target->ros_version==core::version::ros2) {versions|=2;}
        else {return false;}
    }
    return versions==3;
}
template<core::version Version> static void outside_runtime()
{
    check(host::abi_version<Version>()==1,"actual ABI discovery does not require a grant");++calls;
    // Identity refusal must happen before looking up memory0 or touching an
    // output slot. No memory range or output pointer is fabricated here.
    check(host::capability_open<Version>(0,0,1,0)==13,"real Host open outside active Runtime context is denied");++calls;
}
int main()
{
    check(data::snapshot_hook.load()==&host::debug_snapshot,"real Host installs local read-only bridge");
    check(default_policy_has_no_authority(),"fresh Host policy contains only disabled Global slots without authority");
    data::request query{};data::view before{},after{};data::snapshot(query,before);
    check(before.result==data::status::ok&&before.total==0&&host::instances.empty()&&host::denials.empty(),
          "fresh process has no implicit ROS owners or grant aliases");
#if defined(UWVM_IMPORT_ROS1)
    outside_runtime<core::version::ros1>();
#endif
#if defined(UWVM_IMPORT_ROS2)
    outside_runtime<core::version::ros2>();
#endif
    data::snapshot(query,after);
    check(after.result==data::status::ok&&after.total==0&&host::instances.empty()&&host::denials.empty(),
          "denied calls and reads do not register Runtime owners or audit sinks");
    for(auto selected:{data::selection::sessions,data::selection::queues,data::selection::services,data::selection::actions,
        data::selection::parameters,data::selection::tf,data::selection::graph,data::selection::statistics})
    {query.selected=selected;data::snapshot(query,after);check(after.result==data::status::ok&&after.total==0,
          "all families remain empty without implicit native discovery");}
    query={};query.selected=data::selection::trace;query.count=16;data::snapshot(query,before);data::snapshot(query,after);
    check(before.result==data::status::ok&&before.trace_size==calls&&after.trace_size==calls&&before.next==after.next,
          "actual Host call trace is retained across nonconsuming reads");
    bool denied{},abi{};for(std::size_t i{};i<after.trace_size;++i)
    {
        auto const& row=after.trace[i];denied|=row.error==13;abi|=row.function.view()=="abi_version";
        check(row.unresolved&&row.instance==0&&row.module.size==0,"absent Runtime identity is explicitly unresolved");
    }
    check(denied&&abi,"real denied call and ABI discovery are both represented");
    std::promise<void> locked,release;auto released=release.get_future();
    std::thread owner([&]{std::lock_guard lease{host::mutex};locked.set_value();released.wait();});
    locked.get_future().wait();query={};query.selected=data::selection::sessions;data::snapshot(query,after);
    release.set_value();owner.join();
    check(after.result==data::status::busy&&after.size==0,"held actual Host registry mutex returns BUSY without RPC");
    data::snapshot(query,after);check(after.result==data::status::ok&&after.total==0,"same real Host bridge recovers after lock release");
    fast_io::io::println("PASS_ROS_DEBUG_HOST checks=",checks," calls=",calls);
}
