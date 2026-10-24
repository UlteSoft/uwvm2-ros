"""Optional service-client debugger DATA cases, never ROS execution.

Root injects the admitted current DAP helper into cases(); no project helper is
imported by this factory and no mock metadata grants actual debug authority.
"""
import unittest

CAPABILITY = 'row kind=0 backend=ros2 instance=3 generation=5 module="main" alias="pub" op="topic.publish" resource="/example" type="std_msgs/msg/String" enabled=1 alive=1 revoked=0 connected=0 retired=0 opens=0 active_handles=0 observations=0 requests=0 delivered=0 stored_tokens=4 stored_creation_tokens=2 stored_byte_tokens=100 audit_records=0 audit_bytes=0 audit_suppressed=0 depth=0 bytes=0 pending=0 pending_bytes=0 reserved=0 reserved_bytes=0 high_water=0 high_water_bytes=0 enqueued=0 dequeued=0 drop_oldest=0 drop_newest=0 rejected=0 backpressured=0 admission_refused=0 completed=0 closed=0'
CALL = 'call seq=12 ns=19 backend=ros2 instance=3 generation=5 module="main" function="capability_open" op="topic.publish" alias="pub" errno=13 result_known=1 unresolved=0'

def packet(selection, rows):
    trace=selection=='trace';next_value=12 if trace and rows else len(rows)
    return (f'ros scope=process-owned stop=11 selection={selection} status=available\n'+
            ''.join(row+'\n' for row in rows)+
            f'total={len(rows)} next={next_value} more=0 gap=0 overwritten=0 dropped_busy=0\n')


def cases(dap):
    class Protocol(unittest.TestCase):
        def parse(self, rows, selection='trace'):
            return dap.parse_ros_state(packet(selection,rows),dap.ros_state_options({'selection':selection}),11)
        def feature(self):
            return CALL.replace('function="capability_open" op="topic.publish" alias="pub" errno=13',
                'function="client.features" op="service.available" alias="available" source_alias="call" client_session_label=3 client_features=7 errno=0')
        def test_original_packet_stays_valid(self):
            self.assertEqual(self.parse([CALL])['rows'][0]['errno'],13)
        def test_successful_feature_and_source_are_preserved(self):
            row=self.parse([self.feature()])['rows'][0]
            self.assertEqual(row['source_alias']['text'],'call');self.assertEqual(row['client_session_label'],3);self.assertEqual(row['client_features'],7)
        def test_query_error_has_no_feature_claim(self):
            line=self.feature().replace('client.features','client.service_available').replace(' client_features=7','').replace('errno=0','errno=110')
            row=self.parse([line])['rows'][0];self.assertNotIn('client_features',row);self.assertEqual(row['errno'],110)
        def test_incomplete_or_unknown_fields_refuse(self):
            for line in (self.feature().replace(' source_alias="call"',''),self.feature().replace(' client_session_label=3',''),self.feature()+' management=1'):
                with self.assertRaises(ValueError):self.parse([line])
        def test_wrong_operation_or_source_refuses(self):
            for line in (self.feature().replace('service.available','topic.publish'),self.feature().replace('source_alias="call"','source_alias="available"'),self.feature().replace('source_alias="call"','source_alias="bad\\x0aname"')):
                with self.assertRaises(ValueError):self.parse([line])
        def test_feature_requires_success_and_actual_backend_mask(self):
            for line in (self.feature().replace('result_known=1','result_known=0'),self.feature().replace('errno=0','errno=13'),self.feature().replace('client_features=7','client_features=11'),self.feature().replace('client.features','client.service_available')):
                with self.assertRaises(ValueError):self.parse([line])
        def test_new_session_metadata_and_old_session_both_parse(self):
            old=CAPABILITY.replace('kind=0','kind=1');self.assertEqual(self.parse([old],'sessions')['rows'][0]['kind'],1)
            row=self.parse([old+' client_bindings=4 client_session_label=3'],'sessions')['rows'][0]
            self.assertEqual(row['client_bindings'],4)
        def test_session_metadata_is_closed_and_bounded(self):
            base=CAPABILITY.replace('kind=0','kind=1')
            for suffix in (' client_bindings=0 client_session_label=3',' client_bindings=65537 client_session_label=3',' client_bindings=4',' client_features=7'):
                with self.assertRaises(ValueError):self.parse([base+suffix],'sessions')
        def test_new_metadata_cannot_appear_on_non_session_row(self):
            with self.assertRaises(ValueError):self.parse([CAPABILITY+' client_bindings=4 client_session_label=3'],'capabilities')
    return unittest.defaultTestLoader.loadTestsFromTestCase(Protocol)
