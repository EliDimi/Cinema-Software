#pragma once
#include <cstring>
#include <iostream>
#include <fstream>
#include "Ticket.h"

using namespace std;

// Represents a user account (admin or normal) in the system
class User {
private:
    char username[30];        // Unique username
    char password[30];        // Account password
    double balance;           // Available funds
    Ticket* tickets[50];      // Array of owned tickets
    int ticketCount;          // Number of tickets
    bool isAdmin;             // True if user is an admin

public:
    // Default constructor
    User();

    // Constructor with parameters
    User(const char* u, const char* p, bool admin = false);

    // Getters
    const char* getUsername() const;
    const char* getPassword() const;
    double getBalance() const;
    bool getIsAdmin() const;

    // Add money to balance
    void addFunds(double amount);

    // Buys a ticket with full details
    bool buyTicket(const char* movieId, const char* hallName, const char* date,
        const char* time, int row, int col, double price);

    // Displays all owned tickets
    void printTickets() const;

    // Save user to binary file stream
    void saveToStream(ofstream& out) const;

    // Load user from binary file stream
    void loadFromStream(ifstream& in);
};
