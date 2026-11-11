"""Shared host launch policy parsing; transport authority stays with the VM."""


PERMISSIONS = ("full", "read-only", "limited", "none")
_FORCE_ARGS_LEADING = {
    "--wasip1-global-force-args": 0,
    "--wasip1-single-force-args": 1,
    "--wasip1-group-force-args": 1,
}


def host_options(arguments, error_type=ValueError):
    """Keep option positions, masking counted guest values before finding --run.

    The VM's force-args pretreatment consumes the target, count and exactly that
    many arbitrary tokens. Other pretreatments only consume numeric operands;
    they cannot hide debugger selectors or run delimiters. None placeholders
    prevent a preceding option from acquiring a different, later operand.
    This is launch-policy classification, not replacement CLI validation.
    """
    if not isinstance(arguments, (list, tuple)) or any(not isinstance(value, str) for value in arguments):
        raise error_type("runtime arguments must be an exact string array")
    options = list(arguments)
    index = 0
    while index < len(options):
        value = options[index]
        if value in ("--run", "-r", "--"):
            return options[:index]
        leading = _FORCE_ARGS_LEADING.get(value)
        if leading is None:
            index += 1
            continue
        count_index = index + 1 + leading
        if count_index >= len(options):
            raise error_type(value + " requires its target, count and counted argument values")
        # Accept decimal spelling without Python's large-integer digit limit.
        # The actual count cannot exceed the remaining argument vector. The VM
        # remains responsible for its target ABI's numeric and policy checks.
        digits = options[count_index].lstrip(" \t\r\n\f\v")
        if digits.startswith("+"):
            digits = digits[1:]
        if not digits or any(char < "0" or char > "9" for char in digits):
            raise error_type(value + " requires an unsigned decimal argument count")
        count = 0
        remaining = len(options) - count_index - 1
        for char in digits:
            count = count * 10 + ord(char) - ord("0")
            if count > remaining:
                raise error_type(value + " has fewer argument values than its count")
        end = count_index + 1 + count
        options[index + 1:end] = [None] * (end - index - 1)
        index = end
    return options


def _default_permissions(options):
    for index, value in enumerate(options):
        if value in ("-Rdbg", "--runtime-debug"):
            return "full"
        if value in ("-m", "--mode") and index + 1 < len(options) and options[index + 1] == "debug":
            return "full"
    return "limited"


def default_debug_permissions(arguments):
    return _default_permissions(host_options(arguments))


def _explicit_permissions(options, error_type):
    positions = [index for index, value in enumerate(options) if value == "--debug-permissions"]
    if len(positions) > 1 or positions and positions[0] + 1 >= len(options):
        raise error_type("exactly one valid --debug-permissions value is required")
    if not positions:
        return None
    value = options[positions[0] + 1]
    if value not in PERMISSIONS:
        raise error_type("--debug-permissions requires full, read-only, limited or none")
    return value


def explicit_debug_permissions(arguments, error_type=ValueError):
    return _explicit_permissions(host_options(arguments, error_type), error_type)


def selected_debug_permissions(arguments, error_type=ValueError):
    options = host_options(arguments, error_type)
    return _explicit_permissions(options, error_type) or _default_permissions(options)


def validate_fault_action(arguments, error_type=ValueError):
    options = host_options(arguments, error_type)
    if any(value is not None and (value == "--debug" or value.startswith("--debug=")) for value in options):
        raise error_type("--debug was removed; use --debug-fault-action pause for explicit terminal pause")
    positions = [i for i, value in enumerate(options) if value == "--debug-fault-action"]
    if len(positions) > 1 or positions and (positions[0]+1 >= len(options) or options[positions[0]+1] not in ("crash", "pause", "exit")):
        raise error_type("--debug-fault-action accepts exactly one crash, pause or exit value; crash is the default")
