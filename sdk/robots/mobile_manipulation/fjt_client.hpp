/** C++20 complete typed FJT adapter, reusing the one actual C codec/ABI path. */
#pragma once
#include <cstdint>
#include "fjt_client.h"
#include "bounded_records.hpp"
namespace uwvm_mobile {
using Goal=uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal;
using Feedback=uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Feedback;
using Result=uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Result;
class TrajectoryClient {
    mm_fjt_client client_{};
public:
    TrajectoryClient()=default;
    TrajectoryClient(TrajectoryClient const&)=delete;
    TrajectoryClient& operator=(TrajectoryClient const&)=delete;
    std::uint32_t bind(roswasmc_capability const& send,roswasmc_capability const& feedback,
        roswasmc_capability const& result,roswasmc_capability const& cancel) noexcept {
        if(client_.used||client_.closed)return 17;
        return mm_fjt_init(&client_,&send,&feedback,&result,&cancel);
    }
    std::uint32_t send(Goal const& value,std::uint32_t& history_index) noexcept {
        return mm_fjt_send(&client_,&value,&history_index);
    }
    std::uint32_t feedback(std::uint32_t index,Feedback& value) noexcept {
        return mm_fjt_feedback_once(&client_,index,&value);
    }
    std::uint32_t result(std::uint32_t index,std::uint8_t& status,Result& value) noexcept {
        return mm_fjt_result_once(&client_,index,&status,&value);
    }
    std::uint32_t cancel(std::uint32_t index,int& accepted) noexcept {
        return mm_fjt_cancel(&client_,index,&accepted);
    }
    std::uint32_t retire_local() noexcept {return mm_fjt_retire_local(&client_);}
    mm_fjt_client const& diagnostics() const noexcept {return client_;}
};
}
