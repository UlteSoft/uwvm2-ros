"""Authored, UNRUN local whole-value contracts using genuine generated ROS types.

Fake Policy below models only the once-inference application boundary. It is not
an implementation/probe of GR00T, CUDA, ROS transport, TypeSupport or Runtime.
"""
import math
import unittest
from types import SimpleNamespace
from unittest.mock import patch
import numpy as np
from uwvm_groot_bridge.msg import RGBFrame,FloatTensor,TextSequence
from uwvm_groot_bridge.srv import InferAction
from native_conversion import Refused,observation,prepared_response,selected_modalities
from native_owner import OncePolicy,main

class LocalPolicy:
    def __init__(self):self.calls=0;self.fail=False;self.info={}
    def get_modality_config(self):
        return {name:SimpleNamespace(modality_keys=[key],delta_indices=delta) for name,key,delta in [('video','cam',[0]),('state','arm',[0]),('language','task',[0]),('action','arm',[0,1])]}
    def get_action(self,private,options):
        self.calls+=1
        if self.fail:raise RuntimeError('local injected SDK fault')
        return {'arm':np.array([[[1.,2.,3.],[4.,5.,6.]]],dtype=np.float32)},self.info

def request():
    q=InferAction.Request();q.application_nonce=7
    frame=RGBFrame();frame.key='cam';frame.shape=[1,1,2,2,3];frame.pixels=list(range(12));q.video=[frame]
    state=FloatTensor();state.key='arm';state.shape=[1,1,2];state.values=[.25,-.5];q.state=[state]
    text=TextSequence();text.key='task';text.shape=[1,1];text.texts=['move µ'];q.language=[text];return q

class Contracts(unittest.TestCase):
    def setUp(self):self.policy=LocalPolicy();self.configuration=selected_modalities(self.policy)
    def owner(self,maximum=2):return OncePolicy(self.policy,np,InferAction.Response,FloatTensor,10**30,maximum,10**20)
    def test_full_actual_record_to_owned_arrays(self):
        q=request();v=observation(q,self.configuration,np);q.video[0].pixels[0]=99
        self.assertEqual(v['video']['cam'].shape,(1,1,2,2,3));self.assertEqual(int(v['video']['cam'].reshape(-1)[0]),0)
        self.assertEqual(v['state']['arm'].dtype,np.float32);self.assertEqual(v['language']['task'],[['move µ']])
    def test_every_action_and_shape_is_copied(self):
        action,info=self.policy.get_action(None,None);r=prepared_response(7,action,info,self.configuration,np,InferAction.Response,FloatTensor)
        self.assertEqual(r.action[0].shape.tolist(),[1,2,3]);self.assertEqual(list(r.action[0].values),[1.,2.,3.,4.,5.,6.]);action['arm'][0,0,0]=99;self.assertEqual(r.action[0].values[0],1.)
    def test_missing_or_extra_modality_refuses(self):
        q=request();q.video=[]
        with self.assertRaises(Refused) as e:observation(q,self.configuration,np)
        self.assertEqual(e.exception.code,22)
    def test_duplicate_key_refuses(self):
        q=request();q.video=[q.video[0],q.video[0]]
        with self.assertRaises(Refused):observation(q,self.configuration,np)
    def test_nul_text_refuses(self):
        q=request();q.language[0].texts=['native\0value']
        with self.assertRaises(Refused) as e:observation(q,self.configuration,np)
        self.assertEqual(e.exception.code,22)
    def test_complete_video_capacity_refuses90(self):
        q=request();q.video[0].shape=[1,1,17,17,3]
        with self.assertRaises(Refused) as e:observation(q,self.configuration,np)
        self.assertEqual(e.exception.code,90)
    def test_nonfinite_state_refuses_before_model(self):
        q=request();q.state[0].values=[math.nan,0.]
        result=self.owner().call(q,None);self.assertEqual(result.status,22);self.assertEqual(self.policy.calls,0)
    def test_nonempty_info_is_not_erased(self):
        self.policy.info={'new_domain':1};owner=self.owner();r=owner.call(request(),None)
        self.assertEqual(r.status,95);self.assertFalse(r.info_is_empty);self.assertEqual(len(r.action),0);self.assertTrue(owner.terminal)
    def test_once_nonce_no_second_native_attempt(self):
        owner=self.owner();self.assertEqual(owner.call(request(),None).status,0);self.assertEqual(owner.call(request(),None).status,125);self.assertEqual(self.policy.calls,1)
    def test_history_exhaustion_is_not_reuse(self):
        owner=self.owner(1);owner.call(request(),None);q=request();q.application_nonce=8;self.assertEqual(owner.call(q,None).status,90);self.assertEqual(self.policy.calls,1)
    def test_native_fault_terminal_no_new_nonce_replay(self):
        self.policy.fail=True;owner=self.owner();self.assertEqual(owner.call(request(),None).status,5)
        q=request();q.application_nonce=8;self.assertEqual(owner.call(q,None).status,125);self.assertEqual(self.policy.calls,1);self.assertTrue(owner.ever_uncertain)
    def test_expired_before_call_no_model(self):
        owner=self.owner();owner.deadline=0;self.assertEqual(owner.call(request(),None).status,110);self.assertEqual(self.policy.calls,0)
    def test_late_result_terminal_without_partial_output(self):
        owner=self.owner();owner.call_ns=10
        with patch('native_owner.time.monotonic_ns',side_effect=[1,2,3,20]):r=owner.call(request(),None)
        self.assertEqual(r.status,110);self.assertFalse(r.info_is_empty);self.assertEqual(len(r.action),0);self.assertEqual(self.policy.calls,1);self.assertTrue(owner.terminal)
    def test_explicit_default_denial_before_sdk_or_model(self):
        with patch('native_owner.capture',side_effect=AssertionError('no Source open')):
            code=main(['--sdk-root','/absent','--sdk-source-inventory','/absent','--model-root','/absent','--embodiment','absent','--device','cuda','--service','/infer','--domain','0','--lifetime-ms','1','--call-ms','1','--maximum-requests','1'])
        self.assertEqual(code,13)

if __name__=='__main__':unittest.main()
