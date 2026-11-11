// Independent real Linux debugger, signed update and diagnostic endpoints.
#include <uwvm2/utils/control/sealed_input.h>
#include <fast_io.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
namespace control = ::uwvm2::utils::control;
#define CHECK(test) do { if(!(test)) { ::fast_io::io::perrln("FAIL independent control seals line=", __LINE__); return 1; } } while(false)
int main()
{
    int update[2], debug[2], diagnostic[2], ordinary[2];
    CHECK(::socketpair(AF_UNIX, SOCK_STREAM|SOCK_CLOEXEC, 0, update) == 0);
    CHECK(::socketpair(AF_UNIX, SOCK_SEQPACKET|SOCK_CLOEXEC, 0, debug) == 0);
    CHECK(::socketpair(AF_UNIX, SOCK_SEQPACKET|SOCK_CLOEXEC, 0, diagnostic) == 0);
    CHECK(::socketpair(AF_UNIX, SOCK_STREAM|SOCK_CLOEXEC, 0, ordinary) == 0);
    CHECK(control::seal_update_control_host_api(update[0]) == control::sealed_input_status::ok);
    CHECK(control::console_input_sealed());
    CHECK(control::seal_console_input_host_api(debug[0], true) == control::sealed_input_status::ok);
    CHECK(control::seal_diagnostic_control_host_api(diagnostic[0]) == control::sealed_input_status::ok);
    for(auto fd : {update[0], debug[0], diagnostic[0]})
    {
        CHECK(control::inspect_guest_file_host_api(fd) == control::sealed_input_decision::denied);
        auto alias{::dup(fd)}; CHECK(alias >= 0);
        CHECK(control::inspect_guest_file_host_api(alias) == control::sealed_input_decision::denied);
        CHECK(control::inspect_guest_output_host_api(alias) == control::sealed_input_decision::denied);
        ::close(alias);
    }
    CHECK(control::inspect_guest_file_host_api(ordinary[0]) == control::sealed_input_decision::allow);
    CHECK(control::seal_update_control_host_api(ordinary[0]) == control::sealed_input_status::already_sealed);
    ::fast_io::io::println("PASS independent debugger/update/diagnostic identities and duplicate aliases remain sealed");
}
