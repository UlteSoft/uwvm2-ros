/** @file
 * @brief Atomic model/buffer admission and authority-fenced native worker access.
 * @details Native computation runs outside authority locks. Every bounded
 * input borrow and prediction publication reacquires the complete originating
 * capability set. Worker objects retain both authority and the actual job;
 * they cannot outlive either allocation or silently keep using revoked data.
 * No remote/export authority, native plugin loader or guest callback is added.
 */
#pragma once
#include "authority.h"
#include "model_contract.h"
#include "byte_buffer.h"
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/container/function_ref.h>

namespace uwvm2::imported::ros::scene
{
    class model_dispatch
    {
        struct admission_record
        {
            caller origin;
            std::array<admission_claim,65> claims{};
            std::size_t count{};
            handle request{};
            ::uwvm2::utils::container::std_string alias;
            std::span<admission_claim const> view() const noexcept{return {claims.data(),count};}
        };
        std::shared_ptr<authority> permissions;
        buffers::registry& inputs;
        models::registry jobs;
        std::mutex records_mutex;
        using records_type=::uwvm2::utils::container::std_map<handle,std::shared_ptr<admission_record>>;
        records_type records;
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
        static bool model_grant(grant const& value) noexcept
        {return (value.kind==purpose::language_model||value.kind==purpose::vision_model||value.kind==purpose::world_model)&&value.binding=="local-model";}
        std::shared_ptr<admission_record> record(std::string_view alias,handle request)
        {
            std::lock_guard lock{records_mutex};auto it=records.find(request);
            return it==records.end()||it->second->alias!=alias?nullptr:it->second;
        }
        static constexpr std::size_t maximum_request=4u*1024u*1024u+12288u;
        static constexpr std::size_t maximum_output=4u*1024u*1024u+512u;
    public:
        using bytes=std::span<std::uint8_t const>;
        using sink=::uwvm2::utils::container::function_ref<bool(bytes)>;
        /** @brief Borrowed authenticated final output, constructible only by this dispatcher.
         * @details It exists only during with_completed_output's synchronous
         * callback. It is not copyable, a guest token, or permission to execute
         * a target. Consumers decode the complete bounded proposal envelope and
         * compare every tool/schema/target/frame field with immutable policy.
         * Revalidate after any callback and immediately before publishing the
         * inert proposal; no mutable native callback may intervene afterwards.
         */
        class completion
        {
            friend class model_dispatch;
            model_dispatch& parent;
            admission_record const& admitted;
            std::span<models::input_evidence const> inputs;
            bytes final;
            models::plan const& selected;
            completion(model_dispatch& parent,admission_record const& admitted,std::span<models::input_evidence const> inputs,bytes final,models::plan const& selected) noexcept
                :parent(parent),admitted(admitted),inputs(inputs),final(final),selected(selected){}
        public:
            completion(completion const&)=delete;completion& operator=(completion const&)=delete;
            bytes output() const noexcept{return final;}
            handle request() const noexcept{return admitted.request;}
            std::string_view alias() const noexcept{return admitted.alias;}
            caller origin() const noexcept{return admitted.origin;}
            models::plan const& policy() const noexcept{return selected;}
            /** @brief Repeat exact authority and final/input evidence checks; no quota charge. */
            error revalidate() const
            {
                if(!parent.same_process())return error::denied;
                group_lease held;auto status=parent.permissions->admit_group(admitted.origin,admitted.view(),held,false);
                return status==error::success?parent.jobs.check_completion(admitted.alias,admitted.request,inputs,final):status;
            }
            bool matches_inputs(std::span<models::input_evidence const> expected) const noexcept
            {return inputs.size()==expected.size()&&std::equal(inputs.begin(),inputs.end(),expected.begin());}
        };
        /** @brief Adopt completed structured output under the original model/source authority.
         * @details Registry ownership stays private. The callback is bounded,
         * synchronous and may only validate/copy/adopt inert data, never perform
         * inference, wait for hardware, grant rights or invoke a robot target.
         * Success authenticates exactly the complete final bytes and original
         * input evidence; emitted streaming chunks have no fallback path.
         * A consumer must retain actual snapshot pins independently and may not
         * retain the borrowed completion object or its views after return.
         */
        template<typename Consumer> error with_completed_output(std::string_view alias,handle request,
            std::span<models::input_evidence const> expected,bytes final_bytes,Consumer&& consume)
        {
            if(!same_process()||!permissions)return error::denied;
            try
            {
                auto source=record(alias,request);if(!source)return error::bad_handle;
                group_lease held;auto status=permissions->admit_group(source->origin,source->view(),held,false);if(status!=error::success)return status;
                models::plan policy;status=jobs.limits(alias,policy);if(status!=error::success)return status;
                status=jobs.check_completion(alias,request,expected,final_bytes);if(status!=error::success)return status;
                completion verified{*this,*source,expected,final_bytes,policy};return consume(std::as_const(verified));
            }
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
            catch(...){return error::io;}
        }
        /** @brief Worker facade; raw job access is deliberately private.
         * @details Visitors are bounded synchronous copies/inspection only;
         * they may not retain borrowed spans, wait for a GPU or perform model
         * computation while the lease is held. Copy into a separately reserved
         * native workspace, release the visitor, then compute. Cancellation
         * cannot recall bytes already disclosed; output still requires every
         * current scope. finish() is called only after actual native cleanup.
         */
        class work
        {
            friend class model_dispatch;
            std::shared_ptr<authority> permissions;
            std::shared_ptr<admission_record const> admitted;
            models::registry::work value;
#if defined(__linux__)
            ::pid_t process{::getpid()};
#endif
            error revalidate(group_lease& lease) const
            {
#if defined(__linux__)
                if(process!=::getpid())return error::denied;
#endif
                return permissions&&admitted?permissions->admit_group(admitted->origin,admitted->view(),lease,false):error::denied;
            }
        public:
            work()=default;work(work const&)=delete;work& operator=(work const&)=delete;
            work(work&&)=default;work& operator=(work&&)=default;
            explicit operator bool() const noexcept{return static_cast<bool>(value);}
            handle identity() const noexcept{return value.identity();}
            bool cancellation_requested() const
            {group_lease lease;return revalidate(lease)!=error::success||value.cancellation_requested();}
            template<typename Visitor> error with_input(std::size_t index,Visitor&& visitor) const
            {group_lease lease;auto status=revalidate(lease);return status==error::success?value.with_input(index,std::forward<Visitor>(visitor)):status;}
            template<typename Visitor> error with_parameters(Visitor&& visitor) const
            {group_lease lease;auto status=revalidate(lease);return status==error::success?value.with_parameters(std::forward<Visitor>(visitor)):status;}
            /** @brief Reserve native copies from the already admitted job workspace.
             * @details Declare the token before its byte allocation so actual
             * bytes are destroyed before capacity is refunded. No borrowing
             * visitor may retain a source span as a substitute for this copy.
             */
            error reserve_scratch(std::uint64_t bytes,models::registry::scratch_lease& output) const
            {group_lease lease;auto status=revalidate(lease);return status==error::success?value.reserve_scratch(bytes,output):status;}
            /** @brief Hold current authority across one nonblocking external disclosure.
             * @details The trusted visitor must perform only one bounded,
             * nonblocking send. It must not wait, retain a lease, invoke model
             * code or reenter the dispatcher. Polling happens after return.
             * This closes the revocation gap between copying and transport.
             */
            template<typename Visitor> error transfer(Visitor&& visitor) const
            {
                group_lease lease;auto status=revalidate(lease);
                if(status!=error::success)return status;
                if(value.cancellation_requested())return error::cancelled;
                return visitor();
            }
            /** @brief Copy immutable worker framing metadata under current authority. */
            error metadata(std::size_t& input_count,std::uint64_t& deadline) const
            {
                group_lease lease;auto status=revalidate(lease);if(status!=error::success)return status;
                input_count=value.input_count();deadline=value.deadline();return error::success;
            }
            error emit(bytes output)
            {group_lease lease;auto status=revalidate(lease);return status==error::success?value.emit(output):status;}
            error finish(models::state outcome,bytes output={})
            {
                group_lease lease;
                if(revalidate(lease)!=error::success)return value.finish(models::state::cancelled);
                return value.finish(outcome,output);
            }
        };
        model_dispatch(std::shared_ptr<authority> owner,buffers::registry& buffers):permissions{std::move(owner)},inputs{buffers}{}
        model_dispatch(model_dispatch const&)=delete;model_dispatch& operator=(model_dispatch const&)=delete;
        ~model_dispatch(){retire();}
        /** @brief Trusted inert binding; worker attachment is separate and explicit. */
        error bind(::uwvm2::utils::container::std_string alias,models::plan plan){return jobs.bind(std::move(alias),plan);}
        error attach(std::string_view alias,std::uint64_t generation){return jobs.attach(alias,generation);}
        error detach(std::string_view alias,std::uint64_t generation){return jobs.detach(alias,generation);}
        error reconcile(std::string_view alias,handle request,std::uint64_t generation,models::state outcome)
        {return jobs.reconcile(alias,request,generation,outcome);}
        error limits(std::string_view alias,models::plan& output) const{return jobs.limits(alias,output);}
        error try_inspect(std::string_view alias,models::usage& output) const noexcept{return jobs.try_inspect(alias,output);}
        error close(std::string_view alias,handle session){return jobs.close(alias,session);}
        void revoke(std::string_view alias){jobs.revoke(alias);}
        void retire() noexcept{jobs.retire();}
        /** @brief Claim a real job with its already committed authorization record.
         * @details The record lock serializes publication with worker claim,
         * but is released before any authority acquisition or provider work.
         * An absent record fails closed and acknowledges stopped local work.
         */
        error claim(std::string_view alias,std::uint64_t generation,work& output)
        {
            if(!same_process())return error::denied;
            if(output)return error::conflict;
            work pending;
            {
                std::lock_guard lock{records_mutex};
                auto status=jobs.claim(alias,generation,pending.value);if(status!=error::success)return status;
                auto it=records.find(pending.value.identity());
                if(it==records.end()||it->second->alias!=alias)
                {pending.value.finish(models::state::cancelled);return error::conflict;}
                pending.permissions=permissions;pending.admitted=it->second;
            }
            if(pending.cancellation_requested())
            {pending.finish(models::state::cancelled);return error::cancelled;}
            output=std::move(pending);return error::success;
        }
        /** @brief Exact action/extent preflight before copying a guest carrier. */
        error preflight(caller who,model_contract::operation action,handle cap,std::size_t extent,bool complete=true)
        {
            if(!same_process())return error::denied;
            auto index=static_cast<std::size_t>(action);if(index>=model_contract::operations.size())return error::unsupported;
            if(!permissions)return error::denied;
            if(extent>maximum_request)return error::too_large;
            lease checked;auto status=complete?permissions->preflight(who,cap,model_contract::operations[index].name,extent+8,checked):
                permissions->admit(who,cap,model_contract::operations[index].name,0,checked,false);
            if(status!=error::success)return status;
            auto const& spec=checked.specification();if(!model_grant(spec))return error::denied;
            models::plan plan;status=jobs.limits(spec.alias,plan);if(status!=error::success)return status;
            return extent<=model_contract::request_bound(action,plan.parameter_bytes,plan.input_count)?error::success:error::too_large;
        }
        /** @brief Reserve large input bytes while exact model authority is held. */
        error reserve_workspace(caller who,model_contract::operation action,handle cap,std::size_t extent,models::registry::workspace_lease& output)
        {
            if(!same_process())return error::denied;
            if(!permissions)return error::denied;
            if(extent<=byte_buffer::inline_capacity||extent>maximum_request)return error::invalid;
            auto index=static_cast<std::size_t>(action);if(index>=model_contract::operations.size())return error::unsupported;
            lease checked;auto status=permissions->preflight(who,cap,model_contract::operations[index].name,extent+8,checked);
            if(status!=error::success)return status;
            auto const& spec=checked.specification();if(!model_grant(spec))return error::denied;
            models::plan plan;status=jobs.limits(spec.alias,plan);if(status!=error::success)return status;
            if(extent>model_contract::request_bound(action,plan.parameter_bytes,plan.input_count))return error::too_large;
            return jobs.reserve_workspace(spec.alias,extent,output);
        }
        /** @brief Dispatch actual model jobs with checked result delivery.
         * @details submit reserves all origin/input rates atomically and moves
         * actual sealed leases. Exact replay returns only the retained admission
         * receipt, charges the origin carrier, and does not acquire or charge
         * input authority again. It remains recoverable after source revocation
         * or request.release; it grants no fresh input access or execution.
         * A short variable receipt reports the conservative 160-byte bound
         * before mutation; successful results return their exact encoded size.
         * take measures its actual chunk and charges only after capacity fits;
         * failed delivery does not consume the chunk. Every copied carrier is
         * owned by the host and prevalidated for guest-memory safety upstream.
         */
        error invoke(caller who,model_contract::operation action,handle cap,bytes input,std::size_t capacity,
                     std::size_t& required,sink deliver,::uwvm2::utils::container::function_ref<bool()> cancelled={})
        {
            using op=model_contract::operation;required=0;
            if(capacity>maximum_output)return error::invalid;
            auto status=preflight(who,action,cap,input.size(),capacity>=8);if(status!=error::success)return status;
            model_contract::request request;if(!model_contract::decode(action,input,request))return error::invalid;
            auto const& descriptor=model_contract::operations[static_cast<std::size_t>(action)];
            lease allowed;status=permissions->admit(who,cap,descriptor.name,0,allowed,false);if(status!=error::success)return status;
            auto const& spec=allowed.specification();models::plan plan;
            status=jobs.limits(spec.alias,plan);if(status!=error::success)return status;
            std::array<std::uint8_t,512> output{};
            std::shared_ptr<admission_record> disclosure_origin;
            auto revalidate_disclosure=[&](lease& primary,group_lease& sources)
            {
                auto status=permissions->admit(who,cap,descriptor.name,0,primary,false);
                if(status!=error::success)return status;
                return disclosure_origin?permissions->admit_group(disclosure_origin->origin,disclosure_origin->view(),sources,false):error::success;
            };
            // Cancellation visitors may revoke a grant while returning false.
            // Fresh output needs both the current reader and original sources;
            // inert cleanup/replay retains its explicit primary-only semantics.
            auto emit=[&](bytes bytes)
            {
                required=bytes.size();if(cancelled&&cancelled())return false;
                lease current;group_lease sources;
                if(revalidate_disclosure(current,sources)!=error::success||!deliver(bytes))return false;
                if(cancelled&&cancelled())return false;
                lease after;group_lease after_sources;
                return revalidate_disclosure(after,after_sources)==error::success;
            };
            auto receipt_out=[&](models::receipt const& receipt)
            {wire::writer encoded{output};return model_contract::encode_receipt(encoded,receipt)&&emit(encoded.written())?error::success:error::io;};
            if(action==op::take)
            {
                auto authorization=record(spec.alias,request.request_id);if(!authorization)return error::bad_handle;
                group_lease sources;status=permissions->admit_group(authorization->origin,authorization->view(),sources,false);if(status!=error::success)return status;
                disclosure_origin=authorization;
                error failure=error::success;
                status=jobs.take(spec.alias,request.session,request.request_id,[&](models::receipt const& receipt,models::output const* chunk)
                {
                    auto measure=wire::writer::measuring(maximum_output);
                    if(!model_contract::encode_take(measure,request.session,receipt,chunk)){failure=error::too_large;return false;}
                    required=measure.size();if(capacity<required){failure=error::too_large;return false;}
                    lease charged;failure=permissions->admit(who,cap,descriptor.name,input.size()+required,charged);if(failure!=error::success)return false;
                    if(cancelled&&cancelled()){failure=error::cancelled;return false;}
                    models::registry::workspace_lease reservation;
                    if(required>byte_buffer::inline_capacity)
                    {failure=jobs.reserve_workspace(spec.alias,required,reservation);if(failure!=error::success)return false;}
                    byte_buffer copy;copy.resize_for_overwrite(required);wire::writer encoded{copy.writable()};
                    if(!model_contract::encode_take(encoded,request.session,receipt,chunk)){failure=error::invalid;return false;}
                    return emit(encoded.written());
                });
                return failure==error::success?status:failure;
            }
            if(action==op::describe)
            {
                models::profile model,task;bool streaming{};
                status=jobs.describe(spec.alias,request.session,model,task,streaming);if(status!=error::success)return status;
                wire::writer encoded{output};if(!model_contract::encode_description(encoded,model,task,streaming))return error::invalid;
                required=encoded.size();if(capacity<required)return error::too_large;
                lease charged;status=permissions->admit(who,cap,descriptor.name,input.size()+required,charged);if(status!=error::success)return status;
                return emit(encoded.written())?error::success:error::io;
            }
            required=action==op::open||action==op::request_release?8:160;
            if(capacity<required)return error::too_large;
            if(cancelled&&cancelled())return error::cancelled;
            if(action==op::submit)
            {
                if(request.input_count>plan.input_count||request.selected!=plan.task||request.parameter_type!=plan.parameter_type||request.encoding!=plan.encoding)return error::denied;
                auto hash=buffers::hash(input);models::receipt receipt;bool replay{};
                status=jobs.probe_retry(spec.alias,request.session,request.key,hash,receipt,replay);if(status!=error::success)return status;
                if(replay)
                {
                    wire::writer encoded{output};
                    if(!model_contract::encode_receipt(encoded,receipt))return error::invalid;
                    lease charged;status=permissions->admit(who,cap,descriptor.name,input.size()+encoded.size(),charged);
                    return status==error::success?receipt_out(receipt):status;
                }
                std::array<admission_claim,65> claims{};claims[0]={cap,"model.submit",input.size()+160};
                for(std::size_t n{};n<request.input_count;++n)
                {
                    auto const& source=request.inputs[n];
                    if(source.source||source.source_generation||source.stamped||source.framed)return error::denied;
                    claims[n+1]={source.capability,"buffer.read",0};
                }
                auto claim_span=std::span<admission_claim>{claims.data(),request.input_count+1};
                group_lease all;status=permissions->admit_group(who,claim_span,all,false);if(status!=error::success)return status;
                for(std::size_t n{};n<request.input_count;++n)
                {auto* grant=all.specification(n+1);if(!grant||grant->kind!=purpose::buffer||grant->binding!="sealed-buffer")return error::denied;}
                for(std::size_t n{};n<request.input_count;++n)
                {
                    buffers::information info;status=inputs.describe(all.specification(n+1)->alias,request.inputs[n].buffer,info);
                    if(status!=error::success)return status;
                    if(!info.sealed||info.provenance!="guest")return error::denied;
                    claims[n+1].bytes=info.value.bytes;
                }
                group_lease charged;status=permissions->admit_group(who,claim_span,charged);if(status!=error::success)return status;
                auto saved=::uwvm2::utils::container::make_shared<admission_record>();saved->origin=who;saved->claims=claims;saved->count=claim_span.size();saved->alias=spec.alias;
                records_type prepared;prepared.emplace(0,saved);auto node=prepared.extract(0);
                ::uwvm2::utils::container::std_vector<buffers::registry::sealed_lease> pinned;pinned.reserve(request.input_count);
                for(std::size_t n{};n<request.input_count;++n)
                {
                    buffers::registry::sealed_lease held;status=inputs.pin(all.specification(n+1)->alias,request.inputs[n].buffer,held);
                    if(status!=error::success)return status;
                    pinned.emplace_back(std::move(held));
                }
                if(cancelled&&cancelled())return error::cancelled;
                group_lease current_sources;status=permissions->admit_group(who,claim_span,current_sources,false);if(status!=error::success)return status;
                {
                    std::lock_guard lock{records_mutex};
                    status=jobs.submit(spec.alias,request.session,request.key,hash,request.selected,request.parameters,pinned,request.reservation,request.deadline,receipt);
                    if(status!=error::success)return status;
                    saved->request=receipt.request;node.key()=receipt.request;records.insert(std::move(node));
                }
                disclosure_origin=saved;
                return receipt_out(receipt);
            }
            lease charged;status=permissions->admit(who,cap,descriptor.name,input.size()+required,charged);if(status!=error::success)return status;
            models::receipt receipt;
            switch(action)
            {
            case op::open:return jobs.open(spec.alias,request.selected,spec.max_resources,[&](handle id){wire::writer out{output};out.u64(id);return emit(out.written());});
            case op::cancel:status=jobs.cancel_request(spec.alias,request.session,request.request_id,receipt);break;
            case op::request_describe:status=jobs.request_describe(spec.alias,request.request_id,receipt);break;
            case op::request_release:
            {
                {
                    std::lock_guard lock{records_mutex};status=jobs.request_release(spec.alias,request.request_id);
                    if(status!=error::success&&status!=error::bad_handle)return status;
                    if(status==error::success)records.erase(request.request_id);
                }
                wire::writer out{output};out.boolean(status==error::success);return emit(out.written())?error::success:error::io;
            }
            default:return error::unsupported;
            }
            return status==error::success?receipt_out(receipt):status;
        }
    };
}
