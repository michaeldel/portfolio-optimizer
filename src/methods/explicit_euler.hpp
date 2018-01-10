#ifndef DEFINE_EXPLICIT_EULER_HPP
#define DEFINE_EXPLICIT_EULER_HPP

#include <utility>

#include "../optimizer.hpp"
#include "../types.hpp"

class ExplicitEulerPortfolioOptimizer: public PortfolioOptimizer {
public:
    ExplicitEulerPortfolioOptimizer(
        unsigned int t_steps, double t_max,
        unsigned int x_steps, double x_min, double x_max,
        double x_0_condition = 0.0,
        const BoundaryFunction v_T_condition = [](double x) { return x; }
    );
    std::pair<Matrix, Matrix> optimize(double yield, double interest_rate, double volatility) const;
    std::pair<Matrix, Matrix> optimize(
        double yield, double interest_rate, double volatility,
        const FictiveBoundaryFunction neumann_condition
    ) const;

private:
    const double m_x_max;
};

#endif