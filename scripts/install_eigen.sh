#!/bin/bash
if [ -z "$1" ]; then
    echo "please provide include dir path";
    exit 1
fi

if [ ! -d "$1" ]; then
  echo "$1 directory must exist"
  exit 1
fi

EIGEN_TARGZ_URL="http://bitbucket.org/eigen/eigen/get/3.3.4.tar.gz"
echo $EIGEN_TARGZ_URL

cd $1
# mkdir Eigen
curl -SL $EIGEN_TARGZ_URL | tar xvz "eigen-eigen-5a0156e40feb/Eigen" --strip-components=1