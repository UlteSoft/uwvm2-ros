// Private ELF admission. Object-relative facts belong to the same group-owned
// byte string as their codegen witness. Loaded sections and entries belong to
// the current worker and are checked by its RuntimeDyld listener after relocation.
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
        [[nodiscard]] preparation_status reject_object(object_evidence const& evidence,::std::size_t relocation_limit,
            bool diagnose,char const* reason,preparation_status why=preparation_status::native_failure) const
        {
            if(diagnose)::fast_io::perrln("uwvm: [error] WASI Threads LLVM object rejected in module ",module_index_,
                " (",::fast_io::mnp::os_c_str(reason),") totals-complete=",static_cast<unsigned>(evidence.complete),
                " sections=",evidence.sections," relocations=",evidence.relocations," symbols=",evidence.symbols,
                " relocation-limit=",relocation_limit," charged-bytes=",evidence.charge,
                " arena-bytes=",evidence.arena_bytes," code-limit=",code_limit_);
            return why;
        }
        [[nodiscard]] preparation_status check_object_limits(object_evidence const& evidence,::std::size_t relocation_limit,bool diagnose) const
        {
            using status=preparation_status;
            auto reject=[&](char const* why,status result){return reject_object(evidence,relocation_limit,diagnose,why,result);};
            if(evidence.relocations>relocation_limit)return reject("relocation count exceeds host limit",status::budget);
            if(evidence.symbols>maximum_object_symbols)return reject("symbol count exceeds 262144",status::budget);
            if(evidence.arena_bytes>code_limit_)return reject("native arena byte charge",status::budget);
            return status::ok;
        }
        [[nodiscard]] static bool check_compressed_relocations(::llvm::object::ObjectFile const& object,object_evidence const& evidence)
        {
            if(evidence.crel)
            {
                auto const& native=::llvm::cast<::llvm::object::ELFObjectFileBase>(object);
                for(auto const& section:object.sections())
                    if(::llvm::object::ELFSectionRef{section}.getType()==::llvm::ELF::SHT_CREL)
                    {
                        // LLVM materializes CREL entries on first use. Decode only after
                        // the declared count and its full byte charge have been admitted.
                        static_cast<void>(section.relocation_begin());
                        if(!native.getCrelDecodeProblem(section).empty())return false;
                    }
            }
            return true;
        }
        [[nodiscard]] preparation_status bind_object_import(::llvm::StringRef name,object_import_mask* required,bool diagnose)
        {
            using status=preparation_status;
            ::std::size_t index{};
            for(auto const& binding:externals_)
            {
                if(binding.name==name){if(!binding.address)return status::native_failure;break;}
                ++index;
            }
            if(index==externals_.size())
            {
                // Only these native helpers may supplement the requesting world's emitter bindings.
                ::std::uintptr_t pointer{};
                if(name=="memcpy")pointer=address(static_cast<void*(*)(void*,void const*,::std::size_t)>(&::std::memcpy));
                else if(name=="memmove")pointer=address(static_cast<void*(*)(void*,void const*,::std::size_t)>(&::std::memmove));
                else if(name=="memset")pointer=address(static_cast<void*(*)(void*,int,::std::size_t)>(&::std::memset));
#if defined(__linux__) && (defined(__i386__) || defined(__arm__))
                // Includes scalar rounding and square root on ARM EABI.
                else pointer=::uwvm2::runtime::compiler::llvm_jit::details::native_compiler_builtins::find(::uwvm2::utils::container::copy_string(name));
#endif
#if defined(UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT) && UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT == 1 && \
    defined(__linux__) && defined(__arm__) && !defined(__ARM_DWARF_EH__) && defined(__GLIBC__)
                // Native backend-generated compact unwind relocations,
                // paired with the same static libunwind as this process.
                if(name=="__aeabi_unwind_cpp_pr0")pointer=reinterpret_cast<::std::uintptr_t>(&::__aeabi_unwind_cpp_pr0);
                else if(name=="__aeabi_unwind_cpp_pr1")pointer=reinterpret_cast<::std::uintptr_t>(&::__aeabi_unwind_cpp_pr1);
                else if(name=="__aeabi_unwind_cpp_pr2")pointer=reinterpret_cast<::std::uintptr_t>(&::__aeabi_unwind_cpp_pr2);
#endif
                if(!pointer)
                {
                    auto const spelling=name.take_front(128u);
                    if(diagnose)::fast_io::perrln("uwvm: [error] WASI Threads LLVM unbound native symbol ",
                        ::fast_io::mnp::strvw(spelling.begin(),spelling.end()));
                    return status::native_failure;
                }
                if(index>=object_binding_limit)return status::budget;
                externals_.push_back({::uwvm2::utils::container::copy_string(name),pointer});
            }
            if(index>=object_binding_limit)return status::budget;
            if(required)(*required)[index]=true;
            return status::ok;
        }
        [[nodiscard]] preparation_status check_object_stack(object_evidence const& evidence,::std::size_t relocation_limit,bool diagnose)
        {
            if(!apply_object_stack_shape(evidence.stack))
                return reject_object(evidence,relocation_limit,diagnose,"native stack witness");
            if(guest_fixed_stack_charge_>guest_native_stack_usable_bytes_)
            {
                if(diagnose)report_native_stack_budget("object");
                return reject_object(evidence,relocation_limit,diagnose,"native stack budget",preparation_status::budget);
            }
            return preparation_status::ok;
        }
        [[nodiscard]] preparation_status check_object(::llvm::object::ObjectFile const& object,::std::size_t relocation_limit,
            object_evidence& evidence,object_import_mask& required,native_frame_index& by_name,bool diagnose)
        {
            using status=preparation_status;
            auto reject=[&](char const* why,status result=status::native_failure){return reject_object(evidence,relocation_limit,diagnose,why,result);};
            auto& sections=evidence.sections;auto& relocations=evidence.relocations;auto& symbols=evidence.symbols;
            auto& charge=evidence.charge;auto& arena_bytes=evidence.arena_bytes;
            if(!object.isELF()||object.getArch()!=native_object_arch||!object.isLittleEndian()||object.getBytesInAddress()!=native_address_bytes)
                return reject("native ELF target");
            auto const& elf=::llvm::cast<::llvm::object::ELFObjectFile<native_elf_layout>>(object).getELFFile();
            auto headers=elf.sections();
            if(!headers){::llvm::consumeError(headers.takeError());return reject("section headers");}
            sections=headers->size();
            if(sections>maximum_object_sections)return reject("section count exceeds 4096",status::budget);
            charge+=sections*2u*native_arena_granule;
            bool saw_symtab{};
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
                evidence.crel|=section.sh_type==::llvm::ELF::SHT_CREL;
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
            evidence.complete=true;
            auto const limits=check_object_limits(evidence,relocation_limit,diagnose);
            if(limits!=status::ok)return limits;
            if(!check_compressed_relocations(object,evidence))return reject("compressed relocation table");
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
                    auto const bound=bind_object_import(*name,&required,diagnose);
                    if(bound!=status::ok)return reject(bound==status::budget?"external binding count exceeds 256":"unbound native symbol",bound);
                }
            }
            if(!check_static_stack_object(object,by_name,evidence.stack))return reject("native stack witness");
            // The machine/object proof is a bijection onto defined ELF functions.
            for(auto const& entry:entries_)
                if(!by_name.contains(::uwvm2::runtime::compiler::llvm_jit::fast_io_support::ref(entry.typed_name)) ||
                   !by_name.contains(::uwvm2::runtime::compiler::llvm_jit::fast_io_support::ref(entry.raw_name)))return reject("entry definition");
            return check_object_stack(evidence,relocation_limit,diagnose);
        }
        [[nodiscard]] preparation_status check_cached_object(::llvm::object::ObjectFile const& object,prepared_object const& saved,
            ::std::size_t relocation_limit,bool diagnose)
        {
            using status=preparation_status;
            auto const& evidence=saved.evidence;
            auto const bytes=object.getMemoryBufferRef().getBuffer();
            auto const owned=::uwvm2::runtime::compiler::llvm_jit::fast_io_support::ref(saved.object);
            if(!evidence.complete || bytes.size()!=owned.size() || (bytes.data()!=owned.data() && bytes!=owned))
                return status::native_failure;
            auto const limits=check_object_limits(evidence,relocation_limit,diagnose);
            if(limits!=status::ok)return limits;
            // LLVM's lazy CREL decoder belongs to this ObjectFile. Admit before it allocates.
            if(!check_compressed_relocations(object,evidence))
                return reject_object(evidence,relocation_limit,diagnose,"compressed relocation table");
            for(auto const& name:saved.imports)
            {
                auto const bound=bind_object_import(::uwvm2::runtime::compiler::llvm_jit::fast_io_support::ref(name),nullptr,diagnose);
                if(bound!=status::ok)return reject_object(evidence,relocation_limit,diagnose,
                    bound==status::budget?"external binding count exceeds 256":"unbound native symbol",bound);
            }
            if(entries_.size()!=saved.entries.size())return status::native_failure;
            for(::std::size_t i{};i<entries_.size();++i)
            {
                auto const& indices=saved.entries[i];
                if(indices.typed>=saved.frames.size() || indices.raw>=saved.frames.size() ||
                   entries_[i].typed_name!=saved.frames[indices.typed].name || entries_[i].raw_name!=saved.frames[indices.raw].name)
                    return status::native_failure;
            }
            return check_object_stack(evidence,relocation_limit,diagnose);
        }
