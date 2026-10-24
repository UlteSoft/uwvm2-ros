#!/usr/bin/env python3
"""@file qemu.py
@brief Resolve and inspect an explicitly selected retained QEMU installation.
@details A distribution may put its executable below usr/bin. Resolution never
downloads, installs or executes a candidate, and never searches another user's
home. Callers must execute the version and ARM cohort inside the resource gate.
"""
import os
from pathlib import Path

from identity import elf, sha256


def locate(*, executable=None, root=None, expected_sha256=None):
    """@brief Locate a pinned native qemu-aarch64 without guessing its layout.
    @param executable Explicit executable path; mutually exclusive with root.
    @param root Installation root containing usr/bin or bin. No PATH search occurs.
    @param expected_sha256 Optional immutable expected SHA-256; mismatch refuses use.
    @return Resolved regular executable path and its byte/header identity.
    @exception ValueError Missing, ambiguous, changed or non-native ELF candidate.
    @exception OSError Selected files are not completely readable.
    @details The host machine must select the expected QEMU ELF architecture
    separately. ELF metadata describes this host executable, not the ARM guest.
    """
    if (executable is None) == (root is None):
        raise ValueError('Select exactly one QEMU executable or installation root')
    choices = [Path(executable)] if executable is not None else [
        Path(root) / 'usr/bin/qemu-aarch64', Path(root) / 'bin/qemu-aarch64']
    paths = sorted({p.resolve() for p in choices if p.is_file() and os.access(p, os.X_OK)})
    if len(paths) != 1:
        raise ValueError('The selected installation must resolve exactly one executable')
    path = paths[0]
    metadata = elf(path)
    if metadata is None or metadata['bits'] != 64:
        raise ValueError('Expected a native ELF64 QEMU executable')
    digest = sha256(path)
    if expected_sha256 is not None and digest != expected_sha256:
        raise ValueError('QEMU executable differs from the selected immutable identity')
    return path, {'bytes': path.stat().st_size, 'sha256': digest, 'elf': metadata}
