#include "Relation.h"

Relation* Relation::Project(const std::vector<int>& indices) const {
    std::vector<std::string> newAttributes;
    std::set<Tuple> newTuples;
    for (int index : indices) {
        newAttributes.push_back(header.getAttributes().at(index));
    }
    for (Tuple t : tuples) {
        Tuple newTuple;
        for (int index : indices) {
            newTuple.Append(t.getValues().at(index));
        }
        newTuples.insert(newTuple);
    }
    Header h(newAttributes);
    Relation* newRelation = new Relation(name, h, newTuples);
    return newRelation;
}

Relation* Relation::Join(const Relation* rhs) const {
    // make new header
    std::vector<std::string> newAttributes;
    // include all attributes from lhs (this)
    for (std::string attribute : this->header.getAttributes()) {
        newAttributes.push_back(attribute);
    }
    std::vector<std::string> commonAttributes;
    // include all unique attributes from rhs
    for (std::string rhs_attribute : rhs->header.getAttributes()) {
        bool isUnique = true;
        for (std::string lhs_attribute : this->header.getAttributes()) {
            if (rhs_attribute == lhs_attribute) isUnique = false;
        }
        if (isUnique) newAttributes.push_back(rhs_attribute);
        else commonAttributes.push_back(rhs_attribute);
    }
    Header newHeader(newAttributes);
    Relation* newRelation = new Relation(name, newHeader);
    // add tuples
    // iterate through lhs tuples
    for (Tuple lhs_tuple : this->tuples) {
        // iterate through rhs tuples
        for (Tuple rhs_tuple : rhs->tuples) {
            // iterate through the rhs tuple and check if the tuples can join
            bool canJoin = true;
            for (int i = 0; i < int(rhs_tuple.getValues().size()); ++i) {
                // if the current attribute is an attribute in common
                if (std::count(commonAttributes.begin(), commonAttributes.end(), rhs->header.getAttributes().at(i)) > 0) {
                    // iterate through lhs attributes to find index of the common attribute
                    int lhs_index = -1;
                    for (int k = 0; k < int(this->header.getAttributes().size()); ++k) {
                        if (this->header.getAttributes().at(k) == rhs->header.getAttributes().at(i)) {
                            lhs_index = k;
                        }
                    }
                    // if the values at the common attribute are different, the tuples cannot join
                    if (lhs_tuple.getValues().at(lhs_index) != rhs_tuple.getValues().at(i)) canJoin = false;
                }
            }
            // if lhs_tuple and rhs_tuple can join
            if (canJoin) {
                // join lhs_tuple and rhs_tuple to make new tuple
                // add the values from lhs_tuple
                std::vector<std::string> newValues;
                for (int i = 0; i < int(lhs_tuple.getValues().size()); ++i) {
                    newValues.push_back(lhs_tuple.getValues().at(i));
                }
                // add the values from rhs_tuple where the attribute is unique
                for (int i = 0; i < int(rhs_tuple.getValues().size()); ++i) {
                    // if the attribute is unique
                    if (std::count(commonAttributes.begin(), commonAttributes.end(), rhs->header.getAttributes().at(i)) == 0) {
                        newValues.push_back(rhs_tuple.getValues().at(i));
                    }
                }
                Tuple newTuple(newValues);
                // add new tuple to new relation
                newRelation->AddTuple(newTuple);
            }
        }
    }
    return newRelation;
}

std::string Relation::ToString() const {
    std::ostringstream oss;
    oss << "Table name: " << name << ", ";
    oss << "Header:";
    for (std::string attribute : header.getAttributes()) {
        oss << " " << attribute;
    }
    oss << ", ";
    oss << "Tuples:";
    for (Tuple tuple : tuples) {
        oss << " " << tuple;
    }
    return oss.str();
}
