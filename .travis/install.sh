#!/bin/bash -eux
# -e: Exit immediately if a command exits with a non-zero status.
# -u: Treat unset variables as an error when substituting.
# -x: Display expanded script commands

if [ "${TRAVIS_PULL_REQUEST}" = "false" -a "${TRAVIS_REPO_SLUG}" = "freemint/fdlibm" ]
then
	BINTRAY_REPO="travis" SYSROOT_DIR="/" ./.travis/install_bintray.sh m68k-atari-mint-binutils-gdb m68k-atari-mint-gcc mintbin
	./.travis/install_bintray.sh mintlib
else
	sudo add-apt-repository -y ppa:vriviere/ppa
	sudo apt-get update
	sudo apt-get install binutils-m68k-atari-mint gcc-m68k-atari-mint mintbin-m68k-atari-mint mintlib-m68k-atari-mint
fi
