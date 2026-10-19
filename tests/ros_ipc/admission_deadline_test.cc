#include "../../gateways/common/server.h"
#include <cstdlib>
#include <new>
#include <iostream>

namespace admission_test
{
    namespace gateway=uwvm2::ros_gateway;
    namespace core=uwvm2::imported::ros;
    namespace ipc=core::ipc;
    thread_local bool server_thread{},record_size{};
    std::atomic<bool> armed{};
    std::atomic<unsigned> hits{},calls{},budget_seen{},operation_pause_ms{};
    std::size_t target{};unsigned delay_ms{};
    void allocation(std::size_t size)
    {
        if(record_size) { target=size; }
        if(server_thread&&size==target&&armed.exchange(false))
        { ++hits;std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms)); }
    }
    void require(bool condition,char const* message) { if(!condition) { throw std::runtime_error(message); } }
    class mock final:public gateway::adapter
    {
    public:
        ipc::call_result publish(core::capability_spec const&,std::vector<std::uint8_t> const&) override { return {}; }
        ipc::call_result subscribe(core::capability_spec const&,std::shared_ptr<gateway::event_queue>) override { return {}; }
        ipc::call_result service(core::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t) override { return {}; }
        ipc::call_result operation(core::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t budget) override
        { ++calls;budget_seen=budget;std::this_thread::sleep_for(std::chrono::milliseconds(operation_pause_ms.exchange(0)));return {}; }
        void close(std::string const&) override {}
    };
    void scenario(unsigned timeout,unsigned delay,bool expired,unsigned cap_timeout=0,bool cap_expired=false)
    {
        hits=0;calls=0;budget_seen=0;operation_pause_ms=cap_expired?180:0;delay_ms=delay;armed=false;
        gateway::configuration cfg;cfg.policy.enabled=true;cfg.policy.ros_version=core::version::ros2;
        cfg.worker_uid=::getuid();cfg.worker_pid=::getpid();cfg.timeout_ms=1000;
        cfg.audit_records_per_second=100;cfg.audit_max_records=10000;cfg.audit_max_bytes=1048576;
        core::capability_spec cap{"clock","time.read-steady","/clock","builtin/time-v1","builtin:time-nanoseconds-v1",{}};
        cap.limits={{"max-rate-hz","100"},{"burst","10"},{"max-inflight","1"},{"max-response-bytes","8"},{"max-bytes-per-second","1024"},
            {"audit","allow-and-deny"},{"audit-max-rate-hz","100"},{"audit-burst","10"},{"audit-max-records","10000"},{"audit-max-bytes","1048576"}};
        auto next{cap};next.alias="next";
        if(cap_timeout)
        {
            cap.operation="param.describe";cap.resource="/deadline/native/value";cap.type="builtin/string-v1";cap.type_identity="builtin:param-string-v1";
            cap.limits.erase("max-response-bytes");cap.limits.insert({{"timeout-ms",std::to_string(cap_timeout)},
                {"max-parameter-bytes","1024"},{"validator","none"},{"parameter-node","/deadline/native"},{"parameter-name","value"},
                {"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","8"},
                {"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}});
        }
        gateway::validate(core::version::ros2,cap);
        cfg.policy.capabilities.emplace(cap.alias,cap);
        if(cap_expired) { gateway::validate(core::version::ros2,next);cfg.policy.capabilities.emplace(next.alias,next); }
        int sockets[2];require(!::socketpair(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0,sockets),"socketpair failed");
        int pidfd{static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0))};require(pidfd>=0,"pidfd_open failed");
        std::exception_ptr failure;
        std::thread server{[&] {
            server_thread=true;
            try { gateway::serve_connection(sockets[1],cfg,[] { return std::make_unique<mock>(); },pidfd); }
            catch(...) { failure=std::current_exception(); }
        }};
        struct cleanup
        {
            int fd,pidfd;std::thread& thread;
            ~cleanup() { armed=false;::shutdown(fd,SHUT_RDWR);::close(fd);if(thread.joinable()) { thread.join(); }::close(pidfd); }
        } lifetime{sockets[0],pidfd,server};
        ipc::frame frame;frame.backend=2;frame.type=ipc::kind::hello;frame.request_id=1;frame.timeout_ms=1000;
        require(ipc::write_frame(sockets[0],frame,ipc::after(1000)),"hello send failed");
        ipc::frame answer;require(ipc::read_frame(sockets[0],answer,ipc::after(1000)),"hello receive failed");
        frame.type=ipc::kind::request;frame.request_id=2;frame.session_id=answer.session_id;
        require(ipc::encode_request({"session.bind","target",core::serialize_manifest(cfg.policy)},frame.payload),"binding encode failed");
        require(ipc::write_frame(sockets[0],frame,ipc::after(1000))&&ipc::read_frame(sockets[0],answer,ipc::after(1000))&&!answer.error,"binding failed");
        frame.request_id=3;frame.timeout_ms=timeout;
        require(ipc::encode_request({cap.operation,cap.alias,{}},frame.payload),"clock request encode failed");
        armed=true;
        require(ipc::write_frame(sockets[0],frame,ipc::after(1000)),"clock request send failed");
        bool received{ipc::read_frame(sockets[0],answer,ipc::after(1000))};
        unsigned first_budget{};
        if(cap_expired)
        {
            require(received&&answer.type==ipc::kind::response&&answer.backend==2&&answer.error==110&&answer.payload.empty()
                    &&answer.request_id==0&&answer.response_id==3&&answer.session_id==frame.session_id,
                    "expired capability result failed to return timeout on live IPC session");
            first_budget=budget_seen.load();
            require(calls==1&&first_budget>0&&first_budget<=cap_timeout-delay,
                    "expired capability operation reset admission time or skipped its native mock");
            frame.request_id=4;
            require(ipc::encode_request({next.operation,next.alias,{}},frame.payload),"next live-session request encode failed");
            require(ipc::write_frame(sockets[0],frame,ipc::after(1000)),"next live-session request send failed");
            received=ipc::read_frame(sockets[0],answer,ipc::after(1000));
            require(received&&answer.type==ipc::kind::response&&answer.backend==2&&!answer.error&&answer.payload.empty()
                    &&answer.request_id==0&&answer.response_id==4&&answer.session_id==frame.session_id&&calls==2,
                    "capability timeout retired the authenticated IPC session");
            require(operation_pause_ms==0&&budget_seen>cap_timeout&&budget_seen<=timeout,
                    "next IPC operation inherited the expired capability deadline");
        }
        ::shutdown(sockets[0],SHUT_RDWR);server.join();if(failure) { std::rethrow_exception(failure); }
        require(hits==1,"quota-node allocation barrier was not hit exactly once");
        if(expired) { require(!received&&calls==0,"expired IPC request reached adapter side effect"); }
        else if(!cap_expired)
        {
            require(received&&!answer.error&&calls==1,"unexpired IPC request failed");
            require(budget_seen>0&&budget_seen<=(cap_timeout?std::min(cap_timeout,timeout):timeout)-delay,"adapter budget reset time spent on admission");
        }
        if(cap_expired)
        {
            std::cout<<"PASS actual authenticated IPC expired capability: timeout_ms="<<timeout<<" cap_timeout_ms="<<cap_timeout
                     <<" barrier_ms="<<delay<<" mock_pause_ms=180 first_budget="<<first_budget
                     <<" timeout_error110 empty_payload1 same_session_next_success1 next_budget="<<budget_seen.load()
                     <<" next_mock_pause_ms=0 adapter_calls="<<calls.load()<<" hits="<<hits.load()<<'\n';
        }
        else
        {
            std::cout<<"PASS actual authenticated IPC "<<(expired?"expired admission: adapter_calls0":"remaining budget: adapter_calls1")
                     <<" timeout_ms="<<timeout<<" barrier_ms="<<delay<<" actual_budget="<<budget_seen.load()<<" hits="<<hits.load()<<'\n';
        }
    }
}
void* operator new(std::size_t size)
{ admission_test::allocation(size);if(auto p=std::malloc(size?size:1)) { return p; }throw std::bad_alloc{}; }
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p,std::size_t) noexcept { std::free(p); }
void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete[](void* p) noexcept { ::operator delete(p); }
void operator delete[](void* p,std::size_t) noexcept { ::operator delete(p); }
int main(int argc,char** argv)
{
    try
    {
        { std::map<std::string,admission_test::gateway::bucket,std::less<>> quota;
          admission_test::record_size=true;quota.try_emplace("clock");admission_test::record_size=false; }
        admission_test::require(admission_test::target!=0,"quota-node allocation size unavailable");
            std::string selected{argc>1?argv[1]:"all"};
        admission_test::require(argc<=2&&(selected=="all"||selected=="expired"||selected=="remaining"||selected=="cap"||selected=="cap-expired"),"invalid admission scenario");
        if(selected=="all"||selected=="expired") { admission_test::scenario(20,80,true); }
        if(selected=="all"||selected=="remaining") { admission_test::scenario(200,60,false); }
        if(selected=="all"||selected=="cap") { admission_test::scenario(400,60,false,200); }
        if(selected=="all"||selected=="cap-expired") { admission_test::scenario(400,60,false,200,true); }
        return 0;
    }
    catch(std::exception const& error) { std::cerr<<"FAIL "<<error.what()<<'\n';return 1; }
}
