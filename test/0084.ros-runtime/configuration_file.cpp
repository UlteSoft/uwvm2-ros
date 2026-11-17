/** @file
 * @brief Exercise pinned configuration files with real filesystem races and recoverable failures.
 * @details Linker wrappers observe only the helper's syscall/allocation window.
 * Test fixtures are explicit SDK temporary paths. Device/FIFO tests perform
 * only O_PATH classification; a data-open attempt on them fails the assertion.
 */
#include <uwvm2/imported/ros/scene/configuration_file.h>
#include <cassert>
#include <cstdio>
#include <cstdarg>
#include <dirent.h>
#include <pthread.h>
#include <sched.h>
#include <sys/socket.h>
#include <sys/un.h>

namespace scene=uwvm2::imported::ros::scene;
namespace container=uwvm2::utils::container;
using E=uwvm2::imported::ros::resources::error;
using bytes=scene::wire::bytes;
extern "C" int __real_open(char const*,int,...);
extern "C" int __real_close(int);
extern "C" int __real_fstat(int,struct stat*);
extern "C" ssize_t __real_pread(int,void*,std::size_t,off_t);
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);

enum class mutation {none,rename_before_stat,rename_after_stat,grow,shrink,rewrite,concurrent_rewrite};
struct observation
{
    bool enabled{},allocation_failed{};
    unsigned open_calls{},opened{},closed{},stat_calls{},read_calls{},allocation_calls{},allocation_failure{},open_failure{},stat_failure{},read_failure{},interrupts{};
    int identity{-1},source{-1},writer{-1};
    std::size_t chunk{},expected{};
    off_t next_offset{};
    char const* path{};char const* moved{};char const* replacement{};
    mutation mutate{};
    std::atomic<unsigned>* handshake{};
};
static thread_local observation watched;
static std::array<std::uint8_t,4096> original{},changed{};

static void replace_path()
{
    assert(watched.path&&watched.moved);
    assert(::rename(watched.path,watched.moved)==0);
    assert(::mkfifo(watched.path,0600)==0);
}
static bool reject_allocation() noexcept
{
    if(!watched.enabled)return false;
    ++watched.allocation_calls;
    if(watched.allocation_failure&&watched.allocation_calls==watched.allocation_failure)
    {watched.allocation_failed=true;return true;}
    return false;
}
extern "C" void* __wrap_malloc(std::size_t size){return reject_allocation()?nullptr:__real_malloc(size);}
extern "C" void* __wrap_calloc(std::size_t count,std::size_t size){return reject_allocation()?nullptr:__real_calloc(count,size);}
extern "C" void* __wrap_realloc(void* pointer,std::size_t size){return reject_allocation()?nullptr:__real_realloc(pointer,size);}
extern "C" int __wrap_open(char const* path,int flags,...)
{
    mode_t mode{};if(flags&O_CREAT){va_list arguments;va_start(arguments,flags);mode=static_cast<mode_t>(va_arg(arguments,int));va_end(arguments);}
    if(!watched.enabled)return __real_open(path,flags,mode);
    ++watched.open_calls;
    assert(flags&O_CLOEXEC);
    if(watched.open_calls==1)assert((flags&(O_PATH|O_NOFOLLOW))==(O_PATH|O_NOFOLLOW));
    else assert(watched.open_calls==2&&(flags&O_NONBLOCK)&&std::string_view{path}.starts_with("/proc/self/fd/"));
    if(watched.open_calls==watched.open_failure){errno=EACCES;return -1;}
    if(watched.open_calls==2&&watched.mutate==mutation::rename_after_stat)replace_path();
    auto actual=watched.open_calls==2&&watched.replacement?watched.replacement:path;
    auto result=__real_open(actual,flags,mode);
    if(result>=0)
    {
        ++watched.opened;assert(::fcntl(result,F_GETFD)&FD_CLOEXEC);
        if(watched.open_calls==1)
        {watched.identity=result;if(watched.mutate==mutation::rename_before_stat)replace_path();}
        else watched.source=result;
    }
    return result;
}
extern "C" int __wrap_close(int fd)
{
    if(watched.enabled){assert(fd==watched.identity||fd==watched.source);++watched.closed;}
    return __real_close(fd);
}
extern "C" int __wrap_fstat(int fd,struct stat* output)
{
    if(watched.enabled)
    {
        ++watched.stat_calls;
        if(watched.stat_calls==watched.stat_failure){errno=EIO;return -1;}
    }
    return __real_fstat(fd,output);
}
extern "C" ssize_t __wrap_pread(int fd,void* destination,std::size_t size,off_t offset)
{
    if(!watched.enabled)return __real_pread(fd,destination,size,offset);
    assert(fd==watched.source&&fd!=watched.identity);++watched.read_calls;
    assert(offset==watched.next_offset);
    if(watched.interrupts){--watched.interrupts;errno=EINTR;return -1;}
    if(watched.read_calls==watched.read_failure){errno=EIO;return -1;}
    if(watched.chunk&&size>watched.chunk)size=watched.chunk;
    auto result=__real_pread(fd,destination,size,offset);
    if(result>0)watched.next_offset+=result;
    if(offset==0&&result>0)
    {
        if(watched.mutate==mutation::grow)
        {std::uint8_t extra=7;assert(::pwrite(watched.writer,&extra,1,static_cast<off_t>(watched.expected))==1);}
        else if(watched.mutate==mutation::shrink)assert(::ftruncate(watched.writer,result)==0);
        else if(watched.mutate==mutation::rewrite)assert(::pwrite(watched.writer,changed.data(),changed.size(),0)==static_cast<ssize_t>(changed.size()));
        else if(watched.mutate==mutation::concurrent_rewrite)
        {
            watched.handshake->store(1,std::memory_order_release);
            while(watched.handshake->load(std::memory_order_acquire)!=2)::sched_yield();
        }
    }
    return result;
}

