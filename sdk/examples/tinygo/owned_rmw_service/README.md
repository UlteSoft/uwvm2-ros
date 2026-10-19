# Complete source-only owned-RMW Service example

This main uses real generated `customtypes` records and the optional TinyGo
`rmwservice` package. It contains no substitute Native struct, fake hash, extern
stub, old-business Service call or unowned capability opener. All source
emission, builds, optional import admission and Guest/Native exchanges remain
unexecuted. Follow [the full ownership and wire contract](../../../tinygo/rmwservice/README.md)
through the SDK root (the relative path from this example is
`../../../tinygo/rmwservice/README.md`).

Run the exact records-only operator from an independently reviewed staged source
pair, inspect its canonical generation proof, and put only the actual emitted
`bounded_records.go` in `customtypes/`. The directory/file are deliberately absent
in this source package until genuine source generation. `go.mod` resolves the
actual SDK locally; no download or module replacement is performed by this task.

The two actual QoS expectations are explicit12-field CSV argv inputs described
in the package guide. They must come from independent actual Native observation;
there is no assumption that nominal middleware defaults equal a chosen numeric
infinity or liveliness. Operator aliases/Native IDs/policy are separate from these
values. The two modes and full argv shapes are in the package recipe.

The GTD client preserves `Include_type_sources=true`, accepts a genuine external
71-byte RIHS string, submits once, and checks complete local response retention,
including raw source bytes/final NUL. Its bounded64 observations are no elapsed
wait or retry guarantee. The server returns one actual PARAMETER_NOT_SET type per
requested name; this deliberate business state preserves the original request
ordering. It responds once only to an actual committed ticket. Both modes keep
primary errors separate from fixed `lastCleanup` records and use Group cleanup.
The top-level panic makes a failure observable to the finite outer harness;
actual TinyGo WASI diagnostic/argv imports still need separate admission.
