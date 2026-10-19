#pragma once
#include "server.h"
#include "nonreusing_ids.h"
#include <condition_variable>

namespace uwvm2::ros_gateway
{
    // All sessions/services share this process-lifetime correlation domain.
    // A completed, expired or queue-rejected request must not consume a retained
    // tombstone, and its old ID must never name a later live request.
    inline nonreusing_id_allocator service_identifiers;
    inline std::uint64_t new_service_id()
    { return service_identifiers.allocate(ipc::random_session); }
    struct pending_service
    {
        std::uint64_t id{};
        std::chrono::steady_clock::time_point deadline;
        ipc::deadline answer_deadline{};
        std::condition_variable ready;
        bool answered{},cancelled{};
        std::vector<std::uint8_t> response;
        std::function<std::uint32_t(std::vector<std::uint8_t> const&,ipc::deadline)> deliver;
    };
    struct service_state
    {
        ros::capability_spec capability;
        std::shared_ptr<event_queue> events;
        std::mutex mutex;
        std::map<std::uint64_t,std::shared_ptr<pending_service>> pending;
        bool active{true};
        service_state(ros::capability_spec c,std::shared_ptr<event_queue> q):capability{std::move(c)},events{std::move(q)} {}
        std::shared_ptr<pending_service> begin(std::vector<std::uint8_t> body,std::function<std::uint32_t(std::vector<std::uint8_t> const&,ipc::deadline)> deliver={})
        {
            auto descriptor{ipc::find_custom_service(1,capability.type)};
            if(!descriptor) { descriptor=ipc::find_custom_service(2,capability.type); }
            // A native request is the generated request component, not the
            // empty command that originally advertised this server. Validate
            // before allocating a ticket or copying it into the event queue.
            bool valid{descriptor ? capability.type_identity==descriptor->native_identity&&
                field(capability,"adapter-codec-identity")==descriptor->codec_identity&&
                ipc::validate_custom_topic(*descriptor->request,body.data(),body.size()) : validate_payload(capability,body)};
            if(body.size()>limit(capability,"max-request-bytes")||!valid) { return {}; }
            std::unique_lock<std::mutex> lock{mutex}; auto now{std::chrono::steady_clock::now()};
            for(auto i=pending.begin();i!=pending.end();)
            { if(i->second->deadline<=now) { i->second->cancelled=true;i->second->ready.notify_all();i=pending.erase(i); } else { ++i; } }
            if(!active||pending.size()>=limit(capability,"max-inflight")) { return {}; }
            auto id{new_service_id()};if(!id){return {};}
            auto p{std::make_shared<pending_service>()}; p->id=id; p->deadline=now+std::chrono::milliseconds(limit(capability,"timeout-ms"));p->deliver=std::move(deliver);
            pending.emplace(p->id,p);lock.unlock();
            std::vector<std::uint8_t> event;event.reserve(8+body.size());ipc::put(event,p->id,8);event.insert(event.end(),body.begin(),body.end());
            if(!events->enqueue(std::move(event))) { std::lock_guard<std::mutex> guard{mutex};pending.erase(p->id);return {}; }
            return p;
        }
        ipc::call_result answer(ros::capability_spec const& cap,std::vector<std::uint8_t> const& data,ipc::deadline until)
        {
            if(cap.resource!=capability.resource||cap.type!=capability.type||cap.type_identity!=capability.type_identity) { return {13,{}}; }
            auto descriptor{ipc::find_custom_service(1,cap.type)};
            if(!descriptor) { descriptor=ipc::find_custom_service(2,cap.type); }
            if(descriptor)
            {
                if(field(cap,"adapter-codec-identity")!=descriptor->codec_identity||
                   field(capability,"adapter-codec-identity")!=descriptor->codec_identity)
                { return {13,{}}; }
                if(!ipc::validate_custom_service_request(*descriptor,"service.respond",data.data(),data.size())) { return {22,{}}; }
            }
            else if(data.size()<13||!ipc::get(data.data(),8)||data[8]>1||ipc::get(data.data()+9,4)!=data.size()-13||!valid_utf8(data.data()+13,data.size()-13)) { return {22,{}}; }
            if(data.size()-8>limit(cap,"max-response-bytes")||data.size()-8>limit(capability,"max-response-bytes")) { return {90,{}}; }
            std::unique_lock<std::mutex> lock{mutex};auto i{pending.find(ipc::get(data.data(),8))};
            if(!active||i==pending.end()) { return {9,{}}; }auto p{i->second};
            auto expire=[&] { p->cancelled=true;p->ready.notify_all();pending.erase(i); };
            if(p->answered) { pending.erase(i);return {110,{}}; }
            if(p->cancelled||p->deadline<=std::chrono::steady_clock::now()) { expire();return {110,{}}; }
            if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }
            // Preparing a wire response consumes the call budget. Do not
            // publish an answered state while a copy can still cross either
            // deadline, and allow close/expiry to retire the request meanwhile.
            lock.unlock();std::vector<std::uint8_t> response{data.begin()+8,data.end()};lock.lock();
            i=pending.find(p->id);
            if(!active||i==pending.end()||i->second!=p) { return {9,{}}; }
            if(p->answered) { pending.erase(i);return {110,{}}; }
            if(p->cancelled||p->deadline<=std::chrono::steady_clock::now()) { expire();return {110,{}}; }
            auto delivery_until{std::min(until,p->deadline)};
            if(std::chrono::steady_clock::now()>=delivery_until) { return {110,{}}; }
            if(p->deliver)
            {
                // Native preparation rechecks this absolute deadline before
                // sending. The state lock orders admission with close and a
                // second response; a short call expiry leaves a live request
                // available for retry rather than consuming its identifier.
                auto error{p->deliver(response,delivery_until)};
                if(error) { if(p->deadline<=std::chrono::steady_clock::now()) { expire(); }return {error,{}}; }
                p->answer_deadline=delivery_until;p->answered=true;pending.erase(i);
            }
            else { p->response.swap(response);p->answer_deadline=delivery_until;p->answered=true;p->ready.notify_all(); }
            return std::chrono::steady_clock::now()>=until?ipc::call_result{110,{}}:ipc::call_result{};
        }
        bool wait(std::shared_ptr<pending_service> const& p,std::vector<std::uint8_t>& output)
        {
            std::unique_lock<std::mutex> lock{mutex};p->ready.wait_until(lock,p->deadline,[&]{return p->answered||p->cancelled||!active;});pending.erase(p->id);
            if(p->cancelled||!p->answered||std::chrono::steady_clock::now()>=std::min(p->deadline,p->answer_deadline)) { return false; }output=std::move(p->response);return true;
        }
        void deactivate()
        { std::lock_guard<std::mutex> lock{mutex};active=false;for(auto const& p:pending){if(!p.second->answered) { p.second->cancelled=true; }p.second->ready.notify_all();}pending.clear();events->deactivate(); }
    };
}
