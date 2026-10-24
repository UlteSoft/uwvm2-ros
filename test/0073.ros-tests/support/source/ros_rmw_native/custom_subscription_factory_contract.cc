/** Actual original-SDK source/link contract, authored and not executed.
 * Checks are unconditional, including optimized builds. A null real Node is
 * intentionally invalid; it is not a fake native entity or an API substitute.
 * This checks macro/ordinal/link and refusal preservation only, never delivery.
 */
#include "../../../../../src/ros/gateway/ros2/native_rmw_subscription_factory.h"
#include <cstdlib>
#include <cstdio>
#include <type_traits>
namespace gw=uwvm2::ros_gateway;
namespace topics=gw::ros2_custom_topics;
namespace wire=uwvm2::imported::ros::ipc::rmw_endpoint;
static void require(bool value){if(!value){std::abort();}}
using registry_factory=std::uint32_t(*)(rclcpp::Node::SharedPtr const&,
    uwvm2::imported::ros::capability_spec const&,wire::binding const&,std::uint64_t,
    std::shared_ptr<gw::native_rmw_subscription::endpoint>&,wire::packet&);
static_assert(std::is_convertible<decltype(&gw::native_rmw_subscription::create_selected),registry_factory>::value,
    "selector must match the real owning registry factory, including source/receipt ownership");
int main()
{
    std::size_t ordinal{};
#define UWVM_CUSTOM_OWNED_SUBSCRIPTION_EXPECTED_ROW(Native,Name,NativeId,CodecId,Bytes,Codec) \
    {auto const& row{topics::detail::owned_publisher_rows[ordinal++]}; \
     require(row.name==Name&&row.native_identity==NativeId&&row.codec_identity==CodecId&&row.wire_bytes==Bytes);}
    UWVM_ROS_CUSTOM_ROS2_TYPES(UWVM_CUSTOM_OWNED_SUBSCRIPTION_EXPECTED_ROW)
#undef UWVM_CUSTOM_OWNED_SUBSCRIPTION_EXPECTED_ROW
    require(ordinal==topics::detail::topic_count);
    std::size_t visited{};
    for(std::size_t shard{};shard!=topics::detail::topic_shards;++shard)
        for(std::size_t i{shard};i<topics::detail::topic_count;i+=topics::detail::topic_shards)
        {require(i%topics::detail::topic_shards==shard);++visited;}
    require(visited==topics::detail::topic_count);
    rclcpp::Node::SharedPtr node;
    uwvm2::imported::ros::capability_spec cap;cap.operation="topic.subscribe";
    wire::binding owner{2,UINT64_C(0xf1234567a1234567),17,23,31};
    std::shared_ptr<gw::native_rmw_subscription::endpoint> endpoint;
    wire::packet receipt{};receipt.incarnation=79;
    // Volatile function pointers deliberately retain the real dispatcher/shard
    // references even though this invalid-argument contract uses a null Node.
    // Actual omitted-shard linking must fail, not optimize away the symbol.
    auto volatile custom=&topics::create_owned_rmw_subscription;
    auto volatile selected=&gw::native_rmw_subscription::create_selected;
    require(custom(node,cap,owner,41,endpoint,receipt)==22&&!endpoint&&receipt.incarnation==79);
    require(selected(node,cap,owner,41,endpoint,receipt)==22&&!endpoint&&receipt.incarnation==79);
    for(std::size_t i{};i<topics::detail::topic_count;++i)
    {
        cap.type=topics::detail::owned_publisher_rows[i].name;
        require(custom(node,cap,owner,41,endpoint,receipt)==22&&!endpoint&&receipt.incarnation==79);
        require(selected(node,cap,owner,41,endpoint,receipt)==22&&!endpoint&&receipt.incarnation==79);
    }
    for(std::string_view type:{gw::native_rmw_builtin::string_codec::type,gw::native_rmw_builtin::bool_codec::type,
        gw::native_rmw_builtin::twist_codec::type,gw::native_rmw_builtin::image_codec::type})
    {
        cap.type=type;
        require(selected(node,cap,owner,41,endpoint,receipt)==22&&!endpoint&&receipt.incarnation==79);
    }
    std::fputs("custom-owned-subscription-source-contract: PASS_METADATA_INVALID_ARGUMENTS_AND_LINK_ONLY\n",stdout);
    return 0;
}
