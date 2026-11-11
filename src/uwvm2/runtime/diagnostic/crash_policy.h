// Industrial crash policy: a launch choice, never Guest authority.
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
# include <cstdint>
# include <cstddef>
# include <cstdlib>
# include <fast_io.h>
# if defined(__linux__)
#  include <unistd.h>
#  include <signal.h>
#  include <cerrno>
#  include <poll.h>
#  include <sys/socket.h>
# endif
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::diagnostic::crash
{
    enum class action : unsigned { none, pause, exit };
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
            if(value>static_cast<unsigned>(action::exit)) return false;
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
    // 0: not attempted, 1: stop in progress, 2: continued after stop (or an
    // authorized controller stop), 3: the operating system refused the stop.
    inline ::std::atomic<unsigned> stop_state{};
    static_assert(::std::atomic<unsigned>::is_always_lock_free);
    [[nodiscard]] inline unsigned exit_status() noexcept
    {auto value=first_fault.load(::std::memory_order_acquire);return value ? value:static_cast<unsigned>(category::execution);}

    // Only a recognized, unhandled Guest fault may opt into ordinary process
    // exit. A missing/unknown/internal fault remains a native invariant crash.
    // This never runs on successful Guest instructions and never grants Guest
    // code the authority to change the sealed launch policy.
    inline void crash_unless_stable_exit() noexcept
    {
        auto const status=first_fault.load(::std::memory_order_acquire);
        if(process_policy.selected()!=action::exit ||
           (status!=static_cast<unsigned>(category::unreachable) &&
            status!=static_cast<unsigned>(category::execution) &&
            status!=static_cast<unsigned>(category::memtag)))
        { ::fast_io::fast_terminate(); }
    }
    [[noreturn]] inline void finish_unhandled_fault() noexcept
    {
        crash_unless_stable_exit();
        // _Exit ends the process, including all threads, without destructors.
        // Linux fast_io::fast_exit uses SYS_exit and ends only this thread.
        ::std::_Exit(static_cast<int>(exit_status()));
    }

    // The trusted host record is independent of stderr. Only static runtime
    // reason strings and numeric Wasm coordinates are retained; no Guest text,
    // native PC, exception payload, allocation or automatic dump is involved.
    struct memory_detail
    {
        ::std::uint64_t memory{},pointer{},granule{};
        unsigned pointer_tag{},allocation_tag{};
        // Software checks capture complete numerical tag evidence. Hardware
        // signals may only retain low tag bits and an owned granule; a native
        // tagged address must never masquerade as a Guest tagged pointer.
        bool pointer_available{true},pointer_tag_available{true},allocation_tag_available{true};
        unsigned tag_mask{0xffu};
        bool allocation_tag_is_snapshot{};
    };
    struct event
    {
        unsigned version{2u}, status{}, backend{}, process{};
        char reason[128]{};
        ::std::uint64_t module{UINT64_MAX},function{UINT64_MAX},wasm_offset{UINT64_MAX};
        memory_detail memory{};
        bool memory_available{},asynchronous{},pause_requested{};
    };
    inline event first_event{};
    inline ::std::atomic<unsigned> event_ready{};
    using event_sink = void (*)(void*,event const&) noexcept;
    inline event_sink host_sink{};
    inline void* host_sink_context{};
    inline ::std::atomic<unsigned> host_sink_state{};
    struct signal_endpoint {int descriptor{-1},peer_pidfd{-1};};
    inline signal_endpoint signal_output{};
    inline ::std::atomic<unsigned> signal_output_ready{};
    // Cold trusted host binding of already authenticated, owned capabilities.
    // Both descriptors remain alive until execution drains. Signal delivery
    // only borrows them; it never invokes an arbitrary embedding callback.
    [[nodiscard]] inline bool install_signal_endpoint_host_api(int descriptor,int peer_pidfd) noexcept
    {
        if(descriptor<3 || peer_pidfd<3 || first_fault.load(::std::memory_order_acquire))return false;
        unsigned empty{};
        if(!signal_output_ready.compare_exchange_strong(empty,1u,::std::memory_order_acq_rel))return false;
        signal_output={descriptor,peer_pidfd};signal_output_ready.store(2u,::std::memory_order_release);return true;
    }
    // Trusted cold host initialization only. No Guest import exposes this API.
    // The owner must keep the context alive until execution has fully drained.
    [[nodiscard]] inline bool install_event_sink_host_api(event_sink sink,void* context) noexcept
    {
        if(!sink || first_fault.load(::std::memory_order_acquire))return false;
        unsigned empty{};
        if(!host_sink_state.compare_exchange_strong(empty,1u,::std::memory_order_acq_rel))return false;
        host_sink=sink;host_sink_context=context;
        host_sink_state.store(2u,::std::memory_order_release);return true;
    }
    [[nodiscard]] inline bool snapshot_host_api(event& out) noexcept
    {
        if(!event_ready.load(::std::memory_order_acquire))return false;
        out=first_event;return true;
    }
    [[nodiscard]] inline char const* category_name(unsigned status) noexcept
    {
        switch(static_cast<category>(status)){
        case category::unreachable:return "unreachable";
        case category::execution:return "execution";
        case category::memtag:return "memory-protection";
        default:return "runtime-internal";}
    }
    // A best-effort diagnostic must not replace the selected fault status.
    // Mask SIGPIPE on this thread only, consume only the signal generated by
    // our failed write, preserve pending signals, errno and the previous mask.
    // Keep the signal-mask sequence out of every fast_io formatting/scatter
    // instantiation. This is a cold syscall boundary, not a guest hot path.
    [[nodiscard, gnu::noinline, gnu::cold]] inline bool write_descriptor(int descriptor,char const* data,::std::size_t length) noexcept
    {
#if defined(__linux__)
        int const saved_errno=errno;
        sigset_t block{},previous{},pending{};
        bool delivered{};
        if(::sigemptyset(&block)==0 && ::sigaddset(&block,SIGPIPE)==0 &&
           ::sigprocmask(SIG_BLOCK,&block,&previous)==0)
        {
            if(::sigpending(&pending)==0)
            {
                bool const was_pending=::sigismember(&pending,SIGPIPE)==1;
                bool broken{};::std::size_t sent{};unsigned interrupted{};
                while(sent<length)
                {
                    auto const n=::write(descriptor,data+sent,length-sent);
                    if(n>0)sent+=static_cast<::std::size_t>(n);
                    else if(n<0 && errno==EINTR && ++interrupted<16u)continue;
                    else {broken=n<0 && errno==EPIPE;break;}
                }
                delivered=sent==length;
                if(broken && !was_pending)
                {
                    struct timespec immediate{};
                    for(unsigned i{};i!=16u;++i)
                    {if(::sigtimedwait(&block,nullptr,&immediate)>=0 || errno!=EINTR)break;}
                }
            }
            static_cast<void>(::sigprocmask(SIG_SETMASK,&previous,nullptr));
        }
        errno=saved_errno;return delivered;
#else
        (void)descriptor;(void)data;(void)length;return false;
#endif
    }
    [[nodiscard]] inline bool write_stderr(char const* data,::std::size_t length) noexcept
    {return write_descriptor(2,data,length);}
    // fast_io-compatible diagnostic device: failure drops text, never throws.
    struct stderr_sink {using char_type=char8_t;using output_char_type=char8_t;};
    inline constexpr stderr_sink io_stream_ref_define(stderr_sink value) noexcept {return value;}
    inline void write_all_overflow_define(stderr_sink,char8_t const* first,char8_t const* last) noexcept
    {static_cast<void>(write_stderr(reinterpret_cast<char const*>(first),static_cast<::std::size_t>(last-first)));}

    struct text_record
    {
        // Appenders initialize every byte below size. Value initialization
        // (out{}) still clears the whole buffer for ordinary crash formatting.
        char bytes[2048];::std::size_t size{};
        void append(char const* text) noexcept
        {while(*text && size<sizeof(bytes))bytes[size++]=*text++;}
        void number(::std::uint64_t value) noexcept
        {
            char digits[20];unsigned n{};do{digits[n++]=static_cast<char>('0'+value%10u);value/=10u;}while(value);
            while(n && size<sizeof(bytes))bytes[size++]=digits[--n];
        }
        void optional(::std::uint64_t value,bool json=false) noexcept
        {if(value==UINT64_MAX)append(json ? "null":"unavailable");else number(value);}
        void quoted(char const* text) noexcept
        {
            append("\"");
            while(*text && size+2u<sizeof(bytes))
            {if(*text=='\"' || *text=='\\')bytes[size++]='\\';bytes[size++]=*text++;}
            append("\"");
        }
    };
    [[nodiscard]] inline text_record format_event_json(event const& e,bool sensitive=false) noexcept
    {
        text_record out{};
        out.append("{\"schema\":\"uwvm.crash.v2\",\"category\":");out.quoted(category_name(e.status));
        out.append(",\"reason\":");out.quoted(e.reason);
        out.append(",\"exit_status\":");out.number(e.status);out.append(",\"backend\":");out.number(e.backend);
        out.append(",\"pid\":");out.number(e.process);out.append(",\"module\":");out.optional(e.module,true);
        out.append(",\"function\":");out.optional(e.function,true);
        out.append(",\"wasm_offset\":");out.optional(e.wasm_offset,true);
        out.append(",\"asynchronous\":");out.append(e.asynchronous ? "true":"false");
        out.append(",\"fault_address_available\":");out.append(e.memory_available && e.memory.pointer_available && sensitive ? "true":"false");
        out.append(",\"memory\":");
        if(e.memory_available && sensitive)
        {
            out.append("{\"index\":");out.number(e.memory.memory);out.append(",\"pointer\":");
            if(e.memory.pointer_available)out.number(e.memory.pointer);else out.append("null");
            out.append(",\"granule\":");out.number(e.memory.granule);out.append(",\"pointer_tag\":");
            if(e.memory.pointer_tag_available)out.number(e.memory.pointer_tag);else out.append("null");
            out.append(",\"allocation_tag\":");
            if(e.memory.allocation_tag_available)out.number(e.memory.allocation_tag);else out.append("null");
            out.append(",\"tag_mask\":");out.number(e.memory.tag_mask);
            out.append(",\"allocation_tag_is_snapshot\":");out.append(e.memory.allocation_tag_is_snapshot ? "true":"false");out.append("}");
        }
        else out.append("null");
        out.append(",\"pause_requested\":");out.append(e.pause_requested ? "true":"false");out.append("}\n");return out;
    }
    inline void signal_event(event const& value) noexcept
    {
#if defined(__linux__)
        if(signal_output_ready.load(::std::memory_order_acquire)!=2u)return;
        int const saved_errno=errno;
        ::pollfd peer{signal_output.peer_pidfd,POLLIN,0};
        int observed{};
        for(unsigned i{};i!=16u;++i)
        {observed=::poll(&peer,1,0);if(observed>=0 || errno!=EINTR)break;}
        if(observed==0 && peer.revents==0)
        {
            auto text=format_event_json(value); // fixed buffer, sensitive fields redacted
            for(unsigned i{};i!=16u;++i)
            {if(::send(signal_output.descriptor,text.bytes,text.size,MSG_DONTWAIT|MSG_NOSIGNAL)>=0 || errno!=EINTR)break;}
        }
        errno=saved_errno;
#else
        (void)value;
#endif
    }
    inline void report(category kind,char const* reason,unsigned backend=0u,
        ::std::uint64_t module=UINT64_MAX,::std::uint64_t function=UINT64_MAX,
        ::std::uint64_t offset=UINT64_MAX,memory_detail const* memory=nullptr,bool asynchronous=false,bool from_signal=false) noexcept
    {
        unsigned empty{};
        if(!first_fault.compare_exchange_strong(empty,static_cast<unsigned>(kind),::std::memory_order_acq_rel))return;
        event value{};value.status=static_cast<unsigned>(kind);value.backend=backend;
        value.asynchronous=asynchronous;value.pause_requested=process_policy.selected()==action::pause;
        // Validity is explicit: software deferred faults may retain the real
        // access evidence; hardware asynchronous notifications pass unknown
        // coordinates and no memory detail, never the notification-site PC.
        value.module=module;value.function=function;value.wasm_offset=offset;
        for(::std::size_t i{};reason[i] && i+1u<sizeof(value.reason);++i)value.reason[i]=reason[i];
        if(memory){value.memory=*memory;value.memory_available=true;}
#if defined(__linux__)
        value.process=static_cast<unsigned>(::getpid());
#endif
        first_event=value;event_ready.store(1u,::std::memory_order_release);
        if(from_signal)signal_event(value);
        else if(host_sink_state.load(::std::memory_order_acquire)==2u)host_sink(host_sink_context,value);
        text_record out{};
        out.append("uwvm: [crash] ");out.append(category_name(value.status));out.append("\n  Reason: ");out.append(value.reason);
        out.append("\n  Exit status: ");out.number(value.status);out.append("\n  Backend: ");
        out.append(backend==1u ? "interpreter" : backend==2u ? "LLVM" : "host / unavailable");
        out.append("\n  Wasm module: ");out.optional(value.module);out.append("\n  Wasm function: ");out.optional(value.function);
        out.append("\n  Wasm byte offset: ");out.optional(value.wasm_offset);
        if(value.memory_available)
        {
            out.append("\n  Memory index: ");out.number(value.memory.memory);out.append("\n  Pointer: ");
            if(value.memory.pointer_available)out.number(value.memory.pointer);else out.append("unavailable");
            out.append("\n  Granule: ");out.number(value.memory.granule);out.append("\n  Pointer tag: ");
            if(value.memory.pointer_tag_available)out.number(value.memory.pointer_tag);else out.append("unavailable");
            out.append("\n  Allocation tag: ");
            if(value.memory.allocation_tag_available)out.number(value.memory.allocation_tag);else out.append("unavailable");
            out.append("\n  Tag evidence mask: ");out.number(value.memory.tag_mask);
            if(value.memory.allocation_tag_is_snapshot)out.append("\n  Allocation tag evidence: post-fault hardware snapshot");
        }
        if(asynchronous)out.append("\n  Delivery: asynchronous");
        if(asynchronous && !memory)out.append("\n  Exact fault address: unavailable");
        out.append(value.pause_requested ? "\n  Fault action: pause for the authorized host, then crash on terminal continuation\n" :
            process_policy.selected()==action::exit && kind!=category::internal ?
                "\n  Fault action: stable process exit\n" : "\n  Fault action: crash (OS core-dump policy applies)\n");
        static_cast<void>(write_stderr(out.bytes,out.size));
        report_complete.store(1u,::std::memory_order_release);
    }
    inline void report_signal(category kind,char const* reason,unsigned backend=0u,
        ::std::uint64_t module=UINT64_MAX,::std::uint64_t function=UINT64_MAX,
        ::std::uint64_t offset=UINT64_MAX,memory_detail const* memory=nullptr,bool asynchronous=false) noexcept
    {report(kind,reason,backend,module,function,offset,memory,asynchronous,true);}
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
        {
            // Direct the stop at this thread. A process-directed kill can
            // select another thread and return before it performs the group
            // stop, allowing this terminal worker to exit the process first.
            // raise(SIGSTOP) returns here only after host continuation on a
            // successful stop; the whole process still stops, not just us.
            auto const result=::raise(SIGSTOP);
            stop_state.store(result==0 ? 2u : 3u,::std::memory_order_release);
        }
        else {while(stop_state.load(::std::memory_order_acquire)==1u){}}
#endif
    }
    // A signal may interrupt the thread that owns an unfinished ordinary
    // report or pause. Never wait for that interrupted continuation here.
    inline void pause_process_signal() noexcept
    {
#if defined(__linux__)
        if(process_policy.selected()!=action::pause || !report_complete.load(::std::memory_order_acquire))return;
        unsigned empty{};
        if(stop_state.compare_exchange_strong(empty,1u,::std::memory_order_acq_rel))
        {
            // POSIX raise is async-signal-safe and targets the interrupted
            // thread, so terminal signal handling cannot overtake its stop.
            auto const result=::raise(SIGSTOP);
            stop_state.store(result==0 ? 2u : 3u,::std::memory_order_release);
        }
#endif
    }
}
