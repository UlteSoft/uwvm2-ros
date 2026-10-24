"""Retire exact obsolete Task08 binaries inside the original finite Linux owner.

The user authorized removal of reproducible old build products. Every selected
ordinary file must still match the inventory, have no observed live reference,
and have an exact matching member in the retained central archive. Source,
recipes, compact logs/results, current providers and the central masters stay.
No SDK/compiler/Guest runs and no new large archive is produced by this script.
"""

import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import tarfile
import time


ROOT = Path("/home/macromodel/Documents/uwvm-test-261018/agents/08")
CENTRAL = ROOT.parent / "director-20261021/infrastructure/archives"


def sha(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def metadata(path):
    value = path.lstat()
    return dict(path=str(path), logical_bytes=value.st_size, allocated_bytes=value.st_blocks * 512,
                uid=value.st_uid, gid=value.st_gid, mode=oct(stat.S_IMODE(value.st_mode)),
                mtime_ns=value.st_mtime_ns, links=value.st_nlink)


def free_bytes():
    value = os.statvfs(ROOT)
    return value.f_bavail * value.f_frsize


def references(path):
    found, inaccessible = [], set()
    spelling = str(path)
    for process in Path("/proc").iterdir():
        if not process.name.isdecimal() or int(process.name) == os.getpid():
            continue
        used = False
        try:
            used = spelling.encode() in (process / "cmdline").read_bytes()
        except OSError:
            inaccessible.add(int(process.name))
        for name in ("cwd", "exe"):
            try:
                used = used or os.readlink(process / name) == spelling
            except OSError:
                inaccessible.add(int(process.name))
        try:
            for descriptor in (process / "fd").iterdir():
                try:
                    used = used or os.readlink(descriptor) == spelling
                except OSError:
                    continue
        except OSError:
            inaccessible.add(int(process.name))
        if used:
            found.append(int(process.name))
    return dict(observed_live_pids=found, permission_limited_pids=sorted(inaccessible),
                serialized_by_original_common_heavy_lock=True)


def main():
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("original Linux finite common owner required")
    if os.getuid() != 1000:
        raise RuntimeError("this ordinary cleanup retires Task08 UID1000 products only")
    manifest = Path(sys.argv[1])
    selected = json.loads(manifest.read_bytes())
    assert selected["schema"] == "task08-exact-owned-build-product-retirement-v1"
    rows = selected["files"]
    assert 1 <= len(rows) <= 24
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    work.mkdir(exist_ok=True)
    report = dict(schema="task08-user-directed-build-cleanup-v1", status="STARTED",
                  observation_utc=time.strftime("%Y-%m-%d %H:%M:%S UTC", time.gmtime()),
                  input_sha256=sha(manifest), runner_sha256=sha(Path(__file__)),
                  uid=os.getuid(), common_heavy_lock=True, filesystem_free_before=free_bytes(),
                  logical_bytes_deleted=0, allocated_bytes_deleted=0, files_deleted=0,
                  results=[], retained_central_archives=[], acceptance_credit=0,
                  compiler_or_Guest_runs=0, default_authority=[])

    def save():
        (work / "cleanup-current.json").write_text(json.dumps(report, indent=2) + "\n")

    verified_archives = {}
    save()
    try:
        for row in rows:
            path = Path(row["path"])
            item = dict(path=str(path), status="STARTED", recovery=row["recovery"])
            report["results"].append(item)
            assert path.is_absolute() and path.is_relative_to(ROOT) and path.resolve(strict=True) == path
            assert row["category"] == "OBSOLETE_REPRODUCIBLE_NATIVE_OR_WASM_BUILD"
            assert row["reproducible"] is True and row["uid"] == 1000
            assert stat.S_ISREG(path.lstat().st_mode) and not path.is_symlink()
            actual = metadata(path)
            assert all(actual[key] == row[key] for key in actual), "file changed since exact inventory"
            assert sha(path) == row["sha256"]
            assert os.access(path.parent, os.W_OK)
            live = references(path)
            item["process_observation"] = live
            assert not live["observed_live_pids"], "current use refuses retirement"
            archive = Path(row["keep_master"]["path"])
            assert archive.is_relative_to(CENTRAL) and archive.resolve(strict=True) == archive
            assert not archive.is_symlink() and stat.S_ISREG(archive.lstat().st_mode)
            assert metadata(archive) == row["keep_master"]
            if archive not in verified_archives:
                digest = sha(archive)
                assert digest == row["recovery_archive_sha256"]
                verified_archives[archive] = digest
                report["retained_central_archives"].append(dict(path=str(archive), sha256=digest,
                                                               bytes=archive.stat().st_size))
            run = ROOT / path.relative_to(ROOT).parts[0]
            member_name = str(path.relative_to(run))
            with tarfile.open(archive, "r:") as package:
                member = package.getmember(member_name)
                assert member.isfile() and member.size == actual["logical_bytes"]
                stream = package.extractfile(member)
                assert stream is not None
                with stream:
                    assert hashlib.file_digest(stream, "sha256").hexdigest() == row["sha256"]
            item.update(before=actual, sha256=row["sha256"], exact_retained_archive_member=member_name)
            save()
            # No recursive/glob removal: only the exact still-identical ordinary
            # file under this owner's obsolete completed run is unlinked.
            assert metadata(path) == actual and not references(path)["observed_live_pids"]
            path.unlink()
            assert not path.exists()
            item["status"] = "DELETED_EXACT_REPRODUCIBLE_BUILD_PRODUCT"
            report["files_deleted"] += 1
            report["logical_bytes_deleted"] += actual["logical_bytes"]
            report["allocated_bytes_deleted"] += actual["allocated_bytes"] if actual["links"] == 1 else 0
            save()
        report["status"] = "COMPLETED_EXACT_OWN_REPRODUCIBLE_BUILD_CLEANUP"
    except BaseException as error:
        report.update(status="PARTIAL_OR_FAILED_RETAINED", error=repr(error))
        raise
    finally:
        report.update(filesystem_free_after=free_bytes(), input_unchanged=sha(manifest) == report["input_sha256"],
                      runner_unchanged=sha(Path(__file__)) == report["runner_sha256"],
                      central_masters_still_present=all(path.is_file() and sha(path) == digest
                                                       for path, digest in verified_archives.items()))
        report["filesystem_free_delta_bytes"] = report["filesystem_free_after"] - report["filesystem_free_before"]
        save()


if __name__ == "__main__":
    main()
