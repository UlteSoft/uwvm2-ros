#!/usr/bin/env python3
"""Explicit physical requalification for the retained Source40 command closure.

Raw reports and individual build receipts are never rewritten. ``same`` is a
comparison predicate at reviewed historical-reference checks: it admits each
selected historical file through PersistentPinSet and compares the real opened
current pin. It does not normalize JSON or replace Frame.pin. Non-pin values
remain exactly equal, including every native identity, compiler argv, command
result, source name and historical process observation.
"""
from __future__ import annotations

import gc
import hashlib
import itertools
import json
import os
from pathlib import Path
import sys
import types


RESTART_SHA256 = "0f14a3e8b122286f5d934f385d8ca582aa14039cd0b97531dad19b528a1b3260"
UNION_SHA256 = "8fbc2d1da44ef07bd8e6f69f4b174df70d802c471e989c1e752073cc95627d61"
PIN = frozenset(("path", "bytes", "sha256", "device", "inode", "mtime_ns", "ctime_ns"))
OLD_CG = {"memory_max": 68719476736, "memory_swap_max": 0, "expected_device": 29,
    "expected_inode": 18151, "expected_boot_id": "af121220-8c6e-4302-9853-f08bf4c50dbb",
    "expected_leaf": "/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope",
    "expected_cpus": "0-31"}
CURRENT_CG = {**OLD_CG, "expected_inode": 18358,
    "expected_boot_id": "501c54bd-af24-4d22-900c-8e24de274464"}
MOUNT = "/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1"
OLD_REPORT = MOUNT + "/source40-unitree-command14-build-r1/report.json"
OLD_REPORT_PIN = {"path": OLD_REPORT, "bytes": 3911428,
    "sha256": "b7c07cc7b83bf5996d85e1e812c7e4bc5221358d6dafcb69c3601d261af86747",
    "device": 81, "inode": 682208, "mtime_ns": 1792120051801163862,
    "ctime_ns": 1792120051808163972}
CARGO_ROOT = "/usr/lib/cargo/bin/coreutils"
# These are actual current metadata observations supplied by Root after the
# restart. They are not invented old-device pins or an execution admission.
# The raw original producer remains FAIL; only its exact catalog reference is
# used to select one additional historical complete regular-name inventory.
CARGO_REPORT_BOOT2_REFERENCE = {"path": MOUNT + "/source39-native-gateways-physics-peer-r1/report.json",
    "bytes": 7986560, "sha256": "75248cdddba3ab42f0020758cec81b0388c59d45ebd46d356ad434f6207e04d2",
    "device": 68, "inode": 677463, "mtime_ns": 1792118141739107205,
    "ctime_ns": 1792118141748107338}
CARGO_CATALOG_HISTORICAL = {"path": MOUNT + "/source39-native-gateways-physics-peer-r1/input-after-19.jsonl",
    "bytes": 27376, "sha256": "c9b899e553c50248a9b82ca898b55691a4100a2d5235174afcb43f7082d1ebc4",
    "device": 81, "inode": 677449, "mtime_ns": 1792118135782019011,
    "ctime_ns": 1792118135782019011}
ARM_CURRENT_ONLY = {"root": "/usr/lib/arm-none-eabi/bin", "historical_anchors": [
    {"device": 66307, "inode": value} for value in (12327242, 12327243, 12327247,
        12327248, 12327249, 12327250, 12327251, 12327252, 12327255)]}
LIMITS = {"catalog_bytes": 256 * 1024**2, "catalog_entries": 1048576,
          "file_bytes": 2 * 1024**3, "total_bytes": 64 * 1024**3}


def need(condition, message):
    if not condition:
        raise ValueError(message)


def _module(frame, path, wanted):
    """Load only an explicitly pinned helper inside the complete control root."""
    pin = frame.pin(path)
    need(pin["sha256"] == wanted and pin["bytes"] <= 2 * 1024**2,
         "reviewed restart helper changed")
    raw = Path(pin["path"]).read_bytes()
    need(hashlib.sha256(raw).hexdigest() == wanted and frame.pin(path) == pin,
         "restart helper changed while loaded")
    value = types.ModuleType("typed_command_explicit_" + Path(path).stem)
    value.__file__ = pin["path"]
    sys.modules[value.__name__] = value
    exec(compile(raw, pin["path"], "exec"), value.__dict__)
    return value


