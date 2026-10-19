# ROS2 legacy identity and container compatibility regressions

These fixtures cover the compatibility implementation's identity boundary.
They must run after implementation freeze in the user's verified Linux
64 GiB, zero-swap cgroup, serially with one compiler job. Local artifacts belong
under `/tmp`. Source availability and preparation do not establish execution.

`schema_models.py` compares the generator's complete Service/Action protocol
closure, including both Action protocol services, service events, standard
dependencies and empty-message placeholders, to immutable official native
exports retained in `tests/ros_custom_types/deployment-final/derivation`.
Its independent JSON/SHA256 oracle checks the exact official native hashes.
The historical exports are declaration fixtures; this does not qualify a current
Gateway, distribution, architecture, SDK binary or ROS behavior.

`rihs_contract.cc` calls the production native RIHS01 implementation with Point
and complete Empty service golden identities, then checks missing, disconnected,
recursive, duplicate, reordered and malformed inputs. Compile with C++17 or the
selected newer target standard, `-UNDEBUG`, the source include directory and
`-DUWVM_ROS2_HAVE_OPENSSL=1`, and link the actual target OpenSSL Crypto library.
A missing target Crypto dependency blocks this regression; a stub/no-Crypto
success is forbidden. Cross execution needs the actual matching target ELF,
loader, sysroot and QEMU, all inside the same attested cgroup.

The native Gateway uses its selected SDK's baked RIHS01 and complete message
TypeDescription on modern ROS2. On an SDK whose typesupport struct genuinely
predates native hashes, it must first verify every actual payload component's
complete introspection metadata and then recompute RIHS01 from the trusted
complete protocol trait. An absent old provider trait, missing Crypto, partial
schema or malformed modern native hash fails admission. No endpoint or authority
is created by these checks.

Lyrical's official unbounded `uint8[]` container is supported by an exact
`rosidl::Buffer<T, Allocator>` generated trait. Native validation accepts CPU
storage before calling size/data/index operations. It refuses implicit GPU to
CPU conversions and never copies an entire unbounded native buffer to discover
whether it satisfies a bounded canonical schema.
