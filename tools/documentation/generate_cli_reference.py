#!/usr/bin/env python3
"""Render the actual advertised CLI; never execute a Guest module."""
import argparse
import pathlib
import re
import subprocess

def parse_help(text):
    text = re.sub(r"\x1b\[[0-?]*[ -/]*[@-~]", "", text)
    entries = []
    pending = None
    names = set()
    for line in text.splitlines():
        match = re.match(r"^\s+(--\S+)\s+-----\s+(.*)$", line)
        if match:
            if pending is not None:
                raise ValueError("command missing usage: " + pending["name"])
            name, description = match.groups()
            if name in names:
                raise ValueError("duplicate command: " + name)
            names.add(name)
            pending = {"name": name, "description": description}
        elif line.lstrip().startswith("Usage:"):
            if pending is None:
                raise ValueError("usage without command")
            pending["usage"] = line.split("Usage:", 1)[1].strip()
            entries.append(pending)
            pending = None
    if pending is not None or not entries:
        raise ValueError("incomplete or empty CLI help")
    return entries

def render(entries, prefix=None):
    output = [
        "# " + ("ROSv2 advertised CLI catalog" if prefix == "--ros-scene-" else
                  "Advertised CLI catalog: " + prefix if prefix else "Complete advertised CLI catalog"), "",
        "Generated from an actual Linux integration binary's §--help all§.",
        ("This catalog contains every advertised primary command matching §" + prefix + "§," if prefix else
         "This catalog contains every advertised primary command,") + " its aliases in",
        "the usage line, and the binary's own description. Availability and",
        "supported transports still follow the ROS product policy. A registered",
        "platform-specific option is not proof that its backend is available.", "",
        "See the [operator whitepaper](../user-whitepaper.md) for build, permissions,",
        "debugging, update boundaries and exit status, and the",
        "[topic references](README.md) for semantic details and subcommands.", "",
        "Regenerate with §tools/documentation/generate_cli_reference.py --vm",
        "/absolute/path/to/uwvm2-ros " + ("--prefix=" + prefix + " " if prefix else "") +
        "--output documents/command-line/" + ("ros-scene-cli-catalog.md" if prefix == "--ros-scene-" else "cli-catalog.md") + "§.",
        "An existing exact help capture can instead be supplied with §--help-file§.",
        "The generator does not run a Guest or confer any permissions.", "",
        "Advertised primary commands: **" + str(len(entries)) + "**.", ""
    ]
    for entry in entries:
        output.extend(["## §" + entry["name"] + "§", "", "§§§text",
                       entry["usage"], "§§§", "", entry["description"], ""])
    return "\n".join(output).replace("§", chr(96))

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument("--vm", type=pathlib.Path)
    source.add_argument("--help-file", type=pathlib.Path)
    parser.add_argument("--prefix", help="Include only advertised primary commands with this exact prefix")
    parser.add_argument("--output", type=pathlib.Path, required=True)
    args = parser.parse_args()
    if args.vm:
        completed = subprocess.run([str(args.vm.resolve()), "--log-color", "disable", "--help", "all"],
                                   stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
        text = (completed.stdout + completed.stderr).decode("utf-8")
    else:
        text = args.help_file.read_text(encoding="utf-8")
    entries = parse_help(text)
    if args.prefix is not None:
        if not re.fullmatch(r"--[a-z0-9-]+", args.prefix):
            parser.error("--prefix must be a literal long-option prefix")
        # The ROSv2 child-module dispatcher is the exact parent option. Keep it
        # in the scene family alongside the older hyphenated configuration forms.
        entries = [entry for entry in entries if entry["name"].startswith(args.prefix) or
                   (args.prefix == "--ros-scene-" and entry["name"] == "--ros-scene")]
        if not entries:
            parser.error("no advertised commands match --prefix")
    args.output.write_text(render(entries, args.prefix), encoding="utf-8")
    print(f"Rendered {len(entries)} commands to {args.output}")

if __name__ == "__main__":
    main()
