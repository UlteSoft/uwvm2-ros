// Verify the actual production audit admission and record writer without granting guest capabilities.
#include <uwvm2/imported/ros/scene/call_audit.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <string>
namespace audit = uwvm2::imported::ros::scene::call_audit;

int main()
{
    audit::journal events;
    assert(!events.active());
    assert(events.add(audit::scope::global,"*",false,{8,8192,100,4}));
    assert(!events.add(audit::scope::global,"*",false,{8,8192,100,4}));
    assert(events.add(audit::scope::group,"machine",false,{2,8192,100,2}));
    assert(events.add(audit::scope::single,"body",true,{7,8192,1,1}));
    std::array<char,2048> last{};
    std::size_t length{}, count{};
    auto emit=[&](std::string_view s) noexcept {
        assert(s.size()<last.size());
        std::copy(s.begin(),s.end(),last.begin());
        length=s.size(); ++count;return true;
    };
    auto check=[&](std::string_view token){return std::string_view{last.data(),length}.find(token)!=std::string_view::npos;};
    events.observe("machine","body",3,"abi",2,1,emit);
    assert(count==0);
    events.observe("machine","body",3,"capability.open",13,1,emit);
    assert(count==1 && check("\"caller_hex\":\"626f6479\"") && check("\"result\":13"));
    events.observe("machine","body",3,"capability.open",13,2,emit);
    assert(count==1 && events.limited==1);
    events.observe("machine","body",3,"capability.open",13,audit::second+1,emit);
    assert(count==2 && check("\"sequence\":2") && check("\"limited\":1"));
    events.observe("machine","sibling",4,"abi",2,2*audit::second,emit);
    assert(count==2);
    events.observe("other","x\"\n",4,"abi",2,2*audit::second,emit);
    assert(count==3 && check("78220a") && check("\"result\":2"));
    events.observe("other","body",4,"capability.open",13,0,emit);
    assert(count==3 && events.clock_failures==1);
    events.observe("other","peer",4,"abi",2,2*audit::second,[](auto) noexcept{return false;});
    assert(events.sink_failures==1);
    events.observe("other","peer",4,"abi",2,2*audit::second,emit);
    assert(check("\"sink_failed\":1") && check("\"sequence\":5"));
    for(auto limits : {audit::ceiling{0,4096,1,1},audit::ceiling{1,1,1,1},audit::ceiling{1,4096,0,1},audit::ceiling{1,4096,1,0}})
    {
        audit::journal zero;
        assert(zero.add(audit::scope::global,"*",false,limits));
        zero.observe("g","m",3,"abi",2,1,[](auto) noexcept {assert(false);return true;});
        assert(zero.limited==1);
        audit::statistics first{}, repeated{};
        assert(zero.inspect(first) && zero.inspect(repeated));
        assert(first.configured && first.admitted==0 && first.limited==1 && repeated.limited==first.limited);
    }
    audit::statistics counts;
    assert(events.inspect(counts) && counts.admitted==5 && counts.sink_failed==1 &&
           counts.clock_failed==1 && counts.limited==2);
    audit::journal linking;
    assert(linking.add(audit::scope::group, "machine", true, {4,8192,100,4}));
    linking.observe("machine", "main", 0, "abi", 0, 1, emit, audit::phase::hidden_import);
    assert(check("\"phase\":\"link\"") && check("\"result\":null") && check("\"reason\":\"hidden\""));
    assert(check("\"function\":\"abi\"") && check("\"generation\":0"));
    linking.observe("machine", "main", 0, "a\"\n", 0, 2, emit, audit::phase::hidden_import);
    assert(check("\"function\":null") && check("\"function_hex\":\"61220a\"") &&
           check("\"function_truncated\":false"));
    std::array<char,256> long_name{};
    long_name.fill('a');
    linking.observe("machine", "main", 0, {long_name.data(),long_name.size()}, 0, 3, emit, audit::phase::hidden_import);
    assert(check("\"function_bytes\":256") && check("\"function_truncated\":true"));
    assert(linking.inspect(counts) && counts.admitted==3 && counts.limited==0);
    audit::journal nesting;
    assert(nesting.inspect(counts) && !counts.configured && counts.admitted==0);
    assert(nesting.add(audit::scope::single,"owned",false,{5,8192,1,5}));
    nesting.observe("unknown","else",4,"abi",2,1,[](auto) noexcept{assert(false);return true;});
    nesting.observe("g","owned",4,"abi",2,1,[&](auto) noexcept {
        nesting.observe("g","owned",4,"abi",2,1,emit);
        return true;
    });
    assert(check("\"sequence\":2"));
}
