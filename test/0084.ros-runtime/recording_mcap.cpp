/** @file @brief Closed MCAP bounds, reserved trailer, malformed lengths and real native interoperability. */
#include <uwvm2/imported/ros/scene/recording_mcap.h>
#include "../0017.runtime/fast_io_allocation_probe.h"
#include <cassert>
#include <iostream>
#ifdef RECORDING_NATIVE_MCAP
#define MCAP_IMPLEMENTATION
#define MCAP_COMPRESSION_NO_ZSTD
#define MCAP_COMPRESSION_NO_LZ4
#include <mcap/reader.hpp>
#include <mcap/writer.hpp>
#endif
namespace m=uwvm2::imported::ros::scene::recording::mcap;
using result=m::result;
static bool ros1_bool(m::bytes value)noexcept{return value.size()==1&&value[0]<=1;}
static bool ros2_bool(m::bytes value)noexcept
{return value.size()==5&&value[0]==0&&value[1]==1&&value[2]==0&&value[3]==0&&value[4]<=1;}
static std::array<m::channel,2> channels(bool ros2)
{
    m::channel value{1,1,"/state","std_msgs/Bool",ros2?"ros2msg":"ros1msg",ros2?"cdr":"ros1",ros2?"[]":"",m::raw("bool data\n"),ros2?ros2_bool:ros1_bool,8};
    if(ros2)value.type="std_msgs/msg/Bool";
    auto other=value;other.id=2;other.topic="/second";return {value,other};
}
struct fixture
{
    std::array<m::channel,2> selected;
    std::array<std::uint8_t,4096> arena{};
    std::array<std::uint8_t,5> payload{0,1,0,0,1};
    m::bytes input,complete;std::string_view profile;
    m::segment writer;
    explicit fixture(bool ros2):selected(channels(ros2)),input(ros2?m::bytes{payload}:m::bytes{payload.data()+4,1}),profile(ros2?"ros2":"ros1")
    {
        assert(writer.open(arena,profile,selected,3)==result::success);
        assert(writer.append({1,UINT32_MAX,1000,500,input})==result::success);
        assert(writer.append({2,0,200,UINT64_MAX,input})==result::success);
        assert(writer.finish(complete)==result::success);
    }
};
static void bounded_roundtrip()
{
    for(bool ros2:{false,true})
    {
        auto before=fast_io_allocation_probe::failures;fast_io_allocation_probe::armed=true;
        fixture f{ros2};std::array<m::position,3> positions{};std::size_t count=99;
        assert(m::index(f.complete,f.profile,f.selected,{4096,100,3},positions,count)==result::success&&count==2);
        assert(positions[0].channel==1&&positions[0].sequence==UINT32_MAX&&positions[0].log_time==1000&&positions[0].publish_time==500);
        assert(positions[1].channel==2&&positions[1].sequence==0&&positions[1].log_time==200&&positions[1].publish_time==UINT64_MAX);
        for(std::size_t i{};i<count;++i)assert(m::same(f.complete.subspan(positions[i].offset,positions[i].size),f.input));
        m::bytes repeated;assert(f.writer.finish(repeated)==result::success&&m::same(repeated,f.complete));
        assert(f.writer.append({1,1,1,1,f.input})==result::conflict);
        fast_io_allocation_probe::armed=false;assert(fast_io_allocation_probe::failures==before);
    }
}
static void reservations_and_failed_writes()
{
    auto selected=channels(false);std::array<std::uint8_t,4096> arena;arena.fill(0xaa);m::segment small;
    assert(small.open(std::span{arena}.first(80),"ros1",selected,2)==result::too_large);
    assert(std::all_of(arena.begin(),arena.end(),[](auto v){return v==0xaa;}));
    m::segment prefix;assert(prefix.open(arena,"ros1",selected,2)==result::success);auto needed=prefix.size()+32+m::trailer_size;
    m::segment exact;assert(exact.open(std::span{arena}.first(needed),"ros1",selected,2)==result::success);
    std::array<std::uint8_t,1> payload{1};auto original=exact.size();auto saved=arena;
    assert(exact.append({3,1,1,1,payload})==result::invalid&&arena==saved&&exact.size()==original);
    assert(exact.append({1,1,1,1,std::span{arena}.first(1)})==result::invalid&&arena==saved);
    payload[0]=2;assert(exact.append({1,1,1,1,payload})==result::invalid&&arena==saved);payload[0]=1;
    assert(exact.append({1,1,1,1,payload})==result::success);saved=arena;auto size=exact.size();
    assert(exact.append({2,2,2,2,payload})==result::too_large&&arena==saved&&exact.size()==size);
    m::bytes complete;assert(exact.finish(complete)==result::success&&complete.size()==needed);
    m::segment zero;assert(zero.open(arena,"ros1",selected,0)==result::success&&zero.append({1,1,1,1,payload})==result::too_large&&zero.finish(complete)==result::success);
    m::segment batch;assert(batch.open(arena,"ros1",selected,3)==result::success);saved=arena;size=batch.size();
    std::array<std::uint8_t,1> invalid{2};std::array records{m::message{1,1,3,2,payload},m::message{2,2,5,4,invalid}};
    assert(batch.append(records)==result::invalid&&batch.size()==size&&arena==saved);
    records[1].data=payload;assert(batch.append(records)==result::success&&batch.count()==2);
    saved=arena;size=batch.size();assert(batch.append(records)==result::too_large&&batch.size()==size&&arena==saved);
    assert(batch.finish(complete)==result::success);
}
static void hostile_containers()
{
    fixture f{true};std::array<m::position,3> output{};std::size_t count=73;
    for(std::size_t n{};n<f.complete.size();++n)
    {assert(m::index(f.complete.first(n),f.profile,f.selected,{4096,100,3},output,count)!=result::success&&count==73);}
    auto check=[&](m::limits bound,auto indexes)
    {assert(m::index(f.complete,f.profile,f.selected,bound,indexes,count)==result::too_large&&count==73);};
    check({f.complete.size()-1,100,3},std::span{output});check({4096,1,3},std::span{output});check({4096,100,1},std::span{output});check({4096,100,3},std::span{output}.first(1));
    auto changed=f.arena;for(unsigned i{};i<8;++i)changed[9+i]=0xff;
    assert(m::index(std::span{changed}.first(f.complete.size()),f.profile,f.selected,{4096,100,3},output,count)==result::invalid&&count==73);
    auto wrong=f.selected;wrong[0].definition=m::raw("uint8 data\n");wrong[1].definition=wrong[0].definition;
    assert(m::index(f.complete,f.profile,wrong,{4096,100,3},output,count)==result::invalid&&count==73);
    changed=f.arena;changed[f.complete.size()-m::trailer_size]=6;
    assert(m::index(std::span{changed}.first(f.complete.size()),f.profile,f.selected,{4096,100,3},output,count)==result::unsupported&&count==73);
    changed=f.arena;changed[f.complete.size()-m::trailer_size+9]=1;
    assert(m::index(std::span{changed}.first(f.complete.size()),f.profile,f.selected,{4096,100,3},output,count)==result::unsupported&&count==73);
    wrong=f.selected;wrong[1].id=wrong[0].id;assert(!m::valid(f.profile,wrong));
    wrong=f.selected;wrong[1].definition=m::raw("different");assert(!m::valid(f.profile,wrong));
    wrong=f.selected;wrong[0].qos="";assert(!m::valid(f.profile,wrong));
    // Exhaustive single-byte corruptions remain bounded under ASan/UBSan.
    for(std::size_t i{};i<f.complete.size();++i)
    {changed=f.arena;changed[i]^=0xff;count=73;auto status=m::index(std::span{changed}.first(f.complete.size()),f.profile,f.selected,{4096,100,3},output,count);assert(status==result::success||count==73);}
}
#ifdef RECORDING_NATIVE_MCAP
struct native_storage final:mcap::IWritable,mcap::IReadable
{
    std::array<std::byte,16384> data{};std::uint64_t used{};bool ended{};
    std::uint64_t size()const override{return used;}
    void handleWrite(std::byte const* bytes,std::uint64_t size)override
    {assert(size<=data.size()-used);std::memcpy(data.data()+used,bytes,size);used+=size;}
    void end()override{ended=true;}
    std::uint64_t read(std::byte** output,std::uint64_t offset,std::uint64_t length)override
    {if(offset>used)return 0;*output=data.data()+offset;return std::min(length,used-offset);}
};
static void actual_upstream_roundtrip()
{
    for(bool ros2:{false,true})
    {
        fixture f{ros2};native_storage storage;storage.handleWrite(reinterpret_cast<std::byte const*>(f.complete.data()),f.complete.size());
        mcap::McapReader reader;assert(reader.open(storage).ok());unsigned count{};
        for(auto const& view:reader.readMessages())
        {
            assert(count<2);auto const& msg=view.message;auto const& c=f.selected[count];
            assert(msg.channelId==c.id&&msg.sequence==(count?0:UINT32_MAX)&&msg.logTime==(count?200:1000)&&msg.publishTime==(count?UINT64_MAX:500));
            assert(view.channel->topic==c.topic&&view.channel->messageEncoding==c.message_encoding&&view.schema->name==c.type&&view.schema->encoding==c.schema_encoding);
            assert(m::same({reinterpret_cast<std::uint8_t const*>(msg.data),static_cast<std::size_t>(msg.dataSize)},f.input));++count;
        }
        assert(count==2);reader.close();native_storage native;mcap::McapWriter writer;mcap::McapWriterOptions options{f.profile};
        options.noChunking=options.noSummary=options.noSummaryCRC=true;writer.open(native,options);
        auto const& c=f.selected[0];mcap::Schema schema{c.type,c.schema_encoding,"bool data\n"};writer.addSchema(schema);
        for(unsigned i{};i<2;++i)
        {
            mcap::Channel channel;channel.topic=f.selected[i].topic;channel.schemaId=schema.id;channel.messageEncoding=c.message_encoding;
            if(ros2)channel.metadata.emplace("offered_qos_profiles","[]");
            writer.addChannel(channel);
            assert(channel.id==i+1);mcap::Message msg;msg.channelId=channel.id;msg.sequence=i;msg.logTime=9-i;msg.publishTime=12;
            msg.data=reinterpret_cast<std::byte const*>(f.input.data());msg.dataSize=f.input.size();assert(writer.write(msg).ok());
        }
        writer.close();assert(native.ended);std::array<m::position,3> positions{};std::size_t indexed{};
        auto input=m::bytes{reinterpret_cast<std::uint8_t const*>(native.data.data()),static_cast<std::size_t>(native.used)};
        assert(m::index(input,f.profile,f.selected,{16384,100,3},positions,indexed)==result::success&&indexed==2);
        for(std::size_t i{};i<indexed;++i)assert(positions[i].channel==i+1&&positions[i].sequence==i&&positions[i].log_time==9-i&&positions[i].publish_time==12&&m::same(input.subspan(positions[i].offset,positions[i].size),f.input));
    }
}
#endif
int main()
{
    bounded_roundtrip();reservations_and_failed_writes();hostile_containers();
#ifdef RECORDING_NATIVE_MCAP
    actual_upstream_roundtrip();std::cout<<"passed 4 recording MCAP groups including actual upstream reader/writer\n";
#else
    std::cout<<"passed 3 recording MCAP security groups\n";
#endif
}
