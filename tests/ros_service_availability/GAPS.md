# Required follow-up / unverified scope

All code in this cohort is source-only. No current ABI, model, native SDK,
Runtime/Wasm, network, distribution or vendor execution is claimed.

* ROS1 `ROS_HOSTNAME`, arbitrary DNS names, DNS rebinding and nonnumeric Master
  or advertised service hosts are unsupported95. A real resolver must itself
  own an absolute deadline and preserve typed endpoint/rebinding validation.
  Do not add blocking `getaddrinfo` to this query and call it bounded.
* Each actual selected ROS1/ROS2 SDK/toolchain/RMW needs genuine compile/link
  and actual network/cleanup tests. Missing real ROS2 client-query/actual-QoS
  APIs refuse95; no distro name, null pointer, source constant or header presence
  can fabricate a passing API/native identity proof. There is no claim of
  complete26-distribution/19-vendor readiness support.
* Native rcl/rmw creation/query/finalizers are synchronous trusted library
  calls. Deadline checks include their elapsed time but cannot preempt a
  blocked library call; a real independently owned hard-deadline process and
  Gateway session teardown are required. Do not claim soft polling preempts
  arbitrary RMW internals or DNS. No detached stuck-query thread is introduced.
* C++/Python/Java/C#/MoonBit/QuickJS/etc. facade adapters and genuinely unchanged
  upstream client wait examples must consume the actually integrated optional
  catalog/hooks in separate reviewed cohorts. Until then their explicit95
  behavior stays truthful. No language-count total is inferred from old six.
* Native helper test alone is not Host authority or actual Guest proof. Actual
  default-deny/two-live-handle/memory/quota/close/revoke/management-race tests
  must run against a newly frozen current source and fresh actual products.
  Old Boot/source Runtime artifacts are historical or intentionally retired.
* ROS1 typed TCPROS and ROS2 RMW readiness are snapshots, not a business-call
  success guarantee or cryptographic remote service attestation. Service can
  disappear immediately afterwards; callers must handle normal call failure.
* No hidden event/queue request, Graph permission, update/debugger entry or
  implicit service request has been added. More service types require their
  real existing builtin/custom native schema/codec qualification, never a
  name-only fallback, invented RIHS/MD5 or fake generated native record.

* Real handle-scoped reuse of an observed native client for subsequent business
  requests is deferred. Current service.call keeps its distinct ephemeral client;
  any future optimization needs real ownership/QoS/identity/cancellation proof
  and actual profiling rather than a source-only performance claim.
