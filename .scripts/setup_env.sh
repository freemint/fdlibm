#!/bin/sh
# Use as: ". setup_env.sh"

PROJECT_REPO=$(echo "${GITHUB_REPOSITORY}" | cut -d '/' -f 1)
echo "PROJECT_REPO=${PROJECT_REPO}" >> $GITHUB_ENV
PROJECT_NAME=$(echo "${GITHUB_REPOSITORY}" | cut -d '/' -f 2)
echo "PROJECT_NAME=${PROJECT_NAME}" >> $GITHUB_ENV
# PROJECT_VERSION is defined in the action script
echo "PROJECT_VERSION=${PROJECT_VERSION}" >> $GITHUB_ENV
INSTALL_DIR="/tmp/${PROJECT_NAME}"
echo "INSTALL_DIR=${INSTALL_DIR}" >> $GITHUB_ENV
DEPLOY_DIR="/tmp/${PROJECT_NAME}-deploy"
echo "DEPLOY_DIR=${DEPLOY_DIR}" >> $GITHUB_ENV
DEPLOY_ARCHIVE="tar.bz2"
echo "DEPLOY_ARCHIVE=${DEPLOY_ARCHIVE}" >> $GITHUB_ENV

SHORT_ID=$(echo ${GITHUB_SHA} | cut -c 1-3)
echo "SHORT_ID=$SHORT_ID" >> $GITHUB_ENV
LONG_ID=$(echo ${GITHUB_SHA} | cut -c 1-8)
echo "LONG_ID=$LONG_ID" >> $GITHUB_ENV
BRANCH=$(echo "${GITHUB_REF}" | cut -d '/' -f 3)

if test "$CPU_TARGET" != ""; then
	echo "CPU_TARGET=$CPU_TARGET" >> $GITHUB_ENV
fi

# GITHUB_HEAD_REF is only set for pull requests
if [ "${GITHUB_HEAD_REF}" = "" ]
then
    COMMIT_MESSAGE="[${PROJECT_NAME}] [${BRANCH}] Commit: https://github.com/${PROJECT_REPO}/${PROJECT_NAME}/commit/${GITHUB_SHA}"
fi

echo "COMMIT_MESSAGE=$COMMIT_MESSAGE" >> $GITHUB_ENV
