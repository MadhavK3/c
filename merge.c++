
#include <bits/stdc++.h>
using namespace std;

struct Order {
    string orderID;
    string customerID;
    string timestamp;
    double amount;
    string status;
};


bool readCSV(vector<Order>& orders) {

    ifstream file("orders.csv");

    if (!file.is_open()) {
        cout << "Unable to open orders.csv\n";
        return false;
    }

    string line;

    getline(file, line);

    while (getline(file, line)) {

        if (line.empty()) continue;

        stringstream ss(line);

        Order x;
        string amountStr;

        getline(ss, x.orderID, ',');
        getline(ss, x.customerID, ',');
        getline(ss, x.timestamp, ',');
        getline(ss, amountStr, ',');
        getline(ss, x.status);

        try {
            x.amount = stod(amountStr);
        }
        catch (...) {
            cout << "Invalid amount. Row skipped.\n";
            continue;
        }

        if (x.orderID.empty() || x.customerID.empty() ||
            x.timestamp.empty() || x.status.empty() ||
            x.amount < 0) {

            cout << "Invalid row skipped.\n";
            continue;
        }

        orders.push_back(x);
    }

    file.close();

    return true;
}


void merge(vector<Order>& a, vector<Order>& temp,
           int left, int mid, int right) {

    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {

        if (a[i].timestamp <= a[j].timestamp)
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while (i <= mid) temp[k++] = a[i++];
    while (j <= right) temp[k++] = a[j++];

    for (int p = left; p <= right; p++)
        a[p] = temp[p];
}


void mergeSort(vector<Order>& a, vector<Order>& temp,
               int left, int right) {

    if (left >= right) return;

    int mid = (left + right) / 2;

    mergeSort(a, temp, left, mid);
    mergeSort(a, temp, mid + 1, right);

    merge(a, temp, left, mid, right);
}


void displayOrders(const vector<Order>& orders) {

    cout << "\nSorted Orders:\n";

    for (const auto& x : orders) {

        cout << x.orderID << " "
             << x.customerID << " "
             << x.timestamp << " "
             << x.amount << " "
             << x.status << "\n";
    }
}


void writeCSV(const vector<Order>& orders) {

    ofstream file("sorted_orders.csv");

    file << "OrderID,CustomerID,Timestamp,Amount,Status\n";

    for (const auto& x : orders) {

        file << x.orderID << ","
             << x.customerID << ","
             << x.timestamp << ","
             << x.amount << ","
             << x.status << "\n";
    }

    file.close();
}


int main() {

    vector<Order> orders;

    if (!readCSV(orders))
        return 1;

    if (orders.empty()) {
        cout << "No valid records found.\n";
        return 0;
    }

    vector<Order> temp(orders.size());

    mergeSort(orders, temp, 0, orders.size() - 1);

    displayOrders(orders);

    writeCSV(orders);

    cout << "\nSorted file created successfully.\n";

    return 0;
}
