#include <uwvm2/runtime/diagnostic/snapshot_archive.h>
#include <uwvm2/runtime/diagnostic/snapshot_diagnostic.h>
#include <cstdio>
#include <cstdlib>
namespace s=uwvm2::runtime::diagnostic::snapshot_archive;
static unsigned checks{};
#define REQUIRE(x) do{++checks;if(!(x)){std::fprintf(stderr,"FAIL %u: %s\n",__LINE__,#x);std::exit(1);}}while(false)
static bool append(void* p,std::span<std::byte const> bytes) noexcept
{try{auto& out=*static_cast<std::vector<std::byte>*>(p);out.insert(out.end(),bytes.begin(),bytes.end());return true;}catch(...){return false;}}
static bool transfer(void* p,void* out,s::writer w) noexcept
{auto const& bytes=*static_cast<std::vector<std::byte> const*>(p);for(auto b:bytes)if(!w(out,{&b,1}))return false;return true;}
static void resign(std::vector<std::byte>& b)
{fast_io::sha256_context h{};h.update(b.data(),b.data()+b.size()-32);h.do_final();h.digest_to_byte_ptr(b.data()+b.size()-32);}
int main(int argc,char** argv)
{
 namespace d=uwvm2::runtime::diagnostic::snapshot_diagnostic;
 d::frame frames[]{{1,0,0,3,2,19},{1,1,0,1,1,UINT64_MAX},{8,0,1,0,0,4}};
 std::vector<std::byte> diagnostic;
 REQUIRE(d::encode({17,2,1,121,frames},diagnostic));REQUIRE(d::validate(diagnostic));
 for(std::size_t cut{};cut<diagnostic.size();++cut)REQUIRE(!d::validate(std::span(diagnostic).first(cut)));
 for(auto at:{12u,32u,40u,48u,56u,72u,112u,120u,168u})
 {auto bad=diagnostic;bad[at]^=std::byte{1};REQUIRE(!d::validate(bad));}
 auto no_stop=diagnostic;s::detail::put(no_stop,16,0u,8);REQUIRE(!d::validate(no_stop));
 auto extra=diagnostic;extra.push_back(std::byte{0});REQUIRE(!d::validate(extra));
 auto before=diagnostic;REQUIRE(!d::encode({17,3,1,121,frames},diagnostic));REQUIRE(before==diagnostic);
 frames[2].thread=1;REQUIRE(!d::encode({17,2,1,121,frames},diagnostic));frames[2].thread=8;
 std::vector<std::byte> payload{std::byte{0},std::byte{0xff},std::byte{0x80},std::byte{0x42}};
 s::record r[]{ {s::component::diagnostic,s::completeness::partial,0,1,payload},
  {s::component::wasm_state,s::completeness::unavailable,0,7,{}},
  {s::component::ros2,s::completeness::complete,99,1,payload} };
 auto platform=s::native_platform();REQUIRE(s::detail::valid(platform));
 for(unsigned arch=1;arch<=5;++arch)for(auto order:{s::byte_order::little,s::byte_order::big})
 {
  platform.arch=static_cast<s::architecture>(arch);platform.pointer_bits=(arch==1||arch==3)?64:32;platform.endian=order;
  platform.product_version={2,0,3,0};platform.logical_abi=7;platform.continuation_abi=2;platform.features=0x3ffff;
  std::vector<std::byte> data;
  auto encoded=s::encode({platform,r},&data,append);REQUIRE(encoded&&encoded.bytes==data.size());
  s::decoded decoded;REQUIRE(s::decode(data,decoded));REQUIRE(decoded.origin==platform&&decoded.records.size()==3);
  REQUIRE(decoded.records[0].payload.size()==payload.size()&&decoded.records[2].id==99);
  REQUIRE(!decoded.grants_restore_authority());
  for(std::size_t cut=0;cut<data.size();++cut){s::decoded old;old.origin=platform;REQUIRE(!s::decode(std::span(data).first(cut),old));REQUIRE(old.origin==platform&&old.records.empty());}
  for(std::size_t at=0;at<data.size();++at){auto bad=data;bad[at]^=std::byte{1};s::decoded d;REQUIRE(!s::decode(bad,d));}
  auto bad=data;s::detail::put(bad,56,UINT64_MAX,8);resign(bad);REQUIRE(!s::decode(bad,decoded));
  bad=data;s::detail::put(bad,s::header_size+24,UINT64_MAX,8);resign(bad);REQUIRE(!s::decode(bad,decoded));
  bad=data;bad[s::header_size+3]=std::byte{1};resign(bad);REQUIRE(!s::decode(bad,decoded));
  bad=data;bad[23]=std::byte{1};resign(bad);REQUIRE(!s::decode(bad,decoded));
  bad=data;bad[21]=std::byte{16};resign(bad);REQUIRE(!s::decode(bad,decoded));
  bad=data;bad[22]=std::byte{1};resign(bad);REQUIRE(s::decode(bad,decoded).status==s::error::disclosure_violation);
  std::vector<std::byte> forbidden;REQUIRE(s::encode({platform,r,s::visibility::diagnostic},&forbidden,append).status==s::error::disclosure_violation);REQUIRE(forbidden.empty());
  s::stream_record stream[]{{r[0].kind,r[0].capture,0,1,payload.size(),&payload,transfer},
   {r[1].kind,r[1].capture,0,7,0,nullptr,nullptr},{r[2].kind,r[2].capture,99,1,payload.size(),&payload,transfer}};
  std::vector<std::byte> streamed;REQUIRE(s::encode_stream(platform,stream,&streamed,append));REQUIRE(streamed==data);
  stream[0].bytes=3;streamed.clear();REQUIRE(!s::encode_stream(platform,stream,&streamed,append));
  REQUIRE(!s::decode(streamed,decoded));stream[0].bytes=5;streamed.clear();REQUIRE(!s::encode_stream(platform,stream,&streamed,append));
  REQUIRE(!s::decode(streamed,decoded));
  std::size_t calls{};auto reject=[](void* p,std::span<std::byte const>)noexcept{++*static_cast<std::size_t*>(p);return false;};
  REQUIRE(s::encode({platform,r},&calls,reject).status==s::error::write_failed&&calls==1);
  calls=0;s::record duplicate[]{r[0],r[0]};REQUIRE(s::encode({platform,duplicate},&calls,reject).status==s::error::duplicate_record&&calls==0);
  auto quota=s::bounds{};quota.bytes=data.size()-1;REQUIRE(!s::encode({platform,r},&calls,reject,quota));
  if(argc==2&&arch==1&&order==s::byte_order::little){auto* file=std::fopen(argv[1],"wb");REQUIRE(file);REQUIRE(std::fwrite(data.data(),1,data.size(),file)==data.size());REQUIRE(!std::fclose(file));}
 }
 std::printf("PASS snapshot archive %u checks; native architecture=%u pointer_bits=%u\n",checks,unsigned(s::native_platform().arch),s::native_platform().pointer_bits);
}
