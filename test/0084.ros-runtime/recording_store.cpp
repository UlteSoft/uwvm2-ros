/** @file @brief Real OOM, exact recording retry, whole batches, immutable pins and replay cancellation. */
#include <uwvm2/imported/ros/scene/recording_store.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <barrier>
#include <sys/wait.h>
namespace r=uwvm2::imported::ros::scene::recording;
namespace m=r::mcap;
namespace b=uwvm2::imported::ros::scene::buffers;
using r::error;
static constexpr r::owner owner{1,2,3};
static thread_local std::size_t fail_after{},allocations{};
static bool fail() noexcept{++allocations;return fail_after&&!--fail_after;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return fail()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t s){return fail()?nullptr:__real_calloc(n,s);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return fail()?nullptr:__real_realloc(p,n);}
static bool boolean(m::bytes value) noexcept{return value.size()==1&&value[0]<=1;}
static std::array<m::channel,2> channels()
{
    m::channel a{1,1,"/state","std_msgs/Bool","ros1msg","ros1","",m::raw("bool data\n"),boolean,1};
    auto b=a;b.id=2;b.topic="/second";return {a,b};
}
static r::plan plan(std::uint64_t segments=2)
{
    r::plan p{segments,1024,4,2,512,3,0};std::uint64_t defs{};
    assert(r::store::required_storage(p,"ros1",channels(),p.reserved_bytes,defs));return p;
}
static void configure(r::store& s,r::plan p=plan()){assert(s.configure(owner,p,"ros1",channels())==error::success);}
static r::handle open(r::store& s){r::handle id{};assert(s.open(owner,id)==error::success&&id);return id;}
static r::usage inspect(r::store& s){r::usage u;assert(s.inspect(owner,u)==error::success);return u;}
static constexpr std::array<std::uint8_t,1> yes{1},no{0},bad{2};
static r::item item(std::uint32_t seq=1,m::bytes payload=yes)
{return {{1,seq,1000-seq,10+seq,payload},4,5,6};}
static r::receipt append(r::store& s,r::handle id,std::uint64_t key,r::item v=item())
{r::receipt out;assert(s.append(owner,id,key,{&v,1},out)==error::success);return out;}
static r::receipt checkpoint(r::store& s,r::handle id,std::uint64_t key)
{r::receipt out;assert(s.checkpoint(owner,id,key,out)==error::success);return out;}
static r::handle replay(r::store& s,r::object_info const& object)
{r::handle id{};assert(s.replay_open(owner,{&object,1},id)==error::success);return id;}
static void configuration_and_owner()
{
    r::store s;r::handle out=91;r::receipt receipt;auto value=item();
    assert(s.open(owner,out)==error::denied&&out==91);
    assert(s.configure(owner,{},"ros1",channels())==error::invalid);
    auto p=plan();--p.reserved_bytes;assert(s.configure(owner,p,"ros1",channels())==error::too_large);++p.reserved_bytes;
    for(auto limit:std::array<std::uint64_t,3>{0,4097,UINT64_MAX}){auto invalid=p;invalid.segments=limit;std::uint64_t total=71,defs=17;assert(!r::store::required_storage(invalid,"ros1",channels(),total,defs)&&total==71&&defs==17);}
    p.segment_bytes=50;allocations=0;fail_after=1;assert(s.configure(owner,p,"ros1",channels())==error::invalid);fail_after=0;assert(!allocations);
    assert(s.configure({},plan(),"ros1",channels())==error::invalid);configure(s);
    assert(s.configure(owner,plan(),"ros1",channels())==error::denied);
    for(auto other:{r::owner{2,2,3},r::owner{1,3,3},r::owner{1,2,4}})
    {
        assert(s.open(other,out)==error::denied&&out==91);
        assert(s.append(other,1,1,{&value,1},receipt)==error::denied);
        assert(s.retire(other)==error::denied);
    }
    auto id=open(s);assert(s.open(owner,out)==error::again&&out==91&&inspect(s).recording);
    assert(s.close_writer(owner,id)==error::success&&!inspect(s).recording);
}
static void atomic_batches_and_retry()
{
    r::store s;configure(s);auto id=open(s);r::receipt output{77},unchanged=output;
    std::array batch{item(1),item(2,bad)};
    assert(s.append(owner,id,1,batch,output)==error::invalid&&output==unchanged&&!inspect(s).active_messages);
    batch[1]=item(2);assert(s.append(owner,id,1,batch,output)==error::success&&output.accepted_messages==2);
    auto accepted=output;assert(s.append(owner,id,1,batch,output)==error::success&&output==accepted&&inspect(s).active_messages==2);
    batch[1].source_generation=7;assert(s.append(owner,id,1,batch,output)==error::conflict);batch[1].source_generation=6;
    batch[1].message.data=no;assert(s.append(owner,id,1,batch,output)==error::conflict);batch[1].message.data=yes;
    assert(s.checkpoint(owner,id,1,output)==error::conflict&&s.append(owner,id,3,batch,output)==error::conflict);
    std::array<r::item,5> excessive;excessive.fill(item());
    assert(s.append(owner,id,2,excessive,output)==error::too_large&&inspect(s).active_messages==2);
    assert(s.append(owner,id,2,batch,output)==error::success&&inspect(s).active_messages==4);
    assert(s.append(owner,id,3,batch,output)==error::too_large&&inspect(s).active_messages==4);
    auto sealed=checkpoint(s,id,3);assert(sealed.object.messages==4&&sealed.object.id&&!inspect(s).active_messages);
    assert(s.checkpoint(owner,id,3,output)==error::success&&output==sealed&&inspect(s).sealed_segments==1);
    assert(s.append(owner,id,3,batch,output)==error::conflict);
}
static void complete_objects_and_owned_schema()
{
    r::store s;auto source=channels();std::array<char,10> definition{'b','o','o','l',' ','d','a','t','a','\n'};
    source[0].definition=source[1].definition={reinterpret_cast<std::uint8_t const*>(definition.data()),definition.size()};
    assert(s.configure(owner,plan(),"ros1",source)==error::success);definition.fill('x');source[0].validate=nullptr;source[0].topic="unapproved";
    auto id=open(s);append(s,id,1,item(UINT32_MAX));auto object=checkpoint(s,id,2).object;
    assert(s.export_object(owner,object,[&](auto const& info,auto bytes)
    {
        assert(info==object&&b::hash(bytes)==object.sha256&&bytes.size()==object.bytes);
        std::array<m::position,4> positions;std::size_t count=0;
        assert(m::index(bytes,"ros1",channels(),{1024,100,4},positions,count)==m::result::success&&count==1);
        assert(positions[0].sequence==UINT32_MAX&&positions[0].size==1&&bytes[positions[0].offset]==1);
        assert(s.erase(owner,object)==error::again&&inspect(s).pins==1);return true;
    })==error::success);
    assert(!inspect(s).pins);
    auto wrong=object;wrong.sha256[0]^=1;bool called{};
    assert(s.export_object(owner,wrong,[&](auto,auto){called=true;return true;})==error::conflict&&!called);
    assert(s.export_object(owner,object,[](auto,auto){return false;})==error::io&&!inspect(s).pins);
    try{s.export_object(owner,object,[](auto,auto)->bool{throw 11;});assert(false);}catch(int n){assert(n==11);}
    assert(!inspect(s).pins);
}
static void capacity_pins_and_stop()
{
    r::store s;configure(s,plan(1));auto id=open(s);append(s,id,1);auto object=checkpoint(s,id,2).object;
    auto p=replay(s,object);r::receipt output;auto value=item(2);
    assert(s.append(owner,id,3,{&value,1},output)==error::again&&s.erase(owner,object)==error::again);
    r::handle unchanged=49;std::array duplicates{object,object};assert(s.replay_open(owner,duplicates,unchanged)==error::invalid&&unchanged==49);
    assert(s.close_replay(owner,p)==error::success&&!inspect(s).pins);
    assert(s.erase(owner,object)==error::success&&s.erase(owner,object)==error::bad_handle);
    append(s,id,3,value);assert(s.stop(owner,id,4,true,output)==error::success&&output.stopped&&output.object.id!=object.id);
    auto stopped=output;assert(s.stop(owner,id,4,true,output)==error::success&&output==stopped);
    assert(s.stop(owner,id,4,false,output)==error::conflict&&s.append(owner,id,5,{&value,1},output)==error::cancelled);
    assert(s.close_writer(owner,id)==error::success&&s.open(owner,unchanged)==error::again&&unchanged==49);
    assert(s.erase(owner,stopped.object)==error::success);auto next=open(s);assert(next!=id);
    append(s,next,1);assert(s.stop(owner,next,2,false,output)==error::success&&output.stopped&&!output.object.id&&!inspect(s).sealed_segments);
    assert(s.close_writer(owner,next)==error::success);next=open(s);auto empty=checkpoint(s,next,1).object;assert(empty.id&&!empty.messages);
}
static void replay_order_and_delivery()
{
    r::store s;configure(s);auto id=open(s);append(s,id,1,item(1));append(s,id,2,item(2,no));auto a=checkpoint(s,id,3).object;
    append(s,id,4,item(3));auto c=checkpoint(s,id,5).object;std::array objects{c,a};r::handle p{};assert(s.replay_open(owner,objects,p)==error::success);
    assert(s.read(owner,p,4,[](auto,auto){assert(false);return true;})==error::too_large);
    auto check=[](auto start,auto records){assert(start==0&&records.size()==3&&records[0].sequence==3&&records[1].sequence==1&&records[2].sequence==2&&records[2].data[0]==0);};
    assert(s.read(owner,p,3,[&](auto start,auto records){check(start,records);return false;})==error::io);
    try{s.read(owner,p,3,[&](auto start,auto records)->bool{check(start,records);throw 7;});assert(false);}catch(int n){assert(n==7);}
    assert(s.read(owner,p,3,[&](auto start,auto records)
    {
        check(start,records);assert(s.read(owner,p,1,[](auto,auto){assert(false);return true;})==error::again);
        assert(s.seek(owner,p,0)==error::again&&s.erase(owner,a)==error::again);return true;
    })==error::success);
    assert(s.read(owner,p,3,[](auto start,auto records){assert(start==3&&records.empty());return true;})==error::success);
    assert(s.seek(owner,p,4)==error::invalid&&s.seek(owner,p,1)==error::success);
    assert(s.read(owner,p,1,[](auto start,auto records){assert(start==1&&records.size()==1&&records[0].sequence==1);return true;})==error::success);
    assert(s.close_replay(owner,p)==error::success&&!inspect(s).pins&&s.seek(owner,p,0)==error::bad_handle);
}
static void concurrent_close_and_retire()
{
    for(bool retire:{false,true})
    {
        r::store s;configure(s);auto id=open(s);append(s,id,1);auto object=checkpoint(s,id,2).object;auto p=replay(s,object);
        std::barrier step{2};error result{};
        std::thread reader{[&]{result=s.read(owner,p,1,[&](auto start,auto records)
        {assert(start==0&&records.size()==1&&records[0].data[0]==1);step.arrive_and_wait();step.arrive_and_wait();assert(records[0].data[0]==1);return true;});}};
        step.arrive_and_wait();assert(inspect(s).pins==1&&s.erase(owner,object)==error::again);
        if(retire)assert(s.retire(owner)==error::success);else assert(s.close_replay(owner,p)==error::success&&inspect(s).pins==1);
        step.arrive_and_wait();reader.join();assert(result==error::cancelled);
        if(retire){assert(inspect(s).pins==1);assert(s.close_replay(owner,p)==error::success);}
        assert(!inspect(s).pins&&!inspect(s).replays);
    }
    r::store s;configure(s);auto id=open(s);append(s,id,1);auto object=checkpoint(s,id,2).object;auto p=replay(s,object);
    try{s.read(owner,p,1,[&](auto,auto)->bool{assert(s.close_replay(owner,p)==error::success);throw 8;});assert(false);}catch(int n){assert(n==8);}
    assert(!inspect(s).pins&&!inspect(s).replays);
    assert(s.export_object(owner,object,[&](auto,auto){assert(s.retire(owner)==error::success);return true;})==error::cancelled&&!inspect(s).pins);
}
static void external_immutable_segments()
{
    std::array<std::uint8_t,1024> bytes{};std::size_t size{};r::object_info original;
    {
        r::store source;configure(source);auto id=open(source);append(source,id,1,item(1));append(source,id,2,item(2,no));original=checkpoint(source,id,3).object;
        assert(source.export_object(owner,original,[&](auto const&,auto data){size=data.size();std::copy(data.begin(),data.end(),bytes.begin());return true;})==error::success);
    }
    r::store destination;configure(destination,plan(1));r::object_info output{71},unchanged=output;
    auto input=m::bytes{bytes.data(),size};auto wrong=original.sha256;wrong[0]^=1;
    assert(destination.import_segment(owner,input,wrong,output)==error::conflict&&output==unchanged&&!inspect(destination).sealed_segments);
    for(std::size_t n{};n<size;++n)
    {
        auto prefix=input.first(n);assert(destination.import_segment(owner,prefix,b::hash(prefix),output)!=error::success&&output==unchanged&&!inspect(destination).sealed_segments);
    }
    assert(destination.import_segment(owner,input,original.sha256,output)==error::success&&output.id!=original.id&&output.messages==2);
    auto imported=output;bytes.fill(0xff);auto p=replay(destination,imported);
    assert(destination.read(owner,p,3,[](auto start,auto messages){assert(!start&&messages.size()==2&&messages[0].data[0]==1&&messages[1].data[0]==0);return true;})==error::success);
    assert(destination.close_replay(owner,p)==error::success&&destination.erase(owner,imported)==error::success);
    assert(destination.replay_open(owner,{&original,1},p)==error::bad_handle);
}
static void real_allocation_failures()
{
    for(std::size_t at=1;at<=9;++at)
    {
        r::store s;allocations=0;fail_after=at;
        auto status=s.configure(owner,plan(),"ros1",channels());
        fail_after=0;assert(status==error::no_memory&&allocations==at);r::usage ignored;assert(s.inspect(owner,ignored)==error::denied);
        configure(s);allocations=0;fail_after=1;
        auto id=open(s);auto result=append(s,id,1);assert(append(s,id,1)==result);auto object=checkpoint(s,id,2).object;auto p=replay(s,object);
        assert(s.read(owner,p,1,[](auto,auto){return false;})==error::io);
        assert(s.read(owner,p,1,[](auto,auto){return true;})==error::success);
        assert(s.seek(owner,p,0)==error::success&&s.close_replay(owner,p)==error::success);
        assert(s.export_object(owner,object,[](auto,auto){return true;})==error::success&&s.erase(owner,object)==error::success);
        r::receipt stopped;assert(s.stop(owner,id,3,false,stopped)==error::success&&s.close_writer(owner,id)==error::success);
        assert(s.retire(owner)==error::success);static_cast<void>(inspect(s));fail_after=0;assert(!allocations);
    }
}
static void fork_does_not_inherit()
{
    r::store s;configure(s);auto id=open(s);append(s,id,1);auto object=checkpoint(s,id,2).object;auto p=replay(s,object);
    auto child=::fork();assert(child>=0);if(!child)
    {
        r::usage usage;r::receipt receipt;auto value=item();r::handle next{};
        bool good=s.inspect(owner,usage)==error::denied&&s.open(owner,next)==error::denied&&s.append(owner,id,3,{&value,1},receipt)==error::denied&&
            s.read(owner,p,1,[](auto,auto){return true;})==error::denied&&s.erase(owner,object)==error::denied&&s.retire(owner)==error::denied;
        ::_exit(good?0:1);
    }
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0&&inspect(s).pins==1);
}
int main()
{
    configuration_and_owner();atomic_batches_and_retry();complete_objects_and_owned_schema();capacity_pins_and_stop();
    replay_order_and_delivery();concurrent_close_and_retire();external_immutable_segments();real_allocation_failures();fork_does_not_inherit();
    std::cout<<"passed 9 recording store security groups\n";
}
