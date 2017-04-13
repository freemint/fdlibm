#!/bin/sh

TMP="$1"
OUT="$2"

CC=m68k-atari-mint-gcc ./configure && make CROSS=yes

make prefix="${TMP}" install
find "{$TMP}" -type f \( -name '*.a' -o -name '*.o' \) -exec m68k-atari-mint-strip -S -x {} \;
mkdir -p "${OUT}" && cd "${TMP}" && tar cjf "${OUT}/${PROJECT}-${SHORT_ID}.tar.bz2" *
