#include <uwvm2/imported/ros/policy/policy.h>
#include "common.h"
#include <set>

int main()
{
    using namespace action_qos_test;
    unsigned checks{};
    auto require=[&](core::target_policy const& policy,core::errc expected,std::string const& label) {
        auto result{core::validate_target(policy)};++checks;
        check(result.code==expected,label+": unexpected policy errno "+std::to_string(static_cast<unsigned>(result.code)));
        if(expected==core::errc::conflict) { ++checks;check(result.detail==diagnostic,label+": wrong conflict phase"); }
    };
    try
    {
        std::set<std::string_view> expected,actual;
        for(auto const& row:mutations) { check(expected.emplace(row.key).second,"duplicate independent matrix key"); }
        for(auto key:core::action_server_qos_fields) { check(actual.emplace(key).second,"duplicate production field"); }
        check(actual==expected&&expected.size()==40,"contract does not cover exactly the independent 40 fields");
        require(pair(),core::errc::success,"matching explicit 40-field baseline");
        for(auto const& row:mutations)
        {
            for(bool enabled:{false,true})
            {
                auto policy{pair(enabled)};auto& response{policy.capabilities.at("respond")};response.limits[row.key]=row.value;
                auto individual{core::validate_capability(core::version::ros2,response)};++checks;
                check(individual.code==(row.valid?core::errc::success:core::errc::unsupported),std::string{row.key}+": wrong individual validity");
                require(policy,enabled&&!row.valid?core::errc::unsupported:core::errc::conflict,std::string{row.key}+" responder mutation");
                policy.capabilities.at("advertise").limits[row.key]=row.value;
                require(policy,enabled&&!row.valid?core::errc::unsupported:core::errc::success,std::string{row.key}+" equal mutation");
                policy.capabilities.at("respond").limits[row.key]=row.baseline;
                require(policy,enabled&&!row.valid?core::errc::unsupported:core::errc::conflict,std::string{row.key}+" advertiser mutation");
            }
            auto missing{pair()};missing.capabilities.at("respond").limits.erase(row.key);
            check(!core::equal_action_server_qos_contract(missing.capabilities.at("advertise"),missing.capabilities.at("respond")),std::string{row.key}+": missing field did not fail closed");++checks;
            require(missing,core::errc::invalid_argument,std::string{row.key}+" missing field");
            missing=pair();missing.capabilities.at("advertise").limits.erase(row.key);
            check(!core::equal_action_server_qos_contract(missing.capabilities.at("advertise"),missing.capabilities.at("respond")),std::string{row.key}+": missing advertiser field did not fail closed");++checks;
            require(missing,core::errc::invalid_argument,std::string{row.key}+" missing advertiser field");
        }
        for(auto const& row:mutations)
        {
            if(std::string_view{row.key}.find(".depth")==std::string_view::npos) { continue; }
            auto policy{pair()};policy.capabilities.at("respond").limits[row.key]="08";
            require(policy,core::errc::conflict,std::string{row.key}+" exact decimal 8 versus 08");
            policy.capabilities.at("advertise").limits[row.key]="08";
            require(policy,core::errc::success,std::string{row.key}+" equal padded decimal");
        }
        auto independent{pair()};independent.capabilities.at("respond").resource="/uwvm_action_qos/other";
        independent.capabilities.at("respond").limits["qos.feedback.depth"]="9";
        require(independent,core::errc::success,"different resource has independent contract");
        auto response_only{pair()};response_only.capabilities.erase("advertise");require(response_only,core::errc::success,"no counterpart required");
        auto advertise_only{pair()};advertise_only.capabilities.erase("respond");require(advertise_only,core::errc::success,"advertiser without responder");
        auto first{pair()},second{pair()};first.target_scope=second.target_scope=core::scope::single;first.name="first";second.name="second";
        second.capabilities.at("respond").limits["qos.feedback.depth"]="9";second.capabilities.at("advertise").limits["qos.feedback.depth"]="9";
        require(first,core::errc::success,"first independent target");require(second,core::errc::success,"second independent target");
        fast_io::println("PASS ROS2 Action explicit QoS policy fields=40 valid_mismatch=35 unsupported_history=5 checks=",checks);
        return 0;
    }
    catch(std::exception const& error) { fast_io::perrln("FAIL ",std::string_view{error.what()});return 1; }
}
