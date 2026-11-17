# Message and event-stream APIs

**Deprecated VM ABI:** all ROSv1 imports on this page belong to the deprecated
legacy family. Existing behavior is retained. New implementations should use
the [ROSv2 design and migration guidance](ros-host-interfaces.md#names-and-compatibility)
and check each provider's implementation status before migrating.

Both `uwvm_ros1_v1` and `uwvm_ros2_v1` expose the following distinct operations.

- `message_poll(source, memory, output, capacity, length_out)` consumes one
  operation-specific message from a subscription, service server, incoming
  action server or parameter subscription. The output has no event-stream
  header. A capacity error may occur after consumption.
- `event_stream_watch(observer, source, correlation)` binds an independently
  granted `event.observe` capability to a source capability. Selecting outgoing
  action data requires the owned goal correlation; other watches use zero.
- `event_stream_poll(observer, memory, output, capacity, length_out)` reads one
  complete event record: a 64-byte header followed by the operation-specific
  body. Error 90 for an undersized output reports the required length and
  retains the record. Error 11 indicates no deliverable event.
- `event_stream_ready(observer, memory, output)` writes the 32-byte readiness
  snapshot: ready count, next record size, admission slots and admission bytes.
  The snapshot does not reserve capacity.

The source and observer handles are distinct. A data watch cannot share an
already activated message consumer. Metadata-only observation can coexist with
message reception. The event record's version field describes its wire format;
the host function name describes the operation.

Gateway requests use `message.poll`, `event.stream.watch`, `event.stream.poll`,
`event.stream.peek`, `event.stream.commit` and `event.stream.ready`. The VM uses
peek and commit to retain a record until delivery succeeds. Resource permissions
continue to describe their actual operations, including `topic.subscribe`,
`service.advertise` and `event.observe`.

This is a breaking naming change across VM imports, Gateway dispatch, SDK
bindings, configuration, diagnostic selectors and debugger/IDE data. There are
no aliases for the previous names and no automatic fallback. Recompile guests
and regenerate their interface configuration with the matching SDK. Existing
Wasm binaries and precompiled language engines require rebuilding.
