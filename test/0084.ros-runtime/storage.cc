/** @file
 * @brief Real-filesystem security checks for independent scene artifact storage.
 * @details Execute only inside the existing 64 GiB / zero-swap test cgroup as
 * an unprivileged UID. Tests use their own private temporary directories and
 * modest bounded tmpfs files; no existing user file or quota is modified.
 */
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE 1
#endif
#include <uwvm2/imported/ros/scene/storage.h>
#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <future>
#include <iostream>
#include <signal.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <thread>

namespace s=uwvm2::imported::ros::scene::storage;
namespace r=uwvm2::imported::ros::scene;
namespace fs=std::filesystem;
static s::principal const alice{1,"recordings"},bob{2,"recordings"};
static std::array<std::byte,8> const payload{std::byte{1},std::byte{2},std::byte{3},std::byte{4},
    std::byte{5},std::byte{6},std::byte{7},std::byte{8}};
static unsigned passed{};

struct directory
{
    std::string path;int fd{-1};
    directory()
    {
        auto base=std::getenv("UWVM_STORAGE_TEST_TMP");assert(base&&*base);
        auto pattern=std::string(base)+"/store-XXXXXX";
        auto raw=::mkdtemp(pattern.data());assert(raw);path=raw;
        fd=::open(path.c_str(),O_RDONLY|O_DIRECTORY|O_NOFOLLOW|O_CLOEXEC);assert(fd>=0);
    }
    ~directory(){if(fd>=0)::close(fd);fs::permissions(path,fs::perms::owner_all);fs::remove_all(path);}
};
static std::string filename(char const* prefix,s::object_id id)
{
    constexpr char hex[]="0123456789abcdef";std::string out(prefix);
    for(unsigned i=64;i;){i-=4;out.push_back(hex[(id>>i)&15]);}return out;
}
static void write_raw(int fd,std::span<std::byte const> bytes)
{assert(::write(fd,bytes.data(),bytes.size())==static_cast<ssize_t>(bytes.size()));}
static void mark(char const* label){++passed;std::cout<<"PASS "<<label<<'\n';}
static std::shared_ptr<s::store> create(directory& root,s::limits limits={32,4,16,8,0})
{std::shared_ptr<s::store> out;assert(s::store::create(root.fd,limits,out)==s::success);assert(out);return out;}

static void admission()
{
    directory root;std::shared_ptr<s::store> out;
    assert(s::store::create(-1,{32,4,16,8,0},out)!=s::success&&!out);
    assert(s::store::create(root.fd,{UINT64_MAX,4,16,8,0},out)==s::invalid);
    assert(s::store::create(root.fd,{32,UINT64_MAX,16,8,0},out)==s::invalid);
    assert(s::store::create(root.fd,{32,4,33,8,0},out)==s::invalid);
    assert(s::store::create(root.fd,{32,4,16,8,UINT64_MAX},out)==s::invalid);
    assert(::chmod(root.path.c_str(),0755)==0);
    assert(s::store::create(root.fd,{32,4,16,8,0},out)==s::denied);
    assert(::chmod(root.path.c_str(),0700)==0);
    int unwanted=::openat(root.fd,"unmanaged",O_CREAT|O_WRONLY|O_EXCL,0600);assert(unwanted>=0);::close(unwanted);
    assert(s::store::create(root.fd,{32,4,16,8,0},out)==s::conflict);
    assert(::unlinkat(root.fd,"unmanaged",0)==0);
    out=create(root);s::object_id id{};
    assert(out->begin({},r::purpose::recording,4,id)==s::denied);
    assert(out->begin(alice,r::purpose::package,4,id)==s::denied);
    assert(out->begin(alice,r::purpose::model_weights,4,id)==s::denied);
    assert(out->begin(alice,static_cast<r::purpose>(255),4,id)==s::denied);
    assert(out->accounting().objects==0&&fs::is_empty(root.path));
    mark("invalid, unbounded, nonprivate, nonempty and wrong-purpose admission");
}

