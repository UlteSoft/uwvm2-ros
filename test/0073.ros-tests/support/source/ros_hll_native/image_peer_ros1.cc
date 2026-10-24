// Independent ROS1 Image witness. Never linked into the Guest or Gateway.
#include <ros/ros.h>
#include <ros/message_event.h>
#include <sensor_msgs/Image.h>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <thread>
#include <unistd.h>

namespace {
using Clock = std::chrono::steady_clock;
void require(bool ok, char const* why) { if (!ok) throw std::runtime_error(why); }
void write_line(int fd, std::string value) {
  value += '\n';
  char const* p = value.data(); auto left = value.size();
  while (left) { auto n = ::write(fd, p, left); require(n > 0, "marker write"); p += n; left -= n; }
}
void marker(char const* phase, std::string const& ns, unsigned warmup,
            unsigned delivered, unsigned publishers) {
  write_line(1, std::string{phase} + "_NATIVE_ROS1_IMAGE namespace=" + ns +
      " gateway=" + ns + "/moonbit_image_gateway transport=tcpros" +
      " type=sensor_msgs/Image md5=060021388200f6f0f447d0fcd9c64743 latch=0" +
      " warmup=" + std::to_string(warmup) + " delivered=" + std::to_string(delivered) +
      " publishers=" + std::to_string(publishers));
}
}

int main(int argc, char** argv) {
  try {
    require(argc == 3 || (argc == 4 && std::string{argv[3]} == "absent"),
            "usage: image-peer-ros1 namespace master-port [absent]");
    std::string const ns{argv[1]}, port{argv[2]}; bool const absent = argc == 4;
    require(ns.size() > 1 && ns.size() < 128 && ns.front() == '/' &&
        ns.find_first_not_of("/abcdefghijklmnopqrstuvwxyz0123456789_") == std::string::npos, "namespace");
    require(!port.empty() && port.find_first_not_of("0123456789") == std::string::npos &&
            std::stoul(port) >= 1024 && std::stoul(port) <= 65535, "master port");
    auto const* master = std::getenv("ROS_MASTER_URI"); auto const* address = std::getenv("ROS_IP");
    require(master && std::string{master} == "http://127.0.0.1:" + port &&
            address && std::string{address} == "127.0.0.1", "explicit loopback master");
    require(!std::getenv("ROS_HOSTNAME") && !std::getenv("ROS_NAMESPACE") &&
            !std::getenv("LD_PRELOAD"), "unexpected environment");
    ros::M_string remaps; remaps["__ns"] = ns;
    ros::init(remaps, "moonbit_image_peer", ros::init_options::NoSigintHandler | ros::init_options::NoRosout);
    ros::NodeHandle node{ns}; auto const gateway = ns + "/moonbit_image_gateway";
    require(std::string{ros::message_traits::MD5Sum<sensor_msgs::Image>::value()} ==
            "060021388200f6f0f447d0fcd9c64743", "native Image MD5");
    unsigned warmup{}, delivered{}; bool matched{}, bad{}, acknowledged{};
    ros::SubscribeOptions options;
    options.initByFullCallbackType<ros::MessageEvent<sensor_msgs::Image const> const&>(
        ns + "/image", 4, [&](ros::MessageEvent<sensor_msgs::Image const> const& event) {
          auto value = event.getMessage(); auto header = event.getConnectionHeaderPtr();
          if (absent || !value || !header || event.getPublisherName() != gateway) { bad = true; return; }
          auto field = [&](char const* key, std::string const& expected) {
            auto i = header->find(key); return i != header->end() && i->second == expected;
          };
          if (!field("callerid", gateway) || !field("type", "sensor_msgs/Image") ||
              !field("md5sum", "060021388200f6f0f447d0fcd9c64743") || !field("latching", "0")) { bad = true; return; }
          auto const& m = *value;
          if (m.header.frame_id == "camera/warmup" && !delivered) {
            if (++warmup > 1 || m.header.seq != 0 || m.header.stamp.sec || m.header.stamp.nsec ||
                m.height != 1 || m.width != 1 || m.encoding != "mono8" || m.is_bigendian ||
                m.step != 1 || m.data != std::vector<std::uint8_t>{0}) bad = true;
          } else if (m.header.frame_id == "camera/optical") {
            if (!matched || ++delivered != 1 || m.header.seq != 1 || m.header.stamp.sec != 1 ||
                m.header.stamp.nsec != 2 || m.height != 2 || m.width != 2 || m.encoding != "mono8" ||
                m.is_bigendian || m.step != 2 || m.data != std::vector<std::uint8_t>{0x4c,0x95,0x1c,0xff}) bad = true;
          } else bad = true;
        });
    options.transport_hints = ros::TransportHints{}.tcpNoDelay().reliable();
    auto subscription = node.subscribe(options);
    auto forbidden = node.subscribe<sensor_msgs::Image>(ns + "/forbidden", 4,
        [&](sensor_msgs::Image::ConstPtr const&) { bad = true; });
    marker("READY", ns, 0, 0, 0); auto const end = Clock::now() + std::chrono::seconds{35};
    auto quiet = end; bool announced{};
    while (Clock::now() < end) {
      ros::spinOnce(); require(ros::ok() && !bad, "unexpected image/header/writer/duplicate");
      auto count = subscription.getNumPublishers(); require(count <= 1 && !forbidden.getNumPublishers(), "foreign publisher");
      if (absent) {
        require(count == 0, "publisher in zero-grant scene");
        if (!acknowledged) {
          pollfd input{0,POLLIN,0}; auto ready = ::poll(&input,1,0); require(ready >= 0, "gate poll");
          if (ready) { char value{}; require(::read(0,&value,1) == 1 && value == 'Q', "absence gate"); acknowledged = true; }
        }
      } else {
        if (!matched && count == 1) { matched = true; marker("MATCHED",ns,warmup,0,1); }
        if (delivered == 1 && !announced) { announced = true; marker("DELIVERED",ns,warmup,1,count); }
      }
      if ((absent ? acknowledged : delivered == 1) && count == 0) {
        if (quiet == end) quiet = Clock::now();
        if (Clock::now() - quiet >= std::chrono::seconds{2}) {
          subscription.shutdown(); forbidden.shutdown(); node.shutdown(); ros::shutdown();
          marker(absent ? "ABSENT" : "PASS",ns,warmup,delivered,0); return 0;
        }
      } else quiet = end;
      std::this_thread::sleep_for(std::chrono::milliseconds{10});
    }
    throw std::runtime_error("delivery/absence/retirement deadline");
  } catch (std::exception const& error) {
    write_line(2,std::string{"FAIL_NATIVE_ROS1_IMAGE "} + error.what()); ros::shutdown(); return 1;
  }
}
