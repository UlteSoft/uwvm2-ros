/** @file Native-only ROS2 witness for complete Sport Request carriers.
 *
 * Two independent native processes communicate through the real selected RMW.
 * Input vectors come from separately validated Rust/AS/TinyGo Wasm output or
 * explicitly labelled host JavaScript output. This process uses the unchanged
 * generated unitree_api::msg::Request, with every field copied and checked.
 * It is a data-carrier fixture, not a robot controller, firmware RPC client or
 * Wasm-to-Gateway permission-boundary test. It opens only a private test topic
 * inside a parent-owned isolated network namespace with loopback enabled.
 *
 * stdin credits are S (start after mapped-file capture), P (publish one next
 * value), F (finish). The subscriber
 * checks the complete generated value and publisher count before each receipt.
 * The parent waits for actual receiver markers before allowing the next send.
 * No implicit resend occurs. DDS acknowledgement alone is never delivery proof.
 * All file and console IO use fast_io; canonical decoding uses the complete
 * delivered C codec and never native struct bytes or a projected message.
 */
#include <rclcpp/rclcpp.hpp>
#include <unitree_api/msg/request.hpp>
#include <fast_io.h>
#include "sdk/origin/custom-types/unitree-api-complete/bounded_records.h"
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

namespace {
using Message=unitree_api::msg::Request;
using Clock=std::chrono::steady_clock;
struct Failure{unsigned line;};
void check(bool ok,unsigned line){if(!ok)throw Failure{line};}
#define require(ok) check((ok),__LINE__)
std::vector<Message> vectors(char const* path){
    fast_io::native_file_loader input{fast_io::mnp::os_c_str(path)};require(input.size()>0&&input.size()<=1048576);
    std::string_view text{input.data(),input.size()};std::vector<Message> result;result.reserve(42);
    while(!text.empty()){
        auto newline=text.find('\n');require(newline!=text.npos);auto line=text.substr(0,newline);text.remove_prefix(newline+1);
        auto tab=line.find('\t');require(tab>0&&tab<=32&&tab!=line.npos);
        auto hex=line.substr(tab+1);require(hex.size()>=74&&hex.size()<=12362&&hex.size()%2==0);
        std::vector<unsigned char> bytes(hex.size()/2);
        for(std::size_t i{};i<bytes.size();++i){
            auto first=hex.data()+2*i;unsigned word{};
            auto parsed=fast_io::parse_by_scan(first,first+2,fast_io::mnp::hex_get<true,true>(word));
            require(parsed.code==fast_io::parse_code::ok&&parsed.iter==first+2&&word<=255);
            bytes[i]=static_cast<unsigned char>(word);
        }
        roswasmc_bounded_2_unitree_api_msg_Request decoded{};
        require(roswasmc_bounded_2_unitree_api_msg_Request_decode(bytes.data(),bytes.size(),&decoded)==0);
        Message value;
        value.header.identity.id=decoded.header.identity.id;
        value.header.identity.api_id=decoded.header.identity.api_id;
        value.header.lease.id=decoded.header.lease.id;
        value.header.policy.priority=decoded.header.policy.priority;
        value.header.policy.noreply=decoded.header.policy.noreply;
        value.parameter.assign(reinterpret_cast<char const*>(decoded.parameter.data),decoded.parameter.size);
        value.binary.assign(decoded.binary.data,decoded.binary.data+decoded.binary.size);
        require(value.header.identity.id==9007199254740993LL&&value.header.lease.id==-9007199254740993LL);
        require(value.header.policy.priority==-101&&value.header.policy.noreply&&value.binary==std::vector<std::uint8_t>({0,255,1,128}));
        result.push_back(std::move(value));require(result.size()<=42);
    }
    require(result.size()==42);return result;
}
unsigned char credit(){char byte{};fast_io::operations::read_all(fast_io::native_stdin(),&byte,&byte+1);return static_cast<unsigned char>(byte);}
}

