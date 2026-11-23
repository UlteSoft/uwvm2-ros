// Unified environment records are detached data, never host authority.
#include <uwvm2/runtime/diagnostic/snapshot_wasip1.h>
#include <cstdio>
#include <cstdlib>
namespace ws=uwvm2::runtime::diagnostic::snapshot_wasip1;
namespace ar=uwvm2::runtime::diagnostic::snapshot_archive;
static void require(bool good,int line){if(!good){std::fprintf(stderr,"snapshot WASI FAIL line=%d\n",line);std::abort();}}
#define CHECK(...) require(bool(__VA_ARGS__),__LINE__)
static bool write(void* context,std::span<std::byte const> bytes) noexcept
{try{auto& out=*static_cast<ws::bytes*>(context);out.insert(out.end(),bytes.begin(),bytes.end());return true;}catch(...){return false;}}
static ws::bytes blob(char const* text)
{auto p=reinterpret_cast<std::byte const*>(text);return {p,p+std::char_traits<char>::length(text)};}
int main()
{
    ws::environment e{};e.recording[0]=std::byte{1};e.module=2;e.members={2,3};e.opens_size=5;
    e.arguments={blob("robot"),ws::bytes(8193,std::byte{'a'})};e.variables={blob("MODE=restore")};
    ws::resource file{};file.kind=ws::resource_kind::managed_file;file.flags_known=true;file.offset_known=true;file.offset=23;
    file.content=ws::bytes(70001,std::byte{0x5a});file.content[65536]=std::byte{0x89};
    e.resources.push_back(file);ws::resource stdio{};stdio.kind=ws::resource_kind::stdio;stdio.stdio_index=1;e.resources.push_back(stdio);
    e.bindings={{1,1,64,0},{3,0,66,0},{INT32_MAX,0,2,0}};
    e.reserved={{0,false,true,0,0},{2,true,true,0,0}};e.closed={4};
    CHECK(ws::valid(e));CHECK(!e.grants_restore_authority());
    ws::bytes encoded{};CHECK(ws::encode(e,&encoded,write));CHECK(ws::measure(e).bytes==encoded.size());
    ws::environment decoded{};CHECK(ws::decode(encoded,decoded));ws::bytes again{};CHECK(ws::encode(decoded,&again,write));CHECK(again==encoded);
    CHECK(decoded.resources[0].content[65536]==std::byte{0x89});CHECK(decoded.arguments[1].size()==8193);
    CHECK(decoded.reserved[0].protected_slot && decoded.reserved[1].null_resource);CHECK(decoded.closed==e.closed);
    ar::record record{ar::component::wasip1,ar::completeness::complete,2,1,encoded};
    CHECK(ws::decode(ar::visibility::full,record,decoded));
    CHECK(ws::decode(ar::visibility::diagnostic,record,decoded).status==ws::error::disclosure_denied);
    record.id=3;CHECK(!ws::decode(ar::visibility::full,record,decoded));record.id=2;
    record.capture=ar::completeness::partial;CHECK(!ws::decode(ar::visibility::full,record,decoded));
    for(std::size_t n=0;n<encoded.size();n+=127)
    {ws::environment guard{};guard.module=999;CHECK(!ws::decode(std::span{encoded}.first(n),guard));CHECK(guard.module==999);}
    for(std::size_t i=112;i<168;i+=8)
    {auto bad=encoded;for(std::size_t j=0;j<8;++j)bad[i+j]=std::byte{0xff};CHECK(!ws::decode(bad,decoded));}
    auto trailing=encoded;trailing.push_back(std::byte{});CHECK(!ws::decode(trailing,decoded));
    auto invalid=e;invalid.members={2,2};CHECK(!ws::valid(invalid));
    invalid=e;invalid.bindings.push_back({0,0,2,0});CHECK(!ws::valid(invalid));
    invalid=e;invalid.bindings[0].resource_index=9;CHECK(!ws::valid(invalid));
    invalid=e;invalid.reserved[0].base=1;CHECK(!ws::valid(invalid));
    invalid=e;invalid.resources[1].content=blob("secret");CHECK(!ws::valid(invalid));
    invalid=e;invalid.resources[1].offset_known=true;CHECK(!ws::valid(invalid));
    invalid=e;invalid.arguments[0].push_back(std::byte{});CHECK(!ws::valid(invalid));
    invalid=e;invalid.resources[0].offset_known=false;CHECK(!ws::valid(invalid));
    CHECK(ws::encode(e,nullptr,+[](void*,std::span<std::byte const>)noexcept{return false;}).status==ws::error::write_failed);
    auto owned=std::make_shared<ws::environment const>(e);ws::environment_stream stream{};CHECK(stream.bind(owned));
    auto view=stream.record();CHECK(view.id==2 && view.bytes==encoded.size());
    ws::bytes streamed{};CHECK(view.emit(view.context,&streamed,write));CHECK(streamed==encoded);
    ::fast_io::sha256_context hash{};hash.update(encoded.data(),encoded.data()+encoded.size());hash.do_final();
    std::array<std::byte,32> digest{};hash.digest_to_byte_ptr(digest.data());
    std::printf("canonical-sha256=");for(auto b:digest)std::printf("%02x",std::to_integer<unsigned>(b));std::printf("\n");
    std::printf("unified WASI canonical codec, 32/64-bit counts, long text, managed bytes, aliases, protected slots and rejection PASS bytes=%zu\n",encoded.size());
}
