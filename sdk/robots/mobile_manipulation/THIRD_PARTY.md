# Original source notices

Every file under `*/upstream/` is an unmodified selected original whose Git blob
is independently checked against the fixed official complete Git tree. The
acquisition ledger records exact repository, original path, commit, tree,
blob SHA-1, byte count and SHA-256. It does not claim that a partial checkout
hash is a whole-source-tree SHA-256. Connector JSON is a representation of the
Git API result, not a byte-for-byte raw HTTP response.

* Boston Dynamics Spot SDK: `spot/upstream/spot_sdk/LICENSE`; retain the Boston
  Dynamics SDK Software License and per-file notices. No permissions to robot
  operation or redistribution beyond that license are inferred.
* gRPC: spot/upstream/grpc_build/LICENSE and the actual grpc_types.h notice.
  Retry configuration uses the real pinned1.46.3 header; no installed library
  identity is inferred.
* Google Protocol Buffers: `spot/upstream/protobuf_build/LICENSE`, original
  well-known `.proto` notices. Version is the actual Git submodule of the
  explicitly pinned grpcio-tools source build dependency; no installed binary
  identity is inferred from it.
* Kinova ROS2 Kortex: `kinova/upstream/kinova/LICENSE`, original BSD notices and
  per-file copyright. The native Kortex binary API has a separate vendor license
  and was not copied/downloaded as a binary by this cohort.
* Hello Robot Stretch: selected core and helper package LICENSE.md files and
  `stretch/upstream/stretch_body/body/LICENSE.md`; retain per-file notices.
  The entire upstream monorepository is not asserted to have one license.
* ROS control_msgs, common_interfaces, rcl_interfaces, unique_identifier_msgs
  rclpy, rclcpp, rosidl and ament: each `interfaces/upstream/<source-key>/LICENSE` and original
  per-file notices. Interfaces/comments/constants/defaults are unchanged.

New wrapper/example/model text outside `upstream` is an adaptation authored for
this project. It does not replace any vendor license or import vendor native
binary APIs into Wasm. Source provenance and future runtime authorization are
separate obligations.

* WASI libc: interfaces/upstream/wasi_libc33/LICENSE and the actual Preview1
  api.h/wasip1.h/version.h.in Sources from the fixed SDK33 libc submodule.
  This proves the used fd_read/iovec signature, not installed sysroot/compiler
  presence or Runtime input rights. Original upstream templates are unchanged.
