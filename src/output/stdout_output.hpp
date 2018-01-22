#ifndef DEFINE_STDOUT_OUTPUT_HPP
#define DEFINE_STDOUT_OUTPUT_HPP

#include "result_output.hpp"
#include "types.hpp"

class StdOutOutput: public ResultOutput {
public:
    StdOutOutput(const std::string& annotation);
    void write_output(const Matrix& portfolio_values, const Matrix& alphas) const;

private:
    const std::string m_annotation;
};

#endif