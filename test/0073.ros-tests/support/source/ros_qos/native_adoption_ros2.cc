/*************************************************************
 * Actual ROS2 Action RMW profile adoption witness. APL-2.0. *
 *************************************************************/
// This executable alone interposes the four public RMW entity constructors.
// Every call is forwarded unchanged to the real selected RMW. The witness
// queries the returned native handles through official actual-QoS APIs; it
// never substitutes a profile, creates a fake endpoint, or changes a Gateway.
// Link with libdl, rclcpp, rclcpp_action and action_tutorials_interfaces.
// Native profile evidence remains separate from Wasm/Guest admission evidence.
#include "common.h"
#include "../../../../../src/ros/gateway/ros2/action.h"
#include <dlfcn.h>
#include <atomic>
#include <array>
#include <chrono>
#include <charconv>
#include <future>
#include <thread>

namespace adoption
{
    namespace action=uwvm2::ros_gateway::ros2_action;
    namespace gateway=uwvm2::ros_gateway;
    using capability=uwvm2::imported::ros::capability_spec;
    constexpr std::array<char const*,5> channels{{"goal-service","cancel-service","result-service","feedback","status"}};
    constexpr std::array<char const*,5> suffixes{{"/_action/send_goal","/_action/cancel_goal","/_action/get_result","/_action/feedback","/_action/status"}};
    struct capture
    {
        capability const* cap{};
        std::array<unsigned,5> entities{},input_fields{},actual_fields{};
        std::size_t actual_queries{};
        char const* error{};
        bool server{};
        std::array<rmw_qos_profile_t,5> actual{},other_actual{};
        std::array<bool,5> queried_other{};
    };
    thread_local capture* active{};
    template<typename Function> Function original(char const* name)
    { return reinterpret_cast<Function>(::dlsym(RTLD_NEXT,name)); }
    inline rmw_qos_profile_t independent_expected(capability const& cap,char const* channel_name)
    {
        // Do not derive expectations from action::explicit_qos: using that
        // constructor for both sides would hide an ignored/swapped CLI field.
        auto prefix{std::string{"qos."}+channel_name+"."};
        auto property=[&](char const* name)->std::string const& {return cap.limits.at(prefix+name);};
        auto decimal=[](std::string const& text)
        {std::uint64_t result{};auto parsed{std::from_chars(text.data(),text.data()+text.size(),result)};
         if(parsed.ec!=std::errc{}||parsed.ptr!=text.data()+text.size()) {throw std::runtime_error("invalid independent fixture integer");}return result;};
        auto duration=[&](char const* name)->rmw_time_t
        {auto const& text{property(name)};if(text=="infinite") {return RMW_DURATION_INFINITE;}auto value{decimal(text)};return {value/1000000000ULL,value%1000000000ULL};};
        rmw_qos_profile_t profile{};
        action_qos_test::check(property("history")=="keep-last","independent profile requires keep-last");
        profile.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;profile.depth=static_cast<std::size_t>(decimal(property("depth")));
        auto const& reliability{property("reliability")};action_qos_test::check(reliability=="reliable"||reliability=="best-effort","independent reliability differs");
        profile.reliability=reliability=="reliable"?RMW_QOS_POLICY_RELIABILITY_RELIABLE:RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
        auto const& durability{property("durability")};action_qos_test::check(durability=="volatile"||durability=="transient-local","independent durability differs");
        profile.durability=durability=="volatile"?RMW_QOS_POLICY_DURABILITY_VOLATILE:RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL;
        profile.deadline=duration("deadline");profile.lifespan=duration("lifespan");
        auto const& liveliness{property("liveliness")};action_qos_test::check(liveliness=="automatic"||liveliness=="manual-by-topic","independent liveliness differs");
        profile.liveliness=liveliness=="automatic"?RMW_QOS_POLICY_LIVELINESS_AUTOMATIC:RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC;
        profile.liveliness_lease_duration=duration("liveliness-lease-duration");profile.avoid_ros_namespace_conventions=false;return profile;
    }
    inline std::uint32_t field_mask(rmw_qos_profile_t const& a,rmw_qos_profile_t const& b) noexcept
    {
        auto same=[](rmw_time_t x,rmw_time_t y) {return x.sec==y.sec&&x.nsec==y.nsec;};
        return (a.reliability==b.reliability?1u:0u)|(a.durability==b.durability?2u:0u)|
            (a.history==b.history?4u:0u)|(a.depth==b.depth?8u:0u)|
            (same(a.deadline,b.deadline)?16u:0u)|(same(a.lifespan,b.lifespan)?32u:0u)|
            (a.liveliness==b.liveliness?64u:0u)|(same(a.liveliness_lease_duration,b.liveliness_lease_duration)?128u:0u);
    }
    inline int channel(char const* name,bool service,bool server) noexcept
    {
        if(!active||!name||server!=active->server) {return -1;}
        std::string_view endpoint{name},resource{active->cap->resource};
        if(endpoint.substr(0,resource.size())!=resource) {return -1;}
        auto tail{endpoint.substr(resource.size())};
        for(unsigned i{};i!=5;++i)
        {if(tail==suffixes[i]) {if(service!=(i<3)) {active->error="wrong native endpoint category";return -1;}return static_cast<int>(i);}}
        active->error="unexpected native Action endpoint";return -1;
    }
    inline void inspect(int which,rmw_qos_profile_t const* input,rmw_ret_t first,rmw_qos_profile_t const& actual,
        rmw_ret_t second=RMW_RET_OK,rmw_qos_profile_t const* other=nullptr) noexcept
    {
        if(which<0) {return;}
        auto& record{*active};auto index{static_cast<unsigned>(which)};++record.entities[index];
        record.actual[index]=actual;record.queried_other[index]=other!=nullptr;if(other) {record.other_actual[index]=*other;}
        try
        {
            auto expected{independent_expected(*record.cap,channels[index])};
            if(!input||first!=RMW_RET_OK||second!=RMW_RET_OK) {record.error="native QoS query failed";return;}
            auto in{field_mask(*input,expected)},out{field_mask(actual,expected)};
            if(other) {out&=field_mask(*other,expected);}
            // All eight fields are checked independently for every channel.
            // The namespace-convention flag is also fixed by this ROS adapter.
            if(in!=255||out!=255||input->avoid_ros_namespace_conventions||actual.avoid_ros_namespace_conventions||
               (other&&other->avoid_ros_namespace_conventions)) {record.error="actual RMW Action QoS differs from explicit profile";}
            record.input_fields[index]|=in;record.actual_fields[index]|=out;
            record.actual_queries+=other?2:1;
        }
        catch(...) {record.error="profile inspection failed";}
    }
    struct scope
    {
        capture* previous{active};explicit scope(capture& record) {active=&record;}
        ~scope() {active=previous;}
    };
}

