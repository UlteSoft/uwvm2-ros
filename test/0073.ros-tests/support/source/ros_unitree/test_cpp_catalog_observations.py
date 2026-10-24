"""Small real-filesystem/DATA factory for Root's manifest-bound test owner.

No test or project module is imported or executed here. Root supplies both
actual helpers, an owned temporary parent and its current64 guard. These
synthetic catalogs are deliberately not authentic producer/SDK receipts; the
cases exercise ordinary-file/link/pin policy and never qualify a real SDK.
"""
import copy
import json
import os
from pathlib import Path
import tempfile
import types
import unittest


def cases(helper,restart,*,temporary_parent,guard):
    """Return fourteen authored cases; only Root may invoke the test factory."""
    class CatalogObservedTargets(unittest.TestCase):
        def setUp(self):
            guard();self.temp=tempfile.TemporaryDirectory(prefix='cpp-catalog-observed-',dir=temporary_parent)
            self.addCleanup(self.temp.cleanup);self.base=Path(self.temp.name).resolve(strict=True)
            self.selected=self.base/'selected';self.catalogs=self.base/'catalogs';self.source=self.base/'source'
            self.outside=self.base/'outside'
            for directory in (self.selected,self.catalogs,self.source,self.outside):directory.mkdir()
            self.target=self.outside/'value';self.target.write_bytes(b'original complete bytes\n')
            self.link=self.selected/'link';self.link.symlink_to('../outside/value')
            self.old_device=self.selected.stat().st_dev+10000
            self.frame=types.SimpleNamespace(source=self.source,roots=[self.selected,self.catalogs],catalogs=[],guard=guard)
            self.frame.pin=lambda p:restart.opened_file(str(Path(p)),1024**2,guard)
            self.evidence=types.SimpleNamespace(restart=restart,mappings=[{'root':str(self.selected),
                'historical_device':self.old_device,'current_device':self.selected.stat().st_dev}],cache={},bridges=[])
            def pin(old):
                path=Path(old['path'])
                if not any(path.is_relative_to(root) for root in self.frame.roots):
                    raise ValueError('ordinary selected Evidence refuses external compiler input')
                actual=self.frame.pin(path)
                if actual!=old:
                    actual=restart.requalify_persistent_pin(old,str(path),guard,
                        historical_device=old['device'],current_device=actual['device'],observed_current=actual)['current_pin']
                self.evidence.cache[path.as_posix()]=actual;return actual
            self.evidence.pin=pin
        def inventory(self,name,row):
            path=self.catalogs/name;path.write_bytes((json.dumps(row,sort_keys=True,separators=(',',':'))+'\n').encode())
            pin=self.frame.pin(path)
            return {'root':str(self.selected),'files':0,'bytes':0,'symlinks':1,'catalog':pin,'content_sha256':pin['sha256']}
        def rows(self,same_device=False):
            pin=self.frame.pin(self.target);info=self.link.lstat()
            now={'relative':'link','kind':'symlink','target':os.readlink(self.link),'resolved':str(self.target),
                'device':info.st_dev,'inode':info.st_ino,'mtime_ns':info.st_mtime_ns,'ctime_ns':info.st_ctime_ns,'target_file':pin}
            old=copy.deepcopy(now)
            if not same_device:old['device']=old['target_file']['device']=self.old_device
            return old,now
        def pair(self,old,now,ledger):
            a=self.inventory('old.jsonl',old);b=self.inventory('new.jsonl',now);self.frame.catalogs=[b]
            return helper.catalog_pair(self.frame,self.evidence,a,b,observed_targets=ledger)
        def test_exact_external_regular_device_bridge_has_separate_ledger(self):
            old,now=self.rows();ledger={};result=self.pair(old,now,ledger)
            self.assertEqual(result['regular_link_targets_current'],[])
            self.assertEqual(set(ledger),{str(self.target)})
            row=ledger[str(self.target)];self.assertEqual(row['historical_pin'],old['target_file'])
            self.assertEqual(row['current_pin'],now['target_file']);self.assertIsNotNone(row['device_bridge'])
            self.assertFalse(row['parent_directory_authority_granted'])
            self.assertNotIn(str(self.target),self.evidence.cache)
            self.assertEqual(helper.preserve_catalog_observations(self.frame,self.evidence,ledger),[row])
            with self.assertRaisesRegex(ValueError,'external compiler input'):self.evidence.pin(old['target_file'])
        def test_same_device_observation_keeps_actual_pin_without_bridge(self):
            old,now=self.rows(True);ledger={};self.pair(old,now,ledger)
            self.assertIsNone(ledger[str(self.target)]['device_bridge'])
            self.assertEqual(ledger[str(self.target)]['current_pin'],now['target_file'])
        def test_selected_regular_target_retains_ordinary_evidence_accessor(self):
            self.link.unlink();self.target=self.selected/'value';self.target.write_bytes(b'selected')
            self.link.symlink_to('value');old,now=self.rows();ledger={};result=self.pair(old,now,ledger)
            self.assertEqual(ledger,{});self.assertIn(str(self.target),self.evidence.cache)
            self.assertEqual(result['regular_link_targets_current'],[now['target_file']])
        def test_external_observation_requires_explicit_separate_ledger(self):
            old,now=self.rows()
            with self.assertRaisesRegex(ValueError,'separate external catalog observation ledger'):self.pair(old,now,None)
        def test_source_root_device_transition_must_be_explicit(self):
            old,now=self.rows();self.evidence.mappings=[]
            with self.assertRaisesRegex(ValueError,'finite restart mapping'):self.pair(old,now,{})
        def test_changed_catalog_target_hash_is_not_renormalized(self):
            old,now=self.rows();now['target_file']['sha256']='1'*64
            with self.assertRaisesRegex(ValueError,'regular symlink target changed'):self.pair(old,now,{})
        def test_changed_catalog_target_inode_and_times_are_refused(self):
            for key in ('inode','mtime_ns','ctime_ns'):
                old,now=self.rows();now['target_file'][key]+=1
                with self.assertRaisesRegex(ValueError,'regular symlink target changed'):self.pair(old,now,{})
                for p in self.catalogs.iterdir():p.unlink()
        def test_literal_alias_spelling_and_resolved_name_cannot_change(self):
            old,now=self.rows();now['target']='../outside/./value'
            with self.assertRaisesRegex(ValueError,'catalog names'):self.pair(old,now,{})
        def test_actual_target_mutation_after_catalog_is_refused(self):
            old,now=self.rows();self.target.write_bytes(b'changed complete bytes\n')
            with self.assertRaises(ValueError):self.pair(old,now,{})
        def test_actual_source_alias_replacement_is_refused(self):
            old,now=self.rows();self.link.unlink();self.link.symlink_to('../outside/./value')
            with self.assertRaisesRegex(ValueError,'source alias changed'):self.pair(old,now,{})
        def test_directory_cannot_be_observed_as_external_regular_file(self):
            self.target.unlink();self.target.mkdir();facts=self.target.stat();info=self.link.lstat()
            pin={'path':str(self.target),'bytes':facts.st_size,'sha256':'0'*64,'device':facts.st_dev,
                'inode':facts.st_ino,'mtime_ns':facts.st_mtime_ns,'ctime_ns':facts.st_ctime_ns}
            now={'relative':'link','kind':'symlink','target':os.readlink(self.link),'resolved':str(self.target),
                'device':info.st_dev,'inode':info.st_ino,'mtime_ns':info.st_mtime_ns,'ctime_ns':info.st_ctime_ns,'target_file':pin}
            old=copy.deepcopy(now);old['device']=old['target_file']['device']=self.old_device
            with self.assertRaisesRegex(ValueError,'bounded ordinary file'):self.pair(old,now,{})
        def test_external_parent_symlink_cannot_redirect_opened_target(self):
            old,now=self.rows();renamed=self.base/'renamed';self.outside.rename(renamed);self.outside.symlink_to('renamed')
            with self.assertRaisesRegex(ValueError,'external canonical'):self.pair(old,now,{})
        def test_observation_preservation_reopens_content_after_admission(self):
            old,now=self.rows();ledger={};self.pair(old,now,ledger);self.target.write_bytes(b'changed')
            with self.assertRaises(ValueError):helper.preserve_catalog_observations(self.frame,self.evidence,ledger)
        def test_observation_preservation_rechecks_original_source_alias(self):
            old,now=self.rows();ledger={};self.pair(old,now,ledger)
            self.link.unlink();self.link.symlink_to('../outside/./value')
            with self.assertRaisesRegex(ValueError,'source alias changed'):
                helper.preserve_catalog_observations(self.frame,self.evidence,ledger)
    return CatalogObservedTargets
