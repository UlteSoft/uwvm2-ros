# ROSv2 debugger and host audit

A ROSv2 guest may have zero execution grants and still be inspected by an authorized host debugger. Inspecting an environment, selecting a trace, and issuing a device command are three independent actions. The runtime attributes an observation to the executing Wasm module. A label supplied inside a message has no role in that decision.

| Question an operator can ask in ROSv1 | ROSv2 evidence and boundary |
| --- | --- |
| Which capabilities, quotas and handles exist for this instance? | At a cooperative stop, `info scene authority` and `info scene resources` return detached grant and resource rows. Use the exact environment and alias filters. |
| What state is known about sessions, queues, services, actions, parameters, transforms and graph entities? | The resource inspector copies the cached metadata implemented by each provider. Different providers expose different counters. A communication alias row alone cannot enumerate a remote DDS graph or prove that an external queue is empty. |
| Which registered host function returned a particular value? | In a development build, enable the ROSv2 host trace. Each completed call retains the actual import namespace, operation and raw result. Qualified caller names come from runtime provenance. Consult the debugger help for exact filters. |
| Which calls did the host refuse? | Configure `--ros-scene-call-audit` at process, environment or module scope. All matching budgets intersect. Code 13 reports a permission refusal, 9 an invalid handle and 116 a stale identity or generation. Other outcomes can be selected with `all`. |
| Did an operation return, or did its external effect finish? | The result trace records only an observed VM return. A remote effect can remain unknown after that return. Look at the provider's own receipt and reconciliation state before deciding whether to retry. |
| Can a forensic capture reveal the request body? | The default call audit and normal ROSv2 debugger rows contain no body. A legacy ROSv1 private-file capture or portable-provider proof must be matched against a separately authorized ROSv2 capture of the same semantic boundary. |
| Can a saved debugger snapshot restore physical state? | Wasm-local state, cached observations and the external participant's state have different owners. Restoring captured observations cannot replay a native side effect, transfer a control lease or establish a physical safe state. |

The [scene debugger contract](../../tools/debug/README.scene.md) describes cooperative stops, pagination, redaction and provider-specific fields. The [scene CLI](../command-line/ros-scene.md) specifies host result records and budgets. The [legacy debugger contract](../../tools/debug/README.ros.md) lists the ROSv1 selectors and filters used when checking equivalence.

The following comparisons require their own evidence before declaring full ROSv1 diagnostic parity:

- Compare aliases, source correlation, pause generation, filters, overwritten records and tail loss under contention for each applicable provider.
- Compare private capture at the authorized transfer boundary, including extent limits, sink replacement, refusal paths, explicit approval and cleanup.
- Compare checkpoint/dump inspection after traps and module retirement using both VM engines. A debugger must expose unknown external state without resubmitting a request.
- Run the same event through the VM console, DAP, export and editor views with full, read-only, limited and disabled inspection permissions. An IDE presentation must never grant additional access.
