/**
 * @file native_ros1_peer.cc
 * @brief Independent TCPROS String witness for the portable language lessons.
 *
 * Build against an explicitly admitted Noetic SDK. This is a native test
 * executable, never a Guest library, Gateway or Runtime import. Its owner
 * starts a separate loopback-only Master, checks the Master graph independently,
 * and releases the Guest's P/A credits only after the corresponding markers.
 * The owner enforces the finite process deadline and records natural exit0.
 */
#include <ros/ros.h>
#include <ros/message_event.h>
#include <std_msgs/String.h>
#include <fast_io.h>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

namespace {
using steady_clock = std::chrono::steady_clock;

// Older generated ROS1 messages require allocator::rebind, removed from the
// C++20 standard allocator. Preserve that source contract in this test's own
// message allocator; do not patch installed ROS headers or the Gateway ABI.
template <typename T> struct message_allocator : std::allocator<T> {
  using value_type = T;
  template <typename U> struct rebind { using other = message_allocator<U>; };
  message_allocator() = default;
  template <typename U> message_allocator(message_allocator<U> const&) noexcept {}
};
using String = std_msgs::String_<message_allocator<void>>;

void require(bool condition, char const* field) {
  if (!condition) throw std::runtime_error(field);
}
void marker(char const* phase, std::string const& ns, unsigned warmup,
            unsigned delivered, unsigned publishers) {
  // Handshake markers must reach the owner immediately even when stdout is a
  // regular evidence file. Select the descriptor explicitly; do not rely on
  // another ROS library to change the C stdout buffering mode.
  fast_io::io::println(fast_io::posix_stdout(), fast_io::mnp::os_c_str(phase), " namespace=", ns,
      " gateway=", ns, "/roswasm_language_gateway transport=tcpros",
      " type=std_msgs/String md5=992ce8a1687cec8c8bd883ec73ca41d1 latch=0",
      " warmup=", warmup, " delivered=", delivered, " publishers=", publishers);
}
}

int main(int argc, char** argv) {
  try {
    require(argc == 2, "usage: native-ros1-peer <explicit-namespace>");
    std::string const ns{argv[1]};
    require(ns.size() > 1 && ns.size() < 128 && ns.front() == '/' &&
        ns.find_first_not_of("/abcdefghijklmnopqrstuvwxyz0123456789_") == std::string::npos,
        "explicit namespace");
    auto const* master = std::getenv("ROS_MASTER_URI");
    auto const* address = std::getenv("ROS_IP");
    require(master && std::string{master} == "http://127.0.0.1:11386" &&
        address && std::string{address} == "127.0.0.1", "explicit loopback Master");
    require(!std::getenv("ROS_HOSTNAME") && !std::getenv("ROS_NAMESPACE") &&
        !std::getenv("LD_PRELOAD"), "unselected environment");
    ros::M_string remaps;
    remaps["__ns"] = ns;
    ros::init(remaps, "roswasm_language_native_peer",
        ros::init_options::NoSigintHandler | ros::init_options::NoRosout);
    ros::NodeHandle node{ns};
    auto const gateway = ns + "/roswasm_language_gateway";
    require(ros::this_node::getName() == ns + "/roswasm_language_native_peer",
        "peer identity");
    require(std::string{ros::message_traits::DataType<String>::value()} == "std_msgs/String" &&
        std::string{ros::message_traits::MD5Sum<String>::value()} ==
            "992ce8a1687cec8c8bd883ec73ca41d1", "native String identity");
    unsigned warmup{}, delivered{};
    bool matched{}, bad{};
    ros::SubscribeOptions options;
    options.initByFullCallbackType<ros::MessageEvent<String const> const&>(
        ns + "/telemetry", 4, [&](ros::MessageEvent<String const> const& event) {
          auto value = event.getMessage();
          auto header = event.getConnectionHeaderPtr();
          if (!value || !header || event.getPublisherName() != gateway) { bad = true; return; }
          auto field = [&](char const* key, std::string const& expected) {
            auto found = header->find(key);
            return found != header->end() && found->second == expected;
          };
          if (!field("callerid", gateway) || !field("type", "std_msgs/String") ||
              !field("md5sum", "992ce8a1687cec8c8bd883ec73ca41d1") ||
              !field("latching", "0")) { bad = true; return; }
          if (value->data == "wasmros-telemetry-ready" && delivered == 0) {
            if (++warmup > 1) bad = true;
          } else if (value->data == "bounded WasmROS telemetry") {
            if (!matched || ++delivered != 1) bad = true;
          } else bad = true;
        });
    options.transport_hints = ros::TransportHints{}.tcpNoDelay().reliable();
    auto subscription = node.subscribe(options);
    marker("READY_NATIVE_ROS1_TELEMETRY", ns, 0, 0, 0);
    auto const deadline = steady_clock::now() + std::chrono::seconds{30};
    auto quiet_since = deadline;
    bool announced{};
    while (steady_clock::now() < deadline) {
      ros::spinOnce();
      require(!bad && ros::ok(), "unexpected writer/header/value/duplicate");
      auto const publishers = subscription.getNumPublishers();
      require(publishers <= 1, "foreign publisher");
      if (!matched && publishers == 1) {
        matched = true;
        marker("MATCHED_NATIVE_ROS1_TELEMETRY", ns, warmup, 0, 1);
      }
      if (delivered == 1 && !announced) {
        announced = true;
        marker("DELIVERED_NATIVE_ROS1_TELEMETRY", ns, warmup, 1, publishers);
      }
      // Require two contiguous quiet seconds after the native publisher has
      // disappeared. A delayed unregister or any new message resets/fails it.
      if (delivered == 1 && publishers == 0) {
        if (quiet_since == deadline) quiet_since = steady_clock::now();
        if (steady_clock::now() - quiet_since >= std::chrono::seconds{2}) {
          subscription.shutdown(); node.shutdown(); ros::shutdown();
          marker("PASS_NATIVE_ROS1_TELEMETRY", ns, warmup, 1, 0);
          return 0;
        }
      } else quiet_since = deadline;
      std::this_thread::sleep_for(std::chrono::milliseconds{10});
    }
    throw std::runtime_error("match/delivery/retirement deadline");
  } catch (std::exception const& error) {
    fast_io::io::println(fast_io::posix_stderr(), "FAIL_NATIVE_ROS1_TELEMETRY ",
        fast_io::mnp::os_c_str(error.what()));
    ros::shutdown();
    return 1;
  }
}
