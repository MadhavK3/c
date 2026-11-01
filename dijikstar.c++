
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

const int INF = INT_MAX;

void dijkstra(int source, int V,
              vector<vector<pair<int, int>>>& adj) {

    vector<int> dist(V, INF);
    vector<int> parent(V, -1);

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {

        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d != dist[u])
            continue;

        for (auto edge : adj[u]) {

            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {

                dist[v] = dist[u] + weight;
                parent[v] = u;

                pq.push({dist[v], v});
            }
        }
    }

    cout << "\n========== SHORTEST PATH RESULTS ==========\n";

    for (int i = 0; i < V; i++) {

        cout << "\nDestination: " << char('A' + i);

        if (dist[i] == INF) {
            cout << "\nTravel Time: Unreachable\n";
            continue;
        }

        cout << "\nTravel Time: " << dist[i] << " minutes";

        vector<int> path;
        int current = i;

        while (current != -1) {
            path.push_back(current);
            current = parent[current];
        }

        reverse(path.begin(), path.end());

        cout << "\nShortest Path: ";

        for (int j = 0; j < path.size(); j++) {
            cout << char('A' + path[j]);

            if (j != path.size() - 1)
                cout << " -> ";
        }

        cout << "\n";
    }
}

int main() {

    int V, E;

    cout << "Enter number of intersections: ";
    cin >> V;

    cout << "Enter number of roads: ";
    cin >> E;

    vector<vector<pair<int, int>>> adj(V);

    cout << "\nAvailable Intersections:\n";

    for (int i = 0; i < V; i++) {
        cout << char('A' + i) << " ";
    }

    cout << "\n\nEnter roads (From To TravelTime):\n";

    for (int i = 0; i < E; i++) {

        char u, v;
        int w;

        cin >> u >> v >> w;

        int x = u - 'A';
        int y = v - 'A';

        adj[x].push_back({y, w});
        adj[y].push_back({x, w});
    }

    char source;

    cout << "\nEnter ambulance starting intersection: ";
    cin >> source;

    int src = source - 'A';

    dijkstra(src, V, adj);

    return 0;
}
