/** @file @brief Exercise real kernel descriptor handoffs against the adapter receiver.
 * @details No native driver or physical device is accessed. All deliberately
 * malformed SCM_RIGHTS deliveries must leave both output and FD counts intact.
 */
#include <uwvm2/imported/ros/scene/provider_bootstrap_receiver.h>
#include <cassert>
#include <cstdio>
#include <dirent.h>
#include <sys/mman.h>
#include <sys/wait.h>
namespace p=uwvm2::imported::ros::scene::provider_bootstrap;
namespace s=uwvm2::imported::ros::scene;
struct fd
{
 int value;
 explicit fd(int n):value(n){assert(n>=0);}
 ~fd(){if(value>=0)::close(value);}
 fd(fd const&)=delete;
};
static unsigned descriptors()
{
 auto* directory=::opendir("/proc/self/fd");assert(directory);unsigned count{};
 while(auto* entry=::readdir(directory))if(entry->d_name[0]!='.')++count;
 ::closedir(directory);return count;
}
static std::uint64_t now()
{timespec t{};assert(!::clock_gettime(CLOCK_MONOTONIC,&t));return std::uint64_t(t.tv_sec)*1000000000+t.tv_nsec;}
static void pair(int (&values)[2],bool pass=true,int type=SOCK_SEQPACKET)
{
 assert(!::socketpair(AF_UNIX,type|SOCK_CLOEXEC|SOCK_NONBLOCK,0,values));int yes=1;
 if(pass)for(auto value:values)assert(!::setsockopt(value,SOL_SOCKET,SO_PASSCRED,&yes,sizeof(yes)));
}
static int sealed(std::span<std::uint8_t const> bytes,bool seals=true)
{
 int value=::memfd_create("bootstrap-receiver-test",MFD_CLOEXEC|MFD_ALLOW_SEALING);assert(value>=0);
 assert(::write(value,bytes.data(),bytes.size())==static_cast<ssize_t>(bytes.size()));
 if(seals)assert(!::fcntl(value,F_ADD_SEALS,F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL));
 return value;
}
static void send(int courier,std::span<int const> files,std::string_view label)
{
 alignas(cmsghdr) std::array<unsigned char,2048> control{};
 iovec vector{const_cast<char*>(label.data()),label.size()};msghdr msg{};
 msg.msg_iov=&vector;msg.msg_iovlen=1;msg.msg_control=control.data();msg.msg_controllen=CMSG_SPACE(files.size_bytes());
 assert(msg.msg_controllen<=control.size());
 auto* c=CMSG_FIRSTHDR(&msg);c->cmsg_level=SOL_SOCKET;c->cmsg_type=SCM_RIGHTS;c->cmsg_len=CMSG_LEN(files.size_bytes());
 std::memcpy(CMSG_DATA(c),files.data(),files.size_bytes());
 assert(::sendmsg(courier,&msg,MSG_DONTWAIT|MSG_NOSIGNAL)==static_cast<ssize_t>(label.size()));
}
static void check(std::string_view mode)
{
 std::printf("BEGIN %.*s\n",static_cast<int>(mode.size()),mode.data());std::fflush(stdout);
 int raw[2];pair(raw,mode!="no-credentials",mode=="stream"?SOCK_STREAM:SOCK_SEQPACKET);
 auto parent=::getpid();auto child=::fork();assert(child>=0);
 bool early=mode=="no-credentials"||mode=="stream"||mode=="zero-timeout"||mode=="large-timeout"||mode=="invalid-pid";
 if(!child)
 {
  ::close(raw[0]);fd channel{raw[1]};p::descriptors objects;p::record record;record.local={91,92,93};auto original=record;
  auto count=descriptors();p::credentials expected{static_cast<std::uint64_t>(parent),::getuid(),::getgid()};
  if(mode=="wrong-credentials")++expected.uid;
  if(mode=="invalid-pid")expected.pid=0;
  auto timeout=mode=="zero-timeout"?0:mode=="large-timeout"?UINT64_C(5000000001):mode=="timeout"?UINT64_C(10000000):UINT64_C(2000000000);
  auto result=p::receive(channel.value,expected,timeout,objects,record);
  if(mode=="success")
  {
   assert(result==p::receive_status::success&&!objects.empty()&&record.local.pid==static_cast<std::uint64_t>(::getpid()));
   for(int value:{objects.carrier(),objects.identity(),objects.clock(),objects.manifest()})assert(::fcntl(value,F_GETFD)&FD_CLOEXEC);
   assert(descriptors()==count+4);
   auto saved=record;assert(p::receive(channel.value,expected,timeout,objects,record)==p::receive_status::invalid&&record==saved);
  }
  else assert(result==(early?p::receive_status::invalid:mode=="timeout"?p::receive_status::expired:p::receive_status::denied)&&objects.empty()&&record==original);
  objects.reset();auto remaining=descriptors();
  if(remaining!=count)std::fprintf(stderr,"descriptor count before=%u after=%u\n",count,remaining);
  assert(remaining==count);::_exit(0);
 }
 ::close(raw[1]);fd courier{raw[0]};
 if(!early)
 {
  pollfd wait{courier.value,POLLIN,0};assert(::poll(&wait,1,1000)==1);
  std::array<char,32> ready{};assert(::recv(courier.value,ready.data(),ready.size(),0)==10&&std::string_view(ready.data(),10)=="WROS-READY");
  if(mode=="timeout")::usleep(30000);
  else if(mode=="closed-courier"){::close(courier.value);courier.value=-1;}
  else
  {
   int application[2];pair(application);fd a{application[0]},b{application[1]};
   std::array<std::uint8_t,64> identity{};fd key{sealed(identity)},clock{::open("/proc/self/ns/time",O_RDONLY|O_CLOEXEC)};
   struct stat channel{},time{};assert(!::fstat(a.value,&channel)&&!::fstat(clock.value,&time));
   std::array<std::uint8_t,256> data{};s::wire::writer out{data};
   for(auto word:std::array<std::uint64_t,32>{p::magic,256,1,1,static_cast<std::uint64_t>(child),::getuid(),::getgid(),
       static_cast<std::uint64_t>(parent),::getuid(),::getgid(),1,0,2,0,3,0,4,0,5,0,1,2,3,4,32,32,1000,
       mode=="expired-record"?1:now()+UINT64_C(2000000000),channel.st_dev,channel.st_ino,time.st_dev,time.st_ino})out.u64(word);
   if(mode=="malformed-record")data[0]^=0x80;
   fd record{sealed(data,mode!="unsealed-record")};
   std::array<int,253> files;files.fill(key.value);files[0]=a.value;files[1]=key.value;files[2]=clock.value;files[3]=record.value;
   if(mode=="substituted-carrier")files[0]=b.value;
   if(mode=="substituted-clock")files[2]=key.value;
   auto size=mode=="extra-descriptors"?253:mode=="missing-descriptors"?3:4;
   auto label=mode=="bad-label"?"BAD-TAGS":mode=="oversized-packet"?"WROS-FDSXXXXXXXXXXXXXXXXXXXXXXXX":"WROS-FDS";
   if(mode=="wrong-sender")
   {
    auto sender=::fork();assert(sender>=0);
    if(!sender){send(courier.value,std::span{files}.first(size),label);::_exit(0);}
    int status{};assert(::waitpid(sender,&status,0)==sender&&status==0);
   }
   else send(courier.value,std::span{files}.first(size),label);
  }
 }
 int status{};assert(::waitpid(child,&status,0)==child&&status==0);std::printf("CASE %.*s PASS\n",static_cast<int>(mode.size()),mode.data());
}
int main()
{
 ::alarm(30);
 for(auto mode:{"success","wrong-credentials","wrong-sender","extra-descriptors","missing-descriptors","unsealed-record",
     "malformed-record","substituted-carrier","substituted-clock","expired-record","bad-label","oversized-packet",
     "timeout","closed-courier","no-credentials","stream","zero-timeout","large-timeout","invalid-pid"})check(mode);
}
