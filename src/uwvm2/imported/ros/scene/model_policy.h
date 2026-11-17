/** @file
 * @brief Inert model bindings, explicit carrier codecs and finite deployment capacity.
 * @details Configuration identifies one immutable model and task. It grants no
 * worker, network, file or device authority. Opaque bytes are deliberately not
 * interpreted by the VM: their task parser belongs in the isolated approved
 * worker. All model outputs remain untrusted predictions and must pass a
 * separate proposal/controller admission before they can affect hardware.
 */
#pragma once
#include "model_jobs.h"

namespace uwvm2::imported::ros::scene::models
{
    /** @brief Aggregate reservations, separate from optional call/byte attenuation.
     * @details Every field defaults to zero; UINT64_MAX is never a capacity.
     * Input bytes count pinned immutable inputs even though the original
     * buffer binding independently owns and charges the underlying storage.
     * Workspace includes copied carriers and native worker scratch, not just
     * model parameter bodies. Output items also bound per-item bookkeeping.
     */
    struct budget
    {
        std::uint64_t sessions{},requests{},input_bytes{},output_bytes{},output_items{},workspace_bytes{};
    };
    inline bool valid(budget const& value) noexcept
    {
        return value.sessions!=UINT64_MAX&&value.requests!=UINT64_MAX&&value.output_items!=UINT64_MAX&&
            value.input_bytes<=INT64_MAX&&value.output_bytes<=INT64_MAX&&value.workspace_bytes<=INT64_MAX;
    }
    /** @brief Host-only codec selection; no guest name resolves code or a plugin.
     * @details text.utf8 is a complete NUL-free UTF-8 body. bytes accepts opaque
     * length-delimited bodies; it does not assert model-level validity. Both
     * preserve the outer canonical message type and bounded ownership rules.
     * Empty bodies are permitted; task-specific validation occurs inside the
     * fixed worker. A malformed prediction can never bypass downstream tool
     * validation merely because it passed this transport codec.
     */
    inline bool select_codec(std::string_view encoding,plan& value) noexcept
    {
        if(encoding=="text.utf8")
        {
            value.validate_parameters=[](wire::bytes bytes) noexcept
            {return wire::utf8(bytes)&&std::find(bytes.begin(),bytes.end(),std::uint8_t{})==bytes.end();};
            value.validate_output=[](wire::bytes bytes,bool) noexcept
            {return wire::utf8(bytes)&&std::find(bytes.begin(),bytes.end(),std::uint8_t{})==bytes.end();};
        }
        else if(encoding=="bytes")
        {
            value.validate_parameters=[](wire::bytes) noexcept{return true;};
            value.validate_output=[](wire::bytes,bool) noexcept{return true;};
        }
        else return false;
        return value.encoding.assign(encoding);
    }
    /** @brief Add one plan atomically to a finite aggregate budget.
     * @details On refusal totals are unchanged. It performs no allocation,
     * provider activation, model parsing, or optional-rate mutation.
     */
    inline bool accumulate(budget const& bound,plan const& value,budget& total) noexcept
    {
        if(!valid(bound)||!valid(value)||!valid(total))return false;
        auto fits=[](std::uint64_t sum,std::uint64_t amount,std::uint64_t maximum) noexcept
        {return sum<=maximum&&amount<=maximum-sum;};
        if(!fits(total.sessions,value.sessions,bound.sessions)||!fits(total.requests,value.requests,bound.requests)||
           !fits(total.input_bytes,value.input_bytes,bound.input_bytes)||!fits(total.output_bytes,value.output_bytes,bound.output_bytes)||
           !fits(total.output_items,value.output_items,bound.output_items)||!fits(total.workspace_bytes,value.workspace_bytes,bound.workspace_bytes))return false;
        total.sessions+=value.sessions;total.requests+=value.requests;total.input_bytes+=value.input_bytes;
        total.output_bytes+=value.output_bytes;total.output_items+=value.output_items;total.workspace_bytes+=value.workspace_bytes;
        return true;
    }
}
