
#include <bits/stdc++.h>
using namespace std;


// FUNCTION 1: Find course index using its name
int findCourse(const vector<string>& courses, string name) {

    for (int i = 0; i < courses.size(); i++) {

        if (courses[i] == name) {
            return i;
        }
    }

    return -1;
}


// FUNCTION 2: Read courses and conflicts
bool readGraph(
    int V,
    vector<string>& courses,
    vector<vector<int>>& graph
) {

    cout << "\nEnter course names:\n";

    for (int i = 0; i < V; i++) {

        string name;
        cin >> name;

        if (findCourse(courses, name) != -1) {
            cout << "Duplicate course name. Enter again.\n";
            i--;
            continue;
        }

        courses.push_back(name);
    }

    int E;

    cout << "\nEnter number of course conflicts: ";
    cin >> E;

    if (E < 0) {
        cout << "Invalid number of conflicts.\n";
        return false;
    }

    cout << "\nEnter conflicting course pairs:\n";

    cout << "Example: DSA DBMS\n";

    for (int i = 0; i < E; i++) {

        string a, b;

        cin >> a >> b;

        int u = findCourse(courses, a);
        int v = findCourse(courses, b);

        if (u == -1 || v == -1 || u == v) {

            cout << "Invalid course pair. Enter again.\n";
            i--;
            continue;
        }

        graph[u][v] = 1;
        graph[v][u] = 1;
    }

    return true;
}


// FUNCTION 3: Check whether assigning a color is safe
bool isSafe(
    int vertex,
    int color,
    const vector<vector<int>>& graph,
    const vector<int>& colors,
    int V
) {

    for (int i = 0; i < V; i++) {

        if (graph[vertex][i] == 1 &&
            colors[i] == color) {

            return false;
        }
    }

    return true;
}


// FUNCTION 4: Graph Coloring using Backtracking
bool graphColoring(
    int vertex,
    int V,
    int m,
    const vector<vector<int>>& graph,
    vector<int>& colors
) {

    // All courses are assigned exam slots
    if (vertex == V) {
        return true;
    }

    // Try every available slot
    for (int color = 1; color <= m; color++) {

        if (isSafe(vertex, color, graph, colors, V)) {

            // Assign exam slot
            colors[vertex] = color;

            // Process next course
            if (graphColoring(
                vertex + 1, V, m, graph, colors
            )) {
                return true;
            }

            // Backtrack
            colors[vertex] = 0;
        }
    }

    return false;
}


// FUNCTION 5: Display final timetable
void displayTimetable(
    const vector<string>& courses,
    const vector<int>& colors
) {

    cout << "\n========== UNIVERSITY EXAM TIMETABLE ==========\n";

    cout << left
         << setw(25) << "Course Name"
         << setw(15) << "Exam Slot" << "\n";

    cout << string(40, '-') << "\n";

    for (int i = 0; i < courses.size(); i++) {

        cout << left
             << setw(25) << courses[i]
             << setw(15) << colors[i] << "\n";
    }

    cout << "\n========== SLOT-WISE SCHEDULE ==========\n";

    int maxColor = *max_element(colors.begin(), colors.end());

    for (int c = 1; c <= maxColor; c++) {

        cout << "\nSlot " << c << ": ";

        for (int i = 0; i < courses.size(); i++) {

            if (colors[i] == c) {
                cout << courses[i] << "  ";
            }
        }
    }

    cout << "\n";
}


// MAIN FUNCTION
int main() {

    int V, m;

    cout << "============================================\n";
    cout << " UNIVERSITY TIMETABLE - GRAPH COLORING\n";
    cout << "============================================\n";

    cout << "\nEnter number of courses: ";
    cin >> V;

    if (cin.fail() || V <= 0) {
        cout << "Invalid number of courses.\n";
        return 1;
    }

    vector<string> courses;

    vector<vector<int>> graph(
        V, vector<int>(V, 0)
    );

    // Read courses and conflicts
    if (!readGraph(V, courses, graph)) {
        return 1;
    }

    cout << "\nEnter number of available exam slots: ";
    cin >> m;

    if (cin.fail() || m <= 0) {
        cout << "Invalid number of exam slots.\n";
        return 1;
    }

    // Initially all courses are uncolored
    vector<int> colors(V, 0);

    // Apply Backtracking
    if (graphColoring(0, V, m, graph, colors)) {

        cout << "\nValid timetable found!\n";

        displayTimetable(courses, colors);
    }
    else {

        cout << "\nNo valid timetable possible with "
             << m << " exam slots.\n";
    }

    return 0;
}
