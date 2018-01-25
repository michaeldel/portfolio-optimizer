#include <utility>

#include "explicit_euler.hpp"
#include "../types.hpp"

ExplicitEulerPortfolioOptimizer::ExplicitEulerPortfolioOptimizer(
    unsigned int t_steps, double t_max,
    unsigned int x_steps, double x_min, double x_max,
    double x_0_condition,
    const BoundaryFunction v_T_condition,
    const FictiveBoundaryFunction neumann_condition
): PortfolioOptimizer(
    t_steps, t_max, x_steps, x_min, x_max, x_0_condition, v_T_condition
), m_x_max{ x_max }, m_neumann_condition{ neumann_condition } {}

static Matrix compute_a_hx_with_fictive_col(const Matrix& v, const Vector& xs, double ht, double hx) {
    const MatrixDimSizeType n = v.cols();
    Matrix a_h = Matrix::Zero(n, n + 1); // add a fictive-column for Neumann
    for (MatrixDimSizeType i = 0; i < a_h.rows(); i++) {
        a_h(i, i) = 1; // diagonal
        a_h(i, i + 1) = -1; // diagonal + 1
    }
    a_h *= - ht / hx;
    return a_h.array().colwise() * xs.array();
}

static Matrix compute_a_hx2_with_fictive_col(const Matrix& v, const Vector& xs, double ht, double hx) {
    const MatrixDimSizeType n = v.cols();
    Matrix a_h2 = Matrix::Zero(n, n + 1); // add a fictive-column for Neumann
    for (MatrixDimSizeType i = 0; i < a_h2.rows(); i++) {
        a_h2(i, i) = 2; // diagonal
        a_h2(i, i + 1) = -1; // diagonal + 1
        if (i != 0) // diagonal - 1
            a_h2(i, i - 1) = -1;
    }
    a_h2 *= -ht / (hx * hx);
    return a_h2.array().colwise() * xs.array().square().array();
}

void ExplicitEulerPortfolioOptimizer::initialize_optimization() {
    m_a_hx = compute_a_hx_with_fictive_col(m_mesh, m_xs, m_ht, m_hx);
    m_a_hx2 = compute_a_hx2_with_fictive_col(m_mesh, m_xs, m_ht, m_hx);
    m_im =  Matrix::Identity(m_mesh.cols(), m_mesh.cols() + 1);
}

double ExplicitEulerPortfolioOptimizer::iterate(
    const Vector& current_row, MatrixDimSizeType col,
    double yield, double interest_rate, double volatility, double allocation
) const {
    const double rho1 = allocation * yield + (1 - allocation) * interest_rate;
    const double rho2 = allocation * allocation * volatility * volatility / 2;

    return (rho1 * m_a_hx.row(col) + rho2 * m_a_hx2.row(col) + m_im.row(col)) * current_row;
}

Vector ExplicitEulerPortfolioOptimizer::compute_current_row(const Matrix& portfolio_values, MatrixDimSizeType row_index) const {
    Vector current_row(portfolio_values.cols() + 1);
    // append Neumann condition to fictive column for row i
    current_row << portfolio_values.row(row_index).transpose(), m_neumann_condition(portfolio_values.row(row_index).transpose(), m_x_max, m_hx);
    return current_row;
}