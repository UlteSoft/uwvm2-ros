/** @file @brief Test-only sealed image manifest launcher; readiness is not ROS data-path qualification. */
#include "support/legacy_process/communication_worker_sandbox.h"
#include <cstdio>
namespace s=uwvm2::imported::ros::scene;
namespace x=s::communication_sandbox;
namespace w=s::communication_worker;
using x::error;
static bool decimal(char const* input,std::uint64_t& output)
{std::string_view text{input};auto done=std::from_chars(text.data(),text.data()+text.size(),output);return done.ec==std::errc{}&&done.ptr==text.data()+text.size();}
static bool digest(char const* input,s::buffers::digest& output)
{
    std::string_view text{input};if(text.size()!=64)return false;
    for(std::size_t i{};i<32;++i){unsigned n{};auto begin=text.data()+i*2;auto done=std::from_chars(begin,begin+2,n,16);if(done.ec!=std::errc{}||done.ptr!=begin+2)return false;output[i]=static_cast<std::uint8_t>(n);}return true;
}
static bool add(char const* path,char const* hash,std::string_view destination,x::image_role role,x::image_spec& result,uwvm2::utils::container::std_vector<x::descriptor>& ownership)
{
    x::descriptor fd{::open(path,O_RDONLY|O_NOFOLLOW|O_CLOEXEC)};struct stat status{};
    if(!fd||::fstat(fd.get(),&status)||!S_ISREG(status.st_mode)||status.st_size<0||!digest(hash,result.sha256)||!result.destination.assign(destination))return false;
    result.role=role;result.source=fd.get();result.bytes=static_cast<std::uint64_t>(status.st_size);ownership.emplace_back(std::move(fd));return true;
}
static int execute(int argc,char** argv)
{
    if(argc<11||(argc-7)%4)return 10;
    x::manifest manifest;uwvm2::utils::container::std_vector<x::descriptor> ownership;std::uint64_t image_bytes{},threads{},request{},response{};
    if(!decimal(argv[3],image_bytes)||!decimal(argv[4],threads)||!decimal(argv[5],request)||!decimal(argv[6],response)||!add(argv[1],argv[2],"/worker/launcher",x::image_role::runtime,manifest.launcher,ownership))return 11;
    for(int i=7;i<argc;i+=4)
    {
        std::string_view role{argv[i]};auto kind=role=="bootstrap"?x::image_role::bootstrap:role=="runtime"?x::image_role::runtime:role=="configuration"?x::image_role::configuration:x::image_role::unconfigured;
        if(kind==x::image_role::unconfigured)return 12;
        x::image_spec image;
        if(!add(argv[i+2],argv[i+3],argv[i+1],kind,image,ownership))return 13;
        if(image.destination.view()=="/worker/profile"){manifest.bound.profile_bytes=image.bytes;manifest.configuration.sha256=image.sha256;}
        manifest.files.emplace_back(std::move(image));
    }
    if(!manifest.configuration.identity.assign("single-node-startup-qualification")||!select_image_prefix(manifest, true))return 14;
    auto profile_bytes=manifest.bound.profile_bytes;
    manifest.bound={image_bytes,2ull*1024*1024*1024,15,4096,manifest.files.size()+64,65536,0,request,response,65536,10000000000ull,1000000,1024u*1024u,profile_bytes,threads};
    x::process child;auto status=child.prepare(manifest);if(status!=error::success){std::fprintf(stderr,"prepare=%u\n",unsigned(status));return 15;}
    status=child.launch();if(status!=error::success){std::fprintf(stderr,"launch=%u\n",unsigned(status));return 16;}
    std::size_t stderr_bytes{};
    auto drain=[&]
    {
        std::array<char,4096> bytes{};auto n=::read(child.stderr_fd(),bytes.data(),bytes.size());
        if(n>0){stderr_bytes+=static_cast<std::size_t>(n);if(::write(2,bytes.data(),n)!=n)return false;}
        return stderr_bytes<=65536;
    };
    auto wait=[&](unsigned phase)
    {
        auto until=uwvm2::imported::ros::resources::steady_now()+11000000000ll;auto result=error::again;
        while(result==error::again&&uwvm2::imported::ros::resources::steady_now()<until)
        {result=phase==0?child.identify():child.readiness(phase==2);if(!drain())return error::too_large;if(result==error::again)::poll(nullptr,0,1);}
        std::fprintf(stderr,"phase=%u result=%u\n",phase,unsigned(result));return result;
    };
    status=wait(0);if(status==error::success)status=wait(1);
    std::array<x::descriptor,4> workers,brokers;std::array<w::role_fd,4> pipes{};
    if(status==error::success)
    {
        for(std::size_t i{};i<4;++i)
        {int pair[2];if(::pipe2(pair,O_CLOEXEC|O_NONBLOCK))return 17;bool rx=w::receiving(w::roles[i]);workers[i].reset(pair[rx?0:1]);brokers[i].reset(pair[rx?1:0]);pipes[i]={w::roles[i],workers[i].get()};}
        status=child.activate(pipes);if(status==error::success)status=wait(2);
    }
    child.request_stop();int exited{};auto until=uwvm2::imported::ros::resources::steady_now()+5000000000ll;auto cleanup=error::again;
    while(cleanup==error::again&&uwvm2::imported::ros::resources::steady_now()<until){cleanup=child.reap(exited);if(cleanup==error::again)::poll(nullptr,0,1);}
    drain();std::fprintf(stderr,"cleanup=%u child_status=%d stderr_bytes=%zu\n",unsigned(cleanup),exited,stderr_bytes);
    if(status!=error::success||cleanup!=error::success||!child.namespace_identity_confirmed())return 18;
    std::puts("PASS approved single-node sealed worker startup and attachment; startup_only=true; no ROS data-path claim");return 0;
}
/** @brief Select the external peer's ament prefix from its sealed ROS images.
 * @details The caller supplies the hash and destination of every image. A mixed
 * installation cannot establish the peer's identity. No host environment or
 * process-global default selects a distribution during activation.
 */
[[maybe_unused]] static bool select_image_prefix(x::manifest& manifest, bool standalone_fixture = false)
{
    constexpr std::string_view root{"/opt/ros/"};
    std::string_view prefix{};
    for(auto const& image : manifest.files)
    {
        auto const path = image.destination.view();
        if(!path.starts_with(root)) continue;
        auto const end = path.find('/', root.size());
        if(end == std::string_view::npos || end == root.size()) return false;
        auto const candidate = path.substr(0, end);
        if(!prefix.empty() && prefix != candidate) return false;
        prefix = candidate;
    }
    if(prefix.empty() && standalone_fixture) prefix = "/opt/ros/fixture";
    return !prefix.empty() && manifest.ros_prefix.assign(prefix);
}
int main(int argc,char** argv)
{
    ::signal(SIGPIPE,SIG_IGN);
    try{return execute(argc,argv);}catch(std::bad_alloc const&){std::fputs("recoverable allocation failure\n",stderr);return 19;}catch(...){return 20;}
}
