#include <utility>

#include "explicit_euler.hpp"
#include "../types.hpp"

ExplicitEulerPortfolioOptimizer::ExplicitEulerPortfolioOptimizer(
    unsigned int t_steps, double t_max,
    unsigned int x_steps, double x_min, double x_max,
    double x_0_condition,
    const BoundaryFunction v_T_condition
): PortfolioOptimizer(
    t_steps, t_max, x_steps, x_min, x_max, x_0_condition, v_T_condition
), m_x_max{ x_max } {}

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

std::pair<Matrix, Matrix> ExplicitEulerPortfolioOptimizer::optimize(double yield, double interest_rate, double volatility) const {
    return optimize(yield, interest_rate, volatility, [](const Vector&, double , double) { return 0; });
}

std::pair<Matrix, Matrix> ExplicitEulerPortfolioOptimizer::optimize(
    double yield, double interest_rate, double volatility,
    const FictiveBoundaryFunction neumann_condition
) const {
    Matrix v = m_mesh;

    const Matrix a_hx = compute_a_hx_with_fictive_col(m_mesh, m_xs, m_ht, m_hx);
    const Matrix a_hx2 = compute_a_hx2_with_fictive_col(m_mesh, m_xs, m_ht, m_hx);
    const Matrix im =  Matrix::Identity(v.cols(), v.cols() + 1);

    Matrix alphas(v.rows() - 1, v.cols() - 1);

    for (MatrixDimSizeType i = v.rows() - 1; i > 0; i--) {
        Vector current_row(v.cols() + 1);
        // append Neumann condition to fictive column for row i
        current_row << v.row(i).transpose(), neumann_condition(v.row(i).transpose(), m_x_max, m_hx);

        // first column is Dirichlet condition and hence not taken
        for (MatrixDimSizeType j = 1; j < v.cols(); j++) {
            double alpha = -1;
            double max_v_ij = -1E99;
            for (double a = 0.2; a <= 0.9; a += 0.001) {
                const double rho1 = a * yield + (1 - a) * interest_rate;
                const double rho2 = a * a * volatility * volatility / 2;

                const double v_ij = (rho1 * a_hx.row(j) + rho2 * a_hx2.row(j) + im.row(j)) * current_row;
                if (v_ij > max_v_ij) {
                    max_v_ij = v_ij;
                    alpha = a;
                }
            }
            v(i - 1, j) = max_v_ij;
            alphas(i - 1, j - 1) = alpha;
        }
    }

    return std::pair<Matrix, Matrix>(v, alphas);
}