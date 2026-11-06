# Host observation qualification

The journal.cc test exercises the production recorder and strict console
grammar, 12,000 concurrent invocations, 100,000-byte payload and label pagination,
nested scopes, clear during an in-flight call, and disabled collection.
The allocation_failure.cc test injects actual C++ allocation failures and checks
that optional observation never escapes or rewrites the operation result.
The wasip1_io.cc test invokes real Preview1 wrappers and real file/socket
operations for both wasm32 and memory64 ABIs, checking copied data, paths,
errno results, and observer-only overflowing ranges. It reuses the existing IO
fixture; the renumber destination is an actual open descriptor, as required by
Preview1.

Build these components with the retained native compiler and run with ASan and
UBSan. Components are not substitutes for actual VM/broker/DAP tests: runtime
qualification also uses SDK WAT examples through the VM, explicit base WASI/ROS
exposure and individual grants, stale-stop rejection, over 64 KiB payload
pagination, collection on/off, and all five Linux target ABIs.
Keep every generated source, binary, transcript and profiler result under the
repository .tmp and inside the configured 64 GiB/zero-swap cgroup.
