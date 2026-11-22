// SOURCE-ONLY candidate: independent WASI Threads child storage.
// A child is not a checkpoint restore and does not select the global registry.
#pragma once
#include <uwvm2/utils/container/standard.h>
#ifndef UWVM_MODULE
# include <memory>
# include <bit>
# include <vector>
# include <limits>
# include <type_traits>
# include <utility>
# include "threaded_allocation.h"
# include "init.h"
# include <uwvm2/runtime/wasm_threads/admission.h>
#endif

namespace uwvm2::runtime::lib
{
    extern "C++" { class ThreadedLlvmChild; class ThreadedInterpreterChild; class runtime_threaded_profile_bridge; }
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
        friend class ::uwvm2::runtime::lib::runtime_threaded_profile_bridge;
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
        ::uwvm2::utils::container::std_vector<::std::shared_ptr<ThreadedChildWorld>, threaded_std_allocator<::std::shared_ptr<ThreadedChildWorld>>> peers_;
        [[nodiscard]] ThreadedChildWorld const* node(::std::size_t index) const noexcept
        { return index==0u ? this : index<=peers_.size() ? peers_[index-1u].get() : nullptr; }
        [[nodiscard]] threaded_child_error build_graph();
        struct function_location { module_type const* parent{}; ::std::size_t index{}; bool imported{}; };
        [[nodiscard]] function_location locate_source_function(
            ::uwvm2::uwvm::runtime::storage::imported_function_storage_t const&) const noexcept;
        [[nodiscard]] function_location locate_source_global(
            ::uwvm2::uwvm::runtime::storage::local_defined_global_storage_t const*) const noexcept;
        [[nodiscard]] function_location locate_source_table(
            ::uwvm2::uwvm::runtime::storage::local_defined_table_storage_t const*) const noexcept;
        [[nodiscard]] function_location locate_source_memory(memory_type const*) const noexcept;
        [[nodiscard]] function_location locate_source_tag(
            ::uwvm2::uwvm::runtime::storage::local_defined_tag_storage_t const*) const noexcept;
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
        ::uwvm2::utils::container::std_vector<memory_type const*, threaded_std_allocator<memory_type const*>> shared_memories_;
        ::uwvm2::utils::container::std_vector<memory_type*, threaded_std_allocator<memory_type*>> imported_memory_targets_;
        ::uwvm2::utils::container::std_vector<global_type*, threaded_std_allocator<global_type*>> shared_globals_;
        using table_type = ::uwvm2::uwvm::runtime::storage::local_defined_table_storage_t;
        ::uwvm2::utils::container::std_vector<table_type*, threaded_std_allocator<table_type*>> imported_table_targets_;
        using table_array = ::uwvm2::utils::container::vector<::uwvm2::uwvm::runtime::storage::local_defined_table_elem_storage_t>;
        using table_charge=::uwvm2::utils::allocator::resource::reservation<::uwvm2::utils::allocator::resource::pool::table>;
        ::uwvm2::utils::container::std_vector<table_charge, threaded_std_allocator<table_charge>> prepared_table_charges_;
        ::uwvm2::utils::container::std_vector<table_array, threaded_std_allocator<table_array>> prepared_tables_;
        ::uwvm2::utils::container::std_vector<::std::size_t,threaded_std_allocator<::std::size_t>> initialization_order_;
        ::std::size_t thread_start_{};
        ::std::optional<::std::size_t> initialization_{};
        ThreadedValidatedCode validated_{this};

        explicit ThreadedChildWorld(::std::shared_ptr<admission_type const> admission, threaded_allocation_pin allocation)
            : allocation_{::std::move(allocation)}, admission_{::std::move(admission)}, source_{admission_->source_owner()},
              parent_{::std::addressof(admission_->source_module())}, source_id_{admission_->source_module_id()}, graph_{this},
              peers_{threaded_std_allocator<::std::shared_ptr<ThreadedChildWorld>>{allocation_}},
              limits_{admission_->initializer_limits()},
              shared_memories_{threaded_std_allocator<memory_type const*>{allocation_}},
              imported_memory_targets_{threaded_std_allocator<memory_type*>{allocation_}},
              shared_globals_{threaded_std_allocator<global_type*>{allocation_}},
              imported_table_targets_{threaded_std_allocator<table_type*>{allocation_}},
              prepared_table_charges_{threaded_std_allocator<table_charge>{allocation_}},
              prepared_tables_{threaded_std_allocator<table_array>{allocation_}},
              initialization_order_{threaded_std_allocator<::std::size_t>{allocation_}} {}

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
        template<typename Member>
        [[nodiscard]] ::std::size_t source_import_count(Member member) const noexcept
        {
            ::std::size_t count{};
            for(auto const& [name,module]:source_->registry())
            {
                static_cast<void>(name);
                auto const size=(module.*member).size();
                if(size>SIZE_MAX-count)return SIZE_MAX;
                count+=size;
            }
            return count;
        }
        [[nodiscard]] threaded_child_error preflight();
        [[nodiscard]] threaded_child_error build();
        [[nodiscard]] threaded_child_error finish_initialization();
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

