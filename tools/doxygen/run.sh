#!/usr/bin/env sh
set -euo pipefail
IFS=$'\n\t'

docker run \
       -it \
       --rm \
       --volumes-from buildkite-builds \
       -w "$PWD" \
       hrektts/doxygen \
       doxygen docs/Doxyfile
