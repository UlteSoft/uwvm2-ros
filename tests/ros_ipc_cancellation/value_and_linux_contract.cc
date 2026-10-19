/** Authored NOT RUN. Actual Linux eventfd/socketpair/poll syscalls, not a fake
 * Native ROS provider. Caller owns the entire finite process/resource group.
 * This test NEVER defines the trusted bridge class or bypasses the private
 * scope constructor; real bridge/caller/World tests belong to its source owner.
 */
#include <uwvm2/imported/ros/ipc/unix.h>
#include <type_traits>
#include <cstdio>
#include <array>
#include <vector>
#include <thread>
#include <future>
#if defined(__linux__)
#include <sys/eventfd.h>
#endif
namespace ipc=uwvm2::imported::ros::ipc;
static_assert(!std::is_constructible_v<ipc::cancellation_scope,int>);
static_assert(!std::is_copy_constructible_v<ipc::cancellation_scope>);
static_assert(!std::is_move_constructible_v<ipc::cancellation_scope>);
static unsigned checks{},failures{};
#define CHECK(x) do{++checks;if(!(x)){++failures;std::fprintf(stderr,"FAIL_IPC_CANCELLATION line=%u\n",unsigned(__LINE__));}}while(false)
#if defined(__linux__)
struct owned_fd
{
 int value{-1};explicit owned_fd(int v=-1):value(v){};~owned_fd(){if(value>=0)::close(value);}
 owned_fd(owned_fd const&)=delete;owned_fd&operator=(owned_fd const&)=delete;
};
struct sockets
{
 int values[2]{-1,-1};sockets(){CHECK(::socketpair(AF_UNIX,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,0,values)==0);}
 ~sockets(){for(auto v:values)if(v>=0)::close(v);}
};
// Retain every actual thread on allocation/setup failure too. No detached
// worker can outlive the eventfd/socket owners. The transfer absolute deadline
// still bounds ordinary I/O; real process supervision owns fatal OS failures.
struct joined_thread
{
 std::thread value;
 template<typename Work>explicit joined_thread(Work&& work):value(std::forward<Work>(work)){}
 ~joined_thread(){if(value.joinable())value.join();}
 void join(){if(value.joinable())value.join();}
 joined_thread(joined_thread const&)=delete;joined_thread&operator=(joined_thread const&)=delete;
};
static owned_fd event(){return owned_fd{::eventfd(0,EFD_NONBLOCK|EFD_CLOEXEC)};}
static void stop(owned_fd const& fd){std::uint64_t one=1;CHECK(::write(fd.value,&one,sizeof one)==sizeof one);}
static bool readable(int fd,int ms=0){pollfd p{fd,POLLIN,0};return ::poll(&p,1,ms)>0&&(p.revents&POLLIN);}
static void ordinary_no_scope()
{CHECK(ipc::current_cancellation_fd()==-1);CHECK(ipc::probe_cancellation(-1)==ipc::cancellation_probe::clear);sockets p;std::array<std::uint8_t,3>in{1,2,3},out{};CHECK(ipc::transfer(p.values[0],in.data(),in.size(),true,ipc::after(1000),-1));CHECK(ipc::transfer(p.values[1],out.data(),out.size(),false,ipc::after(1000),-1)&&out==in);}
static void eventfd_is_not_drained_or_closed()
{auto fd=event();CHECK(fd.value>=0);stop(fd);CHECK(ipc::probe_cancellation(fd.value)==ipc::cancellation_probe::ready);CHECK(ipc::cancellation_requested(fd.value));CHECK(readable(fd.value));CHECK(::fcntl(fd.value,F_GETFD)&FD_CLOEXEC);}
static void rejection_sentinel_never_poll_ignored()
{sockets p;std::uint8_t b=7;CHECK(ipc::probe_cancellation(-2)==ipc::cancellation_probe::rejected);CHECK(!ipc::transfer(p.values[0],&b,1,true,ipc::after(1000),-2)&&errno==ECANCELED);CHECK(!readable(p.values[1]));}
static void closed_descriptor_is_refused()
{sockets p;auto fd=event();auto closed=fd.value;CHECK(::close(closed)==0);fd.value=-1;std::uint8_t b=7;CHECK(ipc::probe_cancellation(closed)==ipc::cancellation_probe::rejected);CHECK(!ipc::transfer(p.values[0],&b,1,true,ipc::after(1000),closed)&&errno==ECANCELED);CHECK(!readable(p.values[1]));}
static void zero_extent_still_observes_stop()
{sockets p;auto fd=event();stop(fd);CHECK(!ipc::transfer(p.values[0],nullptr,0,true,ipc::after(1000),fd.value)&&errno==ECANCELED);CHECK(!readable(p.values[1]));}
static void read_wait_is_woken()
{sockets p;auto fd=event();std::promise<void>entered;auto ready=entered.get_future();bool result=true;int error=0;std::uint8_t out=99;joined_thread worker([&]{entered.set_value();result=ipc::transfer(p.values[0],&out,1,false,ipc::after(2000),fd.value);error=errno;});CHECK(ready.wait_for(std::chrono::seconds(1))==std::future_status::ready);stop(fd);worker.join();CHECK(!result&&error==ECANCELED&&out==99);CHECK(readable(fd.value));}
static void one_level_stop_wakes_two_waiters()
{sockets a,b;auto fd=event();std::promise<void>first,second;auto x=first.get_future(),y=second.get_future();bool r1=true,r2=true;int e1=0,e2=0;std::uint8_t o1=9,o2=9;
 joined_thread w1([&]{first.set_value();r1=ipc::transfer(a.values[0],&o1,1,false,ipc::after(2000),fd.value);e1=errno;});
 joined_thread w2([&]{second.set_value();r2=ipc::transfer(b.values[0],&o2,1,false,ipc::after(2000),fd.value);e2=errno;});
 CHECK(x.wait_for(std::chrono::seconds(1))==std::future_status::ready&&y.wait_for(std::chrono::seconds(1))==std::future_status::ready);stop(fd);w1.join();w2.join();CHECK(!r1&&!r2&&e1==ECANCELED&&e2==ECANCELED&&o1==9&&o2==9);CHECK(readable(fd.value));}