extern "C" rmw_client_t* rmw_create_client(rmw_node_t const* node,rosidl_service_type_support_t const* support,
    char const* name,rmw_qos_profile_t const* profile)
{
    using function=rmw_client_t*(*)(rmw_node_t const*,rosidl_service_type_support_t const*,char const*,rmw_qos_profile_t const*);
    static auto real{adoption::original<function>("rmw_create_client")};
    if(!real) {if(adoption::active) {adoption::active->error="missing real rmw_create_client";}return nullptr;}
    auto result{real(node,support,name,profile)};auto index{adoption::channel(name,true,false)};
    if(index>=0)
    {
        rmw_qos_profile_t request{},response{};
        auto a{result?rmw_client_request_publisher_get_actual_qos(result,&request):RMW_RET_ERROR};
        auto b{result?rmw_client_response_subscription_get_actual_qos(result,&response):RMW_RET_ERROR};
        adoption::inspect(index,profile,a,request,b,&response);
    }
    return result;
}
extern "C" rmw_service_t* rmw_create_service(rmw_node_t const* node,rosidl_service_type_support_t const* support,
    char const* name,rmw_qos_profile_t const* profile)
{
    using function=rmw_service_t*(*)(rmw_node_t const*,rosidl_service_type_support_t const*,char const*,rmw_qos_profile_t const*);
    static auto real{adoption::original<function>("rmw_create_service")};
    if(!real) {if(adoption::active) {adoption::active->error="missing real rmw_create_service";}return nullptr;}
    auto result{real(node,support,name,profile)};auto index{adoption::channel(name,true,true)};
    if(index>=0)
    {
        rmw_qos_profile_t request{},response{};
        auto a{result?rmw_service_request_subscription_get_actual_qos(result,&request):RMW_RET_ERROR};
        auto b{result?rmw_service_response_publisher_get_actual_qos(result,&response):RMW_RET_ERROR};
        adoption::inspect(index,profile,a,request,b,&response);
    }
    return result;
}
extern "C" rmw_publisher_t* rmw_create_publisher(rmw_node_t const* node,rosidl_message_type_support_t const* support,
    char const* name,rmw_qos_profile_t const* profile,rmw_publisher_options_t const* options)
{
    using function=rmw_publisher_t*(*)(rmw_node_t const*,rosidl_message_type_support_t const*,char const*,rmw_qos_profile_t const*,rmw_publisher_options_t const*);
    static auto real{adoption::original<function>("rmw_create_publisher")};
    if(!real) {if(adoption::active) {adoption::active->error="missing real rmw_create_publisher";}return nullptr;}
    auto result{real(node,support,name,profile,options)};auto index{adoption::channel(name,false,true)};
    if(index>=0) {rmw_qos_profile_t actual{};adoption::inspect(index,profile,result?rmw_publisher_get_actual_qos(result,&actual):RMW_RET_ERROR,actual);}
    return result;
}
extern "C" rmw_subscription_t* rmw_create_subscription(rmw_node_t const* node,rosidl_message_type_support_t const* support,
    char const* name,rmw_qos_profile_t const* profile,rmw_subscription_options_t const* options)
{
    using function=rmw_subscription_t*(*)(rmw_node_t const*,rosidl_message_type_support_t const*,char const*,rmw_qos_profile_t const*,rmw_subscription_options_t const*);
    static auto real{adoption::original<function>("rmw_create_subscription")};
    if(!real) {if(adoption::active) {adoption::active->error="missing real rmw_create_subscription";}return nullptr;}
    auto result{real(node,support,name,profile,options)};auto index{adoption::channel(name,false,false)};
    if(index>=0) {rmw_qos_profile_t actual{};adoption::inspect(index,profile,result?rmw_subscription_get_actual_qos(result,&actual):RMW_RET_ERROR,actual);}
    return result;
}

