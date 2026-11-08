// Licensed under the APL-2.0 License (see LICENSE file).
#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/utils/debug/host_calls.h>
#endif

#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::imported::wasi::wasip1::local_imported::observations
{
    namespace journal=::uwvm2::utils::debug::host_calls;
    // These are explicitly labelled ABI snapshots, not kernel-consumption
    // receipts. Shared-memory writers may change a buffer during an import.
    template<typename Environment>
    inline void capture(Environment& env,journal::record const& row,bool after,bool success) noexcept
    {
        if(!journal::active||(after&&!success)) {return;}
        auto name=std::string_view{row.name};
        bool const wide=name.ends_with("_wasm64");if(wide) {name.remove_suffix(7);}
        if(name=="fd_advise"||name=="fd_allocate"||name=="fd_close"||name=="fd_datasync"||
           name=="fd_fdstat_set_flags"||name=="fd_fdstat_set_rights"||name=="fd_filestat_set_size"||
           name=="fd_filestat_set_times"||name=="fd_renumber"||name=="fd_sync"||name=="proc_exit"||
           name=="proc_raise"||name=="sched_yield"||name=="sock_shutdown") {return;}
        auto mem=env.get_memory();
        if(!mem) {journal::missing_range();return;}
        [[maybe_unused]] auto guard=::uwvm2::imported::wasi::wasip1::memory::lock_memory(*mem);
        std::uint64_t const width=wide?8:4;
        auto const& a=row.arguments;
        auto valid=[&](std::uint64_t offset,std::uint64_t length) {
            return static_cast<bool>(::uwvm2::imported::wasi::wasip1::memory::checked_span_unlocked<false>(*mem,offset,length));
        };
        auto bytes=[&](std::uint64_t offset,std::uint64_t length,bool output) {
            // Observation cannot record a fault or change permission/quota denial.
            // Keep the original tagged address in the journal, while dereferencing
            // only the complete checked span under the existing relocation pin.
            auto const span=::uwvm2::imported::wasi::wasip1::memory::checked_span_unlocked<false>(*mem,offset,length);
            if(!span) {journal::missing_range();return;}
            journal::capture(0,offset,span.data,span.bytes,output,true);
        };
        auto integer=[&](std::uint64_t offset,std::uint64_t size,std::uint64_t& value) {
            std::array<std::byte,8> raw{};
            if(size>raw.size() || !::uwvm2::imported::wasi::wasip1::memory::copy_guest_bytes_unlocked<false,false>(
                *mem,offset,raw.data(),static_cast<std::size_t>(size)))
            {journal::missing_range();return false;}
            value=0;for(std::uint64_t i{};i<size;++i)
            {value|=static_cast<std::uint64_t>(std::to_integer<unsigned char>(raw[i]))<<(i*8);}
            return true;
        };
        bool const reading=name=="fd_read"||name=="fd_pread"||name=="sock_recv";
        bool const writing=name=="fd_write"||name=="fd_pwrite"||name=="sock_send";
        if(reading||writing)
        {
            auto const result_index=(name=="fd_pread"||name=="fd_pwrite"||name=="sock_recv"||name=="sock_send")?4u:3u;
            if(a[2]>UINT64_MAX/(2*width)||!valid(a[1],a[2]*(2*width))) {journal::missing_range();return;}
            std::uint64_t remaining=UINT64_MAX;
            if(after)
            {
                bytes(a[result_index],width,true);
                if(!integer(a[result_index],width,remaining)) {return;}
                if(name=="sock_recv") {bytes(a[5],2,true);}
            }
            else {bytes(a[1],a[2]*(2*width),false);}
            if((reading&&!after)||(writing&&after)) {return;}
            for(std::uint64_t i{};i<a[2]&&remaining;++i)
            {
                std::uint64_t offset{},size{};
                if(!integer(a[1]+i*2*width,width,offset)||!integer(a[1]+i*2*width+width,width,size)) {return;}
                auto amount=std::min(size,remaining);bytes(offset,amount,after);
                if(after) {remaining-=amount;}
            }
            if(after&&remaining) {journal::missing_range();}
            return;
        }
        if(!after)
        {
            if(name=="path_create_directory"||name=="path_remove_directory"||name=="path_unlink_file"||name=="path_readlink"||name=="path_rename")
            {bytes(a[1],a[2],false);if(name=="path_rename") {bytes(a[4],a[5],false);}}
            else if(name=="path_filestat_get"||name=="path_filestat_set_times"||name=="path_open")
            {bytes(a[2],a[3],false);}
            else if(name=="path_link") {bytes(a[2],a[3],false);bytes(a[5],a[6],false);}
            else if(name=="path_symlink") {bytes(a[0],a[1],false);bytes(a[3],a[4],false);}
            else if(name=="poll_oneoff")
            {if(a[2]>UINT64_MAX/48) {journal::missing_range();}else {bytes(a[0],a[2]*48,false);}}
            return;
        }
        if(name=="args_sizes_get"||name=="environ_sizes_get") {bytes(a[0],width,true);bytes(a[1],width,true);}
        else if(name=="args_get"||name=="environ_get")
        {
            auto const& values=name=="args_get"?env.argv:env.envs;
            if(values.size()>UINT64_MAX/width) {journal::missing_range();return;}
            bytes(a[0],values.size()*width,true);
            std::uint64_t size{};
            for(auto const& v:values)
            {if(v.size()==UINT64_MAX||v.size()+1>UINT64_MAX-size) {journal::missing_range();return;}size+=v.size()+1;}
            bytes(a[1],size,true);
        }
        else if(name=="clock_res_get") {bytes(a[1],8,true);}
        else if(name=="clock_time_get") {bytes(a[2],8,true);}
        else if(name=="fd_fdstat_get") {bytes(a[1],24,true);}
        else if(name=="fd_filestat_get") {bytes(a[1],64,true);}
        else if(name=="path_filestat_get") {bytes(a[4],64,true);}
        else if(name=="fd_prestat_get") {bytes(a[1],width*2,true);}
        else if(name=="fd_prestat_dir_name") {bytes(a[1],a[2],true);}
        else if(name=="fd_seek") {bytes(a[3],8,true);}
        else if(name=="fd_tell") {bytes(a[1],8,true);}
        else if(name=="path_open") {bytes(a[8],4,true);}
        else if(name=="sock_accept") {bytes(a[2],4,true);if constexpr(requires{env.wasix_socket_abi;}){if(!wide&&env.wasix_socket_abi)bytes(a[3],20,true);}}
        else if(name=="random_get") {bytes(a[0],a[1],true);}
        else if(name=="path_readlink"||name=="fd_readdir")
        {
            std::uint64_t count{};
            auto out=name=="path_readlink"?a[3]:a[1];
            auto capacity=name=="path_readlink"?a[4]:a[2];
            auto size_address=name=="path_readlink"?a[5]:a[4];
            bytes(size_address,width,true);
            if(integer(size_address,width,count)&&count<=capacity) {bytes(out,count,true);}
            else {journal::missing_range();}
        }
        else if(name=="poll_oneoff")
        {
            std::uint64_t count{};bytes(a[3],width,true);
            if(integer(a[3],width,count)&&count<=a[2]&&count<=UINT64_MAX/32) {bytes(a[1],count*32,true);}
            else {journal::missing_range();}
        }
    }
}