static void transactions()
{
    directory root;auto store=create(root);s::object_id id{},second{};std::size_t count{};
    assert(store->begin(alice,r::purpose::recording,16,id)==s::success);
    assert(store->accounting().reserved_bytes==16&&store->accounting().staged==1);
    assert(store->append(bob,id,payload,count)==s::bad_handle&&count==0);
    assert(store->append(alice,id,payload,count)==s::success&&count==8);
    std::array<std::byte,16> large{};assert(store->append(alice,id,large,count)==s::too_large&&count==0);
    assert(store->read(alice,id,0,large,count)==s::denied);
    assert(store->remove(alice,id)==s::denied);
    std::array<std::byte,8> read{};
    assert(store->validate(alice,id,0,read,count)==s::success&&count==8&&read==payload);
    s::publication publication;
    assert(store->commit(bob,id,publication)==s::bad_handle&&!publication.published);
    assert(store->commit(alice,id,publication)==s::success&&publication.published&&publication.durable);
    assert(store->accounting().reserved_bytes==8&&store->accounting().committed==1);
    assert(store->append(alice,id,payload,count)==s::denied&&count==0);
    assert(store->abort(alice,id)==s::denied);
    assert(store->read(alice,id,0,read,count)==s::success&&count==8&&read==payload);
    assert(store->read(bob,id,0,read,count)==s::bad_handle&&count==0);
    assert(store->read(alice,id,UINT64_MAX,read,count)==s::too_large&&count==0);
    assert(store->read(alice,id,8,read,count)==s::success&&count==0);
    assert(store->read(alice,id,0,large,count)==s::too_large&&count==0);
    assert(store->commit(alice,id,publication)==s::success&&publication.published&&publication.durable);
    assert(store->begin(alice,r::purpose::log,16,second)==s::success);
    assert(store->append(alice,second,payload,count)==s::success&&count==8);
    assert(store->abort(alice,second)==s::success);
    assert(store->accounting().reserved_bytes==8&&store->accounting().objects==1);
    assert(store->remove(alice,id)==s::success);
    assert(store->accounting().reserved_bytes==0&&store->accounting().objects==0&&fs::is_empty(root.path));
    mark("ownership, reservation, contiguous writes, publication, immutable commit and explicit deletion");
}

static void finite_and_shared()
{
    directory root;auto store=create(root,{8,2,8,8,0});std::shared_ptr<s::store> alias;
    assert(s::store::create(root.fd,{8,2,8,8,0},alias)==s::success&&alias==store);
    std::shared_ptr<s::store> other;
    assert(s::store::create(root.fd,{16,2,8,8,0},other)==s::conflict&&!other);
    s::object_id first{},second{},third{};
    assert(store->begin(alice,r::purpose::recording,8,first)==s::success);
    assert(alias->begin(bob,r::purpose::recording,1,second)==s::again);
    assert(alias->begin(bob,r::purpose::recording,0,second)==s::success);
    assert(store->begin(alice,r::purpose::recording,0,third)==s::again);
    assert(store->abort(alice,first)==s::success);
    assert(alias->abort(bob,second)==s::success);
    std::vector<std::future<std::pair<s::result,s::object_id>>> competing;
    for(unsigned i=0;i!=16;++i)
        competing.push_back(std::async(std::launch::async,[&]{s::object_id id{};auto e=store->begin(alice,r::purpose::recording,8,id);return std::pair{e,id};}));
    unsigned successful{};
    for(auto& future:competing){auto [status,id]=future.get();if(status==s::success){++successful;first=id;}else assert(status==s::again);}
    assert(successful==1&&store->accounting().reserved_bytes==8);
    alias.reset();assert(store->accounting().reserved_bytes==8);
    assert(store->abort(alice,first)==s::success);
    mark("shared inode budgets, alias-close retention, zero-byte inode bound and concurrent reservation");
    directory zero_root;auto zero=create(zero_root,{0,0,0,0,0});
    assert(zero->begin(alice,r::purpose::recording,0,first)==s::again&&fs::is_empty(zero_root.path));
    mark("explicit zero capacity forbids creation");
    directory full_root;auto full=create(full_root,{8,1,8,8,UINT64_MAX-1});
    assert(full->begin(alice,r::purpose::recording,8,first)==s::no_space&&fs::is_empty(full_root.path));
    mark("insufficient filesystem headroom rejects before creating a file");
}

static void failed_cleanup()
{
    directory root;auto store=create(root);s::object_id id{};
    assert(store->begin(alice,r::purpose::recording,16,id)==s::success);
    assert(::chmod(root.path.c_str(),0500)==0);
    assert(store->abort(alice,id)==EACCES);
    assert(store->accounting().reserved_bytes==16&&store->accounting().objects==1);
    s::object_snapshot info;assert(store->describe(alice,id,info)==s::success&&info.state==s::phase::staged);
    assert(::chmod(root.path.c_str(),0700)==0);
    assert(store->abort(alice,id)==s::success&&store->accounting().reserved_bytes==0);
    mark("failed unlink retains the staged file and its full capacity charge");
}

