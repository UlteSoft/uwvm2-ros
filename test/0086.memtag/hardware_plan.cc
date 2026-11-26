#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/extension/memtag/storage.h>
#include <cstdio>
#include <cstdlib>
#include <optional>
#include <utility>
namespace mt=uwvm2::extension::memtag;
#define CHECK(...) do {if(!(__VA_ARGS__)){std::fprintf(stderr,"hardware plan failure %u\n",__LINE__);std::abort();}}while(false)
using backend = mt::owner_backend;
using storage = mt::storage_backend;
using error = mt::storage_error;

static void check_plan(mt::address_width width)
{
 unsigned const bits{static_cast<unsigned>(width)};
 unsigned const software{bits-4u};
 auto enable = [&](mt::shadow_storage& owner, storage backing, unsigned k, backend requested,
                   std::optional<unsigned> saved = {})
 { return owner.enable(width,false,backing,65536,131072,k,requested,saved); };
 mt::shadow_storage moving;
 CHECK(enable(moving,storage::growable,8,backend::automatic)==error::none);
 CHECK(!moving.hardware_selected() && moving.tag_start_bit()==software);
#if defined(UWVM_SUPPORT_MMAP)
 bool const selected{mt::hardware_mte::backend_available(backend::arm_mte)};
 CHECK(!mt::hardware_mte::backend_available(backend::x86_mtt));
 for(unsigned k : {4u,5u,6u,7u,8u,9u,16u})
 {
  unsigned const arm{bits-(k >= 8u ? 8u : 4u)};
  unsigned const intel{bits-(k >= 7u ? 7u : 4u)};
  auto const mask{(UINT64_C(1) << (bits-k))-1u};
  CHECK(mt::arm_mte_layout.geometry(width,k).start_bit==arm && mt::arm_mte_layout.geometry(width,k).address_mask==mask);
  CHECK(mt::intel_mtt_layout.geometry(width,k).start_bit==intel && mt::intel_mtt_layout.geometry(width,k).address_mask==mask);
  mt::shadow_storage fresh,required,sw,mtt;
  auto const maximum{(mask+1u < 131072u) ? mask+1u : 131072u};
  CHECK(enable(fresh,storage::reserved,k,backend::automatic)==error::none);
  CHECK(fresh.hardware_selected()==selected && !fresh.hardware_active());
  CHECK(fresh.tag_start_bit()==(selected ? arm : software) && fresh.address_mask()==mask && fresh.maximum_memory_bytes()==maximum);
  CHECK(enable(required,storage::reserved,k,backend::arm_mte)==(selected ? error::none : error::unsupported_backend));
  CHECK(required.enabled()==selected);
  CHECK(enable(sw,storage::reserved,k,backend::software)==error::none && !sw.hardware_selected());
  CHECK(sw.tag_start_bit()==software && sw.address_mask()==mask);
  CHECK(enable(mtt,storage::reserved,k,backend::x86_mtt)==error::unsupported_backend && !mtt.enabled());
 }
 for(unsigned start=bits-16u;start<=software;++start)
 {
  mt::shadow_storage restored,required;
  CHECK(enable(restored,storage::reserved,16,backend::automatic,start)==error::none);
  CHECK(restored.hardware_selected()==selected && restored.tag_start_bit()==start);
  CHECK(enable(required,storage::reserved,16,backend::arm_mte,start)==(selected ? error::none : error::unsupported_backend));
  if(selected) { CHECK(required.hardware_selected() && required.tag_start_bit()==start); }
 }
 mt::shadow_storage invalid;
 CHECK(enable(invalid,storage::reserved,3,backend::arm_mte)==error::invalid_configuration);
 CHECK(enable(invalid,storage::reserved,4,backend::automatic,software-1u)==error::invalid_configuration);
 CHECK(enable(invalid,storage::reserved,8,backend::automatic,software+1u)==error::invalid_configuration);
 mt::shadow_storage stable;
 CHECK(enable(stable,storage::reserved,8,backend::automatic)==error::none);
 // Compilation can select the immutable plan before active data is copied.
 auto const plan_before=stable.hardware_selected();
 CHECK(stable.prepare_growth(131072)==mt::storage_error::none);
 CHECK(stable.hardware_selected()==plan_before&&!stable.hardware_active());
 mt::shadow_storage moved(std::move(stable));
 CHECK(moved.hardware_selected()==plan_before&&!stable.hardware_selected());
 mt::shadow_storage assigned;assigned=std::move(moved);
 CHECK(assigned.hardware_selected()==plan_before&&!moved.hardware_selected());
 assigned.reset();CHECK(!assigned.hardware_selected()&&!assigned.hardware_active());
#endif
}
int main()
{
 static_assert(mt::module_memory_policy{}.backend==backend::automatic);
 namespace adm=uwvm2::extension::adm;
 static_assert(adm::maximum_pages(false,4,true,16)==4096u && adm::maximum_pages(false,8,true,16)==256u);
 static_assert(adm::maximum_pages(true,4,true,16)==(UINT64_C(1)<<32u) && adm::maximum_pages(true,8,true,16)==(UINT64_C(1)<<32u));
 mt::shadow_storage disabled;
 CHECK(!disabled.enabled()&&!disabled.hardware_selected()&&!disabled.hardware_active());
 check_plan(mt::address_width::i32);
 check_plan(mt::address_width::i64);
 std::puts("PASS memory32/memory64 owner backend admission, saved layout, growth and move");
}
