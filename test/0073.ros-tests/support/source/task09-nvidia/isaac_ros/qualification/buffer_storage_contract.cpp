// SPDX-License-Identifier: Apache-2.0
// Genuine upstream header-only CPU Buffer storage; no ROS/RMW/GPU substitute.
#include <rosidl_buffer/buffer.hpp>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

static unsigned checks;
#define CHECK(value)                                                                               \
    do {                                                                                           \
        ++checks;                                                                                  \
        if (!(value)) {                                                                            \
            std::fprintf(stderr, "buffer storage check at %u\n", __LINE__);                        \
            return 1;                                                                              \
        }                                                                                          \
    } while (0)
int main() {
    using Buffer = rosidl::Buffer<std::uint8_t>;
    static_assert(
        std::is_same<decltype(std::declval<Buffer const &>().data()), std::uint8_t const *>::value);
    std::vector<std::uint8_t> pixels(224 * 224 * 3);
    for (std::size_t i = 0; i < pixels.size(); ++i)
        pixels[i] = static_cast<std::uint8_t>((i * 37 + 11) & 255);
    Buffer original(pixels);
    CHECK(original.get_backend_type() == "cpu");
    CHECK(original.size() == pixels.size() && original.to_vector() == pixels);
    CHECK(original.data() != pixels.data());
    auto owned = original.to_vector();
    CHECK(owned.data() != original.data() && owned == pixels);
    Buffer copied(original);
    CHECK(copied.data() != original.data() && copied.to_vector() == pixels);
    original[0] ^= 255;
    CHECK(copied.to_vector() == pixels && owned == pixels);
    Buffer moved(std::move(copied));
    CHECK(copied.empty() && copied.get_backend_type() == "cpu");
    CHECK(moved.to_vector() == pixels);
    Buffer assigned;
    assigned = moved;
    CHECK(assigned.data() != moved.data() && assigned.to_vector() == pixels);
    moved.clear();
    CHECK(assigned.to_vector() == pixels && owned == pixels);
    assigned = std::vector<std::uint8_t>{0, 127, 255};
    CHECK(assigned.size() == 3 && assigned[0] == 0 && assigned[1] == 127 && assigned[2] == 255);
    bool rejected = false;
    try {
        Buffer invalid(std::unique_ptr<rosidl::BufferImplBase<std::uint8_t>>{});
        (void)invalid;
    } catch (std::invalid_argument const &) {
        rejected = true;
    }
    CHECK(rejected);
    std::printf(
        "{\"scope\":\"genuine_rosidl_cpu_buffer_storage\",\"checks\":%u,\"complete_rgb_bytes\":%lu,\"native_ros\":0,\"gpu\":0,\"guest\":0}\n",
        checks, static_cast<unsigned long>(pixels.size()));
    return 0;
}