int main(int argc,char** argv){
    try{
        require(argc==3);bool writer=std::strcmp(argv[1],"writer")==0;
        require(writer||std::strcmp(argv[1],"reader")==0);
        require(std::getenv("ROS_DOMAIN_ID")&&std::strcmp(std::getenv("ROS_DOMAIN_ID"),"209")==0);
        require(std::getenv("ROS_LOCALHOST_ONLY")&&std::strcmp(std::getenv("ROS_LOCALHOST_ONLY"),"1")==0);
        auto values=vectors(argv[2]);auto context=std::make_shared<rclcpp::Context>();context->init(0,nullptr);
        rclcpp::NodeOptions options;options.context(context).use_global_arguments(false).enable_rosout(false)
            .start_parameter_services(false).start_parameter_event_publisher(false).use_intra_process_comms(false);
        auto node=std::make_shared<rclcpp::Node>(writer?"carrier_writer":"carrier_reader","/uwvm_unitree_native",options);
        rclcpp::ExecutorOptions executor_options;executor_options.context=context;
        rclcpp::executors::SingleThreadedExecutor executor{executor_options};executor.add_node(node);
        auto qos=rclcpp::QoS(rclcpp::KeepLast(64)).reliable().durability_volatile();
        std::size_t received{};bool failure{};
        rclcpp::Publisher<Message>::SharedPtr publisher;
        rclcpp::Subscription<Message>::SharedPtr subscription;
        if(writer)publisher=node->create_publisher<Message>("/uwvm_unitree_native/request_vectors",qos);
        else subscription=node->create_subscription<Message>("/uwvm_unitree_native/request_vectors",qos,
            [&](Message const& value){
                if(received>=values.size()||value!=values[received]){failure=true;return;}
                ++received;fast_io::io::println(fast_io::native_stdout(),"DELIVERED ",received);
            });
        // Protocol records must reach the pipe immediately. The default
        // print target is C stdout, whose buffering is unsuitable for credits.
        fast_io::io::println(fast_io::native_stdout(),"READY ",writer?"writer":"reader");
        auto deadline=Clock::now()+std::chrono::seconds{15};
        while((writer?publisher->get_subscription_count():subscription->get_publisher_count())!=1){
            require(!failure&&Clock::now()<deadline);executor.spin_some(std::chrono::milliseconds{1});std::this_thread::sleep_for(std::chrono::milliseconds{1});
        }
        fast_io::io::println(fast_io::native_stdout(),"MATCHED ",writer?"writer":"reader");
        // The parent pins actual loaded files while both processes are held.
        // Hashing those files is outside the finite data-delivery interval.
        require(credit()=='S');
        if(writer){
            for(std::size_t i{};i<values.size();++i){require(credit()=='P');require(publisher->get_subscription_count()==1);publisher->publish(values[i]);fast_io::io::println(fast_io::native_stdout(),"PUBLISHED ",i+1);}
            require(credit()=='F');publisher.reset();
        }else{
            deadline=Clock::now()+std::chrono::seconds{25};
            while(received<values.size()){
                require(!failure&&Clock::now()<deadline);executor.spin_some(std::chrono::milliseconds{1});std::this_thread::sleep_for(std::chrono::milliseconds{1});
            }
            require(!failure&&credit()=='F');subscription.reset();
        }
        executor.remove_node(node);node.reset();context->shutdown("native carrier fixture complete");
        fast_io::io::println(fast_io::native_stdout(),"PASS ",writer?"writer":"reader");return 0;
    }catch(Failure const& error){fast_io::io::println(fast_io::native_stderr(),"NATIVE_INVARIANT_LINE ",error.line);return 22;}
    catch(std::exception const& error){fast_io::io::println(fast_io::native_stderr(),"NATIVE_EXCEPTION ",std::string_view{error.what()});return 22;}
    catch(...){fast_io::io::println(fast_io::native_stderr(),"NATIVE_IO_OR_UNKNOWN_EXCEPTION");return 22;}
}
