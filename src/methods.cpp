#include "methods.hpp"
#include "types.hpp"

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

std::tuple<Matrix, Matrix> explicit_euler(
    unsigned int t_steps, double t_max,
    unsigned int x_steps, double x_min, double x_max,
    const EdgeFunctionType phi, const FictiveEdgeFunctionType fictive_edge_function,
    double mu, double r, double sigma
) {
    Matrix v(t_steps + 1, x_steps + 1);
    const Vector xs = Vector::LinSpaced(v.cols(), x_min, x_max);

    // leftmost col edge condition (homogen Dirichlet)
    v.col(0) = Vector::Zero(v.rows());

    // bottom row edge condition (Dirichlet)
    for (MatrixDimSizeType i = 0; i < v.cols(); i++)
        v(v.rows() - 1, i) = phi(xs(i));

    const double ht = t_max / t_steps;
    const double hx = (x_max - x_min) / x_steps;

    const Matrix a_hx = compute_a_hx_with_fictive_col(v, xs, ht, hx);
    const Matrix a_hx2 = compute_a_hx2_with_fictive_col(v, xs, ht, hx);
    const Matrix im =  Matrix::Identity(v.cols(), v.cols() + 1);

    Matrix alphas(v.rows() - 1, v.cols() - 1);

    for (MatrixDimSizeType i = v.rows() - 1; i > 0; i--) {
        Vector current_row(v.cols() + 1);
        // append Neumann condition to fictive column for row i
        current_row << v.row(i).transpose(), fictive_edge_function(v.row(i).transpose(), x_max, hx);

        // first column is Dirichlet condition and hence not taken
        for (MatrixDimSizeType j = 1; j < v.cols(); j++) {
            double alpha = -1;
            double max_v_ij = -1E99;
            for (double a = 0.2; a <= 0.9; a += 0.001) {
                const double rho1 = a * mu + (1 - a) * r;
                const double rho2 = a * a * sigma * sigma / 2;

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

    return std::make_tuple(v, alphas);
}