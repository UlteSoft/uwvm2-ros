"""Run NVIDIA's pinned message buffer and camera timestamp synchronizer suites."""
import argparse
import json
import os
from pathlib import Path
import subprocess
import xml.etree.ElementTree as ET

from run_scene_vm import sha, verify_gate

REVISION = '7bec6ae5dbb2faa11617f40226e8932a4a94e6e4'
SOURCES = {
    'include/isaac_common/messaging/message_buffer.hpp': '2e4f7f09e99dfd4925ca62d39a22487fbb418a80034009f2dd3f8acba1c5d1a1',
    'include/isaac_common/messaging/message_stream_synchronizer.hpp': 'bdcd26471094a93d25c7d7a2503a9cc7e4338ed11560adc943edaf486203911d',
    'test/test_message_buffer.cpp': 'ffe4880d14341e6de2089c6d16803fe221ae7d53e3dc9cea92c430050eab7e90',
    'test/test_message_stream_synchronizer.cpp': '168c7fac20ea51af8604d85471ed4dc16edb08091297c4e8d60ddeb1525df8a8',
}
EXPECTED = {'MessageBufferTest.' + test for test in (
    'TestPushAndPop', 'TestTimestamps', 'TestFixedBufferSize', 'TestRangeAccessAndDelete')}
EXPECTED.add('MessageStreamSynchronizerTests.EverythingTest')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--upstream', type=Path, required=True)
    parser.add_argument('--googletest', type=Path, required=True, help='GoogleTest source directory')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if not __debug__:
        raise RuntimeError('Qualification requires Python assertions')
    verify_gate()
    package = args.upstream.resolve() / 'isaac_common'
    assert {relative: sha(package / relative) for relative in SOURCES} == SOURCES
    gtest = args.googletest.resolve()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    binary = out / 'message-tests'
    report = out / 'gtest.xml'
    compiler = Path('/usr/bin/c++').resolve()
    sources = [str(package / name) for name in SOURCES if name.endswith('.cpp')]
    argv = [str(compiler), '-std=c++17', '-O3', '-g', '-pthread', '-I', str(package / 'include'),
            '-I', str(gtest / 'include'), '-I', str(gtest), *sources,
            str(gtest / 'src/gtest-all.cc'), str(gtest / 'src/gtest_main.cc'), '-o', str(binary)]
    subprocess.run(argv, check=True, timeout=240)
    subprocess.run([str(binary), '--gtest_output=xml:' + str(report)], check=True, timeout=20)
    tests = ET.parse(report).findall('.//testcase')
    names = {test.attrib['classname'] + '.' + test.attrib['name'] for test in tests}
    assert names == EXPECTED and len(tests) == len(EXPECTED)
    assert all(test.attrib['result'] == 'completed' and not list(test) for test in tests)
    result = dict(passed=True, upstream_revision=REVISION, upstream_files=SOURCES, tests=sorted(names),
                  compiler_sha256=sha(compiler), command=argv, binary_sha256=sha(binary),
                  gtest_report_sha256=sha(report), cgroup=Path('/proc/self/cgroup').read_text().strip(),
                  cpus=sorted(os.sched_getaffinity(0)), accelerated_ros_graph_exercised=False,
                  vm_provider_exercised=False)
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')


if __name__ == '__main__':
    main()
