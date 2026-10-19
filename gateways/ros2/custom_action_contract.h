/** Complete typed Action admission without creating a native endpoint.
 * Native lifecycle availability still requires the separate actual SDK probes
 * and UWVM_ROS2_HAVE_CUSTOM_ACTION. This contract grants no authority. */
#pragma once
#include "custom_topics.h"
#if __has_include(<rosidl_typesupport_cpp/action_type_support.hpp>)
#include <rosidl_typesupport_cpp/action_type_support.hpp>
namespace uwvm2::ros_gateway::ros2_custom_actions
{
    template<class Native,class GoalCodec,class ResultCodec,class FeedbackCodec>
    bool qualified(ros::capability_spec const& cap)
    {
        static_assert(std::is_same<typename Native::Goal,typename GoalCodec::Native>::value&&
            std::is_same<typename Native::Result,typename ResultCodec::Native>::value&&
            std::is_same<typename Native::Feedback,typename FeedbackCodec::Native>::value,
            "Action codecs must cover official components");
        auto item=ipc::find_custom_action(2,cap.type);
        auto selected=cap.limits.find("adapter-codec-identity");
        auto const& operation=cap.operation;
        bool allowed=operation=="action.send-goal"||operation=="action.receive-feedback"||operation=="action.receive-result"||
            operation=="action.cancel-own-goal"||operation=="action.cancel-any-goal"||operation=="action.advertise"||operation=="action.respond";
        if(!item||!allowed||cap.type_identity!=item->native_identity||selected==cap.limits.end()||
           selected->second!=item->codec_identity)return false;
        if(ipc::complete_vendor_codec<GoalCodec>::value!=ipc::complete_vendor_codec<ResultCodec>::value||
           ipc::complete_vendor_codec<GoalCodec>::value!=ipc::complete_vendor_codec<FeedbackCodec>::value)return false;
#define UWVM_ACTION_CONTRACT_COMPONENT(Role,Codec) \
        if(item->Role->complete_vendor!=ipc::complete_vendor_codec<Codec>::value||item->Role->type!=Codec::type|| \
           item->Role->native_identity!=Codec::native_identity||item->Role->codec_identity!=Codec::codec_identity|| \
           item->Role->wire_bytes!=Codec::wire_bytes||!ros2_custom_topics::binding<typename Codec::Native,Codec>::native_contract())return false;
        UWVM_ACTION_CONTRACT_COMPONENT(goal,GoalCodec)
        UWVM_ACTION_CONTRACT_COMPONENT(result,ResultCodec)
        UWVM_ACTION_CONTRACT_COMPONENT(feedback,FeedbackCodec)
#undef UWVM_ACTION_CONTRACT_COMPONENT
        auto support=::rosidl_typesupport_cpp::get_action_type_support_handle<Native>();
        return ros2_custom_topics::native_interface_contract<Native,ipc::complete_vendor_codec<GoalCodec>::value>(support,cap.type,item->native_identity);
    }
}
#endif
