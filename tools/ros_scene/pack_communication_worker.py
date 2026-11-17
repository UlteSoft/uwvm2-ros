#!/usr/bin/env python3
"""Copy a finite, hashed communication worker image closure without executing ELF inputs.

DT_NEEDED/PT_INTERP are read with readelf. Lazy libraries and resources must be
explicitly selected or observed in a supplied successful-open strace. This is a
packaging receipt, not proof that ROS can run after the final seccomp boundary.
"""
import argparse
import ast
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import stat
import subprocess

ROOT = Path(__file__).resolve().parents[2]
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')

# Compiler ABIs, not vendor names or claims about installed ROS distributions.
# ARM EABI floating-point calling conventions are deliberately distinct.
LINUX_ABIS = {
    'x86_64': (2, 62, 'x86_64-linux-gnu', '/lib64/ld-linux-x86-64.so.2'),
    'i686': (1, 3, 'i386-linux-gnu', '/lib/ld-linux.so.2'),
    'aarch64': (2, 183, 'aarch64-linux-gnu', '/lib/ld-linux-aarch64.so.1'),
    'arm': (1, 40, 'arm-linux-gnueabi', '/lib/ld-linux.so.3'),
    'armhf': (1, 40, 'arm-linux-gnueabihf', '/lib/ld-linux-armhf.so.3'),
}


def inspect_elf(data, architecture):
    """Require the selected Linux ELF class, endianness and calling convention.

    No input code executes. ARM must carry EABI5 and exactly the selected
    floating-point PCS; a bare EM_ARM check cannot distinguish arm and armhf.
    Full dependency/program-header parsing is subsequently done by readelf.
    """
    elf_class, machine, _, _ = LINUX_ABIS[architecture]
    extent = 64 if elf_class == 2 else 52
    if len(data) < extent or data[:7] != b'\x7fELF'+bytes((elf_class, 1, 1)):
        raise ValueError('ELF class, endianness or header does not match '+architecture)
    if data[7] not in (0, 3) or data[8] != 0:
        raise ValueError('unsupported ELF OS ABI')
    if int.from_bytes(data[16:18], 'little') not in (2, 3) or int.from_bytes(data[18:20], 'little') != machine:
        raise ValueError('ELF machine/type does not match '+architecture)
    if int.from_bytes(data[20:24], 'little') != 1:
        raise ValueError('unsupported ELF version')
    header_size = int.from_bytes(data[52:54] if elf_class == 2 else data[40:42], 'little')
    if header_size != extent:
        raise ValueError('invalid ELF header size')
    flags = int.from_bytes(data[48:52] if elf_class == 2 else data[36:40], 'little')
    if architecture in ('arm', 'armhf'):
        expected = 0x200 if architecture == 'arm' else 0x400
        if flags >> 24 != 5 or flags & 0x600 != expected:
            raise ValueError('ARM EABI version or floating-point PCS mismatch')
    return dict(architecture=architecture, elf_class=elf_class, machine=machine, flags=flags)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def logical(value):
    path = PurePosixPath(value)
    if not value.startswith(('/worker/', '/lib/', '/lib64/', '/usr/lib/', '/opt/ros/')):
        raise ValueError('unsupported image destination: '+value)
    if len(value.encode()) > 255 or any(x in ('', '.', '..') for x in value[1:].split('/')):
        raise ValueError('invalid image destination: '+value)
    if not all(c.isascii() and (c.isalnum() or c in '/._+-') for c in value):
        raise ValueError('unsupported destination character: '+value)
    return str(path)


