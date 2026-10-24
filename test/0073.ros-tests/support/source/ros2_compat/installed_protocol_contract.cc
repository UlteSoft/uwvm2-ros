/** Actual selected-SDK legacy protocol/source proof, without ROS entities.
 * Mutable metadata copies are refusal models; installed libraries stay intact.
 * This is an authored local contract, not an official upstream or robot test. */
#include "native_source.hpp"
#include "native_source_types.h"
#include "src/ros/gateway/ros2/custom_topics.h"
#include <cassert>

namespace source=::uwvm2::ros_gateway::ros2_compat::installed_source;
namespace compat=::uwvm2::ros_gateway::ros2_compat;
struct no_protocol_proof {};
static_assert(!source::interface_traits<no_protocol_proof>::enabled);
struct modern_like_support {void (*get_type_hash_func)(){};};

template<class Native> void check(std::string_view name)
{
    auto support=source::interface_traits<Native>::support();assert(support);
    assert(source::protocol_proof<Native>());
    auto identity=source::protocol_identity<Native>(support);assert(identity.size()==71);
    assert(source::qualify_protocol<Native>(support,name,identity));
    std::string wrong{identity};wrong.back()=wrong.back()=='0'?'1':'0';
    assert(!source::qualify_protocol<Native>(support,name,wrong));
    assert(!source::qualify_protocol<Native>(support,"wrong/srv/Name",identity));
    modern_like_support modern{};
    assert(source::protocol_identity<Native>(&modern).empty());
    assert(!source::qualify_protocol<Native>(&modern,name,identity));
    auto copied=*support;
    assert(source::protocol_identity<Native>(&copied).empty());
    auto schema=source::interface_traits<Native>::schema();
    for(std::size_t i{};i<schema.record_count;++i)
        assert(schema.records[i].type.find("_Event")==std::string_view::npos);
    if constexpr(source::interface_traits<Native>::kind==1)
    {
        auto metadata=source::service_metadata<Native>();assert(metadata);
        assert(source::service_members_match<Native>(metadata,name));
        auto changed=*metadata;
        changed.request_members_=metadata->response_members_;
        assert(!source::service_members_match<Native>(&changed,name));changed=*metadata;
        changed.response_members_=nullptr;
        assert(!source::service_members_match<Native>(&changed,name));changed=*metadata;
        changed.service_name_="Different";
        assert(!source::service_members_match<Native>(&changed,name));
    }
    else
    {
        assert(source::action_links<Native>(support));
#define UWVM_REJECT_CHANGED_LINK(Member) copied=*support;copied.Member=nullptr;assert(!source::action_links<Native>(&copied));
        UWVM_REJECT_CHANGED_LINK(goal_service_type_support)
        UWVM_REJECT_CHANGED_LINK(result_service_type_support)
        UWVM_REJECT_CHANGED_LINK(cancel_service_type_support)
        UWVM_REJECT_CHANGED_LINK(feedback_message_type_support)
        UWVM_REJECT_CHANGED_LINK(status_message_type_support)
#undef UWVM_REJECT_CHANGED_LINK
    }
    // The production root qualifier is usable only for explicitly selected
    // complete public interfaces. Proof-only standard/protocol types do not
    // acquire a public registry slot or complete-vendor Native trait.
    bool expected=::uwvm2::imported::ros::ipc::complete_vendor_native_traits<Native>::enabled;
    assert((::uwvm2::ros_gateway::ros2_custom_topics::native_interface_contract<Native,true>(support,name,identity)==expected));
}
int main()
{
#define UWVM_CHECK_INSTALLED_PROTOCOL(Native,Name) check<Native>(Name);
    UWVM_ROS_INSTALLED_SOURCE_INTERFACES(UWVM_CHECK_INSTALLED_PROTOCOL)
#undef UWVM_CHECK_INSTALLED_PROTOCOL
}
