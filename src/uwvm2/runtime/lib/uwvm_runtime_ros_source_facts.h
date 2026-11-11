// SPDX-License-Identifier: Apache-2.0
// Included inside uwvm2::runtime::lib after the ordinary caller APIs.
// Internal data query only: no permission registration or native effect.
    extern "C++" bool ros_caller_source_facts_host_api(ros_caller_source_facts& out, bool allow_module_exports) noexcept
    {
        out={};
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
# if UWVM2_HAS_THREADED_PROFILE_HOST_API
        if(threaded_profile::has_active_host_call()) { return threaded_profile::caller_source_facts(out,allow_module_exports); }
# endif
        ros_caller_identity caller{};
        if(!ros_caller_identity_host_api(caller, allow_module_exports)) { return false; }
        auto const& context{get_preload_call_context()};
        if(context.ros_provider==nullptr || context.module_id>=g_runtime.modules.size()) { return false; }
        auto const& entry{g_runtime.modules.index_unchecked(context.module_id)};
        auto const* module{entry.runtime_module};
        if(module==nullptr || module!=caller.runtime_instance) { return false; }
        namespace full=::uwvm2::uwvm::runtime::full;
        // This owner comes directly from the actual runtime registry. No caller
        // pointer, serialized digest or boolean can select the source object.
        auto const source{full::selected_full_source_owner_pin()};
        if(!full::full_source_instance::has_canonical_owner(source)) { return false; }
        auto const* file{source->actual_validated_file(context.module_id,caller.generation,module)};
        if(file==nullptr || file->binfmt_ver!=1u || !file->has_owned_source_image()) { return false; }

        full::builtin_ros_function_data provider{};
        bool actual{};
        auto const& imports{module->imported_function_vec_storage};
        for(::std::size_t i{};i!=imports.size();++i)
        {
            auto const& linked{imports.index_unchecked(i)};
            if(linked.link_kind!=::uwvm2::uwvm::runtime::storage::imported_function_link_kind::local_imported ||
               linked.target.local_imported.module_ptr!=context.ros_provider ||
               linked.target.local_imported.index!=context.ros_function_index) { continue; }
            // Independently checks the original declaration, exact signature,
            // initializer-sealed concrete provider and current source epoch.
            if(source->actual_builtin_ros_function(context.module_id,caller.generation,module,i,provider) &&
               provider.function_index==context.ros_function_index) { actual=true;break; }
        }
        if(!actual) { return false; }

        // Do not confuse the policy-visible descriptor count with the actual
        // module's memory count. This first profile deliberately excludes
        // imported storage, even if a policy exposes it as memory zero.
        if(!module->imported_memory_vec_storage.empty() || module->local_defined_memory_vec_storage.size()!=1u)
        { return false; }
        auto const& memory{module->local_defined_memory_vec_storage.index_unchecked(0u)};
        auto own_unshared=[&]<::uwvm2::parser::wasm::concepts::wasm_feature... Fs>(
            ::uwvm2::uwvm::wasm::type::feature_list<Fs...>) noexcept
        {
            auto const& parsed{file->wasm_module_storage.wasm_binfmt_ver1_storage};
            auto const& definitions{::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<
                ::uwvm2::parser::wasm::standard::wasm1::features::memory_section_storage_t<Fs...>>(parsed.sections)};
            auto const& declarations{::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<
                ::uwvm2::parser::wasm::standard::wasm1::features::import_section_storage_t<Fs...>>(parsed.sections)};
            if(!declarations.importdesc.index_unchecked(2u).empty() || definitions.memories.size()!=1u) { return false; }
            auto const& original{definitions.memories.index_unchecked(0u)};
            if(memory.memory_type_ptr!=::std::addressof(original)) { return false; }
            if constexpr(requires { original.shared; }) { if(original.shared) { return false; } }
            return true;
        };
        if(!own_unshared(::uwvm2::uwvm::wasm::type::binfmt_ver1_feature_list_t{})) { return false; }
        resolved_preload_memory_t snapshot{};
        if(!resolve_defined_preload_memory(memory.memory,snapshot)) { return false; }

        auto const length{file->source_size()};
        if(length<8u || length>::uwvm2::utils::control::owned_file_image::maximum_bytes ||
           length>static_cast<::std::size_t>((::std::numeric_limits<::std::ptrdiff_t>::max)())) { return false; }
        auto const* bytes{reinterpret_cast<::std::byte const*>(file->source_cbegin())};
        if(bytes==nullptr) { return false; }
        ros_caller_source_facts result{};
        ::fast_io::sha256_context hash{};
        hash.update(bytes,bytes+length);hash.do_final();hash.digest_to_byte_ptr(result.whole_wasm_sha256.data());
        result.caller=caller;result.whole_wasm_bytes=length;result.memory_bytes=snapshot.byte_length;
        result.builtin_interface_sha256=provider.interface_sha256;
        result.builtin_function_index=provider.function_index;result.builtin_kind=static_cast<unsigned>(provider.kind);
        out=result;return true;
#else
        return false;
#endif
    }
