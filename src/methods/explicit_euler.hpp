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
        const BoundaryFunction v_T_condition = [](double x) { return x; },
        const FictiveBoundaryFunction neumann_condition = [](const Vector& v, double x_max, double hx) {
            const MatrixDimSizeType m = v.size() - 1;
            return 2 * hx / x_max * v(m) + v(m - 1);
        }
    );

private:
    void initialize_optimization();
    double iterate(
        const Vector& current_row, MatrixDimSizeType col,
        double yield, double interest_rate, double volatility, double allocation
    ) const;
    Vector compute_current_row(const Matrix& portfolio_values, MatrixDimSizeType row_index) const;

    const double m_x_max;
    const FictiveBoundaryFunction m_neumann_condition;
    Matrix m_a_hx, m_a_hx2, m_im;
};

#endif