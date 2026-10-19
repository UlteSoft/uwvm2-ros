# Genuine threaded + ROS module contracts

These three `.cpp` consumers use named imports only for project types. They are
uncompiled and unexecuted. The explicit `named_module_threaded_contract` xmake
target is non-default and requires Linux, a real backend, module mode and an
explicit ROS import. It consumes the actual public Runtime BMIs/initializer
objects; it does not build an alternate Runtime, fake Host or adapter.

`api_owner.cpp` retains 19 actual API definitions through volatile function
pointers and checks the genuine initializer limit type/private issuers.
`state_owner.cpp` observes the actual params and ROS policy objects from a second
TU. `contract.cpp` checks identical addresses, default-disabled policy, absent
sender, original typed parser derivation, UINT64_MAX and failed-output sentinels.
Its extra volatile references require the real controller/run/callback/ROS
factory graph. NDEBUG is rejected in source and undef'd by the actual target.

The contract creates no Source/Guest/ROS endpoint or signal handler. Passing it
in the future would establish only these source/link/value properties. Physical
thread lifecycle and native ROS still need the final original test workloads.
See the [source design](../../../../documents/runtime/threaded-named-modules.md)
and the recipe. The Runtime's ordinary cache and DBG policy are not bypassed.
