#!/usr/bin/env python3
"""Pure parser/generator/JavaScript models; never execute a native compiler.

Synthetic native identity strings below test syntax and binding only. They are
not official native identities and must never be treated as ROS qualification.
Generated files live in temporary directories. The persistent JSON report pins
the generator and every model source and explicitly records execution scope.
"""
from __future__ import annotations
import argparse
import hashlib
import importlib.util
import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
GENERATOR = ROOT / "tools/custom_types.py"
spec = importlib.util.spec_from_file_location("uwvm_custom_types_generator", GENERATOR)
g = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = g
spec.loader.exec_module(g)

class Models(unittest.TestCase):
    """Bounded fixed-record preparation tests, independent of native ROS."""
    javascript_checks = 0
    def catalog(self, directory, fields, version=1, name="test_custom/Packet", identity=True):
        p = Path(directory)
        (p/"Packet.msg").write_text(fields)
        entry = {"ros_version":version,"type":name,"file":"Packet.msg"}
        if identity:entry["native_identity"]="0123456789abcdef0123456789abcdef" if version==1 else "RIHS01_"+"a"*64
        path=p/"catalog.json";path.write_text(json.dumps({"format":g.PROFILE,"types":[entry]}));return path

    def test_exact_packet_layout(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.catalog(t,"int32 sequence\nbool active\nfloat64 measurement\nuint8[4] tag\nint16[3] samples\n")
            _,records,_=g.load_catalog(p)
            self.assertEqual((records[0].wire_bytes,len(records[0].cells)),(23,10))
            self.assertTrue(records[0].codec_identity.startswith("UWVMROS1_"))

    def test_preparation_requires_native_identity_for_emission(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.catalog(t,"int32 sequence\n",identity=False)
            self.assertEqual(g.load_catalog(p,False)[1][0].wire_bytes,4)
            with self.assertRaisesRegex(g.GenerationError,"official native_identity"):
                g.generate(p,Path(t)/"out")
            self.assertFalse((Path(t)/"out").exists())
            proof=g.generate(p,Path(t)/"guest",True)
            self.assertEqual(proof["status"],"GENERATED_GUEST_SOURCE_ONLY")
            self.assertFalse(proof["native_registry_emitted"])
            self.assertEqual(len(proof["outputs"]),6)
            for absent in ["manifest.hpp","native_ros1.hpp","native_ros2.hpp"]:self.assertFalse((Path(t)/"guest"/absent).exists())

    def test_comment_independent_codec_identity(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.catalog(t,"int32 sequence\n")
            a=g.load_catalog(p)[1][0].codec_identity
            (Path(t)/"Packet.msg").write_text("# comment\n  int32   sequence # another comment\n")
            self.assertEqual(a,g.load_catalog(p)[1][0].codec_identity)
            (Path(t)/"Packet.msg").write_text("uint32 sequence\n")
            self.assertNotEqual(a,g.load_catalog(p)[1][0].codec_identity)

    def test_version_and_field_name_change_identity(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.catalog(t,"int32 sequence\n");a=g.load_catalog(p)[1][0].codec_identity
            p=self.catalog(t,"int32 sequence\n",2,"test_custom/msg/Packet");b=g.load_catalog(p)[1][0].codec_identity
            p=self.catalog(t,"int32 counter\n");c=g.load_catalog(p)[1][0].codec_identity
            self.assertNotEqual(a,b);self.assertNotEqual(a,c)

    def test_unsupported_fields_never_silently_discarded(self):
        samples=["string text", "int32[] values", "int32[<=3] values", "string<=3 text", "int32 x 4", "int32 X=4", "byte value", "char value", "time stamp", "Header header", "int32 x\n---\nint32 y", "int32[0] values", "int32[4097] values", "int32 class", "int32 __proto__", "int32 x\nint32 x", "int32 x;", "int32[2][2] values"]
        for fields in samples:
            with self.subTest(fields=fields),tempfile.TemporaryDirectory() as t:
                with self.assertRaises(g.GenerationError):g.load_catalog(self.catalog(t,fields))

    def test_catalog_keys_names_paths_and_identity_are_strict(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.catalog(t,"int32 sequence\n");original=json.loads(p.read_text())
            cases=[]
            for key,value in [("native_identity","bad"),("file","../outside.msg"),("ros_version",True),("type","std_msgs/String"),("type","test_custom/srv/Packet")]:
                doc=json.loads(json.dumps(original));doc["types"][0][key]=value;cases.append(doc)
            doc=json.loads(json.dumps(original));doc["types"][0]["ignored"]="field";cases.append(doc)
            doc=json.loads(json.dumps(original));doc["types"]*=2;cases.append(doc)
            for doc in cases:
                p.write_text(json.dumps(doc))
                with self.assertRaises(g.GenerationError):g.load_catalog(p)
            p.write_text('{"format":"x","format":"y","types":[]}')
            with self.assertRaisesRegex(g.GenerationError,"duplicate JSON key"):g.load_catalog(p)

    def test_idl_fixed_record_and_explicit_nested_dependencies(self):
        definitions=g.parse_idl("module test_custom {module msg {struct Small {short value;};struct Packet {Small samples[2];unsigned long long count;};};};")
        self.assertEqual(definitions["test_custom/msg/Packet"][0],g.Member("samples","test_custom/msg/Small",2))
        self.assertEqual(definitions["test_custom/msg/Packet"][1].type,"uint64")

    def test_idl_and_msg_normalize_to_the_same_schema(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.catalog(t,"uint32 sequence\nbool enabled\nfloat32 temperature\nfloat64[3] acceleration\nuint8[8] tag\nint16[2] thresholds\n",2,"test_custom/msg/Packet")
            original=g.load_catalog(p)[1][0].codec_identity
            (Path(t)/"Packet.idl").write_text("module test_custom {module msg {struct Packet {unsigned long sequence;boolean enabled;float temperature;double acceleration[3];uint8 tag[8];short thresholds[2];};};};")
            doc=json.loads(p.read_text());doc["types"][0]["file"]="Packet.idl";p.write_text(json.dumps(doc))
            self.assertEqual(original,g.load_catalog(p)[1][0].codec_identity)

    def test_idl_unsupported_constructs_rejected(self):
        for text in ['#include "x.idl"','module test_custom {module msg {struct Packet {octet value;};};};','module test_custom {module msg {struct Packet {string text;};};};','module test_custom {module msg {struct Packet {sequence<long,4> values;};};};','module test_custom {module msg {struct Packet {@key long value;};};};','typedef long Number;','module test_custom {module msg {struct Packet {long value=3;};};};','module test_custom {module msg {struct Packet {long values[2][2];};};};']:
            with self.subTest(text=text),self.assertRaises(g.GenerationError):g.parse_idl(text)

    def test_graph_bounds_missing_recursive_and_cached_depth(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.catalog(t,"Missing member\n")
            with self.assertRaisesRegex(g.GenerationError,"explicitly"):g.load_catalog(p)
            p=self.catalog(t,"Packet member\n")
            with self.assertRaisesRegex(g.GenerationError,"recursive"):g.load_catalog(p)
            entries=[]
            for i in range(17):
                filename=f"Record{i}.msg";(Path(t)/filename).write_text("uint8 cell\n" if i==0 else f"Record{i-1} child\n")
                entries.append({"ros_version":1,"type":f"test_custom/Record{i}","file":filename,"native_identity":"0"*32})
            p.write_text(json.dumps({"format":g.PROFILE,"types":entries}))
            with self.assertRaisesRegex(g.GenerationError,"depth"):g.load_catalog(p)
            p=self.catalog(t,"uint64[4096] cells\n")
            self.assertEqual(g.load_catalog(p)[1][0].wire_bytes,32768)
            p=self.catalog(t,"uint8[4096] cells\nuint8 extra\n")
            with self.assertRaisesRegex(g.GenerationError,"cells"):g.load_catalog(p)

    def test_regular_bounded_input_and_fresh_output(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.catalog(t,"uint8 cell\n");out=Path(t)/"out"
            g.generate(p,out);old=(out/"generation.json").read_bytes()
            with self.assertRaisesRegex(g.GenerationError,"already exists"):g.generate(p,out)
            self.assertEqual(old,(out/"generation.json").read_bytes())
            fifo=Path(t)/"fifo";os.mkfifo(fifo)
            with self.assertRaisesRegex(g.GenerationError,"regular file"):g.bounded_read(fifo,10)
            huge=Path(t)/"huge";huge.write_bytes(b"a"*11)
            with self.assertRaisesRegex(g.GenerationError,"regular file"):g.bounded_read(huge,10)

    def test_generated_six_languages_and_manifest_native_contract(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.catalog(t,"bool enabled\nint64 counter\nfloat32 temperature\nfloat64[2] samples\n")
            out=Path(t)/"out";proof=g.generate(p,out)
            self.assertEqual(len(proof["outputs"]),9)
            self.assertEqual(proof["status"],"GENERATED_SOURCE_ONLY")
            self.assertEqual(proof["native_execution"],"NOT_RUN")
            for name,pin in proof["outputs"].items():self.assertEqual(hashlib.sha256((out/name).read_bytes()).hexdigest(),pin["sha256"])
            common=(out/"manifest.hpp").read_text();native=(out/"native_ros1.hpp").read_text()
            self.assertIn("custom_topic_entries() noexcept",common);self.assertIn("custom_scalar_kind::boolean",common)
            self.assertIn("UWVM_ROS_CUSTOM_ROS1_TYPES(M)",native)
            self.assertIn("static bool valid(Native const& value) noexcept",native)
            self.assertIn("uint8_t* output,size_t capacity",native)
            self.assertIn("if(value.enabled!=0&&value.enabled!=1)return false;",native)
            self.assertIn("custom_native_schema native_schema() noexcept",native)
            self.assertIn("definition_count=sizeof(definitions)",native)
            self.assertIn('static_assert(std::is_same<Ros1Test_customPacketCodec_field_0_0,uint8_t>::value',native)
            self.assertIn('fixed_array_shape<Ros1Test_customPacketCodec_field_0_3>::size==2',native)
            self.assertIn("namespace roswasmcpp",(out/"fixed_records.hpp").read_text())
            self.assertNotIn("roswasmc_copy_object",native)
            self.assertTrue(next(x for x in native.splitlines() if x.startswith("#define UWVM_ROS_CUSTOM_ROS1_TYPES")).endswith("\\"))
            self.assertFalse(next(x for x in native.splitlines() if x.startswith("#define UWVM_ROS_CUSTOM_ROS1_TYPES")).endswith("\\\\"))

    def test_full_native_schema_metadata_and_ros_boolean_types(self):
        with tempfile.TemporaryDirectory() as t:
            p=Path(t);(p/"Leaf.msg").write_text("int16 number\nbool enabled\n");(p/"Packet.msg").write_text("Leaf[2] first\nLeaf second\n")
            entries=[]
            for version in [1,2]:
                for name in ["Packet","Leaf"]:entries.append({"ros_version":version,"type":"test_custom/"+("msg/" if version==2 else "")+name,"file":name+".msg","native_identity":"0"*32 if version==1 else "RIHS01_"+"0"*64})
            catalog=p/"catalog.json";catalog.write_text(json.dumps({"format":g.PROFILE,"types":entries}));out=p/"out";g.generate(catalog,out)
            one=(out/"native_ros1.hpp").read_text();two=(out/"native_ros2.hpp").read_text()
            self.assertIn('std::is_same<Ros1Test_customPacketCodec_field_1_1,uint8_t>::value',one)
            self.assertIn('std::is_same<Ros2Test_customPacketCodec_field_1_1,bool>::value',two)
            self.assertIn('fixed_array_shape<Ros1Test_customPacketCodec_field_0_0>::size==2',one)
            self.assertIn('fixed_array_shape<Ros2Test_customPacketCodec_field_0_0>::value_type,::test_custom::msg::Leaf',two)
            self.assertIn('{"test_custom/Packet","test_custom/Leaf[2] first\\ntest_custom/Leaf second\\n"}',one)
            self.assertIn('{"test_custom/msg/Packet",schema_fields_0',two)
            self.assertIn('{"test_custom/msg/Leaf",schema_fields_1',two)
            self.assertIn('{"first","test_custom/msg/Leaf",2,true}',two)
            self.assertIn('{"second","test_custom/msg/Leaf",0,false}',two)
            # The root closure contains repeated Leaf fields but one metadata record.
            packet_body=two.split('struct Ros2Test_customPacketCodec {',1)[1].split('struct Ros2Test_customLeafCodec {',1)[0]
            self.assertEqual(packet_body.count('{"test_custom/msg/Leaf",schema_fields_'),1)

    def test_generated_javascript_roundtrip_and_negative_vectors(self):
        with tempfile.TemporaryDirectory() as t:
            p=Path(t);example=ROOT/"custom-types/examples"
            for filename in ["ScalarRecord.msg","NestedRecord.msg"]:(p/filename).write_bytes((example/filename).read_bytes())
            types=[{"ros_version":1,"type":"test_custom/"+n,"file":n+".msg","native_identity":"0"*32} for n in ["ScalarRecord","NestedRecord"]]
            catalog=p/"catalog.json";catalog.write_text(json.dumps({"format":g.PROFILE,"types":types}));out=p/"out";g.generate(catalog,out)
            script=Path(__file__).with_name("javascript_models.mjs")
            result=subprocess.run(["node",str(script),str(out/"fixed_records.mjs")],text=True,capture_output=True,timeout=15)
            self.assertEqual(result.returncode,0,result.stdout+result.stderr)
            data=json.loads(result.stdout);self.assertEqual(data["status"],"PURE_JAVASCRIPT_MODEL_PASS");self.assertGreaterEqual(data["checks"],30);type(self).javascript_checks=data["checks"]

    def test_retained_guest_sources_reproduce_without_native_registration(self):
        directory=ROOT/"custom-types/generated-r3"
        # Keep the historical r3 receipt and all six codec bodies immutable.
        # A separate current proof binds today's generator bytes; every other
        # receipt field must still equal the original preparation exactly.
        historical=json.loads((directory/"generation.json").read_text())
        retained=json.loads((ROOT/"custom-types/generated-r3-current-generation.json").read_text())
        self.assertEqual({key:value for key,value in retained.items() if key!="generator_sha256"},
                         {key:value for key,value in historical.items() if key!="generator_sha256"})
        self.assertEqual(retained["generator_sha256"],hashlib.sha256(GENERATOR.read_bytes()).hexdigest())
        with tempfile.TemporaryDirectory() as t:
            proof=g.generate(ROOT/"custom-types/examples/catalog.json",Path(t)/"fresh",True)
            self.assertEqual(proof,retained)
            for name in proof["outputs"]:self.assertEqual((Path(t)/"fresh"/name).read_bytes(),(directory/name).read_bytes())
        self.assertFalse(retained["native_registry_emitted"])
        self.assertTrue(all(t["native_identity"] is None for t in retained["types"]))

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument("--report",type=Path);a=p.parse_args()
    result=unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(Models))
    evidence={"status":"SOURCE_AND_PURE_MODEL_PASS" if result.wasSuccessful() else "SOURCE_AND_PURE_MODEL_FAIL","methods":result.testsRun,"javascript_checks":Models.javascript_checks,"failures":len(result.failures),"errors":len(result.errors),"actual_compilation":"NOT_RUN","native_ros_behavior":"NOT_RUN","wasm_behavior":"NOT_RUN","synthetic_identities":"syntax/models only; never official native qualification","inputs":{str(p.relative_to(ROOT.parent)):{"bytes":p.stat().st_size,"sha256":hashlib.sha256(p.read_bytes()).hexdigest()} for p in [GENERATOR,Path(__file__),Path(__file__).with_name("javascript_models.mjs")]}}
    if a.report:
        with a.report.open("x") as f:json.dump(evidence,f,indent=2);f.write("\n")
    return 0 if result.wasSuccessful() else 1
if __name__=="__main__":raise SystemExit(main())
