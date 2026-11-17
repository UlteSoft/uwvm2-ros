/** @file
 * @brief Bounded host RPC over already approved private worker pipes.
 * @details This object cannot launch a process or open a network/file path. Its
 * trusted supervisor supplies three existing nonblocking anonymous pipe ends.
 * One request is in flight. Fixed generation/sequence and response extent checks
 * run before accepting any worker result. A timeout, malformed frame, excess
 * diagnostic output or channel death fences future calls. After any request byte
 * was written, failure is an unknown native outcome, never a successful rollback.
 * Detached replies retain their own storage reservation until their final owner
 * releases the actual fast_io-backed bytes. No guest callback runs under the RPC
 * mutex. Stable authority and optional rate attenuation remain the dispatcher's
 * responsibility; these finite transport capacities are not rate limits.
 */
#pragma once
#include "communication_worker_protocol.h"
#include "erased_buffer.h"
#include <uwvm2/utils/container/unique.h>
#include <atomic>
#include <mutex>
#include <fcntl.h>
#include <sys/poll.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
namespace uwvm2::imported::ros::scene::communication_worker_channel
{
    namespace rpc=communication_worker_protocol;
    using error=communication::error;using effect=communication::effect;
    struct limits
    {std::uint64_t request_bytes{},response_bytes{},retained_bytes{},stderr_bytes{},deadline_ns{},poll_ns{};};
    /** @brief Trusted borrowed IPC descriptors; their supervisor outlives the channel. */
    struct descriptors
    {
        int input{-1},output{-1},diagnostics{-1},process{-1};
        std::uint64_t session_until{UINT64_MAX};
        std::atomic<bool> const* session_stopped{};
        /** @brief Recheck deployment expiry and session revocation before each I/O step. */
        bool session_live()const noexcept
        {return communication::now()<session_until&&(!session_stopped||!session_stopped->load(std::memory_order_acquire));}
    };
    struct account{std::atomic<std::uint64_t> bytes{};std::uint64_t maximum{};};
    class reservation
    {
        std::shared_ptr<account> value;std::uint64_t amount{};
    public:
        reservation()=default;reservation(reservation const&)=delete;reservation& operator=(reservation const&)=delete;
        reservation(reservation&& other)noexcept:value(std::move(other.value)),amount(std::exchange(other.amount,0)){}
        reservation& operator=(reservation&& other)noexcept
        {if(this!=&other){reset();value=std::move(other.value);amount=std::exchange(other.amount,0);}return *this;}
        ~reservation(){reset();}
        void reset()noexcept{if(value)value->bytes.fetch_sub(amount,std::memory_order_release);value.reset();amount=0;}
        bool acquire(std::shared_ptr<account> const& source,std::uint64_t bytes)noexcept
        {
            if(value||!source||bytes>source->maximum)return false;
            auto used=source->bytes.load(std::memory_order_relaxed);
            do{if(used>source->maximum-bytes)return false;}while(!source->bytes.compare_exchange_weak(used,used+bytes,std::memory_order_acquire));
            value=source;amount=bytes;return true;
        }
    };
    class reply
    {
        // Destruction order is body first, then its reservation.
        reservation held;
        ::uwvm2::utils::container::std_unique_ptr<std::uint8_t[]> body;
        std::size_t extent{};
        friend class channel;
    public:
        error status{error::io};effect observed{effect::none};std::uint64_t sequence{};
        reply()=default;reply(reply const&)=delete;reply& operator=(reply const&)=delete;
        reply(reply&&)=default;
        reply& operator=(reply&& other)noexcept
        {if(this!=&other){this->~reply();std::construct_at(this,std::move(other));}return *this;}
        wire::bytes bytes()const noexcept{return {body.get(),extent};}
    };
    /** @brief Block only this thread's SIGPIPE while writing a private pipe. */
    class broken_pipe_guard
    {
        sigset_t previous{},mask{};bool armed{},pending{},generated{};
    public:
        broken_pipe_guard()noexcept
        {
            ::sigemptyset(&mask);::sigaddset(&mask,SIGPIPE);armed=::pthread_sigmask(SIG_BLOCK,&mask,&previous)==0;
            sigset_t set;if(armed&&::sigpending(&set)==0)pending=::sigismember(&set,SIGPIPE)==1;
        }
        bool valid()const noexcept{return armed;}
        void broken_write()noexcept{generated=true;}
        ~broken_pipe_guard()
        {
            if(!armed)return;
            if(generated&&!pending){timespec zero{};while(::sigtimedwait(&mask,nullptr,&zero)<0&&errno==EINTR){}}
            ::pthread_sigmask(SIG_SETMASK,&previous,nullptr);
        }
    };
    class channel
    {
        descriptors pipes;limits capacity;std::uint64_t generation{},sequence{},diagnostics_size{};
        ::uwvm2::utils::container::std_vector<std::uint8_t> request;
        std::shared_ptr<account> retained;
        std::mutex mutex;
        std::atomic<bool> failed{};
        bool prepared{};
        pid_t owner{::getpid()};
        static bool pipe(int fd,bool reading)noexcept
        {
            struct stat info{};auto flags=::fcntl(fd,F_GETFL);
            return fd>=0&&flags>=0&&(flags&O_NONBLOCK)&&(flags&O_ACCMODE)==(reading?O_RDONLY:O_WRONLY)&&!::fstat(fd,&info)&&S_ISFIFO(info.st_mode);
        }
        bool drain()noexcept
        {
            std::array<std::uint8_t,1024> bytes{};
            for(unsigned i{};i<4;++i)
            {
                auto count=::read(pipes.diagnostics,bytes.data(),bytes.size());
                if(count>0){auto n=static_cast<std::uint64_t>(count);if(n>capacity.stderr_bytes-diagnostics_size)return false;diagnostics_size+=n;continue;}
                if(!count)return true;
                if(errno==EINTR){--i;continue;}return errno==EAGAIN||errno==EWOULDBLOCK;
            }
            return true;
        }
        error progress(int fd,std::span<std::uint8_t> bytes,bool writing,std::uint64_t deadline,std::size_t& sent)noexcept
        {
            std::size_t offset{};
            while(offset<bytes.size())
            {
                if(failed.load(std::memory_order_acquire))return error::cancelled;
                if(!drain())return error::too_large;
                if(communication::now()>=deadline)return error::timeout;
                auto count=writing?::write(fd,bytes.data()+offset,bytes.size()-offset) : ::read(fd,bytes.data()+offset,bytes.size()-offset);
                if(count>0){auto n=static_cast<std::size_t>(count);offset+=n;if(writing)sent+=n;continue;}
                if(!count)return error::io;
                if(errno==EINTR)continue;
                if(errno!=EAGAIN&&errno!=EWOULDBLOCK)return error::io;
                std::array<pollfd,3> ready{{{fd,static_cast<short>(writing?POLLOUT:POLLIN),0},{pipes.diagnostics,POLLIN,0},{pipes.process,POLLIN,0}}};
                auto delay=static_cast<int>((capacity.poll_ns+999999)/1000000);
                auto result=::poll(ready.data(),ready.size(),delay);if(result<0&&errno!=EINTR)return error::io;
                if(ready[0].revents&(POLLERR|POLLNVAL))return error::io;
                if(ready[2].revents&(POLLIN|POLLHUP|POLLERR|POLLNVAL))return error::io;
            }
            return error::ok;
        }
    public:
        /** @brief Bind only approved existing pipes; every capacity is explicit. */
        error prepare(descriptors selected,limits bound,std::uint64_t incarnation)noexcept
        {
            if(owner!=::getpid()||prepared)return error::denied;
            if(!incarnation||!bound.request_bytes||bound.request_bytes>4u*1024u*1024u||!bound.response_bytes||bound.response_bytes>4u*1024u*1024u||
               bound.retained_bytes<bound.response_bytes||bound.retained_bytes>64u*1024u*1024u||bound.stderr_bytes>65536||!bound.deadline_ns||bound.deadline_ns>60000000000ull||
               !bound.poll_ns||bound.poll_ns>100000000||!pipe(selected.input,false)||!pipe(selected.output,true)||!pipe(selected.diagnostics,true))return error::invalid;
            std::array fds{selected.input,selected.output,selected.diagnostics};for(std::size_t i{};i<fds.size();++i)for(std::size_t j{};j<i;++j)if(fds[i]==fds[j])return error::invalid;
            if(selected.process>=0&&std::find(fds.begin(),fds.end(),selected.process)!=fds.end())return error::invalid;
            try{request.resize(rpc::header_bytes+bound.request_bytes);retained=::uwvm2::utils::container::make_shared<account>();retained->maximum=bound.retained_bytes;}
            catch(...){return error::no_memory;}
            pipes=selected;capacity=bound;generation=incarnation;prepared=true;return error::ok;
        }
        /** @brief Return an owned reply or a fail-closed transport result.
         * @details Output is allocated and reserved before the first request byte.
         * A failed exchange retires the channel; the supervisor must terminate
         * and reap its worker before reclaiming staging/worker capacity. A valid
         * remote error remains a normal sequential reply and does not fence.
         */
        error exchange(rpc::operation action,std::uint64_t binding,wire::bytes body,reply& output)noexcept
        {
            if(owner!=::getpid())return error::denied;
            std::lock_guard lock{mutex};if(!prepared||failed.load()||sequence==UINT64_MAX)return error::cancelled;
            if(body.size()>capacity.request_bytes)return error::too_large;
            if(!rpc::request_header({request.data(),rpc::header_bytes},generation,sequence+1,action,binding,body.size()))return error::invalid;
            reply result;if(!result.held.acquire(retained,capacity.response_bytes))return error::too_large;
            try{result.body=::uwvm2::utils::container::make_unique_for_overwrite<std::uint8_t[]>(capacity.response_bytes);}catch(...){return error::no_memory;}
            if(!body.empty())std::memcpy(request.data()+rpc::header_bytes,body.data(),body.size());
            std::size_t sent{};auto deadline=communication::now()+capacity.deadline_ns;
            broken_pipe_guard signals;if(!signals.valid())return error::io;
            auto status=progress(pipes.input,{request.data(),rpc::header_bytes+body.size()},true,deadline,sent);
            if(status==error::io&&errno==EPIPE)signals.broken_write();
            std::array<std::uint8_t,rpc::header_bytes> header{};
            if(status==error::ok)status=progress(pipes.output,header,false,deadline,sent);
            std::uint64_t extent{};
            if(status==error::ok&&!rpc::response_extent(header,generation,sequence+1,capacity.response_bytes,extent))status=error::io;
            if(status==error::ok)status=progress(pipes.output,{result.body.get(),static_cast<std::size_t>(extent)},false,deadline,sent);
            if(status==error::ok&&!drain())status=error::too_large;
            if(status==error::ok&&failed.load(std::memory_order_acquire))status=error::cancelled;
            if(status!=error::ok)
            {failed.store(true,std::memory_order_release);result.extent=0;result.status=status;result.observed=sent?effect::unknown:effect::none;result.sequence=sequence+1;output=std::move(result);return status;}
            wire::reader decoded{header};std::uint64_t ignored{},remote_status{},observed{};
            decoded.u64(ignored);decoded.u64(ignored);decoded.u64(ignored);decoded.u64(remote_status);decoded.u64(observed);
            result.status=static_cast<error>(remote_status);result.observed=static_cast<effect>(observed);result.sequence=++sequence;result.extent=static_cast<std::size_t>(extent);
            output=std::move(result);return error::ok;
        }
        void fence()noexcept{if(owner==::getpid())failed.store(true,std::memory_order_release);}
        /** @brief Fence and drain borrowed descriptor users before supervisor teardown.
         * @details Call only outside authority, registry and factory locks. A
         * concurrent exchange observes fence and returns its actual unknown or
         * no-effect result; acquiring its mutex confirms it no longer uses the
         * supervisor's pipes. Detached replies keep their own reservations.
         */
        void retire()noexcept
        {if(owner!=::getpid())return;fence();std::lock_guard lock{mutex};}
        bool live()const noexcept{return owner==::getpid()&&prepared&&!failed.load(std::memory_order_acquire);}
        std::uint64_t retained_bytes()const noexcept{return owner==::getpid()&&retained?retained->bytes.load(std::memory_order_acquire):0;}
    };
}
