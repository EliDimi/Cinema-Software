#pragma once
#include "User.h"
#include "Hall.h"
#include "Movie.h"

class CinemaSystem {
private:
    User* users[100];        // List of users
    int userCount;           // Number of users

    Movie* movies[100];      // List of movies
    int movieCount;          // Number of movies

    Hall halls[20];          // List of halls
    int hallCount;           // Number of halls

    User* currentUser;       // Currently logged in user

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
    void listUserTickets(const char* username);
    bool removeUser(const char* username);
    void listUserHistory(const char* username);
    void rateMovie(const char* movieId, double newRating);

    // Movie operations
    void addMovie(Movie* movie);
    bool removeMovieById(const char* movieId);
    bool changeMovieTitle(const char* movieId, const char* newTitle);
    bool changeMovieHall(const char* movieId, const char* newHallName);

    // Hall operations
    void addHall(const Hall& hall);
    bool removeHallByName(const char* hallName);
    void closeHallByName(const char* hallName);

    // File storage
    void saveUsersToFile(const char* filename);
    void loadUsersFromFile(const char* filename);

    void saveMoviesToFile(const char* filename);
    void loadMoviesFromFile(const char* filename);

    void saveHallsToFile(const char* filename);
    void loadHallsFromFile(const char* filename);

    // --- New user commands ---
    bool buyTicket(const char* movieId, int row, int col); // Buy a ticket for current user
    void listMovies() const;                               // Show all scheduled movies
    void listUserHistory() const;                          // Show ticket history of current user
};
