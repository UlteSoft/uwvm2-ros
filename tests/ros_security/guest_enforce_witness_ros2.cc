/**
 * @file guest_enforce_witness_ros2.cc
 * @brief Independent, prepared Jazzy/FastDDS signed native Guest witness.
 *
 * This is a C++20 test fixture, not a Gateway, IPC client or certificate
 * generator. Compile it only against the qualified ARM Jazzy SDK and link
 * rclcpp, rcl, rcutils, rmw, std_msgs, rosidl_typesupport_cpp, fastrtps and pthread.
 * The external controller owns credentials, native environment, immutable
 * executable snapshots, current cgroup/QEMU evidence and the overall process
 * deadline. No compilation or real DDS behavior is established by this source.
 *
 * Invocation: guest-enforce-witness /tmp/g09-keys
 * The controller provides domain74, rmw_fastrtps_cpp and signed /witness
 * credentials using a literal environment allowlist. This fixture validates
 * those inputs; it does not replace inherited values or generate credentials.
 * stdin byte I permits context initialization after the controller proves the
 * stopped live ARM image. READY_SECURE_WITNESS permits Guest endpoint creation.
 * READY_SECURE_MATCHED permits the controller to release the Guest's M gate.
 * stdin byte F is sent only after the Guest has checked every incoming byte,
 * closed its handles and exited successfully. The final signed denial check
 * then runs before PASS_SECURE_WITNESS. EOF, wrong gates or timeouts fail.
 *
 * stdout contains only the three documented marker lines, fixed public node,
 * topic/type identifiers, GIDs and bounded counts. Payloads, raw native error
 * messages, credential paths and credential contents are never printed.
 */
#include <fast_io.h>
#include <fast_io_dsal/string.h>
#include <fastdds/dds/log/Log.hpp>
#include <rcl/context.h>
#include <rcl/error_handling.h>
#include <rcl/publisher.h>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/node_interfaces/node_graph_interface.hpp>
#include <rcutils/logging.h>
#include <rmw/init.h>
#include <rmw/rmw.h>
#include <rosidl_runtime_c/type_hash.h>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <std_msgs/msg/string.hpp>
#include <algorithm>
#include <array>
#include <atomic>
#include <cerrno>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <poll.h>
#include <stdexcept>
#include <string_view>
#include <thread>
#include <unistd.h>

namespace secure_guest_witness
{
    using message=std_msgs::msg::String;
    using clock=std::chrono::steady_clock;
    using gid=std::array<std::uint8_t,RMW_GID_STORAGE_SIZE>;
    namespace dds=eprosima::fastdds::dds;
    constexpr std::string_view node_name{"secure_witness"};
    constexpr std::string_view node_namespace{"/uwvm_test"};
    constexpr std::string_view full_node{"/uwvm_test/secure_witness"};
    constexpr std::string_view gateway_name{"uwvm_ros2_gateway_probe"};
    constexpr std::string_view type_name{"std_msgs/msg/String"};
    constexpr char inbound_topic[]{"/uwvm_test/pubString"};
    constexpr char outbound_topic[]{"/uwvm_test/subString"};
    constexpr char denied_topic[]{"/uwvm_test/not_permitted"};
    constexpr std::string_view keystore{"/tmp/g09-keys"};
    constexpr std::string_view root_path{"/tmp/g09-keys/enclaves/witness"};
    constexpr std::string_view implementation{"rmw_fastrtps_cpp"};
    constexpr std::string_view guest_data{"uwvm real ROS exact payload"};
    constexpr std::string_view reply_data{"native signed reply"};
    constexpr std::string_view native_identity{"RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18"};
    static_assert(guest_data.size()==27&&reply_data.size()==19);
    // The qualified ARM Jazzy rmw/types.h defines exactly sixteen GID bytes.
    // Copy and print that entire native storage as thirty-two lowercase hex
    // digits. Do not pad, truncate or accept another storage width. The separate
    // thirty-two-byte rosidl type hash below has a different protocol meaning.
    static_assert(RMW_GID_STORAGE_SIZE==16,"marker protocol requires the qualified Jazzy16-byte RMW GID");

