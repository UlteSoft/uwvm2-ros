/** Compile-only actual Host numeric signatures. No definitions/stubs/provider.
 * Requires the genuine Runtime build's complete source/include/flag closure.
 * This is not the standalone pure-value CMake target and is NOT RUN here.
 */
#if !defined(UWVM_IMPORT_ROS2)
#error "The actual reviewed ROS2 Runtime compile configuration is required"
#endif
#include <uwvm2/uwvm/imported/ros/host.h>
#include <type_traits>
namespace h=uwvm2::uwvm::imported::ros::host;
using u32=std::uint32_t;using u64=std::uint64_t;
using activate=u32(*)(u64,u64,u32,u64)noexcept;
using inspect=u32(*)(u64,u64,u32,u64)noexcept;
using submit=u32(*)(u64,u64,u32,u64,u64,u64)noexcept;
using abandon=u32(*)(u64,u64,u64)noexcept;
using respond=u32(*)(u64,u64,u64,u64,u32,u64,u64)noexcept;
using peek=u32(*)(u64,u64,u32,u32,u64,u64)noexcept;
using ack=u32(*)(u64,u64,u64,u64)noexcept;
using available=u32(*)(u64,u64,u64,u64,u32,u64)noexcept;
using retire=u32(*)(u64,u64)noexcept;
static_assert(std::is_same_v<decltype(&h::abi_version<uwvm2::imported::ros::version::ros2>),u32(*)()noexcept>);
static_assert(std::is_same_v<decltype(&h::rmw_client_activate),activate>);
static_assert(std::is_same_v<decltype(&h::rmw_service_activate),activate>);
static_assert(std::is_same_v<decltype(&h::rmw_service_inspect),inspect>);
static_assert(std::is_same_v<decltype(&h::rmw_client_submit),submit>);
static_assert(std::is_same_v<decltype(&h::rmw_client_abandon),abandon>);
static_assert(std::is_same_v<decltype(&h::rmw_service_respond),respond>);
static_assert(std::is_same_v<decltype(&h::rmw_service_peek),peek>);
static_assert(std::is_same_v<decltype(&h::rmw_service_commit),ack>);
static_assert(std::is_same_v<decltype(&h::rmw_service_cancel),ack>);
static_assert(std::is_same_v<decltype(&h::rmw_client_available),available>);
static_assert(std::is_same_v<decltype(&h::rmw_service_retire),retire>);
