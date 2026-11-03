/*************************************************************
 * UWVM2 asynchronous ROS2 Action control frames. APL-2.0.    *
 *************************************************************/
#pragma once
#include "protocol.h"

namespace uwvm2::imported::ros::ipc::action_control
{
    // Additive wire operations, not new permissions. All admission still uses
    // the original exact source capability and its per-instance quota.
    enum class command { none, ready, send, response, status, cancel, cancel_response };
    inline constexpr command parse(std::string_view name) noexcept
    {
        if(name=="action.server-is-ready") return command::ready;
        if(name=="action.send-begin") return command::send;
        if(name=="action.goal-response") return command::response;
        if(name=="action.goal-status") return command::status;
        if(name=="action.cancel-begin-own") return command::cancel;
        if(name=="action.cancel-response") return command::cancel_response;
        return command::none;
    }
    inline constexpr std::string_view capability(command cmd) noexcept
    {
        switch(cmd)
        {
        case command::ready: case command::send: case command::response: case command::status:
            return "action.send-goal";
        case command::cancel: case command::cancel_response: return "action.cancel-own-goal";
        default: return {};
        }
    }
    inline bool valid_control_request(command cmd, std::uint8_t const* data, std::size_t size) noexcept
    {
        if(cmd==command::ready) return size==0;
        if(cmd==command::none||cmd==command::send) return false; // send uses the original complete Goal validator
        return data&&size==8&&get(data,8)!=0;
    }
    inline bool uuid(std::uint8_t const* p) noexcept
    { for(unsigned i{};i<16;++i) if(p[i]) return true; return false; }
    inline bool stamp(std::uint8_t const* p) noexcept { return get(p+4,4)<1000000000ULL; }
    inline bool valid_response(command cmd, std::uint64_t requested_goal,
        std::uint8_t const* data, std::size_t size, std::uint64_t maximum_goals) noexcept
    {
        if(!data||size>max_payload_bytes) return false;
        if(cmd==command::ready) return size==1&&data[0]<=1;
        if(cmd==command::send) return size==8&&get(data,8)!=0;
        if(!requested_goal) return false;
        if(cmd==command::cancel) return size==8&&get(data,8)==requested_goal;
        if(size<9||data[0]!=1||get(data+1,8)!=requested_goal) return false;
        if(cmd==command::response)
        {
            if(size==10) return data[9]==0;
            return size==35&&data[9]==1&&uuid(data+10)&&stamp(data+26)&&data[34]<=6;
        }
        if(cmd==command::status) return size==34&&uuid(data+9)&&stamp(data+25)&&data[33]<=6;
        if(cmd==command::cancel_response)
        {
            if(size<14) return false;
            auto count{get(data+10,4)};
            if(count>maximum_goals||count>(max_payload_bytes-14)/24||size!=14+24*count) return false;
            // Preserve the actual signed int8 return code and complete GoalInfo
            // list; an ACK is not a terminal goal/result or a quota release.
            for(std::size_t i{};i<count;++i) if(!stamp(data+14+24*i+16)) return false;
            return true;
        }
        return false;
    }
}
