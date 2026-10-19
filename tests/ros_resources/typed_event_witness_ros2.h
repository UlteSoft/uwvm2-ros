#pragma once
#include "typed_event_common_ros2.h"
#include <iomanip>
#include <map>
#include "../../gateways/common/native_io.h"

namespace typed_event_probe
{
    // Jazzy's TimeSource creates a framework ParameterEvent reader even when
    // the Gateway has no event grant. Identify its actual GID before resuming
    // the Guest; topic totals alone cannot identify a capability projection.
    class reader_observation
    {
        using identities=std::map<std::string,std::string>;
        rclcpp::Node::SharedPtr node_;
        rclcpp::Publisher<rcl_interfaces::msg::ParameterEvent>::SharedPtr publisher_;
        std::string native_node_,gateway_node_;
        identities baseline_;

        static std::string name(rclcpp::TopicEndpointInfo const& endpoint)
        {
            auto const& ns{endpoint.node_namespace()};
            return (ns=="/"?std::string{"/"}:ns+"/")+endpoint.node_name();
        }
        static std::string gid(rclcpp::TopicEndpointInfo const& endpoint)
        {
            auto const& bytes{endpoint.endpoint_gid()};
            require(std::any_of(bytes.begin(),bytes.end(),[](auto byte){return byte!=0;}),"native graph reports an actual nonzero reader GID");
            return gateway::native_io::hexadecimal(bytes.data(),bytes.size());
        }
        identities snapshot(bool log=false,char const* phase="observation") const
        {
            identities result;
            for(auto const& endpoint:node_->get_subscriptions_info_by_topic("/parameter_events"))
            {
                require(endpoint.topic_type()=="rcl_interfaces/msg/ParameterEvent","native event graph reports the official generated type");
                auto id{gid(endpoint)},node_name{name(endpoint)};
                require(result.emplace(id,node_name).second,"native event graph reader GIDs are unique");
                if(log)
                {
                    auto qos{endpoint.qos_profile().get_rmw_qos_profile()};
                    std::cout<<"TYPED_EVENT_READER phase="<<phase<<" node="<<node_name<<" gid="<<id
                        <<" type="<<endpoint.topic_type()<<" endpoint="<<static_cast<unsigned>(endpoint.endpoint_type())
                        <<" reliability="<<static_cast<unsigned>(qos.reliability)<<" durability="<<static_cast<unsigned>(qos.durability)
                        <<" history="<<static_cast<unsigned>(qos.history)<<" depth="<<qos.depth
                        <<" deadline="<<qos.deadline.sec<<":"<<qos.deadline.nsec
                        <<" lifespan="<<qos.lifespan.sec<<":"<<qos.lifespan.nsec
                        <<" liveliness="<<static_cast<unsigned>(qos.liveliness)
                        <<" lease="<<qos.liveliness_lease_duration.sec<<":"<<qos.liveliness_lease_duration.nsec<<"\n";
                }
            }
            if(log) { std::cout<<std::flush; }
            return result;
        }
        std::size_t count(identities const& readers,std::string const& node_name) const
        {
            return static_cast<std::size_t>(std::count_if(readers.begin(),readers.end(),[&](auto const& item){return item.second==node_name;}));
        }
        bool exact_projection(identities const& readers,std::size_t projected) const
        {
            for(auto const& original:baseline_)
            {
                auto found{readers.find(original.first)};
                if(found==readers.end()||found->second!=original.second) { return false; }
            }
            for(auto const& reader:readers)
            {
                if(!baseline_.count(reader.first)&&reader.second!=gateway_node_) { return false; }
            }
            return readers.size()==baseline_.size()+projected&&publisher_->get_subscription_count()==readers.size();
        }
    public:
        reader_observation(rclcpp::Node::SharedPtr node,
            rclcpp::Publisher<rcl_interfaces::msg::ParameterEvent>::SharedPtr publisher,std::string gateway_node)
            :node_{std::move(node)},publisher_{std::move(publisher)},native_node_{node_->get_fully_qualified_name()},gateway_node_{std::move(gateway_node)} {}
        void capture_baseline(bool allow_other_nodes)
        {
            auto started{std::chrono::steady_clock::now()},deadline{started+std::chrono::seconds(8)},stable_since{started};
            identities previous;
            while(rclcpp::ok()&&std::chrono::steady_clock::now()<deadline)
            {
                auto readers{snapshot()};auto now{std::chrono::steady_clock::now()};
                bool ready{count(readers,native_node_)==1&&count(readers,gateway_node_)==1&&
                    (allow_other_nodes||readers.size()==2)&&publisher_->get_subscription_count()==readers.size()};
                if(!ready||readers!=previous) { stable_since=now; }
                previous=readers;
                if(ready&&now-started>=std::chrono::seconds(2)&&now-stable_since>=std::chrono::milliseconds(250))
                {
                    baseline_=std::move(readers);
                    require(snapshot(true,"baseline")==baseline_,"native reader identities remain stable while recording baseline");
                    std::cout<<"BASELINE_READY typed ParameterEvent native="<<native_node_<<" gateway="<<gateway_node_
                        <<" graph_readers="<<baseline_.size()<<" actual_subscriptions="<<publisher_->get_subscription_count()
                        <<" native_framework_readers=1 gateway_framework_readers=1 graph_history_depth=not_verified\n"<<std::flush;
                    return;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            snapshot(true,"baseline-timeout");
            throw std::runtime_error{"native typed-event framework baseline discovery/stability deadline"};
        }
        void wait_projection(std::size_t projected,unsigned timeout_ms=2000) const
        {
            auto deadline{std::chrono::steady_clock::now()+std::chrono::milliseconds(timeout_ms)};
            while(rclcpp::ok()&&std::chrono::steady_clock::now()<deadline)
            {
                auto readers{snapshot()};
                if(exact_projection(readers,projected))
                {
                    require(snapshot(true,projected?"projection-created":"projection-closed")==readers,
                        "native reader identity snapshot stable at projection boundary");
                    std::cout<<"TYPED_EVENT_PROJECTION projection_readers="<<projected<<" retained_baseline_readers="<<baseline_.size()
                        <<" graph_readers="<<readers.size()<<" actual_subscriptions="<<publisher_->get_subscription_count()<<"\n"<<std::flush;
                    return;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            snapshot(true,"projection-timeout");
            throw std::runtime_error{"actual typed-event projection GID creation/cleanup and writer delta deadline"};
        }
        void no_projection_after_worker_exit() const
        {
            auto readers{snapshot()};
            for(auto const& reader:readers)
            {
                auto original{baseline_.find(reader.first)};
                require(original!=baseline_.end()&&original->second==reader.second,"denied typed subscription created a new actual DDS reader GID");
            }
            require(count(readers,native_node_)==1,"native witness framework reader remains present during denial observation");
            // Worker exit can remove the known Gateway framework reader. Its
            // disappearance is allowed; any new GID or writer-count excess is not.
            require(publisher_->get_subscription_count()<=baseline_.size(),"denied typed subscription increased actual writer matches above framework baseline");
        }
        void log_final_absence() const
        {
            no_projection_after_worker_exit();auto readers{snapshot(true,"denied-final")};
            std::cout<<"DENIED_TYPED_SUBSCRIPTION projection_readers=0 baseline_readers="<<baseline_.size()
                <<" remaining_baseline_readers="<<readers.size()<<" actual_subscriptions="<<publisher_->get_subscription_count()<<"\n"<<std::flush;
        }
    };
}

inline int ros2_typed_event_witness(int argc,char** argv)
{
    require(argc==3||argc==4,"usage resource witness --typed-events bool|int64|float64 [scenario]");
    std::string kind{argv[2]},scenario{argc==4?argv[3]:"roundtrip"};
    require(kind=="bool"||kind=="int64"||kind=="float64","supported typed-event witness kind");
    require(scenario=="roundtrip"||scenario=="wrong-kind"||scenario=="wrong-node"||scenario=="wrong-name"||
            scenario=="not-set-delete"||scenario=="bounds"||scenario=="nonfinite"||scenario=="fields"||
            scenario=="bytes"||scenario=="malformed-stamp"||scenario=="deny-subscription","known typed-event witness scenario");
    require((scenario!="bounds"||kind!="bool")&&(scenario!="nonfinite"||kind=="float64"),"numeric typed-event scenario kind");
    int ros_argc{1};rclcpp::init(ros_argc,argv);
    auto node{std::make_shared<rclcpp::Node>("typed_event_publisher","/uwvm_resources")};
    auto publisher{node->create_publisher<rcl_interfaces::msg::ParameterEvent>("/parameter_events",
        rclcpp::QoS{rclcpp::KeepLast{32}}.reliable().durability_volatile())};
    rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
    std::atomic<bool> stop{},spin_failed{};std::thread spinner{[&] {
        try { while(!stop&&rclcpp::ok()) { executor.spin_once(std::chrono::milliseconds(10)); } }
        catch(...) { spin_failed=true; }
    }};
    auto cleanup=[&] { stop=true;executor.cancel();spinner.join();publisher.reset();rclcpp::shutdown(); };
    unsigned publications{};
    try
    {
        std::cout<<"READY typed ParameterEvent native witness kind="<<kind<<" scenario="<<scenario<<"\n"<<std::flush;
        auto publish=[&](rcl_interfaces::msg::ParameterEvent const& message) {
            require(rclcpp::ok()&&!spin_failed,"typed-event publisher stopped before controlled publication");
            publisher->publish(message);++publications;
        };
        typed_event_probe::reader_observation readers{node,publisher,"/uwvm_ros_test/uwvm_ros2_wasm_network"};
        readers.capture_baseline(false);
        if(scenario=="deny-subscription")
        {
            auto until{std::chrono::steady_clock::now()+std::chrono::seconds(5)};
            while(rclcpp::ok()&&std::chrono::steady_clock::now()<until)
            {
                readers.no_projection_after_worker_exit();
                publish(typed_event_probe::event("value",typed_event_probe::value(kind,1),2,9));
                for(unsigned sample{};sample!=10;++sample)
                { readers.no_projection_after_worker_exit();std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
            }
            require(rclcpp::ok(),"typed subscription absence witness completed");readers.log_final_absence();
            std::cout<<"DENIED_TYPED_SUBSCRIPTION native_publications="<<publications<<"\n"<<std::flush;
        }
        else
        {
            auto const needed{scenario=="bytes"?std::size_t{2}:std::size_t{1}};
            readers.wait_projection(needed,8000);
            std::cout<<"MATCHED_TYPED_EVENT projection_readers="<<needed<<" actual_subscriptions="<<publisher->get_subscription_count()<<"\n"<<std::flush;
            if(scenario=="roundtrip")
            {
                auto emit=[&](unsigned phase,std::uint8_t event_kind,unsigned stamp) {
                    publish(typed_event_probe::event("value",typed_event_probe::value(kind,phase),event_kind,stamp));
                    std::this_thread::sleep_for(std::chrono::milliseconds(150));
                };
                emit(0,1,1);emit(1,2,2);
                if(kind=="int64") { emit(2,2,3);emit(2,3,4); }
                else { emit(2,3,3); }
            }
            else
            {
                for(unsigned n{};n!=20;++n)
                {
                    auto message{typed_event_probe::event(scenario=="bytes"?"quota_value":"value",
                        typed_event_probe::value(kind,1),scenario=="not-set-delete"?3:static_cast<std::uint8_t>(n%3+1),1)};
                    auto& parameter{message.new_parameters.empty()?(message.changed_parameters.empty()?message.deleted_parameters.front():message.changed_parameters.front()):message.new_parameters.front()};
                    if(scenario=="wrong-node") { message.node="/uwvm_resources/other"; }
                    if(scenario=="wrong-name") { parameter.name="other"; }
                    if(scenario=="wrong-kind") { parameter.value.type=rcl_interfaces::msg::ParameterType::PARAMETER_STRING;parameter.value.string_value="unselected"; }
                    if(scenario=="not-set-delete") { parameter.value.type=rcl_interfaces::msg::ParameterType::PARAMETER_NOT_SET; }
                    if(scenario=="bounds")
                    { parameter.value.integer_value=n%2?9007199254740996LL:-9007199254740996LL;parameter.value.double_value=n%2?3.0:-3.0; }
                    if(scenario=="nonfinite")
                    { parameter.value.double_value=n%3==0?std::numeric_limits<double>::quiet_NaN():n%3==1?std::numeric_limits<double>::infinity():-std::numeric_limits<double>::infinity(); }
                    if(scenario=="fields") { message.changed_parameters.push_back(typed_event_probe::native("other",typed_event_probe::value(kind,1))); }
                    if(scenario=="malformed-stamp") { message.stamp.nanosec=1000000000U; }
                    publish(message);std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                publish(typed_event_probe::event("value",typed_event_probe::value(kind,1),2,9));
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
            require(rclcpp::ok(),"typed-event positive marker witness retention completed");
            std::cout<<"PUBLISHED_TYPED_EVENT exact_native_publications="<<publications<<" scenario="<<scenario<<"\n"<<std::flush;
        }
        require(!spin_failed,"typed-event executor failed during native observation");
        cleanup();std::cout<<(scenario=="deny-subscription"?"PASS denied typed ParameterEvent subscription\n":"PASS typed ParameterEvent\n")<<std::flush;return 0;
    }
    catch(...) { cleanup();throw; }
}
