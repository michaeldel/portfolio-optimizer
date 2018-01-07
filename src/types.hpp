#ifndef DEFINE_TYPES_HPP
#define DEFINE_TYPES_HPP

#include <functional>

#include <Eigen/Dense>

using Matrix = Eigen::MatrixXd;
using MatrixDimSizeType = int;

using Vector = Eigen::VectorXd;
using RowVector = Eigen::RowVectorXd;

using BoundaryFunction = std::function<double(double)>;
using FictiveBoundaryFunction = std::function<double(const Vector& v, double x_max, double hx)>;

#endif