"""Actual SDK 5.0.1 native denial/identity contracts, with no network or mocks.

This does not execute a Wasm Python Guest or validate actual robot leases.
"""
import argparse
from importlib.metadata import version
import json
from pathlib import Path
import sys
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--dependencies', required=True, type=Path)
    parser.add_argument('--source', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    sys.path[:0] = [str(args.dependencies), str(args.source/'sdk/origin/robots')]
    from bosdyn.client.lease import LeaseClient, LeaseWallet
    from bosdyn.client.robot_command import RobotCommandClient
    from bosdyn.client.robot_state import RobotStateClient
    from mobile_manipulation.common.native_gate import NativeGate, SERVICES
    from mobile_manipulation.spot.native_provider import SpotProvider
    from mobile_manipulation.spot.native_sessions import SpotNativeSessions

    def provider(wallet=None):
        wallet = LeaseWallet() if wallet is None else wallet
        command = RobotCommandClient()
        command.lease_wallet = wallet
        return SpotProvider(RobotStateClient(),LeaseClient(wallet),command,gate=NativeGate())

    rows = []

    def refused(name, kind, call, errno=None):
        try:
            call()
        except kind as error:
            if errno is not None and error.errno != errno:
                raise
            rows.append(dict(name=name,status='PASS',exception=type(error).__name__,errno=getattr(error,'errno',None)))
        else:
            raise AssertionError(name+' unexpectedly returned')

    owner1,owner2,robot=object(),object(),object()
    sessions=SpotNativeSessions(3)
    first,second=provider(),provider()
    def attach(owner,native_provider):
        return sessions.attach_native_only(owner,robot,native_provider,
            expires_at_ns=time.monotonic_ns()+30_000_000_000)
    one=attach(owner1,first)
    two=attach(owner2,second)

    def invoke(binding,owner,operation='state',correlation=1):
        return sessions.invoke_from_authenticated_host(binding,owner,SERVICES[operation],operation,
            correlation,nonce=correlation+100,budget_ns=1000000)

    refused('cross-owner binding',PermissionError,lambda:invoke(one,owner2),13)
    refused('foreign binding',PermissionError,lambda:invoke(object(),owner1),13)
    refused('empty native state grant',PermissionError,lambda:invoke(one,owner1),13)
    refused('body command without owned acquisition',PermissionError,
            lambda:invoke(one,owner1,'command.stop',2),13)
    refused('empty native lease grant',PermissionError,
            lambda:invoke(one,owner1,'lease.acquire',3),13)
    assert not one.body_exclusion and first._body_state=='none'
    rows.append(dict(name='known pre-RPC rejection releases unused exclusion',status='PASS'))
    refused('second owner remains independently denied',PermissionError,
            lambda:invoke(two,owner2,'lease.acquire',4),13)
    assert not two.body_exclusion
    refused('native owner is never rebound',ValueError,
            lambda:attach(owner1,provider()))
    refused('native provider is never shared',ValueError,
            lambda:attach(object(),first))
    refused('genuine SDK wallets are never shared',ValueError,
            lambda:attach(object(),provider(first._wallet)))
    refused('wrong-owner retirement',PermissionError,lambda:sessions.retire_native_only(one,owner2),13)
    sessions.retire_native_only(one,owner1)
    refused('retired binding cannot dispatch',PermissionError,lambda:invoke(one,owner1),13)
    refused('retired identity cannot rebind',ValueError,
            lambda:attach(owner1,provider()))
    attach(object(),provider())
    refused('finite non-reusing session history',OSError,
            lambda:attach(object(),provider()),90)
    deadlines=SpotNativeSessions()
    refused('missing explicit session expiry',ValueError,
            lambda:deadlines.attach_native_only(object(),robot,provider()))
    refused('unbounded session lifetime',ValueError,
            lambda:deadlines.attach_native_only(object(),robot,provider(),
                expires_at_ns=time.monotonic_ns()+301_000_000_000))
    expiring_owner=object()
    expiring=deadlines.attach_native_only(expiring_owner,robot,provider(),
        expires_at_ns=time.monotonic_ns()+1_000_000_000)
    refused('business call cannot outlive session grant',TimeoutError,
            lambda:deadlines.invoke_from_authenticated_host(expiring,expiring_owner,
                SERVICES['state'],'state',1,nonce=2,budget_ns=2_000_000_000))
    time.sleep(1.05)
    refused('actual monotonic session expiry',PermissionError,
            lambda:deadlines.invoke_from_authenticated_host(expiring,expiring_owner,
                SERVICES['state'],'state',2,nonce=3,budget_ns=1_000_000),13)
    refused('expiry does not mint native cleanup authority',PermissionError,
            lambda:deadlines.cleanup_from_native_operator(expiring,expiring_owner),13)
    refused('expiry never rebinds owner identity',ValueError,
            lambda:deadlines.attach_native_only(expiring_owner,robot,provider(),
                expires_at_ns=time.monotonic_ns()+30_000_000_000))
    report=dict(schema='uwvm-task08-spot-native-contract-v2',cases=rows,
        cases_passed=len(rows),cases_denominator=len(rows),
        sdk_versions={name:version(name) for name in ('bosdyn-client','bosdyn-api','bosdyn-core')},
        default_authority=[],network_rpc_cases=0,robot_cases=0,wasm_guest_cases=0,
        positive_lease_isolation_cases=0,
        scope='Actual native SDK client/wallet construction plus pre-RPC denial and identity handling; no mocks, channels or robot connection.')
    args.output.write_text(json.dumps(report,indent=2,sort_keys=True)+'\n')
    print(json.dumps(dict(cases_passed=len(rows),cases_denominator=len(rows),sdk_versions=report['sdk_versions'])))


if __name__=='__main__':
    main()
