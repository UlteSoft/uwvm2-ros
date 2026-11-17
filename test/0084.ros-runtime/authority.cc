#include <uwvm2/imported/ros/scene/authority.h>
#include <atomic>
#include <cassert>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

namespace s=uwvm2::imported::ros::scene;
using namespace std::chrono_literals;

s::grant grant(std::string alias="guard")
{
    s::grant result;result.alias=std::move(alias);result.resource="emergency-stop-notification";
    result.binding="local-runtime";result.kind=s::purpose::guard;
    result.actions={"guard.create","guard.trigger","guard.take","resource.observe","resource.close"};
    return result;
}
void expose(s::authority& authority)
{
    for(auto action:{"capability.open","capability.close","guard.create","guard.trigger","guard.take","resource.close"})
        assert(authority.expose(action,s::exposure::allow)==s::error::success);
}

void default_zero_and_exact_actions()
{
    int first{},other{};s::caller who{&first,1},foreign{&other,1};s::handle handle=999;
    s::authority denied;
    assert(denied.configure(grant())==s::error::success);
    assert(denied.enroll(who)==s::error::success);
    assert(denied.open(who,"guard",handle)==s::error::denied&&handle==999);
    assert(denied.expose("capability.open",s::exposure::allow)==s::error::invalid);
    s::authority a;expose(a);
    assert(a.configure(grant())==s::error::success);
    assert(a.configure(grant())==s::error::conflict);
    assert(a.expose("capability.open",s::exposure::allow)==s::error::conflict);
    assert(a.open(who,"guard",handle)==s::error::denied);
    assert(a.enroll(who)==s::error::success);
    assert(a.configure(grant("other"))==s::error::invalid);
    assert(a.open(foreign,"guard",handle)==s::error::denied);
    assert(a.open(who,"missing",handle)==s::error::denied);
    assert(a.open(who,"guard",handle)==s::error::success&&handle!=999);
    s::lease lease;
    assert(a.admit(who,handle,"guard.trigger",0,lease)==s::error::success&&lease);
    assert(lease.specification().alias=="guard");lease.release();
    assert(a.admit(foreign,handle,"guard.trigger",0,lease)==s::error::denied&&!lease);
    assert(a.admit({&first,2},handle,"guard.trigger",0,lease)==s::error::denied);
    assert(a.admit(who,handle,"device.submit",0,lease)==s::error::denied);
    assert(a.dependency(who,handle,"resource.observe",lease)==s::error::success);lease.release();
    assert(a.visible("resource.observe")==s::exposure::hidden); // Dependency is grant-only.
    assert(a.dependency(who,handle,"clock.update",lease)==s::error::denied);
    assert(a.close(who,handle)==s::error::success);
    assert(a.admit(who,handle,"guard.trigger",0,lease)==s::error::bad_handle);
}

void independent_environments_and_retirement()
{
    int instance{};s::caller who{&instance,1};s::authority a,b;expose(a);expose(b);
    assert(a.configure(grant())==s::error::success&&b.configure(grant())==s::error::success);
    assert(a.enroll(who)==s::error::success&&b.enroll(who)==s::error::success);
    s::handle first{},second{};assert(a.open(who,"guard",first)==s::error::success);
    assert(b.open(who,"guard",second)==s::error::success&&first!=second);
    s::lease lease;assert(a.admit(who,second,"guard.trigger",0,lease)==s::error::bad_handle);
    a.retire_member(who);
    assert(a.admit(who,first,"guard.trigger",0,lease)==s::error::denied);
    assert(a.enroll(who)==s::error::denied);
    s::caller successor{&instance,2};assert(a.enroll(successor)==s::error::success);
    assert(a.admit(successor,first,"guard.trigger",0,lease)==s::error::success);lease.release();
    // Shared environment handles are deliberately usable by explicitly enrolled peers.
    bool retired{};a.retire([&]{retired=true;});assert(retired);
    assert(a.open(successor,"guard",second)==s::error::denied);
    assert(a.enroll({&instance,3})==s::error::denied);
    assert(b.admit(who,second,"guard.trigger",0,lease)==s::error::success);
}

