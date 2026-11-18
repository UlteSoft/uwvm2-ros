/** @file
 * @brief Model inference through a provisioned external channel; no process ownership.
 * @details Deployment owns model loading, sandboxing, scheduling and lifetime.
 * The VM owns only an authenticated channel and one bounded supervisor thread.
 * Every data send holds current model/input authority. Revocation or timeout
 * closes this session without signals. Disclosed unfinished work stays unknown
 * and keeps its input reservations until trusted external reconciliation.
 * A valid terminal response is provider completion evidence, not proof that an
 * untrusted model erased its own copies. All VM-owned carriers are erased.
 */
#pragma once
#include "model_attachment_protocol.h"
#include "attachment_tls.h"
#include "model_worker_protocol.h"
#include "model_dispatch.h"
#include "model_provider.h"
#include "buffer_contract.h"
#include "erased_buffer.h"
#include "supervisor_stack.h"
namespace uwvm2::imported::ros::scene::model_attachment
{
    struct limits
    {
        std::uint64_t request_bytes{},response_bytes{},startup_ns{},poll_ns{},supervisor_bytes{},session_until{};
        int tls_identity_fd{-1},peer_time_namespace_fd{-1};
    };
    /** @brief Closed finite local reservations; external RSS/CPU are deployment policy. */
    inline bool valid(limits const& value)noexcept
    {
        return value.request_bytes>=24&&value.request_bytes<=protocol::maximum_message&&
            value.response_bytes>=24&&value.response_bytes<=protocol::maximum_message&&
            value.startup_ns&&value.startup_ns<=60000000000ull&&value.poll_ns&&value.poll_ns<=100000000&&
            value.supervisor_bytes>=65536&&value.supervisor_bytes<=16777216&&
            value.session_until&&value.session_until<=INT64_MAX&&value.tls_identity_fd>=-1&&value.peer_time_namespace_fd>=-1;
    }
    inline constexpr std::size_t transport_frame(limits const& capacity)noexcept
    {
        auto body=std::max({capacity.request_bytes,capacity.response_bytes,std::uint64_t{1024}});
        return protocol::header_bytes+static_cast<std::size_t>(std::min<std::uint64_t>(body,protocol::fragment_bytes));
    }
#if defined(__linux__)
    class service final:public models::provider_lifetime
    {
        model_dispatch& dispatcher_;
        ::uwvm2::utils::container::std_string alias_;
        std::uint64_t generation_{},sequence_{};
        models::plan plan_;protocol::binding binding_;limits limits_;
        attachment_tls::channel channel_;erased_buffer packet_;
        model_worker::supervisor_stack stack_;pthread_t thread_{};pid_t owner_{::getpid()};
        bool started_{},joined_{};std::atomic<bool> stopped_{};std::atomic<error> startup_{error::again};
        std::mutex retirement_;
        static std::uint64_t now()noexcept{return static_cast<std::uint64_t>(resources::steady_now());}
        bool same_process()const noexcept{return owner_==::getpid();}
        bool cancelled(std::uint64_t deadline,model_dispatch::work const* work)const
        {return stopped_.load(std::memory_order_acquire)||now()>=deadline||now()>=limits_.session_until||
            (work&&work->cancellation_requested());}
        void pause()const noexcept
        {
            timespec delay{static_cast<time_t>(limits_.poll_ns/1000000000),static_cast<long>(limits_.poll_ns%1000000000)};
            // Signals do not restart a full delay or defeat the next deadline check.
            (void)::nanosleep(&delay,nullptr);
        }
        error send(protocol::kind kind,std::uint64_t sequence,std::uint64_t deadline,std::uint64_t offset,
                   wire::bytes body,model_dispatch::work const* work=nullptr)
        {
            protocol::header h{kind,binding_,sequence,sequence?deadline:0,offset};
            std::array<std::uint8_t,protocol::header_bytes> header{};
            if(!protocol::encode(h,body.size(),header))return error::invalid;
            for(;;)
            {
                if(cancelled(deadline,work))return error::cancelled;
                auto operation=[&]()noexcept{return channel_.send(header,body);};
                auto status=work?work->transfer(operation):operation();
                if(status!=error::again)return status;
                pause();
            }
        }
        error receive(protocol::header& h,wire::bytes& body,std::uint64_t deadline,model_dispatch::work const* work=nullptr)
        {
            for(;;)
            {
                if(cancelled(deadline,work))return error::cancelled;
                std::size_t size{};auto status=channel_.receive(packet_.writable(),size);
                if(status==error::again){pause();continue;}
                if(status!=error::success)return status;
                return protocol::decode(packet_.view().first(size),h,body)&&h.bound==binding_?error::success:error::denied;
            }
        }
        error begin()
        {
            std::array<std::uint8_t,1024> expected{};wire::writer out{expected};
            if(!model_contract::write_profile(out,plan_.model)||!model_contract::write_profile(out,plan_.task)||
                !out.text(plan_.parameter_type.view())||!out.text(plan_.output_type.view())||!out.text(plan_.encoding.view())||
                !out.u64(limits_.request_bytes)||!out.u64(limits_.response_bytes)||!out.u64(limits_.session_until))return error::invalid;
            auto instant=now();if(instant>=limits_.session_until)return error::timeout;
            auto deadline=instant+std::min(limits_.startup_ns,limits_.session_until-instant);
            auto status=send(protocol::kind::hello,0,deadline,0,out.written());if(status!=error::success)return status;
            protocol::header h;wire::bytes body;status=receive(h,body,deadline);
            bool accepted=status==error::success&&h.type==protocol::kind::ready&&
                std::ranges::equal(body,out.written());packet_.erase();
            return accepted?dispatcher_.attach(alias_,generation_):status==error::success?error::denied:status;
        }
        bool encode(model_dispatch::work& work,wire::writer& out,std::size_t count)
        {
            if(!out.u64(work.identity())||work.with_parameters([&](wire::bytes bytes){return out.blob(bytes);})!=error::success||!out.u64(count))return false;
            for(std::size_t i{};i<count;++i)
            {
                auto status=work.with_input(i,[&](wire::bytes bytes,buffers::layout const& layout,buffers::digest const&)
                {
                    std::array<std::uint8_t,512> metadata{};wire::writer writer{metadata};
                    return buffer_contract::encode_layout(writer,layout)&&out.blob(writer.written())&&out.blob(bytes);
                });
                if(status!=error::success)return false;
            }
            return out.good();
        }
        /** @brief Execute one framed job; uncertainty never refunds remote cleanup debt. */
        bool execute(model_dispatch::work& work)
        {
            bool disclosed{};
            try
            {
                std::size_t count{};std::uint64_t deadline{};
                if(work.metadata(count,deadline)!=error::success){work.finish(models::state::cancelled);return true;}
                if(deadline>limits_.session_until||sequence_>=UINT64_MAX-1){work.finish(models::state::failed);return false;}
                auto measured=wire::writer::measuring(static_cast<std::size_t>(limits_.request_bytes));
                if(!encode(work,measured,count)){work.finish(models::state::failed);return true;}
                auto bytes=measured.size()+limits_.response_bytes;models::registry::scratch_lease reserved;
                if(work.reserve_scratch(bytes,reserved)!=error::success){work.finish(models::state::failed);return true;}
                erased_buffer data;data.resize_for_overwrite(static_cast<std::size_t>(bytes));
                auto request=data.writable().first(measured.size());auto response=data.writable().subspan(measured.size());
                wire::writer writer{request};
                if(!encode(work,writer,count)||writer.size()!=measured.size()){work.finish(models::state::cancelled);return true;}
                auto sequence=++sequence_;
                for(std::size_t offset{};offset<request.size();)
                {
                    auto fragment=request.subspan(offset,std::min(protocol::fragment_bytes,request.size()-offset));
                    // Conservative on any attempted transport failure: an I/O
                    // error cannot prove the peer did not observe a fragment.
                    disclosed=true;
                    if(send(protocol::kind::request,sequence,deadline,offset,fragment,&work)!=error::success)throw error::io;
                    offset+=fragment.size();
                }
                if(send(protocol::kind::request_end,sequence,deadline,request.size(),{},&work)!=error::success)throw error::io;
                erase_bytes(request);
                for(;;)
                {
                    std::size_t used{};
                    for(;;)
                    {
                        protocol::header h;wire::bytes body;
                        if(receive(h,body,deadline,&work)!=error::success||h.sequence!=sequence||h.deadline!=deadline||h.offset!=used)throw error::denied;
                        if(h.type==protocol::kind::response_end){packet_.erase();break;}
                        if(h.type!=protocol::kind::response||body.size()>response.size()-used)throw error::invalid;
                        std::copy(body.begin(),body.end(),response.begin()+used);used+=body.size();packet_.erase();
                    }
                    model_worker::response kind{};std::uint64_t id{};wire::bytes payload;
                    if(!model_worker::decode_response(response.first(used),kind,id,payload)||id!=work.identity())throw error::invalid;
                    if(kind==model_worker::response::partial)
                    {
                        if(work.emit(payload)!=error::success)throw error::cancelled;
                        erase_bytes(response);continue;
                    }
                    auto status=work.finish(kind==model_worker::response::succeeded?models::state::succeeded:models::state::failed,payload);
                    if(status!=error::success)work.finish(models::state::failed);
                    return true;
                }
            }
            catch(...)
            {
                packet_.erase();
                if(!disclosed)work.finish(work.cancellation_requested()?models::state::cancelled:models::state::failed);
                // Unfinished work destruction retains outcome_unknown. No kill,
                // waitpid or disconnect can substitute for external evidence.
                return false;
            }
        }
        static void* run(void* opaque)noexcept
        {
            auto& self=*static_cast<service*>(opaque);
            try
            {
                auto status=self.begin();self.startup_.store(status,std::memory_order_release);
                if(status==error::success)while(!self.stopped_.load(std::memory_order_acquire))
                {
                    model_dispatch::work work;status=self.dispatcher_.claim(self.alias_,self.generation_,work);
                    if(status==error::again)
                    {
                        if(now()>=self.limits_.session_until)break;
                        std::size_t size{};status=self.channel_.receive(self.packet_.writable(),size);
                        if(status!=error::again)break; // unsolicited packet, peer exit or protocol violation
                        self.pause();continue;
                    }
                    if(status!=error::success||!self.execute(work))break;
                }
            }
            catch(std::bad_alloc const&){self.startup_.store(error::no_memory,std::memory_order_release);}
            catch(...){self.startup_.store(error::io,std::memory_order_release);}
            std::array<std::uint8_t,protocol::header_bytes> header{};
            if(protocol::encode({protocol::kind::release,self.binding_},0,header))(void)self.channel_.send(header,{});
            self.channel_.close();self.packet_.erase();
            try{(void)self.dispatcher_.detach(self.alias_,self.generation_);}catch(...){}
            return nullptr;
        }
    public:
        service(model_dispatch& dispatcher,std::string_view alias,std::uint64_t generation,models::plan plan,
                protocol::binding binding,limits capacity):dispatcher_{dispatcher},alias_{alias},generation_{generation},
                plan_{std::move(plan)},binding_{binding},limits_{capacity}{}
        ~service()override{retire();}
        /** @brief Duplicate a registered channel and attach a matching live provider.
         * @details This creates a local supervisor thread only. The peer must
         * already exist; failure closes private descriptors and joins local
         * work without signalling, reaping or moving the peer process.
         */
        static error prepare(model_dispatch& dispatcher,std::string_view alias,std::uint64_t generation,int fd,
            drivers::credentials peer,protocol::binding binding,limits capacity,std::shared_ptr<models::provider_lifetime>& output)noexcept
        {
            if(output||alias.empty()||!generation||!protocol::valid(binding)||!valid(capacity))return error::invalid;
            try
            {
                models::plan selected;auto status=dispatcher.limits(alias,selected);if(status!=error::success)return status;
                auto value=::uwvm2::utils::container::make_shared<service>(dispatcher,alias,generation,std::move(selected),binding,capacity);
                status=value->channel_.bind(fd,peer,capacity.tls_identity_fd,attachment_tls::purpose::model,
                    binding.deployment,binding.vm,binding.scope,binding.provider,binding.session,binding.contract,transport_frame(capacity),false,capacity.peer_time_namespace_fd);
                if(status!=error::success)return status;
                value->packet_.resize_for_overwrite(protocol::header_bytes+std::min<std::uint64_t>(protocol::fragment_bytes,std::max<std::uint64_t>(capacity.response_bytes,1024)));
                status=value->stack_.prepare(capacity.supervisor_bytes);if(status!=error::success)return status;
                pthread_attr_t attributes;if(::pthread_attr_init(&attributes))return error::io;
                int created=::pthread_attr_setstack(&attributes,value->stack_.data(),value->stack_.size());
                if(!created)created=::pthread_create(&value->thread_,&attributes,run,value.get());
                ::pthread_attr_destroy(&attributes);if(created)return error::io;value->started_=true;
                while((status=value->startup_.load(std::memory_order_acquire))==error::again)value->pause();
                if(status!=error::success){value->retire();return status;}
                output=std::move(value);return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        /** @brief Cancel local transport only; safe while the authority lock is held. */
        void request_stop()noexcept override{if(same_process())stopped_.store(true,std::memory_order_release);}
        /** @brief Join only this VM's thread; never assert external process cleanup. */
        void retire()noexcept override
        {if(!same_process())return;request_stop();std::lock_guard lock{retirement_};if(started_&&!joined_&&!::pthread_join(thread_,nullptr))joined_=true;}
    };
#endif
}
