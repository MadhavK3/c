
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

// Structure to store relief item details
struct Item {
    int itemID;
    string itemName;
    double weight;
    double utility;
    bool divisible;
    double ratio;
    double takenWeight;
    double gainedUtility;
};

// Function to display all items
void displayItems(const vector<Item>& items) {

    cout << "\n========== AVAILABLE RELIEF ITEMS ==========\n";

    cout << left
         << setw(8)  << "ID"
         << setw(18) << "Item Name"
         << setw(12) << "Weight"
         << setw(12) << "Utility"
         << setw(12) << "Ratio"
         << setw(12) << "Divisible" << endl;

    cout << string(74, '-') << endl;

    for (const auto& item : items) {

        cout << left
             << setw(8)  << item.itemID
             << setw(18) << item.itemName
             << setw(12) << item.weight
             << setw(12) << item.utility
             << setw(12) << item.ratio
             << setw(12) << (item.divisible ? "Yes" : "No")
             << endl;
    }
}

// Function to compare items by ratio
bool compare(Item a, Item b) {
    return a.ratio > b.ratio;
}

// Fractional Knapsack function
void fractionalKnapsack(vector<Item>& items, double capacity) {

    // Sort items by descending utility/weight ratio
    sort(items.begin(), items.end(), compare);

    double remainingCapacity = capacity;
    double totalUtility = 0;

    cout << "\n========== ITEMS AFTER SORTING ==========\n";

    displayItems(items);

    // Select items according to capacity
    for (auto& item : items) {

        item.takenWeight = 0;
        item.gainedUtility = 0;

        if (remainingCapacity <= 0)
            break;

        // Take complete item if it fits
        if (item.weight <= remainingCapacity) {

            item.takenWeight = item.weight;
            item.gainedUtility = item.utility;

        }

        // Take fraction if item is divisible
        else if (item.divisible) {

            item.takenWeight = remainingCapacity;

            item.gainedUtility =
                item.ratio * item.takenWeight;
        }

        // Skip indivisible item if it does not fit
        else {
            continue;
        }

        remainingCapacity -= item.takenWeight;
        totalUtility += item.gainedUtility;
    }

    // Display final selected items
    cout << "\n========== FINAL SELECTED ITEMS ==========\n";

    cout << left
         << setw(18) << "Item Name"
         << setw(15) << "Total Weight"
         << setw(15) << "Taken Weight"
         << setw(15) << "Utility Gained" << endl;

    cout << string(63, '-') << endl;

    for (const auto& item : items) {

        if (item.takenWeight > 0) {

            cout << left
                 << setw(18) << item.itemName
                 << setw(15) << item.weight
                 << setw(15) << item.takenWeight
                 << setw(15) << item.gainedUtility
                 << endl;
        }
    }

    cout << "\n========== FINAL RESULT ==========\n";

    cout << "Boat Capacity       : " << capacity << " kg\n";
    cout << "Used Capacity       : " << capacity - remainingCapacity << " kg\n";
    cout << "Remaining Capacity  : " << remainingCapacity << " kg\n";

    cout << fixed << setprecision(2);

    cout << "Maximum Total Utility: "
         << totalUtility << endl;
}

// Main function
int main() {

    int n;
    double capacity;

    vector<Item> items;

    cout << "========================================\n";
    cout << " EMERGENCY RELIEF SUPPLY DISTRIBUTION\n";
    cout << " FRACTIONAL KNAPSACK ALGORITHM\n";
    cout << "========================================\n";

    // Input boat capacity
    cout << "\nEnter boat capacity (kg): ";
    cin >> capacity;

    if (capacity <= 0) {
        cout << "Invalid capacity!\n";
        return 0;
    }

    // Input number of items
    cout << "Enter number of relief items: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number of items!\n";
        return 0;
    }

    // Take item details
    for (int i = 0; i < n; i++) {

        Item item;

        item.itemID = i + 1;

        cout << "\nEnter details for Item " << i + 1 << endl;

        cout << "Item Name: ";
        cin >> item.itemName;

        cout << "Weight (kg): ";
        cin >> item.weight;

        cout << "Utility Value: ";
        cin >> item.utility;

        int choice;

        cout << "Can this item be divided? (1 = Yes, 0 = No): ";
        cin >> choice;

        if (item.weight <= 0 || item.utility < 0 ||
            (choice != 0 && choice != 1)) {
            cout << "Invalid item details!\n";
            return 0;
        }

        item.divisible = (choice == 1);

        // Calculate utility-to-weight ratio
        item.ratio = item.utility / item.weight;

        item.takenWeight = 0;
        item.gainedUtility = 0;

        items.push_back(item);
    }

    // Display original items
    cout << fixed << setprecision(2);
    displayItems(items);

    // Apply Fractional Knapsack
    fractionalKnapsack(items, capacity);

    return 0;
}
