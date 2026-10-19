/** Complete C++ typed call path; all records are real generated aliases. */
#pragma once
#include "../fjt_client.hpp"
#include "../fjt_goal.h"
#include <array>
namespace uwvm_mobile {
struct RunReport {
    std::uint32_t primary{};std::uint64_t goal_id{};std::uint8_t terminal_status{};
    Feedback feedback{};Result result{};
};
/** Borrow already-open explicit roles. No generated/native identity is guessed.
 * The operator obtains telemetry and prepares the whole typed goal first.
 * Each callback result is one actual caller credit, never a fake clock.
 */
template<typename Credit> RunReport run(roswasmc_capability const& send,
    roswasmc_capability const& feedback,roswasmc_capability const& result,
    roswasmc_capability const& cancel,Goal const& complete_goal,Credit&& credit) {
    static_assert(noexcept(credit()),"credit callback must be noexcept; caller owns retirement");
    RunReport report{};TrajectoryClient client;std::uint32_t index{};
    report.primary=client.bind(send,feedback,result,cancel);if(report.primary)return report;
    if(credit()!='S'){report.primary=22;return report;}
    report.primary=client.send(complete_goal,index);if(report.primary)return report;
    report.goal_id=client.diagnostics().history[index].id;
    for(unsigned iteration{};iteration!=64;++iteration) {
        auto operation{credit()};
        if(operation=='R') {
            report.primary=client.result(index,report.terminal_status,report.result);
            if(!report.primary){(void)client.retire_local();return report;}
        } else if(operation=='F')report.primary=client.feedback(index,report.feedback);
        else if(operation=='C'){int accepted{};report.primary=client.cancel(index,accepted);}
        else {report.primary=22;break;}
        if(report.primary&&report.primary!=11)break;
    }
    if(!report.primary||report.primary==11)report.primary=90;
    (void)client.retire_local();return report;
}
}
