// Preserve native ELF macros while using LLVM enumerators in this member fragment.
#pragma push_macro("SHT_RELA")
#pragma push_macro("R_X86_64_64")
#pragma push_macro("SHT_REL")
#pragma push_macro("R_386_32")
#pragma push_macro("R_AARCH64_ABS64")
#pragma push_macro("R_ARM_ABS32")
#undef SHT_RELA
#undef R_X86_64_64
#undef SHT_REL
#undef R_386_32
#undef R_AARCH64_ABS64
#undef R_ARM_ABS32
// PRIVATE actual generated-object fixed-frame evidence. SOURCE ONLY.
// This does NOT certify libLLVM compiler-worker or Native import call chains.
        // Object witnesses use the native target ABI, never a guest-selected
        // machine, relocation kind or presumed pointer width.
#if defined(__x86_64__) && __SIZEOF_POINTER__ == 8
        static constexpr auto native_object_arch=::llvm::Triple::x86_64;
        static constexpr unsigned native_stack_relocation=::llvm::ELF::R_X86_64_64;
        static constexpr ::std::size_t native_call_push=8u;
        static constexpr bool native_stack_rela=true;
        static constexpr ::std::string_view native_printer="X86 Assembly Printer";
#elif defined(__i386__) && __SIZEOF_POINTER__ == 4
        static constexpr auto native_object_arch=::llvm::Triple::x86;
        static constexpr unsigned native_stack_relocation=::llvm::ELF::R_386_32;
        static constexpr ::std::size_t native_call_push=4u;
        static constexpr bool native_stack_rela=false;
        static constexpr ::std::string_view native_printer="X86 Assembly Printer";
#elif defined(__aarch64__) && __SIZEOF_POINTER__ == 8
        static constexpr auto native_object_arch=::llvm::Triple::aarch64;
        static constexpr unsigned native_stack_relocation=::llvm::ELF::R_AARCH64_ABS64;
        static constexpr ::std::size_t native_call_push=0u; // BL writes LR; its saved slot is in MFI.
        static constexpr bool native_stack_rela=true;
        static constexpr ::std::string_view native_printer="AArch64 Assembly Printer";
#elif defined(__arm__) && __SIZEOF_POINTER__ == 4
        static constexpr auto native_object_arch=::llvm::Triple::arm;
        static constexpr unsigned native_stack_relocation=::llvm::ELF::R_ARM_ABS32;
        static constexpr ::std::size_t native_call_push=0u; // BL writes LR, for AAPCS soft and hard float.
        static constexpr bool native_stack_rela=false;
        static constexpr ::std::string_view native_printer="ARM Assembly Printer";
#else
        static constexpr auto native_object_arch=::llvm::Triple::UnknownArch;
        static constexpr unsigned native_stack_relocation=0u;
        static constexpr ::std::size_t native_call_push=0u;
        static constexpr bool native_stack_rela=false;
        static constexpr ::std::string_view native_printer{};
#endif
        static constexpr unsigned native_address_bytes=sizeof(void*);
#if defined(__arm__) && __SIZEOF_POINTER__ == 4
        // AAPCS stack alignment is eight; fixed v128 spills require sixteen.
        // The target's one downward alignment can consume at most 15 bytes.
        static constexpr bool native_fixed_v128_realign=true;
#else
        static constexpr bool native_fixed_v128_realign=false;
