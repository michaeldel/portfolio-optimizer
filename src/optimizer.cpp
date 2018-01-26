#include "optimizer.hpp"
#include "types.hpp"

static Matrix generate_mesh(unsigned int rows, unsigned int cols, double x_0_boundary, const BoundaryFunction& v_T_boundary, const Vector& xs) {
    Matrix mesh(rows, cols);
    mesh.col(0) = Vector::Constant(mesh.rows(), x_0_boundary);
    auto temp = xs.unaryExpr(v_T_boundary).transpose();
    mesh.row(mesh.rows() - 1) = temp;
    return mesh;
}

PortfolioOptimizer::PortfolioOptimizer(
    unsigned int t_steps, double t_max,
    unsigned int x_steps, double x_min, double x_max,
    double x_0_condition,
    const BoundaryFunction v_T_condition
): m_ht{ t_max / t_steps }, m_hx{ (x_max - x_min) / x_steps },
   m_xs{ Vector::LinSpaced(x_steps + 1, x_min, x_max) },
   m_ts{ Vector::LinSpaced(t_steps + 1, 0.0, t_max) },
   m_mesh{ generate_mesh(t_steps + 1, x_steps + 1, x_0_condition, v_T_condition, m_xs) } {}

std::pair<Matrix, Matrix> PortfolioOptimizer::optimize(double yield, double interest_rate, double volatility) {
    initialize_optimization();
    Matrix v = m_mesh;
    Matrix alphas(v.rows() - 1, v.cols() - 1);

    for (MatrixDimSizeType i = v.rows() - 1; i > 0; i--) {
        const Vector current_row = compute_current_row(v, i);

        // first column is Dirichlet condition and hence not taken
        for (MatrixDimSizeType j = 1; j < v.cols(); j++) {
            double alpha = -1;
            double max_v_ij = -1E99;

            for (double a = 0.2; a <= 0.9; a += 0.01) {
                const double v_ij = iterate(
                    current_row, j,
                    yield, interest_rate, volatility, a
                );
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

Vector PortfolioOptimizer::compute_current_row(const Matrix& portfolio_values, MatrixDimSizeType row_index) const {
    return portfolio_values.row(row_index).transpose();
}