// SPDX-License-Identifier: Apache-2.0
#pragma once
/** Fixed-original source port, not a ROS ABI or a type/permission authority.
 * Include only when the two exact original rclcpp headers are source-ported.
 * All rclcpp libraries and template consumers must be rebuilt with this profile.
 * Genuine C generated headers/functions are mandatory. No surrogate structs,
 * guessed layout, TypeSupport hash, allocator replacement or Host import exists.
 */
#if !defined(UWVM_RCLCPP_C_LIFECYCLE_PORT) || UWVM_RCLCPP_C_LIFECYCLE_PORT != 1
#error "Select the explicit source-recompiled C service lifecycle profile"
#endif
#include <memory>
#include <type_traits>
#include <type_description_interfaces/srv/detail/get_type_description__struct.h>
#include <type_description_interfaces/srv/detail/get_type_description__functions.h>
namespace uwvm_rmw { namespace c_lifecycle {
namespace detail {
template<class T> struct c_finalizer; // deliberately incomplete for every other C type
// Exact generated types, not a typename string or an introspection-language guess.
template<> struct c_finalizer<type_description_interfaces__srv__GetTypeDescription_Request> {
 static void finish(type_description_interfaces__srv__GetTypeDescription_Request* p) noexcept {
  type_description_interfaces__srv__GetTypeDescription_Request__fini(p);
 }
};
template<> struct c_finalizer<type_description_interfaces__srv__GetTypeDescription_Response> {
 static void finish(type_description_interfaces__srv__GetTypeDescription_Response* p) noexcept {
  type_description_interfaces__srv__GetTypeDescription_Response__fini(p);
 }
};
/** The real C value is an actual typed subobject at a stable allocation address.
 * Its enclosing standard shared owner, not its public pointer type, owns fini.
 * A cast to shared_ptr<void>, a callback capture or a weak lock retains exactly
 * that same control block. No custom deleter can disappear through type erasure.
 *
 * Do NOT eagerly __init/__create: RCL18d's type-description handler initializes
 * its Response itself. For Requests, the admitted RMW C decoder initializes a
 * private actual C temporary and swaps ownership into this zero-valid object.
 * Zero storage is valid ownership, not a fully initialized/default message.
 */
template<class T> struct zero_owner final {
 static_assert(std::is_standard_layout<T>::value && std::is_trivially_copyable<T>::value &&
               std::is_trivially_destructible<T>::value,
               "Actual generated C aggregate representation required");
 T value{};
 zero_owner() noexcept = default;
 zero_owner(zero_owner const&) = delete;
 zero_owner& operator=(zero_owner const&) = delete;
 zero_owner(zero_owner&&) = delete;
 zero_owner& operator=(zero_owner&&) = delete;
 ~zero_owner() noexcept {c_finalizer<T>::finish(std::addressof(value));}
};
} // namespace detail
/** Preserve the complete original generic C++ make_shared<T>() path.
 * The two proven C cases allocate only one stable shared owner and zero storage.
 * No C dynamic allocation happens until the real decoder/handler initializes it.
 * make_shared failure occurs before owned C fields exist. The C++17 aliasing
 * constructor is nonthrowing; the local owner stays live during construction.
 *
 * C allocator callbacks and real __fini obey their native nonthrowing contract.
 * Terminal traps/asynchronous termination still require actual instance drain.
 * Concurrent mutation of the same message remains the upstream caller's duty.
 */
template<class T> std::shared_ptr<T> make_service_object() {
 if constexpr (
   std::is_same<T,type_description_interfaces__srv__GetTypeDescription_Request>::value ||
   std::is_same<T,type_description_interfaces__srv__GetTypeDescription_Response>::value) {
  auto owner=std::make_shared<detail::zero_owner<T>>();
  return std::shared_ptr<T>(owner,std::addressof(owner->value));
 } else {
  return std::make_shared<T>();
 }
}
}} // namespace uwvm_rmw::c_lifecycle
