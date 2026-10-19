"""Actual connected Runtime/DAP replacement witness, not an injected DATA suite.

Root's already admitted integration owner supplies a real Control instance from
integration_runtime.py, whose ask() verifies the held process and per-packet
kernel credentials on its inherited private SOCK_SEQPACKET channel. The same
owner must create/start the genuine LLVM-only Worker with the matching new WAT
and finite explicit main-module policy, and perform normal cleanup/Frame.finish.
This callable neither starts a process nor fabricates a broker response. Every
request below reaches that actual production controller through Control.ask.
"""
import hashlib
import io
import json
import os
from pathlib import Path
import re
import stat
import time
from types import SimpleNamespace


def require(value,reason):
    if not value:raise ValueError(reason)


def exercise(frame,load,console,*,backend,module,invalid_body,value9_body):
    """Run two real stops with direct/scripted updates and stale DAP requests.

    load(relative, unique_name) must execute only actual current complete-MF
    bytes. The caller owns the process/birth/pidfd/physical Runtime mapping,
    genuine LLVM build receipt, permissions and absolute whole deadline. This
    function additionally enforces a 180-second active budget and bounds each
    real console request through its existing 30-second default. Return only
    after the real _start completes successfully; any failure leaves cleanup
    to the enclosing original owner and is never converted into a PASS row.
    """
    require(type(backend) is int and backend in (1,2) and type(module) is int and 0<=module<64,
            'explicit matching actual ROS backend/module required')
    started=time.monotonic();requests=[];commands=[];scripts=[]
    def guard():
        frame.guard();console.lane.check()
        require(time.monotonic()-started<180,'actual DAP management active deadline')
    guard()
    require(Path(console.lane.source)==Path(frame.source) and
            type(frame.report.get('source_manifest_sha256')) is str and
            re.fullmatch('[0-9a-f]{64}',frame.report['source_manifest_sha256']) and
            console.lane.report.get('source_manifest_sha256')==frame.report['source_manifest_sha256'],
            'actual connected Runtime lane and Frame must select one identical complete current source manifest')
    require(console.worker.poll() is None,'actual owned Runtime must be live before DAP admission')
    dap=load('tools/debug/dap_adapter.py','actual_management_replacement_dap')
    def body(path,expected):
        path=Path(path);guard()
        require(path.is_absolute() and path.resolve(strict=True)==path and
                any(path.is_relative_to(Path(root)) for root in (frame.source,frame.out,*frame.roots)),
                'explicit physical selected replacement input required')
        before=frame.pin(path);require(0<before['bytes']<=65536,'bounded exact replacement body')
        fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC)
        try:
            first=os.fstat(fd);require(stat.S_ISREG(first.st_mode) and first.st_size==before['bytes'],'ordinary selected replacement body')
            raw=bytearray()
            while True:
                guard();part=os.read(fd,min(65536,65537-len(raw)))
                if not part:break
                raw.extend(part);require(len(raw)<=65536,'replacement body grew')
            last=os.fstat(fd);named=path.stat(follow_symlinks=False)
            require(all(getattr(first,k)==getattr(last,k)==getattr(named,k) for k in
                        ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink')) and
                    bytes(raw)==expected and hashlib.sha256(raw).hexdigest()==before['sha256'] and frame.pin(path)==before,
                    'actual replacement body bytes or identity changed')
        finally:os.close(fd)
        return before
    invalid=body(invalid_body,b'\x00\xff\x0b');valid=body(value9_body,b'\x00\x41\x09\x0b')
    def ask(text):
        guard();require(len(text)<=512 and text.isascii() and '\0' not in text and '\n' not in text,'bounded actual console command')
        reply=console.ask(text);commands.append({'command':text,'reply':reply});guard();return reply

    class Output(io.BytesIO):
        def write(self,data):
            require(self.tell()+len(data)<=131072,'bounded real DAP publication')
            return super().write(data)
    output=Output();adapter=dap.Adapter(output);adapter.step_level='wasm';adapter.module_id=module
    # This is the same actual transport binding used by integration_runtime's
    # production DAP projection. Replies are never supplied as fixture DATA.
    adapter.broker=SimpleNamespace(request=ask)
    sequence=0
    def request(command,arguments):
        nonlocal sequence
        guard();sequence+=1;output.seek(0);output.truncate()
        adapter.handle({'seq':sequence,'type':'request','command':command,'arguments':arguments})
        raw=output.getvalue();packets=[]
        while raw:
            header,separator,rest=raw.partition(b'\r\n\r\n')
            match=re.fullmatch(rb'Content-Length: ([0-9]{1,6})',header)
            require(separator and match and 0<int(match[1])<=65536,'actual bounded DAP framing')
            size=int(match[1]);require(len(rest)>=size,'complete actual DAP response')
            packets.append(json.loads(rest[:size]));raw=rest[size:]
            require(len(packets)<=8,'bounded actual DAP events/responses')
        replies=[row for row in packets if row.get('type')=='response' and row.get('request_seq')==sequence]
        require(len(replies)==1 and replies[0].get('command')==command,'exact actual DAP request correlation')
        requests.append({'command':command,'arguments':arguments,'packets':packets});guard();return replies[0]
    def refs():
        adapter.observe(adapter.command('status'),notify=False)
        require(adapter.state=='stopped' and len(adapter.threads)==1 and not adapter.threads[0].get('native_pc'),
                'actual single cooperative participant required')
        thread=adapter.threads[0]['id'];stack=request('stackTrace',{'threadId':thread,'startFrame':0,'levels':4})
        require(stack['success'] is True and stack['body']['stackFrames'],'actual complete Wasm frame page required')
        frame_id=stack['body']['stackFrames'][0]['id'];scopes=request('scopes',{'frameId':frame_id})
        require(scopes['success'] is True,'actual frame scopes required')
        selected=[row['variablesReference'] for row in scopes['body']['scopes'] if row['name'].startswith('ROS capabilities')]
        require(len(selected)==1 and selected[0] in adapter.ros_scope_stops,'actual production-minted ROS scope reference required')
        return frame_id,selected[0],adapter.stop_key,adapter.threads[0]['stop_id']
    def stale(frame_id,reference,*,without_refresh=False):
        transport_before=len(commands)
        require(request('scopes',{'frameId':frame_id})['success'] is False and
                request('variables',{'variablesReference':reference,'start':0,'count':1})['success'] is False,
                'actual old DAP frame/scope references must be rejected')
        if without_refresh:
            require(len(commands)==transport_before,
                    'retired script references must refuse before any authoritative status refresh or other transport request')
    def invalidated_script_state():
        # Read the real Adapter.invalidate_stop contract. It intentionally keeps
        # last diagnostic state/threads, but none of those labels is authority
        # when stop_key and every actual reference cache have been retired.
        names=('frames','scopes','wasm_local_scope_stops','native_frames','native_register_scopes',
               'native_code_refs','source_frames','source_frame_ordinals','source_scope_stops',
               'source_object_views','source_scope_frames','source_pointer_views','wasm_scope_stops',
               'wasip1_scope_stops','ros_scope_stops','wasm_object_views','deep_wasm_paths','inline_frames')
        lengths={name:len(getattr(adapter,name)) for name in names}
        counters={name:getattr(adapter,name) for name in ('source_object_nodes','source_object_bytes','wasm_path_indices')}
        require(adapter.stop_key is None and all(size==0 for size in lengths.values()) and
                all(value==0 for value in counters.values()),
                'actual scripted command must retire stop key and all production reference caches before refresh')
        return {'stop_key':adapter.stop_key,'reference_cache_lengths':lengths,'reference_counters':counters}
    def canonical_status(function):
        # A batch can contain statuses from both sides of a real commit. Never
        # observe that transcript as one stop; request one complete fresh packet.
        reply=adapter.command('status');lines=reply.splitlines()
        state,_,threads,_=dap.parse_status(reply)
        require(lines and lines[0].startswith('stopped: ') and
                sum(line.startswith('stopped: ') for line in lines)==1 and
                state=='stopped' and len(threads)==1 and not threads[0].get('native_pc') and
                threads[0]['module']==module and threads[0]['function']==function and threads[0]['offset']==0 and
                all(type(threads[0].get(name)) is int and 0<threads[0][name]<(1<<64)
                    for name in ('id','generation','stop_id')),
                'one complete actual cooperative status packet required after script')
        adapter.observe(reply,notify=False)
        require(adapter.state=='stopped' and adapter.stop_key is not None,
                'real status must establish the new canonical adapter stop')
        return {'reply':reply,'stop_key':adapter.stop_key,'stop_id':threads[0]['stop_id']}
    def complete_script(reply,count):
        require(reply.startswith('wasm-script command 1\n') and reply.endswith('wasm-script end\n') and
                re.findall(r'^wasm-script command ([1-9][0-9]*)$',reply,re.MULTILINE)==[str(i) for i in range(1,count+1)] and
                not re.search(r'^error:',reply,re.MULTILINE) and 'wasm-script output-truncated' not in reply,
                'actual complete successful script transcript must contain every bounded child without errors or truncation')
    def stopped(function):
        end=time.monotonic()+20
        while time.monotonic()<end:
            guard();adapter.observe(adapter.command('status'),notify=False)
            if adapter.state=='stopped' and len(adapter.threads)==1:
                current=adapter.threads[0]
                if current['module']==module and current['function']==function and current['offset']==0:return current
            time.sleep(.005)
        raise TimeoutError('actual expected emitted management breakpoint did not occur')

    # The caller's worker is genuinely prepared/initially paused. Configure
    # only these two ordinary instruction breakpoints before its _start runs.
    for function in (5,6):
        require('breakpoint' in adapter.command(f'breakwasm {module} {function} 0'),'actual emitted breakpoint refused')
    require('running' in adapter.command('continue'),'initial actual launch resume refused')
    stopped(5);first_frame,first_scope,_,first_stop=refs()
    before=adapter.query_ros_state({'selection':'capabilities','backend':'ros'+str(backend),'module':'management-main','count':8})
    require(before['rows'] and any(row.get('alive') is True and row.get('active_handles',0)>=2 for row in before['rows']),
            'genuine Guest opened current business capabilities before replacement')
    try:adapter.command(f"replace {module} 2 1 {invalid['path']}")
    except RuntimeError:pass
    else:raise ValueError('actual malformed replacement unexpectedly acknowledged')
    stale(first_frame,first_scope)
    after=adapter.query_ros_state({'selection':'capabilities','backend':'ros'+str(backend),'module':'management-main','count':8})
    require(after['stopId']==first_stop and after['rows']==before['rows'],'actual rejected preparation changed authority or canonical stop')

    # The production parser qualifies the COMPLETE batch before its first
    # child. A forbidden resume after a read-only status must reject the whole
    # script, while the Adapter still retires every old reference before send.
    script_frame,script_scope,script_key,script_stop=refs()
    require(script_stop==first_stop and script_frame!=first_frame and script_scope!=first_scope,
            'fresh actual first-stop references required for rejected script')
    command_first=len(commands);request_first=len(requests)
    try:adapter.command('wasm-script status; continue')
    except RuntimeError as error:
        script_error=str(error)
        require(script_error=='error: control request rejected (code 11)',
                'whole invalid script must receive actual malformed rejection, not an arbitrary failure')
    else:raise ValueError('actual script containing forbidden resume unexpectedly acknowledged')
    require(len(commands)==command_first+1 and commands[-1]['command']=='wasm-script status; continue',
            'script rejection must not trigger an automatic authoritative status refresh')
    invalidated=invalidated_script_state();stale(script_frame,script_scope,without_refresh=True)
    invalid_refresh=canonical_status(5)
    invalid_rows=adapter.query_ros_state({'selection':'capabilities','backend':'ros'+str(backend),'module':'management-main','count':8})
    require(invalid_refresh['stop_key']==script_key and invalid_refresh['stop_id']==script_stop and
            invalid_rows['stopId']==first_stop and invalid_rows['rows']==before['rows'],
            'whole rejected script changed canonical stop, business authority or existing handles')
    scripts.append({'kind':'rejected_complete_script','command_indices':[command_first,len(commands)],
        'dap_request_indices':[request_first,len(requests)],'error':script_error,'invalidated_before_refresh':invalidated,
        'authoritative_status':invalid_refresh,'capabilities':invalid_rows})

    # Even a genuine diagnostic-only batch invalidates DAP references before
    # send. Its embedded status text is not itself a new canonical stop.
    readonly_frame,readonly_scope,readonly_key,readonly_stop=refs()
    command_first=len(commands);request_first=len(requests)
    readonly_reply=adapter.command('wasm-script status');complete_script(readonly_reply,1)
    require(len(commands)==command_first+1,'read-only batch must not infer a stop or auto-refresh from batch text')
    invalidated=invalidated_script_state();stale(readonly_frame,readonly_scope,without_refresh=True)
    readonly_refresh=canonical_status(5)
    readonly_rows=adapter.query_ros_state({'selection':'capabilities','backend':'ros'+str(backend),'module':'management-main','count':8})
    require(readonly_refresh['stop_key']==readonly_key and readonly_refresh['stop_id']==readonly_stop==first_stop and
            readonly_rows['stopId']==first_stop and readonly_rows['rows']==before['rows'],
            'read-only script text must not create a new real stop or change authority')
    scripts.append({'kind':'read_only_script','command_indices':[command_first,len(commands)],
        'dap_request_indices':[request_first,len(requests)],'reply':readonly_reply,'invalidated_before_refresh':invalidated,
        'authoritative_status':readonly_refresh,'capabilities':readonly_rows})
    require('running' in adapter.command('continue'),'resume after actual rejection refused')
    stopped(6);old_frame,old_scope,old_key,old_stop=refs()
    reply=adapter.command(f"replace {module} 2 1 {valid['path']}")
    require('function replaced; generation 2' in reply and adapter.state=='stopped' and adapter.stop_key!=old_key,
            'actual successful commit and canonical stop refresh required')
    stale(old_frame,old_scope)
    try:adapter.query_ros_state({'selection':'capabilities','module':'management-main'},expected_stop=old_key)
    except ValueError:pass
    else:raise ValueError('actual retired DAP stop key granted a metadata view')
    retired=adapter.query_ros_state({'selection':'capabilities','backend':'ros'+str(backend),'module':'management-main','count':8})
    sessions=adapter.query_ros_state({'selection':'sessions','backend':'ros'+str(backend),'module':'management-main','count':8})
    require(retired['stopId']!=old_stop and retired['rows'] and all(row.get('retired') is True and row.get('alive') is False and
            row.get('active_handles')==0 for row in retired['rows']) and sessions['rows'] and
            all(row.get('retired') is True and row.get('connected') is False for row in sessions['rows']),
            'actual controller acknowledged before whole-module retirement was observable')
    # Keep the direct generation1 -> 2 commit above. A second real commit via
    # the script grammar publishes the same complete ABI-compatible value9 body
    # as generation3 while the target remains inactive at this second stop.
    script_frame,script_scope,script_key,script_stop=refs()
    require(script_stop==retired['stopId'] and script_frame!=old_frame and script_scope!=old_scope,
            'fresh actual post-direct-commit references required for scripted replacement')
    command_first=len(commands);request_first=len(requests)
    script_reply=adapter.command(f"wasm-script status; replace {module} 2 2 {valid['path']}; status")
    complete_script(script_reply,3)
    require('\nfunction replaced; generation 3\n' in script_reply and len(commands)==command_first+1,
            'actual generation3 script commit required without automatic transcript-based stop refresh')
    invalidated=invalidated_script_state()
    # This rejection must occur BEFORE the next authoritative status command.
    # Otherwise a polling refresh could hide failure to retire cached IDs.
    stale(script_frame,script_scope,without_refresh=True)
    script_refresh=canonical_status(6)
    require(script_refresh['stop_key']!=script_key and script_refresh['stop_id']!=script_stop,
            'actual scripted commit must mint a fresh canonical stop rather than reuse generation2 labels')
    try:adapter.query_ros_state({'selection':'capabilities','module':'management-main'},expected_stop=script_key)
    except ValueError:pass
    else:raise ValueError('actual scripted replacement resurrected a retired ROS stop key')
    script_retired=adapter.query_ros_state({'selection':'capabilities','backend':'ros'+str(backend),'module':'management-main','count':8})
    script_sessions=adapter.query_ros_state({'selection':'sessions','backend':'ros'+str(backend),'module':'management-main','count':8})
    require(script_retired['stopId']==script_sessions['stopId']==script_refresh['stop_id'] and
            script_retired['rows'] and all(row.get('retired') is True and row.get('alive') is False and
                row.get('active_handles')==0 for row in script_retired['rows']) and script_sessions['rows'] and
            all(row.get('retired') is True and row.get('connected') is False for row in script_sessions['rows']),
            'scripted update must preserve whole-module retirement and disconnected sessions without regrant')
    scripts.append({'kind':'committed_replacement_script','command_indices':[command_first,len(commands)],
        'dap_request_indices':[request_first,len(requests)],'reply':script_reply,'generation':3,
        'from_stop':script_stop,'from_stop_key':script_key,'invalidated_before_refresh':invalidated,
        'authoritative_status':script_refresh,'capabilities':script_retired,'sessions':script_sessions})
    require('running' in adapter.command('continue'),'actual post-commit Guest resume refused')
    while console.worker.poll() is None:
        guard();time.sleep(.005)
    require(console.worker.returncode==0,'actual unchanged Guest functions/value9/denied handles failed after replacement')
    require(frame.pin(invalid['path'])==invalid and frame.pin(valid['path'])==valid,'actual input bodies changed during execution')
    return {'kind':'actual-connected-llvm-management-dap-replacement-v1',
        'status':'PASS_REAL_REJECTED_AND_COMMITTED_REPLACEMENT_DAP_REFERENCES_RETIRED',
        'worker_pid':console.worker.pid,'backend':backend,'module':module,'invalid_body':invalid,'value9_body':valid,
        'first_stop':first_stop,'committed_from_stop':old_stop,'committed_stop':retired['stopId'],
        'before':before,'rejected':after,'retired':retired,'sessions':sessions,
        'direct_committed_generation':2,'script_committed_generation':3,'script_committed_stop':script_retired['stopId'],
        'script_retired':script_retired,'script_sessions':script_sessions,'actual_wasm_scripts':scripts,'actual_console_commands':commands,
        'actual_dap_requests':requests,'Runtime_executed':True,'Guest_executed':True,
        'ROS_network_delivery_qualified':False,'active_reset_qualified':False,
        'scope':'Real owning Runtime/control channel, direct and scripted commits, and actual production Adapter reference retirement; no fake broker replies, no ROS network/robot/performance claim'}
