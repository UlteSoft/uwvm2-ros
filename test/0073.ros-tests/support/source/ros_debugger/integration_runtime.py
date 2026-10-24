#!/usr/bin/env python3
"""Actual LLVM-full Runtime/ROS debugger integration; Root executes serially.

The inherited private SOCK_SEQPACKET endpoint is the production authenticated
control console: kernel credentials and the launcher's pidfd are checked by
the real Runtime. Guest stdin/stdout are different objects. No DTO callback,
Runtime identity, native IPC client, Wasm Instance or memory is fabricated.
The memory edit commands only advance the application's own safe-point gate;
they are explicitly distinguished from the read-only ROS observation window.
"""
import argparse
import array
import hashlib
import io
import json
import os
from pathlib import Path
import re
import shutil
import signal
import socket
import stat
import struct
import sys
import time
import traceback
import types

sys.dont_write_bytecode=True
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'ros_acceptance'))
from lane import Lane,add_arguments,process_identity
from protocol import require
from gateway_qualification import read_file_binding
from runtime import elf,opened_target_mappings,mapping_file_facts,_MAP_FACTS
from prepare_native import qualify_peer
from run_delivery import policy,gateway_config,worker_argv,start_master,translate,native_markers,process_state
from integration_guest import guest, MANAGEMENT_TEXT

SELECTORS=('capabilities','sessions','queues','services','actions','parameters','tf','graph','stats')
PIN_KEYS={'path','device','inode','bytes','mtime_ns','ctime_ns','sha256'}


