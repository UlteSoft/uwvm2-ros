#include "common_ros2.h"

int main(int argc,char** argv) {
    using namespace expanded_event_probe;
    try {
        require(argc==4,"usage expanded event witness kind scenario gateway-full-name");
        std::string kind=argv[1],scenario=argv[2];(void)tag(kind);
        auto known=scenarios(kind);require(scenario=="roundtrip"||scenario=="bytes"||scenario=="deny-subscription"||std::find(known.begin(),known.end(),scenario)!=known.end(),"known expanded witness scenario");
        int ros_argc=1;rclcpp::init(ros_argc,argv);
        auto node=std::make_shared<rclcpp::Node>("native","/typed_events");
        auto publisher=node->create_publisher<rcl_interfaces::msg::ParameterEvent>("/parameter_events",rclcpp::QoS{32}.reliable().durability_volatile());
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);std::atomic<bool> stop{},spin_failed{};
        std::thread spinner{[&] { try { while(!stop&&rclcpp::ok()) executor.spin_once(std::chrono::milliseconds(10)); }catch(...) { spin_failed=true; } }};
        auto cleanup=[&] { stop=true;executor.cancel();spinner.join();publisher.reset();rclcpp::shutdown(); };
        try {
            fast_io::io::println("READY expanded ParameterEvent kind=",kind," scenario=",scenario);
            typed_event_probe::reader_observation readers{node,publisher,argv[3]};readers.capture_baseline(false);
            unsigned publications{};auto publish=[&](rcl_interfaces::msg::ParameterEvent const& message) { publisher->publish(message);++publications; };
            if(scenario=="deny-subscription") {
                auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(5);
                while(rclcpp::ok()&&std::chrono::steady_clock::now()<deadline) {
                    readers.no_projection_after_worker_exit();publish(event("value",value(kind),2,9));
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                readers.log_final_absence();
            } else {
                readers.wait_projection(scenario=="bytes"?2:1,8000);
                if(scenario=="roundtrip") {
                    unsigned stamp=1;for(unsigned phase:{0u,1u}) for(unsigned event_kind:{1u,2u,3u}) {
                        publish(event("value",value(kind,phase),event_kind,stamp++));std::this_thread::sleep_for(std::chrono::milliseconds(150));
                    }
                } else {
                    for(unsigned i=0;i<20;++i) {
                        if(scenario=="bytes") publish(event("quota_value",value(kind),i%3+1,80));
                        else publish(rejected(kind,"value",scenario,i%3+1));
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                    publish(event("value",value(kind,2),2,9));
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            require(rclcpp::ok()&&!spin_failed,"expanded native witness retained all publications");
            fast_io::io::println("PUBLISHED expanded ParameterEvent exact_native_publications=",publications);
        } catch(...) { cleanup();throw; }
        cleanup();fast_io::io::println("PASS expanded ParameterEvent witness");return 0;
    } catch(std::exception const& error) { fast_io::io::perrln(fast_io::mnp::os_c_str(error.what()));return 1; }
}
