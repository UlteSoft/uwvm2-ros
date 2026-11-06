// Actual builtin wrapper + native file/socket IO component. No VM pause,
// Runtime identity issuer or native ROS process is manufactured by this test.
#define UWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64
#define UWVM_ENABLE_WASIP1_SOCKET
#include <uwvm2/uwvm/imported/wasi/wasip1/local_imported/define.h>
#include <uwvm2/utils/container/impl.h>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <chrono>
#include <tuple>
#include <type_traits>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
namespace li=uwvm2::uwvm::imported::wasi::wasip1::local_imported::details;
namespace storage=uwvm2::uwvm::imported::wasi::wasip1::storage;
namespace calls=uwvm2::uwvm::debugger::wasip1_calls;
namespace ws=uwvm2::uwvm::debugger::wasip1_state;
namespace fm=uwvm2::imported::wasi::wasip1::fd_manager;
namespace abi=uwvm2::imported::wasi::wasip1::abi;
namespace memory_io=uwvm2::imported::wasi::wasip1::memory;
static unsigned checks{};
static void check(bool good,char const* why)
{ ++checks;if(!good) { std::fprintf(stderr,"FAIL WASIp1 audit IO: %s\n",why);std::exit(1); } }
static void live_hold() { if(std::getenv("UWVM_AUDIT_HOLD")) { std::raise(SIGSTOP); } }
template<typename Function,typename... Arguments> static unsigned invoke(Arguments... arguments)
{
    typename Function::local_imported_function_type frame{};auto values=std::tuple{arguments...};
    [&]<std::size_t... I>(std::index_sequence<I...>)
    { ((uwvm2::utils::container::get<I>(frame.params)=static_cast<std::remove_cvref_t<decltype(uwvm2::utils::container::get<I>(frame.params))>>(std::get<I>(values))),...); }(std::index_sequence_for<Arguments...>{});
    Function::call(frame);return static_cast<unsigned>(uwvm2::utils::container::get<0>(frame.res));
}
static calls::record returned()
{
    ws::request query{};query.operation=ws::action::trace_read;query.count=64;
    auto view=calls::apply(query);check(!view.records.empty()&&view.records.back().returned,"real wrapper emitted normal return");
    return view.records.back();
}
static void count_result(std::uint64_t requested,std::uint64_t transferred)
{
    auto row=returned();check(row.has_errno&&row.result==0&&row.requested_bytes_known&&row.transferred_bytes_known,
        "successful IO carries known numeric result");
    check(row.requested_bytes==requested&&row.transferred_bytes==transferred,"requested/transferred byte counts match actual IO");
    check(row.environment!=0&&row.context_available&&row.participant==7&&row.module==9&&row.duration_known,
        "detached environment/thread/module/time context retained");
}
static void native_file(fm::wasi_fd_t& fd,fast_io::native_file file)
{
    fd.rights_base=static_cast<abi::rights_t>(-1);fd.rights_inherit=static_cast<abi::rights_t>(-1);
    fd.wasi_fd.ptr->wasi_fd_storage.reset_type(fm::wasi_fd_type_e::file);
    fd.wasi_fd.ptr->wasi_fd_storage.storage.file_fd=std::move(file);
}
template<bool Wide> static void io_cases(char const* root)
{
    using size=std::conditional_t<Wide,std::uint64_t,std::uint32_t>;
    using write=std::conditional_t<Wide,li::fd_write_wasm64,li::fd_write>;
    using read=std::conditional_t<Wide,li::fd_read_wasm64,li::fd_read>;
    using pwrite=std::conditional_t<Wide,li::fd_pwrite_wasm64,li::fd_pwrite>;
    using pread=std::conditional_t<Wide,li::fd_pread_wasm64,li::fd_pread>;
    using send=std::conditional_t<Wide,li::sock_send_wasm64,li::sock_send>;
    using recv=std::conditional_t<Wide,li::sock_recv_wasm64,li::sock_recv>;
    using open=std::conditional_t<Wide,li::path_open_wasm64,li::path_open>;
    using close=std::conditional_t<Wide,li::fd_close_wasm64,li::fd_close>;
    using renumber=std::conditional_t<Wide,li::fd_renumber_wasm64,li::fd_renumber>;
    using reduce=std::conditional_t<Wide,li::fd_fdstat_set_rights_wasm64,li::fd_fdstat_set_rights>;
    uwvm2::object::memory::linear::native_memory_t memory{};memory.init_by_page_count(1u);
    storage::wasip1_env_type env{};env.wasip1_memory=std::addressof(memory);env.fd_storage.opens.resize(13u);env.fd_storage.fd_limit=64u;
    env.debug_trace_identity=calls::new_environment();env.debug_fd_created=calls::created_descriptor;env.debug_io_observed=calls::observe_io;
    storage::scoped_current_wasip1_env_t selected{env};calls::observe_thread(7,9);
    ws::request query{};query.operation=ws::action::trace_clear;(void)calls::apply(query);calls::observe_thread(7,9);
    query.operation=ws::action::trace_enable;(void)calls::apply(query);
    fast_io::dir_file directory{fast_io::mnp::os_c_str(root)};
    auto& parent=*env.fd_storage.opens.index_unchecked(0u).fd_p;
    parent.rights_base=parent.rights_inherit=static_cast<abi::rights_t>(-1);
    parent.wasi_fd.ptr->wasi_fd_storage.reset_type(fm::wasi_fd_type_e::dir);
    auto& chain=parent.wasi_fd.ptr->wasi_fd_storage.storage.dir_stack.dir_stack.emplace_back().ptr->dir_stack;
    chain.name=u8"audit-root";chain.storage.file=std::move(directory);
    fast_io::dir_file output_directory{fast_io::mnp::os_c_str(root)};
    native_file(*env.fd_storage.opens.index_unchecked(3u).fd_p,
        fast_io::native_file{fast_io::at(output_directory),u8"trace.bin",fast_io::open_mode::in|fast_io::open_mode::out|fast_io::open_mode::creat|fast_io::open_mode::trunc});
    native_file(*env.fd_storage.opens.index_unchecked(12u).fd_p,fast_io::native_file{u8"/dev/null",fast_io::open_mode::out});
    constexpr char payload[]{"PRIVATE_PAYLOAD_MUST_NOT_BE_LOGGED"};
    memory_io::write_all_to_memory(memory,1024u,reinterpret_cast<std::byte const*>(payload),reinterpret_cast<std::byte const*>(payload)+sizeof(payload));
    auto iov=[&](size length)
    {memory_io::store_basic_wasm_type_to_memory(memory,128u,size{1024});memory_io::store_basic_wasm_type_to_memory(memory,128u+sizeof(size),length);};
    iov(9);check(invoke<write>(3,128,1,64)==0,"fd_write native success");count_result(9,9);
    check(invoke<read>(3,128,1,64)==0,"fd_read native EOF");count_result(9,0);
    check(invoke<pwrite>(3,128,1,std::uint64_t{0},64)==0,"fd_pwrite native success");count_result(9,9);
    iov(16);check(invoke<pread>(3,128,1,std::uint64_t{0},64)==0,"fd_pread native partial read");count_result(16,9);
    check(invoke<write>(3,128,0,64)==0,"zero iovec succeeds");count_result(0,0);
    check(invoke<read>(-1,128,1,64)==static_cast<unsigned>(abi::errno_t::ebadf),"invalid descriptor retains Preview1 errno");
    auto failed=returned();check(!failed.requested_bytes_known&&!failed.transferred_bytes_known,"early error does not claim decoded IO counts");
    check(invoke<reduce>(3,static_cast<std::uint64_t>(abi::rights_t::right_fd_write),std::uint64_t{0})==0,"actual rights reduction succeeds");
    check(invoke<read>(3,128,1,64)==static_cast<unsigned>(abi::errno_t::enotcapable),"actual read capability denial");
    failed=returned();check(!failed.requested_bytes_known&&!failed.transferred_bytes_known,"permission denial does not borrow guest iovec for diagnostics");
    int sockets[2]{};check(::socketpair(AF_UNIX,SOCK_STREAM,0,sockets)==0,"native local socketpair");
    native_file(*env.fd_storage.opens.index_unchecked(6u).fd_p,fast_io::native_file{sockets[0]});
    native_file(*env.fd_storage.opens.index_unchecked(7u).fd_p,fast_io::native_file{sockets[1]});
    iov(9);check(invoke<send>(6,128,1,0,64)==0,"sock_send native success");count_result(9,9);
    iov(16);check(invoke<recv>(7,128,1,0,64,80)==0,"sock_recv native partial receive");count_result(16,9);
    check(::fcntl(sockets[1],F_SETFL,O_NONBLOCK)==0,"test socket nonblocking");
    check(invoke<recv>(7,128,1,0,64,80)==static_cast<unsigned>(abi::errno_t::eagain),"empty native socket reports EAGAIN");
    failed=returned();check(failed.requested_bytes_known&&failed.requested_bytes==16&&!failed.transferred_bytes_known,
        "native error after validation preserves request count with unknown transfer");
    constexpr char path[]{"trace.bin"};memory_io::write_all_to_memory(memory,2048u,reinterpret_cast<std::byte const*>(path),reinterpret_cast<std::byte const*>(path)+9);
    check(invoke<open>(0,0,2048,9,0,static_cast<std::uint64_t>(abi::rights_t::right_fd_read),std::uint64_t{0},0,96)==0,"actual path_open lifecycle");
    auto issued=returned();check(issued.descriptor_created,"actual open publishes created descriptor");
    auto fd=memory_io::get_basic_wasm_type_from_memory<std::uint32_t>(memory,96u);check(issued.created_descriptor==fd,"created descriptor matches published guest result");
    check(invoke<renumber>(fd,12)==0&&invoke<close>(12)==0,"actual renumber and close lifecycle");
    check(invoke<close>(12)==static_cast<unsigned>(abi::errno_t::ebadf),"second close is explicitly unsuccessful");
    query={};query.operation=ws::action::trace_read;query.count=64;auto view=calls::apply(query);
    uwvm2::utils::container::string text{};calls::print(uwvm2::utils::container::string_ref_uwvm{std::addressof(text)},view);
    auto rendered=std::string_view{text.data(),text.size()};
    check(rendered.find(payload)==std::string_view::npos,"default trace never copies IO payload");
    check(rendered.find("created-fd=")!=std::string_view::npos&&rendered.find("renumbered-from=")!=std::string_view::npos&&
        rendered.find("closed-fd=12")!=std::string_view::npos,"renderer labels observed successful lifecycle changes");
    check(text.size()<=65536u,"trace reply stays bounded");
}
using frame=li::fd_write::local_imported_function_type;
[[gnu::noinline]] static unsigned baseline(frame& value)
{li::fd_write::base::call(value);return static_cast<unsigned>(uwvm2::utils::container::get<0>(value.res));}
[[gnu::noinline]] static unsigned observed(frame& value)
{li::fd_write::call(value);return static_cast<unsigned>(uwvm2::utils::container::get<0>(value.res));}
static void benchmark()
{
    uwvm2::object::memory::linear::native_memory_t memory{};memory.init_by_page_count(1u);
    storage::wasip1_env_type env{};env.wasip1_memory=std::addressof(memory);env.fd_storage.opens.resize(1u);
    native_file(*env.fd_storage.opens.index_unchecked(0u).fd_p,fast_io::native_file{u8"/dev/null",fast_io::open_mode::out});
    storage::scoped_current_wasip1_env_t selected{env};calls::enabled.store(false);frame value{};
    uwvm2::utils::container::get<0>(value.params)=0;uwvm2::utils::container::get<1>(value.params)=128;
    uwvm2::utils::container::get<2>(value.params)=0;uwvm2::utils::container::get<3>(value.params)=64;
    constexpr unsigned rounds=1000000;std::uint64_t checksum{};auto sequence=calls::sequence;
    auto measure=[&](auto function)
    {auto start=std::chrono::steady_clock::now();for(unsigned i{};i<rounds;++i) {checksum+=function(value);}return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-start).count();};
    for(unsigned sample{};sample<9;++sample)
    {
        env.debug_io_observed=nullptr;std::int64_t base{},disabled{};
        if(sample%2) {disabled=measure(observed);base=measure(baseline);}else {base=measure(baseline);disabled=measure(observed);}
        env.debug_io_observed=calls::observe_io;auto hooked=measure(observed);
        std::printf("BENCH sample=%u rounds=%u baseline_ns=%lld disabled_ns=%lld hook_ns=%lld\n",sample,rounds,
            static_cast<long long>(base),static_cast<long long>(disabled),static_cast<long long>(hooked));
    }
    check(checksum==0&&calls::sequence==sequence,"disabled real wrapper appends no record and preserves results");
}
int main(int argc,char** argv)
{
    live_hold();if(argc==2&&std::string_view{argv[1]}=="benchmark") {benchmark();}
    else {if(argc!=2) {return 64;}io_cases<false>(argv[1]);io_cases<true>(argv[1]);}
    std::printf("PASS_WASIP1_AUDIT_IO checks=%u\n",checks);
}
