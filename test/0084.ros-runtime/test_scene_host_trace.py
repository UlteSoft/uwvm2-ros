"""Read the C++ recorder's actual bytes through DAP paging and private JSON export."""
from pathlib import Path
import argparse
import json
import stat
import sys

sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'tools/debug'))
import export_host_trace as export
import host_trace_view as view


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('packet', type=Path)
    parser.add_argument('output', type=Path)
    args = parser.parse_args()
    raw = args.packet.read_text()
    header, call, cursor = raw.splitlines()
    assert cursor == 'host-calls next=1 more=0'
    def request(command):
        assert command.endswith(' if-stop=7') and command.startswith('trace host read family=rosv2 ')
        if ' after=1 ' in command:
            return header + '\n' + cursor + '\n'
        assert ' after=0 ' in command
        return raw
    checkpoint = lambda: ('cooperative', 7)
    page = view.query(request, checkpoint, {'family': 'rosv2'})
    row, = page['calls']
    assert row['family'] == 'rosv2' and row['function'] == 'abi' and row['result'] == '2'
    assert not row['args'] and row['segments'] == '0' and page['journal']['rosv2-dropped'] == '0'
    options = {'family': 'rosv2', 'function': 'abi', 'namespace': 'uwvm_ros_v2', 'result': '2', 'ifStop': '7'}
    assert view.query(request, checkpoint, options)['calls'] == [row]
    for bad in ({'ifStop': '8'}, {'module': 'other'}, {'function': 'bad name'}, {'result': '4294967296'},
                {'alias': 'unknown'}, {'family': 'ros1', 'function': 'abi'}):
        try:
            view.query(request, checkpoint, {**options, **bad})
        except ValueError:
            continue
        raise AssertionError(bad)
    export.save_host_trace(request, checkpoint, str(args.output), 'rosv2', filters={'function':'abi','result':'2'}, if_stop='7')
    saved = json.loads(args.output.read_text())
    assert len(saved['calls']) == 1 and saved['calls'][0]['resultUnsigned'] == '2'
    assert saved['summary']['payloadBytes'] == '0'
    assert stat.S_IMODE(args.output.stat().st_mode) == 0o600
    for old,new in [(' segments=0',' segments=1'),(' args= ',' args=32:10 '),
                    (' result=2 ',' result=4294967296 '),(' returned=1',' returned=0'),
                    (' module=0',' module=22'),(' timed=0', ' timed=1')]:
        corrupt = call.replace(old, new)
        assert corrupt != call
        try:
            export.call_row(corrupt)
        except ValueError:
            continue
        raise AssertionError(new)
    print('PASS read, export, raw ABI result, private file and six metadata refusals')


if __name__ == '__main__':
    main()
