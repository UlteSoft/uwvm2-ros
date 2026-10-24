/**
 * @file cancel_contract.cc
 * @brief Prepared C++17 cancellation contract regression, without a ROS Context.
 *
 * Compile and execute only inside the approved Linux 64 GiB cgroup. This fixture
 * calls the production comparator and checks an independently written eight-row
 * matrix. It does not prove native cancellation or actual middleware QoS.
 */
#include "common.h"
#include "cancel_matrix.h"
#include <set>

int main()
{
    using namespace action_qos_test;
    unsigned checks{};
    auto require=[&](bool value,std::string const& label) { ++checks;check(value,label); };
    try
    {
        std::set<std::string_view> independent,production;
        for(auto const& row:cancellation_qos_test::mutations) { require(independent.emplace(row.key).second,"duplicate independent cancellation field"); }
        for(auto key:core::action_cancel_qos_fields) { require(production.emplace(key).second,"duplicate production cancellation field"); }
        require(independent==production&&independent.size()==8,"cancellation comparator differs from the independent eight fields");
        auto sent{capability("sent","action.send-goal")},cancel{capability("cancel","action.cancel-own-goal")};
        cancel.limits["cancel-scope"]="own";
        require(core::equal_action_cancel_qos_contract(sent,cancel),"matching cancellation baseline rejected");
        for(auto const& row:cancellation_qos_test::mutations)
        {
            require(sent.limits.at(row.key)==row.baseline,"independent baseline differs");
            auto changed{cancel};changed.limits[row.key]=row.replacement;
            require(!core::equal_action_cancel_qos_contract(sent,changed),std::string{row.key}+": cancellation mismatch accepted");
            auto changed_sent{sent};changed_sent.limits[row.key]=row.replacement;
            require(!core::equal_action_cancel_qos_contract(changed_sent,cancel),std::string{row.key}+": send mismatch accepted");
            require(core::equal_action_cancel_qos_contract(changed_sent,changed),std::string{row.key}+": equal explicit spelling rejected");
            // Unsupported keep-all is still rejected by the ordinary loader;
            // equality alone is not an individual capability validation.
            bool valid{};try { gateway::validate(core::version::ros2,changed);valid=true; }catch(std::exception const&) {}
            require(valid==row.individually_valid,std::string{row.key}+": independent field validity differs");
            changed=cancel;changed.limits.erase(row.key);
            require(!core::equal_action_cancel_qos_contract(sent,changed),std::string{row.key}+": missing cancellation field accepted");
            changed_sent=sent;changed_sent.limits.erase(row.key);
            require(!core::equal_action_cancel_qos_contract(changed_sent,cancel),std::string{row.key}+": missing send field accepted");
        }
        auto padded{cancel};padded.limits["qos.cancel-service.depth"]="08";
        require(!core::equal_action_cancel_qos_contract(sent,padded),"8 versus 08 did not retain exact spelling");
        sent.limits["qos.cancel-service.depth"]="08";
        require(core::equal_action_cancel_qos_contract(sent,padded),"equal padded decimal rejected");
        auto unrelated{padded};
        for(auto const& row:action_qos_test::mutations)
        { if(std::string_view{row.key}.find("qos.cancel-service.")!=0) { unrelated.limits[row.key]=row.value; } }
        require(core::equal_action_cancel_qos_contract(sent,unrelated),"an unused Action channel affected cancellation");
        line("PASS cancellation comparator independent_fields=8 checks="+std::to_string(checks));return 0;
    }
    catch(std::exception const& error)
    { gateway::native_io::write_all(STDERR_FILENO,std::string{"FAIL "}+error.what()+"\n");return 1; }
}
