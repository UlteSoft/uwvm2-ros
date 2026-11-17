// Included inside uwvm2::runtime::lib after checked caller and memory helpers.
// This boundary retains identities and integer offsets, never guest pointers.
    #include "uwvm_runtime_ros_exchange_erase.h"
    namespace exchange_memory_details
    {
        inline native_memory_t const* current(ros_caller_identity const& who,::std::size_t memory)noexcept
        {
#if defined(UWVM_IMPORT_ROSV2)
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
            if(threaded_profile::has_active_host_call())return nullptr;
#endif
            auto const& context=get_preload_call_context();
            if(context.module_id>=g_runtime.modules.size())return nullptr;
            auto const* module=g_runtime.modules.index_unchecked(context.module_id).runtime_module;
            if(!module||module!=who.runtime_instance||memory<module->imported_memory_vec_storage.size())return nullptr;
            auto local=memory-module->imported_memory_vec_storage.size();
            if(local>=module->local_defined_memory_vec_storage.size())return nullptr;
            auto const& entry=module->local_defined_memory_vec_storage.index_unchecked(local);
            auto shared=[]<typename Declaration>(Declaration const* declaration)noexcept
            {if constexpr(requires{declaration->shared;})return declaration&&declaration->shared;else return false;};
            if(!shared(entry.memory_type_ptr))return nullptr;
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
            // The debugger's value observer shares the compilation-profile
            // holder with replay, but creates no resumable checkpoint or
            // persistent message copy. Permit that live, read-only observer.
            // Every other profile stays closed: serialized/replayed memory
            // could otherwise resurrect an already erased exchange region.
            if(g_runtime.checkpoint_profile && g_runtime.checkpoint_profile->purpose()!=
               ::uwvm2::runtime::checkpoint::compilation_purpose::observe_values)return nullptr;
#endif
            // This proof is reused only inside one closed host invocation.
            // The execution lease excludes declaration replacement/destruction;
            // the backend still pins every copy and validates its current tags.
            namespace full=::uwvm2::uwvm::runtime::full;
            auto source=full::selected_full_source_owner_pin();if(!full::full_source_instance::has_canonical_owner(source))return nullptr;
            auto file=source->actual_validated_file(context.module_id,who.generation,module);if(!file||file->binfmt_ver!=1)return nullptr;
            auto private_memory=[&]<::uwvm2::parser::wasm::concepts::wasm_feature... Fs>(::uwvm2::uwvm::wasm::type::feature_list<Fs...>)noexcept
            {
                auto const& exports=::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<
                    ::uwvm2::parser::wasm::standard::wasm1::features::export_section_storage_t<Fs...>>(file->wasm_module_storage.wasm_binfmt_ver1_storage.sections);
                return exports.exportdesc.index_unchecked(2u).empty();
            };
            if(!private_memory(::uwvm2::uwvm::wasm::type::binfmt_ver1_feature_list_t{}))return nullptr;
            return &entry.memory;
#else
            (void)who;(void)memory;return nullptr;
#endif
        }
    }
    extern "C++" ::std::uint32_t with_ros_exchange_memory_host_api(ros_exchange_memory_callback callback,void* context)noexcept
    {
        ros_exchange_memory_access access;
        if(!callback||!ros_caller_identity_host_api(access.caller_,true))return 13;
        return callback(context,access);
    }
    bool ros_exchange_memory_access::capture(::std::size_t memory,::std::uint64_t offset,::std::size_t size,::std::uint64_t& normalized)noexcept
    {
        normalized=0;
        if(!memory_owner_)
        {
            auto owner=exchange_memory_details::current(caller_,memory);if(!owner)return false;
            memory_owner_=owner;memory_index_=memory;
        }
        if(memory!=memory_index_)return false;
        auto owner=static_cast<native_memory_t const*>(memory_owner_);
        auto checked=::uwvm2::runtime::memtag::check_host_access(*owner,offset,size);
        ::uwvm2::runtime::memtag::record_host_fault(memory,offset,checked);
        if(!checked)return false;
        normalized=checked.address;return true;
    }
    bool ros_exchange_memory_access::copy(::std::size_t memory,::std::uint64_t offset,void* data,::std::size_t size,bool write)noexcept
    {
        if(!memory_owner_||memory!=memory_index_)return false;
        auto owner=static_cast<native_memory_t const*>(memory_owner_);
        return static_cast<bool>(write?::uwvm2::runtime::memtag::copy_host_access<true>(*owner,offset,data,size,true):
            ::uwvm2::runtime::memtag::copy_host_access<false>(*owner,offset,data,size,true));
    }
    bool ros_exchange_memory_access::erase(::std::uint64_t offset,::std::uint64_t size)noexcept
    {
        return memory_owner_&&exchange_memory_details::erase(*static_cast<native_memory_t const*>(memory_owner_),offset,size);
    }
    bool ros_exchange_memory_access::read_input(::std::size_t memory,::std::uint64_t offset,void* data,::std::size_t size,bool clear_after_copy)noexcept
    {
        if(!memory_owner_||memory!=memory_index_)return false;
        return exchange_memory_details::read_input(*static_cast<native_memory_t const*>(memory_owner_),offset,data,size,clear_after_copy);
    }
    bool ros_exchange_memory_access::write_frame(::std::size_t memory,::std::uint64_t offset,
        void const* prefix,::std::size_t prefix_size,void const* payload,::std::size_t payload_size)noexcept
    {
        if(!memory_owner_||memory!=memory_index_)return false;
        return exchange_memory_details::write_frame(*static_cast<native_memory_t const*>(memory_owner_),
            offset,prefix,prefix_size,payload,payload_size);
    }
    /** @brief Erase a sealed logical region, including after permission revocation.
     * @details Publication excludes owner destruction while the memory backend
     * pins growth once for both payload and result-length extents. Each extent
     * is checked independently and attempted even if the other cannot be erased;
     * either failure keeps the route quarantined. Byte stores use atomic_ref because this is declared shared
     * storage. The full registered extent is erased, independent of changed
     * pointer tags. Guest input cannot construct this internal cleanup call.
     */
    extern "C++" bool ros_exchange_memory_erase_host_api(void const* instance,::std::uint64_t generation,::std::uint64_t memory,
        ::std::uint64_t offset,::std::uint64_t size,::std::uint64_t length_offset,::std::uint64_t length_size)noexcept
    {
#if defined(UWVM_IMPORT_ROSV2)
        runtime_state_publication_guard publication;
        if(!instance||!generation||generation!=current_runtime_generation())return false;
        for(auto const& entry:g_runtime.modules)
        {
            auto module=entry.runtime_module;if(module!=instance)continue;
            if(memory<module->imported_memory_vec_storage.size())return false;
            auto local=memory-module->imported_memory_vec_storage.size();if(local>=module->local_defined_memory_vec_storage.size())return false;
            auto const& storage=module->local_defined_memory_vec_storage.index_unchecked(static_cast<::std::size_t>(local));
            return exchange_memory_details::erase(storage.memory,offset,size,length_offset,length_size);
        }
        return false;
#else
        (void)instance;(void)generation;(void)memory;(void)offset;(void)size;(void)length_offset;(void)length_size;return false;
#endif
    }
