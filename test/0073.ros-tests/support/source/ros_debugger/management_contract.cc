/** Actual capability-manager retirement component; no Runtime or ROS network.
 *
 * The owner supplies the real current headers/compiler and runs this in a new
 * bounded process. Trusted native registrations below are fixture state, not
 * Guest caller identities or proof of Runtime commit ordering. Nothing in this
 * component exposes the native management hook as a WebAssembly import.
 */
#include "management_test_policy.h"
#include <uwvm2/imported/ros/common/management.h>
#include <fast_io.h>

namespace ros=uwvm2::imported::ros;
namespace diagnostic=ros::debug;
namespace management=ros::management;
static unsigned checks{},notifications{};
static void const* observed_object{};
static std::uint64_t observed_epoch{};
static management::transition observed_reason{};
static void check(bool condition,char const* detail)
{
    ++checks;
    if(!condition) {fast_io::io::perrln("FAIL_ROS_MANAGEMENT ",fast_io::mnp::os_c_str(detail));fast_io::fast_terminate();}
}
static void notification(management::transition reason,void const* object,std::uint64_t epoch) noexcept
{++notifications;observed_reason=reason;observed_object=object;observed_epoch=epoch;}
static diagnostic::row const& row(diagnostic::view const& view,std::string_view alias)
{
    for(std::size_t index{};index!=view.size;++index) {if(view.rows[index].alias.view()==alias) {return view.rows[index];}}
    check(false,"expected existing diagnostic alias");fast_io::fast_terminate();
}
int main()
{
    ros::capability_manager manager;int replaced{},unrelated{},empty{},unknown{},fresh{};
    auto one=ros_management_test::policy(ros::version::ros1);
    auto two=ros_management_test::policy(ros::version::ros2);
    check(bool(ros::validate_target(one))&&bool(ros::validate_target(two)),"both explicit component policies validate");
    check(bool(manager.register_instance(&replaced,7,"replaced",&one,&two)),"register replaced fixture owner");
    check(bool(manager.register_instance(&unrelated,7,"unrelated",&one,&two)),"register unrelated fixture owner");
    check(bool(manager.register_instance(&empty,7,"default_denied",nullptr,nullptr)),"register inert fixture owner");
    check(!manager.retire_registered(&unknown,7)&&!manager.retire_registered(&replaced,8)&&
          !manager.retire_registered(nullptr,7)&&!manager.retire_registered(&replaced,0),"unknown or wrong incarnation cannot retire an owner");

    std::uint64_t first[2]{},other[2]{};
    for(unsigned index{};index!=2;++index)
    {
        auto version=static_cast<ros::version>(index+1);std::uint64_t revoked{};
        check(bool(manager.open(&replaced,7,version,"sub",first[index]))&&
              bool(manager.open(&unrelated,7,version,"sub",other[index])),"each backend has distinct real handles");
        check(bool(manager.open(&replaced,7,version,"revoked",revoked))&&
              bool(manager.revoke(&replaced,7,version,"revoked")),"existing revocation predates management");
    }
    diagnostic::request query{};query.backend=2;query.module.assign("replaced");
    diagnostic::view before{},after{},again{};manager.debug_snapshot(query,before);
    check(before.result==diagnostic::status::ok&&before.size==2&&row(before,"sub").active_handles==1,
          "pre-retirement snapshot observes actual active handle");
    check(manager.retire_registered(&replaced,7),"retire existing exact incarnation");
    for(unsigned index{};index!=2;++index)
    {
        auto version=static_cast<ros::version>(index+1);ros::authorization lease;std::uint64_t denied{123};
        check(manager.describe(&replaced,7,version,first[index],lease).code==ros::errc::permission_denied&&!lease,
              "old handle loses authorization on both backends");
        check(manager.open(&replaced,7,version,"sub",denied).code==ros::errc::permission_denied&&denied==0,
              "same incarnation cannot reopen a fresh handle");
        check(bool(manager.describe(&unrelated,7,version,other[index],lease))&&bool(lease),"unrelated owner's existing handle survives");
        lease.release();
        check(manager.open(&empty,7,version,"sub",denied).code==ros::errc::permission_denied,
              "management does not grant the default-denied owner");
    }
    check(manager.register_instance(&replaced,7,"replaced",&one,&two).code==ros::errc::conflict,
          "inert registration prevents same-incarnation reconfiguration");
    manager.debug_snapshot(query,after);
    check(after.result==diagnostic::status::ok&&after.size==before.size,"retired aliases remain diagnostic records");
    for(auto alias:{"sub","revoked"})
    {
        auto const& original=row(before,alias);auto const& current=row(after,alias);
        check(!current.alive&&current.retired&&current.active_handles==0,"retired row carries no live handles");
        check(current.instance==original.instance&&current.generation==original.generation&&
              current.open_count==original.open_count&&current.revoked==original.revoked,
              "identity, creation count and revocation history remain unchanged");
        check(current.operation_tokens==original.operation_tokens&&current.creation_tokens==original.creation_tokens&&
              current.byte_tokens==original.byte_tokens,"retirement and reads do not replenish quotas");
    }
    check(manager.retire_registered(&replaced,7),"repeated retirement is idempotent");
    manager.debug_snapshot(query,again);
    check(row(again,"sub").open_count==row(after,"sub").open_count&&
          row(again,"sub").creation_tokens==row(after,"sub").creation_tokens,"repeat does not reset accounting");
    diagnostic::trace_record label{};
    check(manager.debug_identity(&replaced,7,ros::version::ros2,"sub",label)&&label.instance==row(before,"sub").instance,
          "read-only labels remain available without a new authorization lease");
    check(bool(manager.register_instance(&fresh,8,"fresh_default_denied",nullptr,nullptr)),"fresh fixture registration is separately inert");
    std::uint64_t denied{};
    check(manager.open(&fresh,8,ros::version::ros2,"sub",denied).code==ros::errc::permission_denied,
          "fresh registration alone is not an explicit launch grant");

    // Native-only hook forwarding is exercised with real atomics. This test
    // deliberately replaces no installed Host hook; this is a standalone TU.
    auto previous=management::retirement_hook.exchange(&notification);
    management::retire(management::transition::function_replacement,&replaced,7);
    check(notifications==1&&observed_object==&replaced&&observed_epoch==7&&
          observed_reason==management::transition::function_replacement,"native notification preserves exact old identity");
    management::retire(management::transition::runtime_retirement,nullptr,7);
    check(notifications==2&&observed_reason==management::transition::runtime_retirement,"runtime drain has a distinct notification");
    management::retirement_hook.store(nullptr);management::retire(management::transition::function_replacement,&fresh,8);
    check(notifications==2,"ROS-off native notification creates no authority");management::retirement_hook.store(previous);
    fast_io::io::println("PASS_ROS_MANAGEMENT checks=",checks);
}
