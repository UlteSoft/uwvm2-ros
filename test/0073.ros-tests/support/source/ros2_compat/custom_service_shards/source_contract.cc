/* SPDX-License-Identifier: Apache-2.0 */
/** Actual original-SDK metadata/link contract SOURCE, not executed by author.
 * The caller supplies a complete real generated provider/manifest and actual
 * selected-SDK features. No Node, client, server, ROS context, endpoint, fake
 * TypeSupport, fixture identity, native service response or Guest is created.
 * Volatile addresses retain every business method and all shard link tables.
 * This source cannot establish successful native/Guest service behavior.
 */
#ifdef NDEBUG
#error "The source contract must retain its checks (-UNDEBUG)"
#endif
#include "custom_services.h"
#include "src/ros/gateway/common/native_io.h"
#include <type_traits>
namespace service=uwvm2::ros_gateway::ros2_custom_services;
using namespace uwvm2::ros_gateway;
static unsigned checks{},failures{};
static void require(bool ok,unsigned line)
{
    ++checks;if(ok){return;}++failures;native_io::fixed_text<96> message;
    if(message.append("FAIL_LEGACY_SERVICE_SHARDS line=")&&message.number(line)&&message.append("\n"))
    {(void)native_io::write_audit_text(message.view());}
}
#define REQUIRE(x) require(bool(x),unsigned(__LINE__))
int main()
{
    using call=ipc::call_result(service::adapter::*)(ros::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t);
    using advertise=ipc::call_result(service::adapter::*)(ros::capability_spec const&,std::shared_ptr<event_queue>);
    using close=void(service::adapter::*)(std::string const&);
    bool(*volatile qualify)(ros::capability_spec const&)=&service::known;
    call volatile retained_call=&service::adapter::call;
    advertise volatile retained_advertise=&service::adapter::advertise;
    call volatile retained_respond=&service::adapter::respond;
    close volatile retained_close=&service::adapter::close;
    (void)qualify;(void)retained_call;(void)retained_advertise;(void)retained_respond;(void)retained_close;
    static_assert(!std::is_copy_constructible<service::adapter>::value,"original owner remains noncopyable");
    static_assert(!std::is_copy_assignable<service::adapter>::value,"original owner remains nonassignable");
    ros::capability_spec missing{};missing.operation="service.call";
    missing.type="uwvm_unselected/srv/DefinitelyMissing";REQUIRE(!service::known(missing));
    std::size_t actual_rows{};
#define UWVM_LEGACY_SERVICE_METADATA(Native,Type,Identity,Canonical,Request,Response) \
    {ros::capability_spec cap{};cap.operation="service.call";cap.type=Type;cap.type_identity=Identity; \
     cap.limits.emplace("adapter-codec-identity",Canonical);++actual_rows; \
     REQUIRE(service::known(cap));cap.operation="service.advertise";REQUIRE(service::known(cap)); \
     cap.operation="service.respond";REQUIRE(service::known(cap)); \
     cap.operation="service.available";REQUIRE(!service::known(cap));cap.operation="service.call"; \
     cap.type_identity="not-the-selected-native-identity";REQUIRE(!service::known(cap)); \
     cap.type_identity=Identity;cap.limits["adapter-codec-identity"]="not-the-selected-codec";REQUIRE(!service::known(cap));}
    UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(UWVM_LEGACY_SERVICE_METADATA)
#undef UWVM_LEGACY_SERVICE_METADATA
    REQUIRE(actual_rows==service::detail::service_count);
    // An explicit duplicate provider is a negative metadata run: the original
    // known() refuses duplicate names even if the last exact row qualifies.
    // This positive contract requires a unique complete registered catalog.
    native_io::fixed_text<128> summary;
    if(summary.append("LEGACY_SERVICE_METADATA_LINK_ONLY checks=")&&summary.number(checks)&&
       summary.append(" failures=")&&summary.number(failures)&&summary.append("\n"))
    {(void)native_io::write_audit_text(summary.view());}
    return failures?1:0;
}
