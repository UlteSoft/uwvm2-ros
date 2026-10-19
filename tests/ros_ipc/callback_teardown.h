#pragma once
#include "probe_common.h"
namespace ros_probe
{
    template<typename Send,typename Count>
    void callback_teardown(core::version version,std::string const& executable,std::string const& resource,Send send,Count count)
    {
        fixture f{version,executable,[&](gateway::configuration& cfg){auto cap{cfg.policy.capabilities.at("subString")};cfg.policy.capabilities.clear();cap.resource=resource;cap.limits["max-message-bytes"]="64";cap.limits["max-queue-bytes"]="512";cfg.policy.connection["node-name"]="uwvm_callback_teardown_"+std::to_string(::getpid());cfg.policy.capabilities.emplace(cap.alias,std::move(cap));}};
        std::atomic<bool> running{true};std::thread ingress{[&]{while(running){send();std::this_thread::sleep_for(std::chrono::milliseconds(1));}}};
        try
        {
            for(unsigned round{};round!=20;++round)
            {
                ipc::client client;f.connect(client);require(!client.call("topic.subscribe","subString",{},1000).error,"actual racing subscription created");
                for(unsigned n{};n!=300&&!count();++n){std::this_thread::sleep_for(std::chrono::milliseconds(10));}require(count(),"native live publisher sees actual subscriber");
                bool received{};for(unsigned n{};n!=300&&!received;++n){auto event{client.call("event.poll","subString",{},1000)};if(!event.error){received=true;}else{require(event.error==11,"live ingress pending only");}std::this_thread::sleep_for(std::chrono::milliseconds(10));}require(received,"actual callback ran before teardown");
                bool revoke{round%2!=0};require(!client.call(revoke?"capability.revoke":"capability.close","subString",{},1000).error,"teardown races native ingress callbacks");
                for(unsigned n{};n!=300&&count();++n){std::this_thread::sleep_for(std::chrono::milliseconds(10));}require(!count(),"actual native subscriber removed during ingress");
                for(unsigned n{};n!=5;++n){auto event{client.call("event.poll","subString",{},1000)};require(event.error==(revoke?125u:9u)&&event.payload.empty(),"no late callback events after close/revoke");std::this_thread::sleep_for(std::chrono::milliseconds(5));}client.close();
            }
        }
        catch(...){running=false;ingress.join();throw;}
        running=false;ingress.join();::ros_probe::print_fixture(STDOUT_FILENO,"PASS actual ROS",static_cast<unsigned>(version)," twenty live native callback close/revoke races; zero remaining subscribers and no late events\n");
    }
}
