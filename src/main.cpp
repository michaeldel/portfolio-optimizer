#include <cmath>
#include <iostream>

#include "methods/explicit_euler.hpp"
#include "methods/implicit_euler.hpp"
#include "types.hpp"

int main() {
    const unsigned int t_steps = 9;
    const unsigned int x_steps = 10;

    const double mu = 0.06;
    const double r = 0.05;
    const double sigma = 0.2;

    const double p = 0.5;
    const BoundaryFunction phi = [p](double x) { return std::pow(x, p); };
    const FictiveBoundaryFunction fictive_edge_function = [p](const Vector& v, double x_max, double hx) {
        const MatrixDimSizeType m = v.size() - 1;
        if (x_max >= 1)
            return 2 * hx * p / x_max * v(m) + v(m - 1);
        else
            return 2 * hx / x_max * v(m) + v(m - 1);
    };

    const ExplicitEulerPortfolioOptimizer explicit_euler_optimizer(
        t_steps, 1.0,
        x_steps, 0.0, 2.0,
        0.0, phi
    );

    const auto [ee_v, ee_alphas] = explicit_euler_optimizer.optimize(
        mu, r, sigma, fictive_edge_function
    );

    std::cout << "ee alphas:\n" << ee_alphas << std::endl;
    std::cout << "ee v:\n" << ee_v << std::endl;

    const ImplicitEulerPortfolioOptimizer implicit_euleroptimizer(
        t_steps, 1.0,
        x_steps, 0.0, 2.0,
        0.0, phi
    );

    const auto [ie_v, ie_alphas] = implicit_euleroptimizer.optimize(mu, r, sigma);

    std::cout << "ie alphas:\n" << ie_alphas << std::endl;
    std::cout << "ie v:\n" << ie_v << std::endl;

    return 0;
}