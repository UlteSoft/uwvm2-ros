// Included inside Runtime lib. Shared guest bytes are accessed only atomically;
// native sockets see private buffers, never concurrently mutable guest vectors.
extern "C++"
{
    inline auto runtime_threaded_wasip1_adapter::socket_io(context& call,bool writing,::std::uint32_t guest,
        ::std::uint32_t iovs,::std::uint32_t count,::std::uint32_t flags,::std::uint32_t completed,::std::uint32_t returned_flags) noexcept -> error
    {
        if(writing ? flags!=0u : (flags&~3u)!=0u)return error::einval;
        if(count!=0u && 8uz>SIZE_MAX/static_cast<::std::size_t>(count))return error::eoverflow;
        if(!call.aligned(iovs,4u)||!call.aligned(completed,4u)||(!writing&&!call.aligned(returned_flags,2u)))return error::efault;
        
        if(!call.bounds(iovs,static_cast<::std::size_t>(count)*8uz)||!call.bounds(completed,4u)||
            (!writing&&!call.bounds(returned_flags,2u)))return error::efault;
        struct vector {::std::uint32_t base{},size{};};
        auto vectors=allocate_array<vector>(count);if(count&&!vectors)return error::enomem;
        ::std::uint64_t total{};
        for(::std::uint32_t i{};i<count;++i)
        {
            ::std::array<::std::byte,8> raw{};
            if(!call.copy_in(static_cast<::std::uint64_t>(iovs)+static_cast<::std::uint64_t>(i)*8u,raw))return error::efault;
            auto& v=vectors[i];v.base=static_cast<::std::uint32_t>(little(raw.data(),4));v.size=static_cast<::std::uint32_t>(little(raw.data()+4,4));
            if(!call.bounds(v.base,v.size))return error::efault;
            if(v.size>UINT32_MAX-total||v.size>static_cast<::std::uint64_t>(SSIZE_MAX)-total)return error::eoverflow;
            total+=v.size;
        }
        fd_pin pinned{};auto acquired=pinned.acquire(call,guest,writing?right::right_fd_write:right::right_fd_read,true);
        if(acquired.failure!=error::esuccess)return acquired.failure;
        if(pinned.type!=fd_pin::kind::stream_socket&&pinned.type!=fd_pin::kind::datagram_socket)return error::enotsock;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        using policy=::uwvm2::imported::wasi::wasip1::environment::budget_kind;
        ::uwvm2::imported::wasi::wasip1::environment::budget_reservation reservation{
            writing?policy::socket_write:policy::socket_read,call.env.limits,total};
        if(!reservation)return reservation.error<error>();
#endif
        bool const peek=!writing&&(flags&1u)!=0u;
        bool const waitall=!writing&&(flags&2u)!=0u&&pinned.type==fd_pin::kind::stream_socket;
        // Datagram boundaries and WAITALL require the whole legal request.
        // Ordinary streams may return a partial transfer, as native recv/send do.
        auto const capacity=(pinned.type==fd_pin::kind::datagram_socket||waitall)?total:
            (::std::min)(total,static_cast<::std::uint64_t>(transfer_bytes));
        if(capacity>SIZE_MAX)return error::eoverflow;
        auto const wanted=static_cast<::std::size_t>(capacity);
        auto bounce=allocate_array<::std::byte>(wanted);if(wanted&&!bounce)return error::enomem;
        if(writing)
        {
            ::std::size_t at{};
            for(::std::uint32_t i{};i<count&&at<wanted;++i)
            {auto n=(::std::min)(static_cast<::std::size_t>(vectors[i].size),wanted-at);
             if(!call.copy_in(vectors[i].base,{bounce.get()+at,n}))return error::efault;at+=n;}
        }
        auto publish=[&](::std::size_t amount,unsigned truncated) noexcept
        {
            if(!writing)
            {
                ::std::size_t at{};
                for(::std::uint32_t i{};i<count&&at<amount;++i)
                {auto n=(::std::min)(static_cast<::std::size_t>(vectors[i].size),amount-at);
                 if(!call.copy_out(vectors[i].base,{bounce.get()+at,n}))return error::efault;at+=n;}
                if(!call.store<2>(returned_flags,truncated))return error::efault;
            }
            return call.store<4>(completed,amount)?error::esuccess:error::efault;
        };
        if(count==0u)return publish(0u,0u);
        struct edge_watch {int fd{-1};~edge_watch(){if(fd>=0)static_cast<void>(::close(fd));}} edge;
        if(peek&&waitall&&!pinned.guest_nonblock)
        {
            edge.fd=::epoll_create1(EPOLL_CLOEXEC);if(edge.fd<0)return native_error(errno);
            ::epoll_event event{};event.events=EPOLLIN|EPOLLRDHUP|EPOLLET;
            if(::epoll_ctl(edge.fd,EPOLL_CTL_ADD,pinned.descriptor,&event)<0)return native_error(errno);
        }
        ::std::size_t done{};unsigned truncated{};bool final_peek{};
        for(;;)
        {
            if(call.stopped())return error::ecanceled;
            ::iovec vector{bounce.get(),wanted};
            if(!peek&&done!=0u){vector.iov_base=bounce.get()+done;vector.iov_len=wanted-done;}
            ::msghdr message{};message.msg_iov=&vector;message.msg_iovlen=1;
            int const native_flags=MSG_DONTWAIT|MSG_NOSIGNAL|(peek?MSG_PEEK:0);
            auto const n=writing?::sendmsg(pinned.descriptor,&message,native_flags):
                ::recvmsg(pinned.descriptor,&message,native_flags);
            if(n>=0)
            {
                auto const amount=static_cast<::std::size_t>(n);
                if(amount>vector.iov_len){static_cast<void>(call.native_failure());return error::eio;}
                if(message.msg_flags&MSG_TRUNC)truncated=1u;
                done=peek?amount:done+amount;
                if(writing||!waitall||done==wanted||n==0||pinned.guest_nonblock||final_peek)return publish(done,truncated);
                if(!peek)continue;
            }
            else
            {
                auto const failure=errno;
                if(failure==EINTR)continue;
                if(failure!=EAGAIN&&failure!=EWOULDBLOCK)return done?publish(done,truncated):native_error(failure);
                if(final_peek)return publish(done,truncated);
            }
            if(pinned.guest_nonblock)return done?publish(done,truncated):error::eagain;
            short ready{};auto waited=wait_ready(call,edge.fd>=0?edge.fd:pinned.descriptor,
                edge.fd>=0?POLLIN:(writing?POLLOUT:POLLIN),ready);
            if(waited!=error::esuccess)return done&&!call.stopped()?publish(done,truncated):waited;
            if(ready&POLLNVAL)return done?publish(done,truncated):error::ebadf;
            if(edge.fd>=0)
            {
                ::epoll_event event{};auto const observed=::epoll_wait(edge.fd,&event,1,0);
                if(observed<0&&errno!=EINTR)return done?publish(done,truncated):native_error(errno);
                final_peek=observed>0&&(event.events&(EPOLLRDHUP|EPOLLHUP|EPOLLERR))!=0;
            }
        }
    }

    inline auto runtime_threaded_wasip1_adapter::socket_shutdown(context& call,::std::uint32_t guest,::std::uint32_t how) noexcept -> error
    {
        if(how==0u||how>3u)return error::einval;
        fd_pin pinned{};auto acquired=pinned.acquire(call,guest,right::right_sock_shutdown,true);
        if(acquired.failure!=error::esuccess)return acquired.failure;
        if(pinned.type!=fd_pin::kind::stream_socket&&pinned.type!=fd_pin::kind::datagram_socket)return error::enotsock;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        ::uwvm2::imported::wasi::wasip1::environment::budget_reservation reservation{
            ::uwvm2::imported::wasi::wasip1::environment::budget_kind::socket_shutdown,call.env.limits,0u};
        if(!reservation)return reservation.error<error>();
#endif
        if(call.stopped())return error::ecanceled;
        return ::shutdown(pinned.descriptor,how==1u?SHUT_RD:how==2u?SHUT_WR:SHUT_RDWR)==0?error::esuccess:native_error(errno);
    }

    inline auto runtime_threaded_wasip1_adapter::socket_accept(context& call,::std::uint32_t guest,::std::uint32_t flags,
        ::std::uint32_t completed,bool with_address,::std::uint32_t address) noexcept -> error
    {
        if((flags&~4u)!=0u)return error::einval;
        if(!call.aligned(completed,4u))return error::efault;
        
        if(!call.bounds(completed,4u)||(with_address&&!call.bounds(address,20u)))return error::efault;
        fd_pin pinned{};auto acquired=pinned.acquire(call,guest,right::right_sock_accept,true);
        if(acquired.failure!=error::esuccess)return acquired.failure;
        namespace manager=::uwvm2::imported::wasi::wasip1::fd_manager;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        using socket_type=manager::wasi_fd_rc_t::socket_type;
        if(pinned.locked->wasi_fd.ptr->socket_kind!=socket_type::listener)return error::enotsock;
        ::uwvm2::imported::wasi::wasip1::environment::budget_reservation reservation{
            ::uwvm2::imported::wasi::wasip1::environment::budget_kind::socket_accept,call.env.limits,0u};
        if(!reservation)return reservation.error<error>();
#else
        if(pinned.type!=fd_pin::kind::stream_socket)return error::enotsock;
#endif
        ::sockaddr_storage peer{};::socklen_t length{sizeof(peer)};int accepted{-1};
        for(;;)
        {
            if(call.stopped())return error::ecanceled;
            accepted=::accept4(pinned.descriptor,reinterpret_cast<::sockaddr*>(&peer),&length,SOCK_CLOEXEC|SOCK_NONBLOCK);
            if(accepted>=0)break;
            auto const failure=errno;if(failure==EINTR)continue;
            if(failure!=EAGAIN&&failure!=EWOULDBLOCK)return native_error(failure);
            if(pinned.guest_nonblock)return error::eagain;
            short ready{};auto waited=wait_ready(call,pinned.descriptor,POLLIN,ready);
            if(waited!=error::esuccess)return waited;
            if(ready&POLLNVAL)return error::ebadf;
        }
        ::fast_io::posix_file native{accepted};
        manager::wasi_fd_unique_ptr_t child{};
        child.fd_p->rights_inherit={};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        child.fd_p->rights_base=pinned.locked->rights_inherit&::uwvm2::imported::wasi::wasip1::environment::socket_data_rights;
        auto& resource=*child.fd_p->wasi_fd.ptr;
        resource.socket_kind=socket_type::stream;resource.socket_nonblock=(flags&4u)!=0u;
        resource.socket_initial_fd=pinned.locked->wasi_fd.ptr->socket_initial_fd;
#else
        child.fd_p->rights_base=pinned.locked->rights_inherit;
        auto& resource=*child.fd_p->wasi_fd.ptr;
#endif
        resource.wasi_fd_storage.reset_type(manager::wasi_fd_type_e::file);
        resource.wasi_fd_storage.storage.file_fd=::std::move(native);
        pinned.unlock();
        auto& table=call.env.fd_storage;::std::uint32_t number{};
        {
            ::uwvm2::utils::mutex::rw_fair_unique_guard_t lock{table.fds_rwlock};
            if(call.stopped())return error::ecanceled;
            auto const live=table.opens.size()-table.closes.size()+table.renumber_map.size();
            if(live>=table.fd_limit)return error::emfile;
            if(!table.closes.empty())
            {
                auto index=table.closes.back_unchecked();table.closes.pop_back_unchecked();
                table.opens[index]=::std::move(child);number=static_cast<::std::uint32_t>(index);
            }
            else
            {
                if(table.opens.size()>=INT32_MAX)return error::emfile;
                number=static_cast<::std::uint32_t>(table.opens.size());table.opens.emplace_back(::std::move(child));
                while(table.opens.size()<INT32_MAX)
                {
                    auto it=table.renumber_map.find(static_cast<int>(table.opens.size()));
                    if(it==table.renumber_map.end())break;
                    table.opens.push_back(it->second.release());table.renumber_map.erase(it);
                }
            }
        }
        if(with_address)
        {
            ::std::array<::std::byte,20> raw{};
            if(peer.ss_family==AF_INET&&length>=sizeof(::sockaddr_in))
            {
                ::sockaddr_in ip{};::std::memcpy(&ip,&peer,sizeof(ip));raw[0]=::std::byte{1};
                ::std::memcpy(raw.data()+2,&ip.sin_port,2);::std::memcpy(raw.data()+4,&ip.sin_addr,4);
            }
            else if(peer.ss_family==AF_INET6&&length>=sizeof(::sockaddr_in6))
            {
                ::sockaddr_in6 ip{};::std::memcpy(&ip,&peer,sizeof(ip));raw[0]=::std::byte{2};
                ::std::memcpy(raw.data()+2,&ip.sin6_port,2);::std::memcpy(raw.data()+4,&ip.sin6_addr,16);
            }
            if(!call.copy_out(address,raw))return error::efault;
        }
        return call.store<4>(completed,number)?error::esuccess:error::efault;
    }
}
