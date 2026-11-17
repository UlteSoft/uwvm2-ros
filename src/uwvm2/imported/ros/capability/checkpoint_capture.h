// Private capability_manager fragment. Read-only; caller owns a closed host cohort.
public:
    [[nodiscard]] checkpoint::status copy_checkpoint_backend(
        ::std::span<checkpoint::source_member const> members,version v,checkpoint::data::group& output)
    {
        ::std::unique_lock registry{registry_mutex,::std::try_to_lock};
        if(!registry.owns_lock())return checkpoint::status::stale;
        ::std::shared_ptr<backend_state> selected{};
        for(auto const& source:members)
        {
            auto found=instances.find({source.instance,source.epoch});
            if(found==instances.end())continue;
            if(found->second->module!=source.name || found->second->retired.load(::std::memory_order_acquire)!=source.retired)
                return checkpoint::status::stale;
            auto state=v==version::ros1?found->second->ros1:found->second->ros2;
            if(selected && selected!=state)return checkpoint::status::malformed;
            selected=::std::move(state);
        }
        if(!selected)return checkpoint::status::ready;
        auto& state=*selected;
        ::std::unique_lock lock{state.mutex,::std::try_to_lock};
        if(!lock.owns_lock() || state.waiters)return checkpoint::status::stale;
        output.registered=true;output.alive=state.alive;
        output.capabilities.clear();output.handles.clear();
        for(auto const& [key,alias]:state.aliases)
        {
            auto const& a=*alias.accounting;
            ::std::unique_lock account{alias.accounting->mutex,::std::try_to_lock};
            ::std::unique_lock audit{alias.accounting->audit_mutex,::std::try_to_lock};
            if(!account.owns_lock() || !audit.owns_lock() || a.waiters)return checkpoint::status::stale;
            checkpoint::data::capability c{};c.alias=key;c.operation=alias.cap.operation;
            c.resource=alias.cap.resource;c.type=alias.cap.type;c.type_identity=alias.cap.type_identity;
            for(auto const& [k,value]:alias.cap.limits)c.limits.push_back({k,value});
            c.revoked=alias.revoked;
            c.accounting={a.open_count,a.burst,a.bandwidth,a.tokens,a.creation_tokens,a.byte_tokens,a.byte_fraction,
                a.log_bandwidth,a.log_tokens,a.log_fraction,a.limited_rate,a.limited_bytes,a.limited_log};
            // Preserve every fractional bit without serializing a native
            // long-double ABI or narrowing to double (i686 has a distinct ABI).
            auto fraction_text=[](long double value)
            {
                if(value==0)return ::uwvm2::utils::container::std_string{"0x0p0"};
                int exponent{};auto part=::std::frexp(value,&exponent);
                ::uwvm2::utils::container::std_string text=part<0?"-0x0.":"0x0.";if(part<0)part=-part;
                constexpr char digits[]{"0123456789abcdef"};
                for(unsigned i{};i<(::std::numeric_limits<long double>::digits+3u)/4u;++i)
                {part*=16;auto n=static_cast<unsigned>(part);text+=digits[n];part-=n;}
                text+='p';text+=::uwvm2::utils::container::concat(exponent);return text;
            };
            if(!::std::isfinite(a.rate) || !::std::isfinite(a.token_fraction) || !::std::isfinite(a.creation_fraction))
                return checkpoint::status::malformed;
            c.fractions={fraction_text(a.rate),fraction_text(a.token_fraction),fraction_text(a.creation_fraction)};
            auto const t=a.audit.statistics();
            c.audit={a.audit.retained_bytes(),a.audit.dropped_records(),t.attempted,t.accepted,t.overwritten,
                t.rate_limited,t.oversized,t.unconfigured,t.allocation_failed,t.sequence_exhausted,t.drain_failed,
                t.capacity_exhausted,t.cancelled,t.clock_unavailable,t.saturated};
            a.audit.copy_retained_records(c.audit_records);output.capabilities.push_back(::std::move(c));
        }
        for(auto const& [slot,h]:state.handles)output.handles.push_back({slot,h.generation,h.alias,h.active,h.retired});
        return checkpoint::status::ready;
    }
