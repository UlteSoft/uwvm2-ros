// Included inside Runtime lib; only this selected cold adapter touches these paths.
extern "C++"
{
    struct runtime_threaded_wasip1_adapter::fd_pin final
    {
        enum class kind {unsupported,pipe,stream_socket,datagram_socket,native_descriptor};
        context* call{};native_fd* locked{};
        fd_ref retained{::uwvm2::imported::wasi::wasip1::fd_manager::wasi_no_construct};
        int descriptor{-1},flags{};bool owns_descriptor{},guest_nonblock{};kind type{kind::unsupported};
        fd_pin() noexcept=default;fd_pin(fd_pin const&)=delete;
        ~fd_pin() noexcept {if(owns_descriptor&&descriptor>=0)static_cast<void>(::close(descriptor));unlock();}
        void unlock() noexcept
        {
            if(locked!=nullptr)
            {
                auto const result=::pthread_mutex_unlock(::std::addressof(locked->fd_mutex.mutex));locked=nullptr;
                if(result!=0&&call)static_cast<void>(call->native_failure());
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                // Notify only after the real mutex is released. Poll registers
                // before trying that lock, so it cannot lose this transition.
                // Wake other FD contenders, but not this thread's own quota
                // waiter after a rejected attempt: that creates a busy retry loop.
                if(result==0)::uwvm2::imported::wasi::wasip1::environment::process_budget_ledger.notify_waiters(false);
#endif
            }
        }
        struct acquisition {error failure{error::esuccess};bool deferred{};};
        [[nodiscard]] acquisition acquire(context& actual,::std::uint32_t guest,right required,bool keep_lock,bool try_only=false,bool policy_only=false,bool metadata_only=false,bool require_stdio_grant=true) noexcept
        {
            call=&actual;if(guest>INT32_MAX)return {error::ebadf};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            if(require_stdio_grant && guest < 3u && !actual.env.grants_stdio(guest)) return {error::enotcapable};
            if(guest < 3u && (required & right::right_poll_fd_readwrite) != right{} && !actual.env.permits(permission::stdio_readiness)) return {error::enotcapable};
#endif
            auto& map=actual.env.fd_storage;
            // Same actual fair RW lock protocol; no blocking guard constructor
            // can hide cancellation. PENDING/WRITER block new readers.
            auto& bits=map.fds_rwlock.bits;
            constexpr auto increment=::uwvm2::utils::mutex::rwlock_reader_mask();
            constexpr auto excluded=::uwvm2::utils::mutex::rwlock_writer_mask()|::uwvm2::utils::mutex::rwlock_pending_mask();
            for(;;)
            {
                if(actual.stopped())return {error::ecanceled};
                auto old=bits.load(::std::memory_order_relaxed);
                if((old&excluded)==0u)
                {
                    if(old>UINT_MAX-increment)return {error::eoverflow};
                    if(bits.compare_exchange_weak(old,old+increment,::std::memory_order_acquire,::std::memory_order_relaxed))break;
                }
                if(try_only)return {error::esuccess,true};
                ::sched_yield();
            }
            struct release_map final {decltype(bits)& value;~release_map() noexcept{value.fetch_sub(::uwvm2::utils::mutex::rwlock_reader_mask(),::std::memory_order_release);}} read{bits};
            native_fd* selected{};
            if(guest<map.opens.size())selected=map.opens.index_unchecked(guest).fd_p;
            else
            {auto found=map.renumber_map.find(static_cast<::std::int32_t>(guest));if(found==map.renumber_map.end())return {error::ebadf};selected=found->second.fd_p;}
            if(selected==nullptr){static_cast<void>(actual.native_failure());return {error::eio};}
            // Manager pin -> actual descriptor mutex is the existing lifetime
            // order. Do not release the manager before fd mutex is owned.
            for(;;)
            {
                if(actual.stopped())return {error::ecanceled};
                auto const result=::pthread_mutex_trylock(::std::addressof(selected->fd_mutex.mutex));
                if(result==0){locked=selected;break;}
                if(result!=EBUSY){static_cast<void>(actual.native_failure());return {error::eio};}
                // Poll never waits for a descriptor held by a blocking read.
                // The map read RAII above releases before the caller waits on
                // its already-established real timers + Group stop fd.
                if(try_only)return {error::esuccess,true};
                ::sched_yield();
            }
            if(selected->close_pos!=SIZE_MAX)return {error::ebadf};
            if((selected->rights_base&required)!=required)return {error::enotcapable};
            if(policy_only) { unlock(); return {error::esuccess}; }
            if(selected->wasi_fd.ptr==nullptr){static_cast<void>(actual.native_failure());return {error::eio};}
            // Metadata readers need the actual retained binding without a
            // native class probe, duplicate descriptor, or OFD flag query.
            // Directory identity is examined only while this FD mutex is held.
            if(metadata_only) { if(!keep_lock)unlock(); return {error::esuccess}; }
            // The map reader and fd mutex keep the explicit binding alive.
            // Ordinary files/devices are retained without dup/close, which can
            // otherwise introduce an extra filesystem flush. Their synchronous
            // call and final resource release retain the native execution lease.
            auto const& storage=selected->wasi_fd.ptr->wasi_fd_storage;
            using t=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e;
            int borrowed{-1};
            if(storage.type==t::file)borrowed=storage.storage.file_fd.native_handle();
            else if(storage.type==t::file_observer)borrowed=storage.storage.file_observer.native_handle();
            else if(storage.type==t::dir)return {error::eisdir};
            else return {error::enotsup};
            // These class probes dispatch only through the kernel socket/pipe
            // identity checks, not an unknown filesystem/device operation.
            flags=::fcntl(borrowed,F_GETFL);if(flags<0)return {native_error(errno)};
            int socket_type{};::socklen_t extent{sizeof(socket_type)};
            if(::getsockopt(borrowed,SOL_SOCKET,SO_TYPE,&socket_type,&extent)==0)
            {
                if(extent!=sizeof(socket_type)||(socket_type!=SOCK_STREAM&&socket_type!=SOCK_DGRAM))return {error::enotsup};
                type=socket_type==SOCK_STREAM?kind::stream_socket:kind::datagram_socket;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                // Actual native type must match the trusted resource binding.
                // A guest cannot borrow an unclassified host socket as a file.
                if(selected->wasi_fd.ptr->socket_kind==::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_rc_t::socket_type::none)
                    return {error::enotcapable};
                guest_nonblock=selected->wasi_fd.ptr->socket_nonblock;
#else
                guest_nonblock=(flags&O_NONBLOCK)!=0;
#endif
            }
            else
            {
                if(errno!=ENOTSOCK)return {native_error(errno)};
                // Linux F_GETPIPE_SZ identifies genuine pipe/FIFO operations
                // without calling unknown filesystem fstat/getattr/device IO.
                constexpr int linux_getpipe_size{1032};
                type=::fcntl(borrowed,linux_getpipe_size)>0 ? kind::pipe : kind::native_descriptor;
            }
            // Same explicitly approved OFD; no flag changes or path reopen.
            retained=selected->wasi_fd;
            if(type==kind::native_descriptor) descriptor=borrowed;
            else
            {
                constexpr int linux_dupfd_cloexec{1030};
                descriptor=::fcntl(borrowed,linux_dupfd_cloexec,0);
                if(descriptor<0)return {native_error(errno)};
                owns_descriptor=true;
            }
            auto const pinned_flags=::fcntl(descriptor,F_GETFL);
            if(pinned_flags<0)return {native_error(errno)};
            if(pinned_flags!=flags){static_cast<void>(actual.native_failure());return {error::eio};}
            if(!keep_lock)unlock();return {error::esuccess};
        }
    };

    inline auto runtime_threaded_wasip1_adapter::wait_ready(context& call,int descriptor,short events,short& returned) noexcept -> error
    {
        for(;;)
        {
            if(call.stopped())return error::ecanceled;
            auto const stop=call.actual.cancellation_fd();if(stop<0){static_cast<void>(call.native_failure());return error::eio;}
            ::pollfd watched[2]{{stop,POLLIN,0},{descriptor,events,0}};
            // Native blocking semantics with a real cancellation descriptor;
            // no implicit one-second operation deadline.
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            auto const ready{::uwvm2::imported::wasi::wasip1::environment::wait_for_native_io(
                descriptor,events,::std::chrono::steady_clock::time_point::max(),stop)};
            if(ready == ::uwvm2::imported::wasi::wasip1::environment::quota_call_result::cancelled) return error::ecanceled;
            if(ready == ::uwvm2::imported::wasi::wasip1::environment::quota_call_result::wait_unavailable) return error::eio;
            auto const result=::poll(watched,2,0);
#else
            auto const result=::poll(watched,2,-1);
#endif
            if(result<0){if(errno==EINTR)continue;return native_error(errno);}
            if(call.stopped())return error::ecanceled;
            if(watched[0].revents!=0){static_cast<void>(call.native_failure());return error::eio;}
            if(watched[1].revents){returned=watched[1].revents;return error::esuccess;}
        }
    }

    inline auto runtime_threaded_wasip1_adapter::fd_io(context& call,bool writing,::std::uint32_t guest,
        ::std::uint64_t iovs,::std::uint64_t count,::std::uint64_t completed,bool positioned,::std::uint64_t position) noexcept -> error
    {
        if(count > SIZE_MAX / (call.word_bytes()*2u)) return error::eoverflow;
        if(!call.aligned(iovs,call.word_bytes())||!call.aligned(completed,call.word_bytes()))return error::efault;

        if(!call.bounds(iovs,static_cast<::std::size_t>(count)*(call.word_bytes()*2u))||!call.bounds(completed,call.word_bytes()))return error::efault;
        struct vector {::std::uint64_t base{},size{};};
        auto vectors{allocate_array<vector>(count)};
        if(count != 0u && !vectors) return error::enomem;
        ::std::uint64_t total{};
        for(::std::size_t i{};i<count;++i)
        {
            ::std::array<::std::byte,16> raw{};if(!call.copy_in(iovs+i*(call.word_bytes()*2u),{raw.data(),call.word_bytes()*2u}))return error::efault;
            auto& v=vectors[i];v.base=little(raw.data(),call.word_bytes());v.size=little(raw.data()+call.word_bytes(),call.word_bytes());
            if(!call.bounds(v.base,v.size))return error::efault;
            if(v.size>call.size_limit()-total)return error::einval;total+=v.size;
        }
        if(positioned && position>static_cast<::std::uint64_t>(INT64_MAX))return error::eoverflow;
        auto required=writing?right::right_fd_write:right::right_fd_read;
        if(positioned)required|=right::right_fd_seek;
        fd_pin pinned{};auto acquired=pinned.acquire(call,guest,required,true);
        if(acquired.failure!=error::esuccess)return acquired.failure;
        if(acquired.deferred){static_cast<void>(call.native_failure());return error::eio;}
        if(positioned && pinned.type!=fd_pin::kind::native_descriptor)return error::espipe;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        using budgets=::uwvm2::imported::wasi::wasip1::environment::budget_kind;
        bool const socket=pinned.type==fd_pin::kind::stream_socket||pinned.type==fd_pin::kind::datagram_socket;
        auto const kind=socket?(writing?budgets::socket_write:budgets::socket_read):
            ::uwvm2::imported::wasi::wasip1::environment::fd_io_budget(*pinned.locked->wasi_fd.ptr);
        ::uwvm2::imported::wasi::wasip1::environment::budget_reservation reservation{kind, call.env.limits, total};
        if(!reservation) return reservation.error<error>();
#endif
        if(total==0u && (count==0u || pinned.type!=fd_pin::kind::datagram_socket))return call.store_size(completed,0u)?error::esuccess:error::efault;
        // A read end cannot reveal packet-mode buffers from another writer.
        // Keep read(2)'s exact packet/drop semantics instead of consuming bytes
        // through splice. Blocking reads stay synchronous and retain their lease;
        // default proc_exit still ends the process, while explicit wait waits.
        bool const splice_pipe{pinned.type==fd_pin::kind::pipe && writing && (pinned.flags&O_DIRECT)==0};
        // Linux packet pipes split packets at PIPE_BUF. This capacity preserves
        // every packet a full Guest buffer could accept, while larger byte-stream
        // reads may legally report partial progress without a huge allocation.
        auto const capacity=pinned.type==fd_pin::kind::datagram_socket?total:
            (::std::min)(total,static_cast<::std::uint64_t>(transfer_bytes));
        if(capacity > SIZE_MAX) return error::eoverflow;
        auto const wanted{static_cast<::std::size_t>(capacity)};
        auto bounce{allocate_array<::std::byte>(wanted)};
        if(wanted && !bounce) return error::enomem;
        if(writing)
        {
            ::std::size_t at{};
            for(::std::size_t i{};i<count&&at<wanted;++i)
            {auto n=(::std::min)(static_cast<::std::size_t>(vectors[i].size),wanted-at);if(!call.copy_in(vectors[i].base,{bounce.get()+at,n}))return error::efault;at+=n;}
        }
        struct staging final
        {
            int ends[2]{-1,-1};
            ~staging() noexcept{for(auto fd:ends)if(fd>=0)static_cast<void>(::close(fd));}
        } pipe;
        if(splice_pipe)
        {
            if(::syscall(SYS_pipe2,pipe.ends,O_NONBLOCK|O_CLOEXEC)!=0)return native_error(errno);
            for(auto fd:pipe.ends){auto f=::fcntl(fd,F_GETFL);if(f<0||(f&O_NONBLOCK)==0){static_cast<void>(call.native_failure());return error::eio;}}
            if(writing)
            {
                // Only our NEW OFDs are nonblocking. The actual inherited /
                // redirected descriptor and every user alias are unmodified.
                auto const n=::write(pipe.ends[1],bounce.get(),wanted);
                if(n<0)return native_error(errno);if(static_cast<::std::size_t>(n)!=wanted){static_cast<void>(call.native_failure());return error::eio;}
            }
        }
        struct sigpipe_guard final
        {
            context& call;::sigset_t set{},old{};bool active{},preexisting{};
            explicit sigpipe_guard(context& c) noexcept:call{c}
            {
                ::sigemptyset(&set);::sigaddset(&set,SIGPIPE);
                auto result=::pthread_sigmask(SIG_BLOCK,&set,&old);if(result!=0){static_cast<void>(call.native_failure());return;}
                active=true;::sigset_t pending{};if(::sigpending(&pending)!=0){static_cast<void>(call.native_failure());return;}
                preexisting=::sigismember(&pending,SIGPIPE)==1;
            }
            void consume_new_epipe() noexcept
            {
                if(preexisting||!active)return;
                ::timespec now{};int result{};do{result=::sigtimedwait(&set,nullptr,&now);}while(result<0&&errno==EINTR);
                if(result!=SIGPIPE&&!(result<0&&errno==EAGAIN))static_cast<void>(call.native_failure());
            }
            ~sigpipe_guard() noexcept{if(active&&::pthread_sigmask(SIG_SETMASK,&old,nullptr)!=0)static_cast<void>(call.native_failure());}
        } pipe_signals{call};
        if(call.stopped())return error::ecanceled;
        ::ssize_t transferred{};
        for(;;)
        {
            if(call.stopped())return error::ecanceled;
            auto const current=::fcntl(pinned.descriptor,F_GETFL);if(current<0)return native_error(errno);
            // Root excludes native policy mutation during the group lifetime.
            // Still fail on an observed flag drift rather than silently adapt.
            if(current!=pinned.flags){static_cast<void>(call.native_failure());return error::eio;}
            if(pinned.type==fd_pin::kind::stream_socket||pinned.type==fd_pin::kind::datagram_socket)
            {transferred=writing ? ::send(pinned.descriptor,bounce.get(),wanted,MSG_DONTWAIT|MSG_NOSIGNAL) : ::recv(pinned.descriptor,bounce.get(),wanted,MSG_DONTWAIT);}
            else if(splice_pipe)
            {
                auto const owned_flags=::fcntl(pipe.ends[0],F_GETFL);
                if(owned_flags<0||(owned_flags&O_NONBLOCK)==0){static_cast<void>(call.native_failure());return error::eio;}
                constexpr unsigned splice_nonblock{2u};
                transferred=static_cast<::ssize_t>(::syscall(SYS_splice,pipe.ends[0],nullptr,pinned.descriptor,nullptr,wanted,splice_nonblock));
            }
            else if(positioned)
                transferred=writing?::pwrite64(pinned.descriptor,bounce.get(),wanted,static_cast<::off64_t>(position)):
                    ::pread64(pinned.descriptor,bounce.get(),wanted,static_cast<::off64_t>(position));
            else transferred=writing ? ::write(pinned.descriptor,bounce.get(),wanted) : ::read(pinned.descriptor,bounce.get(),wanted);
            if(transferred>=0)break;
            auto const failure=errno;if(failure==EPIPE)pipe_signals.consume_new_epipe();
            if(failure==EINTR)continue;
            if(failure!=EAGAIN&&failure!=EWOULDBLOCK)return native_error(failure);
            if((pinned.type==fd_pin::kind::stream_socket||pinned.type==fd_pin::kind::datagram_socket)?pinned.guest_nonblock:(pinned.flags&O_NONBLOCK)!=0)return error::eagain; // preserve Guest's nonblocking policy.
            short ready{};auto waited=wait_ready(call,pinned.descriptor,writing?POLLOUT:POLLIN,ready);if(waited!=error::esuccess)return waited;
            if(ready&POLLNVAL)return error::ebadf;
        }
        if(call.stopped())return error::ecanceled; // actual external effect is not rolled back/replayed.
        auto const amount=static_cast<::std::size_t>(transferred);
        if(amount>wanted){static_cast<void>(call.native_failure());return error::eio;}
        if(!writing)
        {
            ::std::size_t at{};
            for(::std::size_t i{};i<count&&at<amount;++i)
            {auto n=(::std::min)(static_cast<::std::size_t>(vectors[i].size),amount-at);if(!call.copy_out(vectors[i].base,{bounce.get()+at,n}))return error::efault;at+=n;}
        }
        return call.store_size(completed,amount)?error::esuccess:error::efault;
    }
}
