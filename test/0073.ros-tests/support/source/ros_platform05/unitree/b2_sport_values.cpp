/** Check every named C++ B2 facade against its separate selected method ID.
 * Link the actual formatter. No endpoint, robot or fake middleware is used. */
#include <platform/unitree/cpp/roswasm_unitree/b2_sport.hpp>
#include <fast_io.h>
#include <cstdlib>
namespace {
void require(bool value) {
    if (!value)
        std::abort();
}
} // namespace
int main() {
    using Builder = roswasm_unitree::B2SportClient;
    roswasmc_bounded_2_unitree_api_msg_Request value{};
    value.header.identity.id = 9007199254740993;
    value.header.lease.id = -9007199254740993;
    value.header.policy.priority = -101;
    value.header.policy.noreply = 1;
    value.binary.size = 4;
    value.binary.data[1] = 255;
    value.binary.data[2] = 1;
    value.binary.data[3] = 128;
    uwvm_unitree_b2_path_point points[30]{};
    for (unsigned i{}; i < 30; ++i)
        points[i].timeFromStart = static_cast<float>(i);
    unsigned calls{};
#define CALL_none(name) Builder::name(value)
#define CALL_xyz(name) Builder::name(value, 1.25f, -0.0f, 0.5f)
#define CALL_position(name) CALL_xyz(name)
#define CALL_integer(name) Builder::name(value, INT32_MIN)
#define CALL_boolean(name) Builder::name(value, true)
#define CALL_height(name) Builder::name(value, -0.125f)
#define CALL_trajectory(name)                                                          \
    Builder::name(value, std::span<uwvm_unitree_b2_path_point const>{points, 30})
#define UWVM_UNITREE_B2(name, operation, kind)                                         \
    require(CALL_##kind(name) == 0);                                                   \
    require(value.header.identity.api_id == operation);                                \
    require(value.header.identity.id == 9007199254740993 &&                            \
            value.header.lease.id == -9007199254740993);                               \
    require(value.header.policy.priority == -101 &&                                    \
            value.header.policy.noreply == 1 && value.binary.size == 4 &&              \
            value.binary.data[1] == 255 && value.binary.data[3] == 128);               \
    ++calls;
#include <platform/unitree/c/roswasm_unitree/b2_sport_operations.inc>
#undef UWVM_UNITREE_B2
    require(calls == 21);
    fast_io::io::println(
        fast_io::native_stdout(), "B2_CPP_NAMES_OK operations=", calls);
    return 0;
}
