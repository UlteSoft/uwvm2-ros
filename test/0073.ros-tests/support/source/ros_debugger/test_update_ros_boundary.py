"""Injected DATA regressions for the trusted update/ROS debugger boundary.

The owner must load this file, dap_adapter.py and sdk/origin/tools/abi_contract.py from
one admitted complete source manifest, then call suite(dap, abi.extract).
This factory never imports a project module or starts a Runtime. Broker replies
are mathematical inputs: they prove adapter reference handling, not successful
replacement, native ROS delivery or the security of an IDE/broker transport.
"""
import io
import json
import unittest


STOP = ('stopped: cooperative pause\nstop-id 11\n'
        'thread 7 module=0 function=0 byte-offset=0 generation=1\n')
NEW_STOP = STOP.replace('stop-id 11', 'stop-id 12').replace('generation=1', 'generation=2')
REPLACE = 'replace 0 0 1 /trusted/replacement.body'
SCRIPT = 'wasm-script status; replace 0 0 1 /trusted/replacement.body'
COMMON = frozenset('''abi_version capability_open capability_close capability_revoke
topic_publish topic_subscribe event_poll event_watch event_poll_v1 event_ready
service_submit service_call service_advertise service_respond action_send_goal
action_cancel_own_goal action_cancel_any_goal action_receive_feedback
action_receive_result action_advertise action_respond tf_lookup tf_broadcast
parameter_get parameter_set graph_inspect_topic graph_inspect_node
graph_inspect_service graph_list_topics graph_list_nodes graph_list_services
time_read_ros log_write'''.split())
ROS1_ONLY = frozenset('''parameter_delete parameter_exists parameter_list
parameter_search time_read_wall'''.split())
ROS2_ONLY = frozenset('''parameter_describe parameter_declare parameter_undeclare
parameter_subscribe_events graph_inspect_action lifecycle_get_state
lifecycle_change_state time_read_system time_read_steady parameter_list'''.split())


