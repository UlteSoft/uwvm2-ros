#!/bin/sh
set -eu
exec python3 "$(dirname "$0")/ros/run_network.py" --mode ros1 "$@"
