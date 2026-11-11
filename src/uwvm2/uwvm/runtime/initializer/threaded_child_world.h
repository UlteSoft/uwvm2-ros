// SOURCE-ONLY candidate: independent WASI Threads child storage.
// A child is not a checkpoint restore and does not select the global registry.
#pragma once
#ifndef UWVM_MODULE
# include <memory>
# include <bit>
# include <vector>
# include <limits>
# include <type_traits>
# include <utility>
# include "threaded_allocation.h"
# include "init.h"
# include "threaded_once_init.h"
# include <uwvm2/runtime/wasm_threads/admission.h>
#endif

namespace uwvm2::runtime::lib
{
    extern "C++" { class ThreadedLlvmChild; class ThreadedInterpreterChild; }
}

#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && defined(__linux__)
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::runtime::initializer
{
#ifdef UWVM_MODULE
    extern "C++" {
#endif
    class ThreadedChildWorld;
    class ThreadedValidatedCode final
    {
        friend class ThreadedChildWorld;
        ThreadedChildWorld const* world_{};
        ::uwvm2::uwvm::runtime::full::full_source_instance::owner source_{};
        ::uwvm2::uwvm::wasm::type::wasm_file_t const* file_{};
        ::uwvm2::uwvm::runtime::storage::wasm_module_storage_t const* module_{};
        ::std::uint_least64_t epoch_{};
        explicit ThreadedValidatedCode(ThreadedChildWorld const* world) noexcept : world_{world} {}
    public:
        ThreadedValidatedCode(ThreadedValidatedCode const&) = delete;
        ThreadedValidatedCode& operator=(ThreadedValidatedCode const&) = delete;
        [[nodiscard]] bool covers(ThreadedChildWorld const&) const noexcept;
        [[nodiscard]] auto const* file() const noexcept { return file_; }
        [[nodiscard]] ::std::uint_least64_t source_validation_epoch() const noexcept { return epoch_; }
    };
    // Typed failures are cold admission outcomes, never guest-visible handles.
    enum class threaded_child_error : unsigned char
    {
        ok, invalid_admission, invalid_source, count_limit, table_limit,
        memory_limit, segment_limit, unsupported_start, unsupported_import,
        unsupported_shared_write, unsupported_local_shared_memory,
        invalid_thread_entry, declaration_mismatch, invalid_store, allocation_failure, unsupported_initialization_shape,
        missing_shared_memory
    };

    [[nodiscard]] inline constexpr bool threaded_child_error_is_profile_rejection(threaded_child_error error) noexcept
    {
        switch(error)
        {
            case threaded_child_error::count_limit:
            case threaded_child_error::table_limit:
            case threaded_child_error::memory_limit:
            case threaded_child_error::segment_limit:
            case threaded_child_error::unsupported_start:
            case threaded_child_error::unsupported_import:
            case threaded_child_error::unsupported_shared_write:
            case threaded_child_error::unsupported_local_shared_memory:
            case threaded_child_error::invalid_thread_entry:
            case threaded_child_error::allocation_failure:
            case threaded_child_error::unsupported_initialization_shape:
            case threaded_child_error::missing_shared_memory: return true;
            default: return false; // Broken source/admission/store invariants remain internal failures.
        }
    }

    [[nodiscard]] inline constexpr char const* threaded_child_error_description(threaded_child_error error) noexcept
    {
        switch(error)
        {
            case threaded_child_error::ok: return "no error";
            case threaded_child_error::invalid_admission: return "invalid thread admission";
            case threaded_child_error::invalid_source: return "invalid canonical source";
            case threaded_child_error::count_limit: return "profile object-count limit exceeded";
            case threaded_child_error::table_limit: return "profile table storage limit exceeded";
            case threaded_child_error::memory_limit: return "profile memory storage limit exceeded";
            case threaded_child_error::segment_limit: return "profile segment storage limit exceeded";
            case threaded_child_error::unsupported_start: return "module start does not satisfy the threaded initialization contract";
            case threaded_child_error::unsupported_import: return "import or memory backend is unsupported by the threaded profile";
            case threaded_child_error::unsupported_shared_write: return "child active data segment would overwrite shared memory";
            case threaded_child_error::unsupported_local_shared_memory: return "child shared memory must be imported from its authenticated provider";
            case threaded_child_error::invalid_thread_entry: return "requires a locally defined wasi_thread_start(i32, i32) with no results";
            case threaded_child_error::declaration_mismatch: return "runtime declaration mismatch";
            case threaded_child_error::invalid_store: return "invalid private GC store";
            case threaded_child_error::allocation_failure: return "unable to reserve the selected profile storage";
            case threaded_child_error::unsupported_initialization_shape: return "module shape is unsupported by the threaded initialization contract";
            case threaded_child_error::missing_shared_memory: return "requires shared linear memory; remove --wasi-threads to run a non-threaded module";
        }
        return "unknown thread instance failure";
    }

    class ThreadedChildWorld final
    {
        friend class ThreadedValidatedCode;
        friend class ::uwvm2::runtime::lib::ThreadedLlvmChild;
        friend class ::uwvm2::runtime::lib::ThreadedInterpreterChild;
        template<typename> friend class ::uwvm2::uwvm::runtime::initializer::threaded_std_allocator;
        friend class details::initialization_context<details::initialization_purpose::threaded_child>;
    public:
        using source_type = ::uwvm2::uwvm::runtime::full::full_source_instance;
        using module_type = ::uwvm2::uwvm::runtime::storage::wasm_module_storage_t;
        using registry_type = threaded_single_entry_view<::uwvm2::uwvm::runtime::storage::wasm_module_storage_t const>;
        using file_type = ::uwvm2::uwvm::wasm::type::wasm_file_t;
        using feature_parameter_type = ::std::remove_cvref_t<decltype(
            ::std::declval<file_type const&>().wasm_parameter.binfmt1_para)>;
        using admission_type = ::uwvm2::runtime::wasm_threads::ThreadedSpawnAdmission;
        using owner = ::std::shared_ptr<ThreadedChildWorld const>;
        struct result { threaded_child_error error; owner world; };
    private:
        using store_type = ::uwvm2::uwvm::runtime::storage::gc_object_store;
        using reference = ::uwvm2::uwvm::runtime::storage::gc_reference;
        using memory_type = ::uwvm2::uwvm::runtime::storage::local_defined_memory_storage_t;
        using global_type = ::uwvm2::uwvm::runtime::storage::local_defined_global_storage_t;
        static constexpr auto purpose = details::initialization_purpose::threaded_child;
        using context_type = details::initialization_context<purpose>;

        // Declared FIRST so provider/source/parent lifetime outlasts all child
        // stores and their final native reference releases during destruction.
        threaded_allocation_pin allocation_;
        ::std::shared_ptr<admission_type const> admission_;
        source_type::owner source_;
        file_type const* file_{};
        module_type const* parent_{};
        ::std::size_t source_id_{};
        ThreadedChildWorld* graph_{};
        ::std::vector<::std::shared_ptr<ThreadedChildWorld>, threaded_std_allocator<::std::shared_ptr<ThreadedChildWorld>>> peers_;
        [[nodiscard]] ThreadedChildWorld const* node(::std::size_t index) const noexcept
        { return index==0u ? this : index<=peers_.size() ? peers_[index-1u].get() : nullptr; }
        [[nodiscard]] threaded_child_error build_graph();
        struct function_location { module_type const* parent{}; ::std::size_t index{}; bool imported{}; };
        [[nodiscard]] function_location locate_source_function(
            ::uwvm2::uwvm::runtime::storage::imported_function_storage_t const&) const noexcept;
        [[nodiscard]] ThreadedChildWorld const* node_for_parent(module_type const* parent) const noexcept
        {
            auto const* root=graph_;
            if(root->parent_==parent)return root;
            for(auto const& peer:root->peers_)if(peer->parent_==parent)return peer.get();
            return nullptr;
        }
        initializer_limit_t limits_{};
        ::uwvm2::uwvm::wasm::type::all_module_t declaration_{};
        threaded_single_entry_view<::uwvm2::uwvm::wasm::type::all_module_t> declarations_{};
        decltype(::uwvm2::uwvm::wasm::storage::all_module_export) exports_{};
        ::uwvm2::uwvm::wasm::storage::configured_module_import_reset_map_t resets_{};
        ::uwvm2::uwvm::wasm::storage::configured_module_memory_limit_map_t memory_limits_{};
        module_type owned_module_{};
        threaded_single_entry_view<module_type> modules_{};
        registry_type public_modules_{};
        module_type* module_{};
        module_type const* module_view_{};
        enum class storage_kind : unsigned char { fresh_child, actual_parent };
        storage_kind kind_{storage_kind::fresh_child};
        ::uwvm2::utils::container::u8string_view current_{};
        bool aliases_checked_{true};
        ::std::weak_ptr<ThreadedChildWorld const> self_{};
        ::std::vector<memory_type const*, threaded_std_allocator<memory_type const*>> shared_memories_;
        ::std::vector<memory_type*, threaded_std_allocator<memory_type*>> imported_memory_targets_;
        ::std::vector<global_type*, threaded_std_allocator<global_type*>> shared_globals_;
        using table_array = ::uwvm2::utils::container::vector<::uwvm2::uwvm::runtime::storage::local_defined_table_elem_storage_t>;
        ::std::vector<table_array, threaded_std_allocator<table_array>> prepared_tables_;
        ::std::size_t thread_start_{};
        ::std::optional<threaded_once_init_details::contract> initialization_{};
        ThreadedValidatedCode validated_{this};

        explicit ThreadedChildWorld(::std::shared_ptr<admission_type const> admission, threaded_allocation_pin allocation)
            : allocation_{::std::move(allocation)}, admission_{::std::move(admission)}, source_{admission_->source_owner()},
              parent_{::std::addressof(admission_->source_module())}, source_id_{admission_->source_module_id()}, graph_{this},
              peers_{threaded_std_allocator<::std::shared_ptr<ThreadedChildWorld>>{allocation_}},
              limits_{admission_->initializer_limits()},
              shared_memories_{threaded_std_allocator<memory_type const*>{allocation_}},
              imported_memory_targets_{threaded_std_allocator<memory_type*>{allocation_}},
              shared_globals_{threaded_std_allocator<global_type*>{allocation_}},
              prepared_tables_{threaded_std_allocator<table_array>{allocation_}} {}

        [[nodiscard]] context_type context() noexcept
        {
            return context_type{modules_, declarations_, exports_, resets_, memory_limits_,
                current_, aliases_checked_, limits_, self_};
        }
        [[nodiscard]] bool source_contains_memory(memory_type const* actual) const noexcept
        {
            for(auto const& [name, module] : source_->registry())
            {
                static_cast<void>(name);
                for(auto const& memory : module.local_defined_memory_vec_storage)
                { if(::std::addressof(memory) == actual) { return true; } }
            }
            return false;
        }
        template<typename Memory>
        [[nodiscard]] static bool supports_shared_child_memory(Memory const& memory) noexcept
        {
            // Stable mmap base + concurrent size/grow are the implemented native
            // profile. Allocator relocation needs backend-wide operation pins;
            // merely seeing support_multi_thread is not sufficient evidence.
            if constexpr(Memory::can_mmap && Memory::support_multi_thread &&
                requires { memory.sequentially_consistent_size; memory.memory_length_p; memory.growing_mutex_p; })
            {
                return memory.custom_page_size_log2 == 16u && memory.sequentially_consistent_size &&
                    memory.memory_length_p != nullptr && memory.growing_mutex_p != nullptr;
            }
            else { return false; }
        }
        [[nodiscard]] bool source_contains_global(global_type const* actual) const noexcept
        {
            for(auto const& [name, module] : source_->registry())
            {
                static_cast<void>(name);
                for(auto const& global : module.local_defined_global_vec_storage)
                { if(::std::addressof(global) == actual) { return true; } }
            }
            return false;
        }
        template<typename Imported, typename Member>
        [[nodiscard]] bool source_contains_import(Imported const* actual, Member member) const noexcept
        {
            for(auto const& [name, module] : source_->registry())
            {
                static_cast<void>(name);
                for(auto const& imported : module.*member)
                { if(::std::addressof(imported) == actual) { return true; } }
            }
            return false;
        }
        [[nodiscard]] threaded_child_error preflight();
        [[nodiscard]] threaded_child_error build();
        [[nodiscard]] bool declarations_match_source() const noexcept;
        [[nodiscard]] bool issue_validated_code() noexcept
        {
            if(!declarations_match_source()) { return false; }
            validated_.source_ = source_;
            validated_.file_ = file_;
            validated_.module_ = module_view_;
            validated_.epoch_ = admission_->source_validation_epoch();
            return validated_.covers(*this);
        }
        [[nodiscard]] bool preallocate();
        template<typename Vector> [[nodiscard]] bool preallocate_vector(Vector& vector, ::std::size_t count) noexcept;
        [[nodiscard]] bool verify_thread_entry() noexcept;

        [[nodiscard]] static ::std::shared_ptr<store_type> make_store(
            [[maybe_unused]] ::uwvm2::uwvm::runtime::storage::gc_type::recursive_type_section const& section,
            ::std::shared_ptr<::uwvm2::uwvm::runtime::storage::gc_lease_owner> const& leases,
            ::std::weak_ptr<ThreadedChildWorld const> const& binding)
        {
            auto self{binding.lock()};
            if(!self || !self->parent_->gc_store) { throw ::std::bad_alloc{}; }
            // Copy immutable canonical schemas, including aggregate fields, with
            // checked allocations into a NEW private arena. Never copy objects.
            auto store{::std::allocate_shared<store_type>(threaded_std_allocator<store_type>{self->allocation_},
                *self->parent_->gc_store, leases, function_type_matches,
                store_type::threaded_schema_tag{}, self->allocation_)};
            if(!store->valid()) { throw ::std::bad_alloc{}; }
            store->threaded_world_binding_ = binding;
            return store;
        }
        [[nodiscard]] static bool function_type_matches(reference actual,
            store_type const* expected, ::std::uint_least32_t type_index) noexcept
        {
            if(expected == nullptr) { return false; }
            auto self{expected->threaded_world_binding_.lock()};
            if(!self || self->module_ == nullptr || self->module_->gc_store.get() != expected)
            { return false; }
            // The callback only reads this fully stabilized registry. No global
            // selected-registry pointer, parent engine or saved restore source.
            auto world{const_cast<ThreadedChildWorld&>(*self).context()};
            return details::linked_gc_function_reference_type_matches_in_context(
                world, actual, expected, type_index);
        }
    public:
        ThreadedChildWorld(ThreadedChildWorld const&) = delete;
        ThreadedChildWorld& operator=(ThreadedChildWorld const&) = delete;
        ThreadedChildWorld(ThreadedChildWorld&&) = delete;
        ThreadedChildWorld& operator=(ThreadedChildWorld&&) = delete;
        ~ThreadedChildWorld() = default;

        // Root creates/retains the genuine execution-domain lease. A caller
        // cannot construct an admission using a restored source or ready flag.
        [[nodiscard]] static result create(admission_type const& admission)
        {
            auto retained{admission.retain()};
            if(!retained || retained.get() != ::std::addressof(admission))
            { return {threaded_child_error::invalid_admission, {}}; }
            auto account{threaded_allocation_pin::create(admission.limits().max_child_storage_bytes)};
            if(!account) { return {threaded_child_error::allocation_failure, {}}; }
            try
            {
            auto child{::std::allocate_shared<ThreadedChildWorld>(threaded_std_allocator<ThreadedChildWorld>{account}, ::std::move(retained), account)};
            child->self_ = child;
            auto const error{child->preflight()};
            if(error != threaded_child_error::ok) { return {error, {}}; }
            auto const initialized{child->build()};
            if(initialized != threaded_child_error::ok) { return {initialized, {}}; }
            auto const linked{child->build_graph()};
            if(linked != threaded_child_error::ok) { return {linked, {}}; }
            if(!child->issue_validated_code()) { return {threaded_child_error::declaration_mismatch, {}}; }
            return {threaded_child_error::ok, ::std::move(child)};
            }
            catch(::std::bad_alloc const&) { return {threaded_child_error::allocation_failure, {}}; }
        }
        // The main thread must also use a cancellation-aware profile engine.
        // Its storage is the actual already-initialized parent, not a second
        // instantiation or a live-state clone. Root grants this only while the
        // ordinary parent entry is quiescent and same-source ordinary admission
        // is closed for the entire profile group lifetime.
        [[nodiscard]] static result borrow_actual_parent(admission_type const& admission)
        {
            auto retained{admission.retain()};
            if(!retained || retained.get() != ::std::addressof(admission))
            { return {threaded_child_error::invalid_admission, {}}; }
            auto account{threaded_allocation_pin::create(admission.limits().max_child_storage_bytes)};
            if(!account) { return {threaded_child_error::allocation_failure, {}}; }
            try
            {
            auto parent{::std::allocate_shared<ThreadedChildWorld>(threaded_std_allocator<ThreadedChildWorld>{account}, ::std::move(retained), account)};
            parent->kind_ = storage_kind::actual_parent;
            parent->self_ = parent;
            auto const error{parent->preflight()};
            if(error != threaded_child_error::ok) { return {error, {}}; }
            parent->module_view_ = parent->parent_;
            parent->public_modules_.bind(parent->file_->module_name, *parent->parent_);
            if(!parent->verify_thread_entry()) { return {threaded_child_error::invalid_thread_entry, {}}; }
            auto const linked{parent->build_graph()};
            if(linked != threaded_child_error::ok) { return {linked, {}}; }
            if(!parent->issue_validated_code()) { return {threaded_child_error::declaration_mismatch, {}}; }
            return {threaded_child_error::ok, ::std::move(parent)};
            }
            catch(::std::bad_alloc const&) { return {threaded_child_error::allocation_failure, {}}; }
        }
        [[nodiscard]] registry_type const& registry() const noexcept
        { return public_modules_; }
        [[nodiscard]] ::std::size_t module_count() const noexcept { return module_view_ == nullptr ? 0uz : 1uz + peers_.size(); }
        [[nodiscard]] module_type const* module(::std::size_t index) const noexcept
        { auto const* n=node(index); return n ? n->module_view_ : nullptr; }
        [[nodiscard]] file_type const* file(::std::size_t index) const noexcept
        { auto const* n=node(index); return n ? n->file_ : nullptr; }
        [[nodiscard]] feature_parameter_type const* feature_parameter(::std::size_t index) const noexcept
        { auto const* f=file(index); return f ? ::std::addressof(f->wasm_parameter.binfmt1_para) : nullptr; }
        [[nodiscard]] constexpr ::std::size_t entry_module_index() const noexcept { return 0uz; }
        [[nodiscard]] ::std::size_t thread_start_function_index() const noexcept { return thread_start_; }
        // The engine must execute this ACTUAL function in its real native slot
        // before the selected main/thread entry. This is not a host-side skip,
        // synthetic drop replay or an already-initialized boolean.
        [[nodiscard]] ::std::optional<::std::size_t> initialization_function_index() const noexcept
        { return initialization_ ? ::std::optional<::std::size_t>{initialization_->function_index} : ::std::nullopt; }
        [[nodiscard]] source_type::owner const& source_owner() const noexcept { return source_; }
        [[nodiscard]] admission_type const& admission() const noexcept { return *admission_; }
        [[nodiscard]] ::std::shared_ptr<ThreadedValidatedCode const> validated_code() const noexcept
        {
            auto self{self_.lock()};
            if(!self || !validated_.covers(*this)) { return {}; }
            return {::std::move(self), ::std::addressof(validated_)};
        }
        // Monotonic actual request bytes, including allocations already freed
        // during preparation; safe conservative Group budget charge, not RSS.
        [[nodiscard]] ::std::size_t owned_storage_bytes() const noexcept
        { return allocation_.get().requested_bytes(); }
        // Reserve the backend and remaining GC arena before the Group publishes
        // this child. Dynamic allocations use only this already charged space.
        [[nodiscard]] bool reserve_execution_storage(::std::size_t non_world_bytes) const noexcept
        {
            auto* store=module_view_ ? module_view_->gc_store.get() : nullptr;
            if(!store || (store->threaded_account_ &&
                &store->threaded_account_.get()!=&allocation_.get())) { return false; }
            if(!allocation_.get().reserve_other_storage(non_world_bytes)) { return false; }
            // The main instance is borrowed only while ordinary admission is
            // closed. Its store also retains the account after this world dies.
            store->threaded_account_=allocation_;
            for(auto const& peer:peers_)
            {
                auto* other=peer->module_view_ ? peer->module_view_->gc_store.get() : nullptr;
                if(!other || (other->threaded_account_ && &other->threaded_account_.get()!=&allocation_.get()))return false;
                other->threaded_account_=allocation_;
            }
            return true;
        }
        [[nodiscard]] ::std::uint_least64_t source_generation() const noexcept { return admission_->source_generation(); }
        [[nodiscard]] ::std::size_t source_module_id(::std::size_t index=0u) const noexcept
        { auto const* n=node(index); return n ? n->source_id_ : SIZE_MAX; }
        [[nodiscard]] module_type const* original_module(::std::size_t index) const noexcept
        { auto const* n=node(index); return n ? n->parent_ : nullptr; }
        // The immutable child graph is the only cross-module code authority.
        [[nodiscard]] bool resolve_wasm_import(::std::size_t module_index, ::std::size_t function,
            ::std::size_t& target_module, ::std::size_t& target_function) const noexcept
        {
            using kind=::uwvm2::uwvm::runtime::storage::imported_function_link_kind;
            auto const* current=module(module_index);
            if(!current || function>=current->imported_function_vec_storage.size())return false;
            auto const* record=&current->imported_function_vec_storage.index_unchecked(function);
            ::std::size_t maximum{};
            for(::std::size_t m{};m<module_count();++m)
            {
                auto n=module(m)->imported_function_vec_storage.size();
                if(n>SIZE_MAX-maximum)return false; maximum+=n;
            }
            for(::std::size_t step{};step<=maximum;++step)
            {
                bool found{};
                for(::std::size_t m{};m<module_count();++m)
                {
                    auto const* candidate=module(m);
                    if(record->link_kind==kind::defined)
                    {
                        for(::std::size_t f{};f<candidate->local_defined_function_vec_storage.size();++f)
                            if(record->target.defined_ptr==&candidate->local_defined_function_vec_storage.index_unchecked(f))
                            {target_module=m;target_function=candidate->imported_function_vec_storage.size()+f;return true;}
                    }
                    else if(record->link_kind==kind::imported)
                    {
                        for(auto const& imported:candidate->imported_function_vec_storage)
                            if(record->target.imported_ptr==&imported){record=&imported;found=true;break;}
                    }
                    if(found)break;
                }
                if(!found)return false;
            }
            return false;
        }
        // Raw re-exports retain the actual calling module's environment. Only
        // provider provenance follows this immutable, source-validated graph.
        // Match each pointer to a real graph member BEFORE dereferencing it.
        [[nodiscard]] bool resolve_host_import(::std::size_t module_index, ::std::size_t imported,
            ::std::size_t& provider_module, ::std::size_t& provider_imported) const noexcept
        {
            using kind=::uwvm2::uwvm::runtime::storage::imported_function_link_kind;
            auto const* current=module(module_index);
            if(!current || imported>=current->imported_function_vec_storage.size())return false;
            ::std::size_t maximum{};
            for(::std::size_t m{};m<module_count();++m)
            {
                auto n=module(m)->imported_function_vec_storage.size();
                if(n>SIZE_MAX-maximum)return false; maximum+=n;
            }
            for(::std::size_t step{};step<maximum;++step)
            {
                auto const& record=current->imported_function_vec_storage.index_unchecked(imported);
                if(record.link_kind==kind::local_imported)
                {provider_module=module_index;provider_imported=imported;return true;}
                if(record.link_kind!=kind::imported)return false;
                bool found{};
                for(::std::size_t m{};m<module_count()&&!found;++m)
                {
                    auto const* candidate=module(m);
                    for(::std::size_t f{};f<candidate->imported_function_vec_storage.size();++f)
                        if(record.target.imported_ptr==&candidate->imported_function_vec_storage.index_unchecked(f))
                        {current=candidate;module_index=m;imported=f;found=true;break;}
                }
                if(!found)return false;
            }
            return false; // An import cycle cannot manufacture a host provider.
        }
        [[nodiscard]] memory_type const* shared_memory(::std::size_t index) const noexcept
        { return index < shared_memories_.size() ? shared_memories_[index] : nullptr; }
    };

#include "threaded_child_graph.inc"

    inline bool ThreadedValidatedCode::covers(ThreadedChildWorld const& world) const noexcept
    {
        if(world_ != ::std::addressof(world) || !source_ || source_ != world.source_ ||
            module_ != world.module_view_ || file_ != world.file_ ||
            epoch_ == 0u || epoch_ != world.admission_->source_validation_epoch()) { return false; }
        return ::uwvm2::uwvm::runtime::full::full_source_instance::has_canonical_owner(source_) &&
            source_->actual_validated_file(world.admission_->source_module_id(), epoch_, world.parent_) == file_ &&
            world.declarations_match_source() && [&]() noexcept
            { for(auto const& peer:world.peers_)if(!peer->declarations_match_source())return false; return true; }();
    }

    inline bool ThreadedChildWorld::declarations_match_source() const noexcept
    {
        if(module_view_ == nullptr || file_ == nullptr || !source_type::has_canonical_owner(source_) ||
            source_->actual_validated_file(source_id_, admission_->source_validation_epoch(), parent_) != file_)
        { return false; }
        if(kind_ == storage_kind::actual_parent) { return module_view_ == parent_; }
        auto const& a{*module_view_}; auto const& p{*parent_};
        if(module_view_ != ::std::addressof(owned_module_) || module_view_ == parent_ ||
            a.type_section_storage.type_section_begin != p.type_section_storage.type_section_begin ||
            a.type_section_storage.type_section_end != p.type_section_storage.type_section_end ||
            a.type_section_storage.core3_context_ptr != p.type_section_storage.core3_context_ptr ||
            a.type_section_storage.core3_declaration_context_ptr != p.type_section_storage.core3_declaration_context_ptr ||
            a.type_section_storage.core3_recursive_types_ptr != p.type_section_storage.core3_recursive_types_ptr ||
            a.type_section_storage.owned_signature_begin != p.type_section_storage.owned_signature_begin ||
            a.type_section_storage.owned_signature_end != p.type_section_storage.owned_signature_end ||
            a.type_section_storage.type_section_count != p.type_section_storage.type_section_count ||
            a.local_defined_function_vec_storage.size() != p.local_defined_function_vec_storage.size() ||
            a.local_defined_code_vec_storage.size() != p.local_defined_code_vec_storage.size() ||
            a.local_defined_global_vec_storage.size() != p.local_defined_global_vec_storage.size() ||
            a.local_defined_table_vec_storage.size() != p.local_defined_table_vec_storage.size() ||
            a.local_defined_tag_vec_storage.size() != p.local_defined_tag_vec_storage.size() ||
            a.local_defined_data_vec_storage.size() != p.local_defined_data_vec_storage.size() ||
            a.local_defined_element_vec_storage.size() != p.local_defined_element_vec_storage.size() ||
            !a.local_defined_memory_vec_storage.empty()) { return false; }
        for(::std::size_t i{}; i != a.local_defined_function_vec_storage.size(); ++i)
        {
            auto const& x{a.local_defined_function_vec_storage.index_unchecked(i)};
            auto const& y{p.local_defined_function_vec_storage.index_unchecked(i)};
            if(x.function_type_ptr != y.function_type_ptr || x.wasm_code_ptr != y.wasm_code_ptr ||
                a.local_defined_code_vec_storage.index_unchecked(i).code_type_ptr != p.local_defined_code_vec_storage.index_unchecked(i).code_type_ptr ||
                a.local_defined_code_vec_storage.index_unchecked(i).func_ptr != ::std::addressof(x) || x.wasm_code_ptr == nullptr)
            { return false; }
            auto const image_begin{reinterpret_cast<::std::uintptr_t>(file_->source_cbegin())};
            auto const image_end{reinterpret_cast<::std::uintptr_t>(file_->source_cend())};
            auto const code_begin{reinterpret_cast<::std::uintptr_t>(x.wasm_code_ptr->body.code_begin)};
            auto const expression{reinterpret_cast<::std::uintptr_t>(x.wasm_code_ptr->body.expr_begin)};
            auto const code_end{reinterpret_cast<::std::uintptr_t>(x.wasm_code_ptr->body.code_end)};
            if(image_begin == 0u || image_begin >= image_end || code_begin < image_begin ||
                expression < code_begin || expression >= code_end || code_end > image_end) { return false; }
        }
        for(::std::size_t i{}; i != a.local_defined_global_vec_storage.size(); ++i)
        {
            auto const& x{a.local_defined_global_vec_storage.index_unchecked(i)};
            auto const& y{p.local_defined_global_vec_storage.index_unchecked(i)};
            if(x.global_type_ptr != y.global_type_ptr || x.local_global_type_ptr != y.local_global_type_ptr ||
                x.global.kind != y.global.kind || x.global.is_mutable != y.global.is_mutable ||
                x.owner_module_rt_ptr != module_view_) { return false; }
        }
        for(::std::size_t i{}; i != a.local_defined_table_vec_storage.size(); ++i)
        {
            auto const& x{a.local_defined_table_vec_storage.index_unchecked(i)};
            auto const& y{p.local_defined_table_vec_storage.index_unchecked(i)};
            if(x.table_type_ptr != y.table_type_ptr || x.initializer_expr != y.initializer_expr ||
                x.owner_module_rt_ptr != module_view_) { return false; }
        }
        for(::std::size_t i{}; i != a.local_defined_tag_vec_storage.size(); ++i)
        {
            auto const& x{a.local_defined_tag_vec_storage.index_unchecked(i)};
            auto const& y{p.local_defined_tag_vec_storage.index_unchecked(i)};
            if(x.function_type_ptr != y.function_type_ptr || x.type_index != y.type_index ||
                !x.exception_identity || x.exception_identity == y.exception_identity) { return false; }
        }
        for(::std::size_t i{}; i != a.local_defined_data_vec_storage.size(); ++i)
        { if(a.local_defined_data_vec_storage.index_unchecked(i).data_type_ptr != p.local_defined_data_vec_storage.index_unchecked(i).data_type_ptr) { return false; } }
        for(::std::size_t i{}; i != a.local_defined_element_vec_storage.size(); ++i)
        { if(a.local_defined_element_vec_storage.index_unchecked(i).element_type_ptr != p.local_defined_element_vec_storage.index_unchecked(i).element_type_ptr) { return false; } }
        if(a.imported_function_vec_storage.size() != p.imported_function_vec_storage.size() ||
            a.imported_memory_vec_storage.size() != imported_memory_targets_.size() ||
            a.imported_global_vec_storage.size() != shared_globals_.size() ||
            a.imported_tag_vec_storage.size() != p.imported_tag_vec_storage.size() ||
            !a.imported_table_vec_storage.empty()) { return false; }
        using namespace ::uwvm2::uwvm::runtime::storage;
        for(::std::size_t i{}; i != a.imported_function_vec_storage.size(); ++i)
        {
            auto const& x{a.imported_function_vec_storage.index_unchecked(i)};
            auto const& y{p.imported_function_vec_storage.index_unchecked(i)};
            if(x.link_kind != y.link_kind || x.import_type_ptr != y.import_type_ptr)return false;
            if(y.link_kind==imported_function_link_kind::local_imported)
            {
                if(x.target.local_imported.module_ptr!=y.target.local_imported.module_ptr ||
                    x.target.local_imported.index!=y.target.local_imported.index)return false;
            }
            else
            {
                auto const location=locate_source_function(y);auto const* peer=node_for_parent(location.parent);
                if(!peer || !peer->module_view_)return false;
                if(location.imported)
                {if(x.target.imported_ptr!=&peer->module_view_->imported_function_vec_storage.index_unchecked(location.index))return false;}
                else if(x.target.defined_ptr!=&peer->module_view_->local_defined_function_vec_storage.index_unchecked(location.index))return false;
            }
        }
        for(::std::size_t i{}; i != imported_memory_targets_.size(); ++i)
        {
            auto const& x{a.imported_memory_vec_storage.index_unchecked(i)};
            auto const& y{p.imported_memory_vec_storage.index_unchecked(i)};
            if(x.link_kind != imported_memory_storage_t::imported_memory_link_kind::defined ||
                x.target.defined_ptr != imported_memory_targets_[i] || !source_contains_memory(x.target.defined_ptr) ||
                x.import_type_ptr != y.import_type_ptr || x.effective_limits.min != y.effective_limits.min ||
                x.effective_limits.max != y.effective_limits.max || x.effective_limits.present_max != y.effective_limits.present_max)
            { return false; }
        }
        for(::std::size_t i{}; i != shared_globals_.size(); ++i)
        {
            auto const& x{a.imported_global_vec_storage.index_unchecked(i)};
            if(x.link_kind != imported_global_storage_t::imported_global_link_kind::defined ||
                x.target.defined_ptr != shared_globals_[i] || !source_contains_global(x.target.defined_ptr) ||
                x.import_type_ptr != p.imported_global_vec_storage.index_unchecked(i).import_type_ptr) { return false; }
        }
        for(::std::size_t i{}; i != a.imported_tag_vec_storage.size(); ++i)
        {
            auto const& x{a.imported_tag_vec_storage.index_unchecked(i)};
            auto const& y{p.imported_tag_vec_storage.index_unchecked(i)};
            if(x.import_type_ptr != y.import_type_ptr || x.function_type_ptr != y.function_type_ptr ||
                x.resolved_tag != y.resolved_tag || x.imported_target != y.imported_target || x.defined_target != y.defined_target)
            { return false; }
        }
        return true;
    }

#ifdef UWVM_MODULE
    } // Global-linkage World; initialization_context remains initializer-owned.
#endif
    namespace details
    {
        inline ::std::shared_ptr<::uwvm2::uwvm::runtime::storage::gc_lease_owner>
        initialization_context<initialization_purpose::threaded_child>::make_lease_owner() const
        {
            auto owner{owner_->lock()};
            if(!owner) { throw ::std::bad_alloc{}; }
            using lease = ::uwvm2::uwvm::runtime::storage::gc_lease_owner;
            return ::std::allocate_shared<lease>(threaded_std_allocator<lease>{owner->allocation_});
        }
        inline ::std::shared_ptr<::uwvm2::uwvm::runtime::storage::tag_instance_identity const>
        initialization_context<initialization_purpose::threaded_child>::make_tag_identity() const
        {
            auto owner{owner_->lock()};
            if(!owner) { throw ::std::bad_alloc{}; }
            using tag = ::uwvm2::uwvm::runtime::storage::tag_instance_identity const;
            return ::std::allocate_shared<tag>(threaded_std_allocator<tag>{owner->allocation_});
        }
        inline void initialization_context<initialization_purpose::threaded_child>::prepare_table(
            ::uwvm2::uwvm::runtime::storage::local_defined_table_storage_t& table, ::std::size_t index) const noexcept
        {
            auto owner{owner_->lock()};
            if(!owner || index >= owner->prepared_tables_.size()) { ::std::terminate(); }
            // Construction-only native borrow. No published child engine can
            // observe this world before its factory has completed.
            table.elems = ::std::move(const_cast<ThreadedChildWorld&>(*owner).prepared_tables_[index]);
        }
        inline ::std::shared_ptr<::uwvm2::uwvm::runtime::storage::gc_object_store>
        initialization_context<initialization_purpose::threaded_child>::make_store(
            ::uwvm2::uwvm::runtime::storage::gc_type::recursive_type_section const& section,
            ::std::shared_ptr<::uwvm2::uwvm::runtime::storage::gc_lease_owner> const& leases,
            [[maybe_unused]] ::uwvm2::uwvm::runtime::storage::gc_function_type_match_callback callback) const
        { return ThreadedChildWorld::make_store(section, leases, *owner_); }
    }

#ifdef UWVM_MODULE
    extern "C++" {
#endif
    template<typename Vector>
    inline bool ThreadedChildWorld::preallocate_vector(Vector& vector, ::std::size_t count) noexcept
    {
        using value = typename Vector::value_type;
        if(count == 0uz) { return true; }
        // Deallocation later goes through this EXACT allocator implementation.
        // No malloc block is adopted by a mimalloc/custom/Windows allocator.
        using compatible = ::fast_io::generic_allocator_adapter<::fast_io::c_malloc_allocator>;
        if constexpr(!::std::is_same_v<typename Vector::allocator_type, compatible>) { return false; }
        else
        {
            if(!vector.empty() || vector.capacity() != 0uz ||
                count > static_cast<::std::size_t>((::std::numeric_limits<::std::ptrdiff_t>::max)()) / sizeof(value))
            { return false; }
            auto* memory{allocation_.get().template try_c_malloc_vector_storage<value>(count)};
            if(memory == nullptr) { return false; }
            // The matching allocator storage starts an implicit-lifetime region.
            // No T is live until normal vector resize/push constructs it.
            vector.imp = {memory, memory, memory + count};
            return true;
        }
    }

    inline bool ThreadedChildWorld::preallocate()
    {
        auto& out{*module_};
        auto const& p{*parent_};
        if(!preallocate_vector(out.imported_function_vec_storage, p.imported_function_vec_storage.size()) ||
            !preallocate_vector(out.imported_memory_vec_storage, p.imported_memory_vec_storage.size()) ||
            !preallocate_vector(out.imported_global_vec_storage, p.imported_global_vec_storage.size()) ||
            !preallocate_vector(out.imported_tag_vec_storage, p.imported_tag_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_function_vec_storage, p.local_defined_function_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_code_vec_storage, p.local_defined_code_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_global_vec_storage, p.local_defined_global_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_table_vec_storage, p.local_defined_table_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_tag_vec_storage, p.local_defined_tag_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_element_vec_storage, p.local_defined_element_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_data_vec_storage, p.local_defined_data_vec_storage.size()) ||
            !preallocate_vector(out.declared_ref_funcidx_vec_storage, p.declared_ref_funcidx_vec_storage.size()))
        { return false; }
        prepared_tables_.resize(p.local_defined_table_vec_storage.size());
        for(::std::size_t i{}; i != prepared_tables_.size(); ++i)
        {
            if(!preallocate_vector(prepared_tables_[i],
                static_cast<::std::size_t>(p.local_defined_table_vec_storage.index_unchecked(i).table_type_ptr->limits.min)))
            { return false; }
        }
        ::std::size_t expressions{};
        for(auto const& segment : p.local_defined_element_vec_storage)
        {
            auto const count{segment.element_type_ptr->storage.segment.vec_expr.size()};
            if(count > (::std::numeric_limits<::std::size_t>::max)() - expressions) { return false; }
            expressions += count;
        }
        // First profile admits only local funcref tables. Over-reserving this
        // one actual family is safe; no other family temporary can allocate.
        return preallocate_vector(out.element_expr_funcref_vec_storage, expressions);
    }

    inline threaded_child_error ThreadedChildWorld::preflight()
    {
        using namespace ::uwvm2::uwvm::runtime::storage;
        if(!source_type::has_canonical_owner(source_) || admission_->source_generation() == 0u)
        { return threaded_child_error::invalid_source; }
        file_ = source_->actual_validated_file(source_id_,
            admission_->source_validation_epoch(), parent_);
        // Single selected executable instance. Preloaded providers may own its
        // actual imported memory/global objects, but never supply child code.
        if(file_ == nullptr ||
            file_->binfmt_ver != 1u || !file_->has_owned_source_image())
        { return threaded_child_error::invalid_source; }
        if constexpr(!::uwvm2::object::memory::linear::native_memory_t::support_multi_thread)
        { return threaded_child_error::unsupported_import; }
        if constexpr(::std::endian::native != ::std::endian::little)
        { return threaded_child_error::unsupported_initialization_shape; }
        // Logging is synchronized independently of each private child world.
        // A diagnostic option must not change executable Wasm admission.
        // Zero local-memory bytes is valid for an imported-shared-only shape.
        // Actual local declarations retain their count/child-shape checks and
        // per-memory byte accounting below; the shared budget is independent.
        if(limits_.max_runtime_modules == 0uz || admission_->max_table_elements() == 0uz ||
            admission_->max_total_segment_bytes() == 0u || admission_->max_shared_memory_bytes() == 0u)
        { return threaded_child_error::count_limit; }
        auto const& p{*parent_};
        if(!p.gc_store || !p.gc_store->valid()) { return threaded_child_error::invalid_store; }
        if(kind_ == storage_kind::fresh_child && !p.local_defined_memory_vec_storage.empty())
        { return threaded_child_error::unsupported_initialization_shape; }
        if(p.constant_expressions_require_extended_const)
        { return threaded_child_error::unsupported_initialization_shape; }
        for(auto const& global : p.local_defined_global_vec_storage)
        {
            if(global.local_global_type_ptr == nullptr || global.local_global_type_ptr->expr.opcodes.size() != 1uz)
            { return threaded_child_error::unsupported_initialization_shape; }
            auto const opcode{static_cast<unsigned>(global.local_global_type_ptr->expr.opcodes.front_unchecked().opcode)};
            if(opcode != 0x41u && opcode != 0x42u && opcode != 0x43u && opcode != 0x44u &&
                opcode != 0xfdu && opcode != 0xd0u && opcode != 0xd2u && opcode != 0x23u)
            { return threaded_child_error::unsupported_initialization_shape; }
            if(opcode == 0x23u && global.local_global_type_ptr->expr.opcodes.front_unchecked().storage.imported_global_idx >= p.imported_global_vec_storage.size())
            { return threaded_child_error::unsupported_initialization_shape; }
        }
        if(p.imported_function_vec_storage.size() > limits_.max_imported_functions ||
            p.imported_memory_vec_storage.size() > limits_.max_imported_memories ||
            p.imported_global_vec_storage.size() > limits_.max_imported_globals ||
            p.imported_tag_vec_storage.size() > limits_.max_imported_tags ||
            p.local_defined_function_vec_storage.size() > limits_.max_local_defined_functions ||
            p.local_defined_code_vec_storage.size() > limits_.max_local_defined_codes ||
            p.local_defined_table_vec_storage.size() > limits_.max_local_defined_tables ||
            p.local_defined_memory_vec_storage.size() > limits_.max_local_defined_memories ||
            p.local_defined_global_vec_storage.size() > limits_.max_local_defined_globals ||
            p.local_defined_tag_vec_storage.size() > limits_.max_local_defined_tags ||
            p.local_defined_element_vec_storage.size() > limits_.max_local_defined_elements ||
            p.local_defined_data_vec_storage.size() > limits_.max_local_defined_datas)
        { return threaded_child_error::count_limit; }
        // Imported mutable tables can contain parent-instance function addresses.
        // This slice does not counterfeit an independent child table/code graph.
        if(!p.imported_table_vec_storage.empty()) { return threaded_child_error::unsupported_import; }
        for(auto const& f : p.imported_function_vec_storage)
        {
            if(f.import_type_ptr == nullptr) { return threaded_child_error::unsupported_import; }
            if(f.link_kind==imported_function_link_kind::local_imported)
            { if(f.target.local_imported.module_ptr==nullptr)return threaded_child_error::unsupported_import; }
            else if(locate_source_function(f).parent==nullptr) { return threaded_child_error::unsupported_import; }
        }

        // Resolve only authenticated canonical-source members; flattening this
        // cold chain does not clone any memory bytes, maximum or growth lock.
        shared_memories_.reserve(p.imported_memory_vec_storage.size());
        imported_memory_targets_.reserve(p.imported_memory_vec_storage.size());
        for(auto const& original : p.imported_memory_vec_storage)
        {
            if(original.import_type_ptr == nullptr ||
                !details::wasm_memory_is_shared(original.import_type_ptr->imports.storage.memory))
            { return threaded_child_error::unsupported_import; }
            auto const* cursor{::std::addressof(original)};
            ::std::size_t visited{};
            for(;;)
            {
                if(cursor == nullptr || !source_contains_import(cursor, &module_type::imported_memory_vec_storage))
                { return threaded_child_error::unsupported_import; }
                if(visited++ >= limits_.max_runtime_modules)
                { return threaded_child_error::unsupported_import; }
                using kind = imported_memory_storage_t::imported_memory_link_kind;
                if(cursor->link_kind == kind::imported) { cursor = cursor->target.imported_ptr; continue; }
                if(cursor->link_kind != kind::defined || !source_contains_memory(cursor->target.defined_ptr))
                { return threaded_child_error::unsupported_import; }
                auto* leaf{cursor->target.defined_ptr};
                if(leaf->memory_type_ptr == nullptr || !details::wasm_memory_is_shared(*leaf->memory_type_ptr) ||
                    !leaf->effective_limits.present_max || !supports_shared_child_memory(leaf->memory))
                { return threaded_child_error::unsupported_import; }
                auto const maximum{static_cast<::std::uint_least64_t>(leaf->effective_limits.max)};
                if(maximum > admission_->max_shared_memory_bytes() / 65536u)
                { return threaded_child_error::memory_limit; }
                shared_memories_.push_back(leaf);
                imported_memory_targets_.push_back(leaf);
                break;
            }
        }

        // Imported globals are the same objects, while local/TLS globals below
        // are rebuilt from expressions. Only numeric shared global carriers are
        // admitted here; foreign function/GC references need a multi-world linker.
        shared_globals_.reserve(p.imported_global_vec_storage.size());
        for(auto const& original : p.imported_global_vec_storage)
        {
            auto const* cursor{::std::addressof(original)};
            ::std::size_t visited{};
            for(;;)
            {
                if(cursor == nullptr || !source_contains_import(cursor, &module_type::imported_global_vec_storage))
                { return threaded_child_error::unsupported_import; }
                if(visited++ >= limits_.max_runtime_modules)
                { return threaded_child_error::unsupported_import; }
                using kind = imported_global_storage_t::imported_global_link_kind;
                if(cursor->link_kind == kind::imported) { cursor = cursor->target.imported_ptr; continue; }
                if(cursor->link_kind != kind::defined || !source_contains_global(cursor->target.defined_ptr))
                { return threaded_child_error::unsupported_import; }
                auto* leaf{cursor->target.defined_ptr};
                if(leaf->global.is_mutable || leaf->global.kind == ::uwvm2::object::global::global_type::wasm_ref)
                { return threaded_child_error::unsupported_import; }
                shared_globals_.push_back(leaf);
                break;
            }
        }
        ::std::size_t table_elements{};
        for(auto const& t : p.local_defined_table_vec_storage)
        {
            if(t.table_type_ptr == nullptr) { return threaded_child_error::declaration_mismatch; }
            if(static_cast<unsigned>(t.table_type_ptr->reftype) != 0x70u ||
                (t.initializer_expr != nullptr && t.initializer_expr->opcodes.size() > 1uz))
            { return threaded_child_error::unsupported_initialization_shape; }
            if(t.initializer_expr != nullptr && !t.initializer_expr->opcodes.empty())
            {
                auto const opcode{static_cast<unsigned>(t.initializer_expr->opcodes.front_unchecked().opcode)};
                if(opcode != 0xd0u && opcode != 0xd2u)
                { return threaded_child_error::unsupported_initialization_shape; }
            }
            auto const minimum{t.table_type_ptr->limits.min};
            if(minimum > admission_->max_table_elements() - table_elements)
            { return threaded_child_error::table_limit; }
            table_elements += static_cast<::std::size_t>(minimum);
        }
        ::std::uint_least64_t local_bytes{};
        for(auto const& m : p.local_defined_memory_vec_storage)
        {
            if(m.memory_type_ptr == nullptr) { return threaded_child_error::declaration_mismatch; }
            if(details::wasm_memory_is_shared(*m.memory_type_ptr))
            {
                if(kind_ == storage_kind::fresh_child) { return threaded_child_error::unsupported_local_shared_memory; }
                if(!supports_shared_child_memory(m.memory) || !m.effective_limits.present_max ||
                    static_cast<::std::uint_least64_t>(m.effective_limits.max) > admission_->max_shared_memory_bytes() / 65536u)
                { return threaded_child_error::memory_limit; }
                // No mutable pointer is created by casting const. The canonical
                // source's already-resolved runtime import records carry mutable
                // leaves; a defined main memory stays a read-only storage borrow.
                shared_memories_.push_back(::std::addressof(m));
                continue;
            }
            auto const minimum{static_cast<::std::uint_least64_t>(m.effective_limits.min)};
            if(minimum > (admission_->max_local_memory_bytes() - local_bytes) / 65536u)
            { return threaded_child_error::memory_limit; }
            local_bytes += minimum * 65536u;
        }
        ::std::uint_least64_t segment_bytes{};
        for(auto const& segment : p.local_defined_data_vec_storage)
        {
            auto const* declaration{segment.data_type_ptr};
            if(declaration == nullptr) { return threaded_child_error::declaration_mismatch; }
            auto const& data{declaration->storage.segment};
            if(kind_ == storage_kind::fresh_child && data.active && data.memory_idx < p.imported_memory_vec_storage.size())
            { return threaded_child_error::unsupported_shared_write; }
            auto const size{data.byte.begin == data.byte.end ? 0uz :
                static_cast<::std::size_t>(data.byte.end - data.byte.begin)};
            if(size > admission_->max_total_segment_bytes() - segment_bytes)
            { return threaded_child_error::segment_limit; }
            segment_bytes += size;
        }
        if(shared_memories_.empty()) { return threaded_child_error::missing_shared_memory; }
        // The evaluator may materialize one reference slot per declared element
        // even when the corresponding live parent segment has been dropped.
        for(auto const& segment : p.local_defined_element_vec_storage)
        {
            if(segment.element_type_ptr == nullptr) { return threaded_child_error::declaration_mismatch; }
            auto const& declaration{segment.element_type_ptr->storage.segment};
            if(declaration.active && (declaration.expr.opcodes.size() != 1uz ||
                static_cast<unsigned>(declaration.expr.opcodes.front_unchecked().opcode) != 0x41u))
            { return threaded_child_error::unsupported_initialization_shape; }
            // Prevent a GC/externref constant evaluator or foreign root lease
            // from introducing a hidden allocation after preallocation.
            if(::uwvm2::uwvm::runtime::storage::runtime_element_family(segment, p) !=
                ::uwvm2::uwvm::runtime::storage::runtime_table_reference_family::function)
            { return threaded_child_error::unsupported_initialization_shape; }
            for(auto const& expression : declaration.vec_expr)
            {
                if(expression.opcodes.size() != 1uz)
                { return threaded_child_error::unsupported_initialization_shape; }
                auto const opcode{static_cast<unsigned>(expression.opcodes.front_unchecked().opcode)};
                if(opcode != 0xd0u && opcode != 0xd2u)
                { return threaded_child_error::unsupported_initialization_shape; }
            }
            auto const count{declaration.vec_funcidx.empty() ? declaration.vec_expr.size() : declaration.vec_funcidx.size()};
            constexpr auto slot_size{sizeof(local_defined_table_elem_storage_t)};
            if(count > (admission_->max_total_segment_bytes() - segment_bytes) / slot_size)
            { return threaded_child_error::segment_limit; }
            segment_bytes += count * slot_size;
        }
        auto const& start{::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<
            ::uwvm2::parser::wasm::standard::wasm1::features::start_section_storage_t>(
                file_->wasm_module_storage.wasm_binfmt_ver1_storage.sections)};
        if(start.sec_span.sec_begin != nullptr)
        {
            if(shared_memories_.size() != 1uz) { return threaded_child_error::unsupported_start; }
            initialization_ = threaded_once_init_details::validate(p, shared_memories_[0]->memory,
                static_cast<::std::size_t>(start.start_idx), kind_ == storage_kind::fresh_child,
                reinterpret_cast<::std::byte const*>(file_->source_cbegin()),
                reinterpret_cast<::std::byte const*>(file_->source_cend()), admission_->limits().max_child_code_bytes);
            if(!initialization_) { return threaded_child_error::unsupported_start; }
        }
        return threaded_child_error::ok;
    }

    inline bool ThreadedChildWorld::verify_thread_entry() noexcept
    {
        return [&]<::uwvm2::parser::wasm::concepts::wasm_feature... Fs>(
            ::uwvm2::utils::container::tuple<Fs...>) noexcept
        {
            using section = ::uwvm2::parser::wasm::standard::wasm1::features::export_section_storage_t<Fs...>;
            auto const& exports{::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<section>(
                file_->wasm_module_storage.wasm_binfmt_ver1_storage.sections)};
            for(auto const& e : exports.exports)
            {
                if(e.export_name != u8"wasi_thread_start") { continue; }
                if(e.exports.type != ::uwvm2::parser::wasm::standard::wasm1::type::external_types::func) { return false; }
                auto const index{static_cast<::std::size_t>(e.exports.storage.func_idx)};
                auto const imported{module_view_->imported_function_vec_storage.size()};
                if(index < imported || index - imported >= module_view_->local_defined_function_vec_storage.size()) { return false; }
                auto const* type{module_view_->local_defined_function_vec_storage.index_unchecked(index - imported).function_type_ptr};
                if(type == nullptr || type->parameter.begin == nullptr ||
                    type->parameter.end - type->parameter.begin != 2 || type->result.begin != type->result.end)
                { return false; }
                using value_type = ::uwvm2::uwvm::runtime::storage::wasm_binfmt1_final_value_type_t;
                if(type->parameter.begin[0] != value_type::i32 || type->parameter.begin[1] != value_type::i32) { return false; }
                thread_start_ = index;
                return true;
            }
            return false;
        }(::uwvm2::uwvm::wasm::feature::wasm_binfmt1_features);
    }

    inline threaded_child_error ThreadedChildWorld::build()
    {
        using namespace ::uwvm2::uwvm::runtime::storage;
        declaration_ = ::uwvm2::uwvm::wasm::type::all_module_t{
            .module_storage_ptr = {.wf = file_}, .type = ::uwvm2::uwvm::wasm::type::module_type_t::exec_wasm};
        declarations_.bind(file_->module_name, declaration_);
        module_ = ::std::addressof(owned_module_);
        module_view_ = module_;
        modules_.bind(file_->module_name, *module_);
        public_modules_.bind(file_->module_name, *module_);
        if(!preallocate()) { return threaded_child_error::allocation_failure; }
        current_ = file_->module_name;
        auto world{context()};
        // Final map address is already stable BEFORE declarations, local
        // identities, reference expressions or code addresses can borrow it.
        details::initialize_from_wasm_file_in_context(world, *file_, *module_);
#if defined(UWVM_RUNTIME_LLVM_JIT)
        module_->module_name = file_->module_name;
#endif
        auto const& parent{*parent_};
        if(module_->imported_function_vec_storage.size() != parent.imported_function_vec_storage.size() ||
            module_->imported_memory_vec_storage.size() != shared_memories_.size() ||
            module_->imported_global_vec_storage.size() != shared_globals_.size() ||
            module_->imported_tag_vec_storage.size() != parent.imported_tag_vec_storage.size())
        { return threaded_child_error::declaration_mismatch; }
        // Only already authenticated imported object links are inherited. No
        // local global/table/data/drop/GC store is copied from the live parent.
        for(::std::size_t i{}; i != module_->imported_function_vec_storage.size(); ++i)
        {
            auto& target{module_->imported_function_vec_storage.index_unchecked(i)};
            auto const& original{parent.imported_function_vec_storage.index_unchecked(i)};
            target.target = original.target;
            target.import_type_ptr = original.import_type_ptr;
            target.link_kind = original.link_kind;
            target.is_opposite_side_imported = original.is_opposite_side_imported;
        }
        for(::std::size_t i{}; i != shared_memories_.size(); ++i)
        {
            auto& target{module_->imported_memory_vec_storage.index_unchecked(i)};
            auto const& original{parent.imported_memory_vec_storage.index_unchecked(i)};
            target.target.defined_ptr = imported_memory_targets_[i];
            target.link_kind = imported_memory_storage_t::imported_memory_link_kind::defined;
            target.import_type_ptr = original.import_type_ptr;
            target.effective_limits = original.effective_limits;
            target.is_opposite_side_imported = original.is_opposite_side_imported;
        }
        for(::std::size_t i{}; i != shared_globals_.size(); ++i)
        {
            auto& target{module_->imported_global_vec_storage.index_unchecked(i)};
            auto const& original{parent.imported_global_vec_storage.index_unchecked(i)};
            target.target.defined_ptr = shared_globals_[i];
            target.link_kind = imported_global_storage_t::imported_global_link_kind::defined;
            target.import_type_ptr = original.import_type_ptr;
            target.is_opposite_side_imported = original.is_opposite_side_imported;
        }
        for(::std::size_t i{}; i != module_->imported_tag_vec_storage.size(); ++i)
        { module_->imported_tag_vec_storage.index_unchecked(i) = parent.imported_tag_vec_storage.index_unchecked(i); }
        details::finalize_wasm1_globals_after_linking_in_context(world);
        details::initialize_wasm3_table_expressions_after_linking_in_context(world);
        details::finalize_wasm1_offsets_after_linking_in_context(world);
        if(!module_->gc_store || !module_->gc_store->valid()) { return threaded_child_error::invalid_store; }
        // Preflight forbids any active write to a shared/imported object. This
        // installs only child-local tables/memory and sets the CHILD drop flags.
        if(details::apply_wasm1_active_element_and_data_segments_for_module_in_context(world, file_->module_name, *module_) !=
           details::active_segment_result::ok)
        { return threaded_child_error::unsupported_initialization_shape; }
        // Independent LLVM engines cold-bind their OWN compact table views;
        // neither child nor actual-parent ordinary view arrays are allocated or mutated here.
        current_ = {};
        if(graph_==this && !verify_thread_entry()) { return threaded_child_error::invalid_thread_entry; }
        // Deliberately no global initializer serial/publication and no start
        // call. Child publication is the Root group's real engine+TID handoff.
        return threaded_child_error::ok;
    }
#ifdef UWVM_MODULE
    }
#endif
}
#endif
