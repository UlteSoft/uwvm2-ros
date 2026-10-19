/* Development Source: unexecuted bounded language snapshots, no Host ABI. */
#ifndef UWVM_LANGUAGE_SNAPSHOT_H
#define UWVM_LANGUAGE_SNAPSHOT_H
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* Serialized little-endian, with no native pointers or PyObject references.
 * A Guest counter, frame ID or LIVE bit is presentation DATA, never a Runtime
 * stop token, publication identity, memory owner or permission. The Host reader
 * must obtain the root from an actual current DWARF parameter/global location
 * inside the canonical source-memory transaction. This header mints none. */
enum {
    UWVM_LS_SCHEMA = 1, UWVM_LS_HEADER_BYTES = 128,
    UWVM_LS_FRAME_BYTES = 96, UWVM_LS_VALUE_BYTES = 64,
    UWVM_LS_MAX_FRAMES = 16, UWVM_LS_MAX_VALUES = 512,
    UWVM_LS_MAX_TEXT = 24576, UWVM_LS_MAX_CHILDREN = 32,
    UWVM_LS_MAX_DEPTH = 4, UWVM_LS_TOTAL_BYTES = 59008,
    UWVM_LS_FRAME_OFFSET = 128, UWVM_LS_VALUE_OFFSET = 1664,
    UWVM_LS_TEXT_OFFSET = 34432
};
enum { UWVM_LS_PYTHON = 1, UWVM_LS_KOTLIN = 2, UWVM_LS_MONO = 3, UWVM_LS_GO = 4 };
enum { UWVM_LS_LIVE = 1, UWVM_LS_TRUNCATED = 2, UWVM_LS_CAPTURE_ERROR = 4,
       UWVM_LS_INSTRUMENTED = 8 };
enum { UWVM_LS_UNKNOWN = 0, UWVM_LS_NONE = 1, UWVM_LS_BOOL = 2,
       UWVM_LS_I64 = 3, UWVM_LS_F64 = 4, UWVM_LS_UTF8 = 5,
       UWVM_LS_BYTES = 6, UWVM_LS_LIST = 7, UWVM_LS_TUPLE = 8,
       UWVM_LS_DICT = 9, UWVM_LS_U64 = 10 };
enum { UWVM_LS_ARGUMENT = 1, UWVM_LS_VALUE_TRUNCATED = 2,
       UWVM_LS_UNAVAILABLE = 4 };
enum { UWVM_LS_NAME_KNOWN = 1, UWVM_LS_DATA_KNOWN = 2,
       UWVM_LS_CHILDREN_KNOWN = 4 };
enum { UWVM_LS_FILE_KNOWN = 1, UWVM_LS_FUNCTION_KNOWN = 2,
       UWVM_LS_LINE_KNOWN = 4, UWVM_LS_INSTRUCTION_KNOWN = 8,
       UWVM_LS_ARGUMENTS_KNOWN = 16 };
#define UWVM_LS_NO_PARENT UINT32_MAX

typedef struct uwvm_language_snapshot_header {
    uint8_t magic[8];
    uint32_t schema, total_bytes, header_bytes, flags;
    uint64_t sequence, layout_generation;
    uint32_t language, frame_count, value_count, text_bytes;
    uint32_t frame_offset, value_offset, text_offset, status, event;
    uint32_t reserved[13];
} uwvm_language_snapshot_header;

typedef struct uwvm_language_snapshot_frame {
    uint32_t id, parent, line, flags;
    uint64_t instruction;
    uint32_t file_offset, file_length, function_offset, function_length;
    uint32_t value_first, value_count, argument_count, known;
    uint32_t reserved[10];
} uwvm_language_snapshot_frame;

typedef struct uwvm_language_snapshot_value {
    uint32_t frame_id, parent_value, name_offset, name_length, kind, flags;
    uint64_t payload;
    uint32_t bytes_offset, bytes_length, child_first, child_count, known;
    uint32_t reserved[3];
} uwvm_language_snapshot_value;

