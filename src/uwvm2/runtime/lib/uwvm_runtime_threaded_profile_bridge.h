// Include INSIDE uwvm2::runtime::lib after the ordinary Host helpers and the
// actual ThreadedLlvmChild. Required standalone headers are included at TU scope.
#if defined(__linux__) && defined(UWVM_USE_THREAD_LOCAL) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE) && (defined(UWVM_RUNTIME_UWVM_INTERPRETER) || defined(UWVM_RUNTIME_LLVM_JIT))
extern "C++"
{
    class runtime_threaded_profile_bridge final
    {
        using group = ::uwvm2::runtime::wasm_threads::ThreadedGroup;
        using lease = group::ChildLease;
        using admission = ::uwvm2::runtime::wasm_threads::ThreadedSpawnAdmission;
        using world = ::uwvm2::uwvm::runtime::initializer::ThreadedChildWorld;
        using source = ::uwvm2::uwvm::runtime::full::full_source_instance;
        using import_status = ::uwvm2::runtime::wasm_threads::import_admission;
        using dispatch_status = ::uwvm2::runtime::wasm_threads::host_dispatch_status;
        using builtin_kind = ::uwvm2::uwvm::runtime::full::threaded_builtin_kind;
        using builtin_loader = ::uwvm2::uwvm::runtime::full::threaded_builtin_loader;
        using value_type = ::uwvm2::uwvm::runtime::storage::wasm_binfmt1_final_value_type_t;
        using signature = ::uwvm2::uwvm::runtime::storage::wasm_binfmt1_final_function_type_t;

        struct host_call
        {
            lease* actual{};
            world const* instance{};
            ::std::size_t caller_function{};
            bool ros{};
            ::std::size_t module{},imported{}; bool cross_module{};
        };
        inline static thread_local host_call* current_host_{};
        struct host_scope
        {
            host_call* previous{};
            explicit host_scope(host_call& value) noexcept : previous{current_host_} { current_host_ = &value; }
            ~host_scope() { current_host_ = previous; }
            host_scope(host_scope const&) = delete;
        };

        // Actual production import census. No counter/lock on policy none.
        // Destructor runs after every provider/cancellation/preload Host scope.
        struct host_census final
        {
            lease& actual;bool spawn,entered;
            host_census(lease& value,bool creates) noexcept:actual{value},spawn{creates},
                entered{actual.group_->debug_host_enter(*actual.slot_,spawn)}{}
            ~host_census(){if(entered)actual.group_->debug_host_leave(*actual.slot_,spawn);}
            host_census(host_census const&)=delete;
        };
        [[nodiscard]] static bool current_source(admission const& admitted) noexcept
        {
            auto const& original{admitted.source_owner()};
            if(!admitted.owns_actual_generation() || !source::has_canonical_owner(original) ||
               admitted.retain().get() != &admitted || admitted.source_generation() != current_runtime_generation()) { return false; }
            auto const id{admitted.source_module_id()};
            if(id >= g_runtime.modules.size() ||
               g_runtime.modules.index_unchecked(id).runtime_module != &admitted.source_module()) { return false; }
            auto const selected{::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin()};
            if(selected.get() != original.get() || selected.owner_before(original) || original.owner_before(selected)) { return false; }
            return original->actual_validated_file(id, admitted.source_validation_epoch(), &admitted.source_module()) ==
                ::std::addressof(original->file());
        }
        [[nodiscard]] static bool actual_lease(lease const& actual, world const& instance) noexcept
        {
            if(!actual.owns_current_native_thread() || &actual.world() != &instance ||
               &actual.admission() != &instance.admission() || !current_source(actual.admission())) { return false; }
            auto const validated{instance.validated_code()};
            return validated && validated->covers(instance);
        }
        [[nodiscard]] static host_call* checked_host() noexcept
        {
            auto const value{current_host_};
            return value && value->actual && value->instance && actual_lease(*value->actual, *value->instance) ? value : nullptr;
        }
        [[nodiscard]] static bool numeric_abi(signature const& expected, signature const& provided,
            ::std::size_t& params, ::std::size_t& results) noexcept
        {
            auto match{[] (auto const& a, auto const& b, ::std::size_t& bytes) noexcept
            {
                // Both ranges are canonical validated declarations or the
                // concrete builtin's immutable signature cache, never Guest ABI.
                auto i{a.begin}; auto j{b.begin}; bytes = 0u;
                while(i != a.end && j != b.end)
                {
                    if(*i != *j) { return false; }
                    ::std::size_t width{};
                    switch(*i)
                    {
                        case value_type::i32: case value_type::f32: width = 4u; break;
                        case value_type::i64: case value_type::f64: width = 8u; break;
                        default: return false;
                    }
                    if(bytes > SIZE_MAX - width) { return false; }
                    bytes += width; ++i; ++j;
                }
                return i == a.end && j == b.end;
            }};
            return match(expected.parameter, provided.parameter, params) && match(expected.result, provided.result, results);
        }
        struct imported_leaf
        {
            local_imported_t const* provider{};
            ::std::size_t index{}, parameter_bytes{}, result_bytes{};
            bool ros{}, spawn{}, exits{}, wasi{};
        };
        [[nodiscard]] static bool actual_source_providers(admission const& admitted) noexcept
        {
            if(!current_source(admitted)) { return false; }
            for(auto const& [name,module]:admitted.source_owner()->registry())
            {
            static_cast<void>(name);
            auto const id=admitted.source_owner()->bound_initialized_module_id(&module);
            if(!admitted.source_owner()->actual_validated_file(id,admitted.source_validation_epoch(),&module))return false;
            auto const& imports{module.imported_function_vec_storage};
            using kind = ::uwvm2::uwvm::runtime::storage::imported_function_link_kind;
            for(::std::size_t index{}; index != imports.size(); ++index)
            {
                auto const& original{imports.index_unchecked(index)};
                if(original.link_kind==kind::defined || original.link_kind==kind::imported)continue;
                if(original.link_kind != kind::local_imported || original.import_type_ptr == nullptr ||
                   original.target.local_imported.module_ptr == nullptr) { return false; }
                builtin_kind concrete{};
                if(builtin_loader::actual_member(original.target.local_imported.module_ptr, concrete)) { continue; }
                ::uwvm2::uwvm::runtime::full::builtin_wasip1_function_data wasi{};
                if(!admitted.source_owner()->actual_builtin_wasip1_function(id,
                    admitted.source_validation_epoch(), &module, index, wasi)) { return false; }
            }
            }
            return true;
        }
        [[nodiscard]] static bool resolve_leaf(world const& instance, ::std::size_t module_index,
            ::std::size_t import_index, imported_leaf& out) noexcept
        {
            if(module_index >= instance.module_count() || !current_source(instance.admission())) { return false; }
            auto const validated{instance.validated_code()};
            auto const* module{instance.module(module_index)};
            if(!validated || !validated->covers(instance) || !module || import_index >= module->imported_function_vec_storage.size()) { return false; }
            auto const* declared_import=module->imported_function_vec_storage.index_unchecked(import_index).import_type_ptr;
            ::std::size_t provider_module{},provider_import{};
            if(!instance.resolve_host_import(module_index,import_index,provider_module,provider_import))return false;
            auto const& native{instance.module(provider_module)->imported_function_vec_storage.index_unchecked(provider_import)};
            auto const* original_module=instance.original_module(provider_module); if(!original_module)return false;
            auto const& parent{*original_module};
            if(provider_import >= parent.imported_function_vec_storage.size()) { return false; }
            auto const& original{parent.imported_function_vec_storage.index_unchecked(provider_import)};
            using kind = ::uwvm2::uwvm::runtime::storage::imported_function_link_kind;
            if(native.link_kind != kind::local_imported || original.link_kind != kind::local_imported ||
               native.import_type_ptr == nullptr || native.import_type_ptr != original.import_type_ptr ||
               native.target.local_imported.module_ptr != original.target.local_imported.module_ptr ||
               native.target.local_imported.index != original.target.local_imported.index) { return false; }
            auto const* provider{native.target.local_imported.module_ptr};
            builtin_kind concrete{};
            bool const recorded{builtin_loader::actual_member(provider, concrete)};
            ::uwvm2::uwvm::runtime::full::builtin_wasip1_function_data wasi{};
            bool const actual_wasi{instance.source_owner()->actual_builtin_wasip1_function(
                instance.source_module_id(provider_module), instance.admission().source_validation_epoch(), &parent, provider_import, wasi)};
            if(!recorded && !actual_wasi) { return false; }
            auto const information{provider->get_function_information_from_index(native.target.local_imported.index)};
            auto const* declared{declared_import ? declared_import->imports.storage.function : nullptr};
            ::std::size_t params{}, results{};
            if(!information.successed || declared == nullptr ||
               !numeric_abi(*declared, information.function_type, params, results) ||
               params > instance.admission().limits().max_value_stack_bytes ||
               results > instance.admission().limits().max_value_stack_bytes) { return false; }
            bool const spawn{recorded && concrete == builtin_kind::threads};
            if(spawn && (information.function_name != u8"thread-spawn" || params != 4u || results != 4u ||
                         declared->parameter.begin[0] != value_type::i32 || declared->result.begin[0] != value_type::i32)) { return false; }
            // Proc_exit is intercepted, never delegated to the ordinary process
            // exit implementation. Other WASI imports require the real cancellable
            // shared-memory adapter, not a broad module-name default allow.
            if(actual_wasi && !wasi.exits_guest)
            {
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
                if(runtime_threaded_wasip1_adapter::admit(instance, module_index, import_index, *provider) != import_status::allowed) { return false; }
#else
                return false;
#endif
            }
            if(actual_wasi && wasi.exits_guest && (params != 4u || results != 0u)) { return false; }
            out = {provider, native.target.local_imported.index, params, results,
                   recorded && concrete == builtin_kind::ros, spawn, actual_wasi && wasi.exits_guest, actual_wasi};
            return true;
        }

        // Defined by the concrete lifecycle/work adapter below; no callback,
        // empty provider or ordinary parent entry can implement this operation.
        [[nodiscard]] static ::std::int32_t spawn_actual(lease&, ::std::uint32_t) noexcept;

    public:
        [[nodiscard]] static import_status admit_import_checked(world const& instance,
            ::std::size_t module, ::std::size_t imported) noexcept
        {
            imported_leaf leaf{};
            if(resolve_leaf(instance,module,imported,leaf))return import_status::allowed;
            ::std::size_t target_module{},target_function{};
            if(!instance.resolve_wasm_import(module,imported,target_module,target_function))return import_status::denied;
            auto const* from=instance.module(module);auto const* to=instance.module(target_module);
            auto const* expected=from->imported_function_vec_storage.index_unchecked(imported).import_type_ptr;
            auto const* actual=to->local_defined_function_vec_storage.index_unchecked(target_function-to->imported_function_vec_storage.size()).function_type_ptr;
            ::std::size_t params{},results{};
            return expected && expected->imports.storage.function && actual &&
                numeric_abi(*expected->imports.storage.function,*actual,params,results) ? import_status::allowed : import_status::denied;
        }

        [[nodiscard]] static dispatch_status dispatch_import_checked(lease& actual, world const& instance,
            ::std::size_t module, ::std::size_t caller_function, ::std::size_t imported,
            ::std::span<::std::byte> results, ::std::span<::std::byte const> params, bool cross_module) noexcept
        {
            if(!actual_lease(actual, instance) || current_host_ != nullptr) { return dispatch_status::rejected; }
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            if(::uwvm2::utils::thread::process_exit::exit_claimed.load(::std::memory_order_acquire))
            { ::uwvm2::utils::thread::process_exit::request_process_exit(0u); }
#endif
            if(actual.cancellation().stop_requested()) { return dispatch_status::group_stopped; }
            auto const* caller{instance.module(module)};
            if(!caller || caller_function < caller->imported_function_vec_storage.size() ||
               caller_function - caller->imported_function_vec_storage.size() >= caller->local_defined_function_vec_storage.size())
            { return dispatch_status::rejected; }
            imported_leaf leaf{};
            if(!resolve_leaf(instance, module, imported, leaf) || results.size() != leaf.result_bytes || params.size() != leaf.parameter_bytes)
            { return dispatch_status::rejected; }
            // Actual engine entry established the native Wasm FP environment.
            // Its marker belongs to the private engine, not ordinary g_runtime
            // call-stack TLS. Save/restore actual controls even for cold spawn
            // compilation; never use the ordinary false TLS marker here.
            details::scoped_wasm_host_fp_control_restore native_fp{};
            if(!native_fp.ready())
            { actual.request_trap(1u, module, caller_function, 0u); return dispatch_status::rejected; }
            dispatch_status completed{};
            {
                host_census census{actual,leaf.spawn};
                if(!census.entered)
                {
                    if(!actual.cancellation().stop_requested())actual.request_trap(1u,module,caller_function,0u);
                    return actual.cancellation().stop_requested()?dispatch_status::group_stopped:dispatch_status::rejected;
                }
                completed=[&]() noexcept -> dispatch_status
                {
                    if(leaf.exits)
                    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS) && defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
                        auto& env{resolve_wasip1_env_for_runtime_module_id(instance.source_module_id(module))};
                        auto exit_name=leaf.provider->get_function_information_from_index(leaf.index).function_name;
                        if(!env.allows_interface(exit_name) || !env.permits(::uwvm2::imported::wasi::wasip1::environment::permission_t::proc_exit))
                        {
                            ::uwvm2::imported::wasi::wasip1::environment::diagnostic_denied(env,
                                ::uwvm2::utils::control::host_interfaces::find(::uwvm2::utils::control::host_interfaces::family::wasip1,exit_name),
                                !env.allows_interface(exit_name));
                            struct denied_exit { lease* actual;::std::size_t module,function; } fault{&actual,module,caller_function};
                            // A known denied Guest exit is also a Guest fault.
                            // If an authorized exit already won, do not let this
                            // later denial cancel another operation's exit-wait.
                            if(!::uwvm2::utils::thread::process_exit::run_before_exit_request([](void* pointer) noexcept
                            {
                                auto const& current=*static_cast<denied_exit*>(pointer);
                                current.actual->request_trap(0x57415303u,current.module,current.function,0u);
                            },::std::addressof(fault)))
                            {::uwvm2::utils::thread::process_exit::request_process_exit(0u);}
                            return dispatch_status::rejected;
                        }
#endif
                        ::std::uint32_t code{}; ::std::memcpy(&code, params.data(), sizeof(code));
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS) && defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
                        ::uwvm2::utils::thread::process_exit::request_process_exit(code);
#else
                        actual.request_proc_exit(code); return dispatch_status::group_stopped;
#endif
                    }
                    if(leaf.spawn)
                    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS) && defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
                        auto& env{resolve_wasip1_env_for_runtime_module_id(instance.source_module_id(module))};
                        if(!env.allows_interface(u8"thread-spawn") || !env.permits(::uwvm2::imported::wasi::wasip1::environment::permission_t::thread_spawn))
                        {
                            ::uwvm2::imported::wasi::wasip1::environment::diagnostic_denied(env,
                                ::uwvm2::utils::control::host_interfaces::find(::uwvm2::utils::control::host_interfaces::family::wasip1,::fast_io::u8string_view{u8"thread-spawn"}),
                                !env.allows_interface(u8"thread-spawn"));
                            ::std::int32_t const denied{-1};
                            ::std::memcpy(results.data(), &denied, sizeof(denied));
                            return dispatch_status::returned;
                        }
#elif defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                        ::std::int32_t const denied{-1};
                        ::std::memcpy(results.data(), &denied, sizeof(denied));
                        return dispatch_status::returned;
#endif
                        ::std::uint32_t argument{}; ::std::memcpy(&argument, params.data(), sizeof(argument));
                        auto const tid{spawn_actual(actual, argument)};
                        ::std::memcpy(results.data(), &tid, sizeof(tid));
                        return actual.cancellation().stop_requested() ? dispatch_status::group_stopped : dispatch_status::returned;
                    }
                    if(leaf.wasi)
                    {
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
                        host_call call{&actual, &instance, caller_function, false,module,imported,cross_module}; host_scope selected{call};
                        auto const status{runtime_threaded_wasip1_adapter::dispatch(actual, instance, module, caller_function,
                            imported, *leaf.provider, results, params)};
                        return actual.cancellation().stop_requested() ? dispatch_status::group_stopped : status;
#else
                        return dispatch_status::rejected;
#endif
                    }
                    if(!leaf.ros) { return dispatch_status::rejected; }
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
                    // This lexical private constructor is the agreed Native-only call
                    // boundary. A conflicting nested fd fails closed; no ordinary RPC
                    // fallback, Guest setter, detached callback or fd consumption exists.
                    ::uwvm2::imported::ros::ipc::cancellation_scope cancellation{actual.cancellation_fd()};
                    if(!cancellation.valid())
                    { actual.request_trap(1u, module, caller_function, 0u); return dispatch_status::rejected; }
                    host_call call{&actual, &instance, caller_function, true,module,imported,cross_module}; host_scope selected{call};
                    preload_module_memory_attribute_t memory_attribute{};
                    memory_attribute.memory_access_mode = ::uwvm2::uwvm::wasm::type::preload_module_memory_access_mode_t::copy;
                    preload_call_context_guard context{&memory_attribute, nullptr, instance.source_module_id(module), true};
                    if(actual.cancellation().stop_requested()) { return dispatch_status::group_stopped; }
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                    ::uwvm2::imported::ros::synchronous_wait_context current_wait{
                        [](void* pointer) noexcept { return static_cast<lease*>(pointer)->cancellation().stop_requested(); }, &actual};
