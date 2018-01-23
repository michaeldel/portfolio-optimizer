#!/bin/sh
if [ -z "$1" ]; then
    echo "please provide include dir path";
    exit 1
fi

if [ ! -d "$1" ]; then
  echo "$1 directory must exist"
  exit 1
fi

TCLAP_TARGZ_URL="https://sourceforge.net/projects/tclap/files/latest/download?source=files"

cd $1
curl -SL $TCLAP_TARGZ_URL | tar xvz --wildcards "tclap-*/include/tclap" --strip-components=1
mv include/tclap tclap
rm -rf include