#ifndef DEFINE_METHODS_HPP
#define DEFINE_METHODS_HPP

#include "types.hpp"

Matrix explicit_euler(
    unsigned int t_steps, double t_max,
    unsigned int x_steps, double x_min, double x_max,
    const EdgeFunctionType phi, const FictiveEdgeFunctionType fictive_edge_function,
    double alpha, double mu, double r, double sigma
);

#endif