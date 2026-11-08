// Licensed under APL-2.0 (see LICENSE). Included by base.h after its bounds/errno helpers.
#pragma once
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS) && defined(__linux__)
UWVM_MODULE_EXPORT namespace uwvm2::imported::wasi::wasip1::func::socket_capability
{
    namespace abi=::uwvm2::imported::wasi::wasip1::abi;
    namespace policy=::uwvm2::imported::wasi::wasip1::environment;
    namespace manager=::uwvm2::imported::wasi::wasip1::fd_manager;
    namespace memops=::uwvm2::imported::wasi::wasip1::memory;
    using error=abi::errno_t;
    using socket_type=manager::wasi_fd_rc_t::socket_type;
    using env_type=policy::wasip1_environment<::uwvm2::object::memory::linear::native_memory_t>;
    inline error native_error() noexcept
    {return path_errno_from_fast_io_error(::fast_io::error{::fast_io::posix_domain_value,static_cast<::fast_io::error::value_type>(errno)});}
    inline int native_descriptor(manager::wasi_fd_t const& fd) noexcept
    {
        if(!fd.wasi_fd.ptr)return -1;
        auto const& value=fd.wasi_fd.ptr->wasi_fd_storage;
        if(value.type==manager::wasi_fd_type_e::file)return value.storage.file_fd.native_handle();
        if(value.type==manager::wasi_fd_type_e::file_observer)return value.storage.file_observer.native_handle();
        return -1;
    }
    inline bool granted(manager::wasi_fd_t const& fd,abi::rights_t right) noexcept
    {
        return (fd.rights_base&right)==right;
    }
    struct locked_fd
    {
        manager::wasi_fd_t* object{};
        ::uwvm2::utils::mutex::mutex_merely_release_guard_t lock{};
        locked_fd(env_type& env,int number) noexcept
        {
            if(number<0)return;
            auto& table=env.fd_storage;
            ::uwvm2::utils::mutex::rw_fair_shared_guard_t map_lock{table.fds_rwlock};
            if(static_cast<::std::size_t>(number)<table.opens.size())object=table.opens.index_unchecked(number).fd_p;
            else if(auto it=table.renumber_map.find(number);it!=table.renumber_map.end())object=it->second.fd_p;
            if(!object)return;
            lock.device_p=::std::addressof(object->fd_mutex);lock.lock();
            if(object->close_pos!=SIZE_MAX || !object->wasi_fd.ptr)object=nullptr;
        }
        locked_fd(locked_fd const&)=delete;
        void unlock() noexcept {lock.unlock();lock.device_p=nullptr;}
    };
    template<unsigned Width,typename Memory>
    inline ::std::uint64_t read_word(Memory const& memory,::std::uint64_t offset) noexcept
    {
        static_assert(Width==1u || Width==2u || Width==4u || Width==8u);
        using integer=::std::conditional_t<Width==1u,::std::uint8_t,::std::conditional_t<Width==2u,::std::uint16_t,::std::conditional_t<Width==4u,::std::uint32_t,::std::uint64_t>>>;
        return memops::tagged_read_value_unlocked<integer>(memory,offset);
    }
    template<unsigned Width,typename Memory>
    inline void write_word(Memory& memory,::std::uint64_t offset,::std::uint64_t value) noexcept
    {
        static_assert(Width==1u || Width==2u || Width==4u || Width==8u);
        using integer=::std::conditional_t<Width==1u,::std::uint8_t,::std::conditional_t<Width==2u,::std::uint16_t,::std::conditional_t<Width==4u,::std::uint32_t,::std::uint64_t>>>;
        memops::tagged_store_value_unlocked(memory,offset,static_cast<integer>(value));
    }

    // Shared by sock_recv/send and fd_read/write. Callers hold the descriptor
    // lock, so rights reduction/renumber cannot race resource use. No FD-number
    // based stdio/filesystem branch can change this socket's quota class.
    template<unsigned Width>
    inline error io(env_type& env,manager::wasi_fd_t& fd,::std::uint64_t vectors,
        ::std::uint64_t count,unsigned flags,::std::uint64_t result,::std::uint64_t out_flags,
        bool writing,bool socket_call) noexcept
    {
        auto* memory=env.get_memory();if(!memory)return error::efault;
        auto const required=writing?abi::rights_t::right_fd_write:abi::rights_t::right_fd_read;
        if(!granted(fd,required))return error::enotcapable;
        auto const& resource=*fd.wasi_fd.ptr;
        if(resource.socket_kind==socket_type::none)return error::enotsock;
        if(writing?flags!=0u:(flags&~3u)!=0u)return error::einval;
        if(vectors%Width || result%Width || (!writing&&socket_call&&out_flags%2u))return error::einval;
        auto const descriptor=native_descriptor(fd);if(descriptor<0)return error::ebadf;
        [[maybe_unused]] auto guard=memops::lock_memory(*memory);
        if(!wasip1_guest_range_valid_unlocked(*memory,result,::std::uint64_t{Width}) ||
           (!writing&&socket_call&&!wasip1_guest_range_valid_unlocked(*memory,out_flags,::std::uint64_t{2u})))return error::efault;
        if(count>UINT64_MAX/(Width*2u)||!wasip1_guest_range_valid_unlocked(*memory,vectors,count*(Width*2u)))return error::efault;
        if(count>SIZE_MAX/sizeof(::iovec))return error::eoverflow;
        // The common one/few-vector path needs no allocation. Larger valid
        // vectors remain legal; only the native sendmsg/recvmsg ABI limits apply.
        ::std::array<::iovec,16u> small{};::std::vector<::iovec> large;
        ::iovec* data=small.data();
        if(count>small.size())
        {
#if defined(__cpp_exceptions)
            try {large.resize(static_cast<::std::size_t>(count));}
            catch(::std::bad_alloc const&){return error::enomem;}
            catch(::std::length_error const&){return error::eoverflow;}
#else
            large.resize(static_cast<::std::size_t>(count));
#endif
            data=large.data();
        }
        ::std::uint64_t total{};
        for(::std::uint64_t i{};i<count;++i)
        {
            auto const offset=read_word<Width>(*memory,vectors+i*Width*2u);
            auto const length=read_word<Width>(*memory,vectors+i*Width*2u+Width);
            if(!wasip1_guest_range_valid_unlocked(*memory,offset,length))return error::efault;
            constexpr auto maximum=Width==4u?::std::uint64_t{UINT32_MAX}:UINT64_MAX;
            if(length>maximum-total || length>static_cast<::std::uint64_t>(SSIZE_MAX)-total)return error::eoverflow;
            total+=length;
            auto const span=memops::checked_span_unlocked(*memory,offset,length);
            if(!span)return error::efault;
            data[i]={span.data,span.bytes};
        }
        auto const budget=writing?policy::budget_kind::socket_write:policy::budget_kind::socket_read;
        policy::budget_reservation reservation{budget,env.limits,total};
        if(!reservation)return reservation.template error<error>();
        if(env.debug_io_observed)env.debug_io_observed(total,0u,false);
        auto publish=[&](::std::size_t done,unsigned truncated) noexcept
        {
            write_word<Width>(*memory,result,done);
            if(!writing&&socket_call)write_word<2u>(*memory,out_flags,truncated);
            if(env.debug_io_observed)env.debug_io_observed(total,done,true);
            return error::esuccess;
        };
        if(count==0u)return publish(0u,0u);
        bool const peek=!writing&&(flags&1u)!=0u;
        bool const waitall=!writing&&(flags&2u)!=0u&&resource.socket_kind==socket_type::stream;
        // PEEK must not consume/replay bytes while waiting for the remainder.
        // Edge-triggered readiness waits for NEW arrivals, unlike level poll
        // which would spin on the bytes already peeked. No shared SO_RCVLOWAT
        // or inherited OFD flags are changed.
        struct edge_watch {int fd{-1};~edge_watch(){if(fd>=0)::close(fd);}} edge;
        if(peek&&waitall&&!resource.socket_nonblock)
        {
            edge.fd=::epoll_create1(EPOLL_CLOEXEC);if(edge.fd<0)return native_error();
            ::epoll_event event{};event.events=EPOLLIN|EPOLLRDHUP|EPOLLET;
            if(::epoll_ctl(edge.fd,EPOLL_CTL_ADD,descriptor,&event)<0)return native_error();
        }
        ::std::size_t done{},index{};unsigned truncated{};
        for(;;)
        {
            if(policy::host_wait_cancelled())return done?publish(done,truncated):error::ecanceled;
            ::msghdr message{};message.msg_iov=data+index;message.msg_iovlen=static_cast<::std::size_t>(count)-index;
            int native_flags=MSG_DONTWAIT|MSG_NOSIGNAL|(peek?MSG_PEEK:0);
            auto n=writing?::sendmsg(descriptor,&message,native_flags) : ::recvmsg(descriptor,&message,native_flags);
            if(n>=0)
            {
                if(message.msg_flags&MSG_TRUNC)truncated=1u;
                if(peek)done=static_cast<::std::size_t>(n);else done+=static_cast<::std::size_t>(n);
                if(writing || !waitall || done==total || n==0 || resource.socket_nonblock)return publish(done,truncated);
                if(!peek)
                {
                    auto used=static_cast<::std::size_t>(n);
                    while(index<count && used>=data[index].iov_len){used-=data[index].iov_len;++index;}
                    if(index<count){data[index].iov_base=static_cast<::std::byte*>(data[index].iov_base)+used;data[index].iov_len-=used;}
                    continue;
                }
            }
            else if(errno!=EAGAIN&&errno!=EWOULDBLOCK)
                return done?publish(done,truncated):native_error();
            if(resource.socket_nonblock)return done?publish(done,truncated):error::eagain;
            auto const wait=policy::wait_for_native_io(edge.fd>=0?edge.fd:descriptor,edge.fd>=0?POLLIN:(writing?POLLOUT:POLLIN));
            if(wait!=policy::quota_call_result::completed)
                return done?publish(done,truncated):wait==policy::quota_call_result::cancelled?error::ecanceled:error::eio;
            if(edge.fd>=0)
            {
                ::epoll_event event{};auto observed=::epoll_wait(edge.fd,&event,1,0);
                if(observed<0&&errno!=EINTR)return done?publish(done,truncated):native_error();
                if(observed>0&&(event.events&(EPOLLRDHUP|EPOLLHUP|EPOLLERR)))
                {
                    // One final peek obtains bytes arriving with FIN; it never
                    // turns a partial transfer into a false cancellation.
                    message.msg_flags=0;n=::recvmsg(descriptor,&message,native_flags);
                    return n>=0?publish(static_cast<::std::size_t>(n),0u):done?publish(done,0u):native_error();
                }
            }
        }
    }
    template<unsigned Width>
    inline error transfer(env_type& env,int number,::std::uint64_t vectors,::std::uint64_t count,
        unsigned flags,::std::uint64_t result,::std::uint64_t out_flags,bool writing) noexcept
    {
        locked_fd fd{env,number};if(!fd.object)return error::ebadf;
        return io<Width>(env,*fd.object,vectors,count,flags,result,out_flags,writing,true);
    }
    inline error shutdown(env_type& env,int number,unsigned how) noexcept
    {
        locked_fd fd{env,number};if(!fd.object)return error::ebadf;
        if(!granted(*fd.object,abi::rights_t::right_sock_shutdown))return error::enotcapable;
        if(fd.object->wasi_fd.ptr->socket_kind==socket_type::none)return error::enotsock;
        if(how==0u||how>3u)return error::einval;
        policy::budget_reservation reservation{policy::budget_kind::socket_shutdown,env.limits,0u};
        if(!reservation)return reservation.template error<error>();
        if(::shutdown(native_descriptor(*fd.object),how==1u?SHUT_RD:how==2u?SHUT_WR:SHUT_RDWR)<0)return native_error();
        return error::esuccess;
    }
    template<unsigned Width>
    inline error accept(env_type& env,int number,unsigned flags,::std::uint64_t result,
        bool with_address=false,::std::uint64_t address=0u) noexcept
    {
        auto* memory=env.get_memory();if(!memory)return error::efault;
        if(result%4u || (flags&~4u)!=0u)return error::einval;
        {
            [[maybe_unused]] auto guard=memops::lock_memory(*memory);
            if(!wasip1_guest_range_valid_unlocked(*memory,result,::std::uint64_t{4}) ||
               (with_address&&!wasip1_guest_range_valid_unlocked(*memory,address,::std::uint64_t{20})))return error::efault;
        }
        locked_fd parent{env,number};if(!parent.object)return error::ebadf;
        if(!granted(*parent.object,abi::rights_t::right_sock_accept))return error::enotcapable;
        auto const& source=*parent.object->wasi_fd.ptr;
        if(source.socket_kind!=socket_type::listener)return error::enotsock;
        policy::budget_reservation reservation{policy::budget_kind::socket_accept,env.limits,0u};
        if(!reservation)return reservation.template error<error>();
        ::sockaddr_storage peer{};::socklen_t length{sizeof(peer)};int accepted{-1};
        for(;;)
        {
            if(policy::host_wait_cancelled())return error::ecanceled;
            accepted=::accept4(native_descriptor(*parent.object),reinterpret_cast<::sockaddr*>(&peer),&length,SOCK_CLOEXEC|SOCK_NONBLOCK);
            if(accepted>=0)break;
            if(errno!=EAGAIN&&errno!=EWOULDBLOCK)return native_error();
            // flags describe the CHILD, not whether this accept should wait.
            if(source.socket_nonblock)return error::eagain;
            auto wait=policy::wait_for_native_io(native_descriptor(*parent.object),POLLIN);
            if(wait!=policy::quota_call_result::completed)return wait==policy::quota_call_result::cancelled?error::ecanceled:error::eio;
        }
        ::fast_io::posix_file native{accepted};
        manager::wasi_fd_unique_ptr_t child{};
        child.fd_p->rights_base=parent.object->rights_inherit & policy::socket_data_rights;
        child.fd_p->rights_inherit={};
        auto& resource=*child.fd_p->wasi_fd.ptr;
        resource.socket_kind=socket_type::stream;resource.socket_nonblock=(flags&4u)!=0u;
        resource.socket_initial_fd=source.socket_initial_fd;
        resource.wasi_fd_storage.reset_type(manager::wasi_fd_type_e::file);
        resource.wasi_fd_storage.storage.file_fd=::std::move(native);
        parent.unlock();
        auto& table=env.fd_storage;int new_number{};
        {
            ::uwvm2::utils::mutex::rw_fair_unique_guard_t lock{table.fds_rwlock};
            auto const live=table.opens.size()-table.closes.size()+table.renumber_map.size();
            if(live>=table.fd_limit)return error::emfile;
            if(!table.closes.empty())
            {
                auto index=table.closes.back_unchecked();table.closes.pop_back_unchecked();
                table.opens[index]=::std::move(child);new_number=static_cast<int>(index);
            }
            else
            {
                if(table.opens.size()>=INT32_MAX)return error::emfile;
                new_number=static_cast<int>(table.opens.size());table.opens.emplace_back(::std::move(child));
                while(table.opens.size()<INT32_MAX)
                {
                    auto it=table.renumber_map.find(static_cast<int>(table.opens.size()));
                    if(it==table.renumber_map.end())break;
                    table.opens.push_back(it->second.release());table.renumber_map.erase(it);
                }
            }
        }
        [[maybe_unused]] auto guard=memops::lock_memory(*memory);
        // Memory cannot shrink during the active call. Resolve its potentially
        // grown base again; never retain a pointer across native accept.
        write_word<4>(*memory,result,static_cast<unsigned>(new_number));
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
            if(!memops::copy_guest_bytes_unlocked<true>(*memory,address,raw.data(),raw.size()))return error::efault;
        }
        if(env.debug_fd_created)env.debug_fd_created(static_cast<unsigned>(new_number));
        return error::esuccess;
    }
    inline error accept_wasix(env_type& env,abi::wasi_posix_fd_t fd,abi::fdflags_t flags,
        abi::wasi_void_ptr_t result,abi::wasi_void_ptr_t address) noexcept
    {
        if(!env.allows_interface(u8"sock_accept"))return error::enotcapable;
        return accept<4>(env,fd,static_cast<unsigned>(flags),result,true,address);
    }
}
#endif
