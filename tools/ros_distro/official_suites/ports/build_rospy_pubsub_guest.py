"""Compile the real one-module ROS1 official-case Guest; execute no ROS.

Root admits this module and ``load`` against a complete current source manifest
and constructs a standard current Unitree Frame first. Root calls build(), then
Frame.finish(STATUS); only the resulting preserved terminal report is a runtime
input. Source/SDK roots, cgroup, deadlines, output and supervisor are never
created or widened here. All tools are explicit input labels; no PATH discovery,
stub SDK, replacement libc, permission or native process is manufactured.
"""
from pathlib import Path

STATUS='PASS_PREPARED_OFFICIAL_ROSPY_PUBSUB_GUEST_VALIDATED_NOT_EXECUTED'
RELATIVE='tools/ros_distro/official_suites/ports/rospy_pubsub_order_guest.c'


def need(value,reason):
    if not value:raise ValueError(reason)


def recipes(source,out,tools,sysroot,alias):
    """Shared closed argv constructor, replayed by the runtime admission port."""
    source,out=Path(source),Path(out);obj=out/'official-pubsub.o';dep=out/'official-pubsub.d';wasm=out/'official-pubsub.wasm'
    compile_=[str(tools['cc']),'--target=wasm32-wasi','--sysroot='+str(sysroot),'-std=c11','-O2','-UNDEBUG',
        '-ffreestanding','-fno-builtin','-I'+str(source/'sdk/include'),'-MD','-MF',str(dep),'-MT',str(obj),'-c',str(source/RELATIVE),'-o',str(obj)]
    link=[str(tools['cc']),'--target=wasm32-wasi','--sysroot='+str(sysroot),'-fuse-ld='+alias,
        '-nostdlib','-nostartfiles','-Wl,--threads=1','-Wl,--entry=_start','-Wl,--export=_start',
        '-Wl,--export-memory','-Wl,--initial-memory=131072','-Wl,--max-memory=16777216',
        '-Wl,-z,stack-size=65536','-Wl,--trace',str(obj),str(tools['libc']),str(tools['builtins']),'-o',str(wasm)]
    return compile_,link,[str(tools['validator']),'validate',str(wasm)]


def boundary(raw,utilities,inspector):
    """Keep the real local entry and bounded memory; full validator is separate."""
    value=utilities.wasm_structure(raw,inspector)
    entry=value['exports'].get('_start');imports=sum(item['kind']==0 for item in value['imports'])
    need(entry and entry['kind']==0 and entry['params']==entry['results']==[] and entry['index']>=imports and
         value['start_function'] is None and len(value['memories'])==1 and
         value['memories'][0]=={'minimum':2,'maximum':256,'imported':False} and not value['imported_globals'],
         'standalone same-module local entry/bounded32-bit unshared memory required')
    return dict(value,local_entry=entry['index']-imports)