static void short_write()
{
    directory root;auto store=create(root);s::object_id id{};std::size_t count{};
    assert(store->begin(alice,r::purpose::recording,8,id)==s::success);
    ::rlimit original{};assert(::getrlimit(RLIMIT_FSIZE,&original)==0);
    ::rlimit limited=original;limited.rlim_cur=4;
    struct sigaction saved{},ignore{};ignore.sa_handler=SIG_IGN;::sigemptyset(&ignore.sa_mask);
    assert(::sigaction(SIGXFSZ,&ignore,&saved)==0);assert(::setrlimit(RLIMIT_FSIZE,&limited)==0);
    assert(store->append(alice,id,payload,count)==s::success&&count==4);
    assert(store->append(alice,id,std::span(payload).first<4>(),count)==EFBIG&&count==0);
    assert(::setrlimit(RLIMIT_FSIZE,&original)==0);assert(::sigaction(SIGXFSZ,&saved,nullptr)==0);
    s::object_snapshot info;assert(store->describe(alice,id,info)==s::success&&info.bytes==4&&info.reserved==8);
    assert(store->append(alice,id,std::span(payload).last<4>(),count)==s::success&&count==4);
    s::publication publication;assert(store->commit(alice,id,publication)==s::success);
    std::array<std::byte,8> read{};assert(store->read(alice,id,0,read,count)==s::success&&count==8&&read==payload);
    assert(store->remove(alice,id)==s::success);
    mark("real kernel short-write accounting and append retry without sparse data");
}

static void publication_collision()
{
    directory root;auto store=create(root);s::object_id id{};std::size_t count{};
    assert(store->begin(alice,r::purpose::recording,8,id)==s::success);
    assert(store->append(alice,id,payload,count)==s::success);
    auto target=filename("artifact-",id);
    int collision=::openat(root.fd,target.c_str(),O_WRONLY|O_CREAT|O_EXCL,0600);assert(collision>=0);
    write_raw(collision,std::span(payload).first<1>());::close(collision);
    s::publication publication;
    assert(store->commit(alice,id,publication)==EEXIST&&!publication.published);
    s::object_snapshot info;assert(store->describe(alice,id,info)==s::success&&info.state==s::phase::staged);
    assert(fs::file_size(root.path+"/"+target)==1);
    assert(store->accounting().reserved_bytes==8);
    assert(::unlinkat(root.fd,target.c_str(),0)==0);
    assert(store->commit(alice,id,publication)==s::success&&publication.published);
    assert(store->remove(alice,id)==s::success);
    mark("atomic publication cannot overwrite an existing artifact name");
}

static void tamper()
{
    {
        directory root;auto store=create(root);s::object_id id{};std::size_t count{};
        assert(store->begin(alice,r::purpose::recording,8,id)==s::success);
        auto staged=filename("staged-",id);
        assert(::linkat(root.fd,staged.c_str(),root.fd,"extra-hardlink",0)==0);
        assert(store->append(alice,id,payload,count)==s::denied&&count==0);
        assert(store->accounting().reserved_bytes==8&&store->abort(alice,id)==s::denied);
        s::object_snapshot info;assert(store->describe(alice,id,info)==s::success&&info.state==s::phase::integrity_failure);
    }
    mark("hardlink drift refuses mutation and retains capacity for trusted reconciliation");
    {
        directory root,outside;auto store=create(root);s::object_id id{};std::size_t count{};
        assert(store->begin(alice,r::purpose::recording,8,id)==s::success);
        auto staged=filename("staged-",id);
        assert(::renameat(root.fd,staged.c_str(),root.fd,"original")==0);
        int target=::openat(outside.fd,"untouched",O_WRONLY|O_CREAT|O_EXCL,0600);assert(target>=0);
        write_raw(target,payload);::close(target);
        auto target_path=outside.path+"/untouched";
        assert(::symlinkat(target_path.c_str(),root.fd,staged.c_str())==0);
        assert(store->append(alice,id,payload,count)==s::denied&&count==0);
        assert(fs::file_size(target_path)==8&&store->accounting().reserved_bytes==8);
    }
    mark("symlink substitution never follows or writes the external target");
    {
        directory root;auto store=create(root);s::object_id id{};std::size_t count{};
        assert(store->begin(alice,r::purpose::recording,8,id)==s::success);
        auto staged=filename("staged-",id);int raw=::openat(root.fd,staged.c_str(),O_WRONLY|O_NOFOLLOW);assert(raw>=0);
        assert(::ftruncate(raw,8192)==0);::close(raw);
        assert(store->append(alice,id,payload,count)==s::denied&&count==0);
        assert(store->accounting().reserved_bytes==8);
    }
    mark("unexpected file growth poisons the object instead of granting sparse-offset access");
}

