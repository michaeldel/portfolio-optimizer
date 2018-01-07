#include "optimizer.hpp"
#include "types.hpp"

static Matrix generate_mesh(unsigned int rows, unsigned int cols, double x_0_boundary, const BoundaryFunction& v_T_boundary, const Vector& xs) {
    Matrix mesh(rows, cols);
    mesh.col(0) = Vector::Constant(mesh.rows(), x_0_boundary);
    auto temp = xs.unaryExpr(v_T_boundary).transpose();
    auto row = mesh.row(mesh.rows() - 1);
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
   m_mesh{ generate_mesh(t_steps + 1, x_steps + 1, x_0_condition, v_T_condition, m_xs) } {}