#ifndef PREDICATE_H
#define PREDICATE_H
#include <string>
#include <vector>
#include <sstream>
#include "Parameter.h"

class Predicate
{
private:
    std::string ID;
    std::vector<Parameter> parameters;

public:
    Predicate(std::string id, std::vector<Parameter> p) : ID(id), parameters(p) {}

    ~Predicate() {}

    std::vector<Parameter> getParameters() const { return parameters; }
    std::string getID() const { return ID; }

    std::string ToString() const {
        std::ostringstream oss;
        oss << ID << "(";
        oss << parameters.at(0);
        for (int i = 1; i < int(parameters.size()); ++i) {
            oss << "," << parameters.at(i);
        }
        oss << ")";
        return oss.str();
    }

    friend std::ostream& operator<< (std::ostream& os, const Predicate p) {
        os << p.ToString();
        return os;
    }

};

#endif // PREDICATE_H

