#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H
#include <vector>
#include <algorithm>
#include "Predicate.h"
#include "Rule.h"

class DatalogProgram
{
private:
    std::vector<Predicate> schemes;
    std::vector<Predicate> facts;
    std::vector<Rule> rules;
    std::vector<Predicate> queries;

    // returns true if str is in v, false otherwise
    bool Contains(std::string str, std::vector<std::string> v) const {
        for (std::string s : v) {
            if (s == str) return true;
        }
        return false;
    }

public:
    DatalogProgram() {}

    ~DatalogProgram() {}

    void AddScheme(Predicate s) {
        schemes.push_back(s);
    }

    void AddFact(Predicate f) {
        facts.push_back(f);
    }

    void AddRule(Rule r) {
        rules.push_back(r);
    }

    void AddQuery(Predicate q) {
        queries.push_back(q);
    }

    std::vector<Predicate> getSchemes() const { return schemes; }
    std::vector<Predicate> getFacts() const { return facts; }
    std::vector<Rule> getRules() const { return rules; }
    std::vector<Predicate> getQueries() const { return queries; }

    std::string ToString();

    friend std::ostream& operator<< (std::ostream& os, DatalogProgram d) {
        os << d.ToString();
        return os;
    }
};

#endif // DATALOGPROGRAM_H

