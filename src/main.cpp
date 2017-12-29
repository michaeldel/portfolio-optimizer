#include <cmath>
#include <iostream>

#include "methods.hpp"
#include "types.hpp"

int main() {
    const unsigned int t_steps = 9;
    const unsigned int x_steps = 10;

    const double alpha = 0.3;
    const double mu = 0.06;
    const double r = 0.05;
    const double sigma = 0.2;

    const double p = 0.5;
    const EdgeFunctionType phi = [p](double x) { return x >= 0 && x <= 1 ? x : std::pow(x, p); };
    const FictiveEdgeFunctionType fictive_edge_function = [p](const Vector& v, double x_max, double hx) {
        const MatrixDimSizeType m = v.size() - 1;
        if (x_max >= 1)
            return 2 * hx * p / x_max * v(m) + v(m - 1);
        else
            return 2 * hx / x_max * v(m) + v(m - 1);
    };
    const Matrix v = explicit_euler(
        t_steps, 1.0,
        x_steps, 0.0, 2.0,
        phi, fictive_edge_function,
        alpha, mu, r, sigma
    );

    std::cout << "Computed V:\n" << v << std::endl;
    return 0;
}