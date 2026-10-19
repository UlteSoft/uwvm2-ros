#pragma once
#include <cerrno>
#include <exception>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

namespace ros2_action_ack_probe
{
    struct descriptor
    {
        int value{-1};
        descriptor()=default;
        explicit descriptor(int fd):value{fd} {}
        descriptor(descriptor const&)=delete;
        descriptor& operator=(descriptor const&)=delete;
        ~descriptor() { if(value>=0) { ::close(value); } }
    };

    class ack_gate
    {
        descriptor directory_;
        static void private_directory(int fd)
        {
            struct stat metadata{};
            require(fd>=0&&::fstat(fd,&metadata)==0&&S_ISDIR(metadata.st_mode)&&
                    metadata.st_uid==::geteuid()&&(metadata.st_mode&07777)==0700,
                    "Action ACK directory must be runner-owned and mode0700");
        }
    public:
        explicit ack_gate(std::string const& path)
        {
            std::string const prefix{"/uwvm-ros-test/"},suffix{"/action-goal-ready.ack"};
            require(path.size()>prefix.size()+suffix.size()&&path.compare(0,prefix.size(),prefix)==0&&
                    path.compare(path.size()-suffix.size(),suffix.size(),suffix)==0,
                    "Action ACK path must use the private case directory and fixed filename");
            auto name{path.substr(prefix.size(),path.size()-prefix.size()-suffix.size())};
            require(ipc::valid_token(name)&&name!="."&&name!="..","Action ACK case directory token");
            descriptor root{::open("/uwvm-ros-test",O_RDONLY|O_DIRECTORY|O_CLOEXEC|O_NOFOLLOW)};
            private_directory(root.value);
            directory_.value=::openat(root.value,name.c_str(),O_RDONLY|O_DIRECTORY|O_CLOEXEC|O_NOFOLLOW);
            private_directory(directory_.value);
            struct stat existing{};
            auto rc{::fstatat(directory_.value,"action-goal-ready.ack",&existing,AT_SYMLINK_NOFOLLOW)};
            require(rc<0&&errno==ENOENT,"Action ACK must initially be absent; stale files are rejected");
        }
        bool ready() const
        {
            descriptor file{::openat(directory_.value,"action-goal-ready.ack",O_RDONLY|O_CLOEXEC|O_NOFOLLOW|O_NONBLOCK)};
            if(file.value<0&&errno==ENOENT) { return false; }
            require(file.value>=0,"Action ACK file open failed or followed an unsafe path");
            struct stat metadata{};
            require(::fstat(file.value,&metadata)==0&&S_ISREG(metadata.st_mode)&&metadata.st_uid==::geteuid()&&
                    (metadata.st_mode&07777)==0600&&metadata.st_nlink==1&&metadata.st_size==11,
                    "Action ACK must be a runner-owned mode0600 regular file of exactly11bytes");
            char contents[12]{};std::size_t used{};
            while(used!=sizeof(contents))
            {
                auto count{::read(file.value,contents+used,sizeof(contents)-used)};
                if(count<0&&errno==EINTR) { continue; }
                require(count>=0,"Action ACK read failed");
                if(!count) { break; }
                used+=static_cast<std::size_t>(count);
            }
            require(used==11&&std::string{contents,used}=="goal-ready\n","Action ACK has incorrect contents");
            return true;
        }
    };

    struct state
    {
        std::atomic<bool> stop{},done{},failed{};
        std::atomic<unsigned> goal_requests{},feedback_publications{};
        std::mutex failure_mutex,worker_mutex;
        std::exception_ptr failure;
        std::thread worker;
        void fail(std::exception_ptr error)
        {
            std::lock_guard<std::mutex> guard{failure_mutex};
            if(!failure) { failure=std::move(error); }
            failed=true;
        }
    };

