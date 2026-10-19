/** @file
 * Actual installed ROS2 BoundedVector / generated native-trait contract.
 *
 * The include below must come from a fresh native-identity-bound generation
 * using the actual selected SDK and this current generator. No native type,
 * allocator, namespace alias or ROS message is mocked here. The complete full
 * dependency file must bind that generated header, the actual bounded-vector
 * header and their compiler/SDK closure before any native result is accepted.
 * This executable creates no Node, endpoint, Guest, robot or ROS network route.
 */
#include "tests/ros_custom_types/bounded-v2/native_protocol.h"
#include "native_ros2.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

#ifdef NDEBUG
#error "Build actual native contracts with assertions enabled (-UNDEBUG)"
#endif

namespace actual_bounded_sequence_contract
{
// Selection mirrors the emitted header. Modern wins if both real SDK headers
// exist, including a legacy compatibility header that aliases the modern type.
#if __has_include(<rosidl_runtime_cpp/bounded_vector.hpp>)
using native_container=::rosidl_runtime_cpp::BoundedVector<std::uint32_t,3,std::allocator<std::uint32_t>>;
constexpr char selected_header[]{"rosidl_runtime_cpp/bounded_vector.hpp"};
#elif __has_include(<rosidl_generator_cpp/bounded_vector.hpp>)
using native_container=::rosidl_generator_cpp::BoundedVector<std::uint32_t,3,std::allocator<std::uint32_t>>;
constexpr char selected_header[]{"rosidl_generator_cpp/bounded_vector.hpp"};
#else
#error "Actual ROS2 SDK container required; never substitute a test container"
#endif
using traits=::uwvm_custom_bounded_native::sequence_shape<native_container>;
static_assert(traits::supported,"generated native trait must recognize the actual SDK container");
static_assert(traits::bound==3,"the native sequence bound must be preserved exactly");
static_assert(std::is_same<typename traits::element,std::uint32_t>::value,
              "the native allocator-aware element type must be preserved exactly");
static_assert(::uwvm_custom_bounded_native::sequence_shape<std::vector<std::uint32_t>>::supported,
              "ordinary native vector support is retained");
static_assert(::uwvm_custom_bounded_native::sequence_shape<std::vector<std::uint32_t>>::bound==0,
              "ordinary native vector remains structurally unbounded");
static_assert(!::uwvm_custom_bounded_native::sequence_shape<std::array<std::uint32_t,3>>::supported,
              "an unrelated fixed container must not gain sequence authority");
unsigned checks{},failures{};
void check(bool value,char const* message)
{
    ++checks;
    if(!value){++failures;bounded_native_io::print(STDERR_FILENO,"FAIL_NATIVE_BOUNDED_VECTOR ",message,'\n');}
}
}

int main()
{
    using namespace actual_bounded_sequence_contract;
    native_container values;
    check(values.empty()&&values.size()==0,"actual default container is empty");
    values.reserve(3);
    check(values.capacity()>=3&&values.max_size()==3,"actual reserve remains bounded");
    values.push_back(7);values.push_back(9);
    check(values.size()==2&&values[0]==7&&values[1]==9,"actual SDK stores both elements");
    values.push_back(11);
    check(values.size()==3&&values[2]==11,"actual SDK accepts its exact bound");
    bool refused=false;
    try{values.resize(4);}catch(std::length_error const&){refused=true;}
    check(refused,"actual SDK rejects an over-bound resize");
    check(values.size()==3&&values[0]==7&&values[1]==9&&values[2]==11,"resize refusal preserves contents");
    refused=false;
    try{values.push_back(13);}catch(std::length_error const&){refused=true;}
    check(refused,"actual SDK rejects an over-bound append");
    check(values.size()==3&&values[2]==11,"append refusal preserves contents");
    values.resize(1);
    check(values.size()==1&&values[0]==7,"actual SDK shrinks without replacing the type");
    values.resize(3,23);
    check(values.size()==3&&values[0]==7&&values[1]==23&&values[2]==23,"actual SDK bounded fill resize");
    values.clear();
    check(values.empty()&&values.max_size()==3,"actual SDK clear retains its bound");
    bounded_native_io::print(STDOUT_FILENO,"PASS_NATIVE_BOUNDED_VECTOR_HEADER_TRAITS header=",selected_header,
                             " checks=",checks," failures=",failures,'\n');
    return failures?1:0;
}
