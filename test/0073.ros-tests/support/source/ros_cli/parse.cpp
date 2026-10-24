#include <uwvm2/uwvm/cmdline/impl.h>
#include <uwvm2/uwvm/cmdline/callback/impl.h>
int main(int argc, char** argv)
{
    auto result = ::uwvm2::uwvm::cmdline::parsing(static_cast<std::size_t>(argc), reinterpret_cast<char8_t const* const*>(argv));
    return result == ::uwvm2::uwvm::cmdline::parsing_return_val::returnm1 ? 1 : 0;
}

// CLI fixture intentionally has no host memory provider. Loading a native
// extension gets a null table; no runtime or ROS result is simulated.
namespace uwvm2::uwvm::wasm::type
{
    extern "C" uwvm_preload_host_api_v1 const* uwvm_get_preload_host_api_v1() noexcept { return nullptr; }
}
