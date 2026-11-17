/** @file @brief External driver session isolation, kernel origin and authority regressions. */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
#include <cstdio>
#include <sys/wait.h>
#include <poll.h>
namespace s=uwvm2::imported::ros::scene;
namespace d=s::drivers;
using E=s::error;
static d::protocol::binding identity(unsigned client=1)
{return {{1,2},{3,client},{5,client},{7,8},{9,client},{11,12,13,14}};}
static void pair(int (&fds)[2])
{assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,fds));int one=1;
 for(int fd:fds)assert(!::setsockopt(fd,SOL_SOCKET,SO_PASSCRED,&one,sizeof(one)));}
static void send_packet(int fd,d::protocol::header const& h,s::wire::bytes body={})
{std::array<std::uint8_t,512> bytes{};assert(d::protocol::encode(h,body.size(),std::span{bytes}.first(d::protocol::header_bytes)));
 std::copy(body.begin(),body.end(),bytes.begin()+d::protocol::header_bytes);
 assert(::send(fd,bytes.data(),d::protocol::header_bytes+body.size(),MSG_NOSIGNAL)==static_cast<ssize_t>(d::protocol::header_bytes+body.size()));}
static d::protocol::header receive(int fd)
{std::array<std::uint8_t,512> bytes;auto n=::recv(fd,bytes.data(),bytes.size(),0);assert(n>0);d::protocol::header h;s::wire::bytes body;
 assert(d::protocol::decode({bytes.data(),static_cast<std::size_t>(n)},h,body));return h;}
