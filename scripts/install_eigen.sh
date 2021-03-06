#!/bin/sh
if [ -z "$1" ]; then
    echo "please provide include dir path";
    exit 1
fi

if [ ! -d "$1" ]; then
  echo "$1 directory must exist"
  exit 1
fi

EIGEN_TARGZ_URL="http://bitbucket.org/eigen/eigen/get/default.tar.gz"

cd $1
curl -SL $EIGEN_TARGZ_URL | tar xvz --wildcards "eigen-eigen-*/Eigen" --strip-components=1