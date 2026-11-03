// SPDX-License-Identifier: Apache-2.0
// Owned bounded byte arrays only; no ROS endpoint, Guest or native effect.
#include <uwvm2/imported/ros/portable/memory.h>
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <limits>

namespace p = uwvm2::imported::ros::portable;
static unsigned checks{};
static void check(bool pass,char const* name)
{
    ++checks;
    if(!pass) { ::std::fprintf(stderr,"FAIL %u %s\n",checks,name);::std::exit(1); }
}
// Fixture writers are deliberately independent of the implementation codec.
template<::std::size_t N> static void le(::std::array<::std::uint8_t,N>& out,::std::size_t at,::std::uint64_t value,unsigned width)
{ for(unsigned i{};i!=width;++i) { out[at+i]=static_cast<::std::uint8_t>(value&255);value/=256; } }
template<::std::size_t N> static void header(::std::array<::std::uint8_t,N>& out,unsigned kind,unsigned count)
{
    out[0]='U';out[1]='R';out[2]='P';out[3]='1';out[4]=1;out[6]=static_cast<::std::uint8_t>(kind);
    le(out,8,N,4);le(out,12,count,4);
}
static auto descriptor_fixture()
{
    ::std::array<::std::uint8_t,104> bytes{};header(bytes,1,1);le(bytes,16,7,8);le(bytes,24,15,4);le(bytes,28,1,4);
    for(unsigned i{};i!=32;++i) { bytes[32+i]=static_cast<::std::uint8_t>(i+1); }
    le(bytes,68,1,4);le(bytes,72,static_cast<::std::uint64_t>(-1000000LL),8);le(bytes,80,1000000,8);
    le(bytes,88,10000,8);le(bytes,96,100,8);return bytes;
}
static auto state_fixture()
{
    ::std::array<::std::uint8_t,80> bytes{};header(bytes,2,1);le(bytes,16,7,8);le(bytes,24,11,8);
    le(bytes,32,100,8);le(bytes,40,200,8);le(bytes,68,1,4);le(bytes,72,static_cast<::std::uint64_t>(-20000LL),8);return bytes;
}
static auto command_fixture()
{
    ::std::array<::std::uint8_t,96> bytes{};header(bytes,3,1);le(bytes,16,7,8);le(bytes,24,1,8);le(bytes,32,11,8);
    for(unsigned i{};i!=32;++i) { bytes[40+i]=static_cast<::std::uint8_t>(i+1); }
    le(bytes,72,1000000000,8);le(bytes,88,static_cast<::std::uint64_t>(-10000LL),8);return bytes;
}
static auto receipt_fixture()
{
    ::std::array<::std::uint8_t,96> bytes{};header(bytes,4,0);le(bytes,16,7,8);le(bytes,24,1,8);
    le(bytes,32,2,4);le(bytes,40,12,8);le(bytes,48,180,8);
    for(unsigned i{};i!=32;++i) { bytes[56+i]=static_cast<::std::uint8_t>(31-i); } return bytes;
}
template<class Record,::std::size_t N> static void canonical(::std::array<::std::uint8_t,N> const& golden)
{
    Record parsed{};check(p::decode(golden,parsed),"independent golden frame");
    ::std::array<::std::uint8_t,N+1> bytes{};bytes.fill(0xa5);
    auto encoded{p::encode(parsed,bytes)};
    check(encoded.status==p::error::ok && encoded.bytes==N && bytes[N]==0xa5,"exact output length and tail untouched");
    check(::std::equal(golden.begin(),golden.end(),bytes.begin()),"canonical bytes match independent fixture");
    for(::std::size_t size{};size!=N;++size)
    {
        Record target{};check(!p::decode({golden.data(),size},target),"every truncated owned frame refused");
        bytes.fill(0xa5);auto too_small{p::encode(parsed,{bytes.data(),size})};
        check(too_small.status==p::error::too_small && too_small.bytes==N &&
              ::std::all_of(bytes.begin(),bytes.end(),[](auto c){return c==0xa5;}),"size error preserves whole output");
    }
    bytes.fill(0);::std::copy(golden.begin(),golden.end(),bytes.begin());
    check(!p::decode(bytes,parsed),"trailing bytes refused");
    for(auto offset : {0u,4u,5u,6u,7u,8u,9u,10u,11u,12u,13u,14u,15u})
    {
        auto changed{golden};changed[offset]^=0x80;
        check(!p::decode(changed,parsed),"unknown header field refused");
    }
    auto changed{golden};changed[0]='X';parsed.epoch=99;
    check(!p::decode(changed,parsed) && parsed.epoch==99,"failed decode leaves destination unchanged");
}
int main()
{
    auto dbytes{descriptor_fixture()};auto cbytes{command_fixture()};
    // Separate declarations preserve each fixed fixture's exact array extent.
    auto sbytes{state_fixture()};auto rbytes{receipt_fixture()};
    canonical<p::descriptor>(dbytes);canonical<p::state>(sbytes);
    canonical<p::command>(cbytes);canonical<p::receipt>(rbytes);
    p::descriptor mapping{};p::state sample{};p::command command{};p::receipt receipt{};
    check(p::decode(dbytes,mapping) && p::decode(sbytes,sample) && p::decode(cbytes,command) && p::decode(rbytes,receipt),"all fixture data");
    check(mapping.joints[0].lower==-1000000 && sample.positions[0]==-20000 && command.targets[0]==-10000,"signed little endian values");
    check(p::validate_command_data(command,mapping,sample,150)==p::error::ok,"admit bounded command data");
    auto original{command};
    command.epoch=8;check(p::validate_command_data(command,mapping,sample,150)==p::error::stale,"old mapping epoch");command=original;
    command.mapping[0]^=1;check(p::validate_command_data(command,mapping,sample,150)==p::error::stale,"mapping digest mismatch");command=original;
    command.based_on_sequence=10;check(p::validate_command_data(command,mapping,sample,150)==p::error::stale,"old sample sequence");command=original;
    command.targets[0]=-9999;check(p::validate_command_data(command,mapping,sample,150)==p::error::unsupported,"step one beyond bound");command=original;
    check(p::validate_command_data(command,mapping,sample,99)==p::error::no_fresh_state,"sample from future");
    check(p::validate_command_data(command,mapping,sample,200)==p::error::no_fresh_state,"exact expiry refused");
    check(p::validate_command_data(command,mapping,sample,199)==p::error::ok,"last valid sample instant");
    auto old_mapping{mapping};mapping.rights=1;
    check(p::validate_command_data(command,mapping,sample,150)==p::error::ok,"wire rights never substitute for separate Host authorization");mapping=old_mapping;
    command.targets[0]=::std::numeric_limits<::std::int64_t>::min();
    check(p::validate_command_data(command,mapping,sample,150)==p::error::malformed,"extreme signed value refused before arithmetic");command=original;
    for(unsigned offset : {48u,63u,64u,68u})
    { auto altered{sbytes};altered[offset]^=2;check(!p::decode(altered,sample),"state reserved slot or validity refused"); }
    for(unsigned offset : {80u,84u,87u})
    { auto altered{cbytes};altered[offset]^=2;check(!p::decode(altered,command),"command slot or reserved refused"); }
    for(unsigned offset : {88u,95u})
    { auto altered{rbytes};altered[offset]=1;check(!p::decode(altered,receipt),"receipt reserved refused"); }
    for(auto flags : {0u,2u,3u})
    { auto altered{dbytes};le(altered,28,flags,4);check(!p::decode(altered,mapping),"simulation flag exact"); }
    auto unknown{rbytes};le(unknown,32,6,4);check(!p::decode(unknown,receipt),"unknown outcome refused");
    le(unknown,32,2,4);le(unknown,36,1,4);check(!p::decode(unknown,receipt),"successful receipt cannot carry native failure");
    auto maximal{old_mapping};maximal.count=16;
    for(auto& joint:maximal.joints) { joint={-p::position_limit,p::position_limit,10000,1000}; }
    ::std::array<::std::uint8_t,704> largest{};
    auto max_encoded{p::encode(maximal,largest)};
    check(max_encoded.status==p::error::ok && max_encoded.bytes==704 && p::decode(largest,mapping) && mapping.count==16,"maximum16 joint frame");
    maximal.count=17;largest.fill(0xa5);
    check(p::encode(maximal,largest).status==p::error::malformed && largest[0]==0xa5,"count overflow no output");

    p::memory_shape memory{1024,1,0,false};
    check(p::preflight_open(memory,0,{0,5},8)==p::error::ok,"disjoint open spans");
    check(p::preflight_open(memory,0,{0,5},4)==p::error::malformed,"open alias overlap");
    check(p::preflight_output(memory,0,{64,96},200,96,{0,32})==p::error::ok,"disjoint command and output");
    check(p::preflight_output(memory,0,{64,95},200,96,{0,32})==p::error::too_small,"valid required length only path");
    check(p::preflight_output(memory,0,{64,95},64,96,{0,32})==p::error::malformed,"bad length wins over size error");
    check(p::preflight_output(memory,0,{64,96},200,96,{63,2})==p::error::malformed,"request output overlap");
    check(p::preflight_output(memory,0,{64,96},200,96,{199,2})==p::error::malformed,"request length overlap");
    check(p::preflight_output(memory,0,{1024,0},200,96)==p::error::too_small,"zero capacity at end allowed");
    check(p::preflight_output(memory,0,{1025,0},200,96)==p::error::malformed,"zero capacity beyond end refused");
    check(p::preflight_output(memory,0,{64,::std::numeric_limits<::std::uint64_t>::max()},200,96)==p::error::malformed,"capacity without addition wrap");
    check(p::preflight_output(memory,0,{64,96},::std::numeric_limits<::std::uint64_t>::max(),96)==p::error::malformed,"length word without addition wrap");
    memory.shared=true;check(p::preflight_open(memory,0,{0,5},8)==p::error::unsupported,"shared memory refused");
    memory.shared=false;memory.count=2;check(p::preflight_open(memory,0,{0,5},8)==p::error::unsupported,"multi memory refused");
    memory.count=1;check(p::preflight_open(memory,1,{0,5},8)==p::error::malformed,"only selector zero admitted");
    ::std::printf("{\"passed\":true,\"checks\":%u,\"scope\":\"owned-buffer portable wire and memory contracts only\"}\n",checks);
}
