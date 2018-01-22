#ifndef DEFINE_RESULT_OUTPUT_HPP
#define DEFINE_RESULT_OUTPUT_HPP

#include "types.hpp"

class ResultOutput {
public:
    virtual void write_output(const Matrix& portfolio_values, const Matrix& alphas) const = 0;
};

#endif