static void immutable()
{
    int fd=static_cast<int>(::syscall(SYS_memfd_create,"uwvm-immutable-test",MFD_CLOEXEC|MFD_ALLOW_SEALING));assert(fd>=0);
    write_raw(fd,payload);std::shared_ptr<s::immutable_input> input;
    assert(s::immutable_input::create(fd,alice,r::purpose::model_weights,8,8,input)==s::denied&&!input);
    assert(::fcntl(fd,F_ADD_SEALS,F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL)==0);
    assert(::pwrite(fd,payload.data(),1,0)<0&&errno==EPERM);
    assert(::ftruncate(fd,0)<0&&errno==EPERM);
    assert(s::immutable_input::create(fd,alice,r::purpose::recording,8,8,input)==s::invalid);
    assert(s::immutable_input::create(fd,alice,r::purpose::model_weights,7,8,input)==s::too_large);
    assert(s::immutable_input::create(fd,alice,r::purpose::model_weights,8,8,input)==s::success);
    ::close(fd);std::array<std::byte,8> output{};std::size_t count{};
    assert(input->read(alice,0,output,count)==s::success&&count==8&&output==payload);
    assert(input->read(bob,0,output,count)==s::denied&&count==0);
    assert(input->read(alice,UINT64_MAX,output,count)==s::too_large&&count==0);
    std::uint64_t bytes{};r::purpose kind{};
    assert(input->describe(alice,bytes,kind)==s::success&&bytes==8&&kind==r::purpose::model_weights);
    directory root;int normal=::openat(root.fd,"ordinary",O_CREAT|O_EXCL|O_RDWR,0600);assert(normal>=0);
    std::shared_ptr<s::immutable_input> rejected;
    auto ordinary=s::immutable_input::create(normal,alice,r::purpose::configuration,8,8,rejected);
    // tmpfs supports F_GET_SEALS on ordinary files but lacks the required
    // immutable seals; disk files may not implement the seal query at all.
    assert((ordinary==s::unsupported||ordinary==s::denied)&&!rejected);
    ::close(normal);
    mark("immutable model/config inputs require actual kernel seals and exact principal");
}

/** @brief Exercise actual native widths, including a sealed sparse offset above 4 GiB.
 * @details The private memfd contains only two written pages; its logical hole
 * is never copied or read in full. A 32-bit off_t build must reject admission,
 * while a large-file build reads the distinct high-offset marker correctly.
 * This catches narrowing into a low/negative offset without allocating a huge
 * buffer or requiring a multi-gigabyte physical test file.
 */
