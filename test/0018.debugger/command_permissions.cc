// Actual typed/parsed command admission. No controller or transport mocks.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/debugger/command.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
namespace debug=uwvm2::uwvm::debugger;
namespace permissions=uwvm2::utils::control::debug_permissions;
namespace control=uwvm2::utils::control;
static int child(permissions::permission_level level)
{
    if(!permissions::configure_trusted_launch(level))return 1;
    bool const any=level!=permissions::permission_level::none;
    bool const inspect=level==permissions::permission_level::full || level==permissions::permission_level::read_only;
    bool const full=level==permissions::permission_level::full;
    unsigned count{};
    auto check=[&](debug::console_command const& command,bool expected,char const* name)
    {
        ++count;if(debug::command_permission_allowed(command)==expected)return true;
        std::fprintf(stderr,"FAIL %s permissions=%s expected=%d kind=%u op=%u\n",name,permissions::name(),expected,static_cast<unsigned>(command.kind),static_cast<unsigned>(command.operation));return false;
    };
    for(auto text:{"status","help","memory 0 0 0 1","memtags 0 0 0 1","replace 0 0 1 /private/candidate.wasm"})
    {
        auto command=debug::parse_console_command(fast_io::string_view{fast_io::mnp::os_c_str(text)});
        if(command.kind==debug::console_command_kind::invalid)return 2;
        auto expected=text[0]=='s'||text[0]=='h'?any:text[0]=='r'?full:inspect;
        if(!check(command,expected,text))return 3;
    }
    for(auto kind:{debug::console_command_kind::wasm_mutation,debug::console_command_kind::wasm_script,
                  debug::console_command_kind::execution_relocate,debug::console_command_kind::source_step})
    {
        debug::console_command command{kind};command.operation=control::operation::status;
        if(!check(command,full,"state mutation"))return 4;
    }
    for(auto kind:{debug::console_command_kind::source_value,debug::console_command_kind::wasm_state,
                  debug::console_command_kind::assembly_registers,debug::console_command_kind::ros_state})
    {
        debug::console_command command{kind};command.operation=control::operation::status;
        if(!check(command,inspect,"sensitive inspection"))return 5;
    }
    debug::console_command launch{debug::console_command_kind::protocol};launch.operation=control::operation::resume;
    launch.launch_action=debug::console_launch_action::run;launch.run_from_entry=true;
    if(!check(launch,full,"execution restart"))return 6;
    for(auto operation:{control::operation::pause,control::operation::resume,control::operation::detach})
    {debug::console_command command{debug::console_command_kind::protocol};command.operation=operation;
     if(!check(command,full,"execution control including terminal resume"))return 12;}
    for(auto operation:{debug::host_trace::action::on,debug::host_trace::action::off,debug::host_trace::action::clear})
    {debug::console_command command{debug::console_command_kind::host_trace};command.operation=control::operation::status;
     command.host_trace_request.operation=operation;if(!check(command,full,"trace configuration mutation"))return 13;}
    debug::console_command value{debug::console_command_kind::source_value};value.operation=control::operation::locals;
    if(!check(value,inspect,"actual source-value read opcode"))return 14;
    if(!debug::command_mutates_terminal_execution(launch))return 9;
    for(auto kind:{debug::console_command_kind::wasm_mutation,debug::console_command_kind::wasm_script,
                  debug::console_command_kind::execution_relocate,debug::console_command_kind::replacement_file})
    {debug::console_command command{kind};if(!debug::command_mutates_terminal_execution(command))return 9;}
    for(auto text:{"status","help","memory 0 0 0 1","memtags 0 0 0 1"})
    {auto command=debug::parse_console_command(fast_io::string_view{fast_io::mnp::os_c_str(text)});
     if(debug::command_mutates_terminal_execution(command))return 9;}
    // Once configured, a different host selection cannot upgrade this process.
    auto other=full?permissions::permission_level::none:permissions::permission_level::full;
    if(permissions::configure_trusted_launch(other))return 7;
    return count==21u?0:8;
}
int main()
{
    for(auto level:{permissions::permission_level::full,permissions::permission_level::read_only,
                    permissions::permission_level::limited,permissions::permission_level::none})
    {
        auto pid=::fork();if(pid<0)return 10;
        if(!pid)::_exit(child(level));
        int status{};if(::waitpid(pid,&status,0)!=pid || !WIFEXITED(status) || WEXITSTATUS(status))
        {std::printf("FAIL wait=%d\n",status);return 11;}
    }
    std::puts("PASS 84/84 actual parsed/typed command gates; only full controls execution; four immutable permission levels");
}
