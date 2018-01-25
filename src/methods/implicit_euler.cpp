#include <utility>

#include "implicit_euler.hpp"
#include "../types.hpp"

static Matrix compute_a_hx(const Matrix& v, const Vector& xs, double ht, double hx) {
    const MatrixDimSizeType n = v.cols();
    Matrix a_h = Matrix::Zero(n, n);
    for (MatrixDimSizeType i = 0; i < a_h.rows(); i++) {
        a_h(i, i) = 1; // diagonal
        if (i != a_h.rows() - 1)
            a_h(i, i + 1) = -1; // diagonal + 1
    }
    a_h *= ht / hx;
    return a_h.array().colwise() * xs.array();
}

static Matrix compute_a_hx2(const Matrix& v, const Vector& xs, double ht, double hx) {
    const MatrixDimSizeType n = v.cols();
    Matrix a_h2 = Matrix::Zero(n, n);
    for (MatrixDimSizeType i = 0; i < a_h2.rows(); i++) {
        a_h2(i, i) = 2; // diagonal
        if (i != a_h2.rows() - 1)
            a_h2(i, i + 1) = -1; // diagonal + 1
        if (i != 0) // diagonal - 1
            a_h2(i, i - 1) = -1;
    }
    a_h2 *= ht / (hx * hx);
    return a_h2.array().colwise() * xs.array().square().array();
}

void ImplicitEulerPortfolioOptimizer::initialize_optimization() {
    m_a_hx = compute_a_hx(m_mesh, m_xs, m_ht, m_hx);
    m_a_hx2 = compute_a_hx2(m_mesh, m_xs, m_ht, m_hx);
    m_im = Matrix::Identity(m_mesh.cols(), m_mesh.cols());
}

double ImplicitEulerPortfolioOptimizer::iterate(
    const Vector& current_row, MatrixDimSizeType col,
    double yield, double interest_rate, double volatility, double allocation
) const {
    const double rho1 = allocation * yield + (1 - allocation) * interest_rate;
    const double rho2 = allocation * allocation * volatility * volatility / 2;

    const Vector v_i = (rho1 * m_a_hx + rho2 * m_a_hx2 + m_im).colPivHouseholderQr().solve(current_row);
    return v_i(col);
}