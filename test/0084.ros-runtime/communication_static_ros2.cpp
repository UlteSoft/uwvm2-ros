/** @file @brief Actual ROS2 peer using fixed source UDP against the public pipe transport. */
#include <uwvm2/imported/ros/scene/native/communication_static_transport.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_srvs/srv/set_bool.hpp>
#include <tf2_msgs/action/lookup_transform.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <sys/wait.h>
#include <cassert>
#include <cstdio>
namespace s=uwvm2::imported::ros::scene;
namespace c=s::communication;
namespace t=s::communication_transport;
using Action=tf2_msgs::action::LookupTransform;
using Service=std_srvs::srv::SetBool;
/** @brief Test-only public transport wrapper enforcing the peer's two receive ports.
 * @details Jazzy optionally requests a separate graph-discovery receive port.
 * The fixed route contract refuses that extra port; RMW's documented optional
 * fallback shares the declared user port. This wraps the official UDP transport
 * without patching RMW or opening an extra broker route.
 */
namespace peer_transport
{
    namespace api=eprosima::fastdds::rtps;
    namespace old=eprosima::fastrtps::rtps;
    struct descriptor;
    class fixed final:public api::TransportInterface
    {
        std::unique_ptr<api::TransportInterface> native;
        std::uint16_t discovery,user;
    public:
        fixed(api::UDPv4TransportDescriptor const& source,std::uint16_t d,std::uint16_t u)
            :TransportInterface(LOCATOR_KIND_UDPv4),native(source.create_transport()),discovery(d),user(u){}
        static void operator delete(void* pointer) noexcept
        {uwvm2::utils::allocator::fast_io_strict::native_strict_typed_global_allocator<fixed>::deallocate(static_cast<fixed*>(pointer));}
        bool init(old::PropertyPolicy const* p,std::uint32_t const& m)override{return native&&native->init(p,m);}
        bool OpenInputChannel(api::Locator const& l,api::TransportReceiverInterface* r,std::uint32_t m)override
        {return (l.port==discovery||l.port==user)&&native->OpenInputChannel(l,r,m);}
        bool IsInputChannelOpen(api::Locator const& l) const override{return native->IsInputChannelOpen(l);}
        bool IsLocatorSupported(api::Locator const& l) const override{return native->IsLocatorSupported(l);}
        bool is_locator_allowed(api::Locator const& l) const override{return native->is_locator_allowed(l);}
        api::Locator RemoteToMainLocal(api::Locator const& l) const override{return native->RemoteToMainLocal(l);}
        bool transform_remote_locator(api::Locator const& l,api::Locator& o) const override{return native->transform_remote_locator(l,o);}
        bool OpenOutputChannel(api::SendResourceList& s,api::Locator const& l)  override{return native->OpenOutputChannel(s,l);}
        bool CloseInputChannel(api::Locator const& l)  override{return native->CloseInputChannel(l);}
        bool DoInputLocatorsMatch(api::Locator const& a,api::Locator const& b) const override{return native->DoInputLocatorsMatch(a,b);}
        api::LocatorList NormalizeLocator(api::Locator const& l)  override{return native->NormalizeLocator(l);}
        void select_locators(old::LocatorSelector& s) const override{return native->select_locators(s);}
        bool is_local_locator(api::Locator const& l) const override{return native->is_local_locator(l);}
        api::TransportDescriptorInterface* get_configuration()  override{return native->get_configuration();}
        void AddDefaultOutputLocator(api::LocatorList& l)  override{return native->AddDefaultOutputLocator(l);}
        bool getDefaultMetatrafficMulticastLocators(api::LocatorList& l,std::uint32_t p) const override{return native->getDefaultMetatrafficMulticastLocators(l,p);}
        bool getDefaultMetatrafficUnicastLocators(api::LocatorList& l,std::uint32_t p) const override{return native->getDefaultMetatrafficUnicastLocators(l,p);}
        bool getDefaultUnicastLocators(api::LocatorList& l,std::uint32_t p) const override{return native->getDefaultUnicastLocators(l,p);}
        bool fillMetatrafficMulticastLocator(api::Locator& l,std::uint32_t p) const override{return native->fillMetatrafficMulticastLocator(l,p);}
        bool fillMetatrafficUnicastLocator(api::Locator& l,std::uint32_t p) const override{return native->fillMetatrafficUnicastLocator(l,p);}
        bool configureInitialPeerLocator(api::Locator& l,old::PortParameters const& p,std::uint32_t d,api::LocatorList& o) const override{return native->configureInitialPeerLocator(l,p,d,o);}
        bool fillUnicastLocator(api::Locator& l,std::uint32_t p) const override{return native->fillUnicastLocator(l,p);}
        std::uint32_t max_recv_buffer_size() const override{return native->max_recv_buffer_size();}
        void shutdown()  override{return native->shutdown();}
        bool is_localhost_allowed() const override{return native->is_localhost_allowed();}
    };
    struct descriptor final:api::UDPv4TransportDescriptor
    {
        std::uint16_t discovery{},user{};
        api::TransportInterface* create_transport() const override
        {
            api::UDPv4TransportDescriptor plain=*this;
            return uwvm2::utils::allocator::fast_io_strict::object<fixed>([&](fixed* raw)
            {return ::new(static_cast<void*>(raw)) fixed(plain,discovery,user);});
        }
    };
}
struct endpoints
{
    std::shared_ptr<rclcpp::Publisher<std_msgs::msg::String>> strings;
    std::shared_ptr<rclcpp::Publisher<std_msgs::msg::Bool>> booleans;
    std::shared_ptr<rclcpp::Subscription<std_msgs::msg::String>> string_sub;
    std::shared_ptr<rclcpp::Subscription<std_msgs::msg::Bool>> bool_sub;
    std::shared_ptr<rclcpp::Service<Service>> server;
    std::shared_ptr<rclcpp::Client<Service>> client;
    std::shared_ptr<rclcpp_action::Server<Action>> action_server;
    std::shared_ptr<rclcpp_action::Client<Action>> action_client;
    std::atomic<bool> string_received{},bool_received{},service_received{},service_served{},action_received{},action_served{};
    bool service_sent{},action_sent{};unsigned ticks{};
    endpoints(std::shared_ptr<rclcpp::Node> const& node,bool worker,bool canonical=false)
    {
        auto outgoing=canonical?"/scene/subscription":worker?"/pipe_request":"/pipe_reply",incoming=canonical?"/scene/publisher":worker?"/pipe_reply":"/pipe_request";
        strings=node->create_publisher<std_msgs::msg::String>(outgoing,10);
        booleans=node->create_publisher<std_msgs::msg::Bool>(canonical?"/scene/bool_subscription":worker?"/pipe_bool_request":"/pipe_bool_reply",10);
        string_sub=node->create_subscription<std_msgs::msg::String>(incoming,10,[this](std_msgs::msg::String const& m){if(m.data=="bounded route")string_received=true;});
        bool_sub=node->create_subscription<std_msgs::msg::Bool>(canonical?"/scene/bool_publisher":worker?"/pipe_bool_reply":"/pipe_bool_request",10,[this](std_msgs::msg::Bool const& m){if(m.data)bool_received=true;});
        server=node->create_service<Service>(canonical?"/scene/service_client":worker?"/pipe_service_down":"/pipe_service_up",[this](std::shared_ptr<Service::Request> request,std::shared_ptr<Service::Response> response){service_served=request->data;response->success=request->data;response->message="bounded service";});
        client=node->create_client<Service>(canonical?"/scene/service_server":worker?"/pipe_service_up":"/pipe_service_down");
        action_server=rclcpp_action::create_server<Action>(node,canonical?"/scene/action_client":worker?"/pipe_action_down":"/pipe_action_up",
            [](rclcpp_action::GoalUUID const&,std::shared_ptr<Action::Goal const> goal){return goal->target_frame=="world"?rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE:rclcpp_action::GoalResponse::REJECT;},
            [](std::shared_ptr<rclcpp_action::ServerGoalHandle<Action>>){return rclcpp_action::CancelResponse::REJECT;},
            [this](std::shared_ptr<rclcpp_action::ServerGoalHandle<Action>> goal){action_served=true;auto result=uwvm2::utils::container::make_shared<Action::Result>();result->transform.header.frame_id="world";result->transform.child_frame_id="robot";result->transform.transform.rotation.w=1;goal->succeed(result);});
        action_client=rclcpp_action::create_client<Action>(node,canonical?"/scene/action_server":worker?"/pipe_action_up":"/pipe_action_down");
    }
    void tick()
    {
        if(++ticks%50==0){std_msgs::msg::String text;text.data="bounded route";strings->publish(text);std_msgs::msg::Bool b;b.data=true;booleans->publish(b);}
        if(!service_sent&&client->service_is_ready())
        {auto request=uwvm2::utils::container::make_shared<Service::Request>();request->data=true;client->async_send_request(request,[this](rclcpp::Client<Service>::SharedFuture future){auto result=future.get();service_received=result->success&&result->message=="bounded service";});service_sent=true;}
        if(!action_sent&&action_client->action_server_is_ready())
        {Action::Goal goal;goal.target_frame="world";goal.source_frame="robot";rclcpp_action::Client<Action>::SendGoalOptions options;options.result_callback=[this](rclcpp_action::ClientGoalHandle<Action>::WrappedResult const& result){action_received=result.code==rclcpp_action::ResultCode::SUCCEEDED&&result.result&&result.result->transform.child_frame_id=="robot";};action_client->async_send_goal(goal,options);action_sent=true;}
    }
    bool complete() const noexcept{return string_received&&bool_received&&service_received&&service_served&&action_received&&action_served;}
};
[[maybe_unused]] static t::plan policy()
{
    t::plan p;p.generation=735;p.domain=194;p.local_participant=0;p.peer_participant=1;
    p.local_discovery=55910;p.local_user=55911;p.peer_discovery=55912;p.peer_user=55913;
    p.local_discovery_source=60120;p.local_user_source=60121;p.peer_source=60122;
    p.local_prefix={1,15,3,4,5,6,7,8,9,10,11,1};p.peer_prefix={1,15,8,7,6,5,4,3,9,10,11,2};
    p.allowed.fill(true);p.user_data_receive=p.user_data_transmit=true;p.frame_deadline_ns=5000000000ull;p.max_datagram_bytes=4096;p.max_sample_bytes=65536;p.max_submessages=128;p.socket_buffer_bytes=262144;
    for(auto& q:p.queues){q.packets=128;q.bytes=128*4096;}assert(p.valid());return p;
}
static void environment()
{assert(!::setenv("RMW_IMPLEMENTATION","rmw_fastrtps_cpp",1));assert(!::setenv("ROS_AUTOMATIC_DISCOVERY_RANGE","SYSTEM_DEFAULT",1));assert(!::setenv("SKIP_DEFAULT_XML","1",1));assert(!::setenv("RMW_FASTRTPS_PUBLICATION_MODE","SYNCHRONOUS",1));}
static std::shared_ptr<rclcpp::Context> context(t::plan const& p)
{
    auto result=uwvm2::utils::container::make_shared<rclcpp::Context>();rclcpp::InitOptions init;init.shutdown_on_signal=false;init.auto_initialize_logging(false);init.set_domain_id(p.domain);
    char const* args[]={"static-peer"};result->init(1,args,init);return result;
}
[[maybe_unused]] static void native_peer(t::plan const& p,int completion,int release,bool canonical=false)
{
    environment();auto factory=eprosima::fastdds::dds::DomainParticipantFactory::get_instance();factory->load_profiles();auto qos=factory->get_default_participant_qos();
    auto udp=uwvm2::utils::container::make_shared<peer_transport::descriptor>();udp->discovery=p.peer_discovery;udp->user=p.peer_user;udp->interfaceWhiteList.push_back("127.0.0.1");udp->m_output_udp_socket=p.peer_source;udp->maxMessageSize=p.max_datagram_bytes;
    qos.transport().use_builtin_transports=false;qos.transport().user_transports.clear();qos.transport().user_transports.push_back(udp);
    auto& w=qos.wire_protocol();w.participant_id=p.peer_participant;std::copy(p.peer_prefix.begin(),p.peer_prefix.end(),w.prefix.value);
    w.builtin.metatrafficMulticastLocatorList.clear();w.builtin.metatrafficUnicastLocatorList.clear();w.builtin.metatrafficUnicastLocatorList.push_back(c::ros2::loopback_locator(p.peer_discovery));
    w.builtin.initialPeersList.clear();w.builtin.initialPeersList.push_back(c::ros2::loopback_locator(p.local_discovery));w.default_multicast_locator_list.clear();w.default_unicast_locator_list.clear();w.default_unicast_locator_list.push_back(c::ros2::loopback_locator(p.peer_user));
    assert(factory->set_default_participant_qos(qos)==eprosima::fastrtps::types::ReturnCode_t::RETCODE_OK);
    auto ctx=context(p);rclcpp::NodeOptions options;options.context(ctx);options.enable_rosout(false);options.start_parameter_services(false);options.start_parameter_event_publisher(false);
    auto node=uwvm2::utils::container::make_shared<rclcpp::Node>("native_peer",options);
    rclcpp::ExecutorOptions eo;eo.context=ctx;rclcpp::executors::SingleThreadedExecutor executor{eo};executor.add_node(node);
    {
        endpoints endpoints{node,false,canonical};if(canonical){char ready='R';assert(::write(completion,&ready,1)==1);}
        auto until=std::chrono::steady_clock::now()+std::chrono::seconds{12};bool reported{},released{};
        while(std::chrono::steady_clock::now()<until)
        {
            executor.spin_some();endpoints.tick();
            if(endpoints.complete()&&!reported){char marker=1;assert(::write(completion,&marker,1)==1);reported=true;}
            char marker{};auto got=::read(release,&marker,1);
            if(got==1)
            {
                if(canonical&&marker=='g'){auto names=node->get_node_names();char present=std::find(names.begin(),names.end(),"/scene/robot")!=names.end()?'1':'0';assert(::write(completion,&present,1)==1);}
                else{assert(marker==1);released=true;break;}
            }
            else {if(got==0)std::fprintf(stderr,"native peer before EOF sent=%d/%d flags=%d%d%d%d%d%d\n",endpoints.service_sent,endpoints.action_sent,endpoints.string_received.load(),endpoints.bool_received.load(),endpoints.service_received.load(),endpoints.service_served.load(),endpoints.action_received.load(),endpoints.action_served.load());assert(got<0&&errno==EAGAIN);}
            ::usleep(2000);
        }
        assert(endpoints.complete()&&reported&&released);
    }
    executor.remove_node(node);node.reset();ctx->shutdown("complete");
}
#ifndef UWVM_COMMUNICATION_STATIC_FIXTURE_ONLY
int main()
{
    auto p=policy();int completed[2],release[2];assert(!::pipe2(completed,O_NONBLOCK|O_CLOEXEC)&&!::pipe2(release,O_NONBLOCK|O_CLOEXEC));
    auto child=::fork();assert(child>=0);if(!child){::close(completed[0]);::close(release[1]);native_peer(p,completed[1],release[0]);::_exit(0);}
    ::close(completed[1]);::close(release[0]);
    environment();t::broker broker;assert(broker.open(p));std::array<s::communication_worker::role_fd,4> pipes;assert(broker.take_worker_pipes(pipes));
    auto policy=c::ros2::static_network::install(p);assert(policy&&policy->valid());auto ctx=context(p);
    rclcpp::NodeOptions options;options.context(ctx);options.enable_rosout(false);options.start_parameter_services(false);options.start_parameter_event_publisher(false);
    auto node=uwvm2::utils::container::make_shared<rclcpp::Node>("pipe_worker",options);
    rclcpp::ExecutorOptions eo;eo.context=ctx;rclcpp::executors::SingleThreadedExecutor executor{eo};executor.add_node(node);
    {
        endpoints endpoints{node,true};assert(policy->activate(pipes));assert(broker.activate());
        auto until=std::chrono::steady_clock::now()+std::chrono::seconds{10};bool peer_complete{};
        while((!endpoints.complete()||!peer_complete)&&std::chrono::steady_clock::now()<until)
        {executor.spin_some();endpoints.tick();if(!peer_complete){char marker{};auto got=::read(completed[0],&marker,1);if(got==1){assert(marker==1);peer_complete=true;}else assert(got<0&&errno==EAGAIN);}::usleep(2000);}
        auto snapshot=broker.inspect();std::fprintf(stderr,"relay active=%d fenced=%d counts=%lu,%lu,%lu,%lu rejected=%lu,%lu,%lu,%lu healthy=%d completed=%d flags=%d%d%d%d%d%d\n",snapshot.active,snapshot.fenced,snapshot.forwarded[0],snapshot.forwarded[1],snapshot.forwarded[2],snapshot.forwarded[3],snapshot.rejected[0],snapshot.rejected[1],snapshot.rejected[2],snapshot.rejected[3],policy->healthy(),endpoints.complete(),endpoints.string_received.load(),endpoints.bool_received.load(),endpoints.service_received.load(),endpoints.service_served.load(),endpoints.action_received.load(),endpoints.action_served.load());
        assert(endpoints.complete()&&peer_complete&&policy->healthy()&&!snapshot.fenced&&snapshot.forwarded[0]&&snapshot.forwarded[1]&&snapshot.forwarded[2]&&snapshot.forwarded[3]);
        char marker=1;assert(::write(release[1],&marker,1)==1);
    }
    executor.remove_node(node);node.reset();ctx->shutdown("complete");broker.revoke();for(auto fd:pipes)::close(fd.descriptor);
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
    ::close(completed[0]);::close(release[1]);
    std::puts("passed actual ROS2 String/Bool topics, SetBool services and LookupTransform actions in both directions through public FastDDS custom transport and fixed directional UDP broker");
}

#endif