void finite_zero_and_atomic_reservations()
{
    int instance{};s::caller who{&instance,1};s::handle handle{};s::lease lease;
    auto policy=grant();policy.limits.calls=0;
    assert(!s::valid(policy));policy.limits.uncertain=true;assert(s::valid(policy));
    s::authority zero;expose(zero);assert(zero.configure(policy)==s::error::success);
    assert(zero.enroll(who)==s::error::success&&zero.open(who,"guard",handle)==s::error::success);
    assert(zero.admit(who,handle,"guard.trigger",0,lease)==s::error::again);
    assert(zero.admit(who,handle,"guard.trigger",0,lease,false)==s::error::success);lease.release();
    policy.limits.calls=1;policy.limits.bytes=4;policy.limits.window=1h;
    s::authority bounded;expose(bounded);assert(bounded.configure(policy)==s::error::success);
    assert(bounded.enroll(who)==s::error::success&&bounded.open(who,"guard",handle)==s::error::success);
    assert(bounded.admit(who,handle,"guard.trigger",5,lease)==s::error::again);
    assert(bounded.admit(who,handle,"guard.trigger",4,lease)==s::error::success);lease.release();
    assert(bounded.admit(who,handle,"guard.trigger",0,lease)==s::error::again);
    assert(bounded.close(who,handle)==s::error::success);
    assert(bounded.open(who,"guard",handle)==s::error::success);
    assert(bounded.admit(who,handle,"guard.trigger",0,lease)==s::error::again); // Reopen does not replenish budget.
    std::vector<s::authority_snapshot> rows;
    assert(bounded.inspect(rows,1)&&rows.size()==1&&rows[0].admissions==1&&rows[0].refused==3&&rows[0].attenuated);
    auto previous=rows;assert(!bounded.inspect(rows,0)&&rows[0].admissions==previous[0].admissions);
    policy=grant();policy.max_handles=0;s::authority capacity;expose(capacity);
    assert(capacity.configure(policy)==s::error::success&&capacity.enroll(who)==s::error::success);
    assert(capacity.open(who,"guard",handle)==s::error::again);
}

void semantic_separation()
{
    auto policy=grant();policy.kind=s::purpose::model_weights;assert(!s::valid(policy));
    policy.actions={"artifact.open","artifact.read"};assert(s::valid(policy));
    policy.actions.insert("artifact.append");assert(!s::valid(policy));
    policy.kind=s::purpose::language_model;policy.actions={"model.submit","model.take"};assert(s::valid(policy));
    policy.actions.insert("device.submit");assert(!s::valid(policy));
    policy.kind=static_cast<s::purpose>(255);policy.actions={"resource.close"};assert(!s::valid(policy));
    policy=grant();policy.binding="/etc/shadow";assert(!s::valid(policy));
    policy=grant();policy.resource="https://host/";assert(!s::valid(policy));
    policy=grant();policy.alias="..";assert(!s::valid(policy));
    policy=grant();policy.limits.window=0ns;assert(!s::valid(policy));
}

void lease_linearizes_revocation()
{
    int instance{};s::caller who{&instance,1};s::authority authority;expose(authority);
    assert(authority.configure(grant())==s::error::success&&authority.enroll(who)==s::error::success);
    s::handle handle{};assert(authority.open(who,"guard",handle)==s::error::success);
    s::lease admitted;assert(authority.admit(who,handle,"guard.trigger",0,admitted)==s::error::success);
    std::promise<void> started;auto entered=started.get_future();std::atomic<bool> completed{};
    auto revoke=std::async(std::launch::async,[&]
    {
        started.set_value();return authority.revoke("guard",[&](std::string_view alias){assert(alias=="guard");completed=true;});
    });
    entered.get();assert(!completed.load());admitted.release();
    assert(revoke.get()==s::error::success&&completed.load());
    assert(authority.admit(who,handle,"guard.trigger",0,admitted)==s::error::denied);
    assert(authority.dependency(who,handle,"resource.observe",admitted)==s::error::denied);
    assert(authority.open(who,"guard",handle)==s::error::denied);
}

void handle_accounting_survives_rollback()
{
    int instance{};s::caller who{&instance,1};s::authority authority;expose(authority);
    auto first=grant("first"),second=grant("second");first.max_handles=2;second.max_handles=1;
    assert(authority.configure(first)==s::error::success&&authority.configure(second)==s::error::success);
    assert(authority.enroll(who)==s::error::success);
    s::handle a{},b{},c{},refused=42;
    assert(authority.open(who,"first",a)==s::error::success&&authority.open(who,"first",b)==s::error::success);
    assert(authority.open(who,"second",c)==s::error::success);
    assert(authority.open(who,"first",refused)==s::error::again&&refused==42);
    std::vector<s::authority_snapshot> rows;
    assert(authority.inspect(rows,2)&&rows[0].open_handles==2&&rows[1].open_handles==1);
    authority.abandon_open(b);authority.abandon_open(b);
    assert(authority.inspect(rows,2)&&rows[0].open_handles==1&&rows[1].open_handles==1);
    assert(authority.close(who,a)==s::error::success&&authority.close(who,a)==s::error::bad_handle);
    assert(authority.open(who,"first",b)==s::error::success);
    assert(authority.revoke("first",[](auto){})==s::error::success);
    assert(authority.inspect(rows,2)&&rows[0].open_handles==1&&rows[0].revoked);
    assert(authority.close(who,b)==s::error::success);
    authority.retire([]{});
    assert(authority.inspect(rows,2)&&rows[0].open_handles==0&&rows[1].open_handles==0);
}

