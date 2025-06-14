#pragma once
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

// Represents a cinema hall with seats arranged in rows and columns
class Hall {
private:
    char name[20];        // Hall identifier
    int rows;             // Number of rows
    int cols;             // Number of columns
    bool seats[20][20];   // 2D array representing seat occupancy

public:
    // Default constructor
    Hall();

    // Parameterized constructor
    Hall(const char* name, int rows, int cols);

    // Getters
    const char* getName() const;
    int getRowCount() const;
    int getColCount() const;

    // Validates if the given seat coordinates are within bounds
    bool isSeatValid(int row, int col) const;

    // Checks if the seat is already taken
    bool isSeatTaken(int row, int col) const;

    // Attempts to reserve the seat; returns true on success
    bool reserveSeat(int row, int col);

    // Prints the current state of all seats in the hall
    void printHall() const;

    // Clears all seat reservations
    void clear();

    // Save hall to binary stream
    void saveToStream(ofstream& out) const;

    // Load hall from binary stream
    void loadFromStream(ifstream& in);
};
