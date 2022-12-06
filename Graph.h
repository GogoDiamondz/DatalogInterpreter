#ifndef GRAPH_H
#define GRAPH_H
#include <map>
#include <set>
#include <vector>
#include <sstream>

class Graph {
private:
    std::map<int, std::set<int>> adjacencyList;
    std::map<int, bool> visited;
    std::vector<int> postOrder;
    std::map<int, std::set<int>> SCCs;

public:
    Graph() = default;

    void MarkVisited(int key) {
        visited[key] = true;
    }

    // adds a node and its adjacent nodes to the graph
    void AddPair(int node, std::set<int> adjacentNodes) {
        adjacencyList[node] = adjacentNodes;
        visited[node] = false;
    }

    Graph* Reverse();

    // takes in a vector of ints as an order
    // populates postorder and search trees (in SCCs)
    void DFS_Forest(std::vector<int> order);

    void DFS(int node, int SCC_index);

    std::vector<int> GetPostOrder() const { return postOrder; }

    std::map<int, std::set<int>> GetSCCs() const { return SCCs; }

    std::map<int, std::set<int>> GetAdjacencyList() const { return adjacencyList; }

    std::string ToString() const;

    friend std::ostream& operator<< (std::ostream& os, const Graph* g) {
        os << g->ToString();
        return os;
    }

};


#endif //GRAPH_H
