#!/usr/bin/env python3
"""Independent lifecycle oracle and AST-only deterministic controller models.

This module never imports the execution controller or launches a compiler,
signature tool, native program, Guest, ROS process or QEMU. Its synthetic facts
are deliberately distinguishable from actual process/SDK/signature evidence.
Use --out with a fresh path; an existing receipt is never overwritten.
"""
import sys
sys.dont_write_bytecode = True
import argparse
import ast
import copy
import hashlib
import json
from pathlib import Path
import re


def require(condition, message):
    if not condition:
        raise ValueError(message)


def pin(path):
    data = Path(path).read_bytes()
    return {'path':str(path),'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()}


def independent_trace(events):
    """Require physical fault and actual delivery before closing the survivor.

    The oracle does not read the controller's gate table or implementation.
    Its old/new session and endpoint values represent independently witnessed
    native facts. A fresh transport must not recreate the revoked local alias.
    """
    expected = ('old-matched','baseline-delivered','revoke','stale-before','reopen-before',
                'native-frozen','old-owned-killed','old-owned-reaped','dead-peer',
                'new-owned-mapped','new-session-bound','new-native-matched','stale-after',
                'reopen-after','survivor-published','survivor-delivered','survivor-closed','quiet')
    require(isinstance(events,list) and len(events) == len(expected) and
            tuple(item.get('name') for item in events) == expected, 'independent lifecycle order missing/repeated/reordered')
    values = {item['name']:item for item in events}
    require(values['baseline-delivered'].get('dead') == values['native-frozen'].get('dead') == values['quiet'].get('dead') == 1,
            'revoked native count changed')
    for name, code in (('revoke',0),('stale-before',9),('reopen-before',125),('dead-peer',107),
                       ('stale-after',9),('reopen-after',125),('survivor-published',0),('survivor-closed',0)):
        require(type(values[name].get('errno')) is int and values[name]['errno'] == code, 'exact independent errno '+name)
    require(values['old-owned-killed'].get('pidfd') is True and values['old-owned-killed'].get('signal') == 9 and
            type(values['old-owned-reaped'].get('returncode')) is int and values['old-owned-reaped']['returncode'] == -9 and
            values['old-owned-reaped'].get('all_births_retired') is True, 'old owned Gateway did not retire before dead-peer call')
    old = values['old-matched'];fresh = values['new-native-matched'];session = values['new-session-bound']
    require(type(old.get('session')) is int and old['session'] > 0 and type(session.get('session')) is int and session['session'] > 0 and
            old['session'] != session['session'], 'actual new authenticated session missing')
    require(re.fullmatch(r'[0-9a-f]{32}',old.get('gid','')) is not None and re.fullmatch(r'[0-9a-f]{32}',fresh.get('gid','')) is not None and
            old['gid'] != '0'*32 and old['gid'] != fresh['gid'] and fresh['gid'] != '0'*32 and fresh.get('dead_publishers') == 0,
            'actual native endpoint remains old or revived dead resource')
    require(values['new-owned-mapped'].get('same_worker') is True and values['new-owned-mapped'].get('same_witness') is True,
            'Runtime or independent observer was restarted')
    require(values['survivor-delivered'].get('gid') == fresh['gid'] and values['survivor-delivered'].get('count') == 1,
            'surviving message did not come from independently matched fresh writer')
    require(values['quiet'].get('samples',0) >= 30 and values['quiet'].get('nanoseconds',0) >= 2000000000 and
            values['quiet'].get('unauthorized_dead') == values['quiet'].get('fault_live') == values['quiet'].get('unknown') == 0,
            'independent native no-side-effect observation missing')
    return True


def extract(path):
    """Execute only the named pure functions and their literal constants."""
    tree = ast.parse(Path(path).read_bytes())
    functions = {'protocol_require','parse_witness_markers','parse_guest_markers','parse_gateway_audit',
                 'validate_sessions','valid_identity','validate_combined_completion',
                 'terminal_command_metadata','reject_native_failure_line'}
    constants = {'KIND','DOMAIN','MEMORY_MAX','EXPECTED_BOOT','EXPECTED_LEAF_DEVICE','EXPECTED_LEAF_INODE',
                 'WITNESS','KEYSTORE','NATIVE_TYPE','ROLE_LABELS','WITNESS_COMMON','OLD_GIDS','WITNESS_FIELDS','GUEST_MARKERS',
                 'NATIVE_FAILURE_PHASES','NATIVE_ENDPOINT_FAILURE_FIELDS','NATIVE_FAILURE_FIELDS'}
    selected = [node for node in tree.body if isinstance(node,ast.FunctionDef) and node.name in functions or
                isinstance(node,ast.Assign) and all(isinstance(value,ast.Name) and value.id in constants for value in node.targets)]
    require({node.name for node in selected if isinstance(node,ast.FunctionDef)} == functions, 'pure controller front door is incomplete')
    scope = {'re':re,'hashlib':hashlib}
    exec(compile(ast.Module(body=selected,type_ignores=[]),'<pure-lifecycle-front-door-only>','exec'),scope)
    return scope


def fixture(scope):
    """Build hand-authored synthetic expected facts without execution helpers."""
    hashes = {'worker':'1'*64,'gateway':'2'*64,'witness':'3'*64}
    products = {}
    for index,role in enumerate(('worker','gateway','witness')):
        base = {'path':'/synthetic/'+role,'bytes':1000+index,'device':29,'inode':100+index,
                'elf_class':2,'elf_data':1,'e_machine':183,'sha256':hashes[role]}
        products[role] = {'qualified':True,'origin':base,'snapshot':{**base,'path':'/synthetic/snapshot-'+role,'inode':200+index}}
    membership = ['0::/synthetic-original-container']
    births = {'worker':(31001,101),'gateway_old':(31002,102),'gateway_new':(31004,104),'witness':(31003,103)}
    roles = {}
    for role,(pid,birth) in births.items():
        label = {'worker':'revoke-worker','gateway_old':'revoke-gateway','gateway_new':'revoke-gateway','witness':'revoke-reconnect-witness'}[role]
        actual = {'pid':pid,'starttime_ticks':birth,'state':'T'}
        sample = {'pid':pid,'starttime_ticks':birth,'live_cgroup_procs':True,'membership':membership,'mapped_arm_targets':[label],
                  'kernel_exe_elf':{'e_machine':62,'sha256':'4'*64},'ancestry':[pid]}
        roles[role] = {'pid':pid,'starttime_ticks':birth,'membership':membership,
            'mapped_product':products['gateway' if role.startswith('gateway_') else role]['snapshot'],
            'proof':{'actual_stopped_ARM':actual,'target_proof':sample,'current_mapping_check':True,'ancestry':[pid]},
            'live_before_C' if role == 'gateway_new' else 'live_before_G':True,'pre_init_mapping':role == 'witness'}
    gids = {'dead_sub_gid':'a1'*16,'live_sub_gid':'a2'*16,'old_dead_pub_gid':'b1'*16,'old_live_pub_gid':'b2'*16,'new_live_pub_gid':'c1'*16}
    common = ('pid=31003 node=/uwvm_revoke_test/revoke_witness enclave=/witness domain=75 rmw=rmw_fastrtps_cpp '
              'type=std_msgs/msg/String dead_topic=/uwvm_revoke_test/dead live_topic=/uwvm_revoke_test/live '
              'dead_sub_gid='+gids['dead_sub_gid']+' live_sub_gid='+gids['live_sub_gid'])
    old = ' old_dead_pub_gid='+gids['old_dead_pub_gid']+' old_live_pub_gid='+gids['old_live_pub_gid']
    fresh = old+' new_live_pub_gid='+gids['new_live_pub_gid']
    witness_text = ('READY_REVOKE_WITNESS '+common+'\nMATCHED_REVOKE_OLD '+common+old+' dead_publishers=1 live_publishers=1\n'
        'BASELINE_REVOKE_WITNESS '+common+old+' dead_before=1 live_before=1\n'
        'FROZEN_REVOKE_WITNESS '+common+old+' dead_total=1 live_before=1 unauthorized_dead=0\n'
        'MATCHED_REVOKE_NEW '+common+fresh+' dead_publishers=0 live_publishers=1 dead_total=1 live_before=1 live_after=0\n'
        'DELIVERED_REVOKE_NEW '+common+fresh+' dead_publishers=0 live_publishers=1 dead_total=1 live_before=1 live_after=1\n'
        'PASS_REVOKE_RECONNECT_WITNESS '+common+fresh+' dead_publishers=0 live_publishers=1 dead_total=1 live_before=1 live_after=1 '
        'unauthorized_dead=0 fault_live=0 unknown=0 quiet_samples=100 quiet_ns=2001000000 warmup_dead=1 warmup_live=1 warmup_new=1\n')
    guest_text = '\n'.join(('REVOKE_ENDPOINTS_CREATED','BEFORE_MESSAGES_SENT','REVOKED_STALE9_REOPEN125','GATEWAY_FAULT107',
                            'RECONNECT_ENDPOINT_CREATED','AFTER_MESSAGE_SENT','PASS_REVOKE_RECONNECT_GUEST'))+'\n'
    def audit(session,operation,alias,number):
        return f'ros_audit timestamp_ns={1000000+number} backend=2 worker_uid=1000 worker_pid=31001 session={session} operation={operation} alias={alias} error=0\n'
    old_text = audit(5,'session.bind','target',1)+audit(6,'session.bind','target',2)
    for index,(op,alias) in enumerate((('topic.publish','deadPub'),('topic.publish','livePub'),('topic.publish','deadPub'),
                                      ('topic.publish','livePub'),('capability.revoke','deadPub')),3):old_text += audit(6,op,alias,index)
    prefix = audit(7,'session.bind','target',1)+audit(7,'topic.publish','livePub',2)
    new_text = prefix+audit(7,'topic.publish','livePub',3)+audit(7,'capability.close','livePub',4)
    signed = {'status':'PASS','keystore':'/tmp/revoke-reconnect-keys','subjects':{'gateway':'CN=/gateway','witness':'CN=/witness'},
              'policy_sha256':'5'*64,'closure_sha256':'6'*64}
    for name in ('certificate_CA_verified','permissions_signatures_verified','governance_signatures_verified','key_cert_public_match',
                 'default_deny','exact_topic_permissions','keystore_preserved_before_removal'):signed[name] = True
    before = {'device':29,'inode':18927,'boot_id':'518a7b26-33dd-4044-9456-93e75aac9b63','path':'/synthetic/original-leaf'}
    predecessors = ('ARM_WITNESS_PREINIT_STOP','READY_REVOKE_WITNESS','MATCHED_REVOKE_OLD+REVOKE_ENDPOINTS_CREATED',
        'BASELINE_REVOKE_WITNESS+BEFORE_MESSAGES_SENT','REVOKED_STALE9_REOPEN125','FROZEN_REVOKE_WITNESS+OLD_GATEWAY_REAPED',
        'NEW_GATEWAY_READY+GATEWAY_FAULT107','MATCHED_REVOKE_NEW+RECONNECT_ENDPOINT_CREATED+NEW_SESSION_BIND',
        'DELIVERED_REVOKE_NEW+AFTER_MESSAGE_SENT','PASS_REVOKE_RECONNECT_GUEST')
    gates = []
    for index,(role,byte) in enumerate((('witness','I'),('worker','G'),('worker','B'),('worker','R'),('witness','N'),
                                      ('worker','F'),('worker','C'),('worker','A'),('worker','Q'),('witness','F'))):
        pid,birth = births[role]
        gates.append({'role':role,'byte':byte,'pid':pid,'starttime_ticks':birth,'current_birth_mapping_verified':True,'sent':True,'prior_marker':predecessors[index]})
    gates[5]['old_gateway_retired']=True;gates[6]['new_gateway_live']=True;gates[7]['new_session']=7
    gates[8]['native_survivor_delivered']=True;gates[9]['guest_exit']=0
    row = {'kind':'g09-revoke-fault-reconnect-arm','format_version':1,'mode':'execute','source_manifest_sha256':'7'*64,'qemu_sha256':'4'*64,
           'products':products,'roles':roles,'worker_uid':1000,'signed':signed,
           'cgroup':{'before':before,'after':dict(before),'same_incarnation':True,'memory_max':68719476736,'swap_max':'0','readable_ancestors':True,'zero_oom':True,'membership':membership},
           'native_context':{'witness':{'attested':True,'enclave':'/witness','domain':75,'rmw':'rmw_fastrtps_cpp','security':'Enforce','root':'/tmp/revoke-reconnect-keys/enclaves/witness'},
               'gateway':{'source_selected':True,'enclave':'/gateway','domain':75,'rmw':'rmw_fastrtps_cpp','security':'Enforce','root':'/tmp/revoke-reconnect-keys/enclaves/gateway',
                          'environment_explicit':True,'direct_used_root_observed':False}},
           'markers':{'witness_text':witness_text,'witness':scope['parse_witness_markers'](witness_text,31003),'guest_text':guest_text},'gates':gates,
           'sessions':{'old_text':old_text,'new_text':new_text,'before_A_text':prefix,'parsed':scope['validate_sessions'](old_text,new_text,prefix,31001,1000)},
           'fault':{'intentional':True,'signal':9,'old_pid':31002,'old_starttime_ticks':102,'observed_old_births':[{'pid':31002,'starttime_ticks':102,'pidfd_signal_sent':True,'live_cgroup_verified':True}],
                    'all_old_owned_births_retired':True,'root_reaped':True,'socket_removed_after_retirement':True,'returncode':-9,'same_worker_live':True,'same_witness_live':True,
                    'old_socket':{'path':'/synthetic/owned/ros2.sock','device':29,'inode':300},'new_socket':{'path':'/synthetic/owned/ros2.sock','device':29,'inode':301}},
           'fixture_assertions':{'stale_before':9,'reopen_before':125,'dead_peer':107,'stale_after':9,'reopen_after':125,'surviving_publish':0},
           'commands':{'worker_exit':0,'witness_exit':0,'generation_exit':0,'gateway_old_exit':-9,'gateway_new_exit_after_worker':'OWNED_CLEANUP',
               'gateway_new_terminal':{'status':'OWNED_CLEANUP','returncode':-15,'log_binding':{'path':'/synthetic/gateway-new.log','device':29,'inode':999,
                   'bytes':len(new_text.encode()),'sha256':hashlib.sha256(new_text.encode()).hexdigest()}},'signed_verification_exits':[0,0]},
           'cleanup':{'owned_processes':[],'mounts':[],'socket_absent':True,'secrets_removed':True,'domain_lock_removed':True},'G09_complete':False}
    for name in ('source_preserved','sdk_preserved','driver_inputs_preserved','immutable_inputs_preserved','qualified_inputs_preserved','products_preserved'):row[name]=True
    trace = [{'name':name} for name in ('old-matched','baseline-delivered','revoke','stale-before','reopen-before','native-frozen','old-owned-killed','old-owned-reaped',
             'dead-peer','new-owned-mapped','new-session-bound','new-native-matched','stale-after','reopen-after','survivor-published','survivor-delivered','survivor-closed','quiet')]
    values={item['name']:item for item in trace}
    values['old-matched'].update(session=6,gid='b2'*16);values['new-session-bound']['session']=7
    values['new-native-matched'].update(gid='c1'*16,dead_publishers=0)
    values['new-owned-mapped'].update(same_worker=True,same_witness=True)
    for name,code in (('revoke',0),('stale-before',9),('reopen-before',125),('dead-peer',107),('stale-after',9),('reopen-after',125),('survivor-published',0),('survivor-closed',0)):values[name]['errno']=code
    values['old-owned-killed'].update(pidfd=True,signal=9);values['old-owned-reaped'].update(returncode=-9,all_births_retired=True)
    values['baseline-delivered']['dead']=1;values['native-frozen']['dead']=1
    values['survivor-delivered'].update(gid='c1'*16,count=1)
    values['quiet'].update(dead=1,samples=100,nanoseconds=2001000000,unauthorized_dead=0,fault_live=0,unknown=0)
    return row,trace


def models(root):
    scope = extract(root/'tests/ros_aarch64/run_revoke_reconnect.py');row,trace = fixture(scope);checks=[]
    def allow(name,function):
        require(function() is not False, name);checks.append({'name':name,'status':'PASS_POSITIVE_PURE_MODEL'})
    def deny(name,function):
        try:function()
        except (RuntimeError,ValueError,KeyError,IndexError,TypeError):checks.append({'name':name,'status':'PASS_REFUSAL_PURE_MODEL'});return
        raise ValueError('unexpected model admission: '+name)
    allow('independent full ordered lifecycle',lambda:independent_trace(trace))
    allow('actual pure controller completion shape',lambda:scope['validate_combined_completion'](row))
    selected={'path':'/synthetic/new-current-leaf','device':53,'inode':99123,
              'boot_id':'cccccccc-cccc-cccc-cccc-cccccccccccc'}
    fresh=copy.deepcopy(row);fresh['cgroup']['before']=copy.deepcopy(selected);fresh['cgroup']['after']=copy.deepcopy(selected)
    validate=scope['validate_combined_completion']
    allow('explicit new incarnation is admitted independently of historical constants',lambda:validate(fresh,selected))
    deny('historical row cannot satisfy explicit new incarnation',lambda:validate(row,selected))
    for name in selected:
        wrong=copy.deepcopy(selected);wrong[name]='old' if name=='boot_id' else '/synthetic/replaced' if name=='path' else 1
        deny('explicit cgroup external pin differs '+name,lambda wrong=wrong:validate(fresh,wrong))
    partial=copy.deepcopy(selected);partial.pop('inode')
    deny('partial explicit incarnation is refused',lambda:validate(fresh,partial))
    boolean=copy.deepcopy(selected);boolean['device']=True
    deny('boolean is not an explicit cgroup device identity',lambda:validate(fresh,boolean))
    for index in range(len(trace)):
        changed=copy.deepcopy(trace);changed.pop(index);deny('independent required phase absent '+str(index),lambda changed=changed:independent_trace(changed))
        changed=copy.deepcopy(trace);changed[index]['name']='unknown';deny('independent phase replaced '+str(index),lambda changed=changed:independent_trace(changed))
    for index in range(len(trace)-1):
        changed=copy.deepcopy(trace);changed[index],changed[index+1]=changed[index+1],changed[index]
        deny('independent adjacent phases reordered '+str(index),lambda changed=changed:independent_trace(changed))
    for index,item in enumerate(trace):
        for key,value in item.items():
            if key=='name':continue
            changed=copy.deepcopy(trace);changed[index][key]=False if type(value) is bool else 0 if type(value) is int else '0'*32
            if changed[index][key]==value:changed[index][key]=1
            deny('independent semantic fact mutation '+item['name']+'/'+key,lambda changed=changed:independent_trace(changed))
    for key in row:
        changed=copy.deepcopy(row);del changed[key];deny('completion required field absent '+key,lambda changed=changed:scope['validate_combined_completion'](changed))
    def mutations(value,path=()):
        for key,item in value.items():
            next_path=path+(key,)
            if isinstance(item,dict):yield from mutations(item,next_path)
            elif type(item) is bool:yield next_path,not item
    for path,value in mutations(row):
        changed=copy.deepcopy(row);selected=changed
        for key in path[:-1]:selected=selected[key]
        selected[path[-1]]=value
        deny('completion boolean proof mutation '+'.'.join(path),lambda changed=changed:scope['validate_combined_completion'](changed))
    for key,value in (('format_version',True),('kind','g09-guest-enforce-arm'),('mode','prepare'),('worker_uid',True),('G09_complete',True)):
        changed=copy.deepcopy(row);changed[key]=value;deny('completion exact type/scope '+key,lambda changed=changed:scope['validate_combined_completion'](changed))
    for key in row['fixture_assertions']:
        for value in (0,1,True,110):
            if type(value) is int and value==row['fixture_assertions'][key]:continue
            changed=copy.deepcopy(row);changed['fixture_assertions'][key]=value
            deny('exact errno not broadened '+key+'/'+str(value),lambda changed=changed:scope['validate_combined_completion'](changed))
    text=row['markers']['witness_text']
    for byte_count in range(33):
        if byte_count==16:continue
        changed=text
        for value in ('a1','a2','b1','b2','c1'):changed=changed.replace(value*16,value*byte_count)
        deny('coherent wrong native GID byte width '+str(byte_count),lambda changed=changed:scope['parse_witness_markers'](changed,31003))
    for fragment,substitute in (('domain=75','domain=74'),('enclave=/witness','enclave=/gateway'),('dead_total=1','dead_total=2'),
            ('live_after=1','live_after=0'),('fault_live=0','fault_live=1'),('quiet_samples=100','quiet_samples=0'),('quiet_ns=2001000000','quiet_ns=1'),
            ('warmup_new=1','warmup_new=2'),('new_live_pub_gid='+('c1'*16),'new_live_pub_gid='+('b2'*16))):
        changed=text.replace(fragment,substitute);deny('native exact semantic marker '+fragment,lambda changed=changed:scope['parse_witness_markers'](changed,31003))
    for changed in (text+'garbage',text.replace('\n','\r\n'),text+text.splitlines()[0]+'\n',text.replace(' pid=31003',' pid=31003 pid=31003',1)):
        deny('native raw LF/order/token refusal '+str(len(changed)),lambda changed=changed:scope['parse_witness_markers'](changed,31003))
    changed=text.replace('pid=31003 node=/uwvm_revoke_test/revoke_witness','node=/uwvm_revoke_test/revoke_witness pid=31003',1)
    deny('native marker key order is fixed',lambda:scope['parse_witness_markers'](changed,31003))
    failure='FAIL_REVOKE_RECONNECT_WITNESS phase=fresh-discovery site=fresh_live_candidate field=name_unknown'
    for prefix in ('',text,'\n'.join(text.splitlines()[:4])+'\n'):
        for complete in (False,True):
            deny('fixed native FAIL never enters marker evidence '+str(len(prefix))+'/'+str(complete),
                 lambda prefix=prefix,complete=complete:scope['parse_witness_markers'](prefix+failure+'\n',31003,complete))
    try:scope['reject_native_failure_line'](failure)
    except RuntimeError as error:
        allow('fixed native diagnostic retains exact public refusal codes',lambda:str(error)==
              'native witness refused phase=fresh-discovery site=fresh_live_candidate field=name_unknown')
    for value in (failure+' extra=1',failure.replace('site=','field=',1),failure.replace('name_unknown','/private/key'),
                  failure.replace('name_unknown','payload secret'),failure.replace('name_unknown','x'*65),failure+'\r',
                  failure.replace('name_unknown','\u4e2d'),failure.replace(' phase=','  phase=')):
        deny('malformed native refusal text stays rejected '+repr(value[-24:]),lambda value=value:scope['reject_native_failure_line'](value))
    for phase in scope['NATIVE_FAILURE_PHASES']:
        line='FAIL_REVOKE_RECONNECT_WITNESS phase='+phase+' site=contract field=unspecified'
        deny('every fixed native failure phase is refused '+phase,lambda line=line:scope['reject_native_failure_line'](line))
    for site,fields in scope['NATIVE_FAILURE_FIELDS'].items():
        for field in fields:
            line='FAIL_REVOKE_RECONNECT_WITNESS phase=fresh-discovery site='+site+' field='+field
            try:scope['reject_native_failure_line'](line)
            except RuntimeError as error:
                allow('fixed native failure catalog precise refusal '+site+'/'+field,lambda error=error,site=site,field=field:
                      str(error)=='native witness refused phase=fresh-discovery site='+site+' field='+field)
    for fragment,value in (('fresh-discovery','unknown'),('fresh_live_candidate','unknown'),('name_unknown','unknown'),
                           ('name_unknown','baseline_count'),('fresh_live_candidate','callback_counters')):
        changed=failure.replace(fragment,value)
        deny('unknown or mismatched fixed native failure code '+fragment+'/'+value,lambda changed=changed:scope['reject_native_failure_line'](changed))
        try:scope['reject_native_failure_line'](changed)
        except RuntimeError as error:
            allow('unknown native diagnostic bodies are never echoed '+fragment+'/'+value,lambda error=error:
                  str(error)=='native witness emitted an unknown fixed failure code')
    for key in ('old_text','new_text','before_A_text'):
        changed=copy.deepcopy(row);changed['sessions'][key]=changed['sessions'][key].replace('session=7','session=6') if key!='old_text' else ''
        deny('actual session proof mutation '+key,lambda changed=changed:scope['validate_combined_completion'](changed))
    changed=copy.deepcopy(row);changed['sessions']['new_text']=changed['sessions']['new_text'].replace('alias=livePub','alias=deadPub')
    deny('new session cannot revive dead alias',lambda:scope['validate_combined_completion'](changed))
    changed=copy.deepcopy(row);changed['fault']['new_socket']=copy.deepcopy(changed['fault']['old_socket'])
    deny('replacement socket cannot be old inode',lambda:scope['validate_combined_completion'](changed))
    changed=copy.deepcopy(row);changed['roles']['gateway_new']=copy.deepcopy(changed['roles']['gateway_old'])
    deny('replacement process cannot be old owned birth',lambda:scope['validate_combined_completion'](changed))
    for index in range(len(row['gates'])):
        changed=copy.deepcopy(row);changed['gates'].pop(index);deny('required semantic gate missing '+str(index),lambda changed=changed:scope['validate_combined_completion'](changed))
    for code in (1,126,-5,-6,True,None):
        changed=copy.deepcopy(row);changed['commands']['gateway_new_terminal']['returncode']=code
        deny('cleanup cannot hide unrelated Gateway terminal result '+repr(code),lambda changed=changed:scope['validate_combined_completion'](changed))
    for code in (0,-9):
        changed=copy.deepcopy(row);changed['commands']['gateway_new_terminal']['returncode']=code
        allow('cleanup actual zero or owned fallback terminal '+str(code),lambda changed=changed:scope['validate_combined_completion'](changed))
    changed=copy.deepcopy(row);changed['commands']['gateway_new_exit_after_worker']=0
    changed['commands']['gateway_new_terminal'].update(status='PASS',returncode=0)
    allow('natural replacement retirement after same Worker exit',lambda:scope['validate_combined_completion'](changed))
    for key in ('status','returncode','log_binding'):
        changed=copy.deepcopy(row);changed['commands']['gateway_new_terminal'].pop(key)
        deny('cleanup terminal field absent '+key,lambda changed=changed:scope['validate_combined_completion'](changed))
    changed=copy.deepcopy(row);changed['commands']['gateway_new_terminal']['log_binding']['sha256']='0'*64
    deny('cleanup final native log bytes are immutable',lambda:scope['validate_combined_completion'](changed))
    changed=copy.deepcopy(row);changed['gates'][8]['native_survivor_delivered']=False
    deny('Q cannot release before independent survivor delivery',lambda:scope['validate_combined_completion'](changed))
    raw=(root/'tests/ros_aarch64/run_revoke_reconnect.py').read_text()
    native_raw=(root/'tests/ros_lifecycle/ros2_revoke_reconnect_witness.cc').read_text()
    tree=ast.parse(raw);main=next(node for node in tree.body if isinstance(node,ast.FunctionDef) and node.name=='main')
    native=next(node for node in main.body if isinstance(node,ast.FunctionDef) and node.name=='native')
    wrapper={'root':Path('/synthetic/owned-root')}
    exec(compile(ast.Module(body=[native],type_ignores=[]),'<pure-new-witness-wrapper-only>','exec'),wrapper)
    arguments=['/tmp/revoke-reconnect-witness','/tmp/revoke-reconnect-keys']
    ordinary=wrapper['native'](arguments);witness=wrapper['native'](arguments,witness=True)
    checked='source /opt/ros/jazzy/setup.bash && unset ROS_AUTOMATIC_DISCOVERY_RANGE && [[ ! -v ROS_AUTOMATIC_DISCOVERY_RANGE ]] && exec "$@"'
    allow('ordinary tools and Gateways retain exact native setup script',lambda:ordinary[-4:]==['source /opt/ros/jazzy/setup.bash && exec "$@"','revoke-signed-native',*arguments])
    allow('new witness alone uses checked post-setup cleanup',lambda:witness[-4:]==[checked,'revoke-signed-native',*arguments] and raw.count('witness=True')==1)
    allow('native strict discovery absence was preserved',lambda:'ROS_AUTOMATIC_DISCOVERY_RANGE' in native_raw and 'require(::getenv(name) == nullptr)' in native_raw)
    allow('Q native delivery precedes writer close in source',lambda:raw.index("'DELIVERED_REVOKE_NEW',witness_pid")<raw.index("'Q',worker_proof")<raw.index('worker.stdin.close();finish(worker,worker_row)'))
    allow('actual dead peer is reaped before F and replacement launch',lambda:raw.index('gateway_old.wait(timeout=1)')<raw.index("'F',worker_proof")<raw.index("start('gateway-new'"))
    allow('final writer cleanup precedes final audit binding',lambda:raw.index('owned.stop([child for child,_ in children])')<raw.index("read_file_binding(new_row['log']",raw.index('    finally:')))
    # These diagnostic-only models extract the actual finalization block and
    # replace every process, log binder and cleanup operation with hand-authored
    # inert objects. No execution controller or runtime helper is imported.
    terminal=scope['terminal_command_metadata']
    for previous,before,final,pending,expected in (
        ('RUNNING',1,1,False,'FAIL'),('RUNNING',0,0,False,'EXITED_NOT_ACCEPTED'),
        ('RUNNING',None,-15,False,'OWNED_ABORTED_AFTER_FAILURE'),
        ('RUNNING',None,-9,False,'OWNED_ABORTED_AFTER_FAILURE'),
        ('RUNNING',None,0,False,'OWNED_ABORTED_AFTER_FAILURE'),
        ('RUNNING',None,-15,True,'OWNED_CLEANUP_PENDING_ACCEPTANCE'),
        ('RUNNING',None,1,False,'FAIL_DURING_CLEANUP'),
        ('RUNNING',None,1,True,'FAIL_DURING_CLEANUP'),
        ('PASS',0,0,False,'PASS'),('EXPECTED_OWNED_FAULT',-9,-9,False,'EXPECTED_OWNED_FAULT'),
        ('FAIL',1,1,False,'FAIL'),('EXITED_PENDING_ARCHITECTURE',0,0,False,'EXITED_NOT_ACCEPTED')):
        allow('actual terminal diagnostic classification '+repr((previous,before,final,pending)),
              lambda previous=previous,before=before,final=final,pending=pending,expected=expected:
              terminal(previous,before,final,pending)=={'status':expected,'status_before_cleanup':previous,'returncode':final,
                  'returncode_before_cleanup':before,'exited_before_cleanup':before is not None,'alive_before_cleanup':before is None})
    for previous,before,final,pending in (('RUNNING',None,None,False),('RUNNING',True,1,False),('RUNNING',None,True,False),
            ('RUNNING',None,1,1),('RUNNING',1,2,False),('PASS',0,1,False),('EXPECTED_OWNED_FAULT',-9,-15,False),
            ('unknown',None,0,False)):
        deny('terminal metadata rejects fabricated result '+repr((previous,before,final,pending)),
             lambda previous=previous,before=before,final=final,pending=pending:terminal(previous,before,final,pending))
    lane=next(node for node in main.body if isinstance(node,ast.Try) and node.finalbody)
    end=next(index for index,node in enumerate(lane.finalbody) if isinstance(node,ast.If) and
             ast.unparse(node.test)=='completion is not None')
    cleanup_block=lane.finalbody[:end]
    class FakeChild:
        def __init__(self,code):self.returncode=code
        def poll(self):return self.returncode
    class FakeOwned:
        def __init__(self,children,leave_live=False):self.children=children;self.leave_live=leave_live;self.called=False
        def stop(self,children):
            require(children==self.children,'same retained child list required');self.called=True
            for child in children:
                if child.returncode is None and not self.leave_live:child.returncode=-15
            return {'status':'FAIL' if self.leave_live else 'PASS','remaining_owned_processes':[9] if self.leave_live else []}
    class FakeTraceback:
        @staticmethod
        def format_exc():return 'synthetic fixed failure'
    def cleanup_model(leave_live=False,fail_log=False):
        children=[FakeChild(0),FakeChild(-9),FakeChild(1),FakeChild(None),FakeChild(None)]
        rows=[{'name':name,'status':state,'log':'/synthetic/'+name+'.log'} for name,state in
              (('tool','PASS'),('gateway-old','EXPECTED_OWNED_FAULT'),('witness','RUNNING'),('worker','RUNNING'),('gateway-new','RUNNING'))]
        owner=FakeOwned(children,leave_live);bindings=[];saved=[]
        def bind(path,limit,retain):
            require(owner.called,'final log binding must follow owned cleanup')
            require(limit==16*1024**2 and retain is True,'bounded retained log binder required')
            if fail_log and path.endswith('/witness.log'):raise RuntimeError('synthetic final log failure')
            data=('synthetic '+path+'\n').encode();fact={'path':path,'bytes':len(data),'device':29,'inode':700+len(bindings),
                                                       'sha256':hashlib.sha256(data).hexdigest()}
            bindings.append(fact);return data,fact
        env={'children':list(zip(children,rows)),'owned':owner,'report':{'actual_guest_behavior':'NOT_ACCEPTED'},'status':'FAIL',
             'terminal_command_metadata':terminal,'read_file_binding':bind,'pins':[],
             'traceback':FakeTraceback,'save':lambda:saved.append(True)}
        exec(compile(ast.Module(body=cleanup_block,type_ignores=[]),'<pure-fake-failure-cleanup-only>','exec'),env)
        return rows,env,bindings,saved
    finalized,env,bindings,saved=cleanup_model()
    allow('partial lane keeps all actual terminal command codes',lambda:[value['returncode'] for value in finalized]==[0,-9,1,-15,-15])
    allow('partial lane distinguishes failed native from aborted owned processes',lambda:[value['status'] for value in finalized]==
          ['PASS','EXPECTED_OWNED_FAULT','FAIL','OWNED_ABORTED_AFTER_FAILURE','OWNED_ABORTED_AFTER_FAILURE'])
    allow('every failed-lane retired command retains full final log binding',lambda:len(bindings)==5 and len(env['pins'])==5 and bool(saved) and
          all(value['log_binding']['sha256']==value['log_sha256'] and value['log_binding']['path']==value['log'] for value in finalized))
    allow('failure cleanup cannot manufacture successful behavior',lambda:env['status']=='FAIL' and env['report']['actual_guest_behavior']=='NOT_ACCEPTED')
    finalized,env,bindings,saved=cleanup_model(leave_live=True)
    allow('unretired failure children are explicit and remain failure',lambda:env['status']=='FAIL' and
          [value['status'] for value in finalized[-2:]]==['UNRETIRED_AFTER_CLEANUP']*2 and
          all(value['returncode'] is None and 'terminal_finalization_error' in value for value in finalized[-2:]))
    finalized,env,bindings,saved=cleanup_model(fail_log=True)
    allow('binding failure preserves native actual code without a fabricated hash',lambda:env['status']=='FAIL' and
          finalized[2]['returncode']==1 and 'terminal_finalization_error' in finalized[2] and 'log_binding' not in finalized[2] and
          finalized[-1]['returncode']==-15 and 'log_binding' in finalized[-1])
    execution=next(node for node in lane.body if isinstance(node,ast.If) and ast.unparse(node.test)=="args.mode == 'prepare'")
    release_index=next(index for index,node in enumerate(execution.orelse) if isinstance(node,ast.Expr) and isinstance(node.value,ast.Call) and
                       isinstance(node.value.func,ast.Name) and node.value.func.id=='send_gate' and len(node.value.args)>3 and
                       isinstance(node.value.args[3],ast.Constant) and node.value.args[3].value=='G')
    release=execution.orelse[release_index:release_index+3]
    def release_model(fail=False):
        state={'actual_guest_behavior':'NOT_PERFORMED'};saved=[]
        def gate(*args):
            if fail:raise RuntimeError('synthetic gate write/flush failure')
        env={'report':state,'send_gate':gate,'worker':None,'worker_row':{},'worker_proof':{},'save':lambda:saved.append(True)}
        try:exec(compile(ast.Module(body=release,type_ignores=[]),'<pure-fake-G-release-only>','exec'),env)
        except RuntimeError:pass
        return state,saved
    state,saved=release_model()
    allow('successful exact G release persists actual Guest RUNNING',lambda:state['actual_guest_behavior']=='RUNNING' and saved==[True])
    state,saved=release_model(True)
    allow('G write failure does not fabricate successful Guest release',lambda:state['actual_guest_behavior']=='NOT_PERFORMED' and not saved)
    for current in ('NOT_PERFORMED','RUNNING','OBSERVED_PENDING_FINAL_GUARDS'):
        state={'actual_guest_behavior':current};env={'report':state,'traceback':FakeTraceback,'error':RuntimeError('synthetic fixture rejection')}
        exec(compile(ast.Module(body=lane.handlers[0].body,type_ignores=[]),'<pure-fake-lane-exception-only>','exec'),env)
        allow('actual exception records attempted state '+current,lambda current=current,state=state,env=env:
              env['status']=='FAIL' and state['actual_guest_behavior']==('NOT_PERFORMED' if current=='NOT_PERFORMED' else 'NOT_ACCEPTED'))
    wait=next(node for node in main.body if isinstance(node,ast.FunctionDef) and node.name=='wait_marker')
    class ExitedChild:
        @staticmethod
        def poll():return 1
    def final_native_parser(value,pid,complete):return scope['parse_witness_markers'](value,pid,complete)
    wait_scope={'growing_log':lambda path:failure+'\n'}
    exec(compile(ast.Module(body=[wait],type_ignores=[]),'<pure-already-exited-native-marker-only>','exec'),wait_scope)
    deny('already-exited native FAIL is parsed and always refused',lambda:wait_scope['wait_marker'](ExitedChild(),{'log':'/synthetic/native.log'},
                                                                                               final_native_parser,'MATCHED_REVOKE_NEW',31003))
    try:wait_scope['wait_marker'](ExitedChild(),{'log':'/synthetic/native.log'},final_native_parser,'MATCHED_REVOKE_NEW',31003)
    except RuntimeError as error:
        allow('already-exited native keeps the precise fixed refusal',lambda:str(error)==
              'native witness refused phase=fresh-discovery site=fresh_live_candidate field=name_unknown')
    allow('full native SDK includes and exact source/recipe checks remain',lambda:all(value in raw for value in ('-MD -MF','fixture_closure','native_link_closure','sdk_inventory(sdk)','qualify_gateway(',
          'validate_fresh_worker_inputs','EXPECTED_BOOT','memory.swap.max','cgroup_oom_deltas','pins_preserved(pins)','owned.send(item,signal.SIGKILL)')))
    policy=next(node for node in tree.body if isinstance(node,ast.FunctionDef) and node.name=='topic_policy')
    policy_scope={key:scope[key] for key in ('DOMAIN','NATIVE_TYPE','KEYSTORE')};policy_scope['NATIVE_IDENTITY']='RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18'
    exec(compile(ast.Module(body=[policy],type_ignores=[]),'<pure-explicit-test-policy-only>','exec'),policy_scope)
    connection,grants=policy_scope['topic_policy']()
    allow('only two explicit internal publisher aliases and signed domain75',lambda:connection['domain-id']=='75' and connection['security-mode']=='enforce' and
          [(cap['alias'],cap['operation'],cap['resource']) for cap in grants]==[('deadPub','topic.publish','/uwvm_revoke_test/dead'),('livePub','topic.publish','/uwvm_revoke_test/live')])
    wat=(root/'tests/ros_lifecycle/revoke_reconnect_ros2.wat').read_text()
    data={}
    for match in re.finditer(r'\(data \(i32\.const (\d+)\) "([^"\n]*)"\)',wat):
        literal=match.group(2);out=bytearray();index=0
        while index<len(literal):
            if literal[index]=='\\':out.append(int(literal[index+1:index+3],16));index+=3
            else:out.append(ord(literal[index]));index+=1
        data[int(match.group(1))]=bytes(out)
    for offset,text in ((4096,'WARMUP-DEAD'),(4160,'WARMUP-LIVE'),(4224,'BEFORE-DEAD'),(4288,'BEFORE-LIVE'),
                        (4352,'FAULT-LIVE'),(4416,'RECONNECT-WARMUP'),(4480,'AFTER-LIVE')):
        allow('canonical fixed String source bytes '+text,lambda offset=offset,text=text:data[offset]==len(text).to_bytes(4,'little')+text.encode())
    for offset,length in re.findall(r'\(call \$emit \(i32\.const (\d+)\) \(i32\.const (\d+)\)\)',wat):
        allow('exact complete Guest LF marker bytes '+offset,lambda offset=offset,length=length:len(data[int(offset)])==int(length) and data[int(offset)].endswith(b'\n'))
    abi=json.loads((root/'sdk/abi/abi-v1.json').read_bytes());known={item['name']:item for item in abi['modules']['uwvm_ros2_v1']['functions']}
    imports=re.findall(r'\(import "uwvm_ros2_v1" "([a-z_]+)" \(func \$[a-z_]+ \(param ([^)]*)\) \(result ([^)]*)\)\)\)',wat)
    for name,parameters,results in imports:
        allow('raw ROS ABI signature '+name,lambda name=name,parameters=parameters,results=results:parameters.split()==[item['type'] for item in known[name]['params']] and results.split()==known[name]['results'])
    allow('exact four imported ROS functions authorize no resources',lambda:{item[0] for item in imports}=={'capability_open','capability_close','capability_revoke','topic_publish'})
    allow('fixed memory0 no Guest filesystem or socket imports',lambda:'(memory (export "memory") 1 1)' in wat and
          not any(value in wat for value in ('"path_open"','"sock_open"','"environ_get"','"proc_spawn"')))
    allow('checked Q before close and strict9/125/107 WAT assertions',lambda:wat.index('(call $gate (i32.const 0x51))')<wat.index('(call $close (local.get $live))') and
          all(value in wat for value in ('(i32.const 9))','(i32.const 125))','(i32.const 107))','(i64.const 96)) (i32.const 125)')))
    return checks


def main():
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    parser.add_argument('--source',type=Path,default=Path(__file__).resolve().parents[2])
    parser.add_argument('--out',type=Path,required=True)
    args=parser.parse_args();root=args.source.resolve(strict=True)
    files=('tests/ros_aarch64/run_revoke_reconnect.py','tests/ros_aarch64/revoke_reconnect_models.py',
           'tests/ros_lifecycle/revoke_reconnect_ros2.wat','tests/ros_lifecycle/revoke_reconnect_sros2-policy.xml',
           'tests/ros_lifecycle/ros2_revoke_reconnect_witness.cc','sdk/abi/abi-v1.json')
    before={name:pin(root/name) for name in files};checks=models(root)
    require(before=={name:pin(root/name) for name in files},'pure source changed during models')
    report={'format_version':1,'status':'PASS_PURE_REVOKE_FAULT_RECONNECT_MODELS','actual_compilation':'NOT_EXECUTED',
            'actual_signature_verification':'NOT_EXECUTED','actual_native_guest_ros_qemu':'NOT_EXECUTED','G09_complete':False,
            'scope':'Independent synthetic lifecycle oracle and AST-extracted front-door only; no execution controller/helper import or real products.',
            'source_pins':before,'checks':len(checks),'cases':checks}
    args.out.parent.mkdir(parents=True,exist_ok=True)
    with args.out.open('x') as stream:json.dump(report,stream,indent=2);stream.write('\n')
    print(json.dumps({'report':pin(args.out),'checks':len(checks),'status':report['status']}))


if __name__=='__main__':main()
