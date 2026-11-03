// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "deployment.h"
#include "command_state.h"
#include <uwvm2/imported/ros/ipc/unix.h>
#include <chrono>
#include <cstring>

namespace uwvm2::imported::ros::portable
{
    // Separate native protocol, never a Guest import. Every request/reply binds
    // the complete Wasm, adapter, policy and native provenance digests plus
    // real runtime generation and all three external epochs.
    enum class native_operation : unsigned { bind=1,read_state=2,submit=3,wait=4,close=5 };
    struct native_event
    {
        ::std::array<::std::uint8_t,16> goal_uuid{};
        ::std::uint64_t observed_ns{};
        outcome result{};::std::int32_t status{};
    };
    inline bool decode_native_event(::std::span<::std::uint8_t const> bytes,native_event& output) noexcept
    {
        if(bytes.size()!=32u) { return false; }native_event value{};bool nonzero{};
        for(::std::size_t i{};i!=16u;++i) { value.goal_uuid[i]=bytes[i];nonzero|=bytes[i]!=0; }
        value.observed_ns=wire_detail::get(bytes,16,8);
        auto kind{wire_detail::get(bytes,24,4)};if(!nonzero||!value.observed_ns||kind<1||kind>4) { return false; }
        value.result=static_cast<outcome>(kind);
        value.status=::std::bit_cast<::std::int32_t>(static_cast<::std::uint32_t>(wire_detail::get(bytes,28,4)));
        output=value;return true;
    }
    inline ::std::uint64_t monotonic_ns() noexcept
    {
        auto n{::std::chrono::duration_cast<::std::chrono::nanoseconds>(::std::chrono::steady_clock::now().time_since_epoch()).count()};
        return n>0?static_cast<::std::uint64_t>(n):0u;
    }
    class joint_native_client final
    {
        int fd_{-1};::std::uint64_t session_{},next_{1};
        ::std::array<::std::uint8_t,168> binding_{};
#if defined(__linux__)
        static bool pinned_peer(int socket,joint_deployment const& deployment,::std::chrono::steady_clock::time_point until)
        {
            ::std::uint32_t uid{},pid{};
            if(!ipc::peer_credentials(socket,uid,pid)||uid!=deployment.native_uid) { return false; }
            auto path{::std::string{"/proc/"}+::std::to_string(pid)+"/exe"};
            int fd{::open(path.c_str(),O_RDONLY|O_CLOEXEC)};if(fd<0) { return false; }
            struct release { int fd;~release(){::close(fd);} } held{fd};struct stat before{},after{},selected{};
            if(::fstat(fd,&before)!=0||!S_ISREG(before.st_mode)||before.st_size<=0||before.st_size>512*1024*1024) { return false; }
            ::fast_io::sha256_context hash{};::std::array<::std::byte,65536> buffer{};off_t offset{};
            while(offset!=before.st_size)
            {
                if(::std::chrono::steady_clock::now()>=until) { return false; }
                auto size{::std::min<::std::uint64_t>(buffer.size(),static_cast<::std::uint64_t>(before.st_size-offset))};
                auto count{::pread(fd,buffer.data(),static_cast<::std::size_t>(size),offset)};
                if(count<0&&errno==EINTR) { continue; }if(count<=0) { return false; }
                hash.update(buffer.data(),buffer.data()+count);offset+=count;
            }
            digest actual{};hash.do_final();hash.digest_to_byte_ptr(reinterpret_cast<::std::byte*>(actual.data()));
            // Recheck the actual peer credentials and its currently selected
            // executable; an old open inode cannot authenticate a later exec.
            ::std::uint32_t uid_after{},pid_after{};char extra{};
            return actual==deployment.native_executable&&ipc::peer_credentials(socket,uid_after,pid_after)&&uid_after==uid&&pid_after==pid&&
                ::pread(fd,&extra,1,before.st_size)==0&&::fstat(fd,&after)==0&&::stat(path.c_str(),&selected)==0&&
                before.st_dev==after.st_dev&&before.st_ino==after.st_ino&&before.st_size==after.st_size&&
                before.st_mtim.tv_sec==after.st_mtim.tv_sec&&before.st_mtim.tv_nsec==after.st_mtim.tv_nsec&&
                before.st_ctim.tv_sec==after.st_ctim.tv_sec&&before.st_ctim.tv_nsec==after.st_ctim.tv_nsec&&
                selected.st_dev==before.st_dev&&selected.st_ino==before.st_ino&&::std::chrono::steady_clock::now()<until;
        }
#endif
    public:
        joint_native_client() noexcept=default;
        joint_native_client(joint_native_client const&)=delete;
        joint_native_client& operator=(joint_native_client const&)=delete;
        ~joint_native_client() { close(); }
        void close() noexcept
        {
#if defined(__linux__)
            if(fd_>=0) { ::close(fd_); }
#endif
            fd_=-1;session_=0;
        }
        [[nodiscard]] bool connected() const noexcept { return fd_>=0&&session_; }
        error request(native_operation operation,::std::span<::std::uint8_t const> body,
                      ::std::vector<::std::uint8_t>& output,::std::uint64_t deadline_ns)
        {
#if defined(__linux__)
            if(!connected()||next_==UINT64_MAX||body.size()>4096u||deadline_ns<=monotonic_ns()||
               deadline_ns>static_cast<::std::uint64_t>((::std::numeric_limits<::std::int64_t>::max)())) { return error::deadline; }
            auto until{::std::chrono::steady_clock::time_point{::std::chrono::nanoseconds{static_cast<::std::int64_t>(deadline_ns)}}};
            ::std::array<::std::uint8_t,40> header{};header[0]='U';header[1]='P';header[2]='N';header[3]='1';
            wire_detail::put(header,4,1,2);wire_detail::put(header,6,static_cast<unsigned>(operation),2);
            wire_detail::put(header,8,binding_.size()+body.size(),4);wire_detail::put(header,16,next_,8);
            wire_detail::put(header,24,session_,8);wire_detail::put(header,32,deadline_ns,8);
            ::std::vector<::std::uint8_t> request(header.begin(),header.end());request.insert(request.end(),binding_.begin(),binding_.end());request.insert(request.end(),body.begin(),body.end());
            if(!ipc::transfer(fd_,request.data(),request.size(),true,until)||!ipc::transfer(fd_,header.data(),header.size(),false,until)) { close();return error::deadline; }
            auto size{wire_detail::get(header,8,4)},status{wire_detail::get(header,12,4)};
            if(header[0]!='U'||header[1]!='P'||header[2]!='N'||header[3]!='1'||wire_detail::get(header,4,2)!=1||
               wire_detail::get(header,6,2)!=static_cast<unsigned>(operation)||wire_detail::get(header,16,8)!=next_||
               wire_detail::get(header,24,8)!=session_||wire_detail::get(header,32,8)!=deadline_ns||size<binding_.size()||size>binding_.size()+4096u)
            { close();return error::malformed; }
            ::std::vector<::std::uint8_t> response(static_cast<::std::size_t>(size));
            if(!ipc::transfer(fd_,response.data(),response.size(),false,until)) { close();return error::deadline; }
            if(!::std::equal(binding_.begin(),binding_.end(),response.begin())) { close();return error::denied; }
            ++next_;
            if(status)
            {
                if(response.size()!=binding_.size()) { close();return error::malformed; }
                switch(status)
                {
                    case 9:case 11:case 13:case 22:case 38:case 90:case 95:case 110:case 116:return static_cast<error>(status);
                    default:close();return error::malformed;
                }
            }
            output.assign(response.begin()+static_cast<::std::ptrdiff_t>(binding_.size()),response.end());return error::ok;
#else
            (void)operation;(void)body;(void)output;(void)deadline_ns;return error::unsupported;
#endif
        }
        error connect(joint_deployment const& deployment,owner_key const& owner,::std::uint64_t owner_ordinal,::std::uint64_t deadline_ns)
        {
            if(connected()||!owner.instance||!owner.generation||!owner_ordinal||owner.wasm_sha256!=deployment.wasm_sha256) { return error::denied; }
#if defined(__linux__)
            if(deadline_ns<=monotonic_ns()||deadline_ns>static_cast<::std::uint64_t>((::std::numeric_limits<::std::int64_t>::max)())) { return error::deadline; }
            auto until{::std::chrono::steady_clock::time_point{::std::chrono::nanoseconds{static_cast<::std::int64_t>(deadline_ns)}}};
            sockaddr_un address{};address.sun_family=AF_UNIX;struct stat socket_info{};
            if(deployment.socket.size()>=sizeof address.sun_path||::lstat(deployment.socket.c_str(),&socket_info)!=0||
               !S_ISSOCK(socket_info.st_mode)||socket_info.st_uid!=deployment.native_uid||(socket_info.st_mode&0007)) { return error::denied; }
            ::std::memcpy(address.sun_path,deployment.socket.c_str(),deployment.socket.size()+1u);
            fd_=::socket(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,0);if(fd_<0) { return error::unavailable; }
            if(::connect(fd_,reinterpret_cast<sockaddr*>(&address),sizeof address)<0&&
               (errno!=EINPROGRESS||!ipc::wait_connected(fd_,until,-1))) { close();return error::unavailable; }
            if(!pinned_peer(fd_,deployment,until)) { close();return error::denied; }
            session_=ipc::random_session();if(!session_) { close();return error::unavailable; }
            ::std::size_t at{};
            for(auto const* digest_value:{&owner.wasm_sha256,&deployment.adapter_sha256,&deployment.policy_sha256,&deployment.native_provenance})
            { ::std::copy(digest_value->begin(),digest_value->end(),binding_.begin()+static_cast<::std::ptrdiff_t>(at));at+=32u; }
            for(auto value:{owner.generation,deployment.policy_epoch,deployment.mapping.epoch,deployment.native_generation,owner_ordinal})
            { wire_detail::put(binding_,at,value,8);at+=8u; }
            ::std::vector<::std::uint8_t> answer{};auto e{request(native_operation::bind,{},answer,deadline_ns)};
            if(e!=error::ok||!answer.empty()) { close();return e==error::ok?error::malformed:e; }return error::ok;
#else
            (void)deployment;(void)deadline_ns;return error::unsupported;
#endif
        }
    };
}
