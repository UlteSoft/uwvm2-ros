#include "common.h"
#include "../../../../../src/ros/gateway/ros2/resources.h"
#include <rclcpp/rclcpp.hpp>
#include <atomic>
#include <cstdlib>
#include <iostream>
#include <new>
#include <thread>

namespace parameter_deadline_allocations
{
    // This test-only barrier intercepts exactly one canonical wire allocation
    // on the calling thread. ROS callbacks and middleware threads are unarmed.
    thread_local std::size_t expected_size{},hits{};
    thread_local std::chrono::steady_clock::time_point release;
    void pause(std::size_t size)
    {
        if(expected_size&&size==expected_size)
        {
            expected_size=0;++hits;std::this_thread::sleep_until(release);
        }
    }
    struct barrier
    {
        barrier(std::size_t size,std::chrono::steady_clock::time_point until)
        { expected_size=size;hits=0;release=until; }
        ~barrier() { expected_size=0; }
    };
}
void* operator new(std::size_t size)
{
    parameter_deadline_allocations::pause(size);
    if(auto pointer=std::malloc(size?size:1)) { return pointer; }
    throw std::bad_alloc{};
}
void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* pointer) noexcept { std::free(pointer); }
void operator delete[](void* pointer) noexcept { std::free(pointer); }
void operator delete(void* pointer,std::size_t) noexcept { std::free(pointer); }
void operator delete[](void* pointer,std::size_t) noexcept { std::free(pointer); }

namespace parameter_deadline_test
{
    using namespace ros_resources_probe;
    namespace resources=uwvm2::ros_gateway::ros2_resources;
    using steady=std::chrono::steady_clock;
    unsigned checks{};
    constexpr std::uint32_t budget{150};
    void check(bool value,char const* message) { ++checks;require(value,message); }
    std::vector<std::uint8_t> wire(std::string const& value)
    {
        std::vector<std::uint8_t> bytes;bytes.reserve(4+value.size());
        for(unsigned shift{};shift!=32;shift+=8) { bytes.push_back(static_cast<std::uint8_t>(value.size()>>shift)); }
        bytes.insert(bytes.end(),value.begin(),value.end());return bytes;
    }
    core::capability_spec cap(rclcpp::Node::SharedPtr const& node,std::string const& operation,std::string const& name)
    {
        std::string const node_name{node->get_fully_qualified_name()};bool list{operation=="param.list"};
        auto result{resource(core::version::ros2,"deadline",operation,list?node_name:resources::parameter_resource(node_name,name))};
        result.limits["parameter-node"]=node_name;result.limits["parameter-name"]=list?"not-applicable":name;
        if(list) { result.limits["allowed-name-prefix"]=node_name+"/read"; }
        gateway::validate(core::version::ros2,result);check(resources::known(result),"actual local parameter capability is valid");return result;
    }
    void timeout(ipc::call_result const& result,char const* message)
    { check(result.error==110,message);check(result.payload.empty(),"expired parameter result has no payload"); }
    void exact(ipc::call_result const& result,std::string const& value)
    { check(!result.error,"pre-deadline parameter read succeeds");check(result.payload==wire(value),"complete parameter wire matches independent exact oracle"); }
    bool selected(std::string const& mode,char const* operation) { return mode=="all"||mode==operation; }

    struct executor_runner
    {
        rclcpp::executors::SingleThreadedExecutor executor;
        std::thread thread;
        executor_runner(rclcpp::Node::SharedPtr const& gateway,rclcpp::Node::SharedPtr const& native)
        { executor.add_node(gateway);executor.add_node(native);thread=std::thread{[this] { executor.spin(); }}; }
        ~executor_runner() { executor.cancel();if(thread.joinable()) { thread.join(); } }
    };

