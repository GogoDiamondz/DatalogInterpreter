#ifndef PROJECT_3_HEADER_H
#define PROJECT_3_HEADER_H
#include <vector>
#include <iostream>


class Header {

private:
    std::vector<std::string> attributes;

public:
    Header() = default;

    Header(std::vector<std::string> a) : attributes(a) {}

    std::vector<std::string> getAttributes() const { return attributes; }
};


#endif //PROJECT_3_HEADER_H
