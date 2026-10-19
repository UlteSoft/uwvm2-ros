// ROS-off named-module consumer. Root compiles the existing module closure
// serially; this source imports no native ROS headers or Host implementation.
import uwvm2.uwvm.debugger.ros_state;
import uwvm2.uwvm.debugger;
int main()
{
    namespace dbg=uwvm2::uwvm::debugger;
    dbg::ros_state::request query{};dbg::ros_state::view output{};
    dbg::ros_state::read(query,output);
    auto command=dbg::parse_console_command("info ros capabilities");
    return command.kind==dbg::console_command_kind::ros_state&&
        output.result==dbg::ros_state::status::not_selected?0:1;
}
