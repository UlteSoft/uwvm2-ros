/** @file @brief Real two-thread isolated communication fixture, not a ROS middleware claim. */
#include <uwvm2/imported/ros/scene/communication_worker_backend.h>
#include <array>
#include <atomic>
#include <cerrno>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <unistd.h>
namespace w=uwvm2::imported::ros::scene::communication_worker;
namespace wire=uwvm2::imported::ros::scene::wire;
namespace
{
    struct context
    {
        pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;pthread_cond_t gate=PTHREAD_COND_INITIALIZER;
        std::array<pthread_t,2> threads{};std::array<w::role_fd,4> pipes{};
        std::atomic<bool> stop{};std::atomic<unsigned> completed{},thread_probes{};
        bool active{};unsigned mode{};int event{-1},poll{-1},hidden{-1};
    } state;
    std::uint64_t probes(context const& c)noexcept
    {
        std::uint64_t mask{};char byte{};
        auto denied=[&](unsigned bit,long value){if(value==-1&&errno==ENOSYS)mask|=std::uint64_t{1}<<bit;};
        errno=0;denied(0,::syscall(SYS_socket,AF_INET,SOCK_DGRAM,0));
        errno=0;denied(1,::syscall(SYS_openat,AT_FDCWD,"/worker/profile",O_RDONLY,0));
        errno=0;denied(2,::syscall(SYS_clone,0,0,0,0,0));
        errno=0;denied(3,::syscall(SYS_mmap,0,4096,PROT_READ|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,-1,0));
        errno=0;denied(4,::syscall(SYS_write,c.pipes[0].descriptor,&byte,1));
        errno=0;denied(5,::syscall(SYS_read,c.pipes[1].descriptor,&byte,1));
        errno=0;denied(6,::syscall(SYS_dup,c.pipes[0].descriptor));
        errno=0;denied(7,::syscall(SYS_write,3,&byte,1));
        errno=0;denied(8,::syscall(SYS_sendto,c.pipes[1].descriptor,&byte,1,0,nullptr,0));
        errno=0;denied(9,::syscall(SYS_recvmsg,3,nullptr,0));
        errno=0;denied(10,::syscall(SYS_execve,"/worker/bootstrap",nullptr,nullptr));
        errno=0;denied(11,::syscall(SYS_prctl,0,0,0,0,0));
        auto area=::mmap(nullptr,4096,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
        if(area!=MAP_FAILED){errno=0;denied(12,::mprotect(area,4096,PROT_READ|PROT_EXEC));::munmap(area,4096);}
        if(::getpid()==1)mask|=1u<<13;
        return mask;
    }
    void* pump(void* argument)noexcept
    {
        auto index=reinterpret_cast<std::uintptr_t>(argument);auto& c=state;
        ::pthread_mutex_lock(&c.mutex);while(!c.active&&!c.stop.load())::pthread_cond_wait(&c.gate,&c.mutex);::pthread_mutex_unlock(&c.mutex);
        if(c.stop.load())return nullptr;
        if(probes(c)==16383)c.thread_probes.fetch_add(1);
        // Private-link tests retain the worker's original pipe ends in the
        // trusted fixture to inject precise malformed frames. The sealed worker
        // remains alive without racing the fixture for those test bytes.
        if(c.mode==11){while(!c.stop.load()){timespec pause{0,1000000};::nanosleep(&pause,nullptr);}return nullptr;}
        while(!c.stop.load())
        {
            pollfd ready{c.pipes[index*2].descriptor,POLLIN,0};if(::poll(&ready,1,10)<=0)continue;
            std::uint8_t byte{};if(::read(ready.fd,&byte,1)!=1)continue;++byte;
            if(::write(c.pipes[index*2+1].descriptor,&byte,1)==1)c.completed.fetch_add(1);
        }
        return nullptr;
    }
}
extern "C" bool uwvm_communication_prepare(wire::bytes profile,w::startup_limits const& limits,w::prepared_backend& result)noexcept
{
    if(profile.size()!=8||profile[0]>11||limits.request_bytes<1||limits.response_bytes<8)return false;
    for(int fd{};fd<3;++fd)if((::fcntl(fd,F_GETFL)&O_NONBLOCK)==0)return false;
    state.mode=profile[0];result.context=&state;
    state.event=::eventfd(0,EFD_CLOEXEC|EFD_NONBLOCK);state.poll=::epoll_create1(EPOLL_CLOEXEC);if(state.event<0||state.poll<0)return false;
    result.notifications[0]={state.event,w::io_direction::both,w::notification_kind::event};
    result.notifications[1]={state.poll,w::io_direction::both,w::notification_kind::poll};result.notification_count=2;
    epoll_event watch{};watch.events=EPOLLIN;watch.data.fd=state.event;if(::epoll_ctl(state.poll,EPOLL_CTL_ADD,state.event,&watch))return false;
    if(state.mode==1||state.mode==2){state.hidden=::open("/worker/profile",O_RDONLY|O_CLOEXEC);if(state.hidden<0)return false;}
    if(state.mode==2){result.notifications[2]={state.hidden,w::io_direction::both,w::notification_kind::event};result.notification_count=3;}
    if(state.mode==3){result.notifications[2]=result.notifications[0];result.notification_count=3;}
    if(state.mode==8||state.mode==9)
    {
        auto area=::mmap(nullptr,4096,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);if(area==MAP_FAILED)return false;
        if(state.mode==9&&::mprotect(area,4096,PROT_READ|PROT_EXEC))return false;
    }
    if(state.mode==10)
    {
        int file=::open("/worker/profile",O_RDONLY|O_CLOEXEC);if(file<0)return false;
        auto area=::mmap(nullptr,4096,PROT_READ|PROT_EXEC,MAP_PRIVATE,file,0);::close(file);if(area==MAP_FAILED)return false;
    }
    for(std::size_t i{};i<state.threads.size();++i)if(::pthread_create(&state.threads[i],nullptr,pump,reinterpret_cast<void*>(i)))return false;
    if(state.mode==5){timespec wait{0,200000000};::nanosleep(&wait,nullptr);}
    if(state.mode==6){char wrong='!';if(::write(1,&wrong,1)!=1)return false;}
    return true;
}
extern "C" bool uwvm_communication_activate(void* opaque,std::span<w::role_fd const> pipes)noexcept
{
    if(opaque!=&state||pipes.size()!=4||state.mode==7)return false;
    ::pthread_mutex_lock(&state.mutex);std::copy(pipes.begin(),pipes.end(),state.pipes.begin());state.active=true;
    ::pthread_cond_broadcast(&state.gate);::pthread_mutex_unlock(&state.mutex);return true;
}
extern "C" int uwvm_communication_serve(void* opaque)noexcept
{
    if(opaque!=&state)return 130;
    for(;;)
    {
        char command{};auto n=::read(0,&command,1);
        if(n<0&&(errno==EINTR||errno==EAGAIN)){pollfd wait{0,POLLIN,0};::poll(&wait,1,10);continue;}
        if(n!=1)return 131;if(command=='x')return 0;if(command!='p')return 132;
        for(unsigned spin{};spin<1000&&(state.completed.load()!=2||state.thread_probes.load()!=2);++spin){timespec wait{0,1000000};::nanosleep(&wait,nullptr);}
        auto mask=probes(state);
        std::uint64_t wake=1,readback{};epoll_event event{};
        if(::write(state.event,&wake,8)==8&&::epoll_wait(state.poll,&event,1,0)==1&&::read(state.event,&readback,8)==8&&readback==1)mask|=1u<<14;
        if(state.thread_probes.load()==2&&state.completed.load()==2)mask|=1u<<15;
        std::array<std::uint8_t,8> encoded{};wire::writer writer{encoded};if(!writer.u64(mask)||::write(1,encoded.data(),encoded.size())!=8)return 133;
    }
}
extern "C" void uwvm_communication_destroy(void* opaque)noexcept
{
    if(opaque!=&state)return;state.stop=true;for(auto thread:state.threads)::pthread_join(thread,nullptr);::close(state.event);::close(state.poll);
}
