#include "Graph.h"

std::string Graph::ToString() const {
    std::ostringstream oss;
    for (std::pair<int, std::set<int>> edges : adjacencyList) {
        oss << "R" << edges.first << ":";
        auto iter = edges.second.begin();
        if (iter != edges.second.end()) {
            oss << "R" << *iter;
            ++iter;
        }
        while (iter != edges.second.end()) {
            oss << ",R" << *iter;
            ++iter;
        }
        oss << std::endl;
    }
    return oss.str();
}