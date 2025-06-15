// User.h
#pragma once
#include <fstream>
#include "Ticket.h"

class User {
private:
    char    username[30];
    char    password[30];
    double  balance;
    Ticket* tickets[50];
    int     ticketCount;
    bool    isAdmin;

    // NEW: history of past tickets
    Ticket* history[50];
    int     historyCount;

public:
    User();
    User(const char* u, const char* p, bool admin = false);

    const char* getUsername() const;
    const char* getPassword() const;
    double      getBalance()  const;
    bool        getIsAdmin()  const;

    void addFunds(double amount);
    bool buyTicket(const char* movieId, const char* hallName,
        const char* date, const char* time,
        int row, int col, double price);

    void printTickets() const;

    /// Move any tickets with date < today into history[]
    void expirePastTickets();
    /// Print watched‐history tickets
    void printHistory() const;

    void saveToStream(std::ofstream& out) const;
    void loadFromStream(std::ifstream& in);
    
    void saveHistoryToStream(std::ofstream& out) const;
    void loadHistoryFromStream(std::ifstream& in);

    void removeTicketsForMovie(const char* movieId);
    void removeHistoryForMovie(const char* movieId);

    void updateTicketsForMovie(const char* movieId,
        const char* newDate,
        const char* newTime);

};