/** @brief Validate private file admission and stage-specific evidence from the actual registered router. */
#include <uwvm2/imported/ros/scene/module_exchange.h>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <sys/wait.h>
#include <sys/resource.h>
namespace s=uwvm2::imported::ros::scene;
namespace x=s::module_exchange;
namespace a=s::module_buffer_audit;
using E=s::error;using O=x::operation;
using bytes=std::vector<std::uint8_t>;
static int open_file(char const* name)
{
    int fd=::open(name,O_WRONLY|O_CREAT|O_EXCL|O_CLOEXEC,0600);assert(fd>=3);return fd;
}
static bytes read_file(char const* name)
{
    std::ifstream in(name,std::ios::binary);assert(in.good());return bytes(std::istreambuf_iterator<char>(in),{});
}
static std::uint64_t num(bytes const& content,std::size_t i,unsigned n)
{
    std::uint64_t ret{};assert(n<=8 && i+n<=content.size());
    for(unsigned j{};j<n;++j)ret|=static_cast<std::uint64_t>(content[i+j])<<(8*j);
    return ret;
}
struct frame {unsigned stage;std::uint64_t generation;std::string env,route;bytes body;};
static std::vector<frame> frames(char const* name,a::stats stats)
{
    auto data=read_file(name);
    bytes magic{'W','R','O','S','C','A','P',1};
    assert(data.size()>=64 && std::equal(magic.begin(),magic.end(),data.begin()));
    std::size_t i=8;std::vector<frame> result;
    while(data.size()-i>56 || data.at(i+4)!=0)
    {
        auto size=num(data,i,4),count=num(data,i+24,4);
        std::size_t n=data.at(i+5),m=data.at(i+6);
        assert(data.at(i+4)>=1&&data.at(i+4)<=4&&data.at(i+7)==0&&size==28+n+m+count&&size<=data.size()-i);
        assert(num(data,i+8,8)==result.size()+1);
        std::size_t p=i+28;
        result.push_back({data[i+4],num(data,i+16,8),std::string(data.begin()+p,data.begin()+p+n),
            std::string(data.begin()+p+n,data.begin()+p+n+m),bytes(data.begin()+p+n+m,data.begin()+i+size)});
        i+=size;
    }
    assert(data.size()-i==56&&num(data,i,4)==56&&num(data,i+8,8)==stats.enqueued&&num(data,i+16,8)==stats.written);
    assert(num(data,i+24,8)==stats.oversize&&num(data,i+32,8)==stats.budget&&num(data,i+40,8)==stats.full&&num(data,i+48,8)==stats.io_errors);
    return result;
}
static void await_record(a::journal& j,std::uint64_t count)
{
    auto end=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    while(j.inspect().written!=count && std::chrono::steady_clock::now()<end)std::this_thread::sleep_for(std::chrono::milliseconds(1));
    assert(j.inspect().written==count);
}
static void descriptor_rejections()
{
    a::journal j;a::limits cap{2,256,16};
    assert(!j.enabled()&&!j.configure(STDOUT_FILENO,cap)&&!j.configure(-1,cap));
    int pipefd[2];assert(!::pipe(pipefd)&&!j.configure(pipefd[1],cap));::close(pipefd[0]);::close(pipefd[1]);
    int fd=open_file("policy.bin");
    assert(!j.configure(fd,{0,256,16})&&!j.configure(fd,{2,20,16})&&!j.configure(fd,{2,256,0}));
    assert(!::fchmod(fd,0640)&&!j.configure(fd,cap));assert(!::fchmod(fd,0600));
    assert(!::link("policy.bin","linked.bin")&&!j.configure(fd,cap));assert(!::unlink("linked.bin"));
    int readwrite=::open("policy.bin",O_RDWR);assert(readwrite>=0&&!j.configure(readwrite,cap));::close(readwrite);
    assert(!::fcntl(fd,F_SETFL,O_APPEND)&&!j.configure(fd,cap));assert(!::fcntl(fd,F_SETFL,0));
    assert(::write(fd,"X",1)==1&&!j.configure(fd,cap));assert(!::ftruncate(fd,0));
    assert(!::unlink("policy.bin")&&!j.configure(fd,cap)&&::fcntl(fd,F_GETFD)>=0);::close(fd);
}
static void limits_and_fork()
{
    std::uint64_t who=7;bytes input{'A','B','C','D'},oversize(5,'X');
    a::journal j;int fd=open_file("count.bin");assert(j.configure(fd,{1,256,4})&&::fcntl(fd,F_GETFD)<0);
    pid_t child=::fork();assert(child>=0);
    if(!child){j.capture("e","r",a::stage::submit,who,input);j.finish();::_exit(0);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
    j.capture("e","r",a::stage::take,who,oversize);j.capture("e","r",a::stage::submit,who,input);
    j.capture("e","r",a::stage::take,who,input);j.finish();auto stats=j.inspect();
    assert(stats.written==1&&stats.enqueued==1&&stats.oversize==1&&stats.budget==1&&!stats.io_errors);
    auto one=frames("count.bin",stats);assert(one.size()==1&&one.front().body==input);
    a::journal b;fd=open_file("bytes.bin");assert(b.configure(fd,{12,98,4}));
    b.capture("e","r",a::stage::submit,who,input);b.capture("e","r",a::stage::submit,who,input);b.finish();
    auto limits=b.inspect();assert(limits.written==1&&limits.budget==1&&read_file("bytes.bin").size()==98);
    assert(frames("bytes.bin",limits).size()==1);
}
static void file_identity()
{
    a::journal j;int original=open_file("original.bin");struct stat owned{};assert(!::fstat(original,&owned));
    assert(j.configure(original,{4,1024,16}));
    int decoy=open_file("decoy.bin");assert(::write(decoy,"12345678",8)==8);
    int retained=-1;
    for(int n=3;n<2048;++n){struct stat info{};if(!::fstat(n,&info)&&info.st_dev==owned.st_dev&&info.st_ino==owned.st_ino){assert(retained==-1);retained=n;}}
    assert(retained>=3&&::dup2(decoy,retained)==retained);::close(decoy);
    bytes secret{'P','R','I','V'};j.capture("e","r",a::stage::respond,7,secret);j.finish();
    assert(j.inspect().io_errors==1&&!j.inspect().written);
    assert(read_file("decoy.bin")==bytes({'1','2','3','4','5','6','7','8'})&&read_file("original.bin").size()==8);
    a::journal k;int mode=open_file("mode.bin");assert(k.configure(mode,{4,1024,16}));
    assert(!::chmod("mode.bin",0644));k.capture("e","r",a::stage::respond,7,secret);k.finish();
    assert(k.inspect().io_errors==1&&!k.inspect().written&&read_file("mode.bin").size()==8);
}
static void competing_owners()
{
    int shared=open_file("contended.bin"),gate[2];assert(!::pipe(gate));pid_t children[2]{};
    for(auto& pid:children)
    {
        pid=::fork();assert(pid>=0);
        if(!pid)
        {
            ::close(gate[1]);char go{};assert(::read(gate[0],&go,1)==1);
            a::journal j;bool claimed=j.configure(shared,{1,256,8});j.finish();::_exit(claimed?0:17);
        }
    }
    ::close(gate[0]);assert(::write(gate[1],"XX",2)==2);::close(gate[1]);::close(shared);
    unsigned winners{};for(auto child:children){int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status));
        int code=WEXITSTATUS(status);assert(code==0||code==17);winners+=code==0;}
    assert(winners==1&&frames("contended.bin",{}).empty());
}
static void file_size_signal()
{
    auto child=::fork();assert(child>=0);
    if(!child)
    {
        ::signal(SIGXFSZ,SIG_DFL);
        rlimit cap{8,8};assert(!::setrlimit(RLIMIT_FSIZE,&cap));
        a::journal journal;assert(journal.configure(open_file("disk-limit.bin"),{1,1024,8}));
        bytes payload{'P'};journal.capture("scope","route",a::stage::take,1,payload);
        journal.finish();assert(journal.inspect().io_errors==1&&read_file("disk-limit.bin").size()==8);
        ::_exit(0);
    }
    int status{};assert(::waitpid(child,&status,0)==child && WIFEXITED(status) && !WEXITSTATUS(status));
}
static bool erase(s::caller,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t) noexcept {return true;}
static void exchange_stage_capture()
{
    int client{},server{};s::caller c{&client,5},w{&server,8};
    x::region wc{w,0,0,8,erase},cc{c,0,0,8,erase};x::router router;
    x::plan plan{"alpha","main","worker","echo","request","response",16,16,1,1000};
    plan.capture_environment="plant";plan.capture_stages=15;assert(router.bind(plan,x::reservation(plan))==E::success);
    s::grant permission;permission.kind=s::purpose::module_call;permission.binding="registered-message";permission.resource="alpha";permission.alias="alpha";
    for(auto name:x::names)permission.actions.emplace(name);
    router.bind_grant(permission);auto& journal=a::output;
    bytes req{1,2,3},response{4,5},denied{'D','E','N','Y'};std::uint64_t token{};
    auto sink=[&](std::uint64_t id,std::span<std::uint8_t const>){token=id;return true;};
    auto no=[](std::uint64_t,std::span<std::uint8_t const>){return false;};
    assert(router.invoke(w,"worker",permission,O::take,0,{},wc,16,0,sink)==E::again);
    assert(!journal.enabled()&&router.invoke(c,"main",permission,O::submit,0,denied,{},16,0,no)==E::invalid);
    assert(journal.configure(open_file("exchange.bin"),{64,65536,64}));
    assert(router.invoke(w,"worker",permission,O::submit,0,denied,{},16,0,sink)==E::denied);
    assert(router.invoke(c,"main",permission,O::submit,0,denied,{},16,0,no)==E::invalid);
    assert(router.invoke(c,"main",permission,O::submit,0,req,{},16,0,sink)==E::success);await_record(journal,1);
    auto id=token;assert(router.invoke(w,"worker",permission,O::take,0,{},wc,1,0,sink)==E::too_large);
    assert(router.invoke(w,"worker",permission,O::take,0,{},cc,16,0,sink)==E::invalid);
    assert(router.invoke(w,"worker",permission,O::take,0,{},wc,16,0,no)==E::invalid);
    assert(router.invoke(w,"worker",permission,O::take,0,{},wc,16,0,sink)==E::success);await_record(journal,2);
    assert(router.invoke(w,"worker",permission,O::respond,id,response,{},16,0,sink)==E::success);await_record(journal,3);
    assert(router.invoke(c,"main",permission,O::result,id,{},cc,16,0,no)==E::invalid);
    assert(router.invoke(c,"main",permission,O::result,id,{},cc,16,0,sink)==E::success);await_record(journal,4);
    assert(router.invoke(c,"main",permission,O::acknowledge,id,{}, {},0,0,sink)==E::success);
    // The second scope shares the very same public route spelling. Only its
    // result stage is authorized, including a result retained after failed receipt.
    x::router second;plan.capture_environment="other";plan.capture_stages=a::bit(a::stage::result);
    assert(second.bind(plan,x::reservation(plan))==E::success);second.bind_grant(permission);
    assert(second.invoke(w,"worker",permission,O::take,0,{},wc,16,0,sink)==E::again);
    assert(second.invoke(c,"main",permission,O::submit,0,denied,{},16,0,sink)==E::success);id=token;
    assert(second.invoke(w,"worker",permission,O::take,0,{},wc,16,0,sink)==E::success);
    assert(second.invoke(w,"worker",permission,O::respond,id,response,{},16,0,no)==E::invalid);
    assert(second.invoke(c,"main",permission,O::result,id,{},cc,16,0,sink)==E::success);await_record(journal,5);
    second.revoke("alpha");assert(second.invoke(c,"main",permission,O::submit,0,denied,{},16,0,sink)==E::denied);
    journal.finish();auto fs=frames("exchange.bin",journal.inspect());assert(fs.size()==5);
    for(unsigned i{};i<5;++i){assert(fs[i].stage==std::min(i+1,4u)&&fs[i].route=="alpha"&&fs[i].env==(i==4?"other":"plant"));
        assert(fs[i].body==(i<2?req:response));assert(fs[i].generation==(i==1||i==2?8u:5u));}
}
int main()
{
    descriptor_rejections();limits_and_fork();file_identity();competing_owners();file_size_signal();exchange_stage_capture();puts("private file, budgets, substitution, fork, four authenticated router stages passed");
}
