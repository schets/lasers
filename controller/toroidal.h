#ifndef TOROIDAL_H
#define TOROIDAL_H
#include "controller.h"
//!This class defines the toroidal search controller
/*!
 * This class defines a controller that can be used for searching a toroidal parameter space
 * It increments a set of variables at specified increments a set number of times. In the future,
 * this class will also be able to perform searches until the parameter space has been
 * successfully examined
 */
class toroidal:public controller{
    size_t iterations;
    size_t num_int;
    double initial_inc;
    double mul_fac;
    public:
    virtual void addvar(std::weak_ptr<variable> v); 
        virtual std::vector<std::string> dependencies() const;
        virtual void postprocess(std::map<std::string, std::shared_ptr<item> >& inval);
        virtual std::string type() const;
        virtual void control(comp* u, objective* obj);
        virtual char is_good();
        virtual void pre_set();
};
#endif
