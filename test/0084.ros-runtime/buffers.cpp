/** @file
 * @brief Exercise real bounded buffer allocations, canonical records and sealed leases.
 */
#include <uwvm2/imported/ros/scene/buffer_contract.h>
#include <atomic>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

namespace b=uwvm2::imported::ros::scene::buffers;
namespace c=uwvm2::imported::ros::buffer_contract;
namespace w=uwvm2::imported::ros::scene::wire;
using b::error;
#if defined(UWVM_BUFFER_FAULT_INJECTION)
// QEMU ARM32 can satisfy a guest mapping inside its pre-reserved arena without
// increasing host RLIMIT_DATA usage. Exercise the same real strict allocator
// null-return path with a test-only linker wrapper, never a production hook.
static bool allocation_fault_armed{};
static unsigned allocation_fault_hits{};
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __wrap_malloc(std::size_t size)
{
    if(allocation_fault_armed&&size>=512ull*1024*1024){++allocation_fault_hits;return nullptr;}
    return __real_malloc(size);
}
#endif
static b::profile schema()
{b::profile value;assert(value.identity.assign("robot.tensor"));value.sha256.fill(0x19);return value;}
static b::plan policy()
{
    b::plan value;value.schema=schema();value.objects=2;value.bytes=128;value.object_bytes=64;value.transfer_bytes=64;
    value.workspace_bytes=64;value.leases=2;value.elements=64;value.rank=4;value.element_mask=(1u<<13)-1;
    value.order_mask=3;value.color_mask=255;return value;
}
static b::layout layout(std::uint64_t bytes=4)
{
    b::layout value;value.schema=schema();value.rank=1;value.shape[0]=bytes;value.strides[0]=1;value.bytes=bytes;return value;
}
static constexpr std::array<std::uint8_t,4> payload{1,2,3,4};
struct fixture
{
    b::registry registry;
    explicit fixture(b::plan plan=policy()){assert(registry.bind("data",plan)==error::success);}
    b::handle create(b::layout format=layout())
    {b::handle id{};assert(registry.create("data",format,8,[&](auto value){id=value;return true;})==error::success);return id;}
    b::handle sealed()
    {
        auto id=create();std::uint64_t accepted{};
        assert(registry.write("data",id,0,payload,accepted)==error::success&&accepted==payload.size());
        assert(registry.seal("data",id,b::hash(payload))==error::success);return id;
    }
    b::usage used(){b::usage value;assert(registry.inspect("data",value)==error::success);return value;}
};

static void default_authority_and_capacity()
{
    b::registry registry;
    assert(registry.create("data",layout(),9,[](auto){assert(false);return true;})==error::denied);
    auto zero=policy();zero.objects=zero.bytes=zero.object_bytes=zero.transfer_bytes=zero.workspace_bytes=zero.leases=zero.elements=0;
    assert(registry.bind("zero",zero)==error::success);
    assert(registry.create("zero",layout(0),9,[](auto){assert(false);return true;})==error::again);
    fixture f;auto first=f.create();auto second=f.create();assert(first!=second);
    assert(f.registry.create("data",layout(),9,[](auto){assert(false);return true;})==error::again);
    assert(f.registry.close("other",first)==error::bad_handle);
    assert(f.registry.close("data",first)==error::success);
    assert(f.registry.create("data",layout(),0,[](auto){assert(false);return true;})==error::again);
    assert(f.registry.bind("data",policy())==error::conflict);
    auto invalid=policy();invalid.color_mask=1ull<<63;assert(!b::valid(invalid));
    invalid=policy();invalid.order_mask=1ull<<63;assert(!b::valid(invalid));
    invalid=policy();invalid.objects=UINT64_MAX;assert(!b::valid(invalid));
    invalid=policy();invalid.transfer_bytes=b::maximum_transfer+1;assert(!b::valid(invalid));
    assert(f.used().objects==1&&f.used().bytes==4);
}

