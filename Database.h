#ifndef PROJECT_3_DATABASE_H
#define PROJECT_3_DATABASE_H
#include <map>
#include <iostream>
#include <sstream>
#include "Relation.h"


class Database {

private:
    std::map<std::string, Relation*> tables;

public:
    Database() = default;

    void SetTable(std::string key, Relation* relation) {
        tables[key] = relation;
    }

    Relation* GetTable(std::string key) {
        return tables[key];
    }

    std::string ToString() const {
        std::ostringstream oss;
        oss << "Database:" << std::endl;
        for (std::pair<std::string, Relation*> table : tables) {
            oss << table.second << std::endl;
        }
        return oss.str();
    }

    friend std::ostream& operator<< (std::ostream& os, Database d) {
        os << d.ToString();
        return os;
    }

    friend std::ostream& operator<< (std::ostream& os, Database* d) {
        os << d->ToString();
        return os;
    }
};


#endif //PROJECT_3_DATABASE_H
