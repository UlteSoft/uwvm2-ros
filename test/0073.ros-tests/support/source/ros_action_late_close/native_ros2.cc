#include "../ros_action/ros2_common.h"
#include <rclcpp_action/rclcpp_action.hpp>
#include <action_tutorials_interfaces/action/fibonacci.hpp>
#include <rclcpp/executors/single_threaded_executor.hpp>
#include <fast_io.h>
#include <stdexcept>

namespace late_close_probe
{
    using namespace action_probe;
    using fibonacci=action_tutorials_interfaces::action::Fibonacci;
    using goal_ptr=std::shared_ptr<rclcpp_action::ServerGoalHandle<fibonacci>>;
    struct local_fixture
    {
        gateway::configuration config;
        std::string directory,config_file;
        pid_t gateway_pid{-1};
        explicit local_fixture(std::string const& executable,std::function<void(gateway::configuration&)> configure)
        {
            char path[]{"/tmp/uwvm-ros-late-close-XXXXXX"};auto dir{::mkdtemp(path)};require(dir,"private fixture directory");directory=dir;
            config.policy.enabled=true;config.policy.ros_version=core::version::ros2;
            config.worker_uid=::getuid();config.worker_pid=::getpid();config.timeout_ms=10000;config.max_sessions=1;
            config.socket=directory+"/gateway.sock";config_file=directory+"/policy.txt";
            config.audit_records_per_second=100;config.audit_max_records=10000;config.audit_max_bytes=1048576;
            config.policy.connection={{"node-name","unused"},{"namespace","/uwvm_test"},{"domain-id","199"},{"rmw-implementation","rmw_fastrtps_cpp"},{"security-mode","disabled-test"},{"keystore","not-applicable"},{"enclave","not-applicable"}};
            configure(config);
            {
                fast_io::native_file file{fast_io::mnp::os_c_str(config_file.c_str()),fast_io::open_mode::out};
                fast_io::io::println(file,"backend ros2\nworker-uid ",config.worker_uid,"\nworker-pid ",config.worker_pid,"\nsocket ",config.socket,"\ntimeout-ms 10000\nmax-sessions ",config.max_sessions,"\naudit-max-records-per-second 100\naudit-max-records 10000\naudit-max-bytes 1048576");
                for(auto const& entry:config.policy.connection) { fast_io::io::println(file,"connection ",entry.first," ",entry.second); }
                for(auto const& entry:config.policy.capabilities)
                { auto const& c{entry.second};fast_io::io::println(file,"grant ",c.alias," ",c.operation," ",c.resource," ",c.type," ",c.type_identity);for(auto const& limit:c.limits) { fast_io::io::println(file,"set ",c.alias," ",limit.first," ",limit.second); } }
            }
            require(::chmod(config_file.c_str(),0600)==0,"private policy file");
            gateway_pid=::fork();require(gateway_pid>=0,"fork Gateway before ROS init");
            if(!gateway_pid) { ::execl(executable.c_str(),executable.c_str(),config_file.c_str(),nullptr);::_exit(127); }
            for(unsigned n{};n!=500;++n) { struct stat metadata{};if(!::lstat(config.socket.c_str(),&metadata)) { return; }std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
            ::kill(gateway_pid,SIGKILL);::waitpid(gateway_pid,nullptr,0);gateway_pid=-1;
            ::unlink(config_file.c_str());::rmdir(directory.c_str());throw std::runtime_error("Gateway startup timeout");
        }
        void connect(ipc::client& rpc)
        {
            // A completed connection is reaped by the bounded server loop.
            // Only handshake/rebind readiness is retried; no Action call is
            // issued until this real max_sessions=1 slot becomes available.
            auto until{std::chrono::steady_clock::now()+std::chrono::seconds(2)};std::uint32_t error{};
            do
            {
                error=rpc.connect(config.socket,2,config.worker_uid,1000);
                if(!error) { break; }
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }while(std::chrono::steady_clock::now()<until);
            require(!error,"bounded authenticated IPC reconnect");
            require(!rpc.call("session.bind","target",core::binding_manifest(config.policy),1000).error,"exact rebound policy manifest");
        }
        ~local_fixture()
        {
            if(gateway_pid>0)
            { ::kill(gateway_pid,SIGTERM);for(unsigned n{};n!=200;++n) { if(::waitpid(gateway_pid,nullptr,WNOHANG)==gateway_pid) { gateway_pid=-1;break; }std::this_thread::sleep_for(std::chrono::milliseconds(10)); }if(gateway_pid>0) { ::kill(gateway_pid,SIGKILL);::waitpid(gateway_pid,nullptr,0); } }
            ::unlink(config.socket.c_str());::unlink(config_file.c_str());::rmdir(directory.c_str());
        }
    };
    struct state
    {
        std::mutex mutex;
        std::condition_variable changed;
        bool entered{},release{},gate_expired{},first_decided{},first_rejected{},cancel_release{},cancel_gate_expired{};
        std::atomic<bool> stop{},executor_failed{};
        unsigned requests{},first_cancels{},other_cancels{};
        goal_ptr first;
        std::vector<goal_ptr> all;
        void open() { { std::lock_guard<std::mutex> lock{mutex};release=true; }changed.notify_all(); }
        void open_cancel() { { std::lock_guard<std::mutex> lock{mutex};cancel_release=true; }changed.notify_all(); }
    };
    struct cleanup
    {
        state& shared;
        rclcpp::executors::SingleThreadedExecutor& executor;
        std::thread& spinner;
        ~cleanup()
        {
            shared.open();shared.open_cancel();
            std::vector<goal_ptr> goals;
            { std::lock_guard<std::mutex> lock{shared.mutex};goals=shared.all; }
            for(auto const& goal:goals)
            {
                try { if(goal->is_active()) { auto result=std::make_shared<fibonacci::Result>();if(goal->is_canceling()) { goal->canceled(result); }else { goal->abort(result); } } }
                catch(std::exception const&) {}
            }
            shared.stop=true;executor.cancel();if(spinner.joinable()) { spinner.join(); }
        }
    };
    template<typename Predicate> bool wait(state& shared,std::chrono::milliseconds budget,Predicate predicate)
    { std::unique_lock<std::mutex> lock{shared.mutex};return shared.changed.wait_for(lock,budget,predicate); }