#endif
                    leaf.provider->call_func_index(leaf.index, results.data(), params.data());
                    return actual.cancellation().stop_requested() ? dispatch_status::group_stopped : dispatch_status::returned;
#else
                    return dispatch_status::rejected;
#endif
                }();
            } // Provider/preload/Host TLS/cancellation scopes, THEN census ended.
            // No provider/backend/Host/shard/allocator lock is retained here.
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            // Join the same process exit only AFTER this actual Host operation
            // retires. Never return ECANCELED into Guest or stop the whole Group
            // while another operation owns an explicitly selected exit-wait.
            if(::uwvm2::utils::thread::process_exit::exit_claimed.load(::std::memory_order_acquire))
            { ::uwvm2::utils::thread::process_exit::request_process_exit(0u); }
#endif
            if(actual.group_->debug_control_&&!actual.group_->debug_poll(*actual.slot_,
                ::uwvm2::runtime::wasm_threads::debug_stop_site::host_boundary))
            {
                if(!actual.cancellation().stop_requested())actual.request_trap(1u,module,caller_function,0u);
                return dispatch_status::group_stopped;
            }
            return actual.cancellation().stop_requested()?dispatch_status::group_stopped:completed;
        }

        [[nodiscard]] static bool has_active_host_call() noexcept { return current_host_ != nullptr; }
        [[nodiscard]] static bool active_host_stop_requested() noexcept
        { auto const call{checked_host()}; return !call || call->actual->cancellation().stop_requested(); }
        [[nodiscard]] static runtime_module_storage_t const* active_host_module() noexcept
        { auto const value{checked_host()}; return value ? value->instance->module(0u) : nullptr; }
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
        [[nodiscard]] static bool observed_caller(ros_caller_identity& out) noexcept
        {
            out = {}; auto const call{checked_host()};
            if(!call || !call->ros) { return false; }
            auto const& admitted{call->actual->admission()}; auto const id{call->instance->source_module_id(call->module)};
            if(id>=g_runtime.modules.size())return false;
            auto const& record{g_runtime.modules.index_unchecked(id)};
            if(record.runtime_module!=call->instance->original_module(call->module))return false;
            out = {record.runtime_module, admitted.source_generation(), record.module_name, call->cross_module};
            return true;
        }
