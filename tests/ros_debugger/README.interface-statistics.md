ROS Host interface statistics

`info ros calls` and DAP `uwvm/rosState` with `selection: "calls"` report
process-owned, read-only aggregates at the same cooperative-stop fence as the
other ROS diagnostics. `backend`, `instance`, `module`, `alias`, `function`, and
`op`/DAP `operation` are exact filters; pages use `first`/DAP `start` and count
1..8. Reads never reset counters, consume queue items, invoke ROS, or change
capabilities. A Host audit scope that unwinds or leaves without a normal return has an unknown result; it increments incomplete and is excluded from success/error filters. Trace output carries result_known; DAP exposes its errno as null when unknown. No returned errno is invented. The fixed 128-interface table distinguishes backend, instance,
generation and all exact labels. Replacement generations do not merge.

Each row contains recorded call exits, known errors, incomplete results, timed calls, total/min/max
nanoseconds, and an elapsed monotonic observation window. DAP derives calls per
second and mean nanoseconds from these integer values. Zero windows, unavailable
timing, and saturated totals produce unavailable derived values rather than a
made-up rate/mean. Timing covers the audited Host gateway region, including any
waits inside that region; it does not measure native ROS executor/callback CPU
time or pre-audit argument validation. Existing unresolved denials, ABI queries
and management events remain explicitly named; absence of precise identity or
timing is not repaired by guessing a function or duration. Table-full, truncated
label, and busy-drop counters expose sampling losses. Counters are observation
counts, so they do not claim complete coverage after a drop. Trace ring overwrite
does not reset the independent cumulative table.

`interface_statistics.cc` and `test_dap_ros_interface_statistics.py` are detached
DATA contracts. Their passing status alone is not live ROS1/ROS2 acceptance,
source-level stepping, hot replacement, or cross-architecture qualification.