    // These independent fixed canonical records are the exact bytes the Guest
    // must publish/receive. DDS/CDR framing is deliberately not part of them.
    constexpr auto canonical_guest=[]
    {
        std::array<std::uint8_t,31> bytes{};bytes[0]=27;
        for(std::size_t i{};i!=guest_data.size();++i) { bytes[i+4]=static_cast<std::uint8_t>(guest_data[i]); }
        return bytes;
    }();
    constexpr auto canonical_reply=[]
    {
        std::array<std::uint8_t,23> bytes{};bytes[0]=19;
        for(std::size_t i{};i!=reply_data.size();++i) { bytes[i+4]=static_cast<std::uint8_t>(reply_data[i]); }
        return bytes;
    }();

    void require(bool condition)
    { if(!condition) { throw std::runtime_error("secure witness contract failed"); } }
    bool equal(char const* value,std::string_view expected) noexcept
    { return value!=nullptr&&std::string_view{value}==expected; }
    void environment()
    {
        require(equal(::getenv("ROS_DOMAIN_ID"),"74")&&equal(::getenv("RMW_IMPLEMENTATION"),implementation));
        require(equal(::getenv("ROS_SECURITY_ENABLE"),"true")&&equal(::getenv("ROS_SECURITY_STRATEGY"),"Enforce"));
        require(equal(::getenv("ROS_SECURITY_KEYSTORE"),keystore));
        for(auto name:{"ROS_SECURITY_ENCLAVE_OVERRIDE","ROS_NAMESPACE","ROS_LOCALHOST_ONLY",
            "ROS_AUTOMATIC_DISCOVERY_RANGE","ROS_STATIC_PEERS","FASTRTPS_DEFAULT_PROFILES_FILE",
            "FASTDDS_DEFAULT_PROFILES_FILE","RMW_FASTRTPS_USE_QOS_FROM_XML","LD_PRELOAD"})
        { require(::getenv(name)==nullptr); }
    }

    // poll bounds waiting; fast_io transfers exactly one already-readable byte
    // from the controller's private stdin pipe. No formatted scanner reads
    // ahead across gates. There is one reader, and no descriptor is opened.
    bool gate(char expected,int timeout_ms)
    {
        pollfd descriptor{STDIN_FILENO,POLLIN,0};
        int ready{::poll(&descriptor,1,timeout_ms)};
        if(ready<0&&errno==EINTR) { return false; }
        require(ready>=0);
        if(!ready) { return false; }
        require(!(descriptor.revents&(POLLERR|POLLNVAL)));
        char value{};
        auto end{fast_io::operations::read_some(fast_io::posix_io_observer{STDIN_FILENO},&value,&value+1)};
        require(end==&value+1&&value==expected);return true;
    }
    void initial_gate()
    {
        auto until{clock::now()+std::chrono::seconds{20}};
        while(clock::now()<until) { if(gate('I',20)) { return; } }
        require(false);
    }