    int run(int argc,char** argv)
    {
        require(argc==3,"usage native-action-late-close gateway accept|reject|accepted|disconnect|refuse|expired|expired-owner");
        std::string mode{argv[2]};require(mode=="accept"||mode=="reject"||mode=="accepted"||mode=="disconnect"||mode=="refuse"||mode=="expired"||mode=="expired-owner","unknown late-close mode");
        bool owner_case{mode=="expired-owner"},expired{mode=="expired"||owner_case},normal{mode=="accepted"||expired},reject{mode=="reject"},refuse{mode=="refuse"};
        auto ros_mode{mode};std::replace(ros_mode.begin(),ros_mode.end(),'-','_');
        auto resource{"/uwvm_action_late_close/"+ros_mode+"_"+std::to_string(::getpid())};
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
        local_fixture session{argv[1],[&](gateway::configuration& config) {
            config.policy.capabilities.clear();
            if(owner_case) { config.max_sessions=2; }
            config.policy.connection["domain-id"]=::getenv("ROS_DOMAIN_ID")?::getenv("ROS_DOMAIN_ID"):"199";
            config.policy.connection["node-name"]="gateway_action_late_close_"+ros_mode;
            for(auto const& item:std::vector<std::pair<std::string,std::string>>{{"send","action.send-goal"},{"result","action.receive-result"}})
            {
                auto cap{action(core::version::ros2,item.first,item.second,resource)};
                cap.limits["max-active-goals"]="1";cap.limits["max-actions"]="2";
                cap.limits["timeout-ms"]=expired?"500":!normal?"100":"1000";
                config.policy.capabilities.emplace(cap.alias,std::move(cap));
            }
        }};
        ipc::client rpc;session.connect(rpc);
        int init_argc{1};rclcpp::init(init_argc,argv);
        auto options{rclcpp::NodeOptions{}.start_parameter_services(false).start_parameter_event_publisher(false).enable_rosout(false)};
        options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
        auto native{std::make_shared<rclcpp::Node>("native_action_late_close_"+ros_mode,options)};
        state shared;
        auto server{rclcpp_action::create_server<fibonacci>(native,resource,
            [&](rclcpp_action::GoalUUID const&,std::shared_ptr<fibonacci::Goal const> goal) {
                std::unique_lock<std::mutex> lock{shared.mutex};++shared.requests;
                if(goal->order==1&&!normal)
                {
                    shared.entered=true;shared.changed.notify_all();
                    if(!shared.changed.wait_for(lock,std::chrono::seconds(5),[&] { return shared.release; }))
                    { shared.gate_expired=true;return rclcpp_action::GoalResponse::REJECT; }
                }
                if(goal->order==1) { shared.first_decided=true;shared.first_rejected=reject;shared.changed.notify_all(); }
                return goal->order==1&&reject?rclcpp_action::GoalResponse::REJECT:rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
            },
            [&](goal_ptr goal) {
                std::unique_lock<std::mutex> lock{shared.mutex};
                if(goal->get_goal()->order==1) { ++shared.first_cancels; }else { ++shared.other_cancels; }
                shared.changed.notify_all();
                if((!normal||expired)&&goal->get_goal()->order==1&&!shared.changed.wait_for(lock,std::chrono::seconds(5),[&] { return shared.cancel_release; })) { shared.cancel_gate_expired=true; }
                return refuse?rclcpp_action::CancelResponse::REJECT:rclcpp_action::CancelResponse::ACCEPT;
            },
            [&](goal_ptr goal) {
                { std::lock_guard<std::mutex> lock{shared.mutex};shared.all.push_back(goal);if(goal->get_goal()->order==1) { shared.first=goal; } }shared.changed.notify_all();
                if(goal->get_goal()->order==3)
                { auto result=std::make_shared<fibonacci::Result>();result->sequence={0,1};goal->succeed(result); }
            })};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(native);
        std::thread spinner{[&] {
            try { while(!shared.stop&&rclcpp::ok()) { executor.spin_once(std::chrono::milliseconds(10)); } }
            catch(...) { shared.executor_failed=true;shared.changed.notify_all(); }
        }};
        cleanup guard{shared,executor,spinner};
        // The existing Gateway participant is discovered before creation of its
        // lazy Action client. The acceptance callback itself is the core barrier.
                auto until{std::chrono::steady_clock::now()+std::chrono::seconds(3)};
        bool discovered{};
        while(std::chrono::steady_clock::now()<until)
        {
            auto names{native->get_node_names()};
            if(std::find(names.begin(),names.end(),"/uwvm_test/gateway_action_late_close_"+ros_mode)!=names.end()) { discovered=true;break; }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        require(discovered,"native graph did not discover real Gateway participant");
        auto sent{rpc.call("action.send-goal","send",number(1,4),1500)};
        if(normal) { require(!sent.error&&sent.payload.size()==8,"accepted-handle normal send"); }
        else
        {
            require(wait(shared,std::chrono::milliseconds(500),[&] { return shared.entered&&!shared.gate_expired; }),"100ms native goal request never entered pre-ACCEPT gate");
            require(sent.error==110&&sent.payload.empty(),"native acceptance gate did not exceed100ms capability call budget");
        }
        if(expired)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(650));
            auto timed_out=rpc.call("action.receive-result","result",sent.payload,1500);
            fast_io::io::println("OBSERVED action-late-close-expired mode=",mode," accepted_send_error=",sent.error," explicit_goal_timeout_ms=500 expired_receive_error=",timed_out.error);
            require(timed_out.error==110&&timed_out.payload.empty(),"accepted native goal expiry must consume local result with110");
            if(owner_case)
            {
                auto retired=rpc.call("action.receive-result","result",sent.payload,1500);
                fast_io::io::println("OBSERVED action-late-close-owner mode=",mode," explicit_max_sessions=",session.config.max_sessions," global_capacity=2 owner_max_active_goals=1 retired_goal_error=",retired.error);
                require(retired.error==9&&retired.payload.empty(),"retired guest GoalID must remain inaccessible");
            }
        }
        else if(mode=="disconnect") { rpc.close();session.connect(rpc); }
        else { require(!rpc.call("capability.close","send",{},1000).error,"same authenticated session capability close"); }
        ipc::call_result queued;
        if(!normal||expired) { queued=rpc.call("action.send-goal","send",number(2,4),1500); }
        shared.open();
        require(wait(shared,std::chrono::seconds(2),[&] { return reject?shared.first_decided:bool(shared.first); }),"native late acceptance/rejection decision ACK");
        ipc::call_result cancel_queued;
        if(!reject)
        {
            require(wait(shared,std::chrono::seconds(2),[&] { return shared.first_cancels!=0; }),"exact native first-goal cancel request ACK");
            if(!normal||expired)
            {
                cancel_queued=rpc.call("action.send-goal","send",number(2,4),1500);
                require(cancel_queued.error==11&&cancel_queued.payload.empty(),"cleanup capacity released before native cancel reply");
                shared.open_cancel();
            }
            if(!refuse)
            {
                until=std::chrono::steady_clock::now()+std::chrono::seconds(2);
                goal_ptr acknowledged;
                { std::lock_guard<std::mutex> lock{shared.mutex};acknowledged=shared.first; }
                while(acknowledged&&!acknowledged->is_canceling()&&std::chrono::steady_clock::now()<until) { std::this_thread::sleep_for(std::chrono::milliseconds(5)); }
                require(acknowledged&&acknowledged->is_canceling(),"actual native accepted cancel transition ACK");
            }
        }
        // After the acceptance gate opens, allow executor dispatch of any
        // incorrectly queued second DDS request before sampling the count.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        unsigned first_cancel{},other_cancel{},requests{},native_accepted{};bool first_active{},first_canceling{},gate_expired{},first_rejected{};goal_ptr first;
        {
            std::lock_guard<std::mutex> lock{shared.mutex};first_cancel=shared.first_cancels;other_cancel=shared.other_cancels;requests=shared.requests;gate_expired=shared.gate_expired;first=shared.first;native_accepted=static_cast<unsigned>(shared.all.size());first_rejected=shared.first_rejected;
        }
        // Native callbacks may hold SDK goal locks before the state mutex.
        // Inspect SDK status after releasing that mutex to preserve lock order.
        if(first) { first_active=first->is_active();first_canceling=first->is_canceling(); }
        fast_io::io::println("OBSERVED action-late-close mode=",mode," max_sessions=",session.config.max_sessions," explicit_send_caps=1 max_active_goals=1"," send_error=",sent.error," queued_error=",queued.error," cancel_queued_error=",cancel_queued.error," cancellation_refused=",refuse," native_goal_requests=",requests," native_accepted=",native_accepted," first_rejected=",first_rejected," first_cancel=",first_cancel," other_cancel=",other_cancel," first_active=",first_active," first_canceling=",first_canceling," gate_expired=",gate_expired);
        require(!gate_expired&&!shared.cancel_gate_expired&&!shared.executor_failed,"bounded acceptance/executor fixture failed");
        if(!normal||expired)
        {
            require(queued.error==11&&queued.payload.empty()&&requests==1&&other_cancel==0&&(reject?first_cancel==0:first_cancel==1&&(refuse?first_active&&!first_canceling:first_canceling)),
                "closed pending Action acceptance was orphaned or released its quota");
        }
        else { require(first_cancel==1&&first_canceling&&other_cancel==0&&requests==1,"normal accepted-handle close did not cancel native goal"); }
        // A cancel response does not end the native goal. Retain the C=1
        // lifetime slot until an actual native terminal Result is received.
        ipc::call_result terminal_queued;
        if(!reject)
        {
            require(first&&first_active,"native goal must still be active before terminal Result");
            terminal_queued=rpc.call("action.send-goal","send",number(2,4),1500);
            fast_io::io::println("OBSERVED action-late-close-terminal-gate mode=",mode," post_cancel_ack_error=",terminal_queued.error," first_active=",first->is_active()," first_canceling=",first->is_canceling()," cancellation_refused=",refuse);
            require(terminal_queued.error==11&&terminal_queued.payload.empty(),"native active goal lifetime capacity released before terminal Result");
            auto retired=std::make_shared<fibonacci::Result>();retired->sequence={0,1};
            if(refuse) { first->abort(retired); }else { first->canceled(retired); }
            require(!first->is_active(),"actual native terminal Result transition");
            fast_io::io::println("OBSERVED action-late-close-terminal mode=",mode," native_terminal_kind=",refuse?3u:4u," native_first_active=",first->is_active()," native_terminal_values=",retired->sequence.size());
        }
        // A late rejected acceptance or actual terminal Result releases its
        // lifetime slot. EAGAIN retries emit no DDS request or creation quota.
        ipc::call_result retry;
        until=std::chrono::steady_clock::now()+std::chrono::seconds(2);
        do
        {
            retry=rpc.call("action.send-goal","send",number(3,4),1500);
            if(retry.error!=11) { break; }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }while(std::chrono::steady_clock::now()<until);
        require(!retry.error&&retry.payload.size()==8,"late response did not release same-resource goal quota for retry");
        auto result{poll(rpc,"action.receive-result","result",retry.payload)};ipc::action_sequence parsed;
        require(!result.error&&ipc::decode_action_sequence(result.payload,true,parsed)&&parsed.kind==2&&parsed.values==std::vector<std::int32_t>{0,1},"fresh post-close same-resource result exact sequence");
        unsigned final_requests{},final_first_cancels{},final_other_cancels{};
        { std::lock_guard<std::mutex> lock{shared.mutex};final_requests=shared.requests;final_first_cancels=shared.first_cancels;final_other_cancels=shared.other_cancels; }
        require(final_requests==2&&final_first_cancels==(reject?0u:1u)&&final_other_cancels==0,"exact request/cancel counts after successful retry");
        fast_io::io::println("OBSERVED action-late-close-retry mode=",mode," retry_error=",retry.error," result_kind=",parsed.kind," result_values=",parsed.values.size()," final_native_goal_requests=",final_requests," final_first_cancel=",final_first_cancels," final_other_cancel=",final_other_cancels," cancellation_refused=",refuse);
        rpc.close();
        fast_io::io::println("PASS action-late-close mode=",mode," pre-ACCEPT100ms/accepted-handle, close/disconnect/expired result native lifetime quota through terminal Result, actual native cancel/reject/refusal, same-resource exact retry");
        return 0;
    }
}
int main(int argc,char** argv)
{
    try { auto result=late_close_probe::run(argc,argv);rclcpp::shutdown();return result; }
    catch(std::exception const& error)
    { if(rclcpp::ok()) { rclcpp::shutdown(); }fast_io::io::println(fast_io::err(),"FAIL ",fast_io::mnp::os_c_str(error.what()));return 1; }
}
