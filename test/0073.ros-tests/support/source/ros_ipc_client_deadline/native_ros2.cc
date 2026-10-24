#include "../ros_ipc/probe_common.h"
#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <sys/syscall.h>
#include <exception>

namespace ipc_client_deadline_probe
{
    using namespace ros_probe;
    using clock=std::chrono::steady_clock;
    using trigger=std_srvs::srv::Trigger;
    constexpr std::uint32_t short_budget_ms{100},long_budget_ms{3000},holder_budget_ms{5000};

    struct native_state
    {
        std::mutex mutex;
        std::condition_variable changed;
        unsigned slow_calls{},quick_calls{},released{};
        bool stopping{},callback_expired{};
        std::exception_ptr spinner_error;

        void release_all() noexcept
        { std::lock_guard<std::mutex> lock{mutex};stopping=true;changed.notify_all(); }
        unsigned quick_count()
        { std::lock_guard<std::mutex> lock{mutex};return quick_calls; }
        unsigned slow_count()
        { std::lock_guard<std::mutex> lock{mutex};return slow_calls; }
        void release(unsigned round)
        { std::lock_guard<std::mutex> lock{mutex};released=round;changed.notify_all(); }
    };

    struct call_state
    {
        std::atomic<pid_t> tid{};
        std::atomic<bool> in_call{},finished{};
        ipc::call_result result;
        std::exception_ptr error;
        clock::time_point started,completed;
    };

    std::thread call(ipc::client& client,char const* alias,std::uint32_t budget,call_state& state)
    {
        return std::thread{[&client,alias,budget,&state]
        {
            state.tid.store(static_cast<pid_t>(::syscall(SYS_gettid)),std::memory_order_relaxed);
            state.started=clock::now();
            // After this publication the only blocking operation is client.call:
            // both short string arguments use SSO and the request payload is empty.
            state.in_call.store(true,std::memory_order_release);
            try { state.result=client.call("service.call",alias,{},budget); }
            catch(...) { state.error=std::current_exception(); }
            state.completed=clock::now();state.finished.store(true,std::memory_order_release);
        }};
    }

    struct join_calls
    {
        native_state& native;
        std::thread& first;
        std::thread& second;
        ~join_calls()
        {
            // On an assertion failure the native callback is released before
            // joining either bounded IPC request. The executor remains alive.
            if(first.joinable()||second.joinable()) { native.release_all(); }
            if(first.joinable()) { first.join(); }
            if(second.joinable()) { second.join(); }
        }
    };

    bool futex_wait(pid_t tid)
    {
        if(tid<=0) { return false; }
        gateway::native_io::fixed_text<96> path;
        require(path.append("/proc/self/task/")&&path.number(tid)&&path.append("/wchan"),"bounded own-thread wchan path");
        std::string name{path.view()};
        int fd{::open(name.c_str(),O_RDONLY|O_CLOEXEC)};
        if(fd<0) { return false; }
        char text[96];ssize_t count;
        do { count=::read(fd,text,sizeof text); } while(count<0&&errno==EINTR);
        ::close(fd);
        return count>0&&std::string_view{text,static_cast<std::size_t>(count)}.find("futex")!=std::string_view::npos;
    }