class Pack:
    def __init__(self, args):
        self.args = args
        self.architecture = getattr(args, 'architecture', 'x86_64')
        _, _, triple, self.interpreter = LINUX_ABIS[self.architecture]
        requested_root = getattr(args, 'system_root', None)
        if self.architecture != 'x86_64' and requested_root is None:
            raise ValueError('cross-architecture packaging requires an explicit target --system-root')
        self.system_root = Path(requested_root).resolve(strict=True) if requested_root else Path('/')
        if self.architecture != 'x86_64' and self.system_root == Path('/'):
            raise ValueError('cross-architecture packaging cannot fall back to host system libraries')
        self.ros_root = args.ros_root.resolve(strict=True)
        self.ros = self.ros_root/'opt/ros/jazzy'
        self.sysroot = self.ros_root/'system-libs'
        if not self.ros.is_dir() or not self.sysroot.is_dir():
            raise ValueError('expected a pinned Jazzy ROS root and system-libs directory')
        self.system_destinations = [Path('/lib')/triple, Path('/usr/lib')/triple]
        self.system_destinations += [Path('/lib64')] if self.architecture == 'x86_64' else [Path('/lib'), Path('/usr/lib')]
        self.system = [self.system_root/str(p).lstrip('/') for p in self.system_destinations]
        self.search = [self.ros/'lib', self.sysroot, *self.system]
        self.approved = [self.ros.resolve(), self.sysroot.resolve(), *(p.resolve() for p in self.system if p.is_dir())]
        if self.system_root != Path('/') and any(not p.is_relative_to(self.system_root) for p in self.approved[2:]):
            raise ValueError('target system library root resolves outside the selected sysroot')
        self.files = {}
        self.queue = []
        self.image_bytes = 0
        self.readelf = []
        self.trace = []
        self.ignored = {}
        self.machine = None
        self.runtime_basenames = {}

    def read(self, source):
        # The source is trusted host selection. Resolve a requested library
        # symlink, then pin an O_NOFOLLOW descriptor and copy its exact bytes.
        resolved = Path(source).resolve(strict=True)
        fd = os.open(resolved, os.O_RDONLY | os.O_NOFOLLOW | os.O_CLOEXEC)
        try:
            before = os.fstat(fd)
            if not stat.S_ISREG(before.st_mode) or before.st_size > self.args.max_image_bytes:
                raise ValueError('not a bounded regular image: '+str(source))
            chunks = []
            remaining = before.st_size
            while remaining:
                block = os.read(fd, min(remaining, 1024*1024))
                if not block:
                    raise ValueError('source truncated during copy: '+str(source))
                chunks.append(block)
                remaining -= len(block)
            after = os.fstat(fd)
            if os.read(fd, 1) or (before.st_dev, before.st_ino, before.st_size, before.st_mtime_ns, before.st_ctime_ns) != (
                    after.st_dev, after.st_ino, after.st_size, after.st_mtime_ns, after.st_ctime_ns):
                raise ValueError('source changed during copy: '+str(source))
            return resolved, b''.join(chunks)
        finally:
            os.close(fd)

    def approved_library(self, path):
        resolved = path.resolve(strict=True)
        if not any(resolved.is_relative_to(root) for root in self.approved):
            raise ValueError('library symlink escapes approved roots: '+str(path))
        return path

    def library(self, name):
        if '/' in name or name in ('', '.', '..'):
            raise ValueError('DT_NEEDED/lazy library must be an exact basename: '+name)
        for directory in self.search:
            candidate = directory/name
            if candidate.is_file():
                return self.approved_library(candidate)
        raise ValueError('unresolved approved library: '+name)

    def library_destination(self, path):
        # Preserve the requested soname, including a resolved symlink's alias.
        path = Path(path).absolute()
        if path.is_relative_to(self.ros):
            return '/opt/ros/jazzy/'+str(path.relative_to(self.ros))
        if path.is_relative_to(self.sysroot):
            return '/usr/lib/uwvm-ros/'+str(path.relative_to(self.sysroot))
        for directory, destination in zip(self.system, self.system_destinations):
            if path.is_relative_to(directory):
                return str(destination/path.relative_to(directory))
        raise ValueError('cannot map library path: '+str(path))

    def add(self, source, destination, role, reason):
        destination = logical(destination)
        resolved, data = self.read(source)
        digest = sha(data)
        if role != 'configuration':
            abi = inspect_elf(data, self.architecture)
            machine = (abi['elf_class'], abi['machine'], abi['architecture'])
            if self.machine is None:
                self.machine = machine
            if machine != self.machine:
                raise ValueError('mixed ELF architecture')
            name = PurePosixPath(destination).name
            if name in self.runtime_basenames and self.runtime_basenames[name] != digest:
                raise ValueError('ambiguous runtime basename with different content: '+name)
            self.runtime_basenames[name] = digest
        if destination in self.files:
            previous = self.files[destination]
            if previous['sha256'] != digest or previous['role'] != role:
                raise ValueError('conflicting destination: '+destination)
            previous['reasons'].append(reason)
            return
        for other in self.files:
            if destination.startswith(other+'/') or other.startswith(destination+'/'):
                raise ValueError('file/directory image collision')
        if len(self.files) >= 256 or self.image_bytes+2*len(data) > self.args.max_image_bytes:
            raise ValueError('finite image reservation exceeded')
        self.image_bytes += 2*len(data)
        target = self.args.output/'images'/(role+'-'+digest)
        if target.exists():
            if sha(target.read_bytes()) != digest:
                raise ValueError('existing image digest mismatch')
        else:
            target.write_bytes(data)
        target.chmod(0o400 if role == 'configuration' else 0o500)
        record = dict(role=role, destination=destination, path=str(target), bytes=len(data), sha256=digest,
            source=str(Path(source).absolute()), resolved_source=str(resolved), reasons=[reason])
        if role != 'configuration':
            record['abi'] = abi
        self.files[destination] = record
        if role != 'configuration':
            self.queue.append(record)

    def elf_closure(self):
        while self.queue:
            record = self.queue.pop(0)
            done = subprocess.run(['/usr/bin/readelf', '-W', '-d', '-l', '--', record['path']],
                capture_output=True, text=True, check=True, timeout=20)
            self.readelf.append(dict(destination=record['destination'], output_sha256=sha(done.stdout.encode())))
            needed = re.findall(r'\(NEEDED\).*Shared library: \[([^\]]+)\]', done.stdout)
            record['needed'] = needed
            for name in needed:
                source = self.library(name)
                self.add(source, self.library_destination(source), 'runtime', 'DT_NEEDED by '+record['destination'])
            interpreters = re.findall(r'Requesting program interpreter: ([^\]]+)\]', done.stdout)
            if len(interpreters) > 1:
                raise ValueError('multiple program interpreters')
            for interpreter in interpreters:
                if interpreter != self.interpreter:
                    raise ValueError('unsupported program interpreter: '+interpreter)
                source = self.system_root/interpreter.lstrip('/')
                self.add(self.approved_library(source), interpreter, 'runtime', 'PT_INTERP by '+record['destination'])

    def resource(self, relative, reason):
        path = PurePosixPath(relative)
        if path.is_absolute() or any(x in ('', '.', '..') for x in relative.split('/')):
            raise ValueError('resource must be an exact ROS-relative file')
        source = self.ros/str(path)
        if not source.resolve(strict=True).is_relative_to(self.ros):
            raise ValueError('resource escapes selected ROS root')
        self.add(source, '/opt/ros/jazzy/'+str(path), 'configuration', reason)

    def ingest_trace(self, trace):
        raw = trace.read_bytes()
        # Only successful open/openat calls with complete quoted paths count.
        # Unknown successful path accesses are errors, not silently omitted.
        pattern = re.compile(r'\b(?:openat\([^,]+, |open\()((?:"(?:[^"\\]|\\.)*")), [^\n]*\)\s+=\s+(\d+)(?:<[^>]*>)?\s*$')
        observed = set()
        pending = {}
        for line in raw.decode('utf-8').splitlines():
            prefix = re.match(r'^(?:\[pid\s+(\d+)\]\s+|(\d+)\s+)?(.*)$', line)
            pid = prefix[1] or prefix[2] or 'single'
            line = prefix[3]
            if '<unfinished ...>' in line and re.match(r'(?:open|openat)\(', line):
                if pid in pending:
                    raise ValueError('nested incomplete open in trace')
                pending[pid] = line.split('<unfinished ...>', 1)[0]
                continue
            resumed = re.match(r'<\.\.\. (?:open|openat) resumed>(.*)', line)
            if resumed:
                if pid not in pending:
                    raise ValueError('open resumed without its initial trace line')
                line = pending.pop(pid)+resumed[1]
            match = pattern.search(line)
            if match:
                observed.add(ast.literal_eval(match[1]))
            elif re.search(r'\b(?:open|openat)\(', line) and re.search(r'\)\s+=\s+\d+', line):
                raise ValueError('unparsed successful file open; collect complete strace lines: '+line)
        if pending or not observed:
            raise ValueError('trace has incomplete opens or no successful opens')
        excluded = {'/etc/ld.so.cache': 'dynamic cache excluded: namespace uses explicit LD_LIBRARY_PATH'}
        excluded.update(dict(self.args.transient_probe))
        unknown = []
        for name in sorted(observed):
            if name in excluded:
                self.ignored[name] = excluded[name]
                continue
            path = Path(name)
            if not path.is_absolute():
                unknown.append(name)
                continue
            if path.is_relative_to(self.ros) and path.is_file():
                if path.read_bytes()[:4] == b'\x7fELF':
                    self.add(self.approved_library(path), self.library_destination(path), 'runtime', 'successful traced lazy open')
                else:
                    self.resource(str(path.relative_to(self.ros)), 'successful traced resource open')
            elif path.is_file() and any(path.is_relative_to(root) for root in self.search):
                self.add(self.approved_library(path), self.library_destination(path), 'runtime', 'successful traced system ELF open')
            elif name in {str(Path(self.args.bootstrap).absolute()), str(Path(self.args.backend).absolute()), str(Path(self.args.profile).absolute())}:
                continue
            else:
                unknown.append(name)
        if unknown:
            raise ValueError('successful opens outside selected closure require explicit transient-probe review: '+repr(unknown))
        self.trace.append(dict(path=str(trace.resolve()), sha256=sha(raw), successful_paths=sorted(observed)))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for flag in ('bootstrap', 'backend', 'profile', 'ros-root', 'output'):
        parser.add_argument('--'+flag, type=Path, required=True)
    parser.add_argument('--launcher', type=Path, default=Path('/usr/bin/bwrap'))
    parser.add_argument('--architecture', choices=tuple(LINUX_ABIS), default='x86_64',
                        help='exact Linux compiler ABI for every executable/library in the closure')
    parser.add_argument('--system-root', type=Path,
                        help='target filesystem root; mandatory for non-x86_64 and never substituted with host libraries')
    parser.add_argument('--dlopen', action='append', default=[], help='exact additional approved library basename')
    parser.add_argument('--resource', action='append', default=[], help='exact ROS-prefix-relative NX data file')
    parser.add_argument('--trace', type=Path, action='append', default=[], help='strace -f -qq -s 4096 -e trace=open,openat')
    parser.add_argument('--transient-probe', nargs=2, action='append', default=[], metavar=('PATH', 'REASON'))
    parser.add_argument('--max-image-bytes', type=int, default=256*1024*1024)
    args = parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip() == '68719476736' and (GROUP/'memory.swap.max').read_text().strip() == '0'
    if not 1 <= args.max_image_bytes <= 1024*1024*1024:
        raise ValueError('image budget must be finite and at most 1 GiB for this packer')
    if len({path for path, _ in args.transient_probe}) != len(args.transient_probe) or any(
            not Path(path).is_absolute() or not reason.strip() for path, reason in args.transient_probe):
        raise ValueError('transient probe dispositions require unique absolute paths and nonempty reasons')
    args.output = args.output.resolve()
    assert args.output.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    args.output.mkdir(parents=True, exist_ok=False)
    (args.output/'images').mkdir()
    receipt = dict(complete=False, qualified_runtime=False, kind='communication_sealed_image_pack',
        cgroup=str(GROUP), memory_max=68719476736, swap_max=0, events_before=(GROUP/'memory.events').read_text(),
        helper_sha256=sha(Path(__file__).read_bytes()), readelf_sha256=sha(Path('/usr/bin/readelf').read_bytes()),
        limitations=['packaging alone does not prove late dlopen/thread closure',
            'transient probes require validation against the private namespace',
            'ELF closure validation does not qualify native ROS or kernel isolation on the selected target'],
        architecture=args.architecture)
    pack = None
    try:
        pack = Pack(args)
        launcher, data = pack.read(args.launcher)
        launcher_abi = inspect_elf(data, args.architecture)
        target = args.output/'images'/('launcher-'+sha(data))
        target.write_bytes(data)
        target.chmod(0o500)
        receipt['launcher'] = dict(path=str(target), source=str(launcher), bytes=len(data), sha256=sha(data), abi=launcher_abi)
        pack.image_bytes = len(data)
        pack.add(args.bootstrap, '/worker/bootstrap', 'bootstrap', 'explicit startup executable')
        pack.add(args.backend, '/worker/backend.so', 'runtime', 'explicit native backend')
        pack.add(args.profile, '/worker/profile', 'configuration', 'explicit immutable single-node profile')
        for name in args.dlopen:
            path = pack.library(name)
            pack.add(path, pack.library_destination(path), 'runtime', 'explicit lazy library')
        for path in args.resource:
            pack.resource(path, 'explicit startup resource')
        for path in args.trace:
            pack.ingest_trace(path)
        pack.elf_closure()
        receipt.update(complete=True, files=list(pack.files.values()), required_image_bytes=pack.image_bytes,
            ros_prefix='/opt/ros/jazzy', profile_bytes=pack.files['/worker/profile']['bytes'],
            profile_sha256=pack.files['/worker/profile']['sha256'], readelf=pack.readelf,
            traces=pack.trace, transient_probes=pack.ignored)
    except Exception as error:
        receipt.update(error=str(error), files=list(pack.files.values()) if pack else [])
        raise
    finally:
        receipt['events_after'] = (GROUP/'memory.events').read_text()
        (args.output/'manifest.json').write_text(json.dumps(receipt, indent=2)+'\n')
    print(json.dumps(dict(complete=True, images=len(pack.files), reserved_bytes=pack.image_bytes,
        manifest=str(args.output/'manifest.json'))))


if __name__ == '__main__':
    main()
