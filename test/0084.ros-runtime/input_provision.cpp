/** @file
 * @brief Exercise the real trusted input provisioner with filesystem adversaries.
 * @details Driver-only test; run as a normal UID inside the existing 64 GiB
 * cgroup. Private tmpfs inputs test seals, identity pinning, size/digest refusal,
 * descriptor cleanup, special-file avoidance and preservation of old outputs.
 */
#include <uwvm2/uwvm/imported/scene/input.h>
#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <thread>
#include <sys/inotify.h>
#include <sys/wait.h>
#include <poll.h>

namespace model=uwvm2::imported::ros::scene;
namespace storage=model::storage;
namespace fs=std::filesystem;
using uwvm2::uwvm::imported::scene::prepare_input;
static storage::principal const owner{314,"approved"};
static unsigned passed;
static void mark(char const* value){++passed;std::cout<<"PASS "<<value<<'\n';}
struct directory
{
    std::string path;
    directory()
    {
        auto base=std::getenv("UWVM_INPUT_TEST_TMP");assert(base&&*base);
        std::string pattern=std::string(base)+"/input-XXXXXX";
        auto created=::mkdtemp(pattern.data());assert(created);path=created;
    }
    ~directory(){fs::permissions(path,fs::perms::owner_all);fs::remove_all(path);}
};
static void write_file(std::string const& path,std::string_view bytes)
{
    int fd=::open(path.c_str(),O_WRONLY|O_CREAT|O_TRUNC|O_CLOEXEC,0600);assert(fd>=0);
    std::size_t written{};
    while(written<bytes.size()){auto count=::write(fd,bytes.data()+written,bytes.size()-written);assert(count>0);written+=count;}
    assert(::close(fd)==0);
}
static model::artifacts::input_plan plan(std::string path,std::string_view content,std::uint64_t transfer=4096)
{
    model::artifacts::input_plan result;result.path=std::move(path);result.generation=7;result.bytes=content.size();result.transfer=transfer;
    fast_io::sha256_context digest{};
    auto first=reinterpret_cast<std::byte const*>(content.data());digest.update(first,first+content.size());digest.do_final();
    std::array<std::byte,32> bytes{};digest.digest_to_byte_ptr(bytes.data());
    for(std::size_t index=0;index<bytes.size();++index)result.digest[index]=std::to_integer<std::uint8_t>(bytes[index]);
    return result;
}
static std::size_t descriptors()
{std::size_t count{};for(auto const& entry:fs::directory_iterator("/proc/self/fd")){(void)entry;++count;}return count;}
static void contents(std::shared_ptr<storage::immutable_input> const& input,std::string_view expected)
{
    std::string result(expected.size(),'\0');std::size_t count{};
    assert(input->read(owner,0,{reinterpret_cast<std::byte*>(result.data()),result.size()},count)==storage::success);
    assert(count==expected.size()&&result==expected);
}
static void immutable_and_purposes()
{
    directory root;std::string payload="approved immutable robot model";auto source=root.path+"/source";write_file(source,payload);
    auto approved=plan(source,payload);std::shared_ptr<storage::immutable_input> output;
    for(auto purpose:{model::purpose::package,model::purpose::configuration,model::purpose::calibration,model::purpose::map,model::purpose::model_weights})
    {
        assert(prepare_input(approved,owner,purpose,output)==model::error::success);contents(output,payload);
        std::uint64_t size{};model::purpose observed{};assert(output->describe(owner,size,observed)==storage::success&&size==payload.size()&&observed==purpose);
    }
    write_file(source,"changed!");contents(output,payload);
    std::array<std::byte,1> bytes{};std::size_t count{};assert(output->read({315,"approved"},0,bytes,count)==storage::denied);
    write_file(root.path+"/empty","");auto empty=plan(root.path+"/empty","",0);
    assert(prepare_input(empty,owner,model::purpose::configuration,output)==model::error::success);contents(output,"");
    mark("all input purposes, source mutation isolation, principal isolation, empty input");
}
static void refusal_and_cleanup()
{
    directory root;auto source=root.path+"/input";write_file(source,"known");auto approved=plan(source,"known");
    std::shared_ptr<storage::immutable_input> retained;assert(prepare_input(approved,owner,model::purpose::package,retained)==model::error::success);
    auto initial=descriptors();
    for(unsigned iteration{};iteration!=64;++iteration)
    {
        for(unsigned kind{};kind!=9;++kind)
        {
            auto invalid=approved;auto principal=owner;auto purpose=model::purpose::package;
            switch(kind)
            {
            case 0:invalid.digest[0]^=1;break;
            case 1:--invalid.bytes;break;
            case 2:++invalid.bytes;break;
            case 3:invalid.generation=0;break;
            case 4:invalid.transfer=4u*1024u*1024u+1;break;
            case 5:principal.environment=0;break;
            case 6:purpose=model::purpose::recording;break;
            case 7:invalid.path+="/missing";break;
            case 8:invalid.bytes=UINT64_MAX;break;
            }
            auto output=retained;assert(prepare_input(invalid,principal,purpose,output)!=model::error::success&&output==retained);
        }
    }
    assert(descriptors()==initial);contents(retained,"known");
    mark("digest/extent/generation/purpose/owner refusal preserves output and closes temporary FDs");
}
static void special_files_and_paths()
{
    directory root;write_file(root.path+"/regular","known");assert(::mkfifo((root.path+"/fifo").c_str(),0600)==0);
    assert(::symlink((root.path+"/regular").c_str(),(root.path+"/link").c_str())==0);
    assert(::symlink(root.path.c_str(),(root.path+"/directory-link").c_str())==0);
    int notify=::inotify_init1(IN_CLOEXEC|IN_NONBLOCK);assert(notify>=0);
    assert(::inotify_add_watch(notify,(root.path+"/fifo").c_str(),IN_OPEN)>=0);
    for(auto const& path:{root.path,root.path+"/fifo",root.path+"/link",root.path+"/directory-link/regular",std::string("/dev/null"),
                         root.path+"/../regular",std::string("relative"),std::string("/"),root.path+"/regular/",std::string("/a\0b",4)})
    {std::shared_ptr<storage::immutable_input> output;assert(prepare_input(plan(path,"known"),owner,model::purpose::package,output)!=model::error::success&&!output);}
    std::array<std::byte,4096> events{};assert(::read(notify,events.data(),events.size())<0&&errno==EAGAIN);::close(notify);
    assert(::chmod((root.path+"/regular").c_str(),0000)==0);
    std::shared_ptr<storage::immutable_input> output;
    assert(prepare_input(plan(root.path+"/regular","known"),owner,model::purpose::package,output)!=model::error::success&&!output);
    assert(::chmod((root.path+"/regular").c_str(),0600)==0);
    mark("no read-open of FIFO/device, no final/intermediate symlinks, malformed paths, read permission");
}
static void bounded_chunk_copy()
{
    directory root;std::string payload(3u*64u*1024u+31,'x');
    for(std::size_t i{};i<payload.size();++i)payload[i]=static_cast<char>((i*37u)%251u);
    auto source=root.path+"/weights";write_file(source,payload);auto approved=plan(source,payload,payload.size());
    std::shared_ptr<storage::immutable_input> output;assert(prepare_input(approved,owner,model::purpose::model_weights,output)==model::error::success);
    contents(output,payload);fs::remove(source);contents(output,payload);
    mark("multichunk exact binary snapshot survives source removal");
}
static void replaced_path_cannot_open_fifo()
{
    directory root;auto source=root.path+"/source",other=root.path+"/other";write_file(source,"known");
    assert(::mkfifo(other.c_str(),0600)==0);
    int notify=::inotify_init1(IN_CLOEXEC|IN_NONBLOCK);assert(notify>=0);
    assert(::inotify_add_watch(notify,other.c_str(),IN_OPEN)>=0);
    std::atomic<bool> stop{};std::atomic<unsigned> exchanges{};
    std::thread attacker([&]
    {
        while(!stop.load(std::memory_order_acquire))
        {
            assert(::syscall(SYS_renameat2,AT_FDCWD,source.c_str(),AT_FDCWD,other.c_str(),RENAME_EXCHANGE)==0);
            exchanges.fetch_add(1,std::memory_order_release);
        }
    });
    while(!exchanges.load(std::memory_order_acquire))std::this_thread::yield();
    auto approved=plan(source,"known");
    for(unsigned iteration{};iteration!=512;++iteration)
    {
        std::shared_ptr<storage::immutable_input> output;
        auto status=prepare_input(approved,owner,model::purpose::package,output);
        if(status==model::error::success)contents(output,"known");
        else assert(!output);
    }
    stop.store(true,std::memory_order_release);attacker.join();
    std::array<std::byte,4096> events{};assert(::read(notify,events.data(),events.size())<0&&errno==EAGAIN);::close(notify);
    mark("concurrent regular/FIFO name replacement never read-opens the special inode");
}
int main()
{
    assert(::geteuid()!=0);
    immutable_and_purposes();refusal_and_cleanup();special_files_and_paths();bounded_chunk_copy();replaced_path_cannot_open_fifo();
    std::cout<<"passed "<<passed<<" input provisioning groups\n";
}
