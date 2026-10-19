/* Actual parser negative/ownership fixtures. Unexecuted Source; admitted Linux only. */
#include "../host/snapshot_reader.h"
#include <cassert>
#include <functional>
#include <iostream>

using bytes = std::array<std::byte, UWVM_LS_TOTAL_BYTES>;
using uwvm_language_debug::status;
static void put32(bytes& b, std::size_t at, std::uint32_t n) {
    for (unsigned i{}; i!=4; ++i) b[at+i]=std::byte((n>>(i*8))&255);
}
static void put64(bytes& b, std::size_t at, std::uint64_t n) {
    put32(b,at,std::uint32_t(n)); put32(b,at+4,std::uint32_t(n>>32));
}
static bytes actual_scalar() {
    bytes b{}; unsigned char magic[]{'U','W','L','F',0,0,1,0};
    for (unsigned i{}; i!=8; ++i) b[i]=std::byte(magic[i]);
    put32(b,8,1); put32(b,12,59008); put32(b,16,128); put32(b,20,9);
    put64(b,24,2); put64(b,32,1); put32(b,40,1); put32(b,44,1); put32(b,48,1);
    put32(b,56,128); put32(b,60,1664); put32(b,64,34432);
    put32(b,128,1); put32(b,132,UINT32_MAX); put32(b,172,1);
    put32(b,1664,1); put32(b,1668,UINT32_MAX); put32(b,1680,UWVM_LS_I64);
    put64(b,1688,42); put32(b,1712,UWVM_LS_DATA_KNOWN);
    return b;
}
int main() {
    unsigned rejected{}; auto good=actual_scalar(); uwvm_language_debug::snapshot result;
    assert(uwvm_language_debug::parse(good,result)==status::ok);
    assert(result.values[0].payload==42 && result.header.sequence==2);
    {
        uwvm_language_debug::snapshot alias;
        alias.text.assign(good.begin(),good.end());
        assert(uwvm_language_debug::parse(alias.text,alias)==status::ok);
        assert(alias.values[0].payload==42);
        alias.text.assign(good.begin(),good.end()); alias.text[0]=std::byte{};
        assert(uwvm_language_debug::parse(alias.text,alias)==status::malformed);
        assert(alias.text.empty());
    }
    auto refuse=[&](std::function<void(bytes&)> mutate) {
        auto bad=good; mutate(bad);
        assert(uwvm_language_debug::parse(bad,result)==status::malformed);
        assert(result.header.frame_count==0 && result.text.empty()); ++rejected;
    };
    refuse([](auto& b){b[0]=std::byte{};});
    refuse([](auto& b){put32(b,12,UINT32_MAX);});
    refuse([](auto& b){put32(b,20,8);}); // stale not LIVE
    refuse([](auto& b){put64(b,24,3);}); // incomplete odd sequence
    refuse([](auto& b){put64(b,24,0);});
    refuse([](auto& b){put64(b,32,2);}); // unknown layout, no GC-generation guess
    refuse([](auto& b){put32(b,44,17);});
    refuse([](auto& b){put32(b,48,513);});
    refuse([](auto& b){put32(b,52,UINT32_MAX);});
    refuse([](auto& b){put32(b,60,1660);});
    refuse([](auto& b){put32(b,76,1);});
    refuse([](auto& b){put32(b,132,1);}); // own frame parent
    refuse([](auto& b){put32(b,1680,11);});
    refuse([](auto& b){put32(b,1684,UWVM_LS_UNAVAILABLE);}); // contradictory value-known
    refuse([](auto& b){put32(b,1696,UINT32_MAX);put32(b,1700,1);});
    refuse([](auto& b){put32(b,1668,0);}); // root cannot claim self parent
    refuse([](auto& b){put32(b,48,2);}); // an orphan unvisited record
    refuse([](auto& b){put32(b,1680,UWVM_LS_BOOL);}); // payload42 is not bool
    refuse([](auto& b){put32(b,1680,UWVM_LS_NONE);});
    refuse([](auto& b){b[59007]=std::byte{1};}); // stale unused arena bytes
    {
        auto text=good; put32(text,52,3); put32(text,1680,UWVM_LS_UTF8);
        put64(text,1688,3); put32(text,1700,3);
        text[34432]=std::byte{'a'}; text[34433]=std::byte{}; text[34434]=std::byte{'b'};
        assert(uwvm_language_debug::parse(text,result)==status::ok);
        text[34432]=std::byte{'z'};
        assert(result.text[0]==std::byte{'a'}); // owned output has no input alias
        text[34432]=std::byte{0xc0}; text[34433]=std::byte{0x80};
        assert(uwvm_language_debug::parse(text,result)==status::malformed); ++rejected;
    }
    {
        // Four codepoints: ASCII, two-byte, four-byte, embedded NUL. Eight bytes.
        auto unicode=good; put32(unicode,52,8); put32(unicode,1680,UWVM_LS_UTF8);
        put64(unicode,1688,4); put32(unicode,1700,8);
        unsigned char data[]{'a',0xc3,0xa9,0xf0,0x9f,0x98,0x80,0};
        for (unsigned i{}; i!=8; ++i) unicode[34432+i]=std::byte(data[i]);
        assert(uwvm_language_debug::parse(unicode,result)==status::ok);
        assert(result.values[0].payload==4 && result.values[0].bytes_length==8);
        put64(unicode,1688,8); // UTF-8 bytes cannot substitute for codepoints.
        assert(uwvm_language_debug::parse(unicode,result)==status::malformed); ++rejected;
        put64(unicode,1688,3);
        assert(uwvm_language_debug::parse(unicode,result)==status::malformed); ++rejected;
        put64(unicode,1688,5); // A complete copy cannot omit an original codepoint.
        assert(uwvm_language_debug::parse(unicode,result)==status::malformed); ++rejected;
        put32(unicode,1684,UWVM_LS_VALUE_TRUNCATED);
        assert(uwvm_language_debug::parse(unicode,result)==status::ok);
        assert(result.values[0].payload==5);
        put64(unicode,1688,3); // Even a truncated copy cannot exceed its original.
        assert(uwvm_language_debug::parse(unicode,result)==status::malformed); ++rejected;
        put64(unicode,1688,UINT64_MAX);
        assert(uwvm_language_debug::parse(unicode,result)==status::ok);
        assert(result.values[0].payload==UINT64_MAX);
    }
    {
        auto binary=good; put32(binary,52,3); put32(binary,1680,UWVM_LS_BYTES);
        put64(binary,1688,3); put32(binary,1700,3);
        binary[34432]=std::byte{0xff}; binary[34433]=std::byte{}; binary[34434]=std::byte{0x80};
        assert(uwvm_language_debug::parse(binary,result)==status::ok);
        put64(binary,1688,0); // Former missing original-count payload.
        assert(uwvm_language_debug::parse(binary,result)==status::malformed); ++rejected;
        put64(binary,1688,4);
        assert(uwvm_language_debug::parse(binary,result)==status::malformed); ++rejected;
        put32(binary,1684,UWVM_LS_VALUE_TRUNCATED);
        assert(uwvm_language_debug::parse(binary,result)==status::ok);
        put64(binary,1688,2);
        assert(uwvm_language_debug::parse(binary,result)==status::malformed); ++rejected;
    }
    for (auto kind : {UWVM_LS_UTF8,UWVM_LS_BYTES}) {
        auto empty=good; put32(empty,1680,kind); put64(empty,1688,0);
        assert(uwvm_language_debug::parse(empty,result)==status::ok);
        put64(empty,1688,1);
        assert(uwvm_language_debug::parse(empty,result)==status::malformed); ++rejected;
        put32(empty,1684,UWVM_LS_VALUE_TRUNCATED);
        assert(uwvm_language_debug::parse(empty,result)==status::ok);
        put32(empty,1684,0); put32(empty,1712,0); // Unknown data stays unknown.
        assert(uwvm_language_debug::parse(empty,result)==status::malformed); ++rejected;
        put64(empty,1688,0);
        assert(uwvm_language_debug::parse(empty,result)==status::ok);
    }
    {
        auto unavailable=good; put32(unavailable,20,13); put32(unavailable,68,1);
        assert(uwvm_language_debug::parse(unavailable,result)==status::unavailable);
        assert(result.header.frame_count==0);
    }
    {
        auto tree=good; put32(tree,48,2); put32(tree,1680,UWVM_LS_LIST);
        put64(tree,1688,1); put32(tree,1704,1); put32(tree,1708,1); put32(tree,1712,6);
        put32(tree,1728,1); put32(tree,1732,0); put32(tree,1744,UWVM_LS_U64);
        put64(tree,1752,UINT64_MAX); put32(tree,1776,2);
        assert(uwvm_language_debug::parse(tree,result)==status::ok);
        assert(result.values[1].payload==UINT64_MAX);
        put64(tree,1688,0); // actual child count exceeds known element count
        assert(uwvm_language_debug::parse(tree,result)==status::malformed); ++rejected;
        put64(tree,1688,2); // complete known children cannot omit an element
        assert(uwvm_language_debug::parse(tree,result)==status::malformed); ++rejected;
        put32(tree,1684,UWVM_LS_VALUE_TRUNCATED);
        assert(uwvm_language_debug::parse(tree,result)==status::ok);
        put64(tree,1688,1); put32(tree,1684,0);
        put32(tree,1704,0); // parent references itself; actual parser refuses
        assert(uwvm_language_debug::parse(tree,result)==status::malformed); ++rejected;
    }
    assert(uwvm_language_debug::parse(std::span(good).first(128),result)==status::malformed);
    {
        auto dictionary=good; put32(dictionary,48,4); put32(dictionary,1680,UWVM_LS_DICT);
        put64(dictionary,1688,1); put32(dictionary,1704,1); put32(dictionary,1708,1); put32(dictionary,1712,6);
        put32(dictionary,1728,1); put32(dictionary,1732,0); put32(dictionary,1744,UWVM_LS_TUPLE);
        put64(dictionary,1752,2); put32(dictionary,1768,2); put32(dictionary,1772,2); put32(dictionary,1776,6);
        for (unsigned n=2; n!=4; ++n) {
            auto off=1664+n*64; put32(dictionary,off,1); put32(dictionary,off+4,1);
            put32(dictionary,off+16,UWVM_LS_I64); put64(dictionary,off+24,n); put32(dictionary,off+48,2);
        }
        assert(uwvm_language_debug::parse(dictionary,result)==status::ok);
        put64(dictionary,1688,0); // one pair cannot claim an empty dictionary
        assert(uwvm_language_debug::parse(dictionary,result)==status::malformed); ++rejected;
        put64(dictionary,1688,1);
        put32(dictionary,1744,UWVM_LS_LIST);
        assert(uwvm_language_debug::parse(dictionary,result)==status::malformed); ++rejected;
    }
    std::cout << "snapshot reader actual cases completed, rejected=" << rejected << '\n';
}
