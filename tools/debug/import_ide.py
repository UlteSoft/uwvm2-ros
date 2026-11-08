#!/usr/bin/env python3
"""Import host-owned UWVM debug configurations and run local/SSH stdio adapters.

No permissions are inferred, no debugger capability is copied, and no TCP DAP
listener is created. Generated configuration is not an IDE qualification result.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import stat
import subprocess
import sys
import tempfile
import xml.etree.ElementTree as ET

FORMAT = "uwvm-debug-project-v1"
LEVELS = ("source", "wasm", "native")
SESSION = re.compile(r"[0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12}")
HERE = Path(__file__).resolve().parent


def absolute(value, label, *, remote=False):
    if not isinstance(value, str) or not value or any(ord(c) < 32 for c in value):
        raise ValueError(f"{label} must be an absolute path without control characters")
    if not (value.startswith("/") if remote else Path(value).is_absolute()):
        raise ValueError(f"{label} must be absolute")
    return value


def text(value, label):
    if not isinstance(value, str) or not value or any(ord(c) < 32 for c in value):
        raise ValueError(f"{label} must be nonempty text without control characters")
    return value


PERMISSIONS = ("full", "read-only", "limited", "none")


def selected_permissions(profile):
    args = profile.get("runtime", {}).get("arguments", [])
    stop = next((i for i, a in enumerate(args) if a in ("--run", "--")), len(args))
    options = args[:stop]
    positions = [i for i, a in enumerate(options) if a == "--debug-permissions"]
    if len(positions) > 1 or positions and positions[0] + 1 >= len(options):
        raise ValueError("exactly one valid --debug-permissions value is required")
    cli = options[positions[0]+1] if positions else None
    value = profile.get("debugPermissions", cli or ("full" if any(a in ("-Rdbg", "--runtime-debug") for a in options) else "limited"))
    if value not in PERMISSIONS or cli is not None and cli != value:
        raise ValueError("debugPermissions must match the explicit VM launch option")
    return value


def permission_arguments(profile):
    args = list(profile["runtime"]["arguments"])
    value = selected_permissions(profile)
    stop = next((i for i, a in enumerate(args) if a in ("--run", "--")), len(args))
    return args if "--debug-permissions" in args[:stop] else ["--debug-permissions", value, *args]


def validate(profile):
    if not isinstance(profile, dict) or profile.get("format") != FORMAT:
        raise ValueError(f"Expected {FORMAT}")
    selected_permissions(profile)
    text(profile.get("name"), "name")
    absolute(profile.get("workspace"), "workspace")
    transport = profile.get("transport")
    if not isinstance(transport, dict) or transport.get("kind") not in ("local", "ssh"):
        raise ValueError("transport.kind must be local or ssh")
    remote = transport["kind"] == "ssh"
    python = transport.get("python", "/usr/bin/python3")
    absolute(python, "transport.python", remote=remote)
    if remote:
        host = transport.get("host")
        if not isinstance(host, str) or not re.fullmatch(r"[A-Za-z0-9_][A-Za-z0-9_.@:%\[\]-]*", host):
            raise ValueError("SSH host must be one explicit host/config alias, not shell/options")
        absolute(transport.get("debugDirectory"), "remote debugDirectory", remote=True)
        if "port" in transport and (type(transport["port"]) is not int or not 1 <= transport["port"] <= 65535):
            raise ValueError("SSH port must be 1 through 65535")
        for key in ("identityFile", "knownHostsFile", "ssh"):
            if key in transport:
                absolute(transport[key], f"transport.{key}")
        # Host-key verification and authentication policy are not profile overrides.
        allowed = {"kind", "python", "host", "debugDirectory", "port", "identityFile", "knownHostsFile", "ssh"}
        if set(transport) - allowed:
            raise ValueError("Unsupported SSH setting; no arbitrary ssh options or command strings")
    elif set(transport) - {"kind", "python"}:
        raise ValueError("Unsupported local transport setting")
    broker = profile.get("broker")
    if not isinstance(broker, dict) or set(broker) != {"socketDir"}:
        raise ValueError("Select the host-owned Unix broker socketDir; never include capabilities")
    absolute(broker["socketDir"], "broker.socketDir", remote=remote)
    if profile.get("stepLevel", "source") not in LEVELS:
        raise ValueError("stepLevel must be source, wasm or native")
    module = profile.get("moduleId", 0)
    if type(module) is not int or not 0 <= module < 1 << 32:
        raise ValueError("moduleId must fit u32")
    mapping = profile.get("sourceMap", {})
    if not isinstance(mapping, dict):
        raise ValueError("sourceMap must be an object")
    # Share the adapter's exact normalization, ambiguity and reverse-map policy.
    from dap_adapter import compile_source_map, startup_timeout_seconds
    compile_source_map(mapping)
    if "startupTimeoutSeconds" in profile:
        startup_timeout_seconds(profile["startupTimeoutSeconds"])
    if "runtime" in profile:
        runtime = profile["runtime"]
        if not isinstance(runtime, dict) or set(runtime) != {"executable", "arguments"}:
            raise ValueError("runtime requires only executable and exact argument array")
        absolute(runtime["executable"], "runtime.executable", remote=remote)
        if not isinstance(runtime["arguments"], list) or any(not isinstance(x, str) or "\0" in x for x in runtime["arguments"]):
            raise ValueError("runtime.arguments must be an exact string array")
        if not runtime["arguments"]:
            raise ValueError("Provide the complete authorized VM argument vector, including debug mode and --run")
    allowed = {"format", "name", "workspace", "transport", "broker", "runtime", "moduleId", "stepLevel", "sourceMap", "language", "startupTimeoutSeconds", "debugPermissions"}
    if set(profile) - allowed:
        raise ValueError("Unknown profile property")
    if "language" in profile:
        text(profile["language"], "language")
    return profile


def load_profile(raw):
    path = Path(absolute(str(raw), "profile"))
    flags = os.O_RDONLY | getattr(os, "O_CLOEXEC", 0) | getattr(os, "O_NOFOLLOW", 0)
    descriptor = os.open(path, flags)
    try:
        info = os.fstat(descriptor)
        if not stat.S_ISREG(info.st_mode):
            raise ValueError("Profile must be a regular file")
        if hasattr(os, "geteuid") and (info.st_uid != os.geteuid() or info.st_mode & 0o022):
            raise ValueError("Profile must be owned by the current host user and not group/world writable")
        with os.fdopen(descriptor, encoding="utf-8", closefd=False) as stream:
            profile = json.load(stream)
    finally:
        os.close(descriptor)
    return validate(profile)


def ssh_prefix(transport):
    result = [transport.get("ssh", "/usr/bin/ssh"), "-T", "-o", "BatchMode=yes",
              "-o", "StrictHostKeyChecking=yes", "-o", "ForwardAgent=no",
              "-o", "ClearAllForwardings=yes"]
    if "port" in transport:
        result += ["-p", str(transport["port"])]
    if "identityFile" in transport:
        result += ["-o", "IdentitiesOnly=yes", "-i", transport["identityFile"]]
    if "knownHostsFile" in transport:
        result += ["-o", "UserKnownHostsFile=" + transport["knownHostsFile"]]
    return result + ["--", transport["host"]]


def adapter_command(profile, source_session=None):
    profile = validate(profile)
    if source_session is not None and SESSION.fullmatch(source_session) is None:
        raise ValueError("Invalid host IDE source session")
    transport = profile["transport"]
    python = transport.get("python", "/usr/bin/python3")
    if transport["kind"] == "local":
        return [python, str(HERE / "dap_adapter.py")]
    remote = [python, transport["debugDirectory"].rstrip("/") + "/dap_adapter.py"]
    if source_session is not None:
        remote = ["env", "UWVM_DAP_SOURCE_SESSION=" + source_session, *remote]
    # OpenSSH executes its remote command through the login shell. Quote every
    # individual argument; JSON serialization is not shell quoting.
    return [*ssh_prefix(transport), "exec " + shlex.join(remote)]


def broker_command(profile):
    profile = validate(profile)
    runtime = profile.get("runtime")
    if runtime is None:
        raise ValueError("This attach-only profile has no runtime launch authority")
    transport = profile["transport"]
    if selected_permissions(profile) == "none":
        command = [runtime["executable"], *permission_arguments(profile)]
        return command if transport["kind"] == "local" else [*ssh_prefix(transport), "exec " + shlex.join(command)]
    directory = profile["broker"]["socketDir"]
    python = transport.get("python", "/usr/bin/python3")
    if transport["kind"] == "local":
        Path(directory).mkdir(mode=0o700, parents=False, exist_ok=True)
        # The existing broker performs ownership, symlink, mode and guest-preopen checks.
        server = HERE / ("secure_server_macos.py" if sys.platform == "darwin" else "secure_server.py")
        if profile.get("startupTimeoutSeconds") is not None and sys.platform != "linux":
            raise ValueError("startupTimeoutSeconds currently requires the Linux broker")
        return [python, str(server), "serve", "--uwvm", runtime["executable"],
                "--socket-dir", directory,
                *(["--startup-timeout", str(profile["startupTimeoutSeconds"])] if profile.get("startupTimeoutSeconds") is not None else []),
                "--", *permission_arguments(profile)]
    server = transport["debugDirectory"].rstrip("/") + "/secure_server.py"
    remote = [python, server, "serve", "--uwvm", runtime["executable"],
              "--socket-dir", directory,
                *(["--startup-timeout", str(profile["startupTimeoutSeconds"])] if profile.get("startupTimeoutSeconds") is not None else []),
                "--", *permission_arguments(profile)]
    command = "umask 077; mkdir -p -- " + shlex.quote(directory) + " && exec " + shlex.join(remote)
    return [*ssh_prefix(transport), command]


def write_generated(path, content, replace):
    if path.exists() or path.is_symlink():
        info = path.lstat()
        if not stat.S_ISREG(info.st_mode) or (hasattr(os, "geteuid") and info.st_uid != os.geteuid()):
            raise ValueError(f"Refusing non-owned regular output: {path}")
        if path.read_bytes() == content:
            return
        if not replace:
            raise ValueError(f"Output differs: {path}; use --replace for this generated directory")
    descriptor, temporary = tempfile.mkstemp(prefix=".uwvm-import-", dir=path.parent)
    try:
        with os.fdopen(descriptor, "wb") as stream:
            stream.write(content)
        os.replace(temporary, path)
    finally:
        if os.path.exists(temporary):
            os.unlink(temporary)


def json_bytes(value):
    return (json.dumps(value, ensure_ascii=False, indent=2) + "\n").encode()



def generated_directory(path):
    path.mkdir(mode=0o700, exist_ok=True)
    info = path.lstat()
    if not stat.S_ISDIR(info.st_mode) or (hasattr(os, "geteuid") and
            (info.st_uid != os.geteuid() or info.st_mode & 0o077)):
        raise ValueError(f"Generated directory must be real and owner-only: {path}")
    return path


def xml_bytes(element):
    ET.indent(element, space="  ")
    return ET.tostring(element, encoding="utf-8", xml_declaration=True) + b"\n"


def generate_java_clients(profile, output, profile_path, replace):
    """Emit actual LSP4IJ run configurations and NetBeans bridge inputs."""
    project = generated_directory(output / "jetbrains")
    idea = generated_directory(project / ".idea")
    configurations = generated_directory(idea / "runConfigurations")
    netbeans = generated_directory(output / "netbeans")
    levels = {}
    for level in (() if selected_permissions(profile) == "none" else LEVELS):
        name = profile["name"] + " (" + level + ")"
        attach = {**profile["broker"], "moduleId": profile.get("moduleId", 0),
                  "stepLevel": level, "debugPermissions": selected_permissions(profile), "sourceMap": profile.get("sourceMap", {})}
        if profile.get("startupTimeoutSeconds") is not None:
            attach["startupTimeoutSeconds"] = profile["startupTimeoutSeconds"]
        component = ET.Element("component", {"name": "ProjectRunConfigurationManager"})
        configuration = ET.SubElement(component, "configuration",
            {"default": "false", "name": name, "type": "DAPConfiguration"})
        java_attach = {**attach, "uwvmPython": sys.executable,
                       "uwvmImporter": str(HERE / "import_ide.py"), "uwvmProfile": str(profile_path)}
        for key, value in (("attachConfiguration", json.dumps(java_attach, ensure_ascii=False)),
                           ("debugMode", "ATTACH"), ("serverId", "uwvm"), ("serverName", "UWVM"),
                           ("workingDirectory", profile["workspace"])):
            ET.SubElement(configuration, "option", {"name": key, "value": value})
        mappings = ET.SubElement(ET.SubElement(configuration, "option", {"name": "serverMappings"}), "list")
        mapping = ET.SubElement(mappings, "ServerMappingSettings")
        patterns = ET.SubElement(ET.SubElement(mapping, "option", {"name": "fileNamePatterns"}), "list")
        ET.SubElement(patterns, "option", {"value": "*"})
        file_type = ET.SubElement(ET.SubElement(mapping, "fileType"), "FileTypeSettings")
        patterns = ET.SubElement(file_type, "option", {"name": "patterns"})
        ET.SubElement(patterns, "option", {"value": "*"})
        ET.SubElement(configuration, "envs")
        ET.SubElement(configuration, "method", {"v": "2"})
        write_generated(configurations / (level + ".xml"), xml_bytes(component), replace)
        settings = {"format": "uwvm-netbeans-attach-v1", "name": name,
                    "python": sys.executable, "importer": str(HERE / "import_ide.py"),
                    "profile": str(profile_path), "attach": attach}
        target = netbeans / (level + ".json")
        write_generated(target, json_bytes(settings), replace)
        levels[level] = str(target)
    modules = ET.Element("project", {"version": "4"})
    members = ET.SubElement(ET.SubElement(modules, "component", {"name": "ProjectModuleManager"}), "modules")
    ET.SubElement(members, "module", {"fileurl": "file://$PROJECT_DIR$/uwvm.iml", "filepath": "$PROJECT_DIR$/uwvm.iml"})
    write_generated(idea / "modules.xml", xml_bytes(modules), replace)
    module = ET.Element("module", {"type": "EMPTY_MODULE", "version": "4"})
    roots = ET.SubElement(module, "component", {"name": "NewModuleRootManager"})
    ET.SubElement(roots, "content", {"url": Path(profile["workspace"]).as_uri()})
    ET.SubElement(roots, "orderEntry", {"type": "sourceFolder", "forTests": "false"})
    write_generated(project / "uwvm.iml", xml_bytes(module), replace)
    return str(project), levels


def generate(profile, output, nvim_dap=None, replace=False):
    validate(profile)
    output = Path(absolute(str(output), "output"))
    output.mkdir(mode=0o700, parents=False, exist_ok=True)
    info = output.lstat()
    if not stat.S_ISDIR(info.st_mode) or (hasattr(os, "geteuid") and (info.st_uid != os.geteuid() or info.st_mode & 0o077)):
        raise ValueError("Generated configuration directory must be a real owner-only directory")
    if "runtime" in profile and profile["transport"]["kind"] == "local":
        from secure_server import reject_guest_preopen_overlap
        reject_guest_preopen_overlap(output, profile["runtime"]["arguments"])
    if nvim_dap:
        absolute(nvim_dap, "nvim-dap")
        if not (Path(nvim_dap) / "lua/dap.lua").is_file():
            raise ValueError("Select an existing genuine nvim-dap plugin")
    profile_path = output / "profile.json"
    profile = {**profile, "debugPermissions": selected_permissions(profile)}
    write_generated(profile_path, json_bytes(profile), replace)
    configurations = []
    for level in (() if selected_permissions(profile) == "none" else LEVELS):
        configuration = {"type": "uwvm-llvm-full", "request": "attach", "name": profile["name"] + " (" + level + ")",
                         **profile["broker"], "moduleId": profile.get("moduleId", 0), "stepLevel": level, "debugPermissions": selected_permissions(profile),
                         "sourceMap": profile.get("sourceMap", {}), "pythonPath": sys.executable,
                         "transportProfile": str(profile_path)}
        if profile.get("startupTimeoutSeconds") is not None:
            configuration["startupTimeoutSeconds"] = profile["startupTimeoutSeconds"]
        if "runtime" in profile:
            configuration["preLaunchTask"] = "uwvm: host-owned broker"
        configurations.append(configuration)
    workspace = {"folders": [{"path": profile["workspace"]}], "launch": {"version": "0.2.0", "configurations": configurations}}
    # A watch task still needs a schema-valid file/message pattern. This
    # non-matching pattern emits no diagnostics; only readiness markers matter.
    if "runtime" in profile:
        workspace["tasks"] = {"version": "2.0.0", "tasks": [{
            "label": "uwvm: host-owned broker", "type": "process", "command": sys.executable,
            "args": [str(HERE / "import_ide.py"), "broker", "--profile", str(profile_path)],
            "isBackground": True, "problemMatcher": [{
                "owner": "uwvm", "pattern": [{"regexp": "^(?!)()()$", "kind": "file", "file": 1, "message": 2}],
                "background": {"activeOnStart": True, "beginsPattern": "^debug server:", "endsPattern": "^client:"}}],
            "presentation": {"reveal": "always", "panel": "dedicated"}}]}
    if selected_permissions(profile) == "none" and "tasks" in workspace:
        task = workspace["tasks"]["tasks"][0]
        task["label"] = "uwvm: run without debugger"
        task["isBackground"] = False
        task["problemMatcher"] = []
    workspace_path = output / "uwvm.code-workspace"
    write_generated(workspace_path, json_bytes(workspace), replace)
    generic = {"adapter": {"command": sys.executable, "args": [str(HERE / "import_ide.py"), "adapter", "--profile", str(profile_path)]},
               "attach": [{key: value for key, value in config.items() if key not in ("type", "pythonPath", "preLaunchTask", "transportProfile")}
                          for config in configurations],
               "qualification": "Configuration only; requires a real DAP client/plugin and acceptance run"}
    write_generated(output / "dap.json", json_bytes(generic), replace)
    nvim_path = output / "neovim.lua"
    # Lua receives paths via JSON decoding of a quoted JSON literal, never executable interpolated text.
    def lua_string(value):
        return "vim.json.decode(" + json.dumps(json.dumps(str(value))) + ")"
    lua = ["local profile_path = " + lua_string(profile_path)]
    if nvim_dap:
        absolute(nvim_dap, "nvim-dap")
        if not (Path(nvim_dap) / "lua/dap.lua").is_file():
            raise ValueError("Select an existing genuine nvim-dap plugin")
        lua.append("vim.opt.runtimepath:append(" + lua_string(nvim_dap) + ")")
    lua += ["local ui = dofile(" + lua_string(HERE / "neovim/uwvm.lua") + ")",
            "ui.setup({root=" + lua_string(HERE.parent.parent) + ", profile=profile_path})",
            'local p = vim.json.decode(table.concat(vim.fn.readfile(profile_path), "\\n"))',
            'vim.api.nvim_create_user_command("UwvmAttach", function(args)',
            '  ui.attach(p.broker.socketDir, args.args ~= "" and args.args or p.stepLevel or "source",',
            '    {moduleId=p.moduleId or 0, sourceMap=p.sourceMap, startupTimeoutSeconds=p.startupTimeoutSeconds, debugPermissions=p.debugPermissions})',
            'end, {nargs="?", force=true})', 'return ui']
    write_generated(nvim_path, ("\n".join(lua) + "\n").encode(), replace)
    jetbrains, netbeans = generate_java_clients(profile, output, profile_path, replace)
    manifest = {"format": "uwvm-ide-import-result-v1", "jetbrains": jetbrains, "netbeans": netbeans, "workspace": str(workspace_path), "neovim": str(nvim_path),
                "genericDAP": str(output / "dap.json"), "profile": str(profile_path),
                "runtimeLaunch": "runtime" in profile, "transport": profile["transport"]["kind"],
                "qualification": "generated configuration; no IDE test claimed",
                "additionalClientValidationPending": {"CLion": "built-in DAP client; configure generated stdio/attach settings",
                    "IntelliJ IDEA": "generated LSP4IJ configurations require the UWVM bridge plugin",
                    "Apache NetBeans": "generated attach inputs require the UWVM NetBeans bridge module",
                    "Oracle JDeveloper": "requires a dedicated extension"}}
    write_generated(output / "import-result.json", json_bytes(manifest), replace)
    return manifest


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="operation", required=True)
    for name in ("adapter", "broker", "validate"):
        item = commands.add_parser(name)
        item.add_argument("--profile", required=True)
    item = commands.add_parser("import")
    item.add_argument("--profile", required=True)
    item.add_argument("--output")
    item.add_argument("--nvim-dap")
    item.add_argument("--replace", action="store_true")
    item.add_argument("--open", choices=("code", "cursor", "neovim"))
    arguments = parser.parse_args(argv)
    profile = load_profile(arguments.profile)
    if arguments.operation == "validate":
        print(json.dumps({"valid": True, "format": FORMAT, "transport": profile["transport"]["kind"]}))
        return 0
    if arguments.operation in ("adapter", "broker"):
        if arguments.operation == "adapter":
            if selected_permissions(profile) == "none":
                raise ValueError("Debugger disabled; run the runtime task without attaching")
            session = os.environ.get("UWVM_DAP_SOURCE_SESSION")
            command = adapter_command(profile, session)
        else:
            command = broker_command(profile)
        os.execv(command[0], command)
        return 127
    output = arguments.output or str(Path(profile["workspace"]) / ".uwvm-debug")
    result = generate(profile, output, arguments.nvim_dap, arguments.replace)
    print(json.dumps(result, indent=2))
    if arguments.open:
        editor = shutil.which("nvim" if arguments.open == "neovim" else arguments.open)
        if editor is None:
            raise ValueError("Requested editor is not installed")
        if arguments.open == "neovim":
            command = [editor, "-u", result["neovim"]]
        else:
            command = [editor, "--new-window", "--extensionDevelopmentPath=" + str(HERE), result["workspace"]]
        return subprocess.call(command)
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, RuntimeError, KeyError, json.JSONDecodeError) as error:
        print("UWVM IDE import: " + str(error), file=sys.stderr)
        raise SystemExit(2)
