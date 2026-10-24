# Same-main typed Unitree DDS acceptance

`run_typed_transport.py` is the maintained executable driver for the fourteen
same-main command products described by `typed_command/recipes.json`. The
source contains the entire protocol and admission path; no private `/tmp`
driver is required. Source availability, compilation, a successful full Wasm
validator, and the pure protocol checks are **not** recorded DDS results.

The driver runs 28 serial cases: an independent default-denied process and an
independent positive process for each product. Rust, embedded JavaScript,
AssemblyScript and TinyGo each exercise GO LowState, HG LowState and API topic
RPC. C and C++ exercise API topic RPC. These are the fourteen products that the
actual command producer builds; this driver does not invent missing C/C++
LowState command products or label another language's binary as those products.

## Required execution boundary

Root must invoke the entrypoint from its complete, published frozen source and
an already selected Linux namespace containing only an enabled `lo` interface.
The coordinator and all children must remain in the same exact current cgroup:
`memory.max=68719476736`, `memory.swap.max=0`, with the explicit boot ID, leaf,
device, inode and CPU set in the ordinary Unitree Frame configuration. The
driver never starts a container, creates or enlarges a cgroup, moves a process,
downloads an SDK, mounts a filesystem, initializes a simulator or accesses a
physical robot. Frame's live reserve, ancestor OOM, output/disk, whole-source,
whole-input-root, owned RSS, deadline and pidfd checks apply unchanged.

All cases are serial because the witness intentionally uses one closed set of
topics and domain1. Native peer, Worker and Gateway cooperate inside a single
case. User permission for more compiler jobs does not imply that independent
tests may share these endpoints. A case has at most three cooperating actors;
Frame's aggregate owned RSS limit remains at most8GiB under the64GiB hard cap.

The owned output must be fresh and disjoint from all complete input roots.
`out/s00` through `out/s27` are fresh per-case UNIX socket names. The encoded
path `out/s27` must be shorter than104 bytes; ordinary product log filenames
retain their complete descriptive names. There is no external socket alias or
unowned cleanup directory.

## Inputs and source provenance

Use the ordinary `common.py::Frame` configuration, with `simulation`,
`benchmark` and `profiler` set to JSON null. Add a pinned input for a selection
document following `typed_transport.selection.template.json`. Pass its input
label using `--selection-input`. The template contains input labels and empty
unfilled namespace/root selections; it deliberately refuses actual execution
until Root supplies those observations. It contains no guessed future pins.

Every label names one ordinary Frame input `{path, bytes, sha256}`; Frame
independently opens and records its current physical seven-field pin. The
selection has these fields:

| Field | Required actual input |
| --- | --- |
| `reports.commands` | Completed fourteen-command build report with nine individually retained products, five fresh products, full validation and successful owned cleanup |
| `reports.workers` | Actual four-profile interpreter build report with the original three full-MD compilation records per selected profile |
| `reports.native_products` | Completed Native74 producer report, including the actual typed peer, held exec gate, custom Gateway profiles and complete physical execution catalogs |
| `reports.native_interfaces` | Actual independently compiled/exported native GO24, HG11 and API8 definition/identity producer report consumed by Native74 |
| `workers` | Exact compiled `ros2` and `dual` interpreter binary input labels |
| `products` | Exactly fourteen command IDs, each with exact binary and original build-receipt input labels |
| `native_peer`, `exec_gate` | Exact separately compiled native generated-message witness and test-only held `execveat` gate binary labels |
| `families` | Generation/native-export/Gateway/Worker-registry-generation labels and explicit required mapping-input lists for each family |
| `worker_catalog_roots` | Complete *current* inventoried host roots for every real Worker mapped file; the native SDK COPY catalog cannot replace them |
| `restart_mappings` | Finite explicit persistent filesystem device transitions, if historical full7 pins have survived a restart |
| `expected_network_namespace` | The actual Root-selected current `net:[inode]` string |
| `cyclonedds_xml` | The pinned XML input label selected by the exact `CYCLONEDDS_URI=file://...` environment |

