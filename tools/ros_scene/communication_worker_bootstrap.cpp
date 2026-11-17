/** @file
 * @brief Pin the communication backend's complete startup closure before traffic.
 * @details Only trusted manifest bytes are consumed during prepare. Every ROS,
 * FastDDS, executor and pump thread must already exist behind a closed backend
 * gate. Two all-thread seccomp fences precede traffic: the first freezes code,
 * files and threads; the second grants exact read/write directions to four
 * kernel-enforced one-way pipes plus verified internal notification objects.
 * The supervisor must keep all broker traffic disabled until attached arrives.
 */
#include "../../test/0084.ros-runtime/support/legacy_process/communication_worker_sandbox.h"
#include <charconv>
#include <dlfcn.h>
#include <sys/statvfs.h>
#include <sys/sysmacros.h>
#include <sys/vfs.h>
#include <sys/personality.h>
namespace s=uwvm2::imported::ros::scene;
namespace x=s::communication_sandbox;
namespace w=s::communication_worker;
static bool write_all(int fd,std::string_view value) noexcept
{
    std::size_t offset{};while(offset<value.size())
    {auto n=::write(fd,value.data()+offset,value.size()-offset);if(n<0&&errno==EINTR)continue;if(n<=0)return false;offset+=static_cast<std::size_t>(n);}return true;
}
static bool read_exact(int fd,std::span<std::uint8_t> value)noexcept
{
    std::size_t offset{};while(offset<value.size())
    {auto n=::read(fd,value.data()+offset,value.size()-offset);if(n<0&&errno==EINTR)continue;if(n<=0)return false;offset+=static_cast<std::size_t>(n);}return true;
}
static bool link_name(int fd,std::array<char,128>& output,std::string_view& view)noexcept
{
    std::array<char,64> path{};constexpr std::string_view prefix="/proc/self/fd/";std::copy(prefix.begin(),prefix.end(),path.begin());
    auto n=std::to_chars(path.data()+prefix.size(),path.data()+path.size()-1,fd);if(n.ec!=std::errc{})return false;*n.ptr=0;
    auto size=::readlink(path.data(),output.data(),output.size());if(size<=0||static_cast<std::size_t>(size)>=output.size())return false;
    view={output.data(),static_cast<std::size_t>(size)};return true;
}
static bool notifications(w::prepared_backend const& backend)noexcept
{
    if(!backend.context||backend.notification_count>backend.notifications.size())return false;
    for(std::size_t i{};i<backend.notification_count;++i)
    {
        auto value=backend.notifications[i];if(value.descriptor<4||value.direction<w::io_direction::read||value.direction>w::io_direction::both||value.kind>w::notification_kind::pipe)return false;
        for(std::size_t j{};j<i;++j)if(backend.notifications[j].descriptor==value.descriptor)return false;
        struct stat status{};if(::fstat(value.descriptor,&status))return false;std::array<char,128> text{};std::string_view link;
        if(!link_name(value.descriptor,text,link))return false;
        if(value.kind==w::notification_kind::event)
        {auto flags=::fcntl(value.descriptor,F_GETFL);if(link!="anon_inode:[eventfd]"||flags<0||(flags&O_ACCMODE)!=O_RDWR||!(flags&O_NONBLOCK))return false;}
        else if(value.kind==w::notification_kind::poll)
        {if(link!="anon_inode:[eventpoll]"||value.direction!=w::io_direction::both)return false;}
        else
        {
            if(value.direction==w::io_direction::both||!x::anonymous_pipe(value.descriptor,value.direction==w::io_direction::read,status))return false;
        }
    }
    return true;
}
static bool descriptor_closure(int directory,int status_fd,int maps_fd,w::prepared_backend const& backend)noexcept
{
    if(::lseek(directory,0,SEEK_SET)<0)return false;
    std::array<unsigned char,8192> bytes{};unsigned seen{};
    for(;;)
    {
        auto got=::syscall(SYS_getdents64,directory,bytes.data(),bytes.size());if(got<0&&errno==EINTR)continue;if(got<0)return false;if(!got)return true;
        for(std::size_t offset{};offset<static_cast<std::size_t>(got);)
        {
            if(static_cast<std::size_t>(got)-offset<20)return false;std::uint16_t extent{};std::memcpy(&extent,bytes.data()+offset+16,2);
            if(extent<20||extent>static_cast<std::size_t>(got)-offset)return false;
            auto first=reinterpret_cast<char const*>(bytes.data()+offset+19);auto available=extent-19;
            auto end=static_cast<char const*>(std::memchr(first,0,available));if(!end)return false;std::string_view name{first,static_cast<std::size_t>(end-first)};offset+=extent;
            if(name=="."||name=="..")continue;if(++seen>40)return false;
            int fd{};auto parsed=std::from_chars(name.data(),name.data()+name.size(),fd);if(parsed.ec!=std::errc{}||parsed.ptr!=name.data()+name.size()||fd<0)return false;
            if(fd<=3||fd==directory||fd==status_fd||fd==maps_fd)continue;
            bool found{};for(std::size_t i{};i<backend.notification_count;++i)found|=backend.notifications[i].descriptor==fd;
            if(!found)return false;
        }
    }
}
struct code_identity{dev_t device{};ino_t inode{};};
/** @brief Rehash the exact immutable namespace image and retain its kernel identity. */
static bool executable_identity(char const* path,char const* length,char const* hash,code_identity& result)noexcept
{
    std::string_view name{path},size_text{length},digest_text{hash};std::uint64_t size{};
    auto number=std::from_chars(size_text.data(),size_text.data()+size_text.size(),size);
    if(!x::destination(name)||number.ec!=std::errc{}||number.ptr!=size_text.data()+size_text.size()||!size||digest_text.size()!=64)return false;
    s::buffers::digest expected{};
    for(std::size_t i{};i<32;++i){unsigned byte{};auto first=digest_text.data()+i*2;auto parsed=std::from_chars(first,first+2,byte,16);if(parsed.ec!=std::errc{}||parsed.ptr!=first+2)return false;expected[i]=static_cast<std::uint8_t>(byte);}
    x::descriptor fd{::open(path,O_RDONLY|O_NOFOLLOW|O_CLOEXEC)};struct stat info{};struct statfs fs{};
    if(!fd||::fstat(fd.get(),&info)||!S_ISREG(info.st_mode)||(info.st_mode&0777)!=0500||info.st_size<0||static_cast<std::uint64_t>(info.st_size)!=size||
       ::fstatfs(fd.get(),&fs)||!(fs.f_flags&ST_RDONLY))return false;
    fast_io::sha256_context hashing;std::array<std::byte,65536> buffer{};std::uint64_t offset{};
    while(offset<size)
    {auto n=::pread(fd.get(),buffer.data(),std::min<std::uint64_t>(buffer.size(),size-offset),static_cast<off_t>(offset));if(n<0&&errno==EINTR)continue;if(n<=0)return false;hashing.update(buffer.data(),buffer.data()+n);offset+=static_cast<std::uint64_t>(n);}
    hashing.do_final();s::buffers::digest actual{};hashing.digest_to_byte_ptr(reinterpret_cast<std::byte*>(actual.data()));if(actual!=expected)return false;
    result={info.st_dev,info.st_ino};return true;
}
/** @brief Check actual post-TSYNC executable mappings against immutable object identities.
 * @details New executable mappings, READ_IMPLIES_EXEC and mremap are unavailable
 * during this scan and afterward. Existing executable mappings may only disappear
 * or lose execute permission; an existing RWX map cannot move behind the cursor.
 */
