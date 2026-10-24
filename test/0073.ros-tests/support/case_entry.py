"""Select the invoking numbered case while sharing one finite-owner launcher."""

from pathlib import Path
import os
import sys

root = Path(__file__).resolve().parent.parent
case = Path(sys.argv[0]).absolute().with_name("case.json")
os.execv(
    sys.executable,
    [
        sys.executable,
        "-I",
        "-S",
        "-B",
        str(root / "support/run_case.py"),
        "--case",
        str(case),
        *sys.argv[1:],
    ],
)
