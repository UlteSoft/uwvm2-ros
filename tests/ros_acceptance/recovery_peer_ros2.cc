/** @file recovery_peer_ros2.cc
 * @brief Independently bound DDS recovery peer, including bounded stale discovery.
 *
 * CLI before|survivor, or after plus four pinned old endpoint GIDs from the
 * controller's immutable baseline record. Unknown/bare official name pairs may
 * wait only after stable type/hash/direction/GID/QoS checks. At most a known
 * retired row plus one new candidate is permitted; final matching remains
 * exactly one current endpoint. Neither WAIT nor GID change proves a new IPC
 * session. Worker/Gateway authentication is independently observed by the lane.
 */
#define main sdk_peer_uninvoked_entry
#include "../ros_sdk_relay/native_ros2_peer.cc"
#undef main
#include "recovery_protocol.h"

namespace recovery_ros2
{
    using namespace sdk_ros2_peer;
    gid parse_gid(std::string_view text)
    {
        require(text.size()==32,"old_gid");gid value{};
        auto digit=[](char c)->unsigned{if(c>='0'&&c<='9')return c-'0';if(c>='a'&&c<='f')return c-'a'+10;return 16;};
        for(unsigned n{};n!=16;++n){auto a=digit(text[n*2]),b=digit(text[n*2+1]);require(a<16&&b<16,"old_gid");value[n]=static_cast<std::uint8_t>((a<<4)|b);}require(nonzero(value),"old_gid");return value;
    }
    bool current(rclcpp::Node& node,facts& selected,facts const& old)
    {
        std::array<std::vector<rclcpp::TopicEndpointInfo>,4> rows{node.get_publishers_info_by_topic(in_topic),node.get_subscriptions_info_by_topic(out_topic),node.get_publishers_info_by_topic(out_topic),node.get_subscriptions_info_by_topic(in_topic)};
        std::array<gid,4> retired{old.own_pub,old.own_sub,old.gateway_pub,old.gateway_sub},next{};bool wait{};std::vector<gid> seen;
        for(unsigned kind{};kind!=4;++kind)
        {
            require(rows[kind].size()<=2,"graph_count");unsigned stale{},fresh{};
            for(auto const& info:rows[kind])
            {
                gid value{};bool qualified=endpoint(info,kind<2?peer_name:gateway_name,kind%2==0,value);
                require(std::find(seen.begin(),seen.end(),value)==seen.end(),"duplicate_gid");seen.push_back(value);
                if(nonzero(retired[kind])&&value==retired[kind]){require(++stale==1,"duplicate_retired");wait=true;continue;}
                require(++fresh==1,"two_fresh");next[kind]=value;if(!qualified)wait=true;
            }
            if(fresh!=1)wait=true;
        }
        if(wait)return false;selected={next[0],next[1],next[2],next[3]};return true;
    }
    struct observed
    {
        std::string_view mode;bool matched{};gid source{},warmup_source{};unsigned warmups{};std::array<unsigned,3> received{},sent{};
        void receive(message::ConstSharedPtr const& value,rclcpp::MessageInfo const& info)
        {
            require(value&&!info.get_rmw_message_info().from_intra_process,"message_origin");auto origin=copied(info.get_rmw_message_info().publisher_gid);
            if(value->data==warmup_text&&!warmups&&!received[0]&&!received[2]){warmup_source=origin;++warmups;return;}
            require(matched&&origin==source,"message_before_match_or_source");
            for(unsigned stage{};stage!=3;++stage)
            {
                if(value->data!=recovery_protocol::outbound[stage])continue;
                require(!received[stage]&&(mode=="survivor"||mode=="before"&&stage==0||mode=="after"&&stage==2),"stage_or_duplicate");
                if(mode=="survivor")require(stage==0||received[stage-1]==1,"stage_order");++received[stage];return;
            }
            require(false,"unknown_payload");
        }
    };
    template<typename... Tail> void record(std::string_view name,std::string_view mode,facts const& selected,Tail const&... tail)
    {emit(name," backend=2 mode=",mode," pid=",::getpid()," node=",full_peer," gateway_node=",full_gateway," domain=205 rmw=",implementation,
        " own_pub_gid=",hexadecimal(selected.own_pub)," own_sub_gid=",hexadecimal(selected.own_sub)," gateway_pub_gid=",hexadecimal(selected.gateway_pub),
        " gateway_sub_gid=",hexadecimal(selected.gateway_sub),tail...);}
}
int main(int argc,char** argv)
{
    using namespace recovery_ros2;
    try
    {
        require(argc>=2,"mode");std::string_view mode{argv[1]};require((argc==2&&(mode=="before"||mode=="survivor"))||(argc==6&&mode=="after"),"mode");
        facts old{};if(mode=="after")old={parse_gid(argv[2]),parse_gid(argv[3]),parse_gid(argv[4]),parse_gid(argv[5])};environment();control_descriptors();initial_gate();auto end=clock::now()+std::chrono::seconds{180};
        phase="initialization";logging_owner logging;context_owner context;rclcpp::InitOptions init;init.auto_initialize_logging(false);init.set_domain_id(205);char const* arguments[]{"recovery-peer"};context.value->init(1,arguments,init);
        auto native=rcl_context_get_rmw_context(context.value->get_rcl_context().get());require(native&&native->actual_domain_id==205&&equal(native->implementation_identifier,implementation),"context");
        facts selected;observed seen{mode};unsigned samples{};std::uint64_t quiet{};
        {
            rclcpp::NodeOptions options;options.context(context.value).use_global_arguments(false).enable_rosout(false).start_parameter_services(false).start_parameter_event_publisher(false).use_intra_process_comms(false);options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
            auto node=std::make_shared<rclcpp::Node>("sdk_peer","/uwvm_sdk_relay",options);auto configured=qos();auto pub=node->create_publisher<message>(in_topic,configured);
            auto sub=node->create_subscription<message>(out_topic,configured,[&](message::ConstSharedPtr value,rclcpp::MessageInfo const& info){seen.receive(value,info);});require(same_qos(pub->get_actual_qos(),configured)&&same_qos(sub->get_actual_qos(),configured),"local_qos");
            auto own=copied(pub->get_gid());
            // The executor and node must share this independently initialized
            // context; Jazzy selects it through its one-argument options API.
            rclcpp::ExecutorOptions executor_options;executor_options.context=context.value;
            rclcpp::executors::SingleThreadedExecutor executor{executor_options};executor.add_node(node);
            auto step=[&]{require(clock::now()<end,"deadline");executor.spin_some(std::chrono::milliseconds{2});require(clock::now()<end,"deadline");std::this_thread::sleep_for(std::chrono::milliseconds{2});};
            // READY qualifies the actual local publisher; complete endpoint and
            // cross-GID proof belongs to MATCHED, not to pre-discovery output.
            selected.own_pub=own;record("READY_RECOVERY_PEER",mode,selected);
            phase="discovery";
            for(;;){facts next;bool ready=current(*node,next,old);require(sub->get_publisher_count()<=2&&pub->get_subscription_count()<=2,"match_count");if(ready&&sub->get_publisher_count()==1&&pub->get_subscription_count()==1){require(next.own_pub==own,"own_gid");selected=next;break;}step();}
            require(!seen.warmups||seen.warmup_source==selected.gateway_pub,"warmup_source");seen.source=selected.gateway_pub;seen.matched=true;record("MATCHED_RECOVERY_PEER",mode,selected," publishers=1 subscribers=1");
            phase="delivery";bool finished{};
            while(!finished)
            {
                step();for(unsigned stage{};stage!=3;++stage)if(seen.received[stage]&&!seen.sent[stage])
                {require(pub->get_subscription_count()==1&&sub->get_publisher_count()==1,"reply_match");message value;value.data.assign(recovery_protocol::inbound[stage]);pub->publish(value);++seen.sent[stage];record("RECOVERY_NATIVE_DELIVERY",mode,selected," stage=",recovery_protocol::stages[stage]," received=1 sent=1");}
                finished=mode=="survivor"?seen.sent[2]==1:mode=="before"?seen.sent[0]==1:seen.sent[2]==1;
            }
            if(mode=="before")
            {
                while(!gate('X'))step();require(seen.received[0]==1&&seen.sent[0]==1,"effect_count");executor.cancel();executor.remove_node(node);
                sub.reset();pub.reset();node.reset();context.close();
                record("RETIRED_RECOVERY_PEER",mode,selected," before=1 during=0 after=0 excess=0 graceful=1");return 0;
            }
            while(!gate('Q'))step();phase="quiet";auto begin=clock::now();while(clock::now()-begin<std::chrono::seconds{2}){step();++samples;}quiet=std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now()-begin).count();
            require(samples>=30&&seen.received[0]+seen.received[1]+seen.received[2]==(mode=="survivor"?3u:1u),"effect_count");executor.cancel();executor.remove_node(node);
        }
        context.close();record("PASS_RECOVERY_PEER",mode,selected," before=",seen.received[0]," during=",seen.received[1]," after=",seen.received[2]," excess=0 quiet_samples=",samples," quiet_ns=",quiet);return 0;
    }
    catch(...){try{fast_io::println(fast_io::err(),"FAIL_RECOVERY_PEER backend=2 phase=",phase," field=",field);}catch(...){}return 1;}
}