def cases(dap, extract_abi):
    """Bind the real adapter and real registry extractor supplied by the owner."""
    class Boundary(unittest.TestCase):
        def adapter(self, replies):
            owner = dap.Adapter(io.BytesIO())
            test = self

            class MathematicalBroker:
                def __init__(self):
                    self.commands = []

                def request(self, text):
                    self.commands.append(text)
                    if text.split()[0] in ('replace', 'wasm-script'):
                        # The retirement must precede transport, including a
                        # failure whose publication outcome cannot be known.
                        test.assert_retired(owner)
                    test.assertTrue(replies, 'unexpected extra console command')
                    result = replies.pop(0)
                    if isinstance(result, Exception):
                        raise result
                    return result

            owner.broker = MathematicalBroker()
            owner.observe(STOP, notify=False)
            for index, name in enumerate(('frames', 'scopes', 'wasm_local_scope_stops',
                    'native_register_scopes', 'native_code_refs', 'source_frame_ordinals',
                    'source_scope_stops', 'source_object_views', 'source_scope_frames',
                    'source_pointer_views', 'wasm_scope_stops', 'wasip1_scope_stops',
                    'ros_scope_stops', 'wasm_object_views', 'deep_wasm_paths'), 1):
                getattr(owner, name)[index] = ('retired', 'opaque DATA')
            for name in ('native_frames', 'source_frames', 'inline_frames'):
                getattr(owner, name).add(77)
            owner.source_object_nodes = owner.source_object_bytes = owner.wasm_path_indices = 1
            owner.next_reference = 91
            return owner

        def assert_retired(self, owner):
            self.assertIsNone(owner.stop_key)
            for name in ('frames', 'scopes', 'wasm_local_scope_stops', 'native_frames',
                    'native_register_scopes', 'native_code_refs', 'source_frames',
                    'source_frame_ordinals', 'source_scope_stops', 'source_object_views',
                    'source_scope_frames', 'source_pointer_views', 'wasm_scope_stops',
                    'wasip1_scope_stops', 'ros_scope_stops', 'wasm_object_views',
                    'deep_wasm_paths', 'inline_frames'):
                self.assertFalse(getattr(owner, name), name)
            self.assertEqual((owner.source_object_nodes, owner.source_object_bytes,
                              owner.wasm_path_indices), (0, 0, 0))
            self.assertEqual(owner.next_reference, 91, 'old IDs must not be recycled')

        def test_replace_retires_before_send_and_refreshes_authoritative_stop(self):
            owner = self.adapter(['replacement acknowledged\n', NEW_STOP])
            owner.command(REPLACE)
            self.assertEqual(owner.broker.commands, [REPLACE, 'status'])
            self.assertEqual(owner.threads[0]['generation'], 2)
            self.assertEqual(owner.threads[0]['stop_id'], 12)
            self.assertFalse(owner.ros_scope_stops)
            self.assertEqual(owner.new_reference(), 91)

        def test_rejected_replace_cannot_restore_old_references(self):
            owner = self.adapter(['error: malformed replacement\n'])
            with self.assertRaises(RuntimeError):
                owner.command('replace malformed')
            self.assert_retired(owner)
            self.assertEqual(owner.broker.commands, ['replace malformed'])

        def test_replace_transport_failure_cannot_restore_old_references(self):
            for failure in (OSError('closed'), ValueError('bad frame'), RuntimeError('timeout')):
                owner = self.adapter([failure])
                with self.assertRaises(type(failure)):
                    owner.command(REPLACE)
                self.assert_retired(owner)

        def test_acknowledged_replace_with_no_valid_refresh_remains_retired(self):
            for result in ('running\n', 'stopped: cooperative pause\n', NEW_STOP + NEW_STOP,
                           OSError('refresh closed')):
                owner = self.adapter(['replacement acknowledged\n', result])
                with self.assertRaisesRegex(RuntimeError, 'stop refresh failed'):
                    owner.command(REPLACE)
                self.assert_retired(owner)

        def test_same_pc_and_same_stop_label_with_changed_generation_retire_scope(self):
            owner = self.adapter(['ack\n', STOP.replace('generation=1', 'generation=2')])
            previous = owner.stop_key
            owner.command(REPLACE)
            self.assertNotEqual(owner.stop_key, previous)
            self.assertFalse(owner.ros_scope_stops)

        def test_old_ros_scope_cannot_rebind_to_replacement_cohort(self):
            owner = self.adapter(['ack\n', NEW_STOP, NEW_STOP])
            previous = owner.stop_key
            owner.command(REPLACE)
            with self.assertRaisesRegex(ValueError, 'retired stop'):
                owner.query_ros_state({}, expected_stop=previous)
            self.assertEqual(owner.broker.commands, [REPLACE, 'status', 'status'])

        def test_script_transcript_does_not_publish_a_stop_or_revive_references(self):
            owner = self.adapter(['replacement acknowledged\n' + NEW_STOP])
            owner.command(SCRIPT)
            self.assert_retired(owner)
            self.assertEqual(owner.broker.commands, [SCRIPT])

        def test_rejected_or_broken_script_keeps_references_retired(self):
            for result in ('error: rejected script\n', OSError('script closed')):
                owner = self.adapter([result])
                with self.assertRaises((RuntimeError, OSError)):
                    owner.command(SCRIPT)
                self.assert_retired(owner)

        def test_command_like_stdout_is_only_a_length_delimited_output_event(self):
            owner = self.adapter([])
            previous = owner.stop_key
            text = 'replace 0 0 1 /guest/malicious.body\nwasm-script status; info ros stats\ncontinue\n'
            owner.event('output', {'category': 'stdout', 'output': text})
            raw = owner.output.getvalue()
            header, body = raw.split(b'\r\n\r\n', 1)
            self.assertEqual(header, b'Content-Length: ' + str(len(body)).encode('ascii'))
            message = json.loads(body)
            self.assertEqual(message['event'], 'output')
            self.assertEqual(message['body']['output'], text)
            self.assertEqual(owner.broker.commands, [])
            self.assertEqual(owner.stop_key, previous)
            # This tests serialization only; real Runtime fd separation needs
            # the authenticated integration lane, never this invented broker.

        def test_ros1_raw_registry_is_the_closed_business_import_set(self):
            actual = extract_abi()['modules']['uwvm_ros1_v1']
            names = [row['name'] for row in actual['functions']]
            self.assertEqual(set(names), COMMON | ROS1_ONLY)
            self.assertEqual(len(names), len(set(names)))
            self.assertEqual(actual['function_count'], 38)

        def test_ros2_raw_registry_is_the_closed_business_import_set(self):
            actual = extract_abi()['modules']['uwvm_ros2_v1']
            names = [row['name'] for row in actual['functions']]
            self.assertEqual(set(names), COMMON | ROS2_ONLY)
            self.assertEqual(len(names), len(set(names)))
            self.assertEqual(actual['function_count'], 43)

        def test_registry_has_no_management_debug_or_update_import(self):
            actual = extract_abi()
            self.assertEqual(set(actual['modules']), {'uwvm_ros1_v1', 'uwvm_ros2_v1'})
            for module in actual['modules'].values():
                for row in module['functions']:
                    self.assertFalse(any(word in row['name'] for word in
                                         ('debug', 'update', 'replace', 'management', 'retire')))
                    self.assertEqual(row['results'], ['i32'])

    return Boundary


def suite(dap, extract_abi):
    return unittest.defaultTestLoader.loadTestsFromTestCase(cases(dap, extract_abi))
