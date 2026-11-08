// Included after the actual fd_pin definition, inside uwvm2::runtime::lib.
extern "C++"
{
    inline auto runtime_threaded_wasip1_adapter::close_file(context& call,::std::uint32_t guest) noexcept -> error
    {
        if(guest>INT32_MAX)return error::ebadf;
        return ::uwvm2::imported::wasi::wasip1::func::fd_close_host(call.env,static_cast<::std::int32_t>(guest),
            [&]() noexcept {return call.stopped();},[&](native_fd& selected) noexcept
            {
                for(;;)
                {
                    if(call.stopped())return error::ecanceled;
                    auto const result=::pthread_mutex_trylock(::std::addressof(selected.fd_mutex.mutex));
                    if(result==0)return error::esuccess;
                    if(result!=EBUSY){static_cast<void>(call.native_failure());return error::eio;}
                    ::sched_yield();
                }
            });
    }

    inline auto runtime_threaded_wasip1_adapter::file_position(context& call,::std::uint32_t guest,
        ::std::int64_t offset,::std::uint32_t whence,::std::uint32_t destination,bool tell) noexcept -> error
    {
        if(!call.bounds(destination,8u))return error::efault;
        if((destination&7u)!=0u)return error::einval;
        if(whence>2u)return error::einval;
        fd_pin pinned{};
        auto const acquired=pinned.acquire(call,guest,tell?right::right_fd_tell:right::right_fd_seek,true,false,false,true);
        if(acquired.failure!=error::esuccess)return acquired.failure;
        if(acquired.deferred||pinned.locked==nullptr){static_cast<void>(call.native_failure());return error::eio;}
        auto const* resource=pinned.locked->wasi_fd.ptr;
        if(resource==nullptr){static_cast<void>(call.native_failure());return error::eio;}
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        bool const filesystem_operation=resource->checkpoint_stdio_index<0;
        ::uwvm2::imported::wasi::wasip1::environment::budget_reservation reservation{
            ::uwvm2::imported::wasi::wasip1::environment::budget_kind::filesystem_io,
            call.env.limits,0u,filesystem_operation};
        if(filesystem_operation&&!reservation)return reservation.error<error>();
#endif
        auto const& storage=resource->wasi_fd_storage;
        using kind=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e;
        int native{-1};
        switch(storage.type)
        {
            case kind::file:native=storage.storage.file_fd.native_handle();break;
            case kind::file_observer:native=storage.storage.file_observer.native_handle();break;
            case kind::dir:return error::espipe;
            default:static_cast<void>(call.native_failure());return error::eio;
        }
        if constexpr(sizeof(::std::int64_t)>sizeof(::fast_io::intfpos_t))
        {
            if(offset>(::std::numeric_limits<::fast_io::intfpos_t>::max)() ||
               offset<(::std::numeric_limits<::fast_io::intfpos_t>::lowest)())return error::eoverflow;
        }
        auto const direction=whence==0u ? ::fast_io::seekdir::beg :
            whence==1u ? ::fast_io::seekdir::cur : ::fast_io::seekdir::end;
        if(call.stopped())return error::ecanceled;
        // The complete result range was checked before the cursor effect. No
        // relocation/tag lease spans the native syscall; checked copyback takes
        // a fresh short lease and rejects a concurrently stale output pointer.
        auto const position=::fast_io::posix_seek_nothrow(::fast_io::native_io_observer{native},
            static_cast<::fast_io::intfpos_t>(offset),direction);
        if(!position)
        {
            if(position.error==EISDIR)return error::espipe;
            if(position.error==EBADF)return error::eio;
            return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(
                ::fast_io::error{::fast_io::posix_domain_value,static_cast<::fast_io::error::value_type>(position.error)});
        }
        return call.store<8>(destination,static_cast<::std::uint64_t>(position.position)) ? error::esuccess :
            call.stopped() ? error::ecanceled : error::efault;
    }

    inline auto runtime_threaded_wasip1_adapter::open_file(context& call,::std::array<::std::uint64_t,9> const& arguments) noexcept -> error
    {
        namespace wasi=::uwvm2::imported::wasi::wasip1;
        auto const guest=static_cast<::std::uint32_t>(arguments[0]);
        auto const path_pointer=static_cast<::std::uint32_t>(arguments[2]);
        auto const path_length=static_cast<::std::uint32_t>(arguments[3]);
        auto const destination=static_cast<::std::uint32_t>(arguments[8]);
        if(!call.bounds(path_pointer,path_length)||!call.bounds(destination,4u))return error::efault;
        if((destination&3u)!=0u)return error::einval;
        if((arguments[1]&~1ull)!=0u || (arguments[4]&~15ull)!=0u || (arguments[7]&~31ull)!=0u)return error::einval;
        wasi::func::path_open_parent parent{};
        {
            fd_pin pinned{};
            auto const acquired=pinned.acquire(call,guest,right::right_path_open,true,false,false,true);
            if(acquired.failure!=error::esuccess)return acquired.failure;
            if(acquired.deferred||pinned.locked==nullptr){static_cast<void>(call.native_failure());return error::eio;}
            parent.wasi_fd=pinned.locked->wasi_fd;
            parent.rights_base=pinned.locked->rights_base;
            parent.rights_inherit=pinned.locked->rights_inherit;
        }
        auto const base=static_cast<right>(arguments[5]);
        auto const inheriting=static_cast<right>(arguments[6]);
        if((base&~parent.rights_inherit)!=right{} || (inheriting&~parent.rights_inherit)!=right{})return error::enotcapable;
        ::uwvm2::utils::container::u8string path{};
        path.resize(path_length);
        // The short checked copy releases every relocation/tag lease before
        // quota waiting, path traversal, native open, or any descriptor mutation.
        if(!call.copy_in(path_pointer,{reinterpret_cast<::std::byte*>(path.data()),path.size()}))
            return call.stopped()?error::ecanceled:error::efault;
        wasi::fd_manager::wasi_fd_unique_ptr_t opened{};
        wasi::fd_manager::fd_slot_reservation reserved{};
        auto const failure=wasi::func::path_open_host(call.env,parent,static_cast<wasi::abi::lookupflags_t>(arguments[1]),
            path,static_cast<wasi::abi::oflags_t>(arguments[4]),base,inheriting,
            static_cast<wasi::abi::fdflags_t>(arguments[7]),opened,reserved,[&]() noexcept{return call.stopped();});
        if(failure!=error::esuccess)return failure;
        auto const descriptor=reserved.descriptor();
        // Validate/copy the result before infallible publication, so a failed
        // output copy releases the still-private resource and pending charge.
        if(!call.store<4>(destination,descriptor))return call.stopped()?error::ecanceled:error::efault;
        reserved.commit(*opened.fd_p);
        if(call.env.debug_fd_created!=nullptr)call.env.debug_fd_created(descriptor);
        return error::esuccess;
    }
}
