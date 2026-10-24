/* SPDX-License-Identifier: Apache-2.0 */
/** Actual production time/descriptor helpers; no fake ROS headers or entities.
 * These13 authored groups (14 in the forced feature0 build) have NOT been compiled or executed. */
#include "src/ros/gateway/ros2/native_rcl_graph_wait.h"
#include <fast_io.h>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <thread>
namespace g=uwvm2::ros_gateway::native_rcl_graph_wait;
static unsigned groups{};
static void need(bool ok){if(!ok)std::abort();}
struct resources
{
    int sockets[2]{-1,-1};int worker{-1};std::atomic<std::sig_atomic_t> stop{0};
    resources(){need(::socketpair(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0,sockets)==0);worker=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));need(worker>=0);}
    ~resources(){for(auto fd:sockets)if(fd>=0)::close(fd);if(worker>=0)::close(worker);}
    g::lifetime life()const noexcept{return {sockets[0],worker,&stop};}
};
int main()
{
    auto run=[](auto f){f();++groups;};using namespace std::chrono;
    run([]{g::lifetime absent;need(!absent.valid());need(g::transport_check(absent,g::clock::now()+seconds{1})==95);});
    run([]{auto now=g::clock::now();need(g::slice(now,now+milliseconds{50},now+seconds{1})==milliseconds{50});});
    run([]{auto now=g::clock::now();need(g::slice(now,now+seconds{1},now+milliseconds{20})==milliseconds{20});});
    run([]{auto now=g::clock::now();need(g::slice(now,now+seconds{60},now+seconds{61})==milliseconds{100});});
    run([]{auto now=g::clock::now();need(g::slice(now,now,now+seconds{1}).count()==0);need(g::slice(now,now+seconds{1},now).count()==0);});
    run([]{resources r;need(g::transport_check(r.life(),g::clock::now()+seconds{1})==0);});
    run([]{resources r;char byte='G',kept{};need(::send(r.sockets[1],&byte,1,MSG_NOSIGNAL)==1);
        need(g::transport_check(r.life(),g::clock::now()+seconds{1})==0);need(::recv(r.sockets[0],&kept,1,MSG_PEEK)==1&&kept=='G');});
    run([]{resources r;::close(r.sockets[1]);r.sockets[1]=-1;need(g::transport_check(r.life(),g::clock::now()+seconds{1})==107);});
    run([]{resources r;auto life=r.life();::close(r.worker);r.worker=-1;need(g::transport_check(life,g::clock::now()+seconds{1})==107);});
    run([]{resources r;r.stop.store(1,std::memory_order_relaxed);need(g::transport_check(r.life(),g::clock::now()+seconds{1})==125);});
    run([]{resources r;need(g::transport_check(r.life(),g::clock::now())==110);});
    run([]{resources r;auto life=r.life();::close(r.sockets[0]);r.sockets[0]=-1;need(g::transport_check(life,g::clock::now()+seconds{1})==107);});
    run([]{resources r;std::thread actual{[&]{r.stop.store(1,std::memory_order_relaxed);}};actual.join();
        need(r.stop.load(std::memory_order_relaxed)==1&&g::transport_check(r.life(),g::clock::now()+seconds{1})==125);});
#if defined(UWVM_GRAPH_WAIT_EXPECT_DISABLED)
    static_assert(UWVM_NATIVE_RCL_FINITE_GRAPH_WAIT_API==0);
    run([]{need(g::check({},rclcpp::Node::SharedPtr{},g::clock::now()+seconds{1})==95);});
    need(groups==14);
#else
    need(groups==13);
#endif
    fast_io::io::println("ACTUAL_GRAPH_WAIT_VALUE groups=",groups," Native_ROS=0");
}
