#!/usr/bin/env python3
"""Explicit, reversible workbench repair for one verified VS Code build.

The UWVM extension never invokes this tool automatically. Preparation writes
only a review bundle; apply/restore require a separate explicit invocation.
VS Code's integrity manifest and signature checks are deliberately untouched.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import tempfile

VERSION = "1.119.0"
COMMIT = "8b640eef5a6c6089c029249d48efa5c99adf7d51"
ORIGINAL_SHA256 = "3ec8861e9887971e1e7accb6e7d0c154be279f9490f86f7768c21aa6dcac489f"
REPAIRED_SHA256 = "4ef457921663981e75ced3334024395fb4b42c91a5ac86cbdfa6311905687fd5"
WORKBENCH = Path("out/vs/workbench/workbench.desktop.main.js")
FORMAT = "uwvm-vscode-workbench-repair-v1"

# Keep this list small and reviewable. No call-stack frames are added, removed,
# reordered, or assigned false coordinates. The real top frame remains top.
CHANGES = (
    ("Pass the existing type element to the native expression editor",
     'this.renderInputBox(t.name,t.value,t.inputBoxContainer,n)',
     'this.renderInputBox(t.name,t.value,t.inputBoxContainer,n,t.type)'),
    ("Hide the type only for the lifetime of the native input box",
     'renderInputBox(i,e,t,o){i.style.display="none",e.style.display="none",t.style.display="initial",yt(t);',
     'renderInputBox(i,e,t,o,uwvmTypeElement){const uwvmTypeDisplay=uwvmTypeElement?.style.display;if(uwvmTypeElement)uwvmTypeElement.style.display="none";i.style.display="none",e.style.display="none",t.style.display="initial",yt(t);'),
    ("Restore the type on commit, cancellation, blur, and disposal",
     'let r=ud((l,c)=>{i.style.display="",e.style.display="",t.style.display="none";let u=n.value;',
     'let r=ud((l,c)=>{if(uwvmTypeElement)uwvmTypeElement.style.display=uwvmTypeDisplay;i.style.display="",e.style.display="",t.style.display="none";let u=n.value;'),
    ("Retain boolean source precision only on UWVM frames",
     'return new GT(this,n.id,s,n.name,n.presentationHint,new G(n.line,n.column,n.endLine||n.line,n.endColumn||n.column),i+r,typeof n.canRestart=="boolean"?n.canRestart:!0,n.instructionPointerReference)',
     'return Object.assign(new GT(this,n.id,s,n.name,n.presentationHint,new G(n.line,n.column,n.endLine||n.line,n.endColumn||n.column),i+r,typeof n.canRestart=="boolean"?n.canRestart:!0,n.instructionPointerReference),{uwvmSourceExact:this.session.configuration.type==="uwvm-llvm-full"&&typeof n.uwvmSourceExact==="boolean"?n.uwvmSourceExact:void 0})'),
    ("Use the native green marker for an in-statement UWVM source stop",
     'return a.getId()===r?.getId()?(i&&t.push({options:_dn,range:n})',
     'return a.getId()===r?.getId()&&a.uwvmSourceExact!==!1?(i&&t.push({options:_dn,range:n})'),
)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def repaired(original):
    if digest(original) != ORIGINAL_SHA256:
        raise ValueError("Unrecognized workbench; refusing to patch a different or modified build")
    text = original.decode("utf-8")
    for name, before, after in CHANGES:
        if text.count(before) != 1:
            raise ValueError(f"Expected one repair anchor: {name}")
        text = text.replace(before, after)
    result = text.encode("utf-8")
    if digest(result) != REPAIRED_SHA256:
        raise ValueError("Repair output does not match the verified build")
    return result


def target(app_root):
    root = Path(app_root).resolve(strict=True)
    package = json.loads((root / "package.json").read_text())
    product = json.loads((root / "product.json").read_text())
    if package.get("version") != VERSION or product.get("commit") != COMMIT:
        raise ValueError(f"Only VS Code {VERSION}, commit {COMMIT}, has been verified")
    path = root / WORKBENCH
    if path.is_symlink() or path.resolve(strict=True) != path or not path.is_file():
        raise ValueError("Workbench must be a regular file inside the selected app root")
    return path


def prepare(app_root, bundle):
    path = target(app_root)
    original = path.read_bytes()
    patched = repaired(original)
    bundle = Path(bundle)
    # Never overwrite a previous backup or place output inside the application.
    if bundle.resolve().is_relative_to(Path(app_root).resolve()):
        raise ValueError("Prepare the repair bundle outside the VS Code application")
    bundle.mkdir(mode=0o700, parents=False, exist_ok=False)
    (bundle / "original.js").write_bytes(original)
    (bundle / "repaired.js").write_bytes(patched)
    manifest = dict(format=FORMAT, version=VERSION, commit=COMMIT,
                    originalSha256=ORIGINAL_SHA256, repairedSha256=REPAIRED_SHA256,
                    source=str(path), changes=[name for name, _, _ in CHANGES])
    (bundle / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    return dict(action="prepared", bundle=str(bundle.resolve()), applicationChanged=False)


def load_bundle(bundle):
    bundle = Path(bundle)
    manifest = json.loads((bundle / "manifest.json").read_text())
    expected = dict(format=FORMAT, version=VERSION, commit=COMMIT,
                    originalSha256=ORIGINAL_SHA256, repairedSha256=REPAIRED_SHA256)
    if any(manifest.get(key) != value for key, value in expected.items()):
        raise ValueError("Repair bundle identity does not match the verified build")
    original = (bundle / "original.js").read_bytes()
    patched = (bundle / "repaired.js").read_bytes()
    if repaired(original) != patched:
        raise ValueError("Repair bundle was modified; refusing to apply or restore")
    return original, patched


def replace_checked(path, expected, replacement):
    if path.is_symlink():
        raise ValueError("Refusing to replace a symbolic link")
    original_stat = path.stat()
    if path.read_bytes() != expected:
        raise ValueError("Workbench changed or is already in another state; refusing to overwrite it")
    descriptor, temporary = tempfile.mkstemp(prefix=".uwvm-workbench-", dir=path.parent)
    try:
        with os.fdopen(descriptor, "wb") as output:
            output.write(replacement)
            output.flush()
            if os.name != "nt":
                current = os.fstat(output.fileno())
                if (current.st_uid, current.st_gid) != (original_stat.st_uid, original_stat.st_gid):
                    os.fchown(output.fileno(), original_stat.st_uid, original_stat.st_gid)
                os.fchmod(output.fileno(), stat.S_IMODE(original_stat.st_mode))
            os.fsync(output.fileno())
        current_stat = path.stat()
        identity = lambda value: (value.st_dev, value.st_ino, value.st_size,
                                  value.st_mtime_ns, value.st_ctime_ns)
        if path.is_symlink() or identity(current_stat) != identity(original_stat) or path.read_bytes() != expected:
            raise ValueError("Workbench changed during preparation; refusing replacement")
        os.replace(temporary, path)
        if path.read_bytes() != replacement:
            raise OSError("Workbench replacement did not read back correctly")
    finally:
        if os.path.exists(temporary):
            os.unlink(temporary)


def change(app_root, bundle, *, restore=False):
    path = target(app_root)
    original, patched = load_bundle(bundle)
    replace_checked(path, patched if restore else original, original if restore else patched)
    return dict(action="restored" if restore else "applied", path=str(path),
                sha256=digest(path.read_bytes()), restartRequired=True,
                integrityManifestChanged=False)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("operation", choices=("prepare", "apply", "restore"))
    parser.add_argument("--app-root", required=True, type=Path,
                        help="VS Code resources/app directory containing package.json")
    parser.add_argument("--bundle", required=True, type=Path,
                        help="new directory for prepare; reviewed repair directory for apply/restore")
    args = parser.parse_args()
    try:
        result = (prepare(args.app_root, args.bundle) if args.operation == "prepare" else
                  change(args.app_root, args.bundle, restore=args.operation == "restore"))
    except (OSError, ValueError) as error:
        parser.exit(1, f"VS Code repair refused: {error}\n")
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