    template<typename Bytes> fast_io::string hexadecimal(Bytes const& bytes)
    {
        fast_io::string result;result.reserve(bytes.size()*2);
        fast_io::ostring_ref_fast_io output{std::addressof(result)};
        for(auto byte:bytes)
        { fast_io::print(output,fast_io::mnp::right(fast_io::mnp::hex(static_cast<unsigned>(byte)),2,'0')); }
        return result;
    }
    bool nonzero(gid const& value) noexcept
    { return std::any_of(value.begin(),value.end(),[](auto byte) { return byte!=0; }); }
    gid copied(rmw_gid_t const& value)
    {
        require(equal(value.implementation_identifier,implementation));gid result{};
        std::copy_n(value.data,result.size(),result.begin());require(nonzero(result));return result;
    }
    bool type_hash(rosidl_type_hash_t const& value)
    {
        if(value.version!=1) { return false; }
        std::array<std::uint8_t,32> bytes{};std::copy_n(value.value,bytes.size(),bytes.begin());
        auto encoded{hexadecimal(bytes)};
        return native_identity.substr(7)==std::string_view{encoded.data(),encoded.size()};
    }
    rclcpp::QoS configured_qos()
    {
        rmw_qos_profile_t profile{};profile.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;profile.depth=8;
        profile.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;profile.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;
        profile.deadline=RMW_DURATION_INFINITE;profile.lifespan=RMW_DURATION_INFINITE;
        profile.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;profile.liveliness_lease_duration=RMW_DURATION_INFINITE;
        profile.avoid_ros_namespace_conventions=false;
        return rclcpp::QoS{rclcpp::QoSInitialization::from_rmw(profile),profile};
    }
    bool actual_qos(rclcpp::QoS const& value,rclcpp::QoS const& expected)
    {
        auto actual{value.get_rmw_qos_profile()},wanted{expected.get_rmw_qos_profile()};
        auto same=[](rmw_time_t a,rmw_time_t b) { return a.sec==b.sec&&a.nsec==b.nsec; };
        return actual.history==wanted.history&&actual.depth==wanted.depth&&actual.reliability==wanted.reliability&&
            actual.durability==wanted.durability&&same(actual.deadline,wanted.deadline)&&same(actual.lifespan,wanted.lifespan)&&
            actual.liveliness==wanted.liveliness&&same(actual.liveliness_lease_duration,wanted.liveliness_lease_duration);
    }
    void attest(std::shared_ptr<rclcpp::Context> const& context)
    {
        // Public Jazzy APIs only: rclcpp::Context -> rcl_context_get_rmw_context
        // -> options used to initialize the native context. This verifies the
        // selected root/settings, while actual signed communication below is
        // still mandatory. No private impl or const_cast is used.
        auto rcl_context{context->get_rcl_context()};require(rcl_context!=nullptr);
        auto native{rcl_context_get_rmw_context(rcl_context.get())};require(native!=nullptr);
        require(equal(native->implementation_identifier,implementation)&&native->actual_domain_id==74);
        require(equal(native->options.enclave,"/witness")&&equal(native->options.implementation_identifier,implementation));
        require(native->options.security_options.enforce_security==RMW_SECURITY_ENFORCEMENT_ENFORCE);
        require(equal(native->options.security_options.security_root_path,root_path));
        require(equal(rmw_get_implementation_identifier(),implementation));
    }

