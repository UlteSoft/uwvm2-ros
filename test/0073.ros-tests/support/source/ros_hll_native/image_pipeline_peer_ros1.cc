// Independent native ROS1 source and processed-Image witness; never Guest code.
#include <ros/ros.h>
#include <ros/message_event.h>
#include <ros/single_subscriber_publisher.h>
#include <ros/advertise_options.h>
#include <sensor_msgs/Image.h>
#include <chrono>
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
void marker(char const* phase,std::string const& ns,bool matched_input,bool matched_output,unsigned in,unsigned out,unsigned connects,unsigned disconnects,unsigned sent,unsigned warmup,unsigned delivered){
 auto gateway=ns+"/moonbit_image_gateway";
 line(1,std::string{phase}+"_ROS1_IMAGE_PIPELINE namespace="+ns+" source="+ns+"/moonbit_image_peer input_reader="+(matched_input?gateway:"none")+" output_writer="+(matched_output?gateway:"none")+" transport=tcpros type=sensor_msgs/Image md5=060021388200f6f0f447d0fcd9c64743 latch=0 input_subscribers="+std::to_string(in)+" output_publishers="+std::to_string(out)+" connects="+std::to_string(connects)+" disconnects="+std::to_string(disconnects)+" sent="+std::to_string(sent)+" warmup="+std::to_string(warmup)+" delivered="+std::to_string(delivered));
}
bool shape(sensor_msgs::Image const& m,bool warmup){
 return m.header.seq==(warmup?0u:1u)&&m.header.stamp.sec==(warmup?0u:1u)&&m.header.stamp.nsec==(warmup?0u:2u)&&m.header.frame_id==(warmup?"camera/warmup":"camera/optical")&&m.height==(warmup?1u:2u)&&m.width==(warmup?1u:2u)&&m.encoding=="mono8"&&!m.is_bigendian&&m.step==(warmup?1u:2u)&&m.data==(warmup?std::vector<std::uint8_t>{0}:std::vector<std::uint8_t>{76,149,28,255});
}
}
int main(int argc,char** argv){
 try{
  require(argc==4,"usage: image-pipeline-peer-ros1 namespace master-port zero|publish-only|subscribe-only|full");std::string ns{argv[1]},port{argv[2]},mode{argv[3]};
  require(ns.size()>1&&ns.size()<128&&ns.front()=='/'&&ns.find_first_not_of("/abcdefghijklmnopqrstuvwxyz0123456789_")==std::string::npos,"namespace");
  require(!port.empty()&&port.find_first_not_of("0123456789")==std::string::npos&&std::stoul(port)>=1024&&std::stoul(port)<=65535,"master port");
  require(mode=="zero"||mode=="publish-only"||mode=="subscribe-only"||mode=="full","mode");bool full=mode=="full",can_sub=full||mode=="subscribe-only";
  auto* master=std::getenv("ROS_MASTER_URI");auto* address=std::getenv("ROS_IP");require(master&&std::string{master}=="http://127.0.0.1:"+port&&address&&std::string{address}=="127.0.0.1","explicit loopback Master");
  require(!std::getenv("ROS_HOSTNAME")&&!std::getenv("ROS_NAMESPACE")&&!std::getenv("LD_PRELOAD"),"unexpected environment");
  ros::M_string remaps;remaps["__ns"]=ns;ros::init(remaps,"moonbit_image_peer",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);ros::NodeHandle node{ns};
  auto gateway=ns+"/moonbit_image_gateway";require(ros::this_node::getName()==ns+"/moonbit_image_peer","source identity");
  require(std::string{ros::message_traits::DataType<sensor_msgs::Image>::value()}=="sensor_msgs/Image"&&std::string{ros::message_traits::MD5Sum<sensor_msgs::Image>::value()}=="060021388200f6f0f447d0fcd9c64743","native Image identity");
  unsigned connects{},disconnects{},sent{},warmup{},delivered{};bool bad{},input_matched{},output_matched{},announced{},q{},quiet_started{};
  // Default global callback queue, serviced only by this thread's spinOnce.
  ros::AdvertiseOptions input_options;input_options.init<sensor_msgs::Image>(ns+"/input",4,
   [&](ros::SingleSubscriberPublisher const& peer){if(!can_sub||peer.getSubscriberName()!=gateway||++connects!=1)bad=true;},
   [&](ros::SingleSubscriberPublisher const& peer){if(peer.getSubscriberName()!=gateway||++disconnects!=1)bad=true;});input_options.latch=false;
  auto input=node.advertise(input_options);
  ros::SubscribeOptions output_options;output_options.initByFullCallbackType<ros::MessageEvent<sensor_msgs::Image const> const&>(ns+"/output",4,[&](ros::MessageEvent<sensor_msgs::Image const> const& event){
   auto value=event.getMessage();auto header=event.getConnectionHeaderPtr();if(!full||!value||!header||event.getPublisherName()!=gateway){bad=true;return;}
   auto field=[&](char const* key,std::string const& expected){auto i=header->find(key);return i!=header->end()&&i->second==expected;};
   if(!field("callerid",gateway)||!field("type","sensor_msgs/Image")||!field("md5sum","060021388200f6f0f447d0fcd9c64743")||!field("latching","0")){bad=true;return;}
   if(shape(*value,true)){if(delivered||++warmup!=1)bad=true;}
   else if(!shape(*value,false)||sent!=1||!output_matched||++delivered!=1)bad=true;
  });output_options.transport_hints=ros::TransportHints{}.tcpNoDelay().reliable();auto output=node.subscribe(output_options);
  auto forbidden=node.subscribe<sensor_msgs::Image>(ns+"/forbidden",4,[&](sensor_msgs::Image::ConstPtr const&){bad=true;});
  sensor_msgs::Image source;source.header.seq=0;source.header.stamp.sec=1;source.header.stamp.nsec=2;source.header.frame_id="camera/optical";source.height=2;source.width=2;source.encoding="rgb8";source.is_bigendian=0;source.step=8;source.data={255,0,0,0,255,0,170,187,0,0,255,255,255,255,204,221};
  marker("READY",ns,false,false,0,0,0,0,0,0,0);auto end=Clock::now()+std::chrono::seconds(40),quiet=end;
  while(Clock::now()<end){
   ros::spinOnce();require(ros::ok()&&!bad,"unexpected source/header/pixels/duplicate/connection");auto in=input.getNumSubscribers(),out=output.getNumPublishers();require(in<=1&&out<=1&&!forbidden.getNumPublishers(),"foreign endpoint");
   if(!can_sub)require(in==0&&connects==0&&disconnects==0,"ungranted input reader");
   if(!full)require(out==0&&!sent&&!warmup&&!delivered,"negative output effect");
   if(can_sub&&!input_matched&&in==1&&connects==1&&!disconnects){input_matched=true;marker("INPUT_MATCHED",ns,true,false,in,out,connects,disconnects,sent,warmup,delivered);}
   if(full&&!output_matched&&out==1){require(input_matched,"input matched before output");output_matched=true;marker("OUTPUT_MATCHED",ns,true,true,in,out,connects,disconnects,sent,warmup,delivered);}
   pollfd p{0,POLLIN,0};auto ready=::poll(&p,1,0);require(ready>=0,"native gate poll");
   if(ready&&p.revents){char c{};require(::read(0,&c,1)==1,"one native gate byte");
    if(c=='I'){require(full&&input_matched&&output_matched&&in==1&&out==1&&!sent&&!delivered,"one selected native input send");input.publish(source);sent=1;marker("INPUT_SENT",ns,true,true,in,out,connects,disconnects,sent,warmup,delivered);}
    else{require(c=='Q'&&!full&&!q&&(!can_sub||input_matched),"one absence gate");q=true;}
   }
   if(delivered&&!announced){require(in==1&&out==1&&connects==1&&!disconnects,"endpoints alive until delivery credit");announced=true;marker("DELIVERED",ns,true,true,in,out,connects,disconnects,sent,warmup,delivered);}
   if((full?delivered==1:q)&&in==0&&out==0&&disconnects==connects){
    if(!quiet_started){quiet_started=true;quiet=Clock::now();}
    if(Clock::now()-quiet>=std::chrono::seconds(2)){input.shutdown();output.shutdown();forbidden.shutdown();node.shutdown();ros::shutdown();marker(full?"PASS":"ABSENT",ns,input_matched,output_matched,0,0,connects,disconnects,sent,warmup,delivered);return 0;}
   }else require(!quiet_started,"endpoint reappeared during quiet interval");
   std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  throw std::runtime_error("input/output/gate/retirement deadline");
 }catch(std::exception const& e){line(2,std::string{"FAIL_ROS1_IMAGE_PIPELINE "}+e.what());ros::shutdown();return 1;}
}
