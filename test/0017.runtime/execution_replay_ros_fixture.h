// Test-only setup through the actual policy/loader/preflight APIs. No provider,
// native owner, module identity or replay record is fabricated by this fixture.
#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <uwvm2/uwvm/run/loader.h>
static bool replay_ros_fixture{};
static ::std::size_t replay_worker_function{1u};
static void configure_replay_ros_fixture()
{
    auto path=::std::getenv("UWVM_TEST_ROS_CONTRACT");if(!path)return;
    namespace host=::uwvm2::uwvm::imported::ros;namespace model=::uwvm2::imported::ros;
    ::std::ifstream input{path};REQUIRE(bool(input));::std::string line,backend;
    while(::std::getline(input,line))
    {::std::istringstream row{line};::std::string key;row>>key;if(key=="backend")row>>backend;}
    REQUIRE(backend=="ros1" || backend=="ros2");auto v=backend=="ros1"?model::version::ros1:model::version::ros2;
    auto scope=model::scope::single;::std::string_view name="checkpoint-instance";auto& policy=host::runtime_policy;
    REQUIRE(policy.create_target(v,scope,name));REQUIRE(policy.set_enabled(v,scope,name,true));
    for(auto function:{"capability_open","capability_close","topic_publish"})
        REQUIRE(policy.set_interface(v,scope,name,"uwvm_"+backend+"_v1."+function,"allow"));
    input.clear();input.seekg(0);
    while(::std::getline(input,line))
    {
        ::std::istringstream row{line};::std::string key;row>>key;
        if(key=="socket" || key=="worker-uid")
        {::std::string value;row>>value;REQUIRE(policy.set_connection(v,scope,name,key=="socket"?"gateway-socket":"gateway-uid",value));}
        else if(key=="connection")
        {::std::string field,value;row>>field>>value;REQUIRE(policy.set_connection(v,scope,name,field,value));}
        else if(key=="grant")
        {
            ::std::string alias,operation,resource,type,identity;row>>alias>>operation>>resource>>type>>identity;
            model::capability_spec cap{};cap.alias=alias;cap.operation=operation;cap.resource=resource;cap.type=type;cap.type_identity=identity;
            REQUIRE(policy.grant(v,scope,name,::std::move(cap)));
        }
        else if(key=="set")
        {::std::string alias,field,value;row>>alias>>field>>value;REQUIRE(policy.set_capability(v,scope,name,alias,field,value));}
    }
    host::policy_configured=host::policy_any_enabled=true;replay_ros_fixture=true;
}
static ::std::pair<unsigned,unsigned> replay_ros_audit_counts()
{
    if(!replay_ros_fixture)return {};
    auto path=::std::getenv("UWVM_TEST_ROS_AUDIT");REQUIRE(path!=nullptr);::std::ifstream input{path};REQUIRE(bool(input));
    ::std::string line;::std::pair<unsigned,unsigned> result{};
    while(::std::getline(input,line))
    {
        if(line.find("operation=topic.publish ")!=::std::string::npos)++result.first;
        if(line.find("operation=session.bind ")!=::std::string::npos)++result.second;
    }
    return result;
}
