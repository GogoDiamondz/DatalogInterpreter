#include "Graph.h"

Graph* Graph::Reverse() {
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

void Graph::DFS_Forest(std::vector<int> order) {
    int SCCs_index = -1;
    // mark all as not visited
    for (auto pair : visited) visited[pair.first] = false;
    for (int i = 0; i < int(order.size()); ++i) {
        if (!visited[order.at(i)]) {
            DFS(order.at(i), ++SCCs_index);
        }
    }
}

void Graph::DFS(int node, int SCC_index) {
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