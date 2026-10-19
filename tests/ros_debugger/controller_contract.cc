/** Genuine authenticated HOST controller/cooperative-domain component.
 * The provider below is explicitly a mathematical diagnostic DATA callback.
 * It proves stop admission, not ROS transport, native effects or JIT stepping.
 * No fabricated WebAssembly Instance, ROS imports or native stop is used.
 */
#include <uwvm2/uwvm/debugger/controller.h>
#include <fast_io.h>
#include <chrono>
namespace dbg=uwvm2::uwvm::debugger;
namespace ctl=uwvm2::utils::control;
namespace data=uwvm2::imported::ros::debug;
static unsigned checks{},reads{};
static void check(bool value,char const* message)
{++checks;if(!value) {fast_io::io::perrln("FAIL_ROS_DEBUG_CONTROLLER ",fast_io::mnp::os_c_str(message));fast_io::fast_terminate();}}
static void provider(data::request const& query,data::view& output) noexcept
{++reads;output.result=data::status::ok;output.selected=query.selected;}
static ctl::launch_config config()
{
    ctl::launch_config value{};value.debug_enabled=true;value.compiler=ctl::backend::llvm;value.mode=ctl::compile_mode::full;
    value.origin=ctl::launch_origin::console;value.instance[0]=1;
#if defined(_WIN32) && !defined(__CYGWIN__)
    value.vm_process=fast_io::win32::GetCurrentProcessId();
#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    value.vm_process=static_cast<std::uint64_t>(dbg::posix_abi::getpid_noexcept());
#else
# error actual host process identity adapter required
#endif
    return value;
}
int main()
{
    auto prior=data::snapshot_hook.exchange(&provider);
    auto owner=dbg::controller::create(config(),1);
    check(owner&&owner->status()==ctl::error::none,"real console-origin authority");
    auto query=dbg::parse_console_command("info ros capabilities");auto reply=owner->execute(query);
    check(reply.status==ctl::error::invalid_state&&reads==0,"running controller cannot invoke provider");
    auto stopped=owner->execute(dbg::parse_console_command("pause"),std::chrono::milliseconds(100));
    check(stopped.status==ctl::error::none&&stopped.execution==dbg::execution_status::stopped&&stopped.stop_identifier!=0,
          "real empty-cohort cooperative stop");
    reply=owner->execute(query);
    check(reply.status==ctl::error::none&&reply.ros_state_values.result==data::status::ok&&reads==1&&
          reply.stop_identifier==stopped.stop_identifier,"only current complete cooperative stop reads DATA");
    auto malformed=query;malformed.operation=ctl::operation::resume;reply=owner->execute(malformed);
    check(reply.status==ctl::error::malformed&&reads==1,"ROS query cannot carry resume operation");
    malformed=query;malformed.payload_size=1;reply=owner->execute(malformed);
    check(reply.status==ctl::error::malformed&&reads==1,"ROS query cannot carry a wire payload");
    malformed=query;malformed.ros_state_request.count=0;reply=owner->execute(malformed);
    check(reply.status==ctl::error::malformed&&reads==1,"invalid DTO does not reach provider");
    check(owner->inspect().stop_identifier==stopped.stop_identifier,"refused queries preserve actual stop");
    data::snapshot_hook.store(nullptr);reply=owner->execute(query);
    check(reply.status==ctl::error::none&&reply.ros_state_values.result==data::status::not_selected,
          "ROS-disabled provider is explicit rather than native fallback");
    data::snapshot_hook.store(&provider);auto resumed=owner->execute(dbg::parse_console_command("continue"));
    check(resumed.status==ctl::error::none&&resumed.execution==dbg::execution_status::running,"explicit existing resume remains separate");
    reply=owner->execute(query);check(reply.status==ctl::error::invalid_state&&reads==1,"old stop does not authorize later reads");
    auto later=owner->execute(dbg::parse_console_command("pause"),std::chrono::milliseconds(100));
    check(later.status==ctl::error::none&&later.stop_identifier!=stopped.stop_identifier&&
          later.execution==dbg::execution_status::stopped,"new actual cooperative stop has a new public label");
    query.ros_state_request.stop=stopped.stop_identifier;reply=owner->execute(query);
    check(reply.status==ctl::error::none&&reply.ros_state_values.result==data::status::stale_stop&&reads==1,
          "actual earlier stop label refuses without provider invocation");
    query.ros_state_request.stop=later.stop_identifier;reply=owner->execute(query);
    check(reply.status==ctl::error::none&&reply.ros_state_values.result==data::status::ok&&reads==2,
          "exact current stop assertion permits the read-only provider");
    data::snapshot_hook.store(prior);
    fast_io::io::println("PASS_ROS_DEBUG_CONTROLLER checks=",checks);
}
