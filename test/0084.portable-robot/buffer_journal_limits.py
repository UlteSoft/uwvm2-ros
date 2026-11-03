# SPDX-License-Identifier: Apache-2.0
"""Run within the admitted 64GiB queue: python3 -B buffer_journal_limits.py /path/to/vm.
UWVM_SDK_OUTPUT must name a fresh private output directory.
"""
from pathlib import Path
import hashlib,json,os,subprocess,sys
here=Path(__file__).parent;out=Path(os.environ['UWVM_SDK_OUTPUT']);vm=Path(sys.argv[1])
root=Path(__file__).resolve().parents[2]
cgroup=Path('/proc/self/cgroup').read_text().strip().split(':',2)[2]
assert cgroup.startswith('/uwvmros.slice/'), 'Run through the shared bounded queue'
common=Path('/sys/fs/cgroup/uwvmros.slice')
assert (common/'memory.max').read_text().strip()=='68719476736' and (common/'memory.swap.max').read_text().strip()=='0'
leaf=Path('/sys/fs/cgroup'+cgroup)
assert (leaf/'memory.max').read_text().strip().isdigit() and int((leaf/'memory.max').read_text())<=68719476736 and (leaf/'memory.swap.max').read_text().strip()=='0'
exe=out/'journal-boundaries'
cmd=['/usr/lib/llvm-22/bin/clang++','-std=c++23','-O1','-pthread','-I'+str(root/'src'),'-I'+str(root/'third-parties/fast_io/include'),str(root/'test/0084.portable-robot/buffer_journal_limits.cc'),'-o',str(exe)]
with(out/'compile.log').open('xb') as f:subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,check=True,timeout=120)
report={'compile':cmd,'cases':[],'official_scene':False,'scope':'host journal component plus actual VM CLI rejection'}
def save():(out/'checks.json').write_text(json.dumps(report,indent=2)+'\n')
save()
for mode in ['full','default','count','bytes','record','payload','source','concurrent','short-write','protocol-max','capture','allocation-failure']:
 p=out/(mode+'.jsonl')
 with(out/(mode+'.stdout')).open('xb') as f,(out/(mode+'.stderr')).open('xb') as e:subprocess.run([str(exe),mode,str(p)],stdout=f,stderr=e,check=True,timeout=30)
 raw=p.read_bytes()
 if mode=='allocation-failure':
  assert raw==b''
  report['cases'].append({'case':mode,'configuration_rejected_before_journal':True});save();continue
 if mode=='short-write':
  assert len(raw)==4096 and not raw.endswith(b'\n') and b'"kind":"end"' not in raw
  report['cases'].append({'case':mode,'expected_incomplete_journal':True,'bytes':len(raw)});save();continue
 rows=[json.loads(l) for l in raw.splitlines()];begin=rows[0];end=rows[-1];body=rows[1:-1]
 assert begin['kind']=='begin' and end['kind']=='end' and all(r['kind']=='operation' for r in body)
 assert end['emitted_operations']==len(body) and end['total_file_bytes']==len(raw)
 assert end['bytes_before_footer']==sum(map(len,raw.splitlines(keepends=True)[:-1]))
 assert len(raw)<=begin['max_bytes'] and all(len(l)<=begin['max_record_bytes'] for l in raw.splitlines(keepends=True))
 expected={'full':(1,0,0),'default':(1,1,0),'count':(2,1,0),'bytes':(0,0,1),'record':(0,0,1),'payload':(0,1,0),'source':(0,1,0),'concurrent':(400,0,0),'protocol-max':(1,0,0),'capture':(0,0,1)}[mode]
 assert (len(body),end['suppressed_records'],end['dropped_records'])==expected,(mode,end)
 for row in body:
  for direction in ['input','output']:
   data=bytes.fromhex(row['canonical_'+direction+'_hex']);assert hashlib.sha256(data).hexdigest()==row['canonical_'+direction+'_sha256']
 if mode=='full':
  assert bytes.fromhex(body[0]['canonical_input_hex'])==bytes(range(256))*256
  assert bytes.fromhex(body[0]['canonical_output_hex'])==bytes(range(255,-1,-1))*256
  assert len(raw)>262144 and begin['requested_profile_sha256']=='a'*64
 if mode=='protocol-max':
  assert bytes.fromhex(body[0]['canonical_input_hex'])==bytes([0x31])*4194304 and bytes.fromhex(body[0]['canonical_output_hex'])==bytes([0xa9])*4194304
  assert len(raw)>16777216
 if mode=='concurrent':assert len({(r['module_instance'],r['owner_handle']) for r in body})==400
 if mode!='default':assert begin['requested_profile_sha256']=='a'*64
 key={'default':'payload_suppressed','count':'count_suppressed','bytes':'byte_dropped','record':'record_dropped','payload':'payload_suppressed','source':'source_suppressed','capture':'capture_dropped'}.get(mode)
 if key:assert end[key]==1
 report['cases'].append({'case':mode,'bytes':len(raw),'footer':end});save()
