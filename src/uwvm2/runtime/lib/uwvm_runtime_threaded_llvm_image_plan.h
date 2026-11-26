// A group may remember a simple x86-64 RELA image only after matching an actual
// successful RuntimeDyld load. Cold observed addresses never enter the cache.
#pragma push_macro("SHF_ALLOC")
#undef SHF_ALLOC
#pragma push_macro("SHF_EXECINSTR")
#undef SHF_EXECINSTR
#pragma push_macro("SHF_WRITE")
#undef SHF_WRITE
#pragma push_macro("SHF_MERGE")
#undef SHF_MERGE
#pragma push_macro("SHF_X86_64_LARGE")
#undef SHF_X86_64_LARGE
#pragma push_macro("SHT_PROGBITS")
#undef SHT_PROGBITS
#pragma push_macro("SHT_X86_64_UNWIND")
#undef SHT_X86_64_UNWIND
#pragma push_macro("SHT_NULL")
#undef SHT_NULL
#pragma push_macro("SHT_STRTAB")
#undef SHT_STRTAB
#pragma push_macro("STB_GLOBAL")
#undef STB_GLOBAL
#pragma push_macro("STB_LOCAL")
#undef STB_LOCAL
#pragma push_macro("STT_NOTYPE")
#undef STT_NOTYPE
#pragma push_macro("STT_FUNC")
#undef STT_FUNC
#pragma push_macro("STT_SECTION")
#undef STT_SECTION
#pragma push_macro("STT_OBJECT")
#undef STT_OBJECT
#pragma push_macro("R_X86_64_64")
#undef R_X86_64_64
#pragma push_macro("R_X86_64_PC64")
#undef R_X86_64_PC64
        using image_addresses=::std::array<::std::uint8_t*,planned_section_limit>;
        using image_imports=::std::array<::std::uintptr_t,object_binding_limit>;
        struct image_observer final:public ::llvm::JITEventListener
        {
            ::llvm::ExecutionEngine* engine{};
            ::llvm::object::ObjectFile const* object{};
            image_addresses sections{};
            bool captured{};
            image_observer(::llvm::ExecutionEngine& owner,::llvm::object::ObjectFile const* candidate):object{candidate}
            {
#if defined(__linux__) && defined(__x86_64__) && __SIZEOF_POINTER__ == 8
                if(object){engine=&owner;engine->RegisterJITEventListener(this);}
#else
                static_cast<void>(owner);object=nullptr;
#endif
            }
            ~image_observer() override {if(engine)engine->UnregisterJITEventListener(this);}
            void notifyObjectLoaded(ObjectKey,::llvm::object::ObjectFile const& actual,::llvm::RuntimeDyld::LoadedObjectInfo const& loaded) override
            {
                if(&actual!=object)return;
                if(captured){object=nullptr;return;}
                for(auto const& section:actual.sections())
                {
                    auto const index=section.getIndex();
                    if(index>=sections.size()){object=nullptr;return;}
                    sections[index]=reinterpret_cast<::std::uint8_t*>(loaded.getSectionLoadAddress(section));
                }
                captured=true;
            }
        };
        [[nodiscard]] bool resolve_image_imports(prepared_object const& saved,image_imports& result) const noexcept
        {
            if(saved.imports.size()>result.size())return false;
            for(::std::size_t i{};i<saved.imports.size();++i)
            {
                for(auto const& entry:externals_)if(entry.name==saved.imports[i]){result[i]=entry.address;break;}
                if(!result[i])return false;
            }
            return true;
        }
        [[nodiscard]] static ::std::uint64_t image_word(image_fixup const& fix,image_addresses const& sections,image_imports const& imports) noexcept
        {
            if(fix.kind==fixup_kind::import)return imports[fix.target];
            auto const value=reinterpret_cast<::std::uintptr_t>(sections[fix.target])+fix.target_offset;
            return fix.kind==fixup_kind::relative?value-reinterpret_cast<::std::uintptr_t>(sections[fix.section]+fix.offset):value;
        }
        // On the first load every byte between relocation sites must be original
        // object data, and each relocation must reproduce LLVM's actual word.
        // Sorted, disjoint writes also rule out implicit paired fixups and stubs.
        [[nodiscard]] static bool match_image_bytes(image_plan const& plan,::llvm::StringRef object,image_addresses const& sections,image_imports const& imports) noexcept
        {
            auto fix=plan.fixups.begin();
            for(unsigned i{};i<plan.section_count;++i)
            {
                auto const& s=plan.sections[i];
                if(s.kind==image_section_kind::absent)continue;
                auto const* original=object.data()+s.file_offset;
                auto const* actual=sections[i];::std::size_t cursor{};
                for(;fix!=plan.fixups.end() && fix->section==i;++fix)
                {
                    if(fix->offset<cursor || ::std::memcmp(actual+cursor,original+cursor,fix->offset-cursor)!=0)return false;
                    auto const word=image_word(*fix,sections,imports);
                    if(::std::memcmp(actual+fix->offset,&word,sizeof(word))!=0)return false;
                    cursor=fix->offset+sizeof(word);
                }
                if(::std::memcmp(actual+cursor,original+cursor,s.size-cursor)!=0)return false;
            }
            return fix==plan.fixups.end();
        }
        template<typename T>
        [[nodiscard]] static bool image_read(::llvm::Expected<T>& value)
        {if(value)return true;::llvm::consumeError(value.takeError());return false;}
        [[nodiscard]] bool remember_image(prepared_object& saved,image_observer const& observed,::std::size_t limit) const
        {
            using kind=image_section_kind;
            auto const& object=*observed.object;
            auto const bytes=::uwvm2::runtime::compiler::llvm_jit::fast_io_support::ref(saved.object);
            if(!observed.captured || !object.isRelocatableObject() || object.getArch()!=::llvm::Triple::x86_64 ||
               !object.isLittleEndian() || object.getBytesInAddress()!=8u || object.getMemoryBufferRef().getBuffer()!=bytes ||
               bytes.size()>UINT32_MAX || saved.evidence.crel || saved.evidence.sections>planned_section_limit || saved.retained_bytes>limit)return false;
            auto const room=limit-saved.retained_bytes;
            if(sizeof(image_plan)>room || saved.evidence.relocations>(room-sizeof(image_plan))/sizeof(image_fixup))return false;
            auto plan=::uwvm2::utils::container::make_unique<image_plan>();
            plan->section_count=static_cast<unsigned>(saved.evidence.sections);
            plan->fixups.reserve(static_cast<::std::size_t>(saved.evidence.relocations));
            if(plan->fixups.capacity()>(room-sizeof(image_plan))/sizeof(image_fixup))return false;
            image_imports imports{};if(!resolve_image_imports(saved,imports))return false;
            for(auto const& section:object.sections())
            {
                auto const i=section.getIndex();if(i>=plan->section_count)return false;
                auto const* actual=observed.sections[i];
                auto const elf=::llvm::object::ELFSectionRef{section};
                auto const size=section.getSize();
                auto name=section.getName();if(!image_read(name))return false;
                auto const flags=elf.getFlags();auto const type=elf.getType();
                if(!(flags&::llvm::ELF::SHF_ALLOC))
                {
                    // Leave debug objects, other metadata and unknown section types to LLVM.
                    if(actual || !(type==::llvm::ELF::SHT_NULL || type==::llvm::ELF::SHT_RELA || type==::llvm::ELF::SHT_SYMTAB ||
                       type==::llvm::ELF::SHT_STRTAB || (type==::llvm::ELF::SHT_PROGBITS && (*name==".stack_sizes" || *name==".note.GNU-stack"))))return false;
                    continue;
                }
                auto family=[&](::llvm::StringRef stem){return *name==stem || (name->starts_with(stem) && (*name)[stem.size()]=='.');};
                bool const text=family(".text")||family(".ltext"),unwind=*name==".eh_frame";
                bool const data=family(".data")||family(".ldata"),rodata=family(".rodata")||family(".lrodata");
                constexpr auto supported_flags=static_cast<::std::uint64_t>(::llvm::ELF::SHF_ALLOC)|::llvm::ELF::SHF_WRITE|
                    ::llvm::ELF::SHF_EXECINSTR|::llvm::ELF::SHF_MERGE|::llvm::ELF::SHF_X86_64_LARGE;
                if((!text && !unwind && !rodata && !data) || (type!=::llvm::ELF::SHT_PROGBITS && !(unwind && type==::llvm::ELF::SHT_X86_64_UNWIND)) ||
                   (flags&~supported_flags) || ((flags&::llvm::ELF::SHF_EXECINSTR)!=0u)!=text ||
                   ((flags&::llvm::ELF::SHF_WRITE)!=0u)!=data || ((flags&::llvm::ELF::SHF_MERGE) && !rodata))return false;
                if(!actual){if(size)return false;continue;}
                auto const file_offset=elf.getOffset();auto const alignment=section.getAlignment().value();
                if(section.isVirtual() || section.isBSS() || section.getAddress()!=0u || file_offset>bytes.size() || size>bytes.size()-file_offset ||
                   !::std::has_single_bit(alignment) || alignment>native_arena_granule || reinterpret_cast<::std::uintptr_t>(actual)%alignment)return false;
                bool executable{},read_only{};
                if(!native_arena_->loaded_section(actual,size+(unwind?4u:0u),executable,read_only) || executable!=text || read_only==data)return false;
                if(unwind)
                {
                    constexpr ::std::uint32_t terminator{};
                    if(plan->unwind || !i || ::std::memcmp(actual+size,&terminator,sizeof(terminator))!=0)return false;
                    plan->unwind=static_cast<unsigned>(i);
                }
                plan->sections[i]={static_cast<::std::uint32_t>(file_offset),static_cast<::std::uint32_t>(size),static_cast<::std::uint32_t>(alignment),
                    unwind?kind::unwind:text?kind::code:read_only?kind::read_only:kind::writable};
            }
            auto const unwind=plan->unwind;
            if(!native_arena_->matches_unwind(unwind?observed.sections[unwind]:nullptr,unwind?plan->sections[unwind].size+4uz:0uz))return false;
            for(auto const& table:object.sections())
            {
                auto const type=::llvm::object::ELFSectionRef{table}.getType();
                if(type==::llvm::ELF::SHT_REL || type==::llvm::ELF::SHT_CREL)return false;
                if(type!=::llvm::ELF::SHT_RELA)continue;
                auto target=table.getRelocatedSection();if(!image_read(target) || *target==object.section_end())return false;
                auto const index=(*target)->getIndex();if(index>=plan->section_count)return false;
                auto const& dest=plan->sections[index];
                if(dest.kind==kind::absent)
                {
                    // Object admission separately proved every nonloaded stack-size record and relocation.
                    auto name=(*target)->getName();if(!image_read(name) || *name!=".stack_sizes")return false;
                    continue;
                }
                for(auto const& relocation:table.relocations())
                {
                    auto const offset=relocation.getOffset(),rel_type=relocation.getType();
                    bool const relative=rel_type==::llvm::ELF::R_X86_64_PC64;
                    if((!relative && rel_type!=::llvm::ELF::R_X86_64_64) || (relative && dest.kind!=kind::unwind) ||
                       offset>dest.size || sizeof(::std::uint64_t)>dest.size-offset || plan->fixups.size()==plan->fixups.capacity())return false;
                    // RuntimeDyld includes the in-place PC64 addend. This subset admits zero placeholders only.
                    constexpr ::std::uint64_t zero{};
                    if(::std::memcmp(bytes.data()+dest.file_offset+offset,&zero,sizeof(zero))!=0)return false;
                    auto addend=::llvm::object::ELFRelocationRef{relocation}.getAddend();if(!image_read(addend) || *addend<0)return false;
                    auto symbol=relocation.getSymbol();if(symbol==object.symbol_end())return false;
                    auto flags=symbol->getFlags();if(!image_read(flags))return false;
                    using sym=::llvm::object::SymbolRef;
                    auto const elf_symbol=::llvm::object::ELFSymbolRef{*symbol};
                    auto const symbol_kind=elf_symbol.getELFType(),binding=elf_symbol.getBinding();
                    if((*flags&(sym::SF_Common|sym::SF_Weak|sym::SF_Indirect|sym::SF_Absolute)) ||
                       (binding!=::llvm::ELF::STB_LOCAL && binding!=::llvm::ELF::STB_GLOBAL) ||
                       (symbol_kind!=::llvm::ELF::STT_SECTION && symbol_kind!=::llvm::ELF::STT_FUNC &&
                        symbol_kind!=::llvm::ELF::STT_NOTYPE && symbol_kind!=::llvm::ELF::STT_OBJECT))return false;
                    image_fixup fix{.offset=static_cast<::std::uint32_t>(offset),.section=static_cast<unsigned char>(index)};
                    if(*flags&sym::SF_Undefined)
                    {
                        if(relative || *addend || binding!=::llvm::ELF::STB_GLOBAL || symbol_kind!=::llvm::ELF::STT_NOTYPE)return false;
                        auto name=symbol->getName();if(!image_read(name))return false;
                        auto const found=::std::find_if(saved.imports.begin(),saved.imports.end(),[&](auto const& current){
                            return ::uwvm2::runtime::compiler::llvm_jit::fast_io_support::ref(current)==*name;});
                        if(found==saved.imports.end())return false;
                        fix.target=static_cast<::std::uint16_t>(found-saved.imports.begin());fix.kind=fixup_kind::import;
                    }
                    else
                    {
                        auto source=symbol->getSection();if(!image_read(source) || *source==object.section_end())return false;
                        auto const slot=(*source)->getIndex();if(slot>=plan->section_count || plan->sections[slot].kind==kind::absent)return false;
                        if(relative && plan->sections[slot].kind!=kind::code)return false;
                        auto value=symbol->getValue();if(!image_read(value))return false;
                        auto const extent=plan->sections[slot].size;
                        if(*value>extent || static_cast<::std::uint64_t>(*addend)>extent-*value)return false;
                        fix.target=static_cast<::std::uint16_t>(slot);fix.target_offset=static_cast<::std::uint32_t>(*value+static_cast<::std::uint64_t>(*addend));
                        fix.kind=relative?fixup_kind::relative:fixup_kind::absolute;
                    }
                    plan->fixups.push_back(fix);
                }
            }
            ::std::sort(plan->fixups.begin(),plan->fixups.end(),[](image_fixup const& a,image_fixup const& b){
                return a.section<b.section || (a.section==b.section && a.offset<b.offset);});
            if(!match_image_bytes(*plan,bytes,observed.sections,imports))return false;
            for(::std::size_t i{};i<saved.entries.size();++i)
            {
                auto matches=[&](::std::size_t idx,::std::uintptr_t entry){
                    if(idx>=saved.frames.size())return false;
                    auto const& f=saved.frames[idx];return f.section<plan->section_count && plan->sections[f.section].kind==kind::code &&
                        f.offset<plan->sections[f.section].size && entry==reinterpret_cast<::std::uintptr_t>(observed.sections[f.section]+f.offset);};
                if(!matches(saved.entries[i].typed,entries_[i].typed) || !matches(saved.entries[i].raw,entries_[i].raw))return false;
            }
            saved.retained_bytes+=sizeof(*plan)+plan->fixups.capacity()*sizeof(image_fixup);saved.image=::std::move(plan);return true;
        }
        [[nodiscard]] bool load_image(prepared_object const& saved)
        {
            using kind=image_section_kind;
            auto const& plan=*saved.image;
            image_imports imports{};if(!resolve_image_imports(saved,imports))return false;
            image_addresses sections{};
            for(unsigned i{};i<plan.section_count;++i)
            {
                auto const& s=plan.sections[i];if(s.kind==kind::absent)continue;
                auto const size=static_cast<::std::size_t>(s.size)+(s.kind==kind::unwind?4u:0u);
                sections[i]=s.kind==kind::code?native_arena_->allocateCodeSection(size,s.alignment,i,{}):
                    native_arena_->allocateDataSection(size,s.alignment,i,{},s.kind!=kind::writable);
                if(!sections[i])return false;
                ::std::memcpy(sections[i],saved.object.data()+s.file_offset,s.size);
                if(s.kind==kind::unwind)::std::memset(sections[i]+s.size,0,4u);
            }
            for(auto const& fix:plan.fixups)
            {
                auto const word=image_word(fix,sections,imports);
                ::std::memcpy(sections[fix.section]+fix.offset,&word,sizeof(word));
            }
            if(auto const i=plan.unwind)native_arena_->registerEHFrames(sections[i],reinterpret_cast<::std::uintptr_t>(sections[i]),plan.sections[i].size+4uz);
            if(native_arena_->finalizeMemory())return false;
            auto entry=[&](::std::size_t idx){auto const& f=saved.frames[idx];return reinterpret_cast<::std::uintptr_t>(sections[f.section]+f.offset);};
            for(::std::size_t i{};i<entries_.size();++i)
            {entries_[i].typed=entry(saved.entries[i].typed);entries_[i].raw=entry(saved.entries[i].raw);}
            return true;
        }
#pragma pop_macro("R_X86_64_PC64")
#pragma pop_macro("R_X86_64_64")
#pragma pop_macro("STT_OBJECT")
#pragma pop_macro("STT_SECTION")
#pragma pop_macro("STT_FUNC")
#pragma pop_macro("STT_NOTYPE")
#pragma pop_macro("STB_LOCAL")
#pragma pop_macro("STB_GLOBAL")
#pragma pop_macro("SHT_STRTAB")
#pragma pop_macro("SHT_NULL")
#pragma pop_macro("SHT_X86_64_UNWIND")
#pragma pop_macro("SHT_PROGBITS")
#pragma pop_macro("SHF_X86_64_LARGE")
#pragma pop_macro("SHF_MERGE")
#pragma pop_macro("SHF_WRITE")
#pragma pop_macro("SHF_EXECINSTR")
#pragma pop_macro("SHF_ALLOC")
