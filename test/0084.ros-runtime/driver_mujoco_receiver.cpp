/** @file
 * @brief Test-only external receiver for the canonical driver attachment protocol.
 * @details Loads the pinned official humanoid XML and MuJoCo outside the VM.
 * A fixed single-joint contract validates the complete command, profile digest,
 * lease epoch, monotonic deadline, units, frame and finite range before mj_step.
 * This fixture is not a deployer, vendor adapter, general arbiter or safety
 * certification. Fault injection happens after a real physics effect.
 */
#include <uwvm2/imported/ros/scene/driver_channel.h>
#include <uwvm2/imported/ros/scene/schema_contract.h>
#include <uwvm2/imported/ros/scene/buffer_contract.h>
#include "support/legacy_process/model_sandbox.h"
#include <mujoco/mujoco.h>
#ifdef UWVM_ROS_ATTACHMENT_OPENSSL
#include <uwvm2/imported/ros/scene/attachment_tls.h>
#endif
#include <cmath>
#include <cstdio>
#include <csignal>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
namespace s=uwvm2::imported::ros::scene;namespace p=s::drivers::protocol;
static void require(bool value,char const* message)
{if(!value){std::fprintf(stderr,"fixture refusal: %s\n",message);::_exit(101);}}
static std::uint64_t now(){return static_cast<std::uint64_t>(uwvm2::imported::ros::resources::steady_now());}
static std::uint64_t number(char const* text)
{std::uint64_t value{};auto end=text+std::char_traits<char>::length(text);auto parsed=std::from_chars(text,end,value);require(parsed.ec==std::errc{}&&parsed.ptr==end,"integer");return value;}
static p::binding binding()
{p::binding out{{1,0},{2,0},{3,0},{4,0},{5,0},{}};for(unsigned n{};n!=32;++n)out.contract[n/8]|=std::uint64_t(n+1)<<(8*(n%8));return out;}
static bool profile(s::wire::reader& in,std::string_view expected)
{std::string_view name;s::wire::bytes digest;if(!in.text(name)||name!=expected||!in.blob(digest)||digest.size()!=32)return false;
 auto calculated=s::buffers::hash({reinterpret_cast<std::uint8_t const*>(expected.data()),expected.size()});
 return std::equal(digest.begin(),digest.end(),calculated.begin(),calculated.end());}
