/** @file @brief Complete fixed frames and a real PTY receiver process with independent stop. */
#include <uwvm2/imported/ros/scene/device_protocol.h>
#include <cassert>
#include <iostream>
#include <cerrno>
#include <fcntl.h>
#include <poll.h>
#include <termios.h>
#include <sys/mman.h>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;namespace d=s::devices;namespace p=d::protocol;using d::error;
static s::buffers::profile profile(std::string_view name){s::buffers::profile out;assert(out.identity.assign(name));out.sha256.fill(12);return out;}
static d::scalar_schema schema()
{
    d::scalar_schema out;out.profile=profile("command");assert(out.frame.assign("base")&&out.fields[0].name.assign("joint"));out.count=1;
    out.fields[0].quantity=d::unit::radian;out.fields[0].minimum=-1;out.fields[0].maximum=1;return out;
}
static d::work work(d::handle request=100)
{
    d::work out;out.ticket=3;out.device=6;out.key=1;out.result={request,d::state::running,d::effect::locally_accepted,4,d::reason::none};
    out.deadline=1050;out.controller={9,10,4,{1,11,3,12},1,1100};out.command.count=1;out.command.values[0]=0.5;return out;
}
static p::packet command(d::handle sequence=1,d::handle request=100)
{p::packet out;assert(p::command(work(request),schema(),sequence,out)==error::success);return out;}
static p::header header(p::packet const& packet,p::kind expected=p::kind::command)
{p::header out;s::wire::bytes body;assert(p::decode(packet.view(),expected,out,body)==error::success);return out;}
static void exact_frame_bounds_digest_and_alias()
{
    auto packet=command();auto h=header(packet);assert(h.request==100&&h.provider==4&&h.sequence==1&&h.controller_epoch==10&&h.authority_epoch==12);
    p::header unchanged;unchanged.provider=77;s::wire::bytes body;auto corrupt=packet;corrupt.data[corrupt.size-1]^=1;
    assert(p::decode(corrupt.view(),p::kind::command,unchanged,body)==error::invalid&&unchanged.provider==77);
    corrupt=packet;corrupt.data[16]^=1;assert(p::decode(corrupt.view(),p::kind::command,unchanged,body)==error::invalid);
    assert(p::decode(packet.view(),p::kind::stop,unchanged,body)==error::invalid);
    for(std::size_t n{};n<packet.size;++n)assert(p::decode(packet.view().first(n),p::kind::command,unchanged,body)==error::invalid);
    corrupt=packet;corrupt.data[80]=0xff;corrupt.data[81]=0xff;assert(p::decode_header({corrupt.data.data(),p::header_bytes},p::kind::command,unchanged)==error::invalid);
    assert(p::encode(h,packet.view(),packet)==error::invalid);
    h.body_bytes=p::maximum_payload+1;assert(!p::fields_valid(h));h=header(packet);h.deadline=1101;assert(!p::fields_valid(h));
}
static void byte_fragments_bounded_extra_and_poison()
{
    auto packet=command();p::stream_decoder decoder{p::kind::command};
    for(std::size_t i{};i<packet.size;++i){std::size_t consumed{};assert(decoder.feed(packet.view().subspan(i,1),consumed)==(i+1==packet.size?error::success:error::again)&&consumed==1);}
    assert(decoder.frame().size()==packet.size&&!decoder.remaining());std::size_t consumed{99};assert(decoder.feed(packet.view(),consumed)==error::again&&!consumed);
    assert(decoder.next()==error::success&&decoder.remaining()==p::header_bytes);
    std::array<std::uint8_t,p::maximum_frame*2> joined;std::copy(packet.view().begin(),packet.view().end(),joined.begin());std::copy(packet.view().begin(),packet.view().end(),joined.begin()+static_cast<std::ptrdiff_t>(packet.size));
    assert(decoder.feed({joined.data(),packet.size*2},consumed)==error::success&&consumed==packet.size);
    p::stream_decoder wrong{p::kind::stop};assert(wrong.feed(packet.view(),consumed)==error::invalid&&consumed==p::header_bytes&&wrong.next()==error::invalid);
    assert(wrong.feed(packet.view(),consumed)==error::invalid&&!consumed&&!wrong.remaining());
    auto damaged=packet;damaged.data[damaged.size-1]^=1;p::stream_decoder bad{p::kind::command};assert(bad.feed(damaged.view(),consumed)==error::invalid&&bad.frame().empty());
}
static void receiver_boot_lease_authority_deadline_and_stop()
{
    p::receiver receiver;auto selected=schema();auto stop=profile("stop");assert(receiver.configure(4,selected,stop)==error::success);
    auto packet=command();p::header h;d::scalar_command value;assert(receiver.command(packet.view(),1001,h,value)==error::denied);
    assert(receiver.rearm_after_stopped(4,1)==error::success&&receiver.install_controller(work().controller)==error::success);
    assert(receiver.command(packet.view(),1050,h,value)==error::denied&&receiver.command(packet.view(),1001,h,value)==error::success&&value.values[0]==0.5);
    assert(receiver.command(packet.view(),1001,h,value)==error::conflict);
    auto changed=work(101);changed.controller.owner.authority_epoch++;assert(p::command(changed,selected,2,packet)==error::success);
    assert(receiver.command(packet.view(),1001,h,value)==error::denied);
    changed=work(101);changed.result.provider_generation++;assert(p::command(changed,selected,2,packet)==error::invalid);changed.controller.provider_generation++;
    assert(p::command(changed,selected,2,packet)==error::success);assert(receiver.command(packet.view(),1001,h,value)==error::conflict);
    p::packet stopped;assert(p::stop({102,d::state::queued,d::effect::locally_accepted,4},stop,1,stopped)==error::success);
    assert(receiver.stop(stopped.view(),h)==error::success&&receiver.latched());packet=command(2,101);assert(receiver.command(packet.view(),1001,h,value)==error::denied);
    assert(receiver.rearm_after_stopped(4,1)==error::denied&&receiver.rearm_after_stopped(4,2)==error::success);
    assert(receiver.install_controller(work().controller)==error::denied&&receiver.command(packet.view(),1001,h,value)==error::denied);
    changed=work(101);changed.controller.epoch++;assert(receiver.install_controller(changed.controller)==error::success&&p::command(changed,selected,2,packet)==error::success);
    assert(receiver.command(packet.view(),1001,h,value)==error::success);
}
static void acknowledgements_bind_whole_original()
{
    auto packet=command();auto original=header(packet);p::packet ack;assert(p::acknowledge(original,p::acknowledgement::executed,1,ack)==error::success);
    p::acknowledgement outcome{};assert(p::verify_ack(ack.view(),original,1,outcome)==error::success&&outcome==p::acknowledgement::executed);
    auto wrong=original;wrong.digest[0]^=1;assert(p::verify_ack(ack.view(),wrong,1,outcome)==error::invalid);
    wrong=original;wrong.provider++;assert(p::verify_ack(ack.view(),wrong,1,outcome)==error::conflict);
    assert(p::verify_ack(ack.view(),original,2,outcome)==error::conflict);
    assert(p::acknowledge(original,p::acknowledgement::stopped,1,ack)==error::invalid);
}
struct terminal_pair{int master{-1},slave{-1};};
static terminal_pair terminal()
{
    terminal_pair out;out.master=::posix_openpt(O_RDWR|O_NOCTTY|O_CLOEXEC|O_NONBLOCK);assert(out.master>=0&&::grantpt(out.master)==0&&::unlockpt(out.master)==0);
    std::array<char,256> path;assert(::ptsname_r(out.master,path.data(),path.size())==0);out.slave=::open(path.data(),O_RDWR|O_NOCTTY|O_CLOEXEC|O_NONBLOCK);assert(out.slave>=0);
    ::termios mode{};assert(::tcgetattr(out.slave,&mode)==0);::cfmakeraw(&mode);assert(::tcsetattr(out.slave,TCSANOW,&mode)==0);return out;
}
static void write_bytes(int fd,s::wire::bytes bytes,std::size_t maximum=7)
{
    while(!bytes.empty())
    {
        auto written=::write(fd,bytes.data(),std::min(bytes.size(),maximum));
        if(written<0){assert(errno==EINTR||errno==EAGAIN);if(errno==EAGAIN){::pollfd ready{fd,POLLOUT,0};assert(::poll(&ready,1,5000)==1);}continue;}
        assert(written>0);bytes=bytes.subspan(static_cast<std::size_t>(written));
    }
}
static p::packet read_frame(int fd,p::kind type)
{
    p::stream_decoder decoder{type};std::array<std::uint8_t,64> buffer;
    for(;;)
    {
        ::pollfd ready{fd,POLLIN,0};assert(::poll(&ready,1,5000)==1);auto size=::read(fd,buffer.data(),std::min(buffer.size(),decoder.remaining()));
        if(size<0){assert(errno==EINTR||errno==EAGAIN);continue;}assert(size>0);std::size_t consumed{};
        auto status=decoder.feed({buffer.data(),static_cast<std::size_t>(size)},consumed);assert(consumed==static_cast<std::size_t>(size));
        if(status==error::success){p::packet out;std::copy(decoder.frame().begin(),decoder.frame().end(),out.data.begin());out.size=decoder.frame().size();return out;}assert(status==error::again);
    }
}
static void actual_pty_receiver_partial_frame_and_independent_stop()
{
    auto normal=terminal(),stop=terminal(),acks=terminal();int state=::memfd_create("device-protocol-fixture",MFD_CLOEXEC);assert(state>=0&&::ftruncate(state,16)==0);
    auto child=::fork();assert(child>=0);
    if(!child)
    {
        ::close(normal.master);::close(stop.master);::close(acks.master);p::receiver receiver;
        assert(receiver.configure(4,schema(),profile("stop"))==error::success&&receiver.rearm_after_stopped(4,1)==error::success&&receiver.install_controller(work().controller)==error::success);
        p::stream_decoder normal_decoder{p::kind::command},stop_decoder{p::kind::stop};std::uint64_t ack_sequence{},commands{},stops{};std::array<std::uint8_t,64> buffer;
        auto deadline=uwvm2::imported::ros::resources::steady_now()+5000000000ll;
        while(commands<2||!stops)
        {
            assert(uwvm2::imported::ros::resources::steady_now()<deadline);std::array<::pollfd,2> ready{{{stop.slave,POLLIN,0},{normal.slave,POLLIN,0}}};auto n=::poll(ready.data(),ready.size(),100);assert(n>=0||errno==EINTR);if(n<=0)continue;
            for(std::size_t i{};i<ready.size();++i)if(ready[i].revents&POLLIN)
            {
                auto& decoder=i?normal_decoder:stop_decoder;auto amount=::read(ready[i].fd,buffer.data(),std::min(buffer.size(),decoder.remaining()));if(amount<0){assert(errno==EINTR||errno==EAGAIN);continue;}assert(amount>0);
                std::size_t consumed{};auto status=decoder.feed({buffer.data(),static_cast<std::size_t>(amount)},consumed);assert(consumed==static_cast<std::size_t>(amount));if(status==error::again)continue;assert(status==error::success);
                p::header h;p::acknowledgement outcome;d::scalar_command value;
                if(i)
                {
                    auto accepted=receiver.command(decoder.frame(),1001,h,value);++commands;
                    if(accepted==error::success){double set=value.values[0];assert(::pwrite(state,&set,sizeof(set),0)==sizeof(set));outcome=p::acknowledgement::executed;}
                    else{assert(accepted==error::denied);s::wire::bytes body;assert(p::decode(decoder.frame(),p::kind::command,h,body)==error::success);outcome=p::acknowledgement::rejected;}
                }
                else{assert(receiver.stop(decoder.frame(),h)==error::success);++stops;double stopped{};assert(::pwrite(state,&stopped,sizeof(stopped),0)==sizeof(stopped));outcome=p::acknowledgement::stop_accepted;}
                p::packet ack;assert(p::acknowledge(h,outcome,++ack_sequence,ack)==error::success);write_bytes(acks.slave,ack.view(),5);assert(decoder.next()==error::success);
            }
        }
        assert(::pwrite(state,&commands,sizeof(commands),8)==sizeof(commands));::_exit(0);
    }
    ::close(normal.slave);::close(stop.slave);::close(acks.slave);
    auto first=command();write_bytes(normal.master,first.view());auto reply=read_frame(acks.master,p::kind::acknowledgement);p::acknowledgement outcome;
    assert(p::verify_ack(reply.view(),header(first),1,outcome)==error::success&&outcome==p::acknowledgement::executed);double position{};assert(::pread(state,&position,sizeof(position),0)==sizeof(position)&&position==0.5);
    auto second=command(2,101);write_bytes(normal.master,second.view().first(20));p::packet stop_packet;assert(p::stop({102,d::state::queued,d::effect::locally_accepted,4},profile("stop"),1,stop_packet)==error::success);
    write_bytes(stop.master,stop_packet.view());reply=read_frame(acks.master,p::kind::acknowledgement);assert(p::verify_ack(reply.view(),header(stop_packet,p::kind::stop),2,outcome)==error::success&&outcome==p::acknowledgement::stop_accepted);
    assert(::pread(state,&position,sizeof(position),0)==sizeof(position)&&position==0);write_bytes(normal.master,second.view().subspan(20));reply=read_frame(acks.master,p::kind::acknowledgement);
    assert(p::verify_ack(reply.view(),header(second),3,outcome)==error::success&&outcome==p::acknowledgement::rejected);
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);std::uint64_t commands{};assert(::pread(state,&commands,sizeof(commands),8)==sizeof(commands)&&commands==2);
    assert(::pread(state,&position,sizeof(position),0)==sizeof(position)&&position==0);::close(normal.master);::close(stop.master);::close(acks.master);::close(state);
}
int main()
{
    exact_frame_bounds_digest_and_alias();byte_fragments_bounded_extra_and_poison();receiver_boot_lease_authority_deadline_and_stop();
    acknowledgements_bind_whole_original();actual_pty_receiver_partial_frame_and_independent_stop();std::cout<<"passed 5 device protocol safety groups\n";return 0;
}
