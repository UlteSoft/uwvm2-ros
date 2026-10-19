#!/usr/bin/env python3
"""Compile the two independent SDK peers in the qualified serial ARM lane.

Preparation records actual original -MD dependencies and verbose compiler/link
commands. It starts no Guest or ROS context and reports PREPARED rather than a
native delivery PASS. Root alone runs this tool after source implementation.
"""
import argparse
import json
from pathlib import Path
import posixpath
import shlex
import sys
import traceback

sys.dont_write_bytecode = True
from lane import Lane, add_arguments, atomic_json
from protocol import require
from runtime import digest, require_arm, elf
from rebuild_affected import rooted
from run_local_parameter_events import fixture_closure
from gateway_qualification import architecture_binding, read_file_binding
from rebuild_affected import sdk_inventory, sdk_root_aliases

PEERS = {1: "tests/ros_sdk_relay/native_ros1_peer.cc", 2: "tests/ros_sdk_relay/native_ros2_peer.cc"}
SUITES = {"sdk": PEERS, "bounded-service": {1: "tests/ros_acceptance/ros1_service_transport.cc"},
          "metrics": {2: "tests/ros_acceptance/queue_ipc_metrics.cc"},
          "recovery": {1: "tests/ros_acceptance/recovery_peer_ros1.cc",2: "tests/ros_acceptance/recovery_peer_ros2.cc"},
          "graph-gateway": {2: "tests/ros_acceptance/observed_ros2_gateway.cc"},
          "events": {1:"tests/ros_unified_events/native_events_ros1.cc",2:"tests/ros_unified_events/native_events_ros2.cc"}}
# Only the instrumented Graph Gateway includes the complete production main.
# Actual B4 GCC13/QEMU compilation exceeded the unchanged2GiB actor ceiling by
# 24KiB. Collect the compiler's internal garbage more frequently instead of
# increasing that ceiling or changing generated code/optimization/assertions.
# GCC13 documents these parameters as having no effect on code generation:
# https://gcc.gnu.org/onlinedocs/gcc-13.3.0/gcc/Optimize-Options.html
GRAPH_GGC_FLAGS = ("--param=ggc-min-expand=10", "--param=ggc-min-heapsize=32768")


def qualify_peer(lane, prepared_path, version, suite="sdk"):
    """Reconstruct preparation from current physical source/SDK/raw artifacts.

    A report status alone is insufficient. This function re-reads full-MD,
    actual verbose recipe, object, explicit linked inputs, all SDK bytes and
    aliases, selected native ELF and compilation architecture evidence.
    """
    report, report_pin = lane.json(prepared_path)
    require(suite in SUITES and version in SUITES[suite] and report.get("kind") == "remaining-native-preparation"
            and report.get("suite") == suite and report.get("mode") == "prepare"
            and report.get("status") == "PREPARED" and report.get("source") == str(lane.source)
            and report.get("source_manifest_sha256") == lane.report["source_manifest_sha256"]
            and all(report.get(name) is True for name in ("source_preserved", "sdk_preserved", "inputs_preserved", "products_preserved")),
            "current complete native preparation required")
    architecture, architecture_pin = architecture_binding(report["architecture_evidence"]["path"], lane.pins)
    require(architecture_pin == report["architecture_evidence"], "native preparation architecture changed")
    # An internally valid old receipt cannot establish this lane's live leaf.
    # Lane produces both fields from the caller's current Docker StartedAt and
    # runtime.cgroup_incarnation (path/device/inode/kernel boot identifier).
    require(report.get("container_started_at") == lane.args.container_started_at
            and architecture.get("cgroup_incarnation_before") == lane.before,
            "native preparation differs from current container/cgroup incarnation")
    selected = [row for row in report["cases"] if row.get("version") == version]
    require(len(report["cases"]) == len(SUITES[suite]) and len(selected) == 1 and selected[0]["status"] == "PREPARED"
            and selected[0]["source"] == SUITES[suite][version], "exact native fixture inventory required")
    row = selected[0]; state, state_pin = lane.json(report["sdk_before"][str(version)]["path"])
    require(state_pin == report["sdk_before"][str(version)] and
            state == {"inventory": sdk_inventory(lane.sdks[version]), "aliases": sdk_root_aliases(lane.sdks[version])},
            "prepared full SDK inventory/aliases differ")
    database, database_pin = lane.json(row["compile_database"]["path"])
    require(database_pin == row["compile_database"], "native compile database changed")
    stdout = row["compile"]["stdout_pin"]; pin = lane.pin(stdout["path"])
    require(pin == stdout, "native verbose log changed")
    verbose = Path(stdout["path"]).read_text(); actual = recipe(version, database, verbose, suite)
    require(actual == row["compiler_recipe"], "native original compile recipe differs")
    # The report records physical /tmp ownership explicitly, avoiding guesses
    # based on GCC's nested source-path/object directory depth.
    temporary = Path(row["temporary"])
    dependencies, dependency_pin = fixture_closure(row["dependency_file"]["path"], lane.source,
        lane.sdks[version], temporary, actual)
    require(dependencies == row["dependencies"] and dependency_pin == row["dependency_file"], "native full-MD closure differs")
    for value in dependencies.values():
        pin = lane.pin(value["path"]); require(pin["sha256"] == value["sha256"] and pin["bytes"] == value["bytes"], "native dependency differs")
    for pin in (row["object"], row["link"]["file"], *row["link"]["scalar_sdk_inputs"].values(), row["source_pin"]):
        require(lane.pin(pin["path"]) == pin, "native object/link/source input changed")
    require(require_arm(row["product"]["snapshot"]["path"]) == row["product"]["snapshot"], "prepared native peer changed")
    lane.report.setdefault("native_qualification", {})[suite+":"+str(version)] = {"report": report_pin, "architecture": architecture_pin,
        "row": row, "full_sdk_inventory": state_pin}
    return row["product"]["snapshot"]["path"]


