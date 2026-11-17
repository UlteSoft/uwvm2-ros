/** @file
 * @brief Test-only gateway owned by the scripted deployment simulator.
 * @details No code from this file is linked into the VM. The harness provisions
 * all descriptors and sends the exact stopped VM PID before activation. This
 * helper owns the native ROS sandbox lifetime; the VM only owns channel ends.
 */
#define main communication_startup_unused
#include "communication_worker_startup.cpp"
#undef main
#include "communication_external_fixture.h"
#include <cassert>
int main(int argc,char** argv)
{
    assert(argc>=19);int at=1;
    auto number=[&]{std::uint64_t value{};assert(at<argc&&decimal(argv[at++],value));return value;};
    int control=static_cast<int>(number()),startup=static_cast<int>(number()),identity=static_cast<int>(number());
    std::array<int,7> pipes;for(auto& fd:pipes)fd=static_cast<int>(number());
    auto image_bytes=number(),threads=number(),request=number(),response=number();
    x::manifest manifest;uwvm2::utils::container::std_vector<x::descriptor> ownership;
    auto launcher=argv[at++],hash=argv[at++];assert(add(launcher,hash,"/worker/launcher",x::image_role::runtime,manifest.launcher,ownership));
    auto count=number();assert(count<=256&&count*4==static_cast<unsigned>(argc-at));
    for(std::uint64_t i{};i<count;++i)
    {
        std::string_view role{argv[at++]};auto destination=argv[at++],path=argv[at++],sha=argv[at++];x::image_spec image;
        auto kind=role=="bootstrap"?x::image_role::bootstrap:role=="runtime"?x::image_role::runtime:x::image_role::configuration;
        assert(add(path,sha,destination,kind,image,ownership));
        if(image.destination.view()=="/worker/profile"){manifest.bound.profile_bytes=image.bytes;manifest.configuration.sha256=image.sha256;}
        manifest.files.emplace_back(std::move(image));
    }
    assert(manifest.configuration.identity.assign("startup")&&manifest.ros_prefix.assign("/opt/ros/jazzy"));
    auto profile_bytes=manifest.bound.profile_bytes;
    manifest.bound={image_bytes,2ull*1024*1024*1024,30,4096,manifest.files.size()+64,65536,0,request,response,65536,10000000000ull,1000000,1024u*1024u,profile_bytes,threads};
    std::uint64_t vm{};assert(::read(identity,&vm,sizeof(vm))==sizeof(vm)&&vm&&vm<=INT32_MAX);::close(identity);
    external_fixture::gateway(control,pipes,static_cast<pid_t>(vm),manifest,startup,::getppid());
}
