#ifndef RULE_H
#define RULE_H
#include "Predicate.h"

class Rule
{
private:
    Predicate head;
    std::vector<Predicate> body;

public:
    Rule(Predicate h, std::vector<Predicate> b): head(h), body(b) {}

    ~Rule() {}

    Predicate getHead() const { return head; }
    std::vector<Predicate> getBody() const { return body; }

    std::string ToString() const {
        std::ostringstream oss;
        oss << head << " :- " << body.at(0);
        for (int i = 1; i < int(body.size()); ++i) {
            oss << "," << body.at(i);
        }
        return oss.str();
    }

    friend std::ostream& operator<< (std::ostream& os, const Rule r) {
        os << r.ToString();
        return os;
    }

};

#endif // RULE_H

