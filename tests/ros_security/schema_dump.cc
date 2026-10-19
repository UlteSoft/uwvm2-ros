#include <uwvm2/imported/ros/policy/policy.h>
#include <fast_io.h>
#include <fast_io_unit/string.h>
#include <string_view>
using namespace uwvm2::imported::ros;
int main() {
    for(auto v: {version::ros1,version::ros2}) {
        for(auto op: {"topic.publish","topic.subscribe","service.call","service.advertise","service.respond","param.get","param.set","param.delete","param.exists","param.search","param.list","param.declare","param.undeclare","param.describe","param.subscribe-events","graph.list-topics","graph.list-nodes","graph.list-services","graph.inspect-topic","graph.inspect-node","graph.inspect-service","graph.inspect-action","action.send-goal","action.cancel-own-goal","action.cancel-any-goal","action.receive-feedback","action.receive-result","action.advertise","action.respond","tf.lookup","tf.broadcast","time.read-ros","time.read-wall","time.read-steady","time.read-system","log.write","lifecycle.get-state","lifecycle.change-state"}) {
            auto fields=operation_schema(v,op); if(fields.empty()) continue;
            auto line{fast_io::concat_std(to_string(v),'\t',std::string_view{op},'\t')};
            fast_io::ostring_ref_std output{&line};
            for(std::size_t i{};i!=fields.size();++i) {if(i) fast_io::io::print(output,','); fast_io::io::print(output,fields[i]);}
            bool supported{};
            for(auto type: {"std_msgs/String","std_msgs/msg/String","std_srvs/Trigger","std_srvs/srv/Trigger","builtin/time-v1","builtin/log-v1","builtin/string-v1","builtin/graph-list-v1","geometry_msgs/TransformStamped","geometry_msgs/msg/TransformStamped","rcl_interfaces/msg/ParameterEvent","lifecycle_msgs/srv/GetState","lifecycle_msgs/srv/ChangeState","actionlib_tutorials/Fibonacci","action_tutorials_interfaces/action/Fibonacci"})
            { supported = supported || backend_supported(v,op,type); }
            fast_io::io::println(output,'\t',std::string_view{supported ? "implemented" : "unsupported"});
            fast_io::io::print(line);
        }
    }
}
