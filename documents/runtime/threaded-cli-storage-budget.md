# Threaded CLI storage accounting

`--wasi-threads` specifies per-child storage and a maximum child count. Its
aggregate storage allowance includes the initial-main slot, every declared child
reservation, the actual Group/slot/compiler-queue objects, runtime profile and
admission objects, concrete compiler submissions, and the bounded initial-main
ABI input/output storage. Compiler submissions for spawned children carry no
parameter buffer. The real parameter/result capacities are checked against the
existing value-stack bound.

The CLI computes this finite sum with overflow rejection. At actual cold startup,
after option ordering is resolved, explicitly selected debugger control storage
and the selected signal-pipe allowance plus its bookkeeping are added under the
real registry and maintenance locks. The genuine Group still charges each object,
submission, pipe and child reservation and rejects any excess. Initial-main and
compiler native stacks retain their separate existing accounting.

Native embedders that call `runtime_threaded_profile_configure_host_api` retain
their exact explicit aggregate cap. Only the CLI-specific configuration entry
uses this derived aggregate; it grants no new host operation, source identity,
thread capability or Guest cancellation authority.
