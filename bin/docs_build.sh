#!/usr/bin/env sh
set -euo pipefail
IFS=$'\n\t'

if [ -z ${CI+x} ]; then
    # local invocation
    docker run \
           -it \
           --rm \
           -v $(pwd):/data \
           hrektts/doxygen \
           doxygen docs/Doxyfile
else
    # CI invocation
    docker run \
           -it \
           --rm \
           --volumes-from buildkite-builds \
           -w "$PWD" \
           hrektts/doxygen \
           doxygen docs/Doxyfile
fi
