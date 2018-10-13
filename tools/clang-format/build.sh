#!/usr/bin/env sh
set -euo pipefail
IFS=$'\n\t'


docker build -t clang-format tools/clang-format
