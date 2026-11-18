// Included within the actual bridge class. Ordinary runtime code is never an
// executable fallback. Work owns its concrete engine, World and all raw buffers.
#include "uwvm_runtime_threaded_signal_host.h"
    private:
        enum class backend : unsigned { integer, llvm };
        struct main_result
        {
            ::uwvm2::utils::container::std_vector<::std::byte> bytes;
            ::std::atomic<bool> returned{};
        };
        struct profile_state
        {
            ::std::mutex retirement;
            ::uwvm2::utils::thread::process_exit::guest_domain guest_exit;
            ::uwvm2::utils::thread::execution_domain::private_generation_gate gate;
            ::std::shared_ptr<admission const> admitted;
            ::std::shared_ptr<group> native;
            ::std::shared_ptr<main_result> result;
            group::CompilerTicket main_preparation;
            ::uwvm2::utils::container::std_vector<::uwvm2::runtime::wasm_threads::threaded_module_image> journal_sources;
            ::uwvm2::utils::container::std_vector<::uwvm2::runtime::wasm_threads::threaded_environment_sample> journal_environments;
            ::std::shared_ptr<signal_owner> signal;
            ::std::shared_ptr<threaded_signal_sender const> signal_sender;
            int signal_retirement_error{};
            ::uwvm2::uwvm::runtime::full::full_source_destruction_completion::owner old_source_completion;
            backend selected{};
            ::std::uint_least64_t generation{};
            bool joined{}, ordinary_reopened{}, requires_reset{};
            ::uwvm2::runtime::wasm_threads::group_outcome outcome{};
        };
        // Cold typed cancellation publication for providers without atomic<shared_ptr>.
        // These existing acquire/release/acq_rel accesses synchronize through this
        // independent mutex. It acquires no registry/Group lock and invokes no
        // callback or last-owner release. Ordinary Host context, not signal-safe.
        class stop_owner final
        {
            ::std::mutex mutex_;
            ::std::shared_ptr<group> value_;
        public:
            void store(::std::shared_ptr<group> next, ::std::memory_order) noexcept
            {
                {
                    ::std::lock_guard lock{mutex_};
                    value_.swap(next);
                }
                // The previous owner in next is released after the lock scope.
            }
            [[nodiscard]] ::std::shared_ptr<group> load(::std::memory_order) noexcept
            {
                ::std::shared_ptr<group> result;
                {
                    ::std::lock_guard lock{mutex_};
                    result = value_;
                }
                return result; // Retained owner; caller invokes Group only unlocked.
            }
            [[nodiscard]] ::std::shared_ptr<group> exchange(::std::shared_ptr<group> next, ::std::memory_order) noexcept
            {
                {
                    ::std::lock_guard lock{mutex_};
                    value_.swap(next);
                }
                return next; // Old owner moves to the caller after unlocking.
            }
        };
        struct control
        {
            ::std::mutex registry;
            ::std::shared_ptr<profile_state> profile;
            stop_owner stop_target;
            ::std::atomic_bool builtin_requested{};
            ::uwvm2::runtime::wasm_threads::threaded_debug_observation observation{::uwvm2::runtime::wasm_threads::threaded_debug_observation::none};
            ::uwvm2::runtime::wasm_threads::threaded_profile_limits configured{};
            bool cli_storage_derived{};
            ::uwvm2::runtime::wasm_threads::threaded_debug_control_limits debug_control{};
            ::std::shared_ptr<::uwvm2::runtime::wasm_threads::threaded_execution_journal> journal{};
            ::uwvm2::utils::container::std_vector<::uwvm2::runtime::wasm_threads::threaded_environment_sample> replay_environments;
            ::std::weak_ptr<group> retained_journal_group;
            ::std::uint64_t retained_journal_serial{};
            ::std::array<::std::byte,32> retained_journal_digest{};
            threaded_signal_policy signal_policy{};
            threaded_signal_report last_signal_report{};
            ::std::uint_least64_t blocked_generation{};
        };
        [[nodiscard]] static control& control_owner() noexcept
        {
            // Fixed process-control storage. It has no automatic static
            // destructor ordering dependency on g_runtime. Runtime shutdown
            // explicitly joins and clears every owned resource before its own
            // provider/source/domain destructors execute.
            alignas(control) static ::std::byte storage[sizeof(control)];
            static control* const actual{::new (static_cast<void*>(storage)) control{}};
            return *actual;
        }

        class concrete_work final : public group::ChildWork
        {
            // First member dies last, after engines, World and native join.
            ::uwvm2::utils::control::wasithread::policy::reservation thread_authority_;
            world::owner world_;
            ::std::shared_ptr<admission const> admission_;
            ::uwvm2::utils::container::std_unique_ptr<::uwvm2::runtime::compiler::uwvm_int::threaded::engine> integer_;
#if defined(UWVM_RUNTIME_LLVM_JIT)
            ::uwvm2::utils::container::std_unique_ptr<ThreadedLlvmChild> llvm_;
#endif
            ::std::shared_ptr<main_result> result_;
            ::uwvm2::utils::container::std_vector<::std::byte> parameters_;
            ::std::size_t entry_{};
            ::uwvm2::utils::container::std_vector<::std::size_t> initialization_order_;
            ::uwvm2::utils::thread::process_exit::guest_domain* guest_exit_{};
            bool main_{};
        public:
            concrete_work(world::owner instance, ::std::shared_ptr<main_result> output,
                ::std::size_t entry, ::std::span<::std::byte const> parameters,
                ::uwvm2::utils::control::wasithread::policy::reservation permission)
                : thread_authority_{::std::move(permission)}, world_{::std::move(instance)}, admission_{world_->admission().retain()},
                  result_{::std::move(output)}, entry_{entry}, main_{!!result_}
            { parameters_.resize(parameters.size()); if(!parameters.empty()) { ::std::memcpy(parameters_.data(), parameters.data(), parameters.size()); } }
            void bind_guest_exit(::uwvm2::utils::thread::process_exit::guest_domain& domain) noexcept{guest_exit_=&domain;}
            [[nodiscard]] auto retained_world() const noexcept -> world::owner override { return world_; }
            [[nodiscard]] auto retained_admission() const noexcept -> ::std::shared_ptr<admission const> override { return admission_; }
            [[nodiscard]] ::std::size_t owned_storage_bytes() const noexcept override
            {
                auto const limit{admission_->limits().max_child_storage_bytes};
                ::std::size_t sum{world_->owned_storage_bytes()};
                auto add{[&](::std::size_t n) noexcept { if(sum > limit || n > limit - sum) { sum = SIZE_MAX; } else { sum += n; } }};
                add(sizeof(concrete_work)); add(parameters_.capacity()); add(initialization_order_.capacity()*sizeof(::std::size_t));
                if(integer_) { add(integer_->owned_storage_bytes()); }
#if defined(UWVM_RUNTIME_LLVM_JIT)
                if(llvm_) { add(llvm_->owned_storage_bytes()); }
#endif
                return sum;
            }
            [[nodiscard]] ::std::size_t owned_code_bytes() const noexcept override
            {
                if(integer_) { return integer_->owned_code_bytes(); }
#if defined(UWVM_RUNTIME_LLVM_JIT)
                if(llvm_) { return llvm_->owned_code_bytes(); }
#endif
                return SIZE_MAX;
            }
            [[nodiscard]] ::uwvm2::runtime::wasm_threads::code_debug_image const* debug_image() const noexcept override
            {
                if(integer_)return integer_->debug_image();
#if defined(UWVM_RUNTIME_LLVM_JIT)
                if(llvm_)return llvm_->debug_image();
#endif
                return nullptr;
            }
            void synchronize_debug_stop() noexcept override
            {
                // Invoked only for an armed stop on this work's actual native
                // worker. Ordinary/idle execution pays no additional poll.
                ::uwvm2::runtime::memtag::deferred::poll(true);
            }
            [[nodiscard]] bool prepare(backend selected, group::CompilerLease const& compiler) noexcept
            {
                if(!compiler.owns_current_native_thread() || &compiler.admission() != admission_.get() ||
                   compiler.cancellation().stop_requested()) { return false; }
                auto const& limits{admission_->limits()};
                // Cold, quota-checked dependency traversal. No allocation is
                // introduced on a running Guest stack for module-start order.
                auto const count=world_->module_count();
                if(count>limits.max_child_storage_bytes/(4u*sizeof(::std::size_t)))return false;
                struct start_frame{::std::size_t module{},next{};};
                ::uwvm2::utils::container::std_vector<unsigned char> visited(count);
                ::uwvm2::utils::container::std_vector<start_frame> starts;
                initialization_order_.reserve(count);starts.reserve(count);
                starts.push_back({0u,0u});visited[0]=1u;
                while(!starts.empty())
                {
                    auto& frame=starts.back();
                    if(frame.next<world_->initialization_dependency_count(frame.module))
                    {
                        ::std::size_t target{};auto import=frame.next++;
                        if(world_->initialization_dependency(frame.module,import,target)&&!visited[target])
                        {visited[target]=1u;starts.push_back({target,0u});}
                        continue;
                    }
                    auto module=frame.module;starts.pop_back();visited[module]=2u;
                    if(world_->initialization_function_index(module))initialization_order_.push_back(module);
                }
                auto const before{owned_storage_bytes()};
                if(before >= limits.max_child_storage_bytes) { return false; }
                if(selected == backend::integer)
                {
                    namespace integer = ::uwvm2::runtime::compiler::uwvm_int::threaded;
                    integer::limits budget{};
                    budget.storage_bytes = limits.max_child_storage_bytes - before;
                    budget.source_bytes = limits.max_child_code_bytes;
                    budget.instruction_bytes = limits.max_child_code_bytes;
                    budget.values = limits.max_value_stack_bytes / sizeof(integer::stack_value);
                    budget.controls = limits.max_control_frames;
                    budget.calls = limits.max_call_frames;
                    budget.functions=0u;
                    for(::std::size_t m{};m<world_->module_count();++m)
                    {
                        auto const* module=world_->module(m);
                        auto const n=module->imported_function_vec_storage.size()+module->local_defined_function_vec_storage.size();
                        if(n>SIZE_MAX-budget.functions)return false;budget.functions+=n;
                    }
                    budget.host_bytes = limits.max_value_stack_bytes;
                    budget.exception_objects = limits.max_call_frames;
                    budget.exception_values = budget.values;
                    ::uwvm2::validation::error::code_validation_error_impl error{};
                    auto built{integer::engine::compile(world_, budget, error,!::uwvm2::uwvm::runtime::runtime_mode::runtime_disable_memtag)};
                    if(!built.code || !built.outcome.normal()) { ::fast_io::perrln("uwvm: [error] WASI Threads interpreter preparation rejected: ",static_cast<unsigned>(built.outcome.code)); return false; }
                    integer_ = ::std::move(built.code);
                }
#if defined(UWVM_RUNTIME_LLVM_JIT)
                else
                {
                    auto built{ThreadedLlvmChild::prepare(world_, compiler, limits.max_child_storage_bytes - before)};
                    if(!built.child || built.status != ThreadedLlvmChild::preparation_status::ok) { ::fast_io::perrln("uwvm: [error] WASI Threads LLVM preparation rejected: ",static_cast<unsigned>(built.status)); return false; }
                    llvm_ = ::std::move(built.child);
                }
#else
                else { return false; }
#endif
                auto const storage{owned_storage_bytes()};
                auto const world_storage{world_->owned_storage_bytes()};
                return storage <= limits.max_child_storage_bytes && storage >= world_storage &&
                    owned_code_bytes() <= limits.max_child_code_bytes &&
                    world_->reserve_execution_storage(storage - world_storage);
            }
            [[nodiscard]] bool refresh_after_actual_parent_active() noexcept
            {
                if(integer_) { return true; } // actual table cells read directly, no cached raw table view
#if defined(UWVM_RUNTIME_LLVM_JIT)
                if(llvm_) { return llvm_->refresh_table_views_before_launch(); }
#endif
                return false;
            }
            [[nodiscard]] group::child_completion execute(lease& actual, ::std::size_t entry,
                ::std::span<::std::byte> results, ::std::span<::std::byte const> parameters,::std::size_t module=SIZE_MAX) noexcept
            {
                if(integer_)
                {
                    auto const result{integer_->run_entry(actual, entry, results, parameters,module)};
                    if(result.code==::uwvm2::runtime::compiler::uwvm_int::threaded::status::trap_memtag)
                    {
                        // run_entry has released every memory snapshot/guard.
                        print_memtag_fault(0u,result.memtag_pointer,result.memtag_granule,
                            result.memtag_pointer_tag,result.memtag_allocation_tag);
                    }
                    if(result.normal()) { return group::child_completion::returned; }
                    return actual.cancellation().stop_requested() ? group::child_completion::group_stopped : group::child_completion::rejected;
                }
#if defined(UWVM_RUNTIME_LLVM_JIT)
                if(llvm_)
                {
                    auto const result{llvm_->run_entry(actual, entry, results, parameters,module)};
                    if(result == ThreadedLlvmChild::run_status::completed) { return group::child_completion::returned; }
                    return result == ThreadedLlvmChild::run_status::group_stopped ? group::child_completion::group_stopped : group::child_completion::rejected;
                }
#endif
                return group::child_completion::rejected;
            }
            [[nodiscard]] group::child_completion run(lease& actual, ::std::uint32_t tid, ::std::uint32_t argument) noexcept override try
            {
                if(!actual_lease(actual, *world_) || (main_ ? tid != 0u : tid == 0u)) { return group::child_completion::rejected; }
                namespace threads=::uwvm2::runtime::wasm_threads;
                threads::thread_journal_context recording{};
                recording.journal=actual.group_->execution_journal_.get();recording.worker=actual.slot_->debug_incarnation;
                recording.cancellation=actual.cancellation();recording.native=&actual;
                ::uwvm2::utils::container::std_vector<::std::uint64_t> logical_modules;
                if(recording.journal)
                {
                    logical_modules.reserve(world_->module_count());
                    for(::std::size_t module{};module<world_->module_count();++module)
                        logical_modules.push_back(logical_source_module(world_->source_module_id(module)));
                    recording.logical_modules=logical_modules;
                }
                recording.poll=[](void* pointer) noexcept
                {
                    auto& worker=*static_cast<lease*>(pointer);auto* current=threads::current_thread_journal;
                    if(!current||current->host)return !worker.cancellation().stop_requested();
                    return worker.group_->debug_poll(*worker.slot_,current->waiting?threads::debug_stop_site::atomic_wait:
                        current->has_location?threads::debug_stop_site::before_instruction:threads::debug_stop_site::native_boundary,
                        current->location.module,current->location.function,current->location.offset,current->location.a);
                };
                recording.guest_exiting=[]() noexcept
                {auto* domain=::uwvm2::utils::thread::process_exit::current_guest_domain;return domain&&domain->claimed();};
                recording.return_host=[](void* pointer) noexcept
                {auto& worker=*static_cast<lease*>(pointer);return worker.group_->debug_return_host(*worker.slot_);};
                recording.retain_host=[](void* pointer) noexcept
                {
                    auto& worker=*static_cast<lease*>(pointer);auto* current=threads::current_thread_journal;
                    return current&&worker.group_->debug_retain_host(*worker.slot_,current->location.module,current->location.function,current->location.offset);
                };
                // No journal TLS scope or event allocation for ordinary execution.
                struct selected_scope
                {
                    threads::thread_journal_context* previous;
                    explicit selected_scope(threads::thread_journal_context* p):previous{threads::current_thread_journal}
                    {threads::current_thread_journal=p;}
                    ~selected_scope(){threads::current_thread_journal=previous;}
                } selected{recording.journal?&recording:nullptr};
                namespace deferred=::uwvm2::runtime::memtag::deferred;
                struct fault_context { lease* worker; unsigned backend; } fault_owner{&actual,integer_?1u:2u};
                deferred::execution_fault_receiver fault_receiver{&fault_owner,[](void* pointer,deferred::fault const& evidence) noexcept
                {
                    auto const& owner=*static_cast<fault_context*>(pointer);auto& worker=*owner.worker;
                    namespace crash=::uwvm2::runtime::diagnostic::crash;
                    crash::memory_detail const detail{evidence.memory_index,evidence.raw_pointer,
                        evidence.access.mismatch_granule,evidence.access.pointer_tag,evidence.access.allocation_tag};
                    crash::report(crash::category::memtag,evidence.hardware_notification?
                        "asynchronous hardware memory tag mismatch":"asynchronous memory tag mismatch",
                        owner.backend,evidence.module_id,evidence.function_index,evidence.wasm_offset,
                        evidence.hardware_notification?nullptr:&detail,true);
                    crash::pause_process();
                    struct transition { lease* worker; } request{&worker};
                    if(!::uwvm2::utils::thread::process_exit::run_before_exit_request([](void* address) noexcept
                    {
                        auto& worker=*static_cast<transition*>(address)->worker;
                        auto* context=threads::current_thread_journal;
                        worker.request_trap(122u,context?context->location.module:SIZE_MAX,
                            context?context->location.function:SIZE_MAX,context?context->location.offset:UINT64_MAX,
                            threads::journal_fault::memtag);
                    },&request))::uwvm2::utils::thread::process_exit::request_process_exit(0u);
                }};
                deferred::execution_fault_scope fault_scope{recording.journal?&fault_receiver:nullptr};
                ::uwvm2::utils::thread::process_exit::guest_domain_scope exit_scope{recording.journal?guest_exit_:nullptr};
                ::uwvm2::runtime::memtag::tag_entropy_observer entropy{&recording,
                    [](void* pointer,::std::uint8_t& tag,bool(*native)(::std::uint8_t&) noexcept) noexcept
                    {
                        auto& context=*static_cast<threads::thread_journal_context*>(pointer);
                        auto event=threads::journal_at(threads::journal_kind::entropy);
                        if(!context.journal->select(context,::std::move(event),false))return false;
                        ::std::array<::std::byte,2> result{};
                        if(!context.journal->replaying()){result[0]=static_cast<::std::byte>(native(tag));result[1]=static_cast<::std::byte>(tag);}
                        if(!context.journal->value(context,result))return false;
                        if(result[0]!=::std::byte{1})return false;tag=::std::to_integer<::std::uint8_t>(result[1]);return true;
                    }};
                struct entropy_scope
                {
                    ::uwvm2::runtime::memtag::tag_entropy_observer const* old;
                    explicit entropy_scope(::uwvm2::runtime::memtag::tag_entropy_observer const* value):
                        old{::uwvm2::runtime::memtag::current_tag_entropy_observer}
                    {if(value)::uwvm2::runtime::memtag::current_tag_entropy_observer=value;}
                    ~entropy_scope(){::uwvm2::runtime::memtag::current_tag_entropy_observer=old;}
                } entropy_selected{recording.journal?&entropy:nullptr};
                if(recording.journal)
                {
                    // Worker birth precedes any Wasm location. Its module,
                    // function and offset fields are deliberately zero; bind
                    // source identities only at actual instruction boundaries.
                    threads::journal_event event{};event.kind=threads::journal_kind::enter;
                    event.a=tid;event.b=argument;event.c=entry_;
                    event.input.assign(parameters_.begin(),parameters_.end());
                    if(!recording.journal->select(recording,::std::move(event)))return group::child_completion::rejected;
                }
                auto finish=[&](group::child_completion result)
                {
                    recording.has_location=false;
                    if(recording.journal && result==group::child_completion::returned)
                    {
                        auto event=threads::journal_at(threads::journal_kind::leave);event.a=tid;
                        if(!recording.journal->select(recording,::std::move(event))||
                           !recording.journal->verify(recording,main_?::std::span<::std::byte const>{result_->bytes}: ::std::span<::std::byte const>{}))
                            result=group::child_completion::rejected;
                        if(result==group::child_completion::returned&&!main_)
                            actual.slot_->journal_departed.store(true,::std::memory_order_release);
                    }
                    // Main return terminates the private process. Publish that
                    // cancellation while this worker still owns its journal turn;
                    // otherwise a child could append an instruction between the
                    // recorded terminal event and the actual Group stop.
                    if(main_ && result==group::child_completion::returned) finish_main(actual);
                    if(recording.journal)recording.journal->release(recording);
                    return result;
                };
                if(!main_ && world_->initialize_imported_data() !=
                    ::uwvm2::uwvm::runtime::initializer::details::active_segment_result::ok)
                {
                    namespace crash=::uwvm2::runtime::diagnostic::crash;
                    crash::report(crash::category::execution,"threaded child active data out of bounds");
                    crash::pause_process();
                    actual.request_trap(121u,recording.logical_module(0u),0u,0u,threads::journal_fault::execution);
                    return finish(group::child_completion::group_stopped);
                }
                auto const initialization{world_->initialization_function_index()};
                for(auto module:initialization_order_)
                {
                    auto function=world_->initialization_function_index(module);
                    if(!function)return finish(group::child_completion::rejected);
                    auto initialized=execute(actual,*function,{},{},module);
                    if(initialized!=group::child_completion::returned)return finish(initialized);
                }
                if(main_)
                {
                    auto const result{finish(initialization && *initialization == entry_ ? group::child_completion::returned :
                        execute(actual, entry_, result_->bytes, parameters_))};
                    if(result == group::child_completion::returned)
                    { result_->returned.store(true, ::std::memory_order_release); }
                    return result;
                }
                ::std::array<::std::uint32_t, 2u> parameters{tid, argument};
                return finish(execute(actual, world_->thread_start_function_index(), {}, ::std::as_bytes(::std::span{parameters})));
            }
            catch(...)
            {
                if(!actual.group_->execution_journal_)::std::terminate();
                actual.group_->execution_journal_->refuse();
                actual.request_trap(0x52504c59u,0u,entry_,0u);
                return group::child_completion::rejected;
            }
        };
        static void finish_main(lease& actual) noexcept
        { actual.group_->request_stop({::uwvm2::runtime::wasm_threads::termination_kind::main_return, 0u, 0u, 0u, 0u}); }

        [[nodiscard]] static ::uwvm2::utils::container::std_unique_ptr<concrete_work> prepare_work(profile_state& state,
            group::CompilerLease const& compiler, bool main, ::std::size_t origin, ::std::size_t entry, ::std::span<::std::byte const> parameters,
            ::uwvm2::runtime::wasm_threads::termination_kind& failure,
            ::uwvm2::utils::control::wasithread::policy::reservation& permission) noexcept
        {
            if(!compiler.owns_current_native_thread() || compiler.retained_admission() != state.admitted ||
               compiler.cancellation().stop_requested()) { return {}; }
            try
            {
                if(!actual_source_providers(*state.admitted)) { ::fast_io::perrln("uwvm: [error] WASI Threads source provider authentication failed."); return {}; }
                auto created{main ? world::borrow_actual_parent(*state.admitted) : world::create_for_module(*state.admitted,origin)};
                if(!created.world)
                {
                    namespace initializer = ::uwvm2::uwvm::runtime::initializer;
                    if(initializer::threaded_child_error_is_profile_rejection(created.error))
                    { failure = ::uwvm2::runtime::wasm_threads::termination_kind::preparation_rejected; }
                    ::fast_io::perrln("uwvm: [error] WASI Threads instance preparation rejected: ",
                        ::fast_io::mnp::os_c_str(initializer::threaded_child_error_description(created.error)),
                        " (reason=", static_cast<unsigned>(created.error), ").");
                    return {};
                }
                auto work{::uwvm2::utils::container::make_unique<concrete_work>(::std::move(created.world),
                    main ? state.result : nullptr, entry, parameters, ::std::move(permission))};
                work->bind_guest_exit(state.guest_exit);
                if(!work->prepare(state.selected, compiler) || compiler.cancellation().stop_requested()) { return {}; }
                return work;
            }
            catch(::std::bad_alloc const&)
            { failure=::uwvm2::runtime::wasm_threads::termination_kind::preparation_rejected; return {}; }
            catch(...) { return {}; }
        }

