#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

. ./.env

${DIR_BIN}/devtools_build.sh
${DIR_BIN}/devtools_run.sh format


if [ -z ${CI+x} ]; then
    echo ""
else
    echo "+++ check for diff"
    MODIFIED_FILES=$(git diff --name-only src)
    if [[ -z ${MODIFIED_FILES} ]]; then
        echo -e "\n\033[32m✔ All files formatted correctly\033[0m"
        exit 0;
    else
        for f in ${MODIFIED_FILES}; do
            echo "+++ $f:"
            git diff $f | tail -n +5
        done
        echo -e "\n\033[33m⚠ Some files were not formatted correctly\033[0m"
        exit 1;
    fi
fi
