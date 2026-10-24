#!/usr/bin/env python3
"""Authored pure-DATA policies; the guarded Linux owner supplies the module.

No source import, compiler, simulator or Runtime starts here. These cases check
the same public predicates used on actual binaries. The separate real-Wasm
composition_protocol fixture covers provider lifetime/error paths after Root
compiles it with the real preloads; these models do not replace that execution.
"""
import copy
import unittest


def suite(graph):
    def imports(mapping):
        return [{'module':module,'name':name,'kind':0,'params':list(signature[0]),'results':list(signature[1])}
                for (module,name),signature in mapping.items()]
    def binary(name):
        exports={key:{'kind':0,'index':index,'params':list(sig[0]),'results':list(sig[1])}
                 for index,(key,sig) in enumerate(graph.EXPORTS[name].items())}
        exports['memory']={'kind':2,'index':0}
        return {'imports':imports(graph.IMPORTS[name]),'exports':exports,'start_function':None,
                'memories':[{'minimum':2,'maximum':2,'imported':False}]}
    def libraries():
        return [{'name':name,'guest_input':'actual-'+name,'build_input':'receipt-'+name} for name in graph.NAMES]
    def generation():
        names={'bounded_records.'+suffix for suffix in ('h','hpp','rs','go','ts','mjs')} | {
            'bounded_interfaces.h','bounded_interfaces.hpp','manifest.hpp','native_ros1.hpp','native_ros2.hpp'}
        return {'format':'uwvm-ros-custom-bounded-v2','status':'GENERATED_SOURCE_ONLY',
            'native_registry_emitted':True,'implicit_permissions':[],
            'generator_sha256':'a'*64,'bounded_generator_sha256':'b'*64,
            'records':[{'type':'actual/'+str(i),'native_identity':'id'+str(i),'adapter_codec_identity':'codec'+str(i)} for i in range(24)],
            'types':[{'type':'actual/'+str(i),'native_identity':'id'+str(i)} for i in range(24)],
            'outputs':{name:{'bytes':12,'sha256':'c'*64} for name in names},
            'source_inputs':{'catalog':{'bytes':1,'sha256':'d'*64},'definitions':[]}}
    class Policies(unittest.TestCase):
        def test_exact_ordered_graph(self):
            self.assertEqual(graph.libraries(libraries()),libraries())
        def test_missing_provider(self):
            with self.assertRaises(ValueError):graph.libraries(libraries()[:-1])
        def test_reordered_dependency(self):
            with self.assertRaises(ValueError):graph.libraries(list(reversed(libraries())))
        def test_unlisted_provider(self):
            rows=libraries();rows[-1]['name']='camera'
            with self.assertRaises(ValueError):graph.libraries(rows)
        def test_duplicate_artifact_receipt_label(self):
            rows=libraries();rows[1]['guest_input']=rows[0]['build_input']
            with self.assertRaises(ValueError):graph.libraries(rows)
        def test_unknown_graph_field(self):
            rows=libraries();rows[0]['grants']=['topic.publish']
            with self.assertRaises(ValueError):graph.libraries(rows)
        def test_all_three_real_boundary_shapes(self):
            for name in graph.NAMES:
                result=graph.binary(binary(name),name)
                self.assertEqual(result['default_authority'],[]);self.assertFalse(result['Runtime_executed'])
        def test_provider_wasi_import(self):
            row=binary('perception');row['imports']=imports({('wasi_snapshot_preview1','fd_write'):(['i32']*4,['i32'])})
            with self.assertRaises(ValueError):graph.binary(row,'perception')
        def test_provider_ros_import(self):
            row=binary('callbacks');row['imports']=imports({('uwvm_ros2_v1','abi_version'):([],['i32'])})
            with self.assertRaises(ValueError):graph.binary(row,'callbacks')
        def test_provider_imported_memory(self):
            row=binary('motion');row['memories'][0]['imported']=True
            with self.assertRaises(ValueError):graph.binary(row,'motion')
        def test_provider_memory_growth(self):
            row=binary('motion');row['memories'][0]['maximum']=1024
            with self.assertRaises(ValueError):graph.binary(row,'motion')
        def test_provider_implicit_start(self):
            row=binary('callbacks');row['start_function']=0
            with self.assertRaises(ValueError):graph.binary(row,'callbacks')
        def test_provider_extra_export(self):
            row=binary('perception');row['exports']['grant']={'kind':0,'index':12,'params':[],'results':[]}
            with self.assertRaises(ValueError):graph.binary(row,'perception')
        def test_motion_callback_signature(self):
            row=binary('motion');row['imports'][1]['params']=row['imports'][1]['params'][:-1]
            with self.assertRaises(ValueError):graph.binary(row,'motion')
        def test_motion_cannot_import_main(self):
            row=binary('motion');row['imports'][0]['module']='main'
            with self.assertRaises(ValueError):graph.binary(row,'motion')
        def test_nonfunction_provider_import(self):
            row=binary('motion');row['imports'][0]['kind']=1
            with self.assertRaises(ValueError):graph.binary(row,'motion')
        def test_positive_main_exact_scalar_imports(self):
            actual=imports(graph.MAIN_IMPORTS);self.assertEqual(graph.main_imports(actual,True),graph.MAIN_IMPORTS)
        def test_denied_main_has_no_preload_imports(self):
            self.assertEqual(graph.main_imports([],False),{})
            with self.assertRaises(ValueError):graph.main_imports(imports(graph.MAIN_IMPORTS),False)
        def test_positive_main_missing_perception(self):
            rows=[row for row in imports(graph.MAIN_IMPORTS) if row['module']!='perception']
            with self.assertRaises(ValueError):graph.main_imports(rows,True)
        def test_positive_main_duplicate_import(self):
            rows=imports(graph.MAIN_IMPORTS);rows.append(copy.deepcopy(rows[0]))
            with self.assertRaises(ValueError):graph.main_imports(rows,True)
        def test_current_emitter_change_needs_actual_same_output_contract(self):
            old=generation();fresh=copy.deepcopy(old);fresh['bounded_generator_sha256']='e'*64
            self.assertEqual(len(graph.generation_equivalence(old,fresh,'a'*64,'e'*64)),11)
        def test_changed_generated_provider_body_refused(self):
            old=generation();fresh=copy.deepcopy(old);fresh['outputs']['native_ros2.hpp']['sha256']='f'*64
            with self.assertRaises(ValueError):graph.generation_equivalence(old,fresh,'a'*64,'b'*64)
        def test_changed_native_identity_refused(self):
            old=generation();fresh=copy.deepcopy(old);fresh['types'][0]['native_identity']='invented'
            with self.assertRaises(ValueError):graph.generation_equivalence(old,fresh,'a'*64,'b'*64)
        def test_changed_canonical_codec_refused(self):
            old=generation();fresh=copy.deepcopy(old);fresh['records'][0]['adapter_codec_identity']='invented'
            with self.assertRaises(ValueError):graph.generation_equivalence(old,fresh,'a'*64,'b'*64)
        def test_changed_official_definition_inputs_refused(self):
            old=generation();fresh=copy.deepcopy(old);fresh['source_inputs']['catalog']['sha256']='f'*64
            with self.assertRaises(ValueError):graph.generation_equivalence(old,fresh,'a'*64,'b'*64)
        def test_generation_missing_complete_output_refused(self):
            old=generation();fresh=copy.deepcopy(old);fresh['outputs'].pop('bounded_records.rs')
            with self.assertRaises(ValueError):graph.generation_equivalence(old,fresh,'a'*64,'b'*64)
    return unittest.defaultTestLoader.loadTestsFromTestCase(Policies)


