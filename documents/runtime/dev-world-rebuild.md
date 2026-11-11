# Module reconstruction and update authority

The debugger commands `rebuild`, `rebuild <path>` and
`rebuild-module <index> <path>` are removed. The VM and DAP adapter explicitly
reject them. `help rebuild` directs the host to the independent signed update
coordinator. Full debugger permission does not authorize reading and publishing
an arbitrary replacement module through a second deployment path.

Use the [production update protocol](production-update.md) for an approved
module or configuration transition. It requires the independently selected
update endpoint, fixed Ed25519 issuer, effective permission contract, deployment
scope, current process nonce and exact transaction measurements. The current
CLI uses one runtime coordinator at a time; a debugger connection is not a
substitute for those grants and does not automatically start an update manager.

A terminal runtime fault closes mutation and update admission for that execution.
It does not authorize preserving or migrating contaminated state, restarting the
faulting continuation, or clearing the first-fault record. Status and authorized
diagnostics remain available where the selected transport can serve them. A
supervisor that needs recovery must construct a fresh independently authorized
instance from an approved artifact or a separately qualified trusted checkpoint.
The old debugger image-rebuild command is not a recovery mechanism.

Ordinary authorized updates at normal runtime safe points remain supported.
The independent update protocol decides whether the requested state transition,
policy change and retained resources are admissible before publication.
