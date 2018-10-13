#!/usr/bin/env sh
set -euo pipefail
IFS=$'\n\t'


cp $(pwd)/build/compile_commands.json $(pwd)

INDEX_DIRTY=""
git diff-index --quiet HEAD -- || INDEX_DIRTY=1

echo "--- run clang-format"
if [[ "${INDEX_DIRTY}" ]]; then
    docker run \
      --rm \
      -v "$(pwd)":"/workdir" \
      -w "/workdir" \
      clang-format \
      src/*.[ch]
    echo "\n\033[33m⚠ HEAD dirty - outputted correct formatting to STDOUT"
    echo "  make sure to commit all pending changes if you want this script to fix the formatting.\033[0m"
    exit 1
else
    docker run \
      --rm \
      -v "$(pwd)":"/workdir" \
      -w "/workdir" \
      clang-format \
      -i src/*.[ch]
    echo "\033[32mHEAD clean - reformatted files\033[0m"
fi

if [[ -z "${INDEX_DIRTY}" ]]; then
    echo "+++ check for diff"
    MODIFIED_FILES=$(git diff --name-only src)
    if [[ -z ${MODIFIED_FILES} ]]; then
        echo "\n\033[32m✔ All files formatted correctly"
        exit 0;
    else
        for f in ${MODIFIED_FILES}; do
            echo "+++ $f:"
            git diff $f | tail -n +5
        done
        echo "\n\033[33m⚠ Some files were not formatted correctly"
        echo "  They have been fixed automatically."
        exit 1;
    fi
fi
