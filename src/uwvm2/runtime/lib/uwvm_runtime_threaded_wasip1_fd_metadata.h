// Private shared-memory metadata leaves; never forward ordinary Guest memcpy.
extern "C++"
{
    inline auto runtime_threaded_wasip1_adapter::fdstat_rights(context& call,::std::uint32_t guest,
        ::std::uint64_t requested_base,::std::uint64_t requested_inheriting) noexcept -> error
    {
        fd_pin pinned{};
        // Like canonical Preview1, authority may only be reduced. No separate
        // read/write grant is needed to discard rights on an existing binding.
        auto const acquired=pinned.acquire(call,guest,right{},true,false,false,true,false);
        if(acquired.failure!=error::esuccess)return acquired.failure;
        if(acquired.deferred||pinned.locked==nullptr)
        {static_cast<void>(call.native_failure());return error::eio;}
        auto& descriptor=*pinned.locked;
        auto const base=static_cast<right>(requested_base);
        auto const inheriting=static_cast<right>(requested_inheriting);
        if((base&~descriptor.rights_base)!=right{}||(inheriting&~descriptor.rights_inherit)!=right{})
            return error::enotcapable;
        if(call.stopped())return error::ecanceled;
        descriptor.rights_base=base;
        descriptor.rights_inherit=inheriting;
        return error::esuccess;
    }

    inline auto runtime_threaded_wasip1_adapter::fdstat_get(context& call,::std::uint32_t guest,
        ::std::uint64_t destination) noexcept -> error
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        if(guest<3u&&(!call.env.grants_stdio(guest)||!call.env.permits(permission::stdio_metadata)))
            return error::enotcapable;
#endif
        if(!call.bounds(destination,24u))return error::efault;
        if((destination&7u)!=0u)return error::einval;
        fd_pin pinned{};
        auto const acquired=pinned.acquire(call,guest,right{},true,false,false,true);
        if(acquired.failure!=error::esuccess)return acquired.failure;
        if(acquired.deferred||pinned.locked==nullptr)
        {static_cast<void>(call.native_failure());return error::eio;}
        auto const& descriptor=*pinned.locked;
        auto const* resource=descriptor.wasi_fd.ptr;
        if(resource==nullptr){static_cast<void>(call.native_failure());return error::eio;}
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        // Resource identity follows renumbering; current fd>=3 is not a grant
        // to inspect an original standard stream's metadata.
        if(resource->checkpoint_stdio_index>=0&&!call.env.permits(permission::stdio_metadata))
            return error::enotcapable;
#endif
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        bool const filesystem_operation=resource->checkpoint_stdio_index<0&&
            resource->socket_kind==::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_rc_t::socket_type::none;
        ::uwvm2::imported::wasi::wasip1::environment::budget_reservation reservation{
            ::uwvm2::imported::wasi::wasip1::environment::budget_kind::filesystem_io,
            call.env.limits,0u,filesystem_operation};
        if(filesystem_operation&&!reservation)return reservation.error<error>();
#endif
        using filetype=::uwvm2::imported::wasi::wasip1::abi::filetype_t;
        using flags_type=::uwvm2::imported::wasi::wasip1::abi::fdflags_t;
        filetype file_type{filetype::filetype_unknown};
        flags_type flags{};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        using socket_type=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_rc_t::socket_type;
        if(resource->socket_kind!=socket_type::none)
        {
            file_type=resource->socket_kind==socket_type::datagram?
                filetype::filetype_socket_dgram:filetype::filetype_socket_stream;
            if(resource->socket_nonblock)flags|=flags_type::fdflag_nonblock;
        }
        else
#endif
        {
            using kind=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e;
            auto const& storage=resource->wasi_fd_storage;
            int native{-1};
            bool directory{};
            switch(storage.type)
            {
                case kind::file:native=storage.storage.file_fd.native_handle();break;
                case kind::file_observer:native=storage.storage.file_observer.native_handle();break;
                case kind::dir:
                {
                    auto const& dirs=storage.storage.dir_stack;
                    if(dirs.empty()||dirs.dir_stack.back_unchecked().ptr==nullptr)
                    {static_cast<void>(call.native_failure());return error::eio;}
                    auto const& entry=dirs.dir_stack.back_unchecked().ptr->dir_stack;
                    native=entry.is_observer?entry.storage.observer.native_handle():entry.storage.file.native_handle();
                    directory=true;break;
                }
                default:static_cast<void>(call.native_failure());return error::eio;
            }
            if(call.stopped())return error::ecanceled;
            auto const native_flags=::fcntl(native,F_GETFL);
            if(native_flags<0)return native_error(errno);
#ifdef O_APPEND
            if((native_flags&O_APPEND)!=0)flags|=flags_type::fdflag_append;
#endif
#ifdef O_DSYNC
            if(O_DSYNC!=0&&(native_flags&O_DSYNC)==O_DSYNC)flags|=flags_type::fdflag_dsync;
#endif
#ifdef O_NONBLOCK
            if((native_flags&O_NONBLOCK)!=0)flags|=flags_type::fdflag_nonblock;
#endif
#ifdef O_RSYNC
            if(O_RSYNC!=0&&(native_flags&O_RSYNC)==O_RSYNC)flags|=flags_type::fdflag_rsync;
#endif
#ifdef O_SYNC
            if(O_SYNC!=0&&(native_flags&O_SYNC)==O_SYNC)flags|=flags_type::fdflag_sync;
#endif
            if(directory)file_type=filetype::filetype_directory;
            else
            {
                ::fast_io::posix_file_status observed{};
                try{observed=status(::fast_io::posix_io_observer{native});}
                catch(::fast_io::error const&){return error::eio;}
                switch(observed.type)
                {
                    case ::fast_io::file_type::regular:file_type=filetype::filetype_regular_file;break;
                    case ::fast_io::file_type::directory:file_type=filetype::filetype_directory;break;
                    case ::fast_io::file_type::symlink:file_type=filetype::filetype_symbolic_link;break;
                    case ::fast_io::file_type::block:file_type=filetype::filetype_block_device;break;
                    case ::fast_io::file_type::character:file_type=filetype::filetype_character_device;break;
                    case ::fast_io::file_type::socket:
                    {
                        int type{};::socklen_t extent{sizeof(type)};
                        if(::getsockopt(native,SOL_SOCKET,SO_TYPE,&type,&extent)!=0)return native_error(errno);
                        if(extent!=sizeof(type))return error::eio;
                        file_type=type==SOCK_DGRAM?filetype::filetype_socket_dgram:filetype::filetype_socket_stream;
                        break;
                    }
                    default:break;
                }
            }
        }
        if(call.stopped())return error::ecanceled;
        ::std::array<::std::byte,24> output{};
        little_put(output.data(),static_cast<::std::uint64_t>(file_type),1u);
        little_put(output.data()+2u,static_cast<::std::uint64_t>(flags),2u);
        little_put(output.data()+8u,static_cast<::std::uint64_t>(descriptor.rights_base),8u);
        little_put(output.data()+16u,static_cast<::std::uint64_t>(descriptor.rights_inherit),8u);
        auto const copied=call.copy_out(destination,output);
        return copied?error::esuccess:call.stopped()?error::ecanceled:error::efault;
    }
}
