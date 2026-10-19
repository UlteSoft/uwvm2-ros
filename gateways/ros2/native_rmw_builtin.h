/** @file
 * @brief Complete native String/Bool/Twist/Image codecs for owned RMW endpoints.
 *
 * These bind original generated ROS C++ objects. Actual native TypeSupport
 * identity/schema qualification is separate from payload validation. All fields
 * of every supported original Message are encoded; there is no field projection.
 * Dynamic copies remain bounded by the source's actual max-message-bytes grant
 * in the owning endpoint. Integers reuse the existing canonical wire helpers.
 */
#pragma once
#include "native_rmw_publisher.h"
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/bool.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cmath>

namespace uwvm2::ros_gateway::native_rmw_builtin
{
    struct string_codec
    {
        using Native=std_msgs::msg::String;
        inline static constexpr std::string_view type{"std_msgs/msg/String"};
        inline static constexpr std::size_t minimum_bytes{4};
        static bool validate(std::uint8_t const* p,std::size_t n) noexcept
        {return p&&n>=4&&ipc::get(p,4)==n-4&&valid_utf8(p+4,n-4);}
        static bool decode(std::uint8_t const* p,std::size_t n,Native& value)
        {if(!validate(p,n)){return false;}value.data.assign(reinterpret_cast<char const*>(p+4),n-4);return true;}
        static bool valid(Native const& value) noexcept
        {return value.data.size()<=UINT32_MAX&&valid_utf8(reinterpret_cast<std::uint8_t const*>(value.data.data()),value.data.size());}
        static bool encoded_size(Native const& value,std::size_t& output) noexcept
        {if(!valid(value)||value.data.size()>SIZE_MAX-4){return false;}output=4+value.data.size();return true;}
        static bool encode(Native const& value,std::vector<std::uint8_t>& output)
        {
            std::size_t size{};if(!encoded_size(value,size)){return false;}
            std::vector<std::uint8_t> bytes;bytes.reserve(size);ipc::put(bytes,value.data.size(),4);
            bytes.insert(bytes.end(),value.data.begin(),value.data.end());output=std::move(bytes);return true;
        }
    };
    struct bool_codec
    {
        using Native=std_msgs::msg::Bool;
        inline static constexpr std::string_view type{"std_msgs/msg/Bool"};
        inline static constexpr std::size_t minimum_bytes{1};
        static bool validate(std::uint8_t const* p,std::size_t n) noexcept{return p&&n==1&&p[0]<=1;}
        static bool decode(std::uint8_t const* p,std::size_t n,Native& value)
        {if(!validate(p,n)){return false;}value.data=p[0]!=0;return true;}
        static bool valid(Native const&) noexcept{return true;}
        static bool encoded_size(Native const&,std::size_t& output) noexcept{output=1;return true;}
        static bool encode(Native const& value,std::vector<std::uint8_t>& output)
        {output={static_cast<std::uint8_t>(value.data?1:0)};return true;}
    };
    struct twist_codec
    {
        using Native=geometry_msgs::msg::Twist;
        inline static constexpr std::string_view type{"geometry_msgs/msg/Twist"};
        inline static constexpr std::size_t minimum_bytes{48};
        static bool validate(std::uint8_t const* p,std::size_t n) noexcept
        {if(!p||n!=48){return false;}for(unsigned i{};i<6;++i){auto bits{ipc::get(p+8*i,8)};double value;std::memcpy(&value,&bits,8);if(!std::isfinite(value)){return false;}}return true;}
        static bool decode(std::uint8_t const* p,std::size_t n,Native& value)
        {
            if(!validate(p,n)){return false;}
            double* fields[]{&value.linear.x,&value.linear.y,&value.linear.z,&value.angular.x,&value.angular.y,&value.angular.z};
            for(unsigned i{};i<6;++i){auto bits{ipc::get(p+8*i,8)};std::memcpy(fields[i],&bits,8);}return true;
        }
        static bool valid(Native const& v) noexcept
        {return std::isfinite(v.linear.x)&&std::isfinite(v.linear.y)&&std::isfinite(v.linear.z)&&std::isfinite(v.angular.x)&&std::isfinite(v.angular.y)&&std::isfinite(v.angular.z);}
        static bool encoded_size(Native const& value,std::size_t& output) noexcept
        {if(!valid(value)){return false;}output=48;return true;}
        static bool encode(Native const& value,std::vector<std::uint8_t>& output)
        {
            if(!valid(value)){return false;}std::vector<std::uint8_t> bytes;bytes.reserve(48);
            for(double field:{value.linear.x,value.linear.y,value.linear.z,value.angular.x,value.angular.y,value.angular.z})
            {std::uint64_t bits;std::memcpy(&bits,&field,8);ipc::put(bytes,bits,8);}output=std::move(bytes);return true;
        }
    };
    struct image_codec
    {
        using Native=sensor_msgs::msg::Image;
        inline static constexpr std::string_view type{"sensor_msgs/msg/Image"};
        inline static constexpr std::size_t minimum_bytes{44};
        static bool validate(std::uint8_t const* p,std::size_t n) noexcept
        {ipc::image_view view;return ipc::decode_image_view(p,n,2,view);}
        static bool decode(std::uint8_t const* p,std::size_t n,Native& value)
        {
            ipc::image_view view;if(!ipc::decode_image_view(p,n,2,view)){return false;}
            value.header.stamp.sec=static_cast<std::int32_t>(view.seconds);value.header.stamp.nanosec=view.nanoseconds;
            value.header.frame_id=view.frame_id;value.height=view.height;value.width=view.width;value.encoding=view.encoding;
            value.is_bigendian=view.bigendian?1:0;value.step=view.step;ros2_compat::assign_bytes(value.data,view.data,view.data+view.data_size);return true;
        }
        static ipc::image_view view(Native const& value)
        {return {0,static_cast<std::uint32_t>(value.header.stamp.sec),value.header.stamp.nanosec,value.height,value.width,value.step,value.header.frame_id,value.encoding,value.is_bigendian!=0,value.data.data(),value.data.size()};}
        static bool valid(Native const& value) noexcept
        {return ros2_compat::native_bytes_accessible(value.data)&&value.is_bigendian<=1&&ipc::valid_image_view(view(value),2);}
        static bool encoded_size(Native const& value,std::size_t& output) noexcept
        {
            if(!valid(value)||value.header.frame_id.size()>SIZE_MAX-37||value.encoding.size()>SIZE_MAX-37-value.header.frame_id.size()||
                value.data.size()>SIZE_MAX-37-value.header.frame_id.size()-value.encoding.size()){return false;}
            output=37+value.header.frame_id.size()+value.encoding.size()+value.data.size();return true;
        }
        static bool encode(Native const& value,std::vector<std::uint8_t>& output)
        {if(!valid(value)){return false;}auto bytes{ipc::encode_image_view(view(value))};if(bytes.empty()){return false;}output=std::move(bytes);return true;}
    };
    template<class Codec>struct contract
    {
        using Native=typename Codec::Native;
        static bool known(ros::capability_spec const& cap)
        {return cap.operation=="topic.publish"&&cap.type==Codec::type&&ros2_compat::builtin_message<Native>(cap.type,cap.type_identity);}
        static std::uint64_t minimum(ros::capability_spec const&) noexcept{return Codec::minimum_bytes;}
        static bool validate(ros::capability_spec const& cap,std::uint8_t const* p,std::size_t n)
        {
            if(!Codec::validate(p,n)){return false;}
            if constexpr(std::is_same<Codec,twist_codec>::value)
            {
                if(field(cap,"validator")=="twist-bounds")
                {
                    for(unsigned i{};i<6;++i){auto bits{ipc::get(p+8*i,8)};double value;std::memcpy(&value,&bits,8);
                        if(std::abs(value)>positive(field(cap,i<3?"twist.linear-max-abs":"twist.angular-max-abs"))){return false;}}
                }
            }
            return true;
        }
    };
    template<class Codec>using publisher=native_rmw_publisher::typed_endpoint<typename Codec::Native,Codec,contract<Codec>>;
}
