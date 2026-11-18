/** @file
 * @brief One registered module-to-driver session, with no driver process ownership.
 * @details Trusted preparation fixes identity, schemas, finite buffers and
 * deadlines before any handshake. The caller must hold exact authority during
 * each send and delivery. One outstanding sequence per channel provides bounded
 * backpressure; the deployment supplies distinct channels for other principals
 * and an independent stop lane. Unknown delivery is never retried automatically.
 * All retained carrier storage is securely overwritten. Observation is inert.
 */
#pragma once
#include "attachment_tls.h"
#include "erased_buffer.h"
#include "buffer_contract.h"
#include <mutex>
namespace uwvm2::imported::ros::scene::drivers
{
    enum class state:std::uint64_t{prepared,handshake,ready,pending,result,closed,failed};
    struct snapshot{state phase{};std::uint64_t sequence{},status{};bool effect_unknown{};};
    struct limits{std::size_t request_bytes{},response_bytes{};std::uint64_t request_ns{},session_until{};int tls_identity_fd{-1},peer_time_namespace_fd{-1};};
    inline bool valid(limits const& capacity) noexcept
    {
        return capacity.request_bytes<=protocol::maximum_payload && capacity.response_bytes<=protocol::maximum_payload &&
            capacity.request_ns && capacity.request_ns<=INT64_MAX && capacity.session_until && capacity.session_until<=INT64_MAX &&
            capacity.tls_identity_fd>=-1 && capacity.peer_time_namespace_fd>=-1;
    }
    class session
    {
        mutable std::mutex mutex_;attachment_tls::channel endpoint_;protocol::binding binding_;limits limits_;
        erased_buffer incoming_,request_;state phase_{state::prepared};bool bound_{},unknown_{};
        std::uint64_t sequence_{},deadline_{},status_{};std::size_t response_size_{},request_size_{};
        error fence(error reason)noexcept
        {unknown_|=phase_==state::pending;phase_=state::failed;incoming_.erase();request_.erase();endpoint_.close();return reason;}
        error control(protocol::kind kind)noexcept
        {std::array<std::uint8_t,protocol::header_bytes> packet{};protocol::header h{kind,binding_};
         if(!protocol::encode(h,0,packet))return error::invalid;
         return endpoint_.send(packet,{});}
        bool live(std::uint64_t now)const noexcept{return bound_&&now&&now<limits_.session_until;}
    public:
        session()=default;session(session const&)=delete;session&operator=(session const&)=delete;
        /** @brief Allocate bounded recoverable fast_io storage before binding the endpoint.
         * @details This does not read/send a packet or create an external process.
         * The session budget includes both retained carriers; OOM is recoverable.
         */
        error prepare(int fd,credentials peer,protocol::binding const& binding,limits const& capacity)
        {
            std::lock_guard lock{mutex_};if(bound_)return error::conflict;
            if(!protocol::valid(binding)||!valid(capacity))return error::invalid;
            incoming_.resize_for_overwrite(protocol::header_bytes+capacity.response_bytes);
            request_.resize_for_overwrite(capacity.request_bytes);
            auto status=endpoint_.bind(fd,peer,capacity.tls_identity_fd,attachment_tls::purpose::driver,
                binding.deployment,binding.vm,binding.module,binding.driver,binding.session,binding.contract,
                protocol::header_bytes+std::max(capacity.request_bytes,capacity.response_bytes),false,capacity.peer_time_namespace_fd);
            if(status!=error::success){incoming_.clear();request_.clear();return status;}
            binding_=binding;limits_=capacity;bound_=true;return error::success;
        }
        /** @brief Perform one bounded handshake step; readiness gives no actuation grant. */
        error connect(std::uint64_t now)noexcept
        {
            std::lock_guard lock{mutex_};if(phase_==state::closed||phase_==state::failed)return error::denied;
            if(!live(now))return fence(error::timeout);
            if(phase_==state::ready)return error::success;
            if(phase_!=state::prepared&&phase_!=state::handshake)return error::denied;
            if(phase_==state::prepared)
            {auto status=control(protocol::kind::hello);if(status==error::again)return status;
            if(status!=error::success)return fence(status);
            phase_=state::handshake;}
            std::size_t size{};auto status=endpoint_.receive(incoming_.writable(),size);
            if(status==error::again)return status;
            if(status!=error::success)return fence(status);
            protocol::header h;wire::bytes body;bool valid=protocol::decode(incoming_.view().first(size),h,body)&&h.type==protocol::kind::ready&&h.bound==binding_;
            incoming_.erase();if(!valid)return fence(error::denied);phase_=state::ready;return error::success;
        }
        /** @brief Send one exact bounded request or return its existing receipt.
         * @details An exact in-flight retry does not resend. Changed bytes or
         * deadlines with the same sequence conflict. A successful send means
         * accepted by local transport, not remote execution or physical success.
         */
        error submit(std::uint64_t sequence,std::uint64_t deadline,wire::bytes payload,std::uint64_t now)noexcept
        {
            std::lock_guard lock{mutex_};if(phase_==state::closed||phase_==state::failed)return error::denied;
            if(!live(now))return fence(error::timeout);
            if(payload.size()>limits_.request_bytes||!sequence||sequence==UINT64_MAX)return error::invalid;
            if(phase_==state::pending&&now>=deadline_)return fence(error::timeout);
            if((phase_==state::pending||phase_==state::result)&&sequence==sequence_)
                return deadline==deadline_&&payload.size()==request_size_&&std::equal(payload.begin(),payload.end(),request_.view().begin())?error::success:error::conflict;
            if(phase_!=state::ready)return error::again;
            if(sequence_==UINT64_MAX-1||sequence!=sequence_+1)return error::conflict;
            if(deadline<=now||deadline>limits_.session_until||deadline-now>limits_.request_ns)return error::timeout;
            protocol::header h{protocol::kind::request,binding_,sequence,deadline,0};std::array<std::uint8_t,protocol::header_bytes> packet{};
            if(!protocol::encode(h,payload.size(),packet))return error::invalid;
            auto status=endpoint_.send(packet,payload);if(status==error::again)return status;
            if(status!=error::success){unknown_=true;return fence(status);}
            std::copy(payload.begin(),payload.end(),request_.writable().begin());request_size_=payload.size();
            sequence_=sequence;deadline_=deadline;status_=0;phase_=state::pending;return error::success;
        }
        /** @brief Receive one reply and consume only after transactional delivery succeeds.
         * @details Timeout after submission leaves the effect unknown and fences
         * this session. Failed guest delivery retains the complete result for
         * retry. Result status alone does not establish a physical safe state.
         */
        template<typename Sink,typename Validate>error result(std::uint64_t sequence,std::uint64_t now,Sink&& sink,Validate&& validate)
        {
            std::lock_guard lock{mutex_};if(phase_==state::closed||phase_==state::failed)return error::denied;
            if(!live(now))return fence(error::timeout);
            if(sequence!=sequence_||!sequence)return error::conflict;
            if(phase_!=state::pending&&phase_!=state::result)return error::again;
            if(phase_==state::pending)
            {
                if(now>=deadline_)return fence(error::timeout);
                std::size_t size{};auto status=endpoint_.receive(incoming_.writable(),size);
                if(status==error::again)return status;
            if(status!=error::success)return fence(status);
                protocol::header h;wire::bytes body;
                if(!protocol::decode(incoming_.view().first(size),h,body)||h.type!=protocol::kind::result||h.bound!=binding_||h.sequence!=sequence_||h.deadline!=deadline_)return fence(error::denied);
                if(!validate(h.status,body)){unknown_=true;return fence(error::invalid);}
                status_=h.status;response_size_=body.size();phase_=state::result;
            }
            if(!sink(status_,incoming_.view().subspan(protocol::header_bytes,response_size_)))return error::invalid;
            incoming_.erase();request_.erase();request_size_=response_size_=0;phase_=state::ready;return error::success;
        }
        template<typename Sink>error result(std::uint64_t sequence,std::uint64_t now,Sink&& sink)
        {return result(sequence,now,std::forward<Sink>(sink),[](std::uint64_t,wire::bytes)noexcept{return true;});}
        bool same_endpoint(session const& other)const noexcept{return endpoint_.same_endpoint(other.endpoint_);}
        auto endpoint_identity()const noexcept{return endpoint_.endpoint_identity();}
        snapshot inspect()const noexcept
        {std::lock_guard lock{mutex_};return {phase_,sequence_,status_,unknown_};}
        /** @brief Release only this channel. Never send a process-stop command or signal.
         * @details A best-effort release notifies the peer even if a trusted
         * owner retained a source duplicate. The receiver must expire abandoned
         * sessions independently; release delivery is not a physical stop. */
        void disconnect()noexcept
        {std::lock_guard lock{mutex_};unknown_|=phase_==state::pending;
         if(bound_&&phase_!=state::prepared&&phase_!=state::closed&&phase_!=state::failed)(void)control(protocol::kind::release);
         endpoint_.close();incoming_.erase();request_.erase();phase_=state::closed;}
    };
}
