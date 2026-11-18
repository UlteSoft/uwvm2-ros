# Registered message payload capture

The host can capture a successful stage of a registered cross-Wasm message route for local diagnostics. Capture starts disabled. Open a fresh regular file under your own private directory with write-only access and mode 0600. Pass the file descriptor to the VM. The VM consumes the supplied descriptor on success and keeps a close-on-exec duplicate. The file must have one link, zero length and the VM's effective UID. Standard streams, pipes, sockets, append mode and read/write access are rejected.

Configure the journal once. The three limits count accepted frames, total file bytes and bytes in an individual message body, respectively.

```text
--ros-scene-call-buffer-journal FD MAX_RECORDS MAX_FILE_BYTES MAX_PAYLOAD_BYTES
--ros-scene-call-buffer-capture ENVIRONMENT ROUTE submit
--ros-scene-call-buffer-capture ENVIRONMENT ROUTE take
--ros-scene-call-buffer-capture ENVIRONMENT ROUTE respond
--ros-scene-call-buffer-capture ENVIRONMENT ROUTE result
```

Create the environment and route before selecting its stages. Each selector enables exactly one stage of one route. Bounds are 1–1,048,576 records, 94–67,108,864 file bytes and 1–65,536 bytes per payload. Each file reserves eight bytes for its header and 56 bytes for its footer. A full queue, an oversized body or an exhausted record or byte limit drops the entire observation. The host continues the application operation. Enabling capture grants no Wasm authority.

| Stage | Bytes that can be recorded |
| --- | --- |
| `submit` | The host-owned request after route admission and successful return of its receipt |
| `take` | The queued request after successful delivery to the registered server |
| `respond` | The retained response after the delivered request was cleared and the server received its receipt |
| `result` | The retained response after successful delivery to the registered client |

A failed response receipt can leave an existing result available to the client. The failed `respond` generates no event. A later successful `result` can create an event when selected. Acknowledgment, cancellation, probes, denials and partially written deliveries generate no events.

The dispatch path copies the authenticated active extent into one of four preallocated slots. A VM-owned writer thread with a bounded stack performs the writes outside the router lock. For each frame it pins the open file description and checks its device, inode, link count, mode, owner, access mode and expected length. An output failure disables further capture. Consumed and discarded queue entries are overwritten. Dispatch can wait for queue bookkeeping while the writer holds the mutex. This diagnostic mode has no hard-real-time latency guarantee.

The file contains exact plaintext message bodies for the selected stages. It outlives ordinary guest and broker erasure. Protect the containing directory and restrict access and retention at the deployment boundary. Another process with the same UID or administrative privileges can access or alter an ordinary file. External provenance, signatures, transport encryption and physical device acceptance require separate evidence. An abrupt process kill or power loss can leave an incomplete file. A clean close drains pending frames and emits the footer when the file is healthy.

## Binary format

All integers use little-endian order. The file begins with `57 52 4f 53 43 41 50 01`. At the next offset, read an unsigned 32-bit frame size including the size field. Data frames carry a one-byte stage at offset 4, a one-byte environment length at 5, a one-byte route length at 6 and a reserved zero byte at 7. The one-based record sequence is a 64-bit integer at 8, the caller generation is a 64-bit integer at 16 and the body length is a 32-bit integer at 24. The environment, route and body follow at offset 28 without terminators or padding. Stage values 1–4 select submit, take, respond and result, respectively. No caller pointer, capability handle, slot token, guest address or unused buffer capacity enters the file.

The final 56-byte frame uses stage zero. Six unsigned 64-bit integers at offsets 8 through 48 give enqueued, written, over-size, over-budget, queue-full and I/O-error counts. A missing footer indicates incomplete evidence. Retain the applied scope and limits separately alongside the deployment configuration.
