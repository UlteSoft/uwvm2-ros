/** @file @brief All communication definitions, mutation rejection and recoverable ownership. */
#include <uwvm2/imported/ros/scene/communication_configuration.h>
#include <cassert>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
namespace s=uwvm2::imported::ros::scene;
namespace cfg=s::communication_configuration;
namespace wire=s::wire;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
using text=uwvm2::utils::container::std_string;
#if defined(UWVM_COMMUNICATION_CONFIGURATION_ALLOCATION)
static std::atomic<std::size_t> fail_after{};static std::atomic<bool> failed{};
static bool reject() noexcept
{auto n=fail_after.load();while(n){if(fail_after.compare_exchange_weak(n,n-1)){if(n==1){failed=true;return true;}break;}}return false;}
extern "C" void* __real_malloc(std::size_t);extern "C" void* __real_calloc(std::size_t,std::size_t);extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return reject()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t bytes){return reject()?nullptr:__real_calloc(n,bytes);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return reject()?nullptr:__real_realloc(p,n);}
#endif
static bytes read(char const* directory,std::string_view name)
{
    text path{directory};path+='/';path+=name;path+=".bundle";int fd=::open(path.c_str(),O_RDONLY|O_CLOEXEC);assert(fd>=0);
    bytes output;std::array<std::uint8_t,4096> block{};for(;;){auto count=::read(fd,block.data(),block.size());assert(count>=0);if(!count)break;output.insert(output.end(),block.begin(),block.begin()+count);assert(output.size()<65536);}assert(!::close(fd));return output;
}
struct fixture
{
    bytes owned;cfg::family family;cfg::provider provider;s::communication::profile selected;
    uwvm2::utils::container::std_vector<std::pair<text,std::uint64_t>> limits;wire::bytes definition;
    fixture(char const* directory,std::string_view name):owned(read(directory,name))
    {
        wire::reader in{owned};std::uint64_t f{},p{},count{};assert(in.u64(f)&&f<8&&in.u64(p)&&p>0&&p<4&&cfg::read_profile(in,selected)&&in.u64(count)&&count<=cfg::limit_fields.size());
        family=static_cast<cfg::family>(f);provider=static_cast<cfg::provider>(p);
        for(std::uint64_t i{};i<count;++i){text key;std::uint64_t value{};assert(cfg::read_text(in,key)&&in.u64(value));limits.push_back({std::move(key),value});}
        assert(in.blob(definition)&&in.done());
    }
    void configure(cfg::configuration& out,bool last=true)
    {assert(out.profile(family,provider,selected)==s::error::success);for(std::size_t i{};i<limits.size()-(last?0:1);++i)assert(out.limit(limits[i].first,limits[i].second)==s::error::success);}
};
static void validate(fixture& f)
{
    cfg::configuration missing;assert(missing.parse(f.definition)==s::error::denied);f.configure(missing,false);
    assert(missing.parse(f.definition)==s::error::denied);assert(missing.limit(f.limits.back().first,f.limits.back().second)==s::error::success);
    assert(missing.limit(f.limits.back().first,f.limits.back().second)==s::error::conflict);assert(missing.limit("native.path",0)==s::error::invalid);
    auto corrupted=bytes{f.definition.begin(),f.definition.end()};corrupted.back()^=1;assert(missing.parse(corrupted)!=s::error::success);
    for(std::size_t length{};length<f.definition.size();++length)assert(missing.parse(f.definition.first(length))!=s::error::success);
    assert(missing.parse(f.definition)==s::error::success);assert(missing.parse(f.definition)==s::error::conflict);
    cfg::budget maximum{1,1,1,1,1,1,1,2,1ull<<30,65536,65536},total{};assert(missing.accumulate(maximum,total)==s::error::success);
    auto prior=total;cfg::budget zero{};assert(missing.accumulate(zero,total)==s::error::too_large&&total.definition_bytes==prior.definition_bytes);
    cfg::definition out;assert(missing.freeze(out)==s::error::success&&out.kind==f.family&&out.backend==f.provider&&out.policy.index()==static_cast<std::size_t>(f.family));
    assert(missing.freeze(out)==s::error::conflict&&missing.limit(f.limits[0].first,f.limits[0].second)==s::error::conflict);
    if(f.family==cfg::family::type)
    {
        auto const& type=std::get<s::communication::type_plan>(out.policy);
        if(auto schema=type.validate.bounded())
        {
            assert(total.definition_bytes==f.definition.size()+schema->storage_bytes());
            bool cdr=schema->encoding()==s::bounded_message::layout::cdr;
            std::array<std::uint8_t,28> zero{};zero[1]=1;
            auto message=cdr?wire::bytes{zero}:wire::bytes{zero}.subspan(4);
            assert(type.validate(message));
            auto borrowed=type.validate;out={};assert(borrowed(message));
        }
    }
    // No caller/native owner is supplied by configuration; bind mints it later.
    std::visit([&](auto const& value){if constexpr(requires{value.authority;})assert(!value.authority);else assert(!value.policy.authority);},out.policy);
}
int main(int argc,char** argv)
{
    assert(argc==2);std::size_t allocations{};
    for(auto name:std::array<std::string_view,10>{"context","node","type","topic","service","action","graph","event","bounded-cdr","bounded-ros1"})
    {
        fixture f{argv[1],name};validate(f);
#if defined(UWVM_COMMUNICATION_CONFIGURATION_ALLOCATION)
        bool complete{};
        for(std::size_t point=1;point<128;++point)
        {
            cfg::configuration config;f.configure(config);fail_after=point;failed=false;auto result=config.parse(f.definition);auto injected=failed.load();fail_after=0;
            if(injected){++allocations;assert(result==s::error::no_memory);assert(config.parse(f.definition)==s::error::success);}
            else{assert(result==s::error::success);complete=true;break;}
        }
        assert(complete);
#endif
    }
    std::array<std::uint8_t,1> good{1},bad{2};assert(s::communication::codecs::boolean::validate(good)&&!s::communication::codecs::boolean::validate(bad));
    std::array<std::uint8_t,9> nul{};nul[0]=1;assert(!s::communication::codecs::string::validate(nul));
    assert(s::communication::codecs::lookup_feedback::validate({})&&!s::communication::codecs::lookup_feedback::validate(good));
#if defined(UWVM_COMMUNICATION_CONFIGURATION_ALLOCATION)
    assert(allocations>=4);
#endif
    std::printf("passed 8 communication configuration families; bounded layouts 2; allocation failures %zu\n",allocations);
}
