#!/usr/bin/env python3
"""Finite Unitree complete-source/emission/overlay workload; no language compiler.

Run only under the unchanged shared Linux resource owner. Download only exact
locked Unitree API/GO Message and license leaves, verify bytes/SHA256, invoke the
existing original preparation and actual shared-emitter connection, then check
the fresh canonical source overlays. Never execute original robot programs.
"""
from pathlib import Path
import argparse
import hashlib
import json
import subprocess
import sys
import urllib.request


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--snapshot",type=Path,required=True)
    args=parser.parse_args()
    source=args.snapshot.resolve(strict=True)
    unitree=source/"sdk/origin/robots/unitree"
    work=Path.cwd()
    steps=[]
    result={"status":"FAILED_RETAINED","compiler_runs":0,"Guest_runs":0,
            "native_ROS_runs":0,"default_authority":[],"steps":steps}
    def save():
        (work/"result.json").write_text(json.dumps(result,indent=2)+"\n")
    def run(name,argv):
        stdout=work/(name+".stdout");stderr=work/(name+".stderr")
        with stdout.open("xb") as out,stderr.open("xb") as err:
            completed=subprocess.run([str(a) for a in argv],stdout=out,stderr=err,check=False)
        steps.append({"name":name,"argv":[str(a) for a in argv],"returncode":completed.returncode,
                      "stdout_sha256":digest(stdout),"stderr_sha256":digest(stderr)})
        save()
        if completed.returncode:
            raise ValueError(name+" failed; retained original stdout/stderr")
    try:
        locked=json.loads((unitree/"upstream-lock.json").read_text())
        profiles={name:json.loads((unitree/"profiles"/(name+".json")).read_text()) for name in ("unitree_api","unitree_go")}
        repository="unitreerobotics/unitree_ros2"
        commit="668d1ec5a05d1c38d3306bdca7d59f2ba3581a88"
        selected={"LICENSE"}
        for profile in profiles.values():
            if profile["repository"]!=repository or profile["commit"]!=commit or profile["default_authority"]!=[]:
                raise ValueError("original closed Unitree profile required")
            selected.update(row["path"] for row in profile["definitions"])
        if len(selected)!=33:
            raise ValueError("exact original32 complete definitions and license required")
        facts={}
        for row in locked["files"]:
            if row["repository"]==repository and row["commit"]==commit and row["path"] in selected:
                if row["path"] in facts:
                    raise ValueError("duplicate locked original input")
                facts[row["path"]]=row
        if set(facts)!=selected:
            raise ValueError("every original complete source must be locked")
        upstream=work/"original-unitree-ros2";upstream.mkdir()
        acquisition=[]
        for name in sorted(facts):
            row=facts[name]
            path=Path(name)
            if path.is_absolute() or ".." in path.parts or "\\" in name or not 0<=row["bytes"]<=65536:
                raise ValueError("closed bounded original source leaf required")
            url="https://raw.githubusercontent.com/"+repository+"/"+commit+"/"+name
            if row["source_url"]!=url:
                raise ValueError("exact official pinned source URL required")
            request=urllib.request.Request(url,headers={"User-Agent":"uwvm-unitree-complete-source"})
            with urllib.request.urlopen(request,timeout=10) as response:
                if response.geturl()!=url:
                    raise ValueError("original pinned source redirect refused")
                body=response.read(row["bytes"]+1)
            if len(body)!=row["bytes"] or hashlib.sha256(body).hexdigest()!=row["sha256"]:
                raise ValueError("original source size/SHA mismatch: "+name)
            destination=upstream/path;destination.parent.mkdir(parents=True,exist_ok=True)
            with destination.open("xb") as stream:stream.write(body)
            acquisition.append({"path":name,"url":url,"bytes":len(body),"sha256":row["sha256"]})
        (work/"acquisition.json").write_text(json.dumps({"status":"SOURCE_PINS_ONLY","files":acquisition,
            "original_program_execution":False,"native_identity_acquired":False},indent=2)+"\n")
        result["locked_source_files"]=len(acquisition)
        python=[sys.executable,"-I","-S","-B"]
        pair={
            "parent":source/"sdk/origin/tools/custom_types.py",
            "bounded":source/"sdk/origin/custom-types/bounded_profile.py",
            "adapter":source/"sdk/origin/robots/industrial/vendor_profile.py",
        }
        helper={
            "records":source/"sdk/origin/tools/native_languages/records.py",
            "records-native":source/"sdk/origin/tools/native_languages/records_native.py",
            "documentation":source/"sdk/origin/tools/native_languages/documentation.py",
        }
        hashes={name:digest(path) for name,path in {**pair,**helper}.items()}
        for family in profiles:
            run(family+"-materialize",[*python,unitree/"materialize_catalog.py","--upstream-root",upstream,
                "--profile",unitree/"profiles"/(family+".json"),"--lock",unitree/"upstream-lock.json",
                "--catalog-format","uwvm-ros-vendor-complete-bounded-v1","--out",work/(family+"-preparation")])
            argv=[*python,source/"sdk/origin/robots/industrial/prepare.py","--source-root",source,
                "--catalog",work/(family+"-preparation/catalog.preparation.json"),"--out",work/(family+"-complete"),"--mode","prepare"]
            for name in pair:argv.extend(["--"+name+"-sha256",hashes[name]])
            run(family+"-complete",argv)
        emissions={}
        for language in ("nim","go","haskell","d","swift"):
            destination=work/(language+"-api-emission")
            argv=[*python,unitree/"emit_language_records.py","--source-root",source,
                "--catalog",work/"unitree_api-complete/input/catalog.json","--reference-family",
                source/"sdk/origin/custom-types/unitree-api-complete","--language",language,"--out",destination]
            for name in hashes:argv.extend(["--"+name+"-sha256",hashes[name]])
            run(language+"-emit",argv)
            emissions[language]=destination
        go_emission=work/"nim-go-emission"
        argv=[*python,unitree/"emit_language_records.py","--source-root",source,
            "--catalog",work/"unitree_go-complete/input/catalog.json","--reference-family",
            source/"sdk/origin/custom-types/unitree-go-complete","--language","nim","--out",go_emission]
        for name in hashes:argv.extend(["--"+name+"-sha256",hashes[name]])
        run("nim-go-emit",argv)
        for language in ("nim","go","zig","haskell","d","swift"):
            argv=[*python,unitree/"stage_language_sdk.py","--unitree",unitree,"--reference-api",
                source/"sdk/origin/custom-types/unitree-api-complete","--language",language,
                "--out",work/(language+"-source-overlay")]
            if language!="zig":argv.extend(["--emission",emissions[language]])
            if language=="nim":argv.extend(["--go-emission",go_emission,"--reference-go",
                source/"sdk/origin/custom-types/unitree-go-complete"])
            run(language+"-stage",argv)
        argv=[*python,source/"test/0073.ros-tests/support/source/ros_platform05/unitree/language_overlay_contract.py",
            "--unitree",unitree,"--reference-api",source/"sdk/origin/custom-types/unitree-api-complete",
            "--work",work/"overlay-contract"]
        for language,path in emissions.items():argv.extend(["--emission",language+"="+str(path)])
        run("overlay-contract",argv)
        result.update(status="PASS_SOURCE_PREPARATION_AND_OVERLAY_ONLY",language_emissions=5,
            whole_GO_reader_emissions=1,source_overlays=6,negative_contracts=10,
            whole_sdk_publication=False,whole_official_program_acceptance=False)
        save();print(json.dumps(result,sort_keys=True))
    except BaseException as error:
        result["failure"]=str(error);save();raise


if __name__=="__main__":main()
