/** Authored local SOURCE/value contract, NOT EXECUTED.
 * Actual fast_io old string formatter versus the actual new owned stack
 * formatter; no Runtime/Wasm/ROS/native types or generated record stub.
 */
#include "direct_text.h"
#include <fast_io_unit/string.h>
#include <string>
#include <memory>
#include <new>
#include <cstdlib>
#include <cstring>
#include <utility>
namespace {
bool refuse_allocation{};std::size_t attempts{};
void require(bool v){if(!v)std::abort();}
struct deny_new {deny_new(){attempts=0;refuse_allocation=true;}~deny_new(){refuse_allocation=false;}};
// Literal original production helper's formatting path, retained as the
// independent historical byte oracle. It makes no I/O or permission claim.
template<class... Values>std::string original(Values const&...values){
    std::string text;text.reserve(2048);fast_io::ostring_ref_std sink{std::addressof(text)};
    fast_io::print(sink,values...);return text;
}
template<class... Values>void equal(Values const&...values){
    auto expected=original(values...);
    uwvm_unitree_example_text::record actual;
    {deny_new no_heap;actual=uwvm_unitree_example_text::format(values...);require(attempts==0);}
    require(actual.size()==expected.size()&&actual.size()<=uwvm_unitree_example_text::capacity);
    require(!std::memcmp(actual.data(),expected.data(),expected.size()));
    // Returned/copy/moved owners retain their own complete bytes; no sink or
    // temporary std::string pointer escapes the function under test.
    auto copied=actual;auto moved=std::move(copied);actual.storage[0]^=1;
    require(!std::memcmp(moved.data(),expected.data(),expected.size()));
}
}
void* operator new(std::size_t n){if(refuse_allocation){++attempts;throw std::bad_alloc{};}if(auto p=std::malloc(n?n:1))return p;throw std::bad_alloc{};}
void* operator new[](std::size_t n){return ::operator new(n);}
void operator delete(void*p)noexcept{std::free(p);}void operator delete[](void*p)noexcept{std::free(p);}
void operator delete(void*p,std::size_t)noexcept{std::free(p);}void operator delete[](void*p,std::size_t)noexcept{std::free(p);}
int main(){
    static_assert(!uwvm_unitree_example_text::supported<std::string>);
    static_assert(!uwvm_unitree_example_text::supported<std::uint64_t>);
    static_assert(!uwvm_unitree_example_text::supported<bool>);
    for(std::uint32_t word:{std::uint32_t{0},std::uint32_t{1},std::uint32_t{32},UINT32_MAX}){
        equal("UNITREE_STEP ",word," ",word," ",word," ",word,"\n");
        equal("PASS_UNITREE_DIRECT steps=",word,"\n");
        equal("PASS_UNITREE_HOLD steps=",word,"\n");
    }
    equal("READY_UNITREE_DIRECT_V1\n");equal("READY_UNITREE_HOLD_V1\n");
    equal("PASS_UNITREE_DIRECT_DEFAULT_DENIED\n");equal("PASS_UNITREE_HOLD_DEFAULT_DENIED\n");
    for(unsigned i=0;i<4096;++i)equal("UNITREE_AGAIN\n");
    return 0;
}
