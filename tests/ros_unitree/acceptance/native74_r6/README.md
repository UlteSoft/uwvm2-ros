# Native74 r6 copied glibc library-directory closure

The r5 attempt compiled all69 Gateway TUs and the portable String peer, held
execution gate and String metadata exporter, totaling72 TUs. The metadata
exporter then exited before its cooperative mapped stop: the copied Ubuntu
loader fell through to the host libc because the physical SDK's separate
`lib/x86_64-linux-gnu` directory was absent from the recorded library search.
The failed batch and its actual stderr remain historical failure evidence.

This revision records the actual copied libc ELF and requires its physical
directory to equal the actual copied loader's directory. That directory is
added to the shared native library selection used by the product's exact
DT_RPATH and its recorded execution LD_LIBRARY_PATH. The existing mountpoint
and bind implementations remain byte-identical: no missing /lib mirror
destination is created in the preserved SDK. GNUld records the physical path
while linking in the logical SDK; metadata/transport targets execute as their
ordinary physical ELF outside that logical chroot. No loader argv wrapper,
ELF rewriting, host libc replacement or library installation is used.

A separate actual bounded preflight executed the retained r5 String exporter
with the corrected library environment and returned the genuine String type
identity. Its receipt is SHA256
`1d946971507b32a7f8441332be3b4cd2345b941ca83dc2bd3c1b6640628ac719`.
That one metadata preflight neither promotes r5 nor qualifies72/74 objects,
complete process mappings, DDS, Runtime execution or physical control.

The r6 full batch requires all74 fresh target TUs under the unchanged
memory-aware four-job cap,5-GiB per-heavy-job reservation,20-GiB compiler tree
and24-GiB outer limit in the original64-GiB/swap-zero cgroup. Completed content
prerequisites and each historical producer keep their genuine source labels.
Read the r5/r4 documents for the unchanged exact restart/file/whole-tree and
actual MD, FileAPI, verbose compiler/link, native ELF and mapping contracts.
