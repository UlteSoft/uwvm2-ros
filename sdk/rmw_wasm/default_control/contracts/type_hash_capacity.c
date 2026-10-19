/** @file Canonical GetTypeDescription hash boundary and transaction contract.
 * Include the actual bounded_records.h produced from the reviewed catalog.
 * This C11 contract never opens ROS, acquires a capability or fabricates a
 * native type-support/hash export. Its hash literal is local codec data only.
 * Run only in the later unified test phase under an external bounded harness.
 */
#include "bounded_records.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef roswasmc_bounded_2_type_description_interfaces_srv_GetTypeDescription_Request Request;
#define request_default roswasmc_bounded_2_type_description_interfaces_srv_GetTypeDescription_Request_default
#define request_valid roswasmc_bounded_2_type_description_interfaces_srv_GetTypeDescription_Request_valid
#define request_encode roswasmc_bounded_2_type_description_interfaces_srv_GetTypeDescription_Request_encode
#define request_decode roswasmc_bounded_2_type_description_interfaces_srv_GetTypeDescription_Request_decode

/* These are schema/layout preconditions, not a substitute for exercising the
 * codec. In particular, the old 64-byte header must fail compilation safely. */
_Static_assert(sizeof(((Request *)0)->type_hash.data) == 71,
               "A complete RIHS01_ string requires 7 + 64 bytes");
static unsigned char const hash[] =
    "RIHS01_0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef";
_Static_assert(sizeof hash - 1 == 71, "Complete local canonical hash fixture");

/** Assert transactional refusal using the destination's actual byte snapshot.
 * The input storage is distinct; do not compare padding of successful structs. */
static void expect_decode_refusal(void const *wire, size_t length)
{
    Request destination;
    unsigned char saved[sizeof destination];
    memset(&destination, 0xa5, sizeof destination);
    memcpy(saved, &destination, sizeof saved);
    assert(request_decode(wire, length, &destination) == UWVM_BOUNDED_INVALID);
    assert(memcmp(&destination, saved, sizeof saved) == 0);
}

int main(void)
{
    Request request, decoded;
    unsigned char output[160], saved[sizeof output], expected[80] = {0};
    /* Independent little-endian wire: empty type_name, 71 hash bytes, true.
     * Construct the expected wire without using generated cursor helpers. */
    size_t length = 0;
    assert(request_default(&request) == 0);
    assert(request.type_name.size == 0 && request.type_hash.size == 0);
    assert(request.include_type_sources == 1 && request_valid(&request));
    memset(output, 0xa5, sizeof output);
    assert(request_encode(&request, output, sizeof output, &length) == 0);
    assert(length == 9);
    assert(memcmp(output, (unsigned char[9]){0,0,0,0,0,0,0,0,1}, 9) == 0);
    memset(&decoded, 0xa5, sizeof decoded);
    assert(request_decode(output, length, &decoded) == 0);
    assert(decoded.type_name.size == 0 && decoded.type_hash.size == 0);
    assert(decoded.include_type_sources == 1);

    request.type_hash.size = (uint32_t)(sizeof hash - 1);
    memcpy(request.type_hash.data, hash, sizeof hash - 1);
    assert(request_valid(&request));
    expected[4] = 71;
    memcpy(expected + 8, hash, sizeof hash - 1);
    expected[79] = 1;
    memset(output, 0xa5, sizeof output);
    assert(request_encode(&request, output, sizeof output, &length) == 0);
    assert(length == sizeof expected && memcmp(output, expected, length) == 0);
    /* Bytes outside the encoded prefix remain caller-owned and untouched. */
    memset(saved, 0xa5, sizeof saved);
    assert(memcmp(output + length, saved + length, sizeof output - length) == 0);
    assert(request_decode(output, length, &decoded) == 0);
    assert(decoded.type_name.size == 0 && decoded.type_hash.size == 71);
    assert(memcmp(decoded.type_hash.data, hash, sizeof hash - 1) == 0);
    assert(decoded.include_type_sources == 1);

    /* A valid complete value with a short buffer is error90, with no writes. */
    memset(output, 0x5a, sizeof output);
    memcpy(saved, output, sizeof saved);
    length = 12345;
    assert(request_encode(&request, output, sizeof expected - 1, &length)
           == UWVM_BOUNDED_TOO_LARGE);
    assert(length == 12345 && memcmp(output, saved, sizeof saved) == 0);

    /* Invalid local metadata is error22, before an out-of-bounds data read.
     * Never copy 72 bytes into the 71-byte inline field. Native quota90 belongs
     * to the separate real native-to-canonical converter contract. */
    request.type_hash.size = 72;
    assert(!request_valid(&request));
    assert(request_encode(&request, output, sizeof output, &length)
           == UWVM_BOUNDED_INVALID);
    assert(length == 12345 && memcmp(output, saved, sizeof saved) == 0);

    /* Supply a complete independent oversized field, not just a truncated
     * buffer that could fail for the wrong reason. Its physical byte array
     * has room for all 72 bytes, the prefix and the final boolean. */
    {
        unsigned char oversized[81] = {0};
        oversized[4] = 72;
        memcpy(oversized + 8, hash, sizeof hash - 1);
        oversized[79] = 'a';
        oversized[80] = 1;
        expect_decode_refusal(oversized, sizeof oversized);
    }
    expect_decode_refusal(expected, sizeof expected - 1);
    {
        unsigned char trailing[sizeof expected + 1];
        memcpy(trailing, expected, sizeof expected);
        trailing[sizeof expected] = 0;
        expect_decode_refusal(trailing, sizeof trailing);
    }
    puts("PASS_CANONICAL_ONLY_TYPE_HASH_CAPACITY_NOT_NATIVE");
    return 0;
}