static std::uint64_t now(){return static_cast<std::uint64_t>(uwvm2::imported::ros::resources::steady_now());}
template<typename F>static E until(F&& f)
{auto bound=now()+UINT64_C(3000000000);E e;do{e=f();if(e!=E::again)return e;assert(now()<bound);::sched_yield();}while(true);}
static void authority()
{
 int fd[2];pair(fd);s::deployment config;
 assert(config.create("robot")==E::success&&config.bind("robot","main")==E::success&&config.bind("robot","peer")==E::success);
 for(auto op:d::names)assert(config.expose("robot",uwvm2::utils::container::std_string(op),s::exposure::allow)==E::success);
 assert(config.expose("robot","capability.open",s::exposure::allow)==E::success);
 assert(config.expose("robot","component.start",s::exposure::allow)==E::invalid);
 s::grant g;g.alias="arm-position";g.resource="arm-left-position";g.binding="registered-driver";g.kind=s::purpose::driver;
 for(auto op:d::names)g.actions.emplace(op);assert(config.allow("robot",g)==E::success);
 assert(config.driver_budget("robot",1,65536)==E::success);
 d::credentials peer{static_cast<std::uint64_t>(::getpid()),::getuid(),::getgid()};auto time=now();
 assert(config.driver_bind("robot",g.alias,"absent",fd[0],peer,identity(),{32,32,1000000000,time+10000000000})==E::denied);
 assert(config.driver_bind("robot",g.alias,"main",fd[0],peer,identity(),{32,32,1000000000,time+10000000000})==E::success);
 assert(config.validate()==E::invalid);
 assert(config.driver_schema("robot",g.alias,"parameter.exists",{0,64,64,8})==E::success);
 assert(config.validate()==E::success);::close(fd[0]);
 int a{},b{};s::caller ca{&a,1},cb{&b,1};std::shared_ptr<s::broker> state,other;
 assert(config.select_exposed("main","driver.connect",ca,state)==E::success);
 assert(config.select_exposed("peer","driver.connect",cb,other)==E::success&&other==state);
 s::handle cap{};assert(state->permissions.open(ca,g.alias,cap)==E::success);
 std::size_t required{};std::array<std::uint8_t,64> delivered{};
 auto sink=[&](s::wire::bytes bytes){std::copy(bytes.begin(),bytes.end(),delivered.begin());return true;};
 auto invoke=[&](s::caller who,std::string_view module,d::operation op,s::wire::bytes input={},std::size_t size=64)
 {return state->driver_store->invoke(state->permissions,who,module,op,cap,input,size,required,sink);};
 assert(invoke(cb,"peer",d::operation::connect)==E::denied);
 assert(invoke(ca,"main",d::operation::connect,{},0)==E::too_large&&required==8);
 std::uint8_t byte;assert(::recv(fd[1],&byte,1,MSG_DONTWAIT)<0&&errno==EAGAIN);
 assert(invoke(ca,"main",d::operation::describe)==E::success);
 s::debug::request observation;observation.selected=s::debug::selection::resources;s::debug::view observed;
 config.inspect(observation,observed);assert(observed.result==s::debug::status::ok&&observed.size==1);
 assert(observed.rows[0].type.view()=="driver"&&observed.rows[0].state.view()=="prepared");
 assert(::recv(fd[1],&byte,1,MSG_DONTWAIT)<0&&errno==EAGAIN); // Inspection cannot handshake.
 assert(invoke(ca,"main",d::operation::connect)==E::again);receive(fd[1]);
 send_packet(fd[1],{d::protocol::kind::ready,identity()});assert(invoke(ca,"main",d::operation::connect)==E::success);
 assert(invoke(cb,"main",d::operation::connect)==E::denied); // Runtime incarnation also sealed.
 std::array<std::uint8_t,64> input{};s::wire::writer writer{input};writer.u64(1);writer.u64(now()+1000000000);std::array<std::uint8_t,17> body{};s::wire::writer query{body};query.u64(7);query.text("x");writer.blob(body);
 assert(invoke(ca,"main",d::operation::submit,writer.written())==E::success);auto request=receive(fd[1]);request.type=d::protocol::kind::result;std::array<std::uint8_t,24> answer{};s::wire::writer reply{answer};reply.boolean(true);reply.boolean(false);reply.u64(3);send_packet(fd[1],request,answer);
 std::array<std::uint8_t,8> seq{};s::wire::writer sequence{seq};sequence.u64(1);
 assert(invoke(ca,"main",d::operation::result,seq)==E::success&&required==40);
 s::attenuation reduced;reduced.calls=0;reduced.uncertain=true;
 assert(state->permissions.attenuate(g.alias,reduced)==E::success);
 assert(invoke(ca,"main",d::operation::submit,writer.written())==E::again);
 assert(invoke(ca,"main",d::operation::disconnect)==E::success); // Reserved cleanup bypasses metering only.
 assert(receive(fd[1]).type==d::protocol::kind::release);
 reduced.calls=1;assert(state->permissions.attenuate(g.alias,reduced)==E::denied);
 assert(state->revoke(g.alias)==E::success);
 assert(invoke(ca,"main",d::operation::submit,writer.written())==E::denied);
 config.retire_member(ca);assert(invoke(ca,"main",d::operation::describe)==E::denied);
 d::snapshot snap;assert(state->driver_store->inspect(g.alias,snap)&&snap.phase==d::state::closed);
 ::close(fd[1]);
}
static void shared_process()
{
 int a[2],b[2],barrier[2];pair(a);pair(b);assert(!::pipe2(barrier,O_CLOEXEC));
 auto driver=::fork();assert(driver>=0);
 if(!driver)
 {
  ::close(a[0]);::close(b[0]);::close(barrier[0]);::close(barrier[1]);
  std::array<pollfd,2> pollers{{{a[1],POLLIN,0},{b[1],POLLIN,0}}};unsigned active=2;
  while(active)
  {
   assert(::poll(pollers.data(),pollers.size(),5000)>0);
   for(auto& p:pollers)if(p.fd>=0&&p.revents)
   {
    std::array<std::uint8_t,512> bytes;auto n=::recv(p.fd,bytes.data(),bytes.size(),0);
    if(!n){::close(p.fd);p.fd=-1;--active;continue;}
    assert(n>0);d::protocol::header h;s::wire::bytes body;assert(d::protocol::decode({bytes.data(),static_cast<std::size_t>(n)},h,body));
    if(h.type==d::protocol::kind::hello){h.type=d::protocol::kind::ready;send_packet(p.fd,h);}
    else if(h.type==d::protocol::kind::release){::close(p.fd);p.fd=-1;--active;}
    else{assert(h.type==d::protocol::kind::request);h.type=d::protocol::kind::result;send_packet(p.fd,h,body);}
   }
  }
  ::_exit(0);
 }
 ::close(a[1]);::close(b[1]);
 auto client=[&](unsigned index,int fd)
 {
  d::session session;auto time=now();assert(session.prepare(fd,{static_cast<std::uint64_t>(driver),::getuid(),::getgid()},identity(index),{16,16,1000000000,time+10000000000})==E::success);::close(fd);
  assert(until([&]{return session.connect(now());})==E::success);
  if(index==2){char ready{};assert(::read(barrier[0],&ready,1)==1);}
  std::array<std::uint8_t,8> payload{1,2,3,4,5,6,7,8};auto deadline=now()+1000000000;
  assert(until([&]{return session.submit(1,deadline,payload,now());})==E::success);
  assert(until([&]{return session.result(1,now(),[&](auto status,auto body){return status==0&&std::equal(body.begin(),body.end(),payload.begin(),payload.end());});})==E::success);
  session.disconnect();
  if(index==1){char ready=1;assert(::write(barrier[1],&ready,1)==1);}
 };
 auto first=::fork();assert(first>=0);if(!first){::close(b[0]);::close(barrier[0]);client(1,a[0]);::_exit(0);}
 auto second=::fork();assert(second>=0);if(!second){::close(a[0]);::close(barrier[1]);client(2,b[0]);::_exit(0);}
 ::close(a[0]);::close(b[0]);::close(barrier[0]);::close(barrier[1]);
 for(auto pid:{first,second,driver}){int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&WEXITSTATUS(status)==0);}
}
static void forbidden_descriptor()
{
 int fd[2];pair(fd);d::channel c;assert(c.bind(fd[0],{static_cast<std::uint64_t>(::getpid()),::getuid(),::getgid()})==E::success);
 std::array<std::uint8_t,d::protocol::header_bytes> header{};assert(d::protocol::encode({d::protocol::kind::ready,identity()},0,header));
 alignas(cmsghdr) unsigned char ancillary[CMSG_SPACE(sizeof(int))]{};iovec part{header.data(),header.size()};msghdr msg{};
 msg.msg_iov=&part;msg.msg_iovlen=1;msg.msg_control=ancillary;msg.msg_controllen=sizeof(ancillary);auto* cm=CMSG_FIRSTHDR(&msg);
 cm->cmsg_level=SOL_SOCKET;cm->cmsg_type=SCM_RIGHTS;cm->cmsg_len=CMSG_LEN(sizeof(int));std::memcpy(CMSG_DATA(cm),&fd[1],sizeof(int));
 assert(::sendmsg(fd[1],&msg,MSG_NOSIGNAL)>0);std::size_t size{};assert(c.receive(header,size)==E::denied);::close(fd[0]);::close(fd[1]);
}
static void dead_peer()
{
 int fd[2],signal[2];pair(fd);assert(!::pipe2(signal,O_CLOEXEC));auto pid=::fork();assert(pid>=0);
 if(!pid){::close(fd[0]);::close(signal[1]);char byte{};assert(::read(signal[0],&byte,1)==1);::_exit(17);}
 ::close(fd[1]);::close(signal[0]);d::session session;auto time=now();
 assert(session.prepare(fd[0],{static_cast<std::uint64_t>(pid),::getuid(),::getgid()},identity(),{16,16,1000000,time+1000000000})==E::success);
 char byte=1;assert(::write(signal[1],&byte,1)==1);int status{};assert(::waitpid(pid,&status,0)==pid&&WEXITSTATUS(status)==17);
 assert(session.connect(now())==E::denied);assert(session.inspect().phase==d::state::failed);
 ::close(fd[0]);::close(signal[1]);
}
static void duplicate_channel()
{
 int fd[2];pair(fd);s::driver_dispatch dispatch;assert(dispatch.budget(3,100000)==E::success);
 d::credentials peer{static_cast<std::uint64_t>(::getpid()),::getuid(),::getgid()};auto time=now();
 assert(dispatch.bind("one","main","query",fd[0],peer,identity(),{32,32,1000000,time+1000000000})==E::success);
 assert(dispatch.bind("two","main","query",fd[0],peer,identity(2),{32,32,1000000,time+1000000000})==E::conflict);
 assert(dispatch.bind("two","main","query",fd[1],peer,identity(),{32,32,1000000,time+1000000000})==E::conflict);
 assert(!dispatch.contains("one"));assert(dispatch.schema("one","driver.submit",{64,64,64,8})==E::invalid);
 assert(dispatch.schema("one","parameter.exists",{64,64,64,8})==E::success&&dispatch.contains("one"));
 dispatch.freeze();assert(dispatch.schema("one","parameter.exists",{64,64,64,8})==E::denied);dispatch.retire();::close(fd[0]);::close(fd[1]);
}
static void platform_gate()
{
 int fd[2];pair(fd);s::driver_dispatch dispatch;assert(dispatch.budget(1,65536)==E::success);
 d::credentials peer{static_cast<std::uint64_t>(::getpid()),::getuid(),::getgid()};auto time=now();
 assert(dispatch.bind("arm","main","arm",fd[0],peer,identity(),{32,32,1000000,time+1000000000})==E::success);
 assert(dispatch.schema("arm","device.describe",{0,64,64,8})==E::success);
 constexpr std::array routes{s::platform_extensions::route{"describe",s::robot_contract::operation::device_describe}};
 s::platform_extensions::policy platform;constexpr auto module="uwvm_ros_v2.platform_extension.fixture.v1";
 assert(platform.register_module({module,routes})==E::success);
 s::grant grant;grant.alias="arm";grant.resource="arm";grant.kind=s::purpose::driver;grant.binding="registered-driver";grant.actions={"driver.connect"};
 assert(platform.bind(module,grant)==E::success&&platform.select(module,false)==E::success&&platform.seal()==E::success);
 assert(dispatch.validate_platforms(platform)==E::success);dispatch.freeze(platform);
 s::authority auth;assert(auth.configure(grant)==E::success&&auth.expose("capability.open",s::exposure::allow)==E::success&&auth.expose("driver.connect",s::exposure::allow)==E::success);
 int instance{};s::caller who{&instance,1};assert(auth.enroll(who)==E::success);s::handle cap{};assert(auth.open(who,"arm",cap)==E::success);
 assert(dispatch.preflight(auth,who,"main",d::operation::connect,cap,8)==E::denied);
 std::size_t required{};assert(dispatch.invoke(auth,who,"main",d::operation::connect,cap,{},8,required,[](auto){return true;})==E::denied);
 std::uint8_t byte;assert(::recv(fd[1],&byte,1,MSG_DONTWAIT)<0&&errno==EAGAIN);::close(fd[0]);::close(fd[1]);
}
static void global_endpoint_ownership()
{
 int fd[2];pair(fd);int alias=::fcntl(fd[0],F_DUPFD_CLOEXEC,3);assert(alias>=0);
 s::deployment config;
 for(auto [env,module,channel,number]:{std::tuple{"first","main",fd[0],1u},std::tuple{"second","peer",alias,2u}})
 {
  assert(config.create(env)==E::success&&config.bind(env,module)==E::success);
  for(auto op:d::names)assert(config.expose(env,uwvm2::utils::container::std_string(op),s::exposure::allow)==E::success);
  s::grant g;g.alias="arm";g.resource="joint";g.binding="registered-driver";g.kind=s::purpose::driver;
  for(auto op:d::names)g.actions.emplace(op);assert(config.allow(env,g)==E::success);
  assert(config.driver_budget(env,1,65536)==E::success);
  assert(config.driver_bind(env,"arm",module,channel,{static_cast<std::uint64_t>(::getpid()),::getuid(),::getgid()},identity(number),{32,32,1000000000,now()+10000000000})==E::success);
  assert(config.driver_schema(env,"arm","parameter.exists",{0,64,64,8})==E::success);
 }
 // Different environment, module, session and fd numbers cannot create two
 // independent consumers of the same authenticated kernel endpoint.
 assert(config.validate()==E::conflict);
 s::attachment_registry cross_protocol;assert(cross_protocol.descriptor(fd[0])==E::success);
 assert(cross_protocol.descriptor(alias)==E::conflict);
 assert(cross_protocol.session(identity().session)==E::success&&cross_protocol.session(identity().session)==E::conflict);
 int pipes[2];assert(!::pipe2(pipes,O_CLOEXEC|O_NONBLOCK));
 assert(cross_protocol.descriptor(pipes[0])==E::success&&cross_protocol.descriptor(pipes[1])==E::conflict);
 for(int value:{fd[0],fd[1],alias,pipes[0],pipes[1]})::close(value);
}
int main(){authority();shared_process();forbidden_descriptor();dead_peer();duplicate_channel();platform_gate();global_endpoint_ownership();std::puts("PASS exact module authority, transactional delivery, retirement, shared external process and descriptor rejection");}
