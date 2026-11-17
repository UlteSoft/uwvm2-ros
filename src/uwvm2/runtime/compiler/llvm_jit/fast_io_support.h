#pragma once
#include <uwvm2/utils/container/standard.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/Twine.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Core.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/GlobalVariable.h>
#include <uwvm2/utils/container/unique.h>
namespace uwvm2::runtime::compiler::llvm_jit::fast_io_support
{
    // LLVM allocates these opaque provider objects. Use the same provider's
    // disposal entry points, including when a VM staging transaction rolls back.
    // This also keeps allocation/deallocation inside one CRT on Windows.
    struct context_deleter
    { void operator()(::llvm::LLVMContext* p) const noexcept { if(p) { ::LLVMContextDispose(::llvm::wrap(p)); } } };
    struct module_deleter
    { void operator()(::llvm::Module* p) const noexcept { if(p) { ::LLVMDisposeModule(::llvm::wrap(p)); } } };
    using context_owner=::std::unique_ptr<::llvm::LLVMContext,context_deleter>;
    using module_owner=::std::unique_ptr<::llvm::Module,module_deleter>;
    [[nodiscard]] inline context_owner make_context()
    { return context_owner{::llvm::unwrap(::LLVMContextCreate())}; }
    [[nodiscard]] inline module_owner make_module(::llvm::StringRef name,::llvm::LLVMContext& context)
    {
        auto module=module_owner{::llvm::unwrap(::LLVMModuleCreateWithNameInContext("",::llvm::wrap(&context)))};
        module->setModuleIdentifier(name);return module;
    }
    template<typename Text> [[nodiscard]] inline ::llvm::StringRef ref(Text const& text) noexcept
    { return {text.data(),text.size()}; }
    // LLVM's C entry point constructs its own std::string and invokes the same
    // MCJIT lookup. No fast_io allocation is passed to LLVM's default deleter.
    template<typename Text> [[nodiscard]] inline ::std::uint64_t function_address(::llvm::ExecutionEngine& engine,Text const& name)
    { return ::LLVMGetFunctionAddress(reinterpret_cast<::LLVMExecutionEngineRef>(::std::addressof(engine)),name.c_str()); }
    class string_stream final : public ::llvm::raw_ostream
    {
        ::uwvm2::utils::container::std_string& value_;
        void write_impl(char const* data,::std::size_t count) override { value_.append(data,count); }
        ::std::uint64_t current_pos() const override { return value_.size(); }
    public:
        explicit string_stream(::uwvm2::utils::container::std_string& value) : ::llvm::raw_ostream(true), value_(value) {}
    };
    [[nodiscard]] inline ::uwvm2::utils::container::std_string make_string(::llvm::Twine const& value)
    {
        ::uwvm2::utils::container::std_string result;
        string_stream output{result};value.print(output);
        return result;
    }
    [[nodiscard]] inline ::llvm::GlobalVariable* make_global(::llvm::Module& module,::llvm::Type* type,
        bool constant,::llvm::GlobalValue::LinkageTypes linkage,::llvm::Constant* initial,::llvm::Twine const& name)
    {
        auto* result=::llvm::cast<::llvm::GlobalVariable>(::llvm::unwrap(::LLVMAddGlobalInAddressSpace(
            ::llvm::wrap(&module),::llvm::wrap(type),"",module.getDataLayout().getDefaultGlobalsAddressSpace())));
        result->setName(name);result->setConstant(constant);result->setLinkage(linkage);result->setInitializer(initial);
        return result;
    }

}
// StringRef borrows the actual bytes, independent of the backing allocator.
namespace uwvm2::utils::allocator::fast_io_strict
{
    template<typename View> requires ::std::same_as<View,::llvm::StringRef>
    inline bool operator==(::uwvm2::utils::container::std_string const& lhs,View rhs) noexcept
    { return ::llvm::StringRef{lhs.data(),lhs.size()}==rhs; }
}
