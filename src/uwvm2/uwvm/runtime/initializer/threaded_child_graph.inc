// Authenticated immutable code graph. Mutable Wasm instance state is private
// per child; only declared shared linear memory and immutable globals borrow
// source objects. Every graph node consumes the same child allocation account.
    inline ThreadedChildWorld::function_location ThreadedChildWorld::locate_source_function(
        ::uwvm2::uwvm::runtime::storage::imported_function_storage_t const& f) const noexcept
    {
        using kind=::uwvm2::uwvm::runtime::storage::imported_function_link_kind;
        for(auto const& [name,module]:source_->registry())
        {
            static_cast<void>(name);
            if(f.link_kind==kind::defined)
            {
                for(::std::size_t i{};i<module.local_defined_function_vec_storage.size();++i)
                    if(f.target.defined_ptr==&module.local_defined_function_vec_storage.index_unchecked(i))return {&module,i,false};
            }
            else if(f.link_kind==kind::imported)
            {
                for(::std::size_t i{};i<module.imported_function_vec_storage.size();++i)
                    if(f.target.imported_ptr==&module.imported_function_vec_storage.index_unchecked(i))return {&module,i,true};
            }
        }
        return {};
    }

    inline threaded_child_error ThreadedChildWorld::build_graph()
    {
        using kind=::uwvm2::uwvm::runtime::storage::imported_function_link_kind;
        // Discover a finite transitive graph from authenticated import records.
        // Cycles are represented once; no native call can enter ordinary code.
        for(::std::size_t m{};m<module_count();++m)
        {
            auto const* current=node(m);
            for(auto const& imported:current->parent_->imported_function_vec_storage)
            {
                if(imported.link_kind==kind::local_imported)continue;
                auto const location=locate_source_function(imported);
                if(!location.parent)return threaded_child_error::unsupported_import;
                if(node_for_parent(location.parent))continue;
                if(module_count()>=limits_.max_runtime_modules)return threaded_child_error::count_limit;
                auto peer=::std::allocate_shared<ThreadedChildWorld>(threaded_std_allocator<ThreadedChildWorld>{allocation_},admission_,allocation_);
                peer->self_=peer;peer->parent_=location.parent;peer->graph_=this;peer->kind_=kind_;
                peer->source_id_=source_->bound_initialized_module_id(location.parent);
                auto check=peer->preflight();if(check!=threaded_child_error::ok)return check;
                // Side-module starts require their own once-initialization ABI.
                // Refuse them explicitly, never skip or replay active effects.
                if(peer->initialization_)return threaded_child_error::unsupported_start;
                if(kind_==storage_kind::actual_parent)
                {peer->module_view_=location.parent;peer->public_modules_.bind(peer->file_->module_name,*location.parent);}
                else {check=peer->build();if(check!=threaded_child_error::ok)return check;}
                if(peer->shared_memories_.size()!=1u || shared_memories_.size()!=1u ||
                    peer->shared_memories_[0]!=shared_memories_[0])return threaded_child_error::unsupported_import;
                peers_.push_back(::std::move(peer));
            }
        }
        if(kind_==storage_kind::fresh_child)
        {
            for(::std::size_t m{};m<module_count();++m)
            {
                auto* current=m==0u?this:peers_[m-1u].get();
                for(::std::size_t f{};f<current->parent_->imported_function_vec_storage.size();++f)
                {
                    auto const& original=current->parent_->imported_function_vec_storage.index_unchecked(f);
                    if(original.link_kind==kind::local_imported)continue;
                    auto const location=locate_source_function(original);auto const* peer=node_for_parent(location.parent);
                    if(!peer || !peer->module_)return threaded_child_error::declaration_mismatch;
                    auto& target=current->module_->imported_function_vec_storage.index_unchecked(f);
                    if(location.imported)target.target.imported_ptr=&peer->module_->imported_function_vec_storage.index_unchecked(location.index);
                    else target.target.defined_ptr=&peer->module_->local_defined_function_vec_storage.index_unchecked(location.index);
                }
            }
        }
        return threaded_child_error::ok;
    }
