// Read-only launch inventory. No symbols, addresses, driver callbacks or file
// reads are resolved here. Native code stays outside the Wasm debugger.
#pragma once
#include <uwvm2/utils/container/standard.h>
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/uwvm/wasm/storage/preloaded_dl.h>
# include <fast_io.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::debugger::host_interfaces
{
    [[nodiscard]] inline ::uwvm2::utils::container::std_string inventory(::std::uint64_t first)
    {
        ::uwvm2::utils::container::std_string text{};
        ::uwvm2::utils::container::std_string rows{};
        auto append=[&](auto const&... parts){text+=::uwvm2::utils::container::concat(parts...);};
        ::std::size_t total{};
#if defined(UWVM_SUPPORT_PRELOAD_DL)
        total=::uwvm2::uwvm::wasm::storage::preloaded_dl.size();
#endif
        auto const start{first>total?total:static_cast<::std::size_t>(first)};
        ::std::size_t count{};
#if defined(UWVM_SUPPORT_PRELOAD_DL)
        // Leave room for the header in the 64 KiB authenticated transport.
        // A page may contain fewer than 32 long entries; its next offset
        // still traverses the complete immutable inventory.
        constexpr ::std::size_t row_budget{60000u};
        auto encoded=[](auto& output,auto bytes){
            constexpr char digits[]{"0123456789abcdef"};
            auto const size{bytes.size()>4096u?4096u:bytes.size()};
            if(size==0u){output+='-';return;}
            for(::std::size_t i{};i<size;++i){auto const byte{static_cast<unsigned char>(bytes[i])};output+=digits[byte>>4u];output+=digits[byte&15u];}
        };
        while(count<32u&&count<total-start)
        {
            auto const i{start+count};
            auto const& library{::uwvm2::uwvm::wasm::storage::preloaded_dl[i]};
            auto row{::uwvm2::utils::container::concat("library index=",i," functions=",library.wasm_dl_storage.capi_function_vec.function_size,
                " signature-verified=",library.verified_image.descriptor()>=0?1u:0u," name-hex=")};
            encoded(row,library.module_name);
            row+=::uwvm2::utils::container::concat(" name-truncated=",library.module_name.size()>4096u?1u:0u," path-hex=");
            encoded(row,library.file_name);
            row+=::uwvm2::utils::container::concat(" path-truncated=",library.file_name.size()>4096u?1u:0u,"\n");
            if(row.size()>row_budget-rows.size())break;
            rows+=row;++count;
        }
#endif
        append("host-interfaces schema=1 first=",first," total=",total," count=",count," more=",start+count<total?1u:0u,"\n");
        append("interface u2rosv1-ros1 built=",
#if defined(UWVM_IMPORT_ROS1)
            1u
#else
            0u
#endif
            ,"\ninterface u2rosv1-ros2 built=",
#if defined(UWVM_IMPORT_ROS2)
            1u
#else
            0u
#endif
            ,"\ninterface u2rosv2 built=",
#if defined(UWVM_IMPORT_ROSV2)
            1u
#else
            0u
#endif
            ,"\ninterface preloaddl built=",
#if defined(UWVM_SUPPORT_PRELOAD_DL)
            1u
#else
            0u
#endif
            ,"\n");
        text+=rows;
        return text;
    }
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
