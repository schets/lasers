#include "parser/item.h"
#include "toroidal.h"
#include "comp_funcs.h"
/*
 * This function returns the dependencies of the toroidal class
 * \note
 * This class depends on the parameter iterations, which is the number of iterations that
 * the search will perform
 *
 * @return returns the dependencies of the toroidal class,
 * along with the dependencies of the controller class
 *\sa controller::dependencies, item_dim::dependencies
 */
std::vector<std::string> toroidal::dependencies() const{
    std::string deps[] = {"iterations", "initial_inc", "mul_fac"};
    return appendvec(std::vector<std::string>(deps, deps+3), controller::dependencies());
}


/*!
 * Performs the postprocessing for toroidal,
 * which is setting the number of iterations that are to be performed
 * @param dat Map containing the input values
 * \sa controller::postprocess, item_dim::postprocess
 */
void toroidal::postprocess(std::map<std::string, item*>& dat){
    controller::postprocess(dat);
    num_int=0;
    int _iterations;
    dat["iterations"]->retrieve(&_iterations, this);
    iterations = _iterations;
    dat["mul_fac"]->retrieve(&mul_fac, this);
    if(mul_fac==0){
        err("Multiply factor, mul_fac, must not be equal to zero", 
               "toroidal::postprocess", "controller/toroidal.cpp",
               FATAL_ERROR);
    }
    dat["initial_inc"]->retrieve(&initial_inc, this);
    if(initial_inc==0){
        err("The initial increment, initial_inc, must not be equal to zero", 
               "toroidal::postprocess", "controller/toroidal.cpp",
               FATAL_ERROR);
    }
    //find the controllers place in the number of iterations
}

/*!
 * Returns the type of the class toroidal
 * @return Returns the type of the toroidal class, "toroidal"
 */
std::string toroidal::type() const{
    return "toroidal";
}

void toroidal::control(comp* u, objective* obj){
    double curinc = initial_inc;
    num_int++;
    if(vars.size() == 4 || 1){
        vars[0]->inc(sqrt(0.11)*100.0*PI/1000.0);
        vars[1]->inc(sqrt(0.13)*100.0*PI/1000.0);
        vars[2]->inc(sqrt(0.17)*100.0*PI/1000.0);
        vars[3]->inc(sqrt(0.19)*100.0*PI/1000.0);
        return;
    }
    for(auto& cvar : vars){
        cvar->inc(curinc);
        curinc *= mul_fac;
    }
}

void toroidal::addvar(variable* v){
    vars.push_back(v);
}
char toroidal::is_good(){
    return num_int <= iterations;
}
void toroidal::pre_set(){
    for(size_t i = 0; i < index*iterations; i++){
        double curinc=initial_inc;
        for(size_t j = 0; j < vars.size(); j++){
            vars[j]->inc(curinc);
            curinc *= mul_fac;
        }
    }
}
