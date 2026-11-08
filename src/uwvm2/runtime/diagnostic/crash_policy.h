// Industrial crash policy: a launch choice, never Guest authority.
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
# include <cstdint>
# include <cstddef>
# if defined(__linux__)
#  include <unistd.h>
#  include <signal.h>
#  include <cerrno>
# endif
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::diagnostic::crash
{
    enum class action : unsigned { none, pause };
    // Stable product statuses. A Guest may explicitly proc_exit with the same
    // numeric value: callers must also inspect the runtime-owned crash record.
    enum class category : unsigned { unreachable=120, execution=121, memtag=122, internal=123 };
    class launch_policy
    {
        ::std::atomic<unsigned> state_{};
    public:
        [[nodiscard]] bool seal(action requested) noexcept
        {
            auto const value=static_cast<unsigned>(requested);
            if(value>static_cast<unsigned>(action::pause)) return false;
            unsigned empty{};
            return state_.compare_exchange_strong(empty,value+1u,::std::memory_order_release,::std::memory_order_relaxed);
        }
        [[nodiscard]] action selected() const noexcept
        { auto value=state_.load(::std::memory_order_acquire);return value ? static_cast<action>(value-1u):action::none; }
        [[nodiscard]] bool sealed() const noexcept {return state_.load(::std::memory_order_acquire)!=0u;}
    };
    inline launch_policy process_policy{};
    inline ::std::atomic<unsigned> first_fault{};
    inline ::std::atomic<unsigned> report_complete{};
    inline ::std::atomic<unsigned> stop_state{};
    static_assert(::std::atomic<unsigned>::is_always_lock_free);
    [[nodiscard]] inline unsigned exit_status() noexcept
    {auto value=first_fault.load(::std::memory_order_acquire);return value ? value:static_cast<unsigned>(category::execution);}

    // Only static runtime reason strings are passed here. No allocation, Guest
    // text, path, pointer dereference, native PC or automatic dump. One bounded
    // record fits Linux PIPE_BUF. The size bounds the schema, not Guest memory.
    struct memory_detail
    {
        ::std::uint64_t memory{},pointer{},granule{};
        unsigned pointer_tag{},allocation_tag{};
    };
    inline void report(category kind,char const* reason,unsigned backend=0u,
        ::std::uint64_t module=UINT64_MAX,::std::uint64_t function=UINT64_MAX,
        ::std::uint64_t offset=UINT64_MAX,memory_detail const* memory=nullptr) noexcept
    {
        unsigned empty{};
        if(!first_fault.compare_exchange_strong(empty,static_cast<unsigned>(kind),::std::memory_order_acq_rel)) return;
#if defined(__linux__)
        int const saved_errno=errno;
        char buffer[1024];::std::size_t used{};
        auto append=[&](char const* text) noexcept {while(*text && used<sizeof(buffer))buffer[used++]=*text++;};
        auto number=[&](::std::uint64_t value) noexcept {
            char digits[20];unsigned n{};do{digits[n++]=static_cast<char>('0'+value%10u);value/=10u;}while(value);
            while(n && used<sizeof(buffer))buffer[used++]=digits[--n];
        };
        auto optional=[&](::std::uint64_t value) noexcept {if(value==UINT64_MAX)append("null");else number(value);};
        append("uwvm: [crash] {\"schema\":\"uwvm.crash.v1\",\"category\":\"");
        switch(kind){
        case category::unreachable:append("unreachable");break;
        case category::execution:append("execution");break;
        case category::memtag:append("memory-protection");break;
        default:append("runtime-internal");break;}
        append("\",\"reason\":\"");append(reason);append("\",\"exit_status\":");number(static_cast<unsigned>(kind));
        append(",\"backend\":");number(backend);append(",\"pid\":");number(static_cast<unsigned>(::getpid()));
        append(",\"module\":");optional(module);append(",\"function\":");optional(function);
        append(",\"wasm_offset\":");optional(offset);
        if(memory){
            append(",\"memory\":{\"index\":");number(memory->memory);
            append(",\"pointer\":");number(memory->pointer);append(",\"granule\":");number(memory->granule);
            append(",\"pointer_tag\":");number(memory->pointer_tag);
            append(",\"allocation_tag\":");number(memory->allocation_tag);append("}");
        }
        append(",\"pause_requested\":");append(process_policy.selected()==action::pause ? "true":"false");append("}\n");
        // stderr is the host's inherited diagnostic channel, never Guest stdio.
        // Failure cannot turn a rejected access into success.
        sigset_t block{},previous{},pending{};
        ::sigemptyset(&block);::sigaddset(&block,SIGPIPE);
        bool const masked=::sigprocmask(SIG_BLOCK,&block,&previous)==0;
        bool const was_pending=::sigpending(&pending)==0 && ::sigismember(&pending,SIGPIPE)==1;
        ::std::size_t sent{};
        while(masked && sent<used){auto n=::write(STDERR_FILENO,buffer+sent,used-sent);
            if(n>0)sent+=static_cast<::std::size_t>(n);else if(n<0 && errno==EINTR)continue;else break;}
        if(masked){
            if(!was_pending && ::sigismember(&previous,SIGPIPE)!=1){
                struct timespec immediate{};static_cast<void>(::sigtimedwait(&block,nullptr,&immediate));
            }
            static_cast<void>(::sigprocmask(SIG_SETMASK,&previous,nullptr));
        }
        errno=saved_errno;
#else
        (void)reason;(void)backend;(void)module;(void)function;(void)offset;(void)memory;
#endif
        report_complete.store(1u,::std::memory_order_release);
    }
    // SIGCHLD/CLD_STOPPED lets the actual parent observe the process stop with
    // waitid/waitpid(WUNTRACED). No signal is sent to an arbitrary PID. SIGCONT
    // continues terminal handling, never retries the rejected Guest operation.
    inline void pause_process() noexcept
    {
#if defined(__linux__)
        if(process_policy.selected()!=action::pause)return;
        while(first_fault.load(::std::memory_order_acquire) && !report_complete.load(::std::memory_order_acquire)){}
        unsigned empty{};
        if(stop_state.compare_exchange_strong(empty,1u,::std::memory_order_acq_rel))
        {static_cast<void>(::kill(::getpid(),SIGSTOP));stop_state.store(2u,::std::memory_order_release);}
        else {while(stop_state.load(::std::memory_order_acquire)==1u){}}
#endif
    }
}