def contract_suite(simulation):
    """Four regressions use the actual manifest-bound production contract API.

    Root loads that helper under its guarded test lane; this factory imports no
    project module and starts no actor. In particular, it verifies that the old
    two-provider C++ selection is not silently converted to perception mode.
    """
    def selected(language,names):
        return {'language':language,'controller_mode':'go2-stand','modular':True,
                'libraries':[{'name':name,'guest_input':'module-'+name,'build_input':'receipt-'+name} for name in names]}
    class Contracts(unittest.TestCase):
        def test_historical_cpp_two_provider_contract(self):
            sources,enabled,denied,_,_=simulation.direct_contract({'family':'go'},selected('cpp',('callbacks','motion')))
            self.assertEqual(enabled,{'UWVM_UNITREE_DIRECT_ENABLED':1,'UWVM_UNITREE_MODULAR_MOTION':1})
            self.assertEqual(denied,{'UWVM_UNITREE_DIRECT_ENABLED':0,'UWVM_UNITREE_MODULAR_MOTION':0})
            self.assertIn('sdk/origin/examples/robotics_modular/unitree_motion_abi.h',sources)
            self.assertNotIn('sdk/origin/examples/robotics_modular/unitree_perception_abi.h',sources)
            self.assertNotIn('sdk/origin/examples/robotics_modular/unitree_modular_control.h',sources)
        def test_owned_three_provider_contract_all_languages(self):
            for language in ('c','cpp','javascript'):
                sources,enabled,denied,_,_=simulation.direct_contract({'family':'go'},selected(language,('callbacks','motion','perception')))
                self.assertEqual(enabled['UWVM_UNITREE_MODULAR_PERCEPTION'],1)
                self.assertEqual(denied['UWVM_UNITREE_MODULAR_PERCEPTION'],0)
                self.assertIn('sdk/origin/examples/robotics_modular/unitree_modular_control.h',sources)
        def test_two_provider_not_invented_for_other_languages(self):
            for language in ('c','javascript'):
                with self.assertRaises(ValueError):simulation.direct_contract({'family':'go'},selected(language,('callbacks','motion')))
        def test_unknown_or_reordered_graph_refused(self):
            for names in (('motion','callbacks'),('callbacks','perception','motion'),('callbacks','motion','camera')):
                with self.assertRaises(ValueError):simulation.direct_contract({'family':'go'},selected('cpp',names))
    return unittest.defaultTestLoader.loadTestsFromTestCase(Contracts)
