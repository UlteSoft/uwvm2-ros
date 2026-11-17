/** @file @brief Matched native admission baseline and separate-process driver timing fixture. */
#include <uwvm2/imported/ros/scene/driver_dispatch.h>
#include <cassert>
#include <charconv>
#include <cstdio>
#include <sched.h>
namespace s=uwvm2::imported::ros::scene;
namespace d=s::drivers;
using E=s::error;
static std::uint64_t now(){return static_cast<std::uint64_t>(uwvm2::imported::ros::resources::steady_now());}
static std::uint64_t number(char const* text){std::uint64_t n{};auto last=text+std::char_traits<char>::length(text);auto r=std::from_chars(text,last,n);assert(r.ec==std::errc{}&&r.ptr==last);return n;}
static d::protocol::binding identity()
{d::protocol::binding b{{1,0},{2,0},{3,0},{4,0},{5,0},{}};for(unsigned n{};n<32;++n)b.contract[n/8]|=std::uint64_t(n+1)<<(8*(n%8));return b;}
static int server(int fd,std::uint64_t count,int key_fd=-1,int client_pipe=-1)
{
 std::array<std::uint8_t,512> bytes{};std::array<std::uint8_t,24> response{};s::wire::writer out{response};out.boolean(true);out.boolean(false);out.u64(3);
 s::attachment_tls::channel encrypted;
 if(key_fd>=0){
  std::uint32_t pid{};assert(::read(client_pipe,&pid,sizeof(pid))==sizeof(pid));::close(client_pipe);
  auto b=identity();
  assert(encrypted.bind(fd,{pid,::getuid(),::getgid()},key_fd,s::attachment_tls::purpose::driver,
   b.deployment,b.vm,b.module,b.driver,b.session,b.contract,d::protocol::header_bytes+32,true)==E::success);
  ::close(key_fd);
 }
 std::uint64_t seen{},begin{},end{};
 for(;;)
 {
  ssize_t n{};
  if(key_fd>=0){
   std::size_t size{};auto result=encrypted.receive(bytes,size);if(result==E::again)continue;
   assert(result==E::success);n=static_cast<ssize_t>(size);
  }else{n=::recv(fd,bytes.data(),bytes.size(),0);if(!n)break;assert(n>0);}
  d::protocol::header h;s::wire::bytes body;
  assert(d::protocol::decode({bytes.data(),static_cast<std::size_t>(n)},h,body)&&h.bound==identity());
  if(h.type==d::protocol::kind::release)break;
  if(h.type==d::protocol::kind::hello)h.type=d::protocol::kind::ready;
  else{assert(h.type==d::protocol::kind::request&&h.sequence==seen+1&&body.size()==17);if(!seen)begin=now();++seen;h.type=d::protocol::kind::result;}
  auto length=h.type==d::protocol::kind::result?response.size():0;
  assert(d::protocol::encode(h,length,std::span{bytes}.first(d::protocol::header_bytes)));
  std::copy_n(response.begin(),length,bytes.begin()+d::protocol::header_bytes);
  if(key_fd>=0){
   E sent;do{sent=encrypted.send(std::span{bytes}.first(d::protocol::header_bytes),std::span{response}.first(length));}while(sent==E::again);
   assert(sent==E::success);
  }else assert(::send(fd,bytes.data(),d::protocol::header_bytes+length,MSG_NOSIGNAL)==static_cast<ssize_t>(d::protocol::header_bytes+length));
  if(seen==count){end=now();}
 }
 assert(seen==count&&end>begin);std::printf("{\"calls\":%llu,\"receiver_interval_ns\":%llu,\"ns_per_roundtrip\":%.3f}\n",(unsigned long long)seen,(unsigned long long)(end-begin),double(end-begin)/double(count));return 0;
}
int main(int argc,char** argv)
{
 assert(argc>=4);int fd=static_cast<int>(number(argv[2]));auto count=number(argv[3]);
 if(std::string_view(argv[1])=="server")return server(fd,count);
 if(std::string_view(argv[1])=="tls-server"){assert(argc==6);return server(fd,count,static_cast<int>(number(argv[4])),static_cast<int>(number(argv[5])));}
 assert(argc==6||argc==7);int key_fd=argc==7?static_cast<int>(number(argv[6])):-1;auto pid=number(argv[4]),deadline=number(argv[5]);auto b=identity();s::authority auth;s::driver_dispatch dispatch;
 int instance{};s::caller who{&instance,1};s::grant grant;grant.alias="arm-query";grant.resource="arm-parameters";grant.binding="registered-driver";grant.kind=s::purpose::driver;
 for(auto name:d::names){grant.actions.emplace(name);assert(auth.expose(uwvm2::utils::container::std_string(name),s::exposure::allow)==E::success);}
 assert(auth.expose("capability.open",s::exposure::allow)==E::success&&auth.configure(grant)==E::success&&auth.enroll(who)==E::success);
 s::handle cap{};assert(auth.open(who,grant.alias,cap)==E::success);
 assert(dispatch.budget(1,key_fd<0?65536:1048576)==E::success&&dispatch.bind(grant.alias,"main",grant.resource,fd,{pid,::getuid(),::getgid()},b,{32,32,60000000000,deadline+1000000000,key_fd})==E::success);
 assert(dispatch.schema(grant.alias,"parameter.exists",{0,64,64,8})==E::success);dispatch.freeze();::close(fd);
 std::array<std::uint8_t,64> response{};std::size_t required{};
 auto invoke=[&](d::operation op,s::wire::bytes input)
 {
  auto ready=dispatch.preflight(auth,who,"main",op,cap,input.size()+response.size());if(ready!=E::success)return ready;
  return dispatch.invoke(auth,who,"main",op,cap,input,response.size(),required,[&](auto data){std::copy(data.begin(),data.end(),response.begin());return true;});
 };
 auto wait=[&](d::operation op,s::wire::bytes input)
 {E result;do{result=invoke(op,input);assert(now()<deadline);}while(result==E::again);assert(result==E::success);};
 wait(d::operation::connect,{});
 std::array<std::uint8_t,17> query{};s::wire::writer q{query};q.u64(7);q.text("x");
 std::array<std::uint8_t,41> request{};std::array<std::uint8_t,8> key{};
 for(std::uint64_t i=1;i<=count;++i)
 {
  s::wire::writer w{request};w.u64(i);w.u64(deadline);w.blob(query);wait(d::operation::submit,request);
  s::wire::writer seq{key};seq.u64(i);wait(d::operation::result,key);assert(required==40&&response[16]==1);
 }
 wait(d::operation::disconnect,{});return 0;
}
