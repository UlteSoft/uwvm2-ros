#include <uwvm2/uwvm/debugger/command.h>
#include <uwvm2/uwvm/debugger/console_aliases.h>
#include <cstdlib>
void check(bool value) { if(!value) { std::abort(); } }
int main()
{
 using namespace uwvm2::uwvm::debugger;
 auto t=parse_console_command("tbreak bw 0 1 2 ignore 3 if true");
 check(t.breakpoint_temporary && t.breakpoint_ignore==3 && t.breakpoint_initial_condition);
 check(parse_console_command("tbreak bs 0 a.c:9").breakpoint_temporary);
 check(parse_console_command("tbreak bi 16").breakpoint_temporary);
 check(parse_console_command("tbreak continue").kind==console_command_kind::invalid);
 check(parse_console_command("tbreak tbreak bi 16").kind==console_command_kind::invalid);
 check(parse_console_command("commands 1 echo ready;continue").kind==console_command_kind::breakpoint_commands);
 check(parse_console_command("watch memory 0 0 18446744073709551615 2").kind==console_command_kind::invalid);
 check(parse_console_command("watch memory 0 0 0 65537").kind==console_command_kind::watchpoint);
 check(parse_console_command("watch print x").kind==console_command_kind::watchpoint);
 check(parse_console_command("watchpoints delete 0").kind==console_command_kind::invalid);
 for(auto line:{"set wasm local 1 0 bits i32 ffffffff","set wasm operand 1 1 bits v128 0 1",
   "set wasm local 1 0 null","set wasm local 1 0 from locals 0 1"})
 { auto c=parse_console_command(::fast_io::string_view{line,::std::char_traits<char>::length(line)});check(c.kind==console_command_kind::wasm_mutation);check(wasm_mutation::valid(c.wasm_mutation_request)); }
 for(auto line:{"set wasm local 0 0 bits i32 1","set wasm operand 1 0 bits i32 100000000","set wasm local 1","set wasm global 0 0"})
 { check(parse_console_command(::fast_io::string_view{line,::std::char_traits<char>::length(line)}).kind==console_command_kind::invalid); }

 auto p=parse_console_command("watchpoints new 1 69999 256");
 check(p.kind==console_command_kind::watchpoint && p.watchpoint_policy==watchpoint_action::read_new);
 check(parse_console_command("watchpoints new 1 0 257").kind==console_command_kind::invalid);
 check(parse_console_command("callbacks 0 64 32768 32768").kind==console_command_kind::breakpoint_commands);
 check(parse_console_command("callbacks 0 64 0 32769").kind==console_command_kind::invalid);
 for(auto line:{::fast_io::string_view{"tbreak bw 0 1 0"},::fast_io::string_view{"watch x"},::fast_io::string_view{"commands 1 echo x"}})
 { check(console_aliases::parse(line,0,0).command.kind!=console_command_kind::invalid); }
}