struct descriptor
{
    int value{-1};descriptor()=default;explicit descriptor(int value):value(value){}
    descriptor(descriptor const&)=delete;descriptor& operator=(descriptor const&)=delete;
    ~descriptor(){if(value>=0)assert(__real_close(value)==0);}
};
static unsigned descriptor_count()
{
    auto* directory=::opendir("/proc/self/fd");assert(directory);unsigned count{};
    while(auto* value=::readdir(directory))if(value->d_name[0]!='.')++count;
    assert(::closedir(directory)==0);return count;
}
static void write_file(char const* path,bytes content)
{
    descriptor file{__real_open(path,O_WRONLY|O_CREAT|O_TRUNC|O_CLOEXEC,0600)};assert(file.value>=0);
    std::size_t done{};while(done<content.size())
    {auto result=::write(file.value,content.data()+done,content.size()-done);assert(result>0);done+=static_cast<std::size_t>(result);}
}
struct fixture
{
    container::std_string directory,path,moved,other;
    explicit fixture(std::string_view base,std::string_view name):directory(base)
    {
        directory.push_back('/');directory.append(name);assert(::mkdir(directory.c_str(),0700)==0);
        path=directory;path.append("/source");moved=directory;moved.append("/pinned");other=directory;other.append("/replacement");
        write_file(path.c_str(),original);write_file(other.c_str(),changed);
    }
};
static void seed(scene::byte_buffer& output,std::size_t size=37)
{output.resize_for_overwrite(size);std::fill(output.writable().begin(),output.writable().end(),std::uint8_t{91});}
static E invoke(std::string_view path,std::uint64_t expected,scene::buffers::digest digest,std::size_t ceiling,scene::byte_buffer& output)
{
    auto before=descriptor_count();watched.enabled=true;
    auto status=scene::configuration_file::read(path,expected,digest,ceiling,output);
    watched.enabled=false;assert(watched.opened==watched.closed&&descriptor_count()==before);return status;
}
static void unchanged(scene::byte_buffer const& output,std::size_t expected=37)
{assert(output.view().size()==expected&&std::all_of(output.view().begin(),output.view().end(),[](auto byte){return byte==91;}));}
static void success_and_pinned_identity(std::string_view base)
{
    fixture value{base,"success"};auto digest=scene::buffers::hash(original);scene::byte_buffer output;seed(output);
    watched={};assert(invoke(value.path,original.size(),digest,original.size(),output)==E::success);
    assert(std::equal(output.view().begin(),output.view().end(),original.begin(),original.end()));
    assert(watched.open_calls==2&&watched.stat_calls==3&&watched.read_calls==2);
    write_file(value.other.c_str(),bytes{original}.first(73));watched={};seed(output,1024);
    assert(invoke(value.other,73,scene::buffers::hash(bytes{original}.first(73)),73,output)==E::success&&output.view().size()==73);
    assert(std::equal(output.view().begin(),output.view().end(),original.begin()));
    // Replacing the path after O_PATH pinning cannot redirect the later data open to a FIFO.
    watched={};watched.path=value.path.c_str();watched.moved=value.moved.c_str();watched.mutate=mutation::rename_before_stat;
    assert(invoke(value.path,original.size(),digest,original.size(),output)==E::success);
    assert(std::equal(output.view().begin(),output.view().end(),original.begin(),original.end()));
}
static void reject_paths_before_data_io(std::string_view base)
{
    fixture value{base,"paths"};auto digest=scene::buffers::hash(original);scene::byte_buffer output;seed(output);
    auto rejected=[&](std::string_view path,std::uint64_t expected,std::size_t ceiling,E result,bool no_open)
    {
        watched={};assert(invoke(path,expected,digest,ceiling,output)==result);unchanged(output);
        assert(watched.open_calls==(no_open?0u:1u)&&watched.read_calls==0);
    };
    rejected("",4096,4096,E::invalid,true);rejected(value.path,0,4096,E::invalid,true);
    rejected(value.path,4096,4095,E::invalid,true);rejected(value.path,UINT64_MAX,SIZE_MAX,E::too_large,true);
    std::array<char,4097> huge{};huge.fill('x');rejected({huge.data(),huge.size()},4096,4096,E::invalid,true);
    std::array<char,4> nul{'a','b','\0','c'};rejected({nul.data(),nul.size()},4096,4096,E::invalid,true);
    rejected(value.directory,4096,4096,E::invalid,false);
    rejected("/dev/null",4096,4096,E::invalid,false);rejected("/dev/zero",4096,4096,E::invalid,false);
    auto link=value.directory;link.append("/symlink");assert(::symlink(value.path.c_str(),link.c_str())==0);rejected(link,4096,4096,E::invalid,false);
    auto fifo=value.directory;fifo.append("/fifo");assert(::mkfifo(fifo.c_str(),0600)==0);rejected(fifo,4096,4096,E::invalid,false);
    // Bind a real Unix socket through a short /proc/self/fd directory alias.
    descriptor directory{__real_open(value.directory.c_str(),O_PATH|O_DIRECTORY|O_CLOEXEC)};assert(directory.value>=0);
    descriptor socket{::socket(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0)};assert(socket.value>=0);sockaddr_un address{};address.sun_family=AF_UNIX;
    auto size=std::snprintf(address.sun_path,sizeof(address.sun_path),"/proc/self/fd/%d/socket",directory.value);assert(size>0&&static_cast<std::size_t>(size)<sizeof(address.sun_path));
    assert(::bind(socket.value,reinterpret_cast<sockaddr*>(&address),sizeof(address))==0);
    auto socket_path=value.directory;socket_path.append("/socket");rejected(socket_path,4096,4096,E::invalid,false);
    rejected(value.path,4095,4096,E::conflict,false);rejected(value.path,4097,4097,E::conflict,false);
    auto missing=value.directory;missing.append("/missing");rejected(missing,4096,4096,E::io,false);
    write_file(value.other.c_str(),{});rejected(value.other,1,1,E::conflict,false);
}
static void failures_short_reads_and_interrupts(std::string_view base)
{
    fixture value{base,"syscalls"};auto digest=scene::buffers::hash(original);scene::byte_buffer output;
    for(unsigned stage=1;stage<=2;++stage)
    {seed(output);watched={};watched.open_failure=stage;assert(invoke(value.path,4096,digest,4096,output)==E::io);unchanged(output);}
    for(unsigned stage=1;stage<=3;++stage)
    {seed(output);watched={};watched.stat_failure=stage;assert(invoke(value.path,4096,digest,4096,output)==E::io);unchanged(output);}
    for(unsigned stage=1;stage<=3;++stage)
    {seed(output);watched={};watched.chunk=2048;watched.read_failure=stage;assert(invoke(value.path,4096,digest,4096,output)==E::io);unchanged(output);}
    seed(output);watched={};watched.chunk=7;watched.interrupts=3;
    assert(invoke(value.path,4096,digest,4096,output)==E::success&&watched.read_calls>580);
    assert(std::equal(output.view().begin(),output.view().end(),original.begin(),original.end()));
    seed(output);watched={};digest[0]^=1;assert(invoke(value.path,4096,digest,4096,output)==E::conflict);unchanged(output);
    seed(output);watched={};watched.replacement=value.other.c_str();digest[0]^=1;
    assert(invoke(value.path,4096,digest,4096,output)==E::conflict&&watched.read_calls==0);unchanged(output);
}
static void observed_file_mutations(std::string_view base)
{
    auto digest=scene::buffers::hash(original);
    for(auto operation:{mutation::grow,mutation::shrink,mutation::rewrite,mutation::rename_after_stat})
    {
        auto index=static_cast<unsigned>(operation);std::array<char,32> name{};std::snprintf(name.data(),name.size(),"mutation-%u",index);fixture value{base,name.data()};
        descriptor writer{__real_open(value.path.c_str(),O_RDWR|O_CLOEXEC)};assert(writer.value>=0);scene::byte_buffer output;seed(output);
        watched={};watched.chunk=64;watched.expected=4096;watched.writer=writer.value;watched.path=value.path.c_str();watched.moved=value.moved.c_str();watched.mutate=operation;
        auto result=invoke(value.path,4096,digest,4096,output);assert(result==E::io||result==E::conflict);unchanged(output);
    }
    fixture value{base,"concurrent-writer"};descriptor writer{__real_open(value.path.c_str(),O_RDWR|O_CLOEXEC)};assert(writer.value>=0);
    struct context{std::atomic<unsigned> stage{};int writer;};context state{{},writer.value};pthread_t thread{};
    assert(::pthread_create(&thread,nullptr,[](void* pointer)->void*
    {
        auto& state=*static_cast<context*>(pointer);while(state.stage.load(std::memory_order_acquire)!=1)::sched_yield();
        assert(::pwrite(state.writer,changed.data(),changed.size(),0)==static_cast<ssize_t>(changed.size()));state.stage.store(2,std::memory_order_release);return nullptr;
    },&state)==0);
    scene::byte_buffer output;seed(output);watched={};watched.chunk=64;watched.handshake=&state.stage;watched.mutate=mutation::concurrent_rewrite;
    auto result=invoke(value.path,4096,digest,4096,output);assert(::pthread_join(thread,nullptr)==0);
    assert(result==E::conflict);unchanged(output);
}
static void allocation_failure_preserves_output(std::string_view base)
{
    fixture value{base,"allocation"};auto digest=scene::buffers::hash(original);unsigned failures{};
    for(unsigned point=1;point<=8;++point)
    {
        scene::byte_buffer output;seed(output,1024);watched={};watched.allocation_failure=point;
        auto status=invoke(value.path,4096,digest,4096,output);
        if(!watched.allocation_failed){assert(status==E::success);break;}
        ++failures;assert(status==E::no_memory);unchanged(output,1024);
        watched={};assert(invoke(value.path,4096,digest,4096,output)==E::success);
    }
    assert(failures>=3); // owned path, staged payload, final output allocation
}
static void concurrent_reads_and_descriptor_reclamation(std::string_view base)
{
    fixture value{base,"parallel"};auto digest=scene::buffers::hash(original);auto before=descriptor_count();
    struct context{std::string_view path;scene::buffers::digest digest;};context state{value.path,digest};std::array<pthread_t,4> threads{};
    for(auto& thread:threads)assert(::pthread_create(&thread,nullptr,[](void* pointer)->void*
    {
        auto const& state=*static_cast<context*>(pointer);
        for(unsigned repeat{};repeat<64;++repeat)
        {
            scene::byte_buffer output;watched={};watched.enabled=true;
            auto result=scene::configuration_file::read(state.path,4096,state.digest,4096,output);watched.enabled=false;
            assert(result==E::success&&watched.opened==watched.closed&&std::equal(output.view().begin(),output.view().end(),original.begin(),original.end()));
        }
        return nullptr;
    },&state)==0);
    for(auto thread:threads)assert(::pthread_join(thread,nullptr)==0);
    assert(descriptor_count()==before);
    scene::byte_buffer output;seed(output);
    for(unsigned repeat{};repeat<128;++repeat)
    {watched={};watched.stat_failure=repeat%3+1;assert(invoke(value.path,4096,digest,4096,output)==E::io);unchanged(output);}
    assert(descriptor_count()==before);
}
int main(int argc,char** argv)
{
    assert(argc==2);assert(::mkdir(argv[1],0700)==0);
    for(std::size_t index{};index<original.size();++index){original[index]=static_cast<std::uint8_t>(index*17+3);changed[index]=static_cast<std::uint8_t>(original[index]^0xff);}
    success_and_pinned_identity(argv[1]);reject_paths_before_data_io(argv[1]);failures_short_reads_and_interrupts(argv[1]);
    observed_file_mutations(argv[1]);allocation_failure_preserves_output(argv[1]);concurrent_reads_and_descriptor_reclamation(argv[1]);
    std::puts("passed 6 configuration file security groups");
}
