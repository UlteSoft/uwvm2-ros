// The real binary protocol must repeat console/DAP execution-control admission.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/utils/control/impl.h>
#include <array>
#include <cstdio>
#include <sys/wait.h>
#include <unistd.h>
namespace ctl=uwvm2::utils::control;
static int run(ctl::debug_permissions::permission_level level)
{
    if(!ctl::debug_permissions::configure_trusted_launch(level))return 1;
    bool const full=level==ctl::debug_permissions::permission_level::full;
    for(auto op:{ctl::operation::pause,ctl::operation::resume,ctl::operation::detach,
                 ctl::operation::step,ctl::operation::breakpoint_clear})
    {
        ctl::launch_config config{};config.profile=level;config.debug_enabled=true;config.replacement_enabled=full;
        config.compiler=ctl::backend::llvm;config.mode=ctl::compile_mode::full;config.origin=ctl::launch_origin::console;
        config.instance[0]=7;config.vm_process=static_cast<std::uint64_t>(::getpid());
        config.initial_execution=op==ctl::operation::pause ? ctl::execution_state::running:ctl::execution_state::stopped;
        ctl::launch_authority authority{config};ctl::control_session session;
        if(session.attach_console(authority.issue_permit())!=ctl::error::none)return 2;
        std::array<ctl::wire_byte,ctl::header_bytes+8> bytes{};
        std::array<ctl::wire_byte,8> payload{};payload[0]=1;
        auto const length=op==ctl::operation::breakpoint_clear || op==ctl::operation::step ? 8u:0u;
        ctl::frame_header header{op,length,config.instance,config.generation,1};
        ctl::input_buffer input{payload.data(),payload.data()+length};
        ctl::output_buffer output{bytes.data(),bytes.data()+bytes.size()};
        auto const encoded=ctl::encode_frame(header,input,output);if(encoded.status!=ctl::error::none)return 3;
        auto result=session.receive({bytes.data(),bytes.data()+encoded.written});
        if(full ? result.status!=ctl::error::none || !result.request :
            result.status!=ctl::error::unavailable_capability || result.request.has_value())return 4;
    }
    return 0;
}
int main()
{
    for(auto level:{ctl::debug_permissions::permission_level::full,ctl::debug_permissions::permission_level::read_only,
                    ctl::debug_permissions::permission_level::limited})
    {
        auto pid=::fork();if(pid<0)return 10;if(!pid)::_exit(run(level));
        int status{};if(::waitpid(pid,&status,0)!=pid || !WIFEXITED(status) || WEXITSTATUS(status))
        {std::printf("FAIL wait=%d\n",status);return 11;}
    }
    std::puts("PASS 15/15 actual binary protocol control grants; read-only and limited cannot change execution");
}
