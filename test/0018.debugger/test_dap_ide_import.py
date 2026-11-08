import importlib.util
import json
import os
from pathlib import Path
import shlex
import sys
import tempfile
import unittest
import xml.etree.ElementTree as ET

DEBUG = Path(__file__).resolve().parents[2] / "tools/debug"
sys.path.insert(0, str(DEBUG))
spec = importlib.util.spec_from_file_location("uwvm_ide_import", DEBUG / "import_ide.py")
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)


class IDEImport(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="uwvm-ide-import-")
        self.root = Path(self.temporary.name)
        self.profile = {"format": module.FORMAT, "name": "robot",
            "workspace": str(self.root), "transport": {"kind": "local"},
            "broker": {"socketDir": str(self.root / "broker")}}
        self.path = self.root / "input.json"
        self.path.write_text(json.dumps(self.profile))
        self.path.chmod(0o600)

    def tearDown(self):
        self.temporary.cleanup()

    def test_four_debug_permissions_reach_every_client_and_remote_launch(self):
        for permission in ("full", "read-only", "limited", "none"):
            profile = {**self.profile, "debugPermissions": permission,
                       "runtime": {"executable": "/bin/true", "arguments": ["-Rdbg", "--run", "/fixture.wasm"]}}
            command = module.broker_command(profile)
            self.assertEqual(command[command.index("--debug-permissions") + 1], permission)
            if permission == "none": self.assertNotIn("serve", command)
            remote = {**profile, "transport": {"kind": "ssh", "host": "operator@robot.example",
                       "port": 2222, "identityFile": str(self.root / "ssh-key"),
                       "knownHostsFile": str(self.root / "known-hosts"), "debugDirectory": "/opt/uwvm2-ros/tools/debug"}}
            remote_command = module.broker_command(remote)
            remote_arguments = shlex.split(remote_command[-1])
            self.assertEqual(remote_arguments[remote_arguments.index("--debug-permissions") + 1], permission)
            self.assertIn("ssh", remote_command[0])
            result = module.generate(profile, self.root / ("permissions-" + permission))
            configs = json.loads(Path(result["workspace"]).read_text())["launch"]["configurations"]
            self.assertEqual(len(configs), 0 if permission == "none" else 3)
            self.assertTrue(all(c["debugPermissions"] == permission for c in configs))
            for path in result["netbeans"].values():
                self.assertEqual(json.loads(Path(path).read_text())["attach"]["debugPermissions"], permission)
            for path in (Path(result["jetbrains"])/".idea/runConfigurations").glob("*.xml"):
                opts = {x.attrib["name"]: x.attrib.get("value") for x in ET.parse(path).iter("option") if "name" in x.attrib}
                self.assertEqual(json.loads(opts["attachConfiguration"])["debugPermissions"], permission)
            self.assertIn("debugPermissions=p.debugPermissions", Path(result["neovim"]).read_text())
        for old in ("dev", "release", True, 1):
            with self.assertRaises(ValueError): module.validate({**self.profile, "debugPermissions": old})
        self.assertEqual(module.selected_permissions({"runtime":{"arguments":["-Rdbg","--run","a.wasm"]}}), "full")
        self.assertEqual(module.selected_permissions({"runtime":{"arguments":["--run","a.wasm","-Rdbg"]}}), "limited")
        with self.assertRaises(ValueError):
            module.validate({**self.profile,"debugPermissions":"full","runtime":{"executable":"/bin/true", "arguments":["--debug-permissions","limited","--run","a.wasm"]}})

    def test_explicit_startup_deadline_reaches_all_generated_clients(self):
        self.profile["startupTimeoutSeconds"] = 12.5
        self.profile["runtime"] = {"executable": "/bin/true", "arguments": ["--run", "/fixture.wasm"]}
        argv = module.broker_command(self.profile)
        self.assertEqual(argv[argv.index("--startup-timeout")+1], "12.5")
        self.assertLess(argv.index("--startup-timeout"), argv.index("--"))
        result = module.generate(self.profile, self.root / "generated")
        workspace = json.loads(Path(result["workspace"]).read_text())
        self.assertTrue(all(row["startupTimeoutSeconds"] == 12.5 for row in workspace["launch"]["configurations"]))
        for path in result["netbeans"].values():
            self.assertEqual(json.loads(Path(path).read_text())["attach"]["startupTimeoutSeconds"], 12.5)
        for path in (Path(result["jetbrains"])/".idea/runConfigurations").glob("*.xml"):
            options = {x.attrib["name"]: x.attrib.get("value") for x in ET.parse(path).iter("option") if "name" in x.attrib}
            self.assertEqual(json.loads(options["attachConfiguration"])["startupTimeoutSeconds"], 12.5)
        self.assertIn("startupTimeoutSeconds=p.startupTimeoutSeconds", Path(result["neovim"]).read_text())

    def test_startup_deadline_is_opt_in_and_validated(self):
        self.assertNotIn("startupTimeoutSeconds", module.validate(self.profile))
        for value in (0, -1, True, "10", float("nan"), float("inf"), 10**400):
            self.profile["startupTimeoutSeconds"] = value
            with self.subTest(value=repr(value)), self.assertRaises(ValueError):
                module.validate(self.profile)

    def test_watch_task_has_valid_non_diagnostic_pattern_and_ready_markers(self):
        # VS Code rejects background tasks whose problem pattern lacks a file.
        # Broker readiness is a background transition, never a fabricated file error.
        import re
        self.profile["runtime"] = {"executable": "/bin/true", "arguments": ["--run", "/fixture.wasm"]}
        result = module.generate(self.profile, self.root / "generated")
        task = json.loads(Path(result["workspace"]).read_text())["tasks"]["tasks"][0]
        self.assertTrue(task["isBackground"])
        matcher = task["problemMatcher"][0]
        pattern = matcher["pattern"][0]
        self.assertEqual(pattern["kind"], "file")
        regex = re.compile(pattern["regexp"])
        self.assertTrue(0 < pattern["file"] <= regex.groups)
        self.assertTrue(0 < pattern["message"] <= regex.groups)
        for line in ("debug server: /private/control.sock", "client: secure_server.py connect", "error: startup failed"):
            self.assertIsNone(regex.search(line))
        self.assertRegex("debug server: /private/control.sock", matcher["background"]["beginsPattern"])
        self.assertRegex("client: secure_server.py connect", matcher["background"]["endsPattern"])

    def test_owned_profile_and_local_stdio(self):
        selected = module.load_profile(str(self.path))
        self.assertEqual(module.adapter_command(selected),
                         ["/usr/bin/python3", str(DEBUG / "dap_adapter.py")])

    def test_reject_profile_symlink(self):
        link = self.root / "link.json"
        link.symlink_to(self.path)
        with self.assertRaises(OSError):
            module.load_profile(str(link))

    def test_reject_writable_profile(self):
        self.path.chmod(0o666)
        with self.assertRaises(ValueError):
            module.load_profile(str(self.path))

    def test_ssh_is_verified_stdio_and_quotes_remote_paths(self):
        self.profile["transport"] = {"kind": "ssh", "host": "operator@robot",
            "debugDirectory": "/opt/robot's tools/debug", "python": "/opt/py thon",
            "port": 2222, "identityFile": "/private/id", "knownHostsFile": "/private/hosts"}
        session = "12345678-1234-1234-1234-123456789abc"
        argv = module.adapter_command(self.profile, session)
        self.assertIn("StrictHostKeyChecking=yes", argv)
        self.assertIn("BatchMode=yes", argv)
        self.assertIn("ForwardAgent=no", argv)
        self.assertIn("ClearAllForwardings=yes", argv)
        self.assertEqual(argv[-2], "operator@robot")
        self.assertEqual(shlex.split(argv[-1]),
            ["exec", "env", "UWVM_DAP_SOURCE_SESSION=" + session, "/opt/py thon",
             "/opt/robot's tools/debug/dap_adapter.py"])
        self.assertNotIn("-L", argv)
        self.assertNotIn("-R", argv)

    def test_ssh_shell_or_option_injection_refused(self):
        for host in ("-oProxyCommand=bad", "robot;touch /tmp/bad", "$(id)", "operator robot", "robot\ncommand"):
            self.profile["transport"] = {"kind": "ssh", "host": host, "debugDirectory": "/opt/debug"}
            with self.assertRaises(ValueError):
                module.adapter_command(self.profile)

    def test_ssh_cannot_weaken_host_keys(self):
        self.profile["transport"] = {"kind": "ssh", "host": "robot", "debugDirectory": "/opt/debug",
                                     "StrictHostKeyChecking": "no"}
        with self.assertRaises(ValueError):
            module.validate(self.profile)

    def test_invalid_source_session_refused(self):
        with self.assertRaises(ValueError):
            module.adapter_command(self.profile, "x; anything")

    def test_capability_never_imported(self):
        self.profile["broker"]["capability"] = "launch-secret"
        with self.assertRaises(ValueError):
            module.validate(self.profile)

    def test_ambiguous_source_maps_refused(self):
        self.profile["sourceMap"] = {"/sdk": "/same", "/app": "/same"}
        with self.assertRaises(ValueError):
            module.validate(self.profile)

    def test_profile_arguments_preserved_without_new_permissions(self):
        arguments = ["--mode", "debug", "-Raot", "--run", "/robot/control.wasm"]
        self.profile["runtime"] = {"executable": "/opt/uwvm2-ros", "arguments": arguments}
        command = module.broker_command(self.profile)
        self.assertEqual(command[-len(arguments):], arguments)
        self.assertFalse(any("wasip1" in value or "ros1" in value for value in command))

    def test_generated_workspace_does_not_overwrite_vscode(self):
        existing = self.root / ".vscode"
        existing.mkdir()
        (existing / "launch.json").write_text("// user JSONC remains unchanged\n")
        out = self.root / ".uwvm-debug"
        result = module.generate(self.profile, str(out))
        self.assertEqual((existing / "launch.json").read_text(), "// user JSONC remains unchanged\n")
        configs = json.loads(Path(result["workspace"]).read_text())["launch"]["configurations"]
        self.assertEqual([item["stepLevel"] for item in configs], list(module.LEVELS))
        self.assertTrue(all(item["transportProfile"] == result["profile"] for item in configs))
        self.assertEqual(Path(result["profile"]).stat().st_mode & 0o777, 0o600)
        self.assertIn("no IDE test claimed", result["qualification"])
        self.assertEqual(module.generate(self.profile, str(out)), result)

    def test_changed_generated_file_requires_explicit_replace(self):
        out = self.root / ".uwvm-debug"
        module.generate(self.profile, str(out))
        self.profile["name"] = "new robot"
        with self.assertRaises(ValueError):
            module.generate(self.profile, str(out))
        module.generate(self.profile, str(out), replace=True)

    def test_guest_preopen_cannot_cover_host_launch_profile(self):
        self.profile["runtime"] = {"executable": "/opt/uwvm2-ros", "arguments": [
            "--wasip1-single-mount-dir", "main", "/guest", str(self.root), "--run", "/module.wasm"]}
        with self.assertRaisesRegex(RuntimeError, "preopen"):
            module.generate(self.profile, str(self.root / ".uwvm-debug"))

    def test_java_clients_preserve_exact_attach_arguments(self):
        self.profile["name"] = 'robot <A> & "B"'
        self.profile["moduleId"] = (1 << 32) - 1
        self.profile["sourceMap"] = {"/remote/project": str(self.root / "source & local")}
        result = module.generate(self.profile, str(self.root / "generated"))
        for level in module.LEVELS:
            tree = ET.parse(Path(result["jetbrains"]) / ".idea/runConfigurations" / (level + ".xml"))
            config = tree.getroot().find("configuration")
            self.assertEqual(config.attrib["name"], self.profile["name"] + " (" + level + ")")
            options = {item.attrib["name"]: item.attrib.get("value") for item in config.findall("option")}
            self.assertEqual(options["debugMode"], "ATTACH")
            self.assertEqual(options["serverId"], "uwvm")
            attach = json.loads(options["attachConfiguration"])
            self.assertEqual(attach["moduleId"], (1 << 32) - 1)
            self.assertEqual(attach["sourceMap"], self.profile["sourceMap"])
            self.assertEqual(attach["uwvmProfile"], result["profile"])
            self.assertEqual(attach["stepLevel"], level)
            netbeans = json.loads(Path(result["netbeans"][level]).read_text())
            self.assertEqual(netbeans["format"], "uwvm-netbeans-attach-v1")
            self.assertEqual(netbeans["profile"], result["profile"])
            self.assertEqual(netbeans["attach"]["moduleId"], (1 << 32) - 1)
            self.assertEqual(netbeans["attach"]["sourceMap"], self.profile["sourceMap"])
            self.assertNotIn("port", netbeans["attach"])

    def test_java_client_output_symlink_is_rejected(self):
        out = self.root / "generated"
        out.mkdir(mode=0o700)
        elsewhere = self.root / "outside"
        elsewhere.mkdir()
        (out / "jetbrains").symlink_to(elsewhere, target_is_directory=True)
        with self.assertRaises(ValueError):
            module.generate(self.profile, str(out))
        self.assertEqual(list(elsewhere.iterdir()), [])

    def test_attach_only_does_not_gain_launch_authority(self):
        with self.assertRaises(ValueError):
            module.broker_command(self.profile)


if __name__ == "__main__":
    unittest.main()
