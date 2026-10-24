// Independent native ROS2 rcl witness. No Guest code or Host business import.
#include <rcl/rcl.h>
#include <rcl/graph.h>
#include <rcl/error_handling.h>
#include <rcl/arguments.h>
#include <rmw/rmw.h>
#include <rmw/topic_endpoint_info_array.h>
#include <sensor_msgs/msg/image.h>
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
std::string hex(Gid const& value){std::string s;for(auto b:value){s+="0123456789abcdef"[b>>4];s+="0123456789abcdef"[b&15];}return s;}
void marker(char const* phase,std::string const& ns,Gid const& gid,unsigned warmup,unsigned delivered){
  std::printf("%s_NATIVE_IMAGE namespace=%s gateway=moonbit_image_gateway gid=%s warmup=%u delivered=%u\n",phase,ns.c_str(),hex(gid).c_str(),warmup,delivered);std::fflush(stdout);
}
bool same(rosidl_runtime_c__String const& value,char const* expected){auto n=std::strlen(expected);return value.size==n&&value.data&&std::memcmp(value.data,expected,n)==0;}
bool shape(sensor_msgs__msg__Image const& m,bool warmup){
  constexpr unsigned char pixels[]{76,149,28,255};
  return m.header.stamp.sec==(warmup?0:1)&&m.header.stamp.nanosec==(warmup?0u:2u)&&
    same(m.header.frame_id,warmup?"camera/warmup":"camera/optical")&&m.height==(warmup?1u:2u)&&m.width==(warmup?1u:2u)&&
    same(m.encoding,"mono8")&&m.is_bigendian==0&&m.step==(warmup?1u:2u)&&m.data.size==(warmup?1u:4u)&&m.data.data&&
    (warmup?m.data.data[0]==0:std::memcmp(m.data.data,pixels,4)==0);
}
struct State {
  rcl_allocator_t allocator{rcl_get_default_allocator()};
  rcl_init_options_t init{rcl_get_zero_initialized_init_options()};
  rcl_context_t context{rcl_get_zero_initialized_context()};
  rcl_node_t node{rcl_get_zero_initialized_node()};
  rcl_subscription_t sub{rcl_get_zero_initialized_subscription()},forbidden{rcl_get_zero_initialized_subscription()};
  sensor_msgs__msg__Image message{};
  bool init_ok{},context_ok{},node_ok{},sub_ok{},forbidden_ok{},message_ok{};
  bool close() noexcept {
    bool ok=true;
    if(forbidden_ok){ok=(rcl_subscription_fini(&forbidden,&node)==RCL_RET_OK)&&ok;forbidden_ok=false;}
    if(sub_ok){ok=(rcl_subscription_fini(&sub,&node)==RCL_RET_OK)&&ok;sub_ok=false;}
    if(message_ok){sensor_msgs__msg__Image__fini(&message);message_ok=false;}
    if(node_ok){ok=(rcl_node_fini(&node)==RCL_RET_OK)&&ok;node_ok=false;}
    if(context_ok){ok=(rcl_shutdown(&context)==RCL_RET_OK)&&ok;ok=(rcl_context_fini(&context)==RCL_RET_OK)&&ok;context_ok=false;}
    if(init_ok){ok=(rcl_init_options_fini(&init)==RCL_RET_OK)&&ok;init_ok=false;}
    return ok;
  }
  ~State(){if(!close())std::fprintf(stderr,"FAIL_NATIVE_IMAGE native cleanup\n");}
  void open(std::string const& ns){
    require(rcl_init_options_init(&init,allocator)==RCL_RET_OK,"rcl init options");init_ok=true;
    require(rcl_init(0,nullptr,&init,&context)==RCL_RET_OK,"rcl context");context_ok=true;
    auto opts=rcl_node_get_default_options();opts.enable_rosout=false;opts.use_global_arguments=false;
    require(rcl_parse_arguments(0,nullptr,allocator,&opts.arguments)==RCL_RET_OK,"explicit empty local arguments");
    auto node_result=rcl_node_init(&node,"moonbit_image_peer",ns.c_str(),&context,&opts);node_ok=node_result==RCL_RET_OK;
    std::string node_error=node_ok?"":rcl_get_error_string().str;
    auto options_result=rcl_node_options_fini(&opts);
    if(!node_ok)throw std::runtime_error("rcl node: "+node_error);
    require(options_result==RCL_RET_OK,"node option cleanup");
    auto options=rcl_subscription_get_default_options();options.qos=rmw_qos_profile_default;options.qos.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;options.qos.depth=4;options.qos.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;options.qos.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;
    auto const* type=ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs,msg,Image);
    require(rcl_subscription_init(&sub,&node,type,(ns+"/image").c_str(),&options)==RCL_RET_OK,"rcl Image subscription");sub_ok=true;
    require(rcl_subscription_init(&forbidden,&node,type,(ns+"/forbidden").c_str(),&options)==RCL_RET_OK,"rcl forbidden observer");forbidden_ok=true;
    require(sensor_msgs__msg__Image__init(&message),"native Image storage");message_ok=true;
  }
};
struct Endpoints {
  rmw_topic_endpoint_info_array_t value{rmw_get_zero_initialized_topic_endpoint_info_array()};
  rcutils_allocator_t allocator{rcutils_get_default_allocator()};
  bool closed{};
  Endpoints(rcl_node_t const* node,std::string const& topic){require(rcl_get_publishers_info_by_topic(node,&allocator,topic.c_str(),false,&value)==RCL_RET_OK,"native graph lookup");}
  void close(){require(rmw_topic_endpoint_info_array_fini(&value,&allocator)==RMW_RET_OK,"native graph cleanup");closed=true;}
  ~Endpoints(){if(!closed&&rmw_topic_endpoint_info_array_fini(&value,&allocator)!=RMW_RET_OK)std::fprintf(stderr,"FAIL_NATIVE_IMAGE graph cleanup\n");}
};
std::size_t count(rcl_subscription_t const& sub){std::size_t n{};require(rcl_subscription_get_publisher_count(&sub,&n)==RMW_RET_OK,"native matched publisher count");return n;}
}
int main(int argc,char** argv){
 try{
  bool absent=argc==3&&std::strcmp(argv[2],"absent")==0;
  require(argc==2||absent,"usage: image-peer <explicit-namespace> [absent]");std::string ns{argv[1]};
  require(ns.size()>1&&ns.front()=='/'&&ns.find_first_not_of("/abcdefghijklmnopqrstuvwxyz0123456789_")==std::string::npos,"explicit fixture namespace");
  State s;s.open(ns);require(std::string{rmw_get_implementation_identifier()}=="rmw_fastrtps_cpp","actual selected RMW");
  unsigned warmup{},delivered{};bool matched{},delivered_marked{},quiet_started{};Gid expected{},warmup_gid{};
  marker("READY",ns,expected,0,0);auto end=Clock::now()+std::chrono::seconds(30);auto quiet=end;
  while(Clock::now()<end){
    rmw_message_info_t info{};auto rc=rcl_take(&s.sub,&s.message,&info,nullptr);
    if(rc==RCL_RET_OK){
      require(!absent,"zero-grant native Image receipt");auto const& g=info.publisher_gid;
      require(g.implementation_identifier&&std::string{g.implementation_identifier}=="rmw_fastrtps_cpp","native message RMW identity");Gid actual{};std::copy(g.data,g.data+RMW_GID_STORAGE_SIZE,actual.begin());
      if(shape(s.message,true)){require(++warmup==1,"duplicate warmup");warmup_gid=actual;require(!matched||actual==expected,"warmup writer identity");}
      else{require(shape(s.message,false),"complete native Image fields/pixels");require(matched&&++delivered==1&&actual==expected,"final count and actual writer identity");}
    }else require(rc==RCL_RET_SUBSCRIPTION_TAKE_FAILED,"native Image take");
    rc=rcl_take(&s.forbidden,&s.message,&info,nullptr);require(rc==RCL_RET_SUBSCRIPTION_TAKE_FAILED,"forbidden native Image receipt");
    Endpoints forbidden{&s.node,ns+"/forbidden"};require(count(s.forbidden)==0&&forbidden.value.size==0,"forbidden native Image endpoint");forbidden.close();
    Endpoints graph{&s.node,ns+"/image"};auto publishers=count(s.sub);
    if(absent){
      require(publishers==0&&graph.value.size==0,"zero-grant native Image endpoint");
      if(!quiet_started){pollfd p{STDIN_FILENO,POLLIN,0};int n=::poll(&p,1,0);require(n>=0,"gate poll");if(n&&p.revents){char c{};require(::read(STDIN_FILENO,&c,1)==1&&c=='Q',"one Q gate");quiet_started=true;quiet=Clock::now()+std::chrono::seconds(2);}}
      if(quiet_started&&Clock::now()>=quiet){graph.close();require(s.close(),"native explicit cleanup");marker("ABSENT",ns,expected,0,0);return 0;}
    }else{
      if(!matched&&publishers==1&&graph.value.size==1){auto const& e=graph.value.info_array[0];
        if(e.node_name&&e.node_namespace&&e.topic_type&&std::string{e.node_name}=="moonbit_image_gateway"&&std::string{e.node_namespace}==ns&&std::string{e.topic_type}=="sensor_msgs/msg/Image"){
          std::copy(e.endpoint_gid,e.endpoint_gid+RMW_GID_STORAGE_SIZE,expected.begin());require(std::any_of(expected.begin(),expected.end(),[](auto x){return x!=0;}),"nonzero actual writer GID");
          require(e.qos_profile.reliability==RMW_QOS_POLICY_RELIABILITY_RELIABLE&&e.qos_profile.durability==RMW_QOS_POLICY_DURABILITY_VOLATILE,"actual selected native QoS");require(!warmup||warmup_gid==expected,"warmup writer identity");matched=true;marker("MATCHED",ns,expected,warmup,0);
        }
      }
      if(delivered==1&&!delivered_marked){delivered_marked=true;marker("DELIVERED",ns,expected,warmup,delivered);}
      if(delivered==1&&publishers==0&&graph.value.size==0){
        if(!quiet_started){quiet_started=true;quiet=Clock::now()+std::chrono::seconds(2);}
        if(Clock::now()>=quiet){graph.close();require(s.close(),"native explicit cleanup");marker("PASS",ns,expected,warmup,delivered);return 0;}
      }else require(!quiet_started,"publisher reappeared during retirement quiet interval");
    }
    graph.close();std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  throw std::runtime_error("native discovery/delivery/quiet/retirement deadline");
 }catch(std::exception const& e){std::fprintf(stderr,"FAIL_NATIVE_IMAGE %s\n",e.what());return 1;}
}
