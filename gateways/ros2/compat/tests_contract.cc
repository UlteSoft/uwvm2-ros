/* Native SDK compatibility contracts. This program creates no ROS context,
 * Node, publisher, service, executor, clock or background thread. It exercises
 * the adapters against explicit public metadata and small fake API shapes.
 * Build with the selected SDK and -UNDEBUG; running it is a model verdict, not
 * native ROS delivery, permission enforcement or a distro acceptance verdict. */
#include "api.h"
#include "identity.h"
#include "parameters.h"
#include "../qos.h"
#include <std_msgs/msg/string.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <future>
#include <cstdlib>
#include <vector>
#ifdef NDEBUG
#error Compatibility contracts require enabled checks
#endif
namespace compat=::uwvm2::ros_gateway::ros2_compat;
namespace qos=::uwvm2::ros_gateway::ros2_qos;
namespace
{
    void require(bool value) { if(!value) { std::abort(); } }
    struct absent_hash {};
    struct native_hash { unsigned char version{};unsigned char value[32]{}; };
    struct present_hash
    {
        native_hash const* (*get_type_hash_func)(present_hash const*);
    };
    native_hash invalid{};
    native_hash const* invalid_hash(present_hash const*) { return &invalid; }
    struct goal_handle { using SharedPtr=std::shared_ptr<int>; };
    struct resize_only
    {
        std::vector<unsigned char> storage;
        void resize(std::size_t n) { storage.resize(n); }
        unsigned char* data() { return storage.data(); }
    };
    struct no_actual_qos {};
    struct exact_actual_qos
    {
        compat::qos_type value;
        compat::qos_type get_actual_qos() const { return value; }
        compat::qos_type get_request_publisher_actual_qos() const { return value; }
        compat::qos_type get_response_subscription_actual_qos() const { return value; }
        compat::qos_type get_request_subscription_actual_qos() const { return value; }
        compat::qos_type get_response_publisher_actual_qos() const { return value; }
    };
    struct no_descriptor {};
    struct typed_descriptor { bool dynamic_typing{}; };
    struct old_parameters {};
    struct post_parameters
    {
        std::shared_ptr<int> owned;unsigned removed{};
        template<class Callback> std::shared_ptr<int> add_post_set_parameters_callback(Callback)
        { owned=std::make_shared<int>(1);return owned; }
        void remove_post_set_parameters_callback(int* value)
        { require(value==owned.get());++removed;owned.reset(); }
    };
    // This intentionally exposes only complete rmw-profile overloads. A modern
    // QoS object must be explicitly converted, preserving every selected field.
    struct profile_api
    {
        rmw_qos_profile_t seen{};
        template<class> int create_publisher(std::string const&,rmw_qos_profile_t const& p) { seen=p;return 1; }
        template<class> int create_client(std::string const&,rmw_qos_profile_t const& p) { seen=p;return 2; }
        template<class,class Callback> int create_service(std::string const&,Callback&&,rmw_qos_profile_t const& p) { seen=p;return 3; }
        template<class,class Callback> int create_subscription(std::string const&,Callback&&,rmw_qos_profile_t const& p) { seen=p;return 4; }
    };
}
int main()
{
    require(!compat::has_baked_hash<absent_hash>::value);
    require(compat::has_baked_hash<present_hash>::value);
    present_hash invalid_support{invalid_hash};require(compat::baked_hash(&invalid_support).empty());
    require(compat::baked_hash(static_cast<present_hash const*>(nullptr)).empty());
    auto selected=compat::make_qos(rmw_qos_profile_default);
    require(!qos::entity(no_actual_qos{},selected));require(!qos::client(no_actual_qos{},selected));require(!qos::service(no_actual_qos{},selected));
    require(qos::entity(exact_actual_qos{selected},selected));require(qos::client(exact_actual_qos{selected},selected));require(qos::service(exact_actual_qos{selected},selected));
    auto profile_node=std::make_shared<profile_api>();
    require(compat::create_publisher<int>(profile_node,"/selected",selected)==1);
    require(compat::create_client<int>(profile_node,"/selected",selected)==2);
    require(compat::create_service<int>(profile_node,"/selected",[] {},selected)==3);
    require(compat::create_subscription<int>(profile_node,"/selected",selected,[] {})==4);
    require(profile_node->seen.depth==compat::profile(selected).depth);
    auto pointer=std::make_shared<int>(7);require(compat::goal_response<goal_handle>(pointer)==pointer);
    std::promise<std::shared_ptr<int>> promise;auto ready=promise.get_future().share();promise.set_value(pointer);
    require(compat::goal_response<goal_handle>(ready)==pointer);
    unsigned char bytes[]{1,2,3};resize_only output;compat::assign_bytes(output,bytes,bytes+3);
    require(output.storage==std::vector<unsigned char>({1,2,3}));require(compat::native_bytes_accessible(output.storage));
    require(compat::is_dynamic_typing(no_descriptor{}));typed_descriptor descriptor;
    require(!compat::is_dynamic_typing(descriptor));compat::dynamic_typing(descriptor);require(descriptor.dynamic_typing);
    require(!compat::post_set(std::make_shared<old_parameters>(),[] {}));
    auto callbacks=std::make_shared<post_parameters>();
    { auto first=compat::post_set(callbacks,[] {});auto second=std::move(first);require(!first&&bool(second)); }
    require(callbacks->removed==1&&!callbacks->owned);
    auto string_id=compat::builtin_catalog_identity("std_msgs/msg/String");
    require(!string_id.empty()&&!compat::catalog_identity("std_msgs/msg/String","RIHS01_wrong"));
    require(!compat::catalog_identity("foreign/msg/String",string_id));
    require(compat::builtin_message<std_msgs::msg::String>("std_msgs/msg/String",string_id));
#if UWVM_ROS2_HAVE_INTROSPECTION_CPP
    auto members=compat::message_members_for<std_msgs::msg::String>();require(members!=nullptr);
    std::array<compat::builtin::record,32> selected_records{};
    auto schema=compat::builtin::schema("std_msgs/msg/String",selected_records);
    require(compat::matches_schema(members,schema,"std_msgs/msg/String"));
    auto changed=*members;changed.member_count_=0;
    require(!compat::matches_schema(&changed,schema,"std_msgs/msg/String"));
    auto field=members->members_[0];field.string_upper_bound_=1;changed=*members;changed.members_=&field;
    require(!compat::matches_schema(&changed,schema,"std_msgs/msg/String"));
    field=members->members_[0];field.is_array_=true;changed.members_=&field;
    require(!compat::matches_schema(&changed,schema,"std_msgs/msg/String"));
    require(!compat::matches_schema(members,schema,"foreign/msg/String"));
#endif
#if UWVM_ROS2_HAVE_OPENSSL
    require(compat::builtin_rihs("std_msgs/msg/String",string_id));
    require(!compat::builtin_rihs("std_msgs/msg/String",compat::builtin_catalog_identity("std_msgs/msg/Bool")));
    require(compat::builtin_rihs("std_srvs/srv/Trigger",compat::builtin_catalog_identity("std_srvs/srv/Trigger")));
    require(compat::builtin_rihs("std_srvs/srv/SetBool",compat::builtin_catalog_identity("std_srvs/srv/SetBool")));
    require(compat::builtin_rihs("action_tutorials_interfaces/action/Fibonacci",compat::builtin_catalog_identity("action_tutorials_interfaces/action/Fibonacci")));
#endif
    constexpr std::string_view marker{"PASS_ROS2_COMPAT\n"};
    return ::uwvm2::ros_gateway::native_io::write_all(1,marker)?0:1;
}
