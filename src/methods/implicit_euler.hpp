#ifndef DEFINE_IMPLICIT_EULER_HPP
#define DEFINE_IMPLICIT_EULER_HPP

#include <utility>

#include "../optimizer.hpp"
#include "../types.hpp"

class ImplicitEulerPortfolioOptimizer: public PortfolioOptimizer {
public:
    using PortfolioOptimizer::PortfolioOptimizer;
    std::pair<Matrix, Matrix> optimize(double yield, double interest_rate, double volatility) const;
};

#endif