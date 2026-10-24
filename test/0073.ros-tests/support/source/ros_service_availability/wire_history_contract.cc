// Source-only local wire/lifetime model. These SYNTHETIC handles/generations are
// not forecast Runtime identities or a native SDK/ROS matching observation.
// Root must compile this genuine header TU separately after complete source
// review. No test/compiler has been invoked by the author.
#include <uwvm2/imported/ros/common/service_availability_history.h>
#include <cstdlib>

namespace wire=uwvm2::imported::ros::ipc::client_availability;
using history=uwvm2::imported::ros::service_availability_history;
static void require(bool value){if(!value){std::abort();}}
static std::uint64_t handle(unsigned slot,unsigned generation=1)
{return (std::uint64_t{generation}<<32)|slot;}
int main()
{
    wire::context a{2,handle(1),handle(2),101,17};
    wire::packet query{wire::command::query,wire::retirement::none,0,a,17};
    auto bytes{wire::encode(query)};wire::packet decoded{};
    require(bytes.size()==64&&wire::decode_request(bytes.data(),bytes.size(),decoded));
    require(wire::same(decoded.binding,a)&&decoded.value==17);
    for(unsigned index{48};index!=64;++index)
    {auto wrong{bytes};wrong[index]=1;require(!wire::decode_request(wrong.data(),wrong.size(),decoded));}
    for(unsigned index:{0u,1u,2u,3u})
    {auto wrong{bytes};wrong[index]=255;require(!wire::decode_request(wrong.data(),wrong.size(),decoded));}
    auto zero{query};zero.binding.runtime_generation=0;require(!wire::valid_request(zero));
    zero=query;zero.binding.instance=0;require(!wire::valid_request(zero));
    zero=query;zero.binding.source=handle(2,0);require(!wire::valid_request(zero));
    zero=query;zero.binding.availability=handle(2,2);require(!wire::valid_request(zero));
    zero=query;zero.value=wire::maximum_wait_ns+1;require(!wire::valid_request(zero));
    wire::packet reply{query};reply.value=1;bytes=wire::encode(reply);
    require(wire::decode_response(bytes.data(),bytes.size(),query,decoded));
    auto stale{query};++stale.binding.runtime_generation;
    require(!wire::decode_response(bytes.data(),bytes.size(),stale,decoded));
    reply.value=2;bytes=wire::encode(reply);require(!wire::decode_response(bytes.data(),bytes.size(),query,decoded));
    wire::packet feature{wire::command::features,wire::retirement::none,0,a,0};
    reply=feature;reply.value=7;bytes=wire::encode(reply);require(wire::decode_response(bytes.data(),bytes.size(),feature,decoded));
    reply.value=11;bytes=wire::encode(reply);require(!wire::decode_response(bytes.data(),bytes.size(),feature,decoded));
    wire::packet retire{wire::command::retire,wire::retirement::source_handle,125,a,0};
    require(wire::valid_request(retire));retire.value=1;require(!wire::valid_request(retire));
    retire.value=0;retire.reason=110;require(!wire::valid_request(retire));

    history ledger;history::entry const* entry{};
    require(ledger.bind(a,"available","call",71,2,entry)==0&&entry);
    require(ledger.bind(a,"available","call",71,2,entry)==0);
    auto different{a};different.source=handle(3);
    require(ledger.bind(different,"available","call",71,2,entry)==17&&!entry);
    require(ledger.bind(a,"available","call",72,2,entry)==125);
    different=a;++different.runtime_generation;
    require(ledger.bind(different,"available","call",71,2,entry)==13);
    require(ledger.retire(a,wire::retirement::availability_handle)==0);
    require(ledger.bind(a,"available","call",71,2,entry)==125);
    different=a;different.availability=handle(1,2);
    require(ledger.bind(different,"available","call",71,2,entry)==0);
    auto third{different};third.availability=handle(1,3);
    require(ledger.bind(third,"available","call",71,2,entry)==11);
    // Source-close propagates using a HISTORICAL entry after all query owners
    // closed. It never relies only on currently live query/native objects.
    require(ledger.retire(different,wire::retirement::availability_handle)==0);
    require(ledger.retirement_reference(a.source,"call",wire::retirement::source_handle));
    require(ledger.retire(a,wire::retirement::source_handle)==0);
    third.availability=handle(4);require(ledger.bind(third,"other-available","call",71,2,entry)==125);
    auto fresh{third};fresh.source=handle(2,2);
    require(ledger.bind(fresh,"other-available","call",71,2,entry)==0);
    require(ledger.retire(fresh,wire::retirement::source_alias)==0);
    fresh.availability=handle(4,2);fresh.source=handle(2,3);
    require(ledger.bind(fresh,"third-available","call",71,2,entry)==125);
    // Unrelated source/authority is independent, and a transport loss cannot
    // silently resurrect an old binding in a new native session.
    wire::context unrelated{2,handle(8),handle(9),101,17};
    require(ledger.bind(unrelated,"independent-available","independent-call",71,4,entry)==0);
    ledger.retire_session(71);
    require(ledger.bind(unrelated,"independent-available","independent-call",72,4,entry)==125);
    unrelated.availability=handle(8,2);
    require(ledger.bind(unrelated,"independent-available","independent-call",72,4,entry)==0);
    ledger.seal();unrelated.availability=handle(8,3);
    require(ledger.bind(unrelated,"independent-available","independent-call",72,4,entry)==125);
}
