/** @file @brief Cross-ABI child ownership, namespace and retirement checks. */
#include <uwvm2/imported/ros/scene/platform_extension_policy.h>
#include <cassert>
#include <cstdio>
namespace s=uwvm2::imported::ros::scene;
using op=s::robot_contract::operation;
int main()
{
    constexpr auto module="uwvm_ros_v2.platform_extension.fixture.v4294967295";
    constexpr std::array routes{s::platform_extensions::route{"open",op::device_open}};
    s::platform_extensions::policy children;
    assert(!s::platform_extensions::module_name("uwvm_ros_v2.platform_extension.fixture.v4294967296"));
    assert(!s::platform_extensions::module_name("uwvm_ros_v2.platform_extension.fixture.v18446744073709551616"));
    assert(children.register_module({module,routes})==s::error::success&&!children.enabled(module));
    s::grant grant;grant.alias="drive";grant.resource="approved";grant.kind=s::purpose::device;grant.binding="framed-scalar";
    grant.actions.emplace("device.open");grant.max_handles=1;
    assert(children.bind(module,grant)==s::error::success&&!children.allows("drive"));
    auto disabled=children;assert(disabled.seal()==s::error::success);
    op operation=op::control_stop;assert(disabled.resolve(module,"open","drive",operation)==s::error::denied&&operation==op::control_stop);
    assert(children.select(module,true)==s::error::success&&children.seal()==s::error::success);
    assert(children.resolve(module,"open","drive",operation)==s::error::success&&operation==op::device_open);
    assert(children.resolve(module,"open","different",operation)==s::error::denied);
    s::authority parent;int identity{};s::caller caller{&identity,1};s::handle capability{};
    assert(parent.open(caller,"drive",capability)==s::error::denied); // A child never enrolls a caller.
    assert(parent.expose("device.open",s::exposure::allow)==s::error::success&&parent.expose("capability.open",s::exposure::allow)==s::error::success&&parent.configure(grant)==s::error::success);
    assert(parent.enroll(caller)==s::error::success&&parent.open(caller,"drive",capability)==s::error::success);
    s::lease admitted;assert(parent.admit(caller,capability,"device.open",0,admitted,false)==s::error::success);admitted.release();
    assert(parent.revoke("drive",[](auto){})==s::error::success&&children.enabled(module));
    assert(parent.admit(caller,capability,"device.open",0,admitted,false)==s::error::denied);
    assert(children.select(module,true)==s::error::denied);
    s::platform_extensions::policy driver_children;
    assert(driver_children.register_module({module,routes})==s::error::success);
    auto driver_grant=grant;driver_grant.kind=s::purpose::driver;driver_grant.binding="registered-driver";driver_grant.actions={"driver.connect","driver.submit"};
    assert(driver_children.bind(module,driver_grant)==s::error::success&&!driver_children.allows("drive"));
    assert(driver_children.contract_matches("drive","device.open")&&!driver_children.contract_matches("drive","parameter.exists"));
    assert(driver_children.select(module,true)==s::error::success&&driver_children.seal()==s::error::success&&driver_children.allows("drive"));
    for(auto platform:s::platform_extensions::foundation_platforms)
    {
        uwvm2::utils::container::std_string name{s::platform_extensions::module_prefix};name+=platform;name+=".v1";
        s::platform_extensions::policy basic;
        assert(s::platform_extensions::foundation_module(name)&&!basic.enabled(name));
        assert(basic.bind(name,driver_grant)==s::error::success&&!basic.allows("drive"));
        assert(basic.contract_matches("drive","device.submit")&&!basic.contract_matches("drive","parameter.exists"));
        assert(basic.select(name,true)==s::error::success&&basic.seal()==s::error::success&&basic.allows("drive"));
        assert(basic.resolve(name,"device.submit","drive",operation)==s::error::success&&operation==op::device_submit);
        // Resolving a policy route cannot restore the revoked parent grant.
        assert(parent.admit(caller,capability,"device.open",0,admitted,false)==s::error::denied);
        s::platform_extensions::policy disabled;
        assert(disabled.select(name,false)==s::error::success&&disabled.bind(name,driver_grant)==s::error::success);
        assert(disabled.seal()==s::error::success&&!disabled.allows("drive"));
        s::platform_extensions::policy unbound;
        assert(unbound.select(name,true)==s::error::success&&unbound.seal()==s::error::invalid);
    }
    assert(!s::platform_extensions::foundation_module("uwvm_ros_v2.platform_extension.unitree.v01"));
    assert(!s::platform_extensions::foundation_module("uwvm_ros_v2.platform_extension.unitree.v2"));
    std::puts("PASS cross-ABI child ownership and live parent revocation");
}
