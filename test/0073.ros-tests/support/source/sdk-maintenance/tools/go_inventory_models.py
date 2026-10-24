#!/usr/bin/env python3
"""Exercise the Go result parser without compiling or executing any Go code.

Run ``python3 -B sdk/origin/tools/go_inventory_models.py`` from any directory. The
model extracts only the pure inventory functions from the adjacent controller,
then constructs the documented Go verbose output for its nine top-level tests
and 57 literal positive-vector subtests. Each omitted, duplicated, skipped,
failed, renamed or incorrectly nested result must fail closed.

An optional first argument is an archived verbose log from an actual guarded
Go run. Parsing that historical text is a parser regression check, not a new
compiler, native execution, cgroup or ROS result.
"""

# Test sources are separate from the selected production SDK.
import sys as _source_sys
from pathlib import Path as _SourcePath
_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())
_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))
from source_paths import sdk_path as _sdk_path

import ast
import hashlib
import json
from pathlib import Path
import re
import sys

sys.dont_write_bytecode = True
HERE = _CHECKOUT_ROOT / 'test/0073.ros-tests/support/source/sdk-maintenance/tools'
SOURCE = _sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'tools', 'run_cgroup_models.py')
VECTOR = HERE.parent / 'tinygo/wire_vectors_test.go'


def pure_functions():
    """Exclude all module-level imports, filesystem admission and launch code."""
    tree = ast.parse(SOURCE.read_text())
    functions = {'require', 'go_subtest_inventory', 'validate_go_inventory'}
    body = [node for node in tree.body if
            isinstance(node, ast.FunctionDef) and node.name in functions or
            isinstance(node, ast.Assign) and any(
                isinstance(target, ast.Name) and target.id == 'GO_TESTS'
                for target in node.targets)]
    assert len(body) == 4
    namespace = {'re': re}
    exec(compile(ast.fix_missing_locations(ast.Module(body=body, type_ignores=[])),
                 str(SOURCE), 'exec'), namespace)
    return namespace


def main():
    """Require the valid transcript and independently reject each mutation."""
    functions = pure_functions()
    subtests = functions['go_subtest_inventory'](VECTOR.read_text())
    tests = functions['GO_TESTS']
    lines = []
    for test in tests:
        children = [name for name in subtests if name.startswith(test + '/')]
        lines.append('=== RUN   ' + test)
        lines += ['=== RUN   ' + child for child in children]
        lines.append('--- PASS: ' + test + ' (0.00s)')
        lines += ['    --- PASS: ' + child + ' (0.00s)' for child in children]
    lines.append('PASS')
    transcript = '\n'.join(lines) + '\n'
    validate = functions['validate_go_inventory']
    valid = validate(transcript, subtests)
    assert set(valid['tests']) == set(tests) and set(valid['subtests']) == set(subtests)
    checks = 1

    def rejected(candidate, inventory=subtests):
        nonlocal checks
        try:
            validate(candidate, inventory)
        except ValueError:
            checks += 1
        else:
            raise AssertionError('parser accepted a malformed transcript')

    for index, line in enumerate(lines):
        rejected('\n'.join(lines[:index] + lines[index+1:]) + '\n')
        rejected('\n'.join(lines[:index] + [line] + lines[index:]) + '\n')
        if '--- PASS:' in line:
            for status in ('FAIL', 'SKIP'):
                rejected(transcript.replace(line, line.replace('PASS:', status + ':'), 1))
            wrong_indent = line.lstrip() if line.startswith(' ') else '    ' + line
            rejected(transcript.replace(line, wrong_indent, 1))
        if line.startswith('=== RUN'):
            rejected(transcript.replace(line, '    ' + line, 1))
            rejected(transcript.replace(line, line + '/unexpected', 1))
    for token in ('FAIL', 'panic: unexpected', 'fatal error: unexpected',
                  '=== PAUSE TestSharedPositiveVectors', '=== CONT TestSharedPositiveVectors'):
        rejected(transcript.replace('PASS\n', token + '\nPASS\n'))
    rejected('PASS\n' + transcript)
    rejected(transcript + lines[0] + '\n')
    rejected(transcript, subtests + (subtests[0],))
    rejected(transcript, subtests[:-1])
    first = subtests[0]
    child_pass = '    --- PASS: ' + first + ' (0.00s)'
    parent_pass = '--- PASS: TestSharedPositiveVectors (0.00s)'
    reordered = transcript.replace(child_pass + '\n', '')
    rejected(reordered.replace(parent_pass, child_pass + '\n' + parent_pass, 1))
    first_run = '=== RUN   ' + first
    parent_run = '=== RUN   TestSharedPositiveVectors'
    reordered = transcript.replace(first_run + '\n', '')
    rejected(reordered.replace(parent_run, first_run + '\n' + parent_run, 1))
    for changed in (VECTOR.read_text().replace('t.Run("string-empty"', 't.Run("string-ascii"', 1),
                    VECTOR.read_text().replace('t.Run("string-empty",', 't.Run(dynamicName,', 1)):
        try:
            functions['go_subtest_inventory'](changed)
        except ValueError:
            checks += 1
        else:
            raise AssertionError('unparsed or duplicate literal vector was accepted')
    archived = None
    if len(sys.argv) > 1:
        raw = Path(sys.argv[1]).read_bytes()
        result = validate(raw.decode('utf-8'), subtests)
        assert len(result['tests']) == 9 and len(result['subtests']) == 57
        archived = {'sha256': hashlib.sha256(raw).hexdigest(), 'bytes': len(raw)}
        checks += 1
    print(json.dumps({'status': 'SOURCE_AND_SYNTHETIC_GO_INVENTORY_PASS', 'checks': checks,
                      'top_level_tests': len(tests), 'declared_subtests': len(subtests),
                      'controller_sha256': hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                      'vector_source_sha256': hashlib.sha256(VECTOR.read_bytes()).hexdigest(),
                      'archived_log_parser_check': archived,
                      'actual_compiler_native_ROS_execution': False}))


if __name__ == '__main__':
    main()