def cmake(version, suite="sdk"):
    """Closed single-target recipe, with Noetic's unwanted gtest targets off."""
    standard = 17 if version == 1 or suite in ("graph-gateway","events") else 20; label = "sdk-peer"
    lines = ["cmake_minimum_required(VERSION 3.16)", "project(sdk_peer LANGUAGES CXX)",
             f"set(CMAKE_CXX_STANDARD {standard})", "set(CMAKE_CXX_STANDARD_REQUIRED ON)",
             "set(CMAKE_CXX_EXTENSIONS ON)"]
    if suite=="graph-gateway":
        packages=("rclcpp", "rclcpp_action", "action_tutorials_interfaces", "std_msgs", "geometry_msgs", "sensor_msgs", "std_srvs", "rcl_interfaces", "rcl_action", "lifecycle_msgs", "rclcpp_lifecycle", "tf2", "tf2_ros", "tf2_msgs")
        lines += ["find_package(ament_cmake REQUIRED)"]+["find_package("+name+" REQUIRED)" for name in packages]
    elif suite=="events" and version==2:
        packages=("rclcpp","rclcpp_action","rcl","rcutils","rmw","std_msgs","std_srvs","action_tutorials_interfaces")
        lines += ["find_package(ament_cmake REQUIRED)"]+["find_package("+name+" REQUIRED)" for name in packages]
    elif version == 1:
        lines += ["set(CATKIN_ENABLE_TESTING OFF CACHE BOOL \"Fixture owns its one target\" FORCE)",
                  "find_package(catkin REQUIRED COMPONENTS roscpp std_msgs std_srvs xmlrpcpp"+(" actionlib actionlib_tutorials" if suite=="events" else "")+")"]
    else:
        lines += ["find_package(ament_cmake REQUIRED)"] + ["find_package("+name+" REQUIRED)" for name in
                   ("rclcpp", "rcl", "rcutils", "rmw", "std_msgs", "rosidl_typesupport_cpp", "fastrtps")]
    lines += [f"add_executable({label} /work/{SUITES[suite][version]})",
              f"target_include_directories({label} PRIVATE /work/src /work/third-parties/fast_io/include)",
              f"target_compile_options({label} PRIVATE -O1 -g0 -UNDEBUG -std=gnu++{standard} -MD -MF /tmp/build/CMakeFiles/{label}.dir/peer.d)"]
    if suite=="graph-gateway":
        lines += [f"target_compile_options({label} PRIVATE "+" ".join(GRAPH_GGC_FLAGS)+")",
                  f"ament_target_dependencies({label} "+" ".join(packages)+")", f"target_link_libraries({label} pthread ${{CMAKE_DL_LIBS}})"]
    elif suite=="events" and version==2:
        lines += [f"ament_target_dependencies({label} "+" ".join(packages)+")", f"target_link_libraries({label} pthread)"]
    elif version == 1:
        lines += [f"target_include_directories({label} PRIVATE ${{catkin_INCLUDE_DIRS}})",
                  f"target_link_libraries({label} ${{catkin_LIBRARIES}} pthread)"]
    else:
        lines += [f"ament_target_dependencies({label} rclcpp rcl rcutils rmw std_msgs rosidl_typesupport_cpp)",
                  f"target_link_libraries({label} fastrtps pthread)"]
    return "\n".join(lines)+"\n"


