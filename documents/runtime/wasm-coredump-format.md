# Wasm coredump encoding

The encoder in src/uwvm2/runtime/diagnostic/wasm_coredump.h implements the
[WebAssembly coredump convention](https://github.com/WebAssembly/tool-conventions/blob/main/Coredump.md).
The convention is experimental. A dump is a WebAssembly binary for inspection;
it is not an executable module to instantiate.

This document specifies the encoding API and its extensions. It does not imply
that every execution mode already has a qualified stopped-state collector.

## Collection contract

The measure(dump_view) function validates metadata and computes the entire
serialized length. It does not allocate memory or invoke a memory reader or
writer. The encode function performs the same preflight, then writes through a
synchronous callback. A writer must accept the complete supplied span or report
failure. Failed output must be discarded; only success may publish a finished dump.

The collector owns the stopped-state proof, generation and owner validation,
capture leases and privacy policy. All views and leases must remain stable until
encoding returns. Private callback contexts are never serialized. Memory readers
receive only a guest offset and a bounded destination span. A reader must not
expose VM memory, native stacks or arbitrary process addresses. On a hardware-MTE
owner, the collector must reconstruct the allocation tag for each granule before
reading its native mapping; an untagged raw native base is not a safe reader.

Memory transfer uses a 4 KiB buffer. Tag transfer uses fixed arrays of 128 samples.
These are temporary transfer sizes, not limits on total memory, frames, values or
output size. Allocation, source-read and destination-write failures are explicit.

## Standard sections

Output contains the Wasm header, core, coremodules, coreinstances, one corestack
per thread, and standard memory, global and data sections.

An instance lists dump memory and global indices in its original instance order.
Several instances may refer to the same dump owner index. The collector
deduplicates owners; aliases do not require duplicate memory snapshots.

Memory declarations support memory32, memory64 and shared memories. Committed
length must be a multiple of 64 KiB. Shared declarations have equal current minimum
and maximum lengths in the dump. Data segments use explicit memory indices and
the matching i32.const or i64.const expression. Regions must be ordered,
non-overlapping and within the owner. Complete capture requires exact coverage;
partial permits holes; unavailable has no regions.

Frames are ordered youngest to oldest. Integer local and operand values use signed
LEB128. Floating-point values preserve their little-endian IEEE bit pattern,
including NaN payloads. Missing values use the convention's 0x01 marker.
An unknown code offset is zero and marked unavailable in metadata.

Globals are immutable numeric values. A missing or unsupported reference/vector
global is an explicit missing i32.const 0 placeholder, identified as unavailable
with its original type in metadata. It must never be presented as an observed
zero-valued global.

## Extension uwvm.core.meta, version 1

Integers below use unsigned LEB128 except u8. Names use Wasm UTF-8 name encoding.
This section occurs once:

~~~
version:u32 = 1
coherent:u8
memory-chunks-required:u8
reason:name
memories:vec(
  committed-bytes:u64
  capture:u8
  region-count:u64
  captured-bytes:u64
  memtag-enabled:u8
  if enabled:
    tag-bits:u8
    granule-log2:u8
    tag-capture:u8
    tag-region-count:u64
)
globals:vec(available:u8 original-wasm-type:u8)
threads:vec(capture:u8 frames:vec(code-offset-available:u8))
~~~

Capture codes are 0=complete, 1=partial, 2=unavailable. Boolean and availability
fields are zero or one. Coherent is an assertion by the qualified collector, not
a proof constructed by the encoder. Region counts describe the collector's
requested ranges. Individual tag availability distinguishes unavailable tags
from tag zero. An unavailable original global type may be zero.

## Extension uwvm.core.memory-chunks, version 1

A Wasm section has a u32 payload length. If all requested memory data cannot fit
one standard data section, the encoder emits an empty standard data section,
sets memory-chunks-required=1, and streams **all** requested bytes into repeated
custom sections:

~~~
version:u32 = 1
memory-index:u32
guest-offset:u64
byte-count:u32
bytes:byte[byte-count]
~~~

Sections are in memory-index and guest-offset order. Each fits the standard
section-length encoding. Aggregate capture is not truncated at 4 GiB. Readers
without this extension see an explicitly partial standard image; they must not
claim that empty standard data represents observed zero-filled memory.

Chunk capacity follows the Wasm section-length limit minus framing space, not an
application policy limit. Aggregate arithmetic overflow is an error.

## Extension uwvm.core.memtag, version 1

Repeated sections preserve allocation tags independently of memory-byte coverage:

~~~
version:u32 = 1
memory-index:u32
first-granule:u64
sample-count:u32
samples:
  repeated sample-count times:
    available:u8
    tag:u32-little-endian
~~~

Granule size is 1 shifted left by granule-log2 bytes. Tag width is 1–32 bits.
Unavailable samples have available=0 and tag=0. Available tags fit the declared
width. Tags are numeric values, not native pointers or hardware tag addresses.

## Validation and publication

Invalid indices, ranges, UTF-8, value kinds, capture declarations and arithmetic
overflow fail preflight without invoking the writer. Streaming source or
destination failure returns failure and the count of fully accepted output
bytes. A malformed returned tag is an error. No failure becomes a successful but
silently incomplete dump.

Process-exit policy and RELEASE/DEV permissions belong to the runtime collector
and command controller. This encoder does not grant RELEASE code arbitrary
memory, local-variable or native-state access.
