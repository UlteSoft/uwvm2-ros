/** @file
 * @brief Native reference for the complete registered-message carrier lifecycle.
 * @details Uses production exposure, authority, canonical carriers, router,
 * register FP guards and full-region erasure for submit/take/respond/result/ack.
 * Stable native fixture memories replace the Wasm runtime's incarnation lookup,
 * declaration checks, memory-growth pins and MTE; those are not emulated or
 * qualified here. Copies and erasure still use relaxed byte-atomic accesses.
 * This reference is neither unrestricted C++ calls nor native process IPC.
 * Timings exclude setup; all owning production allocation uses fast_io.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/imported/scene/host.h>
#include <atomic>
#include <cassert>
#include <charconv>
#include <chrono>
#include <cstdio>
#include <cstring>

namespace h=uwvm2::uwvm::imported::scene;
namespace s=uwvm2::imported::ros::scene;
using O=s::module_exchange::operation;
using E=s::error;
static std::array<std::array<std::uint8_t,65536>,2> memories{};
static std::array<int,2> identities{};
static std::array<s::handle,2> capabilities{};
static unsigned actor{};
static bool clear_input=true;

namespace uwvm2::runtime::lib
{
    [[gnu::noinline]] bool ros_caller_identity_host_api(ros_caller_identity& out,bool)noexcept
    {
        if(actor>=2)return false;
        if(actor)out={&identities[actor],1,{u8"worker"},false};
        else out={&identities[actor],1,{u8"main"},false};
        return true;
    }
    // Both native test actors have a fixed identity and no cross-module caller ancestry.
    bool ros_observed_caller_identity_host_api(ros_caller_identity& out) noexcept
    {
        return ros_caller_identity_host_api(out, true);
    }
    [[gnu::noinline]] std::uint32_t with_ros_exchange_memory_host_api(ros_exchange_memory_callback callback,void* context)noexcept
    {
        ros_exchange_memory_access access;
        if(!callback||!ros_caller_identity_host_api(access.caller_,true))return 13;
        return callback(context,access);
    }
    [[gnu::noinline]] bool ros_exchange_memory_access::capture(std::size_t memory,std::uint64_t offset,std::size_t size,std::uint64_t& normalized)noexcept
    {
        normalized=0;
        if(actor>=2||memory||offset>65536||size>65536-offset)return false;
        if(!memory_owner_){memory_owner_=memories[actor].data();memory_index_=memory;}
        if(memory_index_!=memory||memory_owner_!=memories[actor].data())return false;
        normalized=offset;return true;
    }
    [[gnu::noinline]] bool ros_exchange_memory_access::copy(std::size_t memory,std::uint64_t offset,void* data,std::size_t size,bool write)noexcept
    {
        if(actor>=2||!memory_owner_||memory!=memory_index_||memory_owner_!=memories[actor].data()||
            offset>65536||size>65536-offset||(size&&!data))return false;
        auto* native=static_cast<std::uint8_t*>(data);auto* start=memories[actor].data()+offset;
        for(std::size_t i{};i<size;++i)
        {
            std::atomic_ref<std::uint8_t> cell{start[i]};
            if(write)cell.store(native[i],std::memory_order_relaxed);
            else native[i]=cell.load(std::memory_order_relaxed);
        }
        return true;
    }
    [[gnu::noinline]] bool ros_exchange_memory_erase_host_api(void const* instance,std::uint64_t generation,std::uint64_t memory,
        std::uint64_t offset,std::uint64_t size,std::uint64_t length_offset,std::uint64_t length_size)noexcept
    {
        if(!instance||generation!=1||memory||offset>65536||size>65536-offset)return false;
        for(unsigned i{};i<2;++i)if(instance==&identities[i])
        {
            for(std::uint64_t n{};n<size;++n)
                std::atomic_ref<std::uint8_t>{memories[i][offset+n]}.store(0,std::memory_order_relaxed);
            if(length_size){if(length_offset>65536||length_size>65536-length_offset)return false;
                for(std::uint64_t n{};n<length_size;++n)std::atomic_ref<std::uint8_t>{memories[i][length_offset+n]}.store(0,std::memory_order_relaxed);}
            return true;
        }
        return false;
    }
    [[gnu::noinline]] bool ros_exchange_memory_access::write_frame(std::size_t memory,std::uint64_t offset,
        void const* prefix,std::size_t prefix_size,void const* payload,std::size_t payload_size)noexcept
    {
        if((prefix_size&&!prefix)||(payload_size&&!payload)||payload_size>SIZE_MAX-prefix_size||
            actor>=2||!memory_owner_||memory!=memory_index_||memory_owner_!=memories[actor].data()||
            offset>65536||prefix_size+payload_size>65536-offset)return false;
        auto* start=memories[actor].data()+offset;
        auto const* header=static_cast<std::uint8_t const*>(prefix);auto const* body=static_cast<std::uint8_t const*>(payload);
        for(std::size_t i{};i!=prefix_size;++i)std::atomic_ref<std::uint8_t>{start[i]}.store(header[i],std::memory_order_relaxed);
        for(std::size_t i{};i!=payload_size;++i)std::atomic_ref<std::uint8_t>{start[prefix_size+i]}.store(body[i],std::memory_order_relaxed);
        return true;
    }
    [[gnu::noinline]] bool ros_exchange_memory_access::read_input(std::size_t memory,std::uint64_t offset,
        void* data,std::size_t size,bool clear_after_copy)noexcept
    {
        if(actor>=2||!memory_owner_||memory!=memory_index_||memory_owner_!=memories[actor].data()||
            offset>65536||size>65536-offset||(size&&!data))return false;
        auto* output=static_cast<std::uint8_t*>(data);auto* start=memories[actor].data()+offset;
        for(std::size_t i{};i!=size;++i)
            output[i]=std::atomic_ref<std::uint8_t>{start[i]}.load(std::memory_order_relaxed);
        if(clear_after_copy)for(std::size_t i{};i!=size;++i)
            std::atomic_ref<std::uint8_t>{start[i]}.store(0,std::memory_order_relaxed);
        return true;
    }
    [[gnu::noinline]] bool ros_exchange_memory_access::erase(std::uint64_t offset,std::uint64_t size)noexcept
    {
        return memory_owner_&&ros_exchange_memory_erase_host_api(caller_.runtime_instance,caller_.generation,memory_index_,offset,size);
    }
}

static std::uint64_t word(std::size_t offset)
{return s::word(s::bytes{memories[actor]}.subspan(offset,8),0);}
static void word(std::size_t offset,std::uint64_t value)
{s::put(std::span{memories[actor]}.subspan(offset,8),0,value);}
static void request(std::uint64_t token,std::uint64_t size=0)
{word(128,token);word(136,size);}

template<O Action>
[[gnu::noinline]] static void invoke(std::size_t input,std::size_t output,std::size_t capacity,std::size_t length,unsigned expected=0)
{
    using wrapper=h::wrapper::native<&h::invoke_module_call<Action>,h::wrapper::module_call_submit_name>;
    static_assert(wrapper::guards_fp_controls);
    typename wrapper::local_imported_function_type call{};
    uwvm2::utils::container::get<0>(call.params)=capabilities[actor];
    uwvm2::utils::container::get<1>(call.params)=0;
    uwvm2::utils::container::get<2>(call.params)=128;
    uwvm2::utils::container::get<3>(call.params)=input;
    uwvm2::utils::container::get<4>(call.params)=output;
    uwvm2::utils::container::get<5>(call.params)=capacity;
    uwvm2::utils::container::get<6>(call.params)=length;
    wrapper::call(call);
    assert(static_cast<unsigned>(uwvm2::utils::container::get<0>(call.res))==expected);
}

static void setup(std::size_t payload)
{
    auto& config=h::configuration;
    assert(config.create("benchmark")==E::success&&config.bind("benchmark","main")==E::success&&config.bind("benchmark","worker")==E::success);
    assert(config.expose("benchmark","capability.open",s::exposure::allow)==E::success);
    for(auto name:s::module_exchange::names)
        assert(config.expose("benchmark",uwvm2::utils::container::std_string{name},s::exposure::allow)==E::success);
    assert(config.module_call_budget("benchmark",65536,1)==E::success);
    assert(config.module_call_plan("benchmark",{"echo","main","worker","echo","request-octets","response-octets",payload,payload,1,60000000000})==E::success);
    assert(config.module_call_input_clear("benchmark","echo",clear_input)==E::success);
    s::grant grant;grant.alias="exchange";grant.resource="echo";grant.kind=s::purpose::module_call;grant.binding="registered-message";
    for(auto name:s::module_exchange::names)grant.actions.emplace(name);
    assert(config.allow("benchmark",grant)==E::success);
    for(unsigned i{};i<2;++i)
    {
        std::shared_ptr<s::broker> broker;
        assert(config.select(i?"worker":"main",{&identities[i],1},broker)==E::success);
        assert(broker->permissions.open({&identities[i],1},"exchange",capabilities[i])==E::success);
    }
    actor=1;request(0);invoke<O::take>(16,8192,payload+16,24000,11);
}

[[gnu::noinline]] static std::uint64_t round_trip(std::size_t payload)
{
    actor=0;request(0,payload);std::memset(memories[0].data()+144,0xa5,payload);word(144,21);
    invoke<O::submit>(payload+16,20000,16,25000);auto token=word(20000);
    actor=1;request(0);invoke<O::take>(16,8192,payload+16,24000);
    request(word(8192),payload);std::memcpy(memories[1].data()+144,memories[1].data()+8208,payload);word(144,word(144)+1);
    invoke<O::respond>(payload+16,20000,16,25000);
    actor=0;request(token);invoke<O::result>(16,8192,payload+16,24000);
    auto value=word(8208);assert(value==22);
    request(token);invoke<O::acknowledge>(16,20000,0,25000);
    return value;
}

/** @brief Check actual carrier/delivery bytes outside the timed region. */
static void check_erasure(std::size_t payload)
{
    for(std::size_t owner{};owner!=memories.size();++owner)
    {
        auto const& memory=memories[owner];
        if(clear_input){for(auto byte:std::span{memory}.subspan(128,payload+16))assert(!byte);}
        else
        {
            assert(s::word(s::bytes{memory}.subspan(128,8),0)!=0);
            assert(s::word(s::bytes{memory}.subspan(136,8),0)==(owner?payload:0));
            assert(s::word(s::bytes{memory}.subspan(144,8),0)==(owner?22:21));
            for(auto byte:std::span{memory}.subspan(152,payload-8))assert(byte==0xa5);
        }
        for(auto byte:std::span{memory}.subspan(8192,payload+16))assert(!byte);
        for(auto byte:std::span{memory}.subspan(24000,8))assert(!byte);
    }
    std::shared_ptr<s::broker> broker;
    assert(h::configuration.select_active_exposed("main","module.call.submit",{&identities[0],1},broker)==E::success);
    uwvm2::utils::container::std_vector<s::module_exchange::inspection> rows;
    assert(broker->module_calls.inspect(rows,1)&&rows.size()==1&&rows[0].occupied==0&&rows[0].quarantined==0);
    assert(rows[0].clear_after_copy==clear_input);
}
static std::uint64_t number(char const* arg)
{
    std::uint64_t value{};auto end=arg+std::strlen(arg);auto result=std::from_chars(arg,end,value);
    assert(result.ec==std::errc{}&&result.ptr==end);return value;
}
int main(int argc,char** argv)
{
    assert(argc==4||argc==5);auto payload=number(argv[1]),iterations=number(argv[2]),batches=number(argv[3]);
    if(argc==5){assert(!std::strcmp(argv[4],"true")||!std::strcmp(argv[4],"false"));clear_input=!std::strcmp(argv[4],"true");}
    assert(payload>=8&&payload<=4096&&iterations&&iterations<=1000000&&batches&&batches<=501);
    setup(static_cast<std::size_t>(payload));std::array<std::uint64_t,503> elapsed{};std::uint64_t checksum{};
    for(unsigned i{};i<1000;++i)checksum+=round_trip(static_cast<std::size_t>(payload));
    check_erasure(static_cast<std::size_t>(payload));auto begin=std::chrono::steady_clock::now();
    for(std::uint64_t batch{};batch<batches;++batch)
    {
        auto start=std::chrono::steady_clock::now();
        for(std::uint64_t i{};i<iterations;++i)checksum+=round_trip(static_cast<std::size_t>(payload));
        elapsed[batch]=static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-start).count());
    }
    elapsed[batches]=static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-begin).count());
    elapsed[batches+1]=checksum;assert(checksum==22*(iterations*batches+1000));check_erasure(static_cast<std::size_t>(payload));
    return std::fwrite(elapsed.data(),8,static_cast<std::size_t>(batches+2),stdout)==batches+2?0:1;
}
