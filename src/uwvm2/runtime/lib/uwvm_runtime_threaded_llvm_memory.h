// ThreadedLlvmChild private member fragment: one admitted native code arena.
        class native_arena final:public ::llvm::RTDyldMemoryManager
#if defined(UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT) && UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT == 1 && defined(__linux__) && defined(__arm__) && !defined(__ARM_DWARF_EH__) && defined(__GLIBC__)
            ,private ::llvm::JITEventListener,private ::uwvm2::runtime::compiler::llvm_jit::details::arm_ehabi_registration_owner
#endif
        {
            struct section {::llvm::sys::MemoryBlock bytes;bool executable{},read_only{};
#if defined(UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT) && UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT == 1 && defined(__linux__) && defined(__arm__) && !defined(__ARM_DWARF_EH__) && defined(__GLIBC__)
                ::std::size_t allocated{},object_size{};bool index{},table{},authenticated{};
#endif
            };
            ::llvm::sys::MemoryBlock arena_{};
            ::std::vector<section> sections_{};
            ::std::array<::llvm::RTDyldMemoryManager::EHFrame,4096u> frames_{};
            ::std::size_t frame_count_{},used_{};
            ::std::vector<external> const* externals_{};
            bool failed_{},finalized_{};
            [[nodiscard]] ::std::uint8_t* allocate(::std::uintptr_t size,unsigned alignment,bool code,bool read_only,[[maybe_unused]] ::llvm::StringRef name) noexcept
            {
                // The complete real object was checked before loader creation:
                // <=4096 sections, no commons/TLS, alignment<=64KiB, plus bounded
                // native reloc/stub/GOT allowance. Unexpected request is a trusted
                // LLVM/contract failure, never an allocation-at-limit fallback.
                if(failed_||finalized_||!valid()||alignment>65536u||
                   (alignment!=0u&&!::std::has_single_bit(alignment))||sections_.size()==sections_.capacity())
                {failed_=true;return nullptr;}
                auto const base=reinterpret_cast<::std::uintptr_t>(arena_.base());
                constexpr ::std::size_t granule=65536u;
                if(base>UINTPTR_MAX-used_||base+used_>UINTPTR_MAX-(granule-1u)) {failed_=true;return nullptr;}
                auto const aligned=(base+used_+granule-1u)&~static_cast<::std::uintptr_t>(granule-1u);
                auto const offset=aligned-base;auto const bytes=(::std::max)(size,::std::uintptr_t{1u});
                if(offset>arena_.allocatedSize()||bytes>arena_.allocatedSize()-offset||bytes>SIZE_MAX-(granule-1u))
                {failed_=true;return nullptr;}
                auto const padded=(bytes+granule-1u)&~static_cast<::std::uintptr_t>(granule-1u);
                if(padded>arena_.allocatedSize()-offset){failed_=true;return nullptr;}
                used_=offset+padded;
                auto* pointer=reinterpret_cast<::std::uint8_t*>(aligned);
                sections_.push_back({::llvm::sys::MemoryBlock{pointer,padded},code,read_only});
#if defined(UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT) && UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT == 1 && defined(__linux__) && defined(__arm__) && !defined(__ARM_DWARF_EH__) && defined(__GLIBC__)
                auto& section=sections_.back();section.allocated=size;
                section.index=name==".ARM.exidx"||name.starts_with(".ARM.exidx.");
                section.table=name==".ARM.extab"||name.starts_with(".ARM.extab.");
                if((section.index||section.table)&&(code||!read_only))failed_=true;
#endif
                return pointer;
            }
#if defined(UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT) && UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT == 1 && defined(__linux__) && defined(__arm__) && !defined(__ARM_DWARF_EH__) && defined(__GLIBC__)
#include "uwvm_runtime_threaded_llvm_arm_ehabi.h"
#endif
        public:
            [[nodiscard]] static constexpr ::std::size_t unwind_storage_precharge([[maybe_unused]] ::std::size_t functions) noexcept
            {
#if defined(UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT) && UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT == 1 && defined(__linux__) && defined(__arm__) && !defined(__ARM_DWARF_EH__) && defined(__GLIBC__)
                return 8192u*sizeof(::uwvm2::runtime::compiler::llvm_jit::details::arm_ehabi_owned_table)+
                    (functions*2u+256u)*sizeof(::uwvm2::runtime::compiler::llvm_jit::details::arm_ehabi_owned_range);
#else
                return 0u;
#endif
            }
            native_arena(::std::size_t cap,::std::vector<external> const& bindings,[[maybe_unused]] ::std::size_t native_functions):externals_{&bindings}
            {
                // Acquire the ENTIRE finite code budget fallibly BEFORE LLVM
                // can load/relocate any object. No live guest calls this method.
                if(cap==0u||cap%65536u!=0u||::llvm::sys::Process::getPageSizeEstimate()>65536u)return;
                sections_.reserve(8192u);
#if defined(UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT) && UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT == 1 && defined(__linux__) && defined(__arm__) && !defined(__ARM_DWARF_EH__) && defined(__GLIBC__)
                arm_function_limit_=native_functions;
#endif
                ::std::error_code error;
                arena_=::llvm::sys::Memory::allocateMappedMemory(cap,nullptr,
                    ::llvm::sys::Memory::MF_READ|::llvm::sys::Memory::MF_WRITE,error);
                if(error||arena_.base()==nullptr||arena_.allocatedSize()!=cap)
                {if(arena_.base()!=nullptr)::llvm::sys::Memory::releaseMappedMemory(arena_);arena_={};}
            }
            [[nodiscard]] bool valid() const noexcept{return arena_.base()!=nullptr&&!failed_;}
            [[nodiscard]] bool has_finalization_failure() const noexcept{return failed_||!finalized_;}
            [[nodiscard]] ::std::size_t owned_bytes() const noexcept{return arena_.allocatedSize();}
            [[nodiscard]] ::std::size_t retained_storage_bytes() const noexcept
            {return sizeof(*this)+sections_.capacity()*sizeof(section)
#if defined(UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT) && UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT == 1 && defined(__linux__) && defined(__arm__) && !defined(__ARM_DWARF_EH__) && defined(__GLIBC__)
                +arm_registry_bytes_
#endif
                ;}
            [[nodiscard]] ::std::size_t debug_cfi_count() const noexcept{return frame_count_;}
            // Metadata only: bytes remain owned/registered by THIS arena until
            // actual Group join. No unwinder/parser is implied by these ranges.
            void copy_debug_cfi(::std::span<::uwvm2::runtime::wasm_threads::debug_cfi_row> out) const noexcept
            {
                if(out.size()!=frame_count_)return;
                for(::std::size_t i{};i!=frame_count_;++i)
                    out[i]={reinterpret_cast<::std::uintptr_t>(frames_[i].Addr),frames_[i].Size};
            }
            ::std::uint8_t* allocateCodeSection(::std::uintptr_t size,unsigned alignment,unsigned,::llvm::StringRef name) override
            {return allocate(size,alignment,true,true,name);}
            ::std::uint8_t* allocateDataSection(::std::uintptr_t size,unsigned alignment,unsigned,::llvm::StringRef name,bool ro) override
            {return allocate(size,alignment,false,ro,name);}
            bool finalizeMemory(::std::string* =nullptr) override
            {
                if(failed_||!valid())return true;
                if(finalized_)return false;
                for(auto const& section:sections_)
                {
                    unsigned flags=::llvm::sys::Memory::MF_READ;
                    if(section.executable)flags|=::llvm::sys::Memory::MF_EXEC;
                    else if(!section.read_only)flags|=::llvm::sys::Memory::MF_WRITE;
                    if(::llvm::sys::Memory::protectMappedMemory(section.bytes,flags))failed_=true;
                    if(section.executable)::llvm::sys::Memory::InvalidateInstructionCache(section.bytes.base(),section.bytes.allocatedSize());
                }
#if defined(UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT) && UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT == 1 && defined(__linux__) && defined(__arm__) && !defined(__ARM_DWARF_EH__) && defined(__GLIBC__)
                if(!failed_&&!publish_arm_tables())failed_=true;
#endif
                finalized_=!failed_;return failed_;
            }
            ::std::uint64_t getSymbolAddress(::std::string const& name) override
            {for(auto const& entry:*externals_)if(entry.name==name)return entry.address;return 0u;}
            void registerEHFrames(::std::uint8_t* address,::std::uint64_t,::std::size_t size) override
            {
                auto const base=reinterpret_cast<::std::uintptr_t>(arena_.base()),where=reinterpret_cast<::std::uintptr_t>(address);
                if(frame_count_==frames_.size()||where<base||where-base>arena_.allocatedSize()||size>arena_.allocatedSize()-(where-base))
                {failed_=true;return;}
                // Real compiler-generated native CFI only. Guest Wasm EH is not
                // silently admitted by registering this host unwind metadata.
                ::llvm::RTDyldMemoryManager::registerEHFramesInProcess(address,size);frames_[frame_count_++]={address,size};
            }
            void deregisterEHFrames() override
            {
#if defined(UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT) && UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT == 1 && defined(__linux__) && defined(__arm__) && !defined(__ARM_DWARF_EH__) && defined(__GLIBC__)
                retire_arm_ehabi_tables();arm_registry_bytes_=0u;
#endif
                while(frame_count_!=0u){auto const& frame=frames_[--frame_count_];
                    ::llvm::RTDyldMemoryManager::deregisterEHFramesInProcess(frame.Addr,frame.Size);}
            }
            ~native_arena() override
            {deregisterEHFrames();if(arena_.base()!=nullptr)::llvm::sys::Memory::releaseMappedMemory(arena_);}
        };

        class loader_memory_adapter final:public ::llvm::RTDyldMemoryManager
        {
            native_arena& actual_;
        public:
            explicit loader_memory_adapter(native_arena& actual) noexcept:actual_{actual}{}
#if defined(UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT) && UWVM2_ENABLE_LINUX_ARM_EHABI_PRODUCT == 1 && defined(__linux__) && defined(__arm__) && !defined(__ARM_DWARF_EH__) && defined(__GLIBC__)
            void notifyObjectLoaded(::llvm::ExecutionEngine* engine,::llvm::object::ObjectFile const&) override
            {actual_.observe_arm_engine(engine);}
#endif
            ::std::uint8_t* allocateCodeSection(::std::uintptr_t n,unsigned a,unsigned id,::llvm::StringRef name) override
            {return actual_.allocateCodeSection(n,a,id,name);}
            ::std::uint8_t* allocateDataSection(::std::uintptr_t n,unsigned a,unsigned id,::llvm::StringRef name,bool ro) override
            {return actual_.allocateDataSection(n,a,id,name,ro);}
            bool finalizeMemory(::std::string* error=nullptr) override{return actual_.finalizeMemory(error);}
            ::std::uint64_t getSymbolAddress(::std::string const& name) override{return actual_.getSymbolAddress(name);}
            void registerEHFrames(::std::uint8_t* address,::std::uint64_t loaded,::std::size_t size) override
            {actual_.registerEHFrames(address,loaded,size);}
            // Loaded native code/CFI belongs to backend, NOT this temporary
            // engine. Physical Group join precedes actual arena destruction.
            void deregisterEHFrames() override{}
        };
        [[nodiscard]] bool account_retained_storage() noexcept
        {
            if(!world_)return false;
            // Only import-only side modules have no executable allocation.
            // Every module with a local body must retain its genuine arena.
            if(!native_arena_ && (module_index_==0u||!entries_.empty()||native_bytes_!=0u||
               module_==nullptr||!module_->local_defined_function_vec_storage.empty()))return false;
            auto const source_cap=world_->admission().limits().max_child_storage_bytes;
            auto const world_bytes=world_->owned_storage_bytes();
            if(world_bytes>source_cap||storage_limit_>source_cap-world_bytes)return false;
            auto const cap=storage_limit_;
            ::std::size_t total=sizeof(*this);
            auto add=[&](::std::size_t bytes)
            {if(total>cap||bytes>cap-total)return false;total+=bytes;return true;};
            auto array=[&](::std::size_t count,::std::size_t bytes)
            {if(count>SIZE_MAX/bytes)return false;return add(count*bytes);};
            if(!array(gc_scratch_.capacity(),sizeof(::uwvm2::uwvm::runtime::storage::gc_object_value))||
               !array(entries_.capacity(),sizeof(entries))||!array(local_type_ids_.capacity(),sizeof(::std::uint_least32_t))||
               !array(memories_.capacity(),sizeof(memory_type*))||
               !array(table_targets_.capacity(),sizeof(decltype(table_targets_)::value_type))||
               !array(table_views_.capacity(),sizeof(decltype(table_views_)::value_type))||
               !array(externals_.capacity(),sizeof(external))||!array(emitted_bridges_.capacity(),sizeof(native_bridge))||
               !array(locals_.capacity(),sizeof(local_type))||(native_arena_&&!add(native_arena_->retained_storage_bytes())))return false;
            if(!array(eh_schemas_.capacity(),sizeof(eh_schema))||!array(eh_fields_.capacity(),sizeof(eh_field))||
               !array(eh_records_.capacity(),sizeof(eh_record))||!array(eh_payload_.capacity(),1u))return false;
            if(!array(native_frames_.capacity(),sizeof(native_frame_record)))return false;
            if(debug_&&(!add(sizeof(::uwvm2::runtime::wasm_threads::code_debug_image))||!add(debug_->allocated_bytes())))return false;
            for(auto const& frame:native_frames_)if(!add(frame.name.capacity()+1u))return false;
            for(auto const& entry:entries_)
                if(!add(entry.typed_name.capacity()+1u)||!add(entry.raw_name.capacity()+1u))return false;
            for(auto const& entry:externals_)if(!add(entry.name.capacity()+1u))return false;
            for(auto const& targets:table_targets_)if(!array(targets.capacity(),sizeof(target_type)))return false;
            storage_bytes_=total;return true;
        }