        // Invoked by the genuine execution bridge after worker admission and
        // journal enter, before its module start or wasi_thread_start. The
        // compiler never writes a live imported memory on a child's behalf.
        [[nodiscard]] details::active_segment_result initialize_imported_data() const noexcept
        {
            if(kind_ != storage_kind::fresh_child) { return details::active_segment_result::ok; }
            auto* self = const_cast<ThreadedChildWorld*>(this);
            for(auto m:initialization_order_)
            {
                auto* member = m == 0u ? self : self->peers_[m - 1u].get();
                auto context = member->context();
                auto result = details::apply_wasm1_active_element_and_data_segments_for_module_in_context(
                    context, member->file_->module_name, *member->module_);
                if(result != details::active_segment_result::ok) { return result; }
            }
            if(!initialize_private_global_tags())return details::active_segment_result::memtag_initialization_failed;
            return details::active_segment_result::ok;
        }
        [[nodiscard]] bool initialize_private_global_tags() const noexcept
        {
            namespace mt=::uwvm2::extension::memtag;
            namespace rt_mt=::uwvm2::runtime::memtag;
            if(::uwvm2::uwvm::runtime::runtime_mode::runtime_disable_memtag)return true;
            // Source sections were validated before admission. Resolve aliases
            // again against this private graph. Borrowed shared owners retain
            // their existing tags; only newly instantiated owners are tagged.
            using custom_t=::uwvm2::parser::wasm::standard::wasm1::features::custom_section_storage_t;
            using shadow_key=mt::shadow_storage const*;
            using color_usage=rt_mt::static_color_usage<>;
            using color_allocator=threaded_std_allocator<::std::pair<shadow_key const,color_usage>>;
            ::uwvm2::utils::container::std_map<shadow_key,color_usage,::std::less<shadow_key>,color_allocator>
                static_colors{::std::less<shadow_key>{}, color_allocator{allocation_}};
            for(auto m:initialization_order_)
            {
                auto const& parsed=file(m)->wasm_module_storage.wasm_binfmt_ver1_storage;
                auto const& customs=::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<custom_t>(parsed.sections);
                for(auto const& section:customs.customs)
                {
                    if(section.custom_name!=mt::linear_globals_section_name)continue;
                    auto const begin=reinterpret_cast<::std::uintptr_t>(section.custom_begin);
                    auto const end=reinterpret_cast<::std::uintptr_t>(section.sec_span.sec_end);
                    auto const base=reinterpret_cast<::std::uintptr_t>(file(m)->source_cbegin());
                    if(begin<base||end<begin||end-base>file(m)->source_size())return false;
                    bool success=true;
                    auto checked=mt::visit_linear_globals({reinterpret_cast<::std::byte const*>(begin),static_cast<::std::size_t>(end-begin)},
                        [&](::std::uint32_t index,::std::uint64_t& bytes) noexcept
                        {auto* mem=native_memory(m,index);if(!mem)return false;bytes=mem->get_page_size()*UINT64_C(65536);return true;},
                        [&](mt::linear_global_descriptor descriptor) noexcept
                        {
                            if(!success||!descriptor.byte_length)return;
                            auto* mem=native_memory(m,descriptor.memory_index);if(!mem){success=false;return;}
                            bool owned{};
                            for(::std::size_t n{};n<module_count();++n)for(auto const& local:module(n)->local_defined_memory_vec_storage)
                                owned|=&local.memory==mem;
                            if(!owned)return;
                            if(!mem->memtag_shadow.enabled()){success=false;return;}
                            ::std::uint8_t tag{};
                            auto const& shadow{mem->memtag_shadow};
                            try
                            {
                                auto& colors{static_colors.try_emplace(&shadow,shadow.width(),shadow.hardware_selected()).first->second};
                                success=rt_mt::stage_static_allocation(shadow,descriptor.address,descriptor.byte_length,colors,tag) &&
                                    rt_mt::write_static_allocation(shadow,descriptor.address,descriptor.byte_length,tag);
                            }
                            catch(::std::bad_alloc const&) { success=false; }
                        });
                    if(!checked||!success)return false;
                }
            }
            return true;
        }

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
        { return create_for_module(admission,admission.source_module_id()); }
        // The source identity is resolved against the retained canonical graph,
        // never used as an array index or as a pointer supplied by a snapshot.
        // Runtime passes the authenticated executing module of thread-spawn.
        [[nodiscard]] static result create_for_module(admission_type const& admission,::std::size_t source_id)
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
            module_type const* selected{};
            for(auto const& [name,module]:child->source_->registry())
            {
                static_cast<void>(name);
                if(child->source_->bound_initialized_module_id(::std::addressof(module))==source_id)
                { selected=::std::addressof(module);break; }
            }
            if(!selected || !child->source_->actual_validated_file(source_id,
                admission.source_validation_epoch(),selected))
            { return {threaded_child_error::invalid_source,{}}; }
            child->parent_=selected;child->source_id_=source_id;
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
        [[nodiscard]] ::std::optional<::std::size_t> initialization_function_index(::std::size_t module=0u) const noexcept
        { auto selected=node(module);return selected?selected->initialization_: ::std::nullopt; }
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
        // Execution-only backing for escaped exception payloads. It consumes
        // the same admitted private-instance budget as GC/table allocations.
        [[nodiscard]] bool charge_execution_storage(::std::size_t bytes) const noexcept
        { return allocation_.get().charge(bytes); }
        [[nodiscard]] ::uwvm2::uwvm::runtime::storage::local_defined_table_storage_t* table(
            ::std::size_t module_index,::std::size_t index) const noexcept
        {
            using link=::uwvm2::uwvm::runtime::storage::imported_table_storage_t::imported_table_link_kind;
            auto const* m=module(module_index);if(!m)return nullptr;
            table_type* target{};
            if(index<m->imported_table_vec_storage.size())
            {
                auto const* imported=&m->imported_table_vec_storage.index_unchecked(index);
                ::std::size_t maximum{};for(::std::size_t n{};n<module_count();++n)maximum+=module(n)->imported_table_vec_storage.size();
                for(::std::size_t visited{};imported&&visited<maximum;++visited)
                {
                    bool owned{};for(::std::size_t n{};n<module_count();++n)for(auto const& r:module(n)->imported_table_vec_storage)
                        owned|=imported==&r;
                    if(!owned)return nullptr;
                    if(imported->link_kind==link::imported){imported=imported->target.imported_ptr;continue;}
                    if(imported->link_kind==link::defined)target=imported->target.defined_ptr;
                    break;
                }
            }
            else
            {
                index-=m->imported_table_vec_storage.size();
                if(index>=m->local_defined_table_vec_storage.size())return nullptr;
                target=const_cast<table_type*>(&m->local_defined_table_vec_storage.index_unchecked(index));
            }
            // No pointer from an import, journal, or guest is dereferenced before
            // membership in this retained generation is established.
            for(::std::size_t i{};i<module_count();++i)
                for(auto const& owned:module(i)->local_defined_table_vec_storage)
                    if(target==&owned && owned.owner_module_rt_ptr==module(i))return target;
            return nullptr;
        }
        [[nodiscard]] bool grow_table(table_type& table,::std::uint64_t delta,
            ::uwvm2::uwvm::runtime::storage::local_defined_table_elem_storage_t fill) const noexcept
        {
            auto const old=table.elems.size();auto const cap=admission().max_table_elements();
            ::std::size_t total{};bool owned{};
            for(::std::size_t m{};m<module_count();++m)
                for(auto const& t:module(m)->local_defined_table_vec_storage)
                {if(&t==&table)owned=true;if(t.elems.size()>cap-total)return false;total+=t.elems.size();}
            if(!owned||!table.table_type_ptr||delta>cap-total||delta>SIZE_MAX-old)return false;
            auto const& limits=table.table_type_ptr->limits;
            if(limits.present_max&&(old>limits.max||delta>limits.max-old))return false;
            if(delta>SIZE_MAX/sizeof(fill))return false;
            auto const bytes=static_cast<::std::size_t>(delta)*sizeof(fill);
            if(delta&&!allocation_.get().charge(bytes))return false;
            return ::uwvm2::uwvm::runtime::storage::try_grow_table_elements(table,old+static_cast<::std::size_t>(delta),fill);
        }
        [[nodiscard]] ::std::size_t memory_growth_limit(::uwvm2::object::memory::linear::native_memory_t const& memory,
            ::std::size_t declared) const noexcept
        {
            for(::std::size_t m{};m<module_count();++m)
            {
                for(auto const& owned:module(m)->local_defined_memory_vec_storage)
                    if(&owned.memory==&memory)return (::std::min)(declared,admission().max_local_memory_bytes());
                for(auto const* shared:node(m)->shared_memories_)
                    if(&shared->memory==&memory)return (::std::min)(declared,admission().max_shared_memory_bytes());
            }
            return 0u;
        }
        [[nodiscard]] bool charge_memory_growth(::uwvm2::object::memory::linear::native_memory_t const& memory,
            ::std::uint64_t pages) const noexcept
        {
            ::std::size_t total{};bool owned{};auto const cap=admission().max_local_memory_bytes();
            for(::std::size_t m{};m<module_count();++m)
                for(auto const& local:module(m)->local_defined_memory_vec_storage)
                {
                    auto const n=local.memory.get_page_size();if(n>(cap-total)/65536u)return false;
                    total+=n*65536u;if(&local.memory==&memory)owned=true;
                }
            if(owned)
            {
                if(pages>(cap-total)/65536u)return false;
                auto bytes=static_cast<::std::size_t>(pages)*65536u;
                if(memory.memtag_shadow.enabled())
                {
                    auto const old=static_cast<::std::uint64_t>(memory.get_page_size());
                    if(pages>UINT64_MAX-old)return false;
                    auto const before=memory_tag_storage_bytes(old,memory.sequentially_consistent_size);
                    auto const after=memory_tag_storage_bytes(old+pages,memory.sequentially_consistent_size);
                    if(before==SIZE_MAX||after==SIZE_MAX||after<before||after-before>SIZE_MAX-bytes)return false;
                    bytes+=after-before;
                }
                return allocation_.get().charge(bytes);
            }
            for(::std::size_t m{};m<module_count();++m)for(auto const* shared:node(m)->shared_memories_)
                if(&shared->memory==&memory)return true;
            return false;
        }
        // The private account includes committed shadow pages and their shared
        // tag gate. Virtual address reservation alone does not consume bytes.
        [[nodiscard]] static ::std::size_t memory_tag_storage_bytes(::std::uint64_t pages,bool shared) noexcept
        {
            auto const [page,valid]=::uwvm2::object::memory::platform_page::get_platform_page_size();
            if(!valid||page==0u||(page&(page-1u)))return SIZE_MAX;
            auto const stride=shared?sizeof(::std::atomic<::std::uint8_t>):sizeof(::std::uint8_t);
            if(pages>(SIZE_MAX-(page-1u))/(4096u*stride))return SIZE_MAX;
            auto const rounded=(static_cast<::std::size_t>(pages)*4096u*stride+page-1u)&~(page-1u);
            auto const gate=shared?sizeof(::uwvm2::utils::mutex::rwlock_t):0u;
            return rounded>SIZE_MAX-gate?SIZE_MAX:rounded+gate;
        }
        [[nodiscard]] memory_type* memory(::std::size_t module_index,::std::size_t index) const noexcept
        {
            auto const* member=module(module_index);if(!member)return nullptr;
            memory_type* target{};
            auto const imports=member->imported_memory_vec_storage.size();
            if(index<imports)
            {
                auto const* record=&member->imported_memory_vec_storage.index_unchecked(index);
                ::std::size_t maximum{};for(::std::size_t m{};m<module_count();++m)maximum+=module(m)->imported_memory_vec_storage.size();
                for(::std::size_t visited{};record&&visited<maximum;++visited)
                {
                    bool owned{};
                    for(::std::size_t m{};m<module_count();++m)for(auto const& r:module(m)->imported_memory_vec_storage)
                        if(record==&r)owned=true;
                    if(!owned)return nullptr;
                    using link=::uwvm2::uwvm::runtime::storage::imported_memory_storage_t::imported_memory_link_kind;
                    if(record->link_kind==link::imported){record=record->target.imported_ptr;continue;}
                    if(record->link_kind==link::defined)target=record->target.defined_ptr;
                    break;
                }
            }
            else
            {
                index-=imports;if(index>=member->local_defined_memory_vec_storage.size())return nullptr;
                target=const_cast<memory_type*>(&member->local_defined_memory_vec_storage.index_unchecked(index));
            }
            for(::std::size_t m{};m<module_count();++m)
            {
                for(auto const& owned:module(m)->local_defined_memory_vec_storage)if(target==&owned)return target;
                for(auto const* shared:node(m)->shared_memories_)if(target==shared)return target;
            }
            return nullptr;
        }
        [[nodiscard]] ::uwvm2::object::memory::linear::native_memory_t* native_memory(::std::size_t module,::std::size_t index) const noexcept
        {auto* record=memory(module,index);return record?&record->memory:nullptr;}
        [[nodiscard]] memory_type* resolved_private_import_memory(::std::size_t index) const noexcept
        {
            if(index>=imported_memory_targets_.size())return nullptr;
            auto* source=imported_memory_targets_[index];
            if(!source||!source->memory_type_ptr)return nullptr;
            if(details::wasm_memory_is_shared(*source->memory_type_ptr))return source;
            auto const location=locate_source_memory(source);auto const* peer=graph_->node_for_parent(location.parent);
            return !peer||!peer->module_view_||location.index>=peer->module_view_->local_defined_memory_vec_storage.size()?nullptr:
                const_cast<memory_type*>(&peer->module_view_->local_defined_memory_vec_storage.index_unchecked(location.index));
        }
        [[nodiscard]] ::std::uint_least64_t source_generation() const noexcept { return admission_->source_generation(); }
        [[nodiscard]] ::std::size_t source_module_id(::std::size_t index=0u) const noexcept
        { auto const* n=node(index); return n ? n->source_id_ : SIZE_MAX; }
        [[nodiscard]] module_type const* original_module(::std::size_t index) const noexcept
        { auto const* n=node(index); return n ? n->parent_ : nullptr; }
        [[nodiscard]] ::std::size_t initialization_module_at(::std::size_t position) const noexcept
        {return position<initialization_order_.size()?initialization_order_[position]:SIZE_MAX;}
        [[nodiscard]] ::std::size_t initialization_dependency_count(::std::size_t index) const noexcept
        {
            auto const* n=node(index);if(!n)return 0u;
            return n->parent_->imported_function_vec_storage.size()+n->shared_globals_.size()+
                n->parent_->imported_tag_vec_storage.size()+n->imported_table_targets_.size()+n->imported_memory_targets_.size();
        }
        [[nodiscard]] bool initialization_dependency(::std::size_t index,::std::size_t ordinal,
            ::std::size_t& output) const noexcept
        {
            auto const* n=node(index);if(!n)return false;
            module_type const* dependency{};
            auto const functions=n->parent_->imported_function_vec_storage.size();
            if(ordinal<functions)
                dependency=locate_source_function(n->parent_->imported_function_vec_storage.index_unchecked(ordinal)).parent;
            else
            {
                ordinal-=functions;
                if(ordinal<n->shared_globals_.size())dependency=locate_source_global(n->shared_globals_[ordinal]).parent;
                else
                {
                    ordinal-=n->shared_globals_.size();
                    if(ordinal<n->parent_->imported_tag_vec_storage.size())
                        dependency=locate_source_tag(n->parent_->imported_tag_vec_storage.index_unchecked(ordinal).resolved_tag).parent;
                    else if((ordinal-=n->parent_->imported_tag_vec_storage.size())<n->imported_table_targets_.size())
                        dependency=locate_source_table(n->imported_table_targets_[ordinal]).parent;
                    else if((ordinal-=n->imported_table_targets_.size())<n->imported_memory_targets_.size())
                        dependency=locate_source_memory(n->imported_memory_targets_[ordinal]).parent;
                }
            }
            if(!dependency)return false; // A native builtin has no Wasm start.
            for(::std::size_t m{};m<module_count();++m)
                if(node(m)->parent_==dependency){output=m;return true;}
            return false;
        }
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
            source_->actual_validated_file(world.source_id_, epoch_, world.parent_) == file_ &&
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
            a.local_defined_memory_vec_storage.size()!=p.local_defined_memory_vec_storage.size()) { return false; }
        for(::std::size_t i{};i<a.local_defined_memory_vec_storage.size();++i)
        {
            auto const& x=a.local_defined_memory_vec_storage.index_unchecked(i);
            auto const& y=p.local_defined_memory_vec_storage.index_unchecked(i);
            if(x.memory_type_ptr!=y.memory_type_ptr||x.effective_limits.min!=y.effective_limits.min||
               x.effective_limits.max!=y.effective_limits.max||x.effective_limits.present_max!=y.effective_limits.present_max||
               !x.memory.memory_begin||x.memory.memory_begin==y.memory.memory_begin)return false;
        }
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
            a.imported_table_vec_storage.size()!=imported_table_targets_.size()) { return false; }
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
                x.target.defined_ptr != resolved_private_import_memory(i) ||
                x.import_type_ptr != y.import_type_ptr || x.effective_limits.min != y.effective_limits.min ||
                x.effective_limits.max != y.effective_limits.max || x.effective_limits.present_max != y.effective_limits.present_max)
            { return false; }
        }
        for(::std::size_t i{}; i != shared_globals_.size(); ++i)
        {
            auto const& x{a.imported_global_vec_storage.index_unchecked(i)};
            auto const location=locate_source_global(shared_globals_[i]);
            auto const* peer=node_for_parent(location.parent);
            if(!peer || !peer->module_view_ || location.index>=peer->module_view_->local_defined_global_vec_storage.size())return false;
            if(x.link_kind != imported_global_storage_t::imported_global_link_kind::defined ||
                x.target.defined_ptr != &peer->module_view_->local_defined_global_vec_storage.index_unchecked(location.index) ||
                x.import_type_ptr != p.imported_global_vec_storage.index_unchecked(i).import_type_ptr) { return false; }
        }
        for(::std::size_t i{};i!=imported_table_targets_.size();++i)
        {
            auto const location=locate_source_table(imported_table_targets_[i]);
            auto const* peer=node_for_parent(location.parent);
            auto const& target=a.imported_table_vec_storage.index_unchecked(i);
            if(!peer||!peer->module_view_||location.index>=peer->module_view_->local_defined_table_vec_storage.size()||
               target.link_kind!=imported_table_storage_t::imported_table_link_kind::defined||
               target.target.defined_ptr!=&peer->module_view_->local_defined_table_vec_storage.index_unchecked(location.index)||
               target.import_type_ptr!=p.imported_table_vec_storage.index_unchecked(i).import_type_ptr)return false;
        }
        for(::std::size_t i{}; i != a.imported_tag_vec_storage.size(); ++i)
        {
            auto const& x{a.imported_tag_vec_storage.index_unchecked(i)};
            auto const& y{p.imported_tag_vec_storage.index_unchecked(i)};
            auto const location=locate_source_tag(y.resolved_tag);auto const* peer=node_for_parent(location.parent);
            if(!peer || !peer->module_view_ || location.index>=peer->module_view_->local_defined_tag_vec_storage.size())return false;
            auto const* expected=&peer->module_view_->local_defined_tag_vec_storage.index_unchecked(location.index);
            if(x.import_type_ptr != y.import_type_ptr || x.function_type_ptr != y.function_type_ptr ||
                x.resolved_tag != expected || x.imported_target != nullptr || x.defined_target != expected)
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
        inline ::uwvm2::object::memory::linear::native_memory_t
        initialization_context<initialization_purpose::threaded_child>::make_memory() const
        {
            return []<typename Memory>() -> Memory
            {
                if constexpr(requires {typename Memory::fallible_construction_t;})
                {
                    Memory memory{typename Memory::fallible_construction_t{}};
                    if(!memory.memory_length_p||!memory.growing_mutex_p)throw ::std::bad_alloc{};
                    return memory;
                }
                else throw ::std::bad_alloc{};
            }.template operator()<::uwvm2::object::memory::linear::native_memory_t>();
        }
        inline void initialization_context<initialization_purpose::threaded_child>::prepare_memory(
            ::uwvm2::uwvm::runtime::storage::local_defined_memory_storage_t& record,::std::size_t index) const
        {
            auto owner=owner_->lock();if(!owner||index>=owner->parent_->local_defined_memory_vec_storage.size())throw ::std::bad_alloc{};
            auto const& source=owner->parent_->local_defined_memory_vec_storage.index_unchecked(index);
            auto prepare=[&]<typename Memory>(Memory& memory)
            {
                if constexpr(requires{typename Memory::fallible_construction_t;})
                {
                    bool const wide=wasm_memory_is_address64(*record.memory_type_ptr);
                    bool const shared=wasm_memory_is_shared(*record.memory_type_ptr);
                    auto const pages=record.effective_limits.min;
                    constexpr auto metadata=sizeof(::std::atomic_size_t)+sizeof(::uwvm2::utils::mutex::mutex_t);
                    if(pages>(SIZE_MAX-metadata)/65536u||!owner->allocation_.get().charge(static_cast<::std::size_t>(pages)*65536u+metadata))throw ::std::bad_alloc{};
                    memory.status=wide?decltype(memory.status)::wasm64:decltype(memory.status)::wasm32;
                    memory.sequentially_consistent_size=shared;
                    if(!memory.try_init_by_page_count(static_cast<::std::size_t>(pages),record.effective_limits.present_max?
                        static_cast<::std::size_t>((::std::min)(static_cast<::std::uint64_t>(record.effective_limits.max),static_cast<::std::uint64_t>(SIZE_MAX))):SIZE_MAX,
                        owner->parent_->imported_memory_vec_storage.size()+index))throw ::std::bad_alloc{};
                    if(source.memory.memtag_shadow.enabled())
                    {
                        namespace mt=::uwvm2::extension::memtag;
                        auto const tag_bytes=ThreadedChildWorld::memory_tag_storage_bytes(pages,shared);
                        if(tag_bytes==SIZE_MAX||!owner->allocation_.get().charge(tag_bytes))throw ::std::bad_alloc{};
                        if(memory.enable_memtag(wide?mt::address_width::i64:mt::address_width::i32,shared)!=mt::storage_error::none)
                            throw ::std::bad_alloc{};
                        auto const hardware=::uwvm2::runtime::memtag::activate_owner_hardware(memory);
                        if(!hardware&&(hardware.error!=mt::hardware_mte::setup_error::unsupported||memory.memtag_shadow.hardware_selected()))
                            throw ::std::bad_alloc{};
                    }
                }
                else throw ::std::bad_alloc{};
            };prepare(record.memory);
        }
        inline void initialization_context<initialization_purpose::threaded_child>::prepare_table(
            ::uwvm2::uwvm::runtime::storage::local_defined_table_storage_t& table, ::std::size_t index) const noexcept
        {
            auto owner{owner_->lock()};
            if(!owner || index >= owner->prepared_tables_.size()) { ::std::terminate(); }
            // Construction-only native borrow. No published child engine can
            // observe this world before its factory has completed.
            table.elems = ::std::move(const_cast<ThreadedChildWorld&>(*owner).prepared_tables_[index]);
            table.allocation_charge = ::std::move(const_cast<ThreadedChildWorld&>(*owner).prepared_table_charges_[index]);
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
        using compatible = ::fast_io::native_global_allocator;
        if constexpr(!::std::is_same_v<typename Vector::allocator_type, compatible>) { return false; }
        else
        {
            if(!vector.empty() || vector.capacity() != 0uz ||
                count > static_cast<::std::size_t>((::std::numeric_limits<::std::ptrdiff_t>::max)()) / sizeof(value))
            { return false; }
            auto* memory{allocation_.get().template try_vector_storage<value>(count)};
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
            !preallocate_vector(out.imported_table_vec_storage,p.imported_table_vec_storage.size()) ||
            !preallocate_vector(out.imported_global_vec_storage, p.imported_global_vec_storage.size()) ||
            !preallocate_vector(out.imported_tag_vec_storage, p.imported_tag_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_function_vec_storage, p.local_defined_function_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_code_vec_storage, p.local_defined_code_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_memory_vec_storage,p.local_defined_memory_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_global_vec_storage, p.local_defined_global_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_table_vec_storage, p.local_defined_table_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_tag_vec_storage, p.local_defined_tag_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_element_vec_storage, p.local_defined_element_vec_storage.size()) ||
            !preallocate_vector(out.local_defined_data_vec_storage, p.local_defined_data_vec_storage.size()) ||
            !preallocate_vector(out.declared_ref_funcidx_vec_storage, p.declared_ref_funcidx_vec_storage.size()))
        { return false; }
        prepared_tables_.resize(p.local_defined_table_vec_storage.size());
        prepared_table_charges_.resize(prepared_tables_.size());
        for(::std::size_t i{}; i != prepared_tables_.size(); ++i)
        {
            auto const count{p.local_defined_table_vec_storage.index_unchecked(i).table_type_ptr->limits.min};
            ::std::size_t bytes{};
            if(count > SIZE_MAX || !::uwvm2::utils::allocator::resource::checked_bytes(static_cast<::std::size_t>(count),
                sizeof(::uwvm2::uwvm::runtime::storage::local_defined_table_elem_storage_t),bytes) ||
                bytes > ::uwvm2::utils::allocator::resource::initializer_allocation_bytes ||
                !prepared_table_charges_[i].resize(bytes)) { return false; }
            if(!preallocate_vector(prepared_tables_[i],static_cast<::std::size_t>(count)))
            { return false; }
        }
        ::std::size_t expressions{};
        for(auto const& segment : p.local_defined_element_vec_storage)
        {
            auto const count{segment.element_type_ptr->storage.segment.vec_expr.size()};
            if(count > (::std::numeric_limits<::std::size_t>::max)() - expressions) { return false; }
            expressions += count;
        }
        // Every reference family uses separately accounted, preallocated
        // storage before the shared initializer evaluates an element payload.
        return preallocate_vector(out.element_expr_funcref_vec_storage, expressions) &&
            preallocate_vector(out.element_expr_externref_vec_storage, expressions) &&
            preallocate_vector(out.element_expr_gc_ref_vec_storage, expressions);
    }

    inline threaded_child_error ThreadedChildWorld::preflight()
    {
        using namespace ::uwvm2::uwvm::runtime::storage;
        if(!source_type::has_canonical_owner(source_) || admission_->source_generation() == 0u)
        { return threaded_child_error::invalid_source; }
        file_ = source_->actual_validated_file(source_id_,
            admission_->source_validation_epoch(), parent_);
        // The selected executing module and every transitive code provider
        // must belong to this same validated source generation.
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
        // A zero table/segment budget admits a module declaring none. The
        // individual declarations below enforce exact usage; a positive quota
        // is not an implicit prerequisite for unrelated thread authority.
        if(limits_.max_runtime_modules == 0uz) { return threaded_child_error::count_limit; }
        auto const& p{*parent_};
        if(!p.gc_store || !p.gc_store->valid()) { return threaded_child_error::invalid_store; }
        // The actual Core validator and the shared constant evaluator decide
        // validity. Each private instance owns a charged GC store and rebuilds
        // its globals/table expressions; it never copies live parent roots.
        for(auto const& global:p.local_defined_global_vec_storage)
            if(!global.local_global_type_ptr)return threaded_child_error::declaration_mismatch;
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
        if(p.imported_table_vec_storage.size()>limits_.max_imported_tables)return threaded_child_error::count_limit;
        // Resolve only authenticated source records here; graph construction
        // replaces every leaf with its fresh private instance before effects.
        imported_table_targets_.reserve(p.imported_table_vec_storage.size());
        auto const table_links=source_import_count(&module_type::imported_table_vec_storage);
        for(auto const& original:p.imported_table_vec_storage)
        {
            auto const* cursor=&original;
            for(::std::size_t visited{};;++visited)
            {
                if(!cursor||visited>=table_links||
                   !source_contains_import(cursor,&module_type::imported_table_vec_storage))return threaded_child_error::unsupported_import;
                using link=imported_table_storage_t::imported_table_link_kind;
                if(cursor->link_kind==link::imported){cursor=cursor->target.imported_ptr;continue;}
                if(cursor->link_kind!=link::defined||!locate_source_table(cursor->target.defined_ptr).parent)
                    return threaded_child_error::unsupported_import;
                imported_table_targets_.push_back(cursor->target.defined_ptr);break;
            }
        }
        for(auto const& tag:p.imported_tag_vec_storage)
            if(!tag.import_type_ptr || !tag.function_type_ptr || !locate_source_tag(tag.resolved_tag).parent)
                return threaded_child_error::unsupported_import;
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
        auto const memory_links=source_import_count(&module_type::imported_memory_vec_storage);
        for(auto const& original : p.imported_memory_vec_storage)
        {
            if(original.import_type_ptr == nullptr)
            { return threaded_child_error::unsupported_import; }
            auto const* cursor{::std::addressof(original)};
            ::std::size_t visited{};
            for(;;)
            {
                if(cursor == nullptr || !source_contains_import(cursor, &module_type::imported_memory_vec_storage))
                { return threaded_child_error::unsupported_import; }
                if(visited++ >= memory_links)
                { return threaded_child_error::unsupported_import; }
                using kind = imported_memory_storage_t::imported_memory_link_kind;
                if(cursor->link_kind == kind::imported) { cursor = cursor->target.imported_ptr; continue; }
                if(cursor->link_kind != kind::defined || !source_contains_memory(cursor->target.defined_ptr))
                { return threaded_child_error::unsupported_import; }
                auto* leaf{cursor->target.defined_ptr};
                if(!leaf->memory_type_ptr)return threaded_child_error::unsupported_import;
                if(details::wasm_memory_is_shared(*leaf->memory_type_ptr))
                {
                    if(!leaf->effective_limits.present_max||!supports_shared_child_memory(leaf->memory))
                        return threaded_child_error::unsupported_import;
                    auto const maximum=static_cast<::std::uint_least64_t>(leaf->effective_limits.max);
                    if(maximum>admission_->max_shared_memory_bytes()/65536u)return threaded_child_error::memory_limit;
                    if(::std::find(shared_memories_.begin(),shared_memories_.end(),leaf)==shared_memories_.end())shared_memories_.push_back(leaf);
                }
                imported_memory_targets_.push_back(leaf);
                break;
            }
        }

        // Authenticate the source links here. build_graph later rebinds every
        // global to its fresh private provider before evaluating initializers;
        // no mutable parent value or native GC pointer is copied into a child.
        shared_globals_.reserve(p.imported_global_vec_storage.size());
        auto const global_links=source_import_count(&module_type::imported_global_vec_storage);
        for(auto const& original : p.imported_global_vec_storage)
        {
            auto const* cursor{::std::addressof(original)};
            ::std::size_t visited{};
            for(;;)
            {
                if(cursor == nullptr || !source_contains_import(cursor, &module_type::imported_global_vec_storage))
                { return threaded_child_error::unsupported_import; }
                if(visited++ >= global_links)
                { return threaded_child_error::unsupported_import; }
                using kind = imported_global_storage_t::imported_global_link_kind;
                if(cursor->link_kind == kind::imported) { cursor = cursor->target.imported_ptr; continue; }
                if(cursor->link_kind != kind::defined || !source_contains_global(cursor->target.defined_ptr))
                { return threaded_child_error::unsupported_import; }
                auto* leaf{cursor->target.defined_ptr};
                shared_globals_.push_back(leaf);
                break;
            }
        }
        ::std::size_t table_elements{};
        for(auto const& t : p.local_defined_table_vec_storage)
        {
            if(t.table_type_ptr == nullptr) { return threaded_child_error::declaration_mismatch; }
            auto const minimum{t.table_type_ptr->limits.min};
            if(minimum > admission_->max_table_elements() - table_elements)
            { return threaded_child_error::table_limit; }
            table_elements += static_cast<::std::size_t>(minimum);
        }
        ::std::uint_least64_t local_bytes{};
        for(auto const& m : p.local_defined_memory_vec_storage)
        {
            if(m.memory_type_ptr == nullptr) { return threaded_child_error::declaration_mismatch; }
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
            auto const size{data.byte.begin == data.byte.end ? 0uz :
                static_cast<::std::size_t>(data.byte.end - data.byte.begin)};
            if(size > admission_->max_total_segment_bytes() - segment_bytes)
            { return threaded_child_error::segment_limit; }
            segment_bytes += size;
        }
        // Only the thread-entry instance requires shared linear memory. A
        // retained pure code provider can declare no memory at all; granting
        // it a synthetic alias would change its Wasm instance and authority.
        if(shared_memories_.empty() && graph_==this) { return threaded_child_error::missing_shared_memory; }
        // The evaluator may materialize one reference slot per declared element
        // even when the corresponding live parent segment has been dropped.
        for(auto const& segment : p.local_defined_element_vec_storage)
        {
            if(segment.element_type_ptr == nullptr) { return threaded_child_error::declaration_mismatch; }
            auto const& declaration{segment.element_type_ptr->storage.segment};
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
            // Execute the validated start function on the actual native worker
            // under the same journal/import/permission gates as every opcode.
            // Do not impose a libc-specific once-initializer bytecode grammar.
            initialization_=static_cast<::std::size_t>(start.start_idx);
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
            module_->imported_memory_vec_storage.size() != imported_memory_targets_.size() ||
            module_->imported_global_vec_storage.size() != shared_globals_.size() ||
            module_->imported_tag_vec_storage.size() != parent.imported_tag_vec_storage.size() ||
            module_->imported_table_vec_storage.size() != imported_table_targets_.size())
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
        for(::std::size_t i{}; i != imported_memory_targets_.size(); ++i)
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
        for(::std::size_t i{};i!=imported_table_targets_.size();++i)
        {
            auto& target=module_->imported_table_vec_storage.index_unchecked(i);
            target.target.defined_ptr=imported_table_targets_[i];
            target.link_kind=imported_table_storage_t::imported_table_link_kind::defined;
            target.import_type_ptr=parent.imported_table_vec_storage.index_unchecked(i).import_type_ptr;
            target.is_opposite_side_imported=false;
        }
        for(::std::size_t i{}; i != module_->imported_tag_vec_storage.size(); ++i)
        { module_->imported_tag_vec_storage.index_unchecked(i) = parent.imported_tag_vec_storage.index_unchecked(i); }
        current_ = {};
        return threaded_child_error::ok;
    }

    inline threaded_child_error ThreadedChildWorld::finish_initialization()
    {
        // Every private module and import link is stable before any expression
        // can read a peer. The common evaluator follows actual owner records.
        current_=file_->module_name;
        auto world=context();
        details::finalize_wasm1_globals_after_linking_in_context(world);
        details::initialize_wasm3_table_expressions_after_linking_in_context(world);
        details::finalize_wasm1_offsets_after_linking_in_context(world);
        if(!module_->gc_store || !module_->gc_store->valid()) { return threaded_child_error::invalid_store; }
        // Prepare child-local state now, retaining imported active data until
        // this instance's real native execution turn. Every new instance must
        // apply those segments, including their implicit data.drop.
        if(details::apply_wasm1_active_element_and_data_segments_for_module_in_context(world, file_->module_name, *module_, true) !=
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
#include "threaded_child_table.inc"
#include "threaded_child_exception.inc"
#ifdef UWVM_MODULE
    }
#endif
}
#endif
