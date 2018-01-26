#ifndef DEFINE_MATLAB_M_OUTPUT_HPP
#define DEFINE_MATLAB_M_OUTPUT_HPP

#include "result_output.hpp"
#include "types.hpp"

class MatlabMOutput: public ResultOutput {
public:
    MatlabMOutput(const std::string& path);
    void write_result(
        const Vector& xs, const Vector& ts,
        const Matrix& portfolio_values, const Matrix& alphas
    ) const;

private:
    const std::string m_path;
};


#endif