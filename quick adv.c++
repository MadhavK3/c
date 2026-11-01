
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>
#include <cctype>
#include <cmath>
#include <iomanip>
using namespace std;

struct Movie {
    int movieID;
    string movieName;
    double imdbRating;
    int releaseYear;
    int watchTime;
};

int choice;
mt19937 rng(random_device{}());

string trim(string s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos)
        return "";

    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string lowerCase(string s) {
    for (char& c : s)
        c = (char)tolower((unsigned char)c);

    return s;
}

vector<string> parseCSV(const string& line) {
    vector<string> fields;
    string field;
    bool insideQuotes = false;

    for (int i = 0; i < (int)line.size(); i++) {
        char c = line[i];

        if (c == '"') {
            if (insideQuotes && i + 1 < (int)line.size()
                && line[i + 1] == '"') {
                field += '"';
                i++;
            }
            else {
                insideQuotes = !insideQuotes;
            }
        }
        else if (c == ',' && !insideQuotes) {
            fields.push_back(trim(field));
            field.clear();
        }
        else {
            field += c;
        }
    }

    if (insideQuotes)
        return {};

    fields.push_back(trim(field));
    return fields;
}

bool getNumber(string s, double& value) {
    s = trim(s);

    if (s.empty())
        return false;

    try {
        size_t pos;
        value = stod(s, &pos);

        if (!isfinite(value))
            return false;

        return true;
    }
    catch (...) {
        return false;
    }
}

bool getYear(string s, int& year) {
    s = trim(s);

    for (int i = 0; i + 4 <= (int)s.size(); i++) {
        string part = s.substr(i, 4);

        if (all_of(part.begin(), part.end(),
                   [](unsigned char c) { return isdigit(c); })) {
            int y = stoi(part);

            if (y >= 1888 && y <= 2100) {
                year = y;
                return true;
            }
        }
    }

    return false;
}

int findColumn(const vector<string>& header,
               vector<string> aliases) {

    for (int i = 0; i < (int)header.size(); i++) {
        string h = lowerCase(trim(header[i]));

        for (string alias : aliases) {
            if (h == alias)
                return i;
        }
    }

    return -1;
}

bool readCSV(vector<Movie>& movies) {

    ifstream file("movies.csv");

    if (!file.is_open()) {
        cout << "Error: Unable to open movies.csv\n";
        return false;
    }

    string line;

    if (!getline(file, line)) {
        cout << "CSV file is empty.\n";
        return false;
    }

    if (line.size() >= 3 &&
        (unsigned char)line[0] == 0xEF &&
        (unsigned char)line[1] == 0xBB &&
        (unsigned char)line[2] == 0xBF) {
        line.erase(0, 3);
    }

    vector<string> header = parseCSV(line);

    int idCol = findColumn(header,
        {"movieid", "id", "movie_id"});

    int nameCol = findColumn(header,
        {"moviename", "title", "name", "movie_name"});

    int ratingCol = findColumn(header,
        {"imdbrating", "rating", "imdb_rating"});

    int yearCol = findColumn(header,
        {"releaseyear", "year", "release_year"});

    int timeCol = findColumn(header,
        {"watchtime", "runtime", "duration", "watch_time"});

    if (nameCol == -1 || ratingCol == -1 ||
        yearCol == -1 || timeCol == -1) {
        cout << "Error: Required CSV columns are missing.\n";
        cout << "Required: MovieName, IMDbRating, ReleaseYear, WatchTime\n";
        return false;
    }

    int validRows = 0;
    int skippedRows = 0;
    int generatedID = 1;

    while (getline(file, line)) {

        if (trim(line).empty())
            continue;

        vector<string> fields = parseCSV(line);

        if (fields.empty()) {
            skippedRows++;
            continue;
        }

        int maxCol = max({nameCol, ratingCol, yearCol, timeCol, idCol});

        if ((int)fields.size() <= maxCol) {
            skippedRows++;
            continue;
        }

        Movie m;
        double rating, watchTime;
        int year;

        m.movieName = trim(fields[nameCol]);

        if (m.movieName.empty()) {
            skippedRows++;
            continue;
        }

        if (!getNumber(fields[ratingCol], rating) ||
            rating < 0 || rating > 10) {
            skippedRows++;
            continue;
        }

        if (!getYear(fields[yearCol], year)) {
            skippedRows++;
            continue;
        }

        if (!getNumber(fields[timeCol], watchTime) ||
            watchTime <= 0 || watchTime > 600) {
            skippedRows++;
            continue;
        }

        m.imdbRating = rating;
        m.releaseYear = year;
        m.watchTime = (int)watchTime;

        double idValue;

        if (idCol != -1 &&
            getNumber(fields[idCol], idValue) &&
            idValue >= 1 &&
            idValue <= 2147483647 &&
            floor(idValue) == idValue) {
            m.movieID = (int)idValue;
        }
        else {
            m.movieID = generatedID;
        }

        generatedID = max(generatedID, m.movieID + 1);

        movies.push_back(m);
        validRows++;
    }

    file.close();

    cout << "\nDataset loading completed!\n";
    cout << "Valid records: " << validRows << endl;
    cout << "Skipped records: " << skippedRows << endl;

    return true;
}