// Test-only specialization: preserve the production allowlist and replace its
// single receive descriptor with the externally provisioned fixture endpoint.
static void filter_for(int fd,bool final_filter)
{
 if(final_filter){s::model_sandbox::syscall_filter final{true};require(final.install(),"final filter");return;}
 s::model_sandbox::syscall_filter filter{final_filter};auto source=filter.view();std::array<sock_filter,768> code{};
 require(!source.empty()&&source.size()<=code.size(),"filter bound");std::copy(source.begin(),source.end(),code.begin());unsigned replacements{};
 for(std::size_t i{};i+4<source.size();++i)
  if(code[i].code==(BPF_JMP|BPF_JEQ|BPF_K)&&code[i].k==SYS_recvmsg&&code[i].jf==4&&
     code[i+1].code==(BPF_LD|BPF_W|BPF_ABS)&&code[i+1].k==offsetof(seccomp_data,args)&&
     code[i+2].code==(BPF_JMP|BPF_JEQ|BPF_K)&&code[i+2].k==3)
  {code[i+2].k=static_cast<unsigned>(fd);++replacements;}
 require(replacements==1,"exact receive filter");
 auto size=source.size()*sizeof(sock_filter);require(::write(1,code.data(),size)==static_cast<ssize_t>(size),"filter output");
}
// This limit belongs to the external test receiver. The deployment fixture
// sets the same bound on the translator before starting it.
#ifndef UWVM_MUJOCO_COMPILED_MODEL_MAX_BYTES
#define UWVM_MUJOCO_COMPILED_MODEL_MAX_BYTES (64 << 20)
#endif
static constexpr auto max_compiled_model_bytes = UWVM_MUJOCO_COMPILED_MODEL_MAX_BYTES;
static_assert((1 << 20) <= max_compiled_model_bytes && max_compiled_model_bytes <= (512 << 20));
static mjModel* load_model(char const* path, bool compiled)
{
 if(compiled)
 {
  auto fd=::open(path,O_RDONLY|O_CLOEXEC|O_NOFOLLOW);
  require(fd>=0,"compiled model file");
  struct stat info{};
  require(::fstat(fd,&info)==0 && S_ISREG(info.st_mode) && info.st_size>0 && info.st_size<=max_compiled_model_bytes,"compiled model size");
  auto size=static_cast<std::size_t>(info.st_size);
  auto* bytes=::mmap(nullptr,size,PROT_READ,MAP_PRIVATE,fd,0);
  ::close(fd);
  require(bytes!=MAP_FAILED,"compiled model mapping");
  mjVFS files;
  ::mj_defaultVFS(&files);
  require(!::mj_addBufferVFS(&files,"model.mjb",bytes,static_cast<int>(size)),"compiled model VFS");
  ::munmap(bytes,size);
  auto* model=::mj_loadModel("model.mjb",&files);
  ::mj_deleteVFS(&files);
  return model;
 }
 // Read only the externally sealed model; no generic filesystem resolution.
 std::array<char,65536> xml{};auto file=::open(path,O_RDONLY|O_CLOEXEC);require(file>=0,"official model file");std::size_t extent{};
 for(;;){auto n=::read(file,xml.data()+extent,xml.size()-extent-1);require(n>=0,"model read");if(!n)break;extent+=n;require(extent<xml.size()-1,"model bound");}::close(file);
 mjVFS files;::mj_defaultVFS(&files);require(!::mj_addBufferVFS(&files,"model.xml",xml.data(),extent),"model VFS");
 auto* model=::mj_loadXML("model.xml",&files,nullptr,0);::mj_deleteVFS(&files);return model;
}
int main(int argc,char** argv)
{
 if(argc==3&&std::string_view(argv[1])=="--filter"){filter_for(static_cast<int>(number(argv[2])),false);return 0;}
 bool const joint_effort=argc>=8 && std::string_view{argv[argc-3]}=="--urdf-effort";
 bool const compiled=joint_effort || (argc>=8 && std::string_view{argv[argc-3]}=="--compiled-joint");
 auto const* joint_name=compiled?argv[argc-2]:"abdomen_z";
 auto const* actuator_name=compiled?argv[argc-1]:"abdomen_z";
 if(compiled)argc-=3;
 require(argc==5||argc==6||argc==7,"arguments");bool confined=argc==6;require(!confined||std::string_view(argv[5])=="confined","confinement selector");
 int fd=static_cast<int>(number(argv[1]));std::string_view mode{argv[2]};auto count=number(argv[4]);
 auto* model=load_model(argv[3],compiled);
 require(model,"official model");auto* data=::mj_makeData(model);require(data,"simulation data");
 auto joint=::mj_name2id(model,mjOBJ_JOINT,joint_name);
 require(joint>=0&&model->jnt_type[joint]==mjJNT_HINGE,"fixed joint");
 int actuator=-1;
 if(joint_effort)
 {
  require(!*actuator_name && model->nu==0,"expected an unactuated URDF model");
 }
 else if(*actuator_name)
 {
  actuator=::mj_name2id(model,mjOBJ_ACTUATOR,actuator_name);
 }
 else
 {
  for(int i{};i<model->nu;++i)
  {
   if(model->actuator_trntype[i]!=mjTRN_JOINT || model->actuator_trnid[2*i]!=joint)continue;
   require(actuator<0,"ambiguous actuator for the selected joint");
   actuator=i;
  }
 }
 if(!joint_effort)require(actuator>=0&&model->actuator_trntype[actuator]==mjTRN_JOINT&&model->actuator_trnid[2*actuator]==joint,"actuator must drive the selected joint");
 auto q=model->jnt_qposadr[joint],v=model->jnt_dofadr[joint];::mj_forward(model,data);
 bool const passive=joint_effort && mode=="passive";
 auto execute=[&](double angle)
 {
  auto const command=std::clamp(2.0*(angle-data->qpos[q])-.1*data->qvel[v],-1.0,1.0);
  if(joint_effort)data->qfrc_applied[v]=passive?0.0:command;
  else data->ctrl[actuator]=command;
  ::mj_step(model,data);
  require(std::isfinite(data->qpos[q])&&std::isfinite(data->time),"finite state");
 };
 if(mode=="direct" || passive)
 {for(std::uint64_t i=1;i<=count;++i)execute(i%2?.5:-.5);std::printf("{\"steps\":%llu,\"time\":%.17g,\"position\":%.17g}\n",(unsigned long long)count,data->time,data->qpos[q]);::mj_deleteData(data);::mj_deleteModel(model);return 0;}
 require(mode=="success"||mode=="range"||mode=="crash","fault mode");
 auto* crash_page=::mmap(nullptr,4096,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);require(crash_page!=MAP_FAILED,"crash page");
 if(!confined){rlimit core{0,0};require(!::setrlimit(RLIMIT_CORE,&core),"core limit");}
 if(confined)
 {
  rlimit file_limit{0,0};require(!::prlimit(0,RLIMIT_FSIZE,&file_limit,nullptr),"final file limit");
  filter_for(fd,true);
  require(::socket(AF_INET,SOCK_STREAM,0)==-1&&errno==ENOSYS,"network creation denied");
  require(::ptrace(PTRACE_TRACEME,0,nullptr,nullptr)==-1&&errno==ENOSYS,"ptrace denied");
  require(::syscall(SYS_clone,0,nullptr,nullptr,nullptr,0)==-1&&errno==ENOSYS,"clone denied");
  require(::open("/escape",O_WRONLY|O_CREAT,0600)==-1&&errno==ENOSYS,"file creation denied");
  require(::open("/etc/passwd",O_RDONLY)==-1&&errno==ENOENT,"host files absent");
  require(::mmap(nullptr,4096,PROT_READ|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,-1,0)==MAP_FAILED&&errno==ENOSYS,"executable memory denied");
  rlimit denied{0,0};require(::prlimit(0,RLIMIT_AS,&denied,nullptr)==-1&&errno==ENOSYS,"resource changes denied");
  std::fprintf(stderr,"{\"kind\":\"confined\",\"escape_checks\":7}\n");std::fflush(stderr);
 }
 unsigned expected_pid{};require(std::scanf("%u",&expected_pid)==1&&expected_pid,"provisioned client PID");
#ifdef UWVM_ROS_ATTACHMENT_OPENSSL
 require(argc==7&&std::string_view(argv[5])=="--identity-fd","encrypted driver registration");
 auto key_fd=static_cast<int>(number(argv[6]));
 s::attachment_tls::channel encrypted;
 auto b=binding();
 require(encrypted.bind(fd,{expected_pid,::getuid(),::getgid()},key_fd,s::attachment_tls::purpose::driver,
                        b.deployment,b.vm,b.module,b.driver,b.session,b.contract,p::header_bytes+4096,true)==s::attachment_tls::error::success,"secure driver peer");
 ::close(key_fd);
#else
 require(argc!=7,"TLS unavailable in the historical namespace fixture");
#endif
 std::array<std::uint8_t,8192> bytes{};std::uint64_t last{},steps{};bool ready{};
 for(;;)
 {
#ifdef UWVM_ROS_ATTACHMENT_OPENSSL
  std::size_t received{};
  auto read_status=encrypted.receive(bytes,received);
  if(read_status==s::attachment_tls::error::again)continue;
  require(read_status==s::attachment_tls::error::success,"authenticated request");
#else
  alignas(cmsghdr) std::array<unsigned char,CMSG_SPACE(sizeof(ucred))> ancillary{};iovec part{bytes.data(),bytes.size()};msghdr message{};
  message.msg_iov=&part;message.msg_iovlen=1;message.msg_control=ancillary.data();message.msg_controllen=ancillary.size();
  auto received=::recvmsg(fd,&message,MSG_TRUNC);require(received>0&&!(message.msg_flags&(MSG_TRUNC|MSG_CTRUNC)),"packet extent");
  auto* credential=CMSG_FIRSTHDR(&message);require(credential&&credential->cmsg_level==SOL_SOCKET&&credential->cmsg_type==SCM_CREDENTIALS&&credential->cmsg_len==CMSG_LEN(sizeof(ucred))&&!CMSG_NXTHDR(&message,credential),"credentials");
  ucred peer{};std::memcpy(&peer,CMSG_DATA(credential),sizeof(peer));require(peer.pid>0&&static_cast<unsigned>(peer.pid)==expected_pid&&peer.uid==::getuid()&&peer.gid==::getgid(),"exact origin");
#endif
  p::header h;s::wire::bytes body;require(p::decode({bytes.data(),static_cast<std::size_t>(received)},h,body)&&h.bound==binding(),"fixed contract");
  std::array<std::uint8_t,512> answer{};s::wire::writer out{answer};
  if(h.type==p::kind::hello){require(!ready,"one handshake");ready=true;h.type=p::kind::ready;}
  else if(h.type==p::kind::release){require(ready,"release");break;}
  else
  {
   require(ready&&h.type==p::kind::request&&h.sequence==last+1&&h.deadline>now(),"sequence and deadline");last=h.sequence;
   require(s::schema_contract::validate(body,s::schema_contract::type::device_submit__request,{8192,128,128,16}),"canonical command");
   s::wire::reader in{body};std::uint64_t session{},sequence{},device{},capability{},controller{},epoch{},observations{},dependencies{},deadline{};
   std::string_view type,encoding;s::wire::bytes payload;
   require(in.u64(session)&&session==1&&in.u64(sequence)&&sequence==h.sequence&&in.u64(device)&&device==1&&in.u64(capability)&&capability==1&&in.u64(controller)&&controller==1&&in.u64(epoch)&&epoch==1,"controller fencing");
   require(profile(in,"simulation.position")&&profile(in,"simulation.radian.logical")&&in.text(type)&&type=="simulation.joint"&&in.text(encoding)&&encoding=="le64"&&in.blob(payload)&&payload.size()==8&&in.u64(observations)&&!observations&&in.u64(dependencies)&&!dependencies&&in.u64(deadline)&&deadline==h.deadline&&in.done(),"fixed semantic shape");
   s::wire::reader value{payload};std::uint64_t bits{};require(value.u64(bits),"scalar");double angle=std::bit_cast<double>(bits);bool admitted=std::isfinite(angle)&&angle>=-1&&angle<=1;
   if(admitted){require(mode!="range"&&++steps<=count,"exact effects");execute(angle);}
   else require(mode=="range"&&steps==0,"physical range refusal");
   std::fprintf(stderr,"{\"sequence\":%llu,\"steps\":%llu,\"admitted\":%s}\n",(unsigned long long)sequence,(unsigned long long)steps,admitted?"true":"false");std::fflush(stderr);
   if(mode=="crash")*static_cast<volatile unsigned char*>(crash_page)=1;
   out.u64(sequence);out.text(admitted?"succeeded":"failed");out.text(admitted?"observed_remote":"none");out.u64(1);out.text(admitted?"simulation_step":"physical_range");
   require(s::schema_contract::validate(out.written(),s::schema_contract::type::device_submit__result,{512,128,128,16}),"canonical receipt");h.type=p::kind::result;h.status=admitted?0:13;
  }
  require(p::encode(h,out.size(),std::span{bytes}.first(p::header_bytes)),"response header");std::copy(out.written().begin(),out.written().end(),bytes.begin()+p::header_bytes);
#ifdef UWVM_ROS_ATTACHMENT_OPENSSL
  s::attachment_tls::error sent{};
  do{sent=encrypted.send(std::span{bytes}.first(p::header_bytes),out.written());}while(sent==s::attachment_tls::error::again);
  require(sent==s::attachment_tls::error::success,"authenticated reply");
#else
  require(::write(fd,bytes.data(),p::header_bytes+out.size())==static_cast<ssize_t>(p::header_bytes+out.size()),"response");
#endif
 }
 require(steps==(mode=="range"?0:count),"final effect count");
 std::printf("{\"steps\":%llu,\"time\":%.17g,\"position\":%.17g,\"mujoco\":%d}\n",(unsigned long long)steps,data->time,data->qpos[q],::mj_version());
 ::mj_deleteData(data);::mj_deleteModel(model);::close(fd);return 0;
}
