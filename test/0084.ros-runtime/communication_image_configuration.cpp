/** @file @brief Inert isolated-worker configuration and exact regular-image pinning. */
#include "support/legacy_process/communication_worker_manifest.h"
#include <uwvm2/imported/ros/scene/authority.h>
#include <cassert>
#include <cstdio>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace worker=s::communication_worker;
namespace sandbox=s::communication_sandbox;
using text=uwvm2::utils::container::std_string;
struct directory
{
    text path;
    directory()
    {
        std::array<char,4096> binary{};auto size=::readlink("/proc/self/exe",binary.data(),binary.size()-1);assert(size>0);
        text name(binary.data(),static_cast<std::size_t>(size));auto slash=name.rfind('/');assert(slash!=name.npos);
        name.resize(slash);name+="/image-configuration-XXXXXX";assert(::mkdtemp(name.data()));path=std::move(name);
    }
    ~directory()
    {for(auto name:{"launcher","bootstrap","backend","profile","link","fifo","replacement"})::unlink((path+'/'+name).c_str());assert(::rmdir(path.c_str())==0);}
    void file(std::string_view name,std::uint8_t value=7)
    {
        auto route=path+'/'+text(name);sandbox::descriptor fd{::open(route.c_str(),O_WRONLY|O_CREAT|O_TRUNC|O_CLOEXEC,0600)};assert(fd);
        std::array<std::uint8_t,64> contents;contents.fill(value);contents[0]=0x7f;contents[1]='E';contents[2]='L';contents[3]='F';
        assert(::write(fd.get(),contents.data(),contents.size())==static_cast<ssize_t>(contents.size()));
    }
};
static sandbox::limits bounds()
{return {4096,64u*1024u*1024u,5,4096,64,4096,0,1024,1024,0,1000000000,1000000,512u*1024u,64,2};}
static worker::image_configuration setting(directory const& dir,std::string_view bootstrap="bootstrap",bool complete=true)
{
    worker::image_configuration config;s::buffers::profile profile;assert(profile.identity.assign("closed-ros-startup"));profile.sha256[0]=1;
    assert(config.profile(profile,"/opt/ros/jazzy")==s::error::success);
    assert(config.launcher(dir.path+"/launcher",64,{})==s::error::success);
    assert(config.image(sandbox::image_role::bootstrap,"/worker/bootstrap",dir.path+'/'+text(bootstrap),64,{})==s::error::success);
    assert(config.image(sandbox::image_role::runtime,"/worker/backend.so",dir.path+"/backend",64,{})==s::error::success);
    assert(config.image(sandbox::image_role::configuration,"/worker/profile",dir.path+"/profile",64,profile.sha256)==s::error::success);
    auto limits=bounds();
    for(auto const& field:worker::image_limit_fields)if(complete||field.name!="threads")assert(config.limit(field.name,limits.*field.member)==s::error::success);
    return config;
}
static void inert_and_pinned()
{
    directory dir;auto config=setting(dir,"bootstrap",false);
    assert(config.freeze()==s::error::invalid&&!config.frozen());
    assert(config.limit("threads",2)==s::error::success&&config.limit("threads",2)==s::error::conflict);
    assert(config.limit("unknown",1)==s::error::invalid&&config.limit("threads",UINT64_MAX)==s::error::invalid);
    assert(config.freeze()==s::error::success&&config.frozen());
    worker::image_configuration::prepared output;
    assert(config.prepare(output)==s::error::io&&output.descriptors.empty()&&output.manifest.files.empty());
    for(auto name:{"launcher","bootstrap","backend","profile"})dir.file(name);
    assert(config.limit("threads",1)==s::error::denied&&config.launcher(dir.path+"/launcher",64,{})==s::error::denied);
    assert(config.freeze()==s::error::conflict&&config.prepare(output)==s::error::success);
    assert(output.descriptors.size()==4&&output.manifest.files.size()==3&&sandbox::valid(output.manifest));
    assert(config.prepare(output)==s::error::conflict);
    dir.file("replacement",9);assert(::rename((dir.path+"/replacement").c_str(),(dir.path+"/bootstrap").c_str())==0);
    std::array<std::uint8_t,64> bytes{};assert(::pread(output.manifest.files[0].source,bytes.data(),bytes.size(),0)==64&&bytes[4]==7);
    sandbox::descriptor sealed;assert(sandbox::seal(output.manifest.files[0],sealed)!=sandbox::error::success&&!sealed);
    // prepare pins size/type; sealing is the mandatory later SHA256 boundary.
    output.manifest.files[0].sha256=s::buffers::hash(bytes);
    assert(sandbox::seal(output.manifest.files[0],sealed)==sandbox::error::success&&sealed);
}
static void refuse_special_sources()
{
    directory dir;for(auto name:{"launcher","bootstrap","backend","profile"})dir.file(name);
    assert(::symlink((dir.path+"/bootstrap").c_str(),(dir.path+"/link").c_str())==0);
    assert(::mkfifo((dir.path+"/fifo").c_str(),0600)==0);
    for(auto name:{"link","fifo"})
    {
        auto config=setting(dir,name);assert(config.freeze()==s::error::success);
        worker::image_configuration::prepared out;assert(config.prepare(out)==s::error::invalid&&out.descriptors.empty()&&out.manifest.files.empty());
    }
    assert(::truncate((dir.path+"/backend").c_str(),65)==0);
    auto wrong_size=setting(dir);assert(wrong_size.freeze()==s::error::success);
    worker::image_configuration::prepared out;assert(wrong_size.prepare(out)==s::error::invalid&&out.descriptors.empty());
}
static void closed_description()
{
    directory dir;worker::image_configuration empty;assert(empty.freeze()==s::error::invalid);
    s::buffers::profile profile;assert(profile.identity.assign("profile"));
    assert(empty.profile(profile,"/opt/ros/../host")==s::error::invalid&&!empty.frozen());
    assert(empty.profile(profile,"/opt/ros/jazzy")==s::error::success&&empty.profile(profile,"/opt/ros/jazzy")==s::error::conflict);
    assert(empty.launcher("relative",1,{})==s::error::invalid);
    assert(empty.image(sandbox::image_role::runtime,"/host/library",dir.path+"/backend",64,{})==s::error::invalid);
    auto duplicate=setting(dir);assert(duplicate.image(sandbox::image_role::runtime,"/worker/backend.so",dir.path+"/backend",64,{})==s::error::conflict);
    auto zero=setting(dir,"bootstrap",false);assert(zero.limit("threads",0)==s::error::success&&zero.freeze()==s::error::invalid);
    auto placement=setting(dir);assert(placement.placement("inherit","inherit","none")==s::error::success);
    assert(placement.placement("inherit","inherit","none")==s::error::conflict&&placement.freeze()==s::error::success);
}
int main()
{
    inert_and_pinned();refuse_special_sources();closed_description();
    std::puts("PASS isolated worker image configuration: inert finite settings, exact regular pinning, symlink/FIFO/size refusal, inode replacement, mandatory sealed digest and closed placement");
}