class IntegrationLane(Lane):
    def qualify_dap_adapter(self):
        """Admit actual IDE projection code, never a copied console transcript."""
        relative='tools/debug/dap_adapter.py';path=self.source/relative
        fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC)
        try:
            first=os.fstat(fd);require(stat.S_ISREG(first.st_mode) and 0<first.st_size<=1024**2,
                'bounded ordinary current DAP source required')
            raw=bytearray()
            while True:
                self.check();block=os.read(fd,min(65536,1024**2+1-len(raw)))
                if not block:break
                raw.extend(block);require(len(raw)<=1024**2,'DAP source exceeds admitted bound')
            last,named=os.fstat(fd),path.lstat()
            require(len(raw)==first.st_size and all(getattr(first,key)==getattr(last,key)==getattr(named,key)
                for key in ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink')),
                'actual current DAP source descriptor changed')
            pin={'path':str(path),'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest(),
                'device':first.st_dev,'inode':first.st_ino,'mtime_ns':first.st_mtime_ns,'ctime_ns':first.st_ctime_ns}
        finally:os.close(fd)
        require(self.pin(path)==pin and self.frozen.get(relative)=={key:pin[key]for key in ('bytes','sha256')},
            'DAP projection must belong to actual complete frozen source')
        module=types.ModuleType('actual_ros_debugger_dap');module.__file__=str(path)
        exec(compile(bytes(raw),str(path),'exec'),module.__dict__)
        self.dap_adapter=module;self.report['dap_adapter_source']=pin

    def qualify_runtime(self,versions):
        """Consume the closed fresh-build contract, not an interpreter receipt.

        The producer independently verifies original full-MD/link/LLVM closure.
        This consumer reopens every retained physical fact and checks the exact
        compile/object/link association before accepting the executable. Current
        v1 adapter remains native x86_64. v2 requires an independently pinned
        foreign target/QEMU/sysroot selection. Native peers/Gateways retain their
        separate existing ARM SDK proof; their QEMU is not the Runtime's QEMU.
        """
        report,pin=self.json(self.args.runtime_build_report)
        keys={'format_version','kind','status','source','source_manifest_sha256','profile','runtime_backend','binary','elf',
              'input_pins','commands','compile_records','link','llvm_closure','full_dependencies_verified','source_preserved',
              'inputs_preserved','products_preserved','no_reuse','reused_products','compilation_executed','runtime_integration_executed'}
        foreign=type(report.get('format_version')) is int and report['format_version']==2
        if foreign:keys|={'build_configuration','build_evidence','runtime_execution'}
        require(set(report)==keys and type(report['format_version']) is int and report['format_version'] in (1,2) and
                report['kind']=='ros-debugger-runtime-build' and report['status']=='PASS' and
                report['source']==str(self.source) and report['source_manifest_sha256']==self.report['source_manifest_sha256'] and
                report['runtime_backend']=='llvm-full' and report['profile'] in ('ros1','ros2','dual') and
                (report['profile']=='dual' or versions==(int(report['profile'][-1]),)), 'closed current LLVM-full Runtime build required')
        for key in ('full_dependencies_verified','source_preserved','inputs_preserved','products_preserved','no_reuse','compilation_executed'):
            require(report[key] is True,'fresh Runtime build preservation/closure required')
        require(type(report['reused_products']) is int and report['reused_products']==0 and
                report['runtime_integration_executed'] is False,'no reused or previously integrated Runtime accepted')
        def bind(value):
            require(type(value) is dict and set(value)==PIN_KEYS and all(type(value[k]) is int for k in
                    ('device','inode','bytes','mtime_ns','ctime_ns')),'exact seven-field pin required')
            require(self.pin(value['path'])==value,'Runtime build artifact/input changed')
        bind(report['binary']);selected=elf(self.args.worker)
        self.runtime_execution=None;self.execution_adapter=None;self.runtime_target=report['elf']
        def source_module(relative,name):
            helper=self.source/relative
            descriptor=os.open(helper,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC)
            try:
                first=os.fstat(descriptor)
                require(stat.S_ISREG(first.st_mode) and 0<first.st_size<=256*1024,'bounded ordinary adapter source required')
                raw=bytearray()
                while True:
                    self.check();block=os.read(descriptor,min(65536,256*1024+1-len(raw)))
                    if not block:break
                    raw.extend(block);require(len(raw)<=256*1024,'adapter source grew beyond bound')
                last,named=os.fstat(descriptor),helper.lstat()
                require(len(raw)==first.st_size and all(getattr(first,key)==getattr(last,key)==getattr(named,key)
                    for key in ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink')),'opened adapter source changed')
                helper_pin={'path':str(helper),'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest(),
                    'device':first.st_dev,'inode':first.st_ino,'mtime_ns':first.st_mtime_ns,'ctime_ns':first.st_ctime_ns}
            finally:os.close(descriptor)
            require(self.pin(helper)==helper_pin and self.frozen.get(relative)==
                    {key:helper_pin[key] for key in ('bytes','sha256')},'execution adapter differs from complete source')
            adapter=types.ModuleType(name);adapter.__file__=str(helper)
            exec(compile(bytes(raw),adapter.__file__,'exec'),adapter.__dict__)
            return adapter
        if foreign:
            require(self.args.runtime_build_config is not None,'v2 requires the explicit original foreign build configuration')
            build_config,build_config_pin=self.json(self.args.runtime_build_config)
            bind(report['build_configuration'])
            require(build_config_pin==report['build_configuration'] and build_config['format_version']==2,
                    'foreign launch selection differs from original build configuration')
            adapter=source_module('test/0073.ros-tests/support/source/ros_debugger/target_execution.py','debugger_foreign_execution')
            roots=[Path(value).resolve(strict=True) for values in build_config['roots'].values() for value in values]
            execution=adapter.qualify(build_config,roots,self.pin)
            require(execution==report['runtime_execution'] and report['elf']==
                    {key:build_config['target'][key] for key in ('machine','bits','endian')},
                    'foreign target/QEMU/loader/sysroot receipt differs')
            adapter.check_image(self.args.worker,report['elf'],execution,self.pin)
            self.runtime_execution=execution;self.execution_adapter=adapter
            self.runtime_build_configuration=build_config;self.runtime_roots=sorted(set(roots))
            self.foreign_catalog_common=source_module('tools/ros_distro/common.py','debugger_foreign_root_catalog')
            bind(report['build_evidence']);build_evidence,evidence_pin=self.json(Path(report['build_evidence']['path']))
            require(evidence_pin==report['build_evidence'] and build_evidence['kind']=='ros-debugger-runtime-build-evidence' and
                    build_evidence['status']=='PASS' and build_evidence['source']==str(self.source) and
                    build_evidence['source_manifest_sha256']==self.report['source_manifest_sha256'] and
                    build_evidence['config']==build_config_pin and build_evidence['cleanup']['status']=='PASS' and
                    not build_evidence['cleanup']['remaining'] and all(build_evidence.get(key) is True
                    for key in ('source_preserved','inputs_preserved','products_preserved')),
                    'actual complete preserved foreign build evidence required')
            self.foreign_original_roots=build_evidence['input_roots_after']
            require({row['root'] for row in self.foreign_original_roots}=={str(root) for root in self.runtime_roots} and
                    len(self.foreign_original_roots)==len(self.runtime_roots),'complete original foreign provider-root closure required')
            for row in self.foreign_original_roots:bind(row['catalog'])
            self.foreign_roots_before=self.catalog_runtime_roots('before')
            original={row['root']:row for row in self.foreign_original_roots}
            require(all(self.same_root(row,original[row['root']]) for row in self.foreign_roots_before),
                    'current complete foreign input roots differ from original terminal build catalogs')
            self.report['foreign_runtime_roots_before']=self.foreign_roots_before
        else:
            require(self.args.runtime_build_config is None,'native v1 accepts no foreign launch override')
            require(report['elf']=={'machine':62,'bits':64,'endian':'little'} and selected['e_machine']==62,
                    'explicit native x86_64 LLVM-full Worker required by v1 adapter')
        require(read_file_binding(self.args.worker)==report['binary'],'selected actual Runtime binary differs')
        require(report['input_pins'] and report['compile_records'] and report['commands'],'complete physical build closure required')
        for item in report['input_pins']:bind(item)
        commands=report['commands']
        for command in commands:
            require(set(command)=={'role','argv','cwd','returncode','stdout','stderr'} and command['role'] in
                    ('compile','link','configure','tool-version') and type(command['returncode']) is int and command['returncode']==0 and
                    command['argv'] and all(type(x) is str and '\0' not in x for x in command['argv']) and
                    Path(command['cwd']).is_absolute(),'actual successful compiler command required')
            bind(command['stdout']);bind(command['stderr'])
        objects=[]
        for row in report['compile_records']:
            require(set(row)=={'command_index','source','object','dependency_file','dependencies'} and
                    type(row['command_index']) is int and 0<=row['command_index']<len(commands),'closed original compile record required')
            command=commands[row['command_index']];argv=command['argv']
            require(command['role']=='compile' and '-c' in argv and '-MD' in argv and '-MF' in argv and
                    row['source']['path'] in argv and row['object']['path'] in argv and row['dependency_file']['path'] in argv,
                    'original full-MD source/object/depfile command association differs')
            if foreign:
                require(all(flag in argv for flag in ('-DUWVM_USE_LLVM_JIT','-DUWVM_DISABLE_INT','-std=c++26')),
                        'foreign Runtime must retain actual LLVM-only/no-interpreter compile flags')
            for item in (row['source'],row['object'],row['dependency_file'],*row['dependencies']):bind(item)
            require(row['dependencies'] and row['source'] in row['dependencies'],'original source absent from complete dependencies')
            objects.append(row['object'])
        if foreign:
            expected_units={'src/uwvm2/runtime/lib/uwvm_runtime.default.cpp','src/uwvm2/uwvm/main.default.cpp','src/uwvm2/uwvm/host_api.default.cpp'}
            require(len(report['compile_records'])==3 and {str(Path(item['source']['path']).relative_to(self.source))
                    for item in report['compile_records']}==expected_units,'all three actual foreign Runtime translation units required')
        link=report['link'];require(set(link)=={'command_index','objects','explicit_inputs','response_files',
            'archive_statistics','llvm_archive_statistics'} and
            type(link['command_index']) is int and 0<=link['command_index']<len(commands) and link['objects']==objects,
            'exact compiled objects must feed current link')
        command=commands[link['command_index']];require(command['role']=='link' and report['binary']['path'] in command['argv'],
            'current fresh output absent from original link')
        for item in (*link['objects'],*link['explicit_inputs'],*link['response_files'],link['archive_statistics']):bind(item)
        closure=report['llvm_closure'];require(set(closure)=={'compiler','headers','libraries','archives','response_files','configuration'} and
            closure['headers'] and closure['configuration'] and (closure['libraries'] or closure['archives']), 'complete actual LLVM closure required')
        bind(closure['compiler'])
        for category in ('headers','libraries','archives','response_files','configuration'):
            for item in closure[category]:bind(item)
        # Native and foreign routes consume the same current source-bound
        # archive evidence helper. Preserve the real raw sidecar/trace; a zero
        # extraction is not a synthetic trace line or a missing provider.
        builder=source_module('test/0073.ros-tests/support/source/ros_debugger/build_runtime.py','debugger_archive_statistics_builder')
        archive_common=source_module('tools/ros_distro/common.py','debugger_archive_statistics_common')
        candidates=[item for item in report['input_pins'] if Path(item['path']).name=='build-evidence.json']
        require(len(candidates)==1,'one actual complete original build-evidence input required')
        original,evidence_pin=self.json(Path(candidates[0]['path']))
        require(evidence_pin==candidates[0] and original['kind']=='ros-debugger-runtime-build-evidence' and
                original['status']=='PASS' and original['source']==str(self.source) and
                original['source_manifest_sha256']==self.report['source_manifest_sha256'] and
                original['cleanup']['status']=='PASS' and not original['cleanup']['remaining'] and
                all(original.get(name) is True for name in ('source_preserved','inputs_preserved','products_preserved')),
                'actual same-source preserved original build evidence required for archive replay')
        native_config,original_config_pin=self.json(Path(original['config']['path']))
        require(original_config_pin==original['config'] and original_config_pin in report['input_pins'],
                'actual original build configuration association changed')
        normalized=json.loads(json.dumps(native_config))
        _,archive_roots=builder.configuration(normalized,self.source,self.execution_adapter)
        require([row['root'] for row in original['input_roots_after']]==[str(root)for root in archive_roots] and
                report['elf']=={key:normalized['target'][key]for key in ('machine','bits','endian')},
                'archive replay requires original exact provider/tool/stdlib roots and target')
        expected_stats=str(Path(report['binary']['path']).parent/'archive-statistics.tsv')
        require(link['archive_statistics']['path']==expected_stats and
                [value for value in command['argv'] if 'print-archive-stats' in value.lower()]==
                ['-Wl,--print-archive-stats='+expected_stats], 'exact one original owned linker statistics operand required')
        trace_raw,trace_pin=archive_common.body_pin(Path(command['stdout']['path']),32*1024**2)
        require(trace_pin==command['stdout']==command['stderr'],'actual original merged link trace changed')
        traced={}
        provider_paths={item['path']for item in closure['archives']}
        for line in trace_raw.decode(errors='strict').splitlines():
            if not line.startswith('/'):continue
            item=Path(line.split('(',1)[0]).resolve(strict=True)
            require(item.is_file() and builder.inside(item,[*archive_roots,Path(report['binary']['path']).parent]),
                    'archive replay trace escapes original physical roots')
            require(not item.name.lower().startswith(('libllvm','llvm')) or
                    (item.suffix=='.a' and str(item)in provider_paths),'extra/shared trace LLVM provider refused')
            traced[str(item)]=self.pin(item)
        require(len({item['path']for item in link['explicit_inputs']})==len(link['explicit_inputs']) and
                traced=={item['path']:item for item in link['explicit_inputs']},
                'actual canonical trace full-seven ledger differs from original explicit link inputs')
        required={item['path']for item in [*objects,*closure['libraries']] if Path(item['path']).suffix!='.a'}
        required|={name for name in normalized['library_inputs'] if Path(name).suffix!='.a'}
        require(required<=traced.keys() and len(link['response_files'])==1 and
                link['response_files']==closure['response_files'],'actual object/shared-library trace or ordered response missing')
        response_raw,response_pin=archive_common.body_pin(Path(link['response_files'][0]['path']))
        require(response_pin==link['response_files'][0],'actual ordered response changed')
        require([value for value in command['argv']if value.startswith('@')]==['@'+response_pin['path']],
                'original link must consume exactly the one bound ordered response file')
        offered=[value for value in builder.shlex.split(response_raw.decode())
                 if not value.startswith('-') and Path(value).suffix=='.a']
        offered += [name for name in normalized['library_inputs']if Path(name).suffix=='.a']
        archive_proof=builder.llvm_archive_statistics(archive_common,Path(expected_stats),archives=closure['archives'],
            member_counts=original['llvm_package']['native_archive_members'],trace_paths=traced,
            expected_archive_operands=offered,roots=archive_roots,target=normalized['target'],guard=self.check)
        require(archive_proof==link['llvm_archive_statistics'] and archive_proof['statistics']==link['archive_statistics'],
                'actual archive statistics/trace/full-seven/native-member receipt differs')
        for entry in archive_proof['archives']:bind(entry['archive'])
        config,config_pin=self.json(self.args.worker_config)
        require(config['qemu_argv'][0]==str(self.args.qemu.resolve()),'native SDK QEMU configuration differs')
        self.config=config;self.worker=self.out/'llvm-runtime'
        with Path(self.args.worker).open('rb') as source,self.worker.open('xb') as target:shutil.copyfileobj(source,target,1024*1024)
        self.worker.chmod(0o500);copy=self.pin(self.worker)
        require(copy['sha256']==report['binary']['sha256'] and copy['bytes']==report['binary']['bytes'] and
                (copy['device'],copy['inode'])!=(report['binary']['device'],report['binary']['inode']),'separate immutable Runtime copy required')
        self.report['runtime_build']={'report':pin,'config':config_pin,'origin':report['binary'],'snapshot':copy,
            'archive_statistics':archive_proof,
            'scope':('Fresh actual native ELF62 LLVM-full; separate native SDK/Gateway ARM/QEMU proofs; no interpreter fallback'
                     if not foreign else 'Fresh foreign LLVM-full; host QEMU executable and target mapped image separately required; no interpreter evidence')}
        if foreign:
            self.execution_adapter.check_image(self.worker,self.runtime_target,self.runtime_execution,self.pin)
            self.report['runtime_build'].update(build_configuration=report['build_configuration'],execution=self.runtime_execution)

    @staticmethod
    def same_root(left,right):
        return {key:value for key,value in left.items() if key!='catalog'}==\
               {key:value for key,value in right.items() if key!='catalog'}

    def catalog_runtime_roots(self,stage):
        rows=[]
        for index,root in enumerate(self.runtime_roots):
            if stage=='before':self.check()
            row=self.foreign_catalog_common.catalog(root,self.out/('foreign-roots-'+stage+'-%03d.jsonl'%index),self.runtime_roots)
            require(self.pin(row['catalog']['path'])==row['catalog'],'new complete foreign catalog changed')
            rows.append(row)
        return rows

    def close(self,status):
        if getattr(self,'runtime_execution',None) is None:return super().close(status)
        # Inherited Lane.close first retires all owned processes, then calls
        # evidence.finish before any terminal PASS. Only this instance's finish
        # call is wrapped; no generic lane/ARM sampler or native-v1 is changed.
        original_finish=self.evidence.finish
        def finish_with_foreign_closure(current,extra=None):
            try:
                require(self.execution_adapter.qualify(self.runtime_build_configuration,self.runtime_roots,self.pin)==
                        self.runtime_execution,'foreign execution inputs changed before terminal evidence')
                self.execution_adapter.check_image(self.worker,self.runtime_target,self.runtime_execution,self.pin)
                before=self.foreign_roots_before
                require(len(before)==len(self.runtime_roots),'complete foreign initial catalog baseline missing')
                after=self.catalog_runtime_roots('after')
                require(all(self.same_root(left,right) for left,right in zip(before,after)),
                        'whole foreign Runtime provider/sysroot roots changed during integration')
                self.report.update(foreign_runtime_roots_after=after,foreign_runtime_roots_preserved=True)
                # New terminal catalogs are included after the inherited pin
                # check; explicitly recheck the complete extended pin list too.
                from gateway_qualification import pins_preserved
                require(pins_preserved(self.pins),'foreign/current selected pin changed during terminal inventory')
            except BaseException as error:
                self.report['foreign_runtime_selection_finalization_error']=repr(error);current='FAIL'
            return original_finish(current,extra)
        self.evidence.finish=finish_with_foreign_closure
        try:return super().close(status)
        finally:self.evidence.finish=original_finish

    def runtime_argv(self,argv):
        if self.runtime_execution is None:return argv
        require(argv[0]==str(self.worker),'exact snapshotted foreign Runtime principal required')
        return [*self.runtime_execution['argument_prefix'],*argv]

    def stopped_runtime(self,child,row,seconds=20):
        """Bind host QEMU and guest image separately at the same owned kernel stop.

        Guest getpid/SCM_CREDENTIALS translation must keep this actual host PID.
        A QEMU helper child or path-only/maps-text assertion cannot satisfy it.
        Missing map_files access, loader mapping or syscall support fails closed.
        """
        if self.runtime_execution is None:return self.stopped_host(child,row,self.worker,seconds)
        execution=self.runtime_execution;adapter=self.execution_adapter
        image=adapter.check_image(self.worker,self.runtime_target,execution,self.pin)
        principal=execution['qemu'];wanted=dict(image['input'],elf_class=1 if image['bits']==32 else 2,
            elf_data=1 if image['endian']=='little' else 2,e_machine=image['machine'])
        targets={'runtime':wanted}
        if image['interpreter'] is not None:
            targets['loader']=dict(execution['loader'],elf_class=wanted['elf_class'],elf_data=wanted['elf_data'],e_machine=wanted['e_machine'])
        until=time.monotonic()+seconds
        while child.poll() is None and time.monotonic()<until:
            self.check();current=process_identity(child.pid)
            if current['state'] not in ('T','t'):time.sleep(.002);continue
            owner=next(item for item in self.owned.rows.values() if item['identity']['pid']==child.pid)
            task_root=Path('/proc')/str(child.pid)/'task'
            task_ids=sorted(int(path.name) for path in task_root.iterdir())
            require(0<len(task_ids)<=4096,'bounded foreign Runtime thread set required')
            threads={tid:process_identity(tid) for tid in task_ids}
            if not all(thread['state'] in ('T','t') for thread in threads.values()):time.sleep(.002);continue
            def guard():
                self.check();now=process_identity(child.pid)
                require(self.owned.check(owner) and now['starttime_ticks']==row['birth']['starttime_ticks'] and
                        now['state'] in ('T','t'),'foreign owned birth/cgroup/kernel stop changed')
                require(sorted(int(path.name) for path in task_root.iterdir())==task_ids,'stopped foreign thread set changed')
                for tid,previous in threads.items():
                    thread=process_identity(tid)
                    require(thread['starttime_ticks']==previous['starttime_ticks'] and thread['state'] in ('T','t'),
                            'foreign Runtime thread birth/kernel stop changed')
                host_path=Path('/proc')/str(child.pid)/'exe'
                host=mapping_file_facts(host_path.stat())
                require(host=={key:principal[key] for key in _MAP_FACTS},
                        '/proc/PID/exe must be the exact host QEMU, never the guest image')
                membership=(Path('/proc')/str(child.pid)/'cgroup').read_text().splitlines()
                require(membership==self.evidence.before['membership'] and str(child.pid) in
                        (self.evidence.leaf/'cgroup.procs').read_text().split(),'foreign process left current original cgroup')
                return dict(now,membership=membership,host_identity=host)
            before=guard()
            require(self.pin((Path('/proc')/str(child.pid)/'exe').resolve(strict=True))==principal,'current QEMU bytes changed')
            mapped=opened_target_mappings(child.pid,targets,guard)
            require(all(mapped['targets'][name] for name in targets),'actual guest Runtime/loader descriptor mapping absent')
            guard()
            require(self.pin((Path('/proc')/str(child.pid)/'exe').resolve(strict=True))==principal,'QEMU bytes changed during mapped proof')
            proof={'pid':child.pid,'starttime_ticks':before['starttime_ticks'],'membership':before['membership'],
                'kernel_host_QEMU':principal,'foreign_runtime_image':image,'opened_target_mappings':mapped,
                'actual_kernel_stop':True,'stopped_threads':list(threads.values()),'host_executable_is_target':False,'interpreter_execution_claimed':False}
            self.report.setdefault('foreign_runtime_roles',[]).append({'role':row['name'],'proof':proof})
            self.save();guard();self.owned.send(owner,signal.SIGCONT);return proof
        raise RuntimeError('actual stopped foreign Runtime/QEMU mapping unavailable')


class Control:
    def __init__(self,lane,endpoint,worker,row):
        self.lane=lane;self.endpoint=endpoint;self.worker=worker;self.row=row;self.actions=[]
    def ask(self,text,seconds=30):
        require(text.isascii() and 0<len(text)<=512,'bounded actual console command required')
        self.lane.check();raw=text.encode();require(self.endpoint.send(raw,socket.MSG_NOSIGNAL)==len(raw),'short console packet')
        end=time.monotonic()+seconds
        while time.monotonic()<end:
            self.lane.check();require(self.worker.poll() is None,'actual Runtime retired before console reply')
            try:data,ancillary,flags,_=self.endpoint.recvmsg(65537,socket.CMSG_SPACE(12)+socket.CMSG_SPACE(64),socket.MSG_CMSG_CLOEXEC)
            except socket.timeout:continue
            credentials=[];unexpected=False
            for level,kind,body in ancillary:
                if level==socket.SOL_SOCKET and kind==socket.SCM_CREDENTIALS and len(body)==12:credentials.append(struct.unpack('3i',body))
                else:
                    unexpected=True
                    if level==socket.SOL_SOCKET and kind==socket.SCM_RIGHTS:
                        descriptors=array.array('i');descriptors.frombytes(body[:len(body)//descriptors.itemsize*descriptors.itemsize])
                        for fd in descriptors:os.close(fd)
            require(not unexpected and not(flags&(socket.MSG_TRUNC|socket.MSG_CTRUNC)) and 0<len(data)<=65536 and
                    credentials==[(self.worker.pid,os.geteuid(),os.getegid())],'Runtime console reply credentials/size differ')
            reply=data.decode('utf-8','strict');self.actions.append({'command':text,'reply':reply})
            self.lane.report['console_actions']=self.actions;self.lane.save();return reply
        raise TimeoutError('actual authenticated console reply deadline')
    def pause(self):
        reply=self.ask('status')
        if 'stopped:' not in reply:reply=self.ask('pause')
        end=time.monotonic()+20
        while 'stopped:' not in reply and time.monotonic()<end:reply=self.ask('status');time.sleep(.005)
        stop=re.search(r'^stop-id ([1-9][0-9]*)$',reply,re.M)
        thread=re.search(r'^thread ([1-9][0-9]*) module=([0-9]+) function=',reply,re.M)
        require(stop and thread and 'native instruction' not in reply,'actual complete cooperative stop/thread required')
        return int(stop[1]),thread[1],thread[2]
    def advance(self,value):
        stop,thread,module=self.pause()
        reply=self.ask(f'set wasm memory {module} 0 {thread} 2048 bytes {value:02x}')
        require('applied=1' in reply and 'error:' not in reply,'actual application gate edit refused')
        require('running' in self.ask('continue'),'actual Runtime resume refused');return stop


def selected_policy(version,mode):
    connection,caps=policy(version,mode=='default-deny')
    if mode=='retained':
        caps.append({'alias':'events','operation':'event.observe','resource':'/uwvm_sdk_relay/sub','type':'builtin/event-v1',
            'identity':'builtin:ros-event-envelope-le64-v1','limits':{'event-source-alias':'sub','event-kinds':'resource-closed',
            'max-event-bytes':'128','max-event-queue-depth':'4','max-event-queue-bytes':'512','max-inflight':'1',
            'max-rate-hz':'100','burst':'4','max-bytes-per-second':'65536','overflow-policy':'reject',
            'audit':'allow-and-deny','audit-max-rate-hz':'100','audit-burst':'16','audit-max-records':'10000','audit-max-bytes':'1048576'}})
    return connection,caps


def observations(console,versions,mode,gateways):
    stop,_,_=console.pause();rows={}
    # These are historical window pins, not immutable input pins: later Guest
    # cleanup legitimately appends to the same final raw audit files. Final
    # Lane command log pins retain the complete bytes, and reviewers can hash
    # the recorded prefix to replay this no-new-native-RPC observation window.
    native_before=[{key:read_file_binding(row[key]) for key in ('stdout','stderr')} for _,row in gateways]
    for selector in SELECTORS:
        command=f'info ros {selector} module=main count=8 if-stop={stop}'
        first=console.ask(command);second=console.ask(command)
        require(first==second and 'status=available\n' in first,'same-stop ROS snapshot changed/unavailable')
        rows[selector]=first
    command=f'trace ros module=main count=16 if-stop={stop}'
    first=console.ask(command);require(first==console.ask(command) and 'status=available\n' in first and
        re.search(r'generation=[1-9][0-9]* module="main"',first),'real Runtime Host identity or stable trace missing')
    rows['trace']=first
    # Execute the real production DAP query method against the same actual
    # credential-checked private control transport. This proves projection and
    # stop fencing; it does not claim the separate IDE/broker transport ran.
    dap=console.lane.dap_adapter.Adapter(io.BytesIO())
    dap.broker=types.SimpleNamespace(request=console.ask)
    projected={}
    for selector in (*SELECTORS,'instances','events','trace'):
        query={'selection':selector,'module':'main','count':16 if selector=='trace'else 8,'ifStop':stop}
        value=dap.query_ros_state(query)
        require(value==dap.query_ros_state(query) and value['stopId']==stop and value['readOnly']is True and
            value['payloadsRead']is False and value['nativeOperationsIssued']is False,
            'actual DAP ROS projection changed or crossed its current stop')
        projected[selector]=value
    require(projected['instances']['rows']==projected['sessions']['rows']and
        projected['events']['rows']==projected['queues']['rows'],'DAP aliases changed actual instance/event queue meaning')
    console.dap=dap
    if mode=='retained':
        queues=[line for line in rows['queues'].splitlines() if line.startswith('row kind=')]
        require(len(queues)==len(versions) and all(' alias="events" ' in line and ' retired=1 ' in line and
            ' depth=1 bytes=64 pending=0 ' in line and ' dequeued=0 ' in line and ' delivered=0 ' in line for line in queues),
            'real retained local resource-closed queue/counters absent')
    else:
        require('row kind=' not in rows['capabilities'] and 'row kind=' not in rows['queues'],'default deny invented grants or queue')
    for selector in SELECTORS:
        require(console.ask(f'info ros {selector} module=main count=8 if-stop={stop}')==rows[selector],
                'another selector changed retained queue, stored quota or audit counters')
    require(console.ask(command)==rows['trace'],'ROS queries appended/drained actual Host trace')
    native_after=[{key:read_file_binding(row[key]) for key in ('stdout','stderr')} for _,row in gateways]
    require(native_before==native_after,'native Gateway audit/log changed during read-only query window')
    console.lane.report['native_gateway_query_window']={'before':native_before,'after':native_after,
        'unchanged':True,'scope':'Exact actual owned Gateway raw streams unchanged during stopped ROS queries; final full streams retained separately'}
    console.lane.report['same_stop_observation']={'stop':stop,'rows':rows,'repeated_identical':True,
        'scope':'Actual frozen Runtime process-owned metadata; no query-issued native operation; payload effects checked by independent peer'}
    console.lane.report['dap_same_stop_observation']={'stop':stop,'selections':projected,'repeated_identical':True,
        'scope':'Actual production DAP projection over inherited credential-checked console; IDE/broker transport not claimed'}
    return stop


def main():
    parser=argparse.ArgumentParser(description=__doc__);add_arguments(parser,workers=False)
    for name in ('worker','runtime-build-report','worker-config','ros1-gateway','ros1-gateway-report','ros2-gateway','ros2-gateway-report',
                 'prepared-native','wat2wasm'):parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--runtime-build-config',type=Path,help='Required only for explicit v2 foreign Runtime/QEMU selection; native SDK config remains separate.')
    parser.add_argument('--version',choices=('1','2','dual'),required=True)
    parser.add_argument('--mode',choices=('default-deny','retained'),required=True)
    parser.add_argument('--native-step',choices=('required','not-applicable'),required=True)
    parser.add_argument('--native-step-reason',default='')
    args=parser.parse_args();versions=(1,2) if args.version=='dual' else (int(args.version),)
    require(args.native_step!='not-applicable' or args.native_step_reason.strip(),'explicit selected-target native-step reason required')
    lane=None;endpoint=None;vm_endpoint=None;status='FAIL'
    try:
        lane=IntegrationLane(args,'real-runtime-ros-debugger');lane.qualify_runtime(versions);lane.qualify_dap_adapter()
        for v in versions:lane.qualify_gateway(v)
        wasm=translate(lane,args.wat2wasm,'debug-integration',guest(versions,args.mode))
        if 1 in versions:start_master(lane)
        if 2 in versions:lane.lock('domain-205')
        peers=[]
        for v in versions:
            source=qualify_peer(lane,args.prepared_native,v);lane.snapshot(source,lane.out/f'ros{v}/tmp/debug-peer',f'debug-peer-{v}')
        endpoint,vm_endpoint=socket.socketpair(socket.AF_UNIX,socket.SOCK_SEQPACKET)
        endpoint.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1);vm_endpoint.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1)
        endpoint.settimeout(.1)
        selections={v:selected_policy(v,args.mode) for v in versions}
        argv=worker_argv(lane,versions,wasm,absent=args.mode=='default-deny',selections=selections)
        argv[1:1]=['--debug-jit-control-fd',str(vm_endpoint.fileno()),'-Rdbg','--runtime-aot','-Rct','0',
                   '-Rllvm-call-stack','instruction','-Rllvm-cache-path','disable']
        runtime_argv=lane.runtime_argv(argv)
        worker,row=lane.start('debug-runtime',['/bin/bash','-c','kill -STOP $$; exec "$@"','debug-runtime',*runtime_argv],
            pass_fds=(vm_endpoint.fileno(),),seconds=300,rss=8*1024**3)
        vm_endpoint.close();vm_endpoint=None
        until=time.monotonic()+5
        while process_state(worker.pid) not in ('T','t') and time.monotonic()<until:lane.check();time.sleep(.001)
        require(process_state(worker.pid) in ('T','t'),'PID-preserving Runtime shell did not stop')
        gateways=[]
        for v in versions:
            config=lane.short/f'ros{v}.conf';config.write_text(gateway_config(v,worker.pid,args.mode=='default-deny',selections[v]));config.chmod(0o600)
            child,grow=lane.start(f'debug-gateway-{v}',lane.native(v,['/tmp/gateway','/guest/'+config.name]),lane.environment(v,gateway=True),seconds=300)
            lane.stopped(child,grow,f'gateway-{v}');path=lane.short/f'ros{v}.sock';until=time.monotonic()+30
            while not path.exists() and time.monotonic()<until:lane.check();require(child.poll() is None,'Gateway retired before readiness');time.sleep(.01)
            require(path.exists() and stat.S_ISSOCK(path.lstat().st_mode),'owned Gateway socket absent');gateways.append((child,grow))
            lane.report.setdefault('gateway_configs',{})[str(v)]={'pin':lane.pin(config),'text':config.read_text()}
        owner=next(item for item in lane.owned.rows.values() if item['identity']['pid']==worker.pid)
        lane.owned.send(owner,signal.SIGCONT);console=Control(lane,endpoint,worker,row)
        require('prepared; no Wasm instruction executed' in console.ask('status',120),'actual LLVM-full prepared controller absent')
        lane.owned.send(owner,signal.SIGSTOP);lane.stopped_runtime(worker,row)
        # Compile/adopt the real LLVM Runtime before starting the peer's fixed
        # 60-second ROS protocol. Its finite pre-init I gate is not a license to
        # spend that deadline on unrelated JIT or qualification work.
        for v in versions:
            child,prow=lane.start(f'debug-peer-{v}',lane.native(v,['/tmp/debug-peer','positive' if args.mode=='retained' else 'absent'],witness=True),
                lane.environment(v),seconds=120)
            proof=lane.stopped(child,prow,f'debug-peer-{v}',cooperative=True);lane.send(child,prow,b'I','actual stopped independent native peer')
            lane.wait_marker(child,prow,'READY_SDK_PEER');peers.append((v,child,prow,proof['actual_stopped_ARM']['pid']))
        original_check=lane.check
        def check_live_gateways():
            original_check()
            require(all(child.poll() is None or child.poll()==0 and worker.poll()==0 for child,_ in gateways),
                    'Gateway retired before actual Runtime principal completion')
        lane.check=check_live_gateways
        require('running' in console.ask('continue'),'actual Guest did not start')
        marker='DEBUG_ENDPOINTS' if args.mode=='retained' else 'DEBUG_DEFAULT_DENIED'
        lane.wait_marker(worker,row,marker)
        require(lane.read(row).count(MANAGEMENT_TEXT)==1,'Guest console-looking stdout text absent, duplicated or altered')
        require(console.ask('status').startswith('running\n'),'Guest stdout changed the actual authenticated controller state')
        lane.report['guest_stdio_management_isolation']={
            'literal_stdout':MANAGEMENT_TEXT,'occurrences':1,'controller_after_stdout':'running',
            'control_transport':'Actual inherited credential-checked SOCK_SEQPACKET, separate from Guest fd1',
            'management_text_sent_to_control':False,
            'scope':'Actual selected Runtime/Guest execution only; no claim for another IDE transport'}
        if args.mode=='retained':
            for _,child,prow,_ in peers:lane.wait_marker(child,prow,'MATCHED_SDK_PEER')
            console.advance(1)
            for _,child,prow,_ in peers:lane.wait_marker(child,prow,'DELIVERED_SDK_PEER')
            for attempt in range(2,150):
                before=lane.read(row).count('DEBUG_POLL_EMPTY\n');console.advance(attempt);until=time.monotonic()+10
                while time.monotonic()<until:
                    lane.check();text=lane.read(row)
                    if 'DEBUG_RETAINED\n' in text or text.count('DEBUG_POLL_EMPTY\n')>before:break
                    require(worker.poll() is None,'actual Guest retired during bounded receive');time.sleep(.005)
                else:raise TimeoutError('Guest bounded receive checkpoint missing')
                if 'DEBUG_RETAINED\n' in lane.read(row):break
            lane.wait_marker(worker,row,'DEBUG_RETAINED')
        old=observations(console,versions,args.mode,gateways)
        require('running' in console.ask('continue'),'resume after read-only observation failed')
        require('not valid' in console.ask('info ros stats'),'running controller accepted ROS snapshot')
        try:console.dap.query_ros_state({'selection':'stats'})
        except ValueError:pass
        else:raise ValueError('actual DAP metadata query accepted a running Runtime')
        current,_,_=console.pause();require(current!=old,'new actual pause reused old stop identifier')
        refused=console.ask(f'info ros stats if-stop={old}')
        require('stale stop' in refused,'old actual cooperative stop was accepted')
        try:console.dap.query_ros_state({'selection':'stats','ifStop':old})
        except ValueError:pass
        else:raise ValueError('actual DAP query accepted the retired original stop')
        if args.native_step=='required':
            state=console.ask('status');thread=re.search(r'^thread ([1-9][0-9]*) module=',state,re.M);require(thread,'actual stopped thread absent')
            stepped=console.ask('step asm '+thread[1]);require('stopped: native instruction step' in stepped,'selected native step fixture failed')
            refusal=console.ask('info ros stats');require('requires current cooperative stop' in refusal,'native instruction stop admitted ROS callback')
            try:console.dap.query_ros_state({'selection':'stats'})
            except ValueError:pass
            else:raise ValueError('actual DAP query accepted a native instruction stop')
            require('stopped: selected participant step' in console.ask('step wasm '+thread[1]),'actual Wasm cooperative stop not restored')
            lane.report['native_instruction_stop']={'status':'PASS','scope':'Actual selected LLVM-full target native step and production ROS refusal'}
        else:lane.report['native_instruction_stop']={'status':'NOT_APPLICABLE_SELECTED_BACKEND','reason':args.native_step_reason,
            'acceptance_claimed':False,'runnable_fixture':'same entry --native-step required on a supported real target; test/0017.runtime/run_native_step_debug_cli.py'}
        console.advance(200);lane.wait_marker(worker,row,'PASS_ROS_DEBUG_GUEST');lane.finish(worker,row)
        for v,child,prow,pid in peers:
            lane.send(child,prow,b'Q','actual Guest sentinel/payload/retained-record checks and exit0');lane.finish(child,prow)
            parsed=native_markers(lane.read(prow),v,'positive' if args.mode=='retained' else 'absent',pid)
            lane.report.setdefault('independent_native_effects',{})[str(v)]=parsed
        require(all(child.poll() in (None,0) for child,_ in gateways),'Gateway failed after actual Runtime completion')
        lane.report.update(version=args.version,fixture_mode=args.mode,worker_argv=argv,runtime_launch_argv=runtime_argv,
            runtime_backend_verified='llvm-full via source-bound no-interpreter build and actual prepare_debug_control_fd_mode/console',
            interpreter_matrix_evidence_reused=False,new_tests_executed=True,
            actual_runtime_identity=True,authenticated_control='Inherited private SOCK_SEQPACKET; actual kernel launcher/Runtime credentials and pidfds',
            read_only_observation_windows=True,retained_queue_consumed_once_after_queries=args.mode=='retained',
            original_core_sdk_and_regressions_replaced=False)
        status='PASS'
    except BaseException as error:
        if lane is None:raise
        lane.report.update(error=repr(error),traceback=traceback.format_exc())
    finally:
        if endpoint is not None:endpoint.close()
        if vm_endpoint is not None:vm_endpoint.close()
        if lane is not None:status=lane.close(status)
    return 0 if status=='PASS' else 1

if __name__=='__main__':raise SystemExit(main())
