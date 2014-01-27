#include "../utils/defs.h"
#include "integrator.h"
class rk45:public integrator{
    double dt_init, dt_min, dt_max, relerr, abserr;
    comp* restr f0, * restr f1, * restr f2, * restr f3;
    comp* restr f4, * restr f5, * restr f6;
    comp* restr u_calc;
    double* restr u_calc2;
    public:
    void print() const;
    void postprocess(std::map<std::string, item*>& dat);
    void parse(std::string inval);
    std::vector<std::string> dependencies() const;
    std::string type() const;
    int integrate(rhs* rh, comp* restr u, double t0, double tf);
    ~rk45();
};