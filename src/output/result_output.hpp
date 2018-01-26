#ifndef DEFINE_RESULT_OUTPUT_HPP
#define DEFINE_RESULT_OUTPUT_HPP

#include "types.hpp"

class ResultOutput {
public:
    virtual void write_result(
        const Vector& xs, const Vector& ts,
        const Matrix& portfolio_values, const Matrix& alphas
    ) const = 0;
};

#endif