The command Frame report uses `source_preserved`, `input_pins_preserved` and
`whole_roots_preserved`. Native and Worker producers have their own
`source_preserved`, `inputs_preserved` and `products_preserved` contract. The
driver checks each actual schema instead of substituting field names. Original
failed batches remain failed; only the final successful command continuation
can supply the completed fourteen-product inventory. Each retained product's
original individual receipt remains separate and immutable.

`typed_transport_inputs.py::Evidence` reopens actual selected files. If a
historical pin differs, only the explicit root's device transition may explain
the change. Path, inode, mtime, ctime, byte count and full-stream SHA256 must all
remain equal. The accessor returns the actual current full7 and retains a
separate bridge record; it never rewrites historical JSON or cgroup observations.
A mapping root is a physical device-transition scope containing selected
inventories. Every admitted file must independently belong to a complete Frame
inventory or this complete frozen source; a mapping alone grants no input-path
admission. More specific mappings take precedence.

The original compilation source is fully reopened against its exact original
published manifest. Worker records retain their actual original source label,
even when a newer runner source exists. Every consumed original source pin is
checked against that original manifest and its current byte relation is
recorded. A different newer source is not silently qualified by an older
interpreter. The actual native witness and held exec gate must additionally be
byte-identical to the currently reviewed source; changed witness/gate logic
requires a fresh native compilation.

GO and API select the actual `ros2` Worker and its complete GO+API registry;
HG selects the actual `dual` Worker and its HG registry. The dual HG command
keeps ROS1 globally and individually disabled. All three actual Worker TUs must
have the exact backend macros, quoted custom-manifest macro and that same
header in their full MD dependency lists. This is an interpreter observation;
it does not qualify a new LLVM provider or another architecture.

Family native identities are read from the uniquely selected actual exporter
stdout `native-metadata.json` under that family's producer-derived root. The
driver compares all24/11/8 exported identities against all generated record and
root identities. It also reopens original official-package/export full-MD
records. It accepts no identity string in the selection. The compiled Worker
manifest and Gateway native provider/registry/header dependency bytes must
match the complete selected registry generation. GO+API has exactly32 types;
HG has exactly11. No generated table grants an alias by itself.

## Rights and native connection policy

Every Runtime starts with global WASI disabled, no inherited system environment,
and a named `main` principal with basic stdin/stdout/stderr only. It explicitly
denies all optional WASI rights used by this lane's policy adapter: realtime,
monotonic, process and thread clocks, randomness, process signals, scheduling
yield and full stdio rights. ROS defaults are empty. There are no preloads,
mounts, environment grants, raw guest sockets or caller-supplied argv tails.

Each positive GO/HG process receives exactly one subscribe alias:
`unitree-lowstate`, for its actual native LowState type on
`/uwvm_unitree_typed/lowstate`. Each positive API process receives exactly two
aliases: `unitree-api-request` publishes actual Request on
`/uwvm_unitree_typed/request`, and `unitree-api-response` subscribes actual
Response on `/uwvm_unitree_typed/response`. There are no LowCmd, firmware,
service, parameter or action operations. Every alias carries the actual
exported identity, one endpoint/inflight/queue item, finite byte/rate/audit
budgets and reliable/volatile/keep-last1 QoS.

The closed native connection uses domain1, `rmw_cyclonedds_cpp`, a quiet test
node/namespace and an explicitly disabled-security test profile. This test
profile makes no SROS/security-deployment claim. The pinned Cyclone XML must
select loopback, forbid entity/doctype expansion and contain only the reviewed
profile elements. The Linux namespace itself must expose only loopback.
`common.py::current_loopback` obtains the actual current namespace's unique
`lo` interface using `if_nameindex` and a read-only `SIOCGIFFLAGS` query, requiring
both `IFF_UP` and `IFF_LOOPBACK` and the exact namespace before/after. An inherited
sysfs mount is not used as an interface inventory. The short-lived trusted
coordinator descriptor sends no packets, changes no setting and grants no
Guest network right; its actual observation is retained in admission evidence.

## Actual lifecycle and independent witness

1. Start the real native witness, send `UTN1` family/mode framing and wait for
   its actual generated-message context readiness. Prove its actual owned
   executable, namespace and all inode-backed maps through opened descriptors.
