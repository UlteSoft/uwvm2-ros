/** @file @brief Closed recording carriers, truncation, exact sizes and zero allocation. */
#include <uwvm2/imported/ros/scene/recording_contract.h>
#include "../0017.runtime/fast_io_allocation_probe.h"
#include <cassert>
#include <iostream>
namespace c=uwvm2::imported::ros::recording_contract;
namespace w=uwvm2::imported::ros::scene::wire;
namespace r=uwvm2::imported::ros::scene::recording;
using op=c::operation;
static constexpr std::array<std::uint8_t,32> sha{7};
struct record
{
    std::array<std::uint8_t,8192> data{};std::size_t size{};
    w::bytes view()const noexcept{return {data.data(),size};}
};
static bool profile(w::writer& out){return out.text("robot.recording")&&out.blob(sha);}
static record request(op action,std::size_t count=1)
{
    record result;w::writer out{result.data};
    switch(action)
    {
    case op::recording_open:assert(profile(out));break;
    case op::recording_append:
        assert(out.u64(9)&&out.u64(1)&&out.u64(count));
        for(std::size_t i{};i<count;++i)assert(out.u64(2)&&out.u64(i+3)&&out.u64(i+1)&&out.u64(UINT32_MAX)&&out.u64(UINT64_MAX)&&out.u64(0)&&out.u64(7)&&out.blob(sha));
        break;
    case op::recording_checkpoint:assert(out.u64(9)&&out.u64(1));break;
    case op::recording_stop:assert(out.u64(9)&&out.u64(1)&&out.boolean(true));break;
    case op::replay_open:
        assert(out.u64(8)&&profile(out)&&out.u64(count));
        for(std::size_t i{};i<count;++i)assert(out.u64(i+1)&&out.u64(500)&&out.u64(4)&&out.blob(sha));
        break;
    case op::replay_read:assert(out.u64(9)&&out.u64(64));break;
    case op::replay_seek:assert(out.u64(9)&&out.u64(UINT64_MAX));break;
    }
    result.size=out.size();return result;
}
static void write_word(record& value,std::size_t offset,std::uint64_t number)
{assert(offset+8<=value.data.size());for(unsigned i{};i<8;++i)value.data[offset+i]=static_cast<std::uint8_t>(number>>(8*i));}
static void complete_and_truncated()
{
    for(auto descriptor:c::operations)
    {
        auto input=request(descriptor.id);c::request decoded;assert(c::decode(descriptor.id,input.view(),decoded));
        c::request sentinel;sentinel.resource=91;sentinel.key=92;sentinel.count=93;
        for(std::size_t n{};n<input.size;++n)
        {assert(!c::decode(descriptor.id,input.view().first(n),sentinel)&&sentinel.resource==91&&sentinel.key==92&&sentinel.count==93);}
        input.data[input.size++]=0;assert(!c::decode(descriptor.id,input.view(),sentinel)&&sentinel.resource==91);
    }
    c::request decoded;assert(!c::decode(static_cast<op>(255),{},decoded));
    auto maximum=request(op::recording_append,c::maximum_items);assert(maximum.size==c::maximum_request&&c::decode(op::recording_append,maximum.view(),decoded)&&decoded.count==63);
    auto excessive=request(op::recording_append,c::maximum_items+1);assert(!c::decode(op::recording_append,excessive.view(),decoded));
    auto objects=request(op::replay_open,c::maximum_objects);assert(c::decode(op::replay_open,objects.view(),decoded)&&decoded.count==64);
    auto too_many=request(op::replay_open,c::maximum_objects+1);assert(!c::decode(op::replay_open,too_many.view(),decoded));
}
static void hostile_fields()
{
    auto append=request(op::recording_append);c::request output;
    for(auto offset:{0u,8u,16u,24u,32u,40u})
    {auto invalid=append;write_word(invalid,offset,0);assert(!c::decode(op::recording_append,invalid.view(),output));}
    for(auto field:{std::pair{40u,UINT64_MAX},std::pair{48u,UINT64_MAX},std::pair{72u,UINT64_MAX},std::pair{80u,std::uint64_t{31}}})
    {auto invalid=append;write_word(invalid,field.first,field.second);assert(!c::decode(op::recording_append,invalid.view(),output));}
    auto stop=request(op::recording_stop);write_word(stop,16,2);assert(!c::decode(op::recording_stop,stop.view(),output));
    auto read=request(op::replay_read);write_word(read,8,0);assert(!c::decode(op::replay_read,read.view(),output));
    write_word(read,8,65);assert(!c::decode(op::replay_read,read.view(),output));
    auto opened=request(op::recording_open);opened.data[8]=0;assert(!c::decode(op::recording_open,opened.view(),output));
    auto duplicate=request(op::replay_open,2);w::reader in{duplicate.view()};std::uint64_t ignored{};w::bytes text,digest;
    assert(in.u64(ignored)&&in.blob(text)&&in.blob(digest)&&in.u64(ignored));auto first=in.position();
    write_word(duplicate,first+64,1);assert(!c::decode(op::replay_open,duplicate.view(),output));
}
static void output_records()
{
    for(auto mutation:{r::mutation::append,r::mutation::checkpoint,r::mutation::stop_drain,r::mutation::stop_abort})
    {
        r::receipt value{7,3,96,mutation,{5,500,3,sha},mutation==r::mutation::stop_drain||mutation==r::mutation::stop_abort};
        std::array<std::uint8_t,c::receipt_bytes> bytes{};w::writer out{bytes};assert(c::encode(out,value)&&out.size()==c::receipt_bytes);
        w::reader in{bytes};std::uint64_t key{},messages{},size{},tag{},id{},object_size{},object_messages{};w::bytes digest;bool stopped{};
        assert(in.u64(key)&&key==7&&in.u64(messages)&&messages==3&&in.u64(size)&&size==96&&in.u64(tag)&&tag==static_cast<unsigned>(mutation)+1&&
               in.u64(id)&&id==5&&in.u64(object_size)&&object_size==500&&in.u64(object_messages)&&object_messages==3&&in.blob(digest)&&
               r::mcap::same(digest,sha)&&in.boolean(stopped)&&stopped==value.stopped&&in.done());
        for(std::size_t n{};n<bytes.size();++n){w::writer short_output{std::span{bytes}.first(n)};assert(!c::encode(short_output,value));}
    }
    std::array<std::uint8_t,512> bytes{};std::array<std::uint8_t,2> body{1,2};
    std::array records{r::mcap::message{1,UINT32_MAX,9,3,body},r::mcap::message{2,0,4,5,{}}};
    w::writer out{bytes};assert(c::encode(out,3,records)&&out.size()==16+2*40+body.size());
    w::writer empty{bytes};assert(c::encode(empty,5,{})&&empty.size()==16);
    r::receipt invalid{0};w::writer bad{bytes};assert(!c::encode(bad,invalid)&&!bad.size());
}
int main()
{
    auto before=fast_io_allocation_probe::failures;fast_io_allocation_probe::armed=true;
    complete_and_truncated();hostile_fields();output_records();
    fast_io_allocation_probe::armed=false;assert(fast_io_allocation_probe::failures==before);
    std::cout<<"passed 3 recording carrier security groups\n";
}
