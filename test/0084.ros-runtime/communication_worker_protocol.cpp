/** @file @brief Lost reply recovery, changed retry rejection and effect fencing. */
#include <uwvm2/imported/ros/scene/communication_worker_protocol.h>
#include <cassert>
#include <cstdio>
namespace s=uwvm2::imported::ros::scene;
namespace p=s::communication_worker_protocol;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
static bytes request(std::uint64_t sequence,std::uint64_t value)
{bytes result(p::header_bytes+8);assert(p::request_header({result.data(),p::header_bytes},7,sequence,p::operation::topic_publish,3,8));s::wire::writer w{{result.data()+p::header_bytes,8}};assert(w.u64(value));return result;}
int main()
{
    p::ledger ledger;assert(!ledger.prepare(0,32,32)&&ledger.prepare(7,32,32)&&!ledger.prepare(7,32,32));unsigned calls{};
    auto handler=[&](p::request const& input,std::span<std::uint8_t> output)
    {assert(input.binding==3&&input.action==p::operation::topic_publish);++calls;s::wire::reader r{input.body};std::uint64_t value{};assert(r.u64(value)&&r.done());s::wire::writer w{output};assert(w.u64(value+1));return p::result{p::error::ok,p::effect::locally_accepted,w.size()};};
    auto first=request(1,41);s::wire::bytes reply;assert(ledger.execute(first,handler,reply)&&calls==1);bytes original(reply.begin(),reply.end());
    // A lost complete output must not repeat the native publication.
    assert(ledger.execute(first,handler,reply)&&calls==1&&std::equal(reply.begin(),reply.end(),original.begin(),original.end()));
    p::response decoded;assert(p::decode_response(reply,7,1,32,decoded)&&decoded.observed==p::effect::locally_accepted);s::wire::reader answer{decoded.body};std::uint64_t number{};assert(answer.u64(number)&&number==42);
    for(std::size_t n{};n<reply.size();++n)assert(!p::decode_response(reply.first(n),7,1,32,decoded));
    assert(!p::decode_response(reply,8,1,32,decoded)&&!p::decode_response(reply,7,2,32,decoded)&&!p::decode_response(reply,7,1,7,decoded));
    auto second=request(2,8);assert(ledger.execute(second,handler,reply)&&calls==2);auto altered=second;altered.back()^=1;assert(!ledger.execute(altered,handler,reply)&&ledger.failed()&&calls==2);
    assert(ledger.execute(second,handler,reply)&&calls==2);assert(!ledger.execute(request(3,1),handler,reply));
    p::ledger skipped;assert(skipped.prepare(7,32,32));assert(!skipped.execute(request(2,1),handler,reply)&&calls==2);
    for(std::size_t n{};n<first.size();++n){p::request out;assert(!p::decode_request({first.data(),n},7,32,out));}
    auto trailing=first;trailing.push_back(0);p::request out;assert(!p::decode_request(trailing,7,32,out));
    p::ledger unknown;assert(unknown.prepare(7,32,32));
    assert(unknown.execute(first,[&](auto const&,auto)->p::result{++calls;throw 1;},reply));
    assert(calls==3&&unknown.failed()&&p::decode_response(reply,7,1,32,decoded)&&decoded.status==p::error::io&&decoded.observed==p::effect::unknown);
    assert(unknown.execute(first,handler,reply)&&calls==3&&!unknown.execute(second,handler,reply));
    p::ledger oversize;assert(oversize.prepare(7,32,32));
    assert(oversize.execute(first,[&](auto const&,auto){return p::result{p::error::ok,p::effect::locally_accepted,33};},reply));
    assert(oversize.failed()&&p::decode_response(reply,7,1,32,decoded)&&decoded.observed==p::effect::unknown);
    std::puts("passed worker RPC framing, exact lost-reply retry, changed/skipped sequence rejection and unknown-effect fence");
}