def build(frame,load,*,selection_input='official-pubsub-tools'):
    """Run exactly one full-MD compile, one real link and one full validator.

    Selection fields are cc,wasm_ld,libc,builtins,validator (actual Frame input
    labels), sysroot (an actual canonical whole-provider path), and wasm_ld_alias
    (the selected multicall invocation path, whose physical file equals input).
    The actual owner stays serial with aggregate RSS <=768MiB, <=60s/command.
    Root still owns its complete original64-GiB zero-swap parent supervisor.
    """
    frame.guard()
    utilities=load('tests/ros_unitree/command_build_support.py','official_pubsub_command_evidence')
    inspector=load('sdk/tools/check_wasm_imports.py','official_pubsub_build_import_reader')
    frozen=frame.common.document(frame.manifest)[0]
    need(frame.pin(frame.manifest)['sha256']==frame.report['source_manifest_sha256'],'current full manifest required')
    for relative in (RELATIVE,'tools/ros_distro/official_suites/ports/build_rospy_pubsub_guest.py'):
        utilities.source_pin(frame,frozen,frame.source/relative)
    selected,selected_pin=frame.common.document(frame.input(selection_input))
    need(frame.pin(frame.input(selection_input))==selected_pin and type(selected) is dict and set(selected)==
        {'cc','wasm_ld','libc','builtins','validator','sysroot','wasm_ld_alias'},'closed actual Guest tool selection required')
    tools={name:frame.input(selected[name]) for name in ('cc','wasm_ld','libc','builtins','validator')}
    root=Path(selected['sysroot'])
    need(root.resolve(strict=True)==root and root.is_dir() and any(root==value or root.is_relative_to(value) for value in frame.roots),
         'actual complete selected WASI sysroot required')
    alias=utilities.alias(selected['wasm_ld_alias'])
    need(Path(alias['physical'])==tools['wasm_ld'] and any(Path(alias['path']).is_relative_to(value) for value in frame.roots),
         'actual selected wasm-ld alias/provider association required')
    for name in tools:frame.pin(tools[name])
    source=frame.source/RELATIVE;obj=frame.out/'official-pubsub.o';dep=frame.out/'official-pubsub.d';wasm=frame.out/'official-pubsub.wasm'
    limits={'rss_bytes':min(frame.limits['rss_bytes'],768*1024**2),'command_seconds':min(frame.limits['command_seconds'],60),
            'log_bytes':min(frame.limits['log_bytes'],16*1024**2)}
    owner=frame.common.SerialRunner(frame.out,limits,frame.guard);commands=[]
    try:
        compile_,link,validate=recipes(frame.source,frame.out,tools,root,alias['path'])
        argv=compile_
        compiled=utilities.run(frame,owner,'official-pubsub-c-full-MD',argv,frame.out,frame.environment)
        dependencies=utilities.full_md(frame,dep,obj,source,frame.out)
        need(all(any(value['path']==str(frame.source/path) for value in dependencies) for path in
             ('sdk/include/roswasm/ros1.h','sdk/include/roswasm/node.h','sdk/include/roswasm/codec.h')),
             'actual SDK headers must occur in full compiler dependencies')
        commands.append({'role':'compile','command':compiled,'source':frame.pin(source),'object':frame.pin(obj),
                         'dependency_file':frame.pin(dep),'dependencies':dependencies})
        argv=link
        linked=utilities.run(frame,owner,'official-pubsub-link',argv,frame.out,frame.environment)
        links=utilities.trace_inputs(frame,linked,frame.out)
        need(all(frame.pin(path) in links for path in (obj,tools['libc'],tools['builtins'])),'real link trace omitted a selected object/archive')
        commands.append({'role':'link','command':linked,'inputs':links,'artifact':frame.pin(wasm)})
        checked=utilities.run(frame,owner,'official-pubsub-full-validate',validate,frame.out,frame.environment)
        commands.append({'role':'validate','command':checked,'artifact':frame.pin(wasm)})
        raw,actual=frame.common.body_pin(wasm,64*1024**2);need(frame.pin(wasm)==actual,'actual Wasm changed')
        inspected=boundary(raw,utilities,inspector)
        frame.report.update(kind='official-rospy-pubsub-guest-build-v1',source=str(frame.source),
            guest=frame.pin(wasm),selection=selected_pin,tool_pins={name:frame.pin(path) for name,path in tools.items()},
            linker_alias=alias,sysroot=str(root),binary_boundary=inspected,commands=commands,default_authority=[],Runtime_executed=False,
            native_ROS_executed=False,source_inputs=[frame.pin(source)],full_dependencies_verified=True)
    finally:
        cleanup=owner.close();frame.report.setdefault('cleanup',[]).append(cleanup)
        need(cleanup['status']=='PASS' and not cleanup.get('remaining'),'owned Guest compiler cleanup incomplete')
        need(utilities.alias(alias['path'])==alias,'selected linker alias changed')
        for row in owner.commands:
            if 'log' in row:frame.pin(row['log']['path'])
        frame.save()
    return frame.report
