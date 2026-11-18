#!/usr/bin/env python3
"""Security boundary tests for independent scene DAP metadata.

These model console packets and state transitions, not native scene providers.
Optional --formatter consumes bytes from the real C++ producer test. Execute
only through the established 64 GiB cgroup runner.
"""
import importlib.util
import argparse
import io
import json
from pathlib import Path
import sys
import subprocess
import unittest
sys.dont_write_bytecode = True
spec = importlib.util.spec_from_file_location("scene_dap", Path(__file__).resolve().parents[2]/"tools/debug/dap_adapter.py")
dap = importlib.util.module_from_spec(spec); spec.loader.exec_module(dap)
STOP = "stopped: cooperative pause\nstop-id 11\nthread 7 module=0 function=0 byte-offset=0 generation=1\n"
ROW = ('row kind=grant environment="robot" alias="log" resource="records" binding="private" type="record" state="" '
       'id=18446744073709551615 open_handles=0 admissions=1 refused=0 sequence=0 members=0 size_bytes=0 '
       'accepted_bytes=0 reserved_bytes=0 retained_receipts=0 '
       'generation=0 transition=0 deadline_ns=0 status_code=0 lease_count=0 workspace_bytes=0 '
       'input_bytes=0 output_bytes=0 output_items=0 '
       'sample_count=0 query_count=0 pending_queries=0 query_bytes=0 carrier_bytes=0 '
       'emitted_records=0 exported_records=0 lost_records=0 retained_bytes=0 copy_failures=0 '
       'revoked=0 attenuated=0 retired=0 armed=0 immutable=0 published=0 durable=0 callback_pending=0 callback_delivered=0 '
       'counters_saturated=0 pending_page=0 busy=0 exit_observed=0 platform="" platform_enabled=0 clear_after_copy=0')
ARTIFACT = (ROW.replace('kind=grant','kind=resource').replace('type="record"','type="artifact"')
            .replace('size_bytes=0','size_bytes=18446744073709551615').replace('immutable=0','immutable=1'))
OUTPUT = (ROW.replace('kind=grant','kind=resource').replace('type="record"','type="artifact.output"')
          .replace('state=""','state="staged"').replace('accepted_bytes=0','accepted_bytes=9007199254740993')
          .replace('reserved_bytes=0','reserved_bytes=18446744073709551615').replace('retained_receipts=0','retained_receipts=1'))
RECEIPT = (OUTPUT.replace('type="artifact.output"','type="artifact.receipt"').replace('state="staged"','state="removed"')
           .replace('reserved_bytes=18446744073709551615','reserved_bytes=0'))
ABSENT_RECEIPT = (RECEIPT.replace('state="removed"','state="none"').replace('id=18446744073709551615','id=0')
                  .replace('accepted_bytes=9007199254740993','accepted_bytes=0'))
LIFECYCLE = (ROW.replace('kind=grant','kind=resource').replace('type="record"','type="lifecycle"')
             .replace('binding="private"','binding="local-lifecycle"').replace('state=""','state="configuring"')
             .replace('id=18446744073709551615','id=0').replace('generation=0','generation=9007199254740993')
             .replace('transition=0','transition=1').replace('deadline_ns=0','deadline_ns=9223372036854775807')
             .replace('callback_pending=0','callback_pending=1'))
BUFFER = (ROW.replace('kind=grant','kind=resource').replace('type="record"','type="buffer"')
          .replace('binding="private"','binding="sealed-buffer"').replace('id=18446744073709551615','id=0')
          .replace('members=0','members=2').replace('open_handles=0','open_handles=1')
          .replace('reserved_bytes=0','reserved_bytes=9007199254740993')
          .replace('lease_count=0','lease_count=18446744073709551615').replace('workspace_bytes=0','workspace_bytes=9223372036854775807'))
FORMATTER = None
MODEL = (ROW.replace('kind=grant','kind=resource').replace('type="record"','type="model"')
         .replace('binding="private"','binding="local-model"').replace('id=18446744073709551615','id=0')
         .replace('members=0','members=1').replace('open_handles=0','open_handles=2')
         .replace('input_bytes=0','input_bytes=9007199254740993').replace('output_bytes=0','output_bytes=9223372036854775807')
         .replace('output_items=0','output_items=18446744073709551615').replace('workspace_bytes=0','workspace_bytes=9223372036854775807'))
TRANSFORM = (ROW.replace('kind=grant','kind=resource').replace('type="record"','type="transform"')
             .replace('binding="private"','binding="local-transform"').replace('id=18446744073709551615','id=0')
             .replace('members=0','members=3').replace('open_handles=0','open_handles=2')
             .replace('reserved_bytes=0','reserved_bytes=9223372036854775807').replace('workspace_bytes=0','workspace_bytes=9007199254740993')
             .replace('sample_count=0','sample_count=9007199254740993').replace('query_count=0','query_count=18446744073709551615')
             .replace('pending_queries=0','pending_queries=9007199254740993').replace('query_bytes=0','query_bytes=9223372036854775807')
             .replace('carrier_bytes=0','carrier_bytes=18446744073709551615').replace('admissions=1','admissions=0'))
def packet(rows=(), total=None, next_value=None, more=0, selection="authority"):
    return (f"scene scope=process-owned stop=11 selection={selection} status=available\n"+
            ''.join(row+'\n' for row in rows)+f"page total={len(rows) if total is None else total} next={len(rows) if next_value is None else next_value} more={more}\n")
