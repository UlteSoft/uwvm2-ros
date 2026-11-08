// Licensed under the APL-2.0 License (see LICENSE file).
// Private arena member fragment. Only this exact MCJIT object's authenticated
// relocated sections can become native EHABI lookup authority.
            using arm_table=::uwvm2::runtime::compiler::llvm_jit::details::arm_ehabi_owned_table;
            ::llvm::ExecutionEngine* arm_engine_{}; // identity only; engine drains its listeners before arena retirement
            ::std::size_t arm_function_limit_{},arm_registry_bytes_{};
            [[nodiscard]] bool publish_arm_tables()
            {
                namespace native=::uwvm2::runtime::compiler::llvm_jit::details;
                if(!native::arm_ehabi_lookup_is_exported()||arm_engine_==nullptr)return false;
                ::std::size_t table_count{};
                for(auto const& s:sections_)
                {
                    if((s.executable||s.index||s.table)&&!s.authenticated)return false;
                    table_count+=s.index;
                }
                ::std::vector<arm_table> tables;tables.reserve(table_count);
                ::std::size_t records{};
                for(auto const& s:sections_)
                {
                    if(!s.index)continue;
                    auto const address=reinterpret_cast<::std::uintptr_t>(s.bytes.base());
                    if(s.executable||s.object_size%8u||address%4u||s.object_size/8u>static_cast<unsigned>(INT_MAX))return false;
                    auto const count=s.object_size/8u;
                    if(records>arm_function_limit_||count>arm_function_limit_-records)return false;
                    records+=count;
                    arm_table table{reinterpret_cast<::std::uint32_t const*>(address),static_cast<unsigned>(count),{}};
                    table.functions.reserve(count);
                    ::std::uintptr_t previous{};
                    for(unsigned i{};i<table.count;++i)
                    {
                        auto const* entry=table.entries+i*2u;
                        auto const start=native::arm_ehabi_prel31(entry)&~::std::uintptr_t{1u};
                        if(!start||(i&&start<=previous))return false;
                        previous=start;section const* allocation{};
                        for(auto const& owned:sections_)
                        {
                            auto const begin=reinterpret_cast<::std::uintptr_t>(owned.bytes.base());
                            if(owned.executable&&start>=begin&&start-begin<owned.object_size)
                            {if(allocation)return false;allocation=&owned;}
                        }
                        if(!allocation)return false;
                        auto end=reinterpret_cast<::std::uintptr_t>(allocation->bytes.base())+allocation->object_size;
                        if(i+1u<table.count)
                        {auto const next=native::arm_ehabi_prel31(entry+2u)&~::std::uintptr_t{1u};
                         if(next<=start)return false;end=(::std::min)(end,next);}
                        ::std::uint32_t content{};::std::memcpy(&content,entry+1u,4u);
                        if((content&0x80000000u)!=0u&&!native::arm_ehabi_unwind_extent_valid(entry+1u,4u,true))return false;
                        if(content!=1u&&(content&0x80000000u)==0u)
                        {
                            auto const extab=native::arm_ehabi_prel31(entry+1u);::std::size_t available{};
                            for(auto const& owned:sections_)
                            {auto const begin=reinterpret_cast<::std::uintptr_t>(owned.bytes.base());
                             if(owned.table&&extab>=begin&&extab-begin<owned.object_size)
                             {if(available)return false;available=owned.object_size-(extab-begin);}}
                            if(extab%4u||!native::arm_ehabi_unwind_extent_valid(reinterpret_cast<void const*>(extab),available,false))return false;
                            auto const* word=reinterpret_cast<::std::uint32_t const*>(extab);
                            ::std::uint32_t encoding{};::std::memcpy(&encoding,word,4u);
                            if((encoding&0x80000000u)==0u)
                            {
                                auto const personality=native::arm_ehabi_prel31(word)&~::std::uintptr_t{1u};bool executable{};
                                for(auto const& owned:sections_)
                                {auto const begin=reinterpret_cast<::std::uintptr_t>(owned.bytes.base());
                                 if(owned.executable&&personality>=begin&&personality-begin<owned.object_size)executable=true;}
                                if(!executable)return false;
                            }
                        }
                        table.functions.push_back({start,end-start});
                    }
                    tables.push_back(::std::move(table));
                }
                if(!records)return false;
                auto bytes=tables.capacity()*sizeof(arm_table);
                for(auto const& table:tables)bytes+=table.functions.capacity()*sizeof(native::arm_ehabi_owned_range);
                if(!publish_arm_ehabi_tables(::std::move(tables)))return false;
                arm_registry_bytes_=bytes;return true;
            }
        public:
            using ::llvm::RTDyldMemoryManager::notifyObjectLoaded;
            void observe_arm_engine(::llvm::ExecutionEngine* engine)
            {
                if(engine==nullptr||(arm_engine_&&arm_engine_!=engine)){failed_=true;return;}
                if(arm_engine_)return;
                arm_engine_=engine;engine->RegisterJITEventListener(this);
            }
            void notifyObjectLoaded(::llvm::JITEventListener::ObjectKey,::llvm::object::ObjectFile const& object,
                ::llvm::RuntimeDyld::LoadedObjectInfo const& loaded) override
            {
                if(!object.isELF()||object.getArch()!=::llvm::Triple::arm||!object.isLittleEndian()||object.getBytesInAddress()!=4u)
                {failed_=true;return;}
                for(auto const& incoming:object.sections())
                {
                    auto name=incoming.getName();if(!name){::llvm::consumeError(name.takeError());failed_=true;return;}
                    bool const index=*name==".ARM.exidx"||name->starts_with(".ARM.exidx.");
                    bool const table=*name==".ARM.extab"||name->starts_with(".ARM.extab.");
                    if(!incoming.isText()&&!index&&!table)continue;
                    auto const size=incoming.getSize(),address=loaded.getSectionLoadAddress(incoming);
                    if(!size)continue;
                    section* pending{};
                    for(auto& owned:sections_)
                        if(!owned.authenticated&&reinterpret_cast<::std::uintptr_t>(owned.bytes.base())==address)
                        {if(pending){failed_=true;return;}pending=&owned;}
                    if(!pending||size>pending->allocated||pending->executable!=incoming.isText()||pending->index!=index||pending->table!=table)
                    {failed_=true;return;}
                    pending->object_size=static_cast<::std::size_t>(size);pending->authenticated=true;
                }
            }
