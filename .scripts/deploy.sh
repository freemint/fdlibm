#!/bin/sh -ex

UPLOAD_DIR=web196@server43.webgo24.de:/home/www/snapshots

if [ -z "${DEPLOY_ARCHIVE+x}" ]
then
	# zip is default
	DEPLOY_ARCHIVE="zip"
fi

if [ -n "${CPU_TARGET+x}" ]
then
	ARCHIVE_NAME="${PROJECT_NAME}-${PROJECT_VERSION}-${SHORT_ID}-${CPU_TARGET}.${DEPLOY_ARCHIVE}"
else
	ARCHIVE_NAME="${PROJECT_NAME}-${PROJECT_VERSION}-${SHORT_ID}.${DEPLOY_ARCHIVE}"
fi
ARCHIVE_PATH="${DEPLOY_DIR}/${ARCHIVE_NAME}"

mkdir -p "${DEPLOY_DIR}"

if [ "${DEPLOY_ARCHIVE}" = "tar.bz2" ]
then
	cd ${INSTALL_DIR} && tar cjf ${ARCHIVE_PATH} *
elif [ "${DEPLOY_ARCHIVE}" = "tar.xz" ]
then
	cd ${INSTALL_DIR} && tar cJf ${ARCHIVE_PATH} *
elif [ "${DEPLOY_ARCHIVE}" = "tar.gz" ]
then
	cd ${INSTALL_DIR} && tar czf ${ARCHIVE_PATH} *
else
	cd $(dirname ${INSTALL_DIR}) && zip -r -9 ${ARCHIVE_PATH} $(basename ${INSTALL_DIR})
fi

cd -


eval "$(ssh-agent -s)"

PROJECT_DIR="$PROJECT_NAME"
case $PROJECT_DIR in
	m68k-atari-mint-gcc) PROJECT_DIR=gcc ;;
	m68k-atari-mint-binutils-gdb) PROJECT_DIR=binutils ;;
esac

scp -o "StrictHostKeyChecking no" "$ARCHIVE_PATH" "${UPLOAD_DIR}/${PROJECT_DIR}/${ARCHIVE_NAME}"
if test -z "${CPU_TARGET}"
then
	scp -o "StrictHostKeyChecking no" "$ARCHIVE_PATH" "${UPLOAD_DIR}/${PROJECT_DIR}/${PROJECT_DIR}-latest.${DEPLOY_ARCHIVE}"
fi

echo ${PROJECT_NAME}-${PROJECT_VERSION}-${SHORT_ID} > .latest_version
scp -o "StrictHostKeyChecking no" .latest_version "${UPLOAD_DIR}/${PROJECT_DIR}/.latest_version"
