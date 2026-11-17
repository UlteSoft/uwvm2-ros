/** @file @brief Revocation must not join a pump that needs the revoked authority. */
#define UWVM_COMMUNICATION_SERVICE_FIXTURE_ONLY
#include "communication_service_endpoint.cpp"
int main()
{
    service_environment f;auto receipt=f.submit();assert(receipt.request&&f.charges==1);f.pause_admission=true;
    std::atomic<bool> revoked{};c::error progress{};
    std::thread pumping{[&]{progress=f.endpoint->pump(f.pipes.transport);}};
    auto limit=c::now()+5000000000;
    while(!f.admission_entered.load()){assert(c::now()<limit);std::this_thread::yield();}
    std::thread revoking{[&]
    {
        assert(f.permissions.revoke("service",[&](auto){f.registry.revoke(f.owner);})==s::error::success);
        revoked=true;
    }};
    // The watchdog detects a lock-order bug, not an industrial latency promise.
    while(!revoked.load()){assert(c::now()<limit);std::this_thread::yield();}
    f.resume_admission=true;revoking.join();pumping.join();
    assert(progress==c::error::ok||progress==c::error::denied||progress==c::error::cancelled||progress==c::error::io);
    assert(!f.pipes.drain()&&f.charges==1);
    // Only the supervisor drains, after both authority and registry locks end.
    f.endpoint->drain();s::lease held;assert(f.permissions.admit(f.who,f.caps[0],"service.submit",0,held,false)==s::error::denied);
    std::puts("PASS independent service revocation: authority fence completes before paused admission, no native send, no repeated charge and separate supervisor drain");
}
