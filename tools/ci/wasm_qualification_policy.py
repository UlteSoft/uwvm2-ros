"""Build guest policy arguments for comparisons between ROS and ordinary uwvm2."""


def feature_options(ros, enabled, disabled=()):
    """The Core 3 comparison fixtures override individual gates on a Core 2 baseline."""
    if not ros:
        return [*("-WFE-" + name for name in enabled), *("-WFD-" + name for name in disabled)]
    args = ["--wasm-policy-global", "parser.baseline", "wasm2"]
    for state, names in (("on", enabled), ("off", disabled)):
        for name in names:
            args.extend(("--wasm-policy-global", "parser." + name, state))
    return args


def feature_hint(ros, name):
    return "parser." + name + " on" if ros else "--wasm-feature-enable-" + name


# The older comparison matrices store ordinary-upstream option vectors in their
# fixture tables. Resolve them only at the point where a complete host command
# is assembled. Everything after the module operand belongs to the guest.
_BASELINES = {
    "--wasm-feature-mvp": "mvp", "-WFmvp": "mvp",
    "--wasm-feature-wasm1.1": "wasm1p1", "-WF1.1": "wasm1p1",
    "--wasm-feature-wasm2": "wasm2", "-WF2": "wasm2",
    "--wasm-feature-wasm3": "wasm3", "-WF3": "wasm3",
}
_KEYS = {
    "-Rllvm-policy": ("runtime.llvm.policy",),
    "--runtime-llvm-jit-policy": ("runtime.llvm.policy",),
    "-Rllvm-full-policy": ("runtime.llvm.full-policy",),
    "--runtime-llvm-jit-full-policy": ("runtime.llvm.full-policy",),
    "-Rint-op-conbine-level": ("runtime.interpreter.opcode-combine",),
    "-Rint-no-delay-local": ("runtime.interpreter.delay-local", "off"),
    "-Rint-no-loop-unwind": ("runtime.interpreter.loop-unwind", "off"),
    "-Rint-loop-unwind-size": ("runtime.interpreter.loop-unwind-max-size",),
    "-Rint-reorder": ("runtime.interpreter.instruction-reorder", "on"),
}
_FEATURES = (("-WFE-", "on"), ("-WFD-", "off"),
             ("--wasm-feature-enable-", "on"), ("--wasm-feature-disable-", "off"))


def product_command(ros, argv):
    """Render a complete dual-product fixture command, retaining the fixture's baseline."""
    if not ros:
        return argv
    for index, arg in enumerate(argv):
        if arg == "--run":
            break
    else:
        return argv
    host = []
    has_baseline = False
    for arg in argv[:index]:
        if arg in _BASELINES:
            host.extend(("--wasm-policy-global", "parser.baseline", _BASELINES[arg]))
            has_baseline = True
        elif arg in _KEYS:
            host.extend(("--wasm-policy-global", *_KEYS[arg]))
        else:
            for prefix, state in _FEATURES:
                if isinstance(arg, str) and arg.startswith(prefix):
                    host.extend(("--wasm-policy-global", "parser." + arg[len(prefix):], state))
                    break
            else:
                if arg == "parser.baseline":
                    has_baseline = True
                host.append(arg)
    if not has_baseline:
        host.extend(("--wasm-policy-global", "parser.baseline", "wasm2"))
    return host + list(argv[index:])
