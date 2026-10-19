#include <gateways/common/bounded_text.h>
#define bounded_text bounded_text_before
#include "before-bounded-text.inc"
#undef bounded_text
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <new>
namespace stats { thread_local bool active{};thread_local std::size_t calls{},bytes{}; }
void* operator new(std::size_t n) { if(stats::active) { ++stats::calls;stats::bytes+=n; }if(auto p=std::malloc(n?n:1)) return p;throw std::bad_alloc{}; }
void* operator new[](std::size_t n) { return ::operator new(n); }
void operator delete(void* p) noexcept { std::free(p); }
void operator delete[](void* p) noexcept { std::free(p); }
void operator delete(void* p,std::size_t) noexcept { std::free(p); }
void operator delete[](void* p,std::size_t) noexcept { std::free(p); }
using current_rows=uwvm2::ros_gateway::bounded_text::sorted_rows;
using before_rows=uwvm2::ros_gateway::bounded_text_before::sorted_rows;
std::size_t checks{},cases{};
void need(bool ok,char const* message) { ++checks;if(!ok) { std::fprintf(stderr,"FAIL %s\n",message);std::exit(1); } }
void equal(current_rows const& n,before_rows const& o)
{
    need(n.empty()==o.empty()&&n.error()==o.error()&&n.text_bytes()==o.text_bytes(),"row state differs");
    auto a=n.result(),b=o.result();need(a.error==b.error&&a.payload==b.payload,"complete wire/error differs");
}
struct totals { std::size_t calls,bytes; };
template<class Rows> totals scoped_add(Rows& r,std::string_view n,std::string_view t,bool& ok)
{
    stats::calls=stats::bytes=0;stats::active=true;ok=r.add(n,t);stats::active=false;return {stats::calls,stats::bytes};
}
void measured(char const* label,std::string_view name,std::string_view type,int preparation,bool expected,std::size_t old_calls,std::size_t new_calls)
{
    before_rows o{65540};current_rows n{65540};
    if(preparation==1) { need(o.add(name,type)&&n.add(name,type),"prepare duplicate"); }
    if(preparation==2) { need(o.add("a")&&n.add("a"),"prepare aggregate limit"); }
    bool a{},b{};auto old=scoped_add(o,name,type,a),now=scoped_add(n,name,type,b);
    need(a==b&&a==expected,"acceptance changed");equal(n,o);
    need(old.calls==old_calls&&now.calls==new_calls,"scoped ordinary-new allocation count");
    if(!new_calls) { need(now.bytes==0,"refusal/duplicate allocated bytes"); }
    if(old_calls==1) { need(old.bytes>=65537,"original long copy missing"); }
    ++cases;
    std::printf("OBSERVED case=%s accepted=%u error=%u before_calls=%zu before_bytes=%zu after_calls=%zu after_bytes=%zu retained_text_bytes=%zu\n",label,unsigned(b),n.error(),old.calls,old.bytes,now.calls,now.bytes,n.text_bytes());
}
void differential()
{
    std::vector<std::string> names{"","a","a\n","a\t","a\tb",std::string{"\0x",2},std::string{"x\0",2},"\xC3\xA9","\xFF","\xC0\xAF"};
    std::vector<std::string> types{"","b","\n","\t",std::string{"\0",1},"\xC3\xA9"};
    for(std::uint64_t max:{0u,3u,4u,5u,6u,17u,64u,65540u})
    {
        for(bool reverse:{false,true})
        {
            before_rows o{max};current_rows n{max};
            for(unsigned repeat{};repeat!=2;++repeat)
            {
                for(std::size_t i{};i!=names.size()*types.size();++i)
                {
                    auto j=reverse?names.size()*types.size()-1-i:i;
                    auto const& name=names[j/types.size()];auto const& type=types[j%types.size()];
                    need(n.add(name,type)==o.add(name,type),"fragment collision or quota acceptance");equal(n,o);++cases;
                }
            }
        }
    }
    // UTF8 errors are still deferred to result(); aggregate quota keeps priority90.
    current_rows n{7};need(n.add("\xFF"),"invalid UTF8 add changed");need(n.result().error==22,"deferred UTF8 error");need(!n.add("x"),"aggregate quota priority changed");need(n.result().error==90,"quota must precede UTF8 result validation");
    current_rows golden{100};need(golden.add("z")&&golden.add("a","t")&&golden.add("b"),"golden row insertion");
    auto wire=golden.result();std::vector<std::uint8_t> expected{8,0,0,0,'a','\t','t','\n','b','\n','z','\n'};need(wire.error==0&&wire.payload==expected,"explicit lexical newline golden wire");
    // Binary byte ordering and encoded collisions remain equal to std::set<string>.
    std::uint32_t random{0x12345678};std::vector<std::pair<std::string,std::string>> generated;
    for(unsigned i{};i!=256;++i)
    {
        std::string name,type;
        for(unsigned k{};k!=i%33;++k) { random=random*1664525u+1013904223u;name+=char(random>>24); }
        for(unsigned k{};k!=i%9;++k) { random=random*1664525u+1013904223u;type+=char(random>>24); }
        generated.emplace_back(std::move(name),std::move(type));
    }
    before_rows o{65540};current_rows r{65540};
    for(auto const& value:generated) { need(r.add(value.first,value.second)==o.add(value.first,value.second),"binary fragmented ordering");equal(r,o);++cases; }
}
int main()
{
    if(std::getenv("ROS_SORTED_ROWS_STOP")) { std::raise(SIGSTOP); }
    std::string long_name(65535,'x'),too_long(65536,'x'),typed_name(32767,'y'),typed_type(32767,'z');
    measured("individual-refused",too_long,{},0,false,0,0);
    measured("fresh-long-accepted",long_name,{},0,true,2,2);
    measured("full-quota-duplicate",long_name,{},1,true,1,0);
    measured("aggregate-refused",long_name,{},2,false,1,0);
    measured("full-quota-typed-duplicate",typed_name,typed_type,1,true,1,0);
    measured("typed-aggregate-refused",typed_name,typed_type,2,false,1,0);
    differential();std::printf("PASS sorted_rows exact-wire differential cases=%zu checks=%zu; scoped ordinary-new only; no throughput claim\n",cases,checks);
}
