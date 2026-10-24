# Stock standard-library fixture usage

Use [stock_stdlib.py](stock_stdlib.py) with the selected native/WASI producer,
explicit standard-library deployment and runtime. The shared
[stock_stdlib_probe.py](stock_stdlib_probe.py) supplies semantic expectations for
imports, argv/environment, exceptions, GC and Python trace/locals behavior.

Keep permissions, exact output and invalid-input outcomes explicit. Python
trace/locals behavior does not establish a UWVM Python-source debugger bridge.
Execution records belong to the selected new run output, rather than this source
directory. The old revision-specific result narrative has been retired.
