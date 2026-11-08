#include <atomic>
#include <barrier>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <uwvm2/imported/wasi/wasip1/func/path_open.h>
#include <uwvm2/imported/wasi/wasip1/func/path_open_wasm64.h>
#include <uwvm2/imported/wasi/wasip1/func/fd_close.h>
#include <uwvm2/imported/wasi/wasip1/func/fd_renumber.h>

namespace abi = uwvm2::imported::wasi::wasip1::abi;
namespace fd = uwvm2::imported::wasi::wasip1::fd_manager;
namespace fn = uwvm2::imported::wasi::wasip1::func;
namespace wm = uwvm2::imported::wasi::wasip1::memory;
using error = abi::errno_t;
static void require(bool condition,char const* message)
{ if(!condition) { std::fprintf(stderr,"FAIL: %s\n",message); std::abort(); } }

int main(int argc,char** argv)
{
    require(argc==2,"one existing scratch directory argument");
    using memory = uwvm2::object::memory::linear::native_memory_t;
    memory mem{}; mem.init_by_page_count(1u,1u);
    uwvm2::imported::wasi::wasip1::environment::wasip1_environment<memory> env{};
    env.wasip1_memory=&mem; env.interface_enabled=true; env.fd_storage.fd_limit=4u;
    namespace interfaces = uwvm2::utils::control::host_interfaces;
    auto allow=[&](uwvm2::utils::container::u8string_view name)
    { require(env.interfaces.set(interfaces::find(interfaces::family::wasip1,name),interfaces::access::allow),"known interface"); };
    allow(u8"path_open");allow(u8"path_open_wasm64");allow(u8"fd_close");allow(u8"fd_renumber");
    auto& parent=*env.fd_storage.opens.emplace_back().fd_p;
    parent.rights_base=static_cast<abi::rights_t>(UINT64_MAX);
    parent.rights_inherit=static_cast<abi::rights_t>(UINT64_MAX);
    parent.wasi_fd.ptr->wasi_fd_storage.reset_type(fd::wasi_fd_type_e::dir);
    auto& root=parent.wasi_fd.ptr->wasi_fd_storage.storage.dir_stack.dir_stack.emplace_back();
    root.ptr->dir_stack.storage.file=fast_io::dir_file{std::string{argv[1]}};
    root.ptr->dir_stack.name=u8"scratch";
    // A canceled close waiting on the actual live FD mutex releases its writer
    // ticket and preserves the exact existing directory resource and rights.
    {
        auto* original=parent.wasi_fd.ptr;
        std::atomic<bool> cancel{};error result{};
        parent.fd_mutex.lock();
        std::thread worker{[&]{result=fn::fd_close_host(env,3,[&]() noexcept{return cancel.load();},[&](auto& selected) noexcept
        {
            for(;;)
            {
                if(cancel.load())return error::ecanceled;
                if(selected.fd_mutex.try_lock())return error::esuccess;
                std::this_thread::yield();
            }
        });}};
        while(!(env.fd_storage.fds_rwlock.bits.load() & uwvm2::utils::mutex::rwlock_writer_mask())) { std::this_thread::yield(); }
        cancel.store(true);worker.join();parent.fd_mutex.unlock();
        require(result==error::ecanceled && parent.wasi_fd.ptr==original && parent.close_pos==SIZE_MAX &&
            env.fd_storage.fds_rwlock.bits.load()==0u,"canceling a blocked close does not detach the live directory");
    }
    auto host=[&](char const* name) { return std::string{argv[1]}+"/"+name; };
    auto exists=[&](char const* name) { return ::access(host(name).c_str(),F_OK)==0; };
    auto write=[&](char const* name)
    {
        int f=::open(host(name).c_str(),O_WRONLY|O_CREAT|O_TRUNC|O_CLOEXEC,0600);
        require(f>=0,"create host fixture");require(::write(f,"preserve",8)==8,"write host fixture");require(::close(f)==0,"close host fixture");
    };
    auto preserved=[&](char const* name)
    {
        int f=::open(host(name).c_str(),O_RDONLY|O_CLOEXEC);require(f>=0,"open existing host fixture");
        char text[9]{};auto n=::read(f,text,sizeof(text));require(::close(f)==0,"close checked fixture");return n==8 && std::memcmp(text,"preserve",8)==0;
    };
    auto prepare=[&](char const* path,unsigned path_offset,unsigned result_offset)
    {
        auto span=wm::checked_span_unlocked(mem,path_offset,std::strlen(path));require(bool(span),"component input range");
        std::memcpy(span.data,path,std::strlen(path));wm::store_basic_wasm_type_to_memory(mem,result_offset,UINT32_MAX);
    };
    auto open=[&](bool wide,char const* path,abi::oflags_t flags,unsigned path_offset=0u,unsigned result_offset=256u)
    {
        if(wide) { return fn::path_open_wasm64(env,3,{},path_offset,std::strlen(path),flags,abi::rights_t::right_fd_write,{}, {},result_offset); }
        return fn::path_open(env,3,{},path_offset,std::strlen(path),flags,abi::rights_t::right_fd_write,{}, {},result_offset);
    };
    for(bool wide:{false,true})
    {
        char const* create=wide?"create64":"create32";prepare(create,0,256);
        require(open(wide,create,abi::oflags_t::o_creat)==error::emfile,"full table rejects create");
        require(!exists(create) && wm::get_basic_wasm_type_from_memory<std::uint32_t>(mem,256u)==UINT32_MAX,"EMFILE has no create/output effect");
        char const* trunc=wide?"trunc64":"trunc32";write(trunc);prepare(trunc,0,256);
        require(open(wide,trunc,abi::oflags_t::o_trunc)==error::emfile,"full table rejects truncation");
        require(preserved(trunc) && wm::get_basic_wasm_type_from_memory<std::uint32_t>(mem,256u)==UINT32_MAX,"EMFILE has no truncate/output effect");
    }
    env.fd_storage.fd_limit=5u;
    for(unsigned attempt{};attempt!=100u;++attempt)
    {
        prepare("absent/leaf",0,256);
        require(open(attempt&1u,"absent/leaf",abi::oflags_t::o_creat)==error::enoent,"failed native path open");
        require(env.fd_storage.opens.size()==5u && env.fd_storage.closes.size()==1u &&
            env.fd_storage.opens.index_unchecked(4u).fd_p->close_pos==0u,"every failure rolls the occupied slot back");
    }
    auto* stable=env.fd_storage.opens.index_unchecked(4u).fd_p;
    {
        fd::fd_slot_reservation pending{};
        require(pending.reserve(env.fd_storage,[]() noexcept{return false;})==error::esuccess,"reserve reusable handle");
        require(pending.descriptor()==4 && stable==env.fd_storage.opens.index_unchecked(4u).fd_p &&
            stable->close_pos==fd::pending_fd_slot && !env.fd_storage.fits_occupied_slot_limit(4u),"pending handle retains node and occupied charge");
        require(fn::fd_renumber_base(env,3,4)==error::ebadf,"reserved handle is not a renumber target");
        require(fn::fd_close(env,4)==error::ebadf,"reserved handle cannot be closed");
        fd::fd_slot_reservation exhausted{};
        require(exhausted.reserve(env.fd_storage,[]() noexcept{return false;})==error::emfile,"pending last slot excludes another reservation");
    }
    require(stable==env.fd_storage.opens.index_unchecked(4u).fd_p && stable->close_pos==0u,"rollback retains the real node and mutex");
    prepare("race0",0,256);prepare("race1",64,264);
    std::barrier gate{3};error results[2]{};
    std::thread first{[&]{gate.arrive_and_wait();results[0]=open(false,"race0",abi::oflags_t::o_creat,0,256);}};
    std::thread second{[&]{gate.arrive_and_wait();results[1]=open(true,"race1",abi::oflags_t::o_creat,64,264);}};
    gate.arrive_and_wait();first.join();second.join();
    require((results[0]==error::esuccess && results[1]==error::emfile)||(results[1]==error::esuccess && results[0]==error::emfile),"concurrent ordinary32/64 calls share the last slot");
    require(exists("race0")!=exists("race1"),"losing concurrent create has no filesystem effect");
    require(stable==env.fd_storage.opens.index_unchecked(4u).fd_p && stable->close_pos==SIZE_MAX,"successful commit retains node/mutex");
    auto* resource=stable->wasi_fd.ptr;require(resource!=nullptr,"committed actual file resource");
    require(fn::fd_close(env,4)==error::esuccess && stable==env.fd_storage.opens.index_unchecked(4u).fd_p,"normal close preserves reusable node");
    require(fn::fd_close(env,4)==error::ebadf,"closed handle cannot be closed twice");
    std::atomic<bool> stop{};std::atomic<bool> acquired{true};
    {
        uwvm2::utils::mutex::rw_fair_shared_guard_t reader{env.fd_storage.fds_rwlock};
        std::thread cancelled{[&]{fd::fd_table_write_guard writer{};acquired.store(writer.acquire(env.fd_storage.fds_rwlock,[&]() noexcept{return stop.load();}));}};
        while(!(env.fd_storage.fds_rwlock.bits.load() & uwvm2::utils::mutex::rwlock_pending_mask())) { std::this_thread::yield(); }
        stop.store(true);cancelled.join();
        require(!acquired.load() && env.fd_storage.fds_rwlock.bits.load()==uwvm2::utils::mutex::rwlock_reader_mask(),"cancellation releases only its own pending ticket");
    }
    require(env.fd_storage.fds_rwlock.bits.load()==0u,"reader survives canceled writer then unlocks");
    {
        fd::fd_table_write_guard owner{};require(owner.acquire(env.fd_storage.fds_rwlock,[]() noexcept{return false;}),"writer control owner");
        fd::fd_table_write_guard cancelled{};require(!cancelled.acquire(env.fd_storage.fds_rwlock,[]() noexcept{return true;}),"cancellation before owning ticket");
        require(env.fd_storage.fds_rwlock.bits.load()==(uwvm2::utils::mutex::rwlock_writer_mask()|uwvm2::utils::mutex::rwlock_pending_mask()),"canceled contender cannot clear another owner's ticket");
    }
    std::puts("PASS ordinary32/64 EMFILE create/truncate, rollback, fixed-node commit, last-slot race and canceled fair writer");
}
