#!/usr/bin/env python3
"""Generate diagnostic copies which retain the original perf failure checks."""
import argparse
import json
from pathlib import Path

from runtime import digest


def generate(source, out):
    source = Path(source).resolve(strict=True)
    out = Path(out).resolve()
    out.mkdir(mode=0o700, exist_ok=False)
    header = source/'test/0073.ros-tests/support/source/ros_perf/common.h'
    cpp = source/'test/0073.ros-tests/support/source/ros_perf/native_ros2.cc'
    check = 'if(latency.size()!=count){throw std::runtime_error("bounded benchmark lost or duplicated messages");}'
    diagnostic = 'if(latency.size()!=count){std::cerr<<"{\\"status\\":\\"DIAGNOSTIC_FAIL\\",\\"expected\\":"<<count<<",\\"received\\":"<<latency.size()<<",\\"observer_elapsed_nanoseconds\\":"<<(now()-began)<<",\\"first_receive_monotonic_nanoseconds\\":"<<first<<",\\"last_receive_monotonic_nanoseconds\\":"<<last<<"}\\n";throw std::runtime_error("bounded benchmark lost or duplicated messages");}'
    ready = 'std::cout<<"READY\\n"<<std::flush;'
    ready_diagnostic = ready+'std::cerr<<"{\\"status\\":\\"DIAGNOSTIC_READY\\",\\"monotonic_nanoseconds\\":"<<ros_perf::now()<<",\\"collection_seconds\\":10}\\n"<<std::flush;'
    original_header, original_cpp = header.read_text(), cpp.read_text()
    if original_header.count(check) != 1 or original_cpp.count(ready) != 1:
        raise RuntimeError('unrecognized original exact-count or READY statement')
    (out/'common.h').write_text(original_header.replace(check, diagnostic))
    (out/'native_ros2.cc').write_text(original_cpp.replace(ready, ready_diagnostic))
    report = {'scope': 'Diagnostic copies only; original source/binary and historical FAIL unchanged',
        'original_inputs': {str(path.relative_to(source)): {'sha256': digest(path), 'bytes': path.stat().st_size}
                            for path in (header, cpp)},
        'generated_inputs': {path.name: {'path': str(path), 'sha256': digest(path), 'bytes': path.stat().st_size}
                             for path in (out/'common.h', out/'native_ros2.cc')},
        'changes': ['Print READY monotonic time', 'Print expected/received count and monotonic times immediately before the original failure'],
        'preserved': {'count_comparison': 'latency.size()!=count', 'throw_text': 'bounded benchmark lost or duplicated messages',
                      'collection_seconds': 10, 'native_payload_checks': True, 'native_qos_depth': 32},
        'eligible_as_unchanged_original_binary_pass': False}
    (out/'derivation.json').write_text(json.dumps(report, indent=2)+'\n')
    return report


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(generate(args.source, args.out), indent=2))
