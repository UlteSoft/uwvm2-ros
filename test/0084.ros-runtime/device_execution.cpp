/** @file @brief Typed execution against real PTY peers with independent stop and faulted acknowledgements. */
#define main prior_device_core_main
#include "device_core.cpp"
#undef main
#include <uwvm2/imported/ros/scene/device_execution.h>
#include <poll.h>
namespace protocol=d::protocol;
namespace ctl=s::control;
struct terminals
{
    int master{-1},slave{-1};
    terminals()
    {
        master=::posix_openpt(O_RDWR|O_NOCTTY|O_NONBLOCK|O_CLOEXEC);assert(master>=0&&::grantpt(master)==0&&::unlockpt(master)==0);
        std::array<char,256> name;assert(::ptsname_r(master,name.data(),name.size())==0);
        slave=::open(name.data(),O_RDWR|O_NOCTTY|O_NONBLOCK|O_CLOEXEC);assert(slave>=0);
        ::termios mode;assert(::tcgetattr(slave,&mode)==0);::cfmakeraw(&mode);assert(::tcsetattr(slave,TCSANOW,&mode)==0);
    }
    ~terminals(){::close(master);::close(slave);}
};
static protocol::packet receive(int fd,protocol::kind kind)
{
    protocol::stream_decoder decoder{kind};
    while(decoder.frame().empty())
    {
        pollfd ready{fd,POLLIN,0};assert(::poll(&ready,1,5000)==1);
        std::array<std::uint8_t,256> bytes;auto amount=::read(fd,bytes.data(),std::min(bytes.size(),decoder.remaining()));
        if(amount<0){assert(errno==EINTR||errno==EAGAIN);continue;}
        assert(amount>0);std::size_t used{};auto result=decoder.feed({bytes.data(),static_cast<std::size_t>(amount)},used);
        assert(result==error::success||result==error::again);
    }
    protocol::packet packet;packet.size=decoder.frame().size();std::copy(decoder.frame().begin(),decoder.frame().end(),packet.data.begin());return packet;
}
static void send(int fd,s::wire::bytes data)
{
    while(!data.empty())
    {auto amount=::write(fd,data.data(),data.size());if(amount<0){assert(errno==EINTR||errno==EAGAIN);continue;}
        assert(amount);data=data.subspan(static_cast<std::size_t>(amount));}
}
struct executing:fixture
{
    terminals command,ack,stop,stop_ack;ctl::arbitration arb;ctl::lease lease;
    d::execution executor{core,arb};
    executing(std::size_t maximum_write=4096,bool install=true)
    {
        assert(arb.configure(1,3,4,{1000,100,10,false})==error::success);ctl::prepared ready;
        assert(arb.prepare({{1,11,3,12},1,100},1000,ready)==error::success&&arb.commit(ready,1000)==error::success);lease=ready.candidate;
        assert(executor.bind(command.master,ack.master,stop.master,stop_ack.master,plan.schema,plan.stop_profile,{maximum_write,200})==error::success);
        assert(executor.stage_controller(lease,1000)==error::success);
        if(install)
        {
            for(unsigned i{};i<100;++i){auto status=pump();assert(status==error::again);}
            auto frame=receive(command.slave,protocol::kind::controller);protocol::receiver receiver;protocol::header header;
            assert(receiver.configure(4,plan.schema,plan.stop_profile)==error::success&&receiver.rearm_after_stopped(4,1)==error::success);
            assert(receiver.controller(frame.view(),1001,header)==error::success);protocol::packet response;
            assert(protocol::acknowledge(header,protocol::acknowledgement::controller_installed,1,response)==error::success);send(ack.slave,response.view());
            d::execution::status status;
            for(unsigned i{};i<100;++i){assert(pump()==error::again);assert(executor.inspect(status)==error::success);if(status.controller_ready)break;}
            assert(status.controller_ready);
        }
    }
    d::receipt submit()
    {auto request=submission();request.controller=lease;auto pins=evidence();d::receipt output;assert(core.submit(owner,request,pins,1000,output)==error::success);return output;}
    error pump(std::uint64_t time=1001){return executor.pump(time,[](auto const&,auto){return error::success;});}
    d::receipt result(d::receipt const& submitted){d::receipt out;assert(core.result(owner,device,submitted.request,out)==error::success);return out;}
    void acknowledge(protocol::kind kind,protocol::acknowledgement result,bool wrong=false)
    {
        auto packet=receive(kind==protocol::kind::command?command.slave:stop.slave,kind);protocol::header header;s::wire::bytes bytes;
        assert(protocol::decode(packet.view(),kind,header,bytes)==error::success);if(wrong)header.digest[0]^=1;
        protocol::packet response;assert(protocol::acknowledge(header,result,1,response)==error::success);send(kind==protocol::kind::command?ack.slave:stop_ack.slave,response.view());
    }
};
static void actual_process_receipt_and_no_write_completion()
{
    for(bool rejected:{false,true})
    {
        executing f{4096,false};auto request=f.submit();auto child=::fork();assert(child>=0);
        if(!child)
        {
            protocol::receiver receiver;assert(receiver.configure(4,f.plan.schema,f.plan.stop_profile)==error::success);
            assert(receiver.rearm_after_stopped(4,1)==error::success);
            auto installation=receive(f.command.slave,protocol::kind::controller);protocol::header installed;
            assert(receiver.controller(installation.view(),1001,installed)==error::success);protocol::packet installed_ack;
            assert(protocol::acknowledge(installed,protocol::acknowledgement::controller_installed,1,installed_ack)==error::success);send(f.ack.slave,installed_ack.view());
            auto frame=receive(f.command.slave,protocol::kind::command);protocol::header header;d::scalar_command value;
            assert(receiver.command(frame.view(),1001,header,value)==error::success&&value.count==1&&value.values[0]==0.5);
            protocol::packet response;assert(protocol::acknowledge(header,rejected?protocol::acknowledgement::rejected:protocol::acknowledgement::executed,1,response)==error::success);
            send(f.ack.slave,response.view());::_exit(0);
        }
        for(unsigned i{};f.result(request).status!=d::state::succeeded&&f.result(request).status!=d::state::failed;++i)
        {assert(i<5000);auto status=f.pump();assert(status==error::success||status==error::again);::usleep(100);}
        auto outcome=f.result(request);assert(outcome.status==(rejected?d::state::failed:d::state::succeeded)&&outcome.outcome==(rejected?d::effect::none:d::effect::committed));
        int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
        assert(f.core.release(owner,request.request)==error::success);
    }
}
static void no_write_denial_and_partial_unknown_with_stop()
{
    executing denied;auto refused=denied.submit();assert(denied.executor.pump(1001,[](auto const&,auto){return error::denied;})==error::denied);
    assert(denied.result(refused).outcome==d::effect::none&&denied.usage().stop_pending);
    std::array<char,16> bytes;assert(::read(denied.command.slave,bytes.data(),bytes.size())==-1&&errno==EAGAIN);
    executing partial{7};auto request=partial.submit();assert(partial.pump()==error::again);
    assert(partial.executor.pump(1002,[](auto const&,auto){return error::denied;})==error::denied);
    assert(partial.result(request).status==d::state::outcome_unknown&&partial.usage().pins==1&&partial.usage().stop_pending);
    for(unsigned i{};i<100;++i)(void)partial.pump(1003);
    partial.acknowledge(protocol::kind::stop,protocol::acknowledgement::stopped);
    for(unsigned i{};i<100&&!partial.usage().stop_acknowledged;++i)(void)partial.pump(1003);
    assert(partial.usage().stop_acknowledged&&!partial.usage().stopped_observed&&partial.usage().unknown==1);
    assert(partial.core.release(owner,request.request)==error::again);
}
static void missing_and_corrupt_ack_latch()
{
    for(bool corrupt:{false,true})
    {
        executing f;auto request=f.submit();assert(f.pump()==error::again);
        if(corrupt)f.acknowledge(protocol::kind::command,protocol::acknowledgement::executed,true);
        if(corrupt){for(unsigned i{};i<100&&!f.usage().unknown;++i)(void)f.pump(1001);}
        else assert(f.pump(1050)==error::timeout);
        assert(f.result(request).status==d::state::outcome_unknown&&f.usage().stop_pending);
        (void)f.pump(1051);assert(f.pump(1251)==error::conflict);d::receipt stop;
        assert(f.core.pending_stop(stop)==error::success&&stop.status==d::state::outcome_unknown);
    }
}
static void controller_stop_resolves_inflight_uncertainty()
{
    for(auto maximum_write:{std::size_t{7},std::size_t{4096}})
    {
        executing f{maximum_write};auto request=f.submit();assert(f.pump()==error::again);
        ctl::handle stop{};assert(f.arb.stop(stop)==error::success);
        assert(f.pump()==error::cancelled);
        auto result=f.result(request);
        assert(result.status==d::state::outcome_unknown&&result.outcome==d::effect::unknown);
        assert(f.usage().pins==1&&f.usage().unknown==1&&f.usage().stop_pending);
        assert(f.core.release(owner,request.request)==error::again);
        // Stop acceptance cannot reclassify an unacknowledged command as no effect.
        for(unsigned i{};i<100;++i)(void)f.pump();
        f.acknowledge(protocol::kind::stop,protocol::acknowledgement::stopped);
        for(unsigned i{};i<100&&!f.usage().stop_acknowledged;++i)(void)f.pump();
        assert(f.usage().stop_acknowledged&&!f.usage().stopped_observed&&f.usage().unknown==1);
        assert(f.result(request)==result);
    }
    executing unwritten;auto request=unwritten.submit();ctl::handle stop{};
    assert(unwritten.arb.stop(stop)==error::success&&unwritten.pump()==error::cancelled);
    assert(unwritten.result(request).outcome==d::effect::none&&!unwritten.usage().unknown);
    std::array<char,16> bytes;assert(::read(unwritten.command.slave,bytes.data(),bytes.size())==-1&&errno==EAGAIN);
}
static void separation_raw_mode_and_fork_fences()
{
    fixture f;ctl::arbitration arb;terminals a,b,c,e;d::execution executor{f.core,arb};
    termios mode{};assert(::tcgetattr(a.master,&mode)==0);mode.c_lflag|=ECHO;assert(::tcsetattr(a.master,TCSANOW,&mode)==0);
    assert(executor.bind(a.master,b.master,c.master,e.master,f.plan.schema,f.plan.stop_profile,{7,200})==error::denied);
    ::cfmakeraw(&mode);assert(::tcsetattr(a.master,TCSANOW,&mode)==0);
    assert(executor.bind(a.master,b.master,a.master,e.master,f.plan.schema,f.plan.stop_profile,{7,200})==error::denied);
    executing live;auto child=::fork();assert(child>=0);if(!child){d::execution::status status;::_exit(live.executor.inspect(status)==error::denied?0:1);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
}
int main()
{actual_process_receipt_and_no_write_completion();no_write_denial_and_partial_unknown_with_stop();missing_and_corrupt_ack_latch();
 controller_stop_resolves_inflight_uncertainty();separation_raw_mode_and_fork_fences();std::puts("passed 5 device execution safety groups");}
