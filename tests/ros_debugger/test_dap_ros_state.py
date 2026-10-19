"""Source-bound DAP ROS packet/fencing DATA factories; never ROS execution.

Root supplies the current manifest-admitted DAP module and executes these tiny
cases under its guarded owner. The broker below is explicitly a mathematical
packet sequence. Real inherited-console projection is covered separately by
integration_runtime.py; no simulated reply is native/Guest acceptance evidence.
"""
import io
import unittest


STOP = 'stopped: cooperative pause\nstop-id 11\nthread 7 module=0 function=0 byte-offset=0 generation=1\n'
CAPABILITY = ('row kind=0 backend=ros2 instance=3 generation=5 module="main" alias="pub" op="topic.publish" '
    'resource="/example" type="std_msgs/msg/String" enabled=1 alive=1 revoked=0 connected=0 retired=0 '
    'opens=0 active_handles=0 observations=0 requests=0 delivered=0 stored_tokens=4 stored_creation_tokens=2 stored_byte_tokens=100 '
    'audit_records=0 audit_bytes=0 audit_suppressed=0 depth=0 bytes=0 pending=0 pending_bytes=0 reserved=0 reserved_bytes=0 '
    'high_water=0 high_water_bytes=0 enqueued=0 dequeued=0 drop_oldest=0 drop_newest=0 rejected=0 backpressured=0 '
    'admission_refused=0 completed=0 closed=0')
CALL = 'call seq=12 ns=19 backend=ros2 instance=3 generation=5 module="main" function="capability_open" op="topic.publish" alias="pub" errno=13 result_known=1 unresolved=0'


def packet(selection='capabilities', rows=(), total=None, next_value=None, more=0):
    total = len(rows) if total is None else total
    next_value = len(rows) if next_value is None else next_value
    return (f'ros scope=process-owned stop=11 selection={selection} status=available\n'+
        ''.join(row+'\n' for row in rows)+
        f'total={total} next={next_value} more={more} gap=0 overwritten=0 dropped_busy=0\n')


