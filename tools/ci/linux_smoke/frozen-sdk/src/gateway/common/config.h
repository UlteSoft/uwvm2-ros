/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include <uwvm2/imported/ros/common/types.h>
#include <uwvm2/imported/ros/common/action_qos_contract.h>
#include <uwvm2/imported/ros/common/service_availability_history.h>
#include <uwvm2/imported/ros/common/parameter_search_contract.h>
#include <uwvm2/imported/ros/ipc/unix.h>
#include <uwvm2/imported/ros/ipc/parameter.h>
#include <uwvm2/imported/ros/ipc/custom_type.h>
#include <uwvm2/imported/ros/ipc/event.h>
#include <fstream>
#include "native_io.h"
#include <set>
#include <cmath>
#include <memory>
#include <initializer_list>
#include <algorithm>

namespace uwvm2::ros_gateway
{
    namespace ros=uwvm2::imported::ros;
    namespace ipc=ros::ipc;
    inline bool wire_size_fits(std::uint64_t maximum,std::initializer_list<std::uint64_t> parts) noexcept
    { for(auto size:parts) { if(size>maximum) { return false; }maximum-=size; }return true; }
    inline bool valid_utf8(std::uint8_t const* bytes,std::size_t size) noexcept
    {
        std::size_t p{};
        while(p<size)
        {
            auto lead{bytes[p++]};if(lead<128) { continue; }
            unsigned count{};std::uint32_t cp{},minimum{};
            if(lead>=0xc2&&lead<=0xdf) { count=1;cp=lead&31;minimum=128; }
            else if(lead>=0xe0&&lead<=0xef) { count=2;cp=lead&15;minimum=2048; }
            else if(lead>=0xf0&&lead<=0xf4) { count=3;cp=lead&7;minimum=65536; }
            else { return false; }
            if(count>size-p) { return false; }
            while(count--) { auto byte{bytes[p++]};if((byte&0xc0)!=0x80) { return false; }cp=(cp<<6)|(byte&63); }
            if(cp<minimum||cp>0x10ffff||(cp>=0xd800&&cp<=0xdfff)) { return false; }
        }
        return true;
    }
    struct configuration
    {
        ros::target_policy policy{};
        std::uint32_t worker_uid{}, worker_pid{}, timeout_ms{}, max_sessions{}, audit_records_per_second{};
        std::uint64_t audit_max_records{},audit_max_bytes{};
        std::string socket{};
    };
    inline std::uint64_t integer(std::string_view s,std::uint64_t maximum=std::numeric_limits<std::uint32_t>::max())
    {
        if(s.empty()||s.front()<'0'||s.front()>'9') { throw std::runtime_error("empty or invalid integer"); }
        std::uint64_t n{};
#if defined(__cpp_concepts)
        auto const [end,error]{::fast_io::parse_by_scan(s.data(),s.data()+s.size(),n)};
        if(error!=::fast_io::parse_code::ok||end!=s.data()+s.size()) {throw std::runtime_error("integer range");}
#else
        auto const [end,error]{std::from_chars(s.data(),s.data()+s.size(),n)};
        if(error!=std::errc{}||end!=s.data()+s.size()) {throw std::runtime_error("integer range");}
#endif
        if(n>maximum) { throw std::runtime_error("integer range"); } return n;
    }
    inline double positive(std::string const& s)
    {
        double v{};
#if defined(__cpp_concepts)
        auto const [end,error]{::fast_io::parse_by_scan(s.data(),s.data()+s.size(),v)};
        if(error!=::fast_io::parse_code::ok||end!=s.data()+s.size()) {throw std::runtime_error("finite positive rate required");}
#else
        // GCC9's C++17 standard library lacks the complete floating from_chars
        // implementation. Keep its strict locale-independent compatibility
        // path shared with canonical parameters rather than using stod.
        if(!ipc::parse_parameter_real(s,v)) {throw std::runtime_error("finite positive rate required");}
#endif
        if(!std::isfinite(v)||v<=0||v>1000000) { throw std::runtime_error("finite positive rate required"); } return v;
    }
    inline bool canonical(std::string_view s,bool allow_root=false)
    {
        if(allow_root&&s=="/") { return true; }
        if(s.size()<2||s.size()>1024||s.front()!='/'||s.back()=='/') { return false; }
        bool first{true};
        for(std::size_t i=1;i<s.size();++i)
        {
            char c{s[i]};
            if(c=='/') { if(first) { return false; } first=true; }
            else { bool alpha{(c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_'}; if(!alpha&&!(c>='0'&&c<='9'&&!first)) { return false; } first=false; }
        }
        return !first;
    }
    inline std::uint64_t limit(ros::capability_spec const& c,std::string_view key)
    {
        auto i{c.limits.find(key)};
        if(i==c.limits.end()) { throw std::runtime_error("missing explicit limit: "+std::string{key}); }
        // This field is nanoseconds, not a byte quota. Preserve the original
        // ceiling for every other key, and apply the protocol's actual finite
        // wait ceiling before the operation-specific timeout/headroom checks.
        auto maximum{key=="max-wait-ns" ? ipc::client_availability::maximum_wait_ns : 1073741824ULL};
        return integer(i->second,maximum);
    }
    inline std::string const& field(ros::capability_spec const& c,std::string_view key)
    { auto i{c.limits.find(key)}; if(i==c.limits.end()) { throw std::runtime_error("missing explicit field: "+std::string{key}); } return i->second; }
    inline std::vector<std::string> schema(ros::version v,std::string const& operation,std::string_view type={})
    {
        std::vector<std::string> f;
        if(operation=="event.observe")
        { f={"event-source-alias","event-kinds","max-event-bytes","max-event-queue-depth","max-event-queue-bytes","max-inflight","max-rate-hz","burst","max-bytes-per-second","overflow-policy","audit"}; }
        else if(operation=="topic.publish"||operation=="topic.subscribe")
        {
            f={"max-message-bytes","max-rate-hz","burst","max-bytes-per-second","max-queue-bytes","overflow-policy","validator","audit",operation=="topic.publish"?"max-publishers":"max-subscribers"};
            if(v==ros::version::ros1) { f.insert(f.end(),{"queue-depth","transport"}); if(operation=="topic.publish") { f.push_back("latch"); } }
            else { f.insert(f.end(),{"max-inflight","qos.reliability","qos.durability","qos.history","qos.depth","qos.deadline","qos.lifespan","qos.liveliness","qos.liveliness-lease-duration"}); }
        }
        else if(operation=="service.available")
        {
            f={"service-source-alias","timeout-ms","max-wait-ns","max-native-clients","max-binding-history","max-inflight","max-request-bytes","max-response-bytes","max-rate-hz","burst","max-bytes-per-second","audit"};
            if(v==ros::version::ros2) { f.insert(f.end(),{"qos.reliability","qos.durability","qos.history","qos.depth","qos.deadline","qos.lifespan","qos.liveliness","qos.liveliness-lease-duration"}); }
        }
        else if(operation=="service.call"||operation=="service.advertise"||operation=="service.respond")
        {
            f={"timeout-ms","max-inflight","max-request-bytes","max-response-bytes","max-rate-hz","burst","max-bytes-per-second","validator","audit"};
            if(operation=="service.advertise") { f.insert(f.end(),{"max-services","max-event-queue-depth","max-event-queue-bytes","overflow-policy"}); }
            if(v==ros::version::ros2) { f.insert(f.end(),{"qos.reliability","qos.durability","qos.history","qos.depth","qos.deadline","qos.lifespan","qos.liveliness","qos.liveliness-lease-duration"}); }
        }
        else if(operation.rfind("time.",0)==0) { f={"max-rate-hz","burst","max-inflight","max-response-bytes","max-bytes-per-second","audit"}; }
        else if(operation=="log.write") { f={"max-message-bytes","max-log-bytes-per-second","max-bytes-per-second","max-inflight","max-rate-hz","burst","severity","audit"}; }
        else if(operation.rfind("param.",0)==0)
        {
            f={"max-parameter-bytes","timeout-ms","max-inflight","max-rate-hz","burst","max-bytes-per-second","validator","audit"};
            if(operation=="param.list"||operation=="param.search") { f.push_back("allowed-name-prefix"); }
            if(v==ros::version::ros2) { f.insert(f.end(),{"parameter-node","parameter-name"}); if(operation=="param.list") { f.push_back("list-depth"); } }
            if(operation=="param.subscribe-events") { f.insert(f.end(),{"max-subscribers","max-event-fields","max-event-queue-depth","max-event-queue-bytes","overflow-policy","qos.reliability","qos.durability","qos.history","qos.depth","qos.deadline","qos.lifespan","qos.liveliness","qos.liveliness-lease-duration"}); }
            else if(v==ros::version::ros2&&operation!="param.declare"&&operation!="param.undeclare")
            { f.insert(f.end(),{"qos.reliability","qos.durability","qos.history","qos.depth","qos.deadline","qos.lifespan","qos.liveliness","qos.liveliness-lease-duration"}); }
        }
        else if(operation.rfind("graph.",0)==0) { f={"allowed-name-prefix","max-graph-result-bytes","timeout-ms","max-inflight","max-rate-hz","burst","max-bytes-per-second","audit"}; }
        else if(operation.rfind("action.",0)==0)
        { f={"fibonacci.max-order","max-active-goals","max-actions","timeout-ms","max-goal-bytes","max-feedback-bytes","max-result-bytes","cancel-scope","max-rate-hz","burst","max-bytes-per-second","max-event-queue-depth","max-event-queue-bytes","overflow-policy","validator","audit"};if(v==ros::version::ros2){for(auto channel:{"goal-service","cancel-service","result-service","feedback","status"}){for(auto property:{"reliability","durability","history","depth","deadline","lifespan","liveliness","liveliness-lease-duration"}){f.push_back(std::string{"qos."}+channel+"."+property);}}} }
        else if(v==ros::version::ros2&&(operation=="lifecycle.get-state"||operation=="lifecycle.change-state"))
        { f={"timeout-ms","max-inflight","max-request-bytes","max-response-bytes","max-rate-hz","burst","max-bytes-per-second","audit","qos.reliability","qos.durability","qos.history","qos.depth","qos.deadline","qos.lifespan","qos.liveliness","qos.liveliness-lease-duration"};if(operation=="lifecycle.change-state") { f.push_back("allowed-transition"); } }
        else if(operation=="tf.lookup"||operation=="tf.broadcast")
        {
            f={"source-frame","target-frame","tf.max-translation-abs","tf.quaternion-norm-tolerance","timeout-ms","max-message-bytes","max-rate-hz","burst","max-bytes-per-second","max-inflight","max-queue-bytes","overflow-policy","buffer-cache-ms","validator","audit"};
            if(v==ros::version::ros1) { f.insert(f.end(),{"queue-depth","transport"}); if(operation=="tf.broadcast") { f.push_back("latch"); } }
            else { f.insert(f.end(),{"qos.reliability","qos.durability","qos.history","qos.depth","qos.deadline","qos.lifespan","qos.liveliness","qos.liveliness-lease-duration"}); }
        }
        else { throw std::runtime_error("UNSUPPORTED gateway operation: "+operation); }
        if(ipc::find_custom_action(static_cast<unsigned>(v),type)&&operation.rfind("action.",0)==0)
        { f.erase(std::remove(f.begin(),f.end(),"fibonacci.max-order"),f.end()); }
        if(ipc::find_custom_topic_for_operation(static_cast<unsigned>(v),type,operation)||ipc::find_custom_service(static_cast<unsigned>(v),type)||ipc::find_custom_action(static_cast<unsigned>(v),type))
        { f.push_back("adapter-codec-identity"); }
        f.insert(f.end(),{"audit-max-rate-hz","audit-burst","audit-max-records","audit-max-bytes"});
        return f;
    }
    inline void validate(ros::version v,ros::capability_spec const& c)
    {
        if(!ipc::valid_token(c.alias)||!canonical(c.resource,c.operation.rfind("graph.",0)==0||c.operation=="param.list")) { throw std::runtime_error("invalid alias or canonical resource"); }
        auto f{schema(v,c.operation,c.type)}; std::set<std::string> allowed{f.begin(),f.end()};
        auto custom{ipc::find_custom_topic_for_operation(static_cast<unsigned>(v),c.type,c.operation)};
        auto custom_service{ipc::find_custom_service(static_cast<unsigned>(v),c.type)};
        auto custom_action{ipc::find_custom_action(static_cast<unsigned>(v),c.type)};
        bool backpressure{c.limits.count("overflow-policy")&&field(c,"overflow-policy")=="backpressure"};
        if(backpressure)
        {
            allowed.insert("max-pending-events");allowed.insert("max-pending-event-bytes");
            if(limit(c,"max-pending-events")!=1) { throw std::runtime_error("backpressure requires exactly one explicit pending event"); }
            auto key{c.operation=="event.observe"?"max-event-bytes":c.operation=="param.subscribe-events"?"max-parameter-bytes":
                     c.operation=="service.advertise"?"max-request-bytes":c.operation.rfind("action.",0)==0?"max-result-bytes":"max-message-bytes"};
            auto record{limit(c,key)};
            if(c.operation=="service.advertise") { record+=8; }
            if(c.operation.rfind("action.",0)==0) { record=std::max({record,limit(c,"max-feedback-bytes"),limit(c,"max-goal-bytes")+8}); }
            if(limit(c,"max-pending-event-bytes")<record) { throw std::runtime_error("pending capacity is smaller than its explicit maximum record"); }
        }
        if(custom)
        {
            if(c.operation!="topic.publish"&&c.operation!="topic.subscribe")
            { throw std::runtime_error("custom fixed adapter supports Topic messages only"); }
            allowed.insert("adapter-codec-identity");
            if(c.type_identity!=custom->native_identity||field(c,"adapter-codec-identity")!=custom->codec_identity)
            { throw std::runtime_error("custom Topic native/codec identity differs from compiled registry"); }
        }
        if(custom_service||custom_action)
        {
            auto native_identity{custom_service?custom_service->native_identity:custom_action->native_identity};
            auto codec_identity{custom_service?custom_service->codec_identity:custom_action->codec_identity};
            bool operation{custom_service?(c.operation=="service.call"||c.operation=="service.advertise"||c.operation=="service.respond"||c.operation=="service.available"):
                (c.operation=="action.send-goal"||c.operation=="action.cancel-own-goal"||c.operation=="action.cancel-any-goal"||c.operation=="action.receive-feedback"||c.operation=="action.receive-result"||c.operation=="action.advertise"||c.operation=="action.respond")};
            if(!operation||c.type_identity!=native_identity||field(c,"adapter-codec-identity")!=codec_identity)
            { throw std::runtime_error("custom interface operation/native/codec contract differs from compiled registry"); }
        }
        // This additive authority is optional: an older exact-name grant
        // remains exact, including when it has a broad result prefix.
        if(v==ros::version::ros1&&c.operation=="param.search") { allowed.insert("search-parent-namespaces"); }
        if((ipc::scalar_parameter_operation(v,c.operation)&&ipc::array_parameter(ipc::parameter_type(v,c.type)))||
           (c.operation=="param.subscribe-events"&&ipc::array_parameter(ipc::parameter_event_type(v,c.type))))
        { allowed.insert("max-parameter-elements");(void)field(c,"max-parameter-elements"); }
        if((ipc::scalar_parameter_operation(v,c.operation)&&ipc::numeric_parameter(ipc::parameter_type(v,c.type)))||
           (c.operation=="param.subscribe-events"&&ipc::numeric_parameter(ipc::parameter_event_type(v,c.type))))
        { allowed.insert("parameter-min-value");allowed.insert("parameter-max-value"); }
        if(c.limits.count("validator")&&field(c,"validator")=="twist-bounds") { allowed.insert("twist.linear-max-abs"); allowed.insert("twist.angular-max-abs"); positive(field(c,"twist.linear-max-abs")); positive(field(c,"twist.angular-max-abs")); }
        for(auto const& key:f) { (void)field(c,key); }
        for(auto const& kv:c.limits)
        {
            auto const& k{kv.first}; auto const& value{kv.second};
            if(!allowed.count(k)||value.empty()) { throw std::runtime_error("unknown/inapplicable field: "+k); }
            if(k=="search-parent-namespaces"&&value!="true"&&value!="false") { throw std::runtime_error("parent parameter search must explicitly be true or false"); }
            if(k=="service-source-alias"&&(!ipc::valid_token(value)||value=="."||value==".."||value==c.alias)) { throw std::runtime_error("availability needs a distinct explicit source alias"); }
            if(k=="event-source-alias"&&(!ipc::valid_token(value)||value==c.alias)) { throw std::runtime_error("observer requires a distinct explicit source alias"); }
            if(k=="event-kinds") { std::uint32_t mask{};if(!ipc::parse_event_kinds(value,mask)) { throw std::runtime_error("event kinds require an explicit ordered nonempty CSV"); } }
            if(k=="overflow-policy"&&value!="drop-oldest"&&value!="drop-newest"&&value!="reject"&&value!="backpressure")
            { throw std::runtime_error("invalid explicit overflow policy"); }
            if(k=="max-rate-hz"||k=="audit-max-rate-hz"||k=="tf.max-translation-abs"||k=="tf.quaternion-norm-tolerance") { (void)positive(value); }
            else if(c.operation=="service.available"&&k=="max-wait-ns") { if(!integer(value,ipc::client_availability::maximum_wait_ns)) { throw std::runtime_error("positive availability wait ceiling required"); } }
            else if(k.rfind("max-",0)==0||k=="burst"||k=="queue-depth"||k=="qos.depth"||(k.rfind("qos.",0)==0&&k.size()>6&&k.compare(k.size()-6,6,".depth")==0)||k=="timeout-ms"||k=="audit-burst"||k=="audit-max-records"||k=="audit-max-bytes"||k=="buffer-cache-ms")
            { auto n{integer(value,1073741824)}; if(!n||((k.find("bytes")==std::string::npos)&&n>1048576)) { throw std::runtime_error("invalid limit "+k); } }
        }
        if(c.operation=="service.available")
        {
            auto wait{limit(c,"max-wait-ns")},history{limit(c,"max-binding-history")},clients{limit(c,"max-native-clients")};
            if(limit(c,"timeout-ms")>60000||(wait+999999)/1000000+1>limit(c,"timeout-ms")||history>65536||clients>1024||clients>history||
               limit(c,"max-inflight")!=1||limit(c,"max-request-bytes")!=ipc::client_availability::bytes||
               limit(c,"max-response-bytes")!=ipc::client_availability::bytes||limit(c,"max-bytes-per-second")<ipc::client_availability::bytes)
            { throw std::runtime_error("availability finite wait/headroom, exact envelope or history/client bounds differ"); }
        }
        auto validator{c.limits.count("validator")?field(c,"validator"):"none"};
        if(validator!="none"&&validator!="finite"&&validator!="twist-bounds"&&validator!="transform-bounds") { throw std::runtime_error("unsupported validator"); }
        bool twist{c.type=="geometry_msgs/Twist"||c.type=="geometry_msgs/msg/Twist"};
        bool parameter_float{(ipc::scalar_parameter_operation(v,c.operation)&&ipc::float64_parameter(ipc::parameter_type(v,c.type)))||
                             (c.operation=="param.subscribe-events"&&ipc::float64_parameter(ipc::parameter_event_type(v,c.type)))};
        if(validator!="none"&&!twist&&!(validator=="transform-bounds"&&c.operation.rfind("tf.",0)==0)&&!(parameter_float&&validator=="finite")) { throw std::runtime_error("validator does not apply to this adapter"); }
        if(ipc::validate_parameter_contract(v,c)) { throw std::runtime_error("invalid explicit parameter type, bounds or byte quota"); }
        auto audit{field(c,"audit")}; if(audit!="allow-and-deny"&&audit!="deny-only") { throw std::runtime_error("invalid audit"); }
        if(c.operation.rfind("topic.",0)==0||c.operation.rfind("tf.",0)==0||c.operation=="param.subscribe-events")
        {
            auto overflow{field(c,"overflow-policy")}; if(overflow!="drop-oldest"&&overflow!="drop-newest"&&overflow!="reject"&&overflow!="backpressure") { throw std::runtime_error("unsupported overflow"); }
            auto depth{limit(c,v==ros::version::ros1?"queue-depth":"qos.depth")};
            auto message{limit(c,c.operation=="param.subscribe-events"?"max-parameter-bytes":"max-message-bytes")};
            if(message>ipc::max_payload_bytes-518||message>limit(c,c.operation=="param.subscribe-events"?"max-event-queue-bytes":"max-queue-bytes")||message>limit(c,"max-bytes-per-second")||limit(c,"burst")>depth) { throw std::runtime_error("conflicting limits"); }
            if(v==ros::version::ros1)
            {
                if(field(c,"transport")!="tcpros") { throw std::runtime_error("unsupported transport"); }
                if((c.operation=="topic.publish"||c.operation=="tf.broadcast")&&field(c,"latch")!="true"&&field(c,"latch")!="false") { throw std::runtime_error("invalid latch"); }
            }
        }
        else if(c.operation.rfind("service.",0)==0&&(limit(c,"max-request-bytes")>ipc::max_payload_bytes-518||limit(c,"max-response-bytes")>ipc::max_payload_bytes-518||limit(c,"timeout-ms")>ipc::max_timeout_ms))
        { throw std::runtime_error("service limits exceed bounded IPC"); }
        else if(c.operation=="log.write"&&field(c,"severity")!="debug"&&field(c,"severity")!="info"&&field(c,"severity")!="warn"&&field(c,"severity")!="error"&&field(c,"severity")!="fatal") { throw std::runtime_error("invalid explicit logging severity"); }
        if(v==ros::version::ros2&&c.limits.count("qos.reliability"))
        {
            auto reliability{field(c,"qos.reliability")},durability{field(c,"qos.durability")},liveliness{field(c,"qos.liveliness")};
            if((reliability!="reliable"&&reliability!="best-effort")||(durability!="volatile"&&durability!="transient-local")||(liveliness!="automatic"&&liveliness!="manual-by-topic")||field(c,"qos.history")!="keep-last") { throw std::runtime_error("invalid explicit QoS"); }
            for(auto key:{"qos.deadline","qos.lifespan","qos.liveliness-lease-duration"})
            { auto value{field(c,key)};if(value!="infinite"&&!integer(value,86400000000000ULL)) { throw std::runtime_error("invalid explicit QoS duration"); } }
        }
        if(c.operation.rfind("tf.",0)==0) { if(!canonical("/"+field(c,"source-frame"))||!canonical("/"+field(c,"target-frame"))||field(c,"source-frame")==field(c,"target-frame")||field(c,"validator")!="transform-bounds"||limit(c,"buffer-cache-ms")==0||limit(c,"buffer-cache-ms")>60000||positive(field(c,"tf.quaternion-norm-tolerance"))>0.001) { throw std::runtime_error("invalid explicit frame/buffer/validator bounds"); } }
        if(c.operation.rfind("action.",0)==0)
        {
            auto component_min{[](ipc::custom_topic_descriptor const* part) noexcept
            {return part->bounded_layout?part->minimum_wire_bytes:part->wire_bytes;}};
            auto goal_min{custom_action?component_min(custom_action->goal):4u};
            auto feedback_min{custom_action?8u+component_min(custom_action->feedback):12u};
            auto result_min{custom_action?9u+component_min(custom_action->result):13u};
            if((!custom_action&&limit(c,"fibonacci.max-order")>46)||limit(c,"max-goal-bytes")<goal_min||limit(c,"max-feedback-bytes")<feedback_min||limit(c,"max-result-bytes")<result_min||limit(c,"max-event-queue-bytes")<std::max(limit(c,"max-feedback-bytes"),limit(c,"max-result-bytes"))||limit(c,"max-feedback-bytes")>ipc::max_payload_bytes-518||limit(c,"max-result-bytes")>ipc::max_payload_bytes-518) { throw std::runtime_error("invalid explicit Action bounds"); }
            if(v==ros::version::ros2)
            {
                for(auto channel:{"goal-service","cancel-service","result-service","feedback","status"})
                {
                    auto prefix{std::string{"qos."}+channel+"."};auto reliability{field(c,prefix+"reliability")},durability{field(c,prefix+"durability")},liveliness{field(c,prefix+"liveliness")};
                    if((reliability!="reliable"&&reliability!="best-effort")||(durability!="volatile"&&durability!="transient-local")||(liveliness!="automatic"&&liveliness!="manual-by-topic")||field(c,prefix+"history")!="keep-last") { throw std::runtime_error("invalid explicit Action QoS"); }
                    for(auto key:{"deadline","lifespan","liveliness-lease-duration"}){auto value{field(c,prefix+key)};if(value!="infinite"&&!integer(value,86400000000000ULL)){throw std::runtime_error("invalid Action QoS duration");}}
                }
            }
            if(field(c,"validator")!="none"||(field(c,"overflow-policy")!="drop-oldest"&&field(c,"overflow-policy")!="drop-newest"&&field(c,"overflow-policy")!="reject"&&field(c,"overflow-policy")!="backpressure")||field(c,"cancel-scope")!=(c.operation=="action.cancel-any-goal"?"any":c.operation=="action.cancel-own-goal"?"own":"not-applicable")) { throw std::runtime_error("invalid Action validator/overflow/cancel scope"); }
        }
        if(c.operation=="event.observe")
        {
            auto record{limit(c,"max-event-bytes")},depth{limit(c,"max-event-queue-depth")},queue{limit(c,"max-event-queue-bytes")};
            auto overflow{field(c,"overflow-policy")};
            if(c.type!=ipc::event_type||c.type_identity!=ipc::event_type_identity||record<ipc::event_header_bytes||record>ipc::max_payload_bytes-518||
               depth<3||queue<record+2*ipc::event_header_bytes||record>limit(c,"max-bytes-per-second")||limit(c,"burst")>depth||
               (overflow!="drop-oldest"&&overflow!="drop-newest"&&overflow!="reject"&&overflow!="backpressure"))
            { throw std::runtime_error("invalid explicit observer type/queue/control-record bounds"); }
        }
        if(c.limits.count("allowed-transition")&&field(c,"allowed-transition")!="configure"&&field(c,"allowed-transition")!="cleanup"&&field(c,"allowed-transition")!="activate"&&field(c,"allowed-transition")!="deactivate"&&field(c,"allowed-transition")!="shutdown") { throw std::runtime_error("invalid explicit lifecycle transition"); }
        if(c.limits.count("allowed-name-prefix")&&!canonical(field(c,"allowed-name-prefix"),true)) { throw std::runtime_error("invalid canonical result prefix"); }
        if(c.limits.count("timeout-ms")&&limit(c,"timeout-ms")>ipc::max_timeout_ms) { throw std::runtime_error("timeout exceeds wire deadline"); }
        if(c.limits.count("list-depth")&&(limit(c,"list-depth")==0||limit(c,"list-depth")>32)) { throw std::runtime_error("parameter list depth"); }
    }
    inline void validate_service_qos_contract(ros::target_policy const& policy)
    {
        if(policy.ros_version!=ros::version::ros2) { return; }
        for(auto const& response:policy.capabilities)
        {
            if(response.second.operation!="service.respond") { continue; }
            for(auto const& server:policy.capabilities)
            {
                if(server.second.operation!="service.advertise"||server.second.resource!=response.second.resource||server.second.type!=response.second.type||server.second.type_identity!=response.second.type_identity) { continue; }
                for(auto key:{"qos.reliability","qos.durability","qos.history","qos.depth","qos.deadline","qos.lifespan","qos.liveliness","qos.liveliness-lease-duration"})
                { if(field(server.second,key)!=field(response.second,key)) { throw std::runtime_error("service response QoS differs from its explicit server contract"); } }
            }
        }
    }
    inline void validate_action_qos_contract(ros::target_policy const& policy)
    {
        if(policy.ros_version!=ros::version::ros2) { return; }
        for(auto const& response:policy.capabilities)
        {
            if(response.second.operation!="action.respond") { continue; }
            for(auto const& server:policy.capabilities)
            {
                if(server.second.operation!="action.advertise"||server.second.resource!=response.second.resource||server.second.type!=response.second.type||server.second.type_identity!=response.second.type_identity) { continue; }
                if(!ros::equal_action_server_qos_contract(server.second,response.second))
                { throw std::runtime_error("action response QoS differs from its explicit server contract"); }
            }
        }
    }
    /** Observe only an independently granted source in this exact target.
     * Payload kinds remain constrained by that source's operation; metadata
     * never widens access to the native Graph or another session's resources. */
    inline void validate_event_sources(ros::target_policy const& policy)
    {
        std::set<std::string_view> observed;
        for(auto const& item:policy.capabilities)
        {
            auto const& cap{item.second};if(cap.operation!="event.observe") { continue; }
            auto source{policy.capabilities.find(field(cap,"event-source-alias"))};
            if(source==policy.capabilities.end()||source->second.operation=="event.observe"||source->second.resource!=cap.resource||
               !observed.emplace(source->first).second)
            { throw std::runtime_error("observer requires one distinct source on the exact resource in this target"); }
            std::uint32_t kinds{};if(!ipc::parse_event_kinds(field(cap,"event-kinds"),kinds)) { throw std::runtime_error("invalid explicit event kinds"); }
            auto allowed{ipc::event_bit(ipc::event_kind::resource_error)|ipc::event_bit(ipc::event_kind::resource_closed)};
            ipc::event_kind kind{};std::uint8_t flags{};
            if(ipc::event_source_kind(source->second.operation,kind,flags)) { allowed|=ipc::event_bit(kind); }
            if(kinds&~allowed) { throw std::runtime_error("observer event kinds exceed the independent source operation"); }
        }
    }
    inline void validate_availability_sources(ros::target_policy const& policy)
    {
        for(auto const& item:policy.capabilities)
        {
            auto const& cap{item.second};if(cap.operation!="service.available") { continue; }
            auto source{policy.capabilities.find(field(cap,"service-source-alias"))};
            if(source==policy.capabilities.end()||!ros::service_availability_pair(policy.ros_version,cap,source->second))
            { throw std::runtime_error("availability requires its independently granted exact typed service.call source and QoS"); }
        }
    }
    inline configuration load(std::string const& path)
    {
#if defined(__linux__)
        native_io::config_lines in{path};std::string_view line;
#else
        std::ifstream in{path}; if(!in) { throw std::runtime_error("cannot open trusted config"); }
        std::string storage;std::string_view line;
#endif
        configuration c; c.policy.enabled=true; std::set<std::string> singleton;
        unsigned line_number{};
#if defined(__linux__)
        while(in.next(line))
#else
        while(std::getline(in,storage))
#endif
        {
#if !defined(__linux__)
            line=storage;
#endif
            ++line_number; if(line.empty()||line[0]=='#') { continue; }
            if(line.size()>4096) { throw std::runtime_error("oversized config line"); }
            std::array<std::string_view,7> t;auto count{native_io::split(line,t)};
            if(!count) { continue; }
            auto const& key{t[0]};
            if(key=="grant"&&count==6)
            {
                ros::capability_spec cap; cap.alias=t[1]; cap.operation=t[2]; cap.resource=t[3]; cap.type=t[4]; cap.type_identity=t[5];
                if(!c.policy.capabilities.emplace(cap.alias,std::move(cap)).second) { throw std::runtime_error("duplicate grant"); }
            }
            else if(key=="set"&&count==4)
            {
                auto i{c.policy.capabilities.find(t[1])};
                if(i==c.policy.capabilities.end()||!i->second.limits.emplace(t[2],t[3]).second) { throw std::runtime_error("unknown grant or duplicate field"); }
            }
            else if(key=="connection"&&count==3)
            { if(!c.policy.connection.emplace(t[1],t[2]).second) { throw std::runtime_error("duplicate connection field"); } }
            else if(count==2&&singleton.insert(std::string{key}).second)
            {
                if(key=="backend") { if(t[1]!="ros1"&&t[1]!="ros2") { throw std::runtime_error("backend"); } c.policy.ros_version=t[1]=="ros1"?ros::version::ros1:ros::version::ros2; }
                else if(key=="worker-uid") { c.worker_uid=static_cast<std::uint32_t>(integer(t[1])); }
                else if(key=="worker-pid") { c.worker_pid=static_cast<std::uint32_t>(integer(t[1])); }
                else if(key=="socket") { c.socket=t[1]; }
                else if(key=="timeout-ms") { c.timeout_ms=static_cast<std::uint32_t>(integer(t[1],ipc::max_timeout_ms)); }
                else if(key=="max-sessions") { c.max_sessions=static_cast<std::uint32_t>(integer(t[1],256)); }
                else if(key=="audit-max-records-per-second") { c.audit_records_per_second=static_cast<std::uint32_t>(integer(t[1],1000000)); }
                else if(key=="audit-max-records") { c.audit_max_records=integer(t[1],1048576); }
                else if(key=="audit-max-bytes") { c.audit_max_bytes=integer(t[1],67108864); }
                else { throw std::runtime_error("unknown config field "+std::string{key}); }
            }
            else
            {
                native_io::fixed_text<64> message;
                if(!message.append("invalid config at line ")||!message.number(line_number))
                {throw std::runtime_error("invalid config line");}
                throw std::runtime_error(std::string{message.view()});
            }
        }
        for(auto k:{"backend","worker-uid","worker-pid","socket","timeout-ms","max-sessions","audit-max-records-per-second","audit-max-records","audit-max-bytes"}) { if(!singleton.count(k)) { throw std::runtime_error(std::string{"missing explicit config "}+k); } }
        if(!c.worker_pid||!c.timeout_ms||!c.max_sessions||!c.audit_records_per_second||!c.audit_max_records||!c.audit_max_bytes||c.socket.empty()||c.socket[0]!='/') { throw std::runtime_error("invalid explicit session binding"); }
        for(auto const& p:c.policy.capabilities) { validate(c.policy.ros_version,p.second); }
        validate_service_qos_contract(c.policy);
        validate_action_qos_contract(c.policy);
        validate_event_sources(c.policy);
        validate_availability_sources(c.policy);
        return c;
    }
}
