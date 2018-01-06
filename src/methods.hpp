#ifndef DEFINE_METHODS_HPP
#define DEFINE_METHODS_HPP

#include <utility>

#include "types.hpp"

std::pair<Matrix, Matrix> explicit_euler(
    unsigned int t_steps, double t_max,
    unsigned int x_steps, double x_min, double x_max,
    const EdgeFunctionType phi, const FictiveEdgeFunctionType fictive_edge_function,
    double mu, double r, double sigma
);

#endif