#!/bin/sh
ROOT="$(dirname "$(dirname "$(readlink -fm "$0")")")" # quotes to prevent whitespace errors
cd $ROOT
mkdir -p output

./bin/portfolio_optimizer -f expliciteuler -o output/expliciteuler_result.m && \
./bin/portfolio_optimizer -f impliciteuler -o output/impliciteuler_result.m && \
./bin/portfolio_optimizer -f cranknicolson -o output/cranknicolson_result.m && \

matlab -nodisplay -nojvm -nosplash -nodesktop -wait -r "
    try, addpath(genpath('output'));
    expliciteuler_result; addpath(genpath('scripts')); output = 'output/expliciteuler_example.png'; result_to_surf,
    impliciteuler_result; addpath(genpath('scripts')); output = 'output/impliciteuler_example.png'; result_to_surf,
    cranknicolson_result; addpath(genpath('scripts')); output = 'output/cranknicolson_example.png'; result_to_surf,
    catch, exit(1), end, exit(0);
"