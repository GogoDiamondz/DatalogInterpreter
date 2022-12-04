#include <sstream>
#include "DatalogProgram.h"

std::string DatalogProgram::ToString() {
    std::ostringstream oss;
    oss << "Schemes(" << schemes.size() << "):\n";
    for (Predicate scheme : schemes) {
        oss << "   " << scheme << std::endl;
    }
    oss << "Facts(" << facts.size() << "):\n";
    for (Predicate fact : facts) {
        oss << "   " << fact << "." << std::endl;
    }
    oss << "Rules(" << rules.size() << "):\n";
    for (Rule rule : rules) {
        oss << "   " << rule << "." << std::endl;
    }
    oss << "Queries(" << queries.size() << "):\n";
    for (Predicate query : queries) {
        oss << "   " << query << "?" << std::endl;
    }
    // The domain values are the strings (surrounded by quotes) that appear in the facts.
    std::vector<std::string> domain;
    for (Predicate fact : facts) {
        std::vector<Parameter> parameters = fact.getParameters();
        for (Parameter parameter : parameters) {
            if (parameter.getID().at(0) == '\'') {
                if (!Contains(parameter.getID(), domain)) {
                    domain.push_back(parameter.getID());
                }
            }
        }
    }
    std::sort(domain.begin(), domain.end());
    oss << "Domain(" << domain.size() << "):\n";
    for (std::string str : domain) {
        oss << "   " << str << std::endl;
    }
    return oss.str();
}