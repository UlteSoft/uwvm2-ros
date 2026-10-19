# Boot3 backing-storage recovery,2026-10-17

Root recovered backing space by archiving only explicitly selected obsolete
source snapshots. The eight-GiB reserve guard was unchanged. The final
[Source22–26 retirement receipt](continuation-20261016-boot3-evidence/storage-source22-26-archive/retirement.json)
records14,495,014,912 bytes available after retirement, up from8,583,028,736 bytes
before the Source2–26 batches. These are observations at those checkpoints;
subsequent jobs consume space and must continue checking the same live guard.

Each batch first received a complete remote content/inventory archive. The local
full gzip/tar audit checked every file's SHA256, mode and mtime, exact directory
and empty-directory entries, full gzip CRC/EOF, and tar trailing bytes. Only after
that audit did Root retire the selected remote source directories through held
directory descriptors. Source2–26 comprised396,745 files and4,682,671,819 ordinary
file bytes across five batches. Source41/42 were separately archived earlier.

| Archived source epochs | Persistent compressed archive bytes | Complete audit | Selected retirement |
| --- | ---: | --- | --- |
|2–6|184,877,916|[audit](continuation-20261016-boot3-evidence/storage-source2-6-archive/local-audit.json)|[receipt](continuation-20261016-boot3-evidence/storage-source2-6-archive/retirement.json)|
|7–11|184,722,061|[audit](continuation-20261016-boot3-evidence/storage-source7-11-archive/local-audit.json)|[receipt](continuation-20261016-boot3-evidence/storage-source7-11-archive/retirement.json)|
|12–16|184,946,148|[audit](continuation-20261016-boot3-evidence/storage-source12-16-archive/local-audit.json)|[receipt](continuation-20261016-boot3-evidence/storage-source12-16-archive/retirement.json)|
|17–21|193,682,345|[audit](continuation-20261016-boot3-evidence/storage-source17-21-archive/local-audit.json)|[receipt](continuation-20261016-boot3-evidence/storage-source17-21-archive/retirement.json)|
|22–26|225,412,597|[audit](continuation-20261016-boot3-evidence/storage-source22-26-archive/local-audit.json)|[receipt](continuation-20261016-boot3-evidence/storage-source22-26-archive/retirement.json)|
|41–42|92,636,271|[audit](continuation-20261016-boot3-evidence/storage-source41-42-archive/local-audit.json)|[receipt](continuation-20261016-boot3-evidence/storage-source41-42-archive/retirement.json)|

Compressed archives persist outside the project at
`/Users/liyinan/Documents/MacroModel/uwvm2-ros-recovery-archives/20261017/`.
Each evidence directory's `persistent-archive.json` records the exact persisted
path, complete byte count and SHA256; `remote-archive.json` and its actual pin
bind the original complete archive construction. The source manifests and raw
reports remain available. No report or compiled product was retired. The
currently consumed Source33/35/37/39/40/43 and the remaining Source27–40 snapshots
stay live, along with their SDKs, controls and native products. Retirement receipts
record the actual cgroup and preservation checks on selected live dependencies.

An archive preserves bytes, modes and recorded directory structure. Restoration
must acquire fresh current physical observations; it cannot restore an old inode,
ctime, process birth, mount, cgroup incarnation or execution result. The receipts
explicitly make no old-inode restoration claim. Existing failed build/acquisition
reports remain failed, and no storage recovery qualifies ROS, DDS or physics.

After this recovery, actual r6 Typed40
[staging](continuation-20261016-boot3-evidence/typed40-r6-stage-prepare/stage.json)
and [zero-actor preparation](continuation-20261016-boot3-evidence/typed40-r6-stage-prepare/inner.json)
passed, followed by a running build; terminal compiler acceptance remained
pending at this checkpoint. CP310 r3
[metadata staging](continuation-20261016-boot3-evidence/cp310-stage-r3/stage.json)
also passed. Its subsequent independent
[full-view preparation](continuation-20261016-boot3-evidence/cp310-prepare-r3-pass/owner.json)
completed with status `PREPARED_CONTENT_ONLY_NOT_EXECUTED`,32046 files,
1,266,016,500 file bytes and31611 successful CoW clones of nonempty files.
Full source/clone hashes and final preservation passed with zero commands;
no target code, ROS or physics ran. Earlier r5 build and CP310 r2 reserve failures remain
unchanged in their separate raw evidence. The separate Noetic6d
[39,015-byte cache-view report](continuation-20261016-boot3-evidence/noetic6d-cache-view/report.json),
SHA256 `9985cef5f0398c4f48b05142c9b6850e2b6c14b286c4f9de64b759af8ba6de9a`,
prepared41 independent CoW DEB candidates and excluded one original file. It
started no actor, network operation, extraction or compiler and reestablished no
signature authority. Full signed acquisition/rebuild and runtime tests remain
separate work. All actual Linux activity stays in the same hard64-GiB,
zero-swap cgroup with live memory, owned-process and backing-space guards.
