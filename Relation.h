#ifndef PROJECT_3_RELATION_H
#define PROJECT_3_RELATION_H
#include <set>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Tuple.h"
#include "Header.h"


class Relation {

private:
    std::string name;
    Header header;
    std::set<Tuple> tuples;

public:
    Relation() = default;

    Relation(std::string n, Header h) : name(n), header(h) {}

    Relation(std::string n, Header h, std::set<Tuple> t) : name(n), header(h), tuples(t) {}

    Relation* operator= (Relation* r) {
        this->name = r->name;
        this->header = r->header;
        this->tuples = r->tuples;
        return this;
    }

    bool IsEmpty() const {
        if (tuples.empty()) { return true; }
        return false;
    }

    int Size() const {
        return tuples.size();
    }

    void AddTuple(Tuple t) {
        tuples.insert(t);
    }

    void SetName(std::string n) {
        name = n;
    }

    Header getHeader() const { return header; }

    std::set<Tuple> getTuples() const { return tuples; }

    Relation* Select(int index, std::string value) const {
        Relation* newRelation = new Relation(name, header);
        for (Tuple t : tuples) {
            if (t.getValues().at(index) == value) {
                newRelation->AddTuple(t);
            }
        }
        return newRelation;
    }

    Relation* Select(int index1, int index2) {
        Relation* newRelation = new Relation(name, header);
        for (Tuple t : tuples) {
            if (t.getValues().at(index1) == t.getValues().at(index2)) {
                newRelation->AddTuple(t);
            }
        }
        return newRelation;
    }

    Relation* Project(const std::vector<int>& indices) const;

    Relation* Rename(const std::vector<std::string>& newAttributes) const {
        Header newHeader(newAttributes);
        Relation* newRelation = new Relation(name, newAttributes, tuples);
        return newRelation;
    }

    Relation* Join(const Relation* rhs) const;

    // returns true if the database changes
    bool Union(const Relation* rhs) {
        int oldSize = Size();
        for (Tuple rhs_tuple : rhs->tuples) {
            tuples.insert(rhs_tuple);
        }
        // if the size of the relation changed, the database changed
        if (oldSize != Size()) return true;
        return false;
    }

    std::string ToString() const;

    friend std::ostream& operator<< (std::ostream& os, Relation* r) {
        os << r->ToString();
        return os;
    }

    friend std::ostream& operator<< (std::ostream& os, Relation r) {
        os << r.ToString();
        return os;
    }
};


#endif //PROJECT_3_RELATION_H