    void remote_inspect(rclcpp::Node::SharedPtr const& gateway,rclcpp::Node::SharedPtr const& native,std::string const& text)
    {
        using service=rcl_interfaces::srv::GetParameters;
        std::atomic<unsigned> requests{};
        auto server{native->create_service<service>(std::string{native->get_fully_qualified_name()}+"/get_parameters",
            [&,native](std::shared_ptr<service::Request> request,std::shared_ptr<service::Response> response) {
                ++requests;
                if(request->names==std::vector<std::string>{"read.a"})
                { response->values.push_back(native->get_parameter("read.a").get_parameter_value().to_value_msg()); }
            })};
        executor_runner running{gateway,native};auto configuration{cap(native,"param.get","read.a")};
        auto request{std::make_shared<service::Request>()};request->names={"read.a"};
        std::string const resource_name{std::string{native->get_fully_qualified_name()}+"/get_parameters"};
        exact(resources::invoke<service>(gateway,configuration,resource_name,request,steady::now()+std::chrono::seconds(3),
            [&](auto const& response) { check(response.values.size()==1,"actual native getter returned one value");return resources::parameter_result(configuration,response.values[0]); }),text);
        auto const before{requests.load()};auto deadline{steady::now()+std::chrono::seconds(1)};std::size_t hits{};
        auto result{resources::invoke<service>(gateway,configuration,resource_name,request,deadline,[&](auto const& response) {
            check(steady::now()<deadline,"actual remote response reached inspector before deadline");
            check(response.values.size()==1,"actual remote cutoff response contains one value");
            parameter_deadline_allocations::barrier pause{4+text.size(),deadline+std::chrono::milliseconds(100)};
            auto encoded{resources::parameter_result(configuration,response.values[0])};hits=parameter_deadline_allocations::hits;return encoded;
        })};
        check(hits==1,"remote inspector paused exactly one canonical wire allocation");
        check(requests==before+1,"actual native getter handled the remote cutoff request");
        timeout(result,"expired remote parameter inspector accepted success");
        std::cout<<"PASS actual generated GetParameters ready before deadline, inspector wire allocation crosses deadline, final result110 empty native_get_requests1\n";
        (void)server;
    }

