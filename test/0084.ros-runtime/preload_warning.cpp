/** @file @brief Verify native warning promotion before real DSO constructors, including a second load. */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/cmdline/callback/log_disable_warning.h>
#include <uwvm2/uwvm/cmdline/callback/log_enable_warning.h>
#include <uwvm2/uwvm/cmdline/callback/log_convert_warn_to_fatal.h>
#include <uwvm2/uwvm/wasm/loader/dl.h>
#include <cassert>
#include <cstdio>
#include <string_view>
namespace p=uwvm2::uwvm::cmdline::params;
namespace cli=uwvm2::utils::cmdline;
static void setting(cli::parameter const& parameter,std::string_view category)
{
    std::array<cli::parameter_parsing_results,2> parsed{};
    parsed[0].para=&parameter;parsed[0].type=cli::parameter_parsing_results_type::parameter;
    parsed[1].str=uwvm2::utils::container::u8cstring_view{fast_io::containers::null_terminated,reinterpret_cast<char8_t const*>(category.data()),category.size()};
    parsed[1].type=cli::parameter_parsing_results_type::arg;
    assert(parameter.handle(parsed.data(),parsed.data(),parsed.data()+2)==cli::parameter_return_type::def);
}
int main(int argc,char** argv)
{
    assert(argc==4);std::string_view mode{argv[1]};
    if(mode=="suppressed"||mode=="suppressed-fatal"||mode=="reenabled")setting(p::log_disable_warning,"untrusted-dl");
    if(mode=="reenabled")setting(p::log_enable_warning,"untrusted-dl");
    if(mode=="fatal"||mode=="suppressed-fatal")setting(p::log_convert_warn_to_fatal,"untrusted-dl");
    if(mode=="all-fatal")setting(p::log_convert_warn_to_fatal,"all");
    auto load=[](uwvm2::uwvm::wasm::type::wasm_dl_t& loaded,char const* path)
    {
        auto result=uwvm2::uwvm::wasm::loader::load_dl(loaded,
            uwvm2::utils::container::u8cstring_view{fast_io::mnp::os_c_str(reinterpret_cast<char8_t const*>(path))},u8"warning-fixture",{});
        assert(result==uwvm2::uwvm::wasm::loader::load_dl_rtl::ok);
    };
    uwvm2::uwvm::wasm::type::wasm_dl_t first,second;load(first,argv[2]);
    if(mode=="second-fatal"||mode=="second-reenabled-fatal")
    {
        if(mode=="second-reenabled-fatal")setting(p::log_disable_warning,"untrusted-dl");
        setting(p::log_convert_warn_to_fatal,"untrusted-dl");
        if(mode=="second-reenabled-fatal")setting(p::log_enable_warning,"untrusted-dl");
    }
    load(second,argv[3]);std::puts("PASS real preloaded native constructors");
}
// The fixture has no guest memory. It supplies no fake VM or ROS implementation.
namespace uwvm2::uwvm::wasm::type
{
    extern "C" uwvm_preload_host_api_v1 const* uwvm_get_preload_host_api_v1() noexcept{return nullptr;}
    extern "C" uwvm_wasip1_host_api_v1 const* uwvm_get_wasip1_host_api_v1() noexcept{return nullptr;}
}
