#!/usr/bin/env python3
"""Authored scalar-default regression DATA contracts; not native ROS evidence.

Run only after the complete implementation is frozen. --source-root selects the
actual reviewed checkout; every temporary catalog/output belongs beneath /tmp.
This program imports the selected generator when explicitly executed. Merely
shipping this source does not mean generation, compilation or tests occurred.
The constructed TypeDescription dictionaries are deliberately DATA fixtures,
never original SDK types, actual native exports, RIHS qualification or grants.
"""
import argparse
import copy
import importlib.util
from pathlib import Path
import sys
import tempfile
import unittest


def load(path,name):
    spec=importlib.util.spec_from_file_location(name,path)
    module=importlib.util.module_from_spec(spec);sys.modules[name]=module
    spec.loader.exec_module(module);return module


class ScalarDefaults(unittest.TestCase):
    def fields(self,text,version=2):
        return bounded.parse_fields(parent,text,version,'default_fixture',{},set())

    def test_bool_native_text_and_all_language_initializers(self):
        for source,value in [('true','True'),('FALSE','False'),('1','True'),('0','False')]:
            member=self.fields('bool enabled '+source)[0]
            self.assertEqual(member.native_default,value)
            for language in ('c','rust','javascript','assemblyscript','tinygo','python','java','csharp'):
                expected=value if language=='python' else ('1' if value=='True' else '0') if language=='c' else value.lower()
                self.assertEqual(bounded.scalar_default_expression('bool',value,language),expected)
        for source in ('yes','2','False()','true;','false()'):
            with self.assertRaises(parent.GenerationError):self.fields('bool enabled '+source)

    def test_integer_extremes_are_original_width_not_float(self):
        for bits in (8,16,32,64):
            for unsigned in (False,True):
                typ=('u' if unsigned else '')+'int'+str(bits)
                low=0 if unsigned else -(1<<(bits-1));high=(1<<(bits if unsigned else bits-1))-1
                for value in (low,high):
                    self.assertEqual(self.fields(typ+' value '+str(value))[0].native_default,str(value))
                for value in (low-1,high+1):
                    with self.assertRaises(parent.GenerationError):self.fields(typ+' value '+str(value))
        self.assertEqual(self.fields('uint8 value 0x09')[0].native_default,'9')
        self.assertEqual(self.fields('int16 value -0b100')[0].native_default,'-4')
        self.assertEqual(bounded.scalar_default_expression('int64','-9223372036854775808','c'),
            '(-INT64_C(9223372036854775807)-INT64_C(1))')
        self.assertEqual(bounded.scalar_default_expression('uint64','18446744073709551615','java'),'0xffffffffffffffffL')
        self.assertEqual(bounded.scalar_default_expression('uint64','18446744073709551615','javascript'),'18446744073709551615n')
        self.assertEqual(bounded.scalar_default_expression('uint64','18446744073709551615','csharp'),'18446744073709551615UL')

    def test_finite_float_initialization_preserves_rounding_and_signed_zero(self):
        self.assertEqual(self.fields('float64 value -0.0')[0].native_default,'-0.0')
        self.assertEqual(bounded.scalar_default_expression('float32','-0.0','java'),'Float.intBitsToFloat(0x80000000)')
        self.assertEqual(bounded.scalar_default_expression('float32','1e-50','java'),'Float.intBitsToFloat(0x00000000)')
        self.assertEqual(bounded.scalar_default_expression('float64','1.0','rust'),'f64::from_bits(0x3ff0000000000000)')
        for typ,literal in [('float32','3.5e38'),('float64','1e309'),('float64','nan'),('float64','inf')]:
            with self.assertRaises(parent.GenerationError):self.fields(typ+' value '+literal)

    def test_unknown_default_and_ros1_default_are_refused(self):
        for text in ['uint8[2] value [1,2]','uint8[] value []','string value "abc"',
            'Other value anything','int16 VALUE=2','int16 value 2 extra','float64 value __import__("os")']:
            with self.assertRaises(parent.GenerationError):self.fields(text)
        with self.assertRaises(parent.GenerationError):self.fields('int16 value 0',1)

    def test_future_or_corrupted_optional_ir_is_not_a_new_language_feature(self):
        for typ,value in [('uint8','256'),('bool','false'),('int64','1.0'),('float32','nan'),('string','abc')]:
            with self.assertRaises(ValueError):bounded.scalar_default_expression(typ,value,'java')

    def test_complete_service_source_keeps_all_defaults_and_zero_undeclared_values(self):
        text=(source_root/'sdk/origin/robots/xarm/source-definitions/ros2/xarm_msgs/srv/GetSetModbusData.srv').read_text()
        request,response=text.split('---')
        fields=bounded.parse_fields(parent,request,2,'xarm_msgs',{'modbus_data':{'sequence':64}},set())
        self.assertEqual([(m.name,m.native_default) for m in fields],
            [('modbus_data',None),('modbus_length','0'),('ret_length',None),('host_id','9'),
             ('is_transparent_transmission','False'),('use_503_port','False')])
        tail=bounded.parse_fields(parent,response,2,'xarm_msgs',{'message':{'string':128},'ret_data':{'sequence':64}},set())
        self.assertEqual([m.name for m in tail],['ret','message','ret_data'])

    def test_no_default_schema_ir_remains_without_default_key(self):
        fields=self.fields('int16 value\nbool enabled\n')
        self.assertTrue(all(m.native_default is None for m in fields))

    def test_metadata_erasure_bool_int_confusion_and_protocol_mismatch_are_refused(self):
        # Hand-authored DATA fixture for one actual-shaped service. Hashes here
        # are synthetic and MUST NEVER enter a native profile/qualification.
        name='default_fixture/srv/Select';rq=name+'_Request';rp=name+'_Response';event=name+'_Event'
        def f(label,typeid,nested='',capacity=0,default=''):
            return dict(name=label,type_id=typeid,capacity=capacity,string_capacity=0,nested_type_name=nested,default_value=default)
        declarations={
            name:[f('request_message',1,rq),f('response_message',1,rp),f('event_message',1,event)],
            rq:[f('enabled',15,default='False')],rp:[f('value',4)],
            event:[f('info',1,'service_msgs/msg/ServiceEventInfo'),f('request',97,rq,1),f('response',97,rp,1)],
            'service_msgs/msg/ServiceEventInfo':[f('event_type',3),f('stamp',1,'builtin_interfaces/msg/Time'),f('client_gid',51,capacity=16),f('sequence_number',8)],
            'builtin_interfaces/msg/Time':[f('sec',6),f('nanosec',7)]}
        def record(key):return dict(type=key,fields=copy.deepcopy(declarations[key]))
        def desc(key):
            reach=set()
            def visit(n):
                if n in reach:return
                reach.add(n)
                for field in declarations[n]:
                    if field['nested_type_name']:visit(field['nested_type_name'])
            visit(key)
            return dict(native_identity='RIHS01_'+'1'*64,root=record(key),references=[record(n) for n in sorted(reach-{key})])
        catalog=dict(format='uwvm-ros-custom-bounded-v2',types=[dict(ros_version=2,type=name,file='Select.srv')])
        definitions={'Select.srv':b'bool enabled false\n---\nint16 value\n'}
        original=dict(format='uwvm-ros-official-bounded-native-v2',ros_version=2,
            types={name:dict(official=desc(name),components=dict(request=desc(rq),response=desc(rp)))})
        proof=binder.native_default_contract(catalog,definitions,original)
        self.assertEqual(proof['declared_defaults'],{rq:{'enabled':'False'}})
        self.assertFalse(proof['native_execution_qualified']);self.assertEqual(proof['implicit_permissions'],[])
        # RIHS excludes defaults. Removing a source annotation must still
        # compare the complete original exported metadata and refuse erasure.
        with self.assertRaises(ValueError):
            binder.native_default_contract(catalog,{'Select.srv':b'bool enabled\n---\nint16 value\n'},original)
        for mutation in ('erase','lowercase','missing_default_key','nondict_field','wrong_cap','bool_cap','missing_event','duplicate_ref'):
            value=copy.deepcopy(original);actual=value['types'][name]['official']
            request=next(r for r in actual['references'] if r['type']==rq)
            if mutation=='erase':request['fields'][0]['default_value']=''
            elif mutation=='lowercase':request['fields'][0]['default_value']='false'
            elif mutation=='missing_default_key':del request['fields'][0]['default_value']
            elif mutation=='nondict_field':request['fields'][0]=None
            elif mutation=='wrong_cap':actual['root']['fields'][0]['capacity']=1
            elif mutation=='bool_cap':actual['root']['fields'][0]['capacity']=False
            elif mutation=='missing_event':actual['references']=[r for r in actual['references'] if r['type']!=event]
            else:actual['references'].append(copy.deepcopy(actual['references'][0]))
            with self.assertRaises(ValueError):binder.native_default_contract(catalog,definitions,value)
            if mutation in ('missing_default_key','nondict_field'):
                # Every duplicate actual Request occurrence must be malformed;
                # otherwise an unaffected component's nonempty default alone
                # would trigger proof and fail to isolate the missing-key gate.
                erased=copy.deepcopy(original)
                row=erased['types'][name]
                for description in [row['official'],*row['components'].values()]:
                    for record in [description['root'],*description['references']]:
                        if record['type']==rq:
                            if mutation=='missing_default_key':del record['fields'][0]['default_value']
                            else:record['fields'][0]=None
                with self.assertRaises(ValueError):
                    binder.native_default_contract(catalog,{'Select.srv':b'bool enabled\n---\nint16 value\n'},erased)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root',required=True,type=Path)
    args,other=parser.parse_known_args();source_root=args.source_root.resolve(strict=True)
    if not source_root.is_absolute():raise SystemExit('actual absolute source root required')
    parent=load(source_root/'sdk/origin/tools/custom_types.py','_scalar_defaults_parent')
    bounded=load(source_root/'sdk/origin/custom-types/bounded_profile.py','_scalar_defaults_bounded')
    binder=load(source_root/'test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/bind_catalog.py','_scalar_defaults_binder')
    unittest.main(argv=[sys.argv[0],*other])
