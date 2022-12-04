#ifndef PROJECT_3_TUPLE_H
#define PROJECT_3_TUPLE_H
#include <iostream>
#include <vector>
#include <sstream>


class Tuple {
private:
    std::vector<std::string> values;

public:
    Tuple() = default;

    Tuple(std::vector<std::string> v) : values(v) {}

    bool operator< (const Tuple &rhs) const { return values < rhs.values; }

    void Append(std::string value) { values.push_back(value); }

    std::vector<std::string> getValues() { return values; }

    std::string ToString() const {
        std::ostringstream oss;
        oss << "(";
        if (!values.empty()) oss << values[0];
        for (int i = 1; i < int(values.size()); ++i) {
            oss << "," << values[i];
        }
        oss << ")";
        return oss.str();
    }

    friend std::ostream& operator<< (std::ostream& os, Tuple t) {
        os << t.ToString();
        return os;
    }
};


#endif //PROJECT_3_TUPLE_H
