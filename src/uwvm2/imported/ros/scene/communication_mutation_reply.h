/** @file
 * @brief Validate native mutation evidence without exposing worker identities.
 * @details The envelope and canonical body must agree before a trusted adapter
 * updates a host correlation. Unknown transport effects stay unknown. The native
 * request identity is never a guest request, resource, capability or retry key.
 * This decoder performs no I/O, allocation, charging or authority inference.
 */
#pragma once
#include "communication.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
    /** @brief Decode one complete canonical receipt from an isolated provider.
     * @details Empty bodies are allowed only for known pre-effect refusal or
     * failed transport with an unknown effect. A successful mutation requires
     * complete evidence. Oversized bodies must be rejected before constructing
     * their span; callers retain uncertainty instead of truncating a receipt.
     */
    inline bool mutation_evidence(wire::bytes body,communication::native_result result,communication::handle& native_request)noexcept
    {
        using error=communication::error;using effect=communication::effect;
        if(!communication::known_status(result.status)||result.observed>effect::unknown||result.deferred)return false;
        if(body.empty())return result.status!=error::ok&&(result.observed==effect::none||result.observed==effect::unknown);
        wire::reader input{body};std::uint64_t request{},generation{};std::string_view state,observed,reason;
        if(!input.u64(request)||!request||!input.text(state)||!input.text(observed)||!input.u64(generation)||!generation||!input.text(reason)||!input.done())return false;
        auto expected_effect=result.observed==effect::unknown?"unknown":result.observed==effect::observed_remote?"observed_remote":
            result.observed==effect::locally_accepted?"locally_accepted":"none";
        auto expected_state=result.observed==effect::unknown?"outcome_unknown":result.status!=error::ok?"failed":
            result.observed==effect::observed_remote?"succeeded":result.observed==effect::none?"failed":"admitted";
        auto expected_reason=result.observed==effect::unknown?"outcome.unknown":result.status==error::ok?"none":
            result.status==error::timeout?"deadline.elapsed":"provider.error";
        if(generation!=result.provider_generation||state!=expected_state||observed!=expected_effect||reason!=expected_reason)return false;
        native_request=request;return true;
    }
}
