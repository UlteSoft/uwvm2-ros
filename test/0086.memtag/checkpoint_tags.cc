#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/extension/memtag/checkpoint.h>
#include <uwvm2/extension/memtag/debug_view.h>
#include <uwvm2/uwvm/debugger/checkpoint_codec.h>
#include <cstdio>
#include <cstdlib>
namespace mt=uwvm2::extension::memtag;
namespace cp=uwvm2::uwvm::debugger::checkpoint;
static unsigned checks{};
static void require(bool ok,unsigned line) { ++checks;if(!ok) {std::fprintf(stderr,"checkpoint tags line %u\n",line);std::abort();} }
#define CHECK(...) require(static_cast<bool>((__VA_ARGS__)),__LINE__)
static cp::state image(unsigned bits,unsigned tags,bool shared) {
 cp::state s{};s.recording_id[0]=std::byte{1};s.checkpoint_id=1;s.objects.resize(3);s.root_instances={2};
 s.objects[0].kind=cp::object_kind::module;s.objects[0].bytes={std::byte{0},std::byte{'a'},std::byte{'s'},std::byte{'m'},std::byte{1},std::byte{0},std::byte{0},std::byte{0}};
 s.objects[1].kind=cp::object_kind::instance;s.objects[1].words[2]=1;s.objects[1].links={1,3};
 auto& m=s.objects[2];m.kind=cp::object_kind::memory;m.flags=shared;m.words={bits,1,0,2,tags,0,0,0};
 if(tags) {m.bytes.resize(4096);for(std::size_t n{};n<m.bytes.size();++n)m.bytes[n]=std::byte(n&(tags==4?15:255));}
 return s;
}
static void codec(unsigned bits,unsigned tags,bool shared) {
 auto s=image(bits,tags,shared);CHECK(cp::validate_graph(s)==cp::error::none);
 std::vector<unsigned char> buffer(10000);fast_io::basic_obuffer_view<unsigned char> out{buffer.data(),buffer.data()+buffer.size()};
 CHECK(cp::encode_database(out,s)==cp::error::none);
 auto count=static_cast<std::size_t>(out.curr_ptr-buffer.data());
 cp::state decoded{};CHECK(cp::decode_database({reinterpret_cast<std::byte const*>(buffer.data()),count},decoded)==cp::error::none);CHECK(decoded==s);
 buffer[6]='6';CHECK(cp::decode_database({reinterpret_cast<std::byte const*>(buffer.data()),count},decoded)==cp::error::unsupported_version);CHECK(decoded==s);
 if(!tags) {auto bad=s;bad.objects[2].bytes={std::byte{1}};CHECK(cp::validate_graph(bad)==cp::error::invalid_shape);return;}
 auto bad=s;bad.objects[2].bytes.pop_back();CHECK(cp::validate_graph(bad)==cp::error::invalid_shape);
 bad=s;bad.objects[2].bytes.push_back(std::byte{});CHECK(cp::validate_graph(bad)==cp::error::invalid_shape);
 bad=s;bad.objects[2].words[4]=bits==32?8:4;CHECK(cp::validate_graph(bad)==cp::error::invalid_shape);
 bad=s;bad.objects[2].words[5]=1;CHECK(cp::validate_graph(bad)==cp::error::invalid_shape);
 bad=s;bad.objects[2].words[1]=0;CHECK(cp::validate_graph(bad)==cp::error::invalid_shape);
 if(bits==32) {bad=s;bad.objects[2].bytes.back()=std::byte{16};CHECK(cp::validate_graph(bad)==cp::error::invalid_shape);}
 cp::limits limits{};limits.max_payload_bytes=4000;CHECK(cp::validate_graph(s,limits)==cp::error::limit_exceeded);
}
template<class Address,bool Shared> static void restore() {
 constexpr unsigned bits=sizeof(Address)*8;auto s=image(bits,bits==32?4:8,Shared);auto const& bytes=s.objects[2].bytes;
 mt::shadow_storage storage;CHECK(!mt::restore_tag_image(storage,65536,bytes));
 CHECK(storage.enable(bits==32?mt::address_width::i32:mt::address_width::i64,Shared,mt::storage_backend::growable,65536,131072)==mt::storage_error::none);
 CHECK(mt::restore_tag_image(storage,65536,bytes));
 std::vector<std::uint8_t> result(4096);
 auto read=[&] {auto r=mt::copy_debug_tags(storage,65536,0,65536,result);CHECK(r.status==mt::debug_tag_status::available && r.count==4096);for(std::size_t n{};n<bytes.size();++n)CHECK(result[n]==std::to_integer<unsigned>(bytes[n]));};
 read();CHECK(!mt::restore_tag_image(storage,65536,{bytes.data(),bytes.size()-1}));read();
 auto bad=bytes;bad.push_back(std::byte{});CHECK(!mt::restore_tag_image(storage,65536,bad));read();
 if constexpr(sizeof(Address)==4) {bad=bytes;bad.back()=std::byte{0xff};CHECK(!mt::restore_tag_image(storage,65536,bad));read();}
 CHECK(!mt::restore_tag_image(storage,65537,bytes));read();
 CHECK(storage.prepare_growth(65537)==mt::storage_error::none);
 auto tail=bytes;tail.push_back(std::byte{3});CHECK(mt::restore_tag_image(storage,65537,tail));
 auto const view=storage.view<Address,Shared>(65537);CHECK(view.load(static_cast<Address>(65536)).allocation_tag==3);
}
int main() {
 static_assert(cp::database_format_version==7);
 for(bool shared:{false,true}) {codec(32,0,shared);codec(64,0,shared);codec(32,4,shared);codec(64,8,shared);}
 restore<std::uint32_t,false>();restore<std::uint32_t,true>();restore<std::uint64_t,false>();restore<std::uint64_t,true>();
 CHECK(mt::valid_tag_image(64,0,UINT64_MAX,{}));CHECK(!mt::valid_tag_image(64,8,UINT64_MAX,{}));CHECK(!mt::valid_tag_image(32,4,UINT64_C(0x10000001),{}));
 std::printf("checkpoint tags %u checks PASS\n",checks);
}
