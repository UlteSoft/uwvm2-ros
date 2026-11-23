#include <uwvm2/uwvm/debugger/command.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <initializer_list>
#include <sys/wait.h>
#include <unistd.h>
namespace dbg=uwvm2::uwvm::debugger;
namespace permissions=uwvm2::utils::control::debug_permissions;
static unsigned checks{};
#define REQUIRE(x) do{++checks;if(!(x)){std::fprintf(stderr,"FAIL line=%u: %s\n",__LINE__,#x);std::exit(1);}}while(false)
static auto parse(std::string const& input){return dbg::parse_console_command(fast_io::string_view{input.data(),input.size()});}
int main()
{
    auto current=parse("dump restore current 7 /private/robot state.uwsnap");
    REQUIRE(current.kind==dbg::console_command_kind::snapshot_restore);
    REQUIRE(!current.restore_recorded && !current.restore_continue && current.dump_stop_identifier==7u);
    REQUIRE(current.dump_path=="/private/robot state.uwsnap" && current.restore_bindings.empty());
    auto recorded=parse("dump restore recorded 18446744073709551615 0:0:1,2:4294967295:2147483647 /private/world.uwsnap");
    REQUIRE(recorded.kind==dbg::console_command_kind::snapshot_restore && recorded.restore_recorded);
    REQUIRE(recorded.restore_bindings.size()==2u && recorded.restore_bindings[1].module==2u &&
        recorded.restore_bindings[1].resource==UINT32_MAX && recorded.restore_bindings[1].descriptor==INT32_MAX);
    REQUIRE(parse("dump restore recorded 7 - /empty.uwsnap").kind==dbg::console_command_kind::snapshot_restore);
    auto pending=parse("dump restore continue");
    REQUIRE(pending.kind==dbg::console_command_kind::snapshot_restore && pending.restore_continue && pending.dump_path.empty());
    for(auto bad:{"dump restore","dump restore automatic 7 /x","dump restore current 0 /x",
        "dump restore current -1 /x","dump restore current 18446744073709551616 /x","dump restore current 1",
        "dump restore continue /x","dump restore recorded 1 /x","dump restore recorded 1 -",
        "dump restore recorded 1 0:0:1,0:0:2 /x","dump restore recorded 1 0:0:1, /x",
        "dump restore recorded 1 :0:1 /x","dump restore recorded 1 0::1 /x","dump restore recorded 1 0:0: /x",
        "dump restore recorded 1 0:0:1:2 /x","dump restore recorded 1 0:0:-1 /x",
        "dump restore recorded 1 0:0:2147483648 /x","dump restore recorded 1 0:4294967296:1 /x",
        "dump restore recorded 1 18446744073709551616:0:1 /x"})
        REQUIRE(parse(bad).kind==dbg::console_command_kind::invalid);
    std::string embedded="dump restore current 1 /x";embedded.push_back('\0');embedded+="y";
    REQUIRE(parse(embedded).kind==dbg::console_command_kind::invalid);
    for(auto permission:{permissions::permission_level::full,permissions::permission_level::read_only,
        permissions::permission_level::limited,permissions::permission_level::none})
    {
        auto pid=::fork();REQUIRE(pid>=0);
        if(!pid) {
            REQUIRE(permissions::configure_trusted_launch(permission));
            for(auto const* item:{&current,&recorded,&pending})
                REQUIRE(dbg::command_permission_allowed(*item)==(permission==permissions::permission_level::full));
            ::_exit(0);
        }
        int status{};REQUIRE(::waitpid(pid,&status,0)==pid && WIFEXITED(status) && WEXITSTATUS(status)==0);
    }
    std::printf("PASS snapshot restore command and four permission levels: %u checks\n",checks);
}
