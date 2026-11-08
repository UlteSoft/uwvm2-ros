#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/compiler/uwvm_int/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/runtime/compiler/uwvm_int/optable/memtag.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <bit>
namespace op=uwvm2::runtime::compiler::uwvm_int::optable;
namespace md=op::memtag_details;
namespace mt=uwvm2::extension::memtag;
static void check(bool pass,unsigned line){if(!pass){std::fprintf(stderr,"action failed line %u\n",line);std::abort();}}
#define CHECK(...) check(static_cast<bool>((__VA_ARGS__)),__LINE__)
template<class Address>
void run(){
 using Memory=md::native_memory_t;
 constexpr auto width=sizeof(Address)==4?mt::address_width::i32:mt::address_width::i64;
#if defined(UWVM_SUPPORT_MMAP)
 Memory memory(65536,sizeof(Address)==4?uwvm2::object::memory::linear::mmap_memory_status_t::wasm32:
 uwvm2::object::memory::linear::mmap_memory_status_t::wasm64);
#else
 Memory memory;
#endif
 memory.init_by_page_count(1);
 CHECK(memory.enable_memtag(width,false)==mt::storage_error::none);
 std::array<std::byte,128> stack_storage{};auto* stack=stack_storage.data();
 auto tag=mt::shadow_view<Address>::insert(0,3);
 md::push(stack,tag);md::push(stack,Address{64});
 md::instruction<true,Address,false>(memory,stack,static_cast<unsigned>(mt::opcode::store));
 CHECK(stack==stack_storage.data());
 md::push(stack,tag);md::push(stack,std::int32_t{-123});
 md::scalar<Address,false,std::int32_t,4,false,true>(memory,stack,0);
 CHECK(stack==stack_storage.data());
 md::push(stack,tag);md::scalar<Address,false,std::int32_t,4,false,false>(memory,stack,0);
 CHECK(md::pop<std::int32_t>(stack)==-123);
 md::push(stack,tag);md::push(stack,std::int32_t{-2});
 md::scalar<Address,false,std::int32_t,1,false,true>(memory,stack,15);
 md::push(stack,tag);md::scalar<Address,false,std::int32_t,1,true,false>(memory,stack,15);
 CHECK(md::pop<std::int32_t>(stack)==-2);
 // Unaligned accesses spanning two granules, including exact sNaN transport.
 auto bits=std::uint64_t{0x7ff0000000000001ull};
 md::push(stack,tag);md::push(stack,bits);
 md::scalar<Address,false,double,8,false,true>(memory,stack,13);
 md::push(stack,tag);md::scalar<Address,false,double,8,false,false>(memory,stack,13);
 CHECK(md::pop<std::uint64_t>(stack)==bits);
 CHECK(stack==stack_storage.data());
 md::push(stack,tag);md::instruction<true,Address,false>(memory,stack,static_cast<unsigned>(mt::opcode::extract));
 CHECK(md::pop<Address>(stack)==3);
 md::instruction<false,Address,false>(memory,stack,static_cast<unsigned>(mt::opcode::status));
 CHECK(md::pop<Address>(stack)==0);
}
int main(){
 static_assert(op::compile_option{}.execute_memtag);
 run<std::uint32_t>();run<std::uint64_t>();
 std::puts("PASS interpreter MTE actions: i32/i64 addresses, scalar sign extension, unaligned sNaN, opcode execution");
}
