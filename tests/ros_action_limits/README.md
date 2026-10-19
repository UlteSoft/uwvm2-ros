# ROS2 Action advertiser limits

The native Jazzy probe verifies four advertiser rejection cases (feedback,
success, abort and canceled result), two exact-limit cases, and two independent
responder-limit cases. Rejection must prevent the corresponding native effect
and allow a subsequent valid reply to the same live goal. The feedback limit
counts the canonical feedback bytes without the responder's kind envelope.

`run_native.py` builds and runs against the prepared immutable SDK/source
workspace. Its exact invocation and the later prebuilt-Gateway regression
driver are retained in the resources-20261013 evidence archive. Tests use
explicit test grants; they do not change default permissions.
