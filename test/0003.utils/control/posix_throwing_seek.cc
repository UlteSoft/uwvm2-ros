#include "posix_test_abi.h"
#include <array>
#include <limits>
#include <fast_io.h>
#if !defined(__linux__) || !defined(__NR_memfd_create)
# error "This real-kernel seek regression requires Linux memfd"
#endif
static unsigned checks{};
#define CHECK(x) do { ++checks; if(!(x)) { ::fast_io::io::perrln("FAIL ", __LINE__, " ", ::fast_io::mnp::os_c_str(#x)); return 1; } } while(false)
int main()
{
    using namespace ::fast_io;
    int const raw{system_call<__NR_memfd_create, int>("uwvm-throwing-seek", 1u)};
    CHECK(!linux_system_call_fails(raw));
    posix_file file{raw};
    auto const seek = [&](intfpos_t offset, ::fast_io::seekdir direction) { return operations::io_stream_seek_bytes(file, offset, direction); };
    auto const actual = [&] { return ::control_test::posix_abi::lseek_noexcept(raw, 0, SEEK_CUR); };
    truncate(file, 10003);
    CHECK(seek(17, seekdir::beg) == 17);
    CHECK(seek(0, seekdir::cur) == 17 && actual() == 17);
    CHECK(seek(-3, seekdir::cur) == 14 && actual() == 14);
    CHECK(seek(-3, seekdir::end) == 10000 && actual() == 10000);
    std::array<std::byte, 3> const contents{std::byte{'M'}, std::byte{'T'}, std::byte{'E'}};
    operations::pwrite_all_bytes(file, contents.data(), contents.data()+contents.size(), 0);
    std::array<std::byte, 3> read{};
    operations::pread_all_bytes(file, read.data(), read.data()+read.size(), 0);
    CHECK(read == contents && seek(0, seekdir::cur) == 10000);
    constexpr intfpos_t large{8589934592LL + 17};
    static_assert(std::numeric_limits<::control_test::posix_abi::seek_offset_t>::digits >= 63);
    CHECK(seek(large, seekdir::beg) == large && actual() == large);
    CHECK(seek(-8, seekdir::cur) == large-8 && actual() == large-8);
    // Invalid calls must not silently change the cursor (notably on Linux32).
    bool rejected{};
    try { seek(-1, seekdir::beg); } catch (::fast_io::error const& e) { rejected = e.code == EINVAL; }
    CHECK(rejected && actual() == large-8);
    rejected = false;
    try { seek(0, static_cast<::fast_io::seekdir>(255)); } catch (::fast_io::error const& e) { rejected = e.code == EINVAL; }
    CHECK(rejected && actual() == large-8);
    CHECK(seek(0, seekdir::end) == 10003);
    io::println("PASS real-kernel throwing seek: ", checks);
}
