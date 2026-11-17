/** @file
 * @brief Test-only shared original MuJoCo process for two real Wasm callers.
 * @details Tests the production receiver_control gate against simultaneous
 * competing origins, stale/forged epochs, trusted handover and one client's
 * release while another controls the same actuator. External test orchestration
 * creates processes; neither the VM nor this gate implements a deployer.
 */
#include <uwvm2/imported/ros/scene/driver_control.h>
#include <uwvm2/imported/ros/scene/schema_contract.h>
#include <uwvm2/imported/ros/scene/buffer_contract.h>
#include <mujoco/mujoco.h>
#include <cmath>
#include <cstdio>
namespace s=uwvm2::imported::ros::scene;namespace d=s::drivers;namespace p=d::protocol;namespace c=s::control;
using d::error;
static void require(bool v,char const* message){if(!v){std::fprintf(stderr,"fixture refusal: %s\n",message);::_exit(101);}}
static std::uint64_t now(){return static_cast<std::uint64_t>(uwvm2::imported::ros::resources::steady_now());}
static std::uint64_t number(char const* text)
{std::uint64_t v{};auto end=text+std::char_traits<char>::length(text);auto r=std::from_chars(text,end,v);require(r.ec==std::errc{}&&r.ptr==end,"integer");return v;}
static p::binding identity(unsigned n)
{
 p::binding result{{1,0},{2+256*n,0},{3+256*n,0},{4,0},{5+256*n,0},{}};
 for(unsigned i{};i<32;++i)result.contract[i/8]|=std::uint64_t(i+1)<<(8*(i%8));return result;
}
static bool profile(s::wire::reader& in,std::string_view name)
{std::string_view selected;s::wire::bytes digest;auto hash=s::buffers::hash({reinterpret_cast<std::uint8_t const*>(name.data()),name.size()});
 return in.text(selected)&&selected==name&&in.blob(digest)&&digest.size()==hash.size()&&std::equal(digest.begin(),digest.end(),hash.begin());}
