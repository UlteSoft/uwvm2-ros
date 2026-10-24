// Independent native ROS2 input source and processed-Image witness.
#include <rcl/rcl.h>
#include <rcl/graph.h>
#include <rcl/arguments.h>
#include <rcl/error_handling.h>
#include <rmw/rmw.h>
#include <rmw/topic_endpoint_info_array.h>
#include <sensor_msgs/msg/image.h>
#include <rosidl_runtime_c/string_functions.h>
#include <rosidl_runtime_c/primitives_sequence_functions.h>
#include <algorithm>
#include <array>
#include <chrono>
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
void marker(char const* phase,std::string const& ns,Gid const& source,Gid const& reader,Gid const& writer,unsigned sent,unsigned warmup,unsigned delivered){
 std::printf("%s_IMAGE_PIPELINE namespace=%s source=%s input_reader=%s output_writer=%s sent=%u warmup=%u delivered=%u\n",phase,ns.c_str(),hex(source).c_str(),hex(reader).c_str(),hex(writer).c_str(),sent,warmup,delivered);std::fflush(stdout);
}
bool same(rosidl_runtime_c__String const& s,char const* e){auto n=std::strlen(e);return s.size==n&&s.data&&std::memcmp(s.data,e,n)==0;}
bool output_shape(sensor_msgs__msg__Image const& m,bool warmup){
 constexpr unsigned char pixels[]{76,149,28,255};
 return m.header.stamp.sec==(warmup?0:1)&&m.header.stamp.nanosec==(warmup?0u:2u)&&same(m.header.frame_id,warmup?"camera/warmup":"camera/optical")&&m.height==(warmup?1u:2u)&&m.width==(warmup?1u:2u)&&same(m.encoding,"mono8")&&!m.is_bigendian&&m.step==(warmup?1u:2u)&&m.data.size==(warmup?1u:4u)&&m.data.data&&(warmup?m.data.data[0]==0:std::memcmp(m.data.data,pixels,4)==0);
}
struct State {
 rcl_allocator_t allocator{rcl_get_default_allocator()};
 rcl_init_options_t init{rcl_get_zero_initialized_init_options()};
 rcl_context_t context{rcl_get_zero_initialized_context()};rcl_node_t node{rcl_get_zero_initialized_node()};
 rcl_publisher_t input{rcl_get_zero_initialized_publisher()};
 rcl_subscription_t output{rcl_get_zero_initialized_subscription()},forbidden{rcl_get_zero_initialized_subscription()};
 sensor_msgs__msg__Image received{},source{};
 bool init_ok{},context_ok{},node_ok{},input_ok{},output_ok{},forbidden_ok{},received_ok{},source_ok{};
 bool close()noexcept{
  bool ok=true;
  if(forbidden_ok){ok=(rcl_subscription_fini(&forbidden,&node)==RCL_RET_OK)&&ok;forbidden_ok=false;}
  if(output_ok){ok=(rcl_subscription_fini(&output,&node)==RCL_RET_OK)&&ok;output_ok=false;}
  if(input_ok){ok=(rcl_publisher_fini(&input,&node)==RCL_RET_OK)&&ok;input_ok=false;}
  if(source_ok){sensor_msgs__msg__Image__fini(&source);source_ok=false;}
  if(received_ok){sensor_msgs__msg__Image__fini(&received);received_ok=false;}
  if(node_ok){ok=(rcl_node_fini(&node)==RCL_RET_OK)&&ok;node_ok=false;}
  if(context_ok){ok=(rcl_shutdown(&context)==RCL_RET_OK)&&ok;ok=(rcl_context_fini(&context)==RCL_RET_OK)&&ok;context_ok=false;}
  if(init_ok){ok=(rcl_init_options_fini(&init)==RCL_RET_OK)&&ok;init_ok=false;}return ok;
 }
 ~State(){if(!close())std::fprintf(stderr,"FAIL_IMAGE_PIPELINE cleanup\n");}
 void open(std::string const& ns){
  require(rcl_init_options_init(&init,allocator)==RCL_RET_OK,"init options");init_ok=true;
  require(rcl_init(0,nullptr,&init,&context)==RCL_RET_OK,"context");context_ok=true;
  auto opts=rcl_node_get_default_options();opts.enable_rosout=false;opts.use_global_arguments=false;
  require(rcl_parse_arguments(0,nullptr,allocator,&opts.arguments)==RCL_RET_OK,"empty node arguments");
  auto rc=rcl_node_init(&node,"moonbit_image_peer",ns.c_str(),&context,&opts);node_ok=rc==RCL_RET_OK;std::string error=node_ok?"":rcl_get_error_string().str;
  auto freed=rcl_node_options_fini(&opts);if(!node_ok)throw std::runtime_error("node: "+error);require(freed==RCL_RET_OK,"node options cleanup");
  auto sub=rcl_subscription_get_default_options();sub.qos=rmw_qos_profile_default;sub.qos.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;sub.qos.depth=4;sub.qos.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;sub.qos.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;
  auto pub=rcl_publisher_get_default_options();pub.qos=sub.qos;auto const* type=ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs,msg,Image);
  require(rcl_publisher_init(&input,&node,type,(ns+"/input").c_str(),&pub)==RCL_RET_OK,"input publisher");input_ok=true;
  require(rcl_subscription_init(&output,&node,type,(ns+"/output").c_str(),&sub)==RCL_RET_OK,"output subscriber");output_ok=true;
  require(rcl_subscription_init(&forbidden,&node,type,(ns+"/forbidden").c_str(),&sub)==RCL_RET_OK,"forbidden observer");forbidden_ok=true;
  require(sensor_msgs__msg__Image__init(&received),"output storage");received_ok=true;
  require(sensor_msgs__msg__Image__init(&source),"input storage");source_ok=true;
  source.header.stamp.sec=1;source.header.stamp.nanosec=2;
  require(rosidl_runtime_c__String__assign(&source.header.frame_id,"camera/optical")&&rosidl_runtime_c__String__assign(&source.encoding,"rgb8"),"source strings");
  source.height=2;source.width=2;source.is_bigendian=0;source.step=8;
  rosidl_runtime_c__uint8__Sequence__fini(&source.data);require(rosidl_runtime_c__uint8__Sequence__init(&source.data,16),"source pixels");
  constexpr unsigned char pixels[]{255,0,0,0,255,0,170,187,0,0,255,255,255,255,204,221};std::copy(pixels,pixels+16,source.data.data);
 }
};
struct Endpoints {
 rmw_topic_endpoint_info_array_t value{rmw_get_zero_initialized_topic_endpoint_info_array()};rcutils_allocator_t allocator{rcutils_get_default_allocator()};bool closed{};
 Endpoints(rcl_node_t const* node,std::string const& topic,bool readers=false){auto rc=readers?rcl_get_subscriptions_info_by_topic(node,&allocator,topic.c_str(),false,&value):rcl_get_publishers_info_by_topic(node,&allocator,topic.c_str(),false,&value);require(rc==RCL_RET_OK,"graph query");}
 void close(){require(rmw_topic_endpoint_info_array_fini(&value,&allocator)==RMW_RET_OK,"graph cleanup");closed=true;}
 ~Endpoints(){if(!closed&&rmw_topic_endpoint_info_array_fini(&value,&allocator)!=RMW_RET_OK)std::fprintf(stderr,"FAIL_IMAGE_PIPELINE graph cleanup\n");}
};
bool endpoint(rmw_topic_endpoint_info_t const& e,std::string const& ns,char const* name){
 return e.node_name&&e.node_namespace&&e.topic_type&&std::string{e.node_name}==name&&std::string{e.node_namespace}==ns&&std::string{e.topic_type}=="sensor_msgs/msg/Image"&&e.qos_profile.reliability==RMW_QOS_POLICY_RELIABILITY_RELIABLE&&e.qos_profile.durability==RMW_QOS_POLICY_DURABILITY_VOLATILE;
}
std::size_t publishers(rcl_subscription_t const& s){std::size_t n{};require(rcl_subscription_get_publisher_count(&s,&n)==RCL_RET_OK,"publisher count");return n;}
std::size_t subscribers(rcl_publisher_t const& p){std::size_t n{};require(rcl_publisher_get_subscription_count(&p,&n)==RCL_RET_OK,"subscriber count");return n;}
}
int main(int argc,char** argv){
 try{
  require(argc==3,"usage: image-pipeline-peer namespace zero|publish-only|subscribe-only|full");std::string ns{argv[1]},mode{argv[2]};
  require(mode=="zero"||mode=="publish-only"||mode=="subscribe-only"||mode=="full","explicit mode");bool full=mode=="full",can_sub=full||mode=="subscribe-only";
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
    require(full&&info.publisher_gid.implementation_identifier&&std::string{info.publisher_gid.implementation_identifier}=="rmw_fastrtps_cpp","output RMW");auto actual=gid(info.publisher_gid.data);
    if(output_shape(s.received,true)){require(++warmup==1&&(!output_matched||actual==writer),"warmup identity/count");warmup_gid=actual;}
    else{require(output_shape(s.received,false)&&sent==1&&output_matched&&actual==writer&&++delivered==1,"complete processed Image identity/value/count");}
   }else require(rc==RCL_RET_SUBSCRIPTION_TAKE_FAILED,"output take");
   require(rcl_take(&s.forbidden,&s.received,&info,nullptr)==RCL_RET_SUBSCRIPTION_TAKE_FAILED,"forbidden data");
   Endpoints input_graph{&s.node,ns+"/input",true},source_graph{&s.node,ns+"/input"},output_graph{&s.node,ns+"/output"},forbidden_graph{&s.node,ns+"/forbidden"};
   auto in=subscribers(s.input),out=publishers(s.output);require(in<=1&&out<=1&&input_graph.value.size<=1&&output_graph.value.size<=1,"foreign endpoints");require(!publishers(s.forbidden)&&!forbidden_graph.value.size,"forbidden endpoint");
   require(source_graph.value.size<=1,"foreign input source");bool source_verified=source_graph.value.size==1&&endpoint(source_graph.value.info_array[0],ns,"moonbit_image_peer")&&gid(source_graph.value.info_array[0].endpoint_gid)==source;require(source_graph.value.size==0||source_verified,"source graph identity");
   if(!can_sub)require(in==0&&input_graph.value.size==0,"ungranted input endpoint");
   if(!full)require(out==0&&output_graph.value.size==0&&!sent&&!warmup&&!delivered,"negative output effect");
   if(can_sub&&!input_matched&&source_verified&&in==1&&input_graph.value.size==1&&endpoint(input_graph.value.info_array[0],ns,"moonbit_image_gateway")){
    reader=gid(input_graph.value.info_array[0].endpoint_gid);require(nonzero(reader),"input reader GID");input_matched=true;marker("INPUT_MATCHED",ns,source,reader,writer,sent,warmup,delivered);
   }
   if(full&&!output_matched&&out==1&&output_graph.value.size==1&&endpoint(output_graph.value.info_array[0],ns,"moonbit_image_gateway")){
    writer=gid(output_graph.value.info_array[0].endpoint_gid);require(input_matched&&nonzero(writer)&&(!warmup||writer==warmup_gid),"output writer GID");output_matched=true;marker("OUTPUT_MATCHED",ns,source,reader,writer,sent,warmup,delivered);
   }
   pollfd p{STDIN_FILENO,POLLIN,0};int ready=::poll(&p,1,0);require(ready>=0,"gate poll");
   if(ready&&p.revents){char c{};require(::read(0,&c,1)==1,"one native gate byte");
    if(c=='I'){require(full&&source_verified&&input_matched&&output_matched&&!sent&&!delivered,"one selected input send");require(rcl_publish(&s.input,&s.source,nullptr)==RCL_RET_OK,"actual native RGB send");sent=1;marker("INPUT_SENT",ns,source,reader,writer,sent,warmup,delivered);}
    else{require(c=='Q'&&!full&&!q&&(!can_sub||input_matched),"one absence gate");q=true;}
   }
   if(delivered&&!announced){announced=true;marker("DELIVERED",ns,source,reader,writer,sent,warmup,delivered);}
   bool retired=in==0&&out==0&&input_graph.value.size==0&&output_graph.value.size==0;
   if((full?delivered==1:q)&&retired){
    if(!quiet_started){quiet_started=true;quiet=Clock::now()+std::chrono::seconds(2);}
    if(Clock::now()>=quiet){input_graph.close();source_graph.close();output_graph.close();forbidden_graph.close();require(s.close(),"native explicit cleanup");marker(full?"PASS":"ABSENT",ns,source,reader,writer,sent,warmup,delivered);return 0;}
   }else require(!quiet_started,"endpoint reappeared during quiet interval");
   input_graph.close();source_graph.close();output_graph.close();forbidden_graph.close();std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  throw std::runtime_error("input/output/gate/retirement deadline");
 }catch(std::exception const& e){std::fprintf(stderr,"FAIL_IMAGE_PIPELINE %s\n",e.what());return 1;}
}
