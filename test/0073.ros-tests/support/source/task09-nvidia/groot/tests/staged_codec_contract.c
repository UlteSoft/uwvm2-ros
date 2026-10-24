/* Complete authored staged records; no native ROS, model or Gateway credit. */
#include "bounded_records.h"
#include <stdio.h>
#include <string.h>

#define T(name) roswasmc_bounded_2_uwvm_groot_bridge_##name
#define F(name, operation) roswasmc_bounded_2_uwvm_groot_bridge_##name##_##operation
static unsigned char wire[32768], original[32768];
static size_t length;
static unsigned checks, records;
static int check(int value, unsigned line) {
    ++checks;
    if (value)
        return 0;
    fprintf(stderr, "staged codec failure at line %u\n", line);
    return 1;
}
#define CHECK(value)                                                                               \
    do {                                                                                           \
        if (check((value), __LINE__))                                                              \
            return 1;                                                                              \
    } while (0)
#define ROUNDTRIP(name, input, output)                                                             \
    do {                                                                                           \
        CHECK(F(name, encode)(&(input), wire, sizeof wire, &length) == 0);                         \
        CHECK(F(name, decode)(wire, length, &(output)) == 0);                                      \
        ++records;                                                                                 \
    } while (0)
static T(msg_RGBDescriptor) descriptor, descriptor_out;
static T(msg_FloatTensor) tensor, tensor_out;
static T(msg_TextSequence) language, language_out;
static T(srv_BeginObservation_Request) begin, begin_out;
static T(srv_BeginObservation_Response) begun, begun_out;
static T(srv_WriteRGBChunk_Request) chunk, chunk_out;
static T(srv_WriteRGBChunk_Response) written, written_out;
static T(srv_ReleaseObservation_Request) release, release_out;
static T(srv_ReleaseObservation_Response) released, released_out;
static T(srv_InferUploadedAction_Request) infer, infer_out;
static T(srv_InferUploadedAction_Response) action, action_out;

