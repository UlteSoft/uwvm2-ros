// Preserve the native ELF macro while using LLVM enumerators in this member fragment.
#pragma push_macro("STT_TLS")
#pragma push_macro("SHT_SYMTAB")
#pragma push_macro("SHT_REL")
#pragma push_macro("SHT_RELA")
#pragma push_macro("SHT_CREL")
#undef STT_TLS
#undef SHT_SYMTAB
#undef SHT_REL
#undef SHT_RELA
#undef SHT_CREL
// ThreadedLlvmChild private cold factory member fragment.
#include "uwvm_runtime_threaded_llvm_object_stream.h"
        [[nodiscard]] bool build_local_type_ids()
        {
            // Expensive immutable type canonicalization is cold/pre-effect only.
            // Only fixed preallocated IDs survive into the refresh operation.
            if(local_type_ids_.size()!=entries_.size())return false;
            compiled_module_record record{};record.runtime_module=module_;
            record.type_canon_index=build_type_canon_index(*module_);
            for(::std::size_t i{};i<local_type_ids_.size();++i)
            {
                auto const* type=module_->local_defined_function_vec_storage.index_unchecked(i).function_type_ptr;
                auto const id=find_canonical_type_id_for_type_uncached(record,*module_,type);
                if(id==invalid_llvm_jit_encoded_type_id())return false;
                local_type_ids_[i]=id;
            }
            return true;
        }
        [[nodiscard]] preparation_status build_reference_type_ids()
        {
            using status=preparation_status;
            if(!reference_dispatch_)return status::ok;
            auto const charged=account_retained_storage();
            if(charged!=status::ok)return charged;
            auto const remaining=storage_limit_-storage_bytes_;
            auto const modules=world_->module_count();
            if(modules>remaining/sizeof(decltype(reference_type_ids_)::value_type))return status::budget;
            auto bytes=modules*sizeof(decltype(reference_type_ids_)::value_type);
            for(::std::size_t m{};m<modules;++m)
            {
                auto const& peer=*world_->module(m);
                for(auto count : {peer.imported_function_vec_storage.size(),peer.local_defined_function_vec_storage.size()})
                {
                    if(count>(remaining-bytes)/sizeof(::std::uint_least32_t))return status::budget;
                    bytes+=count*sizeof(::std::uint_least32_t);
                }
            }
            reference_type_ids_.resize(modules);
            for(::std::size_t m{};m<modules;++m)
            {
                auto const& peer=*world_->module(m);
                reference_type_ids_[m].resize(peer.imported_function_vec_storage.size()+peer.local_defined_function_vec_storage.size());
            }
            compiled_module_record record{};record.runtime_module=module_;
            record.type_canon_index=build_type_canon_index(*module_);
            if(reference_type_ids_.size()!=world_->module_count())return status::invalid_world;
            for(::std::size_t m{};m<reference_type_ids_.size();++m)
            {
                auto const& module=*world_->module(m);auto const imported=module.imported_function_vec_storage.size();
                if(reference_type_ids_[m].size()!=imported+module.local_defined_function_vec_storage.size())return status::invalid_world;
                for(::std::size_t f{};f<reference_type_ids_[m].size();++f)
                {
                    auto const* type=f<imported?module.imported_function_vec_storage.index_unchecked(f).import_type_ptr->imports.storage.function:
                        module.local_defined_function_vec_storage.index_unchecked(f-imported).function_type_ptr;
                    // Project into THIS caller's forest. Another module's
                    // preorder is not an interchangeable signature identifier.
                    reference_type_ids_[m][f]=find_canonical_type_id_for_type_uncached(record,module,type);
                }
            }
            return status::ok;
        }
        [[nodiscard]] ::std::size_t actual_table_local(
            ::uwvm2::uwvm::runtime::storage::local_defined_function_storage_t const* function) const noexcept
        {
            for(::std::size_t i{};i<entries_.size();++i)
                if(function==::std::addressof(module_->local_defined_function_vec_storage.index_unchecked(i)))return i;
            return SIZE_MAX;
        }
        [[nodiscard]] bool validate_table_targets_noalloc() const noexcept
        {
            if(!module_)return false;
            for(::std::size_t t{};t<module_->imported_table_vec_storage.size()+module_->local_defined_table_vec_storage.size();++t)
                if(!world_->table(module_index_,t))return false;
            return true;
        }
        [[nodiscard]] bool refresh_table_targets_noalloc() noexcept
        {bound_tables_=validate_table_targets_noalloc();return bound_tables_;}
        using native_elf_layout=::std::conditional_t<sizeof(void*)==8u,::llvm::object::ELF64LE,::llvm::object::ELF32LE>;
        [[nodiscard]] static ::llvm::Expected<::std::uint64_t> native_relocation_count(
            ::llvm::object::ELFFile<native_elf_layout> const& file,native_elf_layout::Shdr const& section)
        {
            auto const type=section.sh_type;
            if(type!=::llvm::ELF::SHT_REL && type!=::llvm::ELF::SHT_RELA && type!=::llvm::ELF::SHT_CREL)return 0u;
            auto linked=file.getSection(section.sh_link);
            if(!linked)return linked.takeError();
            if(type==::llvm::ELF::SHT_REL)
            {
                auto entries=file.rels(section);
                if(!entries)return entries.takeError();
                return entries->size();
            }
            if(type==::llvm::ELF::SHT_RELA)
            {
                auto entries=file.relas(section);
                if(!entries)return entries.takeError();
                return entries->size();
            }
            auto contents=file.getSectionContents(section);
            if(!contents)return contents.takeError();
            auto header=file.getCrelHeader(*contents);
            if(!header)return header.takeError();
            // The CREL header encodes the count above its three flag/shift bits.
            return *header >> 3u;
        }
        [[nodiscard]] preparation_status check_object(::llvm::object::ObjectFile const& object,::std::size_t relocation_limit,
            ::std::size_t& reservation,
            ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_string>& definitions)
        {
            // A finite, deliberately conservative native ELF loader charge:
            // real section bytes, max alignment per section, relocation stub/GOT
            // allowance and three native page groups. No null allocator is used
            // to pretend RuntimeDyld's fatal allocation path is recoverable.
            using status=preparation_status;
            ::std::uint64_t sections{},relocations{},symbols{},charge{3u*native_arena_granule},arena_bytes{};
            bool totals_complete{};
            auto reject=[&](char const* reason,status why=status::native_failure)
            {
                ::fast_io::perrln("uwvm: [error] WASI Threads LLVM object rejected in module ",module_index_,
                    " (",::fast_io::mnp::os_c_str(reason),") totals-complete=",static_cast<unsigned>(totals_complete),
                    " sections=",sections," relocations=",relocations," symbols=",symbols," relocation-limit=",relocation_limit,
                    " charged-bytes=",charge," arena-bytes=",arena_bytes," code-limit=",code_limit_);
                return why;
            };
            if(!object.isELF()||object.getArch()!=native_object_arch||!object.isLittleEndian()||object.getBytesInAddress()!=native_address_bytes)
                return reject("native ELF target");
            auto const& elf=::llvm::cast<::llvm::object::ELFObjectFile<native_elf_layout>>(object).getELFFile();
            auto headers=elf.sections();
            if(!headers){::llvm::consumeError(headers.takeError());return reject("section headers");}
            sections=headers->size();
            if(sections>maximum_object_sections)return reject("section count exceeds 4096",status::budget);
            charge+=sections*2u*native_arena_granule;
            bool saw_symtab{},saw_crel{};
            for(auto const& section:*headers)
            {
                if(section.sh_addralign>native_arena_granule)return reject("section alignment exceeds 65536",status::budget);
                auto name=elf.getSectionName(section);
                if(!name){::llvm::consumeError(name.takeError());return reject("section name");}
                if(section.sh_size>UINT64_MAX-charge)return reject("section byte charge overflow",status::budget);
                charge+=section.sh_size;
                auto count=native_relocation_count(elf,section);
                if(!count){::llvm::consumeError(count.takeError());return reject("relocation table");}
                if(*count>UINT32_MAX)return reject("relocation table index range",status::budget);
                if(*count>UINT64_MAX-relocations)return reject("relocation count overflow",status::budget);
                relocations+=*count;
                saw_crel|=section.sh_type==::llvm::ELF::SHT_CREL;
                if(section.sh_type==::llvm::ELF::SHT_SYMTAB && !saw_symtab)
                {
                    auto entries=elf.symbols(&section);
                    if(!entries){::llvm::consumeError(entries.takeError());return reject("symbol table");}
                    // LLVM ObjectFile::symbols() skips ELF's reserved null symbol.
                    symbols=entries->empty()?0u:entries->size()-1u;
                    saw_symtab=true;
                }
            }
            constexpr auto bytes_per_relocation=::uwvm2::runtime::wasm_threads::threaded_llvm_limits::bytes_per_relocation;
            if(relocations>(UINT64_MAX-charge)/bytes_per_relocation)return reject("relocation byte charge overflow",status::budget);
            charge+=relocations*bytes_per_relocation;
            if(charge>UINT64_MAX-(native_arena_granule-1u))return reject("arena rounding overflow",status::budget);
            arena_bytes=(charge+native_arena_granule-1u)&~::std::uint64_t{native_arena_granule-1u};
            totals_complete=true;
            if(relocations>relocation_limit)return reject("relocation count exceeds host limit",status::budget);
            if(symbols>maximum_object_symbols)return reject("symbol count exceeds 262144",status::budget);
            if(arena_bytes>code_limit_)return reject("native arena byte charge",status::budget);
            if(saw_crel)
            {
                auto const& native=::llvm::cast<::llvm::object::ELFObjectFileBase>(object);
                for(auto const& section:object.sections())
                    if(::llvm::object::ELFSectionRef{section}.getType()==::llvm::ELF::SHT_CREL)
                    {
                        // LLVM materializes CREL entries on first use. Decode only after
                        // the declared count and its full byte charge have been admitted.
                        static_cast<void>(section.relocation_begin());
                        if(!native.getCrelDecodeProblem(section).empty())return reject("compressed relocation table");
                    }
            }
            for(auto const& symbol:object.symbols())
            {
                auto flags=symbol.getFlags();auto name=symbol.getName();
                if(!flags){::llvm::consumeError(flags.takeError());return reject("symbol flags");}
                if(!name){::llvm::consumeError(name.takeError());return reject("symbol name");}
                if((*flags&::llvm::object::SymbolRef::SF_Common)||::llvm::object::ELFSymbolRef{symbol}.getELFType()==::llvm::ELF::STT_TLS)
                    return reject("common or TLS symbol");
                if(*flags&::llvm::object::SymbolRef::SF_Undefined)
                {
                    if(name->empty())continue;
                    bool mapped{};for(auto const& binding:externals_)if(binding.name==*name)mapped=true;
                    // Resolve only the native memory, arithmetic and unwind
                    // helpers emitted by the backend, through this object's
                    // finite registry. Guest imports use the world dispatcher.
                    if(!mapped)
                    {
                        ::std::uintptr_t pointer{};
                        if(*name=="memcpy")pointer=address(static_cast<void*(*)(void*,void const*,::std::size_t)>(&::std::memcpy));
                        else if(*name=="memmove")pointer=address(static_cast<void*(*)(void*,void const*,::std::size_t)>(&::std::memmove));
                        else if(*name=="memset")pointer=address(static_cast<void*(*)(void*,int,::std::size_t)>(&::std::memset));
#if defined(__linux__) && (defined(__i386__) || defined(__arm__))
                        // Includes scalar rounding and square root on ARM EABI.
                        else pointer=::uwvm2::runtime::compiler::llvm_jit::details::native_compiler_builtins::find(::uwvm2::utils::container::copy_string(*name));
#endif
#if defined(UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT) && UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT == 1 && \
    defined(__linux__) && defined(__arm__) && !defined(__ARM_DWARF_EH__) && defined(__GLIBC__)
                        // Native backend-generated compact unwind relocations,
                        // paired with the same static libunwind as this process.
                        if(*name=="__aeabi_unwind_cpp_pr0")pointer=reinterpret_cast<::std::uintptr_t>(&::__aeabi_unwind_cpp_pr0);
                        else if(*name=="__aeabi_unwind_cpp_pr1")pointer=reinterpret_cast<::std::uintptr_t>(&::__aeabi_unwind_cpp_pr1);
                        else if(*name=="__aeabi_unwind_cpp_pr2")pointer=reinterpret_cast<::std::uintptr_t>(&::__aeabi_unwind_cpp_pr2);
#endif
                        if(pointer==0u)
                        {
                            auto const spelling=name->take_front(128u);
                            ::fast_io::perrln("uwvm: [error] WASI Threads LLVM unbound native symbol ",
                                ::fast_io::mnp::strvw(spelling.begin(),spelling.end()));
                            return reject("unbound native symbol");
                        }
                        if(externals_.size()>=256u)return reject("external binding count exceeds 256",status::budget);
                        externals_.push_back({::uwvm2::utils::container::copy_string(*name),pointer});
                    }
                }
                else if(!name->empty())
                {
                    definitions.push_back(::uwvm2::utils::container::copy_string(*name));
                    auto type=symbol.getType();if(!type){::llvm::consumeError(type.takeError());return reject("symbol type");}
                }
            }
            if(!check_static_stack_object(object))return reject("native stack witness");
            if(guest_fixed_stack_charge_>guest_native_stack_usable_bytes_)
            {
                report_native_stack_budget("object");
                return reject("native stack budget",status::budget);
            }
            reservation=static_cast<::std::size_t>(arena_bytes);
            return status::ok;
        }
        [[nodiscard]] static preparation_result build(world_type::owner world,
            ::uwvm2::runtime::wasm_threads::ThreadedGroup::CompilerLease const& compiler,
            [[maybe_unused]] ::std::size_t backend_storage_budget, [[maybe_unused]] ::std::size_t code_budget, ::std::size_t module_index,
            [[maybe_unused]] ::uwvm2::runtime::wasm_threads::threaded_llvm_limits const& llvm_limits,
            [[maybe_unused]] bool* native_poll_attempted) noexcept
        {
            namespace compile=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm;
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            using status=preparation_status;
            if(!world||active_!=nullptr||!compiler.owns_current_native_thread()||
               ::std::addressof(compiler.admission())!=::std::addressof(world->admission())||
               compiler.cancellation().stop_requested())return {};
            auto actual_code=world->validated_code();
            if(!actual_code||!actual_code->covers(*world)||actual_code->file()!=world->file(world->entry_module_index()))return {};
            if(!world->admission().owns_actual_generation()||module_index>=world->module_count()||world->entry_module_index()!=0u||
               world->module(module_index)==nullptr||world->file(module_index)==nullptr||world->feature_parameter(module_index)==nullptr)return {};
#if !(((defined(__x86_64__) || defined(__aarch64__)) && __SIZEOF_POINTER__ == 8) || ((defined(__i386__) || defined(__arm__)) && __SIZEOF_POINTER__ == 4))
            return {{},status::unsupported_shape};
#else
            return ::uwvm2::utils::allocator::resource::recoverable_parser_allocations::prepare_candidate([&]() -> preparation_result
            {
            try
            {
                auto data=::uwvm2::utils::container::std_unique_ptr<ThreadedLlvmChild>{::uwvm2::utils::allocator::fast_io_strict::object<ThreadedLlvmChild>([&](ThreadedLlvmChild* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) ThreadedLlvmChild{::std::move(world)}; })};
                data->module_index_=module_index;data->module_=data->world_->module(module_index);auto const& module=*data->module_;
                auto const& limits=data->world_->admission().limits();
                data->call_limit_=limits.max_call_frames;data->code_limit_=code_budget;
                data->guest_native_stack_usable_bytes_=compiler.guest_native_stack_usable_bytes();
                data->compiler_native_stack_usable_bytes_=compiler.native_stack_usable_bytes();
                if(data->guest_native_stack_usable_bytes_==0u||data->compiler_native_stack_usable_bytes_==0u)
                    return {{},status::invalid_world};
                if(data->call_limit_==0u||limits.max_child_code_bytes==0u||limits.max_child_storage_bytes==0u)
                    return {{},status::invalid_world};
                if(data->call_limit_>65536u)return {{},status::budget};
                data->imports_=module.imported_function_vec_storage.size();
                auto const count=module.local_defined_function_vec_storage.size();
                if(count!=0u && code_budget==0u)return {{},status::budget};
                data->start_=module_index==0u?data->world_->thread_start_function_index():data->imports_;
                // An import-only side module has no Wasm entry to execute. It
                // still contributes authenticated aliases and owns its private
                // instance state; only the root requires a thread-start body.
                if(count>65536u||data->imports_>65536u||count>SIZE_MAX-data->imports_||
                   (module_index==0u && (count==0u||data->start_<data->imports_||
                    data->start_-data->imports_>=count)))return {{},status::unsupported_shape};
                // Real imported-memory aliases must have been normalized to a
                // canonical source-owned defined native memory by World factory.
                using memory_link=::uwvm2::uwvm::runtime::storage::imported_memory_storage_t::imported_memory_link_kind;
                for(auto const& imported:module.imported_memory_vec_storage)
                    if(imported.link_kind!=memory_link::defined||imported.target.defined_ptr==nullptr)return {{},status::unsupported_shape};
                auto const memory_count=module.imported_memory_vec_storage.size()+module.local_defined_memory_vec_storage.size();
                if(memory_count>backend_storage_budget/(sizeof(memory_type*)+2u*sizeof(emit::threaded_forbidden_memory_span)))return {{},status::budget};
                data->memories_.reserve(memory_count);
                ::uwvm2::utils::container::std_vector<emit::threaded_forbidden_memory_span> forbidden_memtag_spans(memory_count*2u);
                ::std::size_t forbidden_memtag_count{};
                bool requires_shared_memtag_lease{};
                auto forbid_memtag_span=[&](::std::uintptr_t begin,::std::size_t bytes) noexcept
                {
                    if(bytes==0u)return true;
                    if(begin==0u||bytes>UINTPTR_MAX-begin||forbidden_memtag_count==forbidden_memtag_spans.size())return false;
                    forbidden_memtag_spans[forbidden_memtag_count++]={begin,bytes};return true;
                };
                for(::std::size_t i{};i<memory_count;++i)
                {
                    auto const info=emit::resolve_runtime_memory_access_info(module,static_cast<::std::uint32_t>(i));
                    if(info.memory_p==nullptr||info.memory_p!=data->world_->native_memory(module_index,i)||
                       info.local_imported_module_ptr!=nullptr||info.stable_memory_begin==nullptr||
                       info.stable_memory_length_p==nullptr||(info.custom_page_size_log2!=0u && info.custom_page_size_log2!=16u))return {{},status::unsupported_shape};
                    data->memories_.push_back(info.memory_p);
                    auto const& tags=info.memory_p->memtag_shadow;
                    if(tags.enabled()&&tags.shared())
                    {
                        if(tags.shared()&&!tags.shared_tag_lease_ready())return {{},status::unsupported_shape};
                        requires_shared_memtag_lease|=tags.shared();
                        auto const count=tags.maximum_memory_bytes()/16u+(tags.maximum_memory_bytes()%16u!=0u);
                        if(count>SIZE_MAX/sizeof(::std::atomic<::std::uint8_t>)||
                           !forbid_memtag_span(reinterpret_cast<::std::uintptr_t>(info.stable_memory_begin),info.stable_memory_reserved_span_bytes)||
                           !forbid_memtag_span(reinterpret_cast<::std::uintptr_t>(tags.tag_data()),static_cast<::std::size_t>(count)*sizeof(::std::atomic<::std::uint8_t>)))
                            return {{},status::unsupported_shape};
                    }
                }
                for(::std::size_t i{};i<data->imports_;++i)
                    if(::uwvm2::runtime::wasm_threads::admit_import(*data->world_,module_index,i)!=
                       ::uwvm2::runtime::wasm_threads::import_admission::allowed)return {{},status::unsupported_shape};
                ::std::size_t bodies{};
                auto const* file=data->world_->file(module_index);
                auto const base=reinterpret_cast<::std::uintptr_t>(file->source_cbegin());auto const bytes=file->source_size();
                if(base==0u||bytes==0u)return {{},status::invalid_world};
                for(auto const& local:module.local_defined_function_vec_storage)
                {
                    if(local.function_type_ptr==nullptr||local.wasm_code_ptr==nullptr)return {{},status::invalid_body};
                    auto const& signature=*local.function_type_ptr;
                    auto const parameters=signature.parameter.begin==signature.parameter.end?0u:
                        static_cast<::std::size_t>(signature.parameter.end-signature.parameter.begin);
                    if(parameters>65536u)return {{},status::budget};
                    ::std::size_t locals=parameters;
                    for(auto const& group:local.wasm_code_ptr->locals)
                    {if(group.count>65536u-locals)return {{},status::budget};locals+=group.count;}
                    if(locals>limits.max_value_stack_bytes/16u)return {{},status::budget};
                    auto const begin=reinterpret_cast<::std::uintptr_t>(local.wasm_code_ptr->body.code_begin);
                    auto const expression=reinterpret_cast<::std::uintptr_t>(local.wasm_code_ptr->body.expr_begin);
                    auto const end=reinterpret_cast<::std::uintptr_t>(local.wasm_code_ptr->body.code_end);
                    if(begin<base||expression<begin||end<=expression||end-base>bytes)return {{},status::invalid_body};
                    if(end-begin>(1u<<20u)||
                       bodies>limits.max_child_storage_bytes||end-begin>limits.max_child_storage_bytes-bodies)return {{},status::budget};
                    bodies+=end-begin;if(bodies>(8u<<20u))return {{},status::budget};
                }
                auto const world_charge=data->world_->owned_storage_bytes();
                if(world_charge>limits.max_child_storage_bytes)return {{},status::budget};
                auto const remaining=(::std::min)(backend_storage_budget,limits.max_child_storage_bytes-world_charge);
                data->storage_limit_=remaining;
                // Monotonic structural PRECHARGE: class + fixed native-manager
                // arrays + per-function packet/entry/name space. Never silently
                // use old request counts or assume zero allocator overhead/RSS.
                bool const observing=data->world_->admission().debug_observation()==::uwvm2::runtime::wasm_threads::threaded_debug_observation::readonly_metadata;
                auto const per_debug_function=observing?2u*sizeof(::uwvm2::runtime::wasm_threads::debug_function_row):0u;
                bool const instruction_control=data->world_->admission().instruction_control();
                if(instruction_control&&!observing)return {{},status::invalid_world};
                auto const instruction_capacity=instruction_control?bodies:0u;
                if(instruction_capacity>remaining/sizeof(::uwvm2::runtime::wasm_threads::debug_logical_pc_row))return {{},status::budget};
                auto const instruction_charge=instruction_capacity*sizeof(::uwvm2::runtime::wasm_threads::debug_logical_pc_row);
                auto const fixed_debug=(observing?sizeof(::uwvm2::runtime::wasm_threads::code_debug_image)+maximum_object_sections*sizeof(::uwvm2::runtime::wasm_threads::debug_cfi_row):0u)+instruction_charge;
                ::std::size_t const fixed_charge=sizeof(ThreadedLlvmChild)+sizeof(native_arena)+2u*maximum_object_sections*64u+
                    256u*(sizeof(external)+512u+sizeof(native_frame_record)+256u)+
                    fixed_debug+native_arena::unwind_storage_precharge(count);
                if(fixed_charge>remaining||count>(remaining-fixed_charge)/(sizeof(local_type)+sizeof(entries)+sizeof(::std::uint_least32_t)+2u*sizeof(native_frame_record)+768u+per_debug_function))
                    return {{},status::budget};
                if(observing)
                {
                    data->debug_.reset(::uwvm2::utils::allocator::fast_io_strict::object<::uwvm2::runtime::wasm_threads::code_debug_image>([&](::uwvm2::runtime::wasm_threads::code_debug_image* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) ::uwvm2::runtime::wasm_threads::code_debug_image{data->world_,::uwvm2::runtime::wasm_threads::debug_backend::llvm,data->world_->source_module_id()}; }));
                    data->debug_->functions_.resize(count*2u);data->debug_->cfi_.reserve(4096u);
                    data->debug_->instruction_control_=instruction_control;data->debug_->logical_.reserve(instruction_capacity);
                    if(data->debug_->allocated_bytes()>count*per_debug_function+fixed_debug-sizeof(::uwvm2::runtime::wasm_threads::code_debug_image))return {{},status::budget};
                }
                data->locals_.resize(count);data->entries_.resize(count);data->local_type_ids_.resize(count);data->native_frames_.reserve(count*2u+256u);data->emitted_bridges_.reserve(256u);data->externals_.reserve(256u);
                auto const tables=module.local_defined_table_vec_storage.size();
                if(tables>256u)return {{},status::unsupported_shape};
                auto const structural_function_charge=fixed_charge+count*(sizeof(local_type)+sizeof(entries)+sizeof(::std::uint_least32_t)+2u*sizeof(native_frame_record)+768u+per_debug_function);
                auto const table_shape_unit=sizeof(decltype(data->table_targets_)::value_type)+sizeof(decltype(data->table_views_)::value_type);
                if(structural_function_charge>remaining||tables>(remaining-structural_function_charge)/table_shape_unit)
                    return {{},status::budget};
                auto const table_shape_charge=structural_function_charge+tables*table_shape_unit;
                data->table_targets_.resize(tables);data->table_views_.resize(tables);
                ::std::size_t elements{};
                for(::std::size_t i{};i<tables;++i)
                {
                    auto const n=module.local_defined_table_vec_storage.index_unchecked(i).elems.size();
                    if(elements>limits.max_table_elements||n>limits.max_table_elements-elements)return {{},status::budget};
                    elements+=n;
                    if(elements>(remaining-table_shape_charge)/sizeof(target_type))return {{},status::budget};
                    // Dynamic indirect targets are copied under the actual private instance lease.
                }
                if(elements>(SIZE_MAX-table_shape_charge)/sizeof(target_type))return {{},status::budget};
                auto const schemas=data->prepare_exception_schemas(table_shape_charge+elements*sizeof(target_type));
                if(schemas!=status::ok)return {{},schemas};
                ::uwvm2::validation::error::code_validation_error_impl error{};
                emit::require_runtime_module_declaration_policy(module,*data->world_->feature_parameter(module_index),error);
                if(count==0u)
                {
                    // A side module can re-export an authenticated host import
                    // without defining code. Retain its private instance and
                    // validate its tables, but do not manufacture a native entry
                    // or send an empty object through the executable loader.
                    if(!data->build_local_type_ids()||!data->refresh_table_targets_noalloc())return {{},status::unsupported_shape};
                    ::uwvm2::utils::container::std_vector<native_bridge>{}.swap(data->emitted_bridges_);
                    ::uwvm2::utils::container::std_vector<external>{}.swap(data->externals_);
                    ::uwvm2::utils::container::std_vector<native_frame_record>{}.swap(data->native_frames_);
                    auto const charged=data->account_retained_storage();
                    if(charged!=status::ok)return {{},charged};
                    return {::std::move(data),status::ok};
                }
                    auto validation=emit::build_runtime_validation_module(module);
                compile::compile_option options{};options.curr_wasm_id=module_index;options.compiler_registry=nullptr; // private cold scope authenticates the exact single module
                options.compilation_mode=compile::llvm_jit_compilation_mode::full;
                options.execute_memtag=module.memory_policy.check_tags;
                options.validator_feature_parameter=data->world_->feature_parameter(module_index);options.verify_llvm_jit_ir=true;
                options.emit_call_stack_frames=false;options.emit_unwind_call_stack_frames=false;
                options.emit_precise_gc_root_frames=false;options.emit_debug_safe_points=false;
                compile::llvm_jit_module_storage_t ir{};
                if(!emit::try_prepare_runtime_llvm_jit_module_storage(module,ir,false,nullptr))return {{},status::invalid_body};
                if(requires_shared_memtag_lease&&!emit::set_memtag_shared_lease_module_abi(*ir.llvm_module))
                    return {{},status::unsupported_bridge};
                auto& llvm_module=*ir.llvm_module;
                auto const& ir_limits=llvm_limits.ir;
                auto ir_room=ir_limits;
                // Pre- and post-pass finite IR quotas. These are NOT a claim of
                // a hard bound on all LLVM optimizer heap allocations.
                auto ir_fits=[&](char const* stage)
                {
                    ::std::size_t instructions{},blocks{};
                    for(auto const& f:llvm_module)for(auto const& b:f)
                    {
                        auto const count=b.size();
                        if(++blocks>ir_limits.blocks || count>ir_limits.instructions-instructions)
                        {
                            auto const observed=count>SIZE_MAX-instructions ? SIZE_MAX : instructions+count;
                            ::fast_io::perrln("uwvm: [error] WASI Threads LLVM IR limit in module ",module_index,
                                " (",::fast_io::mnp::os_c_str(stage),") blocks=",blocks,"/",ir_limits.blocks,
                                " instructions=",observed,"/",ir_limits.instructions);
                            return false;
                        }
                        instructions+=count;
                    }
                    ir_room.instructions=ir_limits.instructions-instructions;
                    ir_room.blocks=ir_limits.blocks-blocks;
                    return true;
                };
                {
                    emit::threaded_host_emission_scope owner{*data->world_,module,*ir.llvm_module,data->emitted_bridges_,
                        reinterpret_cast<::std::uintptr_t>(module.llvm_jit_call_indirect_table_views.data()),
                        reinterpret_cast<::std::uintptr_t>(data->table_views_.data()),
                        forbidden_memtag_spans.data(),forbidden_memtag_count,
                        instruction_control?address(&instruction_point):0u,instruction_control?data.get():nullptr,
                        instruction_control?&append_instruction_site:nullptr};
                    for(::std::size_t i{};i<count;++i)
                    {
                        if(compiler.cancellation().stop_requested())return {{},status::invalid_world};
                        if(instruction_control)
                        {
                            auto& row=data->debug_->functions_[i*2u];row.logical_begin=data->debug_->logical_.size();
                            row.wasm_begin=reinterpret_cast<::std::uintptr_t>(module.local_defined_function_vec_storage.index_unchecked(i).wasm_code_ptr->body.expr_begin)-base;
                        }
                        data->locals_[i]=emit::compile_all_from_uwvm_local_func(module,validation,options,i,error,::std::addressof(ir));
                        if(instruction_control)data->debug_->functions_[i*2u].logical_count=
                            data->debug_->logical_.size()-data->debug_->functions_[i*2u].logical_begin;
                        data->locals_[i].compiler_registry=nullptr;
                        if(ir.llvm_module==nullptr||ir.llvm_context_holder==nullptr||ir.first_decline.available())return {{},status::invalid_body};
                        auto const typed=get_runtime_llvm_jit_wasm_function_name(module,data->imports_+i);
                        auto const raw=get_runtime_llvm_jit_wasm_raw_function_name(module,data->imports_+i);
                        if(typed.size()>128u||raw.size()>128u)return {{},status::budget};
                    data->entries_[i].typed_name=::uwvm2::utils::container::copy_string(emit::get_llvm_string_ref(typed));
                        data->entries_[i].raw_name=::uwvm2::utils::container::copy_string(emit::get_llvm_string_ref(raw));
                    }
                    if(!owner.valid()||!emit::finalize_runtime_llvm_jit_module_storage(ir,true))return {{},status::invalid_body};
                    if(compiler.cancellation().stop_requested())return {{},status::invalid_world};
                    if((requires_shared_memtag_lease&&!emit::has_memtag_shared_lease_module_abi(llvm_module))||
                       !data->check_memtag_shared_lease_ir(llvm_module))return {{},status::unsupported_bridge};
                    // Bound the original translation before promoting private
                    // locals and instrumenting its block and call boundaries.
                    if(!ir_fits("before control"))return {{},status::budget};
                    if(!data->add_control(llvm_module))
                    {
                        ::fast_io::perrln("uwvm: [error] WASI Threads LLVM control insertion rejected in module ",module_index);
                        return {{},status::native_failure};
                    }
                    if(!ir_fits("after control"))return {{},status::budget};
                    if(!data->check_static_stack_ir(llvm_module))
                    {
                        ::fast_io::perrln("uwvm: [error] WASI Threads LLVM static stack IR rejected in module ",module_index);
                        return {{},status::native_failure};
                    }
                    if(!owner.valid())return {{},status::unsupported_bridge};
                }
                ::std::size_t gc_inputs{};
                for(auto const& bridge:data->emitted_bridges_)
                {
                    if(bridge.original!=address(&emit::llvm_jit_gc_aggregate_bridge<>))continue;
                    for(auto const* use:bridge.declaration->users())
                    {
                        auto const* call=::llvm::dyn_cast<::llvm::CallBase>(use);
                        auto const* count_value=call && call->arg_size()==7u ? ::llvm::dyn_cast<::llvm::ConstantInt>(call->getArgOperand(5u)) : nullptr;
                        if(!count_value)return {{},status::invalid_body};
                        if(count_value->getZExtValue()>limits.max_value_stack_bytes/sizeof(::uwvm2::uwvm::runtime::storage::gc_object_value))return {{},status::budget};
                        gc_inputs=(::std::max)(gc_inputs,static_cast<::std::size_t>(count_value->getZExtValue()));
                    }
                }
                if(gc_inputs>data->storage_limit_/sizeof(::uwvm2::uwvm::runtime::storage::gc_object_value))return {{},status::budget};
                data->gc_scratch_.resize(gc_inputs);
                if(!data->bind_emitted_bridges())return {{},status::unsupported_bridge};
                if(!ensure_llvm_jit_native_target_initialized())return {{},status::native_failure};
                auto const cpu=get_llvm_jit_host_cpu_name_storage();auto const attrs=get_llvm_jit_host_target_attribute_storage();
                ::llvm::SmallVector<::llvm::StringRef,16> refs;append_llvm_jit_host_target_attribute_refs(attrs,refs);
                // Threaded control is an explicit new profile. No reuse of an
                // ordinary/debug cache object, no inherited publication/epoch.
                auto const policy=resolve_runtime_llvm_jit_full_materialize_strategy(::llvm::CodeGenOptLevel::Default, module.translation);
                ::llvm::EngineBuilder builder;builder.setEngineKind(::llvm::EngineKind::JIT).setOptLevel(policy.codegen_opt_level)
                    .setMCPU(emit::get_llvm_string_ref(cpu)).setMAttrs(refs);
                auto target=select_runtime_llvm_jit_target(builder,cpu,attrs);if(!target)return {{},status::native_failure};
                target->Options.EmitStackSizeSection=true;
                set_llvm_module_target_triple_from_machine(llvm_module,*target);llvm_module.setDataLayout(target->createDataLayout());
                apply_runtime_llvm_jit_native_target_function_attrs(llvm_module,cpu,cpu,*target);
                if(!optimize_runtime_llvm_jit_module(llvm_module,*target,policy.pipeline,policy.codegen_opt_level,true,false))
                    return {{},status::native_failure};
                if(!ir_fits("after optimization"))return {{},status::budget};
                data->reuse_control_targets(llvm_module);
                if(!ir_fits("after control target loads"))return {{},status::budget};
                native_poll_witnesses native_polls;
                if(native_poll_attempted)
                    data->inline_cancellation_loops(llvm_module,ir_room,native_polls,*native_poll_attempted);
                if(!native_polls.empty()&&!ir_fits("after native cancellation polls"))return {{},status::budget};
                if(!data->check_static_stack_ir(llvm_module)||!prove_stop_cycles(llvm_module,native_polls)||::llvm::verifyModule(llvm_module))
                    return {{},status::native_failure};
                if((requires_shared_memtag_lease&&!emit::has_memtag_shared_lease_module_abi(llvm_module))||
                   !data->check_memtag_shared_lease_ir(llvm_module))return {{},status::unsupported_bridge};
                if(!data->record_native_call_graph(llvm_module))return {{},status::native_failure};
                // Strict FP legalization can introduce this integer-only
                // helper after the initial import bridge pass, notably for
                // i686 i64 conversions. Authenticate its exact IR ABI before
                // binding the existing pure arithmetic implementation.
                namespace strict_fp=::uwvm2::runtime::compiler::shared::strict_float_jit;
                if(auto* helper=llvm_module.getFunction(strict_fp::symbol_name))
                {
                    auto& context=llvm_module.getContext();
                    auto* i64=::llvm::Type::getInt64Ty(context);auto* i32=::llvm::Type::getInt32Ty(context);
                    auto* signature=::llvm::FunctionType::get(i64,{i64,i64,i32},false);
                    if(helper->getFunctionType()!=signature||helper->getCallingConv()!=::llvm::CallingConv::C||
                       !data->add_external(*helper,address(&strict_fp::bridge)))return {{},status::unsupported_bridge};
                }
                bounded_object_stream output{data->code_limit_};
                {
                    // Destroy codegen passes/MMI while their actual module,
                    // context and target still exist; never retain this cold
                    // pipeline across loader-engine ownership transfer.
                    observed_native_pass_manager native{*data};
                    if(target->addPassesToEmitFile(native,output,nullptr,::llvm::CodeGenFileType::ObjectFile))return {{},status::native_failure};
                    native.run(llvm_module);
                    if(!native.valid())return {{},status::native_failure};
                }
                if(compiler.cancellation().stop_requested())return {{},status::invalid_world};
                if(!output.valid())
                {
                    ::fast_io::perrln("uwvm: [error] WASI Threads LLVM object stream rejected in module ",module_index,
                        " emitted-bytes=",output.tell()," code-limit=",data->code_limit_);
                    return {{},output.exceeded_budget() ? status::budget : status::native_failure};
                }
                auto buffer=::llvm::MemoryBuffer::getMemBufferCopy(output.bytes(),"uwvm.threaded.child.object");
                if(!buffer)return {{},status::native_failure};
                auto parsed=::llvm::object::ObjectFile::createObjectFile(buffer->getMemBufferRef());
                if(!parsed){::llvm::consumeError(parsed.takeError());return {{},status::native_failure};}
                ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_string> definitions;
                ::std::size_t reservation{};
                auto const checked=data->check_object(**parsed,llvm_limits.relocations,reservation,definitions);
                if(checked!=status::ok)return {{},checked};
                for(auto const& entry:data->entries_)
                {
                    bool raw{},typed{};for(auto const& name:definitions){raw|=name==entry.raw_name;typed|=name==entry.typed_name;}
                    if(!raw||!typed)return {{},status::native_failure};
                }
                // Native object is complete and checked BEFORE an empty loader
                // engine is created. The original IR never gets compiled twice.
                ir.llvm_module.reset();data->emitted_bridges_.clear();
                ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_string>{}.swap(data->native_call_names_);
                ::uwvm2::utils::container::std_vector<native_call_edge>{}.swap(data->native_call_edges_);
                ir.llvm_context_holder.reset();
                ir.llvm_context_holder=::uwvm2::runtime::compiler::llvm_jit::fast_io_support::make_context();
                if(!ir.llvm_context_holder)return {{},status::native_failure};
                auto empty=::uwvm2::runtime::compiler::llvm_jit::fast_io_support::make_module("uwvm.threaded.child.loader",*ir.llvm_context_holder);
                if(!empty)return {{},status::native_failure};
                set_llvm_module_target_triple_from_machine(*empty,*target);empty->setDataLayout(target->createDataLayout());
                ir.llvm_module.reset();
                // check_object charged every section, alignment and relocation
                // allowance. Reserve that conservative object bound, rather
                // than the whole graph budget once per compiled module.
                data->native_arena_=::uwvm2::utils::container::make_unique<native_arena>(reservation,data->externals_,data->native_frames_.size());
                if(!data->native_arena_||!data->native_arena_->valid())return {{},status::native_failure};
                auto* memory_observer=data->native_arena_.get();
                data->native_bytes_=memory_observer->owned_bytes();
                if(data->native_bytes_>data->code_limit_)return {{},status::budget};
                auto memory=::uwvm2::utils::container::make_unique<loader_memory_adapter>(*memory_observer);
                if(!memory)return {{},status::native_failure};
                auto* engine=::llvm::EngineBuilder(llvm_module_owner_t{empty.release()}).setEngineKind(::llvm::EngineKind::JIT)
                    .setOptLevel(policy.codegen_opt_level).setMCPU(emit::get_llvm_string_ref(cpu)).setMAttrs(refs)
                    .setMCJITMemoryManager(llvm_jit_memory_manager_owner_t{memory.release()}).create(target.release());
                if(engine==nullptr)return {{},status::native_failure};
                data->context_=::std::move(ir.llvm_context_holder);data->engine_.reset(engine);engine->DisableSymbolSearching();
                for(auto const& mapping:data->externals_)engine->addGlobalMapping(::uwvm2::runtime::compiler::llvm_jit::fast_io_support::ref(mapping.name),mapping.address);
                data->ranges_=::uwvm2::utils::container::make_unique<details::pending_llvm_jit_code_ranges>(*engine,true);
                if(!data->ranges_)return {{},status::native_failure};data->ranges_->configure_debug_full_capture(false); // Stage A has only its explicitly charged bounded maps.
                engine->addObjectFile(::llvm::object::OwningBinary<::llvm::object::ObjectFile>{::std::move(*parsed),::std::move(buffer)});
                engine->finalizeObject();
                if(engine->hasError()||memory_observer->has_finalization_failure()||data->ranges_->has_observation_failure())return {{},status::native_failure};
                auto resolve=[&](::uwvm2::utils::container::std_string const& name)->::std::uintptr_t
                {
                    // Definitions were proved in THIS object, global searching
                    // is disabled, and this listener must own the exact entry.
                    auto const pointer=::uwvm2::runtime::compiler::llvm_jit::fast_io_support::function_address(*engine,name);
                    if(pointer==0u)return 0u;
                    auto const code=details::native_function_code_address(static_cast<::std::uintptr_t>(pointer));
                    return data->ranges_->owns_pending_loaded_function_entry(code)?static_cast<::std::uintptr_t>(pointer):0u;
                };
                for(auto& entry:data->entries_)
                {entry.typed=resolve(entry.typed_name);entry.raw=resolve(entry.raw_name);if(!entry.typed||!entry.raw)return {{},status::native_failure};}
                if(observing)
                {
                // Copy only metadata proved by this actual object/loader/arena.
                // No ordinary g_runtime publication, old epoch, source-row claim
                // or executable memory pointer is used as debugger authority.
                for(::std::size_t i{};i<count;++i)
                {
                    auto const& entry=data->entries_[i];auto const& body=module.local_defined_function_vec_storage.index_unchecked(i).wasm_code_ptr->body;
                    auto const begin=reinterpret_cast<::std::uintptr_t>(body.expr_begin)-base;
                    auto const end=reinterpret_cast<::std::uintptr_t>(body.code_end)-base;
                    for(unsigned adapter{};adapter!=2u;++adapter)
                    {
                        auto const address=details::native_function_code_address(adapter?entry.raw:entry.typed);
                        auto const extent=data->ranges_->threaded_debug_loaded_extent(address);
                        if(!extent)return {{},status::native_failure};
                        auto& row=data->debug_->functions_[i*2u+adapter];
                        auto const logical_begin=adapter?0u:row.logical_begin;
                        auto const logical_count=adapter?0u:row.logical_count;
                        row={module_index,data->imports_+i,begin,end,address,extent,logical_begin,logical_count,0u,adapter!=0u,false};
                        auto const& name=adapter?entry.raw_name:entry.typed_name;
                        for(auto const& frame:data->native_frames_)if(frame.name==name)
                        {row.fixed_native_frame_bytes=frame.object_bytes;row.fixed_native_frame_available=true;break;}
                    }
                }
                data->debug_->cfi_.resize(memory_observer->debug_cfi_count());
                memory_observer->copy_debug_cfi(data->debug_->cfi_);
                }
                if(!data->build_local_type_ids()||!data->refresh_table_targets_noalloc())return {{},status::unsupported_shape};
                // All object relocations and exact loaded entries are fixed.
                // No lazy function/provider remains. Cold LLVM owns no execution
                // resource: detach observer, drop loader, context and compiler
                // packets BEFORE final actual retained-request accounting.
                data->ranges_.reset();data->engine_.reset();data->context_.reset();
                ::uwvm2::utils::container::std_vector<local_type>{}.swap(data->locals_);
                ::uwvm2::utils::container::std_vector<native_bridge>{}.swap(data->emitted_bridges_);
                for(auto& entry:data->entries_)
                {::uwvm2::utils::container::std_string{}.swap(entry.typed_name);::uwvm2::utils::container::std_string{}.swap(entry.raw_name);}
                ::uwvm2::utils::container::std_vector<external>{}.swap(data->externals_);
                auto const types=data->build_reference_type_ids();
                if(types!=status::ok)return {{},types};
                auto const charged=data->account_retained_storage();
                if(charged!=status::ok)return {{},charged};
                return {::std::move(data),status::ok};
            }
            catch(::fast_io::error const&){return {{},status::invalid_body};}
            catch(::std::bad_alloc const&){return {{},status::budget};}
            catch(...){return {{},status::native_failure};}
            }, [&]() -> preparation_result { return {{},status::budget}; });
#endif
        }
#pragma pop_macro("STT_TLS")
#pragma pop_macro("SHT_CREL")
#pragma pop_macro("SHT_RELA")
#pragma pop_macro("SHT_REL")
#pragma pop_macro("SHT_SYMTAB")