static void checked_layouts()
{
    auto plan=policy();auto value=layout();assert(b::validate(value,plan)==error::success);
    value.schema.sha256[0]^=1;assert(b::validate(value,plan)==error::denied);
    value=layout();value.element=static_cast<b::element_type>(255);assert(b::validate(value,plan)==error::invalid);
    value=layout();value.rank=9;assert(b::validate(value,plan)==error::invalid);
    value=layout();value.shape[0]=UINT64_MAX;assert(b::validate(value,plan)==error::too_large);
    value=layout();value.strides[0]=UINT64_MAX;assert(b::validate(value,plan)==error::too_large);
    value=layout();value.strides[0]=0;assert(b::validate(value,plan)==error::invalid);
    value=layout();value.rank=2;value.shape={2,2};value.strides={1,1};assert(b::validate(value,plan)==error::invalid);
    value.strides={1,3};value.bytes=5;assert(b::validate(value,plan)==error::success); // padded transpose
    value.strides={3,1};assert(b::validate(value,plan)==error::success);
    value.bytes=4;assert(b::validate(value,plan)==error::invalid);
    value=layout(0);assert(b::validate(value,plan)==error::success);
    value.bytes=1;assert(b::validate(value,plan)==error::invalid);
    value=layout();value.rank=0;value.bytes=1;assert(b::validate(value,plan)==error::success);
    value.bytes=0;assert(b::validate(value,plan)==error::invalid);
    value=layout();plan.element_mask=0;assert(b::validate(value,plan)==error::denied);
}

static void publication_rollback()
{
    fixture f;b::handle lost{};b::information unchanged;unchanged.written=77;
    assert(f.registry.create("data",layout(),2,[&](auto id){lost=id;
        assert(f.registry.describe("data",id,unchanged)==error::bad_handle&&unchanged.written==77);
        b::usage pending;assert(f.registry.try_inspect("data",pending)==error::success&&pending.objects==1&&pending.active_handles==0);
        assert(f.registry.close("data",id)==error::bad_handle);return false;})==error::io);
    assert(f.used().bytes==0&&f.used().objects==0);
    assert(f.registry.describe("data",lost,unchanged)==error::bad_handle);
    assert(f.registry.create("data",layout(),2,[](auto){throw std::runtime_error("sink");return true;})==error::io);
    assert(f.used().objects==0);
    assert(f.registry.create("data",layout(),2,[&](auto){f.registry.revoke("data");return true;})==error::cancelled);
    assert(f.used().bytes==0&&f.used().objects==0);
}

