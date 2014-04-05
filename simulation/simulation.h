#ifndef SYSTEM_H
#define SYSTEM_H
#include "utils/defs.h"
#include "parser/item.h"
#include "controller/controller.h"
#include "objective/objective.h"
class writer;
/*!
 * This class defines the system being simulated. It simulates a run of the system
 * and returns a single value for the 
 */
class simulation:public item_dim{
    protected:
        controller* cont;
        objective* obj;
        writer* cur_writer;
    public:
        std::string out_f_name;
        virtual void parse(std::string inval){};
        //!This function simulates the system and returns the resulting score
        virtual double simulate() = 0;
        //i!This function returns the score of the system in its current state
        virtual double score() = 0;
        static item* create(std::string name);
        virtual void postprocess(std::map<std::string, std::shared_ptr<item>>& inval)=0;
        virtual std::vector<std::string> dependencies() const = 0;
};
#endif
