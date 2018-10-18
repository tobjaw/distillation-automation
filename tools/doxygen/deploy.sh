#!/usr/bin/env sh
set -euo pipefail
IFS=$'\n\t'


echo "+++ trying to deploy to target"
ssh-keyscan -t rsa ${DOCS_DEPLOY_HOST} 2>&1 >> ~/.ssh/known_hosts && \
scp -r ${DOCS_DEPLOY_DIR_SRC} ${DOCS_DEPLOY_USER}@${DOCS_DEPLOY_HOST}:${DOCS_DEPLOY_DIR_TARGET} && \
echo "+++ deploy success" && \
echo "deploy ${DOCS_DEPLOY_DIR_SRC} to ${DOCS_DEPLOY_HOST}:${DOCS_DEPLOY_DIR_TARGET}"
