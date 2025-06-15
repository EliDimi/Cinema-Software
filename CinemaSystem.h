#pragma once
#include "User.h"
#include "Hall.h"
#include "Movie.h"

class CinemaSystem {
private:
    User* users[100] = { nullptr };
    int    userCount = 0;

    Movie* movies[100] = { nullptr };
    int    movieCount = 0;

    Hall   halls[20];    
    int    hallCount = 0;

    User* currentUser = nullptr;

public:
    // Constructor & Destructor
    CinemaSystem();
    ~CinemaSystem();

    // User account management
    bool registerUser(const char* username, const char* password, bool isAdmin = false);
    bool login(const char* username, const char* password);
    void logout();
    User* getCurrentUser() const;

    // User account operations (admin)
    void listUsers() const;
    void listUserTickets(const char* username); // For admin
    bool removeUser(const char* username);
    void listUserHistory(const char* username); // For admin
    void listUserHistory() const;               // For current user
    void rateMovie(const char* movieId, double newRating);

    static int toMinutes(const char* hhmm);

    // Movie operations
    void addMovie(Movie* movie);
    bool removeMovieById(const char* movieId);
    bool changeMovieTitle(const char* movieId, const char* newTitle);
    bool changeMovieHall(const char* movieId, const char* newHallName);

    // Hall operations
    void addHall(const Hall& hall);
    bool removeHallByName(const char* hallName);
    void closeHallByName(const char* hallName);
    void listHalls() const;
    bool changeMovieHallIfPossible(const char* movieId, const char* newHallName);
    bool changeMovieDateIfPossible(const char* movieId, const char* newDate);
    bool changeMovieTimeIfPossible(const char* movieId, const char* newStartTime, const char* newEndTime);

    // File storage
    void saveUsersToFile(const char* filename);
    void loadUsersFromFile(const char* filename);

    void saveMoviesToFile(const char* filename);
    void loadMoviesFromFile(const char* filename);

    void saveHallsToFile(const char* filename);
    void loadHallsFromFile(const char* filename);

    bool buyTicket(const char* movieId, int row, int col); // Buy a ticket for current user
    void listMovies() const;                               // Show all scheduled movies

    bool isTimeSlotFree(const char* hallName,
                        const char* date,
                        const char* startTime,
                        const char* endTime) const;

};
