/** @file @brief Single-node manifest origins, truncation, rollback and allocator exhaustion. */
#include <uwvm2/imported/ros/scene/communication_worker_configuration.h>
#include <cassert>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
namespace s=uwvm2::imported::ros::scene;
namespace cfg=s::communication_worker_configuration;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
#if defined(UWVM_WORKER_CONFIGURATION_ALLOCATION)
static std::atomic<std::size_t> fail_after{};static std::atomic<bool> injected{};
static bool reject() noexcept{auto n=fail_after.load();while(n){if(fail_after.compare_exchange_weak(n,n-1)){if(n==1){injected=true;return true;}break;}}return false;}
extern "C" void* __real_malloc(std::size_t);extern "C" void* __real_calloc(std::size_t,std::size_t);extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return reject()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t b){return reject()?nullptr:__real_calloc(n,b);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return reject()?nullptr:__real_realloc(p,n);}
#endif
static bytes read_fixture(char const* directory,char const* name)
{
    uwvm2::utils::container::std_string path{directory};path+='/';path+=name;
    int fd=::open(path.c_str(),O_RDONLY|O_CLOEXEC);assert(fd>=0);bytes input;std::array<std::uint8_t,4096> buffer;
    for(;;){auto count=::read(fd,buffer.data(),buffer.size());assert(count>=0);if(!count)break;input.insert(input.end(),buffer.begin(),buffer.begin()+count);assert(input.size()<=16u*1024u*1024u);}
    assert(::close(fd)==0);return input;
}
static void write_u64(bytes& output,std::size_t offset,std::uint64_t value)
{assert(offset+8<=output.size());for(unsigned i{};i<8;++i)output[offset+i]=static_cast<std::uint8_t>(value>>(i*8));}
int main(int argc,char** argv)
{
    assert(argc==2);auto input=read_fixture(argv[1],"worker.bin");
    cfg::specification result;assert(cfg::decode(input,65536,result)==s::error::success&&result.definitions.size()==16&&cfg::origins(result));
    for(std::size_t n{};n<input.size();++n){assert(cfg::decode({input.data(),n},65536,result)!=s::error::success);assert(result.definitions.size()==16&&cfg::origins(result));}
    auto wrong=input;wrong.push_back(0);assert(cfg::decode(wrong,65536,result)==s::error::invalid&&result.definitions.size()==16);
    assert(cfg::decode(input,0,result)==s::error::too_large);assert(cfg::decode(input,input.size()-1,result)==s::error::too_large);
    auto node=std::get_if<s::communication::node_plan>(&result.definitions[1].policy);assert(node);node->context.digest[0]^=1;assert(!cfg::origins(result));
    assert(cfg::decode(input,65536,result)==s::error::success);auto& type=std::get<s::communication::type_plan>(result.definitions[4].policy);type.type_identity="std_msgs/msg/Bool";assert(!cfg::origins(result));
    for(std::size_t index:std::array<std::size_t,6>{2,4,5,6,7,8})
    {
        assert(cfg::decode(input,65536,result)==s::error::success);
        auto& selected=std::get<s::communication::type_plan>(result.definitions[index].policy);
        selected.validate=+[](s::wire::bytes)noexcept{return true;};
        assert(!cfg::origins(result)); // A matching name/profile cannot impersonate the compiled native validator.
    }
    for(auto [name,count]:std::array<std::pair<char const*,std::size_t>,4>{{{"catalogue-0.bin",0},{"catalogue-8.bin",8},{"catalogue-129.bin",129},{"catalogue-1024.bin",1024}}})
    {
        auto catalogue=read_fixture(argv[1],name);cfg::specification loaded;
        assert(cfg::decode(catalogue,16u*1024u*1024u,loaded)==s::error::success);
        assert(loaded.capacity.types==count&&loaded.definitions.size()==count+2&&cfg::origins(loaded));
        std::fill(catalogue.begin(),catalogue.end(),0); // Validators must retain their own descriptions and graphs.
        std::array<std::uint8_t,8> empty_sequence{0,1,0,0,0,0,0,0};
        for(std::size_t i=2;i<loaded.definitions.size();++i)
        {auto const& plan=std::get<s::communication::type_plan>(loaded.definitions[i].policy);assert(plan.validate.bounded()&&plan.validate(empty_sequence));}
    }
    assert(cfg::decode(input,65536,result)==s::error::success);
    auto catalogue=read_fixture(argv[1],"catalogue-8.bin");
    s::wire::reader cursor{catalogue};std::string_view marker;cfg::transport::plan network;
    assert(cursor.text(marker)&&marker==cfg::magic&&cfg::read_network(cursor,network));auto capacity_offset=cursor.position();
    auto const count_offset=capacity_offset+13*8; // Eleven capacities, then the two RPC bounds.
    auto unchanged=[&]{assert(result.definitions.size()==16&&result.capacity.types==7&&cfg::origins(result));};
    auto reject=[&](bytes const& value,s::error expected){assert(cfg::decode(value,16u*1024u*1024u,result)==expected);unchanged();};
    wrong=catalogue;write_u64(wrong,capacity_offset+2*8,7);reject(wrong,s::error::too_large);
    wrong=catalogue;write_u64(wrong,capacity_offset+2*8,cfg::maximum_types+1);reject(wrong,s::error::invalid);
    wrong=catalogue;write_u64(wrong,count_offset,cfg::maximum_definitions+1);reject(wrong,s::error::invalid);
    wrong=catalogue;write_u64(wrong,count_offset,UINT64_MAX);reject(wrong,s::error::invalid);
    // A count inside the implementation ceiling but beyond the deployment's
    // reservation must be rejected without reserving the definitions vector.
    wrong=catalogue;write_u64(wrong,count_offset,11);
#if defined(UWVM_WORKER_CONFIGURATION_ALLOCATION)
    fail_after=1;injected=false;auto early=cfg::decode(wrong,16u*1024u*1024u,result);fail_after=0;
    assert(early==s::error::too_large&&!injected);unchanged();
#endif
    reject(wrong,s::error::too_large);
    reject(read_fixture(argv[1],"catalogue-duplicate.bin"),s::error::conflict);
    // Calculate the serialized definition sum independently from wire. Retained
    // compiled DAGs need additional capacity, even if the entire wire fits.
    cfg::configuration::budget budget;std::uint64_t request{},response{},count{},serialized{};
    assert(cfg::read_capacity(cursor,budget)&&cursor.u64(request)&&cursor.u64(response)&&cursor.u64(count));
    for(std::uint64_t i{};i<count;++i)
    {
        std::uint64_t kind{},provider{},limits{};s::communication::profile profile;
        assert(cursor.u64(kind)&&cursor.u64(provider)&&cfg::configuration::read_profile(cursor,profile)&&cursor.u64(limits));
        for(std::uint64_t j{};j<limits;++j){std::string_view key;std::uint64_t value{};assert(cursor.text(key)&&cursor.u64(value));}
        s::wire::bytes body;assert(cursor.blob(body));serialized+=body.size();
    }
    assert(cursor.done());wrong=catalogue;write_u64(wrong,capacity_offset+9*8,serialized);reject(wrong,s::error::too_large);
    std::size_t failures{};
#if defined(UWVM_WORKER_CONFIGURATION_ALLOCATION)
    bool complete{};
    for(std::size_t point=1;point<512;++point)
    {
        cfg::specification candidate;fail_after=point;injected=false;auto status=cfg::decode(input,65536,candidate);auto failed=injected.load();fail_after=0;
        if(failed){++failures;assert(status==s::error::no_memory&&candidate.definitions.empty());assert(cfg::decode(input,65536,candidate)==s::error::success);}
        else{assert(status==s::error::success);complete=true;break;}
    }
    assert(complete&&failures>=16);
    complete=false;
    for(std::size_t point=1;point<4096;++point)
    {
        // The previously published manifest remains usable on every failure.
        cfg::specification candidate;assert(cfg::decode(input,65536,candidate)==s::error::success);
        fail_after=point;injected=false;auto status=cfg::decode(catalogue,16u*1024u*1024u,candidate);auto failed=injected.load();fail_after=0;
        if(failed){++failures;assert(status==s::error::no_memory&&candidate.definitions.size()==16&&cfg::origins(candidate));}
        else{assert(status==s::error::success&&candidate.definitions.size()==10);complete=true;break;}
    }
    assert(complete);
#endif
    std::printf("passed single-node worker configuration; recoverable allocation failures %zu\n",failures);
}
