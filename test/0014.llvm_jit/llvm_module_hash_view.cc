// Cache identity must remain byte-for-byte compatible when the redundant
// bitcode copy is removed. No cache entry or source admission is fabricated.
#include <uwvm2/runtime/llvm_jit_cache/llvm_object_cache.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <openssl/evp.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
namespace cache = ::uwvm2::runtime::llvm_jit_cache::details;
static void require(bool ok) { if(!ok) { ::std::abort(); } }
static ::std::string hex(::uwvm2::utils::container::u8string const& value)
{ return {reinterpret_cast<char const*>(value.data()), value.size()}; }
static ::std::string independent(::std::byte const* first, ::std::size_t size)
{
    ::std::array<unsigned char, EVP_MAX_MD_SIZE> output{};
    unsigned length{};
    require(::EVP_Digest(first, size, output.data(), &length, ::EVP_sha256(), nullptr) == 1 && length == 32);
    ::std::string result{};
    constexpr char alphabet[]{"0123456789abcdef"};
    for(unsigned i{}; i != length; ++i)
    { result += alphabet[output[i] >> 4]; result += alphabet[output[i] & 15]; }
    return result;
}
static ::std::string old_module_hash(::llvm::Module const& module)
{
    ::std::string serialized{};
    ::llvm::raw_string_ostream stream{serialized};
    ::llvm::WriteBitcodeToFile(module, stream);stream.flush();
    ::uwvm2::utils::container::vector<::std::byte> copy{};
    auto const first{reinterpret_cast<::std::byte const*>(serialized.data())};
    cache::append_bytes(copy, first, first + serialized.size());
    require(hex(cache::sha256_hex_for_path(copy)) == independent(first, serialized.size()));
    return hex(cache::sha256_hex_for_path(copy));
}
int main()
{
    require(hex(cache::sha256_hex_for_path(nullptr, nullptr)) ==
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    ::std::array<::std::size_t, 17> sizes{0,1,2,3,7,55,56,63,64,65,127,128,129,255,256,4097,131071};
    for(auto size:sizes)
    {
        ::uwvm2::utils::container::vector<::std::byte> bytes{};
        for(::std::size_t i{};i!=size;++i) { bytes.push_back(static_cast<::std::byte>((i * 73u + 19u) & 255u)); }
        auto const view{hex(cache::sha256_hex_for_path(bytes.cbegin(), bytes.cend()))};
        require(view == hex(cache::sha256_hex_for_path(bytes)));
        require(view == independent(bytes.data(), bytes.size()));
    }
    ::llvm::LLVMContext context{};
    ::llvm::Module module{"module", context};
    auto const check{[&]()
    {
        require(!::llvm::verifyModule(module));
        auto const result{hex(cache::module_bitcode_hash(module))};
        require(result == old_module_hash(module));
        require(result == hex(cache::module_bitcode_hash(module)));
        return result;
    }};
    auto previous{check()};
    auto const changed{[&]()
    { auto now{check()};require(now != previous);previous = ::std::move(now); }};
    ::llvm::IRBuilder<> builder{context};
    auto global{new ::llvm::GlobalVariable(module,builder.getInt32Ty(),false,::llvm::GlobalValue::ExternalLinkage,
        builder.getInt32(42),"value")};
    changed();
    global->setInitializer(builder.getInt32(43));changed();
    module.setDataLayout("e-p:32:32");changed();
    module.setDataLayout("e-p:64:64");changed();
    auto const function{::llvm::Function::Create(::llvm::FunctionType::get(builder.getInt32Ty(),false),
        ::llvm::GlobalValue::ExternalLinkage,"function",module)};
    builder.SetInsertPoint(::llvm::BasicBlock::Create(context,"entry",function));
    builder.CreateRet(builder.CreateLoad(builder.getInt32Ty(),global));changed();
    module.getOrInsertNamedMetadata("identity")->addOperand(::llvm::MDNode::get(context,::llvm::MDString::get(context,"original")));changed();
    module.getNamedMetadata("identity")->addOperand(::llvm::MDNode::get(context,::llvm::MDString::get(context,"changed")));changed();
    // Large payload exercises the former full-buffer copy and embedded NULs.
    ::std::vector<unsigned char> payload(1u<<20u);
    for(::std::size_t i{};i!=payload.size();++i) { payload[i] = static_cast<unsigned char>(i * 17u); }
    new ::llvm::GlobalVariable(module,::llvm::ArrayType::get(builder.getInt8Ty(),payload.size()),true,
        ::llvm::GlobalValue::PrivateLinkage,::llvm::ConstantDataArray::get(context,payload),"payload");
    changed();
    ::std::puts("{\"status\":\"pass\",\"byte_ranges\":18,\"module_states\":9,\"independent_digest\":\"OpenSSL SHA-256\"}");
}