namespace adoption
{
    inline void raw_profile(char const* role,char const* channel_name,char const* side,rmw_qos_profile_t const& profile)
    {
        gateway::native_io::fixed_text<512> line;
        bool valid{line.append("RMW_ADOPTION_QOS ")&&line.append(role)&&line.append(" ")&&line.append(channel_name)&&line.append(" ")&&line.append(side)};
        auto scalar=[&](char const* name,auto value) {valid=valid&&line.append(" ")&&line.append(name)&&line.append("=")&&line.number(value);};
        scalar("reliability",static_cast<unsigned>(profile.reliability));scalar("durability",static_cast<unsigned>(profile.durability));
        scalar("history",static_cast<unsigned>(profile.history));scalar("depth",profile.depth);
        scalar("deadline-sec",profile.deadline.sec);scalar("deadline-nsec",profile.deadline.nsec);
        scalar("lifespan-sec",profile.lifespan.sec);scalar("lifespan-nsec",profile.lifespan.nsec);
        scalar("liveliness",static_cast<unsigned>(profile.liveliness));scalar("lease-sec",profile.liveliness_lease_duration.sec);
        scalar("lease-nsec",profile.liveliness_lease_duration.nsec);scalar("namespace-flag",static_cast<unsigned>(profile.avoid_ros_namespace_conventions));
        if(!valid||!line.append("\n")||!gateway::native_io::write_all(STDOUT_FILENO,line.view())) {throw std::runtime_error("cannot write actual native profile");}
    }
    inline void require_capture(capture const& record)
    {
        for(unsigned i{};i!=5;++i)
        {
            raw_profile(record.server?"server":"client",channels[i],record.queried_other[i]?"first":"topic",record.actual[i]);
            if(record.queried_other[i]) {raw_profile(record.server?"server":"client",channels[i],"second",record.other_actual[i]);}
        }
        if(record.error) {throw std::runtime_error(record.error);}
        for(unsigned i{};i!=5;++i)
        {action_qos_test::check(record.entities[i]==1&&record.input_fields[i]==255&&record.actual_fields[i]==255,
            "all five real Action endpoint profiles must be observed exactly once");}
        action_qos_test::check(record.actual_queries==8,"each service request/response and both topic profiles must be queried");
    }
    inline rcl_action_client_options_t client_options(capability const& cap)
    {
        rcl_action_client_options_t options{};options.allocator=rcl_get_default_allocator();
        options.goal_service_qos=action::explicit_qos(cap,"goal-service");options.cancel_service_qos=action::explicit_qos(cap,"cancel-service");
        options.result_service_qos=action::explicit_qos(cap,"result-service");options.feedback_topic_qos=action::explicit_qos(cap,"feedback");
        options.status_topic_qos=action::explicit_qos(cap,"status");return options;
    }
    inline rcl_action_server_options_t server_options(capability const& cap)
    {
        auto client{client_options(cap)};rcl_action_server_options_t options{};options.allocator=rcl_get_default_allocator();
        options.goal_service_qos=client.goal_service_qos;options.cancel_service_qos=client.cancel_service_qos;
        options.result_service_qos=client.result_service_qos;options.feedback_topic_qos=client.feedback_topic_qos;
        options.status_topic_qos=client.status_topic_qos;
        options.result_timeout.nanoseconds=static_cast<std::int64_t>(gateway::limit(cap,"timeout-ms"))*1000000;
        return options;
    }
    inline std::size_t profile_only(capability const& cap,rclcpp::Node::SharedPtr const& client_node,
        rclcpp::Node::SharedPtr const& server_node)
    {
        // Finite one-nanosecond profiles need not deliver business traffic.
        // This independent matrix observes their actual native adoption only;
        // the baseline below separately proves goal/feedback/result delivery.
        using native=action::fibonacci;capture server_record{&cap,{}, {}, {},0,nullptr,true},client_record{&cap};
        rclcpp_action::Server<native>::SharedPtr server;rclcpp_action::Client<native>::SharedPtr client;
        {
            scope observing{server_record};
            server=rclcpp_action::create_server<native>(server_node,cap.resource,
                [](rclcpp_action::GoalUUID const&,std::shared_ptr<native::Goal const>) {return rclcpp_action::GoalResponse::REJECT;},
                [](std::shared_ptr<rclcpp_action::ServerGoalHandle<native>>) {return rclcpp_action::CancelResponse::REJECT;},
                [](std::shared_ptr<rclcpp_action::ServerGoalHandle<native>>) {},server_options(cap));
        }
        {scope observing{client_record};client=rclcpp_action::create_client<native>(client_node,cap.resource,nullptr,client_options(cap));}
        require_capture(client_record);require_capture(server_record);
        return client_record.actual_queries+server_record.actual_queries;
    }
    int run(int argc,char** argv)
    {
        using action_type=action::fibonacci;using clock=std::chrono::steady_clock;
        auto client_context{std::make_shared<rclcpp::Context>()},server_context{std::make_shared<rclcpp::Context>()};
        struct shutdown_contexts
        {
            std::shared_ptr<rclcpp::Context> a,b;
            ~shutdown_contexts() {a->shutdown("adoption fixture cleanup");b->shutdown("adoption fixture cleanup");}
        } contexts{client_context,server_context};
        client_context->init(argc,argv);server_context->init(argc,argv);
        auto node_options=[](std::shared_ptr<rclcpp::Context> context)
        {rclcpp::NodeOptions options;options.context(context);options.use_intra_process_comms(false);options.enable_rosout(false);
         options.start_parameter_services(false);options.start_parameter_event_publisher(false);options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});return options;};
        auto client_node{std::make_shared<rclcpp::Node>("adoption_client","/uwvm_action_qos",node_options(client_context))};
        auto server_node{std::make_shared<rclcpp::Node>("adoption_server","/uwvm_action_qos",node_options(server_context))};
        auto cap{action_qos_test::capability("adoption","action.send-goal","/uwvm_action_qos/adoption")};
        // Distinct nondefault depths make every channel's applied profile
        // distinguishable. Other properties are explicit matrix values.
        for(unsigned i{};i!=5;++i)
        {gateway::native_io::fixed_text<8> depth;if(!depth.number(11+i)) {throw std::length_error("fixture depth overflow");}cap.limits[std::string{"qos."}+channels[i]+".depth"]=std::string{depth.view()};}
        gateway::validate(uwvm2::imported::ros::version::ros2,cap);
        unsigned mutated_profiles{},invalid_profiles{};std::size_t profile_queries{};
        for(auto const& row:action_qos_test::mutations)
        {
            auto selected{cap};selected.limits[row.key]=row.value;
            gateway::native_io::fixed_text<256> begin;
            if(!begin.append("RMW_ADOPTION_BEGIN ")||!begin.append(row.key)||!begin.append(" ")||!begin.append(row.value)||
               !begin.append("\n")||!gateway::native_io::write_all(STDOUT_FILENO,begin.view())) {throw std::runtime_error("cannot write native mutation selection");}
            gateway::native_io::fixed_text<64> resource;
            if(!resource.append("/uwvm_action_qos/adoption_mutation_")||!resource.number(mutated_profiles+invalid_profiles))
            {throw std::length_error("fixture resource overflow");}selected.resource=std::string{resource.view()};
            bool accepted{};try {gateway::validate(uwvm2::imported::ros::version::ros2,selected);accepted=true;}
            catch(std::runtime_error const&) {}
            action_qos_test::check(accepted==row.valid,"independent native matrix configuration classification differs");
            if(!accepted) {++invalid_profiles;continue;}
            profile_queries+=profile_only(selected,client_node,server_node);++mutated_profiles;
            gateway::native_io::fixed_text<256> line;
            if(!line.append("RMW_ADOPTION_MUTATION ")||!line.append(row.key)||!line.append(" ")||!line.append(row.value)||
               !line.append(" client_fields=40 server_fields=40 actual_profile_queries=16\n")||
               !gateway::native_io::write_all(STDOUT_FILENO,line.view())) {throw std::runtime_error("cannot write native mutation witness");}
        }
        action_qos_test::check(mutated_profiles==35&&invalid_profiles==5&&profile_queries==560,"native forty-field matrix is incomplete");
        capture server_record{&cap,{}, {}, {},0,nullptr,true},client_record{&cap};
        std::atomic<unsigned> goals{},feedback{};rclcpp_action::Server<action_type>::SharedPtr server;
        auto options{client_options(cap)};auto selected_server_options{server_options(cap)};
        {
            scope observing{server_record};
            server=rclcpp_action::create_server<action_type>(server_node,cap.resource,
                [&](rclcpp_action::GoalUUID const&,std::shared_ptr<action_type::Goal const> goal)
                {if(goal->order!=3) {return rclcpp_action::GoalResponse::REJECT;}++goals;return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;},
                [](std::shared_ptr<rclcpp_action::ServerGoalHandle<action_type>>) {return rclcpp_action::CancelResponse::REJECT;},
                [&](std::shared_ptr<rclcpp_action::ServerGoalHandle<action_type>> handle)
                {
                    auto value{std::make_shared<action_type::Feedback>()};value->partial_sequence={0,1};handle->publish_feedback(value);
                    auto until{clock::now()+std::chrono::seconds(3)};
                    while(!feedback.load()&&clock::now()<until) {std::this_thread::sleep_for(std::chrono::milliseconds(2));}
                    auto result{std::make_shared<action_type::Result>()};result->sequence={0,1,1};handle->succeed(result);
                },selected_server_options);
        }
        rclcpp_action::Client<action_type>::SharedPtr client;
        {scope observing{client_record};client=rclcpp_action::create_client<action_type>(client_node,cap.resource,nullptr,options);}
        require_capture(client_record);require_capture(server_record);
        rclcpp::ExecutorOptions client_exec_options,server_exec_options;client_exec_options.context=client_context;server_exec_options.context=server_context;
        rclcpp::executors::SingleThreadedExecutor client_executor{client_exec_options},server_executor{server_exec_options};
        client_executor.add_node(client_node);server_executor.add_node(server_node);
        // Establish teardown before either constructor can fail. A second
        // thread creation failure must still cancel and join the first one.
        std::thread a,b;
        struct stop_executors
        {
            rclcpp::Executor& a; rclcpp::Executor& b;std::thread& x;std::thread& y;
            ~stop_executors() {a.cancel();b.cancel();if(x.joinable()) {x.join();}if(y.joinable()) {y.join();}}
        } stop{client_executor,server_executor,a,b};
        a=std::thread{[&]{client_executor.spin();}};b=std::thread{[&]{server_executor.spin();}};
        action_qos_test::check(client->wait_for_action_server(std::chrono::seconds(8)),"native Action discovery failed");
        rclcpp_action::Client<action_type>::SendGoalOptions send;
        send.feedback_callback=[&](rclcpp_action::ClientGoalHandle<action_type>::SharedPtr,std::shared_ptr<action_type::Feedback const> value)
        {if(value->partial_sequence==std::vector<std::int32_t>{0,1}) {++feedback;}};
        action_type::Goal goal;goal.order=3;auto accepted{client->async_send_goal(goal,send)};
        action_qos_test::check(accepted.wait_for(std::chrono::seconds(8))==std::future_status::ready,"native goal acknowledgement timed out");
        auto handle{accepted.get()};action_qos_test::check(handle!=nullptr,"native goal was rejected");
        auto result{client->async_get_result(handle)};
        action_qos_test::check(result.wait_for(std::chrono::seconds(8))==std::future_status::ready,"native result timed out");
        auto terminal{result.get()};
        action_qos_test::check(terminal.code==rclcpp_action::ResultCode::SUCCEEDED&&terminal.result&&terminal.result->sequence==std::vector<std::int32_t>{0,1,1}&&goals.load()==1&&feedback.load()==1,
            "native goal/feedback/result counts or contents differ");
        gateway::native_io::fixed_text<512> record;
        record.append("PASS actual Action RMW adoption client_fields=40 server_fields=40 mutated_profiles=35 invalid_profiles=5 actual_profile_queries=576 native_goals=1 native_feedback=1 native_result=1\n");
        if(!gateway::native_io::write_all(STDOUT_FILENO,record.view())) {throw std::runtime_error("cannot write adoption witness");}
        return 0;
    }
}
int main(int argc,char** argv)
{
    try {return adoption::run(argc,argv);}
    catch(std::exception const& error) {adoption::active=nullptr;uwvm2::ros_gateway::native_io::diagnostic("FAIL actual Action RMW adoption ",error.what());return 1;}
}
