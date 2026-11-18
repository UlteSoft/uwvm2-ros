#include <array>
#include <cassert>
#include <cstdint>
#include <limits>
#include <uwvm2/extension/memtag/allocation_neighborhood.h>

namespace mt = uwvm2::extension::memtag;

static void check_distribution(mt::address_width width, bool physical, unsigned pattern)
{
    const unsigned limit = width == mt::address_width::i32 ? 16u : 256u;
    mt::allocation_neighborhood policy{width, physical};
    std::array<bool, 256> legal{};
    for (unsigned i = 1; i < limit; ++i) { legal[i] = !physical || (i & 15u) != 0u; }
    for (unsigned i = 1; i < limit; ++i)
    {
        if ((pattern == 1 && !(i & 1u)) || (pattern == 2 && i % 7u == 1u) ||
            (pattern == 3 && i != 7u) || pattern == 4)
        {
            policy.forbid(static_cast<std::uint8_t>(i));
            for (unsigned j = 0; j < limit; ++j)
                if (physical ? (i & 15u) == (j & 15u) : i == j) { legal[j] = false; }
        }
    }
    unsigned count{}, rejections{};
    for (bool value : legal) { count += value; }
    std::array<unsigned, 256> hits{};
    for (unsigned bits = 0; bits < 65536; ++bits)
    {
        std::uint8_t result{91};
        auto selected = policy.select(static_cast<std::uint16_t>(bits), result);
        if (!count) { assert(selected == mt::tag_selection::no_color && result == 91); }
        else if (selected == mt::tag_selection::retry_entropy) { assert(result == 91); ++rejections; }
        else { assert(selected == mt::tag_selection::selected && legal[result]); ++hits[result]; }
    }
    if (count) { assert(rejections == 65536u % count); }
    for (unsigned i = 0; i < limit; ++i) { assert(hits[i] == (legal[i] ? 65536u / count : 0u)); }
}

int main()
{
    for (auto width : {mt::address_width::i32, mt::address_width::i64})
        for (bool physical : {false, true})
            for (unsigned pattern{}; pattern < 5; ++pattern)
                check_distribution(width, physical, pattern);
    constexpr auto max = std::numeric_limits<std::uint64_t>::max();
    static_assert(mt::allocations_near(0, 0, 5, 5));
    static_assert(!mt::allocations_near(0, 0, 6, 6));
    static_assert(mt::allocations_near(max, max, max - 8, max - 5));
    static_assert(!mt::allocations_near(0, 1, max, max));
    static_assert(!mt::allocations_near(0, 6, 4, 9));
    for (unsigned size = 1; size < 32; ++size)
        for (unsigned first = 0; first < size; ++first)
            for (unsigned last = first; last < size; ++last)
            {
                std::array<unsigned, 32> visits{};
                assert(mt::visit_allocation_neighbors(first, last, size, [&](auto i) { assert(i < size); ++visits[i]; return true; }));
                for (unsigned i = 0; i < size; ++i)
                    assert(visits[i] == unsigned(mt::allocations_near(first, last, i, i)));
            }
    unsigned visits{};
    assert(!mt::visit_allocation_neighbors(1, 2, 3, [&](auto) { ++visits; return false; }));
    assert(visits == 1);
    assert(!mt::visit_allocation_neighbors(0, 3, 3, [](auto) { assert(false); return true; }));
}
