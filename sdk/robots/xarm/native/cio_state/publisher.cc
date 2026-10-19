/** Explicit one-shot independent fixture publisher. Never connect a robot.
 * This publishes only on /uwvm_xarm_fixture/cio_state after an owned mapping
 * stop and checked I/M/P/F stdin protocol. Native publish alone is not Guest
 * delivery. There is no motion service/client, controller IP or robot driver.
 * Root must supply actual SDK/MD/link/ELF/maps, own process wall/RSS limits and
 * keep this process alive until its separate Guest full-value proof completes.
 */
#include "tests/ros_custom_types/bounded-v2/native_protocol.h"
#include "fixture.hpp"
#include <csignal>
#include <memory>
#include <stdexcept>
#include <string_view>
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
#include <ros/ros.h>
#else
#include <rclcpp/rclcpp.hpp>
#endif
namespace {void need(bool ok,char const* what){if(!ok)throw std::runtime_error{what};}}
int main(int argc,char** argv) {
    if(argc!=2||std::string_view{argv[1]}!="--publish-fixture")return 2;
    try {
        need(std::raise(SIGSTOP)==0,"mapping stop");bounded_native_io::gate('I');
        need(xarm_cio_state_fixture::actual_identities(),"actual native identity mismatch");
        auto value=xarm_cio_state_fixture::sample();std::vector<std::uint8_t> encoded;
        need(Codec::encode(value,encoded),"complete actual fixture refused by generated native codec");
        // Only the actual binary path is passed to the ROS initializer. The
        // explicit application option above is not a ROS remapping argument.
        int ros_argc=1;char* ros_argv[]{argv[0],nullptr};
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
        ::ros::init(ros_argc,ros_argv,"xarm_cio_state_fixture",::ros::init_options::NoSigintHandler);
        struct shutdown {~shutdown(){::ros::shutdown();}} cleanup;
        ::ros::NodeHandle node{"/uwvm_xarm_fixture"};
        auto publication=node.advertise<Native>("/uwvm_xarm_fixture/cio_state",1,false);
#else
        rclcpp::InitOptions options;options.set_domain_id(205);rclcpp::init(ros_argc,ros_argv,options);
        struct shutdown {~shutdown(){rclcpp::shutdown();}} cleanup;
        auto node=std::make_shared<rclcpp::Node>("xarm_cio_state_fixture","/uwvm_xarm_fixture");
        rclcpp::QoS qos{1};qos.reliable().durability_volatile();
        auto publication=node->create_publisher<Native>("/uwvm_xarm_fixture/cio_state",qos);
#endif
        bounded_native_io::print(STDOUT_FILENO,"READY_XARM_CIO_STATE_NATIVE_FIXTURE\n");
        bool matched=false;
        for(unsigned credit{};credit!=64&&!matched;++credit){bounded_native_io::gate('M');
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
            ::ros::spinOnce();matched=publication.getNumSubscribers()==1;
#else
            rclcpp::spin_some(node);matched=publication->get_subscription_count()==1;
#endif
        }
        need(matched,"exact independently owned Gateway subscriber absent");
        bounded_native_io::print(STDOUT_FILENO,"MATCHED_XARM_CIO_STATE_NATIVE_FIXTURE\n");
        bounded_native_io::gate('P');
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
        need(publication.getNumSubscribers()==1,"subscriber changed before publish");publication.publish(value);::ros::spinOnce();
#else
        need(publication->get_subscription_count()==1,"subscriber changed before publish");publication->publish(value);rclcpp::spin_some(node);
#endif
        bounded_native_io::print(STDOUT_FILENO,"PUBLISHED_XARM_CIO_STATE_NATIVE_FIXTURE enqueued=1 delivery_qualified=0\n");
        bounded_native_io::gate('F');
        // F is supplied only after Root retains independent Guest all-field
        // evidence. It is not accepted as an oracle assertion by this source.
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
        publication.shutdown();
#else
        publication.reset();node.reset();
#endif
        return 0;
    }catch(std::exception const& e){bounded_native_io::print(STDERR_FILENO,"FAIL_XARM_CIO_STATE_NATIVE_FIXTURE ",e.what(),'\n');return 1;}
}
