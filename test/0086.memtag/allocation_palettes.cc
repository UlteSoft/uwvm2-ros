// Check every software palette size and every possible set of low-nibble exclusions.
#include <array>
#include <cassert>
#include <cstdint>
#include <uwvm2/extension/memtag/allocation_neighborhood.h>

namespace mt=uwvm2::extension::memtag;

static void check_word(mt::allocation_neighborhood const& policy,
                       std::array<unsigned char,15> const& ordered, unsigned count, unsigned word)
{
    unsigned char tag{201};
    auto const result=policy.select(static_cast<std::uint16_t>(word),tag);
    if(count==0) { assert(result==mt::tag_selection::no_color && tag==201); return; }
    unsigned const product=word*count;
    if(product%65536u < 65536u%count)
    { assert(result==mt::tag_selection::retry_entropy && tag==201); return; }
    assert(result==mt::tag_selection::selected && tag==ordered[product/65536u]);
}

int main()
{
    // Spread each whitelist across both halves of the four-bit palette.
    for(unsigned count=1;count<=15;++count)
    {
        mt::allocation_neighborhood policy{mt::address_width::i64};
        std::array<unsigned char,15> ordered{};
        unsigned size{};
        for(unsigned value=1;value<=15;++value)
        {
            if(((value-1)*8u)%15u < count) { ordered[size++]=static_cast<unsigned char>(value); }
            else { policy.forbid(static_cast<unsigned char>(value)); }
        }
        assert(size==count);
        for(unsigned word=0;word<65536u;++word) { check_word(policy,ordered,size,word); }
    }

    // For every possible whitelist, verify the boundaries of every rank bucket.
    for(unsigned mask=0;mask<32768u;++mask)
    {
        for(bool physical : {false,true})
        {
            mt::allocation_neighborhood policy{physical?mt::address_width::i64:mt::address_width::i32,physical};
            std::array<unsigned char,15> ordered{};
            for(unsigned nibble=1;nibble<=15;++nibble)
                if(!(mask & (1u<<(nibble-1)))) { policy.forbid(static_cast<unsigned char>(nibble)); }
            unsigned count{};
            for(unsigned value=1;value<=15u;++value)
            {
                if(mask & (1u<<(value-1))) { ordered[count++]=static_cast<unsigned char>(value); }
            }
            if(!count) { check_word(policy,ordered,count,0); check_word(policy,ordered,count,65535u); }
            for(unsigned rank=0;rank<count;++rank)
            {
                unsigned const first=(rank*65536u+count-1u)/count;
                unsigned const last=((rank+1u)*65536u-1u)/count;
                check_word(policy,ordered,count,first);
                check_word(policy,ordered,count,first+1u);
                check_word(policy,ordered,count,last);
            }
        }
    }
}
