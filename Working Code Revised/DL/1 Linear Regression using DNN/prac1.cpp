#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

// Structure to represent a graph edge
struct Edge {
    int src, dest;
};

// Structure to represent a graph
class Graph {
public:
    vector<vector<int>> adjList;

    // Constructor
    Graph(vector<Edge> const &edges, int N) {
        adjList.resize(N);

        for (auto &edge : edges) {
            adjList[edge.src].push_back(edge.dest);
            adjList[edge.dest].push_back(edge.src);
        }
    }
};

// Parallel Breadth First Search
void parallelBFS(const Graph &graph, int source, vector<bool> &visited) {
    queue<int> q;
    q.push(source);
    visited[source] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        #pragma omp parallel for
        for (int neighbor : graph.adjList[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

// Parallel Depth First Search
void parallelDFS(const Graph &graph, int current, vector<bool> &visited) {
    visited[current] = true;

    #pragma omp parallel for
    for (int neighbor : graph.adjList[current]) {
        if (!visited[neighbor]) {
            parallelDFS(graph, neighbor, visited);
        }
    }
}

int main() {
    vector<Edge> edges = {{0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}};
    int N = 6; // Number of vertices in the graph

    Graph graph(edges, N);

    // Perform parallel BFS
    vector<bool> visitedBFS(N, false);
    parallelBFS(graph, 0, visitedBFS);

    // Perform parallel DFS
    vector<bool> visitedDFS(N, false);
    parallelDFS(graph, 0, visitedDFS);

    // Print BFS and DFS results
    cout << "BFS traversal: ";
    for (int i = 0; i < N; i++) {
        if (visitedBFS[i]) cout << i << " ";
    }
    cout << endl;

    cout << "DFS traversal: ";
    for (int i = 0; i < N; i++) {
        if (visitedDFS[i]) cout << i << " ";
    }
    cout << endl;

    return 0;
}
