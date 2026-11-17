#!/usr/bin/env python3
"""Retain nonempty address-bounded assembly for exact runtime cohorts.

Use in_existing_cgroup.py. Symbols are resolved with llvm-nm before disassembly;
a successful llvm-objdump exit with no instructions is a failure. Normalization
preserves constants and resolves only annotated branches and RIP-relative
references. Matching normalized text is diagnostic evidence, not a semantic
equivalence or timing proof. Original disassembly and binary hashes are retained.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

GATE = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
INSTRUCTION = re.compile(r'^\s*([0-9a-fA-F]+):\s+(\S.*)$')
BRANCH = re.compile(r'^(j\w+|callq?|loop\w*)\s+(0x[0-9a-fA-F]+)(?:\s+<(.+)>)?$')
RIP = re.compile(r'-?0x[0-9a-fA-F]+\(%rip\)')
RESOLVED = re.compile(r'#\s+0x[0-9a-fA-F]+\s+<(.+)>$')


def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def instructions(text, start, size):
    if size <= 0:
        raise ValueError('A function must have a positive symbol size')
    rows = []
    for line in text.splitlines():
        matched = INSTRUCTION.match(line)
        if matched:
            address = int(matched[1], 16)
            if not start <= address < start + size:
                raise ValueError('Instruction lies outside the selected symbol')
            if rows and address <= rows[-1][0]:
                raise ValueError('Instruction addresses are not strictly increasing')
            if '<unknown>' in matched[2] or matched[2].startswith('.byte'):
                raise ValueError('Incomplete disassembly cannot qualify a comparison')
            rows.append((address, matched[2]))
    if not rows or rows[0][0] != start:
        raise ValueError('Empty disassembly or missing first instruction')
    return rows


def normalize(rows, start, size):
    result = []
    for address, original in rows:
        resolved = RESOLVED.search(original)
        # LLVM's x86 comments start after whitespace. A demangled C++ lambda
        # may contain '#1' inside the target symbol; that is not a comment.
        code = re.split(r'\s+#(?:\s|$)', original, maxsplit=1)[0].strip()
        if resolved and RIP.search(code):
            code = RIP.sub(lambda _: 'RIP<' + resolved[1] + '>', code)
        branch = BRANCH.fullmatch(code)
        if branch:
            target = int(branch[2], 16)
            if start <= target < start + size:
                code = branch[1] + ' LOCAL+' + hex(target - start)
            elif branch[3]:
                code = branch[1] + ' SYMBOL<' + branch[3] + '>'
        result.append({'offset': address - start, 'instruction': re.sub(r'\s+', ' ', code)})
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--before', type=Path, required=True)
    parser.add_argument('--after', type=Path, required=True)
    parser.add_argument('--toolchain', type=Path, required=True)
    parser.add_argument('--function', action='append', required=True, help='Demangled substring; every selector must match both binaries')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    inode = GATE.stat().st_ino
    def gate():
        assert Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GATE.relative_to('/sys/fs/cgroup'))
        assert GATE.stat().st_ino == inode
        assert (GATE/'memory.max').read_text().strip() == '68719476736'
        assert (GATE/'memory.swap.max').read_text().strip() == '0'
    gate()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    binaries = {'before': args.before.resolve(), 'after': args.after.resolve()}
    toolchain = args.toolchain.resolve()
    pins = {str(p): sha(p) for p in [*binaries.values(), Path(__file__).resolve(), *[toolchain/name for name in ('llvm-nm', 'llvm-cxxfilt', 'llvm-objdump')]]}
    report = {'complete': False, 'scope': __doc__, 'inputs': pins, 'cgroup': str(GATE),
              'memory_max': 68719476736, 'swap_max': 0, 'cgroup_inode': inode,
              'events_before': (GATE/'memory.events').read_text(),
              'selectors': args.function, 'variants': {}, 'comparisons': {}, 'commands': []}
    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2) + '\n')
    def run(argv, data=None):
        gate()
        process = subprocess.run([str(x) for x in argv], input=data, text=True, capture_output=True, timeout=180)
        report['commands'].append({'argv': [str(x) for x in argv], 'exit': process.returncode, 'stderr': process.stderr})
        save()
        if process.returncode:
            raise RuntimeError(process.stderr[-3000:])
        return process.stdout
    save()
    try:
        for label, binary in binaries.items():
            symbols = run([toolchain/'llvm-nm', '--defined-only', '--print-size', '--format=posix', binary])
            raw = [line.split() for line in symbols.splitlines() if len(line.split()) >= 4 and line.split()[1] in ('T', 't', 'W', 'w')]
            decoded = run([toolchain/'llvm-cxxfilt'], '\n'.join(row[0] for row in raw) + '\n').splitlines()
            assert len(raw) == len(decoded), 'Demangler result count mismatch'
            selected = [(row, name) for row, name in zip(raw, decoded) if any(selector in name for selector in args.function)]
            assert all(any(selector in name for _, name in selected) for selector in args.function), 'A required function selector matched nothing'
            functions = {}
            for index, (row, name) in enumerate(selected):
                start, size = int(row[2], 16), int(row[3], 16)
                output = run([toolchain/'llvm-objdump', '-d', '--start-address=' + hex(start), '--stop-address=' + hex(start + size), '--demangle', '--no-show-raw-insn', binary])
                path = out/(label + '-' + str(index) + '.asm')
                path.write_text(output)
                parsed = instructions(output, start, size)
                normalized = normalize(parsed, start, size)
                assert row[0] not in functions, 'Duplicate selected symbol'
                functions[row[0]] = {'name': name, 'start': start, 'size': size, 'instructions': len(parsed), 'assembly': str(path), 'assembly_sha256': sha(path), 'normalized': normalized}
            report['variants'][label] = functions
            save()
        before, after = report['variants']['before'], report['variants']['after']
        assert before and before.keys() == after.keys(), 'Selected function sets differ'
        for symbol, old in before.items():
            new = after[symbol]
            report['comparisons'][symbol] = {'name': old['name'], 'before_instructions': old['instructions'],
                'after_instructions': new['instructions'], 'same_normalized_instructions': old['normalized'] == new['normalized']}
        report['complete'] = True
    finally:
        gate()
        report['inputs_unchanged'] = all(sha(path) == digest for path, digest in pins.items())
        report['assembly_unchanged'] = all(sha(row['assembly']) == row['assembly_sha256']
                                           for variant in report['variants'].values() for row in variant.values())
        report['events_after'] = (GATE/'memory.events').read_text()
        report['complete'] = report['complete'] and report['inputs_unchanged'] and report['assembly_unchanged']
        save()
    assert report['complete']
    print(json.dumps({'complete': True, 'functions': len(report['comparisons']), 'changed': sum(not row['same_normalized_instructions'] for row in report['comparisons'].values())}))


if __name__ == '__main__':
    main()
