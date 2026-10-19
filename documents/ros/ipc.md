# IPC protocol v1

All integers are little endian; no native pointer, STL object or C++ ABI is transmitted. Every frame has a 48-byte header. Payloads are bounded before allocation. The hard payload ceiling is 4194304 bytes; the request envelope reserves up to 518 bytes, so configured message/request/response payload limits cannot exceed 4193786. Every configured timeout is 1 through 60000 ms, measured with a trusted steady clock.

| Offset | Width | Field |
| --- | --- | --- |
| 0 | 4 | Magic `0x32524f55` (`UOR2`) |
| 4 | 2 | ABI version, 1 |
| 6 | 2 | Kind: hello=1, request=2, response=3, cancel=4, close=5 |
| 8 | 1 | Backend: ROS1=1, ROS2=2 |
| 9 | 3 | Reserved, all zero |
| 12 | 8 | Request ID |
| 20 | 8 | Response-to ID |
| 28 | 8 | Session ID |
| 36 | 4 | Payload length |
| 40 | 4 | Error code |
| 44 | 4 | Timeout in milliseconds |

The hello has request ID 1, zero session/response/error and no payload. The Gateway verifies exact worker PID and UID with SO_PEERCRED and replies with a random nonzero session. The worker authenticates the Gateway UID independently. Every response, including the hello acknowledgement, has request ID zero and an exact response-to ID. Requests then have strictly increasing IDs and the bound session. Unexpected versions, backends, reserved bytes, kinds, length, replayed IDs or session mismatches close the connection.

A request envelope contains u16 operation length, u16 capability-alias length, u32 data length, then exactly those UTF-8/ASCII operation and alias bytes followed by data. Operation and alias tokens are bounded to 255 bytes and their restricted syntax is checked. Trailing, missing and oversized data is rejected.

The first operation is `session.bind`, alias `target`, carrying `binding_manifest(target)`. It serializes a versioned prefix, ROS version/enabled state and sorted network identity fields plus every alias, operation, resource, type, trusted identity and explicit limit as length-prefixed strings. The Gateway's loaded configuration snapshot is authoritative and its manifest must match byte-for-byte. Its acknowledgement has an empty payload; unexpected response data closes the connection and returns permission error13. Worker-local socket location, expected Gateway UID, connection timeout and deny-audit settings are excluded; they are authenticated/used locally. Scope/name labels are also excluded and cannot confer authority. No resource operation is accepted until this binding succeeds.

`tests/ros/run_unit.py --suite ipc` includes three real Unix-client handshake
cases: a nonzero request ID in the hello response, an unexpected binding body,
and a valid empty acknowledgement. The first two must clear the session; a
subsequent operation on the rejected binding must return disconnected107.
These cases require Linux and `-UNDEBUG`; use a short absolute `--out` path
under the test owner's temporary directory for the Unix socket path limit.
Their successful execution proves this IPC contract, without claiming native
ROS communication or an official upstream test result.

Each Wasm Instance and ROS backend owns an independent connection and session. A client call captures its steady-clock deadline on entry, before waiting for its connection's serialization mutex. Mutex waiting and request encoding consume that budget; the frame carries only the remaining whole milliseconds. An expired request that has not started writing returns timeout110 with an empty output and keeps the authenticated session available for a later call. Mutex acquisition cannot be interrupted and this is not a hard return-time guarantee.

Backend loss or a transport timeout after writing starts closes the connection, cancels the outstanding session and releases its resources. A service already admitted to an external ROS node may have an externally irreversible effect; cancellation of the transport cannot roll back that effect. Relative frame timeouts and socket/middleware scheduling do not establish a shared absolute network deadline. That boundary is reported as timeout/cancellation, never as successful rollback. A new connection must perform a new authenticated bind; local revoked aliases remain revoked.

Private Unix socket directories are owned by the Gateway and mode 0700; sockets are mode 0600. The client rejects world-accessible or wrong-owner sockets. Gateway configuration files are owned regular files and cannot be group/world writable. There is no abstract-namespace or public socket fallback.
