// Included in the LLVM emitter details namespace. No Runtime/source authority
// is issued by this IR witness; the private factory owns the real ChildWorld.
inline constexpr unsigned memtag_shared_lease_abi_version{1u};
enum class memtag_shared_access_family : unsigned { scalar, atomic };
inline constexpr char const* memtag_shared_lease_attribute{"uwvm.memtag.shared-lease"};
inline constexpr char const* memtag_shared_lease_module_flag{"uwvm.memtag.shared-lease.abi"};
[[nodiscard]] inline char const* memtag_shared_lease_family_name(memtag_shared_access_family family) noexcept
{ return family==memtag_shared_access_family::scalar ? "scalar-v1" : "atomic-v1"; }
[[nodiscard]] inline bool has_memtag_shared_lease_module_abi(::llvm::Module const& module) noexcept
{
    auto const* metadata{::llvm::dyn_cast_or_null<::llvm::ConstantAsMetadata>(module.getModuleFlag(memtag_shared_lease_module_flag))};
    auto const* version{metadata==nullptr ? nullptr : ::llvm::dyn_cast<::llvm::ConstantInt>(metadata->getValue())};
    return version!=nullptr&&version->getType()->isIntegerTy(32u)&&version->getZExtValue()==memtag_shared_lease_abi_version;
}
[[nodiscard]] inline bool set_memtag_shared_lease_module_abi(::llvm::Module& module) noexcept
{
    if(module.getModuleFlag(memtag_shared_lease_module_flag)!=nullptr)
    { return has_memtag_shared_lease_module_abi(module); }
    module.addModuleFlag(::llvm::Module::Error,memtag_shared_lease_module_flag,memtag_shared_lease_abi_version);
    return true;
}
[[nodiscard]] inline bool memtag_shared_lease_signature(
    ::llvm::FunctionType const& type,memtag_shared_access_family family,unsigned native_bits) noexcept
{
    if(family!=memtag_shared_access_family::scalar&&family!=memtag_shared_access_family::atomic)return false;
    auto const arguments{family==memtag_shared_access_family::scalar ? 4u : 6u};
    if(type.isVarArg()||!type.getReturnType()->isIntegerTy(64u)||type.getNumParams()!=arguments||
       !type.getParamType(0u)->isIntegerTy(native_bits))return false;
    for(unsigned i{1u};i<arguments;++i)if(!type.getParamType(i)->isIntegerTy(64u))return false;
    return true;
}
[[nodiscard]] inline bool mark_memtag_shared_lease_call(::llvm::CallInst& call,
    memtag_shared_access_family family,unsigned native_bits,::llvm::CallingConv::ID convention) noexcept
{
    auto* module{call.getModule()};
    if(module==nullptr||call.getCallingConv()!=convention||
       !memtag_shared_lease_signature(*call.getFunctionType(),family,native_bits)||
       !set_memtag_shared_lease_module_abi(*module))return false;
    auto& context{call.getContext()};auto const* name{memtag_shared_lease_family_name(family)};
    call.addFnAttr(::llvm::Attribute::get(context,memtag_shared_lease_attribute,name));
    call.setTailCallKind(::llvm::CallInst::TCK_NoTail);
    call.setMetadata(memtag_shared_lease_attribute,::llvm::MDNode::get(context,{
        ::llvm::ConstantAsMetadata::get(::llvm::ConstantInt::get(::llvm::Type::getInt32Ty(context),memtag_shared_lease_abi_version)),
        ::llvm::ConstantAsMetadata::get(::llvm::ConstantInt::get(::llvm::Type::getInt32Ty(context),static_cast<unsigned>(family)))}));
    // Private threaded emission always has a direct declaration. The ordinary
    // RISC-V relocatable-pointer ABI may have an indirect target instead.
    if(auto* function{call.getCalledFunction()})
    {
        auto const old{function->getFnAttribute(memtag_shared_lease_attribute)};
        if(old.isValid()&&old.getValueAsString()!=name)return false;
        function->addFnAttr(memtag_shared_lease_attribute,name);
    }
    return true;
}
[[nodiscard]] inline bool memtag_shared_lease_declaration(::llvm::Function const& function,
    memtag_shared_access_family family,unsigned native_bits,::llvm::CallingConv::ID convention) noexcept
{
    auto const attribute{function.getFnAttribute(memtag_shared_lease_attribute)};
    return function.isDeclaration()&&!function.isIntrinsic()&&function.getCallingConv()==convention&&
        memtag_shared_lease_signature(*function.getFunctionType(),family,native_bits)&&attribute.isValid()&&
        attribute.getValueAsString()==memtag_shared_lease_family_name(family)&&
        function.getParent()!=nullptr&&has_memtag_shared_lease_module_abi(*function.getParent());
}
[[nodiscard]] inline bool memtag_shared_lease_call_valid(::llvm::CallBase const& call,
    memtag_shared_access_family family,unsigned native_bits,::llvm::CallingConv::ID convention) noexcept
{
    auto const* declaration{call.getCalledFunction()};
    auto const attribute{call.getAttributes().getFnAttr(memtag_shared_lease_attribute)};
    auto const* metadata{call.getMetadata(memtag_shared_lease_attribute)};
    if(declaration==nullptr||!memtag_shared_lease_declaration(*declaration,family,native_bits,convention)||
       call.getCallingConv()!=convention||!attribute.isValid()||
       attribute.getValueAsString()!=memtag_shared_lease_family_name(family)||metadata==nullptr||metadata->getNumOperands()!=2u)return false;
    for(unsigned i{};i!=2u;++i)
    {
        auto const* constant{::llvm::dyn_cast_or_null<::llvm::ConstantAsMetadata>(metadata->getOperand(i).get())};
        auto const* value{constant==nullptr ? nullptr : ::llvm::dyn_cast<::llvm::ConstantInt>(constant->getValue())};
        if(value==nullptr||!value->getType()->isIntegerTy(32u)||value->getZExtValue()!=
           (i==0u?memtag_shared_lease_abi_version:static_cast<unsigned>(family)))return false;
    }
    return true;
}

