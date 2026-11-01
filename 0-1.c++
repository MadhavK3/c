
#include <bits/stdc++.h>
using namespace std;

struct Item {
    int value;
    int weight;
};


// FUNCTION 1: Read item details
bool readitems(int n, vector<Item>& items) {

    cout << "Enter the value and weight of each item:\n";

    for(int i = 0; i < n; i++) {

        cout << "Item " << i + 1 << ": ";
        cin >> items[i].value >> items[i].weight;

        if(items[i].value < 0 || items[i].weight <= 0) {

            cout << "Invalid value or weight for item "
                 << i + 1 << ".\n";

            return false;
        }
    }

    return true;
}


// FUNCTION 2: Calculate DP table
bool calculateDP(int n, int W,
                 const vector<Item>& items,
                 vector<vector<int>>& dp) {

    for(int i = 1; i <= n; i++) {

        for(int w = 0; w <= W; w++) {

            // Include item if it fits
            if(items[i - 1].weight <= w) {

                dp[i][w] = max(
                    dp[i - 1][w],
                    dp[i - 1][w - items[i - 1].weight]
                    + items[i - 1].value
                );
            }

            // Otherwise exclude item
            else {

                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    return true;
}


// FUNCTION 3: Display DP table
void displayDP(int n, int W,
               const vector<vector<int>>& dp) {

    cout << "\nDynamic Programming Table:\n";

    cout << setw(8) << "Items";

    for(int w = 0; w <= W; w++) {
        cout << setw(5) << w;
    }

    cout << "\n";

    for(int i = 0; i <= n; i++) {

        cout << setw(8) << i;

        for(int w = 0; w <= W; w++) {
            cout << setw(5) << dp[i][w];
        }

        cout << "\n";
    }
}


// FUNCTION 4: Display selected items
void selectedItems(int n, int W,
                   const vector<Item>& items,
                   const vector<vector<int>>& dp) {

    cout << "\nSelected items:\n";

    int w = W;
    int totalValue = 0;
    int totalWeight = 0;

    for(int i = n; i > 0; i--) {

        // Check whether current item was selected
        if(dp[i][w] != dp[i - 1][w]) {

            cout << "Item " << i
                 << " (Value: " << items[i - 1].value
                 << ", Weight: " << items[i - 1].weight
                 << ")\n";

            w -= items[i - 1].weight;

            totalValue += items[i - 1].value;
            totalWeight += items[i - 1].weight;
        }
    }

    cout << "\nTotal value in knapsack: "
         << totalValue << endl;

    cout << "Total weight in knapsack: "
         << totalWeight << endl;

    cout << "Maximum possible value: "
         << dp[n][W] << endl;
}


// MAIN FUNCTION
int main() {

    int W, n;

    cout << "Enter the maximum weight of the knapsack: ";
    cin >> W;

    cout << "Enter the number of items: ";
    cin >> n;

    // Validate input
    if(cin.fail() || W < 0 || n < 0) {

        cout << "Invalid input." << endl;
        return 1;
    }

    vector<Item> items(n);

    if(!readitems(n, items)) {
        cout << "Invalid item details." << endl;
        return 1;
    }

    // Initialize DP table
    vector<vector<int>> dp(
        n + 1, vector<int>(W + 1, 0)
    );

    if(!calculateDP(n, W, items, dp)) {
        cout << "Error calculating DP table." << endl;
        return 1;
    }

    displayDP(n, W, dp);

    selectedItems(n, W, items, dp);

    return 0;
}
