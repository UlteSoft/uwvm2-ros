/** @file @brief Allocation-free canonical identities for ROSv2 child modules. */
#pragma once
#include <cstdint>
#include <string_view>
#include <array>
extern "C++" {
namespace uwvm2::imported::ros::scene::platform_extensions
{
    inline constexpr std::string_view module_prefix="uwvm_ros_v2.platform_extension.";
    /** @brief Source-reviewed platform identifiers for permission-only foundation profiles.
     * @details These names follow the pinned SDK platform source inventory.
     * They contain no executable, protocol parser, vendor callback or discovery
     * mechanism. Revision v1 identifies this foundation policy, not a vendor
     * driver version. A platform name never proves vendor compatibility.
     */
    inline constexpr std::array<std::string_view,32> foundation_platforms{
        "abb","agibot","agilex","booster","boston_dynamics","clearpath","deep_robotics","dobot",
        "doosan","elephant","fanuc","flexiv","fourier","franka","hello_robot","kinova","kuka","leju",
        "limx","nvidia","nvidia-groot","nvidia-isaac-ros","nvidia-isaac-sim","pal","realman","robotis",
        "trossen","ubtech","ufactory","unitree","universal_robots","yaskawa"};
    inline bool foundation_module(std::string_view name)noexcept
    {
        if(!name.starts_with(module_prefix)||!name.ends_with(".v1"))return false;
        name.remove_prefix(module_prefix.size());name.remove_suffix(3);
        for(auto platform:foundation_platforms)if(name==platform)return true;
        return false;
    }
    /** @brief Parse one exact child ABI name without allocating or accepting aliases.
     * @details The platform is a lowercase SDK identifier, including '_' and
     * '-' where present. A positive decimal protocol major is allowed only at
     * the final module boundary. Wildcards, nested platform names, leading-zero
     * versions and trailing components are refused. No platform is enabled by
     * successfully parsing its name.
     */
    inline bool module_name(std::string_view name)noexcept
    {
        if(name.size()>255||!name.starts_with(module_prefix))return false;
        name.remove_prefix(module_prefix.size());auto at=name.find('.');
        if(at==name.npos||!at||name.front()<'a'||name.front()>'z')return false;
        for(char c:name.substr(0,at))if((c<'a'||c>'z')&&(c<'0'||c>'9')&&c!='_'&&c!='-')return false;
        name.remove_prefix(at+1);
        if(name.size()<2||name.front()!='v'||name[1]<'1'||name[1]>'9')return false;
        name.remove_prefix(1);std::uint32_t major{};
        for(char c:name)
        {if(c<'0'||c>'9'||major>(UINT32_MAX-static_cast<unsigned>(c-'0'))/10)return false;major=major*10+static_cast<unsigned>(c-'0');}
        return major!=0;
    }
}
}
