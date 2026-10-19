# Typed40 boot3 r7: keep low-disk finalization available

This separately named metadata source package fixes one owner guard condition.
Root confirmed that a prior real low-disk failure also made cooperative cleanup
and immutable-input preservation refuse the same eight-GiB disk reserve. The
r6 owner has that same condition. Existing staged r6 source and actual reports
remain unchanged; this package changes no compiler, Runtime or Guest source.

During active preparation and compilation, `guard()` still requires at least
eight GiB free on every previously selected backing path. The owner already
sets `finalizing = True` before its cooperative cleanup, child-first pidfd
fallback and terminal preservation. Only that disk-reserve loop now runs inside
`if not finalizing`. Low disk can therefore reach those finalization operations.
The original current cgroup and aggregate RSS checks, Python alias checks,
whole-operation deadline, bounded 600-second cooperative grace, process-birth
checks, pidfd ownership and child-first cleanup remain identical. Existing
active-phase self-RSS and cancellation semantics also remain identical.

The four owner/stager output paths `CODE`, `META`, `ACTOR` and `OWNER`, plus the
stager's `RECEIPT`, use fresh final revision suffixes `r7`. No old output is
replaced or treated as a fresh successful build. All eight non-owner decoded
payload entries are byte-for-byte identical to r6, including the Source40
compiler recipe, complete dependency checks, restart/union/exact-alias helpers,
original complete manifest and default-empty Guest rights. The original
Source40 manifest and all resource budgets remain unchanged.

Root must independently review and SHA-admit the stage and complete nine-role
package. Run the fresh r7 stage, inspect its actual bound prepare/build commands,
and require actual successful preparation before any future r7 build. A genuine
completed r6 compiler result can retain its original source/product provenance;
this owner-only fix creates no obligation to relabel or repeat unchanged
compiler inputs. Every actual accepted build still requires all fourteen
products, nine retained/five fresh, full binary validation, complete source,
input and product preservation, and owned cleanup. Neither a source review nor
metadata preparation qualifies Runtime, ROS, DDS, physics or performance cases.

The author performed source generation, AST comparisons, complete decoded
payload comparisons and hashing only. No candidate was imported, no test or
compiler ran, and no SSH, Runtime, ROS, DDS or simulation process was started.
`source-ready-before-peer.json` records exact source and payload hashes; Root's
independent source peer must be bound before an actual guarded invocation.