int main(void) {
    size_t i, j, old_length;
    descriptor.key.size = 64;
    memset(descriptor.key.data, 'C', 64);
    descriptor.shape[0] = 1;
    descriptor.shape[1] = 1;
    descriptor.shape[2] = 224;
    descriptor.shape[3] = 224;
    descriptor.shape[4] = 3;
    ROUNDTRIP(msg_RGBDescriptor, descriptor, descriptor_out);
    CHECK(descriptor_out.key.size == 64 &&
          !memcmp(descriptor_out.key.data, descriptor.key.data, 64));
    for (i = 0; i < 5; ++i)
        CHECK(descriptor_out.shape[i] == descriptor.shape[i]);

    tensor.key.size = 3;
    memcpy(tensor.key.data, "arm", 3);
    tensor.shape[0] = 1;
    tensor.shape[1] = 1;
    tensor.shape[2] = 320;
    tensor.values.size = 320;
    for (i = 0; i < 320; ++i)
        tensor.values.data[i] = ((float)i - 160.0f) * 0.25f;
    ROUNDTRIP(msg_FloatTensor, tensor, tensor_out);
    CHECK(tensor_out.key.size == 3 && !memcmp(tensor_out.key.data, "arm", 3));
    for (i = 0; i < 3; ++i)
        CHECK(tensor_out.shape[i] == tensor.shape[i]);
    CHECK(tensor_out.values.size == 320);
    for (i = 0; i < 320; ++i)
        CHECK(tensor_out.values.data[i] == tensor.values.data[i]);

    language.key.size = 4;
    memcpy(language.key.data, "task", 4);
    language.shape[0] = 1;
    language.shape[1] = 1;
    language.texts.size = 1;
    language.texts.data[0].size = 7;
    memcpy(language.texts.data[0].data, "pick \xc2\xb5", 7);
    ROUNDTRIP(msg_TextSequence, language, language_out);
    CHECK(language_out.key.size == 4 && !memcmp(language_out.key.data, "task", 4));
    CHECK(language_out.shape[0] == 1 && language_out.shape[1] == 1);
    CHECK(language_out.texts.size == 1 && language_out.texts.data[0].size == 7);
    CHECK(!memcmp(language_out.texts.data[0].data, "pick \xc2\xb5", 7));

    begin.observation_nonce = UINT64_C(0x8877665544332211);
    begin.video.size = 3;
    for (i = 0; i < 3; ++i)
        begin.video.data[i] = descriptor;
    ROUNDTRIP(srv_BeginObservation_Request, begin, begin_out);
    CHECK(wire[0] == 0x11 && wire[1] == 0x22 && wire[7] == 0x88);
    CHECK(begin_out.observation_nonce == begin.observation_nonce && begin_out.video.size == 3);
    for (i = 0; i < 3; ++i) {
        CHECK(begin_out.video.data[i].key.size == 64);
        CHECK(!memcmp(begin_out.video.data[i].key.data, descriptor.key.data, 64));
        for (j = 0; j < 5; ++j)
            CHECK(begin_out.video.data[i].shape[j] == descriptor.shape[j]);
    }
    begun.status = 125;
    begun.observation_nonce = UINT64_MAX;
    ROUNDTRIP(srv_BeginObservation_Response, begun, begun_out);
    CHECK(begun_out.status == 125 && begun_out.observation_nonce == UINT64_MAX);

    chunk.observation_nonce = UINT64_MAX;
    chunk.offset = UINT32_MAX;
    chunk.key.size = 64;
    memset(chunk.key.data, 'K', 64);
    chunk.pixels.size = 768;
    for (i = 0; i < 768; ++i)
        chunk.pixels.data[i] = (uint8_t)(i % 251);
    ROUNDTRIP(srv_WriteRGBChunk_Request, chunk, chunk_out);
    CHECK(chunk_out.observation_nonce == UINT64_MAX && chunk_out.offset == UINT32_MAX);
    CHECK(chunk_out.key.size == 64 && !memcmp(chunk_out.key.data, chunk.key.data, 64));
    CHECK(chunk_out.pixels.size == 768);
    for (i = 0; i < 768; ++i)
        CHECK(chunk_out.pixels.data[i] == (uint8_t)(i % 251));
    written.status = 90;
    written.observation_nonce = UINT64_MAX;
    written.accepted_bytes = UINT32_MAX;
    ROUNDTRIP(srv_WriteRGBChunk_Response, written, written_out);
    CHECK(written_out.status == 90 && written_out.observation_nonce == UINT64_MAX &&
          written_out.accepted_bytes == UINT32_MAX);

    release.observation_nonce = UINT64_MAX;
    ROUNDTRIP(srv_ReleaseObservation_Request, release, release_out);
    CHECK(release_out.observation_nonce == UINT64_MAX);
    released.status = 110;
    released.observation_nonce = UINT64_MAX;
    ROUNDTRIP(srv_ReleaseObservation_Response, released, released_out);
    CHECK(released_out.status == 110 && released_out.observation_nonce == UINT64_MAX);

    infer.application_nonce = UINT64_MAX;
    infer.observation_nonce = UINT64_C(0x8877665544332211);
    infer.state.size = 4;
    infer.language.size = 2;
    for (i = 0; i < 4; ++i)
        infer.state.data[i] = tensor;
    for (i = 0; i < 2; ++i)
        infer.language.data[i] = language;
    ROUNDTRIP(srv_InferUploadedAction_Request, infer, infer_out);
    CHECK(infer_out.application_nonce == UINT64_MAX &&
          infer_out.observation_nonce == infer.observation_nonce);
    CHECK(infer_out.state.size == 4 && infer_out.language.size == 2);
    for (i = 0; i < 4; ++i) {
        CHECK(infer_out.state.data[i].key.size == 3 &&
              !memcmp(infer_out.state.data[i].key.data, "arm", 3));
        for (j = 0; j < 3; ++j)
            CHECK(infer_out.state.data[i].shape[j] == tensor.shape[j]);
        CHECK(infer_out.state.data[i].values.size == 320);
        for (j = 0; j < 320; ++j)
            CHECK(infer_out.state.data[i].values.data[j] == tensor.values.data[j]);
    }
    for (i = 0; i < 2; ++i) {
        CHECK(infer_out.language.data[i].key.size == 4 &&
              !memcmp(infer_out.language.data[i].key.data, "task", 4));
        CHECK(infer_out.language.data[i].shape[0] == 1 && infer_out.language.data[i].shape[1] == 1);
        CHECK(infer_out.language.data[i].texts.size == 1 &&
              infer_out.language.data[i].texts.data[0].size == 7);
        CHECK(!memcmp(infer_out.language.data[i].texts.data[0].data, "pick \xc2\xb5", 7));
    }

    action.status = 0;
    action.application_nonce = UINT64_MAX;
    action.action.size = 8;
    action.info_is_empty = 1;
    for (i = 0; i < 8; ++i)
        action.action.data[i] = tensor;
    ROUNDTRIP(srv_InferUploadedAction_Response, action, action_out);
    CHECK(action_out.status == 0 && action_out.application_nonce == UINT64_MAX &&
          action_out.info_is_empty == 1 && action_out.action.size == 8);
    for (i = 0; i < 8; ++i) {
        CHECK(action_out.action.data[i].key.size == 3 &&
              !memcmp(action_out.action.data[i].key.data, "arm", 3));
        for (j = 0; j < 3; ++j)
            CHECK(action_out.action.data[i].shape[j] == tensor.shape[j]);
        CHECK(action_out.action.data[i].values.size == 320);
        for (j = 0; j < 320; ++j)
            CHECK(action_out.action.data[i].values.data[j] == tensor.values.data[j]);
    }

    /* Capacity, truncation and malformed UTF-8 must be whole-value failures. */
    memset(wire, 0x5a, sizeof wire);
    memcpy(original, wire, sizeof wire);
    old_length = 123;
    CHECK(F(srv_WriteRGBChunk_Request, encode)(&chunk, wire, 1, &old_length) == 90);
    CHECK(old_length == 123 && !memcmp(wire, original, sizeof wire));
    CHECK(F(srv_WriteRGBChunk_Request, encode)(&chunk, wire, sizeof wire, &length) == 0);
    chunk_out.offset = 123;
    chunk_out.observation_nonce = 456;
    CHECK(F(srv_WriteRGBChunk_Request, decode)(wire, length - 1, &chunk_out) == 22);
    CHECK(chunk_out.offset == 123 && chunk_out.observation_nonce == 456);
    chunk.key.size = 3;
    chunk.key.data[0] = 0xed;
    chunk.key.data[1] = 0xa0;
    chunk.key.data[2] = 0x80;
    CHECK(F(srv_WriteRGBChunk_Request, encode)(&chunk, wire, sizeof wire, &length) == 22);
    printf(
        "{\"scope\":\"authored_staged_canonical_c_codec\",\"record_domains\":%u,\"field_checks\":%u,\"model_calls\":0,\"native_ros\":0}\n",
        records, checks);
    return 0;
}
