# Portfolio Optimizer
## Usage
Compile the program using
```
make
```
By default, produced executable is located in the _bin_ folder.
You can then run the built executable with the following parameters:
```
portfolio_optimizer --help

USAGE:

   portfolio_optimizer  [-p <double>] [-o <string>] [-f <expliciteuler|impliciteuler|cranknicolson>] [-d <double>] [-s <double>] [-r <double>][-m <double>] [--][--version] [-h] <double for limits, unsigned int for steps> ...


Where:

   -p <double>,  --p <double>
     Utility function power

   -o <string>,  --output <string>
     Output Matlab .m file

   -f <expliciteuler|impliciteuler|cranknicolson>,  --fdmethod
      <expliciteuler|impliciteuler|cranknicolson>
     Finite differences method

   -d <double>,  --dirichlet <double>
     Dirichlet condition

   -s <double>,  --sigma <double>
     Volatility

   -r <double>,  --r <double>
     Interest rate

   -m <double>,  --mu <double>
     Yield

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   <double for limits, unsigned int for steps>  (accepted multiple times)
     xmin, xsteps, xmax and tsteps
```

## Example
Here is the output when using default parameters values (meaning the output would be the same when simply calling `portfolio_optimizer` without any other argument)
```
portfolio_optimizer 0 10 2 9 -p 0.5 -f expliciteuler -d 0 -s 0.2 -r 0.05 -m 0.06
Result alphas:
 0.2 0.41 0.45 0.46 0.47 0.48 0.48 0.48 0.47 0.47
 0.2 0.41 0.45 0.46 0.47 0.48 0.48 0.48 0.48 0.47
 0.2 0.41 0.45 0.46 0.47 0.48 0.48 0.48 0.48 0.47
 0.2 0.41 0.45 0.46 0.47 0.48 0.48 0.48 0.48 0.47
 0.2 0.41 0.45 0.46 0.47 0.48 0.48 0.48 0.48 0.47
 0.2 0.41 0.45 0.46 0.47 0.48 0.48 0.48 0.48 0.47
 0.2 0.41 0.45 0.46 0.47 0.48 0.48 0.48 0.48 0.47
 0.2 0.41 0.45 0.46 0.47 0.48 0.48 0.48 0.48 0.46
 0.2 0.41 0.45 0.46 0.47 0.48 0.48 0.48 0.48 0.46

Result portfolio values:
       0  0.45676  0.64744 0.793607 0.916789  1.02529  1.12336  1.21354  1.29746  1.37624  1.45066
       0 0.455686 0.645754 0.791469 0.914274  1.02245  1.12023  1.21013   1.2938  1.37236  1.44657
       0 0.454615 0.644074 0.789337 0.911767  1.01961   1.1171  1.20673  1.29016  1.36848  1.44248
       0 0.453547 0.642399 0.787212 0.909267  1.01678  1.11397  1.20334  1.28652  1.36462  1.43841
       0 0.452483 0.640729 0.785094 0.906775  1.01396  1.11086  1.19996  1.28289  1.36076  1.43435
       0 0.451422 0.639064 0.782981 0.904291  1.01116  1.10776  1.19659  1.27927  1.35692  1.43029
       0 0.450365 0.637404 0.780876 0.901813  1.00835  1.10467  1.19323  1.27567  1.35308  1.42626
       0 0.449311  0.63575 0.778776 0.899344  1.00556  1.10158  1.18989  1.27207  1.34926  1.42223
       0 0.448261   0.6341 0.776683 0.896882  1.00278  1.09851  1.18655  1.26849  1.34545  1.41822
       0 0.447214 0.632456 0.774597 0.894427        1  1.09545  1.18322  1.26491  1.34164  1.41421
```