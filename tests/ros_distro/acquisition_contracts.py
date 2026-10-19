#!/usr/bin/env python3
"""Pure APT trust/architecture configuration contracts; no network or acquisition.

This fixture extracts only the three pure functions and never imports or
executes the acquisition producer's main body. Run it in the admitted cgroup.
The actual retained dump is the positive input; mutations exercise exact
trust/path/list/duplicate/hook failures rather than manufacturing a new dump.
"""
import argparse
import ast
import hashlib
import json
from pathlib import Path
import re


def main():
    parser = argparse.ArgumentParser(allow_abbrev=False)
    for name in ('candidate', 'evidence', 'out'):
        parser.add_argument('--' + name, required=True, type=Path)
    parser.add_argument('--candidate-sha256', required=True)
    parser.add_argument('--evidence-sha256', required=True)
    args = parser.parse_args()
    source = args.candidate.read_bytes(); evidence = args.evidence.read_bytes()
    assert hashlib.sha256(source).hexdigest() == args.candidate_sha256
    assert hashlib.sha256(evidence).hexdigest() == args.evidence_sha256
    original = json.loads(evidence)
    assert original['report_sha256'] == 'b2249f859e0264dacef03423548e95eccf0e5ef6c7ca7bddecf60328c848e1c1'
    log = original['logs'][0]
    assert log['sha256'] == '52e1d25e5bd99dcc8a2cb81e50a66a10e394ce7ad3a319e6362a6f790dacf4a3'
    raw = log['text'].encode(); assert hashlib.sha256(raw).hexdigest() == log['sha256']
    assert len(original['report']['commands']) == 1 and original['report']['commands'][0]['returncode'] == 0
    nodes = ast.parse(source).body
    selected = [n for n in nodes if isinstance(n, ast.FunctionDef) and
                n.name in ('need', 'apt_text', 'effective_apt_configuration')]
    assert len(selected) == 3
    namespace = {'re': re}
    exec(compile(ast.fix_missing_locations(ast.Module(body=selected, type_ignores=[])),
                 str(args.candidate), 'exec'), namespace)
    verify = namespace['effective_apt_configuration']
    root = original['report']['commands'][0]['cwd'] + '/apt'
    expected = {'Dir::Etc::parts': root + '/etc/parts', 'Dir::State::status': root + '/state/status',
                'Dir::Etc::trustedparts': root + '/etc/trustedparts', 'APT::Architecture': 'arm64',
                'APT::Get::AllowUnauthenticated': 'false', 'Acquire::AllowInsecureRepositories': 'false',
                'Acquire::Check-Date': 'true', 'Acquire::Languages': 'none', 'Dpkg::Use-Pty': 'false'}
    baseline = raw.decode().splitlines()
    rows = []

    def check(name, lines, rejection=False):
        rejected = False
        try:
            verify(lines, expected)
        except ValueError:
            rejected = True
        assert rejected is rejection, name
        rows.append({'name': name, 'expected_rejection': rejection, 'status': 'PASS'})

    def replace(key, value):
        prefix = key + ' '
        assert sum(line.startswith(prefix) for line in baseline) == 1
        return [key + ' "' + value + '";' if line.startswith(prefix) else line for line in baseline]

    check('actual-retained-case-and-language-normalization', baseline)
    scalar = [line for line in baseline if not line.startswith('Acquire::Languages')]
    scalar.append('Acquire::Languages "none";')
    check('exact-scalar-none-compatibility', scalar)
    check('allow-unauthenticated-refused', replace('APT::Get::AllowUnauthenticated', 'true'), True)
    check('insecure-repository-refused', replace('Acquire::AllowInsecureRepositories', 'true'), True)
    check('host-status-path-refused', replace('Dir::State::status', '/var/lib/dpkg/status'), True)
    check('host-parts-directory-refused', replace('Dir::Etc::parts', '/etc/apt/apt.conf.d'), True)
    check('case-equivalent-scalar-duplicate-refused', baseline + ['dPkG::Use-Pty "false";'], True)
    check('extra-host-architecture-refused', baseline + ['apt::Architectures:: "amd64";'], True)
    check('duplicate-target-architecture-refused', baseline + ['APT::Architectures:: "arm64";'], True)
    check('extra-translation-refused', baseline + ['Acquire::Languages:: "en";'], True)
    check('mixed-scalar-list-translation-refused', replace('Acquire::Languages', 'none'), True)
    check('hook-array-refused-case-insensitively', baseline + ['dPkG::pRe-InVoKe:: "id";'], True)
    check('hook-scalar-refused', baseline + ['APT::Update::Post-Invoke "id";'], True)
    check('empty-hook-container-does-not-run-hook', baseline + ['DPkg::Pre-Invoke "";'])
    check('nonrecord-diagnostic-refused', baseline + ['W: unexpected configuration diagnostic'], True)
    check('boolean-value-case-not-normalized', replace('DPkg::Use-Pty', 'False'), True)
    check('missing-required-scalar-refused', [s for s in baseline if not s.startswith('Dir::State::status ')], True)
    # Reuse the retained real dump, changing exactly its two architecture
    # selections. This models an explicit native-host SDK without accepting an
    # ambient extra architecture or changing any authentication/path controls.
    amd64 = [line.replace('"arm64";', '"amd64";')
             if line.lower().startswith(('apt::architecture ', 'apt::architectures:: '))
             else line for line in baseline]
    def target_check(name, lines, architecture, reject=False):
        controls = dict(expected, **{'APT::Architecture': architecture})
        refused = False
        try:
            observation = verify(lines, controls)
            assert observation['architecture'] == [architecture]
        except ValueError:
            refused = True
        assert refused is reject, name
        rows.append({'name': name, 'expected_rejection': reject, 'status': 'PASS'})
    target_check('explicit-native-amd64-selection', amd64, 'amd64')
    target_check('arm64-dump-cannot-qualify-amd64', baseline, 'amd64', True)
    target_check('amd64-dump-cannot-qualify-arm64', amd64, 'arm64', True)
    target_check('mixed-architecture-native-list-refused', amd64 + ['APT::Architectures:: "arm64";'], 'amd64', True)
    target_check('duplicate-native-architecture-refused', amd64 + ['apt::architectures:: "amd64";'], 'amd64', True)
    target_check('unsupported-architecture-refused', amd64, 'i386', True)
    target_check('native-target-nonempty-post-hook-refused', amd64 + ['DPkg::Post-Invoke "id";'], 'amd64', True)
    target_check('native-target-trust-bypass-refused', [line.replace('"false";', '"true";') if line.startswith('APT::Get::AllowUnauthenticated ') else line for line in amd64], 'amd64', True)
    assert len(rows) == 25
    args.out.parent.mkdir(parents=True, exist_ok=True)
    with args.out.open('x') as stream:
        json.dump({'kind': 'pure-effective-APT-dump-contracts', 'status': 'PASS', 'cases': rows,
                   'candidate_sha256': args.candidate_sha256, 'actual_evidence_sha256': args.evidence_sha256,
                   'apt_or_network_or_acquisition_executed': False}, stream, indent=2)
        stream.write('\n')
    print('PASS_EFFECTIVE_APT_DUMP_CONTRACTS cases=25')


if __name__ == '__main__':
    main()
