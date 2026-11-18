/** @file @brief Exercise declaration-only validation, output refusal and provider isolation. */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
#include <iostream>

namespace s=uwvm2::imported::ros::scene;
using E=s::error;

int main()
{
    s::deployment review{s::deployment::mode::offline_review};
    uwvm2::utils::container::std_string json{"stale"};
    assert(review.dump_effective_policy(json)==E::denied && json.empty());
    assert(review.create("robot")==E::success && review.bind("robot","main")==E::success);
    assert(review.bind("robot",uwvm2::utils::container::std_string("extended-\xff",10))==E::success);
    assert(review.bind("robot","quoted-\"\\")==E::success);
    s::grant driver;driver.kind=s::purpose::driver;driver.alias="joint";driver.resource="elbow";driver.binding="registered-driver";
    driver.actions.emplace("driver.connect");
    assert(review.allow("robot",driver)==E::success);
    assert(review.expose("robot","driver.connect",s::exposure::allow)==E::success);
    assert(review.driver_budget("robot",1,2000000)==E::success);
    s::drivers::protocol::binding identity{{1,2},{3,4},{5,6},{7,8},{9,10},{11,12,13,14}};
    // Neither the peer nor any of the three provisioner descriptors exist.
    assert(review.driver_bind("robot","joint","main",1023456,{12345,222,333},identity,{32,48,50000,900000,1023457,1023458})==E::success);
    assert(review.validate()==E::invalid);
    assert(review.driver_schema("robot","joint","parameter.exists",{0,6,7,8})==E::success);
    assert(review.configure_payload_journal(1023459,{5,1024,64})==E::success);
    assert(!s::module_buffer_audit::output.enabled());
    assert(review.validate()==E::success && review.dump_effective_policy(json)==E::success);
    assert(review.prepare_resources([](auto const&...){assert(false);return E::io;})==E::denied);
    std::shared_ptr<s::broker> broker;
    assert(review.select_exposed("main","driver.connect",{&review,1},broker)==E::denied && !broker);
    std::cout.write(json.data(),static_cast<std::streamsize>(json.size()))<<'\n';
    auto first=json;
    assert(review.dump_effective_policy(json)==E::success && first==json);
    review.retire();
    assert(review.dump_effective_policy(json)==E::denied && json.empty());
    s::deployment oversized{s::deployment::mode::offline_review};
    assert(oversized.create("robot")==E::success);
    for(std::size_t i{};i<s::policy_dump::writer::maximum_rows;++i)
        assert(oversized.bind("robot",uwvm2::utils::container::to_string(i))==E::success);
    assert(oversized.validate()==E::success);
    json="stale";assert(oversized.dump_effective_policy(json)==E::too_large && json.empty());
}
