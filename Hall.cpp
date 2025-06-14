#define _CRT_SECURE_NO_WARNINGS
#include "Hall.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// Default constructor
Hall::Hall() {
    name[0] = '\0';
    rows = 0;
    cols = 0;
    for (int i = 0; i < 20; ++i)
        for (int j = 0; j < 20; ++j)
            seats[i][j] = false;
}

// Parameterized constructor
Hall::Hall(const char* name, int rows, int cols) : rows(rows), cols(cols) {
    strcpy(this->name, name);
    for (int i = 0; i < 20; ++i)
        for (int j = 0; j < 20; ++j)
            seats[i][j] = false;
}

const char* Hall::getName() const {
    return name;
}

int Hall::getRowCount() const {
    return rows;
}

int Hall::getColCount() const {
    return cols;
}

bool Hall::isSeatValid(int row, int col) const {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

bool Hall::isSeatTaken(int row, int col) const {
    if (!isSeatValid(row, col)) return true;
    return seats[row][col];
}

bool Hall::reserveSeat(int row, int col) {
    if (!isSeatValid(row, col)) return false;
    if (seats[row][col]) return false;
    seats[row][col] = true;
    return true;
}

void Hall::printHall() const {
    cout << "Hall: " << name << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << (seats[i][j] ? "[X]" : "[ ]");
        }
        cout << endl;
    }
}

void Hall::clear() {
    for (int i = 0; i < 20; ++i)
        for (int j = 0; j < 20; ++j)
            seats[i][j] = false;
}

// Save hall data to binary stream
void Hall::saveToStream(ofstream& out) const {
    out.write(name, sizeof(name));
    out.write(reinterpret_cast<const char*>(&rows), sizeof(rows));
    out.write(reinterpret_cast<const char*>(&cols), sizeof(cols));
    out.write(reinterpret_cast<const char*>(seats), sizeof(seats));
}

// Load hall data from binary stream
void Hall::loadFromStream(ifstream& in) {
    in.read(name, sizeof(name));
    in.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    in.read(reinterpret_cast<char*>(&cols), sizeof(cols));
    in.read(reinterpret_cast<char*>(seats), sizeof(seats));
}
