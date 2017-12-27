#include <iostream>

#include <Eigen/Dense>

using Eigen::Matrix3d;

int main() {
    Matrix3d m = Matrix3d::Random();
    std::cout << "Here is the random matrix generated:" << std::endl
              << m << std::endl;
    return 0;
}