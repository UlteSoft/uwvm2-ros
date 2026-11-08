// Licensed under APL-2.0 (see LICENSE).
#pragma once
// Included INSIDE runtime::lib after activation/source-memory/checkpoint helpers.
// Cold host-only export. A pause ticket never grants access to arbitrary native
// memory; every pointer below is resolved from the current sealed source registry.
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS)
extern "C++"
{
    class runtime_crash_dump_collector final
    {
        using domain = ::uwvm2::utils::thread::cooperative_pause_domain;
        using source_owner = ::uwvm2::uwvm::runtime::full::full_source_instance::owner;
        using lease_type = ::uwvm2::utils::thread::execution_domain::lease;
        using result = ::uwvm2::runtime::diagnostic::wasm_coredump::result;
        using error = ::uwvm2::runtime::diagnostic::wasm_coredump::error;
        using memory_view = ::uwvm2::runtime::diagnostic::wasm_coredump::memory_view;
        using region = ::uwvm2::runtime::diagnostic::wasm_coredump::region;
        using value = ::uwvm2::runtime::diagnostic::wasm_coredump::value;
        struct memory_context
        {
            native_memory_t const* owner{};
            lease_type const* lease{};
            domain* control{};
            ::std::uint64_t bytes{};
            region data{}, tags{};
        };
        struct output_context
        {
            lease_type const* lease{};
            domain* control{};
            void* context{};
            ::uwvm2::runtime::diagnostic::wasm_coredump::write_function write{};
        };
        template<typename A, typename B>
        [[nodiscard]] static bool same_owner(A const& a, B const& b) noexcept
        { return a && b && a.get()==b.get() && !a.owner_before(b) && !b.owner_before(a); }
        [[nodiscard]] static bool live(lease_type const* lease, domain* control) noexcept
        { return lease!=nullptr && !lease->stop_requested() && control!=nullptr && !control->is_closed(); }
        [[nodiscard]] static bool write(void* opaque,::std::span<::std::byte const> bytes) noexcept
        {
            auto const& state{*static_cast<output_context const*>(opaque)};
            return live(state.lease,state.control) && state.write(state.context,bytes);
        }
        [[nodiscard]] static bool read_memory(void* opaque,::std::uint64_t offset,::std::span<::std::byte> bytes) noexcept
        {
            auto const& state{*static_cast<memory_context const*>(opaque)};
            if(!live(state.lease,state.control)||state.owner==nullptr) return false;
            return with_native_preload_copy_access(*state.owner,[&](::std::byte* begin,::std::size_t length) noexcept
            {
                return length==state.bytes && live(state.lease,state.control) &&
                    ::uwvm2::extension::memtag::copy_debug_memory(state.owner->memtag_shadow,
                        {begin,length},offset,bytes.data(),bytes.size());
            });
        }
        [[nodiscard]] static bool read_tags(void* opaque,::std::uint64_t first,
            ::std::span<::uwvm2::runtime::diagnostic::wasm_coredump::tag_sample> tags) noexcept
        {
            auto const& state{*static_cast<memory_context const*>(opaque)};
            if(!live(state.lease,state.control)||state.owner==nullptr || first>UINT64_MAX/16u) return false;
            auto const offset{first*16u};
            if(offset>state.bytes || tags.size()>(state.bytes-offset)/16u) return false;
            return with_native_preload_copy_access(*state.owner,[&](::std::byte*,::std::size_t length) noexcept
            {
                if(length!=state.bytes || !live(state.lease,state.control)) return false;
                // The encoder requests at most 128 samples, but this callback
                // also supports arbitrary spans without allocating scratch.
                for(::std::size_t i{}; i!=tags.size(); ++i)
                {
                    ::std::uint8_t tag{};
                    auto const seen{::uwvm2::extension::memtag::copy_debug_tags(state.owner->memtag_shadow,
                        length,offset+static_cast<::std::uint64_t>(i)*16u,16u,{::std::addressof(tag),1u})};
                    if(seen.status!=::uwvm2::extension::memtag::debug_tag_status::available || seen.count!=1u) return false;
                    tags[i]={tag,true};
                }
                return true;
            });
        }
        template<bool Memory, bool Imported>
        [[nodiscard]] static auto const& records(runtime_module_storage_t const& module) noexcept
        {
            if constexpr(Memory && Imported) return module.imported_memory_vec_storage;
            else if constexpr(Memory) return module.local_defined_memory_vec_storage;
            else if constexpr(Imported) return module.imported_global_vec_storage;
            else return module.local_defined_global_vec_storage;
        }
        template<bool Memory, bool Imported,typename Record>
        [[nodiscard]] static bool locate(Record const* token,::std::size_t& module,::std::size_t& index) noexcept
        {
            if(token==nullptr) return false;
            // Compare membership before reading a chained import target. This
            // bound is the actual registry population, never a magic hop limit.
            for(::std::size_t m{};m!=g_runtime.modules.size();++m)
            {
                auto const& entries{records<Memory,Imported>(*g_runtime.modules.index_unchecked(m).runtime_module)};
                auto const address{reinterpret_cast<::std::uintptr_t>(token)};
                if(entries.empty() || entries.size()>PTRDIFF_MAX/sizeof(Record)) continue;
                auto const base{reinterpret_cast<::std::uintptr_t>(entries.data())};
                auto const bytes{entries.size()*sizeof(Record)};
                if(base>UINTPTR_MAX-bytes || address<base || address-base>=bytes || (address-base)%sizeof(Record)!=0u) continue;
                module=m;index=static_cast<::std::size_t>((address-base)/sizeof(Record));return true;
            }
            return false;
        }
        template<bool Memory>
        [[nodiscard]] static bool resolve_import(::std::size_t origin,::std::size_t imported,
            ::std::size_t population,::std::size_t& module,::std::size_t& index) noexcept
        {
            auto const* next{::std::addressof(records<Memory,true>(*g_runtime.modules.index_unchecked(origin).runtime_module).index_unchecked(imported))};
            for(::std::size_t hops{};hops!=population;++hops)
            {
                if(!locate<Memory,true>(next,module,index)) return false;
                auto const& actual{records<Memory,true>(*g_runtime.modules.index_unchecked(module).runtime_module).index_unchecked(index)};
                bool defined{},chained{};
                if constexpr(Memory)
                {
                    using kind=::uwvm2::uwvm::runtime::storage::imported_memory_storage_t::imported_memory_link_kind;
                    defined=actual.link_kind==kind::defined;chained=actual.link_kind==kind::imported;
                }
                else
                {
                    using kind=::uwvm2::uwvm::runtime::storage::imported_global_storage_t::imported_global_link_kind;
                    defined=actual.link_kind==kind::defined;chained=actual.link_kind==kind::imported;
                }
                if(defined) return locate<Memory,false>(actual.target.defined_ptr,module,index);
                if(!chained) return false; // No arbitrary native-provider callback.
                next=actual.target.imported_ptr;
            }
            return false; // Cycle exceeds the actual complete import population.
        }
        template<bool Memory>
        [[nodiscard]] static bool build_aliases(::std::vector<::std::vector<::std::uint32_t>>& maps,
            ::std::vector<::std::size_t> const& starts,::std::size_t population)
        {
            for(::std::size_t m{};m!=g_runtime.modules.size();++m)
            {
                auto const& module{*g_runtime.modules.index_unchecked(m).runtime_module};
                auto const imported{records<Memory,true>(module).size()};
                auto const defined{records<Memory,false>(module).size()};
                if(imported>SIZE_MAX-defined) return false;
                auto& map{maps[m]};map.resize(imported+defined);
                for(::std::size_t i{};i!=imported;++i)
                {
                    ::std::size_t owner{},local{};
                    if(!resolve_import<Memory>(m,i,population,owner,local) || owner>=starts.size() ||
                        starts[owner]>UINT32_MAX || local>UINT32_MAX-starts[owner]) return false;
                    map[i]=static_cast<::std::uint32_t>(starts[owner]+local);
                }
                for(::std::size_t i{};i!=defined;++i) map[imported+i]=static_cast<::std::uint32_t>(starts[m]+i);
            }
            return true;
        }
        [[nodiscard]] static bool local_count(llvm_jit_debug_activation_identity const& frame,::std::size_t& count,
            ::std::uint64_t expression_offset,::std::uint64_t& function_offset) noexcept
        {
            auto const& record{g_runtime.modules.index_unchecked(static_cast<::std::size_t>(frame.module))};
            auto const& module{*record.runtime_module};
            auto const imports{module.imported_function_vec_storage.size()};
            if(frame.function<imports || frame.function-imports>=module.local_defined_function_vec_storage.size()) return false;
            auto const local{static_cast<::std::size_t>(frame.function-imports)};
            auto const& function{module.local_defined_function_vec_storage.index_unchecked(local)};
            if(!function.function_type_ptr || !function.wasm_code_ptr) return false;
            auto const* code{function.wasm_code_ptr};
            if(frame.function_generation!=1u)
            {
                code=nullptr;
                for(auto const& retained:record.llvm_jit_debug_full_retained_generations)
                {
                    if(retained && retained->committed && retained->module_id==frame.module &&
                       retained->function_index==frame.function && retained->local_index==local &&
                       retained->expected_generation==frame.function_generation-1u)
                    { code=::std::addressof(retained->code);break; }
                }
                if(code==nullptr) return false;
            }
            auto const& params{function.function_type_ptr->parameter};
            if((params.begin==nullptr)!=(params.end==nullptr)) return false;
            auto const parameters{params.begin==nullptr ? 0u : static_cast<::std::size_t>(params.end-params.begin)};
            if(parameters>SIZE_MAX-code->all_local_count) return false;
            count=parameters+static_cast<::std::size_t>(code->all_local_count);
            auto const begin{reinterpret_cast<::std::uintptr_t>(code->body.code_begin)};
            auto const expression{reinterpret_cast<::std::uintptr_t>(code->body.expr_begin)};
            auto const end{reinterpret_cast<::std::uintptr_t>(code->body.code_end)};
            if(begin==0u || expression<begin || end<=expression || expression_offset>=end-expression ||
               expression-begin>UINT32_MAX || expression_offset>UINT32_MAX-(expression-begin)) return false;
            // Runtime locations are expression-relative; the Wasm coredump
            // convention is function-body-relative, including local declarations.
            function_offset=expression-begin+expression_offset;
            return count<=UINT32_MAX;
        }
    public:
        [[nodiscard]] static result collect(domain::pause_ticket const& ticket,::std::uint64_t epoch,
            ::std::string_view executable,::std::string_view reason,void* writer_context,
            ::uwvm2::runtime::diagnostic::wasm_coredump::write_function writer) noexcept
        {
            namespace core=::uwvm2::runtime::diagnostic::wasm_coredump;
            if(!::uwvm2::utils::control::debug_permissions::inspection_allowed() || !ticket || writer==nullptr)
                return {.status=error::capture_denied};
            try
            {
                auto lease{g_runtime.execution_domain.try_enter()};
                if(!lease || lease.stop_requested()) return {.status=error::capture_denied};
                auto control{g_runtime.debug_pause_control};
                if(!control || epoch==0u || current_runtime_generation()!=epoch) return {.status=error::stale_snapshot};
                auto observed{control->capture(ticket)};
                if(observed.result!=::uwvm2::utils::thread::cooperative_pause_result::paused || observed.participants.empty())
                    return {.status=error::capture_busy};
                ::std::vector<llvm_jit_debug_activation_capture_owner> registered;
                {
                    ::std::lock_guard registry{g_debug_activation_capture_mutex};
                    registered.reserve(g_debug_activation_captures.size());
                    for(auto const& slot:g_debug_activation_captures) if(auto capture=slot.lock()) registered.push_back(::std::move(capture));
                }
                ::std::vector<llvm_jit_debug_activation_capture_owner> captures(observed.participants.size());
                domain::paused_transition transition{};
                auto transferred{control->transfer_cooperatively_stopped_cohort(ticket,observed.participants,transition,
                    [&](::std::span<domain::stopped_participant const> slots,auto const& current_ticket)
                    {
                        if(slots.size()!=captures.size() || lease.stop_requested()) return false;
                        for(::std::size_t i{};i!=slots.size();++i)
                        {
                            if(slots[i].location.code_generation!=epoch) return false;
                            for(auto const& capture:registered)
                            {
                                if(same_owner(capture->control_,control) && current_ticket(capture->ticket_) &&
                                   capture->snapshot_.participant==slots[i].id && capture->snapshot_.location==slots[i].location &&
                                   !capture->native_return_anchor_)
                                { captures[i]=capture;break; }
                            }
                        }
                        return true;
                    })};
                if(!transferred) return {.status=error::stale_snapshot};
                // The move-only transition now excludes resume/mutation without
                // retaining the pause-domain mutex across allocations or output.
                auto const configuration{g_runtime.checkpoint_host_state};
                ::uwvm2::utils::thread::checkpoint_host_admission::closed_admission closed{};
                bool const host_closed{runtime_checkpoint_host_bridge::try_close_coherent_state(configuration,closed)==
                    runtime_checkpoint_host_bridge::status::ok};
                ::std::vector<source_owner> sources;
                ::std::vector<core::module_view> modules;
                ::std::vector<::std::string> names;
                ::std::vector<core::instance_view> instances;
                ::std::vector<::std::vector<::std::uint32_t>> memory_maps,global_maps;
                ::std::vector<::std::size_t> memory_starts,global_starts;
                ::std::vector<memory_context> contexts;
                ::std::vector<memory_view> memories;
                ::std::vector<core::global_view> globals;
                ::std::vector<core::thread_view> threads(captures.size());
                ::std::vector<::std::string> thread_names(captures.size());
                ::std::vector<::std::vector<core::frame_view>> frames(captures.size());
                ::std::vector<::std::vector<::std::vector<value>>> locals(captures.size());
                bool readable{};
                {
                    runtime_state_publication_guard publication{};
                    if(!live(::std::addressof(lease),control.get()) || current_runtime_generation()!=epoch ||
                       !same_owner(control,g_runtime.debug_pause_control) ||
                       !g_runtime.compiled_all.load(::std::memory_order_acquire)) return {.status=error::stale_snapshot};
                    auto const count{g_runtime.modules.size()};
                    if(count>UINT32_MAX) return {.status=error::format_overflow};
                    sources.reserve(count);modules.resize(count);names.resize(count);instances.resize(count);
                    memory_maps.resize(count);global_maps.resize(count);memory_starts.resize(count);global_starts.resize(count);
                    ::std::size_t total_memory{},total_global{},memory_imports{},global_imports{};
                    for(::std::size_t m{};m!=count;++m)
                    {
                        auto source{debug_source_memory_actual_owner_locked(m,false)};
                        if(!source) return {.status=error::capture_denied};
                        auto const& record{g_runtime.modules.index_unchecked(m)};
                        auto const& module{*record.runtime_module};
                        sources.push_back(::std::move(source));
                        names[m].assign(reinterpret_cast<char const*>(record.module_name.data()),record.module_name.size());
                        if(module.imported_function_vec_storage.size()>UINT32_MAX ||
                           module.local_defined_function_vec_storage.size()>UINT32_MAX-module.imported_function_vec_storage.size())
                            return {.status=error::format_overflow};
                        modules[m]={names[m],module.imported_function_vec_storage.size()+module.local_defined_function_vec_storage.size()};
                        memory_starts[m]=total_memory;global_starts[m]=total_global;
                        if(module.local_defined_memory_vec_storage.size()>UINT32_MAX-total_memory ||
                           module.local_defined_global_vec_storage.size()>UINT32_MAX-total_global ||
                           module.imported_memory_vec_storage.size()>SIZE_MAX-memory_imports ||
                           module.imported_global_vec_storage.size()>SIZE_MAX-global_imports) return {.status=error::format_overflow};
                        total_memory+=module.local_defined_memory_vec_storage.size();
                        total_global+=module.local_defined_global_vec_storage.size();
                        memory_imports+=module.imported_memory_vec_storage.size();
                        global_imports+=module.imported_global_vec_storage.size();
                    }
                    // The actual host gate alone cannot revoke old native mmap
                    // escapes. Require the sealed source/provider census as well.
                    readable=host_closed && runtime_checkpoint_host_bridge::current_coherent_state(configuration,closed) &&
                        debug_source_memory_exclusive_bytes_locked();
                    contexts.resize(total_memory);memories.resize(total_memory);globals.resize(total_global);
                    for(::std::size_t m{};m!=count;++m)
                    {
                        auto const& module{*g_runtime.modules.index_unchecked(m).runtime_module};
                        for(::std::size_t i{};i!=module.local_defined_memory_vec_storage.size();++i)
                        {
                            auto const& actual{module.local_defined_memory_vec_storage.index_unchecked(i)};
                            if(!actual.memory_type_ptr || actual.memory.custom_page_size_log2!=16u) return {.status=error::unsupported_snapshot};
                            auto const id{memory_starts[m]+i};auto& context{contexts[id]};auto& memory{memories[id]};
                            context.owner=::std::addressof(actual.memory);context.lease=::std::addressof(lease);context.control=control.get();
                            memory.memory64=actual.memory_type_ptr->address64;memory.shared=actual.memory_type_ptr->shared;
                            bool const inspected{with_native_preload_copy_access(actual.memory,[&](::std::byte*,::std::size_t length) noexcept
                            {context.bytes=length;return length%65536u==0u;})};
                            if(!inspected) return {.status=error::inconsistent_capture};
                            memory.bytes=context.bytes;memory.context=::std::addressof(context);memory.read=read_memory;
                            if(readable)
                            {memory.state=core::capture::complete;context.data={0u,context.bytes};memory.regions={::std::addressof(context.data),1u};}
                            auto const& shadow{actual.memory.memtag_shadow};
                            if(shadow.enabled())
                            {
                                memory.tags.enabled=true;memory.tags.bits=shadow.width()==::uwvm2::extension::memtag::address_width::i32 ? 4u:8u;
                                memory.tags.context=::std::addressof(context);memory.tags.read=read_tags;
                                if(readable)
                                {memory.tags.state=core::capture::complete;context.tags={0u,context.bytes/16u};memory.tags.regions={::std::addressof(context.tags),1u};}
                            }
                        }
                        for(::std::size_t i{};i!=module.local_defined_global_vec_storage.size();++i)
                        {
                            auto const& actual{module.local_defined_global_vec_storage.index_unchecked(i)};
                            if(!actual.global_type_ptr) return {.status=error::inconsistent_capture};
                            auto& global{globals[global_starts[m]+i]};
                            global.original_type=static_cast<::std::uint8_t>(actual.global_type_ptr->type);
                            using kind=::uwvm2::object::global::global_type;
                            if(readable)
                            {
                                switch(global.original_type)
                                {
                                    case 0x7fu:if(actual.global.kind!=kind::wasm_i32)return {.status=error::inconsistent_capture};
                                        global.data={core::value_kind::i32,static_cast<::std::uint32_t>(actual.global.storage.i32)};break;
                                    case 0x7eu:if(actual.global.kind!=kind::wasm_i64)return {.status=error::inconsistent_capture};
                                        global.data={core::value_kind::i64,static_cast<::std::uint64_t>(actual.global.storage.i64)};break;
                                    case 0x7du:if(actual.global.kind!=kind::wasm_f32)return {.status=error::inconsistent_capture};
                                        global.data={core::value_kind::f32,::std::bit_cast<::std::uint32_t>(actual.global.storage.f32)};break;
                                    case 0x7cu:if(actual.global.kind!=kind::wasm_f64)return {.status=error::inconsistent_capture};
                                        global.data={core::value_kind::f64,::std::bit_cast<::std::uint64_t>(actual.global.storage.f64)};break;
                                    default:break; // Never serialize native reference addresses.
                                }
                            }
                        }
                    }
                    if(!build_aliases<true>(memory_maps,memory_starts,memory_imports) ||
                       !build_aliases<false>(global_maps,global_starts,global_imports)) return {.status=error::unsupported_snapshot};
                    for(::std::size_t m{};m!=count;++m) instances[m]={m,memory_maps[m],global_maps[m]};
                    for(::std::size_t t{};t!=captures.size();++t)
                    {
                        thread_names[t]="wasm-thread-"+::std::to_string(observed.participants[t].id);
                        threads[t].name=thread_names[t];
                        auto const& capture{captures[t]};
                        if(!capture) continue; // Explicitly unavailable, no guessed native stack.
                        if(!capture->matches_publication_locked(observed.participants[t].location)) return {.status=error::stale_snapshot};
                        auto const& chain{capture->snapshot_.frames};frames[t].resize(chain.size());locals[t].resize(chain.size());
                        for(::std::size_t f{};f!=chain.size();++f)
                        {
                            auto const& actual{chain[chain.size()-1u-f]};auto& frame{frames[t][f]};
                            if(actual.module>=count || actual.function>=modules[static_cast<::std::size_t>(actual.module)].functions)
                                return {.status=error::invalid_index};
                            frame.instance=actual.module;frame.function=actual.function;
                            ::std::size_t n{};::std::uint64_t function_offset{};
                            if(!local_count(actual,n,f==0u ? capture->snapshot_.location.offset:0u,function_offset))
                                return {.status=error::inconsistent_capture};
                            if(f==0u)
                            {frame.code_offset=function_offset;frame.code_offset_available=true;}
                            locals[t][f].resize(n);frame.locals=locals[t][f]; // Missing values, never fabricated zeroes.
                        }
                        threads[t].state=core::capture::partial;threads[t].frames=frames[t];
                    }
                } // Release publication before the first writer callback.
                if(!live(::std::addressof(lease),control.get())) return {.status=error::stale_snapshot};
                core::dump_view dump{executable,reason,readable,modules,instances,memories,globals,threads};
                output_context output{::std::addressof(lease),control.get(),writer_context,writer};
                return core::encode(dump,::std::addressof(output),write);
                // Destruction: detached metadata -> host gate -> pause transition
                // -> true generation lease. No resume is performed by exporting.
            }
            catch(...) { return {.status=error::allocation_failed}; }
        }
    };
}
#endif
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
extern "C++" ::uwvm2::runtime::diagnostic::wasm_coredump::result runtime_dump_stopped_guest_host_api(
    ::uwvm2::utils::thread::cooperative_pause_domain::pause_ticket const& ticket,
    ::std::uint64_t epoch,::std::string_view executable,::std::string_view reason,void* context,
    ::uwvm2::runtime::diagnostic::wasm_coredump::write_function writer) noexcept
{
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_CPP_EXCEPTIONS)
    return runtime_crash_dump_collector::collect(ticket,epoch,executable,reason,context,writer);
#else
    (void)ticket;(void)epoch;(void)executable;(void)reason;(void)context;(void)writer;
    return {.status=::uwvm2::runtime::diagnostic::wasm_coredump::error::unsupported_snapshot};
#endif
}
#endif
