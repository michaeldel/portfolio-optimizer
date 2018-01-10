#ifndef DEFINE_OPTIMIZER_HPP
#define DEFINE_OPTIMIZER_HPP

#include <utility>

#include "types.hpp"

class PortfolioOptimizer {
public:
    PortfolioOptimizer(
        unsigned int t_steps, double t_max,
        unsigned int x_steps, double x_min, double x_max,
        double x_0_condition = 0.0,
        const BoundaryFunction v_T_condition = [](double x) { return x; }
    );
    virtual std::pair<Matrix, Matrix> optimize(double yield, double interest_rate, double volatility) const = 0;

protected:
    const double m_ht;
    const double m_hx;

    // order is important for initialization
    const Vector m_xs;
    const Matrix m_mesh;
};

#endif