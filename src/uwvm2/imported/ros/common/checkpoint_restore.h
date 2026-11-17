// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "checkpoint_environment.h"

namespace uwvm2::imported::ros::checkpoint
{
    // A recording is data, never authority. Recorded local delivery can be
    // rebound only to the SAME currently authorized group/handle inventory.
    // Quotas/audit are observations and are intentionally not restored.
    inline bool recorded_group_matches_current(data::group const& saved,data::group const& current) noexcept
    {
        if(saved.scope!=current.scope || saved.name!=current.name || saved.members!=current.members ||
           saved.enabled!=current.enabled || saved.registered!=current.registered || saved.alive!=current.alive ||
           saved.session_present!=current.session_present || saved.connected!=current.connected ||
           saved.session_label!=current.session_label || saved.availability_sealed!=current.availability_sealed ||
           saved.connection!=current.connection || saved.interfaces!=current.interfaces || saved.handles!=current.handles ||
           saved.availability_history!=current.availability_history || saved.capabilities.size()!=current.capabilities.size() ||
           saved.observations.size()!=current.observations.size() || saved.next_service_request>current.next_service_request)
            return false;
        for(std::size_t i{};i<saved.capabilities.size();++i)
        {
            auto const& a=saved.capabilities[i];auto const& b=current.capabilities[i];
            if(a.alias!=b.alias || a.operation!=b.operation || a.resource!=b.resource || a.type!=b.type ||
               a.type_identity!=b.type_identity || a.limits!=b.limits || a.revoked!=b.revoked)return false;
        }
        for(std::size_t i{};i<saved.observations.size();++i)
        {
            auto const& a=saved.observations[i];auto const& b=current.observations[i];
            // The external input cursor and outstanding service identities
            // cannot rewind. Local retained delivery can; native effects cannot.
            if(a.handle!=b.handle || a.source_handle!=b.source_handle || a.correlation!=b.correlation ||
               a.observer_alias!=b.observer_alias || a.source_alias!=b.source_alias || a.retired!=b.retired ||
               a.current_session!=b.current_session || a.native_sequence!=b.native_sequence || a.requests!=b.requests ||
               a.local.configured!=b.local.configured || a.local.active!=b.local.active)return false;
            for(unsigned j{};j<7;++j)if(a.local.state[j]!=b.local.state[j])return false;
        }
        return true;
    }
}
