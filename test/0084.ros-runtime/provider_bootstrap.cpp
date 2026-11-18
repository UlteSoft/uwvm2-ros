/** @file @brief Hostile bootstrap and real Rust-to-C++ driver protocol fixture.
 * @details This executable is a test receiver, not a vendor driver adapter.
 * It exposes one harmless parameter query and no process/device authority.
 */
#include <uwvm2/imported/ros/scene/attachment_tls.h>
#include <uwvm2/imported/ros/scene/provider_bootstrap_receiver.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace p=s::provider_bootstrap;
namespace d=s::drivers;
using E=d::error;
static std::uint64_t now()
{timespec t{};assert(!::clock_gettime(CLOCK_MONOTONIC,&t));return std::uint64_t(t.tv_sec)*1000000000+t.tv_nsec;}
static void mutations()
{
 std::array<std::uint8_t,256> bytes{};
 s::wire::writer out{bytes};
 for(auto n:std::array<std::uint64_t,32>{p::magic,256,1,1,11,12,13,21,22,23,1,0,2,0,3,0,4,0,5,0,1,2,3,4,32,32,1000,10000,1,2,3,4})out.u64(n);
 p::record expected;assert(p::decode(bytes,expected));
 for(auto field:{0,1,2,3,4,5,6,7,8,9,24,25,26,27,29,31})
 {
  auto changed=bytes;
  for(unsigned n{};n<8;++n)changed[field*8+n]=0xff;
  if(field==29||field==31)std::fill_n(changed.data()+field*8,8,0);
  p::record value=expected;assert(!p::decode(changed,value)&&value==expected);
 }
 for(auto offset:{80,96,112,128,144,160})
 {
  auto changed=bytes;std::fill_n(changed.data()+offset,offset==160?32:16,0);
  p::record value=expected;assert(!p::decode(changed,value)&&value==expected);
 }
 for(std::size_t size{};size<bytes.size();++size){p::record value=expected;assert(!p::decode(std::span{bytes}.first(size),value)&&value==expected);}
 std::puts("CASE bounded-bootstrap-decoder PASS");
}
struct received
{
 std::array<int,4> fds{-1,-1,-1,-1};
 ~received(){for(int fd:fds)if(fd>=0)::close(fd);}
};
static bool receive(int courier,d::credentials parent,received& output,std::string_view mode)
{
 constexpr std::string_view ready="WROS-READY",delivery="WROS-FDS";
 if(mode=="unexpected-rights")
 {
  alignas(cmsghdr) std::array<unsigned char,CMSG_SPACE(sizeof(int))> control{};
  iovec vector{const_cast<char*>(ready.data()),ready.size()};msghdr msg{};
  msg.msg_iov=&vector;msg.msg_iovlen=1;msg.msg_control=control.data();msg.msg_controllen=control.size();
  auto* c=CMSG_FIRSTHDR(&msg);c->cmsg_level=SOL_SOCKET;c->cmsg_type=SCM_RIGHTS;c->cmsg_len=CMSG_LEN(sizeof(int));
  std::memcpy(CMSG_DATA(c),&courier,sizeof(courier));if(::sendmsg(courier,&msg,MSG_DONTWAIT|MSG_NOSIGNAL)<0)return false;
 }
 else if(mode=="bad-origin")
 {
  auto child=::fork();assert(child>=0);
  if(!child)::_exit(::send(courier,ready.data(),ready.size(),MSG_DONTWAIT|MSG_NOSIGNAL)==static_cast<ssize_t>(ready.size())?0:1);
  int status{};assert(::waitpid(child,&status,0)==child&&status==0);
 }
 else if(::send(courier,mode=="bad-ready"?"bad":ready.data(),mode=="bad-ready"?3:ready.size(),MSG_DONTWAIT|MSG_NOSIGNAL)<0)return false;
 auto until=now()+5000000000;
 while(now()<until)
 {
  std::array<char,32> data{};alignas(cmsghdr) std::array<unsigned char,256> control{};
  iovec vector{data.data(),data.size()};msghdr msg{};msg.msg_iov=&vector;msg.msg_iovlen=1;
  msg.msg_control=control.data();msg.msg_controllen=control.size();
  auto n=::recvmsg(courier,&msg,MSG_DONTWAIT|MSG_CMSG_CLOEXEC|MSG_TRUNC);
  if(n<0){if(errno!=EAGAIN&&errno!=EINTR)return false;pollfd pfd{courier,POLLIN,0};::poll(&pfd,1,1);continue;}
  bool bad=msg.msg_flags&(MSG_TRUNC|MSG_CTRUNC);unsigned credentials{},rights{},count{};
  for(auto* c=CMSG_FIRSTHDR(&msg);c;c=CMSG_NXTHDR(&msg,c))
  {
   if(c->cmsg_len<CMSG_LEN(0)){bad=true;break;}
   if(c->cmsg_level==SOL_SOCKET&&c->cmsg_type==SCM_RIGHTS)
   {
    ++rights;auto length=c->cmsg_len-CMSG_LEN(0);if(length%sizeof(int))bad=true;
    for(std::size_t at{};at+sizeof(int)<=length;at+=sizeof(int))
    {int fd;std::memcpy(&fd,CMSG_DATA(c)+at,sizeof(fd));if(count<4)output.fds[count++]=fd;else{::close(fd);bad=true;}}
   }
   else if(c->cmsg_level==SOL_SOCKET&&c->cmsg_type==SCM_CREDENTIALS&&c->cmsg_len==CMSG_LEN(sizeof(ucred)))
   {ucred cred{};std::memcpy(&cred,CMSG_DATA(c),sizeof(cred));++credentials;bad|=d::credentials{static_cast<std::uint64_t>(cred.pid),cred.uid,cred.gid}!=parent;}
   else bad=true;
  }
  return !bad&&credentials==1&&rights==1&&count==4&&n==static_cast<ssize_t>(delivery.size())&&std::equal(delivery.begin(),delivery.end(),data.begin());
 }
 return false;
}
static int serve(int courier,d::credentials parent,std::string_view mode)
{
 if(mode=="unexpected-rights"||mode=="bad-origin"||mode=="bad-ready")
 {received hostile;receive(courier,parent,hostile,mode);return 13;}
 p::descriptors descriptors;p::record config;
 if(p::receive(courier,{parent.pid,parent.uid,parent.gid},UINT64_C(5000000000),descriptors,config)!=p::receive_status::success)return 13;
 ::close(courier);
 auto const& b=config.bound;s::attachment_tls::channel channel;
 if(channel.bind(descriptors.carrier(),{config.peer.pid,config.peer.uid,config.peer.gid},descriptors.identity(),s::attachment_tls::purpose::driver,
   b.deployment,b.vm,b.module,b.driver,b.session,b.contract,176+std::max(config.request_bytes,config.response_bytes),true,descriptors.clock())!=E::success)return 13;
 // The channel has duplicated its carrier and consumed its key. Close the
 // delivered originals now; no bootstrap descriptor remains in the hot path.
 descriptors.reset();
 std::array<std::uint8_t,65712> input{};std::array<std::uint8_t,24> body{};
 s::wire::writer out{body};out.boolean(true);out.boolean(false);out.u64(3);unsigned calls{};bool greeted{};
 auto until=std::min(now()+10000000000,config.session_until_ns);
 while(now()<until)
 {
  std::size_t size{};auto result=channel.receive(input,size);
  if(result==E::again)continue;
  if(result!=E::success)return 13;
  d::protocol::header h;s::wire::bytes data;
  if(!d::protocol::decode(std::span{input}.first(size),h,data)||h.bound!=b)return 13;
  if(h.type==d::protocol::kind::release)return greeted&&calls==1?0:13;
  std::size_t length{};
  if(h.type==d::protocol::kind::hello){if(greeted)return 13;greeted=true;h.type=d::protocol::kind::ready;}
  else
  {
   auto timestamp=now();s::wire::reader request{data};std::uint64_t operation{};s::wire::bytes name;
   if(!greeted||h.type!=d::protocol::kind::request||h.sequence!=1||data.size()!=17||calls++||
      h.deadline<=timestamp||h.deadline>config.session_until_ns||h.deadline-timestamp>config.request_ns||
      !request.u64(operation)||operation!=7||!request.blob(name)||name.size()!=1||name[0]!='x'||!request.done())return 13;
   h.type=d::protocol::kind::result;length=body.size();
  }
  std::array<std::uint8_t,176> header{};if(!d::protocol::encode(h,length,header))return 13;
  do{result=channel.send(header,std::span{body}.first(length));}while(result==E::again&&now()<until);
  s::erase_bytes(std::span{input}.first(size));if(result!=E::success)return 13;
 }
 return 13;
}
int main(int argc,char** argv)
{
 ::alarm(20);
 if(argc==1){mutations();return 0;}
 assert(argc==6);
 std::array<std::uint64_t,4> fields{};
 for(unsigned i{};i<4;++i){std::string_view arg{argv[i+1]};auto r=std::from_chars(arg.data(),arg.data()+arg.size(),fields[i]);assert(r.ec==std::errc{}&&r.ptr==arg.data()+arg.size());}
 return serve(static_cast<int>(fields[0]),{fields[1],fields[2],fields[3]},argv[5]);
}
