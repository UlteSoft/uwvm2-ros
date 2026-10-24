/* SPDX-License-Identifier: Apache-2.0 */
/** Future actual authenticated-Gateway witness. Root starts/owns this process
 * and a genuine selected-SDK Gateway separately. No fork, fake Native function,
 * metadata fixture default or compiler is embedded. Logical fixture handles
 * below do NOT prove a Guest/Host manager lease; only this private Gateway route
 * and its actual original RCL entities/transport are exercised. All source unrun.
 */
#include "src/ros/gateway/common/config.h"
#include <uwvm2/imported/ros/common/manifest.h>
#include <uwvm2/imported/ros/ipc/rmw_service.h>
#include <fast_io.h>
#include <thread>
#include <cstdlib>
namespace gw=uwvm2::ros_gateway;
namespace ros=uwvm2::imported::ros;
namespace ipc=ros::ipc;
namespace wire=ipc::rmw_service;
static unsigned checks{};
static void need(bool ok){++checks;if(!ok)throw std::runtime_error("Actual owned service Gateway contract refused");}
static ipc::call_result call(ipc::client& client,std::string_view alias,wire::control const& request,
    std::uint8_t const* body=nullptr,std::size_t bytes=0,bool private_cleanup=false)
{
    need(!private_cleanup||request.operation==wire::command::retire);
    std::array<std::uint8_t,wire::header_bytes> header{};need(wire::encode(request,false,header)&&request.body_bytes==bytes);
    std::vector<std::uint8_t> packet(header.size()+bytes);std::copy(header.begin(),header.end(),packet.begin());
    if(bytes)std::copy_n(body,bytes,packet.begin()+header.size());
    return client.call(private_cleanup?"rmw.service-retire-v1":"rmw.service-v1",alias,std::move(packet),5000);
}
static ipc::call_result poll(ipc::client& client,std::string_view alias,wire::control const& request,ipc::deadline until)
{
    for(;;){auto value=call(client,alias,request);if(value.error!=11)return value;
        if(std::chrono::steady_clock::now()>=until)return {110,{}};
        // Witness driver only; production positive available remains95.
        std::this_thread::sleep_for(std::chrono::milliseconds(1));}
}
int main(int argc,char** argv)
{
    if(argc!=7)return 22;
    try
    {
        // Root learns this real PID before writing the trusted worker config
        // and launching the owned Gateway. No native process is started here.
        fast_io::io::println("READY_ACTUAL_GATEWAY_WITNESS_REQUIRES_OWNED_G");
        char gate{};auto end=fast_io::operations::read_some(fast_io::posix_stdin(),&gate,&gate+1);need(end!=&gate&&gate=='G');
        auto cfg=gw::load(argv[1]);need(cfg.policy.ros_version==ros::version::ros2&&cfg.worker_pid==static_cast<std::uint32_t>(::getpid()));
        auto const& source=cfg.policy.capabilities.at(argv[2]);auto const& observer=cfg.policy.capabilities.at(argv[3]);
        auto const& server=cfg.policy.capabilities.at(argv[4]);auto const& responder=cfg.policy.capabilities.at(argv[5]);auto const& query=cfg.policy.capabilities.at(argv[6]);
        need(source.operation=="service.call"&&server.operation=="service.advertise"&&responder.operation=="service.respond"&&
            source.resource==server.resource&&source.type==server.type&&source.type_identity==server.type_identity);
        need(source.type=="std_srvs/srv/Empty"||source.type=="example_interfaces/srv/AddTwoInts");
        bool add=source.type=="example_interfaces/srv/AddTwoInts";
        ipc::client client;need(client.connect(cfg.socket,2,static_cast<std::uint32_t>(::geteuid()),5000)==0);
        need(client.call("session.bind","target",ros::serialize_manifest(cfg.policy),5000).error==0);
        struct cleanup{ipc::client& value;~cleanup(){value.close();}} retire{client};
        auto session=client.session_id();wire::control c{},s{};c.owner={2,UINT64_C(0x100000001),1,1,session};c.peer=UINT64_C(0x100000002);
        s.owner={2,UINT64_C(0x100000003),1,1,session};s.peer=UINT64_C(0x100000004);s.kind=ipc::rmw_endpoint::role::service;
        wire::control echo{};ipc::rmw_endpoint::packet endpoint{};
        auto result=call(client,server.alias,s);need(!result.error&&wire::associated_endpoint(result.payload.data(),result.payload.size(),s,echo,endpoint));s.incarnation=endpoint.incarnation;
        result=call(client,source.alias,c);need(!result.error&&wire::associated_endpoint(result.payload.data(),result.payload.size(),c,echo,endpoint));c.incarnation=endpoint.incarnation;
        auto available=c;available.operation=wire::command::available;available.peer=UINT64_C(0x100000005);
        result=call(client,query.alias,available);need(!result.error&&wire::associated_ack(result.payload.data(),result.payload.size(),available,echo)&&echo.value<=1);
        // Actual discovery can be false. Observe that exact same native client
        // with independent authority until the real witness deadline, not a
        // fake sleep-based implementation of the positive-wait ABI.
        auto until=ipc::after(5000);
        while(!echo.value){need(std::chrono::steady_clock::now()<until);std::this_thread::sleep_for(std::chrono::milliseconds(1));result=call(client,query.alias,available);need(!result.error&&wire::associated_ack(result.payload.data(),result.payload.size(),available,echo));}
        std::array<std::uint8_t,16> request{};ipc::event_put(request.data(),static_cast<std::uint64_t>(-7),8);ipc::event_put(request.data()+8,49,8);
        auto send=c;send.operation=wire::command::submit;send.body_bytes=add?16:0;
        result=call(client,source.alias,send,request.data(),static_cast<std::size_t>(send.body_bytes));need(!result.error&&wire::associated_ack(result.payload.data(),result.payload.size(),send,echo));auto submitted=echo.ticket;
        auto peek=s;peek.operation=wire::command::peek;peek.capacity=4096;peek.writer_bytes=16;
        result=poll(client,server.alias,peek,ipc::after(5000));wire::control borrowed{};ipc::rmw_metadata::header info{};
        need(!result.error&&wire::associated_delivery(result.payload.data(),result.payload.size(),peek,borrowed,info)&&ipc::rmw_metadata::complete(info));
        need(info.payload_bytes==(add?16u:0u));if(add)need(std::equal(request.begin(),request.end(),result.payload.begin()+wire::minimum_delivery_bytes));
        auto original_info=info;auto ticket=borrowed.ticket;
        auto ack=s;ack.operation=wire::command::cancel;ack.sequence=borrowed.sequence;ack.reservation=borrowed.reservation;
        need(call(client,server.alias,ack).error==0);
        result=call(client,server.alias,peek);wire::control again{};
        need(!result.error&&wire::associated_delivery(result.payload.data(),result.payload.size(),peek,again,info)&&again.sequence==borrowed.sequence&&again.reservation!=borrowed.reservation);
        ack.operation=wire::command::commit;ack.reservation=again.reservation;need(call(client,server.alias,ack).error==0);
        std::array<std::uint8_t,8> response{};ipc::event_put(response.data(),42,8);
        auto respond=s;respond.operation=wire::command::respond;respond.ticket=ticket;respond.body_bytes=add?8:0;
        need(call(client,responder.alias,respond,response.data(),static_cast<std::size_t>(respond.body_bytes)).error==0);
        peek=c;peek.operation=wire::command::peek;peek.capacity=4096;peek.writer_bytes=16;
        result=poll(client,source.alias,peek,ipc::after(5000));
        need(!result.error&&wire::associated_delivery(result.payload.data(),result.payload.size(),peek,borrowed,info)&&ipc::rmw_metadata::complete(info));
        need(info.correlation==submitted&&info.writer==original_info.writer&&info.request_sequence==original_info.request_sequence&&info.payload_bytes==(add?8u:0u));
        if(add)need(std::equal(response.begin(),response.end(),result.payload.begin()+wire::minimum_delivery_bytes));
        ack=c;ack.operation=wire::command::commit;ack.sequence=borrowed.sequence;ack.reservation=borrowed.reservation;need(call(client,source.alias,ack).error==0);
        // Exercise the ACTUAL public retirement lane first. Its real pair is
        // live and current; private cleanup is not silently substituted for it.
        auto stop=c;stop.operation=wire::command::retire;result=call(client,source.alias,stop);
        need(!result.error&&wire::associated_endpoint(result.payload.data(),result.payload.size(),stop,echo,endpoint));
        auto inspect=c;inspect.operation=wire::command::inspect;
        need(call(client,source.alias,inspect).error==125);
        // The distinct cleanup lane is explicitly selected only here. Alias
        // cleanup afterward must not perform either actual Native fini twice.
        stop=s;stop.operation=wire::command::retire;result=call(client,server.alias,stop,nullptr,0,true);
        need(!result.error&&wire::associated_endpoint(result.payload.data(),result.payload.size(),stop,echo,endpoint));
        need(client.call("capability.close",observer.alias,{},5000).error==0);
        need(client.call("capability.close",responder.alias,{},5000).error==0);
        client.close();fast_io::io::println("ACTUAL_GATEWAY_SERVICE_WIRE_COMPONENT checks=",checks," host_runtime_qualification=0");return 0;
    }
    catch(std::exception const& e){gw::native_io::diagnostic("FAIL actual Gateway service witness: ",e.what());return 1;}
}
