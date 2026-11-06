#define main previous_component_main
#include "../0082.audit-qualification/wasip1_io.cc"
#undef main
namespace full=uwvm2::utils::debug::host_calls;
int main(int argc,char** argv)
{
    if(argc!=2) {return 64;}
    full::enabled=1;io_cases<false>(argv[1]);io_cases<true>(argv[1]);
    bool write32{},read32{},write64{},read64{},paths{},denied{};
    for(auto const& p:full::records)
    {
        auto const& r=*p;if(r.result) {denied=true;}check(r.returned,"full journal observes return");
        if(r.name=="fd_write"||r.name=="fd_write_wasm64"||r.name=="fd_pread"||r.name=="fd_pread_wasm64")
        {
            if(r.result) {denied=true;continue;}
            if(r.arguments[2]==0) {continue;}
            bool found{};
            for(auto const& s:r.segments)
            {
                std::string_view bytes{reinterpret_cast<char const*>(s.bytes.data()),s.bytes.size()};
                if(bytes.find("PRIVATE_P")!=bytes.npos) {found=true;check(s.snapshot,"WASI byte observations are labelled snapshots");}
            }
            if(found) {
                if(r.name=="fd_write") {write32=true;}if(r.name=="fd_pread") {read32=true;}
                if(r.name=="fd_write_wasm64") {write64=true;}if(r.name=="fd_pread_wasm64") {read64=true;}
            }
        }
        if(r.name=="path_open"||r.name=="path_open_wasm64") {
            for(auto const& s:r.segments) {if(s.bytes.size()==9&&std::memcmp(s.bytes.data(),"trace.bin",9)==0) {paths=true;}}
        }
    }
    check(write32&&read32&&write64&&read64&&paths&&denied,"both ABIs preserve IO, path bytes and denied results");
    // These adversarial ranges only exercise the observer: it must never trap
    // or change the original ABI invocation's result.
    {
        full::scope scope{full::channel::wasi,"wasi_snapshot_preview1","random_get_wasm64"};
        auto& row=*scope.value;row.arguments[0]=UINT64_MAX-1;row.arguments[1]=8;
        uwvm2::object::memory::linear::native_memory_t memory{};memory.init_by_page_count(1u);
        storage::wasip1_env_type env{};env.wasip1_memory=&memory;
        uwvm2::uwvm::imported::wasi::wasip1::local_imported::observations::capture(env,row,true,true);
        check(row.incomplete,"overflow range explicitly unavailable");
    }
    std::printf("PASS full WASIp1 observation: %u checks\n",checks);
}
