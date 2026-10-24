# SDK typed parameter-event encoding tests

Run on Linux in a readable cgroup v2 hierarchy with an effective 64 GiB memory limit. Use a fresh output path. The driver checks an 8 GiB compiler estimate plus a 16 GiB reserve before each build and records actual commands, logs, normalized emitted MMD inputs, native ELF PID/parent/device/inode/SHA, ancestor limits and OOM deltas, and cleanup.

```sh
python3 test/0073.ros-tests/support/source/ros_sdk_event_encode/run.py --work /tmp/uwvm-ros-sdk-encode-test --compiler clang++
```

`--source` defaults to this repository; `--compiler-library-path` can select the compiler's runtime libraries. C++20 and C++26 each run with libstdc++ and libc++, all with ASan/UBSan and non-PIE ELF binaries. Each profile runs 110 cases and 756 checks: 54 successful encodings and 56 rejections. The native probe stops inside its actual ELF for live identity capture before execution.

`before-encoder.inc` is the exact pre-optimization event encoder, frozen from codec SHA `9503171d7fce61006b200561c777e1b9cdf3c372562405daafdb96ffa5f53268`. It is a test oracle, not production code. Each case compares acceptance and complete wire bytes with this reference; explicit Boolean and Int64 golden bytes separately pin the fixed-width little-endian envelope. A scoped global allocation counter measures only the production encoder call, excluding fixture creation, the reference encoder, decoding, and printing.

The cases cover tags 0–9, kinds 1–3, empty arrays/text, invalid UTF-8, Boolean/finite-number constraints, metadata/time boundaries, element and complete-envelope limits, and transactional rejection. Deletion tag 0 is permitted only with kind 3. Kind 3 emits metadata and an empty payload for every recognized tag without validating or copying undefined prior-value fields.

All successes must allocate exactly one final wire buffer, with allocated bytes equal to its size. Rejections must allocate zero bytes and preserve the caller's four-byte output sentinel. The production format keeps fixed-width little-endian u32/u64 values; no LEB128 conversion is involved. This is an SDK encoder test, without Worker/Gateway/ROS network execution or throughput claims.

Original evidence remains under `/home/macromodel/.local/share/uwvm-ros-extension/`:

- `20261013-sdk-event-encode-before-r1`: probe compile FAIL from passing `char const*` directly to fast_io printing; report SHA `2f14a7d8a556a1069dfd13ccf9ab664cc960bee5966ba9130d9149d5dc57cb9d`. The probe uses explicit string views in subsequent attempts.
- `20261013-sdk-event-encode-before-r2`: four profiles PASS, 110 cases and 702 checks each; report SHA `90672c2273a67690837941ebbfe6aa165a113055d85be6471b378939f927a6f3`. Actual ordinary successful encodings used 2–5 allocations; metadata-only deletions already used one. The exact-envelope String wire is 4,193,786 bytes, while its old encoder allocates 8,387,530 bytes cumulatively.
- `20261013-sdk-event-encode-after-r2`: four profiles PASS, 110 cases and 756 checks each, one allocation per success and zero per refusal. Report SHA `4872ece29d88c74d8743cc3b8747ac2fe7e62653f1b01c2c081c8a26618e9cb9`; independent read-only verification has 11,314 checks, SHA `5d69874c02646e81c418416f45908704e7039747776793795680309f8ed9b356`.
- `20261013-sdk-event-encode-after-r3`: this permanent driver was independently rerun with the frozen contracts candidate, all four profiles/440 cases PASS; report SHA `105f862e2f1b8b0c4e8ccbb0b91e7618791f053c8799e83346c1679f58dd7517`.

The permanent test files were added after the contracts main candidate was frozen. Their own fresh builds are documented above; they were not compiled by that main build.
