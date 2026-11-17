/** @file
 * @brief Distinguish operation work from already-granted resource dependency checks.
 */
#include <uwvm2/imported/ros/scene/authority.h>
#include <cassert>
#include <iostream>
namespace s=uwvm2::imported::ros::scene;
using s::error;
static s::grant grant(std::string_view alias,s::purpose purpose,std::initializer_list<std::string_view> actions)
{
    s::grant value;value.alias=alias;value.resource=alias;value.binding="test";value.kind=purpose;
    for(auto a:actions)value.actions.emplace(a);
    return value;
}
struct fixture
{
    s::authority authority;int instance{};s::caller who{&instance,1};s::handle node{},topic{};
    fixture(bool expose_node=true,std::uint64_t topic_calls=10)
    {
        auto n=grant("node",s::purpose::node,{"node.open","node.describe","resource.observe"});n.limits.calls=1;n.limits.uncertain=true;n.limits.window=std::chrono::hours{1};
        auto t=grant("topic",s::purpose::topic,{"topic.publish","topic.open","qos.describe","data.export"});t.limits.calls=topic_calls;t.limits.uncertain=true;t.limits.window=std::chrono::hours{1};
        assert(authority.expose("capability.open",s::exposure::allow)==error::success);
        for(auto name:{"topic.open","topic.publish","qos.describe"})assert(authority.expose(name,s::exposure::allow)==error::success);
        if(expose_node)assert(authority.expose("node.open",s::exposure::allow)==error::success);
        assert(authority.configure(n)==error::success&&authority.configure(t)==error::success&&authority.enroll(who)==error::success);
        assert(authority.open(who,"node",node)==error::success&&authority.open(who,"topic",topic)==error::success);
    }
    std::uint64_t admitted(std::string_view name)
    {uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(authority.inspect(rows,2));for(auto const& row:rows)if(row.alias==name)return row.admissions;assert(false);return 0;}
};
static void retained_node_is_not_created_again()
{
    fixture f;s::lease created;assert(f.authority.admit(f.who,f.node,"node.open",8,created)==error::success);created.release();
    std::array claims{s::admission_claim{f.topic,"topic.publish",32},s::admission_claim{f.node,"node.open",0,false,true}};
    s::group_lease group;
    for(unsigned i=0;i<5;++i){assert(f.authority.admit_group(f.who,claims,group)==error::success&&group.size()==2);group.release();}
    assert(f.admitted("node")==1&&f.admitted("topic")==5);
    claims[1].validation_only=false;assert(f.authority.admit_group(f.who,claims,group)==error::again&&!group);
    assert(f.admitted("node")==1&&f.admitted("topic")==5); // no partial primary charge
    claims[1].validation_only=true;claims[1].bytes=1;assert(f.authority.admit_group(f.who,claims,group)==error::invalid);
    claims[1].bytes=0;claims[0].validation_only=true;claims[0].bytes=0;
    assert(f.authority.admit_group(f.who,claims,group)==error::invalid);
    assert(f.authority.admit_group(f.who,claims,group,false)==error::success);group.release();
    assert(f.admitted("node")==1&&f.admitted("topic")==5);
}
static void dependencies_still_need_authority_and_exposure()
{
    fixture f(false);std::array claims{s::admission_claim{f.topic,"topic.publish",32},s::admission_claim{f.node,"node.open",0,false,true}};
    s::group_lease group;assert(f.authority.admit_group(f.who,claims,group)==error::denied&&!f.admitted("topic"));
    claims[1].grant_only=true;assert(f.authority.admit_group(f.who,claims,group)==error::denied); // cannot relabel callable imports
    claims[1].action="resource.observe";assert(f.authority.admit_group(f.who,claims,group)==error::success);group.release();
    assert(f.authority.visible("resource.observe")==s::exposure::hidden&&f.admitted("node")==0&&f.admitted("topic")==1);
    assert(f.authority.revoke("node",[](auto){})==error::success);
    assert(f.authority.admit_group(f.who,claims,group)==error::denied&&f.admitted("topic")==1);
}
static void grant_only_does_not_bypass_scope_or_attenuation()
{
    fixture f(true,0);s::group_lease group;
    std::array claims{s::admission_claim{f.topic,"topic.publish",4},s::admission_claim{f.node,"resource.observe",0,true,true}};
    assert(f.authority.admit_group(f.who,claims,group)==error::again);
    assert(f.authority.admit_group(f.who,claims,group,false)==error::success);group.release();
    claims[1].action="data.export";assert(f.authority.admit_group(f.who,claims,group,false)==error::denied);
    claims[1]={f.topic,"data.export",0,true,true};assert(f.authority.admit_group(f.who,claims,group,false)==error::success);group.release();
    assert(f.authority.visible("data.export")==s::exposure::hidden);
    claims[1].grant_only=false;assert(f.authority.admit_group(f.who,claims,group,false)==error::denied);
    int stranger{};assert(f.authority.admit_group({&stranger,1},claims,group,false)==error::denied);
    assert(!f.admitted("node")&&!f.admitted("topic"));
}
static void semantic_catalogue_is_exact()
{
    assert(s::purpose_names[static_cast<std::size_t>(s::purpose::transform)]=="transform");
    assert(s::permits(s::purpose::context,"context.open")&&!s::permits(s::purpose::node,"context.open"));
    assert(s::permits(s::purpose::type,"type.describe")&&!s::permits(s::purpose::context,"type.describe"));
    assert(s::permits(s::purpose::event,"event.stream.poll")&&!s::permits(s::purpose::wait,"event.stream.poll"));
    assert(s::permits(s::purpose::parameter,"parameter.set.atomic")&&!s::permits(s::purpose::node,"parameter.set"));
    assert(s::permits(s::purpose::transform,"transform.static.replace")&&!s::permits(s::purpose::map,"transform.publish"));
    for(auto action:{"action.accept","action.reject","action.execute","action.feedback","action.succeed","action.abort","action.finish.cancelled","action.cancel.respond"})
    {assert(s::grant_only_action(action)&&s::permits(s::purpose::action,action)&&!s::permits(s::purpose::topic,action));}
    assert(!s::grant_only_action("node.open")&&!s::grant_only_action("transform.static.replace.extra"));
    assert(!s::permits(static_cast<s::purpose>(255),"resource.close")&&!s::permits(s::purpose::device,"parameter.get"));
    assert(s::permits(s::purpose::service,"service.abandon")&&s::permits(s::purpose::action,"action.describe"));
}
int main()
{
    retained_node_is_not_created_again();dependencies_still_need_authority_and_exposure();grant_only_does_not_bypass_scope_or_attenuation();semantic_catalogue_is_exact();
    std::cout<<"passed 4 authority dependency security groups\n";
}
