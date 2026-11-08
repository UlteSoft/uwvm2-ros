#!/usr/bin/env python3
"""Allocation-tag DATA admission and stop ownership, not a hardware MTE proof."""
import base64
import io
import unittest
import test_dap_wasm_memory_pages as memory
state=memory.state
dap=state.dap

def color(tag):
    hue=((tag*137)&255)*6;rise=64+(hue%256)*191//255;fall=319-rise
    return "#%02x%02x%02x"%((255,rise,64),(fall,255,64),(64,255,rise),(64,fall,255),(rise,64,255),(255,64,fall))[hue//256]

class Broker(memory.Broker):
    def __init__(self):
        super().__init__();self.tag_status="available";self.tag_after=None;self.tag_fault=None
    def request(self,command):
        if not command.startswith("memtags "):return super().request(command)
        self.commands.append(command)
        _,module,index,offset,count=command.split();module,index,offset,count=map(int,(module,index,offset,count))
        first=offset//16;amount=0 if not count or self.tag_status!="available" else (offset+count-1)//16-first+1
        size=1<<40;bits=8 if self.tag_status=="available" else 0
        if self.tag_status=="unsupported-memory":size=first=0
        text=f"Wasm memory tags module={module} index={index} offset={offset} bytes={count} status={self.tag_status} memory-bytes={size} tag-bits={bits} first-granule={first} count={amount}\n"
        for i in range(amount):
            tag=(first+i)%256;text+=f"tag offset={(first+i)*16} value=0x{tag:02x} color={color(tag)}\n"
        text+="memory-tags-end\n"
        if self.tag_fault=="color":text=text.replace(color(first%256),"#000000")
        elif self.tag_fault=="address":text=text.replace(f"tag offset={first*16}",f"tag offset={first*16+1}")
        elif self.tag_fault=="fabricated":text=text.replace("count=0","count=1")
        elif self.tag_fault=="selector":text=text.replace(f"module={module}",f"module={module+1}")
        elif self.tag_fault=="extra":text+="host pointer 0xffff\n"
        elif self.tag_fault=="bits":text=text.replace("tag-bits=8","tag-bits=3")
        elif self.tag_fault=="width":text=text.replace("tag-bits=8","tag-bits=4")
        if self.tag_after:self.tag_after()
        return text

class MemoryTags(unittest.TestCase):
    send=state.StateProtocol.send
    def setUp(self):
        self.output=io.BytesIO();self.adapter=dap.Adapter(self.output);self.broker=Broker();self.adapter.broker=self.broker;self.sequence=0
    def read(self,**args):
        request={"memoryReference":"wasm-memory:2:1:15","count":256};request.update(args)
        return self.send("uwvm/memoryTags",**request)
    def test_actual_guest_offsets_and_matching_byte_preview(self):
        reply=self.read(includeBytes=True);self.assertTrue(reply["success"],reply);body=reply["body"]
        self.assertEqual(len(body["tags"]),17);self.assertEqual(body["tags"][0],dict(offset="0",tag=0,hex="0x00",color=color(0)))
        self.assertEqual(body["nextMemoryReference"],"wasm-memory:2:1:271")
        self.assertEqual(body["stopId"],str(self.broker.stop))
        self.assertEqual(base64.b64decode(body["data"]),bytes((2*17+31+15+i)&255 for i in range(256)))
    def test_any_valid_memory64_page_and_zero_bytes(self):
        reply=self.read(memoryReference="wasm-memory:2:1:4294967311",count=1);self.assertTrue(reply["success"],reply)
        self.assertEqual(reply["body"]["tags"][0]["offset"],"4294967296")
        self.assertEqual(self.read(count=0)["body"]["tags"],[])
    def test_disabled_and_external_memory_are_explicit(self):
        for status in ("disabled","unsupported-memory"):
            self.setUp();self.broker.tag_status=status;reply=self.read();self.assertTrue(reply["success"],reply)
            self.assertEqual(reply["body"]["status"],status);self.assertEqual(reply["body"]["tags"],[])
    def test_invalid_and_host_selectors_do_not_reach_broker(self):
        for args in ({"count":257},{"count":True},{"count":-1},{"includeBytes":1},{"offset":True},
                     {"memoryReference":"0x1234"},{"memoryReference":"wasm-memory:0:4294967296:0"},
                     {"memoryReference":"wasm-memory:0:0:18446744073709551615","count":1}):
            self.setUp();self.assertFalse(self.read(**args)["success"]);self.assertEqual(self.broker.commands,[])
    def test_running_native_and_unidentified_stop_cannot_read_tags(self):
        for raw in ("running\n","prepared; no Wasm instruction executed\n","stopped: pause\nstop-id 41\n"):
            self.setUp();self.broker.status_override=raw;self.assertFalse(self.read()["success"])
            self.assertFalse(any(x.startswith("memtags ") for x in self.broker.commands))
        self.setUp();self.broker.native=True;self.assertFalse(self.read()["success"])
    def test_retired_stop_generation_and_cohort_do_not_publish(self):
        for field,value in (("stop",100),("epoch",8),("extra_thread",True)):
            self.setUp();self.broker.tag_after=lambda:setattr(self.broker,field,value)
            reply=self.read();self.assertFalse(reply["success"]);self.assertNotIn("body",reply)
    def test_malformed_rows_and_palette_fail_closed(self):
        for fault in ("color","address","selector","extra","bits","width"):
            self.setUp();self.broker.tag_fault=fault;self.assertFalse(self.read()["success"],fault)
        self.setUp();self.broker.tag_status="disabled";self.broker.tag_fault="fabricated";self.assertFalse(self.read()["success"])
    def test_bytes_and_tags_must_come_from_same_stop(self):
        self.broker.after_page=lambda:setattr(self.broker,"stop",self.broker.stop+1)
        reply=self.read(includeBytes=True);self.assertFalse(reply["success"]);self.assertNotIn("body",reply)

if __name__=="__main__":unittest.main()
