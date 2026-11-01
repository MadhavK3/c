
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>
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

bool readCSV(vector<Movie>& movies) {

    ifstream file("movies.csv");

    if (!file.is_open()) {
        cout << "Error: Unable to open movies.csv\n";
        return false;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {

        stringstream ss(line);
        Movie m;
        string rating, year, time;

        if (!getline(ss, line, ','))
            continue;

        try {
            m.movieID = stoi(line);

            if (!getline(ss, m.movieName, ',') ||
                !getline(ss, rating, ',') ||
                !getline(ss, year, ',') ||
                !getline(ss, time))
                continue;

            m.imdbRating = stod(rating);
            m.releaseYear = stoi(year);
            m.watchTime = stoi(time);
        }
        catch (...) {
            cout << "Invalid row skipped.\n";
            continue;
        }

        movies.push_back(m);
    }

    file.close();

    cout << "Total movies loaded: "
         << movies.size() << endl;

    return true;
}

bool compare(Movie a, Movie b) {

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

bool writeCSV(const vector<Movie>& movies) {

    ofstream file("sorted_movies.csv");

    if (!file.is_open()) {
        cout << "Unable to create output file!\n";
        return false;
    }

    file << "MovieID,MovieName,IMDbRating,ReleaseYear,WatchTime\n";

    for (const Movie& m : movies) {

        file << m.movieID << ","
             << m.movieName << ","
             << m.imdbRating << ","
             << m.releaseYear << ","
             << m.watchTime << "\n";
    }

    file.close();

    cout << "\nSorted movies saved in sorted_movies.csv\n";

    return true;
}

int main() {

    vector<Movie> movies;

    if (!readCSV(movies))
        return 1;

    if (movies.empty()) {
        cout << "No movies available.\n";
        return 0;
    }

    cout << "\nMOVIE RECOMMENDATION SYSTEM\n";
    cout << "1. Sort by IMDb Rating\n";
    cout << "2. Sort by Release Year\n";
    cout << "3. Sort by Watch Time\n";

    cout << "\nEnter your choice: ";
    cin >> choice;

    if (choice < 1 || choice > 3) {
        cout << "Invalid choice!\n";
        return 0;
    }

    cout << "\nBefore Sorting:\n";
    displayMovies(movies);

    quickSort(movies, 0, (int)movies.size() - 1);

    cout << "\nAfter Sorting:\n";
    displayMovies(movies);

    writeCSV(movies);

    return 0;
}
