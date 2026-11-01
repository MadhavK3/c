
#include <bits/stdc++.h>
using namespace std;

const double INF = 1e18;

struct Edge {
    int to;
    double base_cost;
};


// FUNCTION 1: Build stages and assign vertices
bool buildstages(int S, vector<int>& stage_count,
                 vector<int>& stage_start,
                 vector<int>& stage_of, int& N) {

    cout << "Enter number of nodes in each stage:\n";

    for(int i = 0; i < S; i++) {

        cin >> stage_count[i];

        if(stage_count[i] <= 0) {
            cout << "Invalid number of nodes in a stage.\n";
            return false;
        }

        stage_start[i+1] =
            stage_start[i] + stage_count[i];
    }

    N = stage_start[S];

    if(N > 26) {
        cout << "Maximum 26 vertices supported.\n";
        return false;
    }

    cout << "\nTotal number of nodes: " << N;

    cout << "\nAvailable nodes:\n";

    for(int i = 0; i < S; i++) {

        cout << "Stage " << i+1 << ": ";

        for(int j = stage_start[i];
            j < stage_start[i+1]; j++) {

            cout << char('A' + j) << " ";
        }

        cout << "\n";
    }

    stage_of.resize(N);

    for(int s = 0; s < S; s++) {

        for(int i = stage_start[s];
            i < stage_start[s+1]; i++) {

            stage_of[i] = s;
        }
    }

    return true;
}


// FUNCTION 2: Read and validate directed edges
bool readEdges(int N, vector<int>& stage_of,
               vector<vector<Edge>>& adj) {

    int M;

    cout << "\nEnter number of edges: ";
    cin >> M;

    if(M < 0) {
        cout << "Invalid number of edges.\n";
        return false;
    }

    cout << "\nEnter edges as (From To Cost):\n";

    for(int i = 0; i < M; i++) {

        char a, b;
        double cost;

        cin >> a >> b >> cost;

        int u = a - 'A';
        int v = b - 'A';

        if(u < 0 || u >= N || v < 0 || v >= N) {
            cout << "Invalid node ID.\n";
            return false;
        }

        if(stage_of[v] != stage_of[u] + 1) {
            cout << "Invalid edge between stages.\n";
            return false;
        }

        if(cost < 0) {
            cout << "Negative edge cost not allowed.\n";
            return false;
        }

        // Directed edge
        adj[u].push_back({v, cost});
    }

    return true;
}


// FUNCTION 3: Calculate minimum cost using DP
void calculateDP(int S, int N,
                 vector<int>& stage_start,
                 vector<vector<Edge>>& adj,
                 vector<double>& best_cost,
                 vector<int>& next_node) {

    best_cost.assign(N, INF);
    next_node.assign(N, -1);

    // Base case: final-stage vertices
    for(int i = stage_start[S-1]; i < N; i++) {
        best_cost[i] = 0;
    }

    // Process stages backwards
    for(int s = S-2; s >= 0; s--) {

        for(int u = stage_start[s];
            u < stage_start[s+1]; u++) {

            for(auto& a : adj[u]) {

                int v = a.to;
                double w = a.base_cost;

                if(best_cost[v] != INF &&
                   w + best_cost[v] < best_cost[u]) {

                    best_cost[u] = w + best_cost[v];
                    next_node[u] = v;
                }
            }
        }
    }
}


// FUNCTION 4: Print minimum-cost path
bool printpath(int src, int N,
               vector<double>& best_cost,
               vector<int>& next_node) {

    if(best_cost[src] == INF) {
        cout << "\nNo path exists from source "
             << char('A' + src) << "\n";
        return false;
    }

    cout << "\nMinimum Cost Path: ";

    int cur = src;
    int steps = 0;

    while(cur != -1) {

        if(cur < 0 || cur >= N || steps++ >= N) {
            cout << "\nInvalid path encountered.\n";
            return false;
        }

        cout << char('A' + cur);

        cur = next_node[cur];

        if(cur != -1) {
            cout << " -> ";
        }
    }

    cout << "\nMinimum Total Cost: "
         << best_cost[src] << "\n";

    return true;
}


// MAIN FUNCTION
int main() {

    int S;

    cout << "Enter number of stages: ";
    cin >> S;

    if(S < 2 || S > 26) {
        cout << "Invalid number of stages.\n";
        return 0;
    }

    vector<int> stage_count(S);
    vector<int> stage_start(S+1, 0);
    vector<int> stage_of;

    int N;

    // Build stages
    if(!buildstages(S, stage_count, stage_start,
                    stage_of, N)) {
        return 0;
    }

    vector<vector<Edge>> adj(N);

    // Read graph edges
    if(!readEdges(N, stage_of, adj)) {
        return 0;
    }

    vector<double> best_cost(N, INF);
    vector<int> next_node(N, -1);

    // Calculate minimum cost
    calculateDP(S, N, stage_start, adj,
                best_cost, next_node);

    // Source is first vertex A
    printpath(0, N, best_cost, next_node);

    return 0;
}
