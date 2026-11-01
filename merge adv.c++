
#include <bits/stdc++.h>
using namespace std;



string trim(string s) {

    size_t start = s.find_first_not_of(" \t\r\n");

    if (start == string::npos)
        return "";

    size_t end = s.find_last_not_of(" \t\r\n");

    return s.substr(start, end - start + 1);
}



bool parseCSV(const string& line, vector<string>& fields) {

    fields.clear();

    string field;
    bool inQuotes = false;

    for (int i = 0; i < line.size(); i++) {

        char ch = line[i];

        if (ch == '"') {

            if (inQuotes && i + 1 < line.size()
                && line[i + 1] == '"') {

                field += '"';
                i++;
            }
            else {
                inQuotes = !inQuotes;
            }
        }

        else if (ch == ',' && !inQuotes) {

            fields.push_back(trim(field));
            field.clear();
        }

        else {
            field += ch;
        }
    }

    if (inQuotes)
        return false;

    fields.push_back(trim(field));

    return true;
}



bool readCSV(
    string filename,
    vector<string>& headers,
    vector<vector<string>>& data
) {

    ifstream file(filename);

    if (!file.is_open()) {

        cout << "Unable to open file!\n";
        return false;
    }

    string line;

  
    if (!getline(file, line)) {

        cout << "Empty CSV file.\n";
        return false;
    }

    if (!parseCSV(line, headers)) {

        cout << "Invalid CSV header.\n";
        return false;
    }

    if (!headers.empty() && headers[0].size() >= 3 &&
        (unsigned char)headers[0][0] == 0xEF &&
        (unsigned char)headers[0][1] == 0xBB &&
        (unsigned char)headers[0][2] == 0xBF) {

        headers[0] = headers[0].substr(3);
    }

    for (auto& h : headers)
        h = trim(h);

    int lineNo = 1;
    int skipped = 0;

   
    while (getline(file, line)) {

        lineNo++;

        if (trim(line).empty())
            continue;

        vector<string> row;

        if (!parseCSV(line, row)) {

            cout << "Malformed CSV at line "
                 << lineNo << ". Skipped.\n";

            skipped++;
            continue;
        }

       
        if (row.size() > headers.size()) {

            cout << "Extra columns at line "
                 << lineNo << ". Skipped.\n";

            skipped++;
            continue;
        }

      
        while (row.size() < headers.size()) {
            row.push_back("");
        }

        data.push_back(row);
    }

    file.close();

    cout << "\nCSV loaded successfully!\n";
    cout << "Valid rows: " << data.size() << "\n";
    cout << "Skipped rows: " << skipped << "\n";

    return true;
}



bool compareValues(
    const string& a,
    const string& b,
    int type,
    bool ascending
) {

    bool result = false;

    if (type == 1) {

       
        try {

            size_t p1, p2;

            double x = stod(a, &p1);
            double y = stod(b, &p2);

            if (p1 != trim(a).size() ||
                p2 != trim(b).size() ||
                !isfinite(x) || !isfinite(y)) {

                result = a < b;
            }
            else {
                result = x < y;
            }
        }
        catch (...) {
            result = a < b;
        }
    }

    else {

        
        result = a < b;
    }

    return ascending ? result : !result && a != b;
}



void merge(
    vector<vector<string>>& data,
    vector<vector<string>>& temp,
    int left,
    int mid,
    int right,
    int column,
    int type,
    bool ascending
) {

    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {

        if (compareValues(
            data[i][column],
            data[j][column],
            type,
            ascending
        )) {

            temp[k++] = data[i++];
        }

        else {

            temp[k++] = data[j++];
        }
    }

    while (i <= mid)
        temp[k++] = data[i++];

    while (j <= right)
        temp[k++] = data[j++];

    for (int p = left; p <= right; p++) {
        data[p] = temp[p];
    }
}



void mergeSort(
    vector<vector<string>>& data,
    vector<vector<string>>& temp,
    int left,
    int right,
    int column,
    int type,
    bool ascending
) {

    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    mergeSort(data, temp, left, mid,
              column, type, ascending);

    mergeSort(data, temp, mid + 1, right,
              column, type, ascending);

    merge(data, temp, left, mid, right,
          column, type, ascending);
}



void displayData(
    const vector<string>& headers,
    const vector<vector<string>>& data
) {

    cout << "\n========== SORTED DATA ==========\n";

    for (auto h : headers) {
        cout << left << setw(20) << h;
    }

    cout << "\n";

    for (int i = 0; i < headers.size() * 20; i++)
        cout << "-";

    cout << "\n";

    for (const auto& row : data) {

        for (auto value : row) {
            cout << left << setw(20) << value;
        }

        cout << "\n";
    }
}



string escapeCSV(const string& s) {

    if (s.find_first_of(",\"\n\r") == string::npos)
        return s;

    string result = "\"";

    for (char ch : s) {

        if (ch == '"')
            result += "\"\"";
        else
            result += ch;
    }

    result += "\"";

    return result;
}



bool writeCSV(
    const string& filename,
    const vector<string>& headers,
    const vector<vector<string>>& data
) {

    ofstream file(filename);

    if (!file.is_open())
        return false;

    for (int i = 0; i < headers.size(); i++) {

        if (i > 0)
            file << ",";

        file << escapeCSV(headers[i]);
    }

    file << "\n";

    for (const auto& row : data) {

        for (int i = 0; i < row.size(); i++) {

            if (i > 0)
                file << ",";

            file << escapeCSV(row[i]);
        }

        file << "\n";
    }

    file.close();

    return true;
}



int main() {

    string filename;

    vector<string> headers;
    vector<vector<string>> data;

    cout << "Enter CSV filename: ";
    cin >> filename;

    if (!readCSV(filename, headers, data))
        return 1;

    if (data.empty()) {

        cout << "No records available for sorting.\n";
        return 0;
    }

   
    cout << "\nAvailable columns:\n";

    for (int i = 0; i < headers.size(); i++) {

        cout << i + 1 << ". " << headers[i] << "\n";
    }

    int choice;

    cout << "\nSelect column number to sort: ";
    cin >> choice;

    if (choice < 1 || choice > headers.size()) {

        cout << "Invalid column selection.\n";
        return 1;
    }

    int column = choice - 1;

    int type;

    cout << "\nSelect data type:\n";
    cout << "1. Numeric\n";
    cout << "2. Text / Date\n";
    cout << "Enter choice: ";

    cin >> type;

    if (type != 1 && type != 2) {

        cout << "Invalid data type.\n";
        return 1;
    }

    int order;

    cout << "\nSelect sorting order:\n";
    cout << "1. Ascending\n";
    cout << "2. Descending\n";
    cout << "Enter choice: ";

    cin >> order;

    if (order != 1 && order != 2) {

        cout << "Invalid sorting order.\n";
        return 1;
    }

    bool ascending = (order == 1);

   
    vector<vector<string>> temp(data.size());

    mergeSort(
        data, temp,
        0, data.size() - 1,
        column, type, ascending
    );

   
    displayData(headers, data);

   
    if (writeCSV("sorted_output.csv", headers, data)) {

        cout << "\nSorted data saved to sorted_output.csv\n";
    }
    else {

        cout << "\nUnable to save output file.\n";
    }

    return 0;
}
