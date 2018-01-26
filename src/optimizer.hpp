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
    std::pair<Matrix, Matrix> optimize(double yield, double interest_rate, double volatility);

    const Vector xs() const { return m_xs; };
    const Vector ts() const { return m_ts; };

protected:
    virtual void initialize_optimization() = 0;
    virtual double iterate(
        const Vector& current_row, MatrixDimSizeType col,
        double yield, double interest_rate, double volatility, double allocation
    ) const = 0;
    virtual Vector compute_current_row(const Matrix& portfolio_values, MatrixDimSizeType row_index) const;

    const double m_ht;
    const double m_hx;

    // order is important for initialization
    const Vector m_xs;
    const Vector m_ts;
    const Matrix m_mesh;
};

#endif