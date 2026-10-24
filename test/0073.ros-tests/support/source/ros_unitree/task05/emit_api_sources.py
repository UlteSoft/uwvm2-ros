#!/usr/bin/env python3
"""Actual five-language complete API emission; finite Linux-owner source scope."""
from pathlib import Path
import argparse
import hashlib
import json
import subprocess
import sys

def fact(path):
    b=path.read_bytes();return {"bytes":len(b),"sha256":hashlib.sha256(b).hexdigest()}

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--snapshot",type=Path,required=True)
    parser.add_argument("--catalog",type=Path,required=True)
    args=parser.parse_args();source=args.snapshot.resolve(strict=True)
    unitree=source/"sdk/origin/robots/unitree";work=Path.cwd()
    roles={"parent":"sdk/origin/tools/custom_types.py","bounded":"sdk/origin/custom-types/bounded_profile.py",
        "adapter":"sdk/origin/robots/industrial/vendor_profile.py","records":"sdk/origin/tools/native_languages/records.py",
        "records-native":"sdk/origin/tools/native_languages/records_native.py",
        "documentation":"sdk/origin/tools/native_languages/documentation.py"}
    inputs={name:fact(source/path) for name,path in roles.items()};rows=[]
    for language in ("nim","go","haskell","d","swift"):
        argv=[sys.executable,"-I","-S","-B",str(unitree/"emit_language_records.py"),"--source-root",str(source),
            "--catalog",str(args.catalog),"--reference-family",str(source/"sdk/origin/custom-types/unitree-api-complete"),
            "--language",language,"--out",str(work/language)]
        for name,pin in inputs.items():argv.extend(["--"+name+"-sha256",pin["sha256"]])
        with (work/(language+".stdout")).open("xb") as out,(work/(language+".stderr")).open("xb") as err:
            completed=subprocess.run(argv,stdout=out,stderr=err,check=False)
        if completed.returncode:raise ValueError(language+" actual emission failed; stderr retained")
        manifest=json.loads((work/language/"emission.json").read_text())
        for product in manifest["products"]:
            if fact(work/language/product["path"])!={k:product[k] for k in ("bytes","sha256")}:
                raise ValueError("actual emitted output pin differs")
        rows.append({"language":language,"record_count":8,"manifest":fact(work/language/"emission.json")})
    for name,path in roles.items():
        if fact(source/path)!=inputs[name]:raise ValueError("actual emitter source changed")
    result={"status":"PASS_FIVE_COMPLETE_API_SOURCE_EMISSIONS_ONLY","emissions":rows,
        "source_inputs":inputs,"compiler_runs":0,"Guest_runs":0,"native_ROS_runs":0,"default_authority":[]}
    (work/"result.json").write_text(json.dumps(result,indent=2)+"\n")
    print(json.dumps({"status":result["status"],"language_emissions":len(rows)},sort_keys=True))

if __name__=="__main__":main()
