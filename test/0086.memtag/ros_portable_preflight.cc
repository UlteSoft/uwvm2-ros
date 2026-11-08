// Real portable-host preflight with a modeled authenticated normalization API.
// Actual runtime identity/tag admission is qualified by runtime bridge tests.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/extension/memtag/shadow.h>
#include <uwvm2/uwvm/imported/ros/portable_host.h>
#include <array>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
namespace mt=uwvm2::extension::memtag;
namespace host=uwvm2::uwvm::imported::ros::portable_host;
namespace model=uwvm2::imported::ros::portable;
static std::array<std::uint8_t,64> tags{};
static std::array<std::byte,1024> bytes{};
struct write_record { std::uint64_t raw;std::size_t size; };
static std::vector<write_record> writes;
static bool enabled{},wide{};
static unsigned normalized{},checks{};
static void check(bool value,unsigned line) { ++checks;if(!value) { std::fprintf(stderr,"portable tag preflight failure %u\n",line);std::abort(); } }
#define CHECK(...) check(static_cast<bool>((__VA_ARGS__)),__LINE__)
namespace uwvm2::runtime::lib
{
 extern "C++" bool preload_memory_normalize_range_host_api(std::size_t index,std::uint_least64_t raw,std::size_t size,std::uint64_t& canonical) noexcept
 {
  ++normalized;canonical=0;if(index!=0) { return false; }
  if(!enabled) { if(raw>bytes.size() || size>bytes.size()-raw) { return false; }canonical=raw;return true; }
  if(!wide && raw>UINT32_MAX) { return false; }
  auto result=wide ? mt::shadow_view<std::uint64_t>{tags.data(),tags.size(),bytes.size()}.check(raw,0,size) :
      mt::shadow_view<std::uint32_t>{tags.data(),tags.size(),bytes.size()}.check(static_cast<std::uint32_t>(raw),0,size);
  if(!result) { return false; }canonical=result.address;return true;
 }
 extern "C++" bool preload_memory_read_host_api(std::size_t index,std::uint_least64_t raw,void* out,std::size_t size) noexcept
 { std::uint64_t at{};if(!preload_memory_normalize_range_host_api(index,raw,size,at)) { return false; }if(size) { std::memcpy(out,bytes.data()+at,size); }return true; }
 extern "C++" bool preload_memory_write_host_api(std::size_t index,std::uint_least64_t raw,void const* in,std::size_t size) noexcept
 { std::uint64_t at{};if(!preload_memory_normalize_range_host_api(index,raw,size,at)) { return false; }writes.push_back({raw,size});if(size) { std::memcpy(bytes.data()+at,in,size); }return true; }
}
static std::uint64_t pointer(std::uint64_t at,unsigned tag=5) { return enabled ? at|(std::uint64_t{tag}<<(wide?56u:28u)) : at; }
static void reset() { tags.fill(5);bytes.fill(std::byte{0x7f});writes.clear();normalized=0; }
int main()
{
 uwvm2::runtime::lib::ros_caller_source_facts facts{};facts.memory_bytes=bytes.size();
 for(unsigned mode{};mode!=3;++mode)
 {
  enabled=mode!=0;wide=mode==2;reset();
  CHECK(host::open_preflight(facts,0,pointer(16),12,pointer(64))==model::error::ok);CHECK(writes.empty());
  CHECK(host::open_preflight(facts,0,pointer(16),12,pointer(20))==model::error::malformed);CHECK(writes.empty());
  normalized=0;CHECK(host::open_preflight(facts,1,pointer(16),12,pointer(64))==model::error::malformed);CHECK(normalized==0);
  CHECK(host::open_preflight(facts,0,pointer(16),256,pointer(64))==model::error::malformed);
  CHECK(host::output_preflight(facts,0,pointer(128),96,pointer(256),96,{pointer(16),32})==model::error::ok);
  CHECK(writes.size()==2 && writes[0].raw==pointer(128) && writes[0].size==0 && writes[1].raw==pointer(256) && writes[1].size==8);
  CHECK(bytes[256]==std::byte{96});
  reset();CHECK(host::output_preflight(facts,0,pointer(128),32,pointer(256),96)==model::error::too_small);
  CHECK(writes.size()==2 && bytes[256]==std::byte{96} && bytes[128]==std::byte{0x7f});
  reset();CHECK(host::output_preflight(facts,0,pointer(128),96,pointer(136),96)==model::error::malformed);CHECK(writes.empty());
  CHECK(host::output_preflight(facts,0,pointer(128),96,pointer(256),96,{pointer(144),16})==model::error::malformed);CHECK(writes.empty());
  CHECK(host::output_preflight(facts,0,pointer(128),96,pointer(256),96,{pointer(252),16})==model::error::malformed);CHECK(writes.empty());
  CHECK(host::output_preflight(facts,0,pointer(1008),32,pointer(256),96)==model::error::malformed);CHECK(writes.empty());
  CHECK(host::output_preflight(facts,0,pointer(128),UINT64_MAX,pointer(256),96)==model::error::malformed);CHECK(writes.empty());
  if(enabled)
  {
   CHECK(host::open_preflight(facts,0,pointer(16,4),12,pointer(64))==model::error::malformed);CHECK(writes.empty());
   CHECK(host::output_preflight(facts,0,pointer(128,4),96,pointer(256),96)==model::error::malformed);CHECK(writes.empty());
   CHECK(host::output_preflight(facts,0,pointer(128),96,pointer(256,4),96)==model::error::malformed);CHECK(writes.empty());
   CHECK(host::output_preflight(facts,0,pointer(128),96,pointer(256),96,{pointer(16,4),32})==model::error::malformed);CHECK(writes.empty());
   tags[13]=6;CHECK(host::output_preflight(facts,0,pointer(128),96,pointer(256),96)==model::error::malformed);CHECK(writes.empty());
   CHECK(bytes[256]==std::byte{0x7f});
  }
 }
 std::printf("portable ROS tag preflight: %u checks passed\n",checks);
}
