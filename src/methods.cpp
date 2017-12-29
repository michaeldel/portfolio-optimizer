#include "methods.hpp"
#include "types.hpp"

static Matrix compute_a_hx_with_fictive_col(const Matrix& v, const Vector& xs, double ht, double hx, double k1) {
    const MatrixDimSizeType n = v.cols();
    Matrix a_h = Matrix::Zero(n, n + 1); // add a fictive-column for Neumann
    for (MatrixDimSizeType i = 0; i < a_h.rows(); i++) {
        a_h(i, i) = 1; // diagonal
        a_h(i, i + 1) = -1; // diagonal + 1
    }
    a_h *= - ht * k1 / hx;
    return a_h.array().colwise() * xs.array();
}

static Matrix compute_a_hx2_with_fictive_col(const Matrix& v, const Vector& xs, double ht, double hx, double k2) {
    const MatrixDimSizeType n = v.cols();
    Matrix a_h2 = Matrix::Zero(n, n + 1); // add a fictive-column for Neumann
    for (MatrixDimSizeType i = 0; i < a_h2.rows(); i++) {
        a_h2(i, i) = 2; // diagonal
        a_h2(i, i + 1) = -1; // diagonal + 1
        if (i != 0) // diagonal - 1
            a_h2(i, i - 1) = -1;
    }
    a_h2 *= -ht * k2 / (hx * hx);
    return a_h2.array().colwise() * xs.array().square().array();
}

Matrix explicit_euler(
    unsigned int t_steps, double t_max,
    unsigned int x_steps, double x_min, double x_max,
    const EdgeFunctionType phi, const FictiveEdgeFunctionType fictive_edge_function,
    double alpha, double mu, double r, double sigma
) {
    Matrix v(t_steps + 1, x_steps + 1);

    // xs vector with fictive last x (for Neumann condition)
    const Vector xs = Vector::LinSpaced(v.cols(), x_min, x_max);

    // leftmost col edge condition (homogen Dirichlet)
    v.col(0) = Vector::Zero(v.rows());

    // bottom row edge condition (Dirichlet)
    for (MatrixDimSizeType i = 0; i < v.cols(); i++)
        v(v.rows() - 1, i) = phi(xs(i));

    const double ht = t_max / t_steps;
    const double hx = (x_max - x_min) / x_steps;

    const double k1 = alpha * mu + (1 - alpha) * r;
    const double k2 = alpha * alpha * sigma * sigma / 2;

    const Matrix a = compute_a_hx_with_fictive_col(v, xs, ht, hx, k1)
                   + compute_a_hx2_with_fictive_col(v, xs, ht, hx, k2)
                   + Matrix::Identity(v.cols(), v.cols() + 1);

    for (MatrixDimSizeType i = v.rows() - 1; i > 0; i--) {
        Vector current_row(v.cols() + 1);
        // append Neumann condition to fictive column for row i
        current_row << v.row(i).transpose(), fictive_edge_function(v.row(i).transpose(), x_max, hx);
        // apply finite-differences scheme to row i to compute row i - 1
        v.row(i - 1) = a * current_row;
    }

    return v;
}