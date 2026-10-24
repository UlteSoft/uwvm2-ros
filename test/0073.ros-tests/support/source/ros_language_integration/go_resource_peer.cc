// Independent native Humble witness for the official-Go resource application.
// Never linked into a Guest. One exact request/message, explicit owner credit,
// actual graph/type/QoS identity, full values, quiet interval and retirement.
#include <cstring>
#include <rclcpp/rclcpp.hpp>
#include <rcl_interfaces/srv/get_parameters.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_srvs/srv/set_bool.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <rmw/rmw.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <fast_io.h>
#include <fast_io_dsal/string.h>
#include <future>
#include <optional>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <thread>
#include <unistd.h>

namespace {
using clock_type = std::chrono::steady_clock;
using Trigger = std_srvs::srv::Trigger;
using SetBool = std_srvs::srv::SetBool;
using GetParameters = rcl_interfaces::srv::GetParameters;
void require(bool value, char const* message) {
  if (!value) throw std::runtime_error(message);
}
template<typename Bytes>
std::string hex(Bytes const& bytes) {
  static constexpr char digits[] = "0123456789abcdef";
  std::string result;
  for (auto byte : bytes) {
    auto value = static_cast<unsigned char>(byte);
    result.push_back(digits[value >> 4]);
    result.push_back(digits[value & 15]);
  }
  return result;
}
struct Witness {
  std::string mode, ns, control, resource, value, identity{"not-applicable"};
  std::string identity_kind{"none"};
  std::string history_depth_observation{"not-applicable"};
  unsigned observed_reliability{}, observed_durability{}, observed_history{};
  std::size_t observed_depth{};
  std::int64_t sequence{};
  std::uint64_t send_call_ns{};
  unsigned count{};
  bool wrong{}, matched{}, delivered{}, acknowledged{}, success{true}, client_verified{};
  rclcpp::Node::SharedPtr node;
  void mark(char const* phase) const {
    fast_io::io::println(fast_io::mnp::os_c_str(phase), "_NATIVE_GO_RESOURCE mode=", mode,
      " namespace=", ns, " gateway=go_resource_gateway control=", control,
      " resource=", resource, " count=", count, " sequence=", sequence,
      " send-call-ns=", send_call_ns,
      " identity-kind=", identity_kind, " identity=", identity,
      " value-success=", (mode == "trigger" || mode == "set-bool" || mode == "server")
        ? std::string{success ? "true" : "false"} : std::string{"not-applicable"},
      " qos-reliability=", observed_reliability, " qos-durability=", observed_durability,
      " qos-history=", observed_history, " qos-depth=", observed_depth,
      " history-depth-observation=", history_depth_observation,
      " value-hex=", value.empty() ? std::string{"none"} : hex(value));
  }
  bool gateway_alive() const {
    auto names = node->get_node_names();
    auto n = std::count(names.begin(), names.end(), ns + "/go_resource_gateway");
    require(n <= 1, "duplicate selected Gateway node identity");
    return n == 1;
  }
  bool graph(bool client, std::string const& type) const {
    if (!gateway_alive()) return false;
    auto interface = node->get_node_graph_interface();
    auto names = client
      ? interface->get_client_names_and_types_by_node("go_resource_gateway", ns)
      : interface->get_service_names_and_types_by_node("go_resource_gateway", ns);
    auto found = names.find(resource);
    return found != names.end() && found->second == std::vector<std::string>{type};
  }
  void request(rmw_request_id_t const& header) {
    require(++count == 1, "duplicate native request");
    require(header.sequence_number > 0, "nonzero native request sequence");
    sequence = header.sequence_number;
    std::array<unsigned char, 16> guid{};
    std::copy(std::begin(header.writer_guid), std::end(header.writer_guid), guid.begin());
    require(std::any_of(guid.begin(), guid.end(), [](auto b) { return b != 0; }),
            "nonzero native request writer GUID");
    identity_kind = "request-writer-guid";
    identity = hex(guid);
    auto type = mode == "parameter" ? "rcl_interfaces/srv/GetParameters"
                : mode == "trigger" ? "std_srvs/srv/Trigger" : "std_srvs/srv/SetBool";
    auto deadline = clock_type::now() + std::chrono::seconds(2);
    while (!graph(true, type) && clock_type::now() < deadline)
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    require(graph(true, type), "actual selected native client graph before response");
    client_verified = true;
    mark("REQUEST");
  }
};

int application(std::string const& mode, std::string const& ns, std::string const& control) {
  Witness w{mode, ns, control};
  w.wrong = control == "wrong-value";
  rclcpp::NodeOptions options;
  options.enable_rosout(false);
  options.start_parameter_services(false);
  options.start_parameter_event_publisher(false);
  w.node = std::make_shared<rclcpp::Node>(
    mode == "parameter" ? "sdk_parameter_node" : "go_resource_native_peer", ns, options);
  auto qos = rclcpp::QoS{rclcpp::KeepLast(4)};
  qos.reliable(); qos.durability_volatile();
  auto service_qos = rmw_qos_profile_services_default;
  service_qos.depth = 4;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher;
  rclcpp::Service<Trigger>::SharedPtr trigger;
  rclcpp::Service<SetBool>::SharedPtr set_bool;
  rclcpp::Service<GetParameters>::SharedPtr parameters;
  rclcpp::Client<Trigger>::SharedPtr client;
  std::optional<rclcpp::Client<Trigger>::FutureAndRequestId> pending;

  if (mode == "subscribe") {
    w.resource = ns + "/sub";
    w.value = w.wrong ? "unexpected native Go telemetry" : "bounded native Go telemetry";
    publisher = w.node->create_publisher<std_msgs::msg::String>(w.resource, qos);
  } else if (mode == "trigger") {
    w.resource = ns + "/trigger";
    w.value = w.wrong ? "unexpected-native-go-ready" : "native-go-ready";
    trigger = w.node->create_service<Trigger>(w.resource,
      [&w](std::shared_ptr<rmw_request_id_t> header, Trigger::Request::SharedPtr,
           Trigger::Response::SharedPtr response) {
        response->success = true; response->message = w.value; w.request(*header);
      }, service_qos);
  } else if (mode == "set-bool") {
    w.resource = ns + "/set_bool";
    w.value = "native-go-enabled"; w.success = !w.wrong;
    set_bool = w.node->create_service<SetBool>(w.resource,
      [&w](std::shared_ptr<rmw_request_id_t> header, SetBool::Request::SharedPtr request,
           SetBool::Response::SharedPtr response) {
        require(request->data, "complete native SetBool request must be true");
        response->success = w.success; response->message = w.value; w.request(*header);
      }, service_qos);
  } else if (mode == "server") {
    w.resource = ns + "/trigger"; w.value = "bounded go wasm service";
    client = w.node->create_client<Trigger>(w.resource, service_qos);
  } else {
    require(mode == "parameter", "known native lesson");
    w.resource = ns + "/sdk_parameter_node/get_parameters";
    w.value = w.wrong ? "unexpected native Go parameter" : "bounded native Go parameter";
    w.node->declare_parameter<std::string>("value", w.value);
    // An actual generated GetParameters service exposes this native node's
    // declared String. Keep automatic services disabled so every read can be
    // counted and the exact requested name checked before the response.
    parameters = w.node->create_service<GetParameters>(w.resource,
      [&w](std::shared_ptr<rmw_request_id_t> header, GetParameters::Request::SharedPtr request,
           GetParameters::Response::SharedPtr response) {
        require(request->names == std::vector<std::string>{"value"},
                "exact policy-selected native parameter name");
        auto value = w.node->get_parameter("value");
        require(value.get_type() == rclcpp::ParameterType::PARAMETER_STRING &&
                value.as_string() == w.value, "actual declared native String parameter");
        response->values.push_back(value.get_parameter_value().to_value_msg());
        w.request(*header);
      }, service_qos);
  }
  w.mark("READY");
  auto deadline = clock_type::now() + std::chrono::seconds(90);
  auto quiet = deadline;
  while (clock_type::now() < deadline) {
    rclcpp::spin_some(w.node);
    if (mode == "subscribe" && !w.matched && publisher->get_subscription_count() == 1) {
      auto endpoints = w.node->get_subscriptions_info_by_topic(w.resource);
      if (endpoints.size() == 1 && endpoints.front().node_name() == "go_resource_gateway" &&
          endpoints.front().node_namespace() == ns &&
          endpoints.front().topic_type() == "std_msgs/msg/String") {
        auto const& actual = endpoints.front().qos_profile().get_rmw_qos_profile();
        w.observed_reliability = actual.reliability;
        w.observed_durability = actual.durability;
        w.observed_history = actual.history; w.observed_depth = actual.depth;
        auto unavailable = actual.history == RMW_QOS_POLICY_HISTORY_UNKNOWN && actual.depth == 0;
        w.history_depth_observation = unavailable ? "unavailable" : "observed";
        w.mark("QOS");
        require(actual.reliability == RMW_QOS_POLICY_RELIABILITY_RELIABLE &&
                actual.durability == RMW_QOS_POLICY_DURABILITY_VOLATILE,
                "actual selected subscription QoS");
        // RTPS discovery omits history/depth in the selected Fast DDS RMW.
        // Preserve UNKNOWN/0 as unavailable, never claim an observed depth4.
        // If an RMW does expose them, require the exact selected contract.
        require(unavailable || (actual.history == RMW_QOS_POLICY_HISTORY_KEEP_LAST && actual.depth == 4),
                "observed subscription history/depth differs from selected contract");
        auto gid = endpoints.front().endpoint_gid();
        require(std::any_of(gid.begin(), gid.end(), [](auto b) { return b != 0; }),
                "actual nonzero subscription GID");
        w.identity_kind = "subscription-gid"; w.identity = hex(gid);
        w.matched = true; w.mark("MATCHED");
        std_msgs::msg::String message; message.data = w.value;
        publisher->publish(message); ++w.count; w.mark("SENT");
      }
    } else if (mode == "server") {
      if (!w.matched && client->service_is_ready() && w.graph(false, "std_srvs/srv/Trigger")) {
        w.matched = true; w.mark("MATCHED");
      }
      if (pending && !w.delivered &&
          pending->future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        auto response = pending->future.get();
        require(response->success && response->message == w.value,
                "complete actual response from Go Wasm Trigger server");
        w.delivered = true; w.mark("DELIVERED");
      }
    } else if (mode != "subscribe" && !w.matched && w.count == 1) {
      if (w.client_verified) {
        w.matched = true; w.mark("MATCHED");
        w.delivered = true; w.mark("DELIVERED");
      }
    }
    pollfd input{STDIN_FILENO, POLLIN, 0};
    require(::poll(&input, 1, 0) >= 0, "native owner credit poll");
    if (!w.acknowledged && (input.revents & (POLLIN | POLLHUP | POLLERR))) {
      char credit{}; require(::read(STDIN_FILENO, &credit, 1) == 1, "native owner credit EOF");
      if (credit == 'S') {
        require(mode == "server" && w.matched && !pending,
                "one native request only after owner observed Guest readiness and native graph");
        auto send_started = clock_type::now();
        pending.emplace(client->async_send_request(std::make_shared<Trigger::Request>()));
        w.send_call_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
          clock_type::now() - send_started).count();
        w.sequence = pending->request_id;
        require(w.sequence > 0, "actual native client request sequence");
        w.identity_kind = "client-request-sequence";
        ++w.count; w.mark("SENT");
        continue;
      }
      require(!w.acknowledged && credit == (w.wrong ? 'X' : 'A'), "exact one native owner credit");
      require(w.count == 1 && w.matched && (mode == "subscribe" || w.delivered),
              "native complete operation before owner acknowledgement");
      w.acknowledged = true; quiet = clock_type::now() + std::chrono::seconds(2); w.mark("ACK");
    }
    if (w.acknowledged && clock_type::now() >= quiet && !w.gateway_alive()) {
      require(w.count == 1, "native final single operation count");
      if (mode == "subscribe") require(publisher->get_subscription_count() == 0,
                                       "native subscriber endpoint retired");
      w.mark("PASS"); return 0;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  throw std::runtime_error("native operation/owner/quiet/retirement90s deadline");
}
} // namespace

int main(int argc, char** argv) {
  try {
    require(argc == 4, "usage: go-resource-peer <mode> <namespace> <positive|wrong-value>");
    std::string mode{argv[1]}, ns{argv[2]}, control{argv[3]};
    require(mode == "subscribe" || mode == "trigger" || mode == "set-bool" ||
            mode == "server" || mode == "parameter", "explicit supported lesson");
    require(control == "positive" || (control == "wrong-value" && mode != "server"),
            "explicit supported native value control");
    require(ns.size() > 1 && ns.front() == '/' &&
            ns.find_first_not_of("/abcdefghijklmnopqrstuvwxyz0123456789_") == std::string::npos,
            "explicit canonical fixture namespace");
    rclcpp::init(0, nullptr);
    require(std::string{rmw_get_implementation_identifier()} == "rmw_fastrtps_cpp", "selected actual RMW");
    auto status = application(mode, ns, control);
    rclcpp::shutdown(); return status;
  } catch (std::exception const& error) {
    fast_io::io::println(fast_io::err(), "FAIL_NATIVE_GO_RESOURCE ", fast_io::mnp::os_c_str(error.what()));
    if (rclcpp::ok()) rclcpp::shutdown();
    return 1;
  }
}
