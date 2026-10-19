/*************************************************************
 * UWVM2 ROS Runtime Extension. APL-2.0.                     *
 *************************************************************/
#pragma once
#include "config.h"
#include "event_stream.h"

namespace uwvm2::ros_gateway
{
    // Feedback and terminal results arrive on independent native channels.
    // Caching a successful terminal result is not Guest consumption: feedback
    // remains admissible while this exact Goal is still owned, before its
    // deadline and without an error. Result dequeue/close/revoke removes or
    // deactivates that owner and therefore prevents any later enqueue.
    inline constexpr bool feedback_callback_live(bool active,std::uint32_t error,bool before_deadline) noexcept
    {return active&&!error&&before_deadline;}
    // The sender-created native queue is immutable for a Goal's lifetime. A
    // receiver's independent permission may not silently substitute a different
    // queue, overflow mode, content bound or ROS2 endpoint QoS. Comparing exact
    // configured fields also avoids repeatedly parsing quota numbers on reads.
    inline bool action_receive_contract(ros::capability_spec const& sender,ros::capability_spec const& receiver,unsigned version)
    {
        if(receiver.operation!="action.receive-feedback"&&receiver.operation!="action.receive-result") { return false; }
        if(sender.resource!=receiver.resource||sender.type!=receiver.type||sender.type_identity!=receiver.type_identity) { return false; }
        for(auto key:{"max-event-queue-depth","max-event-queue-bytes","overflow-policy","max-feedback-bytes","max-result-bytes"})
        { if(field(sender,key)!=field(receiver,key)) { return false; } }
        if(field(sender,"overflow-policy")=="backpressure")
        { for(auto key:{"max-pending-events","max-pending-event-bytes"}) { if(field(sender,key)!=field(receiver,key)) { return false; } } }
        return version!=2||ros::equal_action_server_qos_contract(sender,receiver);
    }
    // Caller holds the owning Goal mutex. Helpers never sleep or wait for credit
    // in a native callback. Pending capacity is an explicit separate grant and
    // is exactly one; a second full push is refused, rather than blocking DDS.
    template<class Goal> inline bool feedback_fits(Goal const& goal,std::size_t bytes)
    { auto maximum{limit(goal.capability,"max-event-queue-bytes")};return goal.feedback.size()<limit(goal.capability,"max-event-queue-depth")&&goal.feedback_bytes<=maximum&&bytes<=maximum-goal.feedback_bytes; }
    template<class Goal> inline void feedback_high_water(Goal& goal) noexcept
    {
        goal.feedback_stats.high_water_depth=std::max(goal.feedback_stats.high_water_depth,std::uint64_t(goal.feedback.size()+(goal.feedback_pending?1:0)));
        goal.feedback_stats.high_water_bytes=std::max(goal.feedback_stats.high_water_bytes,std::uint64_t(goal.feedback_bytes+(goal.feedback_pending?goal.feedback_pending->capacity():0)));
    }
    template<class Goal> inline void feedback_promote(Goal& goal)
    {
        if(goal.feedback_pending&&feedback_fits(goal,goal.feedback_pending->capacity()))
        { auto size{goal.feedback_pending->capacity()};goal.feedback.push_back(std::move(*goal.feedback_pending));goal.feedback_pending.reset();goal.feedback_bytes+=size; }
    }
    template<class Goal> inline bool feedback_admit(Goal& goal,std::vector<std::uint8_t> bytes)
    {
        auto& stats{goal.feedback_stats};auto bump=[](std::uint64_t& value){if(value!=UINT64_MAX) { ++value; }};
        if(bytes.size()>limit(goal.capability,"max-feedback-bytes")) { bump(stats.rejected);return false; }
        feedback_promote(goal);auto size{bytes.capacity()};auto overflow{field(goal.capability,"overflow-policy")};
        if(!goal.feedback_pending&&feedback_fits(goal,size)) { goal.feedback.push_back(std::move(bytes));goal.feedback_bytes+=size;bump(stats.enqueued);feedback_high_water(goal);return true; }
        if(overflow=="backpressure")
        {
            bump(stats.backpressured);
            if(!goal.feedback_pending&&size<=limit(goal.capability,"max-pending-event-bytes")) { goal.feedback_pending=std::move(bytes);bump(stats.enqueued);feedback_high_water(goal);return true; }
            bump(stats.admission_refused);return false;
        }
        while(!feedback_fits(goal,size)&&overflow=="drop-oldest"&&!goal.feedback.empty()) { goal.feedback_bytes-=goal.feedback.front().capacity();goal.feedback.pop_front();bump(stats.dropped_oldest); }
        if(!feedback_fits(goal,size)) { bump(stats.admission_refused);if(overflow=="drop-newest") { bump(stats.dropped_newest); }else { bump(stats.rejected); }return false; }
        goal.feedback.push_back(std::move(bytes));goal.feedback_bytes+=size;bump(stats.enqueued);feedback_high_water(goal);return true;
    }
    template<class Goal> inline ipc::call_result feedback_take(Goal& goal)
    {
        feedback_promote(goal);if(goal.feedback.empty()) { return {11,{}}; }
        auto size{goal.feedback.front().capacity()};auto value{std::move(goal.feedback.front())};goal.feedback.pop_front();goal.feedback_bytes-=size;
        if(goal.feedback_stats.dequeued!=UINT64_MAX) { ++goal.feedback_stats.dequeued; }feedback_promote(goal);return {0,std::move(value)};
    }
    template<class Goal> inline void feedback_discard(Goal& goal) noexcept
    { goal.feedback.clear();goal.feedback_pending.reset();goal.feedback_bytes=0;if(goal.feedback_stats.closed!=UINT64_MAX) { ++goal.feedback_stats.closed; } }
    // Trusted fixture snapshot; caller holds Goal's mutex, no payload copied.
    template<class Goal> inline event_queue_statistics feedback_statistics(Goal const& goal) noexcept
    { auto value{goal.feedback_stats};value.depth=goal.feedback.size();value.bytes=goal.feedback_bytes;value.pending_depth=goal.feedback_pending?1:0;value.pending_bytes=goal.feedback_pending?goal.feedback_pending->capacity():0;return value; }
}
