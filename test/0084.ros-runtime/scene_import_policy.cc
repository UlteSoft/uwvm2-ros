/** @file @brief Compare detached debugger rows with the runtime's actual interface visibility. */
#include <cassert>
#include <iostream>
#include <uwvm2/imported/ros/scene/deployment.h>
#include <uwvm2/uwvm/debugger/scene_state.h>

namespace model=uwvm2::imported::ros::scene;
namespace console=uwvm2::uwvm::debugger::scene_state;

int main()
{
    model::deployment deploy;
    assert(deploy.create("robot")==model::error::success);
    assert(deploy.create("other")==model::error::success);
    assert(deploy.bind("robot","main")==model::error::success);
    assert(deploy.bind("other","peer")==model::error::success);
    assert(deploy.expose("robot","guard.create",model::exposure::deny)==model::error::success);
    assert(deploy.expose("robot","guard.trigger",model::exposure::allow)==model::error::success);
    assert(deploy.expose("robot","guard.describe",model::exposure::hidden)==model::error::invalid);
    assert(deploy.expose("robot","abi",model::exposure::hidden)==model::error::success);
    assert(deploy.expose("other","permission.report",model::exposure::hidden)==model::error::success);
    assert(deploy.visible("unbound","permission.report")==model::exposure::hidden);
    console::request query;
    assert(console::parse("info scene policy environment=robot count=8 if-stop=9",query));
    console::view page;
    std::size_t seen{};
    std::string previous;
    do
    {
        query.first=seen;deploy.inspect(query,page);
        assert(page.result==console::status::ok&&page.total==model::callable_names.size());
        for(std::size_t i{};i<page.size;++i)
        {
            auto const& row=page.policy[i];
            auto operation=row.operation.view();
            assert(previous<operation&&row.environment.view()=="robot");
            previous=operation;
            auto live=deploy.visible("main",operation);
            assert(row.effective==(live==model::exposure::allow?"allow":live==model::exposure::deny?"deny":"hidden"));
            if(operation=="guard.create"||operation=="guard.trigger"||operation=="abi")
                assert(row.origin=="explicit"&&row.configured==row.effective);
            else if(operation=="permission.report")assert(row.origin=="self-default"&&row.effective=="allow");
            else assert(row.origin=="default"&&row.effective=="hidden"&&row.configured=="unset");
            ++seen;
        }
        assert(page.next==seen);
    }while(page.more);
    assert(seen==model::callable_names.size());
    query.first=0;query.environment={};query.operation.assign("permission.report");
    deploy.inspect(query,page);
    assert(page.size==2&&page.policy[0].environment.view()=="other"&&page.policy[0].origin=="explicit"&&
        page.policy[0].effective=="hidden"&&page.policy[1].environment.view()=="robot"&&page.policy[1].effective=="allow");
    std::string wire;auto output=fast_io::ostring_ref_std{&wire};console::print(output,page,9);std::cout<<wire;
    query.operation.assign("unregistered");deploy.inspect(query,page);
    assert(page.result==console::status::invalid_request&&page.total==0&&page.size==0&&!page.more);
    query.operation.assign("guard.trigger");query.environment.assign("robot");deploy.retire();deploy.inspect(query,page);
    assert(page.size==1&&page.policy[0].origin=="retired"&&page.policy[0].configured=="allow"&&page.policy[0].effective=="hidden");
    assert(deploy.visible("main","guard.trigger")==model::exposure::hidden);
    assert(!console::parse("info scene policy alias=resource",query));
    assert(!console::parse("info scene authority operation=abi",query));
    assert(!console::parse("info scene policy operation=abi operation=abi",query));
}