#endif
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
        [[nodiscard]] static bool caller_source_facts(ros_caller_source_facts& out,bool allow_exports) noexcept
        {
            out={};auto const* call=checked_host();
            if(!call||!call->ros||call->module>=call->instance->module_count())return false;
            ros_caller_identity caller{};
            if(!observed_caller(caller)||(!allow_exports&&caller.cross_module_ancestry))return false;
            auto const& admitted=call->actual->admission();auto const& owner=admitted.source_owner();
            auto const id=call->instance->source_module_id(call->module);auto const epoch=admitted.source_validation_epoch();
            auto const* original=call->instance->original_module(call->module);
            auto const* file=owner->actual_validated_file(id,epoch,original);
            ::uwvm2::uwvm::runtime::full::builtin_ros_function_data provider{};
            ::std::size_t provider_module{},provider_import{};
            if(!file||!file->has_owned_source_image()||file->binfmt_ver!=1u||
                !call->instance->resolve_host_import(call->module,call->imported,provider_module,provider_import)||
                !owner->actual_builtin_ros_function(call->instance->source_module_id(provider_module),epoch,
                    call->instance->original_module(provider_module),provider_import,provider))return false;
            imported_leaf leaf{};
            if(!resolve_leaf(*call->instance,call->module,call->imported,leaf)||!leaf.ros||leaf.index!=provider.function_index)return false;
            preload_memory_descriptor_t memory{};
            if(!memory_descriptor(0u,&memory))return false;
            auto const length=file->source_size();
            if(length<8u||length>::uwvm2::utils::control::owned_file_image::maximum_bytes||length>PTRDIFF_MAX)return false;
            auto const* bytes=reinterpret_cast<::std::byte const*>(file->source_cbegin());if(!bytes)return false;
            ros_caller_source_facts result{};::fast_io::sha256_context hash{};
            hash.update(bytes,bytes+length);hash.do_final();hash.digest_to_byte_ptr(result.whole_wasm_sha256.data());
            result.caller=caller;result.whole_wasm_bytes=length;result.memory_bytes=memory.byte_length;
            result.builtin_interface_sha256=provider.interface_sha256;result.builtin_function_index=provider.function_index;
            result.builtin_kind=static_cast<unsigned>(provider.kind);result.shared_memory=true;
            out=result;return true;
        }
