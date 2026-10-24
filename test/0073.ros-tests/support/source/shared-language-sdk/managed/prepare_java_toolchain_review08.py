#!/usr/bin/env python3
"""Acquire and compile the genuine isolated four-patch TeaVM toolchain.

This is a test/build prerequisite producer, not an SDK or application model.
All downloads, real Maven resolution, javac compilation and JAR packaging run
inside the original finite Linux owner. No application or Guest is executed.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tarfile
import urllib.request
import zipfile


PATCHES = {
    'teavm-0.13.1-wasm-dwarf.patch': '49a0283c37386a92384e5968929a05c85b5f61dae17b31a6758bca902a49906a',
    'teavm-0.13.1-ordered-names.patch': 'f185c931ff1a98be4ef8ec16e6bf9e003c1c426cd9dc78e8800ba60c2a80b5e0',
    'teavm-0.13.1-typed-locals.patch': 'b6ba459fbd3e52c3ccc2665e8b1cc275901d7d97144c880858d57ed1ae4f5b5b',
    'teavm-0.13.1-wasi-environ.patch': 'c45c8bc0a32e717294bf61f74d39024bc2d3d009d0cba9a89fc63d1783fb579f',
}
MAVEN_SHA512 = 'bcfe4fe305c962ace56ac7b5fc7a08b87d5abd8b7e89027ab251069faebee516b0ded8961445d6d91ec1985dfe30f8153268843c89aa392733d1a3ec956c9978'
CORE_SHA = 'b9fe1b008512c8f11ff3efbd9cf2f0565b4bfa76943a9f23910befc0812f7ebf'
SOURCE_SHA = '710b67c644e1e7a847f9bf692acc2d9e967783b61bbcc0fe2169840bf89940f4'
CENTRAL = 'https://repo.maven.apache.org/maven2/'
CORE = Path('org/teavm/teavm-core/0.13.1')
JAR, SOURCES = 'teavm-core-0.13.1.jar', 'teavm-core-0.13.1-sources.jar'
FILES = ('org/teavm/backend/wasm/render/WasmBinaryRenderer.java',
         'org/teavm/backend/wasm/render/WasmBinaryRenderingVisitor.java',
         'org/teavm/backend/wasm/generate/DwarfFunctionGenerator.java',
         'org/teavm/backend/wasm/runtime/WasiSupport.java',
         'org/teavm/backend/wasm/transformation/WasiSupportClassTransformer.java')


def digest(path, algorithm='sha256'):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, algorithm).hexdigest()


def pin(path):
    path = path.resolve(strict=True)
    return dict(path=str(path), bytes=path.stat().st_size, sha256=digest(path))


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ('patches', 'java-home', 'validator', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--prerequisite', type=Path,
                        help='Explicit read-only failed-r1 Maven closure; acquire the complete genuine JDK')
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise ValueError('the original admitted Linux owner/common lock is required')
    if any(not path.is_absolute() for path in (args.patches, args.java_home, args.validator, args.out)) or args.out.exists():
        raise ValueError('absolute immutable inputs and fresh owned output required')
    for name, expected in PATCHES.items():
        if digest(args.patches / name) != expected:
            raise ValueError('exact preserved compiler patch differs: ' + name)
    args.out.mkdir(parents=True)
    logs = args.out / 'commands'
    logs.mkdir()
    report = dict(passed=False, commands=[], downloads=[], patch_sha256=PATCHES,
        ordered_name_maps=True, typed_local_values=True, wasi_environ_mapping=True,
        wasi_monotonic_mapping=False, wasi_stdin_mapping=False,
        scope='Actual upstream source patch compilation and compiler JAR closure only',
        application_products=0, Guest_runs=0, native_ROS_runs=0, default_authority=[])
    receipt = args.out / 'toolchain-receipt.json'

    def save():
        receipt.write_text(json.dumps(report, indent=2) + '\n')

    def retrieve(url, path, maximum, expected, algorithm='sha256'):
        total = 0
        with urllib.request.urlopen(url, timeout=45) as source, path.open('xb') as output:
            while block := source.read(1 << 20):
                total += len(block)
                if total > maximum:
                    raise ValueError('bounded official download exceeded')
                output.write(block)
        if digest(path, algorithm) != expected:
            raise ValueError('official download identity differs: ' + url)
        report['downloads'].append(dict(url=url, **pin(path)))
        save()

    env = dict(os.environ, JAVA_HOME=str(args.java_home),
               MAVEN_OPTS='-Xms32m -Xmx512m -XX:ActiveProcessorCount=1 -XX:-UsePerfData')

    def run(label, argv, cwd=args.out):
        with (logs / (label + '.stdout')).open('xb') as stdout, (logs / (label + '.stderr')).open('xb') as stderr:
            result = subprocess.run(list(map(str, argv)), cwd=cwd, env=env,
                stdin=subprocess.DEVNULL, stdout=stdout, stderr=stderr, check=False)
        report['commands'].append(dict(label=label, argv=list(map(str, argv)), cwd=str(cwd), exit=result.returncode))
        save()
        if result.returncode:
            raise RuntimeError('actual toolchain command failed: ' + label)

    try:
        if args.prerequisite is not None:
            previous = args.prerequisite.resolve(strict=True)
            previous_receipt = previous / 'toolchain-receipt.json'
            if digest(previous_receipt) != '868c660d582bd143e9e31cf3035de90a83e465571d6b12abc02f94b3d546aa5c':
                raise ValueError('the exact retained failed prerequisite receipt is required')
            report['previous_failed_prerequisite'] = pin(previous_receipt)
            report['previous_failure_preserved'] = True
            jdk_url = ('https://github.com/adoptium/temurin21-binaries/releases/download/jdk-21.0.10%2B7/'
                       'OpenJDK21U-jdk_x64_linux_hotspot_21.0.10_7.tar.gz')
            expected_jdk = 'ea3b9bd464d6dd253e9a7accf59f7ccd2a36e4aa69640b7251e3370caef896a4'
            with urllib.request.urlopen(jdk_url + '.sha256.txt', timeout=45) as source:
                checksum = source.read(4097)
            if len(checksum) > 4096 or checksum.decode('ascii').split()[0] != expected_jdk:
                raise ValueError('official complete JDK checksum differs')
            (args.out / 'official-jdk.sha256.txt').write_bytes(checksum)
            archive = args.out / 'complete-jdk.tar.gz'
            retrieve(jdk_url, archive, 256 << 20, expected_jdk)
            with tarfile.open(archive, 'r:gz') as source:
                members = source.getmembers()
                if len(members) > 20000 or sum(row.size for row in members) > 768 << 20:
                    raise ValueError('bounded complete genuine JDK archive required')
                for row in members:
                    path = Path(row.name)
                    if path.is_absolute() or '..' in path.parts or path.parts[0] != 'jdk-21.0.10+7':
                        raise ValueError('confined exact JDK root required')
                    source.extract(row, args.out, filter='data')
            if args.java_home != args.out / 'jdk-21.0.10+7' or not (args.java_home / 'lib/ct.sym').is_file():
                raise ValueError('the complete selected JDK with its real release API symbols is required')
            maven_root = previous / 'apache-maven-3.9.11'
            shutil.copytree(previous / 'm2', args.out / 'm2')
        else:
            archive = args.out / 'apache-maven-3.9.11-bin.tar.gz'
            retrieve('https://archive.apache.org/dist/maven/maven-3/3.9.11/binaries/' + archive.name,
                     archive, 16 << 20, MAVEN_SHA512, 'sha512')
            with tarfile.open(archive, 'r:gz') as source:
                members = source.getmembers()
                if len(members) > 1024 or sum(row.size for row in members) > 64 << 20:
                    raise ValueError('bounded genuine Maven archive required')
                for row in members:
                    path = Path(row.name)
                    if path.is_absolute() or '..' in path.parts or not (row.isfile() or row.isdir()):
                        raise ValueError('ordinary confined Maven members required')
                    source.extract(row, args.out, filter='data')
            maven_root = args.out / 'apache-maven-3.9.11'
        maven = maven_root / 'bin/mvn'
        m2 = args.out / 'm2'
        if args.prerequisite is None:
            m2.mkdir()
        settings = args.out / 'settings.xml'
        settings.write_text('<settings xmlns="http://maven.apache.org/SETTINGS/1.2.0"><mirrors><mirror>'
            '<id>exact-central</id><mirrorOf>*</mirrorOf><url>' + CENTRAL + '</url></mirror></mirrors></settings>\n')
        pom = args.out / 'pom.xml'
        pom.write_text('<project xmlns="http://maven.apache.org/POM/4.0.0"><modelVersion>4.0.0</modelVersion>'
            '<groupId>org.uwvm.prerequisite</groupId><artifactId>teavm-offline-inputs</artifactId><version>1</version>'
            '<dependencies>' + ''.join('<dependency><groupId>org.teavm</groupId><artifactId>' + name +
                '</artifactId><version>0.13.1</version></dependency>' for name in
                ('teavm-core', 'teavm-interop', 'teavm-classlib')) + '</dependencies><build><plugins><plugin>'
            '<groupId>org.teavm</groupId><artifactId>teavm-maven-plugin</artifactId><version>0.13.1</version>'
            '</plugin></plugins></build></project>\n')
        run('java-version', [args.java_home / 'bin/java', '-version'])
        run('maven-version', [maven, '--version'])
        if args.prerequisite is None:
            run('real-offline-dependency-resolution', [maven, '-B', '-ntp', '-s', settings,
                '-Dmaven.repo.local=' + str(m2), '-f', pom,
                'org.apache.maven.plugins:maven-dependency-plugin:3.8.1:go-offline'])
        jar, sources = m2 / CORE / JAR, m2 / CORE / SOURCES
        if digest(jar) != CORE_SHA:
            raise ValueError('real resolved upstream TeaVM core differs')
        if sources.exists():
            if digest(sources) != SOURCE_SHA:
                raise ValueError('the retained genuine compiler source JAR differs')
        else:
            retrieve(CENTRAL + (CORE / SOURCES).as_posix(), sources, 32 << 20, SOURCE_SHA)
        upstream = args.out / 'upstream'
        upstream.mkdir()
        shutil.copyfile(jar, upstream / JAR)
        shutil.copyfile(sources, upstream / SOURCES)
        report.update(official_jar=dict(url=CENTRAL + (CORE / JAR).as_posix(), **pin(upstream / JAR)),
                      official_sources=dict(url=CENTRAL + (CORE / SOURCES).as_posix(), **pin(upstream / SOURCES)))
        source_root = args.out / 'source'
        source_root.mkdir()
        with zipfile.ZipFile(sources) as source:
            for name in FILES:
                destination = source_root / name
                destination.parent.mkdir(parents=True, exist_ok=True)
                destination.write_bytes(source.read(name))
        report['source_before'] = {name: digest(source_root / name) for name in FILES}
        for index, name in enumerate(PATCHES):
            run('patch-' + str(index), ['/usr/bin/patch', '--batch', '--forward', '--fuzz=0',
                '-p1', '-i', args.patches / name], source_root)
        report['source_after'] = {name: digest(source_root / name) for name in FILES}
        classes = args.out / 'classes'
        classes.mkdir()
        classpath = ':'.join(map(str, [jar, *sorted(path for path in m2.rglob('*.jar')
            if path != jar and not path.name.endswith('-sources.jar'))]))
        run('real-patched-compiler-javac', [args.java_home / 'bin/java', '-Xmx512m',
            '-XX:ActiveProcessorCount=1', '-XX:-UsePerfData', '-m', 'jdk.compiler/com.sun.tools.javac.Main',
            '--release', '17', '-g', '-proc:none', '-cp', classpath, '-d', classes,
            *[source_root / name for name in FILES]])
        replacements = {path.relative_to(classes).as_posix(): path.read_bytes() for path in classes.rglob('*.class')}
        if not replacements or not all(name.startswith('org/teavm/backend/wasm/') for name in replacements):
            raise ValueError('actual complete patched compiler class output required')

        def repackage(original, output, changed):
            with zipfile.ZipFile(original) as source, zipfile.ZipFile(output, 'x', zipfile.ZIP_DEFLATED) as target:
                names = set()
                for row in source.infolist():
                    if row.filename in names:
                        raise ValueError('duplicate upstream JAR entry')
                    names.add(row.filename)
                    target.writestr(row, changed.get(row.filename, source.read(row)))
                for name in sorted(set(changed) - names):
                    target.writestr(name, changed[name])

        patched_jar, patched_sources = args.out / 'patched-core.jar', args.out / 'patched-sources.jar'
        repackage(upstream / JAR, patched_jar, replacements)
        repackage(upstream / SOURCES, patched_sources, {name: (source_root / name).read_bytes() for name in FILES})
        shutil.copyfile(patched_jar, jar)
        shutil.copyfile(patched_sources, sources)
        for artifact in (jar, sources):
            for algorithm in ('sha1', 'md5'):
                checksum = artifact.with_name(artifact.name + '.' + algorithm)
                if checksum.exists():
                    checksum.write_text(digest(artifact, algorithm) + '\n')
        report.update(passed=True, patched_jar_sha256=digest(jar),
            replaced_classes={name: hashlib.sha256(raw).hexdigest() for name, raw in replacements.items()},
            java_home=str(args.java_home), maven=pin(maven), repository=str(m2))
        save()
        files = {}
        roots = [args.java_home, maven_root, m2]
        for root in roots:
            for path in root.rglob('*'):
                if path.is_file():
                    row = pin(path)
                    previous = files.setdefault(row['path'], row)
                    if previous != row:
                        raise ValueError('genuine tool input changed during inventory')
        for path in (receipt, args.validator):
            row = pin(path)
            files[row['path']] = row
        runtime = dict(name='TeaVM', version='0.13.1', route='java-teavm', mode='WEBASSEMBLY_WASI',
            language='java', source_ready=True, target='wasm32-wasip1', address_bits=32,
            startup='command', exceptions=False)
        inputs = dict(format='uwvm-managed-example-inputs-v1', language='java', runtime=runtime,
            paths=dict(java_home=str(args.java_home), maven=str(maven), m2=str(m2),
                toolchain_receipt=str(receipt), wasm_tools=str(args.validator)),
            directories=list(map(str, roots)), files=sorted(files.values(), key=lambda row: row['path']),
            receipt_sha256=digest(receipt))
        (args.out / 'actual-offline-inputs.json').write_text(json.dumps(inputs, indent=2) + '\n')
    except BaseException as error:
        report.update(passed=False, error=repr(error))
        save()
        raise


if __name__ == '__main__':
    main()
