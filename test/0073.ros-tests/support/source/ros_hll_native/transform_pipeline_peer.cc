// Independent native ROS2 input source and processed-Image witness.
#include <rcl/rcl.h>
#include <rcl/graph.h>
#include <rcl/arguments.h>
#include <rcl/error_handling.h>
#include <rmw/rmw.h>
#include <rmw/topic_endpoint_info_array.h>
#include <tf2_msgs/msg/tf_message.h>
#include <rosidl_runtime_c/string_functions.h>
#include <geometry_msgs/msg/detail/transform_stamped__functions.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <thread>
#include <unistd.h>

namespace {
using Clock=std::chrono::steady_clock;
using Gid=std::array<unsigned char,RMW_GID_STORAGE_SIZE>;
void require(bool ok,char const* why){if(!ok)throw std::runtime_error(std::string{why}+": "+rcl_get_error_string().str);}
bool nonzero(Gid const& g){return std::any_of(g.begin(),g.end(),[](auto b){return b!=0;});}
Gid gid(unsigned char const* p){Gid g{};std::copy(p,p+RMW_GID_STORAGE_SIZE,g.begin());return g;}
std::string hex(Gid const& g){std::string s;for(auto b:g){s+="0123456789abcdef"[b>>4];s+="0123456789abcdef"[b&15];}return s;}
void marker(char const* phase,std::string const& ns,Gid const& source,Gid const& reader,Gid const& writer,unsigned sent,unsigned warmup,unsigned delivered,std::uint64_t quiet_ns=0){
 std::printf("TF_NATIVE backend=2 phase=%s namespace=%s source=%s input_reader=%s output_writer=%s sent=%u warmup=%u delivered=%u quiet_ns=%llu\n",phase,ns.c_str(),hex(source).c_str(),hex(reader).c_str(),hex(writer).c_str(),sent,warmup,delivered,static_cast<unsigned long long>(quiet_ns));std::fflush(stdout);
}
bool same(rosidl_runtime_c__String const& s,char const* e){auto n=std::strlen(e);return s.size==n&&s.data&&std::memcmp(s.data,e,n)==0;}
bool output_shape(tf2_msgs__msg__TFMessage const& batch,bool warmup){
 if(batch.transforms.size!=1||!batch.transforms.data)return false;
 auto const& m=batch.transforms.data[0];
 return m.header.stamp.sec==(warmup?1:2)&&m.header.stamp.nanosec==(warmup?1u:3u)&&same(m.header.frame_id,"map")&&same(m.child_frame_id,"processed_link")&&
  m.transform.translation.x==(warmup?0.:2.25)&&m.transform.translation.y==(warmup?0.:-2.5)&&m.transform.translation.z==(warmup?0.:3.)&&
  m.transform.rotation.x==0.&&m.transform.rotation.y==0.&&m.transform.rotation.z==0.&&m.transform.rotation.w==1.;
}
struct State {
 rcl_allocator_t allocator{rcl_get_default_allocator()};
 rcl_init_options_t init{rcl_get_zero_initialized_init_options()};
 rcl_context_t context{rcl_get_zero_initialized_context()};rcl_node_t node{rcl_get_zero_initialized_node()};
 rcl_publisher_t input{rcl_get_zero_initialized_publisher()};
 rcl_subscription_t output{rcl_get_zero_initialized_subscription()},forbidden{rcl_get_zero_initialized_subscription()};
 tf2_msgs__msg__TFMessage received{},source{};
 bool init_ok{},context_ok{},node_ok{},input_ok{},output_ok{},forbidden_ok{},received_ok{},source_ok{};
 bool close()noexcept{
  bool ok=true;
  if(forbidden_ok){ok=(rcl_subscription_fini(&forbidden,&node)==RCL_RET_OK)&&ok;forbidden_ok=false;}
  if(output_ok){ok=(rcl_subscription_fini(&output,&node)==RCL_RET_OK)&&ok;output_ok=false;}
  if(input_ok){ok=(rcl_publisher_fini(&input,&node)==RCL_RET_OK)&&ok;input_ok=false;}
  if(source_ok){tf2_msgs__msg__TFMessage__fini(&source);source_ok=false;}
  if(received_ok){tf2_msgs__msg__TFMessage__fini(&received);received_ok=false;}
  if(node_ok){ok=(rcl_node_fini(&node)==RCL_RET_OK)&&ok;node_ok=false;}
  if(context_ok){ok=(rcl_shutdown(&context)==RCL_RET_OK)&&ok;ok=(rcl_context_fini(&context)==RCL_RET_OK)&&ok;context_ok=false;}
  if(init_ok){ok=(rcl_init_options_fini(&init)==RCL_RET_OK)&&ok;init_ok=false;}return ok;
 }
 ~State(){if(!close())std::fprintf(stderr,"FAIL_TRANSFORM_PIPELINE cleanup\n");}
 void open(std::string const& ns){
  require(rcl_init_options_init(&init,allocator)==RCL_RET_OK,"init options");init_ok=true;
  require(rcl_init(0,nullptr,&init,&context)==RCL_RET_OK,"context");context_ok=true;
  auto opts=rcl_node_get_default_options();opts.enable_rosout=false;opts.use_global_arguments=false;
  require(rcl_parse_arguments(0,nullptr,allocator,&opts.arguments)==RCL_RET_OK,"empty node arguments");
  auto rc=rcl_node_init(&node,"moonbit_transform_peer",ns.c_str(),&context,&opts);node_ok=rc==RCL_RET_OK;std::string error=node_ok?"":rcl_get_error_string().str;
  auto freed=rcl_node_options_fini(&opts);if(!node_ok)throw std::runtime_error("node: "+error);require(freed==RCL_RET_OK,"node options cleanup");
  auto sub=rcl_subscription_get_default_options();sub.qos=rmw_qos_profile_default;sub.qos.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;sub.qos.depth=4;sub.qos.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;sub.qos.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;
  auto pub=rcl_publisher_get_default_options();pub.qos=sub.qos;auto const* type=ROSIDL_GET_MSG_TYPE_SUPPORT(tf2_msgs,msg,TFMessage);
  require(rcl_publisher_init(&input,&node,type,"/tf",&pub)==RCL_RET_OK,"input publisher");input_ok=true;
  require(rcl_subscription_init(&output,&node,type,"/tf_static",&sub)==RCL_RET_OK,"output subscriber");output_ok=true;
  require(rcl_subscription_init(&forbidden,&node,type,(ns+"/forbidden").c_str(),&sub)==RCL_RET_OK,"forbidden observer");forbidden_ok=true;
  require(tf2_msgs__msg__TFMessage__init(&received),"output storage");received_ok=true;
  require(tf2_msgs__msg__TFMessage__init(&source),"input storage");source_ok=true;
  geometry_msgs__msg__TransformStamped__Sequence__fini(&source.transforms);
  require(geometry_msgs__msg__TransformStamped__Sequence__init(&source.transforms,2),"two input transforms");
  for(unsigned n{};n!=2;++n){
   auto& m=source.transforms.data[n];m.header.stamp.sec=2;m.header.stamp.nanosec=3;
   require(rosidl_runtime_c__String__assign(&m.header.frame_id,"map")&&rosidl_runtime_c__String__assign(&m.child_frame_id,n?"forbidden_link":"base_link"),"input frame storage");
   m.transform.translation.x=n?7.:1.25;m.transform.translation.y=-2.5;m.transform.translation.z=3.;m.transform.rotation.w=1.;
  }
 }
};
struct Endpoints {
 rmw_topic_endpoint_info_array_t value{rmw_get_zero_initialized_topic_endpoint_info_array()};rcutils_allocator_t allocator{rcutils_get_default_allocator()};bool closed{};
 Endpoints(rcl_node_t const* node,std::string const& topic,bool readers=false){auto rc=readers?rcl_get_subscriptions_info_by_topic(node,&allocator,topic.c_str(),false,&value):rcl_get_publishers_info_by_topic(node,&allocator,topic.c_str(),false,&value);require(rc==RCL_RET_OK,"graph query");}
 void close(){require(rmw_topic_endpoint_info_array_fini(&value,&allocator)==RMW_RET_OK,"graph cleanup");closed=true;}
 ~Endpoints(){if(!closed&&rmw_topic_endpoint_info_array_fini(&value,&allocator)!=RMW_RET_OK)std::fprintf(stderr,"FAIL_TRANSFORM_PIPELINE graph cleanup\n");}
};
bool endpoint(rmw_topic_endpoint_info_t const& e,std::string const& ns,char const* name){
 return e.node_name&&e.node_namespace&&e.topic_type&&std::string{e.node_name}==name&&std::string{e.node_namespace}==ns&&std::string{e.topic_type}=="tf2_msgs/msg/TFMessage"&&e.qos_profile.reliability==RMW_QOS_POLICY_RELIABILITY_RELIABLE&&e.qos_profile.durability==RMW_QOS_POLICY_DURABILITY_VOLATILE;
}
std::size_t publishers(rcl_subscription_t const& s){std::size_t n{};require(rcl_subscription_get_publisher_count(&s,&n)==RCL_RET_OK,"publisher count");return n;}
std::size_t subscribers(rcl_publisher_t const& p){std::size_t n{};require(rcl_publisher_get_subscription_count(&p,&n)==RCL_RET_OK,"subscriber count");return n;}
}
int main(int argc,char** argv){
 try{
  require(argc==3,"usage: transform-pipeline-peer namespace zero|lookup-only|broadcast-only|full");
  std::string ns{argv[1]},mode{argv[2]};
  require(mode=="zero"||mode=="lookup-only"||mode=="broadcast-only"||mode=="full","explicit mode");
  bool can_lookup=mode=="full"||mode=="lookup-only",can_broadcast=mode=="full"||mode=="broadcast-only";
  require(ns.size()>1&&ns.size()<128&&ns.front()=='/'&&ns.find_first_not_of("/abcdefghijklmnopqrstuvwxyz0123456789_")==std::string::npos,"namespace");
  State s;s.open(ns);require(std::string{rmw_get_implementation_identifier()}=="rmw_fastrtps_cpp","selected RMW");
  rmw_gid_t source_gid{};require(rmw_get_gid_for_publisher(rcl_publisher_get_rmw_handle(&s.input),&source_gid)==RMW_RET_OK,"source GID");
  require(source_gid.implementation_identifier&&std::string{source_gid.implementation_identifier}=="rmw_fastrtps_cpp","source RMW identity");
  Gid source=gid(source_gid.data),reader{},writer{},warmup_gid{};require(nonzero(source),"nonzero source GID");
  unsigned sent{},warmup{},delivered{};bool input_matched{},output_matched{},announced{},quiet_started{},q{};
  marker("READY",ns,source,reader,writer,0,0,0);auto end=Clock::now()+std::chrono::seconds(40),quiet=end;
  while(Clock::now()<end){
   rmw_message_info_t info{};auto rc=rcl_take(&s.output,&s.received,&info,nullptr);
   if(rc==RCL_RET_OK){
    require(can_broadcast&&info.publisher_gid.implementation_identifier&&std::string{info.publisher_gid.implementation_identifier}=="rmw_fastrtps_cpp","output RMW");
    auto actual=gid(info.publisher_gid.data);
    if(output_shape(s.received,true)){require(!delivered&&++warmup==1&&(!output_matched||actual==writer),"warmup identity/count");warmup_gid=actual;}
    else require(output_shape(s.received,false)&&(!can_lookup||sent==1)&&output_matched&&actual==writer&&++delivered==1,"complete Transform identity/value/count");
   }else require(rc==RCL_RET_SUBSCRIPTION_TAKE_FAILED,"output take");
   require(rcl_take(&s.forbidden,&s.received,&info,nullptr)==RCL_RET_SUBSCRIPTION_TAKE_FAILED,"forbidden data");
   Endpoints input_graph{&s.node,"/tf",true},source_graph{&s.node,"/tf"},output_graph{&s.node,"/tf_static"},forbidden_graph{&s.node,ns+"/forbidden"};
   auto in=subscribers(s.input),out=publishers(s.output);require(in<=1&&out<=1&&input_graph.value.size<=1&&output_graph.value.size<=1,"foreign endpoints");
   require(!publishers(s.forbidden)&&!forbidden_graph.value.size,"forbidden endpoint");require(source_graph.value.size<=1,"foreign input source");
   bool source_verified=source_graph.value.size==1&&endpoint(source_graph.value.info_array[0],ns,"moonbit_transform_peer")&&gid(source_graph.value.info_array[0].endpoint_gid)==source;
   require(source_graph.value.size==0||source_verified,"source graph identity");
   if(!can_lookup)require(in==0&&input_graph.value.size==0&&!sent,"ungranted input endpoint/effect");
   if(!can_broadcast)require(out==0&&output_graph.value.size==0&&!warmup&&!delivered,"ungranted output endpoint/effect");
   if(can_lookup&&!input_matched&&source_verified&&in==1&&input_graph.value.size==1&&endpoint(input_graph.value.info_array[0],ns,"moonbit_transform_gateway")){
    reader=gid(input_graph.value.info_array[0].endpoint_gid);require(nonzero(reader),"input reader GID");input_matched=true;marker("INPUT_MATCHED",ns,source,reader,writer,sent,warmup,delivered);
   }
   if(can_broadcast&&!output_matched&&out==1&&output_graph.value.size==1&&endpoint(output_graph.value.info_array[0],ns,"moonbit_transform_gateway")){
    writer=gid(output_graph.value.info_array[0].endpoint_gid);require(nonzero(writer)&&(!warmup||writer==warmup_gid),"output writer GID");output_matched=true;marker("OUTPUT_MATCHED",ns,source,reader,writer,sent,warmup,delivered);
   }
   pollfd p{0,POLLIN,0};int ready=::poll(&p,1,0);require(ready>=0,"gate poll");
   if(ready&&p.revents){char c{};require(::read(0,&c,1)==1,"one native gate byte");
    if(c=='I'){require(can_lookup&&source_verified&&input_matched&&(!can_broadcast||output_matched)&&!sent&&!delivered,"one selected input send");require(rcl_publish(&s.input,&s.source,nullptr)==RCL_RET_OK,"native two-frame TF send");sent=1;marker("INPUT_SENT",ns,source,reader,writer,sent,warmup,delivered);}
    else{require(c=='Q'&&!q&&sent==unsigned(can_lookup)&&delivered==unsigned(can_broadcast),"one retirement gate");q=true;}
   }
   if(delivered&&!announced){announced=true;marker("DELIVERED",ns,source,reader,writer,sent,warmup,delivered);}
   bool retired=in==0&&out==0&&input_graph.value.size==0&&output_graph.value.size==0;
   if(q&&retired){
    if(!quiet_started){quiet_started=true;quiet=Clock::now();}
    if(Clock::now()-quiet>=std::chrono::seconds(2)){
     auto elapsed=std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now()-quiet).count();
     input_graph.close();source_graph.close();output_graph.close();forbidden_graph.close();require(s.close(),"native explicit cleanup");marker("PASS",ns,source,reader,writer,sent,warmup,delivered,elapsed);return 0;
    }
   }else require(!quiet_started,"endpoint reappeared during quiet interval");
   input_graph.close();source_graph.close();output_graph.close();forbidden_graph.close();std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  throw std::runtime_error("input/output/gate/retirement deadline");
 }catch(std::exception const& e){std::fprintf(stderr,"FAIL_TRANSFORM_PIPELINE %s\\n",e.what());return 1;}
}
