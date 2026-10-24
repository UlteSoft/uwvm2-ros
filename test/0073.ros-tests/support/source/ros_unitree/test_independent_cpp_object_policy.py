"""Small DATA policy factory, invoked only by Root's source-pinned test owner.

Call cases(actual_manifest_bound_helper) and run that returned TestCase under
its existing current64 guard. Importing this file starts nothing. These cases
exercise refusal/association predicates, never forge an authentic producer,
compile an object or qualify a real SDK/ROS/Guest product.
"""
import copy
import unittest


def cases(helper):
    """Bind the real selected helper, with no ambient candidate module import."""
    class IndependentCppPolicy(unittest.TestCase):
        def terminal(self):
            return {'kind':'wasi-cpp-official-termination-overlay',
                'status':'PASS_OFFICIAL_CPP_TERMINATION_OBJECT_PREPARATION_ONLY','runtime_executed':False,
                'SDK_modified':False,'granted_rights':[],'source_preserved':True,
                'input_pins_preserved':True,'whole_roots_preserved':True,'overlay_products_preserved':True,
                'cleanup':[{'status':'PASS','remaining':[]}],'overlay':{'marker':'DATA_ONLY_NOT_AN_OBJECT'}}
        def file_row(self):
            return {'relative':'include/a.h','kind':'file','sha256':'1'*64,'bytes':8,
                'device':81,'inode':7,'mtime_ns':11,'ctime_ns':12,'executable':False}
        def test_terminal_predicate_returns_only_declared_data_section(self):
            r=self.terminal();self.assertIs(helper.terminal(r),r['overlay'])
        def test_running_or_failed_main_status_cannot_replace_object_terminal(self):
            for status in ('RUNNING','FAIL','PASS_SOURCE40_FOURTEEN_COMMAND_PRODUCTS_NINE_RETAINED_FIVE_FRESH_BINARY_VALIDATED_ONLY'):
                r=self.terminal();r['status']=status
                with self.assertRaisesRegex(ValueError,'terminal standalone'):helper.terminal(r)
        def test_preservation_and_no_execution_are_exact_true_false_facts(self):
            for key,value in (('source_preserved',False),('whole_roots_preserved',1),('runtime_executed',True),('SDK_modified',True),('granted_rights',['topic.publish'])):
                r=self.terminal();r[key]=value
                with self.assertRaisesRegex(ValueError,'terminal standalone'):helper.terminal(r)
        def test_cleanup_failure_and_live_descendant_refused(self):
            for row in ({'status':'FAIL','remaining':[]},{'status':'PASS','remaining':[{'pid':4}]},{'status':'PASS','remaining':[],'errors':['error']}):
                r=self.terminal();r['cleanup']=[row]
                with self.assertRaisesRegex(ValueError,'owned cleanup'):helper.terminal(r)
        def test_device_rebind_needs_exact_explicit_mapping(self):
            a=self.file_row();b=copy.deepcopy(a);b['device']=68
            with self.assertRaisesRegex(ValueError,'finite restart mapping'):helper.compare_row(a,b,'/owned/sdk',[])
            helper.compare_row(a,b,'/owned/sdk',[{'root':'/owned/sdk','historical_device':81,'current_device':68}])
        def test_mapping_does_not_admit_changed_hash_inode_time_or_path(self):
            a=self.file_row();mapping=[{'root':'/owned/sdk','historical_device':81,'current_device':68}]
            for key,value in (('sha256','2'*64),('inode',8),('mtime_ns',13),('relative','include/b.h')):
                b=copy.deepcopy(a);b['device']=68;b[key]=value
                with self.assertRaisesRegex(ValueError,'catalog names'):helper.compare_row(a,b,'/owned/sdk',mapping)
        def test_unselected_ancestor_and_relative_escape_cannot_bridge(self):
            a=self.file_row();b=copy.deepcopy(a);b['device']=68
            with self.assertRaisesRegex(ValueError,'finite restart mapping'):
                helper.compare_row(a,b,'/other/sdk',[{'root':'/owned/sdk','historical_device':81,'current_device':68}])
            a['relative']=b['relative']='../escape'
            with self.assertRaisesRegex(ValueError,'safe catalog'):helper.compare_row(a,b,'/owned/sdk',[])
        def test_symlink_literal_and_regular_target_identity_are_not_normalized(self):
            target={'path':'/owned/sdk/a','bytes':1,'sha256':'1'*64,'device':81,'inode':9,'mtime_ns':2,'ctime_ns':3}
            a={'relative':'alias','kind':'symlink','target':'a','resolved':'/owned/sdk/a',
                'device':81,'inode':8,'mtime_ns':2,'ctime_ns':3,'target_file':target}
            b=copy.deepcopy(a);b['target']='./a'
            with self.assertRaisesRegex(ValueError,'catalog names'):helper.compare_row(a,b,'/owned/sdk',[])
            b=copy.deepcopy(a);b['target_file']['inode']=19
            with self.assertRaisesRegex(ValueError,'regular symlink target'):helper.compare_row(a,b,'/owned/sdk',[])
        def test_incomplete_or_new_catalog_fields_are_not_silently_dropped(self):
            a=self.file_row();b=copy.deepcopy(a);del b['sha256']
            with self.assertRaisesRegex(ValueError,'complete original'):helper.compare_row(a,b,'/owned/sdk',[])
        def command(self):
            obj={'path':'/owned/out/object.o'};dep={'path':'/owned/out/object.d'};source={'path':'/owned/llvm/libcxxabi/src/abort_message.cpp'}
            argv=['/owned/sdk/bin/clang++','--driver-mode=g++','--target=wasm32-wasip1','--sysroot=/owned/sdk/sysroot',
                '-std=c++20','-O1','-g0','-fno-builtin','-fno-exceptions','-fno-rtti',
                '-DLIBCXXABI_BAREMETAL','-DNDEBUG','-I/owned/llvm/libcxxabi/include','-MD','-MF',dep['path'],'-MT',obj['path'],'-c',source['path'],'-o',obj['path']]
            proof={'policy':helper.POLICY,'Guest_compile_flags':['-UNDEBUG',helper.TRAP],
                'official':{'commit':helper.COMMIT,'no_assertion_site_in_compiled_TU':True},
                'runtime_executed':False,'SDK_modified':False,'compiler':{'path':'/owned/sdk/bin/clang++','physical':'/owned/sdk/bin/clang-22'},
                'sysroot':'/owned/sdk/sysroot','compile':{'returncode':0,'argv':argv,'cwd':'/owned/out'}}
            return proof,obj,dep,source
        def test_partial_MD_duplicate_roles_and_wrong_dep_target_refused(self):
            proof,obj,dep,source=self.command()
            for change in ('partial','duplicate','foreign-target'):
                bad=copy.deepcopy(proof)
                if change=='partial':bad['compile']['argv'][bad['compile']['argv'].index('-MD')]='-MMD'
                elif change=='duplicate':bad['compile']['argv']+=['-MT',obj['path']]
                else:bad['compile']['argv'][bad['compile']['argv'].index('-MF')+1]='/other/object.d'
                with self.assertRaisesRegex(ValueError,'full-MD compile association'):
                    helper.compile_association(bad,obj,dep,source,'/owned/sdk/bin/clang-22','/owned/sdk/sysroot')
        def test_wrong_official_commit_compiler_or_sysroot_refused(self):
            proof,obj,dep,source=self.command()
            for change in ('commit','compiler','sysroot'):
                bad=copy.deepcopy(proof)
                if change=='commit':bad['official']['commit']='0'*40
                elif change=='compiler':bad['compiler']['physical']='/ambient/clang'
                else:bad['sysroot']='/ambient/sysroot'
                with self.assertRaisesRegex(ValueError,'policy/compiler/sysroot'):
                    helper.compile_association(bad,obj,dep,source,'/owned/sdk/bin/clang-22','/owned/sdk/sysroot')
    return IndependentCppPolicy
