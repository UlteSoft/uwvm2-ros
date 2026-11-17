/** @file
 * @brief Canonical transform admission with live clock ownership and atomic rates.
 * @details This local provider has no ROS subscription, socket or native TF
 * authority. Guests publish into a bounded private profile. Opening and every
 * fresh query/publication hold both origin and the explicitly supplied clock
 * capability; clock.read is a validation dependency, not a new clock creation.
 * The actual clock resource is sampled by its admitted alias, so numeric clock
 * identifiers alone never establish a domain or epoch. Cancellation, release
 * and historical mutation-receipt recovery require origin authority only and
 * remain possible after a source clock becomes unavailable.
 *
 * Inputs are complete host-owned canonical records. Large guest copies reserve
 * separate carrier capacity before allocation. Short output refuses before
 * mutation or charging; mutations reserve a documented bounded receipt, while
 * observations charge actual encoded output. Failed mutation copies recover
 * by exact canonical digest/key. Query delivery rechecks the complete authority
 * group and actual clock/epoch after copying, before consuming its result.
 */
#pragma once
#include "authority.h"
#include "transform_registry.h"
#include "byte_buffer.h"

namespace uwvm2::imported::ros::scene
{
    class transform_dispatch
    {
        std::shared_ptr<authority> permissions_;
        resources::registry& clocks_;
        transforms::registry store_;
#if defined(__linux__)
        ::pid_t process_{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        static bool selected(grant const& value) noexcept
        {return value.kind==purpose::transform&&value.binding=="local-transform";}
        static bool clock_grant(grant const& value) noexcept
        {return value.binding=="local"&&(value.kind==purpose::steady_clock||value.kind==purpose::system_clock||value.kind==purpose::ros_clock);}
        error sample(group_lease const& group,handle clock,transforms::clock_context& output) const
        {
            auto* spec=group.specification(1);if(!spec||!clock_grant(*spec))return error::denied;
            resources::clock_sample value;auto status=clocks_.clock_read(spec->alias,clock,value);if(status!=error::success)return status;
            output={clock,value.epoch,value.now,value.started};return error::success;
        }
        static std::size_t minimum(transform_contract::operation action) noexcept
        {
            using op=transform_contract::operation;
            switch(action)
            {
            case op::open:return 8;
            case op::lookup:return 622;
            case op::available:return 64;
            case op::velocity:return 48;
            case op::take:return 790;
            default:return 160;
            }
        }
    public:
        using bytes=std::span<std::uint8_t const>;
        using sink=::uwvm2::utils::container::function_ref<bool(bytes)>;
        using cancelled=::uwvm2::utils::container::function_ref<bool()>;
        transform_dispatch(std::shared_ptr<authority> authority,resources::registry& clocks):permissions_{std::move(authority)},clocks_{clocks}{}
        error bind(::uwvm2::utils::container::std_string alias,transforms::plan plan){return store_.bind(std::move(alias),std::move(plan));}
        error close(std::string_view alias,handle id){return store_.close(alias,id);}
        void revoke(std::string_view alias){store_.revoke(alias);}
        void retire() noexcept{store_.retire();}
        error inspect(std::string_view alias,transforms::usage& out) const noexcept{return store_.inspect(alias,out);}
        error preflight(caller who,transform_contract::operation action,handle capability,std::size_t extent,bool output_fits=true)
        {
            if(!same_process()||!permissions_)return error::denied;
            auto index=static_cast<std::size_t>(action);if(index>=transform_contract::operations.size())return error::unsupported;
            if(extent>transform_contract::request_bound(transform_contract::operation::publish,transform_contract::maximum_batch))return error::too_large;
            lease allowed;auto status=output_fits?permissions_->preflight(who,capability,transform_contract::operations[index].name,extent+8,allowed):
                permissions_->admit(who,capability,transform_contract::operations[index].name,0,allowed,false);
            if(status!=error::success)return status;
            auto const& spec=allowed.specification();if(!selected(spec))return error::denied;
            std::size_t bound{};buffers::profile profile;status=store_.limits(spec.alias,action,bound,profile);
            return status!=error::success?status:extent>bound?error::too_large:error::success;
        }
        error reserve_workspace(caller who,transform_contract::operation action,handle capability,std::size_t extent,transforms::registry::carrier_lease& out)
        {
            if(extent<=byte_buffer::inline_capacity)return error::invalid;
            auto status=preflight(who,action,capability,extent);if(status!=error::success)return status;
            lease allowed;status=permissions_->admit(who,capability,transform_contract::operations[static_cast<std::size_t>(action)].name,0,allowed,false);
            return status==error::success?store_.reserve_carrier(allowed.specification().alias,extent,out):status;
        }
        error invoke(caller who,transform_contract::operation action,handle capability,bytes input,std::size_t capacity,
                     std::size_t& required,sink deliver,cancelled cancellation={})
        {
            using op=transform_contract::operation;required=0;
            auto status=preflight(who,action,capability,input.size(),capacity>=minimum(action));if(status!=error::success)return status;
            transform_contract::request request;if(!transform_contract::decode(action,input,request))return error::invalid;
            if(!deliver)return error::invalid;
            required=minimum(action);if(capacity<required)return error::too_large;
            if(cancellation&&cancellation())return error::cancelled;
            auto name=transform_contract::operations[static_cast<std::size_t>(action)].name;
            lease origin;status=permissions_->admit(who,capability,name,0,origin,false);if(status!=error::success)return status;
            auto const& spec=origin.specification();if(!selected(spec))return error::denied;
            std::array<std::uint8_t,1024> output{};
            auto simple_emit=[&](bytes encoded)
            {
                required=encoded.size();if(cancellation&&cancellation())return false;
                lease before_copy;if(permissions_->admit(who,capability,name,0,before_copy,false)!=error::success)return false;
                if(!deliver(encoded)||(cancellation&&cancellation()))return false;
                lease current;return permissions_->admit(who,capability,name,0,current,false)==error::success;
            };
            auto receipt_out=[&](transforms::receipt receipt)
            {wire::writer writer{output};return transforms::encode_receipt(writer,receipt)&&simple_emit(writer.written())?error::success:error::io;};
            std::shared_ptr<transforms::session> session;
            if(action!=op::open)
            {status=store_.borrow(spec.alias,request.buffer,session);if(status!=error::success)return status;}
            auto key=transforms::request_key{request.key_session,request.key_sequence};buffers::digest digest{};
            if(action==op::publish||action==op::submit||action==op::clear)
            {
                digest=buffers::hash(input);bool replay{};transforms::receipt receipt;
                status=session->replay(action,key,digest,replay,receipt);if(status!=error::success)return status;
                if(replay)
                {
                    wire::writer writer{output};if(!transforms::encode_receipt(writer,receipt))return error::invalid;
                    lease charged;status=permissions_->admit(who,capability,name,input.size()+writer.size(),charged);
                    return status==error::success?receipt_out(receipt):status;
                }
            }
            if(action==op::cancel)
            {
                lease charged;status=permissions_->admit(who,capability,name,input.size()+160,charged);if(status!=error::success)return status;
                transforms::receipt receipt;status=session->cancel(request.request_id,receipt);return status==error::success?receipt_out(receipt):status;
            }
            handle clock_capability=request.clock_capability,clock=request.clock;
            if(session)
            {
                transforms::session_usage info;status=session->inspect(info);if(status!=error::success)return status;
                clock_capability=info.clock_capability;clock=info.clock;
            }
            std::array<admission_claim,3> claims{};std::size_t count=2;
            claims[0]={capability,name,input.size()+minimum(action)};
            claims[1]={clock_capability,"clock.read",0,false,true};
            bool replace_static=action==op::publish&&spec.actions.contains("transform.static.replace");
            if(replace_static){claims[count++]={capability,"transform.static.replace",0,true,true};}
            auto active_claims=std::span<admission_claim>{claims.data(),count};group_lease group;
            status=permissions_->admit_group(who,active_claims,group,false);if(status!=error::success)return status;
            transforms::clock_context current;status=sample(group,clock,current);if(status!=error::success)return status;
            if(!current.started)return error::again;
            error delivery_error=error::io;
            auto emit=[&](bytes encoded)
            {
                required=encoded.size();if(cancellation&&cancellation()){delivery_error=error::cancelled;return false;}
                // The cancellation hook can reenter authority or the clock.
                // Revalidate the complete provenance before disclosing a result.
                group_lease before_copy;delivery_error=permissions_->admit_group(who,active_claims,before_copy,false);
                if(delivery_error!=error::success)return false;
                transforms::clock_context before;delivery_error=sample(before_copy,clock,before);if(delivery_error!=error::success)return false;
                if(!before.started||before.epoch!=current.epoch){delivery_error=error::conflict;return false;}
                delivery_error=error::io;
                if(!deliver(encoded))return false;
                if(cancellation&&cancellation()){delivery_error=error::cancelled;return false;}
                group_lease checked;delivery_error=permissions_->admit_group(who,active_claims,checked,false);if(delivery_error!=error::success)return false;
                transforms::clock_context after;delivery_error=sample(checked,clock,after);if(delivery_error!=error::success)return false;
                if(!after.started||after.epoch!=current.epoch){delivery_error=error::conflict;return false;}
                delivery_error=error::io;return true;
            };
            auto charge=[&](std::size_t reply)
            {claims[0].bytes=input.size()+reply;return permissions_->admit_group(who,active_claims,group);};
            switch(action)
            {
            case op::open:
            {
                status=charge(8);if(status!=error::success)return status;
                status=store_.open(spec.alias,request.profile,clock_capability,current,spec.max_resources,[&](handle id)
                {wire::writer writer{output};return writer.u64(id)&&emit(writer.written());});
                return status==error::io?delivery_error:status;
            }
            case op::lookup:case op::available:
            {
                transform_contract::transform value;status=session->lookup(request.lookup,current,value);wire::writer writer{output};
                if(action==op::lookup)
                {if(status!=error::success)return status;if(!transform_contract::write_transform(writer,value))return error::invalid;}
                else
                {
                    if(status!=error::success&&status!=error::again)return status;
                    if(!writer.boolean(status==error::success)||!writer.text(status==error::success?"":"missing_history"))return error::invalid;
                }
                status=charge(writer.size());return status==error::success?(emit(writer.written())?error::success:delivery_error):status;
            }
            case op::velocity:
            {
                transforms::math::velocity value;status=session->velocity(request.velocity,current,value);if(status!=error::success)return status;
                wire::writer writer{output};if(!transform_contract::write_vector(writer,value.linear)||!transform_contract::write_vector(writer,value.angular))return error::invalid;
                status=charge(writer.size());return status==error::success?(emit(writer.written())?error::success:delivery_error):status;
            }
            case op::take:
            {
                error copying=error::success;
                status=session->take(request.request_id,current,[&](transforms::receipt const& receipt,transform_contract::transform const* value)
                {
                    wire::writer writer{output};if(!transforms::encode_receipt(writer,receipt)||!writer.boolean(value!=nullptr)||
                        (value&&!transform_contract::write_transform(writer,*value))){copying=error::invalid;return false;}
                    copying=charge(writer.size());return copying==error::success&&emit(writer.written());
                });
                return copying!=error::success?copying:status==error::io?delivery_error:status;
            }
            case op::publish:case op::submit:case op::clear:
            {
                status=charge(160);if(status!=error::success)return status;transforms::receipt receipt;
                if(action==op::publish)status=session->publish(key,digest,{request.transforms.data(),request.count},current,replace_static,receipt,request.expected_generation);
                else if(action==op::submit)status=session->submit(key,digest,request.lookup,request.deadline,current,receipt);
                else status=session->clear(key,digest,request.expected_generation,current,receipt);
                if(status!=error::success)return status;
                wire::writer writer{output};return transforms::encode_receipt(writer,receipt)&&emit(writer.written())?error::success:delivery_error;
            }
            default:return error::unsupported;
            }
        }
        /** @brief The generic request carriers share this exact transform alias only. */
        error preflight_request(caller who,bool release,handle capability,std::size_t size,bool output_fits=true)
        {
            if(!same_process()||!permissions_)return error::denied;
            if(size!=8)return error::invalid;
            auto name=release?"request.release":"request.describe";lease origin;
            auto status=output_fits?permissions_->preflight(who,capability,name,size+8,origin):permissions_->admit(who,capability,name,0,origin,false);
            return status!=error::success?status:selected(origin.specification())?error::success:error::denied;
        }
        error invoke_request(caller who,bool release,handle capability,bytes input,std::size_t capacity,std::size_t& required,sink deliver,cancelled cancellation={})
        {
            required=0;auto limit=release?8u:160u;auto status=preflight_request(who,release,capability,input.size(),capacity>=limit);if(status!=error::success)return status;
            wire::reader reader{input};handle request{};if(!reader.u64(request)||!request||!reader.done()||!deliver)return error::invalid;
            required=limit;if(capacity<limit)return error::too_large;if(cancellation&&cancellation())return error::cancelled;
            auto name=release?"request.release":"request.describe";lease origin;status=permissions_->admit(who,capability,name,0,origin,false);if(status!=error::success)return status;
            auto const& spec=origin.specification();std::shared_ptr<transforms::session> session;status=store_.borrow_request(spec.alias,request,session);
            if(status!=error::success&&!(release&&status==error::bad_handle))return status;
            std::array<std::uint8_t,160> output{};wire::writer writer{output};
            if(release)
            {
                lease charged;status=permissions_->admit(who,capability,name,16,charged);if(status!=error::success)return status;
                status=session?session->release(request):error::bad_handle;
                if(status!=error::success&&status!=error::bad_handle)return status;
                if(!writer.boolean(status==error::success))return error::invalid;
            }
            else
            {
                lease charged;status=permissions_->admit(who,capability,name,input.size()+limit,charged);if(status!=error::success)return status;
                transforms::receipt receipt;status=session->receipt_for(request,receipt);if(status!=error::success)return status;
                if(!transforms::encode_receipt(writer,receipt))return error::invalid;
            }
            required=writer.size();if(cancellation&&cancellation())return error::cancelled;
            lease before_copy;status=permissions_->admit(who,capability,name,0,before_copy,false);if(status!=error::success)return status;
            if(!deliver(writer.written())||(cancellation&&cancellation()))return error::io;
            lease final;status=permissions_->admit(who,capability,name,0,final,false);if(status!=error::success)return status;
            return error::success;
        }
    };
}
