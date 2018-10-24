#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

. ./.env

docker build ${DIR_TOOLS_DEV} -t ${DOCKER_TOOLS_DEV_NAME}
