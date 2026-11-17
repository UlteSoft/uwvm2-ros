#include <uwvm2/imported/ros/scene/driver_session.h>
#include <cassert>
#include <cstdio>
#include <sys/wait.h>
namespace d=uwvm2::imported::ros::scene::drivers;
using E=d::error;
static d::protocol::binding binding()
{return {{1,2},{3,4},{5,6},{7,8},{9,10},{11,12,13,14}};}
static void send_frame(int fd,d::protocol::header const& h,std::span<std::uint8_t const> body={})
{std::array<std::uint8_t,512> bytes{};assert(d::protocol::encode(h,body.size(),std::span{bytes}.first(d::protocol::header_bytes)));
 std::copy(body.begin(),body.end(),bytes.begin()+d::protocol::header_bytes);
 assert(::send(fd,bytes.data(),d::protocol::header_bytes+body.size(),MSG_NOSIGNAL)==static_cast<ssize_t>(d::protocol::header_bytes+body.size()));}
static d::protocol::header receive(int fd)
{std::array<std::uint8_t,512> bytes;auto n=::recv(fd,bytes.data(),bytes.size(),0);assert(n>0);d::protocol::header h;
 uwvm2::imported::ros::scene::wire::bytes body;assert(d::protocol::decode({bytes.data(),static_cast<std::size_t>(n)},h,body));return h;}
static void pair(int (&fds)[2])
{assert(::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,fds)==0);int one=1;assert(::setsockopt(fds[0],SOL_SOCKET,SO_PASSCRED,&one,sizeof(one))==0);}
static void hostile_response(unsigned mode)
{
 int fds[2];pair(fds);d::session session;auto b=binding();
 assert(session.prepare(fds[0],{static_cast<std::uint64_t>(::getpid()),::getuid(),::getgid()},b,{8,8,100,1000})==E::success);
 ::close(fds[0]);assert(session.connect(1)==E::again);receive(fds[1]);
 send_frame(fds[1],{d::protocol::kind::ready,b});assert(session.connect(2)==E::success);
 std::array<std::uint8_t,8> payload{1};assert(session.submit(1,50,payload,3)==E::success);
 auto h=receive(fds[1]);h.type=d::protocol::kind::result;
 if(mode==0)++h.sequence;else if(mode==1)++h.deadline;else if(mode==2)++h.bound.deployment[0];
 if(mode==3){std::uint8_t byte{};assert(::send(fds[1],&byte,1,MSG_NOSIGNAL)==1);}
 else if(mode==4){std::array<std::uint8_t,9> oversized{};send_frame(fds[1],h,oversized);}
 else send_frame(fds[1],h,payload);
 unsigned delivered{};auto result=session.result(1,4,[&](auto,auto){++delivered;return true;},
  [&](auto,auto)noexcept{return mode!=5;});
 assert(result==(mode==5?E::invalid:E::denied)&&delivered==0);
 auto snapshot=session.inspect();assert(snapshot.phase==d::state::failed&&snapshot.effect_unknown);
 assert(session.connect(2000)==E::denied&&session.submit(1,50,payload,2000)==E::denied);
 assert(session.inspect().phase==d::state::failed&&session.inspect().effect_unknown);::close(fds[1]);
}
static void expired_retry()
{
 int fds[2];pair(fds);d::session session;auto b=binding();
 assert(session.prepare(fds[0],{static_cast<std::uint64_t>(::getpid()),::getuid(),::getgid()},b,{8,8,100,1000})==E::success);
 ::close(fds[0]);assert(session.connect(1)==E::again);receive(fds[1]);send_frame(fds[1],{d::protocol::kind::ready,b});
 assert(session.connect(2)==E::success);std::array<std::uint8_t,8> payload{1};
 assert(session.submit(1,50,payload,3)==E::success);receive(fds[1]);
 assert(session.submit(1,50,payload,50)==E::timeout&&session.inspect().effect_unknown);
 assert(session.submit(1,50,payload,51)==E::denied);session.disconnect();
 assert(session.connect(2000)==E::denied&&session.inspect().phase==d::state::closed);::close(fds[1]);
}
int main()
{
 for(unsigned mode{};mode!=6;++mode)hostile_response(mode);expired_retry();
 auto b=binding();d::protocol::header h{d::protocol::kind::hello,b},copy=h;std::array<std::uint8_t,d::protocol::header_bytes> bytes;
 assert(d::protocol::encode(h,0,bytes));uwvm2::imported::ros::scene::wire::bytes body;assert(d::protocol::decode(bytes,copy,body)&&copy==h);
 for(std::size_t size{};size<bytes.size();++size){copy=h;assert(!d::protocol::decode(std::span{bytes}.first(size),copy,body)&&copy==h);}
 bytes[24]=1;assert(!d::protocol::decode(bytes,copy,body));bytes[24]=0;
 int fds[2];pair(fds);d::session s;
 d::credentials peer{static_cast<std::uint64_t>(::getpid()),::getuid(),::getgid()};
 assert(s.prepare(fds[0],peer,b,{16,16,100,1000})==E::success);::close(fds[0]);
 assert(s.connect(1)==E::again);auto hello=receive(fds[1]);assert(hello.type==d::protocol::kind::hello);
 send_frame(fds[1],{d::protocol::kind::ready,b});assert(s.connect(2)==E::success);
 std::array<std::uint8_t,8> data{1,2,3,4,5,6,7,8};
 assert(s.submit(1,50,data,3)==E::success);auto request=receive(fds[1]);assert(request.sequence==1);
 assert(s.submit(1,50,data,4)==E::success);data[0]=9;assert(s.submit(1,50,data,4)==E::conflict);data[0]=1;
 request.type=d::protocol::kind::result;send_frame(fds[1],request,data);
 assert(s.result(1,5,[](auto,auto){return false;})==E::invalid);
 assert(s.result(1,6,[&](auto status,auto value){return !status&&std::equal(value.begin(),value.end(),data.begin(),data.end());})==E::success);
 assert(s.submit(1,50,data,7)==E::conflict);assert(s.submit(2,60,data,7)==E::success);receive(fds[1]);
 assert(s.result(2,60,[](auto,auto){return true;})==E::timeout);assert(s.inspect().effect_unknown);s.disconnect();::close(fds[1]);
 pair(fds);d::session wrong;peer.uid+=1;assert(wrong.prepare(fds[0],peer,b,{16,16,100,1000})==E::success);
 assert(wrong.connect(1)==E::again);receive(fds[1]);send_frame(fds[1],{d::protocol::kind::ready,b});assert(wrong.connect(2)==E::denied);::close(fds[0]);::close(fds[1]);
 std::puts("PASS driver framing, credentials, exact retries, uncertain timeout and attachment-only close");
}
