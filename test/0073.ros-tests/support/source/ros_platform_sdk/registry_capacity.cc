/** Compiled schema-count boundaries only; no invented native type identities.
 * Compile and run in the Linux test owner with the actual production headers.
 * This checks no ROS deployment, driver, permissions or performance claim.
 */
#include <uwvm2/imported/ros/ipc/custom_interface.h>
#include <limits>

namespace ipc = uwvm2::imported::ros::ipc;
#ifndef UWVM_TEST_EXPECTED_REGISTRY_CAPACITY
#define UWVM_TEST_EXPECTED_REGISTRY_CAPACITY 128
#endif
static_assert(ipc::custom_registry_max_interfaces == UWVM_TEST_EXPECTED_REGISTRY_CAPACITY);
#ifndef UWVM_TEST_EXPECTED_REGISTRY_CELLS
#define UWVM_TEST_EXPECTED_REGISTRY_CELLS 16384
#endif
static_assert(ipc::custom_registry_max_cells == UWVM_TEST_EXPECTED_REGISTRY_CELLS);
static_assert(ipc::custom_registry_cells_fit(0, 0));
static_assert(ipc::custom_registry_cells_fit(0, ipc::custom_registry_max_cells));
static_assert(ipc::custom_registry_cells_fit(ipc::custom_registry_max_cells, 0));
static_assert(!ipc::custom_registry_cells_fit(ipc::custom_registry_max_cells, 1));
static_assert(!ipc::custom_registry_cells_fit(0, ipc::custom_registry_max_cells + 1));
static_assert(!ipc::custom_registry_cells_fit(::std::numeric_limits<::std::uint64_t>::max(), 1));
static_assert(!ipc::custom_registry_cells_fit(1, ::std::numeric_limits<::std::uint64_t>::max()));
static_assert(ipc::custom_registry_counts_fit(0, 0, 0));
static_assert(ipc::custom_registry_counts_fit(ipc::custom_registry_max_interfaces, 0, 0));
static_assert(ipc::custom_registry_counts_fit(0, ipc::custom_registry_max_interfaces, 0));
static_assert(ipc::custom_registry_counts_fit(0, 0, ipc::custom_registry_max_interfaces));
static_assert(!ipc::custom_registry_counts_fit(ipc::custom_registry_max_interfaces, 1, 0));
static_assert(!ipc::custom_registry_counts_fit(0, ipc::custom_registry_max_interfaces, 1));
static_assert(!ipc::custom_registry_counts_fit(0, 0, ipc::custom_registry_max_interfaces + 1));
static_assert(!ipc::custom_registry_counts_fit(::std::numeric_limits<::std::size_t>::max(), 1, 1));
static_assert(!ipc::custom_registry_counts_fit(1, ::std::numeric_limits<::std::size_t>::max(), 1));
static_assert(!ipc::custom_registry_counts_fit(1, 1, ::std::numeric_limits<::std::size_t>::max()));
static_assert(ipc::effective_custom_topic_entries().size == 0);
static_assert(ipc::effective_custom_service_entries().size == 0);
static_assert(ipc::effective_custom_action_entries().size == 0);
// These names are real captured ROS1 declarations, not native identities.
// A legal name must remain insufficient to create an endpoint or permission.
static_assert(ipc::complete_vendor_native_name(1, "kuavo_msgs/sensorsData"));
static_assert(ipc::complete_vendor_endpoint_name(1, "kuavo_msgs/jointCmd", "msg"));
static_assert(ipc::complete_vendor_endpoint_name(1, "kuavo_msgs/changeArmCtrlMode", "srv"));
static_assert(!ipc::complete_vendor_native_name(2, "kuavo_msgs/msg/sensorsData"));
static_assert(!ipc::complete_vendor_native_name(1, "kuavo_msgs/1jointCmd"));
static_assert(!ipc::complete_vendor_native_name(1, "kuavo_msgs/_jointCmd"));
static_assert(!ipc::complete_vendor_native_name(1, "kuavo_msgs/jointCmd/extra"));
static_assert(!ipc::complete_vendor_endpoint_name(1, "std_msgs/String", "msg"));

int main()
{
    // Capacity cannot admit missing tables, empty topics or fabricated proof.
    ipc::custom_topic_descriptor missing{};
    if(ipc::custom_topic_registry_well_formed({nullptr, 1})) { return 1; }
    if(ipc::custom_topic_registry_well_formed({&missing, 1})) { return 2; }
    if(ipc::find_custom_topic(2, "dsr_msgs2/msg/RobotState")) { return 3; }
    if(ipc::find_custom_service(2, "dsr_msgs2/srv/MoveStop")) { return 4; }
    if(ipc::find_custom_topic(1, "kuavo_msgs/sensorsData")) { return 9; }
    // These zero descriptors have no native identities and remain invalid.
    // Only the merge's count/pointer precondition is exercised here.
    ::std::array<ipc::custom_topic_descriptor,ipc::custom_registry_max_interfaces> invalid{};
    ipc::custom_topic_registry whole{invalid.data(),invalid.size()}, empty{nullptr,0};
    if(!ipc::custom_registry_detail::mergeable(whole,empty)) { return 5; }
    if(ipc::custom_registry_detail::mergeable(whole,ipc::custom_topic_registry{invalid.data(),1})) { return 6; }
    if(ipc::custom_registry_detail::mergeable(empty,ipc::custom_topic_registry{nullptr,invalid.size()})) { return 7; }
    if(ipc::custom_topic_registry_well_formed(whole)) { return 8; }
    return 0;
}
