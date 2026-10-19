/*************************************************************
 * UWVM2 explicit complete vendor source domain. APL-2.0.     *
 *************************************************************/
#pragma once
#include <initializer_list>
#include <string_view>
#include <type_traits>

namespace uwvm2::imported::ros::ipc
{
    /** These traits are specialized only by a trusted generated native header.
     * They select a schema verification implementation, never a capability.
     * The ordinary custom/standard/control providers retain their own checks.
     * A missing proof is refused; Guest requests cannot enable this trait. */
    template<class Native> struct complete_vendor_native_traits
    {
        static constexpr bool enabled=false;
        static bool constants() noexcept { return false; }
    };
    template<class Codec,class=void> struct complete_vendor_codec : ::std::false_type {};
    template<class Codec> struct complete_vendor_codec<Codec,
        ::std::void_t<decltype(Codec::complete_vendor_profile)>>
        : ::std::bool_constant<Codec::complete_vendor_profile> {};

    /** Full native declaration names, including generated protocol components.
     * A name is only a compiled schema key. It cannot make a dependency an
     * endpoint or replace native identities, full metadata, or policy grants. */
    inline constexpr bool complete_vendor_native_name(unsigned version,
        ::std::string_view name,::std::string_view category={}) noexcept
    {
        if((version!=1&&version!=2)||name.empty()||name.size()>255)return false;
        auto slash=name.find('/');
        if(slash==::std::string_view::npos||!slash||slash>63)return false;
        auto package=name.substr(0,slash),tail=name.substr(slash+1);
        if(package=="builtin"||package.front()<'a'||package.front()>'z')return false;
        for(char c:package)
            if(!((c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='_'))return false;
        if(version==2)
        {
            auto next=tail.find('/');
            if(next==::std::string_view::npos)return false;
            auto actual=tail.substr(0,next);
            if((actual!="msg"&&actual!="srv"&&actual!="action")||
               (!category.empty()&&actual!=category))return false;
            tail.remove_prefix(next+1);
        }
        if(tail.empty()||tail.front()<'A'||tail.front()>'Z')return false;
        for(char c:tail)
            if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||
                 (c>='0'&&c<='9')||c=='_'))return false;
        return true;
    }

    /** Preserve all existing builtin endpoint domains. Complete vendor tables
     * cannot shadow them or grant management through a similarly named type.
     * Standard dependencies remain admissible as full schema records. */
    inline constexpr bool complete_vendor_endpoint_name(unsigned version,
        ::std::string_view name,::std::string_view category) noexcept
    {
        if(!complete_vendor_native_name(version,name,category))return false;
        auto package=name.substr(0,name.find('/'));
        if(package=="rcl_interfaces"||package=="lifecycle_msgs"||
           package=="type_description_interfaces")return false;
        for(auto reserved:{::std::string_view{"std_msgs/String"},::std::string_view{"std_msgs/Bool"},
            ::std::string_view{"std_msgs/Header"},::std::string_view{"geometry_msgs/TransformStamped"},
            ::std::string_view{"sensor_msgs/Image"},::std::string_view{"geometry_msgs/Twist"},
            ::std::string_view{"std_srvs/Trigger"},::std::string_view{"std_srvs/SetBool"},
            ::std::string_view{"actionlib_tutorials/Fibonacci"},
            ::std::string_view{"std_msgs/msg/String"},::std::string_view{"std_msgs/msg/Bool"},
            ::std::string_view{"std_msgs/msg/Header"},::std::string_view{"builtin_interfaces/msg/Time"},
            ::std::string_view{"builtin_interfaces/msg/Duration"},
            ::std::string_view{"geometry_msgs/msg/TransformStamped"},
            ::std::string_view{"sensor_msgs/msg/Image"},::std::string_view{"geometry_msgs/msg/Twist"},
            ::std::string_view{"std_srvs/srv/Empty"},::std::string_view{"example_interfaces/srv/AddTwoInts"},
            ::std::string_view{"std_srvs/srv/Trigger"},::std::string_view{"std_srvs/srv/SetBool"},
            ::std::string_view{"action_tutorials_interfaces/action/Fibonacci"}})
            if(name==reserved)return false;
        return true;
    }
}
