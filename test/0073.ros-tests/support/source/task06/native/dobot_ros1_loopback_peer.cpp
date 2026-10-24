// SPDX-License-Identifier: Apache-2.0
// Genuine ROS1 generated service loopback peer; no controller/device connection.
// Build only against the pinned original rosdemo_v4 native package. The actual
// service traits/serialization become evidence, never an invented native MD5.
#include <ros/ros.h>
#include <ros/serialization.h>
#include <ros/service_traits.h>
#include <rosdemo_v4/GetErrorID.h>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <string>
#include <thread>
#include <vector>

namespace {
unsigned calls=0;
bool capture_ok=false;
std::string capture_path;
std::chrono::steady_clock::time_point replied;

bool respond(rosdemo_v4::GetErrorID::Request&,rosdemo_v4::GetErrorID::Response& response) {
    if(++calls!=1)return false; // Duplicate RPC is visible; never conceal replay.
    response.error_id={-7,16,8752,std::numeric_limits<std::int32_t>::max()};
    response.res=-23; // Retain the original signed field, not a success boolean.
    std::vector<std::uint8_t> bytes(ros::serialization::serializationLength(response));
    ros::serialization::OStream stream(bytes.data(),static_cast<std::uint32_t>(bytes.size()));
    ros::serialization::serialize(stream,response);
    // GetErrorID's complete native response is u32 sequence count + four i32s + i32 res.
    if(bytes.size()!=24)return false;
    std::ofstream output(capture_path,std::ios::binary|std::ios::trunc);
    if(!output)return false;
    output.write(reinterpret_cast<char const*>(bytes.data()),static_cast<std::streamsize>(bytes.size()));
    output.close();capture_ok=static_cast<bool>(output);
    replied=std::chrono::steady_clock::now();
    return capture_ok;
}
}

int main(int argc,char** argv) {
    ros::init(argc,argv,"task06_dobot_loopback_peer",ros::init_options::NoSigintHandler);
    if(argc!=3)return 22;
    char* end=nullptr;auto seconds=std::strtoul(argv[2],&end,10);
    if(!argv[2][0]||*end||seconds<1||seconds>60)return 22;
    capture_path=argv[1];
    auto const* type=ros::service_traits::DataType<rosdemo_v4::GetErrorID>::value();
    auto const* identity=ros::service_traits::MD5Sum<rosdemo_v4::GetErrorID>::value();
    if(std::string(type)!="rosdemo_v4/GetErrorID"||std::string(identity).size()!=32)return 22;
    std::printf("{\"native_type\":\"%s\",\"native_md5\":\"%s\",\"endpoint\":\"/task06_dobot_get_error_id\"}\n",type,identity);
    std::fflush(stdout);
    ros::NodeHandle node;
    auto server=node.advertiseService("/task06_dobot_get_error_id",respond);
    auto start=std::chrono::steady_clock::now();
    while(ros::ok()&&std::chrono::steady_clock::now()-start<std::chrono::seconds(seconds)) {
        ros::spinOnce();
        if(calls>1)break;
        // Allow the genuine native transport to flush its first response.
        if(calls==1&&std::chrono::steady_clock::now()-replied>=std::chrono::milliseconds(100))break;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    server.shutdown();ros::shutdown();
    std::printf("{\"calls\":%u,\"capture_ok\":%s,\"hardware\":false}\n",calls,capture_ok?"true":"false");
    return calls==1&&capture_ok?0:70;
}