bool compare(const Movie& a, const Movie& b) {

    if (choice == 1)
        return a.imdbRating > b.imdbRating;

    else if (choice == 2)
        return a.releaseYear > b.releaseYear;

    else
        return a.watchTime > b.watchTime;
}

int partition(vector<Movie>& movies, int low, int high) {

    uniform_int_distribution<int> dist(low, high);
    int randomIndex = dist(rng);

    swap(movies[randomIndex], movies[high]);

    Movie pivot = movies[high];

    int i = low - 1;

    for (int j = low; j < high; j++) {

        if (compare(movies[j], pivot)) {
            i++;
            swap(movies[i], movies[j]);
        }
    }

    swap(movies[i + 1], movies[high]);

    return i + 1;
}

void quickSort(vector<Movie>& movies, int low, int high) {

    if (low < high) {

        int pi = partition(movies, low, high);

        quickSort(movies, low, pi - 1);
        quickSort(movies, pi + 1, high);
    }
}

void displayMovies(const vector<Movie>& movies) {

    cout << "\nID\tMovie Name\tRating\tYear\tWatch Time\n";

    for (const Movie& m : movies) {

        cout << m.movieID << "\t"
             << m.movieName << "\t"
             << m.imdbRating << "\t"
             << m.releaseYear << "\t"
             << m.watchTime << " mins\n";
    }
}

string escapeCSV(const string& value) {

    string result = "\"";

    for (char c : value) {
        if (c == '"')
            result += "\"\"";
        else
            result += c;
    }

    result += "\"";
    return result;
}

bool writeCSV(const vector<Movie>& movies) {

    ofstream file("sorted_movies.csv");

    if (!file.is_open()) {
        cout << "Unable to create output file!\n";
        return false;
    }

    file << "MovieID,MovieName,IMDbRating,ReleaseYear,WatchTime\n";

    for (const Movie& m : movies) {

        file << m.movieID << ","
             << escapeCSV(m.movieName) << ","
             << m.imdbRating << ","
             << m.releaseYear << ","
             << m.watchTime << "\n";
    }

    file.close();

    cout << "\nSorted data saved in sorted_movies.csv\n";
    return true;
}

int main() {

    vector<Movie> movies;

    if (!readCSV(movies))
        return 1;

    if (movies.empty()) {
        cout << "No valid movie records found.\n";
        return 0;
    }

    cout << "\nMOVIE RECOMMENDATION SYSTEM\n";
    cout << "1. Sort by IMDb Rating\n";
    cout << "2. Sort by Release Year\n";
    cout << "3. Sort by Watch Time\n";

    cout << "\nEnter your choice: ";

    if (!(cin >> choice) || choice < 1 || choice > 3) {
        cout << "Invalid choice!\n";
        return 0;
    }

    cout << "\nBefore Sorting:";
    displayMovies(movies);

    quickSort(movies, 0, (int)movies.size() - 1);

    cout << "\nAfter Sorting:";
    displayMovies(movies);

    writeCSV(movies);

    return 0;
}
