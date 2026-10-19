/** C++ complete typed Spot bridge values; one shared checked C transport path. */
#pragma once
#include <cstdint>
#include "sdk.h"
#include "bounded_records.hpp"
namespace uwvm_mobile::spot {
using StateRequest=uwvm_custom::Ros2Uwvm_spot_bridgeSrvState_Request;
using StateResponse=uwvm_custom::Ros2Uwvm_spot_bridgeSrvState_Response;
using LeaseRequest=uwvm_custom::Ros2Uwvm_spot_bridgeSrvLease_Request;
using LeaseResponse=uwvm_custom::Ros2Uwvm_spot_bridgeSrvLease_Response;
using CommandRequest=uwvm_custom::Ros2Uwvm_spot_bridgeSrvCommand_Request;
using CommandResponse=uwvm_custom::Ros2Uwvm_spot_bridgeSrvCommand_Response;
using VelocityRequest=uwvm_custom::Ros2Uwvm_spot_bridgeSrvVelocity_Request;
using VelocityResponse=uwvm_custom::Ros2Uwvm_spot_bridgeSrvVelocity_Response;
using FeedbackRequest=uwvm_custom::Ros2Uwvm_spot_bridgeSrvFeedback_Request;
using FeedbackResponse=uwvm_custom::Ros2Uwvm_spot_bridgeSrvFeedback_Response;
class Client {
    mm_spot_buffers buffers_{};
public:
    Client()=default;Client(Client const&)=delete;Client& operator=(Client const&)=delete;
    roswasmc_result state(roswasmc_capability const& owner,StateRequest const& request,StateResponse& response) noexcept {
        return mm_spot_State_call(&owner,&request,&response,&buffers_);
    }
    roswasmc_result lease(roswasmc_capability const& owner,LeaseRequest const& request,LeaseResponse& response) noexcept {
        return mm_spot_Lease_call(&owner,&request,&response,&buffers_);
    }
    roswasmc_result command(roswasmc_capability const& owner,CommandRequest const& request,CommandResponse& response) noexcept {
        return mm_spot_Command_call(&owner,&request,&response,&buffers_);
    }
    roswasmc_result velocity(roswasmc_capability const& owner,VelocityRequest const& request,VelocityResponse& response) noexcept {
        return mm_spot_Velocity_call(&owner,&request,&response,&buffers_);
    }
    roswasmc_result feedback(roswasmc_capability const& owner,FeedbackRequest const& request,FeedbackResponse& response) noexcept {
        return mm_spot_Feedback_call(&owner,&request,&response,&buffers_);
    }
};
}
