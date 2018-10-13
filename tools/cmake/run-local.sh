#!/usr/bin/env sh
set -euo pipefail
IFS=$'\n\t'


rm -rf ./build
mkdir -p ./build

docker run \
  --rm \
  -v "$PWD":"/workdir" \
  -w "/workdir/build" \
  cmake \
  -c "cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .."

cp build/compile_commands.json .
