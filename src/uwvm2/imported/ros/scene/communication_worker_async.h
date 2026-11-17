/** @file
 * @brief Nonblocking, pre-reserved RPC for authority-linearized endpoint creation.
 * @details prepare() allocates finite recoverable fast_io request/reply storage.
 * stage() copies the complete owned request before authority is charged. send()
 * requires the current complete admission group and performs ONE nonblocking
 * write: no poll wait, sleep, allocation or callback occurs under that lease.
 * Further send attempts revalidate the complete group without charging creation
 * again. receive() advances only nonblocking reads outside authority locks.
 *
 * A creation coordinator owns the request key, full origin chain, reservation and
 * receipt. It must retain those through exact retries and publish a guest handle
 * only after fresh complete admission. This transport does not mint authority or
 * retry an open after a possibly effective write. A timeout, partial frame, death
 * or revoke fences the channel; any sent byte yields an unknown outcome. Ready
 * receipts remain readable after fencing, because cancellation cannot erase an
 * already observed native result. No stage can reuse the failed incarnation.
 */
#pragma once
#include "communication_worker_channel.h"
#include "communication_pipe.h"
#include "erased_buffer.h"
#include "authority.h"
namespace uwvm2::imported::ros::scene::communication_worker_async
{
#if defined(__linux__)
    namespace protocol=communication_worker_protocol;
    namespace channel=communication_worker_channel;
    using error=communication::error;using effect=communication::effect;
    enum class phase:std::uint8_t{unconfigured,idle,staged,sending,header,body,ready,failed};
    struct ticket
    {std::uint64_t incarnation{},request{};friend bool operator==(ticket const&,ticket const&)=default;};
    struct result
    {error status{error::again};effect observed{};std::uint64_t wire_sequence{},bytes{};};
    struct snapshot
    {phase state{};bool fenced{};ticket current;result receipt;std::uint64_t sent{},diagnostics{};};
    class transport
    {
        channel::descriptors fds;channel::limits limits;
        ::uwvm2::utils::container::std_vector<std::uint8_t> outgoing,incoming;
        std::array<std::uint8_t,protocol::header_bytes> header{};
        std::uint64_t generation{},sequence{},serial{},deadline{},diagnostic_bytes{};
        std::size_t request_bytes{},sent{},header_bytes{},reply_bytes{},received{};
        phase state{};result receipt;
        std::atomic<bool> stopped{};mutable std::mutex mutex;pid_t owner{::getpid()};
        bool same_process()const noexcept{return owner==::getpid();}
        error fail(error status)noexcept
        {
            stopped.store(true,std::memory_order_release);state=phase::failed;
            receipt={status,sent?effect::unknown:effect::none,sequence+1,0};wipe();return status;
        }
        void wipe()noexcept
        {erase_bytes({outgoing.data(),request_bytes});erase_bytes({incoming.data(),received});erase_bytes(header);}
        error check_live()noexcept
        {
            if(stopped.load(std::memory_order_acquire)||
               (fds.session_stopped&&fds.session_stopped->load(std::memory_order_acquire)))return fail(error::cancelled);
            auto instant=communication::now();
            if(instant>=fds.session_until)return fail(error::cancelled);
            if(instant>=deadline)return fail(error::timeout);
            return error::ok;
        }
        error check()noexcept
        {
            auto status=check_live();if(status!=error::ok)return status;
            pollfd identity{fds.process,POLLIN,0};auto count=::poll(&identity,1,0);
            if(count<0&&errno!=EINTR)return fail(error::io);
            if(count>0)return fail(error::io);
            std::array<std::uint8_t,1024> text{};
            struct cleanup{std::span<std::uint8_t> bytes;~cleanup(){erase_bytes(bytes);}} clear{text};
            for(unsigned i{};i<4;++i)
            {
                status=check_live();if(status!=error::ok)return status;
                auto got=::read(fds.diagnostics,text.data(),text.size());
                if(got>0)
                {auto n=static_cast<std::uint64_t>(got);if(n>limits.stderr_bytes-diagnostic_bytes)return fail(error::too_large);diagnostic_bytes+=n;continue;}
                if(!got)break;
                if(errno==EINTR)continue;
                if(errno==EAGAIN||errno==EWOULDBLOCK)break;
                return fail(error::io);
            }
            return error::ok;
        }
        bool matches(ticket value)const noexcept{return value.incarnation==generation&&value.request==serial&&serial;}
    public:
        ~transport(){wipe();}
        /** @brief Bind trusted borrowed pipes and the actual retained namespace pidfd.
         * @details The supervisor must outlive this transport and drain its users
         * before closing descriptors. No worker is launched or endpoint opened.
         */
        error prepare(channel::descriptors selected,channel::limits bound,std::uint64_t incarnation)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(state!=phase::unconfigured||stopped.load())return error::denied;
            if(!incarnation||!bound.request_bytes||bound.request_bytes>4u*1024u*1024u||!bound.response_bytes||bound.response_bytes>4u*1024u*1024u||
                bound.retained_bytes<bound.response_bytes||bound.retained_bytes>64u*1024u*1024u||bound.stderr_bytes>65536||!bound.deadline_ns||
                bound.deadline_ns>60000000000ull||selected.process<0)return error::invalid;
            std::array<int,3> pipes{selected.input,selected.output,selected.diagnostics};std::array<struct stat,3> information;
            for(std::size_t i{};i<3;++i)
            {
                if(!communication_pipe::anonymous(pipes[i],i!=0,information[i])||pipes[i]==selected.process)return error::invalid;
                for(std::size_t j{};j<i;++j)if(information[i].st_dev==information[j].st_dev&&information[i].st_ino==information[j].st_ino)return error::invalid;
            }
            pollfd identity{selected.process,POLLIN,0};if(::poll(&identity,1,0)!=0)return error::denied;
            try{outgoing.resize(protocol::header_bytes+bound.request_bytes);incoming.resize(bound.response_bytes);}
            catch(...){return error::no_memory;}
            fds=selected;limits=bound;generation=incarnation;wipe();state=phase::idle;return error::ok;
        }
        /** @brief Copy one finite request before its creation charge; performs no native I/O. */
        error stage(protocol::operation operation,std::uint64_t binding,wire::bytes body,std::uint64_t absolute_deadline,ticket& output)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(stopped.load()||state==phase::unconfigured||!fds.session_live())return error::denied;
            if(state!=phase::idle)return error::busy;
            if(serial==UINT64_MAX||sequence==UINT64_MAX||body.size()>limits.request_bytes)return error::too_large;
            auto now=communication::now();if(absolute_deadline<=now)return error::timeout;
            if(absolute_deadline-now>limits.deadline_ns)return error::invalid;
            if(!protocol::request_header({outgoing.data(),protocol::header_bytes},generation,sequence+1,operation,binding,body.size()))return error::invalid;
            if(!body.empty())std::memcpy(outgoing.data()+protocol::header_bytes,body.data(),body.size());
            request_bytes=protocol::header_bytes+body.size();sent=header_bytes=reply_bytes=received=0;deadline=absolute_deadline;
            receipt={error::again,effect::none,sequence+1,0};++serial;state=phase::staged;output={generation,serial};return error::ok;
        }
        /** @brief Write at most one nonblocking chunk under CURRENT complete authority.
         * @details The coordinator charges only its first accepted enqueue.
         * Every retry/pump uses validation-only complete origin claims. Missing
         * authority leaves the frame staged and writes no byte. No hidden sleep
         * or RPC response wait occurs here, even when the pipe is full.
         */
        error send(ticket selected,group_lease const& current)noexcept
        {
            if(!same_process()||!current)return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!matches(selected))return error::stale;
            if(state==phase::header||state==phase::body||state==phase::ready)return error::ok;
            if(state!=phase::staged&&state!=phase::sending)return state==phase::failed?receipt.status:error::denied;
            auto status=check();if(status!=error::ok)return status;
            channel::broken_pipe_guard signals;if(!signals.valid())return fail(error::io);
            status=check_live();if(status!=error::ok)return status;
            auto count=::write(fds.input,outgoing.data()+sent,request_bytes-sent);
            if(count<0)
            {
                if(errno==EAGAIN||errno==EWOULDBLOCK||errno==EINTR)return error::again;
                if(errno==EPIPE)signals.broken_write();
                return fail(error::io);
            }
            if(!count)return fail(error::io);
            erase_bytes({outgoing.data()+sent,static_cast<std::size_t>(count)});
            sent+=static_cast<std::size_t>(count);state=sent==request_bytes?phase::header:phase::sending;
            return sent==request_bytes?error::ok:error::again;
        }
        /** @brief Advance at most four nonblocking reads outside all authority leases. */
        error receive(ticket selected)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!matches(selected))return error::stale;
            if(state==phase::ready)return error::ok;
            if(state==phase::failed)return receipt.status;
            if(state!=phase::header&&state!=phase::body)return error::again;
            auto status=check();if(status!=error::ok)return status;
            for(unsigned step{};step<4;++step)
            {
                status=check_live();if(status!=error::ok)return status;
                auto destination=state==phase::header?std::span<std::uint8_t>{header}.subspan(header_bytes):
                    std::span<std::uint8_t>{incoming.data(),reply_bytes}.subspan(received);
                auto count=destination.empty()?0 : ::read(fds.output,destination.data(),destination.size());
                if(count<0){if(errno==EAGAIN||errno==EWOULDBLOCK||errno==EINTR)return error::again;return fail(error::io);}
                if(!count&&!destination.empty())return fail(error::io);
                if(state==phase::header)
                {
                    header_bytes+=static_cast<std::size_t>(count);if(header_bytes<header.size())continue;
                    std::uint64_t extent{};if(!protocol::response_extent(header,generation,sequence+1,limits.response_bytes,extent))return fail(error::io);
                    reply_bytes=static_cast<std::size_t>(extent);state=phase::body;
                }
                else received+=static_cast<std::size_t>(count);
                if(state==phase::body&&received==reply_bytes)
                {
                    wire::reader reader{header};std::uint64_t ignored{},remote_status{},observed{};
                    reader.u64(ignored);reader.u64(ignored);reader.u64(ignored);reader.u64(remote_status);reader.u64(observed);
                    receipt={static_cast<error>(remote_status),static_cast<effect>(observed),++sequence,reply_bytes};state=phase::ready;return error::ok;
                }
            }
            return error::again;
        }
        /** @brief Copy a complete receipt into the coordinator's pre-reserved storage.
         * @details No guest callback runs here. The coordinator revalidates its
         * full origin group before any later guest delivery or handle publication.
         */
        error copy(ticket selected,std::span<std::uint8_t> output,result& answer)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!matches(selected))return error::stale;
            if(state!=phase::ready&&state!=phase::failed)return error::again;
            if(output.size()<receipt.bytes)return error::too_large;
            if(receipt.bytes)std::memcpy(output.data(),incoming.data(),static_cast<std::size_t>(receipt.bytes));
            answer=receipt;return error::ok;
        }
        /** @brief Drop an owned frame only while no byte has reached the worker.
         * @details This does not consume a wire sequence. A partial/full write
         * can never use this path; its result and cleanup debt must be retained.
         */
        error discard(ticket selected)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!matches(selected))return error::stale;
            if(sent||!(state==phase::staged||state==phase::sending))return error::busy;
            wipe();state=phase::idle;return error::ok;
        }
        /** @brief Acknowledge only after the coordinator retained the entire result. */
        error acknowledge(ticket selected)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!matches(selected))return error::stale;
            if(state!=phase::ready)return state==phase::failed?receipt.status:error::again;
            wipe();state=phase::idle;return error::ok;
        }
        void request_stop()noexcept{if(same_process())stopped.store(true,std::memory_order_release);}
        /** @brief Fence and drain short descriptor users outside authority locks. */
        void retire()noexcept
        {if(!same_process())return;request_stop();std::lock_guard lock{mutex};if(state!=phase::idle&&state!=phase::unconfigured&&state!=phase::ready&&state!=phase::failed)fail(error::cancelled);}
        snapshot inspect()const noexcept
        {if(!same_process())return {};std::lock_guard lock{mutex};return {state,stopped.load(),{generation,serial},receipt,sent,diagnostic_bytes};}
    };
#endif
}
