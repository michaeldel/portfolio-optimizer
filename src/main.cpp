#include <cmath>
#include <iostream>

#include "methods/crank_nicolson.hpp"
#include "methods/explicit_euler.hpp"
#include "methods/implicit_euler.hpp"
#include "output/stdout_output.hpp"
#include "output/matlab_m_output.hpp"
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

    const StdOutOutput ee_soo("Explicit Euler");
    ee_soo.write_output(ee_v, ee_alphas);

    const ImplicitEulerPortfolioOptimizer implicit_euler_optimizer(
        t_steps, 1.0,
        x_steps, 0.0, 2.0,
        0.0, phi
    );

    const auto [ie_v, ie_alphas] = implicit_euler_optimizer.optimize(mu, r, sigma);

    const StdOutOutput ie_soo("Implicit Euler");
    ie_soo.write_output(ie_v, ie_alphas);

    const CrankNicolsonPortfolioOptimizer crank_nicolson_optimizer(
        t_steps, 1.0,
        x_steps, 0.0, 2.0,
        0.0, phi
    );

    const auto [cn_v, cn_alphas] = crank_nicolson_optimizer.optimize(mu, r, sigma);

    const StdOutOutput cn_soo("Crank-Nicolson");
    cn_soo.write_output(cn_v, cn_alphas);

    const MatlabMOutput cn_mlo("cn.m");
    cn_mlo.write_output(cn_v, cn_alphas);

    return 0;
}