def recipe(version, database, verbose, suite="sdk"):
    """Require one actual compile of the selected source and its final -MF."""
    source = "/work/"+SUITES[suite][version]; compiler = "/usr/bin/aarch64-linux-gnu-g++-"+("9" if version == 1 else "13")
    require(len(database) == 1 and database[0]["file"] == source and database[0]["directory"] == "/tmp/build",
            "singleton exact fixture compile database required")
    expected = database[0].get("arguments") or shlex.split(database[0]["command"])
    def normalized(argv):
        clean = []; dep = []; index = 0
        while index < len(argv):
            token = argv[index]
            require(token not in ("-M", "-MM", "-MMD", "-E", "-S", "-fsyntax-only", "-MG", "-MP")
                    and not token.startswith(("@", "-Wp,")) and token != "-Xpreprocessor", "opaque/partial compiler recipe")
            if token in ("-MF", "-MT", "-MQ"):
                index += 1; require(index < len(argv), "missing dependency operand")
                if token == "-MF": dep.append(argv[index])
            elif token.startswith(("-MF", "-MT", "-MQ")):
                if token.startswith("-MF"): dep.append(token[3:])
            elif token != "-MD": clean.append(token)
            index += 1
        require(argv[0] == compiler and "-MD" in argv and argv.count("-c") == 1
                and argv[argv.index("-c")+1:] == [source] and argv.count("-o") == 1, "actual source/compiler/output differs")
        standards = [token for token in argv if token.startswith("-std=")]
        optimizations = [token for token in argv if token.startswith("-O")]
        debug = [token for token in argv if token.startswith("-g")]
        macros = [token for token in argv if token in ("-DNDEBUG", "-UNDEBUG")]
        require(standards[-1:] == ["-std=gnu++"+("17" if version == 1 or suite in ("graph-gateway","events") else "20")]
                and optimizations[-1:] == ["-O1"] and debug[-1:] == ["-g0"] and macros[-1:] == ["-UNDEBUG"],
                "effective CPP/optimization/assertion controls differ")
        if suite == "graph-gateway":
            require([token for token in argv if "ggc-min-expand" in token or "ggc-min-heapsize" in token] == list(GRAPH_GGC_FLAGS),
                    "Graph-only compiler heap collection controls differ")
        return clean, dep
    reference, _ = normalized(expected); found = []
    for number, line in enumerate(verbose.splitlines(), 1):
        if compiler not in line or source not in line: continue
        argv = shlex.split(line)
        if len(argv) > 3 and argv[:3] == ["cd", "/tmp/build", "&&"]: argv = argv[3:]
        if not argv or argv[0] != compiler or "-c" not in argv: continue
        clean, outputs = normalized(argv)
        require(clean == reference and outputs, "compile database differs from original verbose command")
        dep = posixpath.normpath(posixpath.join("/tmp/build", outputs[-1]))
        obj = posixpath.normpath(posixpath.join("/tmp/build", argv[argv.index("-o")+1]))
        require(dep.startswith("/tmp/build/CMakeFiles/sdk-peer.dir/") and dep.endswith(".d")
                and obj.startswith("/tmp/build/CMakeFiles/sdk-peer.dir/") and obj.endswith(".o"), "target output escape")
        found.append({"argv": argv, "working_directory": "/tmp/build", "verbose_line": number,
                      "effective_mf": dep, "compiled_object_guest_path": obj})
    require(len(found) == 1, "one actual verbose compilation required"); return found[0]


