#include <fstream>

#include "matlab_m_output.hpp"
#include "types.hpp"

static void write_matrix_to_m_file(std::ofstream& file, const std::string& name, const Matrix& matrix) {
    file << name << " = [";
    for (MatrixDimSizeType i = 0; i < matrix.rows(); i++) {
        for (MatrixDimSizeType j = 0; j < matrix.cols(); j++) {
            file << matrix(i, j);
            if (j < matrix.cols() - 1) // if not last column
                file << ", ";
        }
        if (i < matrix.rows() - 1) // if not last row
                file << "; ";
    }
    file << "];\n";
}

MatlabMOutput::MatlabMOutput(const std::string& path) : m_path(path) {}

void MatlabMOutput::write_output(const Matrix& portfolio_values, const Matrix& alphas) const {
    std::ofstream file;
    file.open(m_path);
    write_matrix_to_m_file(file, "V", portfolio_values);
    write_matrix_to_m_file(file, "alphas", alphas);
    file.close();
}