[[nodiscard]] inline bool validate_memtag_shared_lease_ir(::llvm::Module const& module,
    unsigned native_bits,::llvm::CallingConv::ID convention) noexcept
{
    // Iterate current IR, not pre-optimizer Function pointers. DCE may
    // delete an unused declaration after bind_emitted_bridges().
    for(auto const& declaration:module)
    {
        auto const attribute=declaration.getFnAttribute(memtag_shared_lease_attribute);
        if(!attribute.isValid())continue;
        memtag_shared_access_family family{};
        if(attribute.getValueAsString()=="scalar-v1")family=memtag_shared_access_family::scalar;
        else if(attribute.getValueAsString()=="atomic-v1")family=memtag_shared_access_family::atomic;
        else return false;
        if(!memtag_shared_lease_declaration(declaration,family,native_bits,
            convention))return false;
        for(auto const* user:declaration.users())
        {
            auto const* call=::llvm::dyn_cast<::llvm::CallBase>(user);
            if(call==nullptr||call->getCalledFunction()!=::std::addressof(declaration)||call->getFunction()==nullptr||
               call->getFunction()->getParent()!=::std::addressof(module)||
               !memtag_shared_lease_call_valid(*call,family,native_bits,
                   convention))return false;
        }
    }
    for(auto const& function:module)for(auto const& block:function)for(auto const& instruction:block)
        if(auto const* call=::llvm::dyn_cast<::llvm::CallBase>(::std::addressof(instruction));call!=nullptr&&
           (call->hasFnAttr(memtag_shared_lease_attribute)||call->getMetadata(memtag_shared_lease_attribute)!=nullptr))
        {
            auto const attribute=call->getAttributes().getFnAttr(memtag_shared_lease_attribute);
            if(!attribute.isValid())return false;
            auto const name=attribute.getValueAsString();
            auto const family=name=="scalar-v1"?memtag_shared_access_family::scalar:memtag_shared_access_family::atomic;
            if((name!="scalar-v1"&&name!="atomic-v1")||!memtag_shared_lease_call_valid(*call,family,
                native_bits,convention))return false;
        }
    return true;
 }