struct packet
{
 std::array<std::uint8_t,8192> bytes{};p::header header;s::wire::bytes body;d::credentials peer;
 void receive(int fd,unsigned index,unsigned expected)
 {
  alignas(cmsghdr)std::array<unsigned char,CMSG_SPACE(sizeof(ucred))> ancillary{};iovec buffer{bytes.data(),bytes.size()};msghdr message{};
  message.msg_iov=&buffer;message.msg_iovlen=1;message.msg_control=ancillary.data();message.msg_controllen=ancillary.size();
  auto count=::recvmsg(fd,&message,MSG_TRUNC);require(count>0&&!(message.msg_flags&(MSG_TRUNC|MSG_CTRUNC)),"packet extent");
  auto* entry=CMSG_FIRSTHDR(&message);require(entry&&entry->cmsg_level==SOL_SOCKET&&entry->cmsg_type==SCM_CREDENTIALS&&entry->cmsg_len==CMSG_LEN(sizeof(ucred))&&!CMSG_NXTHDR(&message,entry),"credentials");
  ucred observed{};std::memcpy(&observed,CMSG_DATA(entry),sizeof(observed));require(observed.pid>0&&static_cast<unsigned>(observed.pid)==expected&&observed.uid==::getuid()&&observed.gid==::getgid(),"exact caller");
  peer={static_cast<unsigned>(observed.pid),observed.uid,observed.gid};
  require(p::decode({bytes.data(),static_cast<std::size_t>(count)},header,body)&&header.bound==identity(index),"registered origin");
 }
 void send(int fd,bool accepted,bool ready=false)
 {
  std::array<std::uint8_t,512> payload{};s::wire::writer out{payload};
  if(ready){header.type=p::kind::ready;}
  else
  {
   out.u64(header.sequence);out.text(accepted?"succeeded":"failed");out.text(accepted?"observed_remote":"none");out.u64(1);out.text(accepted?"simulation_step":"ownership_fence");
   require(s::schema_contract::validate(out.written(),s::schema_contract::type::device_submit__result,{512,128,128,16}),"canonical receipt");header.type=p::kind::result;header.status=accepted?0:13;
  }
  require(p::encode(header,out.size(),std::span{bytes}.first(p::header_bytes)),"reply header");std::copy(out.written().begin(),out.written().end(),bytes.begin()+p::header_bytes);
  require(::write(fd,bytes.data(),p::header_bytes+out.size())==static_cast<ssize_t>(p::header_bytes+out.size()),"reply write");
 }
 double command(unsigned expected_sequence,std::uint64_t& epoch)
 {
  require(header.type==p::kind::request&&header.sequence==expected_sequence&&s::schema_contract::validate(body,s::schema_contract::type::device_submit__request,{8192,128,128,16}),"canonical command");
  s::wire::reader in{body};std::uint64_t session{},sequence{},device{},cap{},controller{},observations{},dependencies{},deadline{};std::string_view type,encoding;s::wire::bytes payload;
  require(in.u64(session)&&session==1&&in.u64(sequence)&&sequence==header.sequence&&in.u64(device)&&device==1&&in.u64(cap)&&cap==1&&in.u64(controller)&&controller==1&&in.u64(epoch),"device origin");
  require(profile(in,"simulation.position")&&profile(in,"simulation.radian.logical")&&in.text(type)&&type=="simulation.joint"&&in.text(encoding)&&encoding=="le64"&&in.blob(payload)&&payload.size()==8&&in.u64(observations)&&!observations&&in.u64(dependencies)&&!dependencies&&in.u64(deadline)&&deadline==header.deadline&&in.done(),"command limits");
  s::wire::reader scalar{payload};std::uint64_t bits{};require(scalar.u64(bits),"target");auto target=std::bit_cast<double>(bits);require(std::isfinite(target)&&target>=-1&&target<=1,"physical range");return target;
 }
};
int main(int argc,char** argv)
{
 require(argc==3||argc==5,"arguments");bool direct=std::string_view(argv[1])=="direct";require(direct?argc==3:argc==5,"mode");
 auto* model=::mj_loadXML(argv[2],nullptr,nullptr,0);require(model,"official humanoid");auto* data=::mj_makeData(model);require(data,"physics state");
 auto actuator=::mj_name2id(model,mjOBJ_ACTUATOR,"abdomen_z"),joint=::mj_name2id(model,mjOBJ_JOINT,"abdomen_z");require(actuator>=0&&joint>=0,"joint");auto q=model->jnt_qposadr[joint],v=model->jnt_dofadr[joint];::mj_forward(model,data);unsigned steps{};
 auto execute=[&](double angle){data->ctrl[actuator]=std::clamp(2*(angle-data->qpos[q])-.1*data->qvel[v],-1.0,1.0);::mj_step(model,data);++steps;};
 if(direct){execute(.5);execute(-.5);execute(.5);}
 else
 {
  std::array<int,2> fds{static_cast<int>(number(argv[3])),static_cast<int>(number(argv[4]))};std::array<unsigned,2> pids{};std::uint64_t until{};unsigned long long expiry{};
  require(std::scanf("%u %u %llu",&pids[0],&pids[1],&expiry)==3&&pids[0]&&pids[1],"provisioned identities");until=expiry;
  d::receiver_control gate;require(gate.configure(identity(0).driver,identity(0).contract,1,2,{120000000000ull,1000000000ull,2,true})==error::success,"control policy");std::array<c::handle,2> routes{};
  for(unsigned i{};i<2;++i)require(gate.bind(identity(i),{pids[i],::getuid(),::getgid()},until,60000000000ull,routes[i])==error::success,"registered session");
  require(gate.freeze()==error::success,"freeze");c::lease ownership;auto time=now();require(gate.grant(routes[0],1,until-time,time,ownership)==error::success&&ownership.epoch==2,"first control");
  std::array<packet,2> messages;for(unsigned i{};i<2;++i){messages[i].receive(fds[i],i,pids[i]);require(messages[i].header.type==p::kind::hello,"handshake");}
  for(unsigned i{};i<2;++i)messages[i].send(fds[i],false,true);
  unsigned denied{};
  for(unsigned step=1;step<=3;++step)
  {
   for(unsigned i{};i<2;++i)messages[i].receive(fds[i],i,pids[i]);
   std::array<double,2> targets{};std::array<std::uint64_t,2> epochs{};for(unsigned i{};i<2;++i)targets[i]=messages[i].command(step,epochs[i]);
   auto rejected=step==1?1u:0u,accepted=1u-rejected;c::handle effect{};
   require(gate.begin(routes[rejected],messages[rejected].peer,messages[rejected].header,messages[rejected].body.size(),epochs[rejected],now(),effect)==error::conflict&&!effect,"competing/stale/forged owner refused");++denied;messages[rejected].send(fds[rejected],false);
   if(step==3){messages[0].receive(fds[0],0,pids[0]);require(messages[0].header.type==p::kind::release&&gate.release(routes[0])==error::success,"old origin release");c::status state;require(gate.inspect(state)==error::success&&!state.latched,"release does not stop new owner");}
   require(gate.begin(routes[accepted],messages[accepted].peer,messages[accepted].header,messages[accepted].body.size(),epochs[accepted],now(),effect)==error::success,"final shared gate");
   execute(targets[accepted]);require(gate.finish(routes[accepted],effect,c::effect_result::executed)==error::success,"actual effect completion");
   if(step==1){time=now();require(gate.grant(routes[1],2,until-time,time,ownership)==error::success&&ownership.epoch==3,"trusted ownership handover");}
   messages[accepted].send(fds[accepted],true);
  }
  messages[1].receive(fds[1],1,pids[1]);require(messages[1].header.type==p::kind::release&&gate.release(routes[1])==error::success,"active owner release");c::status state;require(gate.inspect(state)==error::success&&state.latched&&state.stop&&!state.outcome_unknown&&denied==3,"stop reservation after active release");
  std::fprintf(stderr,"{\"denied\":3,\"handover_epoch\":3,\"stop_reserved\":true}\n");
 }
 std::printf("{\"steps\":%u,\"time\":%.17g,\"position\":%.17g}\n",steps,data->time,data->qpos[q]);::mj_deleteData(data);::mj_deleteModel(model);
}
