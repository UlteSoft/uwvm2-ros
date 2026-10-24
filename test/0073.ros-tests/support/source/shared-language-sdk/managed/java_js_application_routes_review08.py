#!/usr/bin/env python3
"""Exercise actual Java POM/source and JS builtin policies as inert data checks.

The filesystem subsets below are explicitly not complete SDKs, generated record
substitutes, compiler inputs or application products. Genuine original POM and
authored source bytes are copied only to test selection and pin refusal rules.
No Java/JS application, compiler, native provider or Guest is executed here.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import sys


def pin(path):
    return dict(path=str(path.resolve()), bytes=path.stat().st_size,
                sha256=hashlib.sha256(path.read_bytes()).hexdigest())


def module(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise ValueError('the original admitted Linux owner is required')
    source = args.source.resolve(strict=True) / 'sdk/origin'
    args.out.mkdir(parents=True, exist_ok=False)
    report = dict(status='STARTED', scope='inert POM/source-path and module-policy fixtures only',
                  checks=[], compiler_executions=0, application_products=0, guest_executions=0,
                  native_ros_executions=0, default_authority=[])
    inputs = []

    def copy(relative, target):
        original = source / relative
        inputs.append(pin(original))
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(original, target)
        return target

    def check(label, callback, accepted=True):
        observed = True
        try:
            result = callback()
        except (ValueError, FileNotFoundError):
            observed, result = False, None
        if observed is not accepted:
            raise RuntimeError('actual source policy outcome differs: ' + label)
        report['checks'].append(dict(label=label, accepted=observed, passed=True))
        return result

    try:
        sdk = args.out / 'inert-subset-policy-sdk'
        builder = copy('examples/java_telemetry/build.py', sdk / 'example/platform/ros/java/java_telemetry/build.py')
        shared = sdk / 'include/platform/ros/java'
        copy('java/pom.xml', shared / 'pom.xml')
        copy('java/src/main/java/org/uwvm/roswasm/Result.java', shared / 'src/main/java/org/uwvm/roswasm/Result.java')
        platform = sdk / 'include/platform/nvidia-isaac-sim/java/clock-control'
        vendor = 'robots/nvidia/isaac_sim/'
        copy(vendor + 'language/java/pom.xml', platform / 'pom.xml')
        original_library = 'src/main/java/org/uwvm/roswasm/nvidia/isaacsim/ClockSession.java'
        library = copy(vendor + 'language/java/' + original_library, platform / original_library)
        application = sdk / 'example/platform/nvidia-isaac-sim/java/clock-program'
        pom = copy(vendor + 'clock_examples/java/pom.xml', application / 'pom.xml')
        original_entry = 'src/main/java/org/uwvm/roswasm/nvidia/isaacsim/example/ClockProgram.java'
        entry = copy(vendor + 'clock_examples/java/' + original_entry, application / original_entry)
        resolved = args.out / 'inert-resolved-source-policy.json'
        selection = dict(builder='java', platform='nvidia-isaac-sim', backend=2, pure=False,
                         project='clock-program/pom.xml', entry='clock-program/' + original_entry,
                         ros2_rmw_service=True)
        runtime = dict(route='java-teavm', version='0.13.1', mode='WEBASSEMBLY_WASI')
        document = dict(format='uwvm-sdk-resolved-profile-v1', language='java', sdk_root=str(sdk),
                        default_authority=[], language_options=dict(consumer=selection, runtime=runtime),
                        artifact_pins=[dict(path=path.relative_to(sdk).as_posix(), bytes=path.stat().st_size,
                            sha256=pin(path)['sha256']) for path in sorted(sdk.rglob('*')) if path.is_file()])

        def save():
            resolved.write_text(json.dumps(document) + '\n')

        implementation = module(builder, '_actual_java_policy')

        def project(selected_entry=entry):
            save()
            return implementation.project_contract(sdk, pom, selected_entry, 'nvidia-isaac-sim',
                shared / 'src/main/java', resolved, pin(resolved)['sha256'])

        parsed = check('original-real-POM/main-selected-over-inert-source-subsets', project)
        if parsed['main'] != 'org.uwvm.roswasm.nvidia.isaacsim.example.ClockProgram' or len(parsed['source_sets']) != 3:
            raise RuntimeError('the actual selected POM/main/source graph was replaced')
        check('entry-drift-refused', lambda: project(library), False)
        original_pom = pom.read_bytes()
        for label, before, after in (
                ('GC-target-refused', b'WEBASSEMBLY_WASI', b'WEBASSEMBLY_GC'),
                ('main-drift-refused', b'example.ClockProgram</mainClass>', b'example.OtherProgram</mainClass>'),
                ('foreign-platform-dependency-refused', b'roswasm-isaac-sim-clock', b'other-platform'),
                ('foreign-plugin-refused', b'maven-compiler-plugin', b'exec-maven-plugin')):
            if before not in original_pom:
                raise RuntimeError('actual original POM fixture changed')
            pom.write_bytes(original_pom.replace(before, after))
            check(label, project, False)
            pom.write_bytes(original_pom)
        inventory = document['artifact_pins']
        document['artifact_pins'] = [row for row in inventory if row['path'] != library.relative_to(sdk).as_posix()]
        check('unlisted-platform-source-refused', project, False)
        document['artifact_pins'] = inventory
        selection['entry'] = 'clock-program/other.java'
        check('resolved-main-drift-refused', project, False)
        selection['entry'] = 'clock-program/' + original_entry
        raw = library.read_bytes()
        library.write_bytes(raw + b'\n')
        check('source-byte-drift-refused', project, False)
        library.write_bytes(raw)
        extra = library.with_name('Unlisted.java')
        extra.write_bytes(raw)
        check('unlisted-extra-source-refused', project, False)
        extra.unlink()

        grammar_path = source / 'tools/shared_managed04/profile.py'
        inputs.append(pin(grammar_path))
        grammar = module(grammar_path, '_actual_service_selection')
        for language, identity, extra_fields in (
                ('java', ('java-teavm', '0.13.1', 'WEBASSEMBLY_WASI'),
                 dict(project='clock-program/pom.xml', entry='clock-program/Main.java')),
                ('js', ('js-quickjs', '2026-06-04', 'static-built-in-ros-bridge'),
                 dict(js_recipe='clock-program/app.recipe.json', entry='clock-program/app.mjs'))):
            selected = dict(builder=language, backend=2, pure=False, ros2_rmw_service=True, **extra_fields)
            actual = dict(zip(('route', 'version', 'mode'), identity))
            check(language + '-exact-service-route', lambda: grammar._service_selection(language, selected, actual))
            for field, changed in (('version', 'wrong'), ('route', 'other'), ('mode', 'host')):
                check(language + '-runtime-' + field + '-refused',
                      lambda field=field, changed=changed: grammar._service_selection(language, selected,
                          dict(actual, **{field: changed})), False)
            for field, changed in (('backend', 1), ('pure', True), ('ros2_rmw_service', 'true')):
                check(language + '-' + field + '-refused',
                      lambda field=field, changed=changed: grammar._service_selection(language,
                          dict(selected, **{field: changed}), actual), False)
            field = 'project' if language == 'java' else 'js_recipe'
            changed = '../outside/pom.xml' if language == 'java' else '../outside.json'
            check(language + '-source-traversal-refused', lambda: grammar._service_selection(language,
                  dict(selected, **{field: changed}), actual), False)

        js_path = source / 'examples/javascript_telemetry/build.py'
        inputs.append(pin(js_path))
        javascript = module(js_path, '_actual_JS_module_policy')
        graph = args.out / 'inert-module-grammar-fixture'
        graph.mkdir()
        bootstrap, main = graph / 'bootstrap.mjs', graph / 'main.mjs'
        bootstrap.write_text('// Static module-name policy fixture; never interpreted.\n')
        main.write_text('import "roswasm:rmw_service_native";\n')
        check('default-refuses-service-builtin', lambda: javascript.closed_modules(graph, graph, main, bootstrap), False)
        check('explicit-service-builtin-only', lambda: javascript.closed_modules(graph, graph, main, bootstrap, True))
        for spelling in ('std', 'os', 'node:fs', 'roswasm:other_native'):
            main.write_text('import ' + json.dumps(spelling) + ';\n')
            check('unknown-builtin-refused-' + spelling, lambda: javascript.closed_modules(graph, graph, main, bootstrap, True), False)
        main.write_text('import "roswasm:guest_native";\n')
        check('service-selection-without-actual-reference-refused',
              lambda: javascript.closed_modules(graph, graph, main, bootstrap, True), False)
        check('legacy-basic-builtin-still-closed', lambda: javascript.closed_modules(graph, graph, main, bootstrap))
        if any(pin(Path(row['path'])) != row for row in inputs):
            raise ValueError('captured actual source changed during policy checks')
        report.update(status='ACTUAL_JAVA_POM_JS_MODULE_AND_SERVICE_POLICY_CHECKS_PASS_NO_PRODUCTS', sources=inputs)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error), sources=inputs)
        raise
    finally:
        (args.out / 'results.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
