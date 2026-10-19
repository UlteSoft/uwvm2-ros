// Installed ROS2 SDK witness only. This TU includes no Wasm/facade headers or
// handwritten native structs. Root must build/run it separately with actual
// fullMD/ELF/maps/native identity/namespace/QoS/source receipts and a finite owned
// pidfd deadline. Stdin R-newline credits are an explicit native supervisor;
// READY/ADMITTED/printed counters alone never establish Guest delivery.
#include <rclcpp/rclcpp.hpp>
#include <example_interfaces/srv/add_two_ints.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <std_srvs/srv/set_bool.hpp>
#include <chrono>
#include <cstdint>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

static std::string text()
{
    static char const value[]{"native:\0\xEF\xBB\xBF机器人🙂"};
    return {value, sizeof(value)-1};
}
static void print_text(std::string const& value)
{
    std::cout << value.size() << ' ';
    for(unsigned char byte:value)
        std::cout << std::hex << std::setfill('0') << std::setw(2) << unsigned(byte);
    std::cout << std::dec << '\n';
}
static bool credit()
{
    char a{},b{};
    if(!std::cin.get(a))return false;
    if(!std::cin.get(b)||a!='R'||b!='\n')throw std::runtime_error("exact R-newline native credit required");
    return true;
}

template<class Service>
static void run(std::shared_ptr<rclcpp::Node> const& node,std::string const& mode,std::string const& name)
{
    typename rclcpp::Service<Service>::SharedPtr server;
    typename rclcpp::Client<Service>::SharedPtr client;
    unsigned received{};
    if(mode=="--serve")
        server=node->create_service<Service>(name,[&](typename Service::Request::SharedPtr request,typename Service::Response::SharedPtr response)
        {
            if constexpr(std::is_same_v<Service,example_interfaces::srv::AddTwoInts>)
            {
                if(request->a!=41||request->b!=1)throw std::runtime_error("wrong complete native AddTwoInts request");
                response->sum=42;
                std::cout << "NATIVE_REQUEST_ADD " << request->a << ' ' << request->b << '\n';
            }
            else
            {
                if constexpr(std::is_same_v<Service,std_srvs::srv::SetBool>)
                    if(!request->data)throw std::runtime_error("wrong complete native SetBool request");
                response->success=true;response->message=text();
                std::cout << "NATIVE_RESPONSE_TEXT ";print_text(response->message);
            }
            ++received;
        });
    else client=node->create_client<Service>(name);
    std::cout << "NATIVE_PREPARED " << mode << ' ' << name << '\n' << std::flush;
    bool submitted{},completed{};
    std::int64_t request_id{};
    typename rclcpp::Client<Service>::SharedFuture future;
    for(unsigned pass{};pass!=64&&credit();++pass)
    {
        rclcpp::spin_some(node);
        if(client&&!submitted&&client->service_is_ready())
        {
            auto request{std::make_shared<typename Service::Request>()};
            if constexpr(std::is_same_v<Service,example_interfaces::srv::AddTwoInts>)
            {request->a=41;request->b=1;}
            else if constexpr(std::is_same_v<Service,std_srvs::srv::SetBool>)request->data=true;
            auto admitted{client->async_send_request(request)};
            request_id=admitted.request_id;
            future=admitted.future.share();submitted=true;
            std::cout << "NATIVE_REQUEST_ADMITTED_NOT_COMPLETED\n";
        }
        if(client&&submitted&&!completed&&future.wait_for(std::chrono::seconds{0})==std::future_status::ready)
        {
            auto response{future.get()};
            if constexpr(std::is_same_v<Service,example_interfaces::srv::AddTwoInts>)
            {
                if(response->sum!=42)throw std::runtime_error("wrong complete native AddTwoInts response");
                std::cout << "NATIVE_RESPONSE_ADD " << response->sum << '\n';
            }
            else
            {
                if(!response->success||response->message!=text())throw std::runtime_error("wrong complete native standard service response");
                std::cout << "NATIVE_RESPONSE_TEXT ";print_text(response->message);
            }
            completed=true;
        }
        std::cout.flush();
    }
    if(mode=="--serve"?!received:!completed)
    {
        // Remove native client bookkeeping only. This does not cancel an
        // already admitted external service effect and never retries it.
        if(client&&submitted)client->remove_pending_request(request_id);
        throw std::runtime_error("no complete native request/response observed");
    }
    server.reset();client.reset();
}
int main(int argc,char**argv)
{
    bool initialized{};
    try
    {
        if(argc!=4||(std::string{argv[1]}!="--serve"&&std::string{argv[1]}!="--call"))
            throw std::runtime_error("--serve|--call AddTwoInts|Trigger|SetBool /exact-service");
        std::string const mode{argv[1]},type{argv[2]},name{argv[3]};
        if(name.empty()||name.front()!='/')throw std::runtime_error("exact absolute service required");
        int native_argc{1};char* native_argv[]{argv[0],nullptr};rclcpp::init(native_argc,native_argv);initialized=true;
        auto node{std::make_shared<rclcpp::Node>("rclpy_service_native_peer")};
        if(type=="AddTwoInts")run<example_interfaces::srv::AddTwoInts>(node,mode,name);
        else if(type=="Trigger")run<std_srvs::srv::Trigger>(node,mode,name);
        else if(type=="SetBool")run<std_srvs::srv::SetBool>(node,mode,name);
        else throw std::runtime_error("unsupported exact native witness type");
        node.reset();rclcpp::shutdown();initialized=false;
        std::cout << "NATIVE_PEER_FINISHED_NOT_WHOLE_LANE_ACCEPTANCE\n";
        return 0;
    }
    catch(std::exception const& error)
    {
        if(initialized)rclcpp::shutdown();
        std::cerr << error.what() << '\n';return 1;
    }
}
