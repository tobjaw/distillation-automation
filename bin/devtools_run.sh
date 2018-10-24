#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

. ./.env

if [ -z ${CI+x} ]; then
    # local invocation
    docker run \
           --rm \
           -it \
           --env-file ".env" \
           -v "$(pwd)":"/workdir" \
           -w "/workdir" \
           --name avr-devtools \
           avr-devtools:latest \
           "$@"
else
    # CI invocation
    docker run \
           --rm \
           -it \
           --env-file ".env" \
           --volumes-from buildkite-builds \
           -w "$PWD" \
           avr-devtools:latest \
           "$@"
fi
