#include <uwvm2/runtime/wasm_threads/execution_archive.h>
#include <cstdio>
#include <cstdlib>
namespace t=uwvm2::runtime::wasm_threads;
namespace ar=uwvm2::runtime::diagnostic::snapshot_archive;
static unsigned checks;
#define CHECK(c) do{++checks;if(!(c)){std::fprintf(stderr,"line %u: %s\n",unsigned(__LINE__),#c);std::abort();}}while(false)
int main()
{
    t::threaded_module_image source;source.identifier=0;
    source.name={std::byte{'m'}};
    source.wasm={std::byte{},std::byte{'a'},std::byte{'s'},std::byte{'m'},std::byte{1},std::byte{},std::byte{},std::byte{}};
    std::array modules{source};std::array<std::byte,32> digest{};
    CHECK(t::threaded_source_digest(modules,digest));
    auto first=source,second=source;first.name={std::byte{'a'}};second.name={std::byte{'b'}};second.identifier=1u;
    std::array ordered{first,second};std::array<std::byte,32> canonical_digest{};
    CHECK(t::threaded_source_digest(ordered,canonical_digest));
    std::swap(ordered[0],ordered[1]);CHECK(!t::threaded_source_digest(ordered,canonical_digest));
    ordered[0].identifier=0u;ordered[1].identifier=1u;CHECK(!t::threaded_source_digest(ordered,canonical_digest));
    ordered={first,second};ordered[1].identifier=9u;CHECK(!t::threaded_source_digest(ordered,canonical_digest));
    t::threaded_execution_history history;history.terminal=true;
    auto add=[&](t::journal_kind kind)->t::journal_event&
    {auto& e=history.events.emplace_back();e.kind=kind;e.worker=1;return e;};
    auto& origin=add(t::journal_kind::origin);origin.input.resize(64);std::copy(digest.begin(),digest.end(),origin.input.begin()+32);
    add(t::journal_kind::enter);
    add(t::journal_kind::instruction);
    add(t::journal_kind::host_begin).c=4;
    auto& memory=add(t::journal_kind::memory_write);memory.c=1;memory.output={std::byte{1},std::byte{},std::byte{42}};
    auto& result=add(t::journal_kind::host_end);result.a=4;result.output.resize(4);
    add(t::journal_kind::leave);
    CHECK(t::valid_execution_history(history));
    uwvm2::utils::container::std_vector<std::byte> bytes;
    CHECK(t::encode_threaded_snapshot(history,modules,bytes));
    t::threaded_snapshot decoded;CHECK(t::decode_threaded_snapshot(bytes,decoded));
    CHECK(decoded.history.terminal && decoded.modules.size()==1 && decoded.modules[0].wasm==source.wasm);
    auto foreign_receiver=history;foreign_receiver.events[3].b=2u;
    CHECK(t::valid_execution_history(foreign_receiver));
    auto stable_bytes=bytes;
    CHECK(!t::encode_threaded_snapshot(foreign_receiver,modules,bytes));
    CHECK(bytes==stable_bytes);
    auto original_count=decoded.history.events.size();
    for(std::size_t n{};n<bytes.size();++n)
    {CHECK(!t::decode_threaded_snapshot({bytes.data(),n},decoded));CHECK(decoded.history.events.size()==original_count);}
    for(std::size_t n{};n<bytes.size();++n)
    {auto corrupt=bytes;corrupt[n]^=std::byte{1};CHECK(!t::decode_threaded_snapshot(corrupt,decoded));}
    for(unsigned test{};test<8;++test)
    {
        auto invalid=history;
        switch(test)
        {
        case 0:invalid.events[3].worker=0;break;
        case 1:invalid.events[1].kind=t::journal_kind::host_end;break;
        case 2:invalid.events[4].c=UINT64_MAX;break;
        case 3:invalid.events[4].output[0]=std::byte{2};break;
        case 4:invalid.events[5].a=9;break;
        case 5:invalid.retained_native=true;break;
        case 6:invalid.replay_until=invalid.events.size()+1;break;
        case 7:invalid.events[2].a=1;break;
        }
        CHECK(!t::valid_execution_history(invalid));
    }
    // A recomputed outer checksum cannot substitute a contradictory original
    // module for the one authenticated by the execution origin.
    ar::decoded envelope;CHECK(bool(ar::decode(bytes,envelope)));
    auto changed=source.wasm;changed.back()=std::byte{1};
    envelope.records[0].payload=changed;
    uwvm2::utils::container::std_vector<std::byte> forged;
    auto writer=[](void* object,std::span<std::byte const> value) noexcept
    {try{auto& target=*static_cast<decltype(forged)*>(object);target.insert(target.end(),value.begin(),value.end());return true;}catch(...){return false;}};
    CHECK(bool(ar::encode({envelope.origin,envelope.records,envelope.disclosure},&forged,writer)));
    CHECK(!t::decode_threaded_snapshot(forged,decoded));
    // A diagnostic export cannot carry a full replay journal.
    forged.clear();CHECK(!bool(ar::encode({envelope.origin,envelope.records,ar::visibility::diagnostic},&forged,writer)));
    std::printf("PASS %u checked threaded archive refusals and roundtrips\n",checks);
}
