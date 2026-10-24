# Typed40 boot3 r6: fresh output paths after the r5 reserve failure

This separately named source package retries the same reviewed r5 continuation.
Root reported that the actual r5 build owner refused to proceed when its backing
filesystem fell below the unchanged eight-GiB reserve. That failed report remains
unchanged. This package does not turn r5 preparation or failure into a completed
compiler result, and it does not reuse an r5 output as a newly successful build.

The only production source changes are the four top-level output path constants
`CODE`, `META`, `ACTOR` and `OWNER` in `root_owner.py` and `stage.py`, plus the
stager's separate `RECEIPT` path. Their final leaf suffix changes from `r5` to
`r6`. The common dated storage parents retain their original spelling. All
function and class ASTs are identical to r5, including compiler admission,
complete inventories, exact two-file alias selection, terminal preservation,
resource guards and cooperative cleanup. Every one of the eight non-owner
payload roles is byte-for-byte identical to r5. The nine-role package contains
the new owner bytes and the unchanged eight other source files.

The package keeps the exact current boot3 cgroup,64-GiB memory limit, zero swap,
CPU selection, original Source40 manifest,60 historical complete roots, complete
current ARM auxiliary root with10 names, and two explicitly pinned current ELF
aliases. It grants no Guest authority and selects no debug parent or sibling.
No disk reserve, class nlink equality, root bound, timeout or RSS bound is relaxed.

Root must independently SHA-admit the external stage source and payload, then
run the fresh r6 stage. Its output contains literal unexecuted prepare/build
commands. Run `prepare` under the owned current64 supervisor first. Only an
actual successful r6 preparation may supply the r6 build command. The r5 controls
and reports remain separate evidence. Actual build acceptance still requires all
14 products, exactly nine retained/five fresh, complete source/input/product
preservation and complete owned cleanup. Metadata stage or preparation success
qualifies no Runtime, ROS, DDS, physics or performance case.

The accompanying `source-ready.json` records exact local source hashes and AST
comparisons. An independent source peer receipt will be referenced once reviewed.
This author performed only source generation, `ast.parse`, hashing and decoded
payload comparison. No candidate was imported and no test, compiler, SSH, Runtime,
ROS, DDS or simulation process was started here.
