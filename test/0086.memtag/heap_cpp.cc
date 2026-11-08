#include <vector>
#include <string>
#include <memory>
#include <new>
#include <cstdint>
#ifndef HEAP_CASE
#define HEAP_CASE 0
#endif
static volatile std::uintptr_t escaped;
static volatile unsigned observed;
static void check(bool p){if(!p)__builtin_trap();}
struct alignas(64) aligned {unsigned bytes[32];};
extern "C" __attribute__((export_name("_start"))) void heap_cpp_probe(){
#if HEAP_CASE==0
 std::vector<unsigned> values;
 for(unsigned i{};i!=1024;++i)values.push_back(i*7);
 for(unsigned i{};i!=1024;++i)check(values[i]==i*7);
 if(__builtin_wasm_memtag_status(0))check(__builtin_wasm_memtag_extract(0,values.data())!=0);
 values.resize(32);values.shrink_to_fit();for(unsigned i{};i!=32;++i)check(values[i]==i*7);
 std::string text;for(unsigned i{};i!=2048;++i)text.push_back(char('a'+i%26));
 for(unsigned i{};i!=2048;++i)check(text[i]==char('a'+i%26));
 auto single=std::make_unique<unsigned>(41);check(*single==41);
 auto array=std::make_unique<unsigned[]>(64);for(unsigned i{};i!=64;++i)check(array[i]==0);
 auto p=std::make_unique<aligned>();check((reinterpret_cast<std::uintptr_t>(p.get())&63)==0);
 p->bytes[31]=9;check(p->bytes[31]==9);
 void* failed=::operator new(32u*1024u*1024u,std::nothrow);check(failed==nullptr);
#elif HEAP_CASE==1
 {auto p=std::make_unique<unsigned>(7);escaped=reinterpret_cast<std::uintptr_t>(p.get());}
 observed=*reinterpret_cast<volatile unsigned*>(escaped);
#elif HEAP_CASE==2
 {std::vector<unsigned> values(16,7);escaped=reinterpret_cast<std::uintptr_t>(values.data());}
 *reinterpret_cast<volatile unsigned*>(escaped)=9;
#endif
}