    struct context_owner
    {
        std::shared_ptr<rclcpp::Context> value{std::make_shared<rclcpp::Context>()};
        void close() { if(value->is_valid()) { require(value->shutdown("secure witness completed")); } }
        ~context_owner() noexcept { try { if(value->is_valid()) { value->shutdown("secure witness cleanup"); } } catch(...) {} }
    };
    struct native_logging_owner
    {
        static void discard(rcutils_log_location_t const*,int,char const*,
            rcutils_time_point_value_t,char const*,va_list*) noexcept {}
        native_logging_owner()
        {
            // rcl logs its selected security directory during initialization.
            // Install a public process-local sink before any context exists;
            // the default rclcpp logging setup is explicitly disabled below.
            // FastDDS permission evidence uses its separate public consumer.
            require(rcutils_logging_initialize()==RCUTILS_RET_OK);
            rcutils_logging_set_output_handler(discard);
        }
        ~native_logging_owner() noexcept
        {
            // A destructor cannot report another exception during failure
            // cleanup. Capture the public return value explicitly; the owned
            // external process deadline still covers native library teardown.
            auto const result{rcutils_logging_shutdown()};(void)result;
        }
    };
    struct denial_evidence
    {
        std::mutex mutex;
        std::atomic<bool> armed{},ambiguous{};
        std::atomic<unsigned> permission_events{};
    };
    class denial_consumer final:public dds::LogConsumer
    {
        std::shared_ptr<denial_evidence> evidence_;
    public:
        explicit denial_consumer(std::shared_ptr<denial_evidence> evidence):evidence_{std::move(evidence)} {}
        void Consume(dds::Log::Entry const& entry) override
        {
            // FastDDS2.14's public log entry identifies the security function,
            // category and actual DDS topic. A generic RMW writer-creation
            // failure cannot distinguish allocation from access refusal.
            // The consumer borrows the entry, stores no error/payload strings,
            // performs no IO and never calls ROS from the logging thread.
            std::lock_guard<std::mutex> lock{evidence_->mutex};
            if(!evidence_->armed.load()||entry.kind!=dds::Log::Error||!equal(entry.context.category,"SECURITY")) { return; }
            std::string_view text{entry.message};
            constexpr std::string_view detail{"rt/uwvm_test/not_permitted topic not found in allow rule."};
            if(text.find(detail)==std::string_view::npos) { return; }
            // Permissions.cpp may additionally log a separate inner denial.
            // Only the outer creation-refusal function counts. Missing or
            // unexpected metadata is ambiguity, never an accepted denial.
            if(equal(entry.context.function,"check_create_datawriter")) { return; }
            if(!equal(entry.context.function,"get_datawriter_sec_attributes")||text.size()>4096||
                !text.starts_with("Error checking creation of local writer ")||
                text.find("(rt/uwvm_test/not_permitted topic not found in allow rule.")==std::string_view::npos||
                text.find("Permissions.cpp:")==std::string_view::npos)
            { evidence_->ambiguous.store(true);return; }
            if(evidence_->permission_events.fetch_add(1)!=0) { evidence_->ambiguous.store(true); }
        }
    };
    struct log_owner
    {
        std::shared_ptr<denial_evidence> evidence{std::make_shared<denial_evidence>()};
        log_owner()
        {
            // Private startup, before the first native context or DDS entity:
            // remove default consumers before they can disclose credentials.
            // FastDDS2.14 ClearConsumers calls Flush internally. At this first
            // logging-engine use it has no active middleware/logging thread;
            // it must never be called again after native initialization.
            dds::Log::ClearConsumers();dds::Log::ReportFunctions(true);
            dds::Log::RegisterConsumer(std::make_unique<denial_consumer>(evidence));
        }
        ~log_owner() noexcept
        {
            // The public API has no per-consumer unregister. Retain its shared
            // evidence until private process exit rather than invoking the
            // unbounded post-context ClearConsumers/Flush path. The external
            // controller also bounds library/process teardown by owned pidfd.
            try { std::lock_guard<std::mutex> lock{evidence->mutex};evidence->armed.store(false); } catch(...) {}
        }
    };
    void signed_denial(rclcpp::Node& node,rclcpp::QoS const& qos,
        rosidl_message_type_support_t const* support,std::shared_ptr<denial_evidence> const& evidence)
    {
        require(node.get_publishers_info_by_topic(denied_topic).empty());
        auto native_node{node.get_node_base_interface()->get_rcl_node_handle()};require(native_node!=nullptr);
        auto publisher{rcl_get_zero_initialized_publisher()};auto options{rcl_publisher_get_default_options()};
        options.qos=qos.get_rmw_qos_profile();rcl_reset_error();
        evidence->permission_events.store(0);evidence->ambiguous.store(false);evidence->armed.store(true);
        auto result{rcl_publisher_init(&publisher,native_node,support,denied_topic,&options)};
        // Always finalize unexpected successful creation before failing. No
        // sample is ever published on the forbidden Topic, even on failure.
        if(result==RCL_RET_OK)
        {
            evidence->armed.store(false);
            auto const cleanup_result{rcl_publisher_fini(&publisher,native_node)};
            (void)cleanup_result;require(false);
        }
        auto error{rcl_get_error_state()};
        constexpr std::string_view creation_error{"create_publisher() could not create data writer"};
        std::string_view error_text{error?error->message:""};
        bool creation_failure{error!=nullptr&&(error_text==creation_error||
            error_text.starts_with("create_publisher() could not create data writer, at "))};
        rcl_reset_error();
        auto until{clock::now()+std::chrono::seconds{2}};
        while(clock::now()<until)
        { std::this_thread::sleep_for(std::chrono::milliseconds{2}); }
        // Serialize disarming with this fixture's tiny borrowed-entry scanner.
        // A callback already running finishes before this lock is acquired;
        // future entries see unarmed state and cannot mutate the proof. Do not
        // call ClearConsumers here: its implementation calls unbounded Flush.
        bool permission_proved{};
        {
            std::lock_guard<std::mutex> lock{evidence->mutex};evidence->armed.store(false);
            permission_proved=evidence->permission_events.load()==1&&!evidence->ambiguous.load();
        }
        require(result==RCL_RET_ERROR&&publisher.impl==nullptr&&creation_failure&&permission_proved);
        require(node.get_publishers_info_by_topic(denied_topic).empty());
        // No unbounded Log::Flush(): a missing/late event is an actual failure.
        // The controller also retains its own bounded process lifetime guard.
    }

