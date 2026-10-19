/** @file recovery_peer_ros1.cc
 * @brief Separate TCPROS peer for before/fault/after recovery phases.
 *
 * CLI before|survivor|after; I follows stopped ARM mapping, Q follows actual
 * Guest exit0. A before actor is intentionally retired by the controller after
 * its exact first delivery. survivor stays alive and receives three distinct
 * stage values. after is a freshly mapped owned process/registration. No
 * external Master, network namespace or unrelated process is changed here.
 */
#define main sdk_peer_uninvoked_entry
#include "../ros_sdk_relay/native_ros1_peer.cc"
#undef main
#include "recovery_protocol.h"

namespace recovery_ros1
{
    using namespace sdk_ros1_peer;
    struct observed
    {
        std::string_view mode;bool matched{};unsigned warmups{};std::array<unsigned,3> received{},sent{};
        void receive(ros::MessageEvent<std_msgs::String const> const& event)
        {
            auto message=event.getMessage();auto header=event.getConnectionHeaderPtr();require(message&&header&&event.getPublisherName()==gateway,"callerid");
            require(header->at("md5sum")==md5&&header->at("type")=="std_msgs/String"&&header->at("callerid")==gateway,"tcpros_header");
            if(message->data==warmup&&!warmups&&!received[0]&&!received[2]){++warmups;return;}
            require(matched,"message_before_match");
            for(unsigned stage{};stage!=3;++stage)
            {
                if(message->data!=recovery_protocol::outbound[stage])continue;
                require(!received[stage]&&(mode=="survivor"||mode=="before"&&stage==0||mode=="after"&&stage==2),"stage_or_duplicate");
                if(mode=="survivor")require(stage==0||received[stage-1]==1,"stage_order");
                ++received[stage];return;
            }
            require(false,"unknown_payload");
        }
    };
    template<typename... Tail> void record(std::string_view name,std::string_view mode,Tail const&... tail)
    {emit(name," backend=1 mode=",mode," pid=",::getpid()," node=",peer," gateway_node=",gateway," transport=tcpros md5=",md5,tail...);}
}
int main(int argc,char** argv)
{
    using namespace recovery_ros1;
    try
    {
        require(argc==2&&(equal(argv[1],"before")||equal(argv[1],"survivor")||equal(argv[1],"after")),"mode");std::string_view mode{argv[1]};environment();
        auto initial=clock::now()+std::chrono::seconds{20};while(!gate('I')){require(clock::now()<initial,"deadline");std::this_thread::sleep_for(std::chrono::milliseconds{2});}
        auto until=clock::now()+std::chrono::seconds{180};ros::M_string remappings;remappings["__ns"]="/uwvm_sdk_relay";
        ros::init(remappings,"sdk_peer",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);ros::NodeHandle node{"/uwvm_sdk_relay"};require(ros::this_node::getName()==peer,"node_identity");
        observed facts{mode};auto publisher=node.advertise<std_msgs::String>(in_topic,4,false);
        ros::SubscribeOptions options;options.initByFullCallbackType<ros::MessageEvent<std_msgs::String const> const&>(out_topic,4,[&](auto const& event){facts.receive(event);});
        options.transport_hints=ros::TransportHints{}.tcpNoDelay().reliable();auto subscriber=node.subscribe(options);
        auto step=[&]{require(ros::ok()&&clock::now()<until,"deadline");ros::spinOnce();std::this_thread::sleep_for(std::chrono::milliseconds{2});};
        phase="discovery";for(;;){auto value=graph();if(value.own_publishers==1&&value.own_subscribers==1)break;step();}
        record("READY_RECOVERY_PEER",mode);
        for(;;){auto value=graph();require(publisher.getNumSubscribers()<=1&&subscriber.getNumPublishers()<=1,"connection_count");if(value.gateway_publishers==1&&value.gateway_subscribers==1&&publisher.getNumSubscribers()==1&&subscriber.getNumPublishers()==1)break;step();}
        facts.matched=true;record("MATCHED_RECOVERY_PEER",mode," publishers=1 subscribers=1");
        phase="delivery";bool finished{};
        while(!finished)
        {
            step();for(unsigned stage{};stage!=3;++stage)if(facts.received[stage]&&!facts.sent[stage])
            {
                require(publisher.getNumSubscribers()==1&&subscriber.getNumPublishers()==1,"reply_match");std_msgs::String message;message.data.assign(recovery_protocol::inbound[stage]);publisher.publish(message);++facts.sent[stage];
                record("RECOVERY_NATIVE_DELIVERY",mode," stage=",recovery_protocol::stages[stage]," received=1 sent=1");
            }
            finished=mode=="survivor"?facts.sent[2]==1:mode=="before"?facts.sent[0]==1:facts.sent[2]==1;
        }
        if(mode=="before")
        {
            while(!gate('X'))step();require(facts.received[0]==1&&facts.sent[0]==1,"effect_count");
            publisher.shutdown();subscriber.shutdown();node.shutdown();ros::shutdown();
            record("RETIRED_RECOVERY_PEER",mode," before=1 during=0 after=0 excess=0 graceful=1");return 0;
        }
        while(!gate('Q'))step();phase="quiet";auto begin=clock::now();unsigned samples{};
        while(clock::now()-begin<std::chrono::seconds{2}){step();++samples;}
        require(samples>=30,"quiet_samples");
        unsigned expected=mode=="survivor"?3:1;require(facts.received[0]+facts.received[1]+facts.received[2]==expected&&facts.sent[0]+facts.sent[1]+facts.sent[2]==expected,"effect_count");
        auto quiet=std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now()-begin).count();publisher.shutdown();subscriber.shutdown();node.shutdown();ros::shutdown();
        record("PASS_RECOVERY_PEER",mode," before=",facts.received[0]," during=",facts.received[1]," after=",facts.received[2]," excess=0 quiet_samples=",samples," quiet_ns=",quiet);return 0;
    }
    catch(...){try{emit("FAIL_RECOVERY_PEER backend=1 phase=",phase," field=",field);}catch(...){}return 1;}
}
