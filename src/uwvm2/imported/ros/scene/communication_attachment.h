/** @file
 * @brief Deployment-owned ROS provider attachment with exact directional channels.
 * @details The provisioner creates and confines the native process, supplies
 * one authenticated seqpacket control endpoint and seven exclusive anonymous
 * pipes, and retains every opposite end in the designated peer only. The VM
 * duplicates descriptors; it never sends descriptors, spawns, signals, reaps,
 * moves a cgroup or enters a namespace. Kernel identity and the fixed contract
 * authenticate control. Pipe provenance is the trusted provisioner's duty;
 * matching peer-reported metadata is not proof of sandbox containment.
 */
#pragma once
#include "driver_channel.h"
#include "communication_worker_configuration.h"
#include "communication_worker_backend.h"
#include "erased_buffer.h"
#include "communication_pipe.h"
#include <atomic>
namespace uwvm2::imported::ros::scene::communication_attachment
{
    namespace protocol=drivers::protocol;
    inline constexpr std::uint64_t magic=UINT64_C(0x43534f52564d5755);
    inline constexpr std::uint64_t hello=1,ready=2,activate=3,attached=4,release=5;
    /** @brief Trusted inherited endpoints; directions are viewed from this VM. */
    struct registration
    {
        int control{-1};drivers::credentials peer;
        // request-write, response-read, diagnostics-read, discovery-write,
        // discovery-read, user-write, user-read.
        std::array<int,7> pipes{-1,-1,-1,-1,-1,-1,-1};
        protocol::binding identity;
        std::uint64_t startup_ns{},session_until{},stderr_bytes{};
    };
    inline bool valid(registration const& value)noexcept
    {
        if(value.control<0||!value.peer.pid||value.peer.pid>INT32_MAX||value.peer.uid>UINT32_MAX||value.peer.gid>UINT32_MAX||
            !protocol::valid(value.identity)||!value.startup_ns||value.startup_ns>60000000000ull||
            !value.session_until||value.session_until>INT64_MAX||value.stderr_bytes>65536)return false;
        for(std::size_t i{};i<value.pipes.size();++i)
        {if(value.pipes[i]<0||value.pipes[i]==value.control)return false;for(std::size_t j{};j<i;++j)if(value.pipes[i]==value.pipes[j])return false;}
        return true;
    }
    /** @brief Inert endpoint registration bound to an authenticated startup definition. */
    class configuration
    {
        registration registration_;buffers::profile profile_;bool profile_set_{},channel_set_{},frozen_{};
    public:
        error profile(buffers::profile value,std::string_view installation)noexcept
        {
            if(frozen_)return error::denied;
            if(profile_set_)return error::conflict;
            // Kept as an inert deployment label; never used to open code/files.
            if(!value.identity.size||value.identity.size>value.identity.bytes.size()||installation.empty()||installation.size()>255||installation.find('\0')!=installation.npos)return error::invalid;
            profile_=value;profile_set_=true;return error::success;
        }
        error channel(registration value)noexcept
        {if(frozen_)return error::denied;if(channel_set_)return error::conflict;if(!valid(value))return error::invalid;
         registration_=value;channel_set_=true;return error::success;}
        error freeze()noexcept
        {if(frozen_)return error::conflict;if(!profile_set_||!channel_set_||!valid(registration_))return error::invalid;frozen_=true;return error::success;}
        registration const& endpoints()const noexcept{return registration_;}
        buffers::profile const& profile()const noexcept{return profile_;}
        bool frozen()const noexcept{return frozen_;}
    };
#if defined(__linux__)
    class descriptor
    {
        int value_{-1};
    public:
        descriptor()=default;explicit descriptor(int value):value_{value}{}
        descriptor(descriptor const&)=delete;descriptor&operator=(descriptor const&)=delete;
        descriptor(descriptor&& other)noexcept:value_{std::exchange(other.value_,-1)}{}
        descriptor&operator=(descriptor&& other)noexcept{if(this!=&other)reset(std::exchange(other.value_,-1));return *this;}
        ~descriptor(){reset();}explicit operator bool()const noexcept{return value_>=0;}int get()const noexcept{return value_;}
        int release()noexcept{return std::exchange(value_,-1);}
        void reset(int next=-1)noexcept{if(value_>=0)::close(value_);value_=next;}
    };
    class connection
    {
        drivers::channel control_;std::array<descriptor,7> pipes_;descriptor lifetime_;
        registration registration_;buffers::profile profile_;std::array<std::uint8_t,1024> identity_{};std::size_t extent_{};
        pid_t owner_{::getpid()};std::atomic<bool> stopping_{};bool prepared_{},sent_{},ready_{},activation_sent_{},attached_{};
        std::uint64_t deadline_{};
        static std::uint64_t now()noexcept{return static_cast<std::uint64_t>(resources::steady_now());}
        bool same_process()const noexcept{return owner_==::getpid();}
        bool live()const noexcept{return same_process()&&!stopping_.load(std::memory_order_acquire)&&now()<registration_.session_until&&static_cast<bool>(control_);}
        error send(std::uint64_t kind)noexcept
        {
            // Use the transport framing, but with a distinct domain magic and
            // a closed control opcode in status. No driver frame is accepted.
            std::array<std::uint8_t,protocol::header_bytes> header{};
            wire::writer out{header};out.u64(magic);out.u64(protocol::header_bytes);out.u64(kind);out.u64(0);
            for(auto const* id:{&registration_.identity.deployment,&registration_.identity.vm,&registration_.identity.module,
                &registration_.identity.driver,&registration_.identity.session})for(auto n:*id)out.u64(n);
            for(auto n:registration_.identity.contract)out.u64(n);
            out.u64(0);out.u64(registration_.session_until);out.u64(extent_);out.u64(0);
            return control_.send(header,{identity_.data(),extent_});
        }
        error receive(std::uint64_t kind)noexcept
        {
            std::array<std::uint8_t,protocol::header_bytes+1024> packet{};std::size_t count{};
            struct cleanup{std::span<std::uint8_t> bytes;~cleanup(){erase_bytes(bytes);}} clear{packet};
            auto status=control_.receive(packet,count);if(status!=error::success)return status;
            if(count!=protocol::header_bytes+extent_)return error::denied;
            // Recreate the expected complete immutable record; equality checks
            // every identity, reserved word, expiry and pipe role binding.
            std::array<std::uint8_t,protocol::header_bytes> expected{};wire::writer out{expected};
            out.u64(magic);out.u64(protocol::header_bytes);out.u64(kind);out.u64(0);
            for(auto const* id:{&registration_.identity.deployment,&registration_.identity.vm,&registration_.identity.module,
                &registration_.identity.driver,&registration_.identity.session})for(auto n:*id)out.u64(n);
            for(auto n:registration_.identity.contract)out.u64(n);
            out.u64(0);out.u64(registration_.session_until);out.u64(extent_);out.u64(0);
            bool accepted=std::equal(expected.begin(),expected.end(),packet.begin())&&
                std::equal(identity_.begin(),identity_.begin()+extent_,packet.begin()+protocol::header_bytes);
            return accepted?error::success:error::denied;
        }
    public:
        connection()=default;connection(connection const&)=delete;connection&operator=(connection const&)=delete;
        ~connection(){disconnect();}
        /** @brief Pin a pre-existing peer and duplicate exact one-way pipe ends.
         * @details No endpoint is read or written during preparation. Every
         * descriptor must be an anonymous pipe with the specified direction
         * and O_NONBLOCK already set. Identity receipt includes kernel dev/inode
         * and direction; the external provider must verify its opposite ends.
         */
        error prepare(configuration const& config)noexcept
        {
            if(!same_process()||prepared_||!config.frozen())return error::denied;
            auto const& input=config.endpoints();if(!valid(input)||now()>=input.session_until)return error::invalid;
            auto status=control_.bind(input.control,input.peer);if(status!=error::success)return status;
            struct rollback{connection* self;~rollback(){if(!self->prepared_)self->disconnect();}} unwind{this};
            registration_=input;profile_=config.profile();
            wire::writer out{identity_};out.text(profile_.identity.view());out.blob(profile_.sha256);
            std::array<std::pair<dev_t,ino_t>,7> identities{};
            for(std::size_t i{};i<7;++i)
            {
                int duplicate=::fcntl(input.pipes[i],F_DUPFD_CLOEXEC,3);if(duplicate<0)return error::bad_handle;pipes_[i].reset(duplicate);
                struct stat info{};bool writing=i==0||i==3||i==5;
                if(!communication_pipe::anonymous(duplicate,!writing,info))return error::denied;
                identities[i]={info.st_dev,info.st_ino};for(std::size_t j{};j<i;++j)if(identities[j]==identities[i])return error::conflict;
                out.u64(i);out.u64(static_cast<std::uint64_t>(info.st_dev));out.u64(static_cast<std::uint64_t>(info.st_ino));out.boolean(writing);
            }
            out.u64(input.stderr_bytes);if(!out.good())return error::invalid;extent_=out.size();
            lifetime_.reset(static_cast<int>(::syscall(SYS_pidfd_open,static_cast<pid_t>(input.peer.pid),0)));
            if(!lifetime_)return error::denied;
            auto instant=now();if(instant>=input.session_until)return error::timeout;
            prepared_=true;return error::success;
        }
        /** @brief One nonblocking admission step; only called while context/node authority is held. */
        error begin()noexcept
        {
            if(!live()||!prepared_)return error::denied;
            if(!deadline_){auto instant=now();if(instant>=registration_.session_until)return error::timeout;deadline_=instant+std::min(registration_.startup_ns,registration_.session_until-instant);}
            if(now()>=deadline_)return error::timeout;
            if(!sent_){auto status=send(hello);if(status!=error::success)return status;sent_=true;}
            if(ready_)return error::success;
            auto status=receive(ready);if(status==error::success)ready_=true;
            else if(status!=error::again)request_stop();
            return status;
        }
        error activate_channel()noexcept
        {
            if(!live()||!ready_)return error::denied;
            if(now()>=deadline_)return error::timeout;
            if(!activation_sent_){auto status=send(activate);if(status!=error::success)return status;activation_sent_=true;}
            if(attached_)return error::success;
            auto status=receive(attached);if(status==error::success)attached_=true;
            else if(status!=error::again)request_stop();
            return status;
        }
        void request_stop()noexcept{if(same_process())stopping_.store(true,std::memory_order_release);}
        /** @brief Close only this session after all local RPC/relay users drain. */
        void disconnect()noexcept
        {
            // A successful preparation consumes this incarnation permanently.
            // Failed preparation may roll back and retry without publishing a
            // borrowed session. Fence every published borrower before closing
            // descriptors; a later fd reuse must never revive the session.
            if(prepared_)request_stop();
            if(same_process()&&sent_&&control_)(void)send(release);
            control_.close();for(auto& pipe:pipes_)pipe.reset();lifetime_.reset();erase_bytes(identity_);
        }
        /** @brief Session fence for borrowed RPC/relay users, without peer lifecycle authority.
         * @details Users must drain before disconnect. Expiry is checked before
         * every bounded nonblocking I/O step; elapsed sessions never auto-renew.
         */
        bool session_live()const noexcept
        {return prepared_&&same_process()&&!stopping_.load(std::memory_order_acquire)&&now()<registration_.session_until;}
        std::atomic<bool> const* stop_flag()const noexcept{return &stopping_;}
        std::uint64_t session_until()const noexcept{return registration_.session_until;}
        /** @brief Reject control or pipe aliases across simultaneously admitted peers. */
        bool overlaps(connection const& other)const noexcept
        {
            if(control_.same_endpoint(other.control_)||registration_.identity.session==other.registration_.identity.session)return true;
            for(auto const& left:pipes_)for(auto const& right:other.pipes_)
            {
                struct stat a{},b{};
                if(!left||!right||::fstat(left.get(),&a)||::fstat(right.get(),&b))return true;
                if(a.st_dev==b.st_dev&&a.st_ino==b.st_ino)return true;
            }
            return false;
        }
        bool privately_ready()const noexcept{return ready_&&live();}
        bool attachment_confirmed()const noexcept{return attached_&&live();}
        bool identity_confirmed()const noexcept{return prepared_&&static_cast<bool>(control_);}
        bool disclosed()const noexcept{return sent_;}
        int process_fd()const noexcept{return lifetime_.get();}
        int input_fd()const noexcept{return pipes_[0].get();}int output_fd()const noexcept{return pipes_[1].get();}
        int stderr_fd()const noexcept{return pipes_[2].get();}int transport_fd(std::size_t index)const noexcept{return index<4?pipes_[index+3].get():-1;}
        std::uint64_t stderr_bytes()const noexcept{return registration_.stderr_bytes;}
        buffers::profile const& configuration_profile()const noexcept{return profile_;}
    };
#endif
}
