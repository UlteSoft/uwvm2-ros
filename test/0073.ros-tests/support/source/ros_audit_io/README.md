# Audit sink regression tests

Run on Linux in a readable cgroup v2 hierarchy with an effective 64 GiB memory limit. The driver requires a fresh output path and checks an 8 GiB compiler estimate plus a 16 GiB reserve before compiling. It records the compiler identity, actual commands, logs, emitted MMD inputs, native ELF PID/parent/device/inode/SHA, readable cgroup ancestor limits and OOM deltas, and cleanup.

```sh
python3 test/0073.ros-tests/support/source/ros_audit_io/run.py --work /tmp/uwvm-ros-audit-test --compiler clang++
```

`--source` defaults to this repository; `--compiler-library-path` can select the compiler's runtime libraries. The profiles are C++26 with libstdc++ and ASan/UBSan, C++26 with libc++ and ASan/UBSan, and C++20 with libstdc++ and exceptions disabled. Each profile runs 24 sink scenarios. The native probe stops inside its actual ELF while the driver captures its live identity, then resumes with a three-second test timeout.

The helper is compiled in this standalone test. `host.h` is copied and hashed for provenance but is not compiled by it. The postfreeze permanent tests are separate from the frozen `20261013-contracts-integrated-r1` main build.

Pipes accept one complete record plus newline atomically when it fits both the bounded buffer and `PIPE_BUF`; unavailable or oversized pipe records are dropped. Socket and TTY writes are nonblocking, but an error or partial write can leave partial output and counts as dropped. Regular files retain the original shared file position and append semantics and have no hard I/O deadline; errors can leave partial output. A TTY backlog can admit an entire record after the kernel forwards existing bytes, so its fixture checks the actual complete/dropped result and exact record tail rather than assuming refusal.

The delivered/dropped counters are saturating trusted-runtime diagnostics. They add no Guest ABI or unbudgeted summary records. JSON records and the existing capability accounting are unchanged. The tests check stderr flags, caller errno, SIGPIPE handler/mask/pending preservation, closed readers, concurrent pipe records, and counter saturation.

Original evidence remains under `/home/macromodel/.local/share/uwvm-ros-extension/`:

- `20261013-audit-io-before-r1`: successful compilation followed by a fixture FAIL waiting for buffered stdout; report SHA `086bedd7257d6600c0938aa246394fd42c805bde3fbb67519f2de88b70d4f1b4`.
- `20261013-audit-io-before-r2`: fixture FAIL requiring the wrong kernel wait-channel name; report SHA `13c746b81c3cc33d9e08a3e344c285dfcfdfa7be53619f1db8e11e619ee77019`.
- `20261013-audit-io-before-r3`: actual PASS counterexample. A filled 65,536-byte blocking pipe leaves the exact old audit primitive in `anon_pipe_write` for at least 0.3 seconds; it returns only after a reader drains space. Report SHA `347b94c618cd61863fbf4d846946befdada8dabe18a1b11b8dc44802bdf2228f`.
- `20261013-audit-io-after-r1`: three successful builds, then a TTY fixture FAIL that assumed refusal; report SHA `00d6ba84aadd888115734bf022d27f135db84892f3e79f94fc303960516a3644`.
- `20261013-audit-io-after-r2`: 72 actual cases PASS, reusing those unchanged successful ELF builds. Report SHA `7e38b654d756775f8a6599b45ea8736cb4f8e3fe13f4c4e849ffd960fba58313`; independent read-only verification has 4,798 checks, SHA `402f789db9f72edbf81e4acf62dde3b88db8bb27096af0fdb67ed4433aa2b925`.
- `20261013-audit-io-after-r3`: permanent-driver migration FAIL because its old TTY output assertion still rejected a complete record; three builds passed, 14 cases completed. Report SHA `f72d2ace9847d77dac634eea9db79978697027324c6544e41262548d48ba52dd`. Production/helper/unit bytes were unchanged.

- `20261013-audit-io-after-r4`: this corrected permanent driver was freshly compiled against the frozen contracts candidate; all three profiles/72 cases PASS. Report SHA `f6a9001ab2755753de17366349385e0a6168fb84751c11ece034b6e177f65600`. Its artifacts also retain the exact observed sink bytes for readable fixtures.

These are standalone sink tests. They make no ROS network or full Worker runtime claim.
