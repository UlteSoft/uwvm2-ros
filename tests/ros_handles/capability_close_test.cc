#include <uwvm2/imported/ros/capability/manager.h>
#include <atomic>
#include <cassert>
#include <future>
#include <thread>

using namespace uwvm2::imported::ros;

static void require(result r,errc expected=errc::success)
{
    if(r.code!=expected)
    {
        fast_io::io::perr("FAIL capability close expected=",static_cast<unsigned>(expected),
                          " actual=",static_cast<unsigned>(r.code)," ",r.detail,"\n");
        std::abort();
    }
}

static target_policy policy(version v,unsigned burst=8)
{
    target_policy p{v,scope::global,"",true,{{"gateway-socket","/tmp/uwvm-close-test.sock"},
        {"gateway-uid","0"},{"node-name","uwvm_close_test"},{"namespace","/"},
        {"connect-timeout-ms","1000"},{"audit-deny-max-rate-hz","100"},{"audit-deny-burst","100"},
        {"audit-deny-max-records","1000"},{"audit-deny-max-bytes","65536"}}, {}};
    if(v==version::ros1) p.connection.insert({{"master-uri","http://127.0.0.1:11311"},{"advertise-host","127.0.0.1"},{"transport","tcpros"}});
    else p.connection.insert({{"domain-id","186"},{"rmw-implementation","rmw_fastrtps_cpp"},
        {"security-mode","disabled-test"},{"keystore","not-applicable"},{"enclave","not-applicable"}});
    capability_spec cap{"sub","topic.subscribe","/uwvm_next_20261013/handle_test",
        v==version::ros1?"std_msgs/String":"std_msgs/msg/String",{},
        {{"max-message-bytes","64"},{"max-rate-hz","0.001"},{"burst",fast_io::concat_std(burst)},
         {"max-bytes-per-second","1024"},{"max-queue-bytes","512"},{"overflow-policy","reject"},
         {"validator","none"},{"max-subscribers","2"},{"audit","allow-and-deny"},
         {"audit-max-rate-hz","100"},{"audit-burst","100"},{"audit-max-records","1000"},{"audit-max-bytes","65536"}}};
    cap.type_identity=builtin_type_identity(v,cap.type);
    if(v==version::ros1)cap.limits.insert({{"queue-depth","8"},{"transport","tcpros"}});
    else cap.limits.insert({{"max-inflight","2"},{"qos.reliability","reliable"},{"qos.durability","volatile"},
        {"qos.history","keep-last"},{"qos.depth","8"},{"qos.deadline","infinite"},{"qos.lifespan","infinite"},
        {"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}});
    p.capabilities.emplace(cap.alias,cap);cap.alias="other";cap.resource="/uwvm_next_20261013/other_handle_test";p.capabilities.emplace(cap.alias,cap);
    require(validate_target(p));return p;
}

static void register_for(capability_manager& manager,int& instance,version v,target_policy const& p)
{require(manager.register_instance(&instance,1,"main",v==version::ros1?&p:nullptr,v==version::ros2?&p:nullptr));}

int main()
{
    for(auto v:{version::ros1,version::ros2})
    {
        auto p=policy(v);int instance{},foreign{};
        {
            capability_manager manager;register_for(manager,instance,v,p);
            std::uint64_t h1{},h2{},h3{},other{},extra{};bool last=true;authorization lease;
            require(manager.open(&instance,1,v,"sub",h1));require(manager.open(&instance,1,v,"sub",h2));
            require(manager.open(&instance,1,v,"other",other));assert(h1!=h2);
            require(manager.open(&instance,1,v,"sub",extra),errc::rate_limited);assert(extra==0);
            require(manager.close(&instance,1,v,h2,&last));assert(!last);
            require(manager.describe(&instance,1,v,h2,lease),errc::bad_handle);
            require(manager.authorize(&instance,1,v,h1,"topic.subscribe",{},lease,false,false));lease.release();
            last=true;require(manager.close(&instance,1,v,h2,&last),errc::bad_handle);assert(!last);
            last=true;require(manager.close(&foreign,1,v,h1,&last),errc::permission_denied);assert(!last);
            last=true;require(manager.close(&instance,1,v==version::ros1?version::ros2:version::ros1,h1,&last),errc::bad_handle);assert(!last);
            require(manager.open(&instance,1,v,"sub",h3));assert(h3!=h2);
            require(manager.open(&instance,1,v,"sub",extra),errc::rate_limited);assert(extra==0);
            require(manager.close(&instance,1,v,h1,&last));assert(!last);
            require(manager.close(&instance,1,v,h3,&last));assert(last);
            require(manager.describe(&instance,1,v,other,lease));lease.release();
            require(manager.describe(&instance,1,v,h1,lease),errc::bad_handle);
            require(manager.describe(&instance,1,v,h3,lease),errc::bad_handle);
            require(manager.open(&instance,1,v,"sub",extra));assert(extra!=h1&&extra!=h2&&extra!=h3);
            require(manager.close(&instance,1,v,extra,&last));assert(last);
            last=true;require(manager.close(&instance,1,v,extra,&last),errc::bad_handle);assert(!last);
            require(manager.close(&instance,1,v,other,&last));assert(last);
            fast_io::io::println("PASS ",to_string(v)," sibling close, stale generation, reopen, count and independent alias");
        }
        {
            capability_manager manager;register_for(manager,instance,v,p);std::uint64_t h1{},h2{};
            require(manager.open(&instance,1,v,"sub",h1));require(manager.open(&instance,1,v,"sub",h2));
            std::atomic<unsigned> ready{};std::atomic<bool> go{};bool first{},second{};
            auto close=[&](std::uint64_t h,bool& last){++ready;while(!go.load())std::this_thread::yield();require(manager.close(&instance,1,v,h,&last));};
            std::thread a{close,h1,std::ref(first)},b{close,h2,std::ref(second)};
            while(ready.load()!=2)std::this_thread::yield();go=true;a.join();b.join();assert(first!=second);
            fast_io::io::println("PASS ",to_string(v)," concurrent sibling closes report exactly one final reference");
        }
        {
            capability_manager manager;register_for(manager,instance,v,p);std::uint64_t h{},fresh{};bool last{};authorization description;
            require(manager.open(&instance,1,v,"sub",h));require(manager.describe(&instance,1,v,h,description));
            require(manager.close(&instance,1,v,h,&last));assert(last);
            std::atomic<bool> started{};
            auto reopened=std::async(std::launch::async,[&]{started=true;return manager.open(&instance,1,v,"sub",fresh);});
            while(!started.load())std::this_thread::yield();
            assert(reopened.wait_for(std::chrono::milliseconds(20))==std::future_status::timeout);
            description.release();assert(reopened.wait_for(std::chrono::seconds(1))==std::future_status::ready);
            require(reopened.get());assert(fresh!=h);require(manager.close(&instance,1,v,fresh,&last));assert(last);
            fast_io::io::println("PASS ",to_string(v)," final-close description lease blocks alias reopening until teardown finishes");
        }
        {
            auto bounded=policy(v,4);capability_manager manager;register_for(manager,instance,v,bounded);std::uint64_t h1{},h2{},h3{},h4{},extra{};bool last{};
            require(manager.open(&instance,1,v,"sub",h1));require(manager.open(&instance,1,v,"sub",h2));
            require(manager.close(&instance,1,v,h2,&last));assert(!last);require(manager.open(&instance,1,v,"sub",h3));
            require(manager.close(&instance,1,v,h1,&last));assert(!last);require(manager.close(&instance,1,v,h3,&last));assert(last);
            require(manager.open(&instance,1,v,"sub",h4));require(manager.close(&instance,1,v,h4,&last));assert(last);
            require(manager.open(&instance,1,v,"sub",extra),errc::rate_limited);assert(!extra);
            fast_io::io::println("PASS ",to_string(v)," nonfinal and final close do not refill alias creation quota");
        }
        {
            capability_manager manager;register_for(manager,instance,v,p);std::uint64_t h1{},h2{},other{},extra{};bool last=true;authorization lease;
            require(manager.open(&instance,1,v,"sub",h1));require(manager.open(&instance,1,v,"sub",h2));require(manager.open(&instance,1,v,"other",other));
            require(manager.revoke(&instance,1,v,"sub"));require(manager.describe(&instance,1,v,h1,lease),errc::bad_handle);require(manager.describe(&instance,1,v,h2,lease),errc::bad_handle);
            require(manager.close(&instance,1,v,h1,&last),errc::bad_handle);assert(!last);
            require(manager.open(&instance,1,v,"sub",extra),errc::revoked);assert(!extra);
            require(manager.describe(&instance,1,v,other,lease));lease.release();require(manager.close(&instance,1,v,other,&last));assert(last);
            fast_io::io::println("PASS ",to_string(v)," revoke still invalidates every sibling and permanently prevents reopening");
        }
    }
}
