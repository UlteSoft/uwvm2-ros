#pragma once
#include "../../gateways/common/config.h"
#include <uwvm2/imported/ros/common/manifest.h>
#include <uwvm2/imported/ros/ipc/image.h>
#include <sys/wait.h>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <functional>
#include <charconv>
#include <type_traits>
#include <utility>
#if __cplusplus >= 202002L && defined(__cpp_concepts)
#include "../../third-parties/fast_io/include/fast_io.h"
#include "../../third-parties/fast_io/include/fast_io_unit/string.h"
#endif

namespace ros_probe
{
    namespace gateway=uwvm2::ros_gateway;
    namespace core=uwvm2::imported::ros;
    namespace ipc=core::ipc;
    inline void require(bool ok,std::string const& what) { if(!ok) { throw std::runtime_error("FAIL "+what); } }
    /** Aggregate fixture text once, without locale-dependent stream state.
     * Worker-side C++20 uses the existing fast_io string sink. Native C++17
     * keeps allocation-free integer conversion because fast_io needs concepts.
     * This formatter changes neither the policy grammar nor any wire codec. */
    struct fixture_text
    {
        std::string bytes;
        fixture_text() { bytes.reserve(4096); }
        template<typename... Values> void append(Values const&... values)
        {
#if __cplusplus >= 202002L && defined(__cpp_concepts)
            fast_io::ostring_ref_std output{std::addressof(bytes)};
            // Preserve literal array extents. Variable C strings need an
            // explicit text view rather than fast_io's raw-pointer formatter.
            auto printable=[](auto const& value)->decltype(auto)
            {
                using value_type=std::remove_reference_t<decltype(value)>;
                if constexpr(std::is_pointer_v<value_type>&&std::is_convertible_v<decltype(value),char const*>)
                { return std::string_view{value}; }
                else { return (value); }
            };
            fast_io::print(output,printable(values)...);
#else
            auto one=[&](auto const& value)
            {
                if constexpr(std::is_integral_v<std::decay_t<decltype(value)>>)
                {
                    gateway::native_io::fixed_text<32> number;
                    require(number.number(value),"fixture integer formatting");bytes.append(number.view());
                }
                else { bytes.append(std::string_view{value}); }
            };
            (one(values),...);
#endif
        }
    };
    /** Emit one complete trusted fixture record at this exact call site.
     * Reuse fixture_text: C++20 formats through fast_io's existing string sink,
     * while native C++17 retains the locale-free compatibility formatter.
     * write_all performs checked short-write/EINTR handling without an
     * additional userspace output buffer, so READY/explicit-flush markers are
     * visible before the caller advances its protocol. The supplied descriptor
     * is borrowed and never closed. Existing strings include their exact
     * newline; this helper inserts no text, separator, retry or permission.
     * Output failure follows the ordinary fixture assertion path. A terminal
     * stderr diagnostic may catch that failure to preserve its original exit1.
     */
    template<typename... Values> inline void print_fixture(int descriptor,Values const&... values)
    {
        fixture_text text;text.append(values...);
        require(gateway::native_io::write_all(descriptor,text.bytes),"complete fixture console output");
    }
    /** Write a private regular fixture file with checked completion and close.
     * Nonblocking/no-follow admission rejects FIFOs, devices and symlinks before
     * truncation. Existing marker files may be replaced; policy files are fresh.
     * No file descriptor or host filesystem permission is exposed to a Guest. */
    inline void write_fixture_file(std::string const& path,std::string_view contents,bool exclusive=false)
    {
        int file{::open(path.c_str(),O_WRONLY|O_CREAT|O_CLOEXEC|O_NOFOLLOW|O_NONBLOCK|(exclusive?O_EXCL:0),0600)};
        require(file>=0,"open private fixture output");struct stat state{};
        if(::fstat(file,&state)||!S_ISREG(state.st_mode)||state.st_uid!=::geteuid()||(state.st_mode&0022)||::ftruncate(file,0))
        { (void)::close(file);require(false,"private fixture output admission"); }
#if __cplusplus >= 202002L && defined(__cpp_concepts)
        fast_io::posix_file output{fast_io::posix_file_factory{file}};
        fast_io::print(output,contents);output.close();
#else
        bool written{gateway::native_io::write_all(file,contents)};
        int closed{::close(file)};require(written&&closed==0,"complete private fixture write/close");
#endif
    }
    inline void service_qos(core::capability_spec& c)
    {
        c.limits.insert({{"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","10"},{"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}});
    }
    inline core::capability_spec topic(core::version v,std::string alias,std::string operation,std::string resource,std::string type,std::string identity)
    {
        core::capability_spec c{alias,operation,resource,type,identity,{}};
        c.limits={{"max-message-bytes","4096"},{"max-rate-hz","1000"},{"burst","4"},{"max-bytes-per-second","4096000"},{"max-queue-bytes","32768"},{"overflow-policy","drop-oldest"},{"validator","none"},{"audit","allow-and-deny"},{"audit-max-rate-hz","100"},{"audit-burst","10"},{"audit-max-records","10000"},{"audit-max-bytes","1048576"},{operation=="topic.publish"?"max-publishers":"max-subscribers","1"}};
        if(v==core::version::ros1) { c.limits.emplace("queue-depth","8"); c.limits.emplace("transport","tcpros"); if(operation=="topic.publish") { c.limits.emplace("latch","false"); } }
        else { c.limits.insert({{"max-inflight","1"},{"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","8"},{"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}}); }
        return c;
    }
    inline core::capability_spec service(core::version v)
    {
        core::capability_spec c{"service","service.call","/uwvm_test/trigger",v==core::version::ros1?"std_srvs/Trigger":"std_srvs/srv/Trigger",v==core::version::ros1?"937c9679a518e3a18d831e57125ea522":"RIHS01_eeff2cd6fa5ad9d27cdf4dec64818317839b62f212a91e6b5304b634b2062c5f",{}};
        c.limits={{"timeout-ms","1000"},{"max-inflight","1"},{"max-request-bytes","1024"},{"max-response-bytes","1024"},{"max-rate-hz","10"},{"burst","2"},{"max-bytes-per-second","10240"},{"validator","none"},{"audit","allow-and-deny"},{"audit-max-rate-hz","100"},{"audit-burst","10"},{"audit-max-records","10000"},{"audit-max-bytes","1048576"}};
        if(v==core::version::ros2) { service_qos(c); }return c;
    }
    struct fixture
    {
        gateway::configuration config;
        std::string directory{},config_file{};
        pid_t gateway_pid{-1};
        fixture(core::version v,std::string const& executable,std::function<void(gateway::configuration&)> configure={})
        {
            char path[]{"/tmp/uwvm-ros-probe-XXXXXX"}; char* dir{::mkdtemp(path)}; require(dir,"mkdtemp"); directory=dir;
            config.policy.ros_version=v; config.policy.enabled=true; config.socket=directory+"/gateway.sock"; config.worker_uid=::getuid(); config.worker_pid=::getpid(); config.timeout_ms=10000; config.max_sessions=8; config.audit_records_per_second=100;config.audit_max_records=10000;config.audit_max_bytes=1048576;
            if(v==core::version::ros1) { config.policy.connection={{"master-uri",::getenv("ROS_MASTER_URI")?::getenv("ROS_MASTER_URI"):"http://127.0.0.1:11329"},{"node-name","uwvm_ros1_gateway_probe"},{"namespace","/uwvm_test"},{"advertise-host","127.0.0.1"},{"transport","tcpros"}}; }
            else { config.policy.connection={{"node-name","uwvm_ros2_gateway_probe"},{"namespace","/uwvm_test"},{"domain-id","72"},{"rmw-implementation","rmw_fastrtps_cpp"},{"security-mode","disabled-test"},{"keystore","not-applicable"},{"enclave","not-applicable"}}; }
            std::vector<std::string> names{"String","Bool","Twist","Image"};
            std::vector<std::string> ros1_id{"992ce8a1687cec8c8bd883ec73ca41d1","8b94c1b53db61fb6aed406028ad6332a","9f195f881246fdfa2798d1d3eebca84a","060021388200f6f0f447d0fcd9c64743"};
            std::vector<std::string> ros2_id{"RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18","RIHS01_feb91e995ff9ebd09c0cb3d2aed18b11077585839fb5db80193b62d74528f6c9","RIHS01_9c45bf16fe0983d80e3cfe750d6835843d265a9a6c46bd2e609fcddde6fb8d2a","RIHS01_d31d41a9a4c4bc8eae9be757b0beed306564f7526c88ea6a4588fb9582527d47"};
            for(unsigned i{};i!=4;++i)
            {
                auto package{i==2?"geometry_msgs":i==3?"sensor_msgs":"std_msgs"}; auto type{std::string{package}+(v==core::version::ros1?"/":"/msg/")+names[i]};
                for(auto const& op:std::vector<std::string>{"pub","sub"})
                { auto alias{op+names[i]}; config.policy.capabilities.emplace(alias,topic(v,alias,op=="pub"?"topic.publish":"topic.subscribe","/uwvm_test/"+op+names[i],type,v==core::version::ros1?ros1_id[i]:ros2_id[i])); }
            }
            config.policy.capabilities.emplace("service",service(v));
            if(configure) { configure(config); }
            config_file=directory+"/policy.txt";
            try
            {
            fixture_text out;
            out.append("backend ",std::string_view{v==core::version::ros1?"ros1":"ros2"},"\nworker-uid ",config.worker_uid,"\nworker-pid ",config.worker_pid,"\nsocket ",config.socket,"\ntimeout-ms ",config.timeout_ms,"\nmax-sessions 8\naudit-max-records-per-second 100\naudit-max-records 10000\naudit-max-bytes 1048576\n");
            for(auto const& p:config.policy.connection) { out.append("connection ",p.first," ",p.second,"\n"); }
            for(auto const& p:config.policy.capabilities)
            { auto const& c{p.second}; out.append("grant ",c.alias," ",c.operation," ",c.resource," ",c.type," ",c.type_identity,"\n"); for(auto const& l:c.limits) { out.append("set ",c.alias," ",l.first," ",l.second,"\n"); } }
            write_fixture_file(config_file,out.bytes,true);require(::chmod(config_file.c_str(),0600)==0,"private gateway policy permissions");
            }
            catch(...) { ::unlink(config_file.c_str());::rmdir(directory.c_str());throw; }
            gateway_pid=::fork(); require(gateway_pid>=0,"fork gateway");
            if(!gateway_pid) { ::execl(executable.c_str(),executable.c_str(),config_file.c_str(),nullptr); ::_exit(127); }
            for(unsigned n{};n!=500;++n) { struct stat s{}; if(!::lstat(config.socket.c_str(),&s)) { return; } std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
            ::kill(gateway_pid,SIGKILL);::waitpid(gateway_pid,nullptr,0);gateway_pid=-1;::unlink(config.socket.c_str());::unlink(config_file.c_str());::rmdir(directory.c_str());
            throw std::runtime_error("gateway socket startup timeout");
        }
        void connect(ipc::client& c,bool bind=true)
        { require(!c.connect(config.socket,static_cast<std::uint8_t>(config.policy.ros_version),config.worker_uid,1000),"authenticated IPC connect"); if(bind) { require(!c.call("session.bind","target",core::binding_manifest(config.policy),1000).error,"exact policy binding"); } }
        ~fixture()
        { if(gateway_pid>0) { ::kill(gateway_pid,SIGTERM); for(unsigned n{};n!=200;++n) { if(::waitpid(gateway_pid,nullptr,WNOHANG)==gateway_pid) { gateway_pid=-1; break; } std::this_thread::sleep_for(std::chrono::milliseconds(10)); } if(gateway_pid>0) { ::kill(gateway_pid,SIGKILL); ::waitpid(gateway_pid,nullptr,0); } } ::unlink(config.socket.c_str()); ::unlink(config_file.c_str()); ::rmdir(directory.c_str()); }
    };
    inline std::vector<std::uint8_t> data(unsigned i)
    {
        if(i==0) { std::vector<std::uint8_t> d; std::string s{"uwvm real ROS exact payload"}; ipc::put(d,s.size(),4); d.insert(d.end(),s.begin(),s.end()); return d; }
        if(i==1) { return {1}; }
        if(i==2) { std::vector<std::uint8_t> d; for(double v:{1.25,-2.,3.5,0.1,0.2,0.3}) { std::uint64_t bits{}; std::memcpy(&bits,&v,8); ipc::put(d,bits,8); } return d; }
        return ipc::encode_image({7,123,456,2,2,6,"camera_frame","rgb8",false,{0,1,2,3,4,5,6,7,8,9,10,11}});
    }
    inline void ipc_security(fixture& f,ipc::client& c)
    {
        require(c.call("topic.publish","unauthorized",data(0),1000).error==13,"unauthorized alias denied");
        require(c.call("topic.publish","subString",data(0),1000).error==13,"operation substitution denied");
        auto malformed=data(0); malformed[0]=255; require(c.call("topic.publish","pubString",malformed,1000).error==22,"malformed String denied before network creation");
        require(c.call("topic.publish","pubString",{1,0,0,0,255},1000).error==22,"invalid UTF-8 String denied before native publisher creation");
        auto nan=data(2); std::uint64_t bits{0x7ff8000000000000ULL}; for(unsigned n{};n!=8;++n) { nan[n]=static_cast<std::uint8_t>(bits>>(8*n)); } require(c.call("topic.publish","pubTwist",nan,1000).error==22,"NaN denied");
        ipc::client unbound; f.connect(unbound,false); require(unbound.call("topic.publish","pubString",data(0),1000).error==13,"unbound worker denied");
        auto wrong=f.config.policy; wrong.capabilities.at("pubString").resource="/different";
        require(unbound.call("session.bind","target",core::binding_manifest(wrong),1000).error==13,"gateway authoritative policy mismatch denied");
        require(unbound.call("topic.publish","pubString",data(0),1000).error==13,"policy mismatch has no subsequent authority"); unbound.close();
        auto child{::fork()};require(child>=0,"fork different-PID peer");
        if(!child){ipc::client foreign;auto error{foreign.connect(f.config.socket,static_cast<std::uint8_t>(f.config.policy.ros_version),f.config.worker_uid,1000)};::_exit(error?0:1);}
        int child_status{};require(::waitpid(child,&child_status,0)==child&&WIFEXITED(child_status)&&!WEXITSTATUS(child_status),"same-UID different-PID peer authentication denied");
        int raw{::socket(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0)}; require(raw>=0,"raw socket"); sockaddr_un address{}; address.sun_family=AF_UNIX;
        std::memcpy(address.sun_path,f.config.socket.c_str(),f.config.socket.size()+1);
        require(!::connect(raw,reinterpret_cast<sockaddr*>(&address),sizeof address),"raw trusted worker socket");
        ipc::frame hello; hello.type=ipc::kind::hello; hello.backend=static_cast<std::uint8_t>(f.config.policy.ros_version);hello.request_id=1;hello.timeout_ms=1000;
        require(ipc::write_frame(raw,hello,ipc::after(1000)),"raw hello");ipc::frame answer;require(ipc::read_frame(raw,answer,ipc::after(1000)),"issued session");
        ipc::frame forged;forged.type=ipc::kind::request;forged.backend=hello.backend;forged.request_id=2;forged.session_id=answer.session_id^1;forged.timeout_ms=1000;
        require(ipc::encode_request({"topic.publish","pubString",data(0)},forged.payload),"encoded forged session");
        require(ipc::write_frame(raw,forged,ipc::after(1000))&&!ipc::read_frame(raw,answer,ipc::after(1000)),"forged session closes connection before side effect");::close(raw);
        print_fixture(STDOUT_FILENO,
            "PASS authenticated IPC, unbound session, exact target binding, unauthorized alias, operation substitution, malformed String and NaN rejection\n");
    }
}