typedef struct uwvm_language_snapshot {
    uwvm_language_snapshot_header header;
    uwvm_language_snapshot_frame frames[UWVM_LS_MAX_FRAMES];
    uwvm_language_snapshot_value values[UWVM_LS_MAX_VALUES];
    uint8_t text[UWVM_LS_MAX_TEXT];
} uwvm_language_snapshot;

#if defined(__cplusplus)
#define UWVM_LS_ASSERT static_assert
#else
#define UWVM_LS_ASSERT _Static_assert
#endif
UWVM_LS_ASSERT(sizeof(uwvm_language_snapshot_header) == UWVM_LS_HEADER_BYTES, "header layout");
UWVM_LS_ASSERT(sizeof(uwvm_language_snapshot_frame) == UWVM_LS_FRAME_BYTES, "frame layout");
UWVM_LS_ASSERT(sizeof(uwvm_language_snapshot_value) == UWVM_LS_VALUE_BYTES, "value layout");
UWVM_LS_ASSERT(offsetof(uwvm_language_snapshot, frames) == UWVM_LS_FRAME_OFFSET, "frames offset");
UWVM_LS_ASSERT(offsetof(uwvm_language_snapshot, values) == UWVM_LS_VALUE_OFFSET, "values offset");
UWVM_LS_ASSERT(offsetof(uwvm_language_snapshot, text) == UWVM_LS_TEXT_OFFSET, "text offset");
UWVM_LS_ASSERT(sizeof(uwvm_language_snapshot) == UWVM_LS_TOTAL_BYTES, "snapshot bound");
#undef UWVM_LS_ASSERT

static inline int uwvm_language_snapshot_reset(uwvm_language_snapshot *out,
                                               uint32_t language, uint64_t sequence) {
    static const uint8_t magic[8] = {'U','W','L','F',0,0,1,0};
    memset(out, 0, sizeof(*out));
    uint32_t endian_probe = 1;
    unsigned char order[sizeof(endian_probe)];
    memcpy(order, &endian_probe, sizeof(order));
    if (order[0] != 1) return 0; /* Reject native BE producers; reader stays portable. */
    memcpy(out->header.magic, magic, sizeof(magic));
    out->header.schema = UWVM_LS_SCHEMA;
    out->header.total_bytes = UWVM_LS_TOTAL_BYTES;
    out->header.header_bytes = UWVM_LS_HEADER_BYTES;
    out->header.sequence = sequence;
    out->header.layout_generation = 1;
    out->header.language = language;
    out->header.frame_offset = UWVM_LS_FRAME_OFFSET;
    out->header.value_offset = UWVM_LS_VALUE_OFFSET;
    out->header.text_offset = UWVM_LS_TEXT_OFFSET;
    return 1;
}

/* Reserve siblings BEFORE recursing so each parent's direct children occupy a
 * finite contiguous range. Root values in a frame obey the same rule. */
static inline uint32_t uwvm_language_snapshot_reserve(uwvm_language_snapshot *out,
                                                     uint32_t count) {
    uint32_t first = out->header.value_count;
    if (first > UWVM_LS_MAX_VALUES || count > UWVM_LS_MAX_VALUES - first) {
        out->header.flags |= UWVM_LS_TRUNCATED;
        return UWVM_LS_NO_PARENT;
    }
    out->header.value_count += count;
    return first;
}

/* UTF-8 names and byte payloads share a byte arena. Offsets are arena-relative;
 * their kind distinguishes text from arbitrary bytes, including embedded NUL.
 * Truncation is explicit. Producers must split UTF-8 only at codepoint ends. */
static inline int uwvm_language_snapshot_bytes(uwvm_language_snapshot *out,
                                               void const *bytes, uint32_t count,
                                               uint32_t *offset) {
    if (out->header.text_bytes > UWVM_LS_MAX_TEXT ||
        count > UWVM_LS_MAX_TEXT - out->header.text_bytes || offset == NULL ||
        (count != 0 && bytes == NULL)) {
        out->header.flags |= UWVM_LS_TRUNCATED;
        return 0;
    }
    *offset = out->header.text_bytes;
    if (count) memcpy(out->text + *offset, bytes, count);
    out->header.text_bytes += count;
    return 1;
}
#endif
