#define _CRT_SECURE_NO_WARNINGS
#include "User.h"
#include <iostream>
#include <cstring>
using namespace std;

User::User()
    : balance(0), ticketCount(0), isAdmin(false), historyCount(0)
{
    username[0] = password[0] = '\0';
    memset(tickets, 0, sizeof(tickets));
    memset(history, 0, sizeof(history));
}

User::User(const char* u, const char* p, bool admin)
    : balance(0), ticketCount(0), isAdmin(admin), historyCount(0)
{
    strcpy(username, u);
    strcpy(password, p);
    memset(tickets, 0, sizeof(tickets));
    memset(history, 0, sizeof(history));
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

static time_t parseTicketDateTime(const Ticket* t) {
    std::tm tm{};
    int y, m, d, hh, mm;

    if (std::sscanf(t->getDate(), "%4d-%2d-%2d", &y, &m, &d) != 3) return -1;
    if (std::sscanf(t->getTime(), "%2d:%2d", &hh, &mm) != 2) return -1;

    tm.tm_year = y - 1900;
    tm.tm_mon = m - 1;
    tm.tm_mday = d;
    tm.tm_hour = hh;
    tm.tm_min = mm;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;

    return std::mktime(&tm);
}

// Add funds to user account
void User::addFunds(double amount) {
    if (amount > 0)
        balance += amount;
    cout << "Added " << amount << " BGN. New balance: " << balance << " BGN\n";
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

void User::expirePastTickets() {
    // build "YYYY-MM-DD" for today
    std::time_t now = std::time(nullptr);
    std::tm* lt = std::localtime(&now);
    char today[11];
    std::sprintf(today, "%04d-%02d-%02d",
        lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);

    for (int i = 0; i < ticketCount; ) {
        const char* d = tickets[i]->getDate();
        if (std::strcmp(d, today) < 0) {
            // move to history
            history[historyCount++] = tickets[i];
            // shift remaining tickets left
            for (int j = i; j < ticketCount - 1; ++j)
                tickets[j] = tickets[j + 1];
            --ticketCount;
        }
        else {
            ++i;
        }
    }
}

void User::printHistory() const {
    std::cout << "\n-- Watch History for " << username << " --\n";
    for (int i = 0; i < historyCount; ++i)
        if (history[i]) history[i]->printTicket();
}

void User::removeTicketsForMovie(const char* movieId) {
    for (int i = 0; i < ticketCount; ) {
        if (std::strcmp(tickets[i]->getMovieId(), movieId) == 0) {
            double price = tickets[i]->getPrice();
            balance += price;
            delete tickets[i];
            for (int j = i; j < ticketCount - 1; ++j)
                tickets[j] = tickets[j + 1];
            --ticketCount;
        }
        else {
            ++i;
        }
    }
}

void User::removeHistoryForMovie(const char* movieId) {
    for (int i = 0; i < historyCount; ) {
        if (std::strcmp(history[i]->getMovieId(), movieId) == 0) {
            double price = history[i]->getPrice();
            balance += price;

            delete history[i];

            for (int j = i; j < historyCount - 1; ++j) {
                history[j] = history[j + 1];
            }
            --historyCount;
        }
        else {
            ++i;
        }
    }
}

void User::saveHistoryToStream(std::ofstream& out) const {
    out.write(reinterpret_cast<const char*>(&historyCount), sizeof(historyCount));
    for (int i = 0; i < historyCount; ++i)
        history[i]->saveToStream(out);
}

void User::loadHistoryFromStream(std::ifstream& in) {
    in.read(reinterpret_cast<char*>(&historyCount), sizeof(historyCount));
    for (int i = 0; i < historyCount; ++i) {
        history[i] = new Ticket();
        history[i]->loadFromStream(in);
    }
}

void User::updateTicketsForMovie(const char* movieId,
    const char* newDate,
    const char* newTime)
{
    // 1) Apply updates
    for (int i = 0; i < ticketCount; ++i) {
        if (std::strcmp(tickets[i]->getMovieId(), movieId) == 0) {
            if (newDate) tickets[i]->setDate(newDate);
            if (newTime) tickets[i]->setTime(newTime);
        }
    }

    // 2) Expire any that are now in the past
    time_t now = std::time(nullptr);
    for (int i = 0; i < ticketCount; ) {
        time_t ts = parseTicketDateTime(tickets[i]);
        if (ts >= 0 && ts < now) {
            // Move to history
            history[historyCount++] = tickets[i];
            // Shift array left
            for (int j = i; j < ticketCount - 1; ++j)
                tickets[j] = tickets[j + 1];
            --ticketCount;
        }
        else {
            ++i;
        }
    }
}