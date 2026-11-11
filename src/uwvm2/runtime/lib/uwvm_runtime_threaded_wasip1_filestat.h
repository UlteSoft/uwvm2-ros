// Private shared-memory filestat leaf. The live descriptor and ChildLease
// remain owned until native observation and checked atomic Guest output finish.
extern "C++"
{
    inline auto runtime_threaded_wasip1_adapter::filestat_get(context& call,::std::uint32_t guest,
        ::std::uint64_t destination) noexcept -> error
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        if(guest<3u&&(!call.env.grants_stdio(guest)||!call.env.permits(permission::stdio_metadata)))
            return error::enotcapable;
#endif
        if(!call.bounds(destination,64u))return error::efault;
        if((destination&7u)!=0u)return error::einval;
        fd_pin pinned{};
        auto const acquired=pinned.acquire(call,guest,right::right_fd_filestat_get,true,false,false,true);
        if(acquired.failure!=error::esuccess)return acquired.failure;
        if(acquired.deferred||pinned.locked==nullptr)
        {static_cast<void>(call.native_failure());return error::eio;}
        auto const* resource=pinned.locked->wasi_fd.ptr;
        if(resource==nullptr){static_cast<void>(call.native_failure());return error::eio;}
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        if(resource->checkpoint_stdio_index>=0&&!call.env.permits(permission::stdio_metadata))
            return error::enotcapable;
        bool const filesystem_operation=resource->checkpoint_stdio_index<0;
        ::uwvm2::imported::wasi::wasip1::environment::budget_reservation reservation{
            ::uwvm2::imported::wasi::wasip1::environment::budget_kind::filesystem_io,
            call.env.limits,0u,filesystem_operation};
        if(filesystem_operation&&!reservation)return reservation.error<error>();
#endif
        using kind=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e;
        auto const& storage=resource->wasi_fd_storage;
        int native{-1};
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
                break;
            }
            default:static_cast<void>(call.native_failure());return error::eio;
        }
        if(call.stopped())return error::ecanceled;
        ::fast_io::posix_file_status observed{};
        try{observed=status(::fast_io::posix_io_observer{native});}
        catch(::fast_io::error const&){return error::eio;}
        using filetype=::uwvm2::imported::wasi::wasip1::abi::filetype_t;
        filetype type{filetype::filetype_unknown};
        switch(observed.type)
        {
            case ::fast_io::file_type::regular:type=filetype::filetype_regular_file;break;
            case ::fast_io::file_type::directory:type=filetype::filetype_directory;break;
            case ::fast_io::file_type::symlink:type=filetype::filetype_symbolic_link;break;
            case ::fast_io::file_type::block:type=filetype::filetype_block_device;break;
            case ::fast_io::file_type::character:type=filetype::filetype_character_device;break;
            case ::fast_io::file_type::socket:
            {
                int socket_type{};::socklen_t extent{sizeof(socket_type)};
                if(::getsockopt(native,SOL_SOCKET,SO_TYPE,&socket_type,&extent)!=0)return native_error(errno);
                if(extent!=sizeof(socket_type))return error::eio;
                type=socket_type==SOCK_DGRAM?filetype::filetype_socket_dgram:filetype::filetype_socket_stream;
                break;
            }
            default:break;
        }
        if(call.stopped())return error::ecanceled;
        // Encode defined ABI fields, never the C++ object's padding. The
        // conversion uses unsigned arithmetic, including pre-epoch timestamps.
        auto const nanoseconds=[](::fast_io::unix_timestamp stamp) noexcept
        {
            constexpr auto factor=::fast_io::uint_least64_subseconds_per_second/1'000'000'000u;
            return static_cast<::std::uint64_t>(stamp.seconds)*1'000'000'000u+
                static_cast<::std::uint64_t>(stamp.subseconds/factor);
        };
        ::std::array<::std::byte,64> output{};
        little_put(output.data(),static_cast<::std::uint64_t>(observed.dev),8u);
        little_put(output.data()+8u,static_cast<::std::uint64_t>(observed.ino),8u);
        little_put(output.data()+16u,static_cast<::std::uint64_t>(type),1u);
        little_put(output.data()+24u,static_cast<::std::uint64_t>(observed.nlink),8u);
        little_put(output.data()+32u,static_cast<::std::uint64_t>(observed.size),8u);
        little_put(output.data()+40u,nanoseconds(observed.atim),8u);
        little_put(output.data()+48u,nanoseconds(observed.mtim),8u);
        little_put(output.data()+56u,nanoseconds(observed.ctim),8u);
        auto const copied=call.copy_out(destination,output);
        return copied?error::esuccess:call.stopped()?error::ecanceled:error::efault;
    }
}