def _same_device_bridge(restart, previous, guard, current=None):
    """Reopen an exact artifact pin when device81 was reused after reboot.

    The unchanged restart helper deliberately requires distinct device values
    for a renumbering bridge. This separate narrow case instead requires ALL
    seven fields to agree after a complete actual streamed read. It preserves
    distinct old/current observations and establishes no boot/process or
    execution continuity merely because a filesystem device number repeated.
    The caller must select the exact old pin from its finite bound closure.
    """
    need(type(previous) is dict and set(previous) == PIN and previous["device"] == 81,
         "one exact selected old artifact81 full-seven observation required")
    bridge = restart.requalify_file(previous, previous["path"], guard)
    need(bridge["current_pin"] == previous and
         (current is None or bridge["current_pin"] == current),
         "same-device artifact must preserve its complete opened identity and bytes")
    bridge.update(content_relation="PERSISTENT_FULL7_REOBSERVED_SAME_DEVICE_WITHOUT_BOOT_CONTINUITY",
        permitted_identity_changes=[], device_transition={"historical": 81, "current": 81},
        boot_or_process_continuity_qualified=False)
    return bridge


class RetainedClosure:
    """One finite immutable old closure and separately recorded current facts."""
    def __init__(self, frame, frozen, namespace):
        self.frame, self.frozen, self.namespace = frame, frozen, namespace
        self.restart = _module(frame, Path(__file__).with_name("requalify_after_restart.py"), RESTART_SHA256)
        self.union_module = _module(frame, Path(__file__).with_name("requalify_hardlink_union.py"), UNION_SHA256)
        self.aliases, self.trees, self.symlink_targets = {}, {}, {}
        self.external_metadata = {}
        self.output = frame.out / "postboot-input-requalification"
        self.output.mkdir(mode=0o700)
        raw, report_pin = frame.common.body_pin(Path(OLD_REPORT))
        need(report_pin["sha256"] == OLD_REPORT_PIN["sha256"] and
             report_pin["bytes"] == OLD_REPORT_PIN["bytes"], "the original full failed batch changed")
        report = json.loads(raw)
        # This small maintained predicate is embedded in the derived builder.
        namespace["admit_batch"](report, report_pin)
        selected = {}
        def add(value):
            if type(value) is dict:
                if PIN <= set(value):
                    need(set(value) == PIN, "extended historical physical-pin schema is not admitted")
                    previous = selected.get(value["path"])
                    need(previous is None or previous == value,
                         "one retained path has conflicting historical full-seven observations")
                    selected[value["path"]] = dict(value)
                else:
                    for child in value.values():
                        add(child)
            elif type(value) is list:
                for child in value:
                    add(child)
        # Select the real persistent ledger, all nine product records and the
        # real original whole-root catalogs. Historical physical_path compiler
        # temporaries are deliberately not current file selections.
        add(report["input_pins"])
        add(report["configuration"])
        add(OLD_REPORT_PIN)
        for row in report["products"]:
            if row["status"] == "COMPILED_COMMAND_AND_FULL_BINARY_VALIDATED_NOT_EXECUTED":
                add(row)
        add(report["inventories_before"])
        add(report["inventories_after"])
        add(report["official_cpp_object"])
        # These additional catalogs were previously checked by physical
        # catalog hashes. Reopen their original raw documents, selecting only
        # the finite package/catalog references actually consumed below.
        for label, keys in (("typed_rust_selection_report", ("tool_roots_after",)),):
            document, _ = namespace["doc"](frame.input(label))
            for key in keys:
                add(document[key])
            del document
        native, _ = namespace["doc"](Path(report["original_native_producer"]["path"]))
        for family in native["families"]:
            add(family["derived_tree"])
        del native
        cpp, _ = namespace["doc"](Path(report["official_cpp_object"]["qualification"]["report"]["path"]))
        add(cpp["inventories_before"])
        add(cpp["inventories_after"])
        add(cpp["input_pins"])
        del cpp
        # An exact separately opened raw report anchors the additional cargo
        # catalog. Neither surrounding artifact directory becomes a Frame
        # root, and Frame.pin never accepts an out-of-closure path here.
        anchor_bridge = self.restart.requalify_persistent_pin(CARGO_REPORT_BOOT2_REFERENCE,
            CARGO_REPORT_BOOT2_REFERENCE["path"], frame.guard, historical_device=68, current_device=81)
        anchor_pin, anchor_raw = self.restart.opened_file(CARGO_REPORT_BOOT2_REFERENCE["path"],
            16 * 1024**2, frame.guard, collect=True)
        need(anchor_pin == anchor_bridge["current_pin"], "exact freshly reopened cargo-catalog report anchor changed")
        anchor = json.loads(anchor_raw)
        need(anchor["status"] == "FAIL", "the original cargo catalog producer must remain FAIL")
        cargo = anchor["input_roots_after"][19]
        need(cargo["root"] == CARGO_ROOT and cargo["catalog"] == CARGO_CATALOG_HISTORICAL and
             cargo["files"] == 114 and cargo["symlinks"] == 0,
             "one genuine complete historical cargo114 inventory required")
        cargo_bridge = _same_device_bridge(self.restart, CARGO_CATALOG_HISTORICAL, frame.guard)
        self.external_metadata = {"current_raw_report": anchor_pin,
            "boot2_to_current_raw_report_bridge": anchor_bridge,
            "raw_report_status_unchanged": "FAIL", "historical_catalog_bridge": cargo_bridge,
            "selection_scope": "ONE_EXACT_CATALOG_REFERENCE_FROM_UNMODIFIED_SOURCE39_INPUT_ROOTS_AFTER_19"}
        add(cargo["catalog"])
        del anchor, anchor_raw
        gc.collect()
        roots = [str(root) for root in frame.roots]
        roots += [str(frame.source), str(frame.manifest)]
        roots = sorted(set(roots), key=lambda value: (len(Path(value).parts), value))
        # Remove only redundant descendants of an already explicit complete
        # root on the same retained filesystem. Never admit a broad /var/tmp,
        # artifact mount, filesystem root or guessed package parent.
        reduced = []
        for root in roots:
            if any(root.startswith(parent + "/") for parent in reduced):
                continue
            reduced.append(root)
        mappings = []
        for root in reduced:
            old = 81 if root == MOUNT or root.startswith(MOUNT + "/") else 66307
            if old != 81:
                mappings.append({"root": root, "historical_device": old, "current_device": 66312})
        # New writable controls have no historical pin selection. The mapping
        # is consulted only for one of the finite selected historical pins.
        self.expected = selected
        self.same_device_expected = {name: pin for name, pin in selected.items() if pin["device"] == 81}
        self.same_device_evidence = {}
        self.pins = self.restart.PersistentPinSet(
            [pin for pin in selected.values() if pin["device"] != 81], mappings, frame.guard)
        need(frame.config["cgroup"] == CURRENT_CG, "exact current reboot incarnation required")
        self.report = report
        self.original_configuration, _ = namespace["doc"](Path(report["configuration"]["path"]))
        need(self.original_configuration["cgroup"] == OLD_CG,
             "the original producer cgroup must remain a historical observation")
        original = report["inventories_before"]
        need(len(original) == 59 and len({item["root"] for item in original}) == 59 and
             CARGO_ROOT not in {item["root"] for item in original} and Path(CARGO_ROOT) in frame.roots,
             "original59 plus exactly one newly explicit cargo complete Frame root required")
        inventories = [*original, cargo]
        self.union = self.union_module.HistoricalUnion(self.restart,
            [{"root": item["root"], "catalog": item["catalog"]} for item in inventories],
            frame.guard, LIMITS, device_transitions={81: 81, 66307: 66312},
            auxiliary_roots=[ARM_CURRENT_ONLY])
        # ARM aliases were never in these old complete catalogs. Their finite
        # current-only closure must be proved against the nine original inode
        # classes before walking any historical root. No historical ARM names,
        # timestamps, nlink or broad additional Frame root is invented.
        auxiliary_output = self.output / "current-only-arm"
        auxiliary_output.mkdir(mode=0o700)
        auxiliary = self.union.qualify_auxiliary(auxiliary_output)
        need(len(auxiliary) == 1 and auxiliary[0]["current_root"] == ARM_CURRENT_ONLY["root"] and
             auxiliary[0]["totals"]["files"] == 10 and auxiliary[0]["totals"]["symlinks"] == 0 and
             auxiliary[0]["historical_anchor_classes"] == ARM_CURRENT_ONLY["historical_anchors"],
             "only the finite ten current ARM names bound to nine old classes may close this union")
        auxiliary_raw, auxiliary_pin = frame.common.body_pin(Path(auxiliary[0]["current_catalog"]["path"]))
        need(auxiliary_pin == auxiliary[0]["current_catalog"], "actual current-only auxiliary catalog changed")
        auxiliary_rows = [json.loads(line) for line in auxiliary_raw.splitlines()]
        need(len(auxiliary_rows) == 10 and {row["relative"] for row in auxiliary_rows} ==
             {"ar", "as", "ld", "ld.bfd", "nm", "objcopy", "objdump", "ranlib", "readelf", "strip"} and
             all(row["kind"] == "file" for row in auxiliary_rows),
             "additional current-only auxiliary files, links or nested names are not admitted")
        frame.pin(auxiliary_pin["path"])
        del auxiliary_raw, auxiliary_rows
        for item in inventories:
            matches = [now for now in frame.catalogs if now["root"] == item["root"]]
            if not matches:
                need(any(Path(item["root"]).is_relative_to(root) for root in frame.roots),
                     "selected historical subcatalog lacks complete current Frame coverage")
                actual = frame.common.catalog(Path(item["root"]),
                    self.output / ("original-format-%03d.jsonl" % len(self.trees)), frame.roots)
            else:
                need(len(matches) == 1, "one actual current original-format catalog required")
                actual = matches[0]
            self.tree_matches(item, actual)
        self.union.finish()
        self.sync()

    def pin(self, previous, current=None):
        """Return only a real current pin and record its separate old bridge."""
        need(self.expected.get(previous["path"]) == previous,
             "historical pin is outside the preselected finite producer closure")
        if current is None and previous["path"] == CARGO_CATALOG_HISTORICAL["path"]:
            need(previous == CARGO_CATALOG_HISTORICAL, "only the exact selected external cargo catalog is admitted")
            current = self.restart.opened_file(previous["path"], previous["bytes"], self.frame.guard)
        else:
            current = self.frame.pin(previous["path"]) if current is None else current
        if previous["device"] != 81:
            return self.pins.admit(previous, observed_current=current)
        name = previous["path"]
        if name in self.same_device_evidence:
            need(self.same_device_evidence[name]["current_pin"] == current,
                 "memoized exact same-device pin differs from the freshly opened current owner observation")
            self.frame.guard()
            return dict(current)
        bridge = _same_device_bridge(self.restart, previous, self.frame.guard, current)
        need(name not in self.same_device_evidence or self.same_device_evidence[name] == bridge,
             "exact same-device current artifact observation changed")
        self.same_device_evidence[name] = bridge
        return dict(bridge["current_pin"])

    def alias(self, current, previous):
        """Rebind an unchanged invocation spelling after its device renumbering."""
        need(type(current) is dict and type(previous) is dict and set(current) == set(previous) and
             "physical" in current and "sha256" not in current, "exact tool alias observations required")
        need(previous["device"] in (81, 66307) and
             current["device"] == {81: 81, 66307: 66312}[previous["device"]] and
             all(current[key] == previous[key] for key in current if key != "device"),
             "tool alias changed beyond its explicitly permitted device transition")
        physical = previous["physical"]
        need(physical in self.expected, "selected invocation lacks a retained physical executable pin")
        self.pin(self.expected[physical])
        key = previous["path"]
        bridge = {"historical_alias": previous, "current_alias": current,
                  "content_relation": "SAME_INVOCATION_AND_PHYSICAL_EXECUTABLE_EXPLICIT_DEVICE_REBIND"}
        need(key not in self.aliases or self.aliases[key] == bridge, "invocation changed between comparisons")
        self.aliases[key] = bridge
        return True

    def same(self, left, right):
        """Compare a selected observation; never transform either raw document.

        Recursive structure is used only at explicitly reviewed field-level
        comparisons such as complete-family, full-MD or native proof records.
        Every non-pin field must still be exactly equal. Pin bridges are
        limited to the preselected finite persistent closure.
        """
        if type(left) is dict and type(right) is dict and set(left) == set(right) == PIN:
            if left["path"] != right["path"]:
                return False
            if left == right:
                return True  # Identical historical references remain historical.
            if left["device"] in (81, 66307) and right["device"] in (81, 66312):
                return self.pin(left, right) == right
            if right["device"] in (81, 66307) and left["device"] in (81, 66312):
                return self.pin(right, left) == left
            return False
        if type(left) is dict and type(right) is dict:
            if set(left) != set(right):
                return False
            if "physical" in left and "target" in left and "device" in left and "sha256" not in left:
                if left == right:
                    return True
                if left["device"] in (81, 66307) and right["device"] in (81, 66312):
                    return self.alias(right, left)
                if right["device"] in (81, 66307) and left["device"] in (81, 66312):
                    return self.alias(left, right)
                return False
            return all(self.same(left[key], right[key]) for key in left)
        if type(left) is list and type(right) is list:
            return len(left) == len(right) and all(self.same(a, b) for a, b in zip(left, right))
        return left == right

    def member(self, current, previous_values):
        return any(self.same(current, previous) for previous in previous_values)

    def retained_input(self, value):
        """Reopen persistent pins without confusing Rust's retired temporaries."""
        if type(value) is dict:
            if PIN <= set(value):
                need(set(value) == PIN, "extended historical physical-pin schema is not admitted")
                current = self.pin(value)
                path = Path(current["path"])
                if path.is_relative_to(self.frame.source):
                    need(self.frozen.get(str(path.relative_to(self.frame.source))) ==
                         {key: current[key] for key in ("bytes", "sha256")},
                         "retained consumed current source is absent from the complete manifest")
            elif {"path", "bytes", "sha256"} <= set(value):
                current = self.frame.pin(value["path"])
                need(all(current[key] == value[key] for key in ("path", "bytes", "sha256")),
                     "retained byte-only reference changed")
            for child in value.values():
                self.retained_input(child)
        elif type(value) is list:
            for child in value:
                self.retained_input(child)

    def _target(self, previous, current):
        """Retain the original catalog's separately pinned regular-file target.

        common.catalog explicitly permits individual external regular targets.
        Their exact old names/full-seven pins are selected by the bound old
        catalog, never by a broad new directory. They remain separate sidecars;
        Frame.pin's complete-root containment rules are unchanged.
        """
        need(set(previous) == set(current) == PIN and previous["path"] == current["path"],
             "exact historical/current symlink target observations required")
        path = previous["path"]
        if path in self.symlink_targets:
            bridge = self.symlink_targets[path]
            need(bridge["historical_pin"] == previous and bridge["current_pin"] == current,
                 "catalog target changed between whole-root observations")
            return
        need(previous["device"] in (81, 66307), "unselected historical target filesystem")
        bridge = (_same_device_bridge(self.restart, previous, self.frame.guard, current)
            if previous["device"] == 81 else self.restart.requalify_persistent_pin(previous, path, self.frame.guard,
                historical_device=66307, current_device=66312, observed_current=current))
        bridge["selection_scope"] = "EXACT_REGULAR_TARGET_FILE_FROM_BOUND_ORIGINAL_COMMON_CATALOG"
        self.symlink_targets[path] = bridge

    def _catalog_semantics(self, previous, current):
        """Check complete original file/link observations without JSON rewriting.

        The generic restart walk proves exact regular bytes and literal links.
        This independent original-format comparison additionally binds each
        resolved spelling, dangling state, link identity and external target.
        Both catalogs are opened at anchored directory descriptors and their
        complete bytes are hashed again before accepting any comparison.
        """
        old_pin = self.pin(previous["catalog"])
        now_pin = self.frame.pin(current["catalog"]["path"])
        need(now_pin == current["catalog"], "current Frame catalog physical identity differs")
        handles = []
        try:
            for pin in (old_pin, now_pin):
                parent, name = self.restart._parent(pin["path"])
                try:
                    descriptor = os.open(name, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK, dir_fd=parent)
                except BaseException:
                    os.close(parent)
                    raise
                try:
                    info = os.fstat(descriptor)
                except BaseException:
                    os.close(descriptor)
                    os.close(parent)
                    raise
                handles.append((parent, name, descriptor, info, pin))
                need(info.st_size == pin["bytes"] and info.st_dev == pin["device"] and
                     info.st_ino == pin["inode"] and info.st_mtime_ns == pin["mtime_ns"] and
                     info.st_ctime_ns == pin["ctime_ns"], "catalog changed before semantic comparison")
            streams = [os.fdopen(row[2], "rb", closefd=False) for row in handles]
            digests, counts = [hashlib.sha256(), hashlib.sha256()], [0, 0]
            try:
                rows = [iter(lambda stream=stream: stream.readline(65537), b"") for stream in streams]
                for index, lines in enumerate(itertools.zip_longest(*rows)):
                    self.frame.guard()
                    need(index < LIMITS["catalog_entries"] and all(line is not None and
                         len(line) <= 65536 and line.endswith(b"\n") for line in lines),
                         "bounded identical complete original/current catalog row sets required")
                    for number, line in enumerate(lines):
                        digests[number].update(line)
                        counts[number] += len(line)
                    old, now = map(json.loads, lines)
                    need(type(old) is dict and type(now) is dict and set(old) == set(now) and
                         old["relative"] == now["relative"] and old["kind"] == now["kind"] and
                         old["device"] in (81, 66307) and
                         now["device"] == {81: 81, 66307: 66312}[old["device"]],
                         "original/current catalog file or link identity mapping differs")
                    need(all(old[key] == now[key] for key in old if key not in ("device", "target_file")),
                         "original catalog metadata, literal/resolved link or dangling state changed")
                    if "target_file" in old:
                        need(old["kind"] == "symlink" and old["resolved"] == old["target_file"]["path"],
                             "regular symlink target must match the original resolved spelling")
                        self._target(old["target_file"], now["target_file"])
                for number, (_, _, _, _, pin) in enumerate(handles):
                    need(counts[number] == pin["bytes"] and digests[number].hexdigest() == pin["sha256"],
                         "complete catalog bytes changed during semantic comparison")
            finally:
                for stream in streams:
                    stream.close()
            for parent, name, descriptor, before, pin in handles:
                after = os.fstat(descriptor)
                need(self.restart._same_stat(before, after) and self.restart._same_stat(before,
                     os.stat(name, dir_fd=parent, follow_symlinks=False)), "catalog changed while compared")
                self.restart._verify_named_file(pin["path"], parent, name, before)
        finally:
            for parent, _, descriptor, _, _ in handles:
                os.close(descriptor)
                os.close(parent)

    def tree_matches(self, previous, current=None):
        """Compare full historical package content, not device-bearing hashes."""
        root = previous["root"]
        need(any(Path(root) == item or Path(root).is_relative_to(item) for item in self.frame.roots),
             "historical whole-root proof lacks an explicit complete current input root")
        need(type(current) is dict and current["root"] == root and all(current[key] == previous[key]
             for key in ("files", "bytes", "symlinks")), "actual current Frame catalog and matching package dimensions required")
        self.pin(previous["catalog"])
        key = root + "\0" + previous["content_sha256"]
        if key not in self.trees:
            output = self.output / ("root-%03d.jsonl" % len(self.trees))
            if not self.union.initial_complete:
                result = self.union.requalify_tree(previous["catalog"], root, str(output))
            else:
                result = self._covered_view(previous, output)
            need(all(result["totals"][name] == previous[name] for name in ("files", "bytes", "symlinks")),
                 "whole-root requalification totals differ from historical producer")
            self.frame.pin(output)
            self._catalog_semantics(previous, current)
            self.trees[key] = {"historical_inventory": previous, "current_content_qualification": result,
                "original_link_resolution_and_target_files": "SEPARATELY_STRICTLY_REQUALIFIED",
                "current_original_format_catalog": current["catalog"]}
        return True

    def _covered_view(self, previous, output):
        """Check a later original catalog inside the already complete union.

        Other genuine producers may have recorded a separate catalog of the
        same root or a subdirectory. They do not add any physical file name to
        the selected historical union. Every current regular file must already
        be a member of that original59/cargo closure, and the union observer
        still enforces its full exact current inode/content/nlink class.
        """
        root = previous["root"]
        need(self.union.initial_complete and any(Path(root) == Path(item["root"]) or
             Path(root).is_relative_to(item["root"]) for item in self.union.selections.values()),
             "a later catalog may cover only an already admitted complete historical root")
        expected, bridge = self.restart._old_catalog(previous["catalog"], self.frame.guard, LIMITS)
        seen = set()
        with output.open("xb") as stream:
            def visit(row):
                relative = row["relative"]
                content = {key: value for key, value in row.items()
                           if key not in ("current_pin", "current_link", "current_nlink")}
                need(relative in expected and relative not in seen and content == expected[relative],
                     "later original catalog differs inside the complete historical union")
                seen.add(relative)
                stream.write((json.dumps(row, sort_keys=True, separators=(",", ":")) + "\n").encode())
            totals = self.restart._walk(root, self.frame.guard, LIMITS, visit,
                file_identity=lambda relative, pin, info: self.union.observe(root, relative, pin, info))
            need(seen == set(expected), "later original covered catalog is missing a file or symlink")
            stream.flush(); os.fsync(stream.fileno())
        return {"historical_catalog": bridge, "current_root": root, "totals": totals,
            "current_catalog": self.restart.opened_file(str(output), LIMITS["catalog_bytes"], self.frame.guard),
            "content_relation": "EXACT_REGULAR_FILE_AND_SYMLINK_CONTENT_SET",
            "symlink_target_qualification": "NOT_PERFORMED", "execution_qualification": "NOT_PERFORMED",
            "hardlink_policy": self.union_module.POLICY,
            "hardlink_scope": "COVERED_VIEW_WITH_NO_ADDITIONAL_HISTORICAL_UNION_NAMES"}

    def sync(self):
        self.frame.report["postboot_retained_input_qualification"] = {
            "historical_cgroup": OLD_CG, "current_cgroup": CURRENT_CG,
            "historical_batch_status": "FAIL", "raw_reports_rewritten": False,
            "old_execution_promoted_to_current": False,
            "persistent_file_bridges": (self.pins.evidence +
                [self.same_device_evidence[name] for name in sorted(self.same_device_evidence)])
                if hasattr(self, "pins") else [],
            "invocation_bridges": list(self.aliases.values()),
            "original_catalog_regular_target_bridges": list(self.symlink_targets.values()),
            "separately_selected_cargo_metadata": self.external_metadata,
            "historical_hardlink_union": self.union.evidence if hasattr(self, "union") and self.union.initial_complete else None,
            "whole_root_qualifications": list(self.trees.values())}

    def preserve(self):
        self.pins.preserve()
        for bridge in self.same_device_evidence.values():
            pin = bridge["current_pin"]
            need(self.restart.opened_file(pin["path"], pin["bytes"], self.frame.guard) == pin,
                 "exact same-device current artifact changed before finish")
        # The complete union reopens every selected root and every multi-name
        # class, including the separately qualified current-only ARM aliases.
        # A covered view introduces no extra file names or authority.
        self.union.preserve()
        for key in ("current_raw_report",):
            pin = self.external_metadata[key]
            need(self.restart.opened_file(pin["path"], pin["bytes"], self.frame.guard) == pin,
                 "exact separately selected cargo anchor metadata changed before finish")
        for bridge in self.symlink_targets.values():
            pin = bridge["current_pin"]
            need(self.restart.opened_file(pin["path"], pin["bytes"], self.frame.guard) == pin,
                 "separately selected current catalog target changed before finish")
        for item in self.aliases.values():
            need(self.namespace["utilities_for_postboot"].alias(item["current_alias"]["path"]) == item["current_alias"],
                 "current selected invocation changed before finish")
        self.sync()


def attach(frame, frozen, namespace, utilities):
    """Attach explicit comparison services while preserving Frame.pin semantics."""
    namespace["utilities_for_postboot"] = utilities
    closure = RetainedClosure(frame, frozen, namespace)
    frame.postboot = closure
    namespace["POSTBOOT"] = closure
    return closure
