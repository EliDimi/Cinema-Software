#define _CRT_SECURE_NO_WARNINGS
#include "User.h"
#include <iostream>
#include <cstring>
using namespace std;

// Default constructor
User::User() : balance(0), ticketCount(0), isAdmin(false) {
    username[0] = '\0';
    password[0] = '\0';
    for (int i = 0; i < 50; ++i)
        tickets[i] = nullptr;
}

// Constructor with username, password, and optional isAdmin flag
User::User(const char* u, const char* p, bool admin)
    : balance(0), ticketCount(0), isAdmin(admin) {
    strcpy(username, u);
    strcpy(password, p);
    for (int i = 0; i < 50; ++i)
        tickets[i] = nullptr;
}

// Getters
const char* User::getUsername() const {
    return username;
}

const char* User::getPassword() const {
    return password;
}

double User::getBalance() const {
    return balance;
}

bool User::getIsAdmin() const {
    return isAdmin;
}

// Add funds to user account
void User::addFunds(double amount) {
    if (amount > 0)
        balance += amount;
}

// Buys a ticket and stores it if user has enough balance
bool User::buyTicket(const char* movieId, const char* hallName, const char* date,
    const char* time, int row, int col, double price) {
    if (ticketCount >= 50) return false;          // Max ticket limit
    if (balance < price) return false;            // Not enough money

    Ticket* t = new Ticket(movieId, hallName, date, time, row, col, price);
    tickets[ticketCount++] = t;
    balance -= price;
    return true;
}

// Print all tickets owned by user
void User::printTickets() const {
    cout << "\n-- Tickets for " << username << " --\n";
    for (int i = 0; i < ticketCount; ++i)
        if (tickets[i]) tickets[i]->printTicket();
}

// Save user data to binary stream
void User::saveToStream(ofstream& out) const {
    out.write(username, sizeof(username));
    out.write(password, sizeof(password));
    out.write(reinterpret_cast<const char*>(&balance), sizeof(balance));
    out.write(reinterpret_cast<const char*>(&ticketCount), sizeof(ticketCount));
    for (int i = 0; i < ticketCount; ++i)
        tickets[i]->saveToStream(out);
    out.write(reinterpret_cast<const char*>(&isAdmin), sizeof(isAdmin));
}

// Load user data from binary stream
void User::loadFromStream(ifstream& in) {
    in.read(username, sizeof(username));
    in.read(password, sizeof(password));
    in.read(reinterpret_cast<char*>(&balance), sizeof(balance));
    in.read(reinterpret_cast<char*>(&ticketCount), sizeof(ticketCount));
    for (int i = 0; i < ticketCount; ++i) {
        tickets[i] = new Ticket();
        tickets[i]->loadFromStream(in);
    }
    in.read(reinterpret_cast<char*>(&isAdmin), sizeof(isAdmin));
}
