#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include <tclap/CmdLine.h>

#include "methods/crank_nicolson.hpp"
#include "methods/explicit_euler.hpp"
#include "methods/implicit_euler.hpp"
#include "optimizer.hpp"
#include "output/stdout_output.hpp"
#include "output/matlab_m_output.hpp"
#include "types.hpp"

enum Method {
    EXPLICIT_EULER, IMPLICIT_EULER, CRANK_NICOLSON
};

int main(int argc, char** argv) {
    TCLAP::CmdLine cmd("Portfolio optimizer", ' ', "1.0");

    TCLAP::ValueArg<double> yield_arg("m", "mu", "Yield", false, 0.06, "double");
    cmd.add(yield_arg);
    TCLAP::ValueArg<double> interest_rate_arg("r", "r", "Interest rate", false, 0.05, "double");
    cmd.add(interest_rate_arg);
    TCLAP::ValueArg<double> volatility_arg("s", "sigma", "Volatility", false, 0.2, "double");
    cmd.add(volatility_arg);


    TCLAP::ValueArg<double> dirichlet_condition_args("d", "dirichlet", "Dirichlet condition", false, 0.0, "double");
    cmd.add(dirichlet_condition_args);

    std::vector<std::string> allowed_methods;
    allowed_methods.push_back("expliciteuler");
    allowed_methods.push_back("impliciteuler");
    allowed_methods.push_back("cranknicolson");
    TCLAP::ValuesConstraint<std::string> constraints(allowed_methods);
    TCLAP::ValueArg<std::string> method_arg("f", "fdmethod", "Finite differences method", false, "expliciteuler", &constraints);
    cmd.add(method_arg);

    TCLAP::ValueArg<double> utility_power_arg("p", "p", "Utility function power", false, 0.5, "double");
    cmd.add(utility_power_arg);

    TCLAP::UnlabeledMultiArg<double> xt_config_arg("xtconfig", "xmin, xsteps, xmax and tsteps", false, "double for limits, unsigned int for steps");
    cmd.add(xt_config_arg);

    try {
        cmd.parse(argc, argv);
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }

    std::vector<double> xt_config(xt_config_arg.getValue());
    if (xt_config.size() == 0) { // default values
        xt_config.push_back(0); // xmin
        xt_config.push_back(10); // xsteps
        xt_config.push_back(2); // xmax
        xt_config.push_back(9); // tsteps
    } else if (xt_config.size() != 4) {
        std::cerr << "size: " << xt_config.size() << " : " << xt_config[0] << std::endl;
        std::cerr << "x and t config must be in the form: <xmin> <xsteps> <xmax> <tsteps>" << std::endl;
        return 1;
    }

    const double x_min = xt_config[0];
    const double x_max = xt_config[2];
    const unsigned int x_steps = static_cast<unsigned int>(xt_config[1]);

    const double t_max = 1;
    const unsigned int t_steps = static_cast<unsigned int>(xt_config[3]);

    const double mu = yield_arg.getValue();
    const double r = interest_rate_arg.getValue();
    const double sigma = volatility_arg.getValue();

    const double dirichlet_condition = dirichlet_condition_args.getValue();

    Method method;
    if (!method_arg.getValue().compare("expliciteuler"))
        method = EXPLICIT_EULER;
    else if (!method_arg.getValue().compare("impliciteuler"))
        method = IMPLICIT_EULER;
    else if (!method_arg.getValue().compare("cranknicolson"))
        method = CRANK_NICOLSON;
    else {
        std::cerr << "error: method not recognized" << std::endl;
        return 1;
    }

    const double p = utility_power_arg.getValue();
    const BoundaryFunction phi = [p](double x) { return std::pow(x, p); };
    const FictiveBoundaryFunction fictive_edge_function = [p](const Vector& v, double x_max, double hx) {
        const MatrixDimSizeType m = v.size() - 1;
        if (x_max >= 1)
            return 2 * hx * p / x_max * v(m) + v(m - 1);
        else
            return 2 * hx / x_max * v(m) + v(m - 1);
    };

    std::unique_ptr<PortfolioOptimizer> optimizer;
    switch (method) {
        case EXPLICIT_EULER:
            optimizer = std::make_unique<ExplicitEulerPortfolioOptimizer>(
                t_steps, t_max, x_steps, x_min, x_max, dirichlet_condition, phi, fictive_edge_function
            );
            break;
        case IMPLICIT_EULER:
            optimizer = std::make_unique<ImplicitEulerPortfolioOptimizer>(
                t_steps, t_max, x_steps, x_min, x_max, dirichlet_condition, phi
            );
            break;
        case CRANK_NICOLSON:
            optimizer = std::make_unique<CrankNicolsonPortfolioOptimizer>(
                t_steps, t_max, x_steps, x_min, x_max, dirichlet_condition, phi
            );
            break;
    }

    const auto [v, alphas] = optimizer->optimize(mu, r, sigma);
    const StdOutOutput soo("Results");
    soo.write_output(v, alphas);

    return 0;
}