    inline int run(int argc,char** argv)
    {
        require(argc==4,"usage Action witness --serve-native-ack absolute-resource /uwvm-ros-test/case/action-goal-ready.ack");
        auto resource{std::string{argv[2]}},path{std::string{argv[3]}};
        require(gateway::canonical(resource),"canonical witness Action resource");
        auto gate{std::make_shared<ack_gate>(path)};
        auto shared{std::make_shared<state>()};
        std::vector<std::int32_t> const expected{0,1,1,2,3,5};
        int init_argc{1};rclcpp::init(init_argc,argv);
        auto node{std::make_shared<rclcpp::Node>("uwvm_ros2_action_ack_network_witness")};
        using feedback_message=fibonacci::Impl::FeedbackMessage;
        // Test-only generated Action feedback writer: public Publisher API exposes
        // actual DDS subscription matching, unlike Jazzy's opaque Action ServerBase.
        // The ordinary server below continues to own Goal acceptance and Result.
        auto publisher{node->create_publisher<feedback_message>(resource+"/_action/feedback",
            rclcpp::QoS{rclcpp::KeepLast{10}}.reliable().durability_volatile())};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
        std::thread spinner;
        rclcpp_action::Server<fibonacci>::SharedPtr server;
        auto cleanup=[&] {
            shared->stop=true;
            executor.cancel();
            if(spinner.joinable()) { spinner.join(); }
            std::thread worker;
            { std::lock_guard<std::mutex> guard{shared->worker_mutex};worker=std::move(shared->worker); }
            if(worker.joinable()) { worker.join(); }
            server.reset();publisher.reset();rclcpp::shutdown();
        };
        try
        {
            server=rclcpp_action::create_server<fibonacci>(node,resource,
                [shared](auto const&,std::shared_ptr<fibonacci::Goal const> goal) {
                    if(++shared->goal_requests!=1||goal->order!=5)
                    {
                        shared->fail(std::make_exception_ptr(std::runtime_error{"Action ACK witness requires exactly one order5 goal"}));
                        return rclcpp_action::GoalResponse::REJECT;
                    }
                    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
                },[](auto) { return rclcpp_action::CancelResponse::REJECT; },
                [shared,gate,publisher,expected](auto goal) {
                    std::lock_guard<std::mutex> guard{shared->worker_mutex};
                    if(shared->stop) { return; }
                    if(shared->worker.joinable())
                    {
                        shared->fail(std::make_exception_ptr(std::runtime_error{"Action ACK witness received a second accepted goal"}));return;
                    }
                    shared->worker=std::thread{[shared,gate,publisher,expected,goal] {
                        try
                        {
                            auto const deadline{std::chrono::steady_clock::now()+std::chrono::seconds(5)};
                            bool acknowledged{};std::size_t matches{};
                            while(!shared->stop&&rclcpp::ok()&&std::chrono::steady_clock::now()<deadline)
                            {
                                if(!acknowledged) { acknowledged=gate->ready(); }
                                matches=publisher->get_subscription_count();
                                if(acknowledged&&matches) { break; }
                                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                            }
                            require(!shared->stop&&rclcpp::ok()&&std::chrono::steady_clock::now()<deadline&&acknowledged&&matches,
                                    "Action ACK or actual DDS feedback subscription matching exceeded5seconds or stopped");
                            require(goal->is_active(),"ACK accepted native Action goal remains active");
                            feedback_message feedback;feedback.goal_id.uuid=goal->get_goal_id();feedback.feedback.partial_sequence=expected;
                            publisher->publish(feedback);++shared->feedback_publications;
                            std::cout<<"ACK_FEEDBACK native_goal_requests=1 auxiliary_generated_feedback_publications=1 actual_feedback_subscriptions="<<matches<<"\n"<<std::flush;
                            auto const result_at{std::chrono::steady_clock::now()+std::chrono::milliseconds(100)};
                            while(!shared->stop&&rclcpp::ok()&&std::chrono::steady_clock::now()<result_at)
                            { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
                            require(!shared->stop&&rclcpp::ok()&&goal->is_active(),"Action ACK witness stopped before native result");
                            auto result{std::make_shared<fibonacci::Result>()};result->sequence=expected;goal->succeed(result);shared->done=true;
                        }
                        catch(...) { shared->fail(std::current_exception()); }
                    }};
                });
            spinner=std::thread{[&executor,shared] {
                try
                {
                    // A bounded spin_once also avoids cancelling before spin()
                    // starts and then waiting forever for a newly started spin.
                    while(!shared->stop&&rclcpp::ok()) { executor.spin_once(std::chrono::milliseconds(10)); }
                }
                catch(...) { shared->fail(std::current_exception()); }
            }};
            std::cout<<"READY Action ACK witness ROS2 auxiliary-generated-feedback private-ACK maximum-wait=5s\n"<<std::flush;
            auto const deadline{std::chrono::steady_clock::now()+std::chrono::seconds(12)};
            while(!shared->done&&!shared->failed&&rclcpp::ok()&&std::chrono::steady_clock::now()<deadline)
            { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
            if(shared->failed)
            {
                std::exception_ptr error;{ std::lock_guard<std::mutex> guard{shared->failure_mutex};error=shared->failure; }
                if(error) { std::rethrow_exception(error); }
            }
            require(shared->done&&rclcpp::ok()&&shared->goal_requests==1&&shared->feedback_publications==1,
                    "Action ACK witness actual goal/unique feedback/result failed or timed out");
            auto const until{std::chrono::steady_clock::now()+std::chrono::milliseconds(500)};
            while(rclcpp::ok()&&std::chrono::steady_clock::now()<until)
            { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
            require(rclcpp::ok()&&!shared->failed,"Action ACK witness interrupted before result retention completed");
            cleanup();
            std::cout<<"PASS actual ROS2 Wasm/native ACK Action exact order/unique generated feedback/result; ordinary early-feedback behavior is not certified\n"<<std::flush;
            return 0;
        }
        catch(...) { cleanup();throw; }
    }
}

inline int ros2_action_witness(int argc,char** argv)
{
    if(argc>=2&&std::string{argv[1]}=="--serve-native-ack") { return ros2_action_ack_probe::run(argc,argv); }
    require(argc==3,"usage Action witness --serve-native|--call-guest|--active-native|--flood-native|--deny-native absolute-resource");
    auto mode{std::string{argv[1]}},resource{std::string{argv[2]}};require(gateway::canonical(resource),"canonical witness Action resource");
    int init_argc{1};rclcpp::init(init_argc,argv);
    auto node{std::make_shared<rclcpp::Node>("uwvm_ros2_action_network_witness")};
    rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
    std::thread spinner{[&] { executor.spin(); }};std::thread worker;
    auto cleanup=[&] { if(worker.joinable()) { worker.join(); }executor.cancel();spinner.join();rclcpp::shutdown(); };
    std::vector<std::int32_t> const expected{0,1,1,2,3,5};
    try
    {
        auto deadline{std::chrono::steady_clock::now()+std::chrono::seconds(12)};
        if(mode=="--deny-native")
        {
            auto requests{std::make_shared<std::atomic<unsigned>>(0)};
            auto accepted{std::make_shared<std::atomic<bool>>(false)};
            auto server{rclcpp_action::create_server<fibonacci>(node,resource,
                [requests](auto const&,std::shared_ptr<fibonacci::Goal const>) {
                    ++*requests;
                    return rclcpp_action::GoalResponse::REJECT;
                },[](auto) { return rclcpp_action::CancelResponse::REJECT; },[accepted](auto goal) {
                    *accepted=true;
                    if(goal->is_active()) { goal->abort(std::make_shared<fibonacci::Result>()); }
                })};
            std::cout<<"READY native Action denial witness ROS2\n"<<std::flush;
            auto const until{std::chrono::steady_clock::now()+std::chrono::seconds(4)};
            while(std::chrono::steady_clock::now()<until) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
            auto const count{requests->load()};
            std::cout<<"NO_UNAUTHORIZED_GOAL native_goal_requests="<<count<<" native_goals_accepted="<<(accepted->load()?1:0)<<"\n"<<std::flush;
            require(count==0&&!accepted->load(),"denied Wasm Action send reaches zero generated native goal requests");
            cleanup();std::cout<<"PASS actual ROS2 denied Action native_goal_requests=0\n"<<std::flush;return 0;
        }
        else if(mode=="--serve-native")
        {
            std::atomic<bool> done{},wrong{};
            auto server{rclcpp_action::create_server<fibonacci>(node,resource,[&](auto const&,std::shared_ptr<fibonacci::Goal const> goal) {
                if(goal->order!=5) { wrong=true;return rclcpp_action::GoalResponse::REJECT; }return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
            },[](auto) { return rclcpp_action::CancelResponse::REJECT; },[&](auto goal) {
                if(worker.joinable()) { wrong=true;auto response=std::make_shared<fibonacci::Result>();goal->abort(response);return; }
                worker=std::thread{[&,goal] { auto feedback=std::make_shared<fibonacci::Feedback>();feedback->partial_sequence=expected;goal->publish_feedback(feedback);std::this_thread::sleep_for(std::chrono::milliseconds(100));auto response=std::make_shared<fibonacci::Result>();response->sequence=expected;goal->succeed(response);done=true; }};
            })};
            std::cout<<"READY Action witness ROS2\n"<<std::flush;
            while(!done&&!wrong&&std::chrono::steady_clock::now()<deadline) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
            require(done&&!wrong,"native Action receives exact Wasm order");std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else if(mode=="--flood-native")
        {
            std::atomic<unsigned> requests{};std::atomic<bool> wrong{};std::mutex goals_mutex;
            std::vector<std::shared_ptr<rclcpp_action::ServerGoalHandle<fibonacci>>> goals;
            auto server{rclcpp_action::create_server<fibonacci>(node,resource,[&](auto const&,auto goal) {
                auto count=++requests;if(goal->order!=5||count>2) { wrong=true;return rclcpp_action::GoalResponse::REJECT; }return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
            },[](auto) { return rclcpp_action::CancelResponse::ACCEPT; },[&](auto goal) { std::lock_guard<std::mutex> guard{goals_mutex};goals.push_back(goal); })};
            std::cout<<"READY native Action flood witness ROS2\n"<<std::flush;
            auto until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
            while(std::chrono::steady_clock::now()<until) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
            require(requests==2&&!wrong,"100-request malicious Wasm Action flood produces exactly2 generated native goal requests");
            { std::lock_guard<std::mutex> guard{goals_mutex};require(goals.size()==2,"exactly2 native accepted goals retained");for(auto const& goal:goals) { auto result=std::make_shared<fibonacci::Result>();result->sequence=expected;if(goal->is_canceling()) { goal->canceled(result); }else if(goal->is_active()) { goal->succeed(result); } } }
            std::cout<<"BOUNDED_NATIVE_GOALS native_goal_requests=2 excess_goal_requests=0\n"<<std::flush;
        }
        else if(mode=="--active-native")
        {
            std::atomic<unsigned> goals{},cancellations{};std::atomic<bool> done{};
            auto server{rclcpp_action::create_server<fibonacci>(node,resource,[&](auto const&,auto goal) {
                if(goal->order!=5) { return rclcpp_action::GoalResponse::REJECT; }++goals;return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
            },[&](auto) { ++cancellations;return rclcpp_action::CancelResponse::ACCEPT; },[&](auto handle) {
                worker=std::thread{[&,handle] { auto until=std::chrono::steady_clock::now()+std::chrono::seconds(4);
                    while(std::chrono::steady_clock::now()<until&&!handle->is_canceling()) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
                    auto result=std::make_shared<fibonacci::Result>();result->sequence=expected;
                    done=true;if(handle->is_canceling()) { handle->canceled(result); }else { handle->succeed(result); }
                }};
            })};
            auto client{rclcpp_action::create_client<fibonacci>(node,resource)};
            while(!client->action_server_is_ready()&&std::chrono::steady_clock::now()<deadline) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
            require(client->action_server_is_ready(),"active native cancellation witness server readiness");fibonacci::Goal goal;goal.order=5;auto sent{client->async_send_goal(goal)};
            require(sent.wait_until(deadline)==std::future_status::ready,"active native goal accepted before deadline");auto handle=sent.get();require(handle!=nullptr,"native goal active before malicious Wasm cancellation");
            auto result=client->async_get_result(handle);std::cout<<"READY active native Action witness ROS2\n"<<std::flush;
            require(result.wait_until(deadline)==std::future_status::ready,"active native result deadline");auto actual=result.get();
            require(done&&goals==1&&cancellations==0&&actual.code==rclcpp_action::ResultCode::SUCCEEDED&&actual.result&&actual.result->sequence==expected,"denied own/any Wasm cancellation causes zero native cancellation requests and preserves actual goal success");
            std::cout<<"NO_UNAUTHORIZED_CANCEL native_goals=1 native_cancel_requests=0\n"<<std::flush;
        }
        else
        {
            require(mode=="--call-guest","known Action witness mode");auto client{rclcpp_action::create_client<fibonacci>(node,resource)};
            std::cout<<"READY Action witness ROS2\n"<<std::flush;
            while(!client->action_server_is_ready()&&std::chrono::steady_clock::now()<deadline) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
            require(client->action_server_is_ready(),"native discovers Wasm Action server");std::atomic<bool> feedback_seen{},feedback_wrong{};
            typename rclcpp_action::Client<fibonacci>::SendGoalOptions options;
            options.feedback_callback=[&](auto,auto feedback) { if(!feedback||feedback->partial_sequence!=expected) { feedback_wrong=true; }else { feedback_seen=true; } };
            fibonacci::Goal goal;goal.order=5;auto sent{client->async_send_goal(goal,options)};
            require(sent.wait_until(deadline)==std::future_status::ready,"native Action accepted before deadline");auto handle{sent.get()};require(handle!=nullptr,"native generated Action accepted");auto result{client->async_get_result(handle)};
            require(result.wait_until(deadline)==std::future_status::ready,"native Action result before deadline");auto actual{result.get()};require(actual.code==rclcpp_action::ResultCode::SUCCEEDED&&actual.result&&actual.result->sequence==expected,"native receives exact Wasm Action success/result");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));require(feedback_seen&&!feedback_wrong,"native receives exact Wasm Action full feedback before witness success");
        }
        cleanup();std::cout<<"PASS actual ROS2 Wasm/native Action exact order/result\n";return 0;
    }
    catch(...) { cleanup();throw; }
}