2. Start the test-only pinned exec gate. Reopen and fully hash its actual held
   Runtime descriptor and retain the same PID/birth. For positive cases start
   a fresh real Gateway whose trusted manifest binds that exact Worker PID/UID.
   Default-denied cases start no Gateway.
3. Release the held exec. Wait for the actual module's `READY_TYPED_MAIN`, prove
   the live interpreter executable and *complete host* file/ELF mapping closure,
   then send `UTM1`. The entry is the observed void export's local defined
   function index. The binary must have no implicit start section, foreign
   bridge, imported memory/global or optional WASI imports; all typed scalar
   exports remain present. The original full validator job is separately bound.
   Exactly the fourteen admitted Guest artifacts also enter the Worker file
   catalog because `native_file_loader` may retain an inode-backed Wasm mapping;
   this is explicit artifact admission, not an unknown host-file fallback.
4. Default-denied commands verify actual permission error13, no endpoints,
   no delivered payload and invalid-handle9 for subsequent operations. The
   native peer independently matches only its own baseline endpoints, observes
   at least30 actual graph samples for at least2 seconds with zero payloads,
   then both real processes complete normally.
5. Positive state commands require actual native discovery, publish one real
   complete generated message and receive it through real DDS self-observation.
   The driver compares all60 GO or105 HG carrier binary32 words, tick42 and
   CRC field0x13572468 against both native and Guest observations and an
   independent exact-fraction oracle. The native witness independently compares
   every generated nested/default message field, not just the printed carriers.
6. Positive API commands submit exactly once. The native witness receives and
   compares the complete generated Request, including identity1/api_id1,
   parameter`{}` and empty binary, then publishes and independently observes
   the complete generated Response with status0 and data`typed-example-ack`.
   The same actual Guest returns its response/callback result. A second submit
   is required to refuse rather than publish again. Request publisher discovery
   is intentionally checked after submission because native creation is lazy.
7. Close the actual Guest handles. The native graph must return to its baseline
   and remain quiet for another real2-second/30-sample window with unchanged
   payload counters. Subsequent close/poll/submit calls must return9. Both
   command and native witness then finish with exact complete transcripts and
   exit0. The Gateway must retire normally with the real Worker principal;
   its actual SO_PEERCRED bind audit must identify that one PID/UID/session.
8. Require terminal owned pidfds and successful cleanup before the next case.
   Final PASS additionally requires all28 distinct cases and Frame's complete
   source, input, whole-root and live cgroup preservation checks.

There is no fabricated host callback, encoded substitute DDS message, Node.js
ROS mock, std_msgs/String relay, raw Gateway RPC client or transport PASS derived
from a build receipt. A partial case inventory remains a failed/incomplete run.

## Invocation and pure checks

After Root publishes a complete source freeze and fills all actual inputs:

```text
python3 -B FROZEN_SOURCE/test/0073.ros-tests/support/source/ros_unitree/run_typed_transport.py
  --source FROZEN_SOURCE --manifest ACTUAL_MANIFEST
  --manifest-sha256 ACTUAL_MANIFEST_SHA256
  --config ACTUAL_FRAME_CONFIG --config-sha256 ACTUAL_CONFIG_SHA256
  --selection-input typed-transport-selection --out FRESH_OWNED_OUTPUT
```

The words in capitals denote actual caller-selected paths/digests, not values
to paste literally. The shell display spans lines for readability; the actual
owner must launch a literal argv array under its existing current cgroup guard.

`test_typed_transport_protocol.py` contains25 pure parser/oracle/quiet-shape
checks, including decimal overflow, noncanonical words, all carrier counts,
64-bit elapsed nanoseconds and refusal of denied payloads/missing API response.
Its CLI requires the explicitly hashed session helper, the exact current
seven-field cgroup selection in an ordinary JSON input, and a fresh report path
under `/tmp`. It starts no children and performs no DDS or Runtime execution.
Its success status explicitly says `TRANSPORT_NOT_EXECUTED`; it cannot satisfy
any of the28 transport cases. Neither these checks nor transport acceptance
have been executed merely by creating these source files.
