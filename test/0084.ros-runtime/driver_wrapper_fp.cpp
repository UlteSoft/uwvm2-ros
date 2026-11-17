/** @file
 * @brief Exercise all five real driver wrappers around a hostile identity hook.
 * @details The hook changes rounding/flush/precision controls before refusing
 * identity. The wrapper must restore the caller's controls on this earliest
 * refusal path. Actual success, IPC faults and guest arithmetic are covered by
 * run_driver_attachment_vm.py --fp-controls. Sticky status is caller-saved.
 * Test-only identity/memory hooks never enter the VM product.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/imported/scene/host.h>
#include "../0008.imported/wasi/wasip1/func/fp_control_probe.h"
#include <cassert>
#include <cstdio>
namespace host=uwvm2::uwvm::imported::scene;
namespace driver=uwvm2::imported::ros::scene::drivers;
namespace probe=uwvm2test::wasip1_fp_control;
static unsigned hook_calls{};
namespace uwvm2::runtime::lib
{
    [[gnu::noinline]] bool ros_caller_identity_host_api(ros_caller_identity&,bool)noexcept
    {
        ++hook_calls;
        assert(std::fesetenv(FE_DFL_ENV)==0&&std::fesetround(FE_DOWNWARD)==0);
        probe::enable_flush_control();return false;
    }
    bool preload_memory_normalize_range_host_api(std::size_t,std::uint_least64_t,std::size_t,std::uint64_t&)noexcept
    {assert(false);return false;}
    bool preload_memory_read_host_api(std::size_t,std::uint_least64_t,void*,std::size_t)noexcept
    {assert(false);return false;}
    bool preload_memory_write_host_api(std::size_t,std::uint_least64_t,void const*,std::size_t)noexcept
    {assert(false);return false;}
}
struct controls
{
    int rounding{std::fegetround()};std::uint64_t arch{probe::read_arch_control()};std::uint16_t x87{};
    controls(){__asm__ volatile("fnstcw %0":"=m"(x87)::"memory");}
    bool operator==(controls const&)const=default;
};
template<driver::operation Action>static void exercise()
{
    using wrapper=host::wrapper::native<&host::invoke_driver<Action>,host::wrapper::driver_connect_name>;
    static_assert(wrapper::guards_fp_controls);
    typename wrapper::local_imported_function_type call{};
    auto before=controls{};auto count=hook_calls;wrapper::call(call);
    assert(hook_calls==count+1&&controls{}==before&&uwvm2::utils::container::get<0>(call.res)==13);
}
int main()
{
    probe::initial_state_restore restore;assert(restore.valid);
    unsigned profiles{};
    for(int rounding:{FE_TONEAREST,FE_DOWNWARD,FE_UPWARD,FE_TOWARDZERO})
        for(bool flush:{false,true})for(bool unmask:{false,true})for(unsigned precision:{0u,2u,3u})
    {
        assert(std::fesetenv(FE_DFL_ENV)==0&&std::fesetround(rounding)==0);
        if(flush)probe::enable_flush_control();auto state=controls{};
        auto x87=static_cast<std::uint16_t>((state.x87&~0x0300u)|(precision<<8));
        if(unmask)x87=static_cast<std::uint16_t>(x87&~4u);
        __asm__ volatile("fnclex\n\tfldcw %0"::"m"(x87):"memory");
        if(unmask)_mm_setcsr(_mm_getcsr()&~0x023fu);
        exercise<driver::operation::connect>();exercise<driver::operation::submit>();exercise<driver::operation::result>();
        exercise<driver::operation::describe>();exercise<driver::operation::disconnect>();++profiles;
    }
    assert(profiles==48&&hook_calls==240);
    std::puts("PASS five actual driver wrappers / 48 FP-control profiles / 240 hostile-hook refusals");
}
