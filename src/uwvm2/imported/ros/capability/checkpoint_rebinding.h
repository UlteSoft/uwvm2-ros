// Private capability_manager fragment. There is no Guest entry for this API.
// This preserves CURRENT owners, not numeric authority deserialized from disk.
public:
    class checkpoint_rebinding final
    {
        friend class capability_manager;
        capability_manager* manager_{};
        decltype(instances) previous_{}, replacement_{};
        ::uwvm2::utils::container::std_map<identity,bool,identity_less> retired_{};
        bool published_{};
        [[nodiscard]] bool current_locked() const noexcept
        {
            if(!manager_ || published_ || previous_.size()!=manager_->instances.size())return false;
            auto actual=manager_->instances.begin();
            for(auto const& [key,value]:previous_)
            {
                if(actual==manager_->instances.end() || actual->first!=key || actual->second!=value)return false;
                auto const expected=retired_.find(key);
                if(expected==retired_.end() || value->retired.load(::std::memory_order_acquire)!=expected->second)return false;
                ++actual;
            }
            return true;
        }
    public:
        checkpoint_rebinding() = default;
        checkpoint_rebinding(checkpoint_rebinding const&) = delete;
        checkpoint_rebinding& operator=(checkpoint_rebinding const&) = delete;
        [[nodiscard]] bool current() const noexcept
        {
            if(!manager_)return false;
            ::std::lock_guard guard{manager_->registry_mutex};return current_locked();
        }
        [[nodiscard]] bool publish() noexcept
        {
            if(!manager_)return false;
            ::std::lock_guard guard{manager_->registry_mutex};
            if(!current_locked())return false;
            // No allocation or backend retirement. Handles, alias revocation,
            // quota accounts, audit buffers and group/global owners are SAME.
            manager_->instances.swap(replacement_);published_=true;return true;
        }
    };
    [[nodiscard]] checkpoint::status prepare_checkpoint_rebinding(
        ::std::span<checkpoint::member const> members,checkpoint_rebinding& output)
    {
        if(output.manager_ || members.empty())return checkpoint::status::malformed;
        ::uwvm2::utils::container::std_map<identity,checkpoint::member const*,identity_less> selected;
        ::uwvm2::utils::container::std_set<void const*,::std::less<>> old_objects,new_objects;
        ::uwvm2::utils::container::std_set<::std::string_view,::std::less<>> names;
        for(auto const& member:members)
        {
            if(!member.previous || !member.replacement || !member.previous_epoch ||
               member.previous_epoch==UINT64_MAX || member.replacement_epoch!=member.previous_epoch+1u ||
               !valid_module_name(member.module) || !old_objects.insert(member.previous).second ||
               !new_objects.insert(member.replacement).second || !names.insert(member.module).second ||
               !selected.emplace(identity{member.previous,member.previous_epoch},::std::addressof(member)).second)
                return checkpoint::status::malformed;
        }
        for(auto object:new_objects)if(old_objects.contains(object))return checkpoint::status::malformed;
        decltype(instances) previous,replacement;
        decltype(output.retired_) retired;
        ::std::lock_guard guard{registry_mutex};
        for(auto const& [key,state]:instances)
        {
            auto found=selected.find(key);
            if(found==selected.end() || state->module!=found->second->module)return checkpoint::status::stale;
            auto const& member=*found->second;
            auto const was_retired=state->retired.load(::std::memory_order_acquire);
            // A permanent runtime retirement must agree with its registered
            // native owner. Neither direction may be silently reinterpreted.
            if(was_retired!=member.authority_retired)return checkpoint::status::stale;
            previous.emplace(key,state);retired.emplace(key,was_retired);
            if(!replacement.emplace(identity{member.replacement,member.replacement_epoch},state).second)
                return checkpoint::status::malformed;
        }
        output.previous_.swap(previous);output.replacement_.swap(replacement);output.retired_.swap(retired);
        output.manager_=this;return checkpoint::status::ready;
    }
