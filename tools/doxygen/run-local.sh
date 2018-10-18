#!/usr/bin/env sh
set -euo pipefail
IFS=$'\n\t'

docker run \
       -it \
       --rm \
       -v $(pwd):/data \
       hrektts/doxygen \
       doxygen docs/Doxyfile
