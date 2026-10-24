"""Strict detached ROS metric packet contracts; not live ROS execution."""
import unittest

ROW = ('interface backend=ros2 instance=7 generation=3 module="main" alias="pub" op="topic.publish" function="publish" '
       'calls=2 errors=1 incomplete=0 timed=2 total_ns=80 min_ns=30 max_ns=50 window_ns=1000000000 unresolved=0 saturated=0')
def packet(row=ROW):
    return ('ros scope=process-owned stop=11 selection=calls status=available\n'+row+'\n'
            'total=1 next=1 more=0 gap=0 overwritten=0 dropped_busy=0 dropped_full=0 dropped_labels=0\n')
def suite(dap):
    class Contract(unittest.TestCase):
        def parse(self, text, **options):
            return dap.parse_ros_state(text,dap.ros_state_options(dict(selection='calls',**options)),11)
        def test_rate_and_mean_are_from_integer_counts_and_window(self):
            r=self.parse(packet())['rows'][0];self.assertEqual(r['callsPerSecond'],2);self.assertEqual(r['meanNanoseconds'],40)
        def test_unknown_timing_and_zero_window_are_unavailable(self):
            r=self.parse(packet(ROW.replace('timed=2 total_ns=80 min_ns=30 max_ns=50 window_ns=1000000000',
                'timed=0 total_ns=0 min_ns=0 max_ns=0 window_ns=0')))['rows'][0]
            self.assertIsNone(r['callsPerSecond']);self.assertIsNone(r['meanNanoseconds'])
        def test_saturation_never_presents_precise_mean_or_rate(self):
            r=self.parse(packet(ROW.replace('saturated=0','saturated=1')))['rows'][0]
            self.assertIsNone(r['callsPerSecond']);self.assertIsNone(r['meanNanoseconds'])
        def test_impossible_counters_and_trace_loss_are_refused(self):
            for a,b in [('errors=1','errors=3'),('timed=2','timed=3'),('min_ns=30','min_ns=51'),
                        ('total_ns=80','total_ns=49'),('instance=7','instance=0'),('calls=2','calls=0'),
                        ('gap=0','gap=1'),('overwritten=0','overwritten=1'),('dropped_full=0','dropped_full=-1')]:
                with self.assertRaises(ValueError):self.parse(packet().replace(a,b))
        def test_missing_unknown_duplicate_columns_refused(self):
            for text in [packet().replace(' dropped_labels=0',''),packet(ROW+' native_pointer=1'),packet(ROW+' calls=2')]:
                with self.assertRaises(ValueError):self.parse(text)
        def test_incomplete_outcomes_are_bounded_and_do_not_become_errors(self):
            r=self.parse(packet(ROW.replace('incomplete=0','incomplete=1')))['rows'][0]
            self.assertEqual(r['incomplete'],1);self.assertEqual(r['errors'],1)
            with self.assertRaises(ValueError):self.parse(packet(ROW.replace('incomplete=0','incomplete=2')))
        def test_saturated_outcome_counters_are_lower_bounds(self):
            row=ROW.replace('calls=2 errors=1 incomplete=0', 'calls=18446744073709551615 errors=18446744073709551615 incomplete=1').replace('saturated=0','saturated=1')
            value=self.parse(packet(row))['rows'][0]
            self.assertEqual(value['incomplete'],1);self.assertIsNone(value['callsPerSecond'])
            with self.assertRaises(ValueError):self.parse(packet(row.replace('saturated=1','saturated=0')))
        def test_read_only_filter_grammar(self):
            self.assertEqual(self.parse(packet(),function='publish',operation='topic.publish')['rows'][0]['calls'],2)
            for args in [dict(selection='calls',after=1),dict(selection='calls',error=0),dict(selection='stats',function='publish')]:
                with self.assertRaises(ValueError):dap.ros_state_options(args)
    return unittest.defaultTestLoader.loadTestsFromTestCase(Contract)
