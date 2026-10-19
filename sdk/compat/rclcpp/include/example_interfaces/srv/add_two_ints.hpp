/** Complete fixed-field source value for the acquired official AddTwoInts.srv.
 * This is an opt-in source include map, not native generated type support. Its
 * canonical bytes are signed i64 little-endian a,b and signed i64 sum. Actual
 * native identities must come from the installed SDK export/bind pipeline.
 */
#pragma once
#include <roswasm/roswasmcpp.hpp>
#include <bit>
#include <cstdint>
#include <memory>
namespace example_interfaces::srv {
struct AddTwoInts {
    struct Request {using SharedPtr=std::shared_ptr<Request>;using ConstSharedPtr=std::shared_ptr<Request const>;std::int64_t a{},b{};};
    struct Response {using SharedPtr=std::shared_ptr<Response>;using ConstSharedPtr=std::shared_ptr<Response const>;std::int64_t sum{};};
};
}
namespace roswasmcpp {
template<> struct ServiceCodec<::example_interfaces::srv::AddTwoInts> {
    using S=::example_interfaces::srv::AddTwoInts;
    static wire::bytes encode(S::Request const& value){wire::bytes out;out.reserve(16);wire::append_u64(out,std::bit_cast<std::uint64_t>(value.a));wire::append_u64(out,std::bit_cast<std::uint64_t>(value.b));return out;}
    static bool decode(std::span<unsigned char const> bytes,S::Request& value){if(bytes.size()!=16)return false;S::Request next{std::bit_cast<std::int64_t>(uwvm_ros_load_u64(bytes.data())),std::bit_cast<std::int64_t>(uwvm_ros_load_u64(bytes.data()+8))};value=next;return true;}
    static bool decode_response(std::span<unsigned char const> bytes,S::Response& value){if(bytes.size()!=8)return false;S::Response next{std::bit_cast<std::int64_t>(uwvm_ros_load_u64(bytes.data()))};value=next;return true;}
    static bool encode_response(S::Response const& value,wire::bytes& bytes){wire::bytes next;next.reserve(8);wire::append_u64(next,std::bit_cast<std::uint64_t>(value.sum));bytes=std::move(next);return true;}
};
}
