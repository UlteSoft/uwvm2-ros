// Test unpublished global admission with hostile entropy, aliases and raw neighbors.
#include <array>
#include <cassert>
#include <cstdint>
#include <uwvm2/runtime/memtag/static_allocation.h>

namespace mt = uwvm2::extension::memtag;
namespace rt = uwvm2::runtime::memtag;

template<class Address, bool Shared>
static void check_owner()
{
    constexpr auto width = sizeof(Address) == 4 ? mt::address_width::i32 : mt::address_width::i64;
    constexpr unsigned slots = 257;
    constexpr unsigned bytes = (slots + 2u) * 16u;
    mt::shadow_storage owner;
    assert(owner.enable(width, Shared, mt::storage_backend::growable, bytes, bytes) == mt::storage_error::none);
    auto const view = owner.view<Address,Shared>(bytes);
    constexpr unsigned raw_color = 7;
    assert(view.store(view.insert(0,raw_color),16));
    std::array<std::uint8_t,slots> tags{};
    rt::tag_entropy_observer observer{nullptr, [](void*,std::uint8_t& value, bool(*)(std::uint8_t&) noexcept) noexcept
    { value=255;return true; }};
    rt::current_tag_entropy_observer=&observer;
    // Reserve both sides in a nonmonotone order, then publish in reverse.
    for(unsigned n=0;n<slots;++n)
    {
        auto const index=(n*127u)%slots;
        auto const address=(index+1u)*16u+3u;
        assert(rt::stage_static_allocation(owner,address,7,tags[index]));
        assert(view.load(address).allocation_tag==0);
        if(index<5) { assert(tags[index]!=raw_color); }
        std::uint8_t unchanged{99};
        assert(!rt::stage_static_allocation(owner,address+1u,1,unchanged) && unchanged==99);
    }
    for(unsigned i=slots;i-- > 0;)
    {
        for(unsigned j=i+1;j<slots && j<=i+5;++j) { assert(tags[i]!=tags[j]); }
        auto const address=(i+1u)*16u;
        assert(rt::write_static_allocation(owner,address+3u,7,tags[i]));
        assert(view.check(view.insert(address,tags[i]),0,16));
        if(i+1u<slots) { assert(!view.check(view.insert(address+15u,tags[i]),0,2)); }
    }
    rt::current_tag_entropy_observer=nullptr;
    std::uint8_t unchanged{99};
    assert(!rt::stage_static_allocation(owner,bytes-1u,2,unchanged) && unchanged==99);
    assert(!rt::stage_static_allocation(owner,0,1,unchanged) && unchanged==99);
    assert(!rt::stage_static_allocation(owner,0,0,unchanged) && unchanged==99);
    auto const spare=(slots+1u)*16u;
    rt::tag_entropy_observer failed{nullptr, [](void*,std::uint8_t&,bool(*)(std::uint8_t&) noexcept) noexcept { return false; }};
    rt::current_tag_entropy_observer=&failed;
    assert(!rt::stage_static_allocation(owner,spare,1,unchanged) && unchanged==99);
    rt::current_tag_entropy_observer=nullptr;
    assert(!owner.allocation_directory().exact(slots+1u,slots+1u));
}

int main()
{
    mt::managed_allocations directory;
    assert(directory.add_permanent(0,1,1));
    assert(directory.add_permanent(6,6,2));
    std::array<unsigned char,13> image{};
    image[0]=image[1]=image[6]=3;
    assert(!rt::bind_migrated_static_image(directory,image));
    assert(directory.exact(0,1)->tag==1 && directory.exact(6,6)->tag==2);
    image[6]=4; image[0]=0;
    assert(!rt::bind_migrated_static_image(directory,image));
    image[0]=image[1]=3; image[10]=4;
    assert(!rt::bind_migrated_static_image(directory,image));
    image[10]=0;
    assert(rt::bind_migrated_static_image(directory,image));
    assert(directory.exact(0,1)->tag==3 && directory.exact(6,6)->tag==4);
    assert(!rt::bind_migrated_static_image(directory,{image.data(),1}));
    check_owner<std::uint32_t,false>();
    check_owner<std::uint32_t,true>();
    check_owner<std::uint64_t,false>();
    check_owner<std::uint64_t,true>();
}
