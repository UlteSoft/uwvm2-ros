# Real contract producer and driver recipe (not executed)

1. Root first composes the two Runtime include hunks and the two existing header
   after bodies with the actual HLL/current Source. Never overwrite Runtime with
   an old whole-file copy. Independently requalify that actual composed Runtime
   and keep its full manifest, compiler/link/import census and resource evidence.
2. Use an actual admitted WAT producer to compile frame_reader.wat and the two
   mutation-import fixtures. Preserve the original WAT and exact output hashes.
   Run actual full Wasm validation/metadata; no expected hash or ordinal is
   supplied by this package. The positive fixture exports `read_signal` (one i32
   result), `wasi_thread_start` (two i32 parameters) and a 64KiB shared memory.
   Its fd3/cookie0x105 are explicit contract policy inputs, not production grants.
   Its complete positive import closure includes fd_read AND poll_oneoff; do not
   substitute an immediate read, a timer/clock grant, a sleep loop, or a synthetic
   readiness callback. The real single-consumer FD_READ wait returns only an
   actual validated event; the subsequent read still validates actual 32-byte
   progress and all URS1 fields. Root's real Group deadline/stop owns cancellation
   independently of Guest clock authority when no frame arrives.
3. Use the real Native Runtime loader to own, parse, initialize and seal the
   selected full_source_instance. Supply that genuine initializer callback and
   its actual numeric export ordinal to host_contract.hpp. The callback is only
   borrowed loader state: the existing replace_full_source_after_drain_host_api
   and actual start_main admission independently validate Source/epoch/ownership.
   Do not supply a callback that merely returns true or a synthetic Group.
4. In a fresh dedicated process call two_actual_sources with two actual Source
   initializations and finite Root-selected threaded_profile_limits. Reserve
   enough real shared memory (64KiB), pipe maximum (<=1MiB) plus fixed owner budget,
   compiler/native stack/code/storage/tables/initializer limits. Limits are NOT
   fabricated here or inferred from an alias. Give one actual steady absolute
   deadline covering both sources, and preserve every pending join/error result.
5. Attach host_contract.cc to that real existing executable with Attach.cmake;
   it is not an alternate Runtime. There is no main that could report success
   without running actual source replacement/main/native join. The driver must
   invoke the strong wrappers and require `failure==0`, both completed real join
   records, actual Source generations distinct and both Guest i32 results1.
   Preserve raw results and report every real check count; do not label missing
   driver invocation as a skipped success. Failure with a live/pending Group
   retains ownership; Root must invoke actual bounded retirement and never
   destroy/detach that owner or reuse Source while it remains pending.
6. Run each forbidden-import source in its own fresh process using the real
   initializer and forbidden_import_source. Require actual profile rejection,
   no sender/map and no write. If the actual producer/full validator rejects
   earlier, retain that as a distinct earlier gate, not this helper's execution.
7. Separately run pending-join/races/fault outcomes with the genuine existing
   Runtime/native test seams. This candidate introduces no fake syscall or
   force-pass shim to fabricate them. All claimed outcomes require raw evidence.
8. Only after the complete SDK33/libc++/threads/EH producer and original default
   Node endpoints are actually admitted, compose the peer-closed Signal25 port.
   Root configures its private fd/cookie before original main and retains this
   Native sender. Send only after genuine authorized ready evidence. The actual
   pipe receiver then invokes original graceful Context shutdown. Root forced
   Group retirement remains separate. No raw sigaction handler may call push.

All steps are future actual work. This package executed none of them. There is
no automatic host fd/clock right, executable installation, CLI switch, signal
handler registration, SDK download or producer invocation in these files.
