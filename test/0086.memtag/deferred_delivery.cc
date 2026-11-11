#include <uwvm2/runtime/memtag/deferred.h>
#include <array>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <thread>
#include <vector>
namespace mt=uwvm2::extension::memtag;
namespace delivery=uwvm2::runtime::memtag::deferred;
static void require(bool value) { if(!value) { std::abort(); } }
static delivery::fault evidence(std::uint64_t memory)
{ return {{mt::access_error::tag_mismatch,32,2,0xa5,0xb5},UINT64_C(0xa500000000000020),memory,3,7,19,0}; }

int main(int argc,char** argv)
{
    if(argc==2 && (std::strcmp(argv[1],"fault")==0 || std::strcmp(argv[1],"prior-fault")==0))
    {
        if(std::strcmp(argv[1],"prior-fault")==0)
        {
            // A deferred fault racing an already published terminal record
            // cannot replace that record's selected process exit status.
            uwvm2::runtime::diagnostic::crash::report(
                uwvm2::runtime::diagnostic::crash::category::unreachable,"earlier terminal fault");
        }
        mt::selected_fault_delivery.store(mt::fault_delivery::software_async);
        mt::asynchronous_worker_count.store(4);
        delivery::entry_scope entry;require(entry.enter());
        delivery::set_origin(3,7,19);
        auto value=evidence(2);
        auto result=delivery::defer_if_requested(value.access,value.raw_pointer,value.memory_index);
        // ASYNC permits the access, but the next draining boundary must stop
        // this process with 122. A return of zero would fail the VM contract.
        require(static_cast<bool>(result));
        delivery::poll(true);return 99;
    }
    auto original=evidence(2);
    require(delivery::defer_if_requested(original.access,original.raw_pointer,2).error==mt::access_error::tag_mismatch);
    {
        delivery::pool fallback;
        auto target=std::make_shared<delivery::stream>();
        fallback.submit(target,original);original.access.pointer_tag=0;
        delivery::fault out{};require(delivery::pool::drain(target,out));
        require(out.access.pointer_tag==0xa5 && out.memory_index==2 && out.sequence==0);
        require(!fallback.start(0) && !fallback.start(33));
    }
    std::uint64_t total{};
    for(unsigned count:{1u,2u,4u,8u})
    {
        delivery::pool workers;require(workers.start(count));
        require(workers.start(count));require(!workers.start(count==1 ? 2 : 1));
        auto shared=std::make_shared<delivery::stream>();
        std::array<std::shared_ptr<delivery::stream>,8> streams;
        for(auto& stream:streams) { stream=std::make_shared<delivery::stream>(); }
        std::vector<std::thread> producers;
        for(unsigned thread{};thread!=8;++thread)
            producers.emplace_back([&,thread]
            {
                for(unsigned n{};n!=4096;++n)
                {
                    auto copy=evidence(thread);
                    workers.submit(streams[thread],copy);workers.submit(shared,copy);
                    copy.access.pointer_tag=0;copy.module_id=999;
                }
            });
        for(auto& producer:producers) { producer.join(); }
        delivery::fault out{};
        require(delivery::pool::drain(shared,out));
        require(out.sequence==0 && out.module_id==3 && out.access.pointer_tag==0xa5);
        require(shared->pending==0 && shared->next_sequence==32768);
        for(unsigned thread{};thread!=8;++thread)
        {
            require(delivery::pool::drain(streams[thread],out));
            require(out.sequence==0 && out.memory_index==thread && out.function_index==7 && out.wasm_offset==19);
            require(streams[thread]->pending==0 && streams[thread]->next_sequence==4096);
        }
        total+=65536;
    }
    mt::selected_fault_delivery.store(mt::fault_delivery::software_async);
    {
        delivery::entry_scope outer;require(outer.enter());
        { delivery::entry_scope inner;require(inner.enter());require(delivery::entry_depth==2); }
        require(delivery::entry_depth==1);
        auto result=original.access;result.error=mt::access_error::out_of_bounds;
        require(delivery::defer_if_requested(result,0,0).error==mt::access_error::out_of_bounds);
    }
    require(!delivery::active_stream && delivery::entry_depth==0);
    // Feature decoding is a pure UAPI capability decision, never CPU branding.
    require(!mt::hardware_mte::decode_capabilities(0,3).checked_mappings);
    require(mt::hardware_mte::decode_capabilities(1ul<<18,0).minimum_architectural_version==2);
    require(mt::hardware_mte::decode_capabilities((1ul<<18)|(1ul<<22),0).minimum_architectural_version==3);
    auto mte4=mt::hardware_mte::decode_capabilities(1ul<<18,3);
    require(mte4.minimum_architectural_version==4 && mte4.tagged_fault_address && mte4.store_only_checking);
    std::printf("PASS: %llu copied fault records; 1/2/4/8 workers, 8 concurrent producers; scoped lifecycle and capability checks\n",
        static_cast<unsigned long long>(total));
}
