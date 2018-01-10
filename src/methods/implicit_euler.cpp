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

std::pair<Matrix, Matrix> ImplicitEulerPortfolioOptimizer::optimize(double yield, double interest_rate, double volatility) const {
    Matrix v = m_mesh;

    const Matrix a_hx = compute_a_hx(m_mesh, m_xs, m_ht, m_hx);
    const Matrix a_hx2 = compute_a_hx2(m_mesh, m_xs, m_ht, m_hx);
    const Matrix im = Matrix::Identity(v.cols(), v.cols());

    Matrix alphas(v.rows() - 1, v.cols() - 1);

    for (MatrixDimSizeType i = v.rows() - 1; i > 0; i--) {
        const Vector current_row = v.row(i).transpose();

        // first column is Dirichlet condition and hence not taken
        for (MatrixDimSizeType j = 1; j < v.cols(); j++) {
            double alpha = -1;
            double max_v_ij = -1E99;
            for (double a = 0.2; a <= 0.9; a += 0.01) {
                const double rho1 = a * yield + (1 - a) * interest_rate;
                const double rho2 = a * a * volatility * volatility / 2;

                const Vector v_i = (rho1 * a_hx + rho2 * a_hx2 + im).colPivHouseholderQr().solve(current_row);
                const double v_ij = v_i(j);
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