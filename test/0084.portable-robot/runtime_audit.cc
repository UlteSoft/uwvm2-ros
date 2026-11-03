// SPDX-License-Identifier: Apache-2.0
// Linux component tests for bounded diagnostics, not native/VM execution evidence.
#include <uwvm2/imported/ros/portable/runtime_audit.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <string>
#if !defined(__linux__)
#error These tests require actual Linux file descriptors and signal behavior.
#endif
namespace p=uwvm2::imported::ros::portable;
namespace a=p::runtime_audit;
namespace io=uwvm2::imported::ros::audit_io;
static unsigned checks{};
static void check(bool value,char const* why)
{
    ++checks;if(!value) { std::fprintf(stderr,"FAIL audit check %u: %s\n",checks,why);std::exit(1); }
}
static constexpr std::string_view run_id{"0123456789abcdef0123456789abcdef"};
static void configured(a::channel& channel) { check(channel.configure(run_id),"explicit run ID accepted"); }
static void redirect(int target,int source) { check(::dup2(source,target)==target,"redirect exact descriptor"); }
static void close_fd(int fd) { check(::close(fd)==0,"close descriptor"); }
int main()
{
    p::joint_deployment deployment{};deployment.module="main\n\"\\\t中";deployment.alias="robot";
    deployment.mapping.epoch=11;deployment.policy_epoch=12;deployment.native_generation=13;
    deployment.wasm_sha256.fill(1);deployment.adapter_sha256.fill(2);
    deployment.policy_sha256.fill(3);deployment.native_provenance.fill(4);
    a::observation value{a::event::vm_goal_accepted,42,7,91,1,40,{},p::outcome::accepted,0,{}};
    for(unsigned i{};i!=16;++i) { value.goal_uuid[i]=static_cast<std::uint8_t>(i+1); }
    value.command_sha256.fill(5);
    int saved_output{::dup(STDOUT_FILENO)},saved_error{::dup(STDERR_FILENO)};
    check(saved_output>=0&&saved_error>=0,"retain original outputs");
    close_fd(STDOUT_FILENO);
    a::channel channel;
    auto delivered{io::delivered_records()},dropped{io::dropped_records()};
    errno=733;check(!channel.emit(deployment,value)&&errno==733,"disabled with closed stdout has no side effects");
    check(!channel.enabled()&&channel.attempts()==0&&channel.dropped()==0&&
          io::delivered_records()==delivered&&io::dropped_records()==dropped,"disabled does not reach the sink or consume quota");
    redirect(STDOUT_FILENO,saved_output);
    for(auto const& bad:std::array<std::string,7>{"",std::string(31,'1'),std::string(33,'1'),std::string(32,'A'),
            std::string(32,'g'),std::string(32,'0'),std::string("1\n")+std::string(30,'1')})
    { check(!channel.configure(bad)&&!channel.enabled(),"invalid run ID keeps diagnostics disabled"); }
    configured(channel);check(!channel.configure(run_id),"duplicate configuration refused");
    int file{::open("stdout-audit.jsonl",O_RDWR|O_CREAT|O_EXCL|O_CLOEXEC,0600)};
    check(file>=0,"fresh audit output");redirect(STDOUT_FILENO,file);
    auto original_flags{::fcntl(STDOUT_FILENO,F_GETFL,0)},original_fdflags{::fcntl(STDOUT_FILENO,F_GETFD,0)};
    auto start{std::chrono::steady_clock::now()};
    errno=734;check(channel.emit(deployment,value)&&errno==734,"accepted record delivered without errno change");
    value.kind=a::event::vm_terminal;value.result=p::outcome::succeeded;value.observed_sequence=41;
    check(channel.emit(deployment,value),"VM terminal delivered");
    value.kind=a::event::native_terminal;check(channel.emit(deployment,value),"native terminal delivered");
    value.kind=a::event::vm_terminal;
    for(unsigned i{3};i<a::channel::maximum_records;++i)
    {
        if(i==3)
        {
            auto extreme{value};extreme.runtime_generation=UINT64_MAX;extreme.owner_handle=UINT64_MAX;extreme.command_id=UINT64_MAX;
            extreme.observed_sequence=UINT64_MAX;extreme.result=p::outcome::rejected;extreme.native_status=INT32_MIN;
            check(channel.emit(deployment,extreme),"maximum unsigned and minimum signed fields delivered");
        }
        else { check(channel.emit(deployment,value),"remaining bounded record delivered"); }
    }
    struct stat before{},after{};
    check(::fstat(file,&before)==0,"stat completed audit output");
    for(unsigned i{};i!=100;++i) { check(!channel.emit(deployment,value),"exhausted quota rejects another record"); }
    check(channel.attempts()==a::channel::maximum_records&&channel.dropped()==0&&
          ::fstat(file,&after)==0&&before.st_size==after.st_size&&before.st_size<=8*4096,"quota cannot grow the sink");
    check(::fcntl(STDOUT_FILENO,F_GETFL,0)==original_flags&&::fcntl(STDOUT_FILENO,F_GETFD,0)==original_fdflags,
          "regular output flags unchanged");
    a::channel invalid;configured(invalid);
    auto bad{value};bad.goal_uuid.fill(0);check(!invalid.emit(deployment,bad),"zero UUID refused");
    bad=value;bad.owner_handle=0;check(!invalid.emit(deployment,bad),"missing admitted owner refused");
    bad=value;bad.native_status=1;check(!invalid.emit(deployment,bad),"failed native status cannot claim success");
    bad=value;bad.result=p::outcome::accepted;check(!invalid.emit(deployment,bad),"terminal cannot claim acceptance");
    bad=value;bad.native_observed_ns=UINT64_MAX;check(!invalid.emit(deployment,bad),"future native timestamp refused");
    auto wide{deployment};wide.module.assign(256,'x');check(!invalid.emit(wide,value),"oversized module refused");
    wide=deployment;wide.module=std::string("bad\0module",10);check(!invalid.emit(wide,value),"embedded NUL refused");
    wide=deployment;wide.module=std::string("\xc0\x80",2);check(!invalid.emit(wide,value),"noncanonical UTF-8 refused");
    check(invalid.dropped()==8&&::fstat(file,&after)==0&&after.st_size==before.st_size,"invalid observations write no bytes");
    redirect(STDOUT_FILENO,saved_output);close_fd(file);

    int old_file{::open("stderr-audit.jsonl",O_RDWR|O_CREAT|O_EXCL|O_CLOEXEC,0600)};
    check(old_file>=0,"fresh legacy sink");redirect(STDERR_FILENO,old_file);
    check(io::write_record("{\"old_sink\":true}"),"existing stderr API remains functional");
    redirect(STDERR_FILENO,saved_error);close_fd(old_file);

    int pipes[2];check(::pipe2(pipes,O_CLOEXEC|O_NONBLOCK)==0,"create real pipe");
    redirect(STDOUT_FILENO,pipes[1]);a::channel pipe_channel;configured(pipe_channel);
    check(pipe_channel.emit(deployment,value),"atomic pipe record");
    std::array<char,8192> bytes{};auto size{::read(pipes[0],bytes.data(),bytes.size())};
    check(size>0&&size<=4096&&bytes[static_cast<std::size_t>(size)-1]=='\n',"complete pipe line");
    std::string_view line{bytes.data(),static_cast<std::size_t>(size)};
    check(line.find("0102030405060708090a0b0c0d0e0f10")!=line.npos,"all UUID bytes on actual pipe");
    while(::write(pipes[1],bytes.data(),4096)>0) {}
    check(errno==EAGAIN||errno==EWOULDBLOCK,"fill actual pipe");
    auto flags{::fcntl(pipes[1],F_GETFL,0)};
    check(::fcntl(pipes[1],F_SETFL,flags&~O_NONBLOCK)==0,"original pipe is blocking");
    auto failed{io::failed_records(io::failure::would_block)};
    auto begun{std::chrono::steady_clock::now()};
    check(!pipe_channel.emit(deployment,value),"full pipe drops without blocking");
    check(std::chrono::steady_clock::now()-begun<std::chrono::seconds{1}&&
          io::failed_records(io::failure::would_block)==failed+1,"bounded full-pipe outcome");
    check(!(::fcntl(pipes[1],F_GETFL,0)&O_NONBLOCK),"original blocking OFD is unchanged");
    close_fd(pipes[0]);failed=io::failed_records(io::failure::peer_closed);
    check(!pipe_channel.emit(deployment,value)&&io::failed_records(io::failure::peer_closed)==failed+1,
          "closed pipe drops without terminating on SIGPIPE");
    redirect(STDOUT_FILENO,saved_output);close_fd(pipes[1]);

    int sockets[2];check(::socketpair(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0,sockets)==0,"create actual socket pair");
    redirect(STDOUT_FILENO,sockets[0]);a::channel socket_channel;configured(socket_channel);
    check(socket_channel.emit(deployment,value),"socket record");
    size=::recv(sockets[1],bytes.data(),bytes.size(),MSG_DONTWAIT);
    check(size>0&&size<=4096&&bytes[static_cast<std::size_t>(size)-1]=='\n',"complete socket line");
    failed=io::failed_records(io::failure::oversized);
    check(!io::write_record_to(io::stream::output,std::string(4096,'x'))&&
          io::failed_records(io::failure::oversized)==failed+1,"oversized socket record rejected before write");
    close_fd(sockets[1]);failed=io::failed_records(io::failure::peer_closed);
    check(!socket_channel.emit(deployment,value)&&io::failed_records(io::failure::peer_closed)==failed+1,
          "closed socket drops without SIGPIPE");
    redirect(STDOUT_FILENO,saved_output);close_fd(sockets[0]);

    // These are synthetic caller facts for the diagnostic component. Actual
    // runtime identities and permission decisions require separate VM execution.
    int objects[10]{};
    a::channel binding;
    auto disabled_call{binding.enter_call(&objects[0],42,deployment.wasm_sha256,1,&deployment,true)};
    check(!disabled_call.module_instance&&binding.attempts()==0,"disabled correlation retains no caller");
    std::array<char,4096> cwd{};
    check(::getcwd(cwd.data(),cwd.size())!=nullptr,"actual component working directory");
    auto adapter_path{std::string(cwd.data())+"/observation-adapter.json"};
    auto policy_path{std::string(cwd.data())+"/observation-policy.json"};
    auto make_input=[&](std::string const& path,std::string_view content)
    {
        int input{::open(path.c_str(),O_WRONLY|O_CREAT|O_EXCL|O_CLOEXEC,0600)};
        check(input>=0,"fresh observation input");
        check(::write(input,content.data(),content.size())==static_cast<ssize_t>(content.size()),"complete bounded observation input");
        close_fd(input);
    };
    make_input(adapter_path,"{\"observation\":\"adapter\"}\n");
    make_input(policy_path,"{\"observation\":\"policy\"}\n");
    check(binding.configure_inputs("relative",policy_path)==p::error::malformed,"relative observation input refused");
    check(binding.configure_inputs(adapter_path+".missing",policy_path)==p::error::unavailable,"missing observation input refused");
    check(binding.configure_inputs(cwd.data(),policy_path)==p::error::malformed,"directory observation input refused");
    auto link_path{adapter_path+".link"};
    check(::symlink(adapter_path.c_str(),link_path.c_str())==0,"actual input symlink");
    check(binding.configure_inputs(link_path,policy_path)==p::error::unavailable,"symlink observation input refused");
    auto fifo_path{adapter_path+".fifo"};check(::mkfifo(fifo_path.c_str(),0600)==0,"actual input FIFO");
    auto observation_start{std::chrono::steady_clock::now()};
    check(binding.configure_inputs(fifo_path,policy_path)==p::error::malformed&&
          std::chrono::steady_clock::now()-observation_start<std::chrono::seconds{1},"FIFO input refused without blocking");
    auto huge_path{adapter_path+".huge"};make_input(huge_path,std::string(32769,'x'));
    check(binding.configure_inputs(huge_path,policy_path)==p::error::malformed,"oversized observation input refused");
    check(::unlink(link_path.c_str())==0&&::unlink(fifo_path.c_str())==0&&::unlink(huge_path.c_str())==0,
          "retire exact component-created special and oversized inputs");
    check(binding.configure_inputs(adapter_path,policy_path)==p::error::ok,"bounded observation input snapshots retained");
    check(!binding.enabled()&&!p::deployment_selection::current(),"observed grant bytes neither enable audit nor select authority");
    check(binding.configure_inputs(adapter_path,policy_path)==p::error::denied,"observation input replacement refused");
    configured(binding);
    auto denied{binding.enter_call(&objects[0],42,deployment.wasm_sha256,1,nullptr,false)};
    check(denied.module_instance==1&&denied.permission_session==1&&denied.call_sequence==1&&!denied.allowed,
          "actual caller tuple creates bounded denied observation identity");
    check(denied.adapter_sha256==p::hash_bytes("{\"observation\":\"adapter\"}\n")&&
          denied.policy_sha256==p::hash_bytes("{\"observation\":\"policy\"}\n"),"denied context pins observed bytes");
    auto again{binding.enter_call(&objects[0],42,deployment.wasm_sha256,2,nullptr,false)};
    check(again.module_instance==denied.module_instance&&again.permission_session==denied.permission_session&&again.call_sequence==2,
          "same denied permission session advances imported-call sequence");
    auto changed_digest{deployment.wasm_sha256};changed_digest[0]^=1;
    check(!binding.enter_call(&objects[0],42,changed_digest,1,nullptr,false).module_instance,"same object/generation cannot change source digest");
    auto generation{binding.enter_call(&objects[0],43,deployment.wasm_sha256,1,nullptr,false)};
    auto other{binding.enter_call(&objects[1],42,deployment.wasm_sha256,1,nullptr,false)};
    check(generation.module_instance==2&&other.module_instance==3&&generation.permission_session!=other.permission_session,
          "new generation and different runtime objects do not inherit correlation identities");
    auto allowed_open{binding.enter_call(&objects[0],42,deployment.wasm_sha256,1,&deployment,true)};
    check(allowed_open.module_instance==denied.module_instance&&allowed_open.permission_session!=denied.permission_session&&allowed_open.call_sequence==1,
          "selected permission creates a separate observed session for the same module");
    (void)binding.enter_call(&objects[0],42,deployment.wasm_sha256,2,&deployment,true);
    (void)binding.enter_call(&objects[0],42,deployment.wasm_sha256,3,&deployment,true);
    auto allowed_submit{binding.enter_call(&objects[0],42,deployment.wasm_sha256,4,&deployment,true)};
    check(allowed_submit.call_sequence==4&&allowed_submit.adapter_sha256==deployment.adapter_sha256&&
          allowed_submit.policy_sha256==deployment.policy_sha256,"submit correlation uses actual selected permission bytes");
    int bindings_file{::open("command-bindings.jsonl",O_RDWR|O_CREAT|O_EXCL|O_CLOEXEC,0600)};
    check(bindings_file>=0,"fresh binding stream");redirect(STDOUT_FILENO,bindings_file);
    errno=735;check(binding.emit_binding(denied)&&errno==735,"denied binding delivered without errno change");
    check(!binding.emit_binding(allowed_open,&value.command_sha256,&value.goal_uuid),"open cannot claim an accepted native goal");
    auto zero_goal{value.goal_uuid};zero_goal.fill(0);
    check(!binding.emit_binding(allowed_submit,&value.command_sha256,&zero_goal),"zero native goal cannot create allowed binding");
    check(binding.emit_binding(allowed_submit,&value.command_sha256,&value.goal_uuid),"accepted submit binding delivered");
    check(!binding.emit_binding(denied,&value.command_sha256,&value.goal_uuid),"denied call cannot attach command or UUID");
    check(!binding.emit_binding({}),"missing caller cannot create a binding record");
    check(binding.emit_binding(generation)&&binding.emit_binding(other),"separate caller generations retain distinct denied records");
    check(!binding.emit_binding(denied)&&binding.attempts()==8&&binding.dropped()==4,"binding records share bounded attempt/drop budget");
    check(::fstat(bindings_file,&after)==0&&after.st_size>0&&after.st_size<=4*4096,"four bounded successful binding lines");
    std::string binding_text(static_cast<std::size_t>(after.st_size),'\0');
    check(::pread(bindings_file,binding_text.data(),binding_text.size(),0)==static_cast<ssize_t>(binding_text.size()),"read original actual binding stream");
    check(binding_text.find("\"native_operation_id\":null")!=binding_text.npos&&
          binding_text.find("\"phase\":\"native-goal-accepted\"")!=binding_text.npos&&
          binding_text.find("\"call_sequence\":4")!=binding_text.npos,"binding records carry deny and accepted-submit semantics");
    redirect(STDOUT_FILENO,saved_output);close_fd(bindings_file);
    a::channel saturated;configured(saturated);
    for(unsigned i{};i!=8;++i)
    {
        check(saturated.enter_call(&objects[i],42,deployment.wasm_sha256,1,&deployment,true).module_instance==i+1u,
              "bounded caller registry admits exact distinct identity");
    }
    check(!saturated.enter_call(&objects[8],42,deployment.wasm_sha256,1,&deployment,true).module_instance,
          "ninth caller cannot grow bounded correlation registry");
    check(saturated.enter_call(&objects[0],42,deployment.wasm_sha256,2,&deployment,true).call_sequence==2,
          "registry saturation preserves an existing caller sequence");
    check(!p::deployment_selection::current(),"all observation and emission checks leave actual authority absent");

    close_fd(saved_output);close_fd(saved_error);
    auto elapsed{std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-start).count()};
    std::printf("{\"passed\":true,\"checks\":%u,\"pid\":%ld,\"records\":8,\"elapsed_ns\":%lld,\"scope\":\"diagnostic-component-only\"}\n",
        checks,static_cast<long>(::getpid()),static_cast<long long>(elapsed));
}
