#pragma once
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

// Represents a single ticket for a specific movie, seat, and time
class Ticket {
private:
    char movieId[20];      // Unique movie identifier
    char hallName[20];     // Hall where the movie is shown
    char date[11];         // Date of screening: YYYY-MM-DD
    char time[6];          // Start time: HH:MM
    int row;               // Row number
    int col;               // Column number
    double price;          // Ticket price

public:
    // Default constructor
    Ticket();

    // Parameterized constructor
    Ticket(const char* movieId, const char* hallName, const char* date,
        const char* time, int row, int col, double price);

    // Getters
    const char* getMovieId() const;
    const char* getHallName() const;
    const char* getDate() const;
    const char* getTime() const;
    int getRow() const;
    int getCol() const;
    double getPrice() const;

    // Displays ticket info
    void printTicket() const;

    void setDate(const char* newDate) {
        std::strncpy(date, newDate, sizeof(date));
        date[sizeof(date) - 1] = '\0';
    }
    void setTime(const char* newTime) {
        std::strncpy(time, newTime, sizeof(time));
        time[sizeof(time) - 1] = '\0';
    }

    // Saves ticket to a binary stream
    void saveToStream(ofstream& out) const;

    // Loads ticket from a binary stream
    void loadFromStream(ifstream& in);
};