    int run(int argc,char** argv)
    {
        std::string const mode{argc==2?argv[1]:"all"};
        check(argc<=2&&(mode=="all"||mode=="set"||mode=="declare"||mode=="undeclare"||mode=="get"||mode=="describe"||mode=="list"||mode=="graph"||mode=="remote"),
              "usage parameter-deadline-ros2 [all|set|declare|undeclare|get|describe|list|graph|remote]");
        rclcpp::init(argc,argv);
        rclcpp::NodeOptions options;options.start_parameter_services(false).start_parameter_event_publisher(false).enable_rosout(false);
        options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
        auto node{std::make_shared<rclcpp::Node>("gateway_deadline","/uwvm_parameter_deadline",options)};
        auto native{std::make_shared<rclcpp::Node>("native_deadline","/uwvm_parameter_deadline",options)};
        rcl_interfaces::msg::ParameterDescriptor removable;removable.dynamic_typing=true;
        std::string const text(1377,'x');node->declare_parameter<std::string>("read.a",text,removable);
        node->declare_parameter<std::string>("read.z","last");native->declare_parameter<std::string>("read.a",text);
        node->declare_parameter<std::string>("mutation.set","before");
        std::string paused_name;steady::time_point release;unsigned callbacks{};
        auto callback{node->add_on_set_parameters_callback([&](std::vector<rclcpp::Parameter> const& parameters) {
            for(auto const& parameter:parameters)
            {
                if(parameter.get_name()==paused_name) { ++callbacks;std::this_thread::sleep_until(release); }
            }
            rcl_interfaces::msg::SetParametersResult result;result.successful=true;return result;
        })};
        for(auto const& operation:{"param.get","param.describe","param.list","param.undeclare"})
        {
            auto configuration{cap(node,operation,"read.a")};
            timeout(resources::call(node,configuration,{},0),"zero-budget local read/removal accepted success");
            check(node->has_parameter("read.a"),"zero-budget local removal retains native parameter");
        }
        {
            auto configuration{cap(node,"param.set","mutation.set")};
            timeout(resources::call(node,configuration,wire("forbidden"),0),"zero-budget local set accepted success");
            check(node->get_parameter("mutation.set").as_string()=="before"&&callbacks==0,"zero-budget set has no native effect or callback");
            configuration=cap(node,"param.declare","mutation.zero");
            timeout(resources::call(node,configuration,wire("forbidden"),0),"zero-budget local declare accepted success");
            check(!node->has_parameter("mutation.zero")&&callbacks==0,"zero-budget declare has no native effect or callback");
        }
        for(auto const& operation:{"set","declare"})
        {
            if(!selected(mode,operation)) { continue; }
            std::string const name{std::string{"mutation."}+operation};auto configuration{cap(node,std::string{"param."}+operation,name)};
            auto payload{wire("after callback")};paused_name=name;auto const before{callbacks};
            auto started{steady::now()};release=started+std::chrono::milliseconds(budget+100);
            auto result{resources::call(node,configuration,payload,budget)};paused_name.clear();
            check(callbacks==before+1&&steady::now()>=release,"actual native on-set callback crossed the operation deadline once");
            check(node->get_parameter(name).as_string()=="after callback","already admitted native mutation remains observable after deadline");
            std::cout<<"OBSERVED actual local "<<operation<<" callback crossed budget150ms native_value_changed1\n";
            timeout(result,operation==std::string{"set"}?"expired local parameter set accepted success":"expired local parameter declare accepted success");
            if(operation==std::string{"declare"}) { configuration=cap(node,"param.declare","mutation.normal"); }
            auto normal{resources::call(node,configuration,wire("normal"),1000)};
            check(!normal.error&&normal.payload.empty(),"pre-deadline local mutation still succeeds");
            check(node->get_parameter(operation==std::string{"set"}?name:"mutation.normal").as_string()=="normal","normal local mutation has the exact native value");
            std::cout<<"PASS actual local "<<operation<<" callback deadline result110 empty, admitted native effect retained, normal mutation success\n";
        }
        std::string const node_name{node->get_fully_qualified_name()};
        std::string const descriptor{"name=read.a\ntype=4\nread_only=false\ndescription=\nconstraints="};
        std::string const rows{node_name+"/read/a\n"+node_name+"/read/z\n"};
        for(auto const& operation:{"get","describe","list","undeclare"})
        {
            if(!selected(mode,operation)) { continue; }
            auto configuration{cap(node,std::string{"param."}+operation,"read.a")};
            auto const& expected{operation==std::string{"describe"}?descriptor:operation==std::string{"list"}?rows:text};
            if(operation!=std::string{"undeclare"}) { exact(resources::call(node,configuration,{},1000),expected); }
            ipc::call_result result;std::size_t hits{};auto const started{steady::now()};
            {
                parameter_deadline_allocations::barrier pause{4+expected.size(),started+std::chrono::milliseconds(budget+100)};
                result=resources::call(node,configuration,{},budget);hits=parameter_deadline_allocations::hits;
            }
            check(hits==1&&steady::now()>=started+std::chrono::milliseconds(budget+100),"local cutoff paused exactly one canonical wire allocation through deadline");
            if(operation==std::string{"undeclare"})
            { check(node->has_parameter("read.a")&&node->get_parameter("read.a").as_string()==text,"expired undeclare preflight removed the actual native parameter"); }
            timeout(result,"expired local parameter encoding accepted success");
            std::cout<<"PASS actual local "<<operation<<" wire allocation barrier crossed deadline result110 empty"<<(operation==std::string{"undeclare"}?" native_parameter_retained1":"")<<"\n";
        }
        if(selected(mode,"graph"))
        {
            auto configuration{resource(core::version::ros2,"graph-deadline","graph.inspect-node",node_name)};
            configuration.limits["allowed-name-prefix"]=node_name;gateway::validate(core::version::ros2,configuration);
            check(resources::known(configuration),"actual Graph inspect-node capability is valid");
            std::string const expected{node_name+"\n"};ipc::call_result positive;
            auto const discovery{steady::now()+std::chrono::seconds(3)};
            do
            {
                positive=resources::call(node,configuration,{},1000);
                if(!positive.error) { break; }
                check(positive.error==2&&positive.payload.empty(),"Graph discovery retry is exact missing-node result");
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            while(steady::now()<discovery);
            exact(positive,expected);
            timeout(resources::call(node,configuration,{},0),"zero-budget Graph inspect accepted success");
            auto const started{steady::now()};ipc::call_result expired;std::size_t hits{};
            {
                parameter_deadline_allocations::barrier pause{4+expected.size(),started+std::chrono::milliseconds(budget+100)};
                expired=resources::call(node,configuration,{},budget);hits=parameter_deadline_allocations::hits;
            }
            check(hits==1&&steady::now()>=started+std::chrono::milliseconds(budget+100),"actual Graph wire allocation crossed deadline exactly once");
            timeout(expired,"expired actual Graph inspect-node encoding accepted success");
            std::cout<<"PASS actual Graph inspect-node exact own FQN, canonical wire allocation barrier crosses deadline result110 empty\n";
        }
        if(selected(mode,"remote")) { remote_inspect(node,native,text); }
        if(mode=="all"||mode=="undeclare")
        {
            auto normal{resources::call(node,cap(node,"param.undeclare","read.a"),{},1000)};
            check(!normal.error&&normal.payload.empty()&&!node->has_parameter("read.a"),"normal pre-deadline undeclare retires the native parameter");
        }
        std::cout<<"PASS actual ROS2 Parameter deadline "<<checks<<" checks mode="<<mode<<"\n";
        (void)callback;return 0;
    }
}
int main(int argc,char** argv)
{
    try { auto result{parameter_deadline_test::run(argc,argv)};rclcpp::shutdown();return result; }
    catch(std::exception const& error)
    { if(rclcpp::ok()) { rclcpp::shutdown(); }std::cerr<<"FAIL "<<error.what()<<'\n';return 1; }
}
