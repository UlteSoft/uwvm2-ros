// Real bounded poll + owned timerfds + actual Group cancellation fd. No sleep
// loop, fake-ready event, borrowed-global O_NONBLOCK change or ordinary helper.
extern "C++"
{
    inline auto runtime_threaded_wasip1_adapter::poll_oneoff(context& call,::std::uint64_t input,
        ::std::uint64_t output,::std::uint64_t count,::std::uint64_t completed) noexcept -> error
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        if(count > call.env.limits.max_poll_subscriptions)
        { ::uwvm2::utils::control::host_diagnostics::limited("poll-subscriptions",false); return error::e2big; }
#endif
        if(count==0u)return error::einval;
        if((count > SIZE_MAX / 48uz) || count>=SIZE_MAX) return error::eoverflow;
        if(!call.aligned(input,8u)||!call.aligned(output,8u)||!call.aligned(completed,call.word_bytes()))return error::efault;

        if(!call.bounds(input,static_cast<::std::size_t>(count)*48uz)||
           !call.bounds(output,static_cast<::std::size_t>(count)*32uz)||!call.bounds(completed,call.word_bytes()))return error::efault;
        struct subscription
        {
            ::std::uint64_t userdata{},timeout{};::std::uint32_t id{},fd{};unsigned kind{};bool absolute{},immediate{},deferred{};
            error failure{error::esuccess};
        };
        auto subscriptions{allocate_array<subscription>(count)};
        auto pins{allocate_array<fd_pin>(count)};
        auto const watched_count{static_cast<::std::size_t>(count)+1u};
        auto watched{allocate_array<::pollfd>(watched_count)};
        auto descriptors{allocate_array<int>(count)};
        if(!subscriptions || !pins || !watched || !descriptors) return error::enomem;
        for(::std::size_t i{}; i < watched_count; ++i) watched[i].fd=-1;
        for(::std::size_t i{}; i < count; ++i) descriptors[i]=-1;
        struct timers final
        {
            ::std::unique_ptr<int[]> descriptors; ::std::size_t count{};
            ~timers() noexcept{for(::std::size_t i{};i<count;++i)if(descriptors[i]>=0)static_cast<void>(::close(descriptors[i]));}
        } owned{::std::move(descriptors),static_cast<::std::size_t>(count)};
        // Snapshot every subscription before any output can overlap it. Each
        // byte is atomic; torn Guest writes cannot create an unchecked pointer.
        // Check all clock authority before native clocks/FD readiness are read.
        for(::std::size_t i{};i<count;++i)
        {
            ::std::array<::std::byte,48> raw{};
            if(!call.copy_in(static_cast<::std::uint64_t>(input)+i*48u,raw))return error::efault;
            auto& sub=subscriptions[i];sub.userdata=little(raw.data(),8);sub.kind=::std::to_integer<unsigned>(raw[8]);
            if(sub.kind==0u)
            {
                sub.id=static_cast<::std::uint32_t>(little(raw.data()+16,4));sub.timeout=little(raw.data()+24,8);
                auto const flags=little(raw.data()+40,2);if(flags&~1u)return error::einval;sub.absolute=(flags&1u)!=0u;
                if(sub.id>3u)return error::einval;::clockid_t native{};
                if(!clock_native(call,sub.id,native,true))return error::enotcapable;
                if(sub.id>1u)return error::enotsup; // timerfd cannot implement CPU-clock deadlines.
            }
            else if(sub.kind==1u||sub.kind==2u)sub.fd=static_cast<::std::uint32_t>(little(raw.data()+16,4));
            else return error::einval;
        }
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        for(::std::size_t i{}; i<count; ++i)
        {
            auto& sub{subscriptions[i]};
            if(sub.kind == 0u)
            {
                if(call.env.limits.clock_granularity_ns[sub.id] == 0u) return error::enotcapable;
            }
            else
            {
                fd_pin policy{};
                auto const required{right::right_poll_fd_readwrite | (sub.kind==1u ? right::right_fd_read : right::right_fd_write)};
                auto const checked{policy.acquire(call, sub.fd, required, false, true, true)};
                if(checked.failure != error::esuccess) return checked.failure;
                if(checked.deferred) return error::eagain;
            }
        }
#endif
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        // All clocks and descriptor rights have passed preflight. No clock
        // observation may precede a later subscription's authority check.
        try
        {
            for(::std::size_t i{}; i<count; ++i)
            {
                auto& sub{subscriptions[i]};
                if(sub.kind != 0u) continue;
                auto const id{sub.id};
                auto timeout{sub.timeout};
                auto const granularity{call.env.limits.clock_granularity_ns[id]};
                if(auto remainder{timeout % granularity}; remainder != 0u)
                {
                    auto const increment{granularity - remainder};
                    if(timeout > UINT64_MAX - increment) { return error::eoverflow; }
                    timeout += increment;
                }
                if(call.env.limits.max_poll_wait_ns != ::uwvm2::imported::wasi::wasip1::environment::unlimited)
                {
                    auto remaining{timeout};
                    if(sub.absolute)
                    {
                        auto const sample{::fast_io::posix_clock_gettime(id == 0u ? ::fast_io::posix_clock_id::realtime :
                                                                                     ::fast_io::posix_clock_id::monotonic)};
                        if(sample.seconds < 0) { return error::eoverflow; }
                        constexpr ::std::uint64_t billion{1'000'000'000u};
                        constexpr auto fraction{::fast_io::uint_least64_subseconds_per_second / billion};
                        if(static_cast<::std::uint64_t>(sample.seconds) > UINT64_MAX / billion) { remaining = 0u; }
                        else
                        {
                            auto const seconds{static_cast<::std::uint64_t>(sample.seconds) * billion};
                            auto const nanos{static_cast<::std::uint64_t>(sample.subseconds / fraction)};
                            if(seconds > UINT64_MAX - nanos) { remaining = 0u; }
                            else
                            {
                                auto const now{seconds + nanos};
                                remaining = timeout > now ? timeout - now : 0u;
                            }
                        }
                    }
                    if(remaining > call.env.limits.max_poll_wait_ns)
                    { ::uwvm2::utils::control::host_diagnostics::limited("poll-wait-ns",false); return error::eagain; }
                }
                sub.timeout=timeout;
            }
        }
        catch(::fast_io::error const&) { return error::eio; }
        auto const deadline{::uwvm2::imported::wasi::wasip1::environment::deadline_from_ns(call.env.limits.max_poll_wait_ns)};
        // An unlimited deadline performs no observation. Do not reuse a
        // failure flag from an unrelated earlier call; reservation samples anew.
        if(call.env.limits.max_poll_wait_ns <= static_cast<::std::uint64_t>(INT64_MAX) &&
           !::uwvm2::utils::host_clock::available()) return error::eio;
        ::uwvm2::imported::wasi::wasip1::environment::budget_reservation reservation{
            ::uwvm2::imported::wasi::wasip1::environment::budget_kind::poll, call.env.limits, 0u};
        if(!reservation) return reservation.error<error>();
#endif
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        // Register the process-exit/quota event before trying any fd mutex.
        // The real unlock notification cannot then race ahead of registration.
        ::uwvm2::imported::wasi::wasip1::environment::quota_native_wait native_wait{};
        if(!native_wait.prepare())return error::eio;
        struct readiness_set final
        {
            int descriptor{::epoll_create1(EPOLL_CLOEXEC)};
            ~readiness_set() noexcept {if(descriptor>=0)static_cast<void>(::close(descriptor));}
            bool add(::pollfd const& watch) noexcept
            {
                if(watch.fd<0)return true;
                ::epoll_event event{};
                if(watch.events&POLLIN)event.events|=EPOLLIN;
                if(watch.events&POLLOUT)event.events|=EPOLLOUT;
                if(watch.events&0x2000)event.events|=EPOLLRDHUP;
                // Each pin is a distinct owned duplicate, including duplicate
                // Guest subscriptions to one underlying open file description.
                return ::epoll_ctl(descriptor,EPOLL_CTL_ADD,watch.fd,&event)==0;
            }
        } readiness{};
        if(readiness.descriptor<0)return native_error(errno);
#endif
        bool immediate{};
        for(::std::size_t i{};i<count;++i)
        {
            if(call.stopped())return error::ecanceled;
            auto& sub=subscriptions[i];auto& watch=watched[i+1u];
            if(sub.kind==0u)
            {
                ::clockid_t native{};if(!clock_native(call,sub.id,native,true))return error::enotcapable;
                // Zero is already expired. Other absolute deadlines remain
                // kernel absolute timers, including past values; do not freeze
                // realtime into a relative interval before blocking.
                if(sub.timeout==0u) {sub.immediate=true;immediate=true;continue;}
                auto const seconds=sub.timeout/1000000000u;
                if(seconds>static_cast<::std::uint64_t>((::std::numeric_limits<::time_t>::max)()))return error::eoverflow;
                int fd=::timerfd_create(native,TFD_NONBLOCK|TFD_CLOEXEC);
                if(fd<0){sub.failure=native_error(errno);immediate=true;continue;}
                owned.descriptors[i]=fd;
                ::itimerspec timer{};timer.it_value.tv_sec=static_cast<::time_t>(seconds);timer.it_value.tv_nsec=static_cast<long>(sub.timeout%1000000000u);
                if(::timerfd_settime(fd,sub.absolute?TFD_TIMER_ABSTIME:0,&timer,nullptr)!=0)
                {sub.failure=native_error(errno);immediate=true;continue;}
                watch={fd,POLLIN,0};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                if(!readiness.add(watch))return native_error(errno);
#endif
            }
            else sub.deferred=true;
            // Establish ALL timers before attempting any resource pin. A
            // reader owning an FD mutex cannot hide an unrelated deadline.

        }
        auto const stop=call.actual.cancellation_fd();if(stop<0){static_cast<void>(call.native_failure());return error::eio;}
        watched[0]={stop,POLLIN,0};
        for(;;)
        {
            if(call.stopped())return error::ecanceled;
#if !defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            bool pending{};
#endif
            for(::std::size_t i{};i<count;++i)
            {
                auto& sub=subscriptions[i];if(!sub.deferred)continue;
                auto const required=right::right_poll_fd_readwrite|(sub.kind==1u?right::right_fd_read:right::right_fd_write);
                auto const acquired=pins[i].acquire(call,sub.fd,required,false,true);
                // Even a failed attempt may own the actual descriptor mutex;
                // release it before any duplicate/cross subscription or wait.
                pins[i].unlock();
                if(acquired.deferred)
                {
#if !defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                    pending=true;
#endif
                    continue;
                }
                sub.deferred=false;sub.failure=acquired.failure;
                if(sub.failure!=error::esuccess){immediate=true;continue;}
                short interest=static_cast<short>(sub.kind==1u?POLLIN:POLLOUT);
                // Linux UAPI POLLRDHUP: a TCP peer FIN can be read-hung-up
                // without POLLHUP while this endpoint remains writable.
                constexpr short linux_pollrdhup{0x2000};
                if(pins[i].type==fd_pin::kind::stream_socket)interest|=linux_pollrdhup;
                watched[i+1u]={pins[i].descriptor,interest,0};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                if(!readiness.add(watched[i+1u]))return native_error(errno);
#endif
            }
            for(::std::size_t i{};i<watched_count;++i)watched[i].revents=0;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            // Observe real readiness without blocking first. The persistent
            // native waiter combines the aggregate kernel readiness fd, the
            // actual ChildLease cancellation fd, process-exit broadcast and a
            // real finite deadline timer. No periodic 1/10ms polling remains.
            auto result=::poll(watched.get(),static_cast<::nfds_t>(count)+1u,0);
            if(result<0){if(errno==EINTR)continue;return native_error(errno);}
            if(!immediate && result==0)
            {
                if(::uwvm2::imported::wasi::wasip1::environment::host_wait_cancelled())return error::ecanceled;
                if(::uwvm2::utils::host_clock::now()>=deadline)return error::etimedout;
                if(!native_wait.wait(deadline,stop,readiness.descriptor,POLLIN))return error::eio;
                if(::uwvm2::imported::wasi::wasip1::environment::host_wait_cancelled())return error::ecanceled;
                continue; // Notification is not a Guest event: recheck pins and all native fds.
            }
#else
            auto result=::poll(watched.get(),static_cast<::nfds_t>(count)+1u,immediate?0:(pending?1:-1));
            if(result<0){if(errno==EINTR)continue;return native_error(errno);}
#endif
            if(call.stopped())return error::ecanceled;
            if(watched[0].revents!=0){static_cast<void>(call.native_failure());return error::eio;}
            ::std::uint64_t written{};
            for(::std::size_t i{};i<count;++i)
            {
                auto const& sub=subscriptions[i];auto const revents=watched[i+1u].revents;
                if(!sub.immediate&&sub.failure==error::esuccess&&revents==0)continue;
                auto failure=sub.failure;
                if(revents&POLLNVAL)failure=error::ebadf;
                else if(revents&POLLERR)failure=error::eio;
                ::std::array<::std::byte,32> event{};
                little_put(event.data(),sub.userdata,8);little_put(event.data()+8,static_cast<::std::uint16_t>(failure),2);
                event[10]=static_cast<::std::byte>(sub.kind);
                if(sub.kind!=0u)
                {
                    // Snapshot availability for these authenticated kernel
                    // pipe/socket classes. It is advisory: another reader may
                    // win between poll/FIONREAD/the later actual I/O call.
                    ::std::uint64_t available{};
                    if(failure==error::esuccess&&sub.kind==1u)
                    {
                        int bytes{};
                        if(::ioctl(pins[i].descriptor,FIONREAD,&bytes)!=0)
                        {failure=native_error(errno);little_put(event.data()+8,static_cast<::std::uint16_t>(failure),2);}
                        else if(bytes>0)available=static_cast<unsigned>(bytes);
                    }
                    else if(failure==error::esuccess&&(revents&POLLOUT))available=1u;
                    // Write readiness guarantees progress at its observation,
                    // not an exact portable socket send-buffer byte count.
                    little_put(event.data()+16,available,8);
                    constexpr short linux_pollrdhup{0x2000};
                    little_put(event.data()+24,(revents&(POLLHUP|linux_pollrdhup))?1u:0u,2);
                }
                if(!call.copy_out(static_cast<::std::uint64_t>(output)+written*32u,event))return error::efault;++written;
            }
            if(written)return call.store_size(completed,written)?error::esuccess:error::efault;
            // Native spurious wakeup: retry only through the actual stop fd.
            immediate=false;
        }
    }
}
