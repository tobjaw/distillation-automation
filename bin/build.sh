#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

. ./.env

${DIR_BIN}/devtools_build.sh
${DIR_BIN}/devtools_run.sh build
