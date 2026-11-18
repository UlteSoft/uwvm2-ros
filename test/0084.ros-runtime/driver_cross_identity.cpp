/** @file @brief Full-kernel tests for provisioned IPC across unprivileged identities.
 * @details Run only as root inside the disposable, bounded QEMU fixture. Root is
 * the test provisioner; endpoint code runs with distinct non-root UID/GID pairs.
 * No root or namespace-creation privilege enters either IPC participant.
 */
#include <uwvm2/imported/ros/scene/attachment_tls.h>
#include <cassert>
#include <cstdio>
#include <grp.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <openssl/rand.h>
namespace s=uwvm2::imported::ros::scene;
namespace d=s::drivers;
using E=d::error;
static constexpr uid_t client_uid=21001,server_uid=21002;
static void wait_child(pid_t child){int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);}
static void drop(uid_t uid)
{
 assert(::setgroups(0,nullptr)==0&&::setresgid(uid,uid,uid)==0&&::setresuid(uid,uid,uid)==0);
 assert(::prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0)==0&&::prctl(PR_SET_DUMPABLE,0,0,0,0)==0);
}
static void pipe_pair(int (&fds)[2]){assert(::pipe2(fds,O_CLOEXEC)==0);}
static void socket_pair(int (&fds)[2])
{
 assert(::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,fds)==0);
 for(int fd:fds){int yes=1;assert(::setsockopt(fd,SOL_SOCKET,SO_PASSCRED,&yes,sizeof(yes))==0);}
}
static void clock_case(char const* name,int proof,bool accepted,bool wrong_uid=false)
{
 int endpoints[2],hold[2];socket_pair(endpoints);pipe_pair(hold);
 auto peer=::fork();assert(peer>=0);
 if(!peer){::close(endpoints[0]);::close(hold[1]);drop(server_uid);
  assert(::send(endpoints[1],"checked",7,MSG_NOSIGNAL)==7);char byte;assert(::read(hold[0],&byte,1)==1);::_exit(0);}
 ::close(endpoints[1]);::close(hold[0]);
 auto client=::fork();assert(client>=0);
 if(!client)
 {
  ::close(hold[1]);drop(client_uid);d::channel channel;
#if defined(WROS_CLOCK_BASELINE)
  (void)proof;auto result=channel.bind(endpoints[0],{static_cast<std::uint64_t>(peer),server_uid,server_uid});
#else
  auto result=channel.bind(endpoints[0],{static_cast<std::uint64_t>(peer),wrong_uid?client_uid:server_uid,server_uid},proof);
#endif
  if(!accepted){assert(result==E::denied);::_exit(0);}
  assert(result==E::success);::close(endpoints[0]);if(proof>=0)::close(proof);
  std::array<std::uint8_t,32> data{};std::size_t size{};
  do{result=channel.receive(data,size);}while(result==E::again);
  assert(wrong_uid?result==E::denied:result==E::success&&size==7&&std::memcmp(data.data(),"checked",7)==0);
  ::_exit(0);
 }
 ::close(endpoints[0]);wait_child(client);assert(::write(hold[1],"x",1)==1);::close(hold[1]);wait_child(peer);
 std::printf("CASE %s PASS\n",name);std::fflush(stdout);
}
static void kernel_tests()
{
 clock_case("cross-uid-without-clock-evidence-denied",-1,false);
#if !defined(WROS_CLOCK_BASELINE)
 int clock=::open("/proc/self/ns/time",O_RDONLY|O_CLOEXEC);assert(clock>=0);
 clock_case("cross-uid-clock-evidence-accepted",clock,true);
 clock_case("clock-evidence-cannot-bypass-packet-identity",clock,true,true);
 ::close(clock);
 int net=::open("/proc/self/ns/net",O_RDONLY|O_CLOEXEC);assert(net>=0);
 clock_case("wrong-namespace-type-denied",net,false);::close(net);
 int file=::open("/dev/null",O_RDONLY|O_CLOEXEC);assert(file>=0);
 clock_case("ordinary-file-evidence-denied",file,false);::close(file);
 clock_case("closed-evidence-denied",99999,false);
 auto isolated=::fork();assert(isolated>=0);
 if(!isolated)
 {
  assert(::unshare(CLONE_NEWTIME)==0);
  int other=::open("/proc/self/ns/time_for_children",O_RDONLY|O_CLOEXEC);assert(other>=0);
  // Keep the client in the original namespace; hand it a real, different clock.
  int own=::open("/proc/self/ns/time",O_RDONLY|O_CLOEXEC);assert(own>=0);
  assert(::setns(own,CLONE_NEWTIME)==0);::close(own);
  clock_case("different-time-namespace-denied",other,false);::close(other);::_exit(0);
 }
 wait_child(isolated);
#endif
}
#if !defined(WROS_CLOCK_BASELINE)
static d::protocol::binding binding()
{d::protocol::binding b{{1,0},{2,0},{3,0},{4,0},{5,0},{}};for(unsigned n{};n<32;++n)b.contract[n/8]|=std::uint64_t(n+1)<<(8*(n%8));return b;}
static int identity(std::array<unsigned char,32> const& seed,std::array<unsigned char,32> const& peer)
{
 int fd=static_cast<int>(::syscall(SYS_memfd_create,"wros-test-identity",MFD_CLOEXEC|MFD_ALLOW_SEALING));assert(fd>=0);
 assert(::write(fd,seed.data(),32)==32&&::write(fd,peer.data(),32)==32);
 assert(::fcntl(fd,F_ADD_SEALS,F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL)==0);return fd;
}
static std::array<int,2> keys()
{
 std::array<std::array<unsigned char,32>,2> seed{},public_key{};
 for(unsigned n{};n<2;++n)
 {
  assert(RAND_priv_bytes(seed[n].data(),32)==1);
  EVP_PKEY* key=EVP_PKEY_new_raw_private_key_ex(nullptr,"ED25519",nullptr,seed[n].data(),32);assert(key);
  std::size_t size=32;assert(EVP_PKEY_get_raw_public_key(key,public_key[n].data(),&size)==1&&size==32);EVP_PKEY_free(key);
 }
 std::array<int,2> result{identity(seed[0],public_key[1]),identity(seed[1],public_key[0])};
 OPENSSL_cleanse(seed.data(),sizeof(seed));return result;
}
static void serve(int endpoint,int key,int clock,int pid_pipe)
{
 pid_t pid{};assert(::read(pid_pipe,&pid,sizeof(pid))==sizeof(pid));::close(pid_pipe);
 drop(server_uid);auto b=binding();s::attachment_tls::channel channel;
 assert(channel.bind(endpoint,{static_cast<std::uint64_t>(pid),client_uid,client_uid},key,s::attachment_tls::purpose::driver,
  b.deployment,b.vm,b.module,b.driver,b.session,b.contract,208,true,clock)==E::success);
 ::close(endpoint);::close(key);::close(clock);
 std::array<std::uint8_t,208> input{};std::array<std::uint8_t,24> body{};
 s::wire::writer out{body};out.boolean(true);out.boolean(false);out.u64(3);unsigned calls{};
 for(;;)
 {
  std::size_t size{};E result;do{result=channel.receive(input,size);}while(result==E::again);
  assert(result==E::success);d::protocol::header h;s::wire::bytes data;
  assert(d::protocol::decode(std::span{input}.first(size),h,data)&&h.bound==b);
  if(h.type==d::protocol::kind::release){assert(calls==1);return;}
  std::size_t length{};
  if(h.type==d::protocol::kind::hello)h.type=d::protocol::kind::ready;
  else{assert(h.type==d::protocol::kind::request&&h.sequence==1&&data.size()==17&&calls++==0);h.type=d::protocol::kind::result;length=body.size();}
  std::array<std::uint8_t,d::protocol::header_bytes> head{};assert(d::protocol::encode(h,length,head));
  do{result=channel.send(head,std::span{body}.first(length));}while(result==E::again);
  assert(result==E::success);
 }
}
static void vm_test(char const* runtime,char const* wasm)
{
 int endpoint[2],pids[2];socket_pair(endpoint);pipe_pair(pids);auto secret=keys();
 int clock=::open("/proc/self/ns/time",O_RDONLY|O_CLOEXEC);assert(clock>=0);
 pid_t server=::fork();assert(server>=0);
 if(!server){::close(endpoint[0]);::close(secret[0]);::close(pids[1]);serve(endpoint[1],secret[1],clock,pids[0]);::_exit(0);}
 ::close(endpoint[1]);::close(secret[1]);::close(pids[0]);
 // The trusted test provisioner verifies the actual peer clock while root.
 std::array<char,64> path{};std::snprintf(path.data(),path.size(),"/proc/%d/ns/time",server);
 struct stat peer_clock{},own_clock{};assert(::stat(path.data(),&peer_clock)==0&&::fstat(clock,&own_clock)==0);
 assert(peer_clock.st_ino==own_clock.st_ino&&peer_clock.st_dev==own_clock.st_dev);
 pid_t client=::fork();assert(client>=0);
 if(!client)
 {
  ::close(pids[1]);drop(client_uid);
  for(int fd:{endpoint[0],secret[0],clock})assert(::fcntl(fd,F_SETFD,0)==0);
  std::array<char,32> socket_text{},key_text{},pid_text{},clock_text{};
  std::snprintf(socket_text.data(),socket_text.size(),"%d",endpoint[0]);std::snprintf(key_text.data(),key_text.size(),"%d",secret[0]);
  std::snprintf(pid_text.data(),pid_text.size(),"%d",server);std::snprintf(clock_text.data(),clock_text.size(),"%d",clock);
  char const* args[]={runtime,"--wasm-set-main-module-name","main","--runtime-int","--ros-scene-create","robot","--ros-scene-bind","robot","main",
   "--ros-scene-interface","robot","capability.open","allow","--ros-scene-interface","robot","driver.connect","allow",
   "--ros-scene-interface","robot","driver.submit","allow","--ros-scene-interface","robot","driver.result","allow",
   "--ros-scene-interface","robot","driver.describe","allow","--ros-scene-interface","robot","driver.disconnect","allow",
   "--ros-scene-grant","robot","arm-query","driver","arm-parameters","registered-driver","driver.connect,driver.submit,driver.result,driver.describe,driver.disconnect",
   "--ros-scene-driver-budget","robot","1","1048576","--ros-scene-driver-channel","robot","arm-query","main",socket_text.data(),pid_text.data(),"21002","21002",
   "01000000000000000000000000000000","02000000000000000000000000000000","03000000000000000000000000000000",
   "04000000000000000000000000000000","05000000000000000000000000000000","0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f20",
   "32","32","60000000000","900000000000",key_text.data(),clock_text.data(),"--ros-scene-driver-schema","robot","arm-query","parameter.exists","64","64","8",
   "--run",wasm,nullptr};
  ::execv(runtime,const_cast<char* const*>(args));assert(false);
 }
 assert(::write(pids[1],&client,sizeof(client))==sizeof(client));::close(pids[1]);::close(endpoint[0]);::close(secret[0]);::close(clock);
 wait_child(client);wait_child(server);std::puts("CASE actual-vm-distinct-uid-mutual-tls PASS");
}
#endif
int main(int argc,char** argv)
{
 assert(::geteuid()==0);::alarm(30);
#if defined(WROS_CLOCK_BASELINE)
 (void)argc;(void)argv;kernel_tests();
#else
 if(argc==1)kernel_tests();else{assert(argc==3);vm_test(argv[1],argv[2]);}
#endif
}
