#define UWVM_TEST
#include <uwvm2/runtime/memtag/random.h>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace rt = uwvm2::runtime::memtag;
using width = uwvm2::extension::memtag::address_width;
static void check(bool passed, int line)
{
    if(!passed) { std::fprintf(stderr,"global tag test failed at line %d\n",line); std::abort(); }
}
#define CHECK(result) check((result),__LINE__)

#if defined(__linux__)
static unsigned refills;
static bool fail_refill;

static ssize_t byte_sequence(void* out, std::size_t count, unsigned flags)
{
    CHECK(count == 256 && flags == 0);
    ++refills;
    if(fail_refill) { errno = EIO; return -1; }
    auto* bytes = static_cast<std::uint8_t*>(out);
    for(unsigned i{}; i != count; ++i) { bytes[i] = static_cast<std::uint8_t>(i); }
    return static_cast<ssize_t>(count);
}

static void uniform_nonzero(width w, unsigned domain)
{
    auto& buffer = rt::details::entropy_buffer;
    buffer.position = buffer.bytes.size();
    refills = 0;
    std::array<unsigned,256> histogram{};
    unsigned const accepted = 256 - 256 / domain;
    for(unsigned i{}; i != accepted; ++i)
    {
        std::uint8_t value{};
        CHECK(rt::next_nonzero_global_tag(w,value));
        ++histogram[value];
    }
    CHECK(refills == 1 && buffer.position == 256);
    for(unsigned i{}; i != histogram.size(); ++i)
    {
        CHECK(histogram[i] == ((i && i < domain) ? 256 / domain : 0));
    }
}

static void rejection_limits(width w)
{
    auto& buffer = rt::details::entropy_buffer;
    buffer.bytes.fill(0);
    buffer.position = 0;
    buffer.bytes[31] = 5;
    buffer.bytes[64] = 7;
    std::uint8_t tag = 99;
    CHECK(rt::next_nonzero_global_tag(w,tag) && tag == 5 && buffer.position == 32);
    CHECK(!rt::next_nonzero_global_tag(w,tag) && tag == 5 && buffer.position == 64);
    CHECK(rt::next_nonzero_global_tag(w,tag) && tag == 7 && buffer.position == 65);
    fail_refill = true;
    refills = 0;
    buffer.position = 254;
    CHECK(!rt::next_nonzero_global_tag(w,tag) && tag == 7 && refills == 1 && buffer.position == 256);
    fail_refill = false;
}
#endif

#if defined(__linux__)
// Replay must consume the same raw draws as a recording, even when one generation
// rejects zero on memory32 but the same byte would be a valid memory64 tag.
static void observer_replay()
{
    struct transcript
    {
        std::array<std::uint8_t,4> bytes{};
        unsigned position{};
        bool replay{};
    } log;
    rt::tag_entropy_observer source{&log, [](void* ctx, std::uint8_t& byte,
                                             bool (*native)(std::uint8_t&) noexcept) noexcept {
        auto& entry = *static_cast<transcript*>(ctx);
        if(entry.position == entry.bytes.size()) { return false; }
        if(entry.replay) { byte = entry.bytes[entry.position]; }
        else
        {
            if(!native(byte)) { return false; }
            entry.bytes[entry.position] = byte;
        }
        ++entry.position;
        return true;
    }};
    auto& buffer = rt::details::entropy_buffer;
    buffer.position = 0;
    buffer.bytes[0] = 0x10;
    buffer.bytes[1] = 0xff;
    buffer.bytes[2] = 0;
    buffer.bytes[3] = 0x90;
    rt::current_tag_entropy_observer = &source;
    for(bool replay : {false,true})
    {
        log.replay = replay;
        log.position = 0;
        std::uint8_t tag{};
        CHECK(rt::next_nonzero_global_tag(width::i32,tag) && tag == 15 && log.position == 2);
        CHECK(rt::next_nonzero_global_tag(width::i64,tag) && tag == 144 && log.position == 4);
        CHECK(!rt::next_nonzero_global_tag(width::i32,tag) && tag == 144 && log.position == 4);
        CHECK(buffer.position == 4);
    }
    rt::current_tag_entropy_observer = nullptr;
}
#endif

int main()
{
    std::uint8_t tag = 11;
    CHECK(!rt::next_nonzero_global_tag(static_cast<width>(0),tag) && tag == 11);
#if defined(__linux__)
    rt::details::entropy_read_test_hook = byte_sequence;
    uniform_nonzero(width::i32,16);
    uniform_nonzero(width::i64,256);
    rejection_limits(width::i32);
    rejection_limits(width::i64);
    observer_replay();
    rt::details::entropy_read_test_hook = nullptr;
#else
    CHECK(!rt::next_nonzero_global_tag(width::i32,tag) && tag == 11);
#endif
}