def cases(dap):
    """Bind production parser and query method without importing project code."""
    class Protocol(unittest.TestCase):
        def parse(self, body, **options):
            return dap.parse_ros_state(body,dap.ros_state_options(options),11)
        def test_empty_default_denied_table_remains_empty(self):
            value=self.parse(packet());self.assertEqual(value['rows'],[])
            self.assertFalse(value['payloadsRead']);self.assertFalse(value['nativeOperationsIssued'])
        def test_actual_formatter_capability_schema_and_identity(self):
            row=self.parse(packet(rows=[CAPABILITY]))['rows'][0]
            self.assertEqual((row['instance'],row['generation'],row['backend']),(3,5,'ros2'))
            self.assertEqual(row['module']['text'],'main');self.assertEqual(row['depth'],0)
        def test_metadata_bytes_are_preserved_without_injection(self):
            body=packet(rows=[CAPABILITY.replace('module="main"','module="a\\x0a\\xff\\x22"')])
            value=self.parse(body)['rows'][0]['module']
            self.assertEqual(value['bytesBase64'],'YQr/Ig==');self.assertFalse(value['truncated'])
        def test_explicit_truncation_is_not_silently_complete(self):
            body=packet(rows=[CAPABILITY.replace('module="main"','module="'+('a'*255)+'"[truncated]')])
            self.assertTrue(self.parse(body)['rows'][0]['module']['truncated'])
        def test_resource_redaction_is_a_distinct_value(self):
            row=self.parse(packet(rows=[CAPABILITY.replace('resource="/example"','resource=<redacted>')]))['rows'][0]
            self.assertIsNone(row['resource']);self.assertTrue(row['resourceRedacted'])
        def test_aliases_select_sessions_and_queue_metadata_only(self):
            self.assertEqual(dap.ros_state_options({'selection':'instances'})['selection'],'sessions')
            self.assertEqual(dap.ros_state_options({'selection':'events'})['selection'],'queues')
        def test_trace_cursor_and_error_filter_are_exact(self):
            value=self.parse(packet('trace',[CALL],next_value=12),selection='trace',after=11,error=13)
            self.assertEqual(value['rows'][0]['seq'],12)
        def test_unknown_result_remains_unavailable_and_cannot_match_error_filter(self):
            unknown=CALL.replace('errno=13 result_known=1','errno=0 result_known=0')
            value=self.parse(packet('trace',[unknown],next_value=12),selection='trace')['rows'][0]
            self.assertIsNone(value['errno']);self.assertFalse(value['result_known'])
            with self.assertRaises(ValueError):self.parse(packet('trace',[unknown],next_value=12),selection='trace',error=0)
            with self.assertRaises(ValueError):self.parse(packet('trace',[CALL.replace('result_known=1','result_known=0')],next_value=12),selection='trace')
        def test_boolean_or_out_of_range_request_integers_refused(self):
            for key,value in [('count',True),('instance',0),('start',4097),('count',9),('ifStop',0)]:
                with self.assertRaises(ValueError):dap.ros_state_options({key:value})
        def test_unknown_or_mutating_request_role_refused(self):
            for value in [{'selection':'revoke'},{'grant':'pub'},{'module':'main\ncontinue'},{'selection':'stats','after':1}]:
                with self.assertRaises(ValueError):dap.ros_state_options(value)
        def test_duplicate_or_unknown_packet_field_refused(self):
            for suffix in (' kind=0',' pointer=123'):
                with self.assertRaises(ValueError):self.parse(packet(rows=[CAPABILITY+suffix]))
        def test_wrong_stop_or_selection_refused(self):
            for body in [packet().replace('stop=11','stop=12'),packet('queues')]:
                with self.assertRaises(ValueError):self.parse(body)
        def test_incomplete_or_excessive_page_refused(self):
            for body in [packet(rows=[CAPABILITY],total=2),packet(rows=[CAPABILITY],next_value=0),packet(rows=[CAPABILITY],more=1)]:
                with self.assertRaises(ValueError):self.parse(body)
        def test_nonfinite_quota_or_bad_boolean_refused(self):
            for before,after in [('stored_tokens=4','stored_tokens=NaN'),('alive=1','alive=true')]:
                with self.assertRaises(ValueError):self.parse(packet(rows=[CAPABILITY.replace(before,after)]))
        def test_wrong_backend_instance_or_text_filter_refused(self):
            for options in [{'backend':'ros1'},{'instance':4},{'module':'other'}]:
                with self.assertRaises(ValueError):self.parse(packet(rows=[CAPABILITY]),**options)
        def test_incomplete_row_or_trace_reordering_refused(self):
            with self.assertRaises(ValueError):self.parse(packet(rows=[CAPABILITY.replace(' closed=0','')]))
            with self.assertRaises(ValueError):self.parse(packet('trace',[CALL,CALL],next_value=12),selection='trace')
        def test_wrong_row_kind_cannot_be_relabelled_as_session(self):
            with self.assertRaises(ValueError):self.parse(packet('sessions',[CAPABILITY]),selection='instances')
        def test_trailing_packet_or_control_character_refused(self):
            for body in [packet()+'extra\n',packet().replace('\n','\r\n'),packet()[:-1]]:
                with self.assertRaises(ValueError):self.parse(body)
        def test_noncanonical_quote_or_escape_is_not_raw_text(self):
            for literal in ('"a"b"','"a\\n"','"a\\xGG"'):
                with self.assertRaises(ValueError):self.parse(packet(rows=[CAPABILITY.replace('"main"',literal)]))
        def adapter(self, replies):
            class MathematicalBroker:
                def __init__(self):self.commands=[]
                def request(self,text):self.commands.append(text);return replies.pop(0)
            value=dap.Adapter(io.BytesIO());value.broker=MathematicalBroker();return value
        def test_query_sends_only_current_stop_read_only_command(self):
            value=self.adapter([STOP,packet(),STOP]);result=value.query_ros_state({})
            self.assertEqual(result['stopId'],11);self.assertEqual(value.broker.commands,
                ['status','info ros capabilities backend=all count=8 if-stop=11 first=0','status'])
        def test_running_or_native_stop_never_sends_ros_query(self):
            for stopped in ['running\n',STOP.replace('cooperative pause','native instruction step')+'  native-pc=0x12\n']:
                value=self.adapter([stopped])
                with self.assertRaises(ValueError):value.query_ros_state({})
                self.assertEqual(value.broker.commands,['status'])
        def test_stop_change_during_copy_retires_dap_references(self):
            value=self.adapter([STOP,packet(),STOP.replace('stop-id 11','stop-id 12')]);value.ros_scope_stops[3]=('old','queues')
            with self.assertRaises(ValueError):value.query_ros_state({})
            self.assertIsNone(value.stop_key);self.assertEqual(value.ros_scope_stops,{})
        def test_explicit_retired_stop_assertion_never_rebinds(self):
            value=self.adapter([STOP])
            with self.assertRaises(ValueError):value.query_ros_state({'ifStop':10})
            self.assertEqual(value.broker.commands,['status'])
        def test_scope_reference_is_retired_by_common_invalidation(self):
            value=dap.Adapter(io.BytesIO());value.ros_scope_stops[1]=('original','events');value.invalidate_stop()
            self.assertEqual(value.ros_scope_stops,{})
    return Protocol


def suite(dap):
    return unittest.defaultTestLoader.loadTestsFromTestCase(cases(dap))