valid=['65536','22000','134217728','524288','a'*64]
invalid=[]
for index,values in [(0,['0','4194305','-1','18446744073709551616']),(1,['0','4294967296']),(2,['8191','4398046511105']),(3,['1023','33554433']),(4,['0'*64,'A'*64,'a'*63])]:
 for value in values:
  args=valid.copy();args[index]=value;invalid.append((f'field-{index}-{value[:16]}',args))
for ordinal,(label,args) in enumerate(invalid):
 p=out/f'cli-{ordinal}.jsonl';fd=os.open(p,os.O_WRONLY|os.O_CREAT|os.O_EXCL,0o600)
 command=[str(vm),'--ros2-buffer-audit-limits',*args,'--ros2-buffer-audit','invalid-'+str(ordinal),str(fd),'--run','/nonexistent-guest-must-not-run.wasm']
 try:r=subprocess.run(command,pass_fds=(fd,),capture_output=True,timeout=10)
 finally:os.close(fd)
 (out/f'cli-{ordinal}.stderr').write_bytes(r.stderr);assert r.returncode and ((b'buffer journal limits' in r.stderr or (args[0]=='18446744073709551616' and b'quota must be an unsigned decimal integer' in r.stderr)) or (args[0]=='-1' and b'invalid parameter:' in r.stderr and b'-1' in r.stderr)) and p.stat().st_size==0,(label,r.stderr)
 report['cases'].append({'case':'CLI-reject-'+label,'exit_code':r.returncode,'argv':command});save()
for label,args in [('orphan',valid),('out-of-order',valid+['--version'])]:
 r=subprocess.run([str(vm),'--ros2-buffer-audit-limits',*args],capture_output=True,timeout=10)
 (out/(label+'.stderr')).write_bytes(r.stderr);assert r.returncode and b'buffer journal limits' in r.stderr
 report['cases'].append({'case':label,'exit_code':r.returncode});save()
for label in ['duplicate','late']:
 paths=[out/(label+str(i)+'.jsonl') for i in range(2)]
 fds=[os.open(p,os.O_WRONLY|os.O_CREAT|os.O_EXCL,0o600) for p in paths]
 first=['--ros2-buffer-audit-limits',*valid] if label=='duplicate' else []
 command=[str(vm),*first,'--ros2-buffer-audit',label+'-first',str(fds[0]),'--ros2-buffer-audit-limits',*valid,'--ros2-buffer-audit',label+'-second',str(fds[1]),'--run','/nonexistent-guest-must-not-run.wasm']
 try:r=subprocess.run(command,pass_fds=tuple(fds),capture_output=True,timeout=10)
 finally:
  for fd in fds:os.close(fd)
 (out/(label+'.stderr')).write_bytes(r.stderr)
 assert r.returncode and b'buffer journal limits/profile invalid, repeated, late' in r.stderr and paths[1].stat().st_size==0
 assert b'"kind":"operation"' not in paths[0].read_bytes()
 report['cases'].append({'case':label,'exit_code':r.returncode});save()
report['status']='PASS';save()