    clock::time_point await_mutex(call_state const& state)
    {
        auto until{clock::now()+std::chrono::seconds(2)};
        while(clock::now()<until)
        {
            require(!state.finished.load(std::memory_order_acquire),"second shared-client call finished before mutex observation");
            if(state.in_call.load(std::memory_order_acquire)&&futex_wait(state.tid.load(std::memory_order_relaxed))) { return clock::now(); }
            // This only polls Linux's own-thread observation. Budget exhaustion
            // below uses the callback gate and an absolute CV deadline.
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        throw std::runtime_error("second shared-client call did not reach the native futex mutex wait");
    }

    std::vector<std::uint8_t> expected(char const* text)
    {
        auto size{std::strlen(text)};std::vector<std::uint8_t> out;out.reserve(5+size);out.push_back(1);ipc::put(out,size,4);
        out.insert(out.end(),text,text+size);return out;
    }

    void observed(char const* mode,ipc::call_result const& result,unsigned quick,std::uint64_t elapsed)
    {
        gateway::native_io::fixed_text<320> line;
        require(line.append("OBSERVED native-client mode=")&&line.append(mode)&&line.append(" error=")&&line.number(result.error)&&
                line.append(" native_quick=")&&line.number(quick)&&line.append(" futex_wait=1 elapsed_ms=")&&line.number(elapsed)&&line.append("\n"),"bounded actual observation");
        require(gateway::native_io::write_all(STDOUT_FILENO,line.view()),"actual observation output");
    }

    void exercise(ipc::client& client,native_state& native,bool short_budget)
    {
        auto before_slow{native.slow_count()},before_quick{native.quick_count()};
        call_state holder,queued;
        auto first{call(client,"slow",holder_budget_ms,holder)};
        std::thread second;
        join_calls cleanup{native,first,second};
        {
            std::unique_lock<std::mutex> lock{native.mutex};
            require(native.changed.wait_for(lock,std::chrono::seconds(3),[&] { return native.slow_calls==before_slow+1||native.callback_expired||native.spinner_error; }),"actual slow native callback barrier missing");
            require(native.slow_calls==before_slow+1&&!native.callback_expired&&!native.spinner_error,"slow native barrier failed");
        }
        require(!holder.finished.load(std::memory_order_acquire),"holder returned while native callback was gated");
        second=call(client,"quick",short_budget?short_budget_ms:long_budget_ms,queued);
        auto blocked{await_mutex(queued)};
        require(native.quick_count()==before_quick,"quick native callback ran before the shared-client holder released");
        if(short_budget)
        {
            std::unique_lock<std::mutex> lock{native.mutex};
            auto exhausted{blocked+std::chrono::milliseconds(short_budget_ms+50)};
            bool unexpected{native.changed.wait_until(lock,exhausted,[&] { return native.callback_expired||native.spinner_error; })};
            require(!unexpected&&clock::now()>=exhausted,"native callback gate did not exhaust the observed queued call budget");
            require(!queued.finished.load(std::memory_order_acquire),"queued request escaped the proven mutex gate");
        }
        native.release(before_slow+1);
        first.join();second.join();
        if(holder.error) { std::rethrow_exception(holder.error); }
        if(queued.error) { std::rethrow_exception(queued.error); }
        require(!holder.result.error&&holder.result.payload==expected("slow"),"holder real Trigger response differs");
        if(short_budget)
        {
            // Observe the real callback even if a late native effect and the
            // caller's timeout raced. Absence also has a bounded DDS grace.
            std::unique_lock<std::mutex> lock{native.mutex};
            native.changed.wait_for(lock,std::chrono::milliseconds(300),[&] { return native.quick_calls>before_quick||native.callback_expired||native.spinner_error; });
        }
        auto delta{native.quick_count()-before_quick};
        auto elapsed{std::chrono::duration_cast<std::chrono::milliseconds>(queued.completed-queued.started).count()};
        observed(short_budget?"expired":"remaining",queued.result,delta,static_cast<std::uint64_t>(elapsed));
        if(short_budget)
        {
            require(queued.result.error==110&&queued.result.payload.empty()&&delta==0,"expired queued native request reached DDS");
            auto retry{client.call("service.call","quick",{},2000)};
            require(!retry.error&&retry.payload==expected("quick")&&native.quick_count()==before_quick+1,"same session did not survive the unsent expired request for exact native retry");
            require(gateway::native_io::write_all(STDOUT_FILENO,"PASS IPC client short queued deadline, native effects0, same-session exact retry count1\n"),"short PASS output");
        }
        else
        {
            require(!queued.result.error&&queued.result.payload==expected("quick")&&delta==1,"long queued budget did not execute exactly one real native request");
            require(gateway::native_io::write_all(STDOUT_FILENO,"PASS IPC client long queued budget, actual mutex wait, exact native response count1\n"),"long PASS output");
        }
    }
}

int main(int argc,char** argv)
{
    try
    {
        using namespace ipc_client_deadline_probe;
        require(argc==3,"usage native_ros2 actual-gateway-path {expired|remaining}");
        std::string mode{argv[2]};require(mode=="expired"||mode=="remaining","unknown queued-client mode");
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
        if(!::getenv("ROS_DOMAIN_ID")) { require(!::setenv("ROS_DOMAIN_ID","196",1),"set isolated default ROS domain"); }
        require(!::setenv("RMW_IMPLEMENTATION","rmw_fastrtps_cpp",1)&&!::setenv("ROS_SECURITY_ENABLE","false",1),"explicit native RMW/security environment");
        std::string domain{::getenv("ROS_DOMAIN_ID")};std::string suffix{std::to_string(::getpid())};
        std::string prefix{"/ipc_client_deadline_"+suffix};
        // This starts/forks the gateway before rclcpp::init creates native threads.
        fixture gateway_process{core::version::ros2,argv[1],[&](gateway::configuration& cfg)
        {
            cfg.policy.connection["domain-id"]=domain;cfg.policy.connection["node-name"]="ipc_client_deadline_gateway_"+suffix;
            cfg.policy.capabilities.clear();cfg.timeout_ms=12000;
            for(auto name:{"slow","quick"})
            {
                auto cap{service(core::version::ros2)};cap.alias=name;cap.resource=prefix+"/"+name;
                cap.limits["timeout-ms"]=std::string_view{name}=="slow"?"5000":"3000";cap.limits["max-inflight"]="4";cap.limits["max-rate-hz"]="100";cap.limits["burst"]="16";
                cap.limits["max-bytes-per-second"]="65536";cap.limits["max-request-bytes"]="128";cap.limits["max-response-bytes"]="128";
                require(cfg.policy.capabilities.emplace(cap.alias,std::move(cap)).second,"unique explicit native service grant");
            }
        }};
        rclcpp::init(argc,argv);
        native_state state;
        auto node{std::make_shared<rclcpp::Node>("ipc_client_deadline_native_"+suffix)};
        auto group{node->create_callback_group(rclcpp::CallbackGroupType::Reentrant)};
        auto slow{node->create_service<trigger>(prefix+"/slow",[&](std::shared_ptr<trigger::Request>,std::shared_ptr<trigger::Response> response)
        {
            std::unique_lock<std::mutex> lock{state.mutex};auto round{++state.slow_calls};state.changed.notify_all();
            bool released{state.changed.wait_for(lock,std::chrono::seconds(5),[&] { return state.released>=round||state.stopping; })};
            if(!released) { state.callback_expired=true;state.changed.notify_all(); }
            response->success=released&&!state.stopping;response->message=response->success?"slow":"native-gate-expired";
        },rclcpp::ServicesQoS{},group)};
        auto quick{node->create_service<trigger>(prefix+"/quick",[&](std::shared_ptr<trigger::Request>,std::shared_ptr<trigger::Response> response)
        { std::lock_guard<std::mutex> lock{state.mutex};++state.quick_calls;response->success=true;response->message="quick";state.changed.notify_all(); },rclcpp::ServicesQoS{},group)};
        rclcpp::executors::MultiThreadedExecutor executor{rclcpp::ExecutorOptions{},2};executor.add_node(node);
        std::thread spinner{[&] { try { executor.spin(); }catch(...) { std::lock_guard<std::mutex> lock{state.mutex};state.spinner_error=std::current_exception();state.changed.notify_all(); } }};
        struct stop
        {
            native_state& state;rclcpp::Executor& executor;std::thread& thread;
            ~stop() { state.release_all();executor.cancel();if(thread.joinable()) { thread.join(); }rclcpp::shutdown(); }
        } cleanup{state,executor,spinner};
        // Official SDK discovery only, without issuing a warm-up native request.
        auto slow_ready{node->create_client<trigger>(prefix+"/slow",rclcpp::ServicesQoS{})};
        auto quick_ready{node->create_client<trigger>(prefix+"/quick",rclcpp::ServicesQoS{})};
        require(slow_ready->wait_for_service(std::chrono::seconds(3))&&quick_ready->wait_for_service(std::chrono::seconds(3)),"official native Trigger DDS discovery");
        ipc::client client;gateway_process.connect(client);
        exercise(client,state,mode=="expired");
        require(!state.callback_expired&&!state.spinner_error,"bounded native callback/executor failure");
        client.close();return 0;
    }
    catch(std::exception const& error)
    {
        auto prefix=std::string_view{"FAIL "};ros_probe::gateway::native_io::write_all(STDERR_FILENO,prefix);
        ros_probe::gateway::native_io::write_all(STDERR_FILENO,std::string_view{error.what()});ros_probe::gateway::native_io::write_all(STDERR_FILENO,"\n");return 1;
    }
}