    struct observations
    {
        unsigned received{},sent{},mismatched{};gid received_gid{};
        bool selected{};gid gateway_pub{};
        void receive(message::ConstSharedPtr const& value,rclcpp::MessageInfo const& info)
        {
            // This callback runs only in the main thread's bounded spin_some.
            // It retains no middleware message, Guest pointer or payload copy.
            auto const& native{info.get_rmw_message_info()};
            auto source{copied(native.publisher_gid)};
            bool valid{!native.from_intra_process&&value&&value->data.size()==27};
            if(valid)
            {
                for(std::size_t i{};i!=value->data.size();++i)
                { if(static_cast<std::uint8_t>(value->data[i])!=canonical_guest[i+4]) { valid=false;break; } }
            }
            if(received&&(received_gid!=source)) { valid=false; }
            if(selected&&(gateway_pub!=source)) { valid=false; }
            if(!valid||received==8) { ++mismatched;require(false); }
            received_gid=source;++received;
        }
    };
    struct matched_endpoints
    { gid witness_sub{},witness_pub{},gateway_pub{},gateway_sub{}; };
    bool endpoints(rclcpp::Node& node,matched_endpoints& found)
    {
        // Require exact-one complementary node/type/direction per topic.
        // Reliable/volatile are the advertised remote interoperability fields;
        // complete local native QoS is verified separately, not inferred from
        // remote history/depth values that DDS discovery may not provide.
        auto inspect=[&](char const* topic,bool publishers,std::string_view name,gid& output)
        {
            auto entries{publishers?node.get_publishers_info_by_topic(topic):node.get_subscriptions_info_by_topic(topic)};
            require(entries.size()<=1);
            if(entries.empty()) { return false; }
            auto const& entry{entries.front()};auto qos{entry.qos_profile().get_rmw_qos_profile()};
            require(entry.node_name()==name&&entry.node_namespace()==node_namespace&&entry.topic_type()==type_name&&
                entry.endpoint_type()==(publishers?rclcpp::EndpointType::Publisher:rclcpp::EndpointType::Subscription)&&
                type_hash(entry.topic_type_hash())&&nonzero(entry.endpoint_gid())&&
                qos.reliability==RMW_QOS_POLICY_RELIABILITY_RELIABLE&&qos.durability==RMW_QOS_POLICY_DURABILITY_VOLATILE);
            output=entry.endpoint_gid();return true;
        };
        bool gateway_pub{inspect(inbound_topic,true,gateway_name,found.gateway_pub)};
        bool gateway_sub{inspect(outbound_topic,false,gateway_name,found.gateway_sub)};
        bool own_pub{inspect(outbound_topic,true,node_name,found.witness_pub)};
        bool own_sub{inspect(inbound_topic,false,node_name,found.witness_sub)};
        return gateway_pub&&gateway_sub&&own_pub&&own_sub;
    }
    struct final_facts
    { unsigned received{},sent{};fast_io::string witness_sub,witness_pub,gateway_pub,gateway_sub; };
}

