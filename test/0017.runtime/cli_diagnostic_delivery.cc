// Real CLI diagnostic device, including its pre-main initialization.
#define UWVM
#include <uwvm2/uwvm_predefine/io/impl.h>
#include <uwvm2/runtime/diagnostic/crash_policy.h>
#include <cstdio>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#if !defined(__linux__)
#error "Requires actual Linux diagnostic descriptor semantics."
#endif
int main(int argc,char**)
{
    namespace io=uwvm2::uwvm::io;
    namespace crash=uwvm2::runtime::diagnostic::crash;
    static_assert(!std::is_base_of_v<fast_io::u8native_file,io::diagnostic_file>);
    static_assert(fast_io::semantic_optional_scatter_plan_stream<char8_t,io::diagnostic_observer>);
    errno=E2BIG;
    fast_io::io::perrln(io::u8log_output,u8"uwvm: [warn] boot warning");
    // Real diagnostic packs contain many optional color fragments. Their
    // compact scatter plan must preserve the safe writer instead of selecting
    // native writev or enumerating exponentially many active argument types.
    bool const color=argc>1;
    fast_io::io::perrln(io::u8log_output,
        fast_io::mnp::cond(color,u8"\033[0;37m"),u8"uwvm: ",
        fast_io::mnp::cond(color,u8"\033[91m"),u8"[warn] ",
        fast_io::mnp::cond(color,u8"\033[37m"),u8"conditional scatter ",
        fast_io::mnp::cond(color,u8"\033[33m"),17u,
        fast_io::mnp::cond(color,u8"\033[0m"),u8" retained ",
        fast_io::mnp::cond(color,u8"\033[1m"),u8"safe ",
        fast_io::mnp::cond(color,u8"\033[32m"),u8"ordered ",
        fast_io::mnp::cond(color,u8"\033[34m"),u8"fragments ",
        fast_io::mnp::cond(color,u8"\033[35m"),u8"complete",
        fast_io::mnp::cond(color,u8"\033[0;1m"));
    auto output=fast_io::operations::output_stream_ref(io::u8log_output);
    {
        fast_io::operations::decay::stream_ref_decay_lock_guard guard{
            fast_io::operations::decay::output_stream_mutex_ref_decay(output)};
        auto unlocked=fast_io::operations::decay::output_stream_unlocked_ref_decay(output);
        fast_io::io::perrln(unlocked,u8"uwvm: [error] cold validation diagnostic ",127u);
    }
    fast_io::io::perrln(io::u8log_output.handle,u8"uwvm: direct owned handle diagnostic");
    if(errno!=E2BIG)return 91;
    if(argc>1)
    {
        io::u8log_output.reopen(fast_io::io_dup,fast_io::u8out());
        fast_io::io::perrln(io::u8log_output,u8"explicit log output restored");
    }
    io::u8runtime_log_output.reopen(fast_io::io_dup,fast_io::u8out());
    fast_io::io::perrln(io::u8runtime_log_output,u8"compiler log survives");
    crash::report(crash::category::memtag,"memory tag mismatch");
    return static_cast<int>(crash::exit_status());
}