static void writes_sealing_and_initialized_reads()
{
    fixture f;auto id=f.create();std::uint64_t accepted=77;b::information info;
    assert(f.registry.read("data",id,0,1,[](auto){assert(false);return true;})==error::too_large);
    assert(f.registry.seal("data",id,b::hash(payload))==error::conflict);
    assert(f.registry.write("data",id,1,payload,accepted)==error::too_large&&accepted==77);
    assert(f.registry.write("data",id,1,std::span{payload}.first(1),accepted)==error::conflict&&accepted==77);
    assert(f.registry.write("data",id,0,std::span{payload}.first(2),accepted)==error::success&&accepted==2);
    assert(f.registry.write("data",id,0,std::span{payload}.first(2),accepted)==error::success); // exact retry does not hash twice
    std::array<std::uint8_t,2> wrong{7,8};accepted=77;
    assert(f.registry.write("data",id,0,wrong,accepted)==error::conflict&&accepted==77);
    assert(f.registry.write("data",id,1,std::span{payload}.subspan(1,2),accepted)==error::conflict);
    assert(f.registry.read("data",id,0,2,[](auto bytes){return bytes[0]==1&&bytes[1]==2;})==error::success);
    assert(f.registry.read("data",id,0,3,[](auto){assert(false);return true;})==error::too_large);
    assert(f.registry.write("data",id,2,std::span{payload}.subspan(2),accepted)==error::success);
    b::digest invalid{};assert(f.registry.seal("data",id,invalid)==error::conflict);
    assert(f.registry.seal("data",id,b::hash(payload))==error::success);
    assert(f.registry.seal("data",id,b::hash(payload))==error::success);
    assert(f.registry.seal("data",id,invalid)==error::conflict);
    accepted=77;assert(f.registry.write("data",id,0,payload,accepted)==error::denied&&accepted==77);
    assert(f.registry.describe("data",id,info)==error::success&&info.sealed&&info.provenance=="guest"&&info.sha256==b::hash(payload));
    assert(f.registry.read("data",id,UINT64_MAX,1,[](auto){assert(false);return true;})==error::too_large);
    auto empty=f.create(layout(0));assert(f.registry.seal("data",empty,b::hash({}))==error::success);
    assert(f.registry.read("data",empty,0,0,[](auto bytes){return bytes.empty();})==error::success);
    constexpr b::digest abc{0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
        0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
    constexpr std::array<std::uint8_t,3> abc_bytes{'a','b','c'};assert(b::hash(abc_bytes)==abc);
}

static void real_sealed_leases_and_retained_quota()
{
    auto plan=policy();plan.objects=1;plan.bytes=plan.object_bytes=4;plan.leases=1;
    fixture f{plan};auto id=f.create();b::registry::sealed_lease lease,extra;
    assert(f.registry.pin("data",id,lease)==error::denied);std::uint64_t written{};
    assert(f.registry.write("data",id,0,payload,written)==error::success);
    assert(f.registry.seal("data",id,b::hash(payload))==error::success);
    assert(f.registry.pin("other",id,lease)==error::bad_handle);
    assert(f.registry.pin("data",id,lease)==error::success&&lease.identity()==id);
    assert(f.registry.pin("data",id,extra)==error::again);
    assert(f.registry.pin("data",id,lease)==error::conflict);
    assert(f.registry.close("data",id)==error::success);
    assert(f.used().bytes==4&&f.used().objects==1&&f.used().leases==1);
    assert(f.used().active_handles==0);
    assert(f.registry.create("data",layout(),1,[](auto){assert(false);return true;})==error::again);
    extra=std::move(lease);assert(!lease&&extra);
    assert(extra.visit([](auto bytes,auto const& format,auto const& hash){return bytes.size()==4&&bytes[3]==4&&format.bytes==4&&hash==b::hash(payload);})==error::success);
    extra={};assert(f.used().bytes==0&&f.used().objects==0&&f.used().leases==0);
    id=f.sealed();assert(f.registry.pin("data",id,extra)==error::success);
    f.registry.revoke("data");assert(extra.visit([](auto,auto const&,auto const&){assert(false);return true;})==error::denied);
    assert(f.used().bytes==4);extra={};assert(f.used().bytes==0&&f.used().objects==0);
    b::registry::sealed_lease survivor;
    {fixture temporary;auto object=temporary.sealed();assert(temporary.registry.pin("data",object,survivor)==error::success);}
    assert(survivor.visit([](auto,auto const&,auto const&){assert(false);return true;})==error::denied);survivor={};
}

static void detached_read_workspace()
{
    auto plan=policy();plan.workspace_bytes=4;fixture f{plan};auto id=f.sealed();
    assert(f.registry.read("data",id,0,4,[&](auto bytes){
        assert(f.used().workspace==4);
        assert(f.registry.read("data",id,0,1,[](auto){assert(false);return true;})==error::again);
        assert(bytes[3]==4);return false;})==error::io);
    assert(f.used().workspace==0);
    assert(f.registry.read("data",id,0,4,[](auto){throw std::runtime_error("sink");return true;})==error::io);
    assert(f.used().workspace==0);
    assert(f.registry.read("data",id,0,4,[&](auto bytes){
        assert(f.registry.close("data",id)==error::success);assert(f.used().bytes==0&&f.used().workspace==4);
        return bytes[0]==1&&bytes[3]==4;})==error::success);
    assert(f.used().workspace==0);
}

static void concurrent_write_close_and_fork()
{
    fixture f;auto id=f.create();std::atomic<bool> go{};error a{},b{};
    std::array<std::uint8_t,4> other{9,8,7,6};
    std::thread first([&]{go.wait(false);std::uint64_t count{};a=f.registry.write("data",id,0,payload,count);});
    std::thread second([&]{go.wait(false);std::uint64_t count{};b=f.registry.write("data",id,0,other,count);});
    go=true;go.notify_all();first.join();second.join();assert((a==error::success&&b==error::conflict)||(b==error::success&&a==error::conflict));
    assert(f.registry.seal("data",id,b::hash(a==error::success?payload:other))==error::success);
    b::registry::sealed_lease lease;assert(f.registry.pin("data",id,lease)==error::success);
    auto child=::fork();assert(child>=0);
    if(!child){auto denied=lease.visit([](auto,auto const&,auto const&){return true;});::_exit(denied==error::denied?0:1);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
    std::thread closer([&]{assert(f.registry.close("data",id)==error::success);});
    assert(lease.visit([](auto bytes,auto const&,auto const&){return bytes.size()==4;})==error::success);
    closer.join();assert(f.used().bytes==4);lease={};assert(f.used().bytes==0);
}

static void canonical_wire()
{
    std::array<std::uint8_t,1024> bytes{};w::writer out{bytes};auto format=layout();
    assert(c::encode_layout(out,format));auto encoded=out.written();assert(encoded.size()<=c::maximum_layout_wire);
    c::request decoded;assert(c::decode(c::operation::create,encoded,decoded)&&decoded.format==format);
    for(std::size_t n{};n<encoded.size();++n)
    {c::request untouched;untouched.buffer=77;assert(!c::decode(c::operation::create,encoded.first(n),untouched)&&untouched.buffer==77);}
    bytes[encoded.size()]=0;assert(!c::decode(c::operation::create,{bytes.data(),encoded.size()+1},decoded));
    out=w::writer{bytes};assert(out.u64(123)&&out.u64(0)&&out.blob(payload));
    assert(c::decode(c::operation::write,out.written(),decoded)&&decoded.buffer==123&&decoded.payload.size()==4);
    out=w::writer{bytes};assert(out.u64(123)&&out.blob(b::hash(payload)));
    assert(c::decode(c::operation::seal,out.written(),decoded)&&decoded.sha256==b::hash(payload));
    out=w::writer{bytes};assert(out.u64(123)&&out.blob(payload));assert(!c::decode(c::operation::seal,out.written(),decoded));
    out=w::writer{bytes};assert(out.u64(123)&&out.u64(1)&&out.u64(2));
    assert(c::decode(c::operation::read,out.written(),decoded)&&decoded.offset==1&&decoded.bytes==2);
    out=w::writer{bytes};assert(out.u64(123));assert(c::decode(c::operation::describe,out.written(),decoded));
    b::information info{format,true,"guest",b::hash(payload),4};auto measure=w::writer::measuring(1024);
    assert(c::encode_description(measure,info));out=w::writer{bytes};assert(c::encode_description(out,info)&&out.size()==measure.size());
    w::reader input{out.written()};b::layout result;bool sealed{};std::string_view origin;
    assert(c::decode_layout(input,result)&&input.boolean(sealed)&&input.text(origin)&&input.done()&&sealed&&origin=="guest");
    auto invalid=format;invalid.element=static_cast<b::element_type>(255);out=w::writer{bytes};assert(!c::encode_layout(out,invalid)&&out.size()==0);
    assert(c::request_bound(c::operation::write,UINT64_MAX)==24+b::maximum_transfer);
}

static void allocation_failure_and_preflight()
{
    fixture f;std::uint64_t bound=77;
    assert(f.registry.transfer_limit("missing",bound)==error::denied&&bound==77);
    assert(f.registry.transfer_limit("data",bound)==error::success&&bound==64&&f.used().objects==0);
    // Force a real strict-allocator failure with a lower child address-space
    // limit. The existing 64 GiB cgroup remains unchanged and still encloses it.
    auto child=::fork();assert(child>=0);
    if(!child)
    {
        b::plan plan=policy();plan.bytes=plan.object_bytes=512ull*1024*1024;plan.elements=plan.bytes;
        fixture limited{plan};
        auto ready=std::getenv("UWVM_BUFFER_FAULT_READY_FD"),proceed=std::getenv("UWVM_BUFFER_FAULT_CONTINUE_FD");
        if(ready&&proceed)
        {
            // The driver lowers the actual host child's RLIMIT_DATA. QEMU may
            // emulate guest RLIMIT_AS without enforcing subsequent mmap calls.
            std::array<std::uint8_t,8> identity{};auto pid=static_cast<std::uint64_t>(::getpid());
            for(unsigned i{};i!=8;++i)identity[i]=static_cast<std::uint8_t>(pid>>(i*8));
            if(::write(std::atoi(ready),identity.data(),identity.size())!=8)::_exit(2);
            std::uint8_t acknowledgement{};
            if(::read(std::atoi(proceed),&acknowledgement,1)!=1||acknowledgement!=1)::_exit(3);
        }
        else
        {
            std::uint64_t pages{};std::ifstream statm{"/proc/self/statm"};statm>>pages;statm.close();
            if(!pages)::_exit(2);
            auto page=::sysconf(_SC_PAGESIZE);if(page<=0)::_exit(3);
            auto maximum=pages*static_cast<std::uint64_t>(page)+8ull*1024*1024;
            struct rlimit cap{static_cast<rlim_t>(maximum),static_cast<rlim_t>(maximum)};
            if(::setrlimit(RLIMIT_AS,&cap))::_exit(4);
        }
        bool failed{};
#if defined(UWVM_BUFFER_FAULT_INJECTION)
        allocation_fault_armed=true;
#endif
        try {static_cast<void>(limited.registry.create("data",layout(plan.bytes),2,[](auto){return true;}));}
        catch(std::bad_alloc const&){failed=true;}
#if defined(UWVM_BUFFER_FAULT_INJECTION)
        allocation_fault_armed=false;if(!allocation_fault_hits)::_exit(6);
#endif
        auto used=limited.used();::_exit(failed&&!used.bytes&&!used.objects?0:5);
    }
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
}

int main()
{
    default_authority_and_capacity();checked_layouts();publication_rollback();writes_sealing_and_initialized_reads();
    real_sealed_leases_and_retained_quota();detached_read_workspace();concurrent_write_close_and_fork();canonical_wire();
    allocation_failure_and_preflight();
    std::cout<<"passed 9 production buffer safety groups\n";
}