#endif
        struct native_frame_record
        {
            ::std::string name;
            ::std::uint64_t section{UINT64_MAX},offset{UINT64_MAX},bytes{},object_bytes{UINT64_MAX},alignment_slack{};
        };
        struct native_call_edge {::std::size_t caller{},callee{};};
        struct acyclic_bound {bool valid{},acyclic{};::std::size_t bytes{};};
        // Pure cold proof over exact object/MFI frame costs. Acyclic call chains
        // cannot repeat their largest frame at every configured recursion depth.
        // Unknown indirect targets and recursive graphs retain the original
        // depth envelope; this never relaxes the final machine/object witness.
        [[nodiscard]] static acyclic_bound prove_acyclic_stack(
            ::std::span<::std::size_t const> costs,::std::span<native_call_edge const> edges)
        {
            if(costs.empty()||costs.size()==SIZE_MAX)return {};
            ::std::vector<::std::size_t> outgoing(costs.size()),offsets(costs.size()+1u),pending;
            for(auto const& edge:edges)
            {
                if(edge.caller>=costs.size()||edge.callee>=costs.size()||
                   outgoing[edge.caller]==SIZE_MAX||offsets[edge.callee+1u]==SIZE_MAX)return {};
                ++outgoing[edge.caller];++offsets[edge.callee+1u];
            }
            for(::std::size_t i{1u};i<offsets.size();++i)
            {if(offsets[i]>SIZE_MAX-offsets[i-1u])return {};offsets[i]+=offsets[i-1u];}
            ::std::vector<::std::size_t> parents(edges.size()),positions(offsets),bounds(costs.begin(),costs.end());
            for(auto const& edge:edges)parents[positions[edge.callee]++]=edge.caller;
            for(::std::size_t i{};i<costs.size();++i)if(outgoing[i]==0u)pending.push_back(i);
            ::std::size_t consumed{},maximum{};
            while(!pending.empty())
            {
                auto const node=pending.back();pending.pop_back();++consumed;
                maximum=(::std::max)(maximum,bounds[node]);
                for(auto at=offsets[node];at!=offsets[node+1u];++at)
                {
                    auto const parent=parents[at];
                    if(bounds[node]>SIZE_MAX-costs[parent])return {};
                    bounds[parent]=(::std::max)(bounds[parent],costs[parent]+bounds[node]);
                    if(--outgoing[parent]==0u)pending.push_back(parent);
                }
            }
            return {true,consumed==costs.size(),maximum};
        }
        ::std::vector<::std::string> native_call_names_{};
        ::std::vector<native_call_edge> native_call_edges_{};
        bool native_call_graph_known_{};
        [[nodiscard]] bool record_native_call_graph(::llvm::Module const& module)
        {
            native_call_names_.clear();native_call_edges_.clear();native_call_graph_known_=true;
            ::llvm::DenseMap<::llvm::Function const*,::std::size_t> indices;
            for(auto const& function:module)if(!function.isDeclaration())
            {
                auto const index=native_call_names_.size();
                if(!indices.try_emplace(::std::addressof(function),index).second)return false;
                native_call_names_.push_back(function.getName().str());
            }
            if(native_call_names_.empty())return false;
            ::std::size_t caller{};
            for(auto const& function:module)
            {
                if(function.isDeclaration())continue;
                for(auto const& block:function)for(auto const& instruction:block)
                    if(auto const* call=::llvm::dyn_cast<::llvm::CallBase>(&instruction))
                    {
                        if(call->isInlineAsm())continue; // Already restricted to the empty memory-clobber barrier.
                        auto const* callee=::llvm::dyn_cast<::llvm::Function>(call->getCalledOperand()->stripPointerCasts());
                        if(callee==nullptr){native_call_graph_known_=false;continue;}
                        if(callee->isDeclaration())continue; // Audited native host bridges have a separate stack boundary.
                        auto const found=indices.find(callee);
                        if(found==indices.end())return false;
                        native_call_edges_.push_back({caller,found->second});
                    }
                ++caller;
            }
            return caller==native_call_names_.size();
        }
        ::std::vector<native_frame_record> native_frames_{};
        ::std::size_t maximum_fixed_frame_{},guest_fixed_stack_charge_{};
        ::std::size_t guest_native_stack_usable_bytes_{},compiler_native_stack_usable_bytes_{};
        bool machine_stack_valid_{true};
        // This pass is inserted immediately BEFORE the trusted native printer in
        // THIS private native pipeline. It observes final, post-PEI machine
        // frames; it neither changes ordinary pipelines nor estimates from IR.
        class machine_stack_observer final:public ::llvm::MachineFunctionPass
        {
            ThreadedLlvmChild& owner_;
            inline static char pass_id_{};
        public:
            explicit machine_stack_observer(ThreadedLlvmChild& owner):MachineFunctionPass(pass_id_),owner_{owner}{}
            ::llvm::StringRef getPassName() const override{return "UWVM threaded final machine frame witness";}
            void getAnalysisUsage(::llvm::AnalysisUsage& usage) const override
            {MachineFunctionPass::getAnalysisUsage(usage);usage.setPreservesAll();}
            bool runOnMachineFunction(::llvm::MachineFunction& function) override
            {
                auto& owner=owner_;auto const& frame=function.getFrameInfo();
                auto const* registers=function.getSubtarget().getRegisterInfo();
                auto const* lowering=function.getSubtarget().getFrameLowering();
                auto const cap=owner.entries_.size()*2u+256u;
                auto const name=function.getName();
                if(!owner.machine_stack_valid_)return false;
                if(registers==nullptr||lowering==nullptr||name.empty()||name.size()>256u||
                   !function.getProperties().hasProperty(::llvm::MachineFunctionProperties::Property::NoVRegs)||
                   !function.getFunction().hasFnAttribute(::llvm::Attribute::NoRedZone)||
                   frame.hasVarSizedObjects()||frame.hasOpaqueSPAdjustment()||frame.getUnsafeStackSize()!=0u||
                   frame.getMaxAlign().value()>16u||
                   (registers->hasStackRealignment(function)&&(!native_fixed_v128_realign||frame.getMaxAlign().value()!=16u))||
                   !lowering->hasReservedCallFrame(function)||
                   owner.native_frames_.size()==cap)
                {owner.machine_stack_valid_=false;return false;}
                for(auto const& old:owner.native_frames_)if(old.name==name)
                {owner.machine_stack_valid_=false;return false;}
                owner.native_frames_.push_back({name.str(),UINT64_MAX,UINT64_MAX,frame.getStackSize(),UINT64_MAX,
                    registers->hasStackRealignment(function)?frame.getMaxAlign().value()-1u:0u});
                return false;
            }
        };
        class observed_native_pass_manager final:public ::llvm::legacy::PassManager
        {
            ThreadedLlvmChild& owner_;
            ::std::size_t printers_{};
        public:
            explicit observed_native_pass_manager(ThreadedLlvmChild& owner):owner_{owner}{}
            void add(::llvm::Pass* pass) override
            {
                // Exact source-bound LLVM target pipeline stage, not a Guest name
                // or external callback authority. An unknown/renamed pipeline
                // fails coverage; no metadata-only success is manufactured.
                if(pass->getPassName()==::llvm::StringRef{native_printer.data(),native_printer.size()})
                {++printers_;::llvm::legacy::PassManager::add(new machine_stack_observer{owner_});}
                ::llvm::legacy::PassManager::add(pass);
            }
            [[nodiscard]] bool valid() const noexcept
            {return printers_==1u&&owner_.machine_stack_valid_&&!owner_.native_frames_.empty();}
        };
        [[nodiscard]] bool check_static_stack_ir(::llvm::Module& module) noexcept
        {
            for(auto& function:module)
            {
                if(function.isDeclaration())continue;
                function.addFnAttr(::llvm::Attribute::NoRedZone);
                if(function.hasFnAttribute(::llvm::Attribute::Naked)||function.hasFnAttribute("split-stack"))return false;
                for(auto& block:function)for(auto& instruction:block)
                {
                    if(auto alloca=::llvm::dyn_cast<::llvm::AllocaInst>(&instruction))
                        if(!alloca->isStaticAlloca()||alloca->getAlign().value()>16u)return false;
                    if(auto call=::llvm::dyn_cast<::llvm::CallBase>(&instruction))
                    {
                        if(call->hasFnAttr(::llvm::Attribute::ReturnsTwice))return false;
                        auto callee=call->getCalledFunction();if(callee==nullptr)continue;
                        auto name=callee->getName();
                        if(name.starts_with("llvm.stacksave")||name.starts_with("llvm.stackrestore")||
                           name.starts_with("llvm.coro.")||name.starts_with("llvm.localescape")||
                           name.starts_with("llvm.localrecover"))return false;
                    }
                }
            }
            return true;
        }
        [[nodiscard]] bool check_static_stack_object(::llvm::object::ObjectFile const& object)
        {
            if(!machine_stack_valid_||native_frames_.empty())return false;
            auto cap=entries_.size()*2u+256u;
            ::std::size_t actual_functions{};
            for(auto const& symbol:object.symbols())
            {
                auto flags=symbol.getFlags();if(!flags){::llvm::consumeError(flags.takeError());return false;}
                if(*flags&::llvm::object::SymbolRef::SF_Undefined)continue;
                auto type=symbol.getType();if(!type){::llvm::consumeError(type.takeError());return false;}
                if(*type!=::llvm::object::SymbolRef::ST_Function)continue;
                auto name=symbol.getName();auto section=symbol.getSection();auto value=symbol.getValue();
                if(!name){::llvm::consumeError(name.takeError());return false;}
                if(!section){::llvm::consumeError(section.takeError());return false;}
                if(!value){::llvm::consumeError(value.takeError());return false;}
                if(name->empty()||name->size()>256u||*section==object.section_end()||!(*section)->isText()||++actual_functions>cap)return false;
                native_frame_record* found{};
                for(auto& frame:native_frames_)if(frame.name==*name)
                {if(found!=nullptr||frame.section!=UINT64_MAX)return false;found=::std::addressof(frame);}
                if(found==nullptr)return false; // Every actual body must have a final MFI witness.
                found->section=(*section)->getIndex();found->offset=*value;
            }
            if(actual_functions!=native_frames_.size())return false;
            for(auto const& frame:native_frames_)if(frame.section==UINT64_MAX)return false;
            ::std::size_t records{};
            for(auto const& section:object.sections())
            {
                auto name=section.getName();if(!name){::llvm::consumeError(name.takeError());return false;}
                if(*name!=".stack_sizes")continue;
                auto data=section.getContents();if(!data){::llvm::consumeError(data.takeError());return false;}
                struct raw_record{::std::uint64_t offset{},bytes{},implicit_addend{};bool relocated{};};
                ::std::vector<raw_record> parsed;
                ::std::size_t at{};
                while(at<data->size())
                {
                    auto start=at;if(data->size()-at<native_address_bytes)return false;
                    // ELF64 uses explicit RELA addends; ELF32 uses the checked
                    // in-place REL addend. Both resolve to an exact text symbol.
                    ::std::uint64_t implicit{};
                    for(unsigned i{};i<native_address_bytes;++i)
                        implicit|=static_cast<::std::uint64_t>(static_cast<unsigned char>((*data)[at+i]))<<(8u*i);
                    if(native_stack_rela&&implicit!=0u)return false;
                    at+=native_address_bytes;::std::uint64_t bytes{};unsigned shift{};bool ended{};
                    for(unsigned i{};i<10u&&at<data->size();++i)
                    {
                        auto value=static_cast<unsigned char>((*data)[at++]);
                        if(shift==63u&&(value&0x7fu)>1u)return false;
                        bytes|=static_cast<::std::uint64_t>(value&0x7fu)<<shift;
                        if(!(value&0x80u)){ended=true;break;}shift+=7u;
                    }
                    if(!ended||bytes>SIZE_MAX||++records>cap)return false;
                    parsed.push_back({start,bytes,implicit,false});
                }
                ::std::size_t relocation_sections{};
                for(auto const& relocation_section:object.sections())
                {
                    auto target=relocation_section.getRelocatedSection();
                    if(!target){::llvm::consumeError(target.takeError());return false;}
                    if(*target==object.section_end()||(*target)->getIndex()!=section.getIndex())continue;
                    if(++relocation_sections!=1u||::llvm::object::ELFSectionRef{relocation_section}.getType()!=
                        (native_stack_rela?::llvm::ELF::SHT_RELA : ::llvm::ELF::SHT_REL))return false;
                    for(auto const& relocation:relocation_section.relocations())
                    {
                        if(relocation.getType()!=native_stack_relocation)return false;
                        raw_record* record{};
                        for(auto& candidate:parsed)if(candidate.offset==relocation.getOffset())
                        {if(record!=nullptr)return false;record=&candidate;}
                        if(record==nullptr||record->relocated)return false;
                        auto symbol=relocation.getSymbol();if(symbol==object.symbol_end())return false;
                        auto symbol_section=symbol->getSection();auto value=symbol->getValue();
                        if(!symbol_section){::llvm::consumeError(symbol_section.takeError());return false;}
                        if(!value){::llvm::consumeError(value.takeError());return false;}
                        auto addend=record->implicit_addend;
                        if constexpr(native_stack_rela)
                        {
                            auto explicit_addend=::llvm::object::ELFRelocationRef{relocation}.getAddend();
                            if(!explicit_addend){::llvm::consumeError(explicit_addend.takeError());return false;}
                            if(*explicit_addend<0)return false;
                            addend=static_cast<::std::uint64_t>(*explicit_addend);
                        }
                        if(*symbol_section==object.section_end()||!(*symbol_section)->isText()||
                           addend>UINT64_MAX-*value)return false;
                        auto offset=*value+addend;
                        if(offset>=(*symbol_section)->getSize())return false;
                        bool found{};
                        for(auto& frame:native_frames_)if(frame.section==(*symbol_section)->getIndex()&&frame.offset==offset)
                        {
                            if(frame.object_bytes!=UINT64_MAX||frame.bytes!=record->bytes)return false;
                            frame.object_bytes=record->bytes;found=true;
                        }
                        if(!found)return false;record->relocated=true;
                    }
                }
                if(relocation_sections!=1u)return false;
                for(auto const& record:parsed)if(!record.relocated)return false;
            }
            maximum_fixed_frame_=0u;
            for(auto const& frame:native_frames_)
            {
                if(frame.object_bytes==UINT64_MAX||frame.bytes!=frame.object_bytes||frame.bytes>SIZE_MAX)return false;
                if(frame.alignment_slack>SIZE_MAX-frame.bytes)return false;
                maximum_fixed_frame_=(::std::max)(maximum_fixed_frame_,static_cast<::std::size_t>(frame.bytes+frame.alignment_slack));
            }
            // SysV x86 CALL pushes one native pointer (4/8 bytes). Arm BL
            // pushes nothing; saved LR bytes are already in the final MFI.
            // ARM fixed 16-byte realignment is separately charged above by its
            // maximum downward adjustment; it never changes the object witness.
            // No red zone, unbounded realignment, dynamic alloca, unsafe stack or unreserved outgoing
            // call-frame shape survived the actual final-machine observer.
            // enter() rejects depth > limit only after that callee's prologue:
            // charge limit+1, including its unsuccessful entry. Native Host/
            // run_entry/compiler stacks remain SEPARATE, unproven boundaries.
            constexpr ::std::size_t return_address=native_call_push;
            if(maximum_fixed_frame_>SIZE_MAX-return_address||call_limit_==SIZE_MAX)return false;
            auto const frame_charge=maximum_fixed_frame_+return_address;
            guest_fixed_stack_charge_=frame_charge>SIZE_MAX/(call_limit_+1u)?
                SIZE_MAX:frame_charge*(call_limit_+1u);
            if(native_call_graph_known_&&native_call_names_.size()==native_frames_.size())
            {
                ::std::vector<::std::size_t> costs(native_call_names_.size());
                ::std::vector<bool> seen(native_call_names_.size());
                ::llvm::StringMap<::std::size_t> indices;
                for(::std::size_t i{};i<native_call_names_.size();++i)
                    if(!indices.try_emplace(native_call_names_[i],i).second)return false;
                for(auto const& frame:native_frames_)
                {
                    auto const found=indices.find(frame.name);
                    if(found==indices.end()||seen[found->second])return false;
                    if(frame.bytes>SIZE_MAX-frame.alignment_slack||
                       frame.bytes+frame.alignment_slack>SIZE_MAX-return_address)return false;
                    costs[found->second]=static_cast<::std::size_t>(frame.bytes+frame.alignment_slack)+return_address;
                    seen[found->second]=true;
                }
                auto const bound=prove_acyclic_stack(costs,native_call_edges_);
                if(!bound.valid)return false;
                if(bound.acyclic)guest_fixed_stack_charge_=(::std::min)(guest_fixed_stack_charge_,bound.bytes);
            }
            if(guest_fixed_stack_charge_==SIZE_MAX)return false;
            // max_value_stack_bytes grants the interpreter's separate operand
            // storage. The JIT's actual native frame/locals reservation is bounded
            // by the explicitly admitted native mapping, excluding its guards.
            // These are different resources; neither may borrow the other's quota.
            if(guest_native_stack_usable_bytes_==0u||guest_fixed_stack_charge_>guest_native_stack_usable_bytes_)
            {
                ::fast_io::perrln("uwvm: [error] WASI Threads JIT requires ",guest_fixed_stack_charge_,
                    " bytes of proven Guest native frames; --wasi-threads native-stack-bytes provides ",
                    guest_native_stack_usable_bytes_," usable bytes. Increase that native stack quota, including guard pages.");
                return false;
            }
            return true;
        }
#pragma pop_macro("R_ARM_ABS32")
#pragma pop_macro("R_AARCH64_ABS64")
#pragma pop_macro("R_386_32")
#pragma pop_macro("SHT_REL")
#pragma pop_macro("R_X86_64_64")
#pragma pop_macro("SHT_RELA")
