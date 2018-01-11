#ifndef DEFINE_CRANK_NICOLSON_HPP
#define DEFINE_CRANK_NICOLSON_HPP

#include <utility>

#include "../optimizer.hpp"
#include "../types.hpp"

class CrankNicolsonPortfolioOptimizer: public PortfolioOptimizer {
public:
    using PortfolioOptimizer::PortfolioOptimizer;
    std::pair<Matrix, Matrix> optimize(double yield, double interest_rate, double volatility) const;
};

#endif