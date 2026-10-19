/* SPDX-License-Identifier: Apache-2.0
 * A bounded native-component example over actual generated C++ objects. This is
 * not a ROS route or an invented native-identity export. An independently
 * admitted bootstrap must define the selector below; absent selection denies13.
 * No default profile, capability, native endpoint, clock/logger or graph opens.
 */
#include <uwvm_rmw/cpp_codecs.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_srvs/srv/empty.hpp>
#include <example_interfaces/srv/add_two_ints.hpp>
#include <array>
#include <string>
#if defined(UWVM_RMW_CPP_IDENTITY_SELECTION_HEADER)
#include UWVM_RMW_CPP_IDENTITY_SELECTION_HEADER
#else
inline uwvm_rmw_cpp::NativeIdentities const* selected_cpp_native_identities() noexcept{return nullptr;}
#endif
int main(){
  auto ids=selected_cpp_native_identities();if(!ids)return 13;
  uwvm_rmw_cpp::Registry registry{*ids,128};if(registry.error())return static_cast<int>(registry.error());
  auto codec=registry.codec(uwvm_rmw_cpp::Codec::string);std::array<unsigned char,128> bytes{};std::size_t length{};
  std_msgs::msg::String text;text.data=std::string{"A\0B",3};auto code=codec->encode(&text,bytes.data(),bytes.size(),&length);if(code)return static_cast<int>(code);
  std_msgs::msg::String copy;code=codec->decode(bytes.data(),length,&copy);if(code||copy.data!=text.data)return code?static_cast<int>(code):22;
  std_srvs::srv::Empty::Request empty;codec=registry.codec(uwvm_rmw_cpp::Codec::empty_request);length=77;
  code=codec->encode(&empty,bytes.data(),0,&length);if(code||length)return code?static_cast<int>(code):22;
  example_interfaces::srv::AddTwoInts::Request request;request.a=-7;request.b=49;codec=registry.codec(uwvm_rmw_cpp::Codec::add_request);
  code=codec->encode(&request,bytes.data(),bytes.size(),&length);if(code)return static_cast<int>(code);
  example_interfaces::srv::AddTwoInts::Request decoded;code=codec->decode(bytes.data(),length,&decoded);
  return code?static_cast<int>(code):decoded.a==-7&&decoded.b==49?0:22;
}
