// Allocation-free regression for finite maxima, fixed quotas, and both ends of the native address range.
#include <uwvm2/object/memory/linear/mmap_reservation.h>
namespace r = uwvm2::object::memory::linear::mmap_reservation;
constexpr r::bytes_type page{65536}, os{4096}, tib{1ull << 40}, gib{1ull << 30};
constexpr auto fixed{r::bounds(64, false, 0, UINT64_MAX, page, os)};
constexpr auto wide{r::bounds(64, true, 0, UINT64_MAX, page, os)};
constexpr auto small{r::bounds(64, true, 0, 2, page, os)};
constexpr auto large_min{r::bounds(64, true, 12*tib/page, UINT64_MAX, page, os)};
static_assert(r::default_pool(64) == 16*tib && r::default_pool(32) == 2*gib && r::default_pool(16) == 65536);
static_assert(fixed.minimum_span == 8*gib + 69632 && fixed.minimum_span == fixed.maximum_span);
static_assert(small.minimum_span == fixed.minimum_span && small.maximum_data == 2*page);
static_assert(r::round_up(UINT64_MAX, os) == 0 && !r::bounds(32, true, UINT64_MAX, UINT64_MAX, page, os));
static_assert(r::make_layout(32, false, UINT64_C(1)<<32, UINT64_MAX, page, os).reserved < (UINT64_C(1)<<32));
constexpr r::bytes_type quota{16*gib}, one{1}, odd{3};
static_assert(!r::bounds(64, false, 0, 1, page, os, &one));
static_assert(!r::bounds(64, true, 1, 1, page, os, &one));
static_assert(!r::bounds(64, true, 0, 1, page, os, &odd));
static_assert(r::bounds(64, true, 0, 1, page, os, &quota).maximum_data == page);
static_assert(r::bounds(32, false, 0, 1, page, os, &one).maximum_data == 1u);
// One accessible byte occupies a whole native page in addition to the guard.
static_assert(r::outer_guard(1, 16) == 80u);
static_assert(r::bounds(16, true, 0, 1, 1, 16, &one).minimum_span == 16u + 80u);
constexpr auto tagged32{r::bounds(64,false,0,UINT64_MAX,page,os,nullptr,true)};
constexpr auto tagged64{r::bounds(64,true,0,UINT64_MAX,page,os,nullptr,true)};
static_assert(tagged32.maximum_data == (1ull << 28) && tagged32.maximum_span == fixed.maximum_span);
static_assert(tagged64.maximum_span == (1ull << 48) && tagged64.maximum_data < tagged64.maximum_span);
constexpr bool shares()
{
    r::census s{{16*tib, false}};
    s.add(wide, true);
    if(s.common_share() != 16*tib) { return false; }
    s.add(wide, true);
    if(s.common_share() != 8*tib) { return false; }
    s.add(wide, true);
    if(s.common_share() != 4*tib) { return false; }
    r::census mixed{{16*tib, false}};
    mixed.add(wide,true,2); mixed.add(small,true,3); mixed.add(fixed,false);
    if(mixed.common_share() != 4*tib || mixed.participants != 5) { return false; }
    r::census minima{{16*tib, false}};
    minima.add(large_min,true); minima.add(wide,true);
    if(minima.common_share() != 2*tib || !minima.fits_minimum()) { return false; }
    r::census threshold{{16*tib, false}};
    threshold.add(small,true,2047);
    if(!threshold.fits_minimum()) { return false; }
    threshold.add(small,true);
    if(threshold.fits_minimum()) { return false; }
    r::byte_budget whole{0,true};
    if(!whole.take(UINT64_MAX) || whole.bytes != 1u || !whole.take(1u) || whole.take(1u)) { return false; }
    r::byte_budget product{0,true};
    if(product.take(1ull<<63, 3) || !product.full_address64 || !product.take(1ull<<63,2) || product.bytes != 0u) { return false; }
    return true;
}
static_assert(shares());
int main() {}