static void native_extents()
{
    directory root;std::shared_ptr<s::store> out;
    auto file_limit=static_cast<std::uint64_t>(std::numeric_limits<::off_t>::max());
    auto count_limit=static_cast<std::uint64_t>(std::numeric_limits<::ssize_t>::max());
    auto invalid_file=file_limit==static_cast<std::uint64_t>(INT64_MAX)?s::invalid:s::too_large;
    auto invalid_count=count_limit==static_cast<std::uint64_t>(INT64_MAX)?s::invalid:s::too_large;
    assert(s::store::create(root.fd,{file_limit+1,1,file_limit+1,8,0},out)==invalid_file&&!out);
    assert(s::store::create(root.fd,{8,1,8,count_limit+1,0},out)==invalid_count&&!out);
    assert(fs::is_empty(root.path));
    // Aggregate capacity is not a per-file native offset. A collection of
    // small files may safely reserve more than a 32-bit individual extent.
    out=create(root,{static_cast<std::uint64_t>(INT64_MAX),1,8,8,0});
    s::object_id id{};assert(out->begin(alice,r::purpose::cache,8,id)==s::success);
    assert(out->abort(alice,id)==s::success);

    int fd=static_cast<int>(::syscall(SYS_memfd_create,"uwvm-offset-width-test",MFD_CLOEXEC|MFD_ALLOW_SEALING));assert(fd>=0);
    constexpr std::uint64_t high=(std::uint64_t{1}<<32)+4096;
    constexpr std::array<std::byte,8> marker{std::byte{8},std::byte{7},std::byte{6},std::byte{5},
        std::byte{4},std::byte{3},std::byte{2},std::byte{1}};
    assert(::pwrite64(fd,payload.data(),payload.size(),0)==8);
    assert(::pwrite64(fd,marker.data(),marker.size(),static_cast<::off64_t>(high))==8);
    struct ::stat64 stat{};assert(::fstat64(fd,&stat)==0);
    auto page_size=::sysconf(_SC_PAGESIZE);assert(page_size>0);
    assert(static_cast<std::uint64_t>(stat.st_size)==high+8&&stat.st_blocks>=0&&stat.st_blocks<=2*page_size/512);
    assert(::fcntl(fd,F_ADD_SEALS,F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL)==0);
    std::shared_ptr<s::immutable_input> input;
    assert(s::immutable_input::create(fd,alice,r::purpose::package,8,count_limit+1,input)==invalid_count&&!input);
    auto result=s::immutable_input::create(fd,alice,r::purpose::package,high+8,8,input);
    if(file_limit<high+8)
    {
        assert(result==s::too_large&&!input);
        assert(s::immutable_input::create(fd,alice,r::purpose::package,file_limit,8,input)!=s::success&&!input);
    }
    else
    {
        assert(result==s::success&&input);
        std::array<std::byte,8> bytes{};std::size_t count{};
        assert(input->read(alice,high,bytes,count)==s::success&&count==8&&bytes==marker);
        assert(input->read(alice,0,bytes,count)==s::success&&count==8&&bytes==payload);
        assert(input->read(alice,high+8,bytes,count)==s::success&&count==0);
        assert(input->read(alice,high+9,bytes,count)==s::too_large&&count==0);
        assert(input->read(alice,UINT64_MAX,bytes,count)==s::too_large&&count==0);
    }
    ::close(fd);
    std::cout<<"native widths off_t="<<sizeof(::off_t)*8<<" ssize_t="<<sizeof(::ssize_t)*8<<'\n';
    mark("native file/transfer admission prevents offset narrowing and preserves large-file reads");
}

static void lifetimes()
{
    directory root;auto store=create(root);s::object_id id{};std::size_t count{};
    assert(store->begin(alice,r::purpose::recording,8,id)==s::success);
    auto child=::fork();assert(child>=0);
    if(child==0)
    {
        s::object_id other{};std::shared_ptr<s::store> duplicate;
        s::publication publication;s::object_snapshot info;std::vector<s::object_snapshot> view;
        bool refused=store->begin(alice,r::purpose::recording,8,other)==s::denied&&
                     s::store::create(root.fd,{32,4,16,8,0},duplicate)==s::conflict&&
                     store->append(alice,id,payload,count)==s::denied&&
                     store->commit(alice,id,publication)==s::denied&&
                     store->describe(alice,id,info)==s::denied&&!store->inspect(view,8)&&
                     store->abort(alice,id)==s::denied;
        ::_exit(refused?0:1);
    }
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
    assert(store->append(alice,id,payload,count)==s::success);
    s::publication publication;assert(store->commit(alice,id,publication)==s::success);
    std::vector<s::object_snapshot> snapshots;
    assert(!store->inspect(snapshots,0));assert(store->inspect(snapshots,1)&&snapshots.size()==1&&snapshots[0].bytes==8);
    store->retire();s::object_id blocked{};
    assert(store->begin(alice,r::purpose::recording,8,blocked)==s::denied);
    assert(store->accounting().reserved_bytes==8);
    store.reset();std::shared_ptr<s::store> reopened;
    assert(s::store::create(root.fd,{32,4,16,8,0},reopened)==s::conflict&&!reopened);
    assert(fs::file_size(root.path+"/"+filename("artifact-",id))==8);
    mark("fork/retires cannot reuse authority; persistent files block accounting-reset restart");
}

int main()
{
    assert(::geteuid()!=0); // Permission-failure tests must exercise real denial.
    admission();transactions();finite_and_shared();failed_cleanup();short_write();
    publication_collision();tamper();immutable();native_extents();lifetimes();
    std::cout<<"completed "<<passed<<" real filesystem security groups\n";
}
