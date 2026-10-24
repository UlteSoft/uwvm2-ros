/* SPDX-License-Identifier: Apache-2.0 */
/** UNRUN contracts against the actual production value/diagnostic helpers.
 * No fake ROS header, endpoint subclass, RCL symbol or Native entity appears.
 * These logical source cases are not actual middleware/Host/permission proof.
 */
#include "src/ros/gateway/ros2/native_rcl_service_registry.h"
#include <cstdlib>
namespace impl=uwvm2::ros_gateway::native_rcl_service_registry;
namespace wire=uwvm2::imported::ros::ipc::rmw_endpoint;
namespace transport=uwvm2::imported::ros::ipc::rmw_service;
namespace ros=uwvm2::imported::ros;
static void require(bool v){if(!v)std::abort();}
static void three_independent_failed_results()
{
    impl::diagnostics d{};impl::record_error(d,90);impl::record_cancel(d,110);impl::record_retire(d,95);
    require(d.first_error==90&&d.last_error==95&&d.cancel_attempted&&d.last_cancel_error==110&&d.retire_attempted&&d.last_retire_error==95);
}
static void failed_commit_survives_retirement()
{
    impl::diagnostics d{};impl::record_commit(d,5);impl::record_retire(d,110);
    require(d.first_error==5&&d.commit_attempted&&d.last_commit_error==5&&!d.last_commit_completed&&d.last_retire_error==110);
}
static void completed_commit_is_not_rollback()
{
    impl::diagnostics d{};impl::record_commit(d,0);impl::record_error(d,110);impl::record_retire(d,5);
    require(d.commit_attempted&&d.last_commit_completed&&d.last_commit_error==0&&d.first_error==110&&d.last_retire_error==5);
}
static void known_cancel_does_not_erase_primary()
{
    impl::diagnostics d{};impl::record_error(d,22);impl::record_cancel(d,0);impl::record_retire(d,0);
    require(d.first_error==22&&d.cancel_attempted&&d.last_cancel_error==0&&d.retire_attempted&&d.last_retire_error==0);
}
static void original_cap_full_snapshot()
{
    ros::capability_spec a{};a.alias="call";a.operation="service.call";a.resource="/service";
    a.type="example_interfaces/srv/AddTwoInts";a.type_identity="logical-not-a-native-proof";
    a.limits.emplace("qos.depth","1");auto b=a;require(impl::exact_cap(a,b));
    b.limits["qos.depth"]="2";require(!impl::exact_cap(a,b));b=a;b.type_identity="different";require(!impl::exact_cap(a,b));
}
static void finite_storage_bounds()
{
    require(!impl::limits{0,1}.valid()&&!impl::limits{1,0}.valid()&&!impl::limits{65537,1}.valid());
    require(!impl::limits{1,257}.valid()&&impl::limits{1,1}.valid()&&impl::limits{65536,256}.valid());
}
static void unavailable_factory_cannot_supply_a_native_owner()
{
    ros::capability_spec cap{};cap.limits.emplace("adapter-codec-identity","explicit-unproven");
    std::shared_ptr<impl::endpoint> owner;wire::packet receipt{};receipt.incarnation=91;
    require(impl::create_fixed({}, {},cap,{},0,1,{},owner,receipt)==95&&!owner&&receipt.incarnation==91);
}
static void no_authority_or_default_native_registry()
{
    impl::registry r{{},{},{1,1}};impl::admitted a{};transport::control p{};
    p.owner={2,UINT64_C(0x100000001),2,3,4};p.peer=UINT64_C(0x100000002);
    auto result=r.execute(a,p,nullptr,0,uwvm2::ros_gateway::ipc::after(100));
    require(result.error==13&&result.payload.empty()&&r.bindings_after_quiescence().empty());
    impl::diagnostics d{};d.first_error=73;uwvm2::ros_gateway::native_rcl_service::diagnostic n{};
    require(r.observe_after_quiescence(p.owner.source,d,n)==9&&d.first_error==73);
}
int main()
{
    three_independent_failed_results();failed_commit_survives_retirement();completed_commit_is_not_rollback();
    known_cancel_does_not_erase_primary();original_cap_full_snapshot();finite_storage_bounds();
    unavailable_factory_cannot_supply_a_native_owner();no_authority_or_default_native_registry();
}
