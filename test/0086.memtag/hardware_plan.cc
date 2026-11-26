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
 unsigned const arm{bits-8u};
 for(unsigned k : {4u,5u,6u,7u})
 {
  mt::shadow_storage owner;
  CHECK(enable(owner,storage::reserved,k,backend::arm_mte)==error::unsupported_backend && !owner.enabled());
  CHECK(enable(owner,storage::reserved,k,backend::automatic)==error::none);
  CHECK(!owner.hardware_selected() && owner.tag_start_bit()==software);
 }
 for(unsigned saved : {software,bits-7u})
 {
  mt::shadow_storage restored;
  CHECK(enable(restored,storage::reserved,16,backend::arm_mte,saved)==error::unsupported_backend);
  CHECK(enable(restored,storage::reserved,16,backend::automatic,saved)==error::none);
  CHECK(!restored.hardware_selected() && restored.tag_start_bit()==saved);
 }
 mt::shadow_storage stable;
 bool const selected{mt::hardware_mte::backend_available(backend::arm_mte)};
 CHECK(enable(stable,storage::reserved,8,backend::automatic)==error::none);
 CHECK(stable.hardware_selected()==selected && !stable.hardware_active());
 CHECK(stable.tag_start_bit()==(selected ? arm : software));
 mt::shadow_storage restored;
 CHECK(enable(restored,storage::reserved,16,backend::automatic,arm)==error::none);
 CHECK(restored.hardware_selected()==selected && restored.tag_start_bit()==arm);
 mt::shadow_storage required;
 CHECK(enable(required,storage::reserved,8,backend::arm_mte,arm)==(selected ? error::none : error::unsupported_backend));
 for(unsigned k : {6u,7u,16u})
 {
  mt::shadow_storage intel;
  CHECK(enable(intel,storage::reserved,k,backend::x86_mtt)==error::unsupported_backend && !intel.enabled());
 }
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
 mt::shadow_storage disabled;
 CHECK(!disabled.enabled()&&!disabled.hardware_selected()&&!disabled.hardware_active());
 check_plan(mt::address_width::i32);
 check_plan(mt::address_width::i64);
 std::puts("PASS memory32/memory64 owner backend admission, saved layout, growth and move");
}
