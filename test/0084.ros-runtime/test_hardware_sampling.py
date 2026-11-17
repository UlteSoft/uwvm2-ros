#!/usr/bin/env python3
"""Prevent successful-but-clamped profiler results from qualifying 10 us."""
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'tools/ros_scene'))
from hardware_sampling import audit_samples, reference_frequency, vtune_periods
from run_scene_vm import verify_gate

EVENT='CPU_CLK_UNHALTED.REF_TSC'
ATTR=EVENT+': type: 4 (cpu_core), { sample_period, sample_freq }: 24000, sample_type: IP|TID|TIME, exclude_kernel: 1'
SAMPLES='\n'.join(f'uwvm2-ros 9/9 1.{t:09d}: 24000 {EVENT}: abc' for t in (10000,20000,30000))
RAW='Time Shift      31\nTime Multiplier 894784853\n'+': PERF_RECORD_SAMPLE(IP)\n'*6


class SamplingEvidence(unittest.TestCase):
    def audit(self,attr=ATTR,samples=SAMPLES,raw=RAW):
        return audit_samples(attr,samples,raw,{EVENT:24000},2400000000)

    def test_period_and_observed_spacing(self):
        evidence=self.audit()
        self.assertTrue(evidence['ten_us_reference_period_verified'])
        self.assertEqual(evidence['events'][EVENT]['wall_spacing_ns_p50'],10000)
        self.assertFalse(evidence['fixed_wall_interval_guaranteed'])

    def test_request_is_not_period_evidence(self):
        for attrs,samples in [(ATTR.replace('24000','100000'),SAMPLES),
                              (ATTR,SAMPLES.replace('24000','100000')),(ATTR,''),('',SAMPLES),
                              (ATTR+', freq: 1',SAMPLES),(ATTR.replace('exclude_kernel: 1','exclude_kernel: 0'),SAMPLES)]:
            self.assertFalse(self.audit(attrs,samples)['periods_verified'])

    def test_drop_and_throttle_rejected(self):
        for kind in ('PERF_RECORD_LOST','PERF_RECORD_LOST_SAMPLES','PERF_RECORD_THROTTLE','PERF_RECORD_UNTHROTTLE'):
            self.assertFalse(self.audit(raw=RAW+'1 100 0x10 [0x30]: '+kind+' 10')['periods_verified'])

    def test_timestamps_are_per_thread(self):
        other=SAMPLES.replace('9/9','9/10').replace('1.','2.')
        evidence=self.audit(samples=SAMPLES+'\n'+other)
        self.assertEqual(evidence['events'][EVENT]['sample_count'],6)
        self.assertEqual(evidence['events'][EVENT]['wall_spacing_ns_p95'],10000)

    def test_native_reference_requires_its_own_samples(self):
        samples=SAMPLES.replace('uwvm2-ros','native')
        evidence=audit_samples(ATTR,samples,RAW,{EVENT:24000},2400000000,target_comm='native')
        self.assertTrue(evidence['periods_verified'])
        self.assertEqual(evidence['events'][EVENT]['sample_count'],3)
        self.assertFalse(self.audit(samples=samples)['periods_verified'])

    def test_missing_reference_is_not_ten_us(self):
        result=audit_samples(ATTR,SAMPLES,RAW,{EVENT:24000},0)
        self.assertTrue(result['periods_verified'])
        self.assertFalse(result['ten_us_reference_period_verified'])

    def test_missing_raw_or_wrong_frequency(self):
        self.assertFalse(self.audit(raw='')['periods_verified'])
        result=audit_samples(ATTR,SAMPLES,RAW,{EVENT:24000},3000000000)
        self.assertFalse(result['ten_us_reference_period_verified'])

    def test_unparsed_sample_fails(self):
        with self.assertRaises(ValueError):self.audit(samples='unparsed record')

    def test_vtune_clamp_and_frequency_provenance(self):
        with tempfile.TemporaryDirectory() as temporary:
            root=Path(temporary);(root/'data.0').mkdir()
            (root/'data.0/perfcmd').write_text('perf record -e cpu_core/period=100000/')
            (root/'data.0/sep.events').write_text('Warning: sav for event '+EVENT+' is too low, value reset to 100000\n')
            self.assertEqual(vtune_periods(root)['actual_perf_periods'],[100000])
            self.assertEqual(len(vtune_periods(root)['adjustments']),1)
            context=root/'context.xml'
            context.write_text('<root xmlns:n="urn:example"><contextValue id="referenceFrequency" n:value="2400000000"/></root>')
            self.assertEqual(reference_frequency(context),2400000000)
            with self.assertRaises(ValueError):reference_frequency(context,'target-host')
            context.write_text('<root><contextValue id="hostName" value="target-host"/><contextValue id="computerName" value="target-host"/><contextValue id="referenceFrequency" value="2400000000"/></root>')
            self.assertEqual(reference_frequency(context,'target-host'),2400000000)
            with self.assertRaises(ValueError):reference_frequency(context,'another-host')
            context.write_text('<root><contextValue id="referenceFrequency" value=""/></root>')
            with self.assertRaises(ValueError):reference_frequency(context)


if __name__=='__main__':
    verify_gate()
    unittest.main()