void compiled_actions_match_exact_policy()
{
    namespace contract=uwvm2::imported::ros::runtime_contract;
    for(auto const& selected:contract::operations)
        for(auto exposure:{s::exposure::hidden,s::exposure::deny,s::exposure::allow})
        {
            s::authority authority;
            assert(authority.expose("capability.open",s::exposure::allow)==s::error::success);
            for(auto const& item:contract::operations)
                if(exposure!=s::exposure::hidden)assert(authority.expose(std::string(item.name),exposure)==s::error::success);
            auto policy=grant();policy.actions={std::string(selected.grant)};
            if(selected.name.starts_with("clock."))policy.kind=s::purpose::ros_clock;
            else if(selected.name.starts_with("timer."))policy.kind=s::purpose::timer;
            else if(selected.name.starts_with("wait."))policy.kind=s::purpose::wait;
            assert(authority.configure(policy)==s::error::success);
            int instance{};s::caller who{&instance,1};s::handle handle{};
            assert(authority.enroll(who)==s::error::success&&authority.open(who,"guard",handle)==s::error::success);
            s::lease lease;
            for(auto const& candidate:contract::operations)
            {
                auto expected=authority.admit(who,handle,candidate.grant,0,lease);lease.release();
                assert(authority.admit(who,handle,candidate.id,0,lease)==expected);lease.release();
                assert(expected==((exposure==s::exposure::allow&&candidate.id==selected.id)?s::error::success:s::error::denied));
            }
            for(auto invalid:{16,31,32,63,127,255})
                assert(authority.admit(who,handle,static_cast<contract::operation>(invalid),0,lease)==s::error::unsupported&&!lease);
            if(exposure==s::exposure::allow)
            {
                assert(authority.admit(who,handle,selected.id,0,lease)==s::error::success);
                assert(authority.admit(who,handle,static_cast<contract::operation>(255),0,lease)==s::error::unsupported&&!lease);
                assert(authority.revoke("guard",[](auto){})==s::error::success);
                assert(authority.admit(who,handle,selected.id,0,lease)==s::error::denied&&!lease);
            }
        }
    s::authority typo;
    assert(typo.expose("capability.open",s::exposure::allow)==s::error::success);
    for(auto name:{"guard.create.extra","guard.create_v1","guard"})assert(typo.expose(name,s::exposure::allow)==s::error::success);
    assert(typo.configure(grant())==s::error::success);
    int instance{};s::caller who{&instance,1};s::handle handle{};s::lease lease;
    assert(typo.enroll(who)==s::error::success&&typo.open(who,"guard",handle)==s::error::success);
    assert(typo.admit(who,handle,contract::operation::guard_create,0,lease)==s::error::denied&&!lease);
}

template<typename Descriptors>
void artifact_indices_match(Descriptors const& descriptors,s::purpose purpose)
{
    using operation=decltype(descriptors[0].id);
    for(auto const& selected:descriptors)for(auto visibility:{s::exposure::hidden,s::exposure::deny,s::exposure::allow})
    {
        s::authority authority;assert(authority.expose("capability.open",s::exposure::allow)==s::error::success);
        for(auto const& item:descriptors)if(visibility!=s::exposure::hidden)
            assert(authority.expose(std::string(item.name),visibility)==s::error::success);
        auto policy=grant();policy.kind=purpose;policy.actions={std::string(selected.name)};
        assert(authority.configure(policy)==s::error::success);
        int instance{};s::caller who{&instance,1};s::handle handle{};s::lease lease;
        assert(authority.enroll(who)==s::error::success&&authority.open(who,"guard",handle)==s::error::success);
        for(auto const& candidate:descriptors)
        {
            auto expected=authority.admit(who,handle,candidate.name,0,lease,false);lease.release();
            assert(authority.admit(who,handle,candidate.id,0,lease,false)==expected);lease.release();
            assert(expected==((visibility==s::exposure::allow&&candidate.id==selected.id)?s::error::success:s::error::denied));
        }
        for(auto invalid:{31,32,63,127,255})
            assert(authority.admit(who,handle,static_cast<operation>(invalid),0,lease)==s::error::unsupported&&!lease);
        assert(authority.revoke("guard",[](auto){})==s::error::success);
        assert(authority.admit(who,handle,selected.id,0,lease)==s::error::denied&&!lease);
    }
}

int main()
{
    default_zero_and_exact_actions();independent_environments_and_retirement();
    finite_zero_and_atomic_reservations();semantic_separation();lease_linearizes_revocation();handle_accounting_survives_rollback();
    compiled_actions_match_exact_policy();
    artifact_indices_match(uwvm2::imported::ros::artifact_contract::operations,s::purpose::package);
    artifact_indices_match(uwvm2::imported::ros::output_contract::operations,s::purpose::cache);
    std::cout<<"PASS: zero authority, exact actions, environment isolation, generation retirement, atomic quotas, reopen accounting, model/device separation and revocation leases\n";
}
