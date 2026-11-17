// Authenticated immutable code graph. Mutable Wasm instance state is private
// per child; only declared shared linear memory borrows source objects.
// Every graph node consumes the same child allocation account.
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

    inline ThreadedChildWorld::function_location ThreadedChildWorld::locate_source_global(
        ::uwvm2::uwvm::runtime::storage::local_defined_global_storage_t const* global) const noexcept
    {
        for(auto const& [name,module]:source_->registry())
        {
            static_cast<void>(name);
            for(::std::size_t i{};i<module.local_defined_global_vec_storage.size();++i)
                if(global==&module.local_defined_global_vec_storage.index_unchecked(i))return {&module,i,false};
        }
        return {};
    }

    inline ThreadedChildWorld::function_location ThreadedChildWorld::locate_source_table(
        ::uwvm2::uwvm::runtime::storage::local_defined_table_storage_t const* global) const noexcept
    {
        for(auto const& [name,module]:source_->registry())
        {
            static_cast<void>(name);
            for(::std::size_t i{};i<module.local_defined_table_vec_storage.size();++i)
                if(global==&module.local_defined_table_vec_storage.index_unchecked(i))return {&module,i,false};
        }
        return {};
    }

    inline ThreadedChildWorld::function_location ThreadedChildWorld::locate_source_memory(memory_type const* memory) const noexcept
    {
        for(auto const& [name,module]:source_->registry())
        {
            static_cast<void>(name);
            for(::std::size_t i{};i<module.local_defined_memory_vec_storage.size();++i)
                if(memory==&module.local_defined_memory_vec_storage.index_unchecked(i))return {&module,i,false};
        }
        return {};
    }
    inline ThreadedChildWorld::function_location ThreadedChildWorld::locate_source_tag(
        ::uwvm2::uwvm::runtime::storage::local_defined_tag_storage_t const* tag) const noexcept
    {
        for(auto const& [name,module]:source_->registry())
        {
            static_cast<void>(name);
            for(::std::size_t i{};i<module.local_defined_tag_vec_storage.size();++i)
                if(tag==&module.local_defined_tag_vec_storage.index_unchecked(i))return {&module,i,false};
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
            auto admit=[&](module_type const* parent)->threaded_child_error
            {
                if(!parent)return threaded_child_error::unsupported_import;
                if(node_for_parent(parent))return threaded_child_error::ok;
                if(module_count()>=limits_.max_runtime_modules)return threaded_child_error::count_limit;
                auto peer=::std::allocate_shared<ThreadedChildWorld>(threaded_std_allocator<ThreadedChildWorld>{allocation_},admission_,allocation_);
                peer->self_=peer;peer->parent_=parent;peer->graph_=this;peer->kind_=kind_;
                peer->source_id_=source_->bound_initialized_module_id(parent);
                auto check=peer->preflight();if(check!=threaded_child_error::ok)return check;
                if(kind_==storage_kind::actual_parent)
                {peer->module_view_=parent;peer->public_modules_.bind(peer->file_->module_name,*parent);}
                else {check=peer->build();if(check!=threaded_child_error::ok)return check;}
                peers_.push_back(::std::move(peer));
                return threaded_child_error::ok;
            };
            for(auto const& imported:current->parent_->imported_function_vec_storage)
            {
                if(imported.link_kind==kind::local_imported)continue;
                auto const location=locate_source_function(imported);
                auto const check=admit(location.parent);if(check!=threaded_child_error::ok)return check;
            }
            for(auto const* global:current->shared_globals_)
            {auto const check=admit(locate_source_global(global).parent);if(check!=threaded_child_error::ok)return check;}
            for(auto const* table:current->imported_table_targets_)
            {auto const check=admit(locate_source_table(table).parent);if(check!=threaded_child_error::ok)return check;}
            for(auto const* memory:current->imported_memory_targets_)
                if(!details::wasm_memory_is_shared(*memory->memory_type_ptr))
                {auto const check=admit(locate_source_memory(memory).parent);if(check!=threaded_child_error::ok)return check;}
            for(auto const& tag:current->parent_->imported_tag_vec_storage)
            {auto const check=admit(locate_source_tag(tag.resolved_tag).parent);if(check!=threaded_child_error::ok)return check;}
        }
        ::uwvm2::utils::container::std_vector<memory_type const*,threaded_std_allocator<memory_type const*>> shared_owners{threaded_std_allocator<memory_type const*>{allocation_}};
        ::std::uint64_t shared_bytes{},private_bytes{};
        for(::std::size_t m{};m<module_count();++m)
        {
            for(auto const* memory:node(m)->shared_memories_)
            {
                if(::std::find(shared_owners.begin(),shared_owners.end(),memory)!=shared_owners.end())continue;
                auto const pages=static_cast<::std::uint64_t>(memory->effective_limits.max);
                if(pages>(admission().max_shared_memory_bytes()-shared_bytes)/65536u)return threaded_child_error::memory_limit;
                shared_bytes+=pages*65536u;shared_owners.push_back(memory);
            }
            for(auto const& memory:node(m)->parent_->local_defined_memory_vec_storage)
            {
                auto const pages=static_cast<::std::uint64_t>(memory.effective_limits.min);
                if(pages>(admission().max_local_memory_bytes()-private_bytes)/65536u)return threaded_child_error::memory_limit;
                private_bytes+=pages*65536u;
            }
        }
        // Determine one dependency order before effects or native launch. The
        // same order applies to ordinary main and every newly created instance;
        // an importer's active table segment must not be erased by its provider.
        struct pending {::std::size_t module,next;};
        ::uwvm2::utils::container::std_vector<pending,threaded_std_allocator<pending>> stack{threaded_std_allocator<pending>{allocation_}};
        ::uwvm2::utils::container::std_vector<unsigned char,threaded_std_allocator<unsigned char>> visited{threaded_std_allocator<unsigned char>{allocation_}};
        visited.resize(module_count());stack.reserve(module_count());initialization_order_.reserve(module_count());
        for(::std::size_t root{};root<module_count();++root)
        {
            if(visited[root])continue;visited[root]=1;stack.push_back({root,0u});
            while(!stack.empty())
            {
                auto& current=stack.back();
                if(current.next<initialization_dependency_count(current.module))
                {
                    ::std::size_t dependency{};
                    if(initialization_dependency(current.module,current.next++,dependency)&&!visited[dependency])
                    {visited[dependency]=1;stack.push_back({dependency,0u});}
                }
                else {auto const m=current.module;stack.pop_back();visited[m]=2;initialization_order_.push_back(m);}
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
                for(::std::size_t i{};i<current->imported_memory_targets_.size();++i)
                {
                    auto* target=current->resolved_private_import_memory(i);
                    if(!target)return threaded_child_error::declaration_mismatch;
                    current->module_->imported_memory_vec_storage.index_unchecked(i).target.defined_ptr=target;
                }
                for(::std::size_t g{};g<current->shared_globals_.size();++g)
                {
                    auto const location=locate_source_global(current->shared_globals_[g]);
                    auto const* peer=node_for_parent(location.parent);
                    if(!peer || !peer->module_)return threaded_child_error::declaration_mismatch;
                    current->module_->imported_global_vec_storage.index_unchecked(g).target.defined_ptr=
                        &peer->module_->local_defined_global_vec_storage.index_unchecked(location.index);
                }
                for(::std::size_t t{};t<current->imported_table_targets_.size();++t)
                {
                    auto const location=locate_source_table(current->imported_table_targets_[t]);
                    auto const* peer=node_for_parent(location.parent);
                    if(!peer||!peer->module_)return threaded_child_error::declaration_mismatch;
                    current->module_->imported_table_vec_storage.index_unchecked(t).target.defined_ptr=
                        &peer->module_->local_defined_table_vec_storage.index_unchecked(location.index);
                }
                for(::std::size_t t{};t<current->parent_->imported_tag_vec_storage.size();++t)
                {
                    auto const location=locate_source_tag(current->parent_->imported_tag_vec_storage.index_unchecked(t).resolved_tag);
                    auto const* peer=node_for_parent(location.parent);
                    if(!peer || !peer->module_)return threaded_child_error::declaration_mismatch;
                    auto& imported=current->module_->imported_tag_vec_storage.index_unchecked(t);
                    imported.imported_target=nullptr;
                    imported.resolved_tag=imported.defined_target=&peer->module_->local_defined_tag_vec_storage.index_unchecked(location.index);
                }
                for(auto& table:current->module_->local_defined_table_vec_storage)table.owner_module_rt_ptr=current->module_;
                // A global initializer can recursively read a later graph
                // member. Install all ownership and ref.func identities before
                // the first initializer runs; never borrow a parent identity.
                for(auto& global:current->module_->local_defined_global_vec_storage)
                {
                    global.owner_module_rt_ptr=current->module_;
                    if(global.init_state==::uwvm2::uwvm::runtime::storage::wasm_global_init_state::initialized &&
                       global.global.kind==::uwvm2::object::global::global_type::wasm_ref)
                        details::canonicalize_wasm1_funcref(*current->module_,global.global.storage.ref);
                }
            }
            for(auto m:initialization_order_)
            {
                auto* current=m==0u?this:peers_[m-1u].get();
                auto const check=current->finish_initialization();if(check!=threaded_child_error::ok)return check;
            }
        }
        return threaded_child_error::ok;
    }
