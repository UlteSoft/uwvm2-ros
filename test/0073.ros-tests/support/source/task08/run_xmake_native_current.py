"""Actual xmake native Python components and original Kortex descriptor program."""

import hashlib, importlib.util, json, os, shutil, subprocess, sys
from pathlib import Path

if os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
    raise RuntimeError("common finite owner required")
inputs = Path(__file__).parent
work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
sdk = work / "sdk"
shutil.copytree(inputs / "sdk", sdk)
agents = Path("/home/macromodel/Documents/uwvm-test-261018/agents/08")
compiler = Path(
    "/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/bin/clang++"
)
cmake = Path("/usr/bin/cmake")
xmake = Path("/home/macromodel/.local/bin/xmake")


def sha(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


environment = dict(
    os.environ,
    XMAKE_STATS="false",
    XMAKE_SKIP_HISTORY="y",
    XMAKE_PROGRAM_DIR="/home/macromodel/.local/share/xmake",
    UWVM_PLATFORM08_PYTHON=sys.executable,
    UWVM_PLATFORM08_PYTHON_SHA256=sha(sys.executable),
    UWVM_PLATFORM08_CMAKE=str(cmake),
    UWVM_PLATFORM08_CMAKE_SHA256=sha(cmake),
    UWVM_PLATFORM08_CXX=str(compiler),
    UWVM_PLATFORM08_CXX_SHA256=sha(compiler),
    UWVM_KORTEX_PREFIX=str(
        agents / "20261021-kortex-acquire-r1/work/kortex-2.8.0/original"
    ),
    UWVM_KORTEX_ARCHIVE=str(
        agents / "20261021-kortex-acquire-r1/work/kortex-2.8.0/original.zip"
    ),
    UWVM_FAST_IO_INCLUDE=str(
        agents / "20261021-contracts-r6/inputs/source/third-parties/fast_io/include"
    ),
    LD_LIBRARY_PATH="/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib/x86_64-unknown-linux-gnu:/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu",
    OPENBLAS_NUM_THREADS="1",
    OMP_NUM_THREADS="1",
)
rows = []
builds = []


def run(name, command, cwd=sdk / "example"):
    folder = work / name
    folder.mkdir()
    with (folder / "stdout").open("xb") as out, (folder / "stderr").open("xb") as err:
        result = subprocess.run(
            list(map(str, command)),
            cwd=cwd,
            env=environment,
            stdin=subprocess.DEVNULL,
            stdout=out,
            stderr=err,
            timeout=120,
            check=False,
        )
    row = dict(name=name, argv=list(map(str, command)), exit_code=result.returncode)
    rows.append(row)
    print(json.dumps(row), flush=True)
    return result.returncode == 0


for platform in ("kinova", "hello_robot"):
    name = platform + "-python"
    if not run(
        name + "-configure",
        [
            xmake,
            "f",
            "-y",
            "--sdk-example-platform=" + platform,
            "--sdk-example-language=py",
            "--sdk-example=native-peer",
        ],
    ):
        continue
    output = work / (name + "-product")
    if not run(
        name + "-build", [xmake, "sdk-example-build", "--output=" + str(output)]
    ):
        continue
    receipt = json.loads((output / "build-receipt.json").read_text())
    assert (
        receipt["status"] == "BUILT_NOT_EXECUTED" and receipt["default_authority"] == []
    )
    for item in receipt["artifacts"]:
        assert (
            item["kind"] == "native-component" and sha(item["path"]) == item["sha256"]
        )
    support = []
    for file in sorted(
        (sdk / "example/platform" / platform / "py/native_support").rglob("*.py")
    ):
        body = file.read_bytes()
        compile(body, str(file), "exec")
        target = (
            output
            / "native_support"
            / file.relative_to(
                sdk / "example/platform" / platform / "py/native_support"
            )
        )
        assert target.read_bytes() == body
        support.append(dict(path=str(target), sha256=sha(target)))
    builds.append(
        dict(
            platform=platform,
            language="py",
            receipt=str(output / "build-receipt.json"),
            complete_support=support,
            native_ros_runtime_imported=False,
        )
    )
platform = "kinova"
if run(
    "kortex-configure",
    [
        xmake,
        "f",
        "-y",
        "--sdk-example-platform=kinova",
        "--sdk-example-language=cpp",
        "--sdk-example=original-descriptors",
    ],
):
    output = work / "kortex-product"
    if run("kortex-build", [xmake, "sdk-example-build", "--output=" + str(output)]):
        receipt = json.loads((output / "build-receipt.json").read_text())
        artifact = receipt["artifacts"][0]
        assert (
            artifact["kind"] == "native-component"
            and sha(artifact["path"]) == artifact["sha256"]
        )
        assert receipt["default_authority"] == []
        assert run(
            "original-descriptor-export",
            [artifact["path"], work / "original-descriptors.pb"],
            work,
        )
        descriptors = work / "original-descriptors.pb"
        assert descriptors.stat().st_size == 101775
        builds.append(
            dict(
                platform="kinova",
                language="cpp",
                receipt=str(output / "build-receipt.json"),
                original_descriptors=dict(
                    path=str(descriptors),
                    bytes=descriptors.stat().st_size,
                    sha256=sha(descriptors),
                ),
                native_sdk_rpcs=0,
            )
        )
pins = []
for file in sorted((inputs / "sdk").rglob("*")):
    if file.is_file():
        pins.append(
            dict(
                path=str(file.relative_to(inputs)),
                bytes=file.stat().st_size,
                sha256=sha(file),
            )
        )
report = dict(
    schema="uwvm-task08-xmake-native-current-v1",
    rows=rows,
    builds=builds,
    input_hashes=pins,
    actual_native_python_component_builds=sum(
        row["language"] == "py" for row in builds
    ),
    actual_native_cpp_component_builds=sum(row["language"] == "cpp" for row in builds),
    native_sdk_rpcs=0,
    native_ros_cases=0,
    robot_cases=0,
    wasm_guest_cases=0,
    default_authority=[],
)
(work / "native-examples.json").write_text(
    json.dumps(report, indent=2, sort_keys=True) + "\n"
)
if any(row["exit_code"] for row in rows) or len(builds) != 3:
    raise SystemExit(1)
