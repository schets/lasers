#ifndef OBJECTIVE_H
#define OBJECTIVE_H
#include "parser/item.h"
#include "defs.h"
#include <map>
class objective:public item_dim{
    public:
        virtual double score(comp* invals) = 0;
        virtual std::vector<std::string> dependencies() const;
        virtual void postprocess(std::map<std::string, item*>& inval);
        virtual void parse(std::string inval){};
        static item* create(std::string nameval);
};
#endif