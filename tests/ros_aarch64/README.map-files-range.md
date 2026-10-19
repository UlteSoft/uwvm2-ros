# Canonical Linux map_files lookup names

Linux prints low addresses in `/proc/PID/maps` with padding, while its
`map_files` dentries use unpadded lowercase hexadecimal endpoints. The kernel
`dname_to_vma_addr` parser rejects redundant leading zeroes; directory emission
uses `%lx-%lx`. See the primary
[Linux base.c source](https://github.com/torvalds/linux/blob/master/fs/proc/base.c)
and its readable
[parser](https://codebrowser.dev/linux/linux/fs/proc/base.c.html#dname_to_vma_addr)
and [directory emission](https://codebrowser.dev/linux/linux/fs/proc/base.c.html#2478).

Root's actual Source64 held-stdin diagnostic completed collection and normal
preservation/cleanup, with real compiler EOF return code zero. It recorded
three stopped rounds for each real GCC and cc1plus birth. All six rounds had
the selected native QEMU host image but unavailable target-map proof. For
example, raw `00400000-0050c000` and `00400000-016e2000` mapping rows were used
literally as `map_files` names and returned ENOENT. The original raw reports
and earlier failed preparations remain immutable. This observation identifies
a concrete current lookup defect; it does not prove a later canonical lookup
or authorize ARM, Gateway, SDK or ROS acceptance.

`canonical_map_files_range` validates a bounded native64 numeric interval and
formats its endpoints in lowercase hexadecimal without padding. Only the
kernel lookup basename changes. The original raw row and receipt `range`
retain their exact spelling. Every pidfd/birth/cgroup/executable bracket,
actual opened FD fstat filter, complete hash/extent, retained-descriptor cache,
ELF check and positive receipt validator stays unchanged. No raw pathname,
argument, normalization result or maps device/inode text substitutes for an
opened mapping proof. Lookup errors continue to fail; there is no alternate
path retry or permission relaxation.

The separate nine-method suite accepts an already MF-bound runtime plus an
exclusive owned scratch. It covers the actual low-address spellings, PIE and
uppercase spelling, native64 bounds, malformed/nonstring/oversized/reversed
ranges, a real regular-file FD under a modeled canonical proc tree, and
unchanged hash/inode rejection. The proc/pidfd/birth facts in that filesystem
model are explicitly mocked. No child or kernel mapping is established by
the model. All nine methods are source-only and unexecuted in this package.

The candidate was prepared in `/tmp`; the repository, Source64 source/runtime
products, all old acceptance copies and the actual diagnostic controller are
unchanged by its author. The external diagnostic's original raw observer has
the same padded-name construction and is preserved as actual evidence; any
future diagnostic successor needs separate source review. Adoption requires a
new complete-source freeze and real current qualification, never rewriting a
failed Source64 receipt.
