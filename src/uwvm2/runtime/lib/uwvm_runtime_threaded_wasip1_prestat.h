// Included after the actual threaded WASIp1 FD pin implementation.
extern "C++"
{
    inline auto runtime_threaded_wasip1_adapter::prestat(context& call,::std::uint32_t guest,
        ::std::uint32_t destination,::std::uint32_t capacity,bool sizes) noexcept -> error
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        // Interface denial is handled before this leaf. Preserve the ordinary
        // leaf's stdio grant check before examining any Guest memory.
        if(guest<3u&&!call.env.grants_stdio(guest))return error::enotcapable;
#endif
        if(!call.bounds(destination,capacity))return error::efault;
        // Preview1 prestat is eight bytes, aligned to four, independently of
        // the more strictly aligned host staging structure.
        if(sizes&&(destination&3u)!=0u)return error::einval;
        fd_pin pinned{};
        auto const acquired=pinned.acquire(call,guest,right{},true,false,false,true);
        if(acquired.failure!=error::esuccess)return acquired.failure;
        if(acquired.deferred||pinned.locked==nullptr)
        {static_cast<void>(call.native_failure());return error::eio;}
        auto const* resource=pinned.locked->wasi_fd.ptr;
        if(resource==nullptr){static_cast<void>(call.native_failure());return error::eio;}
        using type=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e;
        if(resource->wasi_fd_storage.type!=type::dir)return error::enotdir;
        auto const& directories=resource->wasi_fd_storage.storage.dir_stack;
        if(directories.empty()){static_cast<void>(call.native_failure());return error::eio;}
        if(!directories.is_preload_dir())return error::enotdir;
        auto const* directory=directories.dir_stack.front_unchecked().ptr;
        if(directory==nullptr){static_cast<void>(call.native_failure());return error::eio;}
        auto const& name=directory->dir_stack.name;
        if(name.size()>UINT32_MAX)return error::eoverflow;
        if(call.stopped())return error::ecanceled;
        bool copied{};
        if(sizes)
        {
            ::std::array<::std::byte,8> output{};
            // Tag zero denotes a directory; every ABI padding byte is zero.
            little_put(output.data()+4u,name.size(),4u);
            copied=call.copy_out(destination,output);
        }
        else
        {
            if(capacity<name.size())return error::enametoolong;
            // No trailing NUL and no invented path-length bound. The actual
            // preopen name stays immutable under the retained descriptor lock.
            copied=call.copy_out(destination,{reinterpret_cast<::std::byte const*>(name.data()),name.size()});
        }
        return copied?error::esuccess:call.stopped()?error::ecanceled:error::efault;
    }
}
