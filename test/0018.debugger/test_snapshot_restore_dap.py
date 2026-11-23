#!/usr/bin/env python3
"""DAP admission and stale-reference tests; actual execution has a native fixture."""
import importlib.util
import io
import json
from pathlib import Path
import sys
import unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/debug"))
spec = importlib.util.spec_from_file_location("restore_dap", ROOT / "tools/debug/dap_adapter.py")
dap = importlib.util.module_from_spec(spec); spec.loader.exec_module(dap)
PENDING = "snapshot-restore pending phase=7 detail=10 preparation=12 state-error=0 applied=0 retained=0; use dump restore continue; guest execution remains closed\n"
class Broker:
    def __init__(self, reply=PENDING): self.commands=[]; self.reply=reply
    def request(self, command): self.commands.append(command); return self.reply
class Restore(unittest.TestCase):
    def run_request(self, args, permission="full", reply=PENDING):
        output=io.BytesIO(); adapter=dap.Adapter(output); broker=Broker(reply)
        adapter.broker=broker; adapter.debug_permissions=permission
        adapter.handle({"seq":1,"command":"uwvm/restoreSnapshot","arguments":args})
        output.seek(0); messages=[]
        while line:=output.readline():
            count=int(line.split(b":")[1]); self.assertEqual(output.readline(),b"\r\n")
            messages.append(json.loads(output.read(count)))
        response=next(m for m in messages if m["type"]=="response")
        return response,broker,adapter
    def test_explicit_current_and_recorded(self):
        for mode, bindings, suffix in [("current",[],""),("recorded",[],"- "),
                ("recorded",[{"module":"2","resource":"0","descriptor":7}],"2:0:7 ")]:
            response,broker,adapter=self.run_request({"path":"/private/state with spaces.uwsnap","stopId":"7","environment":mode,"bindings":bindings})
            self.assertTrue(response["success"],response)
            self.assertEqual(broker.commands,[f"dump restore {mode} 7 {suffix}/private/state with spaces.uwsnap"])
            self.assertTrue(response["body"]["pending"]); self.assertIsNone(adapter.stop_key)
    def test_permissions_decline_before_transport(self):
        for permission in ("none","limited","read-only"):
            response,broker,_=self.run_request({"continue":True},permission)
            self.assertFalse(response["success"]); self.assertEqual(broker.commands,[])
            adapter=dap.Adapter(io.BytesIO()); adapter.broker=Broker(); adapter.debug_permissions=permission
            with self.assertRaises(ValueError): adapter.command("dump restore continue")
            self.assertEqual(adapter.broker.commands,[])
    def test_replay_uses_the_same_retirement_boundary(self):
        response,broker,adapter=self.run_request({"path":"/private/recording.uwsnap","stopId":"7","environment":"replay"})
        self.assertTrue(response["success"],response)
        self.assertEqual(broker.commands,["dump replay 7 /private/recording.uwsnap"])
        self.assertTrue(adapter.snapshot_restore_pending)
        for permission in ("none","limited","read-only"):
            response,broker,_=self.run_request({"path":"/private/a","stopId":"7","environment":"replay"},permission)
            self.assertFalse(response["success"]);self.assertEqual(broker.commands,[])
            adapter=dap.Adapter(io.BytesIO());adapter.broker=Broker();adapter.debug_permissions=permission
            for text in ("dump replay 7 /private/a","dump record start 7","dump record status"):
                with self.assertRaises(ValueError):adapter.command(text)
            self.assertEqual(adapter.broker.commands,[])
    def test_recording_arguments_refuse_before_transport(self):
        invalid=[{"action":"automatic"},{"action":"start","stopId":True},
            {"action":"start","stopId":"0"},{"action":"save","stopId":"7","path":"relative"},
            {"action":"save","stopId":"7","path":"/private/a\ncontinue"}]
        for args in invalid:
            adapter=dap.Adapter(io.BytesIO());adapter.broker=Broker()
            adapter.handle({"seq":1,"command":"uwvm/recordSnapshot","arguments":args})
            self.assertEqual(adapter.broker.commands,[],args)
        for permission in ("none","limited","read-only"):
            adapter=dap.Adapter(io.BytesIO());adapter.broker=Broker();adapter.debug_permissions=permission
            adapter.handle({"seq":1,"command":"uwvm/recordSnapshot","arguments":{"action":"start","stopId":"7"}})
            self.assertEqual(adapter.broker.commands,[])
    def record_request(self, action, reply):
        output=io.BytesIO();adapter=dap.Adapter(output);adapter.debug_permissions="full";adapter.broker=Broker(reply)
        args={"action":action}
        if action!="status":args["stopId"]="7"
        if action=="save":args["path"]="/private/recording.uwsnap"
        adapter.handle({"seq":1,"command":"uwvm/recordSnapshot","arguments":args})
        output.seek(0)
        while line:=output.readline():
            size=int(line.split(b":")[1]);self.assertEqual(output.readline(),b"\r\n")
            message=json.loads(output.read(size))
            if message["type"]=="response":return message
        self.fail("no DAP recording response")
    def test_recording_reports_actual_execution_state(self):
        for text,expected in (("guest stopped","stopped"),("guest running","running"),
                ("pause incomplete","stopping"),("guest exited","exited"),("debug domain closed","closed")):
            reply=f"snapshot-record mode=1 instructions=9 events=12 cursor=0 state-error=0 journal-error=0 saved=0; {text}\n"
            response=self.record_request("status",reply)
            self.assertTrue(response["success"],response);self.assertEqual(response["body"]["execution"],expected)
            response=self.record_request("start",reply)
            self.assertEqual(response["success"],expected=="stopped",response)
    def test_recording_save_requires_sealed_stopped_acknowledgement(self):
        for mode,saved,state,success in ((2,1,"guest stopped",True),(1,1,"guest stopped",False),
                (2,0,"guest stopped",False),(2,1,"guest running",False),(2,1,"guest stop retained",False)):
            reply=f"snapshot-record mode={mode} instructions=9 events=12 cursor=0 state-error=0 journal-error=0 saved={saved}; {state}\n"
            response=self.record_request("save",reply);self.assertEqual(response["success"],success,response)
    def test_recording_refusal_preserves_actual_diagnostic(self):
        reply="error: snapshot-record rejected mode=1 instructions=9 events=12 cursor=0 state-error=0 journal-error=0 saved=0; guest running\n"
        response=self.record_request("save",reply);self.assertFalse(response["success"])
        self.assertIn("mode=1",response["message"]);self.assertIn("guest running",response["message"])
    def test_retained_pending_recording_is_explicit(self):
        for mode,saved,state,success in ((1,1,"guest stopped",True),(2,1,"guest stopped",False),
                (1,0,"guest stopped",False),(1,1,"guest running",False)):
            reply=f"snapshot-record mode={mode} instructions=9 events=12 cursor=0 state-error=0 journal-error=0 saved={saved} retained=1; {state}\n"
            response=self.record_request("save",reply);self.assertEqual(response["success"],success,response)
            if success:self.assertTrue(response["body"]["retainedNativePending"])
        reply="snapshot-record mode=1 instructions=9 events=12 cursor=0 state-error=0 journal-error=0 saved=1 retained=1; guest stopped\n"
        self.assertFalse(self.record_request("start",reply)["success"])
        self.assertFalse(self.record_request("status",reply)["success"])
    def test_native_reattachment_reopens_only_authenticated_stop(self):
        adapter=dap.Adapter(io.BytesIO());adapter.debug_permissions="full"
        ack="snapshot-reattached stop=7 epoch=2; original native operation retained; guest paused; no external operation repeated\n"
        adapter.broker=Broker(ack)
        adapter.stop_key=("old",);adapter.frames[1]={"old":True}
        self.assertEqual(adapter.command("dump restore current 7 /x"),ack)
        self.assertFalse(adapter.snapshot_restore_pending);self.assertEqual(adapter.frames,{})
        adapter.broker.reply=ack.replace("epoch=2",f"epoch={1<<64}")
        adapter.command("dump restore current 7 /x")
        self.assertTrue(adapter.snapshot_restore_pending)
    def test_malformed_never_reaches_vm(self):
        base={"path":"/x","stopId":"7","environment":"recorded"}
        invalid=[{"continue":1},{"continue":True,"path":"/x"},
            {**base,"path":"relative"},{**base,"path":"/x\ncontinue"},{**base,"stopId":True},
            {**base,"stopId":"0"},{**base,"stopId":str(1<<64)},{**base,"environment":"automatic"},
            {**base,"bindings":{}},{**base,"bindings":[{"module":0,"resource":0,"descriptor":1<<31}]},
            {**base,"bindings":[{"module":True,"resource":0,"descriptor":1}]},
            {**base,"bindings":[{"module":0,"resource":0,"descriptor":1}]*2},
            {**base,"environment":"current","bindings":[{"module":0,"resource":0,"descriptor":1}]}]
        for args in invalid:
            with self.subTest(args=args):
                response,broker,_=self.run_request(args)
                self.assertFalse(response["success"],response); self.assertEqual(broker.commands,[])
    def test_old_values_retire_even_on_failure(self):
        adapter=dap.Adapter(io.BytesIO());adapter.broker=Broker("error: snapshot-restore rejected\n")
        adapter.stop_key=("old",);adapter.frames[1]={"old":True}
        with self.assertRaises(RuntimeError): adapter.command("dump restore current 7 /x")
        self.assertIsNone(adapter.stop_key); self.assertEqual(adapter.frames,{})
    def test_pending_is_not_reported_as_completed(self):
        for reply in ("", "snapshot-restored stop=8 epoch=3 source=ready; guest paused; external effects are not rewound\n"):
            response,_,adapter=self.run_request({"continue":True},reply=reply)
            self.assertFalse(response["success"]);self.assertIsNone(adapter.stop_key)
    def test_pending_does_not_terminate_or_republish_old_threads(self):
        response,broker,adapter=self.run_request({"continue":True})
        self.assertTrue(response["body"]["pending"])
        self.assertTrue(adapter.snapshot_restore_pending)
        self.assertEqual(adapter.threads,[])
        adapter.poll()
        self.assertEqual(broker.commands,["dump restore continue"])
        self.assertFalse(adapter.finished)
        for text in ("status","continue","locals wasm 7","dump restore current 7 /x"):
            with self.assertRaises(dap.LocalRequestRefusal): adapter.command(text)
        self.assertEqual(broker.commands,["dump restore continue"])
    def test_timeout_keeps_pending_and_erases_previous_values(self):
        class TimedOut(Broker):
            def request(self,command):
                self.commands.append(command)
                raise TimeoutError("transport timeout after dispatch")
        adapter=dap.Adapter(io.BytesIO());adapter.broker=TimedOut()
        adapter.stop_key=("old",);adapter.threads=[{"id":1}];adapter.frames[7]={"old":True}
        with self.assertRaises(TimeoutError):adapter.command("dump restore current 7 /x")
        self.assertTrue(adapter.snapshot_restore_pending)
        self.assertEqual(adapter.threads,[]);self.assertEqual(adapter.frames,{})
        adapter.poll();self.assertFalse(adapter.finished)
    def test_preparation_refusal_can_reauthenticate_original_stop(self):
        refusal="error: snapshot-restore rejected phase=4 detail=1 preparation=0 state-error=0 applied=0 retained=1; original stopped world retained; inspect status before further management\n"
        response,broker,adapter=self.run_request({"continue":True},reply=refusal)
        self.assertFalse(response["success"])
        self.assertFalse(adapter.snapshot_restore_pending)
        self.assertIsNone(adapter.stop_key)
    def test_native_preparation_refusal_exposes_error_and_allows_fresh_reads(self):
        refusal="error: snapshot-restore rejected phase=6 detail=11 preparation=5 state-error=14 applied=0 retained=1; original stopped world retained; inspect status before further management\n"
        response,broker,adapter=self.run_request({"continue":True},reply=refusal)
        self.assertFalse(response["success"])
        self.assertIn("state-error=14",response["message"])
        self.assertFalse(adapter.snapshot_restore_pending)
        self.assertIsNone(adapter.stop_key)
        broker.reply="stopped: pause\nstop-id 7\nthread 9 module=0 function=4 byte-offset=0 generation=2\n"
        self.assertEqual(adapter.command("status"),broker.reply)
        self.assertEqual(broker.commands,["dump restore continue","status"])
    def test_ambiguous_refusal_cannot_reopen_execution_access(self):
        for phase,detail,applied,retained in ((6,10,0,1),(6,14,0,1),(7,11,0,1),
                (6,11,1,1),(6,11,0,0),(11,11,0,1),(1,0,0,0)):
            refusal=f"error: snapshot-restore rejected phase={phase} detail={detail} preparation=5 state-error=14 applied={applied} retained={retained}; original stopped world retained; inspect status before further management\n"
            response,broker,adapter=self.run_request({"continue":True},reply=refusal)
            self.assertFalse(response["success"])
            self.assertTrue(adapter.snapshot_restore_pending)
            with self.assertRaises(dap.LocalRequestRefusal):adapter.command("status")
            self.assertEqual(broker.commands,["dump restore continue"])
    def test_success_requires_unique_bounded_new_cohort(self):
        complete="snapshot-restored stop=8 epoch=3 source=ready; guest paused; external effects are not rewound\n"
        thread="thread 9 module=0 function=4 byte-offset=0 generation=3\n"
        for rows,success in ((thread,True),(thread+thread,False),
                (thread.replace("thread 9","thread 0"),False),
                (thread.replace("module=0",f"module={1<<64}"),False),
                (thread.replace("generation=3","generation=2"),False)):
            class Completed(Broker):
                def request(self,command):
                    self.commands.append(command)
                    if command=="dump restore continue":return complete
                    if command=="status":return "stopped: pause\nstop-id 8\n"+rows
                    if command=="info breakpoints":return "no breakpoints\n"
                    raise AssertionError(command)
            output=io.BytesIO();adapter=dap.Adapter(output);adapter.broker=Completed()
            adapter.configuration_complete=True
            adapter.stop_key=("old",);adapter.frames[1]={"old":True}
            adapter.handle({"seq":1,"command":"uwvm/restoreSnapshot","arguments":{"continue":True}})
            output.seek(0);messages=[]
            while line:=output.readline():
                size=int(line.split(b":")[1]);self.assertEqual(output.readline(),b"\r\n")
                messages.append(json.loads(output.read(size)))
            response=next(m for m in messages if m["type"]=="response")
            self.assertEqual(response["success"],success,response)
            self.assertEqual(adapter.frames,{})
            if success:
                self.assertTrue(response["body"]["applied"])
                events=[m["event"] for m in messages if m["type"]=="event"]
                self.assertIn("invalidated",events);self.assertEqual(events[-1],"stopped")
                self.assertFalse(adapter.snapshot_restore_pending)
            else:self.assertIsNone(adapter.stop_key)
if __name__=="__main__": unittest.main()