#include "uwvm_runtime_threaded_history_environment.h"
        [[nodiscard]] static bool bind_journal_origin(profile_state& state,world const& instance,
            ::std::size_t entry,::std::span<::std::byte const> parameters) noexcept
        {
            auto journal=state.native->execution_journal_;if(!journal)return true;
            try
            {
                ::fast_io::sha256_context hash{},code_hash{};
                struct tag_owner {::uwvm2::object::memory::linear::native_memory_t const* memory;::std::size_t length,begin,count;};
                ::uwvm2::utils::container::std_vector<tag_owner> tag_owners;
                ::uwvm2::utils::container::std_vector<::std::byte> initial_tags;
                auto code_number=[&](::std::uint64_t value){::std::array<::std::byte,8> bytes{};
                    for(unsigned n{};n<8u;++n)bytes[n]=static_cast<::std::byte>(value>>(8u*n));code_hash.update(bytes.data(),bytes.data()+bytes.size());};
                state.journal_sources.clear();state.journal_sources.reserve(g_runtime.modules.size());code_number(g_runtime.modules.size());
                auto number=[&](::std::uint64_t value){::std::array<::std::byte,8> bytes{};
                    for(unsigned n{};n<8u;++n)bytes[n]=static_cast<::std::byte>(value>>(8u*n));hash.update(bytes.data(),bytes.data()+bytes.size());};
                // Every identity and the visitation order must be independent
                // of the native hash-table order. Preserve the selected root too.
                ::uwvm2::utils::container::std_vector<::std::size_t> source_order(g_runtime.modules.size());
                for(::std::size_t i{};i<source_order.size();++i)
                    source_order[static_cast<::std::size_t>(logical_source_module(i))]=i;
                number(g_runtime.modules.size());number(logical_source_module(instance.source_module_id(0u)));
                number(entry);number(parameters.size());
                if(!parameters.empty())hash.update(parameters.data(),parameters.data()+parameters.size());
                auto function_identity=[&](void const* pointer,bool imported)
                    {
                        if(!pointer){number(UINT64_MAX);number(0u);return true;}
                        for(::std::size_t owner_index{};owner_index<g_runtime.modules.size();++owner_index)
                        {
                            auto* owner=g_runtime.modules.index_unchecked(source_order[owner_index]).runtime_module;if(!owner)return false;
                            if(imported)
                            {for(::std::size_t f{};f<owner->imported_function_vec_storage.size();++f)
                                if(pointer==::std::addressof(owner->imported_function_vec_storage.index_unchecked(f)))
                                {number(owner_index);number(f);return true;}}
                            else
                            {for(::std::size_t f{};f<owner->local_defined_function_vec_storage.size();++f)
                                if(pointer==::std::addressof(owner->local_defined_function_vec_storage.index_unchecked(f)))
                                {number(owner_index);number(owner->imported_function_vec_storage.size()+f);return true;}}
                        }
                        return false;
                    };
                namespace st=::uwvm2::uwvm::runtime::storage;
                using reference_kind=::uwvm2::object::global::wasm_ref_kind;
                ::uwvm2::utils::container::std_vector<st::gc_reference> pending_references;
                auto reference_identity=[&](st::gc_reference const& ref) -> bool
                {
                    number(static_cast<unsigned>(ref.kind));
                    if(ref.kind==reference_kind::wasm_null)return true;
                    if(ref.kind==reference_kind::wasm_i31){number(ref.storage.wasm_i31.get_u());return true;}
                    if(ref.kind==reference_kind::wasm_func){number(ref.storage.func_idx);return true;}
                    if(ref.kind==reference_kind::wasm_func_imported||ref.kind==reference_kind::wasm_func_defined)
                        return function_identity(ref.storage.ptr,ref.kind==reference_kind::wasm_func_imported);
                    if(!ref.storage.ptr)return false;
                    if(ref.kind!=reference_kind::wasm_struct&&ref.kind!=reference_kind::wasm_array&&ref.kind!=reference_kind::wasm_extern)return false;
                    for(::std::size_t i{};i<pending_references.size();++i)
                        if(pending_references[i].kind==ref.kind&&pending_references[i].storage.ptr==ref.storage.ptr)
                        {number(i);return true;}
                    number(pending_references.size());pending_references.push_back(ref);return true;
                };
                for(::std::size_t m{};m<g_runtime.modules.size();++m)
                {
                    auto id=source_order[m];auto* parent=g_runtime.modules.index_unchecked(id).runtime_module;
                    if(!parent)return false;
                    auto const* file=instance.source_owner()->actual_validated_file(id,state.admitted->source_validation_epoch(),parent);
                    if(!file||!file->has_owned_source_image() ||
                       (!::uwvm2::uwvm::runtime::runtime_mode::runtime_disable_memtag &&
                        !::uwvm2::uwvm::runtime::validator::admits_tag_only_checkpoint(*file)))return false;
                    auto name=g_runtime.modules.index_unchecked(id).module_name;
                    number(m);number(name.size());auto* text=reinterpret_cast<::std::byte const*>(name.data());
                    if(name.size())hash.update(text,text+name.size());number(file->source_size());
                    auto* bytes=reinterpret_cast<::std::byte const*>(file->source_cbegin());hash.update(bytes,bytes+file->source_size());
                    code_number(m);code_number(name.size());if(name.size())code_hash.update(text,text+name.size());
                    code_number(file->source_size());code_hash.update(bytes,bytes+file->source_size());
                    ::uwvm2::runtime::wasm_threads::threaded_module_image original;
                    original.identifier=m;original.name.assign(text,text+name.size());original.wasm.assign(bytes,bytes+file->source_size());
                    state.journal_sources.push_back(::std::move(original));
                    // Authenticate logical initial values, never pointer bytes.
                    // Imported aliases resolve into this same owned source set.
                    number(parent->local_defined_global_vec_storage.size());
                    for(auto const& value:parent->local_defined_global_vec_storage)
                    {
                        auto const& global=value.global;number(static_cast<unsigned>(global.kind));number(global.is_mutable);
                        using kind=::uwvm2::object::global::global_type;
                        if(global.kind==kind::wasm_ref)
                        {
                            if(!reference_identity(global.storage.ref))return false;
                        }
                        else
                        {
                            auto bytes=global.kind==kind::wasm_v128?16u:(global.kind==kind::wasm_i64||global.kind==kind::wasm_f64)?8u:4u;
                            ::std::array<::std::byte,16> bits{};::std::memcpy(bits.data(),&global.storage,bytes);
                            // All admitted Linux ABIs are little-endian; canonical
                            // scalar words also avoid hashing padding or native refs.
                            for(unsigned offset{};offset<bytes;offset+=4u)
                            {::std::uint32_t word{};::std::memcpy(&word,bits.data()+offset,4u);number(word);}
                        }
                    }
                    number(parent->local_defined_table_vec_storage.size());
                    for(auto const& table:parent->local_defined_table_vec_storage)
                    {
                        number(table.elems.size());
                        for(auto const& value:table.elems)
                        {
                            if(!reference_identity(st::runtime_table_slot_to_gc_reference(value)))return false;
                        }
                    }
                    number(parent->local_defined_memory_vec_storage.size());
                    for(auto const& memory:parent->local_defined_memory_vec_storage)
                    {
                        auto const& owner=memory.memory;auto length=::uwvm2::runtime::wasm_threads::threaded_memory_length(owner);
                        number(length);
                        if(!::uwvm2::runtime::wasm_threads::visit_threaded_memory(owner,
                            [&](::std::byte* bytes,::std::size_t,::std::size_t count) noexcept
                            {hash.update(bytes,bytes+count);return true;}))return false;
                        auto const& tags=owner.memtag_shadow;number(tags.enabled());
                        if(tags.enabled())
                        {
                            if(!tags.allocation_directory().empty())return false;
                            number(static_cast<unsigned>(tags.width()));number(tags.shared());
                            auto const tag_count=length/16u+(length%16u!=0u);number(tag_count);
                            tag_owners.push_back({&owner,length,initial_tags.size(),tag_count});
                            if(tag_count>SIZE_MAX-initial_tags.size())return false;
                            initial_tags.resize(initial_tags.size()+tag_count);
                            auto read_tags=[&]<typename Address,bool Shared>()
                            {
                                auto view=tags.template view<Address,Shared>(length);
                                for(::std::size_t n{};n<tag_count;++n)
                                {auto value=view.load(static_cast<Address>(n*16u));if(!value)return false;
                                 initial_tags[tag_owners.back().begin+n]=static_cast<::std::byte>(value.allocation_tag);}
                                return true;
                            };
                            bool const wide=tags.width()==::uwvm2::extension::memtag::address_width::i64;
                            bool valid=wide?(tags.shared()?read_tags.template operator()<::std::uint64_t,true>():read_tags.template operator()<::std::uint64_t,false>()):
                                (tags.shared()?read_tags.template operator()<::std::uint32_t,true>():read_tags.template operator()<::std::uint32_t,false>());
                            if(!valid)return false;
                        }
                    }
                }
                // Iterative logical object IDs preserve sharing/cycles across
                // all module roots. Every pointer is authenticated against a
                // retained store before reading a header or payload. All Guest
                // participants are still unstarted under the generation gate.
                for(::std::size_t i{};i<pending_references.size();++i)
                {
                    auto const ref=pending_references[i]; // enqueue may move the vector
                    if(ref.kind==reference_kind::wasm_extern)
                    {
                        st::gc_reference inner{};::std::size_t owner=SIZE_MAX;
                        {
                            st::gc_object_store::bridge_guard guard{};
                            auto const* token=st::gc_object_store::find_bridge_token_locked(ref.storage.ptr);
                            if(!token)return false;
                            for(::std::size_t m{};m<source_order.size();++m)
                                if(g_runtime.modules.index_unchecked(source_order[m]).runtime_module->gc_store.get()==token->owner){owner=m;break;}
                            inner=token->inner;
                        }
                        if(owner==SIZE_MAX)return false;number(owner);
                        if(!reference_identity(inner))return false;
                        continue;
                    }
                    auto const kind=ref.kind==reference_kind::wasm_struct?st::gc_type::composite_kind::struct_:st::gc_type::composite_kind::array;
                    st::gc_object_store const* store{};::std::size_t owner=SIZE_MAX,length{};::std::uint_least32_t type{};
                    for(::std::size_t m{};m<source_order.size();++m)
                    {
                        auto const* candidate=g_runtime.modules.index_unchecked(source_order[m]).runtime_module->gc_store.get();
                        if(!candidate)continue;
#if defined(UWVM_EXPERIMENTAL_COMPACT_NUMERIC) && UWVM_EXPERIMENTAL_COMPACT_NUMERIC == 1
                        if(ref.kind==reference_kind::wasm_struct)
                        {
                            auto compact=candidate->checked_local_compact_object(ref);
                            if(compact){store=candidate;owner=m;type=compact.type_index();length=1u;break;}
                        }
#endif
                        auto const* object=candidate->checked_local_object(ref,kind);
                        if(object){store=candidate;owner=m;type=object->type_index;length=object->length;break;}
                    }
                    if(!store)return false;
                    auto const* layout=store->checked_type(type,kind);
                    if(!layout||(kind==st::gc_type::composite_kind::struct_?layout->field_count!=length:layout->field_count!=1u))return false;
                    number(owner);number(type);number(length);
                    for(::std::size_t member{};member<length;++member)
                    {
                        st::gc_object_value value{};
                        auto const result=kind==st::gc_type::composite_kind::struct_?store->struct_get(ref,member,false,value):store->array_get(ref,member,false,value);
                        if(result!=st::gc_object_status::ok)return false;
                        auto const& field=layout->fields[kind==st::gc_type::composite_kind::struct_?member:0u].storage;
                        number(static_cast<unsigned>(field.value.kind));number(static_cast<unsigned>(field.packed));
                        if(field.value.kind==st::gc_type::value_kind::reference)
                        {
                            st::gc_reference child{};::std::memcpy(&child,value.bits.data(),sizeof(child));
                            if(!reference_identity(child))return false;
                        }
                        else
                        {
                            auto const count=field.value.kind==st::gc_type::value_kind::v128?16u:
                                (field.value.kind==st::gc_type::value_kind::i64||field.value.kind==st::gc_type::value_kind::f64)?8u:4u;
                            for(unsigned offset{};offset<count;offset+=4u)
                            {::std::uint32_t word{};::std::memcpy(&word,value.bits.data()+offset,4u);number(word);}
                        }
                    }
                }
                number(pending_references.size());
                hash.do_final();code_hash.do_final();::std::array<::std::byte,64> digest{};
                hash.digest_to_byte_ptr(digest.data());code_hash.digest_to_byte_ptr(digest.data()+32u);
                if(!journal->origin(digest,initial_tags))return false;
                if(journal->replaying())
                {
                    // Initial allocation tags include nondeterministic static
                    // global colors. Rebind them only after authenticating the
                    // complete source/byte/global/table state and every owner
                    // geometry; these bytes cannot supply an owner or address.
                    namespace mt=::uwvm2::extension::memtag;
                    for(auto const& plan:tag_owners)
                    {
                        unsigned const maximum=plan.memory->memtag_shadow.width()==mt::address_width::i32?15u:255u;
                        for(::std::size_t n{};n<plan.count;++n)
                            if(::std::to_integer<unsigned>(initial_tags[plan.begin+n])>maximum)return false;
                    }
                    for(auto const& plan:tag_owners)
                    {
                        auto const& tags=plan.memory->memtag_shadow;
                        auto apply=[&]<typename Address,bool Shared>() noexcept
                        {
                            auto view=tags.template view<Address,Shared>(plan.length);
                            for(::std::size_t n{};n<plan.count;++n)
                                if(!view.store(view.insert(static_cast<Address>(n*16u),::std::to_integer<unsigned>(initial_tags[plan.begin+n])),
                                    (::std::min)(::std::size_t{16u},plan.length-n*16u)))return false;
                            return true;
                        };
                        bool const shared=tags.shared();bool const wide=tags.width()==mt::address_width::i64;
                        if(!(wide?(shared?apply.template operator()<::std::uint64_t,true>():apply.template operator()<::std::uint64_t,false>()):
                            (shared?apply.template operator()<::std::uint32_t,true>():apply.template operator()<::std::uint32_t,false>())))return false;
                    }
                    return true;
                }
                ::std::array<::std::byte,16> label{};::std::copy_n(digest.begin(),label.size(),label.begin());
                if(::std::all_of(label.begin(),label.end(),[](::std::byte b){return b==::std::byte{};}))label[0]=::std::byte{1};
                return capture_environment_sample(state,label,1u);
            }
            catch(...){return false;}
        }
        class concrete_compile_work final : public group::CompilerWork
        {
            ::uwvm2::utils::control::wasithread::policy::reservation thread_authority_;
            ::std::shared_ptr<profile_state> state_;
            ::uwvm2::utils::container::std_vector<::std::byte> parameters_;
            ::std::size_t origin_{},entry_{};
            ::std::uint32_t argument_{};
            bool main_{};
        public:
            concrete_compile_work(::std::shared_ptr<profile_state> state, bool main, ::std::size_t origin, ::std::size_t entry,
                ::std::uint32_t argument, ::std::span<::std::byte const> parameters,
                ::uwvm2::utils::control::wasithread::policy::reservation permission)
                : thread_authority_{::std::move(permission)}, state_{::std::move(state)}, origin_{origin}, entry_{entry}, argument_{argument}, main_{main}
            {
                // Submission never borrows a Guest/Host raw span beyond return.
                parameters_.resize(parameters.size());
                if(parameters_.capacity()>state_->admitted->limits().max_value_stack_bytes) { throw ::std::bad_alloc{}; }
                if(!parameters.empty()) { ::std::memcpy(parameters_.data(), parameters.data(), parameters.size()); }
            }
            [[nodiscard]] auto retained_admission() const noexcept -> ::std::shared_ptr<admission const> override
            { return state_->admitted; }
            [[nodiscard]] ::std::size_t owned_submission_bytes() const noexcept override
            { return parameters_.capacity() <= SIZE_MAX - sizeof(concrete_compile_work) ?
                sizeof(concrete_compile_work) + parameters_.capacity() : SIZE_MAX; }
            [[nodiscard]] group::compiler_result run(group::CompilerLease& actual) noexcept override
            {
                using completion = group::compiler_completion;
                using termination = ::uwvm2::runtime::wasm_threads::termination_kind;
                auto fail{[&](completion why, int error, termination reason = termination::native_failure) noexcept -> group::compiler_result
                {
                    if(main_)
                    {
                        // A native allocation/thread limit before first entry is
                        // admission refusal, not a corrupt VM invariant.
                        if(reason==termination::native_failure && (error==ENOMEM || error==EAGAIN))
                        { reason=termination::preparation_rejected; }
                        state_->native->request_stop({reason, static_cast<::std::uint32_t>(error), 0u, entry_, 0u});
                    }
                    return {why, {}, error};
                }};
                if(!actual.owns_current_native_thread() || actual.retained_admission() != state_->admitted)
                { return fail(completion::rejected, EINVAL); }
                if(actual.cancellation().stop_requested()) { return {completion::cancelled, {}, 0}; }
                termination failure{termination::native_failure};
                auto work{prepare_work(*state_, actual, main_, origin_, entry_, parameters_, failure, thread_authority_)};
                if(!work) { return fail(actual.cancellation().stop_requested() ? completion::cancelled : completion::rejected, EINVAL, failure); }
                if(main_)
                {
                    auto const initialized{::uwvm2::uwvm::runtime::initializer::ThreadedMainActiveSegments::apply(
                        *state_->admitted, work->retained_world(), *state_->native, actual.actual_reservation())};
                    using active_status=::uwvm2::uwvm::runtime::initializer::threaded_main_active_status;
                    if(initialized != active_status::applied)
                    {
                        ::fast_io::perrln("uwvm: [error] WASI Threads active initialization rejected: ",static_cast<unsigned>(initialized));
                        if(initialized==active_status::bounds)
                        {
                            ::uwvm2::runtime::diagnostic::crash::report(
                                ::uwvm2::runtime::diagnostic::crash::category::execution,"threaded active segment out of bounds");
                            state_->native->request_stop({termination::trap,
                                static_cast<::std::uint32_t>(::uwvm2::runtime::diagnostic::crash::category::execution),0u,entry_,0u});
                            return {completion::rejected,{},EINVAL};
                        }
                        if(initialized==active_status::unsupported_shape || initialized==active_status::stopped)
                            return fail(completion::rejected,EINVAL,termination::preparation_rejected);
                        return fail(completion::rejected,EINVAL);
                    }
                    if(!work->refresh_after_actual_parent_active())return fail(completion::rejected,EINVAL);
                    // Ordinary initialization deliberately defers physical MTE
                    // activation together with active segments. This private
                    // initializer owns that deferred phase: publish checked
                    // mappings before the first actual native Guest is launched.
                    // Every new worker then enables its own checked thread mode.
                    if(::uwvm2::extension::memtag::hardware_checked_access_requested())
                    {
                        auto const& source=state_->admitted->source_owner();
                        auto& registry=::uwvm2::uwvm::runtime::storage::active_runtime_registry();
                        if(!source || &source->registry()!=&registry || !state_->admitted->owns_actual_generation())
                            return fail(completion::rejected,EINVAL,termination::preparation_rejected);
                        for(auto& [name,module]:registry)
                        {
                            static_cast<void>(name);
                            for(auto& record:module.local_defined_memory_vec_storage)
                            {
                                auto& memory=record.memory;
                                if(!memory.memtag_shadow.enabled())continue;
                                if(!memory.memtag_shadow.hardware_selected() ||
                                   !::uwvm2::runtime::memtag::activate_owner_hardware(memory) ||
                                   !memory.memtag_shadow.hardware_active())
                                {
                                    ::fast_io::perrln("uwvm: [error] WASI Threads cannot activate the selected hardware MTE mapping; no Guest was launched.");
                                    return fail(completion::rejected,EINVAL,termination::preparation_rejected);
                                }
                            }
                        }
                    }
                    if(!bind_journal_origin(*state_,*work->retained_world(),entry_,parameters_))
                    {
                        ::fast_io::perrln("uwvm: [error] Thread history origin does not match the validated source, entry or initial state; no Guest instruction or live I/O was dispatched.");
                        return fail(completion::rejected,125,termination::replay_refused);
                    }
                }
                if(actual.cancellation().stop_requested()) { return {completion::cancelled, {}, 0}; }
                auto const launched{actual.launch(::std::move(work), argument_)};
                if(launched.tid < 0) { return fail(completion::native_failure, launched.native_error != 0 ? launched.native_error : EINVAL); }
                return {completion::launched, launched, 0};
            }
        };

    public:
        [[nodiscard]] static threaded_management_exclusion try_management(threaded_management_kind purpose) noexcept
        {
            if(current_host_ != nullptr || get_runtime_execution_entry_depth() != 0u ||
               get_runtime_state_publication_depth() != 0u || get_runtime_compilation_metadata_callback_depth() != 0u)
            { return {}; }
            ::std::unique_lock registry{control_owner().registry, ::std::try_to_lock};
            if(!registry.owns_lock()) { return {}; }
            auto const& state{control_owner().profile};
            if(state && (!state->joined || state->native || state->admitted ||
                (state->requires_reset && purpose == threaded_management_kind::incremental_mutation))) { return {}; }
            auto maintenance{g_runtime.execution_domain.try_maintenance_transition()};
            if(!maintenance) { return {}; }
            source::owner observed;
            {
                runtime_state_publication_guard publication{::std::try_to_lock};
                if(!publication) { return {}; }
                observed = ::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin();
                if(observed && !source::has_canonical_owner(observed)) { return {}; }
            }
            auto completion{observed ? observed->destruction_completion() :
                ::uwvm2::uwvm::runtime::full::full_source_destruction_completion::owner{}};
            return threaded_management_exclusion{::std::move(registry), observed, ::std::move(completion),
                current_runtime_generation(), purpose, state && state->requires_reset};
        }
        [[nodiscard]] static threaded_management_exclusion try_management_under_publication(threaded_management_kind purpose) noexcept
        {
            // Actual same-thread Runtime guard owns the real atomic publication
            // lock and its private TLS depth. No external bool/label is accepted.
            if(current_host_ != nullptr || get_runtime_state_publication_depth() == 0u ||
               get_runtime_execution_entry_depth() != 0u || get_runtime_compilation_metadata_callback_depth() != 0u)
            { return {}; }
            ::std::unique_lock registry{control_owner().registry, ::std::try_to_lock};
            if(!registry.owns_lock()) { return {}; }
            auto const& state{control_owner().profile};
            if(state && (!state->joined || state->native || state->admitted ||
                (state->requires_reset && purpose == threaded_management_kind::incremental_mutation))) { return {}; }
            auto const observed{::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin()};
            if(observed && !source::has_canonical_owner(observed)) { return {}; }
            auto completion{observed ? observed->destruction_completion() :
                ::uwvm2::uwvm::runtime::full::full_source_destruction_completion::owner{}};
            return threaded_management_exclusion{::std::move(registry), observed, ::std::move(completion),
                current_runtime_generation(), purpose, state && state->requires_reset};
        }
        [[nodiscard]] static bool recheck_management(threaded_management_exclusion const& actual,
            threaded_management_kind purpose) noexcept
        {
            // Caller already holds its original exclusion before pause/Host
            // admission/publication. No lock, wait, allocation or join here.
            if(!actual || actual.actual_mutex_ != &control_owner().registry || actual.purpose_ != purpose ||
               actual.observed_generation_ != current_runtime_generation()) { return false; }
            auto const& state{control_owner().profile};
            if(state && (!state->joined || state->native || state->admitted ||
                (state->requires_reset && purpose == threaded_management_kind::incremental_mutation))) { return false; }
            auto const selected{::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin()};
            if(!selected) { return actual.observed_source_.expired() && !actual.source_completion_; }
            return !actual.observed_source_.owner_before(selected) && !selected.owner_before(actual.observed_source_) &&
                source::has_canonical_owner(selected);
        }
        [[nodiscard]] static bool source_destruction_completed(threaded_management_exclusion const& actual) noexcept
        {
            if(!actual || actual.actual_mutex_ != &control_owner().registry ||
               actual.purpose_ != threaded_management_kind::full_source_reset) { return false; }
            auto const& state{control_owner().profile};
            if(state && (!state->joined || state->native || state->admitted)) { return false; }
            bool const original{!state || !state->requires_reset ||
                (state->old_source_completion && state->old_source_completion->completed())};
            return original && (!actual.requires_reset_ || !actual.source_completion_ || actual.source_completion_->completed());
        }
        [[nodiscard]] static threaded_policy_status policy(::uwvm2::runtime::wasm_threads::threaded_profile_limits& out) noexcept
        {
            ::std::unique_lock registry{control_owner().registry, ::std::try_to_lock};
            if(!registry.owns_lock()) { return threaded_policy_status::busy; }
            out = control_owner().configured;
            return out.max_active_children != 0u ? threaded_policy_status::configured : threaded_policy_status::disabled;
        }
        [[nodiscard]] static bool builtin_requested() noexcept
        { return control_owner().builtin_requested.load(::std::memory_order_acquire); }
        [[nodiscard]] static bool configure_journal(
            ::uwvm2::runtime::wasm_threads::threaded_execution_history const* replay,
            ::std::span<::uwvm2::runtime::wasm_threads::threaded_environment_sample const> environments={},bool hold_entry=false) noexcept
        {
            namespace t=::uwvm2::runtime::wasm_threads;
            if(native_process_!=::getpid() || (replay&&(!t::valid_execution_history(*replay)||replay->retained_native)))return false;
            ::std::unique_lock lock{control_owner().registry,::std::try_to_lock};
            if(!lock.owns_lock()||control_owner().profile||
               control_owner().debug_control.kind!=t::threaded_debug_control::cooperative_all_stop)return false;
            try
            {
                auto value=replay?::uwvm2::utils::container::make_shared<t::threaded_execution_journal>(*replay):
                    ::uwvm2::utils::container::make_shared<t::threaded_execution_journal>(hold_entry);
                ::uwvm2::utils::container::std_vector<t::threaded_environment_sample> samples{environments.begin(),environments.end()};
                return g_runtime.execution_domain.with_quiescent_configuration([&]() noexcept
                    {control_owner().journal=::std::move(value);control_owner().replay_environments=::std::move(samples);});
            }
            catch(...){return false;}
        }
        [[nodiscard]] static auto journal_status() noexcept -> ::uwvm2::runtime::wasm_threads::journal_observation
        {
            if(native_process_!=::getpid())return {};
            ::std::lock_guard lock{control_owner().registry};auto const& value=control_owner().journal;
            return value?value->observe(): ::uwvm2::runtime::wasm_threads::journal_observation{};
        }
        [[nodiscard]] static bool terminal_decision(::std::uint32_t& code,unsigned& kind) noexcept
        {
            code=0u;kind=static_cast<unsigned>(::uwvm2::runtime::wasm_threads::termination_kind::running);
            if(native_process_!=::getpid())return false;
            ::std::shared_ptr<profile_state> state;
            {::std::lock_guard lock{control_owner().registry};state=control_owner().profile;}
            if(!state||!state->native)return false;
            auto const outcome=state->native->read_outcome();
            if(outcome.kind==::uwvm2::runtime::wasm_threads::termination_kind::running)return false;
            code=outcome.code;kind=static_cast<unsigned>(outcome.kind);return true;
        }
        [[nodiscard]] static bool unresolved_native_imports() noexcept
        {
            if(native_process_!=::getpid())return false;
            ::std::shared_ptr<::uwvm2::runtime::wasm_threads::threaded_execution_journal> journal;
            {::std::lock_guard lock{control_owner().registry};journal=control_owner().journal;}
            return journal&&journal->has_unresolved_native_imports();
        }
        [[nodiscard]] static bool capture_journal(::uwvm2::runtime::wasm_threads::threaded_debug_all_stop const* stop,
            ::uwvm2::runtime::wasm_threads::threaded_execution_history& output) noexcept
        {
            namespace t=::uwvm2::runtime::wasm_threads;
            if(native_process_!=::getpid())return false;
            ::std::shared_ptr<profile_state> state;::std::shared_ptr<t::threaded_execution_journal> journal;
            {::std::lock_guard lock{control_owner().registry};state=control_owner().profile;journal=control_owner().journal;}
            if(!state||!journal)return false;
            ::std::unique_lock retirement{state->retirement,::std::try_to_lock};if(!retirement.owns_lock())return false;
            bool pending{};
            if(stop)
            {
                if(stop->native_process_!=static_cast<::std::uint64_t>(::getpid())||stop->group_.lock()!=state->native||
                   stop->admission_.lock()!=state->admitted)return false;
                ::std::size_t needed{};
                if(stop->read({},needed)!=t::debug_stop_status::insufficient_capacity||!needed)return false;
                try
                {
                    ::uwvm2::utils::container::std_vector<t::debug_stopped_worker> rows(needed);
                    if(stop->read(rows,needed)!=t::debug_stop_status::stopped)return false;
                    for(auto const& row:rows)pending|=row.retained_host;
                }
                catch(...){return false;}
            }
            else if(!state->joined||(!state->result->returned.load(::std::memory_order_acquire)&&
                state->outcome.kind!=t::termination_kind::proc_exit&&state->outcome.kind!=t::termination_kind::trap))return false;
            t::threaded_execution_history captured;
            if(!journal->capture(captured,!stop,pending))return false;
            if(captured.retained_native)
            {
                ::uwvm2::utils::container::std_vector<::std::byte> bytes;
                if(!t::encode_execution_history(captured,bytes))return false;
                ::fast_io::sha256_context hash{};hash.update(bytes.data(),bytes.data()+bytes.size());hash.do_final();
                ::std::array<::std::byte,32> digest{};hash.digest_to_byte_ptr(digest.data());
                ::std::lock_guard lock{control_owner().registry};
                if(control_owner().profile!=state||control_owner().journal!=journal)return false;
                control_owner().retained_journal_group=state->native;control_owner().retained_journal_serial=stop->serial_;
                control_owner().retained_journal_digest=digest;
            }
            output=::std::move(captured);return true;
        }
        [[nodiscard]] static bool capture_journal_archive(::uwvm2::runtime::wasm_threads::threaded_debug_all_stop const* stop,
            ::uwvm2::utils::container::std_vector<::std::byte>& output) noexcept
        {
            namespace t=::uwvm2::runtime::wasm_threads;
            try
            {
                ::std::shared_ptr<profile_state> state;
                ::std::shared_ptr<t::threaded_execution_journal> journal;
                {::std::lock_guard lock{control_owner().registry};state=control_owner().profile;journal=control_owner().journal;}
                if(!state||!journal)return false;
                t::threaded_execution_history history;if(!capture_journal(stop,history))return false;
                ::std::unique_lock retirement{state->retirement,::std::try_to_lock};if(!retirement.owns_lock())return false;
                {::std::lock_guard lock{control_owner().registry};
                    if(control_owner().profile!=state||control_owner().journal!=journal||state->generation!=current_runtime_generation())return false;}
                if(stop)
                {
                    if(stop->group_.lock()!=state->native||stop->admission_.lock()!=state->admitted)return false;
                    ::std::size_t count{};if(stop->read({},count)!=t::debug_stop_status::insufficient_capacity||!count)return false;
                }
                else if(!state->joined)return false;
                if(!history.retained_native && !journal->replaying() &&
                   !capture_environment_sample(*state,t::threaded_history_label(history),history.events.size()))return false;
                return t::encode_threaded_snapshot(history,state->journal_sources,output,state->journal_environments);
            }
            catch(...){return false;}
        }
        [[nodiscard]] static bool reattach_journal(::uwvm2::runtime::wasm_threads::threaded_debug_all_stop const& stop,
            ::uwvm2::runtime::wasm_threads::threaded_execution_history const& history) noexcept
        {
            namespace t=::uwvm2::runtime::wasm_threads;
            if(native_process_!=::getpid()||!history.retained_native||!t::valid_execution_history(history))return false;
            ::uwvm2::utils::container::std_vector<::std::byte> bytes;if(!t::encode_execution_history(history,bytes))return false;
            ::fast_io::sha256_context hash{};hash.update(bytes.data(),bytes.data()+bytes.size());hash.do_final();
            ::std::array<::std::byte,32> digest{};hash.digest_to_byte_ptr(digest.data());
            ::std::shared_ptr<profile_state> state;
            {
                ::std::lock_guard lock{control_owner().registry};state=control_owner().profile;
                if(!state||stop.native_process_!=static_cast<::std::uint64_t>(::getpid())||!stop.serial_||
                   control_owner().retained_journal_serial!=stop.serial_||control_owner().retained_journal_digest!=digest||
                   control_owner().retained_journal_group.lock()!=stop.group_.lock())return false;
            }
            ::std::unique_lock retirement{state->retirement,::std::try_to_lock};if(!retirement.owns_lock())return false;
            if(stop.group_.lock()!=state->native||stop.admission_.lock()!=state->admitted)return false;
            ::std::size_t needed{};
            return stop.read({},needed)==t::debug_stop_status::insufficient_capacity&&needed!=0u;
        }
        [[nodiscard]] static bool continue_recorded_interval(::uwvm2::runtime::wasm_threads::threaded_debug_all_stop const& stop) noexcept
        {
            namespace t=::uwvm2::runtime::wasm_threads;if(native_process_!=::getpid())return false;
            ::std::shared_ptr<profile_state> state;::std::shared_ptr<t::threaded_execution_journal> journal;
            {::std::lock_guard lock{control_owner().registry};state=control_owner().profile;journal=control_owner().journal;}
            if(!state||!journal)return false;
            ::std::unique_lock retirement{state->retirement,::std::try_to_lock};if(!retirement.owns_lock())return false;
            if(stop.native_process_!=static_cast<::std::uint64_t>(::getpid())||stop.group_.lock()!=state->native||stop.admission_.lock()!=state->admitted)return false;
            ::std::size_t count{};
            if(stop.read({},count)!=t::debug_stop_status::insufficient_capacity||!count)return false;
            return journal->continue_recorded_interval();
        }
        [[nodiscard]] static bool configure_debug_observation(::uwvm2::runtime::wasm_threads::threaded_debug_observation purpose) noexcept
        {
            using observation=::uwvm2::runtime::wasm_threads::threaded_debug_observation;
            if(purpose!=observation::none&&purpose!=observation::readonly_metadata)return false;
            ::std::unique_lock registry{control_owner().registry,::std::try_to_lock};
            if(!registry.owns_lock()||control_owner().profile||control_owner().blocked_generation==current_runtime_generation())return false;
            if(purpose==observation::none&&control_owner().debug_control.kind!=::uwvm2::runtime::wasm_threads::threaded_debug_control::none)return false;
            return g_runtime.execution_domain.with_quiescent_configuration([&]() noexcept {control_owner().observation=purpose;});
        }
        [[nodiscard]] static bool configure_debug_control(::uwvm2::runtime::wasm_threads::threaded_debug_control_limits const& purpose) noexcept
        {
            using policy=::uwvm2::runtime::wasm_threads::threaded_debug_control;
            if(purpose.kind!=policy::none&&purpose.kind!=policy::cooperative_all_stop)return false;
            if(purpose.kind==policy::none?purpose.maximum_control_storage_bytes!=0u:
                purpose.maximum_control_storage_bytes==0u||purpose.maximum_control_storage_bytes==SIZE_MAX)return false;
            ::std::unique_lock registry{control_owner().registry,::std::try_to_lock};
            if(!registry.owns_lock()||control_owner().profile||control_owner().blocked_generation==current_runtime_generation())return false;
            if(purpose.kind!=policy::none&&control_owner().observation!=::uwvm2::runtime::wasm_threads::threaded_debug_observation::readonly_metadata)return false;
            return g_runtime.execution_domain.with_quiescent_configuration([&]() noexcept {control_owner().debug_control=purpose;});
        }
        // CLI specifies per-child storage. Derive the aggregate from actual
        // requested object sizes, without changing explicit native total caps.
        [[nodiscard]] static bool complete_cli_storage_limits(
            ::uwvm2::runtime::wasm_threads::threaded_profile_limits& limits) noexcept
        {
            if(!group::limits_admitted(limits)) { return false; }
            if(limits.max_active_children==SIZE_MAX) { limits.max_live_storage_bytes=SIZE_MAX;return true; }
            auto const slots{limits.max_active_children+1u};
            ::std::size_t total{};
            auto add=[&](::std::size_t n) noexcept { if(n>=SIZE_MAX-total) { return false; } total+=n;return true; };
            auto repeated=[&](::std::size_t count,::std::size_t each) noexcept
            { return each!=0u && count<=SIZE_MAX/each && add(count*each); };
            // Every live Guest owns its full reservation. Every possible queue
            // entry owns one concrete submission; only initial-main has ABI
            // parameters, bounded by the real value-stack cap.
            if(!repeated(slots,limits.max_child_storage_bytes) ||
               !repeated(slots,sizeof(group::slot)+2u*sizeof(void*)) || !repeated(slots,sizeof(group::compiler_slot)+2u*sizeof(void*)) ||
               !repeated(slots,sizeof(concrete_compile_work)) || !add(sizeof(group)) ||
               !add(sizeof(profile_state)+sizeof(main_result)+sizeof(admission)) ||
               !add(limits.max_value_stack_bytes) || !add(limits.max_value_stack_bytes)) { return false; }
            limits.max_live_storage_bytes=total;
            return true;
        }
        [[nodiscard]] static bool configure(::uwvm2::runtime::wasm_threads::threaded_profile_limits const& limits,
            bool cli_storage_derived=false) noexcept
        {
            if(!group::limits_admitted(limits)) { return false; }
            ::std::unique_lock registry{control_owner().registry, ::std::try_to_lock};
            if(!registry.owns_lock()) { return false; }
            if(control_owner().profile || control_owner().blocked_generation == current_runtime_generation()) { return false; }
            bool const accepted{g_runtime.execution_domain.with_quiescent_configuration([&]() noexcept
            { control_owner().configured=limits;control_owner().cli_storage_derived=cli_storage_derived; })};
            if(accepted) { control_owner().builtin_requested.store(true, ::std::memory_order_release); }
            return accepted;
        }
        [[nodiscard]] static threaded_main_status start_main(::uwvm2::utils::container::u8string_view name,
            ::std::size_t entry, ::std::span<::std::byte const> parameters, ::std::size_t result_bytes) noexcept
        {
            ::std::shared_ptr<profile_state> state;
            group::SpawnReservation reservation;
            ::uwvm2::utils::control::wasithread::policy::reservation thread_authority;
            {
                ::std::unique_lock registry{control_owner().registry, ::std::try_to_lock};
                if(!registry.owns_lock()) { return threaded_main_status::rejected; }
                if(control_owner().configured.max_active_children == 0u) { return threaded_main_status::disabled; }
                if(control_owner().profile || control_owner().blocked_generation == current_runtime_generation() || current_host_ != nullptr ||
                   parameters.size() > control_owner().configured.max_value_stack_bytes || result_bytes > control_owner().configured.max_value_stack_bytes)
                { return threaded_main_status::rejected; }
#if defined(UWVM_RUNTIME_LLVM_JIT)
                // Ordinary cold compilation can enqueue authenticated writes.
                // Registry ownership excludes its next producer. Retire those
                // real generation leases before requesting exclusive admission;
                // the writer needs no registry/publication lock to finish.
                // Never synchronously drain from an executing native callback.
                if(get_runtime_execution_entry_depth()!=0u || get_runtime_state_publication_depth()!=0u)
                { return threaded_main_status::rejected; }
                ::uwvm2::runtime::llvm_jit_cache::flush_async_store_objects();
#endif
                auto maintenance{g_runtime.execution_domain.try_maintenance_transition()};
                if(!maintenance) { return threaded_main_status::rejected; }
                if(control_owner().cli_storage_derived)
                {
                    auto completed{control_owner().configured};
                    if(!complete_cli_storage_limits(completed)) { return threaded_main_status::rejected; }
                    auto add=[&](::std::size_t bytes) noexcept
                    { if(completed.max_live_storage_bytes==SIZE_MAX)return true;
                      if(bytes>=SIZE_MAX-completed.max_live_storage_bytes) { return false; }
                      completed.max_live_storage_bytes+=bytes;return true; };
                    // CLI options may be parsed in either order. Bind optional
                    // owners only now, under the real cold registry/maintenance
                    // gate. Their actual allocations remain charged by Group.
                    auto const& control_limits{control_owner().debug_control};
                    if(control_limits.kind!=::uwvm2::runtime::wasm_threads::threaded_debug_control::none &&
                       !add(control_limits.maximum_control_storage_bytes)) { return threaded_main_status::rejected; }
                    auto const& signal_limits{control_owner().signal_policy};
                    if(signal_limits.read_fd!=0u)
                    {
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
                        if(!add(signal_limits.maximum_pipe_bytes)||!add(signal_owner::bookkeeping_bytes))
                        { return threaded_main_status::rejected; }
#else
                        return threaded_main_status::rejected;
#endif
                    }
                    if(!group::limits_admitted(completed)) { return threaded_main_status::rejected; }
                    control_owner().configured=completed;
                }
                try
                {
                    state = ::uwvm2::utils::container::make_shared<profile_state>();
                    state->gate = g_runtime.execution_domain.suspend_public_admission(maintenance);
                    if(!state->gate) { throw ::std::bad_alloc{}; }
                    // Public admission is really closed and the current native
                    // generation has no active ordinary reader/compiler. Only
                    // now may startup read source/record/body state without racing
                    // ordinary compilation. Other mutation holds this same cold
                    // exclusion or fails its nonwaiting commit recheck.
                    auto const owner{::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin()};
                    if(!source::has_canonical_owner(owner)) { throw ::std::bad_alloc{}; }
                    auto const* parent{owner->initialized_main_module()};
                    auto const id{owner->bound_initialized_main_module_id()};
                    auto const generation{current_runtime_generation()};
                    auto const validation{owner->actual_full_validation_epoch()};
                    namespace mode = ::uwvm2::uwvm::runtime::runtime_mode;
                    if(parent == nullptr || id >= g_runtime.modules.size() || validation != generation ||
                       mode::global_runtime_mode != mode::runtime_mode_t::full_compile ||
                       g_runtime.modules.index_unchecked(id).runtime_module != parent ||
                       g_runtime.modules.index_unchecked(id).module_name != name ||
                       owner->actual_validated_file(id, validation, parent) != &owner->file() ||
                       (!parameters.empty() && parameters.data() == nullptr)) { throw ::std::bad_alloc{}; }
                    namespace i1t=::uwvm2::utils::control::wasithread;
                    auto main_authority=i1t::runtime_policy().reserve({name.data(),name.size()},false);
                    if(!main_authority.held)
                    {
                        ::fast_io::perrln("uwvm: [error] wasithread I1T main plus child thread capacity rejects the initial main.");
                        throw ::std::bad_alloc{};
                    }
                    thread_authority=::std::move(main_authority.held);
#if defined(UWVM_RUNTIME_LLVM_JIT)
                    // The first profile has no borrowed ordinary debugger or
                    // checkpoint execution state. Native transactions must use
                    // their real exclusion and a complete Source reset first.
                    if(g_runtime.debug_pause_control || g_runtime.checkpoint_profile ||
                       g_runtime.checkpoint_host_enabled.load(::std::memory_order_acquire)) { throw ::std::bad_alloc{}; }
#endif
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
                    if(::uwvm2::uwvm::wasm::storage::preload_expose_wasip1_host_api) { throw ::std::bad_alloc{}; }
#endif
                    auto const imported{parent->imported_function_vec_storage.size()};
                    if(entry < imported || entry - imported >= parent->local_defined_function_vec_storage.size()) { throw ::std::bad_alloc{}; }
                    auto const* entry_type{parent->local_defined_function_vec_storage.index_unchecked(entry - imported).function_type_ptr};
                    ::std::size_t parameter_bytes{}, output_bytes{};
                    if(!entry_type || !numeric_abi(*entry_type, *entry_type, parameter_bytes, output_bytes) ||
                       parameters.size() != parameter_bytes || result_bytes != output_bytes) { throw ::std::bad_alloc{}; }
                    state->result = ::uwvm2::utils::container::make_shared<main_result>(); state->result->bytes.resize(result_bytes);
                    if(state->result->bytes.capacity()>control_owner().configured.max_value_stack_bytes) { throw ::std::bad_alloc{}; }
                    state->selected = backend::integer;
#if defined(UWVM_RUNTIME_LLVM_JIT)
                    if(runtime_compiler_requests_llvm_jit_translation()) { state->selected = backend::llvm; }
#endif
                    state->generation = generation;
                    state->old_source_completion = owner->destruction_completion();
                    if(!state->old_source_completion || state->old_source_completion->completed()) { throw ::std::bad_alloc{}; }
                    auto actual_generation{state->gate.try_enter()};
                    if(!actual_generation) { throw ::std::bad_alloc{}; }
                    auto admitted{::std::shared_ptr<admission>{::uwvm2::utils::container::adopt_shared(::uwvm2::utils::allocator::fast_io_strict::object<admission>([&](admission* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) admission{owner, *parent, id,
                        generation, validation, control_owner().configured, ::std::move(actual_generation), control_owner().observation,
                        control_owner().debug_control.kind==::uwvm2::runtime::wasm_threads::threaded_debug_control::cooperative_all_stop}; }))}};
                    admitted->canonical_owner_ = admitted; state->admitted = ::std::move(admitted);
                    if(control_owner().signal_policy.read_fd!=0u)
                    {
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
                        auto const& selected_signal{control_owner().signal_policy};
                        if(!signal_policy_admitted(selected_signal)||!signal_source_census(*state->admitted)||
                           selected_signal.maximum_pipe_bytes>SIZE_MAX-signal_owner::bookkeeping_bytes||
                           selected_signal.maximum_pipe_bytes+signal_owner::bookkeeping_bytes>control_owner().configured.max_child_storage_bytes||
                           selected_signal.maximum_pipe_bytes+signal_owner::bookkeeping_bytes>control_owner().configured.max_live_storage_bytes)
                        { throw ::std::bad_alloc{}; }
                        // Real Source/admission and all owner/sender/control/map
                        // storage precede pipe creation and Guest/compiler launch.
                        state->signal_sender=::std::shared_ptr<threaded_signal_sender const>{::uwvm2::utils::container::adopt_shared(::uwvm2::utils::allocator::fast_io_strict::object<threaded_signal_sender>([&](threaded_signal_sender* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) threaded_signal_sender{state->admitted}; }))};
                        state->signal=::std::shared_ptr<signal_owner>{::uwvm2::utils::container::adopt_shared(::uwvm2::utils::allocator::fast_io_strict::object<signal_owner>([&](signal_owner* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) signal_owner{state->admitted,
                            resolve_wasip1_env_for_runtime_module_id(id),selected_signal}; }))};
                        auto const installed{state->signal->install(selected_signal)};
                        if(installed){state->signal->note(installed);throw ::std::bad_alloc{};}
#else
                        throw ::std::bad_alloc{};
#endif
                    }
                    state->native = ::std::shared_ptr<group>{::uwvm2::utils::container::adopt_shared(::uwvm2::utils::allocator::fast_io_strict::object<group>([&](group* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) group{state->admitted,control_owner().debug_control}; }))};
                    state->native->canonical_owner_ = state->native;
                    state->native->execution_journal_=control_owner().journal;
                    state->journal_environments=control_owner().replay_environments;
                    if(state->native->execution_journal_ && !state->native->execution_journal_->bind_failure_owner(state->native,
                        [](void* pointer) noexcept
                        {
                            auto& actual=*static_cast<group*>(pointer);
                            actual.request_stop({::uwvm2::runtime::wasm_threads::termination_kind::replay_refused,125u,0u,0u,0u});
                        }))throw ::std::bad_alloc{};
                    if(state->native->initialization_error_ != 0) { throw ::std::bad_alloc{}; }
                    auto control_bytes{sizeof(profile_state) + sizeof(main_result) + sizeof(admission)};
                    if(state->result->bytes.capacity() > SIZE_MAX - control_bytes) { throw ::std::bad_alloc{}; }
                    control_bytes += state->result->bytes.capacity();
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
                    if(state->signal)
                    {
                        auto const bytes{state->signal->facts_.pipe_capacity+signal_owner::bookkeeping_bytes};
                        if(bytes>SIZE_MAX-control_bytes)throw ::std::bad_alloc{};
                        control_bytes+=static_cast<::std::size_t>(bytes);
                    }
