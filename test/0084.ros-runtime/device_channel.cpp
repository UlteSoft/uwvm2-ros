/** @file @brief Owned typed PTY endpoints, bounded syscalls and retained partial-write debt. */
#define main previous_device_protocol_main
#include "device_protocol.cpp"
#undef main
#include <uwvm2/imported/ros/scene/device_channel.h>
#include <cstdarg>
#include <dirent.h>
#include <sys/socket.h>
#include <thread>
namespace ch=d::channel;
#if defined(UWVM_DEVICE_CHANNEL_FAULT_INJECTION)
static bool duplicate_failure{};
extern "C" int __real_fcntl(int,int,...);
extern "C" int __wrap_fcntl(int fd,int command,...)
{
    if(command==F_GETFL||command==F_GETFD)return __real_fcntl(fd,command);
    va_list arguments;va_start(arguments,command);auto value=va_arg(arguments,int);va_end(arguments);
    if(command==F_DUPFD_CLOEXEC&&duplicate_failure){errno=EMFILE;return -1;}return __real_fcntl(fd,command,value);
}
#endif
static std::size_t descriptors()
{auto directory=::opendir("/proc/self/fd");assert(directory);std::size_t result{};while(::readdir(directory))++result;assert(::closedir(directory)==0);return result;}
static void descriptor_kinds_directions_failure_and_leaks()
{
    auto before=descriptors();
    {
        auto pair=terminal();ch::endpoint endpoint;
#if defined(UWVM_DEVICE_CHANNEL_FAULT_INJECTION)
        duplicate_failure=true;assert(endpoint.bind(pair.master,ch::direction::send)==error::again);duplicate_failure=false;
#endif
        auto flags=::fcntl(pair.master,F_GETFL);assert(flags>=0&&::fcntl(pair.master,F_SETFL,flags&~O_NONBLOCK)==0);
        assert(endpoint.bind(pair.master,ch::direction::send)==error::denied&&::fcntl(pair.master,F_SETFL,flags)==0);
        assert(endpoint.bind(pair.master,ch::direction::send)==error::success&&endpoint.bind(pair.master,ch::direction::send)==error::conflict);
        std::array<std::uint8_t,1> value{7};std::size_t count{};assert(::fcntl(pair.master,F_SETFL,flags&~O_NONBLOCK)==0&&endpoint.write(value,count)==error::denied&&!count);
        assert(::fcntl(pair.master,F_SETFL,flags)==0);::close(pair.master);assert(endpoint.read(value,count)==error::denied&&endpoint.write(value,count)==error::success&&count==1);
        std::uint8_t received{};assert(::read(pair.slave,&received,1)==1&&received==7);::close(pair.slave);
        int pipe[2];assert(::pipe2(pipe,O_NONBLOCK|O_CLOEXEC)==0);ch::endpoint sender,receiver;
        assert(sender.bind(pipe[1],ch::direction::send)==error::denied&&receiver.bind(pipe[0],ch::direction::receive)==error::success);
        assert(receiver.write(value,count)==error::denied);::close(pipe[0]);::close(pipe[1]);
        int null=::open("/dev/null",O_RDWR|O_NONBLOCK|O_CLOEXEC);assert(null>=0&&sender.bind(null,ch::direction::send)==error::denied);::close(null);
        int file=::memfd_create("device-no-file",MFD_CLOEXEC);assert(file>=0&&::fcntl(file,F_SETFL,O_NONBLOCK)==0&&sender.bind(file,ch::direction::send)==error::denied);::close(file);
        int sockets[2];assert(::socketpair(AF_UNIX,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,0,sockets)==0&&sender.bind(sockets[0],ch::direction::send)==error::denied);::close(sockets[0]);::close(sockets[1]);
    }
    assert(descriptors()==before);
}
static ch::send_status inspect(ch::sender& sender){ch::send_status value;assert(sender.inspect(value)==error::success);return value;}
static void partial_expiry_keeps_debt_and_stop_independent()
{
    auto normal=terminal(),stop=terminal();ch::sender commands{p::kind::command},stops{p::kind::stop};
    assert(commands.bind(normal.master,7)==error::success&&stops.bind(stop.master,7)==error::success&&commands.source()!=stops.source());
    auto packet=command();assert(commands.stage(packet)==error::success&&commands.pump(1001)==error::success);auto state=inspect(commands);assert(state.state==ch::transmission::partial&&state.written==7);
    assert(commands.pump(1050)==error::timeout&&inspect(commands).state==ch::transmission::unknown&&commands.discard_unwritten()==error::denied&&commands.stage(packet)==error::again);
    p::packet stopped;assert(p::stop({102,d::state::queued,d::effect::locally_accepted,4},profile("stop"),1,stopped)==error::success&&stops.stage(stopped)==error::success);
    while(inspect(stops).state!=ch::transmission::awaiting_ack)assert(stops.pump(0)==error::success);
    auto complete=read_frame(stop.slave,p::kind::stop);auto original=header(complete,p::kind::stop);p::packet ack;
    assert(p::acknowledge(original,p::acknowledgement::stop_accepted,1,ack)==error::success);p::acknowledgement outcome;
    assert(stops.acknowledge(ack.view(),1,outcome)==error::success&&inspect(stops).state==ch::transmission::empty&&inspect(commands).state==ch::transmission::unknown);
    ::close(normal.master);::close(normal.slave);::close(stop.master);::close(stop.slave);
}
static void actual_ack_receive_and_unknown_digest()
{
    for(unsigned wrong{};wrong!=2;++wrong)
    {
        auto tx=terminal(),rx=terminal();ch::sender sender{p::kind::command};ch::acknowledgements receiver;
        assert(sender.bind(tx.master,4096)==error::success&&receiver.bind(rx.master)==error::success);auto packet=command();assert(sender.stage(packet)==error::success&&sender.pump(1001)==error::success);
        assert(inspect(sender).state==ch::transmission::awaiting_ack);auto received=read_frame(tx.slave,p::kind::command);auto original=header(received);if(wrong)original.digest[0]^=1;
        p::packet ack;assert(p::acknowledge(original,p::acknowledgement::executed,1,ack)==error::success);write_bytes(rx.slave,ack.view().first(10));
        assert(receiver.pump()==error::again);p::packet copied;copied.size=17;assert(receiver.copy(copied)==error::again&&copied.size==17);
        write_bytes(rx.slave,ack.view().subspan(10));
        for(unsigned tries{};receiver.copy(copied)==error::again;++tries){assert(tries<1000);auto status=receiver.pump();assert(status==error::again||status==error::success);}
        p::acknowledgement outcome;assert(sender.acknowledge(copied.view(),1,outcome)==(wrong?error::invalid:error::success));
        assert(inspect(sender).state==(wrong?ch::transmission::unknown:ch::transmission::empty));assert(receiver.consume()==error::success&&receiver.copy(copied)==error::again);
        ::close(tx.master);::close(tx.slave);::close(rx.master);::close(rx.slave);
    }
}
static void zero_write_retire_concurrent_pump_and_fork()
{
    auto pair=terminal();ch::sender sender{p::kind::command};assert(sender.bind(pair.master,7)==error::success);auto packet=command();assert(sender.stage(packet)==error::success&&sender.pump(1050)==error::timeout);
    assert(inspect(sender).state==ch::transmission::failed&&!inspect(sender).written&&sender.discard_unwritten()==error::success);
    assert(sender.stage(packet)==error::success);std::atomic<bool> done{};
    auto pump=[&]{for(unsigned n{};n<2000&&!done.load();++n){auto result=sender.pump(1001);assert(result==error::success||result==error::again);if(inspect(sender).state==ch::transmission::awaiting_ack)done=true;}};
    std::thread first{pump},second{pump};first.join();second.join();assert(done);
    auto received=read_frame(pair.slave,p::kind::command);assert(received.size==packet.size&&std::equal(received.view().begin(),received.view().end(),packet.view().begin()));
    auto child=::fork();assert(child>=0);if(!child){ch::send_status status;::_exit(sender.inspect(status)==error::denied?0:1);}int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
    sender.retire();assert(inspect(sender).state==ch::transmission::unknown&&sender.pump(1001)==error::denied);::close(pair.master);::close(pair.slave);
}
int main()
{
    assert(previous_device_protocol_main()==0);descriptor_kinds_directions_failure_and_leaks();partial_expiry_keeps_debt_and_stop_independent();
    actual_ack_receive_and_unknown_digest();zero_write_retire_concurrent_pump_and_fork();std::cout<<"passed 4 device channel safety groups\n";return 0;
}