int main(int argc,char** argv)
{
    using namespace secure_guest_witness;
    std::string_view phase{"input"};
    try
    {
        require(argc==2&&equal(argv[1],keystore));environment();initial_gate();phase="initialization";
        native_logging_owner native_logging;log_owner logging;context_owner context;
        rclcpp::InitOptions init_options;init_options.auto_initialize_logging(false);
        char const* args[]{"secure-witness","--ros-args","--enclave","/witness"};context.value->init(4,args,init_options);attest(context.value);
        final_facts facts;
        {
            observations observed;rclcpp::NodeOptions options;options.context(context.value).use_global_arguments(true);
            options.enable_rosout(false).start_parameter_services(false).start_parameter_event_publisher(false).use_intra_process_comms(false);
            options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
            auto node{std::make_shared<rclcpp::Node>("secure_witness","/uwvm_test",options)};
            require(equal(node->get_fully_qualified_name(),full_node));
            auto support{rosidl_typesupport_cpp::get_message_type_support_handle<message>()};
            require(support&&support->get_type_hash_func);auto hash{support->get_type_hash_func(support)};require(hash&&type_hash(*hash));
            auto qos{configured_qos()};
            auto subscription{node->create_subscription<message>(inbound_topic,qos,
                [&](message::ConstSharedPtr value,rclcpp::MessageInfo const& info) { observed.receive(value,info); })};
            auto publisher{node->create_publisher<message>(outbound_topic,qos)};
            require(actual_qos(subscription->get_actual_qos(),qos)&&actual_qos(publisher->get_actual_qos(),qos));
            auto own_pub{copied(publisher->get_gid())};auto own_pub_hex{hexadecimal(own_pub)};
            fast_io::println(fast_io::out(),"READY_SECURE_WITNESS pid=",::getpid()," node=",full_node,
                " enclave=/witness domain=74 rmw=",implementation," type=",type_name," sub_topic=",std::string_view{inbound_topic},
                " pub_topic=",std::string_view{outbound_topic}," pub_gid=",own_pub_hex," out_bytes=31 in_bytes=23");
            rclcpp::ExecutorOptions executor_options;executor_options.context=context.value;
            rclcpp::executors::SingleThreadedExecutor executor{executor_options};executor.add_node(node);
            phase="discovery";matched_endpoints matched;auto until{clock::now()+std::chrono::seconds{20}};
            while(clock::now()<until)
            {
                executor.spin_some(std::chrono::milliseconds{2});
                if(endpoints(*node,matched)&&subscription->get_publisher_count()==1&&publisher->get_subscription_count()==1) { break; }
                std::this_thread::sleep_for(std::chrono::milliseconds{2});
            }
            require(endpoints(*node,matched)&&subscription->get_publisher_count()==1&&publisher->get_subscription_count()==1&&matched.witness_pub==own_pub);
            observed.selected=true;observed.gateway_pub=matched.gateway_pub;
            require(!observed.received||observed.received_gid==matched.gateway_pub);
            facts.witness_sub=hexadecimal(matched.witness_sub);facts.witness_pub=hexadecimal(matched.witness_pub);
            facts.gateway_pub=hexadecimal(matched.gateway_pub);facts.gateway_sub=hexadecimal(matched.gateway_sub);
            fast_io::println(fast_io::out(),"READY_SECURE_MATCHED pid=",::getpid()," node=",full_node," type=",type_name,
                " sub_topic=",std::string_view{inbound_topic}," pub_topic=",std::string_view{outbound_topic},
                " witness_sub_gid=",facts.witness_sub," witness_pub_gid=",facts.witness_pub,
                " gateway_pub_gid=",facts.gateway_pub," gateway_sub_gid=",facts.gateway_sub,
                " matched_publishers=1 matched_subscribers=1 received=",observed.received," sent=0");
            phase="exchange";message reply;reply.data.assign(reply_data);bool acknowledged{};
            until=clock::now()+std::chrono::seconds{20};auto next{clock::now()};
            while(clock::now()<until)
            {
                executor.spin_some(std::chrono::milliseconds{2});
                require(observed.mismatched==0&&observed.received<=8);
                if(gate('F',0)) { acknowledged=true;break; }
                if(observed.received&&clock::now()>=next)
                {
                    require(observed.sent<100);publisher->publish(reply);++observed.sent;
                    next=clock::now()+std::chrono::milliseconds{100};
                }
                std::this_thread::sleep_for(std::chrono::milliseconds{2});
            }
            require(acknowledged&&observed.received>0&&observed.sent>0&&observed.mismatched==0);
            require(observed.received_gid==matched.gateway_pub&&canonical_reply[0]==reply.data.size());
            phase="signed-denial";signed_denial(*node,qos,support,logging.evidence);
            facts.received=observed.received;facts.sent=observed.sent;
            executor.cancel();executor.remove_node(node);
        }
        // No spinner thread survives this scope. Native entities/node/executor
        // retire before explicit context shutdown; only public metadata remains.
        phase="cleanup";context.close();
        fast_io::println(fast_io::out(),"PASS_SECURE_WITNESS pid=",::getpid()," node=",full_node," type=",type_name,
            " sub_topic=",std::string_view{inbound_topic}," pub_topic=",std::string_view{outbound_topic},
            " witness_sub_gid=",facts.witness_sub," witness_pub_gid=",facts.witness_pub,
            " gateway_pub_gid=",facts.gateway_pub," gateway_sub_gid=",facts.gateway_sub,
            " matched_publishers=1 matched_subscribers=1 received=",facts.received," sent=",facts.sent,
            " mismatched=0 denied_native=1 permission_events=1 out_bytes=31 in_bytes=23");
        return 0;
    }
    catch(...)
    {
        // Raw native exception strings can contain paths or externally supplied
        // values. A fixed phase token identifies failure without disclosing them.
        fast_io::println(fast_io::err(),"FAIL_SECURE_WITNESS phase=",phase);return 1;
    }
}