#endif
        [[nodiscard]] static ::std::size_t memory_descriptor_count() noexcept
        { auto const call{checked_host()}; return call && call->instance->shared_memory(0u) ? 1u : 0u; }
        [[nodiscard]] static bool memory_descriptor(::std::size_t ordinal, preload_memory_descriptor_t* out) noexcept
        {
            auto const call{checked_host()};
            if(!call || ordinal != 0u || out == nullptr) { return false; }
            auto const* shared{call->instance->shared_memory(0u)};
            if(!shared) { return false; }
            auto const& memory{shared->memory};
            if(memory.memory_length_p == nullptr || memory.custom_page_size_log2 != 16u) { return false; }
            auto const length{memory.memory_length_p->load(::std::memory_order_seq_cst)};
            if(length > call->actual->admission().max_shared_memory_bytes()) { return false; }
            *out = preload_memory_descriptor_t{
                .memory_index = 0u,
                .delivery_state = ::uwvm2::uwvm::wasm::type::uwvm_preload_memory_delivery_copy,
                .backend_kind = ::uwvm2::uwvm::wasm::type::uwvm_preload_memory_backend_native_defined,
                .reserved0 = 0u, .reserved1 = 0u, .page_count = length >> 16u,
                .page_size_bytes = 65536u, .byte_length = length,
                .partial_protection_limit_bytes = 0u, .mmap_view_begin = nullptr, .dynamic_length_atomic_object = nullptr};
            return true;
        }
        [[nodiscard]] static bool memory_normalize_range(::std::size_t index,::std::uint_least64_t offset,
            ::std::size_t bytes,::std::uint64_t& normalized) noexcept
        {
            normalized=0u;
            auto const call{checked_host()}; preload_memory_descriptor_t descriptor{};
            if(!call || index!=0u || !memory_descriptor(0u,&descriptor)) { return false; }
            auto const* shared{call->instance->shared_memory(0u)};
            if(!shared) { return false; }
            if(shared->memory.memtag_shadow.enabled())
            {
                auto const checked{::uwvm2::runtime::memtag::check_host_access(shared->memory,offset,bytes)};
                ::uwvm2::runtime::memtag::record_host_fault(index,offset,checked);
                if(!checked) { return false; }
                normalized=checked.address;return true;
            }
            if(offset>descriptor.byte_length || bytes>descriptor.byte_length-offset) { return false; }
            normalized=offset;return true;
        }
        [[nodiscard]] static bool memory_copy(::std::size_t index, ::std::uint_least64_t offset,
            void* buffer, ::std::size_t bytes, bool write) noexcept
        {
            auto const call{checked_host()}; preload_memory_descriptor_t descriptor{};
            if(!call || index != 0u || !memory_descriptor(0u, &descriptor) ||
               (buffer == nullptr && bytes != 0u))
            { return false; }
            auto const* shared{call->instance->shared_memory(0u)};
            if(!shared) { return false; }
            if(shared->memory.memtag_shadow.enabled())
            {
                bool completed{true};
                auto const checked{::uwvm2::runtime::memtag::with_host_access(
                    shared->memory,offset,bytes,[&](::std::span<::std::byte> data) noexcept
                    {
                        auto* native{static_cast<::std::byte*>(buffer)};
                        for(::std::size_t i{};i!=data.size();++i)
                        {
                            if((i & 4095u)==0u && call->actual->cancellation().stop_requested())
                            { completed=false; return; }
                            ::std::atomic_ref<::std::byte> cell{data[i]};
                            if(write) { cell.store(native[i],::std::memory_order_relaxed); }
                            else { native[i]=cell.load(::std::memory_order_relaxed); }
                        }
                    })};
                ::uwvm2::runtime::memtag::record_host_fault(index,offset,checked);
                return static_cast<bool>(checked) && completed;
            }
            if(offset>descriptor.byte_length || bytes>descriptor.byte_length-offset) { return false; }
            auto* const base{shared->memory.memory_begin};
            auto* const native{static_cast<::std::byte*>(buffer)};
            if(bytes != 0u && base == nullptr) { return false; }
            // Guest plain accesses in this profile use atomic byte operations.
            // Host copies must use the same genuine storage discipline. No
            // atomic vector snapshot is promised; partial copy on stop is kept.
            static_assert(::std::atomic_ref<::std::byte>::is_always_lock_free);
            for(::std::size_t i{}; i != bytes; ++i)
            {
                if((i & 4095u) == 0u && call->actual->cancellation().stop_requested()) { return false; }
                auto cell{::std::atomic_ref<::std::byte>{base[static_cast<::std::size_t>(offset) + i]}};
                if(write) { cell.store(native[i], ::std::memory_order_relaxed); }
                else { native[i] = cell.load(::std::memory_order_relaxed); }
            }
            return true;
        }

#include "uwvm_runtime_threaded_profile_lifecycle.h"
    };
}
#endif
