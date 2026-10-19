#!/usr/bin/env python3
"""Explicit future Linux producer. AUTHOR SOURCE: NEVER EXECUTED IN THIS TASK.

Invoke only through the reviewed captured-entry bootstrap, with externally
admitted complete Source/tool/runtime/classlib/startup closure and existing
64GiB cgroup. No download, workload install, SDK edit or Host transport exists.
Successful production records actual AOT objects; it does NOT qualify Guest
execution, every managed method, permissions, DBG, Native ROS or performance.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import selectors
import shutil
import stat
import subprocess
import sys
import time
import types
from xml.sax.saxutils import escape

COMMIT='573461dc897731c11d6166574b77a8c3aabba220'
RUNTIME='9.0.0-preview.6.24327.7'
CGROUP_MEMORY=64<<30
BASE='sdk/dotnet/mono-wasi-aot'
PROJECTS={'csharp':('CSharp','CSharp.csproj','NonRos.csproj'),
          'fsharp':('FSharp','FSharp.fsproj','NonRos.fsproj'),
          'visualbasic':('VisualBasic','VisualBasic.vbproj','NonRos.vbproj')}

def sha(raw):return hashlib.sha256(raw).hexdigest()
def identity(s):return tuple(str(getattr(s,'st_'+k))for k in('dev','ino','mode','nlink','uid','gid','size','mtime_ns','ctime_ns'))
def simple(path):
    p=Path(path)
    if not p.is_absolute() or not re.fullmatch(r'/[A-Za-z0-9_.+/-]+',str(p)) or '..'in p.parts:
        raise ValueError('explicit absolute path without shell/MSBuild delimiters required')
    return p
def read(path,limit=32<<20,expected=None):
    path=simple(path)
    if path.resolve(strict=True)!=path:raise ValueError('resolved admitted file path required, including ancestors')
    fd=os.open(path,os.O_RDONLY|os.O_NONBLOCK|os.O_NOFOLLOW|os.O_CLOEXEC)
    try:
        first=os.fstat(fd)
        if not stat.S_ISREG(first.st_mode)or not 0<=first.st_size<=limit:raise ValueError('bounded regular file required')
        pieces=[];total=0
        while total<=first.st_size:
            b=os.read(fd,min(65536,first.st_size+1-total))
            if not b:break
            pieces.append(b);total+=len(b)
        raw=b''.join(pieces)
        if len(raw)!=first.st_size or identity(first)!=identity(os.fstat(fd))or identity(first)!=identity(os.lstat(path)):
            raise ValueError('admitted file changed while reading')
        if expected is not None and sha(raw)!=expected:raise ValueError('exact admitted SHA differs: '+str(path))
        return raw,{'bytes':len(raw),'sha256':sha(raw),'stat':identity(first)}
    finally:os.close(fd)
def strict(raw):
    def pairs(rows):
        result={}
        for key,value in rows:
            if key in result:raise ValueError('duplicate JSON key')
            result[key]=value
        return result
    return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:(_ for _ in()).throw(ValueError('nonfinite JSON')))
def write(path,raw):
    path=Path(path);path.parent.mkdir(parents=True,exist_ok=True)
    with path.open('xb')as stream:stream.write(raw)
def record(path,value):write(path,(json.dumps(value,indent=2)+'\n').encode())
def load_bytes(name,path,raw):
    if name in sys.modules:raise ValueError('private module name already present')
    module=types.ModuleType(name);module.__file__=str(path)
    module.__package__='';module.__cached__=module.__spec__=module.__loader__=None
    sys.modules[name]=module
    try:exec(compile(raw,str(path),'exec',dont_inherit=True),module.__dict__)
    except BaseException:sys.modules.pop(name,None);raise
    return module

class Admission:
    def __init__(self,path,expected):
        raw,self.pin=read(path,8<<20,expected);self.value=strict(raw);self.rows={};self.source={};self.total=0
        v=self.value
        if v.get('format')!='uwvm-mono-wasi-aot-offline-admission-v1'or v.get('runtime_commit')!=COMMIT or v.get('runtime_version')!=RUNTIME:
            raise ValueError('independently admitted exact Preview6 research route required')
        self.guard_pin=v['existing_cgroup']
        self.guard()  # Check the actual group before admitting any large tool file.
        if not re.fullmatch(r'9\.0\.100-preview\.[0-9]+\.[0-9.]+',v['sdk_version']):raise ValueError('explicit prerelease9 SDK version required; no roll-forward')
        for name in('sdk_root','wasi_sdk_root','runtime_pack_root','offline_nuget'):
            p=simple(v[name])
            if p.resolve(strict=True)!=p or not p.is_dir():raise ValueError('existing resolved offline root required')
        if type(v['tool_files'])is not list or not 1<=len(v['tool_files'])<=20000:raise ValueError('complete finite tool/native/classlib file closure required')
        for row in v['tool_files']:
            p=simple(row['path'])
            if str(p)in self.rows:raise ValueError('duplicate admitted tool/runtime file')
            # Hash by bounded blocks, without retaining multi-GiB SDK binaries.
            s=p.lstat();h=hashlib.sha256();n=0
            if not stat.S_ISREG(s.st_mode)or not 0<=s.st_size<=1<<30:raise ValueError('ordinary bounded admitted tool file required')
            with p.open('rb')as f:
                first=os.fstat(f.fileno())
                if identity(first)!=identity(s):raise ValueError('tool inode changed')
                while True:
                    b=f.read(65536)
                    if not b:break
                    h.update(b);n+=len(b)
                    if n>1<<30:raise ValueError('tool file grew')
                if identity(first)!=identity(os.fstat(f.fileno()))or identity(first)!=identity(p.lstat()):raise ValueError('tool file changed')
            if n!=row['bytes']or h.hexdigest()!=row['sha256']:raise ValueError('tool/classlib admission differs')
            self.total+=n
            if self.total>8<<30:raise ValueError('finite aggregate tool admission budget exceeded')
            self.rows[str(p)]={'bytes':n,'sha256':h.hexdigest(),'stat':identity(s)}
        tools=v['tools']
        if set(tools)!={'dotnet','clang','crosscompiler','llvm-dis','llvm-nm','llvm-objdump','validator'}:raise ValueError('all actual tools, including real Mono crosscompiler, required')
        for path in tools.values():
            if str(simple(path))not in self.rows:raise ValueError('selected executable not in admitted exact file closure')
        if not 1<=len(v['sources'])<=2048:raise ValueError('bounded externally reviewed Source closure required')
        for row in v['sources']:
            target=Path(row['target'])
            if target.is_absolute()or '..'in target.parts or str(target)in self.source:raise ValueError('unique relative Source destination required')
            raw,pin=read(simple(row['source']),8<<20,row['sha256'])
            if len(raw)!=row['bytes']:raise ValueError('Source extent differs')
            self.source[str(target)]=(raw,pin,row['source'])
        roots=[simple(path)for path in v['source_roots']]
        if any(p.resolve(strict=True)!=p or not p.is_dir()for p in roots):raise ValueError('resolved complete Source roots required')
        if not roots or any(not any(simple(row['source']).is_relative_to(base)for base in roots)for row in v['sources']):raise ValueError('actual complete Source roots required')
        if str(Path(sys.executable).resolve(strict=True))not in self.rows:raise ValueError('actual Python entry executable also requires independent binary/stdlib admission')
        if sum(len(r[0])for r in self.source.values())>64<<20:raise ValueError('aggregate captured Source budget')
        if self.source.get(BASE+'/producer.py',(b'',))[0]!=globals().get('__captured_source__'):
            raise ValueError('actual captured entry bytes differ from admitted producer Source; ordinary loader/PYC is insufficient')
        self.staged_root=None;self.staged_extra={}
        self.guard()
    def guard(self):
        if sys.platform!='linux':raise ValueError('only the already admitted SSH Linux execution keeper may run this producer')
        g=self.guard_pin;group=g['path'];path=Path('/sys/fs/cgroup')/group.lstrip('/')
        if (not group.startswith('/')or '..'in Path(group).parts or Path('/proc/self/cgroup').read_text().strip()!='0::'+group):raise ValueError('existing exact cgroup membership differs')
        s=path.stat()
        if (str(s.st_dev),str(s.st_ino))!=(g['dev'],g['ino'])or Path('/proc/sys/kernel/random/boot_id').read_text().strip()!=g['boot_id']:
            raise ValueError('existing cgroup directory/boot identity differs')
        if (path/'memory.max').read_text().strip()!=str(CGROUP_MEMORY)or(path/'memory.swap.max').read_text().strip()!='0':raise ValueError('existing64GiB/no-swap cgroup required')
        fields=dict(line.split()for line in(path/'memory.stat').read_text().splitlines())
        return {'memory_events':(path/'memory.events').read_text(),'anon':int(fields['anon']),'cgroup_dev':str(s.st_dev),'cgroup_ino':str(s.st_ino)}
    def recheck(self):
        for path,pin in self.rows.items():
            if identity(Path(path).lstat())!=pin['stat']:raise ValueError('admitted tool/runtime inode/stat changed')
        for raw,pin,path in self.source.values():
            again,now=read(path,8<<20,pin['sha256'])
            if again!=raw or now!=pin:raise ValueError('captured source changed')
        if self.staged_root is not None:
            for relative,(raw,_,_)in self.source.items():
                if read(self.staged_root/relative,8<<20,sha(raw))[0]!=raw:raise ValueError('staged build Source differs')
            for relative,raw in self.staged_extra.items():
                if read(self.staged_root/relative,8<<20,sha(raw))[0]!=raw:raise ValueError('staged SDK selection differs')
        self.guard()

class PipeDrains:
    """Parent-only, bounded nonblocking PIPE collection; no reader threads.

    Each poll consumes at most four64KiB blocks per ready stream, so an
    unbounded writer cannot suppress resource/deadline/ownership observation.
    A capped prefix is retained on disk; overflow is a failed command, never a
    successful truncation. EOF means a real zero-byte read, not leader exit.
    """
    LIMIT=64<<20
    def __init__(self,folder):
        self.selector=selectors.DefaultSelector();self.rows={};self.errors=[];self.closed=False
        try:
            for label in('stdout','stderr'):
                self.rows[label]={'pipe':None,'sink':None,'registered':False,
                    'seen':0,'retained':0,'eof':False,'overflow':False,
                    'forced_close':False,'pipe_closed':False,'sink_closed':False,'errors':[]}
                self.rows[label]['sink']=(folder/(label+'.log')).open('xb',buffering=0)
        except BaseException:
            self.stop();raise
    def attach(self,child):
        # Take both handles first: a failure configuring stdout must not lose
        # ownership of the already-created stderr descriptor.
        for label,row in self.rows.items():row['pipe']=getattr(child,label)
        for row in self.rows.values():
            if row['pipe'] is None:raise RuntimeError('actual child PIPE is missing')
        for label,row in self.rows.items():
            os.set_blocking(row['pipe'].fileno(),False)
            self.selector.register(row['pipe'],selectors.EVENT_READ,label);row['registered']=True
    def poll(self):
        if self.closed:return
        try:ready=self.selector.select(0)
        except BaseException as error:
            self.errors.append(error);return
        for key,_ in ready:
            row=self.rows[key.data]
            for _ in range(4):
                try:chunk=os.read(key.fd,65536)
                except BlockingIOError:break
                except BaseException as error:
                    row['errors'].append(error);break
                if not chunk:
                    row['eof']=True
                    try:self.selector.unregister(row['pipe']);row['registered']=False
                    except BaseException as error:row['errors'].append(error)
                    break
                row['seen']+=len(chunk)
                keep=min(len(chunk),max(0,self.LIMIT-row['retained']))
                if keep and not row['errors']:
                    try:
                        part=memoryview(chunk)[:keep]
                        while part:
                            size=row['sink'].write(part)
                            if size is None or size<=0:raise OSError('parent log write made no progress')
                            row['retained']+=size;part=part[size:]
                    except BaseException as error:row['errors'].append(error)
                if row['seen']>self.LIMIT:row['overflow']=True
    def failed(self):
        return bool(self.errors)or any(row['errors']or row['overflow']for row in self.rows.values())
    def eof(self):return all(row['eof']for row in self.rows.values())
    def stop(self):
        # Synchronous collector: no detached reader or reader join is needed.
        # Only stop closes descriptors, after the bounded EOF grace period.
        if self.closed:return
        for row in self.rows.values():
            row['forced_close']=row['pipe'] is not None and not row['eof']
            if row['registered']:
                try:self.selector.unregister(row['pipe']);row['registered']=False
                except BaseException as error:row['errors'].append(error)
            if row['pipe'] is not None:
                try:row['pipe'].close();row['pipe_closed']=True
                except BaseException as error:row['errors'].append(error)
            if row['sink'] is not None:
                try:row['sink'].close();row['sink_closed']=True
                except BaseException as error:row['errors'].append(error)
        try:self.selector.close();self.closed=True
        except BaseException as error:self.errors.append(error)
    def evidence(self):
        return {'model':'parent synchronous nonblocking selector; no reader threads',
            'per_stream_limit':self.LIMIT,'closed':self.closed,
            'errors':[repr(e)for e in self.errors],
            'streams':{label:{k:([repr(e)for e in value]if k=='errors'else value)
                for k,value in row.items()if k not in('pipe','sink')}
                for label,row in self.rows.items()}}
    def complete(self):
        return self.closed and not self.failed()and all(
            row['eof']and not row['forced_close']and not row['registered']and
            row['pipe_closed']and row['sink_closed']and row['seen']==row['retained']
            for row in self.rows.values())

class OwnedRunner:
    """Actual bounded owner API; sampled helper discovery limits are retained."""
    def __init__(self,admission,stage):
        self.a,self.stage=admission,stage;self.commands=[];self.origin=shutil.disk_usage(stage).free
        key='test/0050.high_level_languages/owned_process.py'
        raw,_,path=admission.source[key]
        self.module_name='_uwvm_mono_aot_actual_pidfd_owner'
        self.owner=load_bytes(self.module_name,path,raw)
    def run(self,name,args,cwd,env,timeout=1800):
        if not 1<=timeout<=1800:raise ValueError('finite process deadline')
        self.a.recheck();folder=self.stage/'commands'/name;folder.mkdir(parents=True,exist_ok=False)
        before=self.a.guard();start=time.monotonic();child=drains=None;read_fd=write_fd=None
        row={'argv':list(map(str,args)),'cwd':str(cwd),'before':before,'observations':[],
             'passed':False,'physical_retirement':False,'leader_joined':False,
             'cleanup_errors':[],'ownership':None,'output_complete':False}
        # Record ownership of this row before any tool is released.
        self.commands.append(row);primary=None;cleanup=[];last=0.0
        def remember_cleanup(error):
            cleanup.append(error);row['cleanup_errors'].append(repr(error))
        def observe(force=False):
            nonlocal last
            if drains is not None:
                drains.poll()
                if drains.failed():raise RuntimeError('parent PIPE overflow/read/write failure')
            now=time.monotonic()
            if not force and now-last<0.5:return
            last=now;point=self.a.guard()
            if point['memory_events']!=before['memory_events']or point['anon']>=56<<30:raise RuntimeError('new cgroup pressure/shared anonymous budget')
            free=shutil.disk_usage(self.stage).free
            if free<2<<30 or self.origin-free>3<<30:raise RuntimeError('disk reserve/cumulative producer budget')
            rss=0
            ownership=self.owner.owned_process_record(child)if child is not None else None
            for member in ownership['members']if ownership is not None else():
                pid=member['identity']['pid']
                try:
                    actual=Path('/proc/'+str(pid)+'/stat').read_text().rpartition(')')[2].split()
                    if int(actual[19])!=member['identity']['start']:continue
                    if Path('/proc/'+str(pid)+'/cgroup').read_text().strip()!='0::'+self.a.guard_pin['path']:raise RuntimeError('owned child escaped admitted cgroup')
                    status=dict(line.split(':',1)for line in Path('/proc/'+str(pid)+'/status').read_text().splitlines()if ':'in line)
                    if actual[0]not in('Z','X')and 'VmRSS'not in status:raise RuntimeError('live owned process has no RSS witness')
                    rss+=int(status.get('VmRSS','0 kB').split()[0])*1024
                except(FileNotFoundError,ProcessLookupError):continue
            if rss>=8<<30:raise RuntimeError('actual owned process RSS exceeds8GiB')
            row['observations'].append({'seconds':now-start,'rss':rss,'shared_anon':point['anon'],'disk_free':free})
        bootstrap=('import os,resource,sys; f=int(sys.argv[1]); '
            'resource.setrlimit(resource.RLIMIT_CORE,(0,0)); '
            'resource.setrlimit(resource.RLIMIT_AS,(8<<30,8<<30)); '
            'resource.setrlimit(resource.RLIMIT_FSIZE,(512<<20,512<<20)); '
            'assert os.read(f,1)==b"G"; os.close(f); os.execve(sys.argv[2],sys.argv[2:],os.environ)')
        try:
            drains=PipeDrains(folder);read_fd,write_fd=os.pipe()
            child=subprocess.Popen([sys.executable,'-I','-S','-c',bootstrap,str(read_fd),*map(str,args)],cwd=cwd,env=env,
                start_new_session=True,pass_fds=(read_fd,),stdin=subprocess.DEVNULL,
                stdout=subprocess.PIPE,stderr=subprocess.PIPE,bufsize=0)
            # Capture both output descriptors before any other fallible
            # post-spawn operation; the tool is still behind its barrier.
            drains.attach(child)
            os.close(read_fd);read_fd=None
            self.owner.remember_process(child,force=True)
            observe(force=True)
            if os.write(write_fd,b'G')!=1:raise RuntimeError('tool start barrier was not released exactly once')
            os.close(write_fd);write_fd=None
            row['exit']=self.owner.wait_process(child,timeout,observe=observe)
        except BaseException as error:primary=error;row['primary']=repr(error)
        finally:
            for fd in(read_fd,write_fd):
                if fd is not None:
                    try:os.close(fd)
                    except BaseException as error:remember_cleanup(error)
            if child is not None:
                try:
                    # Keeps the exact real pidfd helper. This bounded cleanup
                    # can signal retained helpers; such a result MUST fail.
                    self.owner.terminate_group(child)
                    row['physical_retirement']=True
                    row['leader_joined']=child.returncode is not None
                except BaseException as error:remember_cleanup(error)
                try:row['ownership']=self.owner.owned_process_record(child)
                except BaseException as error:remember_cleanup(error)
            if drains is not None:
                # Retained helpers can inherit writer FDs. Leader exit is not
                # EOF. Allow only this bounded grace, then explicitly stop.
                deadline=time.monotonic()+3.0
                try:
                    while not drains.eof()and time.monotonic()<deadline:
                        drains.poll()
                        try:observe()
                        except BaseException as error:
                            remember_cleanup(error)
                            # Continue draining, without growing an error list
                            # on each subsequent poll of the same failed guard.
                            break
                        if not drains.eof():time.sleep(0.01)
                    if not drains.eof():remember_cleanup(TimeoutError('PIPE EOF incomplete within3s after owned retirement'))
                except BaseException as error:remember_cleanup(error)
                finally:
                    drains.stop();row['output']=drains.evidence();row['output_complete']=drains.complete()
            # Both natural zero exit and complete, unsignalled pidfd closure
            # are mandatory. Missing/null/partial ownership never qualifies.
            owned=row['ownership']
            row['ownership_complete']=(type(owned)is dict and owned.get('closed')is True and
                type(owned.get('signals'))is list and not owned['signals']and
                type(owned.get('members'))is list and bool(owned['members'])and
                owned.get('leader_pid')==(child.pid if child is not None else None)and
                type(owned.get('leader_start'))is int and owned['leader_start']>0 and
                all(type(m)is dict and m.get('pidfd_retained')is False for m in owned['members'])and
                any(m.get('identity',{}).get('pid')==owned['leader_pid']and
                    m.get('identity',{}).get('start')==owned['leader_start']for m in owned['members']))
            for filename in('stdout.log','stderr.log'):
                if(folder/filename).exists():
                    try:row[filename]=read(folder/filename,PipeDrains.LIMIT)[1]
                    except BaseException as error:remember_cleanup(error)
            try:observe(force=True);self.a.recheck()
            except BaseException as error:remember_cleanup(error)
            row['seconds']=time.monotonic()-start
            row['passed']=(primary is None and not cleanup and row.get('exit')==0 and
                child is not None and child.returncode==0 and row['leader_joined']and
                row['physical_retirement']and row['ownership_complete']and row['output_complete'])
            record(folder/'receipt.json',row)
        if primary is not None:raise primary
        if cleanup:raise cleanup[0]
        if not row['passed']:raise RuntimeError('actual owned command failed: '+name)
        return row
    def close(self):sys.modules.pop(self.module_name,None)

def produce(admission_path,admission_sha,stage,language,ros,mode,case='application'):
    a=Admission(admission_path,admission_sha);v=a.value;stage=simple(stage)
    forbidden=[simple(v[k])for k in('sdk_root','wasi_sdk_root','runtime_pack_root','offline_nuget')]+[simple(p)for p in v['source_roots']]
    if ros:forbidden.append(simple(v['canonical_source_revision']))
    if stage.exists()or stage.resolve(strict=False)!=stage or any(stage.is_relative_to(p)or p.is_relative_to(stage)for p in forbidden):
        raise ValueError('fresh output disjoint from every admitted tool/runtime root required')
    if language not in PROJECTS or ros not in(0,1)or mode not in('LLVMOnly','LLVMOnlyInterp'):raise ValueError('explicit finite route selection')
    if case not in('application','guest-faults')or case=='guest-faults'and(language!='csharp'or not ros):raise ValueError('real shared C#-SDK fault build is explicit ROS2-only')
    required={BASE+'/producer.py',BASE+'/inspect_aot.py',BASE+'/Aot.props',BASE+'/Aot.targets',BASE+'/Directory.Build.props','test/0050.high_level_languages/owned_process.py'}
    required.add('sdk/tools/check_wasm_imports.py')
    if ros:
        required|={'sdk/csharp/RosWasm/'+name+'.cs'for name in('Actions','Codecs','Events','NativeTransport','Node','Result','Retirement','SelectedRaw','Session')}
        required|={'sdk/csharp/owned/'+name+'.cs'for name in('NativeOwned','PreparedSession','OwnedWire','OwnedGroup','ControlWire')}
        required|={BASE+'/'+name for name in('generate_control.py','control_csharp_emitter.py','Uwvm.RosWasm.Aot.csproj','ApplicationLifetime.cs','AotWitness.cs')}
        required|={'sdk/csharp/roswasm_bridge.c','sdk/csharp/owned/roswasm_owned_bridge.c','sdk/csharp/owned/include/roswasm_csharp_owned_bridge.h',
            'sdk/ros2/abi.h','sdk/common/abi.h','sdk/include/roswasm/rmw_service_v1.h','sdk/abi/abi-v1.json','sdk/abi/rmw-service-v1.json'}
    if not required<=a.source.keys():raise ValueError('complete actual entry/helper closure required')
    if case=='guest-faults'and not {BASE+'/tests/GuestFaults.csproj','sdk/csharp/owned/examples/AppCommon.cs','sdk/csharp/owned/tests/GuestFaultFixture.cs'}<=a.source.keys():raise ValueError('actual existing fault callers and complete SDK sources required')
    root=stage/'source';stage.mkdir(parents=True,exist_ok=False)
    for rel,(raw,_,_)in a.source.items():write(root/rel,raw)
    build=stage/'build';evidence=stage/'evidence';evidence.mkdir();build.mkdir()
    global_source=(json.dumps({'sdk':{'version':v['sdk_version'],'allowPrerelease':True,'rollForward':'disable'}})+'\n').encode()
    if 'global.json'in a.source:raise ValueError('producer writes its own exact SDK selection, not a second ambiguous global.json')
    write(root/'global.json',global_source);a.staged_root=root;a.staged_extra={'global.json':global_source}
    write(stage/'NuGet.config',('<configuration><packageSources><clear/><add key="admitted-offline" value="'+escape(v['offline_nuget'],{'"':'&quot;'})+'"/></packageSources></configuration>\n').encode())
    env={k:os.environ[k]for k in('LANG','LC_ALL','TZ')if k in os.environ}
    env.update(PATH=str(Path(v['tools']['dotnet']).parent)+':'+str(Path(v['tools']['clang']).parent)+':/usr/bin:/bin',
        DOTNET_ROOT=v['sdk_root'],DOTNET_CLI_HOME=str(stage/'cli-home'),DOTNET_SKIP_FIRST_TIME_EXPERIENCE='1',DOTNET_NOLOGO='1',
        DOTNET_CLI_TELEMETRY_OPTOUT='1',DOTNET_CLI_WORKLOAD_UPDATE_NOTIFY_DISABLE='true',DOTNET_GCHeapHardLimit='0x80000000',
        MSBUILDDISABLENODEREUSE='1',MSBUILDTERMINALLOGGER='off',NUGET_PACKAGES=str(stage/'packages'),WASI_SDK_PATH=v['wasi_sdk_root'])
    runner=OwnedRunner(a,stage);result={'status':'INCOMPLETE_NOT_QUALIFIED','commands':runner.commands,'admission':a.pin,'requested_mode':mode,'language':language,'ros':bool(ros),'case':case,'runtime_commit':COMMIT,'candidate_execution_before_this_run':0}
    try:
        runner.run('sdk-version',[v['tools']['dotnet'],'--version'],root,env,60)
        if read(stage/'commands/sdk-version/stdout.log',4096)[0].decode().strip()!=v['sdk_version']:raise ValueError('actual SDK resolution differs; no roll-forward/download fallback')
        generated=stage/'generated'
        if ros:
            operator=root/BASE/'generate_control.py';emitter=operator.with_name('control_csharp_emitter.py')
            module=load_bytes('_uwvm_mono_aot_control_generator',operator,a.source[BASE+'/generate_control.py'][0])
            try:module.run(root,simple(v['canonical_source_revision']),generated,sha(read(operator)[0]),sha(read(emitter)[0]))
            finally:sys.modules.pop('_uwvm_mono_aot_control_generator',None)
            common=[v['tools']['clang'],'--target=wasm32-wasi','--sysroot='+v['wasi_sdk_root']+'/share/wasi-sysroot','-O2','-g','-DROSWASM_BACKEND=2','-DROSWASM_OWNED_SERVICE=1','-I'+str(root/'sdk'),'-I'+str(root/'sdk/include'),'-I'+str(root/'sdk/csharp/owned/include')]
            for name,source in [('roswasm_bridge','sdk/csharp/roswasm_bridge.c'),('roswasm_owned_bridge','sdk/csharp/owned/roswasm_owned_bridge.c')]:
                runner.run('compile-'+name,common+['-c',root/source,'-o',build/(name+'.o')],root,env,300)
        group,ros_project,nonros_project=PROJECTS[language];project=root/BASE/'apps'/group/(ros_project if ros else nonros_project)
        if case=='guest-faults':project=root/BASE/'tests/GuestFaults.csproj'
        properties=['-p:RosAotOptIn=1','-p:RosAotRos='+str(ros),'-p:RosBackend=2','-p:RosOwnedService=1','-p:AOTMode='+mode,
            '-p:RosBuildRoot='+str(build),'-p:RosSourceRoot='+str(root),'-p:RosAotEvidenceDir='+str(evidence),'-p:RosOwnedGeneratedDir='+str(generated),
            '-p:RosNativeBridgeObject='+str(build/'roswasm_bridge.o'),'-p:RosOwnedBridgeObject='+str(build/'roswasm_owned_bridge.o'),
            '-p:WasiSdkRoot='+v['wasi_sdk_root'],'-p:MicrosoftNetCoreAppRuntimePackDir='+v['runtime_pack_root'],
            '-p:BuildInParallel=false','-p:UseSharedCompilation=false','-p:MSBuildEnableWorkloadResolver=true']
        runner.run('offline-restore',[v['tools']['dotnet'],'restore',project,'--configfile',stage/'NuGet.config','--disable-parallel',*properties],root,env)
        runner.run('actual-aot-publish',[v['tools']['dotnet'],'publish',project,'--no-restore','-c','Release','-m:1','-nodeReuse:false','-v:normal','-bl:'+str(evidence/'publish.binlog'),*properties],root,env)
        inspector_path=root/BASE/'inspect_aot.py';inspector=load_bytes('_uwvm_mono_aot_actual_artifact_inspector',inspector_path,a.source[BASE+'/inspect_aot.py'][0])
        try:result['artifacts']=inspector.inspect(a,runner,root,build,evidence,env,language,bool(ros),mode,case)
        finally:sys.modules.pop('_uwvm_mono_aot_actual_artifact_inspector',None)
        a.recheck();result['status']='ACTUAL_AOT_ARTIFACT_EVIDENCE_ONLY_RUNTIME_AND_DEBUG_UNQUALIFIED'
        return result
    except BaseException as error:result['primary']=repr(error);raise
    finally:record(stage/'producer.json',result);runner.close()

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--admission',type=Path,required=True);p.add_argument('--admission-sha256',required=True)
    p.add_argument('--stage',type=Path,required=True);p.add_argument('--language',choices=tuple(PROJECTS),required=True)
    p.add_argument('--ros',type=int,choices=(0,1),required=True);p.add_argument('--mode',choices=('LLVMOnly','LLVMOnlyInterp'),required=True)
    p.add_argument('--case',choices=('application','guest-faults'),default='application')
    args=p.parse_args()
    if sys.platform!='linux':raise ValueError('all candidate operations require the admitted Linux keeper')
    import resource
    import signal
    # Bound the producer itself, including in-process captured generation and
    # artifact reading. A lower inherited hard limit is never raised or skipped.
    for which,ceiling in((resource.RLIMIT_CORE,0),(resource.RLIMIT_AS,8<<30),(resource.RLIMIT_FSIZE,512<<20)):
        _,hard=resource.getrlimit(which)
        limit=ceiling if hard==resource.RLIM_INFINITY else min(hard,ceiling)
        resource.setrlimit(which,(limit,limit))
    def deadline(_number,_frame):raise TimeoutError('finite total producer deadline1800s; preserve incomplete receipt')
    signal.signal(signal.SIGALRM,deadline);signal.alarm(1800)
    try:produce(args.admission,args.admission_sha256,args.stage,args.language,args.ros,args.mode,args.case)
    finally:signal.alarm(0)
if __name__=='__main__':main()