static void partial_write_is_not_zero_effect()
{sockets p;int small=1024;CHECK(::setsockopt(p.values[0],SOL_SOCKET,SO_SNDBUF,&small,sizeof small)==0);auto fd=event();std::vector<std::uint8_t>body(2*1024*1024,0x5a);bool result=true;int error=0;
 joined_thread worker([&]{result=ipc::transfer(p.values[0],body.data(),body.size(),true,ipc::after(2000),fd.value);error=errno;});
 // Peer-readable proves a real prefix reached the kernel before stop. The peer
 // does not drain it while the writer runs, so a whole2MiB cannot fit this socket.
 CHECK(readable(p.values[1],1000));stop(fd);worker.join();CHECK(!result&&error==ECANCELED);std::array<std::uint8_t,128>prefix{};auto n=::recv(p.values[1],prefix.data(),prefix.size(),MSG_DONTWAIT);CHECK(n>0&&prefix[0]==0x5a);CHECK(readable(fd.value));}
static void stopped_retry_cannot_send()
{sockets p;auto fd=event();stop(fd);std::uint8_t b=7;for(unsigned i=0;i<2;++i){CHECK(!ipc::transfer(p.values[0],&b,1,true,ipc::after(1000),fd.value)&&errno==ECANCELED);}CHECK(!readable(p.values[1]));}
static void connection_wait_observes_stop()
{sockets p;auto fd=event();stop(fd);CHECK(!ipc::wait_connected(p.values[0],ipc::after(1000),fd.value)&&errno==ECANCELED);CHECK(readable(fd.value));}
static void ordinary_connection_wait_preserved()
{sockets p;CHECK(ipc::wait_connected(p.values[0],ipc::after(1000),-1));}
static void frame_read_stop_before_output()
{sockets p;auto fd=event();stop(fd);ipc::frame f;f.backend=2;f.request_id=77;f.payload={1,2,3};CHECK(!ipc::read_frame(p.values[0],f,ipc::after(1000),fd.value)&&errno==ECANCELED);CHECK(f.request_id==77&&f.payload==std::vector<std::uint8_t>({1,2,3}));}
#endif
int main()
{
#if defined(__linux__)
 ordinary_no_scope();eventfd_is_not_drained_or_closed();rejection_sentinel_never_poll_ignored();closed_descriptor_is_refused();zero_extent_still_observes_stop();read_wait_is_woken();one_level_stop_wakes_two_waiters();partial_write_is_not_zero_effect();stopped_retry_cannot_send();connection_wait_observes_stop();ordinary_connection_wait_preserved();frame_read_stop_before_output();
 std::printf("%s_IPC_NATIVE_CANCELLATION groups=12 checks=%u failures=%u\n",failures?"FAIL":"PASS",checks,failures);return failures?1:0;
#else
 std::fprintf(stderr,"UNSUPPORTED_IPC_NATIVE_CANCELLATION requires actual Linux eventfd/socket/poll\n");return 95;
#endif
}
