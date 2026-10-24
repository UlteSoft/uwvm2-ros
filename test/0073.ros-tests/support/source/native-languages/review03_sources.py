#!/usr/bin/env python3
"""@brief Check and format captured native sources inside the finite Linux owner.

@details This driver works on a fresh private source capsule only. Its changed
file archive is a reviewable candidate, not an SDK publication. Real language
formatters are version-pinned; their second pass must be byte-idempotent. Source
generation and syntax checks establish no standalone build or Guest result.
@see sdk/origin/coordination/20261021-director/handoffs/03/review03/FORMAT-PLAN.md
"""

from pathlib import Path
import argparse
import hashlib
import importlib
import json
import os
import shutil
import subprocess
import sys
import tarfile


def digest(path):
    """@brief Hash an ordinary captured file without executing its contents.

    @param path Readable regular file under the active owner.
    @return Lowercase SHA-256 of its complete bytes; I/O errors propagate.
    @see main
    """
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    """@brief Generate documented candidates and run genuine native formatters.

    @details The source archive and every formatter receive explicit identities.
    Only current owned native sources/examples/test consumers are formatted.
    Historical evidence, upstream definitions, SDK sysroots and other owners'
    live sources are never modified. Every subprocess is synchronous and bounded
    by this driver and by its enclosing shared owner.
    @return Zero after source syntax, generation, configuration and selected
    formatter idempotence pass; qualification gaps remain in the receipt.
    @see digest; sdk/origin/tools/native_languages/documentation.py
    """
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--source-sha256', required=True)
    args = parser.parse_args()
    root = Path.cwd()
    if digest(args.source) != args.source_sha256:
        raise ValueError('Captured source capsule differs')
    source = root / 'source'
    source.mkdir()
    with tarfile.open(args.source) as archive:
        members = archive.getmembers()
        if sum(item.size for item in members) > 12 * 1024 * 1024:
            raise ValueError('Finite source capsule exceeded')
        for item in members:
            path = Path(item.name)
            if path.is_absolute() or '..' in path.parts or not (item.isfile() or item.isdir()):
                raise ValueError('Ordinary confined source capsule required')
        archive.extractall(source, filter='data')
    before = {str(path.relative_to(source)): digest(path)
              for path in source.rglob('*') if path.is_file()}
    rows = []
    report = {'kind': 'task03-review03-source-format-v1', 'status': 'RUNNING',
              'source_sha256': args.source_sha256, 'rows': rows,
              'documentation_acceptance': 'INCOMPLETE_PUBLIC_API_INVENTORY_REQUIRED',
              'compiler_runs': 0, 'Guest_runs': 0, 'default_authority': [],
              'formatter_gaps': ['Swift', 'Haskell', 'D', 'Odin', 'C', 'Python', 'Lua']}

    def save():
        (root / 'source-format.json').write_text(json.dumps(report, indent=2) + '\n')

    def run(name, command, environment=None, timeout=90):
        log = root / (name + '.log')
        row = {'name': name, 'argv': [str(item) for item in command], 'log': log.name}
        rows.append(row)
        save()
        with log.open('wb') as output:
            result = subprocess.run(row['argv'], cwd=source,
                                    env=environment or os.environ,
                                    stdout=output, stderr=subprocess.STDOUT,
                                    timeout=timeout)
        row.update(exit=result.returncode, status='PASS' if result.returncode == 0 else 'FAIL')
        save()
        if result.returncode:
            raise ValueError('Retained command failure: ' + name)
        return log.read_text(errors='replace')

    helper = source / 'sdk/origin/tools/native_languages'
    for path in sorted(helper.glob('*.py')):
        compile(path.read_bytes(), str(path), 'exec', dont_inherit=True)
        rows.append({'name': str(path.relative_to(source)), 'layer': 'Python_syntax', 'status': 'PASS'})
    save()
    sys.path[:0] = [str(helper), str(source / 'sdk/origin/tools')]
    import numeric
    schema_path = source / 'sdk/origin/abi/abi-v1.json'
    schema = json.loads(schema_path.read_bytes())
    products = numeric.products(schema_path.read_bytes())
    origin = source / 'sdk/origin'
    for relative, body in products.items():
        if relative == 'numeric-inventory.json':
            continue
        target = origin / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(body)
    raw_out = root / 'raw-products'
    candidate = source / 'sdk/origin/coordination/20261021-director/handoffs/03/review03/candidates/generate_portable_numeric_bindings.py'
    run('portable-numeric-generation', [sys.executable, '-I', '-S', '-B',
        '-c', 'import runpy,sys;sys.path.insert(0,sys.argv.pop(1));runpy.run_path(sys.argv.pop(1),run_name="__main__")',
        source / 'sdk/origin/tools', candidate, '--schema', schema_path, '--out', raw_out])
    for relative in ('go/raw_ros1.go', 'go/raw_ros2.go', 'zig/ros1.zig', 'zig/ros2.zig'):
        shutil.copy2(raw_out / relative, origin / relative)
    from documentation import raw_function
    for backend in (1, 2):
        path = origin / ('rust/src/raw/ros' + str(backend) + '.rs')
        lines = path.read_text().splitlines()
        declarations = {row['name']: row for row in
                        schema['modules']['uwvm_ros' + str(backend) + '_v1']['functions']}
        output = []
        documented = []
        for line in lines:
            name = next((name for name in declarations
                         if line.lstrip().startswith('pub fn ' + name + '(')), None)
            if name is not None:
                output += raw_function('rust', declarations[name], name, backend, '    ')
                documented.append(name)
            output.append(line)
        if set(documented) != set(declarations) or len(documented) != len(declarations):
            raise ValueError('Complete exact Rust raw declaration coverage required')
        path.write_text('\n'.join(output) + '\n')
    tests = source / 'test/0073.ros-tests/support/source/sdk-native-languages'
    environment = dict(os.environ, UWVM_NATIVE_LANGUAGES_TOOLS=str(helper))
    for name in ('test_profile.py', 'test_arguments.py'):
        run(name, [sys.executable, '-I', '-S', '-B', tests / 'configuration' / name], environment)
    run('complete-record-generation', [sys.executable, '-I', '-S', '-B', helper / 'emit.py',
        '--origin', origin, '--catalog', tests / 'fixtures/catalog.json', '--guest-only', '--out', root / 'records'])
    base = Path('/home/macromodel/Documents/uwvm-test-261018/agents/03')
    tool_base = base / '20261021-toolchains-r1/work/toolchains'
    rustfmt = base / '20261021-round03-rustfmt-r1/work/tools/bin/rustfmt'
    zig_candidates = sorted((tool_base / 'zig-input').glob('*/zig'))
    if len(zig_candidates) != 1:
        raise ValueError('Exact acquired Zig compiler/formatter required')
    formatters = {
        'rust': (rustfmt, '.rs'),
        'go': (tool_base / 'go-input/go/bin/gofmt', '.go'),
        'zig': (zig_candidates[0], '.zig'),
    }
    formatter_environment = dict(os.environ)
    formatter_environment['LD_LIBRARY_PATH'] = str(tool_base / 'rust/lib')
    report['formatters'] = {}
    for language, (program, suffix) in formatters.items():
        version_command = ([program, '--version'] if language == 'rust' else
                           [program.parent / 'go', 'version'] if language == 'go' else [program, 'version'])
        version = run(language + '-formatter-version', version_command, formatter_environment).strip()
        if language == 'rust' and not version.startswith('rustfmt 1.8.0-stable'):
            raise ValueError('Matching Rust1.85 formatter required')
        if language == 'go' and version != 'go version go1.24.2 linux/amd64':
            raise ValueError('Matching Go1.24.2 formatter required')
        if language == 'zig' and version != '0.14.1':
            raise ValueError('Matching Zig0.14.1 formatter required')
        files = sorted(path for directory in (origin, tests, root / 'records')
                       for path in directory.rglob('*' + suffix)
                       if path.is_file() and not path.name.startswith('._'))
        report['formatters'][language] = {'program': str(program), 'sha256': digest(program),
                                         'version': version, 'inputs': [str(path) for path in files]}
        def command(paths):
            if language == 'rust':
                return [program, '--edition', '2021', '--config', 'skip_children=true', *paths]
            if language == 'go':
                return [program, '-w', *paths]
            return [program, 'fmt', *paths]
        run(language + '-format-first', command(files), formatter_environment, timeout=120)
        first = {str(path): digest(path) for path in files}
        run(language + '-format-second', command(files), formatter_environment, timeout=120)
        if any(digest(Path(path)) != pin for path, pin in first.items()):
            raise ValueError('Formatter second pass is not byte-idempotent: ' + language)
        report['formatters'][language]['idempotent'] = True
        report['formatters'][language]['after'] = first
        save()
    nimpretty = base / '20261021-additional-tools-r2/work/toolchains/nim/nim-2.2.4/bin/nimpretty'
    report['nimpretty_help'] = run('nimpretty-help', [nimpretty, '--help'])[:8000]
    report['formatter_gaps'].append('Nim: exact invocation reviewed after retained help')
    generation = json.loads((raw_out / 'generation.json').read_bytes())
    generation.update(documentation_helper_sha256=digest(helper / 'documentation.py'),
                      producer_candidate_sha256=digest(candidate),
                      formatting='gofmt1.24.2 / zig fmt0.14.1; byte-idempotent before these final pins')
    for row in generation['products']:
        path = origin / row['path']
        row.update(bytes=path.stat().st_size, sha256=digest(path))
    (origin / 'go/generation.numeric.json').write_text(json.dumps(generation, indent=2) + '\n')
    owned = ('sdk/origin/rust/', 'sdk/origin/go/', 'sdk/origin/tinygo/', 'sdk/origin/zig/',
             'sdk/origin/swift/', 'sdk/origin/haskell/', 'sdk/origin/nim/', 'sdk/origin/d/',
             'sdk/origin/odin/', 'sdk/origin/examples/rust/', 'sdk/origin/examples/go/',
             'sdk/origin/examples/tinygo/', 'sdk/origin/examples/zig/',
             'test/0073.ros-tests/support/source/sdk-native-languages/')
    changes = []
    with tarfile.open(root / 'formatted-candidates.tar', 'w') as archive:
        for path in sorted(source.rglob('*')):
            if not path.is_file():
                continue
            relative = str(path.relative_to(source))
            after = digest(path)
            if relative.startswith(owned) and before.get(relative) != after:
                archive.add(path, arcname=relative, recursive=False)
                changes.append({'path': relative, 'before_sha256': before.get(relative),
                                'after_sha256': after, 'bytes': path.stat().st_size})
    report.update(status='PASS_SOURCE_GENERATION_AND_THREE_FORMATTERS', changes=changes,
                  candidate_archive_sha256=digest(root / 'formatted-candidates.tar'),
                  candidate_archive_bytes=(root / 'formatted-candidates.tar').stat().st_size)
    save()
    print(report['status'], 'changed_files=' + str(len(changes)), flush=True)
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
