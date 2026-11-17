"""Use a real C++ metadata packet, then test closed grammar and stale/permission rejection."""
import argparse
import sys
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/debug'))
import dap_adapter as d
PACKET=''
class Tests(unittest.TestCase):
    def test_roundtrip(self):
        split=PACKET.index('wasithread stop=11 selection=members')
        p=d.parse_wasithread_state(PACKET[:split],d.wasithread_state_options({}),11)
        self.assertEqual(p['rows'][0]['Additional threads'],'1')
        self.assertEqual(p['rows'][1]['Scope'],'group')
        self.assertEqual(p['rows'][0]['Maximum extra threads'],'unlimited')
        m=d.parse_wasithread_state(PACKET[split:],d.wasithread_state_options({'selection':'members'}),11)
        self.assertEqual({x['Module'] for x in m['rows']},{'module with spaces','robot-机器人'})
    def test_four_permissions(self):
        for p in ('full','read-only','limited','none'):
            self.assertEqual(d.dap_permission_allowed(p,'uwvm/wasithreadState'),p in ('full','read-only'))
    def test_bad_requests(self):
        for value in [None,[],{'grant':'allow'},{'selection':'spawn'},{'start':-1},{'start':1<<64},
                      {'count':0},{'count':9},{'count':True},{'count':1.0},{'ifStop':0},{'ifStop':'11\ncontinue'}]:
            with self.subTest(value=value), self.assertRaises(ValueError):d.wasithread_state_options(value)
    def test_bad_packets(self):
        wire=PACKET[:PACKET.index('wasithread stop=11 selection=members')]
        for broken in [wire.replace('stop=11','stop=12'),wire.replace('selection=policy','selection=members'),
                       wire.replace('count=2','count=3'),wire.replace('mains=1','mains=18446744073709551616'),
                       wire.replace('interface=allow','interface=superuser'),wire.replace('target=-','target=00'),
                       wire.replace('enabled=1','enabled=2'),wire+'garbage',wire.replace('scope=group','scope=invalid')]:
            with self.subTest(broken=broken),self.assertRaises(ValueError):
                d.parse_wasithread_state(broken,d.wasithread_state_options({}),11)
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--packet',required=True,type=Path);args,rest=p.parse_known_args()
    PACKET=args.packet.read_text();unittest.main(argv=[sys.argv[0],*rest])