static bool executable_mappings(int fd,std::span<code_identity const> images)noexcept
{
    std::array<char,1024*1024> buffer{};std::size_t size{};
    for(;;){if(size==buffer.size())return false;auto n=::read(fd,buffer.data()+size,buffer.size()-size);if(n<0&&errno==EINTR)continue;if(n<0)return false;if(!n)break;size+=static_cast<std::size_t>(n);}
    std::string_view remaining{buffer.data(),size};std::uint64_t previous_end{};bool executable{};
    while(!remaining.empty())
    {
        auto newline=remaining.find('\n');if(newline==remaining.npos)return false;auto line=remaining.substr(0,newline);remaining.remove_prefix(newline+1);
        auto token=[&]()noexcept{while(!line.empty()&&line.front()==' ')line.remove_prefix(1);auto n=line.find(' ');auto value=line.substr(0,n);line=n==line.npos?std::string_view{}:line.substr(n);return value;};
        auto address=token(),permissions=token(),offset=token(),device=token(),inode=token();
        auto parse=[](std::string_view text,std::uint64_t& value,int base){auto r=std::from_chars(text.data(),text.data()+text.size(),value,base);return !text.empty()&&r.ec==std::errc{}&&r.ptr==text.data()+text.size();};
        auto dash=address.find('-'),colon=device.find(':');std::uint64_t begin{},end{},file_offset{},major_id{},minor_id{},inode_id{};
        if(dash==address.npos||colon==device.npos||!parse(address.substr(0,dash),begin,16)||!parse(address.substr(dash+1),end,16)||begin>=end||begin<previous_end||
           permissions.size()!=4||!parse(offset,file_offset,16)||!parse(device.substr(0,colon),major_id,16)||!parse(device.substr(colon+1),minor_id,16)||!parse(inode,inode_id,10))return false;
        previous_end=end;if(permissions[2]!='x')continue;executable=true;if(permissions[1]=='w')return false;
        while(!line.empty()&&line.front()==' ')line.remove_prefix(1);
        if(!inode_id&&!major_id&&!minor_id&&(line=="[vdso]"||line=="[vsyscall]"))continue;
        bool approved{};
        for(auto image:images)approved|=major_id==::major(image.device)&&minor_id==::minor(image.device)&&inode_id==static_cast<std::uint64_t>(image.inode);
        if(!approved)return false;
    }
    return executable;
}
static bool process_status(int fd,std::uint64_t maximum_threads)noexcept
{
    if(::lseek(fd,0,SEEK_SET)<0)return false;std::array<char,16384> text{};std::size_t size{};
    for(;;)
    {
        if(size==text.size())return false;auto n=::read(fd,text.data()+size,text.size()-size);if(n<0&&errno==EINTR)continue;if(n<0)return false;if(!n)break;size+=static_cast<std::size_t>(n);
    }
    std::string_view value{text.data(),size};
    auto number=[&](std::string_view label,std::uint64_t& output,int base=10)
    {
        auto at=value.find(label);if(at==value.npos||value.find(label,at+1)!=value.npos)return false;at+=label.size();
        while(at<value.size()&&(value[at]==' '||value[at]=='\t'))++at;
        auto end=value.find('\n',at);if(end==value.npos)return false;
        auto parsed=std::from_chars(value.data()+at,value.data()+end,output,base);return parsed.ec==std::errc{}&&parsed.ptr==value.data()+end;
    };
    std::uint64_t threads{},capabilities{},nonewprivs{},seccomp{};
    return number("\nThreads:",threads)&&threads&&threads<=maximum_threads&&number("\nCapEff:",capabilities,16)&&!capabilities&&
        number("\nNoNewPrivs:",nonewprivs)&&nonewprivs==1&&number("\nSeccomp:",seccomp)&&seccomp==2&&::getpid()==1;
}
int main(int argc,char** argv)
{
    if(argc<8||(argc-5)%3||(argc-5)/3>256)return 100;
    auto parse=[](char const* argument,std::uint64_t& output)
    {std::string_view v{argument};auto r=std::from_chars(v.data(),v.data()+v.size(),output);return r.ec==std::errc{}&&r.ptr==v.data()+v.size();};
    w::startup_limits limits;std::uint64_t profile_bytes{};
    if(!parse(argv[1],limits.request_bytes)||!parse(argv[2],limits.response_bytes)||!parse(argv[3],profile_bytes)||!parse(argv[4],limits.thread_count)||
       !limits.request_bytes||limits.request_bytes>4u*1024u*1024u||!limits.response_bytes||limits.response_bytes>4u*1024u*1024u||
       !profile_bytes||profile_bytes>16u*1024u*1024u||!limits.thread_count||limits.thread_count>256)return 101;
    try
    {
        auto personality=::syscall(SYS_personality,0xfffffffful);
        if(personality<0||(static_cast<unsigned long>(personality)&READ_IMPLIES_EXEC))return 123;
        for(int fd{};fd<3;++fd)
        {auto flags=::fcntl(fd,F_GETFL);struct stat info{};if(flags<0||!(flags&O_NONBLOCK)||(flags&O_ACCMODE)!=(fd?O_WRONLY:O_RDONLY)||::fstat(fd,&info)||!S_ISFIFO(info.st_mode))return 127;}
        std::array<code_identity,256> code{};std::size_t code_count{};
        for(int i=5;i<argc;i+=3)if(!executable_identity(argv[i],argv[i+1],argv[i+2],code[code_count++]))return 124;
        x::descriptor profile_fd{::open("/worker/profile",O_RDONLY|O_NOFOLLOW|O_CLOEXEC)};struct stat info{};
        if(!profile_fd||::fstat(profile_fd.get(),&info)||!S_ISREG(info.st_mode)||info.st_size<0||static_cast<std::uint64_t>(info.st_size)!=profile_bytes)return 102;
        s::byte_buffer profile;profile.resize_for_overwrite(static_cast<std::size_t>(profile_bytes));if(!read_exact(profile_fd.get(),profile.writable()))return 103;profile_fd.reset();
        auto library=::dlopen("/worker/backend.so",RTLD_NOW|RTLD_LOCAL);if(!library)return 104;
        auto prepare=reinterpret_cast<w::prepare_function>(::dlsym(library,"uwvm_communication_prepare"));
        auto activate=reinterpret_cast<w::activate_function>(::dlsym(library,"uwvm_communication_activate"));
        auto serve=reinterpret_cast<w::run_function>(::dlsym(library,"uwvm_communication_serve"));
        auto destroy=reinterpret_cast<w::destroy_function>(::dlsym(library,"uwvm_communication_destroy"));
        if(!prepare||!activate||!serve||!destroy)return 105;
        w::prepared_backend backend;if(!prepare(profile.view(),limits,backend)||!notifications(backend))return 106;
        x::descriptor directory{::open("/proc/self/fd",O_RDONLY|O_DIRECTORY|O_CLOEXEC)},status{::open("/proc/self/status",O_RDONLY|O_CLOEXEC)},maps{::open("/proc/self/maps",O_RDONLY|O_CLOEXEC)};
        if(!directory||!status||!maps)return 107;
        x::syscall_filter confined{x::syscall_phase::confined};if(!confined.install())return 108;
        // No thread or descriptor creation can race these checks after TSYNC.
        if(!process_status(status.get(),limits.thread_count)||!notifications(backend)||!descriptor_closure(directory.get(),status.get(),maps.get(),backend))return 109;
        if(!executable_mappings(maps.get(),{code.data(),code_count}))return 125;
        maps.reset();
        if(!write_all(1,w::ready))return 110;
        std::array<std::uint8_t,40> payload{};alignas(cmsghdr) std::array<std::uint8_t,CMSG_SPACE(sizeof(int)*4)> ancillary{};
        iovec vector{payload.data(),payload.size()};msghdr message{};message.msg_iov=&vector;message.msg_iovlen=1;message.msg_control=ancillary.data();message.msg_controllen=ancillary.size();
        ssize_t got;do{got=::recvmsg(3,&message,MSG_CMSG_CLOEXEC);}while(got<0&&errno==EINTR);
        if(got!=static_cast<ssize_t>(payload.size())||(message.msg_flags&(MSG_TRUNC|MSG_CTRUNC)))return 111;
        auto header=CMSG_FIRSTHDR(&message);if(!header||header->cmsg_level!=SOL_SOCKET||header->cmsg_type!=SCM_RIGHTS||header->cmsg_len!=CMSG_LEN(sizeof(int)*4)||CMSG_NXTHDR(&message,header))return 112;
        std::array<w::role_fd,4> pipes{};std::array<struct stat,4> information{};s::wire::reader reader{payload};std::uint64_t count{};if(!reader.u64(count)||count!=4)return 113;
        for(std::size_t i{};i<pipes.size();++i)
        {
            std::uint64_t role{};if(!reader.u64(role)||role!=static_cast<unsigned>(w::roles[i]))return 114;
            pipes[i].role=w::roles[i];std::memcpy(&pipes[i].descriptor,CMSG_DATA(header)+i*sizeof(int),sizeof(int));
            if(pipes[i].descriptor<4||!x::anonymous_pipe(pipes[i].descriptor,w::receiving(pipes[i].role),information[i]))return 115;
            for(std::size_t j{};j<i;++j)if(information[j].st_dev==information[i].st_dev&&information[j].st_ino==information[i].st_ino)return 116;
        }
        if(!reader.done())return 117;directory.reset();status.reset();::close(3);
        x::syscall_filter active{x::syscall_phase::active,pipes,{backend.notifications.data(),backend.notification_count}};
        if(!active.install())return 118;
        if(!activate(backend.context,pipes))return 119;
        if(!write_all(1,w::attached))return 120;
        auto result=serve(backend.context);destroy(backend.context);return result;
    }
    catch(std::bad_alloc const&){return 121;}catch(...){return 122;}
}
