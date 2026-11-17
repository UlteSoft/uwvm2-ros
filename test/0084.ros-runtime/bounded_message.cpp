/** @file @brief Bounded wire semantics, hostile descriptions, conversion atomicity and allocation failures. */
#include <uwvm2/imported/ros/scene/communication_validation.h>
#include <cassert>
#include <cstdio>
#include <cfenv>
#include <atomic>

namespace b=uwvm2::imported::ros::scene::bounded_message;
namespace w=uwvm2::imported::ros::scene::wire;
using uwvm2::imported::ros::scene::communication::message_validator;
template<typename T>using vector=uwvm2::utils::container::std_vector<T>;
using bytes=vector<std::uint8_t>;
struct member{std::string_view name;std::uint64_t type;};
struct node{std::string_view kind;std::uint64_t bound{},flags{};vector<member> members{};};
static bytes describe(std::string_view format,vector<node> const& nodes,std::uint64_t work=1000000,std::uint64_t depth=64,std::uint64_t maximum=65536)
{
    auto write=[&](w::writer& out)
    {
        assert(out.text(b::magic)&&out.text(format)&&out.u64(maximum)&&out.u64(work)&&out.u64(depth)&&out.u64(nodes.size()));
        for(auto const& node:nodes)
        {assert(out.text(node.kind)&&out.u64(node.bound)&&out.u64(node.flags)&&out.u64(node.members.size()));for(auto const& field:node.members)assert(out.text(field.name)&&out.u64(field.type));}
    };
    auto measured=w::writer::measuring(4u*1024u*1024u);write(measured);bytes data(measured.size());w::writer out{data};write(out);return data;
}
static b::schema prepare(std::string_view format,vector<node> const& nodes)
{b::schema result;assert(result.parse(describe(format,nodes))==b::status::ok);return result;}
struct message
{
    bool cdr{},little{true};bytes data;
    explicit message(bool selected,bool order=true):cdr(selected),little(order){if(cdr)data={0,static_cast<std::uint8_t>(little),0,0};}
    void scalar(unsigned width,std::uint64_t value)
    {
        if(cdr)while((data.size()-4)%width)data.push_back(0);
        for(unsigned i{};i<width;++i)data.push_back(static_cast<std::uint8_t>(value>>(8*(little?i:width-1-i))));
    }
    void string(std::string_view value)
    {scalar(4,value.size()+(cdr?1:0));data.insert(data.end(),value.begin(),value.end());if(cdr)data.push_back(0);}
};
static bytes converted(b::schema const& schema,w::bytes data,b::layout target,bool little)
{
    bytes output(65536,0x55);std::size_t length=777;
    assert(schema.convert(data,target,little,{},length)==b::status::too_large||length==0);
    assert(schema.convert(data,target,little,output,length)==b::status::ok);
    assert(std::all_of(output.begin()+length,output.end(),[](auto x){return x==0x55;}));output.resize(length);return output;
}
static void hostile()
{
    vector<node> scalar{{"bool"},{"record",0,0,{{"value",0}}}};auto description=describe("cdr",scalar);
    b::schema empty;assert(!empty.validate({}));std::size_t required=123;assert(empty.convert({},b::layout::cdr,true,{},required)==b::status::denied&&required==123);
    for(std::size_t i{};i<description.size();++i){b::schema value;assert(value.parse(w::bytes{description}.first(i))!=b::status::ok&&!value.ready());}
    auto bad=description;bad.push_back(0);assert(empty.parse(bad)==b::status::invalid&&!empty.ready());
    assert(empty.parse(description)==b::status::ok&&empty.parse(description)==b::status::conflict);
    for(auto nodes:vector<vector<node>>{
        {{"uint64",1},{"record",0,0,{{"value",0}}}},
        {{"bool",0,b::finite},{"record",0,0,{{"value",0}}}},
        {{"record",0,0,{{"cycle",0}}}},
        {{"bool"},{"record",0,0,{{"a",0},{"a",0}}}},
        {{"bool"},{"record",0,0,{{"bad.name",0}}}},
        {{"bool"},{"record"}},
        {{"uint64"}},
        {{"bool"},{"array",0,0,{{"element",0}}},{"record",0,0,{{"value",1}}}},
        {{"bool"},{"sequence",2,0,{{"other",0}}},{"record",0,0,{{"value",1}}}},
        {{"pointer"},{"record",0,0,{{"value",0}}}}})
    {b::schema result;assert(result.parse(describe("cdr",nodes))!=b::status::ok&&!result.ready());}
    vector<node> bomb{{"record"},{"sequence",UINT32_MAX,0,{{"element",0}}},{"record",0,0,{{"values",1}}}};
    b::schema rejected;assert(rejected.parse(describe("ros1",bomb))==b::status::too_large);
    vector<node> deep{{"record"}};for(unsigned i=1;i<65;++i)deep.push_back({"record",0,0,{{"nested",i-1}}});
    assert(rejected.parse(describe("cdr",deep))==b::status::too_large);
    deep.pop_back();assert(rejected.parse(describe("cdr",deep))==b::status::ok);assert(rejected.validate(std::array<std::uint8_t,5>{0,1,0,0,0}));
    auto frame=std::array<std::uint8_t,5>{0,1,0,0,1};assert(empty.validate(frame));
    for(std::size_t i{};i<frame.size();++i)assert(!empty.validate(w::bytes{frame}.first(i)));
    for(auto index:{0,1,2,3,4}){auto changed=frame;changed[index]=2;assert(!empty.validate(changed));}
    auto old=required;std::array<std::uint8_t,32> output;output.fill(0x5a);auto invalid=frame;invalid[4]=3;
    assert(empty.convert(invalid,b::layout::ros1,true,output,required)==b::status::invalid&&required==old);
    assert(std::all_of(output.begin(),output.end(),[](auto x){return x==0x5a;}));
    assert(empty.convert(frame,b::layout::ros1,true,std::span{frame}.subspan(1),required)==b::status::invalid);
    message_validator copied,other;assert(copied.assign(description)==b::status::ok);other=copied;
    assert(copied==other&&copied(frame));assert(copied.assign(bad)==b::status::invalid&&copied==other&&copied(frame));
    message_validator independent;assert(independent.assign(description)==b::status::ok&&independent==copied);
    auto builtin=+[](w::bytes)noexcept{return true;};assert(copied!=builtin);message_validator fixed=builtin;assert(fixed==builtin&&fixed!=copied);
    std::fill(description.begin(),description.end(),0);assert(copied(frame));
}
static void layouts()
{
    vector<node> nodes{{"uint8"},{"uint64"},{"float32",0,b::finite},{"string",16,b::unicode|b::no_nul},
        {"record",0,0,{{"tag",0},{"counter",1},{"measurement",2},{"name",3}}},
        {"sequence",4,0,{{"element",4}}},{"record",0,0,{{"samples",5},{"last",1}}}};
    auto packed=prepare("ros1",nodes),cdr=prepare("cdr",nodes);
    for(unsigned count=0;count<=4;++count)
    {
        auto make=[&](bool cd,bool le)
        {message result(cd,le);result.scalar(4,count);for(unsigned i{};i<count;++i){result.scalar(1,i);result.scalar(8,UINT64_C(0x8765432101234567)+i);result.scalar(4,0x3f800000u);result.string("robot");}result.scalar(8,47);return result.data;};
        auto ros=make(false,true),little=make(true,true),big=make(true,false);
        assert(packed.validate(ros)&&cdr.validate(little)&&cdr.validate(big));
        assert(converted(packed,ros,b::layout::cdr,true)==little&&converted(packed,ros,b::layout::cdr,false)==big);
        assert(converted(cdr,little,b::layout::ros1,true)==ros&&converted(cdr,big,b::layout::ros1,false)==ros);
        assert(converted(cdr,little,b::layout::cdr,false)==big&&converted(cdr,big,b::layout::cdr,true)==little);
        for(std::size_t n{};n<little.size();++n)assert(!cdr.validate(w::bytes{little}.first(n)));
        little.push_back(0);assert(!cdr.validate(little));
    }
    auto empty_packed=prepare("ros1",{{"record"}}),empty_cdr=prepare("cdr",{{"record"}});
    assert(empty_packed.validate({})&&empty_cdr.validate(std::array<std::uint8_t,5>{0,1,0,0,0}));
    auto raw=converted(empty_packed,{},b::layout::cdr,true);assert(raw==bytes({0,1,0,0,0}));
    assert(converted(empty_cdr,raw,b::layout::ros1,true).empty());
    auto array=prepare("cdr",{{"float64"},{"array",3,0,{{"element",0}}},{"record",0,0,{{"points",1}}}});
    message numeric(true);numeric.scalar(8,0x7ff0000000000001ull);numeric.scalar(8,0x8000000000000000ull);numeric.scalar(8,0xfff0000000000000ull);
    auto round=std::fegetround();auto flags=std::fetestexcept(FE_ALL_EXCEPT);
    assert(array.validate(numeric.data));auto swapped=converted(array,numeric.data,b::layout::cdr,false);assert(array.validate(swapped));
    assert(converted(array,swapped,b::layout::cdr,true)==numeric.data&&std::fegetround()==round&&std::fetestexcept(FE_ALL_EXCEPT)==flags);
    auto strict=prepare("cdr",{{"float64",0,b::finite},{"array",3,0,{{"element",0}}},{"record",0,0,{{"points",1}}}});
    assert(!strict.validate(numeric.data));
}
static void text()
{
    auto raw=prepare("ros1",{{"string",8},{"record",0,0,{{"value",0}}}});
    auto utf=prepare("cdr",{{"string",8,b::unicode},{"record",0,0,{{"value",0}}}});
    for(auto value:{std::string_view{},std::string_view{"robot"},std::string_view{"\xf0\x9f\xa4\x96"}})
    {message ros(false);ros.string(value);assert(raw.validate(ros.data));auto converted_bytes=converted(raw,ros.data,b::layout::cdr,true);assert(utf.validate(converted_bytes));}
    message nul(false);nul.string(std::string_view{"a\0b",3});assert(raw.validate(nul.data));std::array<std::uint8_t,32> output;output.fill(7);std::size_t length=99;
    assert(raw.convert(nul.data,b::layout::cdr,true,output,length)==b::status::invalid&&length==99&&output.front()==7);
    message invalid(true);invalid.string("\xc0\x80");assert(!utf.validate(invalid.data));
    invalid=message(true);invalid.string("toolong!!");assert(!utf.validate(invalid.data));
    auto wide=prepare("cdr",{{"wstring",4,b::unicode|b::no_nul},{"record",0,0,{{"value",0}}}});
    message robot(true);robot.scalar(4,2);robot.scalar(2,0xd83e);robot.scalar(2,0xdd16);assert(wide.validate(robot.data));
    assert(wide.validate(converted(wide,robot.data,b::layout::cdr,false)));
    assert(wide.convert(robot.data,b::layout::ros1,true,output,length)==b::status::unsupported);
    robot.data.back()=0;assert(!wide.validate(robot.data));
    robot=message(true);robot.scalar(4,1);robot.scalar(2,0xd800);assert(!wide.validate(robot.data));
}
static void scalar_collections()
{
    // Lengths straddle vector widths and tails. A prefix forces unaligned ROS1
    // words; the extra leading byte also misaligns every CDR input address.
    for(auto type:{std::string_view{"bool"},std::string_view{"float32"},std::string_view{"float64"}})
    for(bool cd:{false,true})for(bool little:{false,true})
    {
        if(!cd&&!little)continue;
        unsigned width=type=="bool"?1:type=="float32"?4:8;
        auto schema=prepare(cd?"cdr":"ros1",{{"uint8"},{type,0,width==1?0:b::finite},
            {"sequence",65,0,{{"element",1}}},{"record",0,0,{{"prefix",0},{"values",2},{"tail",0}}}});
        for(unsigned count{};count<=65;++count)
        {
            message wire(cd,little);wire.scalar(1,7);wire.scalar(4,count);
            for(unsigned i{};i<count;++i)
            {auto bits=width==1?std::uint64_t(i&1):width==4?UINT64_C(0x7f7fffff):UINT64_C(0x7fefffffffffffff);wire.scalar(width,bits);}
            wire.scalar(1,0x6a);bytes unaligned{0x55};unaligned.insert(unaligned.end(),wire.data.begin(),wire.data.end());
            auto input=w::bytes{unaligned}.subspan(1);assert(schema.validate(input));
            for(bool target_little:{false,true})
            {
                auto output=converted(schema,input,cd?b::layout::cdr:b::layout::ros1,target_little);
                assert(schema.validate(output));
                auto back=converted(schema,output,cd?b::layout::cdr:b::layout::ros1,little);
                assert(back==wire.data);
            }
            auto first=wire.data.size()-1-count*width;
            for(unsigned index{};index<count;++index)
            {
                auto bad=wire.data;
                // Infinity and signaling NaN in every position must fail before
                // any output byte or required-size value is published.
                for(auto bits:width==1?std::array<std::uint64_t,2>{2,255}:
                     width==4?std::array<std::uint64_t,2>{0x7f800000u,0xff800001u}:
                              std::array<std::uint64_t,2>{UINT64_C(0x7ff0000000000000),UINT64_C(0xfff0000000000001)})
                {
                    for(unsigned byte{};byte<width;++byte)bad[first+index*width+byte]=static_cast<std::uint8_t>(bits>>(8*(little?byte:width-1-byte)));
                    std::array<std::uint8_t,1024> destination;destination.fill(0x5a);std::size_t required=177;
                    auto rounding=std::fegetround(),flags=std::fetestexcept(FE_ALL_EXCEPT);
                    assert(!schema.validate(bad));assert(schema.convert(bad,cd?b::layout::cdr:b::layout::ros1,little,destination,required)==b::status::invalid);
                    assert(required==177&&std::all_of(destination.begin(),destination.end(),[](auto value){return value==0x5a;}));
                    assert(std::fegetround()==rounding&&std::fetestexcept(FE_ALL_EXCEPT)==flags);
                }
            }
        }
    }
}
#if defined(UWVM_BOUNDED_MESSAGE_ALLOCATION)
static std::atomic<std::size_t> fail_after{};static bool failed{};
static bool reject()noexcept{auto n=fail_after.load();if(n&&fail_after.fetch_sub(1)==1){failed=true;return true;}return false;}
extern "C" void* __real_malloc(std::size_t);extern "C" void* __real_calloc(std::size_t,std::size_t);extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return reject()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t s){return reject()?nullptr:__real_calloc(n,s);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return reject()?nullptr:__real_realloc(p,n);}
static void allocations()
{
    auto description=describe("cdr",{{"float64"},{"array",3,0,{{"element",0}}},{"record",0,0,{{"position",1}}}});
    std::size_t failures{};bool finished{};
    for(std::size_t point=1;point<256;++point)
    {
        message_validator value;fail_after=point;failed=false;auto status=value.assign(description);auto injected=failed;fail_after=0;
        if(injected){++failures;assert(status==b::status::no_memory&&!value);assert(value.assign(description)==b::status::ok);}
        else{assert(status==b::status::ok);finished=true;break;}
    }
    assert(finished&&failures>=8);message_validator valid;assert(valid.assign(description)==b::status::ok);
    std::array<std::uint8_t,28> message{};message[1]=1;std::array<std::uint8_t,28> output;std::size_t size{};
    fail_after=1;failed=false;assert(valid(message));assert(valid.bounded()->convert(message,b::layout::cdr,false,output,size)==b::status::ok);assert(!failed);fail_after=0;
    std::printf("recoverable fast_io allocation points: %zu\n",failures);
}
#endif
int main()
{
    hostile();layouts();text();scalar_collections();
#if defined(UWVM_BOUNDED_MESSAGE_ALLOCATION)
    allocations();
#endif
    std::puts("PASS bounded ROS messages: schema, wire, conversion, ownership and failure atomicity");
}
