/* SPDX-License-Identifier: Apache-2.0 */
/** Actual production value/parser helpers only. No fake ROS entity, middleware
 * function or Native identity proof is installed. All authored groups unrun. */
#include "src/ros/gateway/common/owned_rmw_service.h"
#include "src/ros/gateway/ros2/owned-service-profile.h"
#include <cstdlib>
#include <fast_io.h>
namespace gw=uwvm2::ros_gateway;
namespace ros=uwvm2::imported::ros;
static unsigned groups{};
static void need(bool value){if(!value)std::abort();}
static ros::capability_spec source()
{ros::capability_spec c{};c.alias="call";c.operation="service.call";c.resource="/service";c.type="std_srvs/srv/Empty";return c;}
static ros::capability_spec observer()
{ros::capability_spec c{};c.alias="observe";c.operation="event.observe";c.resource="/service";c.type=ros::ipc::event_type;c.type_identity=ros::ipc::event_type_identity;c.limits={{"event-source-alias","call"},{"event-kinds","service-response"}};return c;}
int main(int argc,char** argv)
{
    auto run=[](auto fn){fn();++groups;};
    run([]{ // Pure grammar DATA is never passed off as an actual Native hash.
        uwvm_rmw_cpp::Hash value{};value.fill(0x5a);auto old=value;
        need(!gw::owned_service_profile::hash("",value)&&value==old);
        need(!gw::owned_service_profile::hash(std::string(64,'0'),value)&&value==old);
        need(!gw::owned_service_profile::hash(std::string(64,'A'),value)&&value==old);
        need(gw::owned_service_profile::hash(std::string(64,'1'),value)&&value[0]==0x11&&value[31]==0x11);
    });
    run([]{auto c=source();need(gw::owned_service_body(c,false,nullptr,0));need(gw::owned_service_body(c,true,nullptr,0));
        std::uint8_t b{};need(!gw::owned_service_body(c,false,&b,1)&&!gw::owned_service_body(c,true,&b,1));});
    run([]{auto c=source();c.type="example_interfaces/srv/AddTwoInts";std::array<std::uint8_t,17> body{};body.fill(0xff);
        need(gw::owned_service_body(c,false,body.data(),16)&&gw::owned_service_body(c,true,body.data(),8));
        for(std::size_t size:{0u,7u,8u,15u,17u})need(!gw::owned_service_body(c,false,body.data(),size));
        need(!gw::owned_service_body(c,false,nullptr,16));});
    run([]{auto c=source();c.limits["adapter-codec-identity"]="unproven";
        need(!gw::owned_service_body(c,false,nullptr,0));c.limits.clear();c.type="vendor/srv/WidthIsNotProof";need(!gw::owned_service_body(c,false,nullptr,0));});
    run([]{gw::configuration cfg;auto c=source(),o=observer();cfg.policy.capabilities.emplace(c.alias,c);cfg.policy.capabilities.emplace(o.alias,o);
        auto p=gw::owned_service_pair(cfg,c,ros::ipc::rmw_endpoint::role::client);need(p&&p->alias==o.alias);
        o.alias="second";cfg.policy.capabilities.emplace(o.alias,o);need(!gw::owned_service_pair(cfg,c,ros::ipc::rmw_endpoint::role::client));});
    run([]{gw::configuration cfg;auto c=source(),o=observer();o.limits["event-kinds"]="resource-error";cfg.policy.capabilities.emplace(o.alias,o);
        need(!gw::owned_service_pair(cfg,c,ros::ipc::rmw_endpoint::role::client));o=observer();o.resource="/other";cfg.policy.capabilities[o.alias]=o;
        need(!gw::owned_service_pair(cfg,c,ros::ipc::rmw_endpoint::role::client));});
    run([]{need(gw::owned_service_operation("rmw.service-v1")&&gw::owned_service_operation("rmw.service-retire-v1"));
        for(auto name:{"service.call","service.submit","service.respond","event.poll","rmw.publish-v1"})need(!gw::owned_service_operation(name));});
    run([]{ // Actual default seam is honest unsupported, not a fake endpoint.
        gw::owned_service_backend unselected;need(unselected.owned_rmw_service_capacity()==0);
        auto r=unselected.owned_rmw_service({}, {},nullptr,0,ros::ipc::after(1));need(r.error==95&&r.payload.empty());});
    run([]{using c=ros::ipc::rmw_service::command;
        for(auto command:{c::activate,c::inspect,c::retire,c::submit,c::peek,c::commit,c::cancel,c::respond,c::available,c::abandon})
        {need(gw::owned_service_control_lane("rmw.service-v1",command));need(gw::owned_service_control_lane("rmw.service-retire-v1",command)==(command==c::retire));}
        need(!gw::owned_service_control_lane("service.call",c::retire));need(!gw::owned_service_control_lane("rmw.service-v1",static_cast<c>(255)));});
    run([]{using c=ros::ipc::rmw_service::command;ros::ipc::call_result idle{11,{}};
        need(gw::owned_service_idle_reply(c::peek,idle));
        idle.payload.push_back(0);need(!gw::owned_service_idle_reply(c::peek,idle));idle.payload.clear();
        for(auto command:{c::submit,c::respond,c::commit,c::cancel,c::retire,c::available})need(!gw::owned_service_idle_reply(command,idle));
        for(auto error:{0u,17u,90u,95u,107u,110u}){idle.error=error;need(!gw::owned_service_idle_reply(c::peek,idle));}});
    if(argc==2)
    {
        // Optional separately admitted ACTUAL profile file tests the real
        // no-follow bounded reader. It does not warm/prove any Native identity.
        auto selected=gw::owned_service_profile::load(argv[1]);need(selected.storage.valid()&&selected.maximum_string_wire>=4&&!selected.registry);++groups;
    }
    else need(argc==1);
    fast_io::io::println("OWNED_SERVICE_GATEWAY_VALUE groups=",groups," native_calls=0");
}
