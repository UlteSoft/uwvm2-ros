/** @file
 * @brief Closed portable validators shared by scene configuration and ROS workers.
 * @details These validate complete canonical messages without ROS headers, native
 * pointers, dynamic type loading or allocation. Native codec classes inherit the
 * same validate function, so exact trusted validator identity remains meaningful.
 * Unknown schemas are refused; this fixed registry does not claim generic IDL.
 */
#pragma once
#include "wire.h"
#include <cmath>
#include <array>
namespace uwvm2::imported::ros::scene::communication::codecs
{
    inline bool text(wire::reader& in,std::string_view& out) noexcept
    {return in.text(out)&&out.find('\0')==out.npos;}
    inline bool stamp(wire::reader& in) noexcept
    {std::int64_t seconds{};std::uint64_t nanos{};return in.i64(seconds)&&seconds>=INT32_MIN&&seconds<=INT32_MAX&&in.u64(nanos)&&nanos<1000000000;}
    struct string
    {static bool validate(wire::bytes bytes) noexcept{wire::reader in{bytes};std::string_view value;return text(in,value)&&in.done();}};
    struct boolean
    {static bool validate(wire::bytes bytes) noexcept{return bytes.size()==1&&bytes[0]<=1;}};
    struct set_bool_request:boolean{};
    struct set_bool_response
    {static bool validate(wire::bytes bytes) noexcept{return !bytes.empty()&&bytes[0]<=1&&string::validate(bytes.subspan(1));}};
    struct lookup_goal
    {
        static bool validate(wire::bytes bytes) noexcept
        {wire::reader in{bytes};std::string_view a,b,c;bool advanced{};return text(in,a)&&text(in,b)&&stamp(in)&&stamp(in)&&stamp(in)&&text(in,c)&&in.boolean(advanced)&&in.done();}
    };
    struct lookup_feedback
    {static bool validate(wire::bytes bytes) noexcept{return bytes.empty();}};
    struct lookup_result
    {
        static bool validate(wire::bytes bytes) noexcept
        {
            wire::reader in{bytes};std::string_view frame,child,message;if(!stamp(in)||!text(in,frame)||!text(in,child))return false;
            for(unsigned i{};i<7;++i){double value{};if(!in.f64(value)||!std::isfinite(value))return false;}
            std::uint64_t error{};return in.u64(error)&&error<=UINT8_MAX&&text(in,message)&&in.done();
        }
    };
    struct descriptor
    {std::string_view codec,identity,encoding,schema;bool(*validate)(wire::bytes) noexcept;};
    inline constexpr std::array descriptors{
        descriptor{"std-string","std_msgs/msg/String","scene-canonical","text<utf8,no-nul>",string::validate},
        descriptor{"std-bool","std_msgs/msg/Bool","scene-canonical","u8<bool>",boolean::validate},
        descriptor{"set-bool-request","std_srvs/srv/SetBool_Request","scene-canonical","u8<bool>",set_bool_request::validate},
        descriptor{"set-bool-response","std_srvs/srv/SetBool_Response","scene-canonical","u8<bool>,text<utf8,no-nul>",set_bool_response::validate},
        descriptor{"lookup-transform-goal","tf2_msgs/action/LookupTransform_Goal","scene-canonical","target:text,source:text,source_time:(i64<i32>,u64<ns>),timeout:(i64<i32>,u64<ns>),target_time:(i64<i32>,u64<ns>),fixed:text,advanced:u64<bool>",lookup_goal::validate},
        descriptor{"lookup-transform-feedback","tf2_msgs/action/LookupTransform_Feedback","scene-canonical","empty",lookup_feedback::validate},
        descriptor{"lookup-transform-result","tf2_msgs/action/LookupTransform_Result","scene-canonical","stamp:(i64<i32>,u64<ns>),frame:text,child:text,translation:3*f64<finite>,rotation:4*f64<finite>,error:u64<u8>,message:text",lookup_result::validate}};
    inline descriptor const* find(std::string_view name) noexcept
    {for(auto const& value:descriptors)if(value.codec==name)return &value;return nullptr;}
}
