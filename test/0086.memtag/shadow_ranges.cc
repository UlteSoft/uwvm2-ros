#include <array>
#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <uwvm2/extension/memtag/shadow.h>

namespace mt = uwvm2::extension::memtag;
static void require(bool value) { if(!value) { ::std::abort(); } }

template<typename Address>
void exercise()
{
    using view = mt::shadow_view<Address>;
    ::std::array<::std::uint8_t, 1024> first{};
    ::std::array<::std::uint8_t, 1024> second{};
    view owner{first.data(), first.size(), first.size() * 16u};
    view alias{first.data(), first.size(), first.size() * 16u};
    view independent{second.data(), second.size(), second.size() * 16u};
    auto const tagged{view::insert(32u, 3u)};
    require(static_cast<bool>(owner.store(tagged, 128u)));
    require(static_cast<bool>(alias.check(tagged, 0u, 128u)));
    require(independent.check(tagged, 0u, 128u).error == mt::access_error::tag_mismatch);
    require(owner.check(32u, 0u, 128u).error == mt::access_error::tag_mismatch);
    require(owner.load(32u).allocation_tag == 3u);

    // Same first/last tags must not conceal an invalid interior granule.
    first[5] = 7u;
    auto mismatch{owner.check(tagged, 0u, 128u)};
    require(mismatch.error == mt::access_error::tag_mismatch);
    require(mismatch.mismatch_granule == 80u && mismatch.pointer_tag == 3u && mismatch.allocation_tag == 7u);
    require(owner.check(view::insert(79u, 3u), 0u, 2u).error == mt::access_error::tag_mismatch);
    auto const snapshot{first};
    require(owner.store(view::insert(16380u, 9u), 8u).error == mt::access_error::out_of_bounds);
    require(first == snapshot);
    view short_shadow{first.data(), 2u, 16384u};
    require(short_shadow.store(tagged, 16u).error == mt::access_error::shadow_out_of_bounds);
    require(first == snapshot);
    view absent{nullptr, 0u, 16384u};
    require(absent.check(0u, 0u, 1u).error == mt::access_error::shadow_out_of_bounds);
    require(static_cast<bool>(absent.check(16384u, 0u, 0u)));
    require(absent.check(16385u, 0u, 0u).error == mt::access_error::out_of_bounds);
    require(owner.check(tagged, ::std::numeric_limits<::std::uint64_t>::max(), 2u).error == mt::access_error::out_of_bounds);
    require(owner.check(tagged, 0u, ::std::numeric_limits<::std::uint64_t>::max()).error == mt::access_error::out_of_bounds);
    require(owner.check(tagged, ::std::uint64_t{1u} << 32u, 1u).error == mt::access_error::out_of_bounds);
    view too_large{first.data(), first.size(), view::address_capacity + 1u};
    require(too_large.check(0u, 0u, 0u).error == mt::access_error::memory_not_representable);
    view end_only{nullptr, 0u, view::address_capacity};
    require(static_cast<bool>(end_only.check(view::address_mask, 1u, 0u)));
    require(end_only.check(view::address_mask, 2u, 0u).error == mt::access_error::out_of_bounds);
    require(end_only.check(view::address_mask, 1u, 1u).error == mt::access_error::out_of_bounds);

    // Exercise both sides of the block-setup threshold at every alignment.
    // Every covered granule, including the scalar tail, must identify a mismatch.
    for(::std::uint64_t length : {127u, 128u, 129u})
    {
        for(::std::uint64_t shift{}; shift != 16u; ++shift)
        {
            auto const start{64u + shift};
            first.fill(3u);
            auto const pointer{view::insert(static_cast<Address>(start), 3u)};
            require(static_cast<bool>(owner.check(pointer, 0u, length)));
            for(auto byte{start & ~::std::uint64_t{15u}}; byte < start + length; byte += 16u)
            {
                first[byte / 16u] = 7u;
                auto const failed{owner.check(pointer, 0u, length)};
                require(failed.error == mt::access_error::tag_mismatch &&
                        failed.mismatch_granule == byte && failed.allocation_tag == 7u);
                first[byte / 16u] = 3u;
            }
        }
    }

    // Independently compute each intersecting granule for varied alignments,
    // lengths and mismatches; in particular, exercise eight-granule fast paths.
    ::std::uint64_t state{0x482a930711d89full};
    auto random{[&]() { state ^= state << 13u; state ^= state >> 7u; state ^= state << 17u; return state; }};
    for(unsigned iteration{}; iteration != 40000u; ++iteration)
    {
        auto const tag{static_cast<::std::uint8_t>(random() & view::maximum_tag)};
        first.fill(tag);
        auto const start{random() % 16385u};
        auto const length{random() % (16385u - start)};
        if(iteration % 3u != 0u)
        {
            auto const changed{random() % first.size()};
            first[changed] = static_cast<::std::uint8_t>(tag ^ 1u);
        }
        bool accepted{true};
        ::std::uint64_t bad{};
        if(length != 0u)
        {
            for(auto byte{start & ~::std::uint64_t{15u}}; byte < start + length; byte += 16u)
            {
                if(first[byte / 16u] != tag) { accepted = false; bad = byte; break; }
            }
        }
        auto const result{owner.check(view::insert(static_cast<Address>(start), tag), 0u, length)};
        require(static_cast<bool>(result) == accepted);
        if(!accepted) { require(result.error == mt::access_error::tag_mismatch && result.mismatch_granule == bad); }
    }
}

int main()
{
    exercise<::std::uint32_t>();
    exercise<::std::uint64_t>();
    ::std::array<::std::atomic<::std::uint8_t>, 32> shared{};
    mt::shadow_view<::std::uint32_t, true> view{shared.data(), shared.size(), 512u};
    auto const pointer{decltype(view)::insert(15u, 11u)};
    require(static_cast<bool>(view.store(pointer, 258u)));
    require(static_cast<bool>(view.check(pointer, 0u, 258u)));
    shared[7].store(9u, ::std::memory_order_relaxed);
    auto const mismatch{view.check(pointer, 0u, 258u)};
    require(mismatch.error == mt::access_error::tag_mismatch && mismatch.mismatch_granule == 112u);
    return 0;
}
