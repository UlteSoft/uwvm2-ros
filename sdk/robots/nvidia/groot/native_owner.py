"""Explicit trusted native ROS2 -> actual GR00T N1.7 inference owner.

No model, ROS, CUDA or network operation until --enable-native-inference. This
runs OUTSIDE Wasm under the operator's finite whole-process/cgroup owner. It
never forwards SDK reset/kill/model-path/options to the Guest and never actuates.
"""
import argparse
import os
from pathlib import Path
import time
from native_conversion import Refused,observation,prepared_response,selected_modalities
from owned_sdk_source import capture,install


class OncePolicy:
    def __init__(self,policy,np,response_type,tensor_type,deadline,maximum,call_ns):
        self.policy=policy;self.np=np;self.response_type=response_type;self.tensor_type=tensor_type
        self.configuration=selected_modalities(policy)
        self.deadline=deadline;self.call_ns=call_ns
        self.nonces=[0]*maximum;self.codes=[0]*maximum;self.called=[False]*maximum;self.used=0;self.busy=False;self.terminal=False
        self.actual_calls=0;self.last_error=0;self.effect_unknown=False;self.ever_uncertain=False
    def failure(self,nonce,code):
        out=self.response_type();out.application_nonce=nonce if type(nonce) is int and 0<=nonce<2**64 else 0
        out.status=code;out.action=[];out.info_is_empty=False;return out
    def call(self,request,_unused_response):
        nonce=request.application_nonce
        if self.busy:return self.failure(nonce,17)
        if self.terminal:return self.failure(nonce,125)
        if type(nonce) is not int or not 0<nonce<2**64:return self.failure(nonce,22)
        if nonce in self.nonces[:self.used]:return self.failure(nonce,125)
        if self.used==len(self.nonces):return self.failure(nonce,90)
        if time.monotonic_ns()>=self.deadline:return self.failure(nonce,110)
        # Preallocated, nonreusing application transaction history. This nonce
        # is NOT the real ROS request writer/sequence or the Gateway ticket.
        slot=self.used;self.nonces[slot]=nonce;self.used+=1;self.busy=True
        until=min(self.deadline,time.monotonic_ns()+self.call_ns)
        try:
            private=observation(request,self.configuration,self.np)
            if time.monotonic_ns()>=until:raise Refused(110,'deadline before inference')
            self.actual_calls+=1;self.called[slot]=True;self.effect_unknown=True
            action,info=self.policy.get_action(private,None)  # exactly ONCE
            result=prepared_response(nonce,action,info,self.configuration,self.np,self.response_type,self.tensor_type)
            if time.monotonic_ns()>=until:raise Refused(110,'inference completed after finite deadline')
            self.effect_unknown=False
            return result
        except Refused as error:
            if self.effect_unknown:self.ever_uncertain=True;self.terminal=True
            self.codes[slot]=error.code
            if not self.last_error:self.last_error=error.code
            return self.failure(nonce,error.code)
        except Exception:
            # SDK/model faults have no replay or partial tensor success. Native
            # inference may already have happened; keep that uncertainty.
            if self.effect_unknown:self.ever_uncertain=True;self.terminal=True
            self.codes[slot]=5
            if not self.last_error:self.last_error=5
            return self.failure(nonce,5)
        finally:self.busy=False


def parser():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--enable-native-inference',action='store_true')
    p.add_argument('--sdk-root',required=True);p.add_argument('--sdk-source-inventory',required=True)
    p.add_argument('--model-root',required=True);p.add_argument('--embodiment',required=True)
    p.add_argument('--device',required=True);p.add_argument('--service',required=True)
    p.add_argument('--domain',type=int,required=True);p.add_argument('--lifetime-ms',type=int,required=True)
    p.add_argument('--call-ms',type=int,required=True);p.add_argument('--maximum-requests',type=int,required=True)
    return p


def main(argv=None):
    a=parser().parse_args(argv)
    if not a.enable_native_inference:return 13
    if not(0<=a.domain<=232 and 1<=a.lifetime_ms<=60000 and 1<=a.call_ms<=a.lifetime_ms and 1<=a.maximum_requests<=64):return 22
    if not a.service.startswith('/') or '//' in a.service or len(a.service)>255:return 22
    model=Path(a.model_root)
    if not model.is_absolute() or model.resolve(strict=True)!=model or not model.is_dir():return 22
    deadline=time.monotonic_ns()+a.lifetime_ms*1000000
    captured=capture(a.sdk_root,a.sdk_source_inventory);install(captured)
    os.environ['HF_HUB_OFFLINE']='1';os.environ['TRANSFORMERS_OFFLINE']='1'
    # These are genuine native imports. Their full environment, weight files,
    # binary extensions and device must be independently frozen/qualified by
    # the process owner. No remote model identifier or Guest-supplied path.
    import numpy as np
    from gr00t.policy.gr00t_policy import Gr00tPolicy
    import rclpy
    from rclpy.context import Context
    from uwvm_groot_bridge.srv import InferAction
    from uwvm_groot_bridge.msg import FloatTensor
    policy=Gr00tPolicy(a.embodiment,str(model),device=a.device,strict=True)
    owner=OncePolicy(policy,np,InferAction.Response,FloatTensor,deadline,a.maximum_requests,a.call_ms*1000000)
    if time.monotonic_ns()>=deadline:return 110
    context=Context();node=None;service=None;cleanup=[];primary=0
    try:
        rclpy.init(context=context,domain_id=a.domain)
        node=rclpy.create_node('uwvm_groot_inference',context=context)
        service=node.create_service(InferAction,a.service,owner.call)
        while context.ok() and time.monotonic_ns()<deadline and owner.used<a.maximum_requests and not owner.terminal:
            remaining=(deadline-time.monotonic_ns())/1e9
            if remaining<=0:break
            rclpy.spin_once(node,timeout_sec=min(.05,remaining))
        primary=owner.last_error
    except Exception:primary=5
    finally:
        if service is not None:
            try:cleanup.append(('service',bool(node.destroy_service(service))))
            except Exception as e:cleanup.append(('service',e))
        if node is not None:
            try:cleanup.append(('node',node.destroy_node()))
            except Exception as e:cleanup.append(('node',e))
        try:
            if context.ok():context.shutdown();cleanup.append(('context',True))
        except Exception as e:cleanup.append(('context',e))
        # Retain independent real cleanup and inference facts for a caller's
        # harness; never label inference timeout as DDS cancellation or stop.
        main.last={'primary':primary,'cleanup':cleanup,'calls':owner.actual_calls,'unknown':owner.effect_unknown,'ever_uncertain':owner.ever_uncertain}
    return primary or (5 if any(isinstance(v,Exception) or v is False for _,v in cleanup) else 0)


if __name__=='__main__':raise SystemExit(main())
