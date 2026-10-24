// Independent native ROS1 source and processed-Image witness; never Guest code.
#include <ros/ros.h>
#include <ros/message_event.h>
#include <ros/single_subscriber_publisher.h>
#include <ros/advertise_options.h>
#include <tf2_msgs/TFMessage.h>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <thread>
#include <unistd.h>

namespace {
using Clock=std::chrono::steady_clock;
void require(bool ok,char const* why){if(!ok)throw std::runtime_error(why);}
void line(int fd,std::string s){s+='\n';char const* p=s.data();auto left=s.size();while(left){auto n=::write(fd,p,left);require(n>0,"marker write");p+=n;left-=n;}}
void marker(char const* phase,std::string const& ns,bool input_matched,bool output_matched,unsigned sent,unsigned warmup,unsigned delivered,unsigned connects,unsigned disconnects,std::uint64_t quiet_ns=0){
 auto gateway=ns+"/moonbit_transform_gateway";
 line(1,std::string{"TF_NATIVE backend=1 phase="}+phase+" namespace="+ns+" source="+ns+"/moonbit_transform_peer input_reader="+(input_matched?gateway:"none")+" output_writer="+(output_matched?gateway:"none")+" sent="+std::to_string(sent)+" warmup="+std::to_string(warmup)+" delivered="+std::to_string(delivered)+" connects="+std::to_string(connects)+" disconnects="+std::to_string(disconnects)+" quiet_ns="+std::to_string(quiet_ns));
}
bool output_shape(tf2_msgs::TFMessage const& batch,bool warmup){
 if(batch.transforms.size()!=1)return false;auto const& m=batch.transforms[0];
 return m.header.seq==0&&m.header.stamp.sec==(warmup?1u:2u)&&m.header.stamp.nsec==(warmup?1u:3u)&&m.header.frame_id=="map"&&m.child_frame_id=="processed_link"&&
 m.transform.translation.x==(warmup?0.:2.25)&&m.transform.translation.y==(warmup?0.:-2.5)&&m.transform.translation.z==(warmup?0.:3.)&&
 m.transform.rotation.x==0.&&m.transform.rotation.y==0.&&m.transform.rotation.z==0.&&m.transform.rotation.w==1.;
}
}
int main(int argc,char** argv){
 try{
  require(argc==4,"usage: transform-pipeline-peer-ros1 namespace master-port zero|lookup-only|broadcast-only|full");
  std::string ns{argv[1]},port{argv[2]},mode{argv[3]};
  require(ns.size()>1&&ns.size()<128&&ns.front()=='/'&&ns.find_first_not_of("/abcdefghijklmnopqrstuvwxyz0123456789_")==std::string::npos,"namespace");
  require(!port.empty()&&port.find_first_not_of("0123456789")==std::string::npos&&std::stoul(port)>=1024&&std::stoul(port)<=65535,"master port");
  require(mode=="zero"||mode=="lookup-only"||mode=="broadcast-only"||mode=="full","mode");
  bool can_lookup=mode=="full"||mode=="lookup-only",can_broadcast=mode=="full"||mode=="broadcast-only";
  auto* master=std::getenv("ROS_MASTER_URI");auto* address=std::getenv("ROS_IP");
  require(master&&std::string{master}=="http://127.0.0.1:"+port&&address&&std::string{address}=="127.0.0.1","explicit loopback Master");
  require(!std::getenv("ROS_HOSTNAME")&&!std::getenv("ROS_NAMESPACE")&&!std::getenv("LD_PRELOAD"),"unexpected environment");
  ros::M_string remaps;remaps["__ns"]=ns;ros::init(remaps,"moonbit_transform_peer",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);ros::NodeHandle node{ns};
  auto gateway=ns+"/moonbit_transform_gateway";require(ros::this_node::getName()==ns+"/moonbit_transform_peer","source identity");
  require(std::string{ros::message_traits::DataType<tf2_msgs::TFMessage>::value()}=="tf2_msgs/TFMessage"&&std::string{ros::message_traits::MD5Sum<tf2_msgs::TFMessage>::value()}=="94810edda583a504dfda3829e70d7eec","native TFMessage type");
  unsigned sent{},warmup{},delivered{},connects{},disconnects{};bool bad{},input_matched{},output_matched{},announced{},quiet_started{},q{};
  ros::AdvertiseOptions options;options.init<tf2_msgs::TFMessage>("/tf",4,
   [&](ros::SingleSubscriberPublisher const& peer){if(!can_lookup||peer.getSubscriberName()!=gateway||++connects!=1)bad=true;},
   [&](ros::SingleSubscriberPublisher const& peer){if(peer.getSubscriberName()!=gateway||++disconnects!=1)bad=true;});options.latch=false;
  auto input=node.advertise(options);
  ros::SubscribeOptions output_options;output_options.initByFullCallbackType<ros::MessageEvent<tf2_msgs::TFMessage const> const&>("/tf_static",4,[&](ros::MessageEvent<tf2_msgs::TFMessage const> const& event){
   auto value=event.getMessage();auto header=event.getConnectionHeaderPtr();if(!can_broadcast||!value||!header||event.getPublisherName()!=gateway){bad=true;return;}
   auto field=[&](char const* key,std::string const& expected){auto i=header->find(key);return i!=header->end()&&i->second==expected;};
   if(!field("callerid",gateway)||!field("type","tf2_msgs/TFMessage")||!field("md5sum","94810edda583a504dfda3829e70d7eec")||!field("latching","0")){bad=true;return;}
   if(output_shape(*value,true)){if(delivered||++warmup!=1)bad=true;}
   else if(!output_shape(*value,false)||(can_lookup&&sent!=1)||!output_matched||++delivered!=1)bad=true;
  });output_options.transport_hints=ros::TransportHints{}.tcpNoDelay().reliable();auto output=node.subscribe(output_options);
  auto forbidden=node.subscribe<tf2_msgs::TFMessage>(ns+"/forbidden",4,[&](tf2_msgs::TFMessage::ConstPtr const&){bad=true;});
  tf2_msgs::TFMessage source;source.transforms.resize(2);
  for(unsigned n{};n!=2;++n){auto& m=source.transforms[n];m.header.seq=0;m.header.stamp.sec=2;m.header.stamp.nsec=3;m.header.frame_id="map";m.child_frame_id=n?"forbidden_link":"base_link";m.transform.translation.x=n?7.:1.25;m.transform.translation.y=-2.5;m.transform.translation.z=3.;m.transform.rotation.w=1.;}
  auto emit=[&](char const* phase,std::uint64_t quiet_ns=0){marker(phase,ns,input_matched,output_matched,sent,warmup,delivered,connects,disconnects,quiet_ns);};
  emit("READY");auto end=Clock::now()+std::chrono::seconds(40),quiet=end;
  while(Clock::now()<end){
   ros::spinOnce();require(ros::ok()&&!bad,"unexpected TF source/header/value/duplicate/connection");
   auto in=input.getNumSubscribers(),out=output.getNumPublishers();require(in<=1&&out<=1&&!forbidden.getNumPublishers(),"foreign endpoint");
   if(!can_lookup)require(in==0&&connects==0&&disconnects==0&&!sent,"ungranted input endpoint/effect");
   if(!can_broadcast)require(out==0&&!warmup&&!delivered,"ungranted output endpoint/effect");
   if(can_lookup&&!input_matched&&in==1&&connects==1&&!disconnects){input_matched=true;emit("INPUT_MATCHED");}
   if(can_broadcast&&!output_matched&&out==1){output_matched=true;emit("OUTPUT_MATCHED");}
   pollfd p{0,POLLIN,0};auto ready=::poll(&p,1,0);require(ready>=0,"native gate poll");
   if(ready&&p.revents){char c{};require(::read(0,&c,1)==1,"one native gate byte");
    if(c=='I'){require(can_lookup&&input_matched&&(!can_broadcast||output_matched)&&in==1&&!sent&&!delivered,"one selected native input send");input.publish(source);sent=1;emit("INPUT_SENT");}
    else{require(c=='Q'&&!q&&sent==unsigned(can_lookup)&&delivered==unsigned(can_broadcast),"one retirement gate");q=true;}
   }
   if(delivered&&!announced){announced=true;emit("DELIVERED");}
   if(q&&in==0&&out==0&&disconnects==connects){
    if(!quiet_started){quiet_started=true;quiet=Clock::now();}
    if(Clock::now()-quiet>=std::chrono::seconds(2)){
     auto elapsed=std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now()-quiet).count();
     input.shutdown();output.shutdown();forbidden.shutdown();node.shutdown();ros::shutdown();emit("PASS",elapsed);return 0;
    }
   }else require(!quiet_started,"endpoint reappeared during quiet interval");
   std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  throw std::runtime_error("input/output/gate/retirement deadline");
 }catch(std::exception const& e){line(2,std::string{"FAIL_ROS1_TRANSFORM_PIPELINE "}+e.what());ros::shutdown();return 1;}
}
