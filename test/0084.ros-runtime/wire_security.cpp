/** @file
 * @brief Canonical provider framing rejects malformed/truncated/resource-heavy records.
 * @details The generated fixtures independently encode every proposed operation
 * from the catalogue. These tests exercise real parser bounds and output
 * canaries, not provider authority or native effect semantics.
 */
#include <uwvm2/imported/ros/scene/schema_contract.h>
#include "wire_cases.h"
#include <array>
#include <bit>
#include <cassert>
#include <iostream>
#include <limits>
#include <vector>
namespace w=uwvm2::imported::ros::scene::wire;
namespace s=uwvm2::imported::ros::scene::schema_contract;
static constexpr s::limits allowance{65536,4096,4096,64};

static void catalogue_records()
{
    for(auto const& item:wire_cases)
    {
        assert(s::validate(item.data,item.shape,allowance));
        for(std::size_t length{};length<item.data.size();++length)
            assert(!s::validate(item.data.first(length),item.shape,allowance));
        std::vector<std::uint8_t> extended(item.data.begin(),item.data.end());extended.push_back(0);
        assert(!s::validate(extended,item.shape,allowance));
        auto limits=allowance;limits.nodes=0;assert(!s::validate(item.data,item.shape,limits));
        if(!item.data.empty()){limits=allowance;limits.bytes=item.data.size()-1;assert(!s::validate(item.data,item.shape,limits));}
    }
}
static void primitive_roundtrip_and_canaries()
{
    std::array<std::uint8_t,160> storage;storage.fill(0xa5);
    w::writer out{{storage.data()+8,144}};
    assert(out.u32(0x89abcdefu)&&out.u64(UINT64_MAX)&&out.i64(INT64_MIN));
    auto nan=std::bit_cast<double>(std::uint64_t{0x7ff8123456789abc});
    assert(out.f64(nan)&&out.boolean(true)&&out.text("\xe4\xb8\xad\xe6\x96\x87")&&out.blob({}));
    w::reader in{out.written()};std::uint32_t small{};std::uint64_t large{};std::int64_t signed_value{};
    double number{};bool boolean{};std::string_view text;w::bytes bytes;
    assert(in.u32(small)&&small==0x89abcdefu&&in.u64(large)&&large==UINT64_MAX&&in.i64(signed_value)&&signed_value==INT64_MIN);
    assert(in.f64(number)&&std::bit_cast<std::uint64_t>(number)==0x7ff8123456789abc);
    assert(in.boolean(boolean)&&boolean&&in.text(text)&&text=="\xe4\xb8\xad\xe6\x96\x87"&&in.blob(bytes)&&bytes.empty()&&in.done());
    for(unsigned i{};i<8;++i)assert(storage[i]==0xa5&&storage[152+i]==0xa5);
    auto saved=storage;w::writer short_output{{storage.data()+8,7}};
    assert(!short_output.u64(7)&&!short_output.good()&&storage==saved);
    std::uint64_t unchanged=17;w::reader short_input{{storage.data(),7}};
    assert(!short_input.u64(unchanged)&&unchanged==17&&!short_input.good());
    std::array<std::uint8_t,8> bad_bool{2};w::reader invalid{bad_bool};boolean=true;
    assert(!invalid.boolean(boolean)&&boolean&&!invalid.good());
    std::array<std::uint8_t,32> overlap{};for(unsigned i{};i<16;++i)overlap[i]=static_cast<std::uint8_t>(i+1);
    w::writer prefixed{overlap};assert(prefixed.blob({overlap.data(),16}));
    w::reader back{prefixed.written()};assert(back.blob(bytes)&&bytes.size()==16&&back.done());
    for(unsigned i{};i<16;++i)assert(bytes[i]==i+1);
    auto measured=w::writer::measuring(24);assert(measured.blob(bytes)&&measured.size()==24&&measured.written().empty());
    assert(!measured.boolean(false)&&!measured.good());
}
static void canonical_text_and_selectors()
{
    for(auto bad:std::array{std::vector<std::uint8_t>{0xc0,0x80},std::vector<std::uint8_t>{0xed,0xa0,0x80},
        std::vector<std::uint8_t>{0xf4,0x90,0x80,0x80},std::vector<std::uint8_t>{0x80},std::vector<std::uint8_t>{0xe2,0x82}})
    {
        assert(!w::utf8(bad));std::array<std::uint8_t,32> storage{};w::writer out{storage};assert(out.blob(bad));
        w::reader in{out.written()};std::string_view value="canary";assert(!in.text(value)&&value=="canary");
    }
    std::array<std::uint8_t,256> storage{};w::writer out{storage};
    assert(out.u64(1)&&out.text("invented_state")&&out.text("none")&&out.u64(1)&&out.text(""));
    assert(!s::validate(out.written(),s::type::receipt,allowance));
    out=w::writer{storage};assert(out.u64(35));assert(!s::validate(out.written(),s::type::parameter_value,allowance));
    out=w::writer{storage};assert(out.u64(6)&&out.u64(UINT64_MAX));assert(!s::validate(out.written(),s::type::parameter_value,allowance));
    out=w::writer{storage};assert(out.u64(1)&&out.u64(2));assert(!s::validate(out.written(),s::type::parameter_value,allowance));
}
static void recursion_and_global_budgets()
{
    std::array<std::uint8_t,4096> storage{};w::writer out{storage};
    for(unsigned i{};i<12;++i)assert(out.u64(32)&&out.u64(1));
    assert(out.u64(0));assert(s::validate(out.written(),s::type::parameter_value,allowance));
    auto limits=allowance;limits.depth=8;assert(!s::validate(out.written(),s::type::parameter_value,limits));
    limits=allowance;limits.elements=11;assert(!s::validate(out.written(),s::type::parameter_value,limits));
    limits=allowance;limits.nodes=8;assert(!s::validate(out.written(),s::type::parameter_value,limits));
    out=w::writer{storage};for(unsigned i{};i<70;++i)assert(out.u64(32)&&out.u64(1));assert(out.u64(0));
    limits=allowance;limits.depth=1000;assert(!s::validate(out.written(),s::type::parameter_value,limits));
    assert(!s::validate({},static_cast<s::type>(UINT16_MAX),allowance));
}
static void malformed_corpus()
{
    std::array<std::uint8_t,256> data{};std::uint64_t state=0x561af34bcde17529;
    for(unsigned iteration{};iteration<12000;++iteration)
    {
        for(auto& byte:data){state^=state<<13;state^=state>>7;state^=state<<17;byte=static_cast<std::uint8_t>(state);}
        auto type=static_cast<s::type>(iteration%s::shapes.size());
        (void)s::validate({data.data(),iteration%data.size()},type,allowance);
    }
}
int main()
{
    catalogue_records();primitive_roundtrip_and_canaries();canonical_text_and_selectors();recursion_and_global_budgets();malformed_corpus();
    std::cout<<"passed canonical wire safety: "<<wire_cases.size()<<" catalogue records, 5 safety groups\n";
}
