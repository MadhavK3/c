
#include <bits/stdc++.h>
using namespace std;

const double INF = 1e18;

struct State {
    vector<int> path;
    vector<bool> visited;
    double cost;
    double time;
    double bound;
    int current;
};

struct Compare {
    bool operator()(const State& a, const State& b) const {
        return a.bound > b.bound;
    }
};

int n;
double maxTime;

vector<vector<double>> distanceMatrix;
vector<vector<double>> fuelRate;
vector<vector<double>> timeMatrix;
vector<vector<double>> costMatrix;

double calculateBound(const State& s) {
    double bound = s.cost;

    vector<int> nodes;
    nodes.push_back(s.current);

    for (int i = 1; i < n; i++) {
        if (!s.visited[i])
            nodes.push_back(i);
    }

    for (int u : nodes) {
        double minimum = INF;

        for (int v = 0; v < n; v++) {
            if (u == v)
                continue;

            if (v == 0 || !s.visited[v]) {
                if (costMatrix[u][v] < minimum)
                    minimum = costMatrix[u][v];
            }
        }

        if (minimum == INF)
            return INF;

        bound += minimum;
    }

    return bound;
}

double calculateTimeBound(const State& s) {
    double bound = s.time;

    vector<int> nodes;
    nodes.push_back(s.current);

    for (int i = 1; i < n; i++) {
        if (!s.visited[i])
            nodes.push_back(i);
    }

    for (int u : nodes) {
        double minimum = INF;

        for (int v = 0; v < n; v++) {
            if (u == v)
                continue;

            if ((v == 0 || !s.visited[v]) &&
                costMatrix[u][v] != INF) {
                minimum = min(minimum, timeMatrix[u][v]);
            }
        }

        if (minimum == INF)
            return INF;

        bound += minimum;
    }

    return bound;
}

void LCBranchAndBound() {
    priority_queue<State, vector<State>, Compare> pq;

    State root;

    root.path = {0};
    root.visited.assign(n, false);
    root.visited[0] = true;
    root.cost = 0;
    root.time = 0;
    root.current = 0;

    root.bound = calculateBound(root);

    if (root.bound == INF ||
        calculateTimeBound(root) > maxTime) {
        cout << "\nNo feasible route exists.\n";
        return;
    }

    pq.push(root);

    double minCost = INF;
    double bestTime = 0;
    vector<int> bestPath;

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        if (current.bound >= minCost)
            continue;

        if (calculateTimeBound(current) > maxTime)
            continue;

        if ((int)current.path.size() == n) {
            double returnCost = costMatrix[current.current][0];
            double returnTime = timeMatrix[current.current][0];

            if (returnCost == INF)
                continue;

            double totalCost = current.cost + returnCost;
            double totalTime = current.time + returnTime;

            if (totalTime <= maxTime && totalCost < minCost) {
                minCost = totalCost;
                bestTime = totalTime;
                bestPath = current.path;
                bestPath.push_back(0);
            }

            continue;
        }

        for (int city = 1; city < n; city++) {
            if (current.visited[city])
                continue;

            if (costMatrix[current.current][city] == INF)
                continue;

            State next = current;

            next.current = city;
            next.cost += costMatrix[current.current][city];
            next.time += timeMatrix[current.current][city];

            if (next.time > maxTime)
                continue;

            next.path.push_back(city);
            next.visited[city] = true;

            next.bound = calculateBound(next);

            if (next.bound >= minCost)
                continue;

            if (calculateTimeBound(next) > maxTime)
                continue;

            pq.push(next);
        }
    }

    if (bestPath.empty()) {
        cout << "\nNo feasible route exists within the time limit.\n";
        return;
    }

    cout << "\nOptimal Delivery Route: ";

    for (int city : bestPath)
        cout << char('A' + city) << " ";

    cout << "\nMinimum Fuel Cost: "
         << fixed << setprecision(2) << minCost;

    cout << "\nTotal Delivery Time: "
         << bestTime << endl;
}

int main() {
    cout << "Enter number of cities: ";
    cin >> n;

    if (n < 2 || n > 20) {
        cout << "Invalid number of cities.\n";
        return 0;
    }

    cout << "Enter maximum delivery time: ";
    cin >> maxTime;

    if (!cin || maxTime < 0) {
        cout << "Invalid delivery time.\n";
        return 0;
    }

    distanceMatrix.assign(n, vector<double>(n));
    fuelRate.assign(n, vector<double>(n));
    timeMatrix.assign(n, vector<double>(n));
    costMatrix.assign(n, vector<double>(n, INF));

    cout << "\nEnter distance matrix (-1 for no road):\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> distanceMatrix[i][j];

            if (!cin || (i != j && distanceMatrix[i][j] < -1) ||
                (i == j && distanceMatrix[i][j] != 0)) {
                cout << "Invalid distance matrix.\n";
                return 0;
            }
        }
    }

    cout << "\nEnter fuel cost per km matrix:\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> fuelRate[i][j];

            if (!cin || fuelRate[i][j] < 0) {
                cout << "Invalid fuel cost matrix.\n";
                return 0;
            }

            if (i != j && distanceMatrix[i][j] >= 0) {
                costMatrix[i][j] =
                    distanceMatrix[i][j] * fuelRate[i][j];
            }
        }
    }

    cout << "\nEnter travel time matrix (-1 for no road):\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> timeMatrix[i][j];

            if (!cin ||
                (i != j && timeMatrix[i][j] < -1) ||
                (i == j && timeMatrix[i][j] != 0)) {
                cout << "Invalid travel time matrix.\n";
                return 0;
            }

            if (i != j &&
                distanceMatrix[i][j] >= 0 &&
                timeMatrix[i][j] < 0) {
                costMatrix[i][j] = INF;
            }
        }
    }

    LCBranchAndBound();

    return 0;
}
