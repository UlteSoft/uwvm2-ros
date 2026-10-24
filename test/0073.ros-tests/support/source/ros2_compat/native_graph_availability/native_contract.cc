/* SPDX-License-Identifier: Apache-2.0 */
/** Genuine selected-SDK Native component, UNEXECUTED. Root independently owns
 * its isolated DDS domain, actual trusted profile, authenticated inherited UNIX
 * socket, pinned peer/worker and any real service peer. No expected identity
 * fixture, fake ROS header, replacement client or request send is used here.
 * Logical binding values below are fixture DATA, not actual Host handle leases.
 */
#include "src/ros/gateway/ros2/owned-service-profile.h"
#include <fast_io.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <unistd.h>
#include <csignal>
#include <climits>
using namespace uwvm2::ros_gateway;
namespace direct=native_rcl_service;
namespace graph=native_rcl_graph_wait;
static_assert(std::atomic<std::sig_atomic_t>::is_always_lock_free);
static std::atomic<std::sig_atomic_t> stopped{0};
static void cancel_wait(int)noexcept{stopped.store(1,std::memory_order_relaxed);}
static unsigned checks{};
static void need(bool ok){++checks;if(!ok)throw std::runtime_error("Actual Native graph wait contract refused");}
struct fd_owner{int value{-1};~fd_owner(){if(value>=0)::close(value);}};
static ros::capability_spec cap(std::string alias,std::string operation,std::string resource,
    uwvm_rmw_codec const& root)
{
    ros::capability_spec c{};c.alias=std::move(alias);c.operation=std::move(operation);c.resource=std::move(resource);
    c.type=root.native_type;c.type_identity=direct::identity_text(root.native_rihs01);
    c.limits={{"validator","none"},{"qos.history","keep-last"},{"qos.depth","4"},{"qos.reliability","reliable"},
        {"qos.durability","volatile"},{"qos.deadline","infinite"},{"qos.lifespan","infinite"},
        {"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"},
        {"max-request-bytes",root.request->maximum_wire_bytes?"16":"0"},
        {"max-response-bytes",root.response->maximum_wire_bytes?"8":"0"},{"max-inflight","4"},{"timeout-ms","5000"}};
    return c;
}
template<class Service>static void route(rclcpp::Node::SharedPtr const& node,
    std::shared_ptr<uwvm_rmw_cpp::Registry const> const& codecs,std::string const& resource,
    std::string_view mode,graph::lifetime const& lifetime)
{
    auto root=codecs->codec(direct::family_traits<Service>::codec);need(root&&root->request&&root->response);
    auto source=cap("graph-call","service.call",resource,*root);
    auto available=cap("graph-available","service.available",resource,*root);
    available.limits.erase("validator");available.limits["service-source-alias"]=source.alias;
    available.limits["max-inflight"]="1";available.limits["max-request-bytes"]="64";available.limits["max-response-bytes"]="64";
    available.limits["max-wait-ns"]="2000000000";available.limits["timeout-ms"]="5000";
    available.limits["max-native-clients"]="1";available.limits["max-binding-history"]="4";
    direct::wire::binding owner{2,UINT64_C(0x100000001),2,3,4},query_owner{2,UINT64_C(0x100000002),2,3,4};
    auto report=std::make_shared<direct::report>();std::shared_ptr<direct::client<Service>> client;direct::wire::packet observed{};
    need(direct::client<Service>::create(node,codecs,source,owner,11,4,report,client,observed)==0&&client);
    graph::domain domain{node};graph::admission waiting{&domain,lifetime};
    bool value=true;need(client->available_until(owner,11,query_owner,available,0,graph::clock::now()+std::chrono::seconds{5},{},value)==0&&!value);
    need(domain.observe().registration_attempts==0); // zero query created no graph loan
    auto denied=available;denied.limits["service-source-alias"]="different-source";value=true;
    need(client->available_until(owner,11,query_owner,denied,1000000,graph::clock::now()+std::chrono::seconds{5},waiting,value)==13&&value);
    auto missing=waiting;missing.graph=nullptr;
    need(client->available_until(owner,11,query_owner,available,1000000,graph::clock::now()+std::chrono::seconds{5},missing,value)==95&&value);
    need(domain.observe().registration_attempts==0);
    fast_io::io::println("NATIVE_GRAPH_WAIT_READY mode=",fast_io::mnp::strvw(mode)," checks=",checks);
    char go{};need(fast_io::operations::read_some(fast_io::posix_stdin(),&go,&go+1)==&go+1&&go=='G');
    auto entered=graph::clock::now();auto wait_ns=mode=="absent"?UINT64_C(120000000):UINT64_C(2000000000);
    auto until=entered+(mode=="transport"?std::chrono::milliseconds{25}:std::chrono::milliseconds{4000});
    value=true;auto result=client->available_until(owner,11,query_owner,available,wait_ns,until,waiting,value);
    auto elapsed=graph::clock::now()-entered;auto facts=domain.observe();auto calls=report->observe();
    need(facts.registration_attempts==1&&facts.registration_completed==1&&!facts.poisoned&&facts.wait_attempts>0);
    if(mode=="ready")need(result==0&&value);
    else if(mode=="absent")need(result==0&&!value&&elapsed>=std::chrono::nanoseconds{wait_ns});
    else if(mode=="disconnect")need(result==107&&value);
    else if(mode=="stop")need(result==125&&value);
    else if(mode=="transport")need(result==110&&value);
    else need(false);
    // No business request was sent/taken/responded by an availability query.
    need(calls.native[static_cast<unsigned>(direct::operation::init)].calls==1);
    for(auto op:{direct::operation::send,direct::operation::take,direct::operation::respond})need(calls.native[static_cast<unsigned>(op)].calls==0);
    need(calls.native[static_cast<unsigned>(direct::operation::available)].calls>=2);
    need(client->retire(owner,11)==0);calls=report->observe();
    need(calls.fini_attempted&&calls.fini_returned&&calls.fini_result==RCL_RET_OK&&calls.native[static_cast<unsigned>(direct::operation::fini)].calls==1);
    fast_io::io::println("ACTUAL_NATIVE_GRAPH_WAIT result=",result," value=",value," registrations=",facts.registration_attempts,
        " waits=",facts.wait_attempts," completed=",facts.wait_completed," elapsed_ns=",std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count(),
        " checks=",checks," Guest_Host_ROS_acceptance=0");
}
int main(int argc,char** argv)
{
    try
    {
        if(argc!=8||std::string_view{argv[1]}!="--owned-native-graph-wait")return 13;
#if !UWVM_NATIVE_RCL_SERVICE_API || !UWVM_NATIVE_RCL_AVAILABILITY_API || !UWVM_ROS2_HAVE_COMPLETE_RMW_QOS || !UWVM_NATIVE_RCL_FINITE_GRAPH_WAIT_API
        fast_io::io::perrln("UNAVAILABLE actual selected SDK graph wait feature=0");return 95;
#else
        std::string_view family{argv[3]},mode{argv[5]};
        need(family=="empty"||family=="add");need(mode=="absent"||mode=="ready"||mode=="disconnect"||mode=="stop"||mode=="transport");
        std::string resource{argv[4]};need(canonical(resource,false));
        int socket=static_cast<int>(integer(argv[6],INT_MAX));int worker=static_cast<int>(integer(argv[7],INT_MAX));need(worker>0);
        struct stat actual{};need(::fstat(socket,&actual)==0&&S_ISSOCK(actual.st_mode));
        int type{};socklen_t n=sizeof(type);need(::getsockopt(socket,SOL_SOCKET,SO_TYPE,&type,&n)==0&&n==sizeof(type)&&type==SOCK_STREAM);
        sockaddr_un peer{};n=sizeof(peer);need(::getpeername(socket,reinterpret_cast<sockaddr*>(&peer),&n)==0&&peer.sun_family==AF_UNIX);
        ucred credentials{};n=sizeof(credentials);need(::getsockopt(socket,SOL_SOCKET,SO_PEERCRED,&credentials,&n)==0&&n==sizeof(credentials)&&credentials.uid==::geteuid()&&credentials.pid==worker);
        fd_owner pinned{static_cast<int>(::syscall(SYS_pidfd_open,worker,0))};need(pinned.value>=0);
        struct sigaction action{},previous{};action.sa_handler=&cancel_wait;need(::sigemptyset(&action.sa_mask)==0&&::sigaction(SIGUSR1,&action,&previous)==0);
        struct signal_restore{struct sigaction old;~signal_restore(){::sigaction(SIGUSR1,&old,nullptr);}} restore{previous};
        auto profile=owned_service_profile::load(argv[2]);owned_service_profile::warm(profile);
        auto context=std::make_shared<rclcpp::Context>();char const* program="uwvm_native_graph_wait_contract";context->init(1,&program);
        struct context_cleanup{std::shared_ptr<rclcpp::Context> value;~context_cleanup(){if(value&&value->is_valid())value->shutdown("owned graph wait contract exit");}} cleanup{context};
        rclcpp::NodeOptions options;options.context(context); // retain every ORIGINAL NodeOptions default
        auto node=std::make_shared<rclcpp::Node>("uwvm_native_graph_wait_contract",options);
        graph::lifetime life{socket,pinned.value,&stopped};
        if(family=="empty")route<std_srvs::srv::Empty>(node,profile.registry,resource,mode,life);
        else route<example_interfaces::srv::AddTwoInts>(node,profile.registry,resource,mode,life);
        node.reset();need(context->shutdown("actual graph wait contract completed"));
        return 0;
#endif
    }
    catch(std::exception const& error){fast_io::io::perrln("FAIL_NATIVE_GRAPH_WAIT ",fast_io::mnp::strvw(std::string_view{error.what()}));return 1;}
}
