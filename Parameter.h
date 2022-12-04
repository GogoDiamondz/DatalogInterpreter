#ifndef PARAMETER_H
#define PARAMETER_H
#include <string>

class Parameter
{
private:
    std::string ID;

public:
    Parameter(std::string id) : ID(id) {}

    ~Parameter() {}

    std::string getID () const { return ID; }

    std::string ToString() const {
        return ID;
    }

    friend std::ostream& operator<< (std::ostream& os, const Parameter p) {
        os << p.ToString();
        return os;
    }

};

#endif // PARAMETER_H

