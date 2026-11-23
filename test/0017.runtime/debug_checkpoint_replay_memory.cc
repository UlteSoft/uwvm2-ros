// Licensed under APL-2.0 (see LICENSE).
#include <uwvm2/uwvm/debugger/checkpoint_replay_memory.h>
#include <uwvm2/utils/container/standard.h>
#include <array>
#include <cstdio>
#include <thread>
namespace cp=::uwvm2::uwvm::debugger::checkpoint;
static unsigned checks{};
static void require(bool value,char const* reason)
{
    ++checks;
    if(!value){::std::fprintf(stderr,"replay memory check failed: %s\n",reason);::fast_io::fast_terminate();}
}
static cp::state image(::uwvm2::utils::container::std_vector<::std::byte> payload)
{
    cp::state saved{};saved.recording_id[0u]=::std::byte{1u};saved.checkpoint_id=1u;saved.next_logical_thread=2u;
    saved.objects.resize(5u);saved.root_instances={2u};
    saved.objects[0u].kind=cp::object_kind::module;
    saved.objects[0u].bytes={::std::byte{},::std::byte{'a'},::std::byte{'s'},::std::byte{'m'},::std::byte{1u},::std::byte{},::std::byte{},::std::byte{}};
    auto& instance=saved.objects[1u];instance.kind=cp::object_kind::instance;instance.words[2u]=2u;instance.links={1u,3u,4u};
    for(unsigned n:{2u,3u}){auto& memory=saved.objects[n];memory.kind=cp::object_kind::memory;memory.words={32u,1u,1u,1u};}
    auto& event=saved.objects[4u];event.kind=cp::object_kind::event;event.flags=2u;
    event.words={1u,7u,1u,42u,1u,1u,0u,1u};event.links={3u,4u};
    cp::value argument{};argument.low_bits=42u;cp::value result{};result.low_bits=9u;
    event.values={argument,result};event.bytes=::std::move(payload);
    require(cp::validate_graph(saved)==cp::error::none,"typed event fixture");
    return saved;
}
int main()
{
    ::std::array<::std::byte,64u> first{},second{};
    auto changed_first=first,changed_second=second;
    changed_first[1u]=::std::byte{0x21u};changed_first[2u]=::std::byte{0x32u};
    changed_first[62u]=::std::byte{0x43u};changed_first[63u]=::std::byte{0x54u};
    changed_second[0u]=::std::byte{0x65u};changed_second[31u]=::std::byte{0x76u};changed_second[63u]=::std::byte{0x87u};
    ::std::array<cp::replay_memory_image,2u> before{{{3u,first},{4u,second}}};
    ::std::array<cp::replay_memory_image,2u> after{{{3u,changed_first},{4u,changed_second}}};
    ::uwvm2::utils::container::std_vector<::std::byte> wire{};
    require(cp::encode_replay_memory_effect(before,after,wire)==cp::error::none,"encode two independent memories");
    auto saved=image(wire);
    ::std::array<cp::object_id,2u> memories{3u,4u};
    ::std::array<cp::value,1u> arguments{};arguments[0u].low_bits=42u;
    cp::replay_request request{1u,7u,1u,42u,2u,memories,arguments};
    auto identity=[](cp::replay_result const&) noexcept{return cp::error::none;};
    auto no_commit=[](cp::replay_result const&) noexcept{};
    auto reject=[&](::uwvm2::utils::container::std_vector<::std::byte> const& payload)
    {
        auto live_first=first,live_second=second;
        ::std::array<cp::replay_memory_view,2u> views{{{3u,live_first},{4u,live_second}}};
        cp::replay_log log;auto candidate=image(payload);
        require(log.bind(candidate)==cp::error::none,"malformed payload remains bounded opaque DATA");
        unsigned commits{};
        auto result=cp::apply_replay_memory_effect(log,request,views,identity,[&](cp::replay_result const&) noexcept{++commits;});
        require(result.status!=cp::error::none && log.cursor()==0u && commits==0u,"invalid event neither commits nor advances");
        require(live_first==first && live_second==second,"late rejection leaves every earlier write unapplied");
    };
    for(::std::size_t count{};count!=wire.size();++count)reject({wire.begin(),wire.begin()+count});
    for(::std::size_t offset{};offset!=wire.size();++offset)
    {
        auto wrong=wire;wrong[offset]^=::std::byte{0x80u};reject(wrong);
    }
    auto trailing=wire;trailing.push_back(::std::byte{});reject(trailing);
    auto unchanged_output=wire;
    require(cp::encode_replay_memory_effect(before,after,unchanged_output,wire.size()-1u)==cp::error::limit_exceeded &&
        unchanged_output==wire,"explicit quota rejects atomically");
    require(cp::encode_replay_memory_effect(before,after,unchanged_output,wire.size())==cp::error::none &&
        unchanged_output==wire,"exact quota accepted");
    auto duplicate=before;duplicate[1u].memory=3u;
    require(cp::encode_replay_memory_effect(duplicate,after,unchanged_output)==cp::error::invalid_reference &&
        unchanged_output==wire,"duplicate memory identities rejected");
    auto shorter=after;shorter[1u].bytes=shorter[1u].bytes.first(63u);
    require(cp::encode_replay_memory_effect(before,shorter,unchanged_output)==cp::error::replay_diverged &&
        unchanged_output==wire,"extent changes need a separate resource adapter");
    auto live_first=first,live_second=second;
    ::std::array<cp::replay_memory_view,2u> views{{{3u,live_first},{4u,live_second}}};
    cp::replay_log log;require(log.bind(saved)==cp::error::none,"bind actual event");
    live_second[27u]=::std::byte{1u};
    require(cp::apply_replay_memory_effect(log,request,views,identity,no_commit).status==cp::error::replay_diverged &&
        log.cursor()==0u && live_first==first,"unmodified input bytes still participate in dependency matching");
    live_second=second;
    auto aliased=views;aliased[1u].bytes=live_first;
    require(cp::apply_replay_memory_effect(log,request,aliased,identity,no_commit).status==cp::error::invalid_reference &&
        log.cursor()==0u && live_first==first,"two IDs cannot write aliased native storage");
    auto foreign=views;foreign[1u].memory=5u;
    require(cp::apply_replay_memory_effect(log,request,foreign,identity,no_commit).status==cp::error::invalid_reference &&
        live_first==first && live_second==second,"same bytes do not authorize another memory object");
    auto wrong_thread=request;wrong_thread.logical_thread=2u;
    require(cp::apply_replay_memory_effect(log,wrong_thread,views,identity,no_commit).status==cp::error::replay_diverged &&
        log.cursor()==0u,"wrong scheduled logical thread cannot consume effects");
    require(cp::apply_replay_memory_effect(log,request,views,[](cp::replay_result const&) noexcept{return cp::error::incompatible_type;},
        no_commit).status==cp::error::incompatible_type && live_first==first && live_second==second && log.cursor()==0u,
        "typed-result or resource rejection precedes all memory writes");
    try
    {
        (void)cp::apply_replay_memory_effect(log,request,views,[](cp::replay_result const&)->cp::error{throw 7;},no_commit);
        require(false,"throwing preparation");
    }
    catch(int){require(live_first==first && live_second==second && log.cursor()==0u,"allocation/preparation exceptions leave the entire transaction intact");}
    unsigned commits{};::std::uint64_t result_bits{};
    auto replay=cp::apply_replay_memory_effect(log,request,views,[&](cp::replay_result const&)
    {
        require(cp::apply_replay_memory_effect(log,request,views,identity,no_commit).status==cp::error::unavailable_capability,
            "nested application cannot consume an event");
        cp::error concurrent=cp::error::none;
        ::std::thread other([&]{concurrent=cp::apply_replay_memory_effect(log,request,views,identity,no_commit).status;});other.join();
        require(concurrent==cp::error::unavailable_capability,"concurrent adapter cannot interleave the transaction");
        require(log.bind(saved)==cp::error::unavailable_capability,"rebind cannot replace pinned payload during prepare");
        return cp::error::none;
    },[&](cp::replay_result const& matched) noexcept{++commits;result_bits=matched.results[0u].low_bits;});
    require(replay.status==cp::error::none && result_bits==9u && commits==1u && log.exhausted() &&
        live_first==changed_first && live_second==changed_second,"one complete memory and result commit");
    require(cp::apply_replay_memory_effect(log,request,views,identity,no_commit).status==cp::error::replay_diverged &&
        live_first==changed_first,"consumed effects cannot run twice");
    ::uwvm2::utils::container::std_vector<::std::byte> empty;
    require(cp::encode_replay_memory_effect({}, {},empty)==cp::error::none && empty.size()==16u,"zero-memory calls have explicit empty encoding");
    ::uwvm2::utils::container::std_vector<cp::replay_memory_detail::write> writes;
    require(cp::replay_memory_detail::prepare(empty,{},writes)==cp::error::none && writes.empty(),"zero-memory effects validate without pointer arithmetic");
    ::std::array<cp::replay_memory_image,1u> zero{{{1u,{}}}};
    require(cp::encode_replay_memory_effect(zero,zero,empty)==cp::error::none,"empty memory image");
    ::std::array<cp::replay_memory_view,1u> zero_view{{{1u,{}}}};
    require(cp::replay_memory_detail::prepare(empty,zero_view,writes)==cp::error::none,"empty memory pre/post digests");
    auto digest=cp::replay_memory_detail::hash(wire);
    ::std::printf("replay memory transaction PASS checks=%u wire=",checks);
    for(auto byte:digest)::std::printf("%02x",static_cast<unsigned>(byte));
    ::std::printf("\n");
}
