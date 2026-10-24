#!/usr/bin/env python3
"""Acquire the formatter from the same official manifest as Rust1.85.0.

@brief Prepare a version-matched, checksum-verified Rust formatter.
@details Run only inside the shared finite Linux owner. The immutable compiler
installation and its original download/evidence files remain untouched. This
acquisition/version check establishes no SDK compilation or Guest acceptance.
@see sdk/origin/rust/contract.json; FORMAT-PLAN.md in Task03's round03 handoff.
"""

from pathlib import Path
import hashlib
import json
import os
import subprocess
import tarfile
import tomllib
import urllib.request


def main():
    """@brief Acquire and inspect the explicitly pinned Rust1.85 formatter.

    @details The captured official release manifest selects the exact archive.
    Only its regular rustfmt executable is materialized under the active owner.
    A matching existing compiler library directory is explicit, never searched
    through ambient user paths. Network/checksum/process failures are retained.
    No SDK source, permission or shared publication is changed. Execution is
    synchronous and single-job; the enclosing owner supplies all finite limits.
    @return Process status0 only after a real formatter version command succeeds.
    @see https://static.rust-lang.org/dist/2025-02-20/channel-rust-1.85.0.toml
    """
    root = Path.cwd()
    rust = Path('/home/macromodel/Documents/uwvm-test-261018/agents/03/20261021-toolchains-r1')
    manifest = rust / 'work/downloads/rust-1.85.0.toml'
    raw = manifest.read_bytes()
    descriptor = tomllib.loads(raw.decode())['pkg']['rustfmt-preview']['target']['x86_64-unknown-linux-gnu']
    url = descriptor['xz_url']
    expected = '99e3bc5f77720bac977a84444428b9162b242031d2cf6949bdb66ead0d3bc5f5'
    if url != 'https://static.rust-lang.org/dist/2025-02-20/rustfmt-1.85.0-x86_64-unknown-linux-gnu.tar.xz' or descriptor['xz_hash'] != expected:
        raise ValueError('Exact original compiler-release formatter descriptor required')
    archive = root / 'rustfmt-1.85.0.tar.xz'
    state = {'kind': 'task03-rustfmt-acquisition-v1', 'status': 'DOWNLOADING',
             'url': url, 'expected_sha256': expected,
             'manifest_sha256': hashlib.sha256(raw).hexdigest(),
             'compiler_runs': 0, 'Guest_runs': 0, 'default_authority': []}

    def save():
        (root / 'acquisition.json').write_text(json.dumps(state, indent=2) + '\n')

    save()
    with urllib.request.urlopen(url, timeout=30) as response, archive.open('xb') as output:
        total = 0
        while chunk := response.read(262144):
            total += len(chunk)
            if total > 16 * 1024 * 1024:
                raise ValueError('Formatter archive exceeds its declared finite extent')
            output.write(chunk)
    with archive.open('rb') as stream:
        observed = hashlib.file_digest(stream, 'sha256').hexdigest()
    if observed != expected:
        raise ValueError('Official formatter archive checksum differs')
    destination = root / 'tools/bin/rustfmt'
    destination.parent.mkdir(parents=True)
    with tarfile.open(archive) as source:
        members = [item for item in source.getmembers()
                   if item.name.endswith('/rustfmt-preview/bin/rustfmt')]
        if len(members) != 1 or not members[0].isfile() or members[0].size > 24 * 1024 * 1024:
            raise ValueError('One bounded real formatter executable required')
        with source.extractfile(members[0]) as stream, destination.open('xb') as output:
            while chunk := stream.read(262144):
                output.write(chunk)
    destination.chmod(0o755)
    environment = dict(os.environ)
    environment['LD_LIBRARY_PATH'] = str(rust / 'work/toolchains/rust/lib')
    result = subprocess.run([str(destination), '--version'], env=environment,
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=30)
    state.update(archive_bytes=archive.stat().st_size, archive_sha256=observed,
                 executable=str(destination), executable_bytes=destination.stat().st_size,
                 executable_sha256=hashlib.sha256(destination.read_bytes()).hexdigest(),
                 version_argv=[str(destination), '--version'], exit=result.returncode,
                 stdout=result.stdout.decode(errors='replace'), stderr=result.stderr.decode(errors='replace'),
                 library_directory=environment['LD_LIBRARY_PATH'],
                 status='PASS_ACQUISITION_ONLY' if result.returncode == 0 else 'FAILED_RETAINED')
    save()
    return result.returncode


if __name__ == '__main__':
    raise SystemExit(main())
