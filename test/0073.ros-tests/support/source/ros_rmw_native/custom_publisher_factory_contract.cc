/** Selected original-SDK SOURCE contract, not executed native delivery.
 * No substitute ROS structs or fake Node is constructed. Every provider row is
 * checked as untyped DATA; actual typed factories are supplied only by the same
 * complete generated shard object set used by the Gateway target.
 */
#include "../../../../../src/ros/gateway/ros2/custom_topics.h"
#include <cstdlib>
#include <cstdio>
namespace topics=uwvm2::ros_gateway::ros2_custom_topics;
namespace wire=uwvm2::imported::ros::ipc::rmw_endpoint;
static void require(bool value){if(!value){std::abort();}}
int main()
{
    std::size_t ordinal{};
#define UWVM_CUSTOM_OWNED_PUBLISHER_EXPECTED_ROW(Native,Name,NativeId,CodecId,Bytes,Codec) \
    {auto const& row{topics::detail::owned_publisher_rows[ordinal++]}; \
     require(row.name==Name&&row.native_identity==NativeId&&row.codec_identity==CodecId&&row.wire_bytes==Bytes);}
    UWVM_ROS_CUSTOM_ROS2_TYPES(UWVM_CUSTOM_OWNED_PUBLISHER_EXPECTED_ROW)
#undef UWVM_CUSTOM_OWNED_PUBLISHER_EXPECTED_ROW
    require(ordinal==topics::detail::topic_count);
    std::size_t visited{};
    for(std::size_t shard{};shard!=topics::detail::topic_shards;++shard)
        for(std::size_t i{shard};i<topics::detail::topic_count;i+=topics::detail::topic_shards)
        {require(i%topics::detail::topic_shards==shard);++visited;}
    require(visited==topics::detail::topic_count);
    // Actual invalid arguments must precede native type qualification/effects.
    // A NULL actual owner is invalid, never a pretend native test endpoint.
    rclcpp::Node::SharedPtr node;
    uwvm2::imported::ros::capability_spec cap;cap.operation="topic.publish";
    wire::binding owner{2,UINT64_C(0xf1234567a1234567),17,23,31};
    std::shared_ptr<uwvm2::ros_gateway::native_rmw_publisher::endpoint> endpoint;
    wire::packet receipt{};receipt.incarnation=79;
    auto volatile factory=&topics::create_owned_rmw_publisher;
    require(factory(node,cap,owner,41,endpoint,receipt)==22&&!endpoint&&receipt.incarnation==79);
    for(std::size_t i{};i<topics::detail::topic_count;++i)
    {
        cap.type=topics::detail::owned_publisher_rows[i].name;
        require(factory(node,cap,owner,41,endpoint,receipt)==22&&!endpoint&&receipt.incarnation==79);
    }
    std::fputs("custom-owned-publisher-source-contract: PASS_METADATA_AND_INVALID_ARGUMENTS_ONLY\n",stdout);
    return 0;
}
