#include "../../gateways/common/config.h"
#include <uwvm2/imported/ros/capability/manager.h>
#include <uwvm2/imported/ros/common/manifest.h>
#include <atomic>
#include <thread>
#include <span>
#include <filesystem>
#include <unistd.h>
#include <string>
#include <string_view>
#include <fast_io.h>
#include <fast_io_unit/string.h>

namespace core = uwvm2::imported::ros;
namespace ipc = core::ipc;
void require(bool value, std::string const& detail)
{ if(!value) { throw std::runtime_error{detail}; } }
std::vector<std::uint8_t> payload(std::string const& value)
{
    std::vector<std::uint8_t> result; ipc::put(result, value.size(), 4);
    result.insert(result.end(), value.begin(), value.end()); return result;
}
// Markers are visible only after a complete checked write/close.  The runner
// supplies a private directory; exclusive 0600 staging rejects stale/colliding
// temporary files rather than silently consuming partially written evidence.
template<typename... Args>
void write_marker(std::string_view directory, std::string_view name, Args const&... args)
{
    auto path{fast_io::concat_std(directory,"/",name)};
    auto temporary{fast_io::concat_std(path,".pending")};
    bool created{};
    try
    {
        {
            fast_io::native_file file{temporary,fast_io::open_mode::out|fast_io::open_mode::creat|fast_io::open_mode::excl,
                static_cast<fast_io::perms>(0600)};
            created=true;
            fast_io::io::println(file,args...);
            file.close();
        }
        fast_io::native_renameat(fast_io::at_fdcwd(),temporary,fast_io::at_fdcwd(),path);
    }
    catch(...)
    {
        if(created) { try { fast_io::native_unlinkat(fast_io::at_fdcwd(),temporary); } catch(...) {} }
        throw;
    }
}
int main(int argc, char** argv)
{
    try
    {
        require(argc == 4, "usage revoke-bridge fixed-config actual-socket marker-directory");
        auto cfg{uwvm2::ros_gateway::load(argv[1])}; auto const version{cfg.policy.ros_version};
        cfg.policy.connection.insert({{"gateway-socket",argv[2]}, {"gateway-uid",fast_io::concat_std(fast_io::mnp::dec(::getuid()))},
            {"connect-timeout-ms","1000"}, {"audit-deny-max-rate-hz","100"}, {"audit-deny-burst","100"},
            {"audit-deny-max-records","10000"}, {"audit-deny-max-bytes","1048576"}});
        ipc::client client; require(!client.connect(argv[2],static_cast<std::uint8_t>(version),::getuid(),1000),"authenticated IPC connect");
        require(!client.call("session.bind","target",core::binding_manifest(cfg.policy),1000).error,"exact manifest bind");
        auto const marker_directory{std::string_view{argv[3]}};
        auto const warmup_marker{fast_io::concat_std(marker_directory,"/warmup.ready")};
        auto const observation_marker{fast_io::concat_std(marker_directory,"/observation.done")};
        auto deadline{std::chrono::steady_clock::now()+std::chrono::seconds{6}};
        while(!std::filesystem::exists(warmup_marker))
        {
            require(std::chrono::steady_clock::now()<deadline,"actual native data-path warmup deadline");
            require(!client.call("topic.publish","pub",payload("warmup"),1000).error,"native readiness publish");
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
        }
        core::capability_manager manager; int trusted_native_test_object{}; std::uint64_t handle{};
        auto registered{manager.register_instance(&trusted_native_test_object,1,"native-race-test",version==core::version::ros1?&cfg.policy:nullptr,version==core::version::ros2?&cfg.policy:nullptr)};
        require(bool(registered),"native test manager instance: "+registered.detail);
        require(bool(manager.open(&trusted_native_test_object,1,version,"pub",handle)),"native test capability open");
        auto first{payload("race-0")}; core::authorization held;
        require(bool(manager.authorize(&trusted_native_test_object,1,version,handle,"topic.publish",std::as_bytes(std::span{first}),held)),"first admitted operation lease");
        std::atomic<bool> revoke_started{},revoke_finished{},failed{};
        std::atomic<unsigned> next{1},admitted{},denied{};
        auto publish_observed{[&](std::string const& value,std::vector<std::uint8_t> const& bytes) {
            if(client.call("topic.publish","pub",bytes,1000).error) { failed=true; return; }
            auto const until{std::chrono::steady_clock::now()+std::chrono::seconds{2}};
            auto const seen_marker{fast_io::concat_std(marker_directory,"/",value,".seen")};
            while(!std::filesystem::exists(seen_marker))
            {
                if(std::chrono::steady_clock::now()>until) { failed=true; return; }
                std::this_thread::sleep_for(std::chrono::milliseconds{10});
            }
            ++admitted;
        }};
        std::thread revoker{[&] {
            revoke_started=true;
            auto result{manager.revoke(&trusted_native_test_object,1,version,"pub")};
            if(!result || client.call("capability.revoke","pub",{},1000).error) { failed=true; }
            revoke_finished=true;
        }};
        std::vector<std::thread> threads;
        for(unsigned thread{};thread!=4;++thread)
        {
            threads.emplace_back([&] {
                for(;;)
                {
                    auto const number{next.fetch_add(1)}; if(number>=100) { return; }
                    auto const label{fast_io::concat_std("race-",fast_io::mnp::dec(number))};
                    auto bytes{payload(label)}; core::authorization lease;
                    auto result{manager.authorize(&trusted_native_test_object,1,version,handle,"topic.publish",std::as_bytes(std::span{bytes}),lease)};
                    if(!result) { if(result.code!=core::errc::bad_handle) { failed=true; } ++denied; continue; }
                    publish_observed(label,bytes);
                }
            });
        }
        while(!revoke_started) { std::this_thread::yield(); }
        std::this_thread::sleep_for(std::chrono::milliseconds{30});
        bool const revoke_waited{!revoke_finished};
        publish_observed("race-0",first);
        held.release();
        for(auto& thread:threads) { thread.join(); } revoker.join();
        require(revoke_waited&&!failed&&admitted+denied==100,"revoke waited for admitted IPC lease; all100 race operations classified");
        for(unsigned attempt{};attempt!=100;++attempt)
        {
            core::authorization lease;
            auto result{manager.authorize(&trusted_native_test_object,1,version,handle,"topic.publish",std::as_bytes(std::span{first}),lease)};
            require(result.code==core::errc::bad_handle,"retired handle denies all post-revoke admission");
            require(client.call("topic.publish","pub",first,1000).error==125,"actual gateway denies all post-revoke side effects");
        }
        require(manager.open(&trusted_native_test_object,1,version,"pub",handle).code==core::errc::revoked,"alias permanently revoked");
        auto const admitted_count{admitted.load()},denied_count{denied.load()};
        write_marker(marker_directory,"revoked.count",fast_io::mnp::dec(admitted_count));
        write_marker(marker_directory,"bridge.json","{\"race_attempts\":100,\"threads\":4,\"admitted\":",
            fast_io::mnp::dec(admitted_count),",\"denied\":",fast_io::mnp::dec(denied_count),
            ",\"revoke_waited_for_admitted_ipc_lease\":true,\"post_revoke_manager_denials\":100,\"post_revoke_gateway_denials\":100}");
        auto const observation_deadline{std::chrono::steady_clock::now()+std::chrono::seconds{20}};
        while(!std::filesystem::exists(observation_marker))
        {
            require(std::chrono::steady_clock::now()<observation_deadline,"native observer completion deadline");
            std::this_thread::sleep_for(std::chrono::milliseconds{10});
        }
        fast_io::io::println(fast_io::out(),"PASS real native manager IPC revoke race admitted=",fast_io::mnp::dec(admitted_count)," denied=",fast_io::mnp::dec(denied_count));
        manager.clear(); client.close(); return 0;
    }
    catch(std::exception const& error) { try { fast_io::io::perrln("FAIL ",fast_io::mnp::os_c_str(error.what())); } catch(...) {} return 1; }
}
