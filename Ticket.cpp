#define _CRT_SECURE_NO_WARNINGS
#include "Ticket.h"
#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

// Default constructor
Ticket::Ticket() : row(0), col(0), price(0.0) {
    movieId[0] = '\0';
    hallName[0] = '\0';
    date[0] = '\0';
    time[0] = '\0';
}

// Constructor with parameters
Ticket::Ticket(const char* movieId, const char* hallName, const char* date,
    const char* time, int row, int col, double price)
    : row(row), col(col), price(price) {
    strcpy(this->movieId, movieId);
    strcpy(this->hallName, hallName);
    strcpy(this->date, date);
    strcpy(this->time, time);
}

// Getters
const char* Ticket::getMovieId() const {
    return movieId;
}

const char* Ticket::getHallName() const {
    return hallName;
}

const char* Ticket::getDate() const {
    return date;
}

const char* Ticket::getTime() const {
    return time;
}

int Ticket::getRow() const {
    return row;
}

int Ticket::getCol() const {
    return col;
}

double Ticket::getPrice() const {
    return price;
}

// Prints ticket details to the console
void Ticket::printTicket() const {
    cout << "------------------------------" << endl;
    cout << "Movie ID: " << movieId << endl;
    cout << "Hall: " << hallName << endl;
    cout << "Date: " << date << "   Time: " << time << endl;
    cout << "Seat: Row " << row << ", Column " << col << endl;
    cout << "Price: " << price << " BGN" << endl;
    cout << "------------------------------" << endl;
}

// Saves ticket information to a binary stream
void Ticket::saveToStream(ofstream& out) const {
    out.write(movieId, sizeof(movieId));
    out.write(hallName, sizeof(hallName));
    out.write(date, sizeof(date));
    out.write(time, sizeof(time));
    out.write(reinterpret_cast<const char*>(&row), sizeof(row));
    out.write(reinterpret_cast<const char*>(&col), sizeof(col));
    out.write(reinterpret_cast<const char*>(&price), sizeof(price));
}

// Loads ticket information from a binary stream
void Ticket::loadFromStream(ifstream& in) {
    in.read(movieId, sizeof(movieId));
    in.read(hallName, sizeof(hallName));
    in.read(date, sizeof(date));
    in.read(time, sizeof(time));
    in.read(reinterpret_cast<char*>(&row), sizeof(row));
    in.read(reinterpret_cast<char*>(&col), sizeof(col));
    in.read(reinterpret_cast<char*>(&price), sizeof(price));
}
