#!/usr/bin/env python3
"""Pure adversarial data/source models, never native or Runtime acceptance.

Root executes this later in its serial lane with a fresh /tmp receipt. Only
selected pure definitions are AST-extracted; controller modules, subprocess,
socket and ROS APIs are not imported/executed. Generated WAT is examined as
source, not compiled, validated or instantiated. Real behavior needs the
separate qualified fixture/compiler/Runtime/native executions.
"""
import argparse
import ast
import copy
import hashlib
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[2];checks=[]


def check(value,label):
    if not value:raise AssertionError(label)
    checks.append(label)


def extract(relative,names,globals):
    source=ROOT/relative;tree=ast.parse(source.read_text(),filename=str(source))
    nodes=[node for node in tree.body if isinstance(node,(ast.FunctionDef,ast.AsyncFunctionDef)) and node.name in names]
    check({node.name for node in nodes}==set(names),"exact pure extraction "+relative)
    scope=dict(globals);exec(compile(ast.Module(body=nodes,type_ignores=[]),str(source),"exec"),scope);return scope


def refuses(function,label):
    try:function()
    except (ValueError,KeyError,TypeError,IndexError):checks.append(label);return
    raise AssertionError("unexpected acceptance: "+label)


def balanced(text):
    """Source parentheses only; explicitly not a Wasm validator."""
    depth=0;quoted=False;escape=False
    for line in text.splitlines():
        if line.lstrip().startswith(";;"):continue
        for c in line:
            if quoted:
                if escape:escape=False
                elif c=="\\":escape=True
                elif c=='"':quoted=False
            elif c=='"':quoted=True
            elif c=='(':depth+=1
            elif c==')':
                depth-=1
                if depth<0:return False
    return depth==0 and not quoted


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument("--out",type=Path,required=True);args=parser.parse_args()
    check(args.out.is_absolute() and args.out.is_relative_to(Path("/tmp")) and not args.out.exists(),"fresh explicit temporary receipt")
    protocol=extract("tests/ros_acceptance/protocol.py",("require","integer","marker_lines","queue_samples","side_effect"),{})
    pure=extract("tests/ros_acceptance/normalize.py",("select","fact"),{"require":protocol["require"]})
    select,fact=pure["select"],pure["fact"]
    source={"rows":[{"name":"one","count":2},{"name":"two","count":3}]}
    check(select(source,[{"key":"rows"},{"where":"name","equals":"two"},{"key":"count"}])==3,"exact-one nested raw selection")
    for path in ([{"key":"missing"}],[{"key":"rows"},{"index":-1}],[{"key":"rows"},{"index":True}],
                 [{"key":"rows"},{"where":"name","equals":"absent"}],[{"key":"rows"},{"key":"count"}],
                 [{"key":"rows"},{"wildcard":True}], [{"key":"rows"},{"index":99999}]):
        refuses(lambda path=path:select(source,path),"refuse raw selector "+repr(path))
    duplicate={"rows":[{"name":"one"},{"name":"one"}]}
    refuses(lambda:select(duplicate,[{"key":"rows"},{"where":"name","equals":"one"}]),"duplicate raw record never first-match")
    for actual,operator,expected in ((True,"eq",1),(1,"eq",True),(True,"gte",1),(2,"lte",True),
                                     (2,"eq",3),([],"length",1),(2,"gte",3),(4,"lte",3),(1,"unknown",1)):
        refuses(lambda actual=actual,operator=operator,expected=expected:fact(actual,{"operator":operator,"expected":expected}),"refuse raw fact "+repr((actual,operator,expected)))
    for actual,operator,expected in ((2,"eq",2),([0,1],"length",2),(2,"gte",1),(2,"lte",3)):
        fact(actual,{"operator":operator,"expected":expected});check(True,"accept exact typed raw fact "+operator)
    positive={"expected_errno":13,"actual_errno":13,"output_sentinels_unchanged":True,"native_observer_independent":True,
        "native_effects_before":{"publishers":0,"messages":0},"native_effects_after":{"publishers":0,"messages":0},
        "observer_kind":"dds","quiet_samples":30,"quiet_ns":2000000000}
    protocol["side_effect"](positive);check(True,"independent exact native no-effect model")
    for key,value in (("expected_errno",True),("actual_errno",9),("output_sentinels_unchanged",False),
                      ("native_observer_independent",False),("native_effects_after",{"publishers":1,"messages":0}),
                      ("observer_kind","audit"),("quiet_samples",29),("quiet_samples",True),("quiet_ns",1999999999)):
        wrong=copy.deepcopy(positive);wrong[key]=value
        refuses(lambda wrong=wrong:protocol["side_effect"](wrong),"refuse attack without actual bound "+key)
    for counters in (None,{}, {"messages":True},{"payload":"unmeasured"}):
        wrong=copy.deepcopy(positive);wrong["native_effects_before"]=wrong["native_effects_after"]=counters
        refuses(lambda wrong=wrong:protocol["side_effect"](wrong),"refuse absent/nonnumeric native counters "+repr(counters))
    wrong=copy.deepcopy(positive);wrong["expected_errno"]=1;wrong["actual_errno"]=True
    refuses(lambda:protocol["side_effect"](wrong),"boolean is not actual errno1")
    marker={"PASS_NATIVE":("pid","count")}
    check(protocol["marker_lines"]("PASS_NATIVE pid=123 count=1\n","",marker)==[("PASS_NATIVE",{"pid":"123","count":"1"})],"closed native marker model")
    for text in ("PASS_NATIVE count=1 pid=123\n","PASS_NATIVE pid=123 pid=123\n","PASS_NATIVE pid=123 count=1 extra=0\n",
                 "PASS_NATIVE pid=123 count=1","PASS_NATIVE pid=123 count=1\r\n","FAIL_NATIVE secret=neverecho\n",
                 "UNKNOWN pid=123 count=1\n","PASS_NATIVE pid=123 count=1\x00\n"):
        refuses(lambda text=text:protocol["marker_lines"](text,"",marker),"refuse native transcript "+repr(text))
    fixtures=extract("tests/ros_acceptance/fixtures.py",("escaped","memory_guest","recovery_guest","graph_guest","sdk_gate"),{})
    for versions in ((1,),(2,),(1,2)):
        text=fixtures["memory_guest"](versions);check(balanced(text),"source-balanced memory fixture "+str(versions))
        check(text.count('(memory')>=2 and '(memory (export "memory") 1 1)' in text and '(memory 1 1)' in text,"fixed separate memories "+str(versions))
        check('(i64.load 1 (i32.const 48))' in text and '(memory.fill 1 (i32.const 8192)' in text and '(i32.const 4096)' in text,"memory1 payload and memory0 full sentinel "+str(versions))
        check('(i32.const 200)' in text and '(i32.const 11)' in text and '(i32.const 9)' in text,"bounded poll/refusal/stale checks "+str(versions))
    for version in (1,2):
        text=fixtures["recovery_guest"](version);check(balanced(text),"source-balanced recovery "+str(version))
        check('(data (memory 1) (i32.const 256)' in text and '(data (memory 1) (local.get' not in text,"constant data survives response parameterization "+str(version))
        check('(i32.ne (i32.and (local.get $mask) (i32.const 2)) (i32.const 0))' in text,"bit2 mask normalized before boolean-and "+str(version))
        check(text.count('PASS_BACKEND_RECOVERY')==0 and '(i64.const 31)' in text and '(i64.const 30)' in text,"escaped exact distinct stages "+str(version))
        check(all('(i32.const '+str(byte)+')' in text for byte in (71,66,88,89,65,82)),"all explicit recovery gates "+str(version))
        graph=fixtures["graph_guest"](version);check(balanced(graph),"source-balanced Graph quota "+str(version))
        check('(i32.const 98)' in graph and 'graph_inspect_node' in graph and 'GRAPH_READY_AGAIN' in graph,"readiness separated from exact98 refusal window "+str(version))
    requirements=extract("tests/ros_acceptance/requirements.py",("inventories",),{"Path":Path,"re":re,
        "ORIGINAL":"documents/ros/original-task-audit-20261013-evidence/original-task.txt",
        "SECTIONS":{"security":("二十八、完整权限攻击测试","二十九、性能与稳定性测试",55),"examples":("三十二、必须提供的示例","三十三、任务实施顺序",20),
                    "acceptance":("三十四、最终验收标准","三十五、最终输出要求",50),"outputs":("三十五、最终输出要求",None,23)}})
    inventory=requirements["inventories"](ROOT)
    for name,count in (("chapters",35),("phases",12),("acceptance",50),("security",55),("examples",20),("outputs",23)):
        check(len(inventory[name])==count,"complete original inventory "+name)
    templates=extract("tests/ros_acceptance/selection_template.py",
        ("key","where","suite","eq","count","base_record","legacy_case","legacy_records","original_mapping"),
        {"json":json,"NETWORK_CASES":("cross-module-proxy","cross-module-tail-proxy","cross-module-handle","cross-backend-handle",
            "ros1-wasm-publish","ros1-wasm-subscribe","ros2-wasm-publish","ros2-wasm-subscribe","hybrid-ros1-to-ros2","hybrid-ros2-to-ros1",
            "gateway-disconnect-independent","gateway-disconnect-recovery","gateway2-disconnect-independent","gateway2-disconnect-recovery")})
    references,examples=templates["original_mapping"]();records,documents=templates["legacy_records"]()
    for name,rows in inventory.items():check(set(references[name])=={str(row["id"]) for row in rows},"concrete numeric selection "+name)
    names={row["id"] for row in records}
    check(len(examples)==20,"twenty exact current legacy example identities")
    check(all(reference["record"] in names or reference["record"].startswith("example-") for category in references.values()
        for refs in category.values() for reference in refs if reference["launch"]=="original-all"),"every concrete legacy reference exists")
    check(references["security"]["10"][0]["record"]=="ros1-graph_denied" and
        references["security"]["44"][0]["record"]=="graph-query-flood-ros1","Graph13 and allowed2/flood11 have separate contracts")
    check(len(documents)==len({entry["id"] for entry in documents}) and
        all(entry["from"]=="raw" or entry["from"] in {old["id"] for old in documents[:index]} for index,entry in enumerate(documents)),
        "nested report references have unique closed preceding parents")
    paths=list((ROOT/"tests/ros_acceptance").glob("*.py"))+[ROOT/"examples/ros/materials.py"]
    for path in paths:ast.parse(path.read_text(),filename=str(path))
    check(True,"all new Python entry ASTs only")
    pins={str(path.relative_to(ROOT)):{"sha256":hashlib.sha256(path.read_bytes()).hexdigest(),"bytes":path.stat().st_size} for path in paths}
    args.out.write_text(json.dumps({"status":"PURE_SOURCE_MODELS_PASS","scope":"Synthetic data and source only; no compiler/validator/Wasm/Runtime/native/ROS/QEMU/network execution",
        "checks":len(checks),"labels":checks,"source_pins":pins,"actual_behavior":"NOT_EXECUTED"},indent=2)+"\n")
    return 0


if __name__=="__main__":raise SystemExit(main())
