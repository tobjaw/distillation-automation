#!/usr/bin/env sh
set -euo pipefail
IFS=$'\n\t'


docker build -t cmake tools/cmake
