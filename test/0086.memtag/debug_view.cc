#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/extension/memtag/debug_view.h>
#include <uwvm2/uwvm/debugger/console_memory.h>
#include <uwvm2/uwvm/debugger/command.h>
#include <array>
#include <cstdio>
#include <cstdlib>
namespace mt=uwvm2::extension::memtag;
static unsigned checks;
static void check(bool value,unsigned line) { ++checks;if(!value) { std::fprintf(stderr,"debug tags failure %u\n",line);std::abort(); } }
#define CHECK(...) check(static_cast<bool>((__VA_ARGS__)),__LINE__)
template<typename Address,bool Shared> static void exercise()
{
 mt::shadow_storage storage;std::array<std::uint8_t,17> destination{};
 CHECK(mt::copy_debug_tags(storage,4096,0,256,destination).status==mt::debug_tag_status::disabled);
 CHECK(mt::copy_debug_tags(storage,4096,4096,1,destination).status==mt::debug_tag_status::invalid_range);
 auto const width{sizeof(Address)==4u ? mt::address_width::i32 : mt::address_width::i64};
 CHECK(storage.enable(width,Shared,mt::storage_backend::growable,4096,8192)==mt::storage_error::none);
 auto view{storage.view<Address,Shared>(4096)};
 for(unsigned i{};i!=16;++i) { CHECK(view.store(decltype(view)::insert(i*16u,i),16)); }
 auto value=mt::copy_debug_tags(storage,4096,1,255,destination);
 CHECK(value.status==mt::debug_tag_status::available && value.count==16 && value.first_granule==0 && value.tag_bits==(sizeof(Address)==4 ? 4u : 8u));
 for(unsigned i{};i!=16;++i) { CHECK(destination[i]==i); }
 value=mt::copy_debug_tags(storage,4096,15,2,destination);CHECK(value.count==2 && destination[0]==0 && destination[1]==1);
 CHECK(mt::copy_debug_tags(storage,4096,0,256,std::span(destination).first(15)).status==mt::debug_tag_status::invalid_range);
 CHECK(mt::copy_debug_tags(storage,4096,UINT64_MAX,2,destination).status==mt::debug_tag_status::invalid_range);
 CHECK(mt::copy_debug_tags(storage,4096,4096,0,{}).status==mt::debug_tag_status::available);
 CHECK(mt::copy_debug_tags(storage,4096,4096,1,destination).status==mt::debug_tag_status::invalid_range);
 CHECK(mt::copy_debug_tags(storage,8192,4096,1,destination).status==mt::debug_tag_status::invalid_range);
 // An actual snapshot shorter than prepared storage never exposes spare tags.
 CHECK(mt::copy_debug_tags(storage,2048,2048,1,destination).status==mt::debug_tag_status::invalid_range);
 mt::shadow_storage isolated;CHECK(isolated.enable(width,Shared,mt::storage_backend::growable,4096,4096)==mt::storage_error::none);
 value=mt::copy_debug_tags(isolated,4096,16,16,destination);CHECK(value.count==1 && destination[0]==0);
 auto const moved{std::move(storage)};value=mt::copy_debug_tags(moved,4096,16,16,destination);CHECK(value.count==1 && destination[0]==1);
 CHECK(mt::copy_debug_tags(storage,4096,16,16,destination).status==mt::debug_tag_status::disabled);
}
int main()
{
 exercise<std::uint32_t,false>();exercise<std::uint32_t,true>();exercise<std::uint64_t,false>();exercise<std::uint64_t,true>();
 namespace dbg=uwvm2::uwvm::debugger;
 auto parsed=dbg::parse_console_command("memtags 2 3 17 256");CHECK(parsed.memory_tags && parsed.operation==uwvm2::utils::control::operation::read_memory);
 CHECK(dbg::parse_console_command("memory 2 3 17 256").memory_tags==false);
 CHECK(dbg::parse_console_command("memtags 2 4294967296 0 1").kind==dbg::console_command_kind::invalid);
 CHECK(dbg::parse_console_command("memtags 2 3 18446744073709551615 1").kind==dbg::console_command_kind::invalid);
 std::array<std::byte,2> bytes{std::byte{65},std::byte{66}},tags{std::byte{1},std::byte{2}};
 auto const plain{dbg::console_memory::dump(bytes,15)},colored{dbg::console_memory::dump(bytes,15,120,tags,0)};
 CHECK(plain.find("\x1b")==fast_io::containers::npos && colored.find("\x1b")!=fast_io::containers::npos);
 std::printf("debug tag components: %u checks passed\n",checks);
}
