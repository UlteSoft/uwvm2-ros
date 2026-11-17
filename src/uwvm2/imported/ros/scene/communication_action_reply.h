/** @file
 * @brief Decode isolated action events before resolving any native identity.
 * @details The complete canonical record must match the sealed endpoint,
 * worker incarnation, role and all three schemas. UUIDs are protocol data, never
 * authority. Returned spans borrow the caller's owned reply buffer; this decoder
 * performs no allocation, I/O or state change. Sequence freshness and host/native
 * correlation ownership are independently checked by the receiving adapter.
 */
#pragma once
#include "communication_action_contract.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
    struct action_record
    {
        communication::metadata information;
        communication::handle native_goal{},native_request{};
        communication::actions::event_kind kind{};
        communication::actions::state observed{};
        communication::actions::uuid identity{};
        wire::bytes payload;
    };
    /** @brief Validate one complete worker event without accepting unscoped clock data. */
    inline bool decode_action_record(wire::bytes body,communication::actions::plan const& policy,
                                    communication::handle native_endpoint,std::uint64_t generation,action_record& output)noexcept
    {
        namespace action=communication::actions;namespace contract=::uwvm2::imported::ros::action_contract;
        if(!native_endpoint||!generation||policy.direction>action::role::server||body.size()>4u*1024u*1024u)return false;
        wire::reader input{body};action_record value;auto& info=value.information;bool present{};
        // The private worker has no authority to mint a host clock/epoch or
        // endpoint identity. The current action adapter emits neither optional field.
        if(!input.u64(info.source)||info.source!=native_endpoint||!input.u64(info.generation)||info.generation!=generation||
            !input.u64(info.sequence)||!info.sequence||!input.boolean(present)||present||!input.boolean(present)||present||
            !input.boolean(present)||present||!input.u64(info.loss_before)||!input.u64(value.native_request)||!value.native_request)return false;
        std::string_view event,type,encoding;wire::bytes encoded;
        if(!input.text(event)||!input.text(type)||type!="uwvm/action/Event"||!input.text(encoding)||encoding!="scene-canonical"||
            !input.blob(encoded)||!input.done())return false;
        auto found=std::find(contract::event_names.begin(),contract::event_names.end(),event);
        if(found==contract::event_names.end())return false;
        value.kind=static_cast<action::event_kind>(found-contract::event_names.begin()+1);
        bool server_event=value.kind==action::event_kind::proposal||value.kind==action::event_kind::cancel_request;
        if(server_event!=(policy.direction==action::role::server))return false;
        wire::reader nested{encoded};wire::bytes uuid;std::uint64_t state{};
        if(!nested.u64(value.native_goal)||!value.native_goal||!nested.blob(uuid)||uuid.size()!=16||
            std::none_of(uuid.begin(),uuid.end(),[](auto byte){return byte!=0;})||!nested.u64(state)||state>static_cast<std::uint64_t>(action::state::rejected)||
            !nested.boolean(present))return false;
        value.observed=static_cast<action::state>(state);std::copy(uuid.begin(),uuid.end(),value.identity.begin());
        auto schema=value.kind==action::event_kind::proposal?&policy.goal:value.kind==action::event_kind::feedback?&policy.feedback:
            value.kind==action::event_kind::result?&policy.result:nullptr;
        if(schema)
        {
            auto bound=value.kind==action::event_kind::proposal?policy.max_goal_bytes:value.kind==action::event_kind::feedback?policy.max_feedback_bytes:policy.max_result_bytes;
            if(!present||!schema->validate||!nested.text(type)||type!=schema->type_identity||!nested.text(encoding)||encoding!=schema->encoding||
                !nested.blob(value.payload)||value.payload.size()>bound||!schema->validate(value.payload))return false;
        }
        else if(value.kind==action::event_kind::cancel_response)
        {
            if(!present||!nested.text(type)||type!="uwvm/action/CancelResponse"||!nested.text(encoding)||encoding!="scene-canonical"||
                !nested.blob(value.payload))return false;
            wire::reader response{value.payload};std::uint64_t code{};
            if(!response.u64(code)||code>3||!response.done()||(code==0&&value.observed!=action::state::cancelling))return false;
        }
        else if(present)return false;
        if(!nested.done())return false;
        if(value.kind==action::event_kind::proposal&&value.observed!=action::state::pending)return false;
        if(value.kind==action::event_kind::acceptance&&value.observed!=action::state::accepted&&value.observed!=action::state::rejected)return false;
        if(value.kind==action::event_kind::cancel_request&&value.observed!=action::state::accepted&&value.observed!=action::state::executing)return false;
        if(value.kind==action::event_kind::result&&(!action::terminal(value.observed)||value.observed==action::state::rejected))return false;
        output=value;return true;
    }
}
