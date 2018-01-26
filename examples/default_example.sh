#!/bin/sh
ROOT="$(dirname "$(dirname "$(readlink -fm "$0")")")" # quotes to prevent whitespace errors
cd $ROOT
mkdir -p output
./bin/portfolio_optimizer -o output/default_result.m
matlab -nodisplay -nojvm -nosplash -nodesktop -wait -r \
    "try, addpath(genpath('output')); default_result; addpath(genpath('scripts')); output = 'output/default_example.png'; result_to_surf, catch, exit(1), end, exit(0);"