class SceneTests(unittest.TestCase):
    def test_process_audit_tail_losses(self):
        row = "audit configured=1 admitted=1 limited=18446744073709551615 busy=2 clock_failed=3 sink_failed=1"
        summary = self.parse(packet([row], selection="audit"), selection="audit")
        self.assertEqual(summary["rows"][0]["limited"], "18446744073709551615")
        self.assertTrue(summary["rows"][0]["configured"])
        self.assertFalse(summary["payloadsRead"] or summary["nativeOperationsIssued"])
        self.assertEqual(self.parse(packet((), total=1, selection="audit"), selection="audit", start=1)["rows"], [])
        for invalid in (row + " bytes=0", row.replace("sink_failed=1", "sink_failed=2"),
                        row.replace("configured=1", "configured=0"), row.replace("busy=2", "busy=-1")):
            with self.subTest(invalid=invalid), self.assertRaises(ValueError):
                self.parse(packet([invalid], selection="audit"), selection="audit")
        for options in ({"alias": "signal"}, {"environment": "robot"}):
            with self.assertRaises(ValueError):
                dap.scene_state_options({"selection": "audit", **options})
        with self.assertRaises(ValueError):
            self.parse(packet((), selection="audit"), selection="audit")
    def test_registered_message_counts_and_quarantine(self):
        row=(ROW.replace('kind=grant','kind=resource').replace('id=18446744073709551615','id=0')
             .replace('admissions=1','admissions=0').replace('type="record"','type="module.call"')
             .replace('binding="private"','binding="registered-message"').replace('state=""','state="active"')
             .replace('members=0','members=2').replace('open_handles=0','open_handles=1')
             .replace('sample_count=0','sample_count=1').replace('reserved_bytes=0','reserved_bytes=65536'))
        self.assertEqual(self.parse(packet([row],selection='resources'),selection='resources')['rows'][0]['sample_count'],'1')
        for clear in (0,1):
            parsed=self.parse(packet([row.replace('clear_after_copy=0',f'clear_after_copy={clear}')],selection='resources'),selection='resources')
            self.assertEqual(parsed['rows'][0]['clear_after_copy'],bool(clear))
        for invalid in (ROW.replace('clear_after_copy=0','clear_after_copy=1'),row.replace('clear_after_copy=0','clear_after_copy=2')):
            with self.assertRaises(ValueError):self.parse(packet([invalid]))
        for before,after in [('members=2','members=0'),('query_count=0','query_count=1'),
                             ('id=0','id=99'),('input_bytes=0','input_bytes=1'),('callback_pending=0','callback_pending=1')]:
            with self.subTest(after=after),self.assertRaises(ValueError):
                self.parse(packet([row.replace(before,after)],selection='resources'),selection='resources')
        quarantine=row.replace('state="active"','state="quarantined"').replace('sample_count=1','sample_count=0').replace('query_count=0','query_count=1')
        self.assertEqual(self.parse(packet([quarantine],selection='resources'),selection='resources')['rows'][0]['query_count'],'1')
    def test_platform_is_child_configuration(self):
        module = 'uwvm_ros_v2.platform_extension.fixture.v1'
        row = ROW.replace('type="record"','type="device"').replace('platform=""',f'platform="{module}"')
        for enabled in (0,1):
            parsed=self.parse(packet([row.replace('platform_enabled=0',f'platform_enabled={enabled}')]))['rows'][0]
            self.assertEqual(parsed['platform']['text'],module)
            self.assertEqual(parsed['platform_enabled'],bool(enabled))
        for malformed in ('uwvm_platform_v1','uwvm_ros_v2.platform_extension.*.v1','uwvm_ros_v2.platform_extension.fixture.v01',
                          'uwvm_ros_v2.platform_extension.fixture.v0','uwvm_ros_v2.platform_extension.fixture.v4294967296'):
            with self.subTest(module=malformed),self.assertRaises(ValueError):self.parse(packet([row.replace(module,malformed)]))
        for malformed in (ROW.replace('platform_enabled=0','platform_enabled=1'),row.replace('type="device"','type="record"')):
            with self.assertRaises(ValueError):self.parse(packet([malformed]))
    def parse(self, body, **options):
        return dap.parse_scene_state(body,dap.scene_state_options(options),11)
    def test_media_and_bridge_closed_aggregates(self):
        base=ROW.replace('kind=grant','kind=resource').replace('id=18446744073709551615','id=0').replace('admissions=1','admissions=0')
        media=(base.replace('type="record"','type="media"').replace('binding="private"','binding="immutable-buffer-media"')
               .replace('state=""','state="active"').replace('members=0','members=1').replace('open_handles=0','open_handles=1')
               .replace('output_items=0','output_items=9007199254740993').replace('exported_records=0','exported_records=9007199254740992'))
        bridge=(base.replace('type="record"','type="bridge"').replace('binding="private"','binding="topic-forwarder"')
                .replace('state=""','state="outcome_unknown"').replace('members=0','members=1').replace('sample_count=0','sample_count=1')
                .replace('query_count=0','query_count=1').replace('lost_records=0','lost_records=9007199254740993'))
        for row in (media,bridge):
            self.assertEqual(len(self.parse(packet([row],selection='resources'),selection='resources')['rows']),1)
            for before,after in [('id=0','id=1'),('admissions=0','admissions=1'),('workspace_bytes=0','workspace_bytes=1') if row==bridge else ('exported_records=9007199254740992','exported_records=9007199254740994')]:
                with self.subTest(after=after),self.assertRaises(ValueError):
                    self.parse(packet([row.replace(before,after)],selection='resources'),selection='resources')
            for extra in (' native_fd=3',' payload="private"',' source_handle=9',' request_key=1'):
                with self.assertRaises(ValueError):self.parse(packet([row+extra],selection='resources'),selection='resources')
        for before,after in [('sample_count=1','sample_count=0'),('query_count=1','query_count=2'),('state="outcome_unknown"','state="running"')]:
            with self.assertRaises(ValueError):self.parse(packet([bridge.replace(before,after)],selection='resources'),selection='resources')
    def test_device_control_component_closed_states(self):
        base=ROW.replace('kind=grant','kind=resource').replace('id=18446744073709551615','id=0').replace('admissions=1','admissions=0')
        device=base.replace('type="record"','type="device"').replace('binding="private"','binding="framed-scalar"').replace('state=""','state="stop_accepted"')
        control=device.replace('type="device"','type="control"').replace('framed-scalar','framed-control').replace('generation=0','generation=9007199254740993')
        component=base.replace('type="record"','type="component"').replace('binding="private"','binding="isolated-roles"').replace('state=""','state="exited"').replace('exit_observed=0','exit_observed=1')
        for row in (device,control,component):
            self.assertEqual(len(self.parse(packet([row],selection='resources'),selection='resources')['rows']),1)
            for extra in (' native_pid=3',' payload="private"',' request_key=1'):
                with self.assertRaises(ValueError):self.parse(packet([row+extra],selection='resources'),selection='resources')
        for row in (device.replace('stop_accepted','stopped'),device.replace('armed=0','armed=1'),control.replace('open_handles=0','open_handles=2'),
                    component.replace('members=0','members=1'),component.replace('exit_observed=1','exit_observed=0'),base.replace('exit_observed=0','exit_observed=1')):
            with self.assertRaises(ValueError):self.parse(packet([row],selection='resources'),selection='resources')
        failed=component.replace('state="exited"','state="failed"').replace('status_code=0','status_code=4')
        self.assertEqual(self.parse(packet([failed],selection='resources'),selection='resources')['rows'][0]['status_code'],'4')
        retired=failed.replace('state="failed"','state="retired"').replace('retired=0','retired=1')
        self.assertEqual(self.parse(packet([retired],selection='resources'),selection='resources')['rows'][0]['status_code'],'4')
        for before,after in [('status_code=4','status_code=0'),('status_code=4','status_code=10'),
                             ('state="failed"','state="ready"'),('state="failed"','state="exited"')]:
            with self.subTest(after=after),self.assertRaises(ValueError):
                self.parse(packet([failed.replace(before,after)],selection='resources'),selection='resources')
    def test_external_driver_state(self):
        base=(ROW.replace('kind=grant','kind=resource').replace('type="record"','type="driver"')
              .replace('binding="private"','binding="registered-driver"').replace('id=18446744073709551615','id=0')
              .replace('admissions=1','admissions=0').replace('state=""','state="pending"').replace('sequence=0','sequence=4'))
        self.assertEqual(self.parse(packet([base],selection='resources'),selection='resources')['rows'][0]['sequence'],'4')
        failed=base.replace('state="pending"','state="failed"').replace('query_count=0','query_count=1')
        self.assertEqual(self.parse(packet([failed],selection='resources'),selection='resources')['rows'][0]['query_count'],'1')
        for before,after in [('state="failed"','state="ready"'),('query_count=1','query_count=2'),('armed=0','armed=1'),
                             ('members=0','members=1'),('status_code=0','status_code=4294967296')]:
            with self.subTest(after=after),self.assertRaises(ValueError):
                self.parse(packet([failed.replace(before,after)],selection='resources'),selection='resources')
    def test_default_and_closed_options(self):
        self.assertEqual(self.parse(packet())["rows"],[])
        for arg in ({"selection":"revoke"},{"selection":[]},{"handle":1},{"environment":"a/b"},{"alias":"x\ncontinue"},
                    {"count":True},{"count":0},{"count":9},{"start":4097},{"ifStop":"01"},{"ifStop":"18446744073709551616"},
                    {"alias":"a"*256},{"environment":"机器"}):
            with self.subTest(arg=arg),self.assertRaises(ValueError):dap.scene_state_options(arg)
    def test_actual_driver_formatter(self):
        if FORMATTER is None:self.skipTest('real formatter not supplied')
        body=subprocess.check_output([FORMATTER,'--driver-packet'],text=True)
        row=self.parse(body,selection='resources')['rows'][0]
        self.assertEqual(row['type']['text'],'driver')
        self.assertEqual(row['sequence'],'9007199254740993')
        self.assertEqual(row['state']['text'],'failed')
        self.assertEqual(row['query_count'],'1')
        self.assertFalse(row['armed'])
    def test_actual_robot_formatter(self):
        if FORMATTER is None:self.skipTest('real formatter not supplied')
        body=subprocess.check_output([FORMATTER,'--robot-packet'],text=True)
        rows=self.parse(body,selection='resources')['rows']
        self.assertEqual([r['type']['text'] for r in rows],['media','bridge','device','control','component'])
        self.assertEqual(rows[0]['output_items'],'9007199254740993')
        self.assertEqual(rows[1]['query_count'],'1')
        self.assertEqual(rows[2]['state']['text'],'stop_accepted')
        self.assertEqual(rows[3]['generation'],'9007199254740993')
        self.assertEqual(rows[3]['sequence'],'18446744073709551615')
        self.assertTrue(rows[4]['exit_observed']);self.assertEqual(rows[4]['members'],'0')
        self.assertEqual(rows[4]['state']['text'],'outcome_unknown')
    def test_log_diagnostic_aggregate_schema(self):
        log=(ROW.replace('kind=grant','kind=resource').replace('type="record"','type="log"')
             .replace('binding="private"','binding="private-log"').replace('id=18446744073709551615','id=0')
             .replace('admissions=1','admissions=0').replace('emitted_records=0','emitted_records=18446744073709551615')
             .replace('lost_records=0','lost_records=9007199254740993').replace('reserved_bytes=0','reserved_bytes=1024')
             .replace('retained_bytes=0','retained_bytes=512').replace('counters_saturated=0','counters_saturated=1'))
        value=self.parse(packet([log],selection='resources'),selection='resources')['rows'][0]
        self.assertEqual(value['lost_records'],'9007199254740993');self.assertTrue(value['counters_saturated'])
        diagnostic=(ROW.replace('kind=grant','kind=resource').replace('type="record"','type="diagnostic"')
                    .replace('binding="private"','binding="local-diagnostic"').replace('id=18446744073709551615','id=0')
                    .replace('admissions=1','admissions=0').replace('reserved_bytes=0','reserved_bytes=512')
                    .replace('pending_page=0','pending_page=1').replace('busy=0','busy=1'))
        value=self.parse(packet([diagnostic],selection='resources'),selection='resources')['rows'][0]
        self.assertTrue(value['pending_page']);self.assertTrue(value['busy'])
        for before,after in [('id=0','id=1'),('admissions=0','admissions=1'),('deadline_ns=0','deadline_ns=1'),
                             ('pending_page=0','pending_page=1'),('retained_bytes=512','retained_bytes=1025'),
                             ('emitted_records=18446744073709551615','emitted_records=1')]:
            with self.subTest(after=after),self.assertRaises(ValueError):self.parse(packet([log.replace(before,after)],selection='resources'),selection='resources')
        for field in ('emitted_records','exported_records','lost_records','retained_bytes','copy_failures','counters_saturated','refused'):
            with self.assertRaises(ValueError):self.parse(packet([diagnostic.replace(field+'=0',field+'=1')],selection='resources'),selection='resources')
        for field in ('emitted_records','exported_records','lost_records','retained_bytes','copy_failures','counters_saturated','pending_page','busy'):
            with self.assertRaises(ValueError):self.parse(packet([ROW.replace(field+'=0',field+'=1')]))
        for extra in (' message="private"',' logger="private"',' cursor=7',' revision=9',' expires=1',' counter_value=1'):
            with self.assertRaises(ValueError):self.parse(packet([diagnostic+extra],selection='resources'),selection='resources')
    def test_actual_log_diagnostic_formatter(self):
        if FORMATTER is None:self.skipTest('real formatter not supplied')
        body=subprocess.check_output([FORMATTER,'--log-diagnostic-packet'],text=True)
        rows=self.parse(body,selection='resources')['rows']
        self.assertEqual(rows[0]['emitted_records'],'18446744073709551615');self.assertEqual(rows[0]['exported_records'],'9007199254740993')
        self.assertTrue(rows[0]['counters_saturated']);self.assertTrue(rows[1]['pending_page'])
    def test_private_provider_closed_schemas(self):
        base=ROW.replace('kind=grant','kind=resource').replace('id=18446744073709551615','id=0').replace('admissions=1','admissions=0')
        def record(kind,binding,**fields):
            row=base.replace('type="record"',f'type="{kind}"').replace('binding="private"',f'binding="{binding}"').replace('state=""','state="active"')
            for key,value in fields.items():row=row.replace(key+'=0',key+'='+str(value))
            return row
        recording=record('recording','private-recording',open_handles=3,members=2,reserved_bytes=9223372036854775807,
                         retained_bytes=9007199254740993,sample_count=18446744073709551615,query_count=2,lease_count=4)
        replay=record('replay','private-replay',open_handles=1,members=1,reserved_bytes=512)
        sensor=record('sensor','bounded-sensor',open_handles=1,members=2,reserved_bytes=1024,retained_bytes=128,
                      sample_count=3,lease_count=2,input_bytes=9007199254740993,output_items=18446744073709551615,
                      lost_records=9,counters_saturated=1,workspace_bytes=512,busy=1)
        proposal=record('model.proposal','model-proposals',members=4,lease_count=2,sample_count=3,query_count=2,workspace_bytes=9007199254740993)
        rows=self.parse(packet([recording,replay,sensor,proposal],selection='resources'),selection='resources')['rows']
        self.assertEqual(rows[0]['retained_bytes'],'9007199254740993');self.assertEqual(rows[2]['output_items'],'18446744073709551615')
        for row in (recording,replay,sensor,proposal):
            for before,after in [('id=0','id=1'),('admissions=0','admissions=1'),('deadline_ns=0','deadline_ns=1'),
                                 ('published=0','published=1'),('durable=0','durable=1'),('state="active"','state="unknown"')]:
                with self.subTest(before=before,row=row[:80]),self.assertRaises(ValueError):
                    self.parse(packet([row.replace(before,after)],selection='resources'),selection='resources')
            for extra in (' cursor=1',' request_key=1',' descriptor=7',' payload="private"',' approval=1'):
                with self.assertRaises(ValueError):self.parse(packet([row+extra],selection='resources'),selection='resources')
        for row in (recording.replace('members=2','members=4'),replay.replace('members=1','members=2'),
                    sensor.replace('status_code=0','status_code=5'),proposal.replace('open_handles=0','open_handles=1')):
            with self.assertRaises(ValueError):self.parse(packet([row],selection='resources'),selection='resources')
        failed=sensor.replace('status_code=0','status_code=5').replace('state="active"','state="failed"')
        self.assertEqual(self.parse(packet([failed],selection='resources'),selection='resources')['rows'][0]['status_code'],'5')
    def test_communication_closed_schema(self):
        base=ROW.replace('kind=grant','kind=resource').replace('id=18446744073709551615','id=0').replace('admissions=1','admissions=0')
        row=base.replace('type="record"','type="communication"').replace('binding="private"','binding="native-communication"').replace('state=""','state="ready"').replace('members=0','members=1')
        value=self.parse(packet([row],selection='resources'),selection='resources')['rows'][0]
        self.assertEqual(value['state']['text'],'ready');self.assertEqual(value['members'],'1')
        action=row.replace('binding="native-communication"','binding="native-action"')
        self.assertEqual(self.parse(packet([action],selection='resources'),selection='resources')['rows'][0]['binding']['text'],'native-action')
        for extra in (' goal_uuid="private"',' goal_handle=1',' worker_pid=1'):
            with self.assertRaises(ValueError):self.parse(packet([action+extra],selection='resources'),selection='resources')
        for before,after in [('id=0','id=1'),('members=1','members=2'),('open_handles=0','open_handles=2'),
                             ('retained_receipts=0','retained_receipts=2'),('state="ready"','state="active"'),
                             ('retired=0','retired=1'),('binding="native-communication"','binding="native"'),
                             ('workspace_bytes=0','workspace_bytes=1'),('deadline_ns=0','deadline_ns=1'),('status_code=0','status_code=1')]:
            with self.subTest(after=after),self.assertRaises(ValueError):
                self.parse(packet([row.replace(before,after)],selection='resources'),selection='resources')
        for extra in (' worker_pid=1',' request=1',' endpoint="/private"',' image="/host/path"'):
            with self.assertRaises(ValueError):self.parse(packet([row+extra],selection='resources'),selection='resources')
    def test_actual_communication_formatter(self):
        if FORMATTER is None:self.skipTest('real formatter not supplied')
        rows=self.parse(subprocess.check_output([FORMATTER,'--communication-packet'],text=True),selection='resources')['rows']
        self.assertEqual([row['state']['text'] for row in rows],['ready','retired'])
        self.assertEqual([row['retired'] for row in rows],[False,True])
        self.assertTrue(all(row['id']=='0' and row['members']=='1' and row['retained_receipts']=='1' for row in rows))
    def test_actual_action_formatter(self):
        if FORMATTER is None:self.skipTest('real formatter not supplied')
        rows=self.parse(subprocess.check_output([FORMATTER,'--action-packet'],text=True),selection='resources')['rows']
        self.assertEqual(len(rows),1);self.assertEqual(rows[0]['binding']['text'],'native-action')
        self.assertEqual(rows[0]['open_handles'],'1');self.assertEqual(rows[0]['id'],'0')
    def test_actual_observation_formatter(self):
        if FORMATTER is None:self.skipTest('real formatter not supplied')
        rows=self.parse(subprocess.check_output([FORMATTER,'--observation-packet'],text=True),selection='resources')['rows']
        self.assertEqual([r['binding']['text'] for r in rows],['native-graph','event-stream'])
        self.assertEqual([r['open_handles'] for r in rows],['1','2'])
        self.assertTrue(all(r['id']=='0' and r['retained_receipts']=='0' for r in rows))
        base=ROW.replace('kind=grant','kind=resource').replace('id=18446744073709551615','id=0').replace('admissions=1','admissions=0')
        row=base.replace('type="record"','type="communication"').replace('binding="private"','binding="event-stream"').replace('state=""','state="ready"').replace('members=0','members=1')
        for extra in (' watch_id=1',' source_capability=1',' graph_node="/private"',' source_payload="private"'):
            with self.assertRaises(ValueError):self.parse(packet([row+extra],selection='resources'),selection='resources')
    def test_parameter_closed_schema(self):
        base=ROW.replace('kind=grant','kind=resource').replace('id=18446744073709551615','id=0').replace('admissions=1','admissions=0')
        row=base.replace('type="record"','type="parameter"').replace('binding="private"','binding="local-parameters"').replace('members=0','members=2')
        row=row.replace('open_handles=0','open_handles=1').replace('sample_count=0','sample_count=5').replace('query_count=0','query_count=3')
        row=row.replace('retained_bytes=0','retained_bytes=9007199254740993').replace('workspace_bytes=0','workspace_bytes=18446744073709551615')
        row=row.replace('input_bytes=0','input_bytes=1024').replace('output_items=0','output_items=4')
        value=self.parse(packet([row],selection='resources'),selection='resources')['rows'][0]
        self.assertEqual(value['retained_bytes'],'9007199254740993');self.assertEqual(value['workspace_bytes'],'18446744073709551615')
        for before,after in [('id=0','id=1'),('open_handles=1','open_handles=3'),('state=""','state="ready"'),
                             ('retained_receipts=0','retained_receipts=1'),('binding="local-parameters"','binding="remote"'),
                             ('deadline_ns=0','deadline_ns=1'),('lease_count=0','lease_count=1'),('published=0','published=1')]:
            with self.subTest(after=after),self.assertRaises(ValueError):
                self.parse(packet([row.replace(before,after)],selection='resources'),selection='resources')
        for extra in (' key="secret"',' value=1',' node=1',' cursor=1',' request=1'):
            with self.assertRaises(ValueError):self.parse(packet([row+extra],selection='resources'),selection='resources')
    def test_actual_parameter_formatter(self):
        if FORMATTER is None:self.skipTest('real formatter not supplied')
        rows=self.parse(subprocess.check_output([FORMATTER,'--parameter-packet'],text=True),selection='resources')['rows']
        self.assertEqual(len(rows),1);self.assertEqual(rows[0]['type']['text'],'parameter')
        self.assertEqual(rows[0]['retained_bytes'],'9007199254740993');self.assertEqual(rows[0]['sample_count'],'5')
        self.assertEqual(rows[0]['workspace_bytes'],'18446744073709551615');self.assertEqual(rows[0]['query_count'],'3')
    def test_actual_private_provider_formatter(self):
        if FORMATTER is None:self.skipTest('real formatter not supplied')
        body=subprocess.check_output([FORMATTER,'--private-provider-packet'],text=True)
        rows=self.parse(body,selection='resources')['rows']
        self.assertEqual([row['type']['text'] for row in rows],['recording','replay','sensor','model.proposal'])
        self.assertEqual(rows[0]['retained_bytes'],'9007199254740993');self.assertEqual(rows[2]['status_code'],'5')
        self.assertEqual(rows[3]['workspace_bytes'],'9007199254740993')
    def test_transform_aggregate_closed_schema(self):
        value=self.parse(packet([TRANSFORM],selection='resources'),selection='resources')['rows'][0]
        self.assertEqual(value['sample_count'],'9007199254740993');self.assertEqual(value['query_count'],'18446744073709551615')
        self.assertEqual(value['carrier_bytes'],'18446744073709551615');self.assertEqual(value['id'],'0')
        for before,after in [('id=0','id=1'),('binding="local-transform"','binding="native"'),('open_handles=2','open_handles=4'),
                             ('query_count=18446744073709551615','query_count=1'),('state=""','state="ready"'),
                             ('accepted_bytes=0','accepted_bytes=1'),('retained_receipts=0','retained_receipts=1'),
                             ('generation=0','generation=1'),('deadline_ns=0','deadline_ns=1'),('input_bytes=0','input_bytes=1'),
                             ('lease_count=0','lease_count=1'),('published=0','published=1'),('durable=0','durable=1')]:
            with self.subTest(after=after),self.assertRaises(ValueError):
                self.parse(packet([TRANSFORM.replace(before,after)],selection='resources'),selection='resources')
        for field in ('admissions','refused','sequence','revoked','attenuated','armed'):
            with self.assertRaises(ValueError):self.parse(packet([TRANSFORM.replace(field+'=0',field+'=1')],selection='resources'),selection='resources')
        for field in ('sample_count','query_count','pending_queries','query_bytes','carrier_bytes'):
            current=next(word for word in TRANSFORM.split() if word.startswith(field+'='))
            for malformed in ('-1','01','18446744073709551616'):
                with self.assertRaises(ValueError):self.parse(packet([TRANSFORM.replace(current,field+'='+malformed)],selection='resources'),selection='resources')
            with self.assertRaises(ValueError):self.parse(packet([ROW.replace(field+'=0',field+'=1')]))
            with self.assertRaises(ValueError):self.parse(packet([TRANSFORM.replace(' '+current,'')],selection='resources'),selection='resources')
            with self.assertRaises(ValueError):self.parse(packet([MODEL.replace(field+'=0',field+'=1')],selection='resources'),selection='resources')
        for extra in (' target="map"',' source="camera"',' quaternion="secret"',' translation="secret"',' profile_digest="secret"',' query_id=1',' clock_epoch=1'):
            with self.assertRaises(ValueError):self.parse(packet([TRANSFORM+extra],selection='resources'),selection='resources')
    def test_actual_transform_formatter(self):
        if FORMATTER is None:self.skipTest('real formatter not supplied')
        body=subprocess.check_output([FORMATTER,'--transform-packet'],text=True)
        value=self.parse(body,selection='resources')['rows'][0]
        self.assertEqual(value['sample_count'],'9007199254740993');self.assertEqual(value['query_count'],'18446744073709551615')
        self.assertEqual(value['query_bytes'],'9223372036854775807');self.assertTrue(value['retired'])
    def test_exact_uint64_and_detached_flags(self):
        result=self.parse(packet([ROW]),environment="robot",alias="log")
        self.assertEqual(result["rows"][0]["id"],"18446744073709551615")
        self.assertEqual(result["stopId"],"11")
        self.assertFalse(result["payloadsRead"]);self.assertFalse(result["nativeOperationsIssued"])
    def test_escaped_label_bytes(self):
        result=self.parse(packet([ROW.replace('binding="private"','binding="a\\x0a\\xff\\x22"')]))
        self.assertEqual(result["rows"][0]["binding"]["bytesBase64"],"YQr/Ig==")
    def test_real_cpp_formatter(self):
        if FORMATTER is None:self.skipTest("--formatter not supplied")
        value=subprocess.run([str(FORMATTER),"--packet"],capture_output=True,check=True,timeout=20)
        result=self.parse(value.stdout.decode("ascii"),count=1)
        self.assertEqual(result["rows"][0]["binding"]["bytesBase64"],"Igr/")
        self.assertEqual(result["rows"][0]["id"],"18446744073709551615")
        self.assertTrue(result["more"])
        value=subprocess.run([str(FORMATTER),"--artifact-packet"],capture_output=True,check=True,timeout=20)
        artifact=self.parse(value.stdout.decode("ascii"),selection="resources")['rows'][0]
        self.assertEqual(artifact['size_bytes'],'18446744073709551615')
        self.assertTrue(artifact['immutable']);self.assertEqual(artifact['type']['text'],'artifact')
        value=subprocess.run([str(FORMATTER),"--output-packet"],capture_output=True,check=True,timeout=20)
        rows=self.parse(value.stdout.decode("ascii"),selection="resources")['rows']
        self.assertEqual([row['state']['text'] for row in rows],['staged','published','aborted','removed','none'])
        self.assertEqual(rows[0]['accepted_bytes'],'9007199254740993')
        self.assertEqual(rows[0]['reserved_bytes'],'18446744073709551615')
        self.assertTrue(rows[1]['published']);self.assertTrue(all(not row['durable'] for row in rows))
        self.assertEqual(rows[-1]['id'],'0');self.assertEqual(rows[-1]['retained_receipts'],'1')
        value=subprocess.run([str(FORMATTER),"--lifecycle-packet"],capture_output=True,check=True,timeout=20)
        rows=self.parse(value.stdout.decode('ascii'),selection='resources')['rows']
        self.assertEqual([row['state']['text'] for row in rows],['unconfigured','configuring','configuring','error_processing','finalized'])
        self.assertEqual(rows[1]['generation'],'9007199254740993');self.assertEqual(rows[1]['deadline_ns'],'9223372036854775807')
        self.assertFalse(rows[1]['callback_delivered']);self.assertTrue(rows[2]['callback_delivered'])
        self.assertEqual(rows[-1]['status_code'],'110')
        value=subprocess.run([str(FORMATTER),"--buffer-packet"],capture_output=True,check=True,timeout=20)
        rows=self.parse(value.stdout.decode('ascii'),selection='resources')['rows']
        self.assertEqual(len(rows),1);self.assertEqual(rows[0]['type']['text'],'buffer')
        self.assertEqual(rows[0]['lease_count'],'18446744073709551615')
        self.assertEqual(rows[0]['workspace_bytes'],'9223372036854775807')
        value=subprocess.run([str(FORMATTER),"--model-packet"],capture_output=True,check=True,timeout=20)
        rows=self.parse(value.stdout.decode('ascii'),selection='resources')['rows']
        self.assertEqual(len(rows),1);self.assertEqual(rows[0]['type']['text'],'model')
        self.assertEqual(rows[0]['input_bytes'],'9007199254740993');self.assertEqual(rows[0]['output_items'],'18446744073709551615')
    def test_model_aggregate_closed_schema(self):
        value=self.parse(packet([MODEL],selection='resources'),selection='resources')['rows'][0]
        self.assertEqual(value['input_bytes'],'9007199254740993');self.assertEqual(value['id'],'0')
        self.assertEqual(value['open_handles'],'2');self.assertEqual(value['members'],'1')
        for before,after in [('id=0','id=1'),('binding="local-model"','binding="remote"'),('state=""','state="running"'),
                             ('lease_count=0','lease_count=1'),('retained_receipts=0','retained_receipts=1'),
                             ('generation=0','generation=1'),('accepted_bytes=0','accepted_bytes=1'),('reserved_bytes=0','reserved_bytes=1')]:
            with self.subTest(after=after),self.assertRaises(ValueError):
                self.parse(packet([MODEL.replace(before,after)],selection='resources'),selection='resources')
        for field in ('input_bytes','output_bytes','output_items'):
            for malformed in ('-1','01','18446744073709551616'):
                current=next(word for word in MODEL.split() if word.startswith(field+'='))
                with self.assertRaises(ValueError):self.parse(packet([MODEL.replace(current,field+'='+malformed)],selection='resources'),selection='resources')
            with self.assertRaises(ValueError):self.parse(packet([ROW.replace(field+'=0',field+'=1')]))
            with self.assertRaises(ValueError):self.parse(packet([ROW.replace(' '+field+'=0','')]))
        for extra in (' prompt="secret"',' prediction="secret"',' worker_pid=1',' request_key=1',' model_digest="secret"'):
            with self.assertRaises(ValueError):self.parse(packet([MODEL+extra],selection='resources'),selection='resources')
    def test_buffer_aggregate_closed_schema(self):
        value=self.parse(packet([BUFFER],selection='resources'),selection='resources')['rows'][0]
        self.assertEqual(value['reserved_bytes'],'9007199254740993');self.assertEqual(value['id'],'0')
        self.assertEqual(value['lease_count'],'18446744073709551615');self.assertEqual(value['workspace_bytes'],'9223372036854775807')
        for before,after in [('id=0','id=1'),('binding="sealed-buffer"','binding="native"'),('open_handles=1','open_handles=3'),
                             ('state=""','state="sealed"'),('accepted_bytes=0','accepted_bytes=1'),('retained_receipts=0','retained_receipts=1'),
                             ('generation=0','generation=1'),('size_bytes=0','size_bytes=1'),('immutable=0','immutable=1'),
                             ('published=0','published=1'),('durable=0','durable=1')]:
            with self.subTest(after=after),self.assertRaises(ValueError):
                self.parse(packet([BUFFER.replace(before,after)],selection='resources'),selection='resources')
        for field in ('lease_count','workspace_bytes'):
            for malformed in ('-1','01','18446744073709551616'):
                current=next(word for word in BUFFER.split() if word.startswith(field+'='))
                with self.assertRaises(ValueError):self.parse(packet([BUFFER.replace(current,field+'='+malformed)],selection='resources'),selection='resources')
            with self.assertRaises(ValueError):self.parse(packet([ROW.replace(field+'=0',field+'=1')]))
            with self.assertRaises(ValueError):self.parse(packet([ROW.replace(' '+field+'=0','')]))
        for extra in (' schema="private"',' sha256="private"',' payload="secret"',' native_pointer=1',' lease_count=1'):
            with self.assertRaises(ValueError):self.parse(packet([BUFFER+extra],selection='resources'),selection='resources')
    def test_managed_lifecycle_cached_schema(self):
        value=self.parse(packet([LIFECYCLE],selection='resources'),selection='resources')['rows'][0]
        self.assertTrue(value['callback_pending']);self.assertFalse(value['callback_delivered'])
        self.assertEqual(value['generation'],'9007199254740993');self.assertEqual(value['id'],'0')
        for before,after in [('generation=9007199254740993','generation=0'),('id=0','id=1'),
                             ('binding="local-lifecycle"','binding="native"'),('state="configuring"','state="unknown"'),
                             ('state="configuring"','state="inactive"'),('callback_pending=1','callback_pending=0'),
                             ('transition=1','transition=2'),('transition=1','transition=8'),
                             ('deadline_ns=9223372036854775807','deadline_ns=0'),
                             ('deadline_ns=9223372036854775807','deadline_ns=9223372036854775808'),
                             ('status_code=0','status_code=110'),('reserved_bytes=0','reserved_bytes=1'),
                             ('accepted_bytes=0','accepted_bytes=1'),('retained_receipts=0','retained_receipts=1'),
                             ('published=0','published=1'),('durable=0','durable=1')]:
            with self.subTest(after=after),self.assertRaises(ValueError):
                self.parse(packet([LIFECYCLE.replace(before,after)],selection='resources'),selection='resources')
        terminal=(LIFECYCLE.replace('state="configuring"','state="finalized"').replace('transition=1','transition=0')
                  .replace('deadline_ns=9223372036854775807','deadline_ns=0').replace('callback_pending=1','callback_pending=0'))
        for status in ('0','110','125'):
            self.parse(packet([terminal.replace('status_code=0','status_code='+status)],selection='resources'),selection='resources')
        for before,after in [('status_code=0','status_code=9'),('callback_delivered=0','callback_delivered=1'),('transition=0','transition=1')]:
            with self.assertRaises(ValueError):self.parse(packet([terminal.replace(before,after)],selection='resources'),selection='resources')
        for state,transition in [('inactive',0),('active',0),('unconfigured',0),('cleaning_up',2),('activating',3),
                                 ('deactivating',4),('shutting_down',5),('shutting_down',6),('shutting_down',7),('error_processing',1)]:
            candidate=(LIFECYCLE if transition else terminal).replace('state="configuring"','state="'+state+'"').replace('state="finalized"','state="'+state+'"')
            if transition:candidate=candidate.replace('transition=1','transition='+str(transition))
            self.parse(packet([candidate],selection='resources'),selection='resources')
        for field in ('generation','transition','deadline_ns','status_code','callback_pending','callback_delivered'):
            with self.assertRaises(ValueError):self.parse(packet([ROW.replace(field+'=0',field+'=1')]))
            marker=next(word for word in LIFECYCLE.split() if word.startswith(field+'='))
            with self.assertRaises(ValueError):self.parse(packet([LIFECYCLE.replace(' '+marker,'')],selection='resources'),selection='resources')
        for extra in (' request=1',' request_key=1',' native_node="secret"',' deadline_ns=1'):
            with self.assertRaises(ValueError):self.parse(packet([LIFECYCLE+extra],selection='resources'),selection='resources')
    def test_volatile_output_state_and_receipt_schema(self):
        result=self.parse(packet([OUTPUT,RECEIPT,ABSENT_RECEIPT],selection="resources"),selection="resources")
        live,receipt,absent=result['rows']
        self.assertEqual(live['accepted_bytes'],'9007199254740993')
        self.assertEqual(live['reserved_bytes'],'18446744073709551615')
        self.assertEqual(live['retained_receipts'],'1');self.assertEqual(live['state']['text'],'staged')
        self.assertFalse(live['published']);self.assertFalse(live['durable']);self.assertFalse(live['immutable'])
        self.assertEqual(receipt['state']['text'],'removed');self.assertEqual(receipt['reserved_bytes'],'0')
        self.assertEqual(absent['id'],'0');self.assertEqual(absent['state']['text'],'none')
        published=OUTPUT.replace('state="staged"','state="published"').replace('published=0','published=1')
        self.assertTrue(self.parse(packet([published],selection="resources"),selection="resources")['rows'][0]['published'])
        for row in (OUTPUT.replace('state="staged"','state="unknown"'),OUTPUT.replace('state="staged"','state="none"'),
                    OUTPUT.replace('state="staged"','state="aborted"'),OUTPUT.replace('published=0','published=1'),
                    published.replace('published=1','published=0'),OUTPUT.replace('durable=0','durable=1'),
                    OUTPUT.replace('retained_receipts=1','retained_receipts=2'),OUTPUT.replace('id=18446744073709551615','id=0'),
                    OUTPUT.replace('reserved_bytes=18446744073709551615','reserved_bytes=9007199254740992'),
                    RECEIPT.replace('reserved_bytes=0','reserved_bytes=1'),RECEIPT.replace('retained_receipts=1','retained_receipts=0'),
                    RECEIPT.replace('state="removed"','state="staged"'),RECEIPT.replace('id=18446744073709551615','id=0'),
                    ABSENT_RECEIPT.replace('id=0','id=1'),ABSENT_RECEIPT.replace('accepted_bytes=0','accepted_bytes=1')):
            with self.subTest(row=row),self.assertRaises(ValueError):
                self.parse(packet([row],selection="resources"),selection="resources")
        for field in ('accepted_bytes','reserved_bytes','retained_receipts'):
            for value in ('-1','01','18446744073709551616'):
                with self.assertRaises(ValueError):
                    self.parse(packet([OUTPUT.replace(field+'='+dict(item.split('=',1) for item in OUTPUT.split() if '=' in item)[field],field+'='+value)],selection="resources"),selection="resources")
        for field in ('state="staged"','accepted_bytes=9007199254740993','reserved_bytes=18446744073709551615','retained_receipts=1','published=0','durable=0'):
            with self.assertRaises(ValueError):self.parse(packet([OUTPUT.replace(' '+field,'')],selection="resources"),selection="resources")
        for extra in (' request_key=123',' sha256="private"',' path="private"',' fd=9',' payload="secret"',' published=0'):
            with self.assertRaises(ValueError):self.parse(packet([OUTPUT+extra],selection="resources"),selection="resources")
        for field,value in (('accepted_bytes','1'),('reserved_bytes','1'),('retained_receipts','1'),('published','1'),('durable','1')):
            with self.assertRaises(ValueError):self.parse(packet([ROW.replace(field+'=0',field+'='+value)]))
        with self.assertRaises(ValueError):self.parse(packet([ROW.replace('state=""','state="none"')]))
        with self.assertRaises(ValueError):self.parse(packet([ARTIFACT.replace('accepted_bytes=0','accepted_bytes=1')],selection="resources"),selection="resources")

    def test_immutable_artifact_metadata_is_exact_and_closed(self):
        result=self.parse(packet([ARTIFACT],selection="resources"),selection="resources")
        self.assertEqual(result['rows'][0]['size_bytes'],'18446744073709551615')
        self.assertTrue(result['rows'][0]['immutable'])
        self.assertFalse(result['payloadsRead']);self.assertFalse(result['nativeOperationsIssued'])
        for before,after in [(' size_bytes=18446744073709551615',''),
                             ('size_bytes=18446744073709551615','size_bytes=18446744073709551616'),
                             ('size_bytes=18446744073709551615','size_bytes=01'),
                             (' immutable=1',''),('immutable=1','immutable=0'),('immutable=1','immutable=2'),
                             ('type="artifact"','type="guard"')]:
            with self.subTest(after=after),self.assertRaises(ValueError):
                self.parse(packet([ARTIFACT.replace(before,after)],selection="resources"),selection="resources")
        for extra in (' size_bytes=1',' immutable=1',' path="private"',' fd=4',' sha256="secret"',' content="secret"',' memory=0'):
            with self.subTest(extra=extra),self.assertRaises(ValueError):
                self.parse(packet([ARTIFACT+extra],selection="resources"),selection="resources")
        for before,after in [('size_bytes=0','size_bytes=1'),('immutable=0','immutable=1')]:
            with self.assertRaises(ValueError):self.parse(packet([ROW.replace(before,after)]))
    def test_schema_flags_counts_and_text_are_closed(self):
        for before,after in [(' revoked=0',''),('revoked=0','revoked=2'),('id=18446744073709551615','id=18446744073709551616'),
                             ('sequence=0','sequence=-1'),('members=0','members=00'),('type="record"','type="'+'x'*33+'"'),
                             ('binding="private"','binding="private"[truncated]'),('kind=grant','kind=resource'),
                             ('binding="private"','binding="a\\n"')]:
            with self.subTest(after=after),self.assertRaises(ValueError):self.parse(packet([ROW.replace(before,after)]))
        for row in (ROW+' id=1',ROW+' fd=4',ROW+' cookie="secret"'):
            with self.assertRaises(ValueError):self.parse(packet([row]))
        with self.assertRaises(ValueError):
            self.parse(packet([ROW.replace('binding="private"','binding="'+'x'*255+'"[truncated]')]))
    def test_whole_packet_and_stop_bounds(self):
        for value in (packet()[:-1],packet()+'extra\n',packet().replace('\n','\r\n'),packet().replace('stop=11','stop=12'),
                      packet(selection="resources"),packet().replace('status=available','status=busy'), 'x'*65537):
            with self.assertRaises(ValueError):self.parse(value)
    def test_pagination_and_filters(self):
        value=self.parse(packet([ROW],total=2,next_value=1,more=1),count=1)
        self.assertTrue(value['more'])
        for value in (packet([ROW],total=2),packet([ROW],next_value=0),packet([ROW],more=1),packet(total=4097),packet([ROW,ROW])):
            with self.assertRaises(ValueError):self.parse(value,count=1)
        self.assertEqual(self.parse(packet(total=1),start=2)['rows'],[])
        with self.assertRaises(ValueError):self.parse(packet([ROW]),alias="other")
    def adapter(self,replies):
        class Broker:
            def __init__(self):self.commands=[]
            def request(self,text):self.commands.append(text);return replies.pop(0)
        adapter=dap.Adapter(io.BytesIO());adapter.broker=Broker();return adapter
    def test_query_only_sends_metadata(self):
        adapter=self.adapter([STOP,packet([ROW]),STOP]);result=adapter.query_scene_state({"environment":"robot"})
        self.assertEqual(result['stopId'],'11')
        self.assertEqual(adapter.broker.commands,['status','info scene authority first=0 count=8 if-stop=11 environment=robot','status'])
    def test_running_native_wrong_stop_are_refused_before_query(self):
        for value in ('running\n',STOP.replace('cooperative pause','native instruction step')+'  native-pc=0x12\n'):
            adapter=self.adapter([value])
            with self.assertRaises(ValueError):adapter.query_scene_state({})
            self.assertEqual(adapter.broker.commands,['status'])
        adapter=self.adapter([STOP])
        with self.assertRaises(ValueError):adapter.query_scene_state({'ifStop':'12'})
        self.assertEqual(adapter.broker.commands,['status'])
    def test_changed_stop_and_malformed_copy_retire_references(self):
        for replies in ([STOP,packet(),STOP.replace('stop-id 11','stop-id 12')],[STOP,packet().replace('available','busy')]):
            adapter=self.adapter(list(replies));adapter.ros_scope_stops[1]=('old','queues')
            with self.assertRaises(ValueError):adapter.query_scene_state({})
            self.assertIsNone(adapter.stop_key);self.assertEqual(adapter.ros_scope_stops,{})
    def test_import_policy(self):
        options=dap.scene_state_options({"selection":"policy","operation":"permission.report"})
        first='policy environment="other" operation="permission.report" configured=hidden effective=hidden origin=explicit'
        second='policy environment="robot" operation="permission.report" configured=unset effective=allow origin=self-default'
        packet='scene scope=process-owned stop=9 selection=policy status=available\n'+first+'\n'+second+'\npage total=2 next=2 more=0\n'
        sample=POLICY_PACKET.read_text() if POLICY_PACKET else packet
        self.assertEqual(sample,packet)
        rows=dap.parse_scene_state(sample,options,9)['rows']
        self.assertEqual([(r['environment']['text'],r['effective']) for r in rows],[('other','hidden'),('robot','allow')])
        for bad in (sample.replace(first,second),sample.replace('origin=explicit','origin=default'),
                    sample.replace('effective=allow','effective=hidden'),sample.replace('operation="permission.report"','operation="abi"'),
                    sample.replace('configured=unset','configured=deny'),sample.replace('more=0','more=1'),
                    sample.replace('origin=self-default','origin=self-default alias=x')):
            with self.subTest(bad=bad),self.assertRaises(ValueError):
                dap.parse_scene_state(bad,options,9)
        for bad in ({'selection':'policy','alias':'a'}, {'selection':'audit','operation':'abi'},
                    {'selection':'resources','operation':'abi'}, {'selection':'policy','operation':'a'*65}):
            with self.assertRaises(ValueError):dap.scene_state_options(bad)
    def test_dap_route(self):
        adapter=self.adapter([STOP,packet(),STOP])
        adapter.handle({'seq':1,'command':'uwvm/sceneState','arguments':{}})
        raw=adapter.output.getvalue() if hasattr(adapter,'output') else None
        self.assertEqual(adapter.broker.commands,['status','info scene authority first=0 count=8 if-stop=11','status'])
POLICY_PACKET=None
if __name__=='__main__':
    parser=argparse.ArgumentParser(add_help=False)
    parser.add_argument('--formatter',type=Path)
    parser.add_argument('--policy-packet',type=Path)
    options,rest=parser.parse_known_args()
    POLICY_PACKET=options.policy_packet
    FORMATTER=options.formatter.resolve() if options.formatter else None
    unittest.main(argv=[sys.argv[0],*rest])