#endif
                    if(!state->native->charge_runtime_control_storage(control_bytes)) { throw ::std::bad_alloc{}; }
                    reservation = state->native->reserve_initial_main();
                    if(!reservation) { throw ::std::bad_alloc{}; }
                    control_owner().profile = state; // reachable BEFORE any cold compiler/provider callback
                    control_owner().stop_target.store(state->native, ::std::memory_order_release);
                }
                catch(...)
                {
                    reservation.cancel();
                    if(state)
                    {
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
                        if(state->signal)
                        {
                            (void)state->signal->retire();
                            threaded_signal_report actual{};
                            if(state->signal->snapshot(actual)!=0||!actual.retired)::std::terminate();
                            control_owner().last_signal_report=actual;
                            state->signal.reset();state->signal_sender.reset();
                        }
#endif
                      state->native.reset(); state->admitted.reset();
                      (void)g_runtime.execution_domain.reopen_public_admission(maintenance, state->gate);
                    }
                    return threaded_main_status::rejected;
                }
            }
            // The true preparing slot/provider lease remains visible while the
            // host allocates a finite submission. The only code builder runs on
            // the real Group-owned compiler pthread, never a Guest parent stack.
            auto reject{[&]() noexcept
            {
                state->native->request_stop({::uwvm2::runtime::wasm_threads::termination_kind::native_failure, 1u, 0u, entry, 0u});
                reservation.cancel();
                (void)retire_impl(::std::chrono::steady_clock::now(), threaded_management_kind::stop_only, true);
                return threaded_main_status::rejected;
            }};
            if(state->native->start_compiler_worker() != 0) { return reject(); }
            ::uwvm2::utils::container::std_unique_ptr<concrete_compile_work> job;
            try { job = ::uwvm2::utils::container::make_unique<concrete_compile_work>(state, true, state->admitted->source_module_id(), entry, 0u, parameters, ::std::move(thread_authority)); }
            catch(...) { return reject(); }
            {
                // Publish the actual ticket before a concurrent manager can
                // report native retirement. No wait/compile/callback under Root.
                ::std::unique_lock registry{control_owner().registry, ::std::try_to_lock};
                if(!registry.owns_lock()) { job.reset(); return reject(); }
                if(control_owner().profile != state || state->joined) { return threaded_main_status::rejected; }
                auto submitted{state->native->submit_compilation(::std::move(reservation), ::std::move(job))};
                if(!submitted.ticket)
                {
                    registry.unlock();
                    return reject();
                }
                state->main_preparation = ::std::move(submitted.ticket);
            }
            // Running includes admitted asynchronous preparation. Its native
            // failure/stop/result is observed only by wait/result, never restart.
            return threaded_main_status::running;
        }

        [[nodiscard]] static threaded_retirement retire_impl(::std::chrono::steady_clock::time_point deadline,
            threaded_management_kind purpose, bool deliver_stop) noexcept
        {
            if(current_host_ != nullptr) { return {}; } // never join the native caller's own Group
            ::std::shared_ptr<profile_state> state;
            { ::std::unique_lock registry{control_owner().registry, ::std::try_to_lock};
              if(!registry.owns_lock()) { return {false, false, 0u, 0u, EBUSY}; }
              state = control_owner().profile; }
            if(!state) { return {true, false, 0u, 0u, 0}; }
            ::std::unique_lock retirement{state->retirement, ::std::try_to_lock};
            if(!retirement.owns_lock())
            {
                ::std::unique_lock registry{control_owner().registry, ::std::try_to_lock};
                if(!registry.owns_lock()) { return {false, false, 0u, 0u, EBUSY}; }
                bool const reset{state->native ?
                    (state->native->ever_launched_.load(::std::memory_order_acquire) ||
                     state->native->shared_effects_started_.load(::std::memory_order_acquire)) : state->requires_reset};
                return {false, reset, 0u, 0u, EBUSY};
            }
            if(state->joined)
            {
                if(state->requires_reset || state->ordinary_reopened) { return {true, state->requires_reset, 0u, 0u, state->signal_retirement_error}; }
                // Keep the same registry -> maintenance order as start,
                // configuration and source reset. Waiting for the registry
                // while owning maintenance would invert reset's real locks.
                ::std::lock_guard registry{control_owner().registry};
                auto maintenance{g_runtime.execution_domain.try_maintenance_transition()};
                if(!maintenance) { return {true, false, 0u, 0u, EBUSY}; }
                if(control_owner().profile.get() != state.get() ||
                   !g_runtime.execution_domain.reopen_public_admission(maintenance, state->gate))
                { return {true, false, 0u, 0u, EBUSY}; }
                state->ordinary_reopened = true;
                return {true, false, 0u, 0u, state->signal_retirement_error};
            }
            if(deliver_stop) { state->native->request_stop({::uwvm2::runtime::wasm_threads::termination_kind::cancelled, 0u, 0u, 0u, 0u}); }
            {
                ::std::unique_lock registry{control_owner().registry, ::std::try_to_lock};
                if(!registry.owns_lock()) { return {false, false, 0u, 0u, EBUSY}; }
                // Abandoning a live compiler ticket requests actual Group stop.
                // A normal wait must therefore retain it until real main/Host
                // termination has already closed this Group. Compiler work stays
                // owned until its own body/TLS and native pthread physically join.
                if(state->native->stop_requested_.load(::std::memory_order_acquire)) { state->main_preparation.reset(); }
            }
            auto const joined{state->native->join_until(deadline)};
            bool const reset{state->native->ever_launched_.load(::std::memory_order_acquire) ||
                state->native->shared_effects_started_.load(::std::memory_order_acquire)};
            if(!joined.physically_joined)
            { return {false, reset, joined.preparing, joined.native_pending, joined.native_error,
                joined.compiler_tickets_pending, joined.native_cleanup_pending}; }
            // Native workers have physically joined outside both locks.
            // Re-enter the canonical registry -> maintenance order before
            // inspecting ownership or publishing retirement.
            ::std::lock_guard registry{control_owner().registry};
            auto maintenance{g_runtime.execution_domain.try_maintenance_transition()};
            if(!maintenance) { return {false, reset, 0u, 0u, EBUSY}; }
            if(control_owner().profile.get() != state.get()) { return {}; }
            state->outcome = state->native->read_outcome(); state->requires_reset = reset;
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
            if(state->signal)
            {
                auto const closed{state->signal->retire()};threaded_signal_report actual{};
                if(state->signal->snapshot(actual)!=0||!actual.retired)
                { return {false,reset,0u,0u,closed?closed:EBUSY,0u,1u}; }
                control_owner().last_signal_report=actual;state->signal_retirement_error=closed;
            }
#endif
            // True join destroyed every slot's engine/World. Release the real
            // provider-generation pins BEFORE any ordinary domain drain/reset.
            auto stop_target{control_owner().stop_target.exchange({}, ::std::memory_order_acq_rel)};
            stop_target.reset(); state->native.reset(); state->admitted.reset(); state->joined = true;
            if(reset) { control_owner().blocked_generation = state->generation; }
            else if(!g_runtime.execution_domain.reopen_public_admission(maintenance, state->gate))
            { return {true, false, 0u, 0u, EBUSY}; }
            else { state->ordinary_reopened = true; }
            (void)purpose; // caller MUST inspect permits(purpose); no mutation ACK is synthesized
            return {true, reset, 0u, 0u, state->signal_retirement_error};
        }

        [[nodiscard]] static threaded_retirement retire(::std::chrono::steady_clock::time_point deadline,
            threaded_management_kind purpose) noexcept
        { return retire_impl(deadline, purpose, true); }
        [[nodiscard]] static threaded_retirement wait(::std::chrono::steady_clock::time_point deadline) noexcept
        { return retire_impl(deadline, threaded_management_kind::stop_only, false); }
        static void request_stop() noexcept
        {
            // Independent typed cancellation route; no recursive cold-registry
            // lock when a genuine management exclusion is already held. This
            // owner grants neither execution nor source/provider permission.
            auto actual{control_owner().stop_target.load(::std::memory_order_acquire)};
            if(actual) { actual->request_stop({::uwvm2::runtime::wasm_threads::termination_kind::cancelled, 0u, 0u, 0u, 0u}); }
        }
        [[nodiscard]] static ::uwvm2::runtime::wasm_threads::threaded_debug_session debug_session() noexcept
        {
            // Reject inherited profile state before taking copied registry locks.
            if(native_process_!=::getpid())return {};
            // Short genuine Root registry read only; no Group lock, native
            // compile/wait/join, provider callback or Guest invocation here.
            ::std::unique_lock registry{control_owner().registry,::std::try_to_lock};
            if(!registry.owns_lock())return {};
            auto const& state{control_owner().profile};
            if(!state||state->joined||!state->native||!state->admitted||!current_source(*state->admitted))return {};
            return state->native->debug_session();
        }
        [[nodiscard]] static ::uwvm2::runtime::wasm_threads::threaded_debug_control_session debug_control_session() noexcept
        {
            // Reject inherited profile state before taking copied registry locks.
            if(native_process_!=::getpid())return {};
            // Independent short registry read. try_management intentionally
            // rejects live groups and is NOT used to mint control sessions.
            ::std::unique_lock registry{control_owner().registry,::std::try_to_lock};
            if(!registry.owns_lock()||current_host_!=nullptr)return {};
            auto const& state{control_owner().profile};
            if(!state||state->joined||!state->native||!state->admitted||!current_source(*state->admitted))return {};
            return state->native->debug_control_session();
        }
        [[nodiscard]] static threaded_main_status result(::std::span<::std::byte> bytes,
            ::std::uint32_t* code, unsigned* kind) noexcept
        {
            ::std::unique_lock registry{control_owner().registry, ::std::try_to_lock};
            if(!registry.owns_lock()) { return threaded_main_status::retirement_pending; }
            auto const& state{control_owner().profile};
            if(!state) { return threaded_main_status::disabled; }
            if(!state->joined) { return threaded_main_status::retirement_pending; }
            if(bytes.size() != state->result->bytes.size() || code == nullptr || kind == nullptr) { return threaded_main_status::rejected; }
            if(state->result->returned.load(::std::memory_order_acquire) && !bytes.empty())
            { ::std::memcpy(bytes.data(), state->result->bytes.data(), bytes.size()); }
            *code = state->outcome.code; *kind = static_cast<unsigned>(state->outcome.kind);
            return threaded_main_status::finished;
        }
        static void after_actual_source_reset() noexcept
        {
            ::std::shared_ptr<profile_state> retired;
            { ::std::lock_guard registry{control_owner().registry};
              auto const& state{control_owner().profile};
              if(!state || !state->joined || state->native || state->admitted || state->generation == current_runtime_generation() ||
                 (state->requires_reset && (!state->old_source_completion || !state->old_source_completion->completed()))) { return; }
              retired = ::std::move(control_owner().profile); control_owner().blocked_generation = 0u; }
            // Gate/control/result destruction outside all Runtime publication
            // locks. The old Source/NativeMemory has already been destroyed by
            // the genuine full reset; an epoch setter cannot invoke this API.
        }
        static void after_actual_source_reset(threaded_management_exclusion const& actual) noexcept
        {
            if(!source_destruction_completed(actual)) { return; }
            auto const& state{control_owner().profile};
            if(!state || !state->joined || state->generation == current_runtime_generation()) { return; }
            auto retired{::std::move(control_owner().profile)};
            control_owner().blocked_generation = 0u;
        }
        [[nodiscard]] static bool source_destruction_completed() noexcept
        {
            ::std::lock_guard registry{control_owner().registry};
            auto const& state{control_owner().profile};
            if(!state) { return true; }
            return state->joined && (!state->requires_reset ||
                (state->old_source_completion && state->old_source_completion->completed()));
        }
        static void process_shutdown() noexcept
        {
            // Runtime-global destructor invokes this BEFORE destroying its real
            // execution domain/providers/source. Compiler preparation may still
            // be pending; never claim a timeout/body-return as physical join.
            for(;;)
            {
                auto const retired{retire_impl(::std::chrono::steady_clock::now() + ::std::chrono::milliseconds{100},
                    threaded_management_kind::stop_only, true)};
                if(retired.physically_joined && retired.native_error == 0) { break; }
                if(retired.native_error != 0 && retired.native_error != EBUSY) { ::std::terminate(); }
            }
            ::std::shared_ptr<profile_state> retired;
            { ::std::lock_guard registry{control_owner().registry}; retired = ::std::move(control_owner().profile); }
        }

#include "uwvm_runtime_threaded_signal_host_lifecycle.h"
