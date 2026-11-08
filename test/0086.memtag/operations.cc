#include <uwvm2/extension/memtag/operations.h>
#include <array>
#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <span>
namespace mt = uwvm2::extension::memtag;
static void require(bool value) { if(!value) { ::std::abort(); } }
template<typename Address, bool Shared>
void exercise()
{
    using view=mt::shadow_view<Address,Shared>;
    ::std::array<::std::byte,256> data{};data.fill(::std::byte{0x5a});
    ::std::array<typename view::tag_type,16> tags{};
    view shadow{tags.data(),tags.size(),data.size()};
    unsigned random_calls{};
    auto random{[&]() noexcept -> Address {++random_calls;return 11u;}};
    auto run{[&](mt::opcode code,::std::initializer_list<Address> args)
    {return mt::execute_operation<true,Address,Shared>(code,{args.begin(),args.size()},data,shadow,random);}};
    auto disabled{[&](mt::opcode code,::std::initializer_list<Address> args)
    {return mt::execute_operation<false,Address,Shared>(code,{args.begin(),args.size()},data,shadow,random);}};
    require(run(mt::opcode::status,{}).value==2u);
    require(run(mt::opcode::tagbits,{}).value==view::TagBits);
    require(run(mt::opcode::startbit,{}).value==view::start_bit);
    auto const pointer{view::insert(37u,9u)};
    require(run(mt::opcode::extract,{pointer}).value==9u);
    require(run(mt::opcode::untag,{pointer}).value==37u);
    require(run(mt::opcode::insert,{37u,9u}).value==pointer);
    require(run(mt::opcode::copy,{37u,view::insert(8u,9u)}).value==pointer);
    require(run(mt::opcode::sub,{pointer,view::insert(21u,3u)}).value==16u);
    require(run(mt::opcode::hint,{37u,view::insert(0u,2u),7u}).value==pointer);
    require(static_cast<bool>(run(mt::opcode::store,{pointer,32u})));
    require(static_cast<bool>(shadow.check(pointer,0u,32u)));
    require(run(mt::opcode::load,{37u}).value==pointer);
    require(data[37]==::std::byte{0x5a});
    require(static_cast<bool>(run(mt::opcode::storez,{pointer,32u})));
    for(unsigned index{};index!=data.size();++index)
    {require(data[index]==((index>=37u&&index<69u)?::std::byte{} : ::std::byte{0x5a}));}
    require(run(mt::opcode::untagstore,{pointer,32u}).value==37u);
    require(shadow.check(pointer,0u,32u).error==mt::access_error::tag_mismatch);
    require(run(mt::opcode::random,{37u}).value==view::insert(37u,11u));
    require(random_calls==1u);
    require(run(mt::opcode::randomstore,{37u,32u}).value==view::insert(37u,11u));
    require(static_cast<bool>(shadow.check(view::insert(37u,11u),0u,32u)));
    require(run(mt::opcode::hintstore,{37u,32u,view::insert(0u,2u),7u}).value==pointer);
    require(static_cast<bool>(shadow.check(pointer,0u,32u)));

    auto const prior_calls{random_calls};
    auto const prior_data{data};
    require(run(mt::opcode::randomstorez,{250u,7u}).error==mt::operation_error::memory_access);
    require(random_calls==prior_calls && data==prior_data);
    require(static_cast<bool>(shadow.check(pointer,0u,32u)));
    require(run(mt::opcode::add,{pointer,1u,1u}).error==mt::operation_error::undefined_placeholder);
    require(run(mt::opcode::insert,{pointer}).error==mt::operation_error::wrong_arity);
    require(run(static_cast<mt::opcode>(0x30u),{}).error==mt::operation_error::unknown_opcode);

    require(disabled(mt::opcode::status,{}).value==0u);
    require(disabled(mt::opcode::tagbits,{}).value==0u);
    require(disabled(mt::opcode::startbit,{}).value==sizeof(Address)*8u);
    require(disabled(mt::opcode::extract,{pointer}).value==0u);
    for(auto op:{mt::opcode::untag,mt::opcode::load,mt::opcode::random})
    {require(disabled(op,{pointer}).value==pointer);}
    require(disabled(mt::opcode::insert,{pointer,7u}).value==pointer);
    require(disabled(mt::opcode::copy,{pointer,7u}).value==pointer);
    require(disabled(mt::opcode::sub,{pointer,7u}).value==static_cast<Address>(pointer-7u));
    require(disabled(mt::opcode::hint,{pointer,7u,2u}).value==pointer);
    for(auto op:{mt::opcode::store,mt::opcode::untagstore,mt::opcode::randomstore})
    {require(static_cast<bool>(disabled(op,{pointer,static_cast<Address>(~Address{})})));}
    require(static_cast<bool>(disabled(mt::opcode::hintstore,{pointer,static_cast<Address>(~Address{}),7u,2u})));
    require(random_calls==prior_calls && data==prior_data);

    // All four zeroing forms retain data writes with tag execution disabled.
    for(auto op:{mt::opcode::storez,mt::opcode::untagstorez,mt::opcode::randomstorez,mt::opcode::hintstorez})
    {
        data.fill(::std::byte{0x3c});
        auto result{op==mt::opcode::hintstorez?disabled(op,{37u,32u,7u,2u}):disabled(op,{37u,32u})};
        require(static_cast<bool>(result));
        for(unsigned index{};index!=data.size();++index)
        {require(data[index]==((index>=37u&&index<69u)?::std::byte{} : ::std::byte{0x3c}));}
        require(static_cast<bool>(shadow.check(pointer,0u,32u)));
        require(random_calls==prior_calls);
    }
    // A data span smaller than its shadow cannot authorize an overrun.
    ::std::array<Address,2> args{pointer,32u};
    auto const before{data};
    auto result{mt::execute_operation<true,Address,Shared>(mt::opcode::storez,args,
        ::std::span(data).first(64u),shadow,random)};
    require(result.error==mt::operation_error::memory_access && data==before);
    require(static_cast<bool>(shadow.check(pointer,0u,32u)));
    ::std::array<Address,1> load_args{pointer};
    auto const loaded{mt::execute_operation<true,Address,Shared>(mt::opcode::load,load_args,
        ::std::span(data).first(64u),shadow,random)};
    require(loaded.error==mt::operation_error::memory_access && data==before);
}
int main()
{
    exercise<::std::uint32_t,false>();exercise<::std::uint64_t,false>();
    exercise<::std::uint32_t,true>();exercise<::std::uint64_t,true>();
}
