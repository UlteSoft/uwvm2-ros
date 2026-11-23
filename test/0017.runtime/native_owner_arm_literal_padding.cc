// Parse an actual emitted ARM ELF object with a trailing literal pool and
// inter-function alignment. This test issues no loaded-code/read authority.
#define UWVM_RUNTIME_LLVM_JIT 1
#include <uwvm2/runtime/lib/uwvm_runtime_native_owner_function_claims.h>
#include <llvm/Support/MemoryBufferRef.h>
#include <fast_io.h>
namespace ledger = ::uwvm2::runtime::lib::details::native_owner_function_claims;
namespace mapping = ::uwvm2::runtime::lib::details::native_arm_mapping;
#define CHECK(x) do { if(!(x)) { ::fast_io::io::perrln("ARM literal-padding FAIL line=",__LINE__);return 1; } } while(false)
int main(int argc,char** argv)
{
    CHECK(argc==2);
    ::fast_io::native_file file{::fast_io::mnp::os_c_str(argv[1]),::fast_io::open_mode::in};
    auto const size{::fast_io::file_size(file)};CHECK(size!=0u && size<=64u*1024u*1024u);
    ::fast_io::native_file_loader input{::fast_io::at(file)};CHECK(input.size()==size);
    auto object{::llvm::object::ObjectFile::createObjectFile(::llvm::MemoryBufferRef{
        ::llvm::StringRef{input.data(),input.size()},"actual-arm-literal-padding"})};
    CHECK(object);auto const* elf{::llvm::dyn_cast<::llvm::object::ELFObjectFile<::llvm::object::ELF32LE>>(object->get())};
    CHECK(elf && elf->getArch()==::llvm::Triple::arm);
    ledger::image positive{};CHECK(ledger::collect(**object,positive) && positive.endpoints_unambiguous());
    ::uwvm2::utils::container::std_vector<mapping::data_range> ranges{};CHECK(mapping::collect(**object,ranges));
    unsigned gaps{},mutations{};
    auto reject=[&](::uwvm2::utils::container::std_string const& bytes)
    {
        auto changed{::llvm::object::ObjectFile::createObjectFile(::llvm::MemoryBufferRef{
            ::llvm::StringRef{bytes.data(),bytes.size()},"invalid-arm-literal-padding"})};
        if(!changed) { ::llvm::consumeError(changed.takeError());return true; }
        ledger::image invalid{};return !ledger::collect(**changed,invalid) || !invalid.endpoints_unambiguous();
    };
    for(auto const& data:ranges)
    {
        for(auto const& body:positive.bodies)
        {
            if(data.section!=body.section || data.begin<body.begin_offset || data.begin>=body.end_offset || data.end<=body.end_offset) { continue; }
            // A gap is not silently incorporated into this function's extent.
            CHECK(body.endpoints_proved && data.end<=body.section_size);++gaps;
            for(auto const& next:positive.bodies)
            {
                if(next.section!=body.section || next.begin_offset!=data.end || next.end_offset-next.begin_offset<=4u) { continue; }
                for(auto const& symbol:elf->symbols())
                {
                    auto name{symbol.getName()};CHECK(name);
                    if(mapping::kind(*elf,symbol,*name)!='a') { continue; }
                    auto section{symbol.getSection()};auto address{symbol.getAddress()};CHECK(section && address);
                    if(*section==elf->section_end() || (**section).getIndex()!=data.section ||
                       *address-(**section).getAddress()!=data.end) { continue; }
                    auto record{elf->getSymbol(symbol.getRawDataRefImpl())};CHECK(record);
                    auto const* raw{reinterpret_cast<char const*>(::std::addressof((*record)->st_value))};
                    auto const start{reinterpret_cast<::std::uintptr_t>(input.data())},place{reinterpret_cast<::std::uintptr_t>(raw)};
                    CHECK(place>=start && place-start<=input.size() && input.size()-(place-start)>=4u);
                    auto const value{static_cast<::std::uint32_t>((*record)->st_value)};CHECK(value<=UINT32_MAX-4u);
                    ::uwvm2::utils::container::std_string changed{input.data(),input.size()};
                    for(unsigned byte{};byte!=4u;++byte) { changed[place-start+byte]=static_cast<char>((value+4u)>>(byte*8u)); }
                    // Moving the next ARM marker into a proved function would
                    // let a data interval cover its entry: still reject it.
                    CHECK(reject(changed));++mutations;
                }
            }
        }
    }
    CHECK(gaps!=0u && mutations!=0u);
    unsigned kinds{};
    for(auto const& symbol:elf->symbols())
    {
        auto name{symbol.getName()};CHECK(name);
        if(mapping::kind(*elf,symbol,*name)!='d') { continue; }
        auto const start{reinterpret_cast<::std::uintptr_t>(input.data())},place{reinterpret_cast<::std::uintptr_t>(name->data())};
        CHECK(place>=start && place-start<input.size() && input.size()-(place-start)>=2u);
        for(char replacement:{'t','e'})
        {
            ::uwvm2::utils::container::std_string changed{input.data(),input.size()};changed[place-start+1u]=replacement;
            CHECK(reject(changed));++kinds;
        }
        break;
    }
    CHECK(kinds==2u);
    ::fast_io::io::println("PASS actual ARM alignment gaps=",gaps," cross-body refusals=",mutations,
        " invalid mapping refusals=",kinds," function endpoints unchanged; no native authority issued");
}
