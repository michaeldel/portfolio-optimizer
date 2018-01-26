#include <iostream>

#include "stdout_output.hpp"
#include "types.hpp"

StdOutOutput::StdOutOutput(const std::string& annotation) : m_annotation(annotation) {}

void StdOutOutput::write_result(
    const Vector&, const Vector&, // we do not need these
    const Matrix& portfolio_values, const Matrix& alphas
) const {
    std::cout << m_annotation << " alphas:\n" << alphas << std::endl << std::endl;
    std::cout << m_annotation << " portfolio values:\n" << portfolio_values << std::endl;
}