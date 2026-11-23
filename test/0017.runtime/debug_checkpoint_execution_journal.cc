// Licensed under APL-2.0 (see LICENSE).
#include <uwvm2/uwvm/debugger/checkpoint_execution_replay.h>
#include <cstdio>
#include <cstdlib>
#include <array>
namespace cp=::uwvm2::uwvm::debugger::checkpoint;
namespace ex=cp::execution_replay;
static unsigned checks{};
static void require(bool valid,char const* message)
{++checks;if(!valid){::std::fprintf(stderr,"execution journal: %s\n",message);::fast_io::fast_terminate();}}
static cp::value word(::std::uint64_t value,cp::value_kind kind=cp::value_kind::i64)
{cp::value out{};out.type.kind=kind;out.low_bits=value;return out;}
static cp::state origin(bool waiting)
{
    cp::state saved{};saved.recording_id[0]=::std::byte{1};saved.checkpoint_id=1;saved.next_logical_thread=3;saved.root_instances={2};
    saved.objects.resize(waiting?11u:10u);
    auto& module=saved.objects[0];module.kind=cp::object_kind::module;
    module.bytes={::std::byte{},::std::byte{'a'},::std::byte{'s'},::std::byte{'m'},::std::byte{1},::std::byte{},::std::byte{},::std::byte{}};
    auto& instance=saved.objects[1];instance.kind=cp::object_kind::instance;instance.words[0]=1;instance.words[2]=1;instance.links={1,4,3};
    auto& memory=saved.objects[2];memory.kind=cp::object_kind::memory;memory.flags=1;memory.words={32,1,1,1};
    auto& function=saved.objects[3];function.kind=cp::object_kind::function;function.words[1]=1;function.links={2};
    for(unsigned i=0;i<2;++i)
    {
        auto& thread=saved.objects[4+3*i];thread.kind=cp::object_kind::thread;thread.words={i+1u,i};thread.links={6u+3u*i};
        auto& frame=saved.objects[5+3*i];frame.kind=cp::object_kind::frame;frame.words[3]=1;frame.words[5]=1;frame.links={4,7u+3u*i};
        auto& control=saved.objects[6+3*i];control.kind=cp::object_kind::control;control.flags=3;
    }
    if(waiting)
    {
        auto& thread=saved.objects[4];thread.flags=2;thread.links.push_back(11);
        auto& wait=saved.objects[10];wait.kind=cp::object_kind::atomic_wait;wait.links={3};wait.words={0,4,0,UINT64_MAX,0,0,0,32};
    }
    require(cp::validate_graph(saved)==cp::error::none,"complete origin");return saved;
}
static cp::object event(unsigned kind,::std::uint64_t sequence,::std::uint64_t ordinal,::std::uint64_t thread)
{cp::object out{};out.kind=cp::object_kind::event;out.flags=kind;out.words={sequence,ordinal,thread,0,0,0,0,0};return out;}
static cp::object begin(::std::uint64_t sequence,::std::uint64_t ordinal,::std::uint64_t thread,unsigned operation)
{
    auto out=event(ex::wait_begin_kind,sequence,ordinal,thread);out.words[3]=operation;out.words[4]=4;out.words[6]=ex::schema;out.words[7]=32;
    out.links={3};out.values={word(0),word(0),word(operation?0:1),word(operation?UINT64_MAX:0)};
    out.bytes.resize(32u);out.bytes[2]=::std::byte{1};return out;
}
static cp::object end(::std::uint64_t sequence,::std::uint64_t ordinal,::std::uint64_t thread,unsigned operation,::std::uint64_t begin_sequence)
{
    auto out=event(ex::wait_end_kind,sequence,ordinal,thread);out.words[3]=operation;out.words[5]=1;out.words[6]=ex::schema;out.words[7]=begin_sequence;
    out.values={word(operation?0:1,cp::value_kind::i32)};return out;
}
static cp::state journal(bool pending,bool endpoint)
{
    auto saved=origin(pending);::std::uint64_t sequence{},ordinal{};
    auto push=[&](auto item){auto stamp=sequence*100u;for(unsigned n{};n<8u;++n)item.bytes.push_back(static_cast<::std::byte>(stamp>>(8u*n)));saved.objects.push_back(::std::move(item));};
    if(!pending){push(event(ex::instruction_kind,++sequence,++ordinal,1));push(begin(++sequence,ordinal,1,1));}
    push(event(ex::instruction_kind,++sequence,++ordinal,2));
    if(!endpoint)
    {
        auto notify=++sequence;push(begin(notify,ordinal,2,0));
        auto ready=end(++sequence,ordinal,1,1,pending?UINT64_MAX:2);ready.flags=ex::wait_ready_kind;push(::std::move(ready));
        push(end(++sequence,ordinal,2,0,notify));
        push(end(++sequence,ordinal,1,1,pending?UINT64_MAX:2));
    }
    push(event(ex::endpoint_kind,++sequence,ordinal+1,2));
    auto stopped=event(endpoint?ex::wait_endpoint_kind:ex::endpoint_kind,++sequence,ordinal+1,1);
    if(endpoint)
    {stopped.words[4]=9;stopped.links={3};for(auto value: ::std::array<::std::uint64_t,9>{0,4,0,UINT64_MAX,0,0,0,32,0})stopped.values.push_back(word(value));
     stopped.bytes.resize(32u);stopped.bytes[2]=::std::byte{1};}
    push(::std::move(stopped));return saved;
}
static cp::state phased_import(bool reports=false)
{
    auto saved=origin(false);::std::uint64_t sequence{},ordinal{};
    ::std::array<::std::byte,8> before{},after{};after[3]=::std::byte{42};
    auto push=[&](auto item)
    {
        item.words[0]=++sequence;auto stamp=sequence*100u;
        for(unsigned n{};n<8u;++n)item.bytes.push_back(static_cast<::std::byte>(stamp>>(8u*n)));
        saved.objects.push_back(::std::move(item));
    };
    push(event(ex::instruction_kind,0,++ordinal,1));
    auto start=event(ex::import_begin_kind,0,ordinal,1);start.words[6]=ex::schema;start.words[4]=1;start.values={word(17,cp::value_kind::i32)};start.links={3};
    ::std::array<cp::replay_memory_image,1> initial{{{3,before}}},changed{{{3,after}}};
    require(cp::encode_replay_memory_effect(initial,initial,start.bytes)==cp::error::none,"encode import origin");push(::std::move(start));
    push(event(ex::instruction_kind,0,++ordinal,2));
    auto tags=event(ex::tags_kind,0,ordinal,1);tags.words[6]=ex::schema;tags.links={3};tags.bytes.resize(32);push(::std::move(tags));
    if(reports)
    {
        auto report=event(ex::import_memtag_report_kind,0,ordinal,1);
        report.words[4]=10;report.words[6]=ex::schema;report.words[7]=2;
        for(auto value: ::std::array<::std::uint64_t,10>{64,4,0x20000040u,0,0,1,5,2,3,0})
            report.values.push_back(word(value));
        push(::std::move(report));
    }
    auto access=event(ex::import_memory_kind,0,ordinal,1);access.words[4]=4;access.words[5]=1;access.words[6]=ex::schema;access.words[7]=2;
    access.values={word(2),word(0),word(3),word(1),word(1)};access.links={3};
    require(cp::encode_replay_memory_effect(initial,changed,access.bytes)==cp::error::none,"encode actual host write");push(::std::move(access));
    push(event(ex::instruction_kind,0,++ordinal,2));
    auto finish=event(ex::import_finish_kind,0,ordinal,1);finish.words[5]=1;finish.words[6]=ex::schema;finish.words[7]=2;
    finish.values={word(0,cp::value_kind::i32)};push(::std::move(finish));
    push(event(ex::endpoint_kind,0,ordinal+1,1));push(event(ex::endpoint_kind,0,ordinal+1,2));return saved;
}
int main()
{
    if(::std::getenv("UWVM_TEST_LARGE_JOURNAL"))
    {
        auto saved=origin(false);
        constexpr ::std::uint64_t instructions=1048577u;
        saved.objects.reserve(saved.objects.size()+instructions+2u);
        for(::std::uint64_t n=1;n<=instructions;++n)
        {
            auto instruction=event(ex::instruction_kind,n,n,1u);instruction.bytes.resize(8u);
            saved.objects.push_back(::std::move(instruction));
        }
        for(unsigned thread=1;thread<=2;++thread)
        {
            auto last=event(ex::endpoint_kind,instructions+thread,instructions+1u,thread);last.bytes.resize(8u);
            saved.objects.push_back(::std::move(last));
        }
        ex::decoded decoded{};
        auto bounded=cp::limits{};bounded.max_objects=1048576u;
        require(ex::decode(saved,decoded,bounded)==cp::error::limit_exceeded && decoded.events.empty(),
            "explicit object budget rejects transactionally");
        require(ex::decode(saved,decoded)==cp::error::none && decoded.events.size()==instructions+2u,
            "default journal has no million-object ceiling");
        require(cp::limits{}.max_file_bytes>256u*1024u*1024u && cp::limits{}.max_threads>4096u,
            "default budgets are native addressability, not deployment quotas");
        ::std::printf("large journal PASS instructions=%llu events=%zu\n",
            static_cast<unsigned long long>(instructions),decoded.events.size());
        return 0;
    }
    {
        auto saved=phased_import(true);ex::decoded decoded{};
        require(ex::decode(saved,decoded)==cp::error::none && decoded.events.size()==10u,"MTE import report retained between authentic phases");
        auto index=saved.objects.size()-6u;
        require(saved.objects[index].flags==ex::import_memtag_report_kind,"actual report fixture index");
        for(auto field:{2u,3u,7u})
        {
            auto bad=saved;bad.objects[index].words[field]^=1u;
            require(ex::decode(bad,decoded)!=cp::error::none && decoded.events.size()==10u,"report bound to its exact import and worker");
        }
        for(::std::size_t field{};field<10u;++field)
        {
            auto bad=saved;bad.objects[index].values[field].type.kind=cp::value_kind::f64;
            require(ex::decode(bad,decoded)!=cp::error::none,"report uses exact integer DATA");
        }
        for(auto field:{4u,5u,6u,7u,8u,9u})
        {
            auto bad=saved;bad.objects[index].values[field].low_bits=UINT64_MAX-1u;
            require(ex::decode(bad,decoded)!=cp::error::none,"report cannot carry unknown source/tag/boolean metadata");
        }
        auto bad=saved;bad.objects[index].values[7]=bad.objects[index].values[8];
        require(ex::decode(bad,decoded)!=cp::error::none,"matching tags cannot be claimed as a violation");
        bad=saved;bad.objects[index].links={3};
        require(ex::decode(bad,decoded)!=cp::error::none,"diagnostic cannot convey memory mutation authority");
        bad=saved;bad.objects[index].bytes.insert(bad.objects[index].bytes.begin(),::std::byte{1});
        require(ex::decode(bad,decoded)!=cp::error::none,"diagnostic cannot carry native text or payload");
        bad=saved;bad.objects[index].values.pop_back();
        require(ex::decode(bad,decoded)!=cp::error::none,"truncated diagnostic rejected before indexing");
    }

    {
        auto saved=phased_import();ex::decoded decoded{};
        require(ex::decode(saved,decoded)==cp::error::none && decoded.events.size()==9u,"blocking import yields between authentic memory transactions");
        auto const first=saved.objects.size()-9u;
        for(auto n:{1u,4u,6u})
        {
            auto bad=saved;bad.objects[first+n].words[2]=2u;
            require(ex::decode(bad,decoded)!=cp::error::none && decoded.events.size()==9u,"import event cannot borrow another worker");
        }
        for(auto n:{4u,6u})
        {
            auto bad=saved;bad.objects[first+n].words[7]=1u;
            require(ex::decode(bad,decoded)!=cp::error::none,"memory/result phase belongs to its exact import begin");
        }
        auto bad=saved;bad.objects[first+4].values[0].low_bits=3u;
        require(ex::decode(bad,decoded)!=cp::error::none,"host memory access has known operation");
        bad=saved;bad.objects[first+4].values.back().low_bits=2u;
        require(ex::decode(bad,decoded)!=cp::error::none,"host memory result is boolean");
        bad=saved;bad.objects[first+6].flags=ex::import_memory_kind;
        require(ex::decode(bad,decoded)!=cp::error::none,"open import cannot be silently converted to opcode endpoint");
        bad=saved;bad.objects[first+5].words[2]=1u;
        require(ex::decode(bad,decoded)!=cp::error::none,"no guest opcode runs during its unfinished host call");
        bad=saved;bad.objects[first+6].words[2]=2u;
        require(ex::decode(bad,decoded)!=cp::error::none,"return cannot complete another worker import");
    }

    {
        // A wait reached after memory.grow is bounded by the declared maximum,
        // not the smaller memory captured at the beginning of the interval.
        auto grown=journal(false,true);grown.objects[2].words[3]=2u;
        auto& endpoint=grown.objects.back();endpoint.values[0].low_bits=65536u;
        endpoint.values[5].low_bits=65536u;endpoint.bytes[2]=::std::byte{2};endpoint.bytes[10]=::std::byte{1};
        ex::decoded decoded{};
        require(ex::decode(grown,decoded)==cp::error::none,"wait in grown memory at endpoint");
        endpoint.bytes[2]=::std::byte{3};
        require(ex::decode(grown,decoded)!=cp::error::none,"grown wait still respects maximum");
    }
    {
        auto saved=journal(true,false);ex::decoded decoded{};
        auto ready=::std::find_if(saved.objects.begin(),saved.objects.end(),[](auto const& item){return item.kind==cp::object_kind::event && item.flags==ex::wait_ready_kind;});
        ready->values[0].low_bits=2u;
        require(ex::decode(saved,decoded)!=cp::error::none,"completion cannot change its delivered result");
        ready->values[0].low_bits=0u;ready->words[3]=2u;
        require(ex::decode(saved,decoded)!=cp::error::none,"completion retains actual wait width");
        saved=journal(true,true);auto& pending=saved.objects[10];pending.flags=2u;pending.words[3]=0u;pending.words[4]=UINT64_MAX;
        auto& stopped=saved.objects.back();stopped.values[3].low_bits=0u;stopped.values[4].low_bits=UINT64_MAX;stopped.values[8].low_bits=2u;
        require(ex::decode(saved,decoded)==cp::error::none,"already completed timeout can remain parked");
        stopped.values[3].low_bits=1u;
        require(ex::decode(saved,decoded)!=cp::error::none,"completed timeout cannot retain time credit");
        saved=journal(true,true);saved.objects.back().values[8].low_bits=1u;saved.objects.back().values[4].low_bits=UINT64_MAX;
        require(ex::decode(saved,decoded)!=cp::error::none,"pending endpoint cannot invent an unrecorded notification");
        saved=journal(false,true);auto& last=saved.objects.back();
        for(unsigned n{};n<8u;++n)last.bytes[last.bytes.size()-8u+n]=::std::byte{};
        require(ex::decode(saved,decoded)!=cp::error::none,"recorded monotonic time cannot move backward");
    }
    for(bool pending:{false,true})for(bool endpoint:{false,true})
    {
        auto saved=journal(pending,endpoint);ex::decoded decoded{};
        require(ex::decode(saved,decoded)==cp::error::none && decoded.thread_count==2,"concurrent wait timeline");
        auto unchanged=decoded.events.size();auto first=saved.objects.size()-decoded.events.size();
        for(::std::size_t n=first;n<saved.objects.size();++n)
        {
            auto bad=saved;bad.objects[n].words[2]=0;
            require(ex::decode(bad,decoded)!=cp::error::none && decoded.events.size()==unchanged,"bad worker cannot alter output");
            bad=saved;bad.objects[n].words[0]=0;
            require(ex::decode(bad,decoded)!=cp::error::none && decoded.events.size()==unchanged,"invalid order cannot alter output");
            for(::std::size_t k=0;k<8;++k)
            {
                bad=saved;bad.objects[n].words[k]^=UINT64_MAX;
                auto error=ex::decode(bad,decoded);
                require(error==cp::error::none || decoded.events.size()==unchanged,"bounded hostile word decode");
            }
        }
        auto missing=saved;missing.objects.pop_back();
        require(ex::decode(missing,decoded)!=cp::error::none,"all participants need an endpoint or exit");
        if(endpoint)
        {
            for(::std::size_t n=0;n<9;++n)
            {
                auto bad=saved;bad.objects.back().values[n].type.kind=cp::value_kind::f64;
                require(ex::decode(bad,decoded)!=cp::error::none,"wait endpoint has exact numeric types");
            }
            for(auto n:{1u,4u,7u,8u})
            {
                auto bad=saved;bad.objects.back().values[n].low_bits=9999;
                require(ex::decode(bad,decoded)!=cp::error::none,"invalid wait width/rank/address/flags");
            }
        }
        else
        {
            auto bad=saved;bad.objects[bad.objects.size()-3u].words[7]=1;
            require(ex::decode(bad,decoded)!=cp::error::none,"completion cannot consume another instruction");
        }
        for(::std::size_t n=first;n<saved.objects.size();++n)if(saved.objects[n].flags==ex::wait_begin_kind)
        {
            for(auto byte:{0u,8u,23u,24u,25u,31u})
            {
                auto bad=saved;bad.objects[n].bytes[byte]=::std::byte{0xff};
                require(ex::decode(bad,decoded)!=cp::error::none,"invalid wait memory/tag preimage");
            }
            auto bad=saved;bad.objects[n].bytes.pop_back();
            require(ex::decode(bad,decoded)!=cp::error::none,"truncated wait preimage");
        }
    }
    ::std::printf("execution journal PASS checks=%u\n",checks);
}
