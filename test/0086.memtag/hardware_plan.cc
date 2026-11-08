#include <uwvm2/extension/memtag/storage.h>
#include <cstdio>
#include <cstdlib>
#include <utility>
namespace mt=uwvm2::extension::memtag;
#define CHECK(...) do {if(!(__VA_ARGS__)){std::fprintf(stderr,"hardware plan failure %u\n",__LINE__);std::abort();}}while(false)
int main(){
 mt::shadow_storage disabled;
 CHECK(!disabled.enabled()&&!disabled.hardware_selected()&&!disabled.hardware_active());
 mt::shadow_storage moving;
 CHECK(moving.enable(mt::address_width::i32,false,mt::storage_backend::growable,65536,131072)==mt::storage_error::none);
 CHECK(!moving.hardware_selected()&&!moving.hardware_active());
#if defined(__linux__) && defined(__aarch64__)
 constexpr bool eligible=true;
#else
 constexpr bool eligible=false;
#endif
 mt::shadow_storage stable;
 CHECK(stable.enable(mt::address_width::i32,false,mt::storage_backend::reserved,65536,131072)==mt::storage_error::none);
 bool const selected=eligible&&mt::hardware_mte::supported();
 CHECK(stable.hardware_selected()==selected&&!stable.hardware_active());
 // Compilation can select the immutable plan before active data is copied.
 auto const plan_before=stable.hardware_selected();
 CHECK(stable.prepare_growth(131072)==mt::storage_error::none);
 CHECK(stable.hardware_selected()==plan_before&&!stable.hardware_active());
 mt::shadow_storage moved(std::move(stable));
 CHECK(moved.hardware_selected()==plan_before&&!stable.hardware_selected());
 mt::shadow_storage assigned;assigned=std::move(moved);
 CHECK(assigned.hardware_selected()==plan_before&&!moved.hardware_selected());
 assigned.reset();CHECK(!assigned.hardware_selected()&&!assigned.hardware_active());
 std::printf("PASS immutable hardware plan: selected=%u, preactivation/grow/move/reset/software owner\n",selected);
}
