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

    Graph* Reverse() {
        Graph* reverseGraph = new Graph();
        for (auto pair1 : adjacencyList) {
            std::set<int> adjacentNodes;
            int targetNode = pair1.first;
            for (auto pair2 : adjacencyList) {
                for (auto node : pair2.second) {
                    if (node == targetNode) adjacentNodes.insert(pair2.first);
                }
            }
            reverseGraph->AddPair(targetNode, adjacentNodes);
        }
        return reverseGraph;
    }

    // takes in a vector of ints as an order
    // populates postorder and returns the search trees (as a vector of sets)
    void DFS_Forest(std::vector<int> order) {
        int SCCs_index = -1;
        // mark all as not visited
        for (auto pair : visited) visited[pair.first] = false;
        for (int i = 0; i < int(order.size()); ++i) {
            if (!visited[order.at(i)]) {
                DFS(order.at(i), ++SCCs_index);
            }
        }
    }

    void DFS(int node, int SCC_index) {
        visited[node] = true;
        SCCs[SCC_index].insert(node);
        // for each neighbor of node
        for (int neighbor : adjacencyList[node]) {
            // if neighbor is not visited
            if (!visited[neighbor]) {
                DFS(neighbor, SCC_index);
            }
        }
        postOrder.push_back(node);
    }

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
