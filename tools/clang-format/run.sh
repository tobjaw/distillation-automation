#!/usr/bin/env sh
set -euo pipefail
IFS=$'\n\t'


cp $(pwd)/build/compile_commands.json $(pwd)

echo "--- run clang-format"
docker run \
       --rm \
       --volumes-from buildkite-builds \
       -w "$PWD" \
       clang-format \
       -i src/*.[ch]

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
