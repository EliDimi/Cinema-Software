#include "Hall.h"

// Constructor: initializes name and seat grid
Hall::Hall(const char* name, int rows, int cols)
    : rows(rows), cols(cols) {
    strncpy(this->name, name, sizeof(this->name));
    clear(); // Set all seats to unoccupied
}

// Returns the name of the hall
const char* Hall::getName() const {
    return name;
}

// Returns number of rows
int Hall::getRowCount() const {
    return rows;
}

// Returns number of columns
int Hall::getColCount() const {
    return cols;
}

// Checks if the specified seat position is within valid bounds
bool Hall::isSeatValid(int row, int col) const {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

// Returns true if the seat is already taken or invalid
bool Hall::isSeatTaken(int row, int col) const {
    if (!isSeatValid(row, col)) return true;
    return seats[row][col];
}

// Reserves a seat if it is valid and available
bool Hall::reserveSeat(int row, int col) {
    if (!isSeatValid(row, col)) return false;
    if (seats[row][col]) return false;

    seats[row][col] = true;
    return true;
}

// Displays the seating layout to the console
void Hall::printHall() const {
    cout << "Hall: " << name << "\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << (seats[i][j] ? "[X]" : "[ ]");
        }
        cout << endl;
    }
}

// Clears all seat reservations (sets all to false)
void Hall::clear() {
    for (int i = 0; i < 20; ++i)
        for (int j = 0; j < 20; ++j)
            seats[i][j] = false;
}