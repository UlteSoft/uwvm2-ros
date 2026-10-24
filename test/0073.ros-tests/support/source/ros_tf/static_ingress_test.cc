#include "common.h"
#include "../../../../../src/ros/gateway/common/tf_state.h"
#include <cstring>

namespace
{
    unsigned checks{};
    void check(bool value,char const* message)
    { ++checks;ros_probe::require(value,message); }
    bool put(uwvm2::ros_gateway::tf_ingress& state,ros_probe::ipc::transform_data const& value)
    { return state.admit(value)&&state.retain(value); }
}
int main()
{
    try
    {
        using namespace ros_probe;
        // Permit the external collector to attest this short-lived ELF before
        // any token buckets or assertions are initialized.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        for(auto version:{core::version::ros1,core::version::ros2})
        {
            auto value{ros_tf_probe::transform()};value.seconds=0;value.nanoseconds=0;
            auto size{ipc::encode_transform(value).size()};
            for(auto overflow:{"reject","drop-newest","drop-oldest"})
            {
                auto cap{ros_tf_probe::capability(version,"static","tf.lookup")};cap.resource="/tf_static";
                cap.limits[version==core::version::ros1?"queue-depth":"qos.depth"]="1";
                cap.limits["overflow-policy"]=overflow;cap.limits["max-message-bytes"]=std::to_string(size);
                cap.limits["max-queue-bytes"]=std::to_string(size);cap.limits["burst"]="100";
                cap.limits["max-rate-hz"]="1000";cap.limits["max-bytes-per-second"]="1048576";
                gateway::tf_ingress state{cap};
                check(put(state,value),"first static sample must be admitted");
                auto changed{value};changed.values[0]=2.5;
                check(put(state,changed),"same-stamp static replacement must not consume another cache slot");
                check(state.history.size()==1&&state.history_bytes==size&&state.history.front().values[0]==2.5,"static replacement retains one latest sample and exact bytes");
                check(!put(state,changed),"exact same static sample must be ignored");
                changed.seconds=123;changed.values[0]=3.75;
                check(put(state,changed)&&state.history.size()==1&&state.history_bytes==size,"new-stamp static replacement remains one cache slot");
                changed.values[0]=-0.;check(put(state,changed),"static negative zero replacement");
                changed.values[0]=0.;check(put(state,changed),"static different signed-zero wire must replace");
                auto bad{changed};bad.values[0]=11.;
                check(!put(state,bad)&&state.history.front().values[0]==0.,"static replacement cannot bypass translation permission");
                bad=changed;bad.parent="secret";
                check(!put(state,bad)&&state.history.front().parent=="map","static replacement cannot bypass exact frame permission");

                cap.resource="/tf";gateway::tf_ingress dynamic{cap};
                check(put(dynamic,value),"first dynamic sample must be admitted");
                changed=value;changed.values[0]=2.5;
                check(!put(dynamic,changed)&&dynamic.history.front().values[0]==1.25,"dynamic same-stamp replacement remains rejected");
                changed.seconds=123;
                bool replaced{put(dynamic,changed)};
                check(replaced==(std::string{overflow}=="drop-oldest"),"dynamic depth1 overflow policy remains effective");
                check(dynamic.history.size()==1&&dynamic.history_bytes==size&&dynamic.history.front().values[0]==(replaced?2.5:1.25),"dynamic exact cache bytes after overflow");
            }
            auto cap{ros_tf_probe::capability(version,"bounded","tf.lookup")};cap.resource="/tf_static";
            cap.limits["burst"]="100";cap.limits["max-rate-hz"]="1000";cap.limits["max-bytes-per-second"]="1048576";
            cap.limits["max-message-bytes"]=std::to_string(size-1);
            gateway::tf_ingress message_limited{cap};
            check(!put(message_limited,value)&&message_limited.history.empty()&&!message_limited.history_bytes,"static native wire must obey message-byte grant");
            cap.limits["max-message-bytes"]=std::to_string(size);cap.limits["max-queue-bytes"]=std::to_string(size-1);
            gateway::tf_ingress queue_limited{cap};
            check(!put(queue_limited,value)&&queue_limited.history.empty()&&!queue_limited.history_bytes,"static native wire must obey queue-byte grant");
            cap.limits["max-queue-bytes"]=std::to_string(size);cap.limits["burst"]="1";cap.limits["max-rate-hz"]="0.001";
            gateway::tf_ingress rate_limited{cap};auto changed{value};changed.values[0]=2.5;
            check(put(rate_limited,value)&&!put(rate_limited,changed)&&rate_limited.history.front().values[0]==1.25,"static replacement must pass rate admission");
            cap.limits["burst"]="100";cap.limits["max-bytes-per-second"]=std::to_string(size);
            gateway::tf_ingress byte_rate_limited{cap};
            check(put(byte_rate_limited,value)&&!put(byte_rate_limited,changed)&&byte_rate_limited.history.front().values[0]==1.25,"static replacement must pass byte-rate admission");
        }
        std::cout<<"PASS static/dynamic ROS1/ROS2 cache semantics, exact duplicate/signed-zero, three overflow policies, frame/bounds/message/queue/rate/byte-rate checks="<<checks<<'\n';
        return 0;
    }
    catch(std::exception const& error) { std::cerr<<error.what()<<'\n';return 1; }
}
