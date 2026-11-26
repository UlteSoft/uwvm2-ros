#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/extension/memtag/checkpoint.h>
#include <uwvm2/extension/memtag/allocation_transition.h>
#include <uwvm2/extension/memtag/debug_view.h>
#include <uwvm2/uwvm/debugger/checkpoint_codec.h>
#include <cstdio>
#include <cstdlib>
namespace mt=uwvm2::extension::memtag;
namespace cp=uwvm2::uwvm::debugger::checkpoint;
namespace layout=uwvm2::runtime::checkpoint::memory_layout;
static unsigned checks{};
static void require(bool ok,unsigned line) { ++checks;if(!ok) {std::fprintf(stderr,"checkpoint tags line %u\n",line);std::abort();} }
#define CHECK(...) require(static_cast<bool>((__VA_ARGS__)),__LINE__)
static cp::state image(unsigned bits,unsigned tags,bool shared) {
 cp::state s{};s.recording_id[0]=std::byte{1};s.checkpoint_id=1;s.objects.resize(3);s.root_instances={2};
 s.objects[0].kind=cp::object_kind::module;s.objects[0].bytes={std::byte{0},std::byte{'a'},std::byte{'s'},std::byte{'m'},std::byte{1},std::byte{0},std::byte{0},std::byte{0}};
 s.objects[1].kind=cp::object_kind::instance;s.objects[1].words[2]=1;s.objects[1].links={1,3};
 auto& m=s.objects[2];m.kind=cp::object_kind::memory;m.flags=shared;m.words={bits,1,0,2,tags,0,0,0};
 if(tags) {m.words[7]=4u|((bits-4u)<<8u);m.bytes.resize(4096);for(std::size_t n{};n<m.bytes.size();++n)m.bytes[n]=std::byte(n&15u);}
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
 auto complete=s;complete.objects[2].words[5]=mt::checkpoint_directory_revision;
 CHECK(cp::validate_graph(complete)==cp::error::none);
 auto bad=s;bad.objects[2].bytes.pop_back();CHECK(cp::validate_graph(bad)==cp::error::invalid_shape);
 bad=s;bad.objects[2].bytes.push_back(std::byte{});CHECK(cp::validate_graph(bad)==cp::error::invalid_shape);
 bad=s;bad.objects[2].words[4]=8;CHECK(cp::validate_graph(bad)==cp::error::invalid_shape);
 bad=s;bad.objects[2].words[5]=2;CHECK(cp::validate_graph(bad)==cp::error::invalid_shape);
 bad=s;bad.objects[2].words[1]=0;CHECK(cp::validate_graph(bad)==cp::error::invalid_shape);
 bad=s;bad.objects[2].bytes.back()=std::byte{16};CHECK(cp::validate_graph(bad)==cp::error::invalid_shape);
 cp::limits limits{};limits.max_payload_bytes=4000;CHECK(cp::validate_graph(s,limits)==cp::error::limit_exceeded);
}
template<class Address,bool Shared> static void restore() {
 constexpr unsigned bits=sizeof(Address)*8;auto s=image(bits,4,Shared);auto const& bytes=s.objects[2].bytes;
 mt::shadow_storage storage;CHECK(!mt::restore_tag_image(storage,65536,bytes));
 CHECK(storage.enable(bits==32?mt::address_width::i32:mt::address_width::i64,Shared,mt::storage_backend::growable,65536,131072)==mt::storage_error::none);
 CHECK(mt::restore_tag_image(storage,65536,bytes));
 std::vector<std::uint8_t> result(4096);
 auto read=[&] {auto r=mt::copy_debug_tags(storage,65536,0,65536,result);CHECK(r.status==mt::debug_tag_status::available && r.count==4096);for(std::size_t n{};n<bytes.size();++n)CHECK(result[n]==std::to_integer<unsigned>(bytes[n]));};
 read();CHECK(!mt::restore_tag_image(storage,65536,{bytes.data(),bytes.size()-1}));read();
 CHECK(storage.allocation_directory().add_permanent(1,1,1));
 CHECK(!mt::restore_tag_image(storage,65536,bytes)); read();
 CHECK(storage.allocation_directory().remove_permanent(1,1,1));
 auto bad=bytes;bad.push_back(std::byte{});CHECK(!mt::restore_tag_image(storage,65536,bad));read();
 bad=bytes;bad.back()=std::byte{0xff};CHECK(!mt::restore_tag_image(storage,65536,bad));read();
 CHECK(!mt::restore_tag_image(storage,65537,bytes));read();
 CHECK(storage.prepare_growth(65537)==mt::storage_error::none);
 auto tail=bytes;tail.push_back(std::byte{3});CHECK(mt::restore_tag_image(storage,65537,tail));
 auto const view=storage.view<Address,Shared>(65537);CHECK(view.load(static_cast<Address>(65536)).allocation_tag==3);
}
template<class Address, bool Shared>
static void restore_ownership(unsigned start)
{
    constexpr auto width{sizeof(Address)==4u ? mt::address_width::i32 : mt::address_width::i64};
    constexpr auto bits{static_cast<unsigned>(width)}, tag_bits{4u};
    constexpr std::size_t length{65536u};
    mt::shadow_storage source, restored;
    CHECK(source.enable(width,Shared,mt::storage_backend::growable,length,length,16,mt::owner_backend::software,start)==mt::storage_error::none);
    std::vector<std::byte> memory(length);
    auto run=[&](mt::shadow_storage& owner,mt::opcode op,Address pointer,Address bytes)
    {
        Address const args[]{pointer,bytes};
        bool retry{};
        return mt::transition_allocation<Address,Shared>(owner.allocation_directory(),false,op,args,memory,
            owner.view<Address,Shared>(length),0x5421u,retry);
    };
    auto reservation{run(source,mt::opcode::reserve,16u,32u)};
    auto active{run(source,mt::opcode::claim,64u,16u)};
    CHECK(reservation && active);
    constexpr std::uint8_t global_tag{14u};
    auto source_view{source.view<Address,Shared>(length)};
    CHECK(source.allocation_directory().add_permanent(200,201,global_tag));
    CHECK(source_view.store(source_view.with_tag(3200,global_tag),32));
    CHECK(source_view.store(source_view.with_tag(8192,4),16));
    auto saved{image(bits,tag_bits,Shared)};
    auto& record{saved.objects[2]};
    auto const count{mt::checkpoint_extent_count(source.allocation_directory())};
    CHECK(count==3u);
    record.words[3]=length/65536u;
    record.words[5]=mt::checkpoint_directory_revision;
    record.words[6]=count;
    record.words[7]=layout::encode(source);
    record.bytes.resize(length/16u+count*mt::checkpoint_extent_bytes);
    CHECK(mt::copy_debug_tags(source,length,0,length,{reinterpret_cast<std::uint8_t*>(record.bytes.data()),length/16u}).count==length/16u);
    CHECK(mt::copy_checkpoint_extents(source.allocation_directory(),std::span{record.bytes}.subspan(length/16u)));
    std::vector<unsigned char> buffer(10000);
    fast_io::basic_obuffer_view<unsigned char> output{buffer.data(),buffer.data()+buffer.size()};
    CHECK(cp::encode_database(output,saved)==cp::error::none);
    cp::state decoded{};
    CHECK(cp::decode_database({reinterpret_cast<std::byte const*>(buffer.data()),static_cast<std::size_t>(output.curr_ptr-buffer.data())},decoded)==cp::error::none);
    CHECK(decoded==saved);
    auto const& row{decoded.objects[2]};
    CHECK(layout::tag_start(row.words[7])==start);
    CHECK(restored.enable(width,Shared,mt::storage_backend::growable,length,length,layout::ignored_bits(row.words[7]),
                          mt::owner_backend::automatic,layout::tag_start(row.words[7]))==mt::storage_error::none);
    CHECK(layout::encode(restored)==row.words[7] && restored.resolved_backend()==mt::owner_backend::software);
    mt::checkpoint_tag_image decoded_image{};
    CHECK(mt::split_checkpoint_tag_image(tag_bits,row.words[5],row.words[6],row.bytes,decoded_image));
    CHECK(mt::matches_checkpoint_permanent_extents(source,decoded_image));
    CHECK(!mt::matches_checkpoint_permanent_extents(restored,decoded_image));

    // A normal image must roll back all staged nodes if a second node exceeds
    // the configured host budget. Preserve the destination's previous raw tag.
    auto restored_view{restored.view<Address,Shared>(length)};
    CHECK(restored_view.store(restored_view.with_tag(0,10),16));
    namespace resource=uwvm2::utils::allocator::resource;
    constexpr auto pool{static_cast<unsigned>(resource::pool::memtag)};
    auto const previous{resource::policies[pool]};
    auto const charged{resource::live_bytes[pool].load()};
    resource::policies[pool].live=charged+mt::managed_allocations::bytes_per_extent;
    CHECK(!mt::restore_checkpoint_tag_image(restored,length,decoded_image));
    resource::policies[pool]=previous;
    CHECK(resource::live_bytes[pool].load()==charged && restored.allocation_directory().empty());
    CHECK(restored_view.load(0).allocation_tag==10);

    CHECK(mt::restore_checkpoint_tag_image(restored,length,decoded_image));
    CHECK(mt::matches_checkpoint_permanent_extents(restored,decoded_image));
    CHECK(restored_view.load(8192).allocation_tag==4);
    CHECK(!restored_view.check(reservation.value,0,32));
    CHECK(run(restored,mt::opcode::activate,reservation.value,32));
    CHECK(restored_view.check(reservation.value,0,32));
    CHECK(run(restored,mt::opcode::release,reservation.value,32));
    CHECK(restored_view.check(active.value,0,16));
    CHECK(run(restored,mt::opcode::release,active.value,16));
    auto const global{restored_view.with_tag(3200,global_tag)};
    CHECK(!run(restored,mt::opcode::release,global,32));
    CHECK(restored_view.check(global,0,32));
    CHECK(mt::checkpoint_extent_count(restored.allocation_directory())==1u);
}

int main() {
 static_assert(cp::database_format_version==10);
 for(bool shared:{false,true}) {codec(32,0,shared);codec(64,0,shared);codec(32,4,shared);codec(64,4,shared);}
 restore<std::uint32_t,false>();restore<std::uint32_t,true>();restore<std::uint64_t,false>();restore<std::uint64_t,true>();
 for(unsigned depth:{4u,7u,8u,16u}) {
  restore_ownership<std::uint32_t,false>(32u-depth);restore_ownership<std::uint32_t,true>(32u-depth);
  restore_ownership<std::uint64_t,false>(64u-depth);restore_ownership<std::uint64_t,true>(64u-depth);
 }
 CHECK(mt::valid_tag_image(64,0,UINT64_MAX,{}));CHECK(!mt::valid_tag_image(64,8,UINT64_MAX,{}));CHECK(!mt::valid_tag_image(32,4,UINT64_C(0x10000001),{}));
 std::printf("checkpoint tags %u checks PASS\n",checks);
}