def main():
    parser = argparse.ArgumentParser(description=__doc__); add_arguments(parser, workers=False)
    parser.add_argument("--suite", choices=tuple(SUITES), default="sdk"); args = parser.parse_args()
    lane = None; status = "FAIL"
    try:
        lane = Lane(args, "remaining-native-preparation"); lane.report.update(mode="prepare", suite=args.suite)
        for version in SUITES[args.suite]:
            root = lane.root(version); temporary = lane.out/("ros"+str(version))/"tmp"; sdk = lane.sdks[version]
            project = temporary/"cmake"; project.mkdir(); (project/"CMakeLists.txt").write_text(cmake(version,args.suite))
            compiler = "/usr/bin/aarch64-linux-gnu-g++-"+("9" if version == 1 else "13")
            for label, logical in (("cmake-"+str(version), "/usr/bin/cmake"), ("gcc-"+str(version), compiler)):
                path = rooted(sdk, logical); lane.pin(path); lane.evidence.target(label, path)
            configured, configure_row = lane.start("configure-ros"+str(version), lane.native(version, ["/usr/bin/cmake",
                "-S", "/tmp/cmake", "-B", "/tmp/build", "-DCMAKE_CXX_COMPILER="+compiler,
                "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"]), seconds=180)
            lane.finish(configured, configure_row)
            compiled, compile_row = lane.start("compile-ros"+str(version), lane.native(version,
                ["/usr/bin/cmake", "--build", "/tmp/build", "--parallel", "1", "--verbose"]), seconds=600)
            lane.finish(compiled, compile_row)
            # Tool mappings must belong to the command which actually ran
            # that tool. Configuration and compilation have distinct retained
            # process trees; a different role's sample cannot qualify them.
            lane.evidence.check(["cmake-"+str(version)], configure_row["name"]+"/")
            lane.evidence.check(["gcc-"+str(version)], compile_row["name"]+"/")
            database_path = temporary/"build/compile_commands.json"; database, database_pin = lane.json(database_path)
            verbose = lane.read(compile_row); actual = recipe(version, database, verbose,args.suite)
            dependencies, dependency_pin = fixture_closure(temporary/actual["effective_mf"][5:], lane.source, sdk, temporary, actual)
            lane.pins.append(dependency_pin)
            for item in dependencies.values():
                pin = lane.pin(item["path"]); require(pin["sha256"] == item["sha256"], "native dependency changed")
            object_pin = lane.pin(temporary/actual["compiled_object_guest_path"][5:])
            link_path = temporary/"build/CMakeFiles/sdk-peer.dir/link.txt"; link_pin = lane.pin(link_path)
            link = shlex.split(link_path.read_text()); require(link[0] == compiler and link.count("-o") == 1,
                                                             "actual native link recipe invalid")
            require(posixpath.normpath(posixpath.join("/tmp/build", link[link.index("-o")+1])) == "/tmp/build/sdk-peer"
                    and [posixpath.normpath(posixpath.join("/tmp/build", token)) for token in link if token.endswith(".o")]
                    == [actual["compiled_object_guest_path"]], "actual linked object/output differs")
            matches = []
            for line in verbose.splitlines():
                if compiler not in line: continue
                argv = shlex.split(line)
                if argv[:3] == ["cd", "/tmp/build", "&&"]: argv = argv[3:]
                if argv == link: matches.append(line)
            require(len(matches) == 1, "one original verbose link required")
            linked = {}
            for token in link:
                if token.startswith(("/usr/", "/opt/", "/lib/")):
                    path = rooted(sdk, token); linked[token] = lane.pin(path)
            destination = lane.out/("native-ros"+str(version)); product = lane.snapshot(temporary/"build/sdk-peer", destination, "peer-"+str(version))
            lane.report["cases"].append({"version": version, "status": "PREPARED", "source": SUITES[args.suite][version], "temporary": str(temporary),
                "source_pin": lane.pin(lane.source/SUITES[args.suite][version]), "product": product, "compiler_recipe": actual,
                "compile_database": database_pin, "dependency_file": dependency_pin, "dependencies": dependencies,
                "object": object_pin, "link": {"argv": link, "file": link_pin, "scalar_sdk_inputs": linked},
                "configure": configure_row, "compile": compile_row,
                "qualification_scope": "Actual compiler/CMake maps, original full-MD and full SDK inventory; no separate ld/DT_NEEDED graph attestation"})
            lane.save()
        status = "PASS"
    except BaseException as error:
        if lane is not None: lane.report.update(error=repr(error), traceback=traceback.format_exc())
        else: raise
    finally:
        if lane is not None:
            terminal = lane.close(status)
            if terminal == "PASS": lane.report["status"] = "PREPARED"; lane.save()
            status = "PREPARED" if terminal == "PASS" else "FAIL"
    return 0 if status == "PREPARED" else 1


if __name__ == "__main__": raise SystemExit(main())
