/** @file
 * @brief Exact caller-bound node attachment to an externally managed ROS process.
 * @details Complete context/node authority precedes every handshake step.
 * The VM duplicates preprovisioned channels, checks peer identity and immutable
 * profiles, and fences only its own traffic. Native process lifetime belongs
 * entirely to deployment. Every disclosed failed attachment retains cleanup
 * debt; local disconnect or elapsed time never substitutes for remote evidence.
 */
#pragma once
#include "communication.h"
#include "communication_attachment.h"
#include <uwvm2/utils/container/standard.h>

namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    enum class stage_phase:std::uint8_t
    {unconfigured,prepared,identifying,waiting_ready,ready,waiting_attachment,attached,committed,retiring,retired,cleanup_unknown};
    struct stage_snapshot
    {stage_phase phase{};communication::error failure{};std::uint64_t launches{},diagnostic_bytes{};bool cleanup_debt{},identity_confirmed{};};
    class node_stage
    {
        using error=communication::error;
        using process=communication_attachment::connection;
        using descriptor=communication_attachment::descriptor;
        std::shared_ptr<process> worker;
        communication::node_plan plan;
        communication::node_stage_request identity;
        std::shared_ptr<communication::native_context> context;

        std::atomic<stage_phase> phase{stage_phase::unconfigured};
        std::atomic<bool> stopping{};
        error failure{error::ok};std::uint64_t launches{},diagnostic_bytes{};
        pid_t owner{::getpid()};
        static error convert(resources::error status) noexcept
        {
            switch(status)
            {
                case resources::error::success:return error::ok;
                case resources::error::again:return error::again;
                case resources::error::denied:return error::denied;
                case resources::error::cancelled:return error::cancelled;
                case resources::error::timeout:return error::timeout;
                case resources::error::no_memory:return error::no_memory;
                case resources::error::too_large:return error::too_large;
                case resources::error::unsupported:return error::unsupported;
                case resources::error::conflict:return error::stale;
                default:return error::io;
            }
        }
        bool matches(communication::node_plan const& value) const noexcept
        {return value.authority==plan.authority&&value.selected==plan.selected&&value.context==plan.context&&
            value.name==plan.name&&value.name_space==plan.name_space&&value.max_observers==plan.max_observers;}
        bool same_process()const noexcept{return owner==::getpid();}
        error fail(error status)noexcept
        {
            if(failure==error::ok)failure=status;
            request_stop();
            auto current=phase.load(std::memory_order_acquire);
            if(current!=stage_phase::retired&&current!=stage_phase::cleanup_unknown)
                phase.store(stage_phase::retiring,std::memory_order_release);
            return status;
        }
        bool drain()noexcept
        {
            if(!worker)return true;
            std::array<std::uint8_t,1024> output{};
            struct cleanup{std::span<std::uint8_t> bytes;~cleanup(){erase_bytes(bytes);}} clear{output};
            for(unsigned i{};i<4;++i)
            {
                auto got=::read(worker->stderr_fd(),output.data(),output.size());
                if(got>0)
                {auto n=static_cast<std::uint64_t>(got);if(n>worker->stderr_bytes()-diagnostic_bytes)return false;diagnostic_bytes+=n;continue;}
                if(!got)return true;
                if(errno==EINTR)continue;
                return errno==EAGAIN||errno==EWOULDBLOCK;
            }
            return true;
        }
        bool valid_identity(communication::node_stage_request const& request) const noexcept
        {
            return request.caller_instance&&request.caller_generation&&request.node_capability&&request.context_capability&&request.context&&
                static_cast<bool>(request.context_authority)&&request.context_authority.environment==plan.authority.environment;
        }
    public:
        node_stage()=default;node_stage(node_stage const&)=delete;node_stage& operator=(node_stage const&)=delete;
        /** @brief Duplicate a deployment-owned peer and its exact channels without sending data. */
        error configure(communication_attachment::configuration const& manifest,communication::node_plan const& selected)noexcept
        {
            if(!same_process()||phase.load()!=stage_phase::unconfigured||stopping.load())return error::denied;
            try
            {
                if(!communication::valid(selected.selected)||!communication::valid(selected.context)||!selected.authority.environment||
                    !selected.authority.grant||!selected.authority.generation||selected.name.empty()||selected.name.find('/')!=selected.name.npos||!communication::absolute_name('/'+selected.name)||
                    (selected.name_space!="/"&&!communication::absolute_name(selected.name_space))||!selected.max_observers)return error::invalid;
                auto candidate=::uwvm2::utils::container::make_shared<process>();auto owned=selected;
                auto status=convert(candidate->prepare(manifest));if(status!=error::ok)return status;
                worker=std::move(candidate);plan=std::move(owned);phase.store(stage_phase::prepared,std::memory_order_release);return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Advance one bounded attachment step after authority preflight; no process launch. */
        error prepare_node(communication::node_plan const& selected,std::shared_ptr<communication::native_context> const& retained,
                           communication::node_stage_request const& request)noexcept
        {
            if(!same_process())return error::denied;
            auto current=phase.load(std::memory_order_acquire);
            if(current==stage_phase::unconfigured||!matches(selected)||!valid_identity(request)||!retained)return error::denied;
            if(current!=stage_phase::prepared&&(request!=identity||retained!=context))return error::denied;
            if(stopping.load()||!retained->live())return fail(error::cancelled);
            if(current==stage_phase::prepared)
            {
                identity=request;context=retained;
                phase.store(stage_phase::identifying,std::memory_order_release);
            }
            if(!drain())return fail(error::too_large);
            if(current==stage_phase::prepared||current==stage_phase::identifying)
            {
                auto status=convert(worker->begin());if(status==error::again)return status;if(status!=error::ok)return fail(status);
                phase.store(stage_phase::ready,std::memory_order_release);return error::ok;
            }
            if(current==stage_phase::ready||current==stage_phase::attached)return error::ok;
            if(current==stage_phase::waiting_attachment)return error::again;
            return current==stage_phase::committed?error::busy:error::cancelled;
        }
        /** @brief Activate only the preprovisioned channels; no descriptor transfer occurs. */
        error attach()noexcept
        {
            if(!same_process()||phase.load()!=stage_phase::ready)return error::denied;
            if(stopping.load()||!context->live())return fail(error::cancelled);
            phase.store(stage_phase::waiting_attachment,std::memory_order_release);return attachment_progress();
        }
        /** @brief Progress exact peer acknowledgement without assuming native completion. */
        error attachment_progress()noexcept
        {
            if(!same_process()||phase.load()!=stage_phase::waiting_attachment)return error::denied;
            if(stopping.load()||!context->live())return fail(error::cancelled);
            if(!drain())return fail(error::too_large);
            auto status=convert(worker->activate_channel());if(status==error::again)return status;if(status!=error::ok)return fail(status);
            phase.store(stage_phase::attached,std::memory_order_release);return error::ok;
        }
        /** @brief Bind the final node only under a newly held complete authority group. */
        error commit(communication::node_plan const& selected,std::shared_ptr<communication::native_context> const& retained,
                     communication::node_stage_request const& request)noexcept
        {
            if(!same_process()||phase.load()!=stage_phase::attached||!matches(selected)||retained!=context||request!=identity)return error::denied;
            if(stopping.load()||!context->live())return fail(error::cancelled);
            pollfd live{worker->process_fd(),POLLIN,0};auto status=::poll(&live,1,0);
            if(status<0&&errno==EINTR)return error::again;
            if(status!=0||!worker->attachment_confirmed())return fail(error::cancelled);
            phase.store(stage_phase::committed,std::memory_order_release);return error::ok;
        }
        /** @brief Revoke without waiting; no future caller may adopt this incarnation. */
        void request_stop()noexcept
        {if(same_process()){stopping.store(true,std::memory_order_release);if(worker)worker->request_stop();}}
        /** @brief Close local duplicates after users drain; remote cleanup stays unknown.
         * @details Once hello was disclosed, neither a pidfd exit nor a socket
         * close proves that the provider's descendants, DDS effects or retained
         * copies are gone. Only external trusted cleanup evidence can reconcile
         * its ledger. This operation never waits or signals the peer.
         */
        error cleanup_progress()noexcept
        {
            if(!same_process()||!stopping.load())return error::denied;
            bool debt=worker&&worker->disclosed();if(worker)worker->disconnect();
            phase.store(debt?stage_phase::cleanup_unknown:stage_phase::retired,std::memory_order_release);
            return debt?error::busy:error::ok;
        }
        bool references_capability(communication::handle cap)const noexcept
        {return same_process()&&cap&&(cap==identity.node_capability||cap==identity.context_capability);}
        communication::owner authority()const noexcept{return plan.authority;}
        std::shared_ptr<process> retained_process()const noexcept{return same_process()?worker:nullptr;}
        stage_snapshot inspect()const noexcept
        {
            if(!same_process())return {stage_phase::cleanup_unknown,error::denied,0,0,true,false};
            auto current=phase.load(std::memory_order_acquire);
            return {current,failure,launches,diagnostic_bytes,worker&&worker->disclosed()&&current!=stage_phase::retired,worker&&worker->identity_confirmed()};
        }
    };
#endif
}
