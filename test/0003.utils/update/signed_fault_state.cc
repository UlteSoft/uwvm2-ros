// Real Ed25519 authentication and product update state machine. No verifier stubs.
#include <uwvm2/utils/update/protocol.h>
#include <uwvm2/utils/control/debug_permissions.h>
#include <cstdio>
#include <ctime>
#if !defined(UWVM_WSEA_SUPPORTED)
#error "This regression requires the actual OpenSSL Ed25519 verifier."
#endif
namespace update=uwvm2::utils::update;
static bool sign(EVP_PKEY* key,update::permit& value)
{
    auto bytes=value.unsigned_encoding();auto* ctx=EVP_MD_CTX_new();if(!ctx)return false;
    std::size_t length=value.signature.size();
    bool okay=EVP_DigestSignInit(ctx,nullptr,nullptr,nullptr,key)==1 &&
        EVP_DigestSign(ctx,value.signature.data(),&length,bytes.data(),bytes.size())==1 && length==value.signature.size();
    EVP_MD_CTX_free(ctx);return okay;
}
int main()
{
    unsigned checks{};
#define CHECK(expression) do{++checks;if(!(expression)){std::printf("FAIL line=%u checks=%u\n",__LINE__,checks);return 1;}}while(false)
    namespace permission=uwvm2::utils::control::debug_permissions;
    CHECK(permission::configure_trusted_launch(permission::permission_level::full));
    update::protocol protocol{};
    CHECK(protocol.observe().state==update::phase::disabled && !protocol.observe().active);
    CHECK(protocol.begin({})==update::status::unauthorized); // full debug is not an update permit
    std::array<unsigned char,32> seed{};for(unsigned i{};i<seed.size();++i)seed[i]=i+1u;
    auto* raw=EVP_PKEY_new_raw_private_key(EVP_PKEY_ED25519,nullptr,seed.data(),seed.size());CHECK(raw);
    uwvm2::utils::security::wsea::detail::owned<EVP_PKEY,EVP_PKEY_free> key{raw};
    update::configuration config{};std::size_t size=config.public_key.size();
    CHECK(EVP_PKEY_get_raw_public_key(key.get(),config.public_key.data(),&size)==1 && size==config.public_key.size());
    config.contract[0]=1;config.scope[0]=2;update::digest base{};base[0]=3;
    CHECK(protocol.configure(config,base)==update::status::ok);
    update::permit launch{};launch.operation=update::action::launch;launch.issuer=protocol.observe().issuer;
    launch.nonce=protocol.observe().nonce;launch.transaction[0]=4;launch.base=base;launch.target=base;
    launch.contract=config.contract;launch.scope=config.scope;launch.target_epoch=1;launch.policy_generation=1;
    launch.expires_unix_seconds=static_cast<std::uint64_t>(std::time(nullptr))+300;launch.decision_sequence=1;
    CHECK(sign(key.get(),launch) && protocol.authorize_launch(launch)==update::status::ok && protocol.launch_activated());
    auto prepare=launch;prepare.operation=update::action::prepare;prepare.transaction[0]=5;prepare.base_epoch=1;
    prepare.target_epoch=2;prepare.target[0]=9;prepare.functions[0]=6;prepare.decision_sequence=0;
    CHECK(protocol.begin(prepare)==update::status::unauthorized && protocol.observe().active); // stale signature
    CHECK(sign(key.get(),prepare) && protocol.begin(prepare)==update::status::ok);
    update::digest prepared{},ready{};prepared[0]=7;ready[0]=8;
    CHECK(protocol.prepared(prepared) && protocol.ready(ready));
    auto commit=prepare;commit.operation=update::action::commit;commit.prepared=prepared;commit.ready=ready;commit.decision_sequence=2;
    CHECK(sign(key.get(),commit));auto forged=commit;forged.scope[0]^=1;
    CHECK(protocol.decide(forged)==update::status::unauthorized && protocol.observe().state==update::phase::waiting_for_decision);
    CHECK(protocol.decide(commit)==update::status::ok);protocol.installed();protocol.activated();
    CHECK(protocol.observe().active && protocol.observe().installed_epoch==2);
    auto another=prepare;another.transaction[0]=10;another.base=commit.target;another.base_epoch=2;another.target_epoch=3;another.target[0]=11;
    CHECK(sign(key.get(),another) && protocol.begin(another)==update::status::ok);
    protocol.guest_faulted();
    CHECK(!protocol.cancel_private() && !protocol.prepared(prepared) && !protocol.ready(ready));
    auto abort=another;abort.operation=update::action::abort;abort.decision_sequence=3;
    CHECK(sign(key.get(),abort) && protocol.decide(abort)==update::status::invalid_state);
    CHECK(protocol.begin(another)==update::status::busy);
    // An authenticated old launch receipt is only a receipt; neither its retry
    // nor activated() may turn the contaminated old epoch back into execution.
    CHECK(protocol.authorize_launch(launch)==update::status::ok && !protocol.launch_activated());
    protocol.installed();protocol.activated();
    CHECK(protocol.observe().state==update::phase::guest_fault && !protocol.observe().active && protocol.observe().installed_epoch==2);
    std::printf("PASS %u/%u real signed-update checks; full debug grants no update; faulted epoch cannot cancel/reopen/preserve/resume\n",checks,checks);
}
