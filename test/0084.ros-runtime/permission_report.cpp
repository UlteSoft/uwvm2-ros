/** @brief Read-only observation, deny precedence, pagination and zero overwrite. */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <uwvm2/imported/ros/scene/erased_buffer.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <thread>
namespace s=uwvm2::imported::ros::scene;
namespace p=s::permission_report;
using E=s::error;
static auto snapshot(s::deployment const& d,std::uint64_t cursor=0)
{
    std::array<std::uint8_t,p::maximum_response> storage{};
    p::page out{storage,cursor};p::query query{p::family::rosv2,{},cursor};
    assert(d.report_self("main",query,out)==E::success);
    return uwvm2::utils::container::std_vector<std::uint8_t>(out.bytes().begin(),out.bytes().end());
}
int main()
{
    s::deployment d;assert(d.create("robot")==E::success);assert(d.bind("robot","main")==E::success);
    p::query query{p::family::rosv2,{},0};
    assert(d.visible("main",p::import_name)==s::exposure::allow);
    assert(d.visible("unknown",p::import_name)==s::exposure::hidden);
    assert(d.visible("main","guard.create")==s::exposure::hidden);
    auto before=snapshot(d);
    for(unsigned i{};i<100;++i){assert(d.report_allowed("main",query)==E::success);assert(snapshot(d)==before);}
    // Reports did not freeze policy, create a live broker or enroll an epoch.
    assert(d.bind("robot","attached")==E::success);assert(snapshot(d)==before); // no other member disclosure
    s::grant grant;grant.alias="clock";grant.kind=s::purpose::steady_clock;grant.resource="clock";grant.binding="local";
    grant.actions={"clock.create","clock.read"};grant.limits.calls=2;grant.limits.uncertain=true;
    assert(d.allow("robot",grant)==E::success);
    assert(d.expose("robot","capability.open",s::exposure::allow)==E::success);
    assert(d.expose("robot","clock.create",s::exposure::allow)==E::success);
    assert(d.expose("robot","clock.read",s::exposure::allow)==E::success);
    int instance{};std::shared_ptr<s::broker> broker;assert(d.select("main",{&instance,7},broker)==E::success);
    s::handle cap{};assert(broker->permissions.open({&instance,7},"clock",cap)==E::success);
    auto frozen=snapshot(d);
    for(unsigned i{};i<100;++i)assert(snapshot(d)==frozen);
    // Observation never reserves from the two-call execution budget.
    s::lease held;
    assert(broker->permissions.admit({&instance,7},cap,"clock.read",0,held)==E::success);held.release();
    assert(broker->permissions.admit({&instance,7},cap,"clock.read",0,held)==E::success);held.release();
    assert(broker->permissions.admit({&instance,7},cap,"clock.read",0,held)==E::again);
    assert(d.platform_setting("robot","permission.report","wasip1.fd_write","deny")==E::success);
    assert(d.report_allowed("main",{p::family::wasip1,{},0})==E::denied);
    assert(d.report_allowed("main",{p::family::wasip1,"fd_write",0})==E::denied);
    assert(d.report_allowed("main",{p::family::wasip1,"fd_read",0})==E::success);
    assert(d.platform_setting("robot","permission.report","preloaddl","deny")==E::success);
    assert(d.report_allowed("main",{p::family::preloaddl,{},0})==E::denied);
    assert(d.platform_setting("robot","permission.report","wasip1.fd_write","allow")==E::invalid);
    bool called{};
    assert(d.deliver_report("main",{p::family::preloaddl,{},0},[&]{called=true;return true;})==E::denied&&!called);
    s::deployment hidden;assert(hidden.create("off")==E::success);assert(hidden.bind("off","main")==E::success);
    assert(hidden.expose("off",uwvm2::utils::container::std_string(p::import_name),s::exposure::hidden)==E::success);
    assert(hidden.visible("main",p::import_name)==s::exposure::hidden&&hidden.report_allowed("main",query)==E::denied);
    std::array<std::uint8_t,8192> storage{};p::page page{storage,31};
    for(unsigned i{};i<66;++i)assert(page.add("scope","key",i));
    assert(page.finish(true,false));
    s::wire::reader read{page.bytes()};std::uint64_t supported{},enabled{},first{},total{},count{};
    assert(read.u64(supported)&&read.u64(enabled)&&read.u64(first)&&read.u64(total)&&read.u64(count));
    assert(supported==1&&!enabled&&first==31&&total==66&&count==32);
    for(auto size:{0u,16u,512u,513u,65536u})
    {
        s::erased_buffer sensitive;sensitive.resize_for_overwrite(size);
        std::memset(sensitive.writable().data(),0xa5,size);auto* retained=sensitive.writable().data();
        sensitive.erase();assert(sensitive.writable().data()==retained&&sensitive.view().size()==size);
        for(auto byte:sensitive.view())assert(byte==0);
    }
    // Foreign observation requires both exact exposure and an exact target grant.
    s::deployment observed;assert(observed.create("viewer")==E::success);
    assert(observed.bind("viewer","main")==E::success);
    assert(observed.create("worker-group")==E::success);
    assert(observed.bind("worker-group","worker")==E::success);
    assert(observed.bind("worker-group","peer")==E::success);
    bool seen{};
    auto inspect=[&](std::string_view target){seen=true;assert(target=="worker");return E::success;};
    assert(observed.observe_module("main","camera","module.describe",inspect)==E::denied&&!seen);
    assert(observed.expose("viewer","module.describe",s::exposure::allow)==E::success);
    assert(observed.observe_module("main","camera","module.describe",inspect)==E::denied&&!seen);
    s::grant inspect_grant;inspect_grant.alias="camera";inspect_grant.resource="worker";
    inspect_grant.kind=s::purpose::module_inspection;inspect_grant.binding="metadata";
    inspect_grant.actions={"module.describe","module.group","module.permission.report","module.importers"};
    assert(observed.allow("viewer",inspect_grant)==E::success);
    assert(observed.observe_module("main","camera","module.describe",inspect)==E::success&&seen);
    assert(observed.observe_module("main","worker","module.describe",inspect)==E::denied);
    assert(observed.expose("viewer","module.group",s::exposure::allow)==E::success);
    p::page members{storage,0};
    assert(observed.observe_module("main","camera","module.group",[&](auto target)
        {return observed.report_module_group_locked(target,members);})==E::success);
    s::wire::reader membership{members.bytes()};
    assert(membership.u64(supported)&&membership.u64(enabled)&&membership.u64(first)&&membership.u64(total)&&membership.u64(count));
    assert(total==3&&count==3); // group plus exactly its two members
    assert(observed.expose("viewer","module.permission.report",s::exposure::allow)==E::success);
    assert(observed.platform_setting("viewer","permission.report","rosv2.clock.read","deny")==E::success);
    p::query denied_query{p::family::rosv2,"clock.read",0};seen=false;
    assert(observed.observe_module("main","camera","module.permission.report",inspect,&denied_query)==E::denied&&!seen);
    assert(observed.bind("viewer","still-unfrozen")==E::success);
    // Reverse dependencies reveal peers and therefore require their own action,
    // even for a grant targeting the immediate caller. Forward imports do not
    // imply reverse-import or VM observation authority.
    seen=false;
    assert(observed.observe_module("main","camera","module.importers",inspect)==E::denied&&!seen);
    assert(observed.expose("viewer","module.importers",s::exposure::allow)==E::success);
    assert(observed.observe_module("main","camera","module.importers",inspect)==E::success&&seen);
    assert(!s::permits(s::purpose::module_inspection,"vm.describe"));
    assert(!s::permits(s::purpose::vm_inspection,"module.importers"));
    assert(!s::permits(s::purpose::vm_inspection,"resource.close"));
    assert(!s::permits(s::purpose::vm_inspection,"resource.observe"));
    auto vm_query=[&](std::string_view target){seen=true;assert(target=="runtime");return E::success;};
    seen=false;
    assert(observed.visible("main","vm.describe")==s::exposure::hidden);
    assert(observed.observe_vm("main","vm-view",vm_query)==E::denied&&!seen);
    assert(observed.expose("viewer","vm.describe",s::exposure::allow)==E::success);
    assert(observed.observe_vm("main","camera",vm_query)==E::denied&&!seen);
    s::grant vm_grant;vm_grant.alias="vm-view";vm_grant.kind=s::purpose::vm_inspection;
    vm_grant.resource="runtime";vm_grant.binding="metadata";vm_grant.actions={"vm.describe"};
    auto invalid_vm=vm_grant;invalid_vm.resource="other-runtime";
    assert(observed.allow("viewer",invalid_vm)==E::unsupported);
    invalid_vm=vm_grant;invalid_vm.limits.calls=0;invalid_vm.limits.uncertain=true;
    assert(observed.allow("viewer",invalid_vm)==E::unsupported); // read-only observation never ignores a configured budget
    assert(observed.allow("viewer",vm_grant)==E::success);
    assert(observed.observe_vm("main","vm-view",vm_query)==E::success&&seen);
    assert(observed.observe_module("main","vm-view","vm.describe",inspect)==E::denied);
    assert(observed.bind("viewer","queries-did-not-freeze")==E::success);
    std::shared_ptr<s::broker> observed_broker;
    assert(observed.select("main",{&instance,8},observed_broker)==E::success);
    assert(observed.observe_vm("main","vm-view",vm_query)==E::success);
    assert(observed_broker->permissions.revoke("vm-view",[](auto){})==E::success);seen=false;
    assert(observed.observe_vm("main","vm-view",vm_query)==E::denied&&!seen);
    assert(observed.observe_module("main","camera","module.importers",inspect)==E::success);
    assert(observed_broker->permissions.revoke("camera",[](auto){})==E::success);seen=false;
    assert(observed.observe_module("main","camera","module.importers",inspect)==E::denied&&!seen);
    std::puts("PASS permission reports: no freeze/enroll/quota debit, self scope, reverse-import/VM grant separation, live revocation, deny precedence, pagination, retained zero overwrite");
}
