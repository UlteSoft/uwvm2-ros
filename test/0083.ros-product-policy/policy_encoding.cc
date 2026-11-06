
#include <uwvm2/utils/update/policy_change.h>
#include <cassert>
#include <iostream>
namespace u=uwvm2::utils::update;
namespace a=uwvm2::utils::control::host_interfaces;
int main()
{
    u::wasi_policy_spec original;
    original.interface_enabled=true;
    auto const random=a::find(a::family::wasip1,std::string_view{"random_get"});
    auto const write=a::find(a::family::wasip1,std::string_view{"fd_write"});
    auto const ros=a::find(a::family::ros1,std::string_view{"uwvm_ros1_v1.abi_version"});
    assert(original.interfaces.set(random,a::access::allow));
    assert(original.interfaces.set(write,a::access::deny));
    std::vector<unsigned char> bytes;
    assert(u::encode_wasi_policy(original,bytes));
    u::wasi_policy_spec restored;
    assert(u::wasi_policy_detail::decode(bytes,restored));
    assert(restored.interfaces.entries==original.interfaces.entries);
    assert(restored.interfaces.exposed(write) && !restored.interfaces.allowed(write));
    auto old=bytes;old[5]='4';assert(!u::wasi_policy_detail::decode(old,restored));
    auto bad=bytes;bad[32+random]=3;assert(!u::wasi_policy_detail::decode(bad,restored));
    bad=bytes;bad[32+ros]=2;assert(!u::wasi_policy_detail::decode(bad,restored));
    for(std::size_t n{};n<bytes.size();++n)
        assert(!u::wasi_policy_detail::decode(std::span<unsigned char const>{bytes.data(),n},restored));
    auto denied=original;denied.interfaces.entries[random]=a::access::deny;
    std::vector<unsigned char> denied_bytes;assert(u::encode_wasi_policy(denied,denied_bytes));
    assert(denied_bytes!=bytes);
    std::cout<<"policy roundtrip, removed wire format, truncation and cross-family checks passed\n";
}
