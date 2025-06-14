#define _CRT_SECURE_NO_WARNINGS
#include "CinemaSystem.h"
#include <fstream>
#include <cstring>
using namespace std;

// Constructor
CinemaSystem::CinemaSystem() : userCount(0), movieCount(0), hallCount(0), currentUser(nullptr) {}

// Destructor
CinemaSystem::~CinemaSystem() {
    for (int i = 0; i < userCount; ++i)
        delete users[i];
    for (int i = 0; i < movieCount; ++i)
        delete movies[i];
}

// Register a new user
bool CinemaSystem::registerUser(const char* username, const char* password, bool isAdmin) {
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i]->getUsername(), username) == 0)
            return false;
    }
    users[userCount++] = new User(username, password, isAdmin);
    return true;
}

// Login
bool CinemaSystem::login(const char* username, const char* password) {
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i]->getUsername(), username) == 0 &&
            strcmp(users[i]->getPassword(), password) == 0) {
            currentUser = users[i];
            return true;
        }
    }
    return false;
}

// Logout
void CinemaSystem::logout() {
    currentUser = nullptr;
}

// Get current user
User* CinemaSystem::getCurrentUser() const {
    return currentUser;
}

// Add movie
void CinemaSystem::addMovie(Movie* movie) {
    if (movieCount < 100)
        movies[movieCount++] = movie;
}

// Remove movie
bool CinemaSystem::removeMovieById(const char* movieId) {
    for (int i = 0; i < movieCount; ++i) {
        if (strcmp(movies[i]->getId(), movieId) == 0) {
            delete movies[i];
            for (int j = i; j < movieCount - 1; ++j)
                movies[j] = movies[j + 1];
            --movieCount;
            return true;
        }
    }
    return false;
}

// Change movie title
bool CinemaSystem::changeMovieTitle(const char* movieId, const char* newTitle) {
    for (int i = 0; i < movieCount; ++i) {
        if (strcmp(movies[i]->getId(), movieId) == 0) {
            movies[i]->setTitle(newTitle);
            return true;
        }
    }
    return false;
}

// Change movie hall
bool CinemaSystem::changeMovieHall(const char* movieId, const char* newHallName) {
    for (int i = 0; i < movieCount; ++i) {
        if (strcmp(movies[i]->getId(), movieId) == 0) {
            movies[i]->setHall(newHallName);
            return true;
        }
    }
    return false;
}

// Add hall
void CinemaSystem::addHall(const Hall& hall) {
    if (hallCount < 20)
        halls[hallCount++] = hall;
}

// Remove hall
bool CinemaSystem::removeHallByName(const char* hallName) {
    for (int i = 0; i < hallCount; ++i) {
        if (strcmp(halls[i].getName(), hallName) == 0) {
            for (int j = i; j < hallCount - 1; ++j)
                halls[j] = halls[j + 1];
            --hallCount;
            return true;
        }
    }
    return false;
}

// Close hall
void CinemaSystem::closeHallByName(const char* hallName) {
    for (int i = 0; i < movieCount; ++i) {
        if (strcmp(movies[i]->getHall(), hallName) == 0)
            movies[i]->setHall("CLOSED");
    }
    cout << "All movies in hall '" << hallName << "' are now CLOSED.\n";
}

// Save and load users
void CinemaSystem::saveUsersToFile(const char* filename) {
    ofstream out(filename, ios::binary);
    if (!out) return;
    out.write(reinterpret_cast<const char*>(&userCount), sizeof(userCount));
    for (int i = 0; i < userCount; ++i)
        users[i]->saveToStream(out);
    out.close();
}

void CinemaSystem::loadUsersFromFile(const char* filename) {
    ifstream in(filename, ios::binary);
    if (!in) return;
    in.read(reinterpret_cast<char*>(&userCount), sizeof(userCount));
    for (int i = 0; i < userCount; ++i) {
        users[i] = new User();
        users[i]->loadFromStream(in);
    }
    in.close();
}

// Save and load movies
void CinemaSystem::saveMoviesToFile(const char* filename) {
    ofstream out(filename, ios::binary);
    if (!out) return;
    out.write(reinterpret_cast<const char*>(&movieCount), sizeof(movieCount));
    for (int i = 0; i < movieCount; ++i)
        movies[i]->saveToStream(out);
    out.close();
}

void CinemaSystem::loadMoviesFromFile(const char* filename) {
    ifstream in(filename, ios::binary);
    if (!in) return;
    in.read(reinterpret_cast<char*>(&movieCount), sizeof(movieCount));
    for (int i = 0; i < movieCount; ++i) {
        char type;
        in.read(&type, sizeof(type));
        Movie* m = nullptr;
        if (type == 'A') m = new ActionMovie();
        else if (type == 'D') m = new DramaMovie();
        else if (type == 'C') m = new DocumentaryMovie();
        if (m) {
            m->loadFromStream(in);
            movies[i] = m;
        }
    }
    in.close();
}

// Save and load halls
void CinemaSystem::saveHallsToFile(const char* filename) {
    ofstream out(filename, ios::binary);
    if (!out) return;
    out.write(reinterpret_cast<const char*>(&hallCount), sizeof(hallCount));
    for (int i = 0; i < hallCount; ++i)
        halls[i].saveToStream(out);
    out.close();
}

void CinemaSystem::loadHallsFromFile(const char* filename) {
    ifstream in(filename, ios::binary);
    if (!in) return;
    in.read(reinterpret_cast<char*>(&hallCount), sizeof(hallCount));
    for (int i = 0; i < hallCount; ++i)
        halls[i].loadFromStream(in);
    in.close();
}

// List users
void CinemaSystem::listUsers() const {
    cout << "\n-- Users List --\n";
    for (int i = 0; i < userCount; ++i) {
        cout << users[i]->getUsername();
        if (users[i]->getIsAdmin()) cout << " (admin)";
        cout << endl;
    }
}

// List tickets for user
void CinemaSystem::listUserTickets(const char* username) {
    for (int i = 0; i < userCount; ++i)
        if (strcmp(users[i]->getUsername(), username) == 0) {
            users[i]->printTickets();
            return;
        }
    cout << "User not found.\n";
}

// Remove user
bool CinemaSystem::removeUser(const char* username) {
    for (int i = 0; i < userCount; ++i)
        if (strcmp(users[i]->getUsername(), username) == 0) {
            delete users[i];
            for (int j = i; j < userCount - 1; ++j)
                users[j] = users[j + 1];
            --userCount;
            return true;
        }
    return false;
}

void CinemaSystem::listUserHistory(const char* username) {
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i]->getUsername(), username) == 0) {
            users[i]->printTickets();
            return;
        }
    }
    cout << "User not found.\n";
}

// Rate movie
void CinemaSystem::rateMovie(const char* movieId, double newRating) {
    for (int i = 0; i < movieCount; ++i)
        if (strcmp(movies[i]->getId(), movieId) == 0) {
            movies[i]->setRating(newRating);
            cout << "Rating updated.\n";
            return;
        }
    cout << "Movie not found.\n";
}

// --- NEW USER COMMANDS ---

// Buy ticket for current user
bool CinemaSystem::buyTicket(const char* movieId, int row, int col) {
    if (!currentUser) return false;

    for (int i = 0; i < movieCount; ++i) {
        if (strcmp(movies[i]->getId(), movieId) == 0) {
            const char* hallName = movies[i]->getHall();
            for (int j = 0; j < hallCount; ++j) {
                if (strcmp(halls[j].getName(), hallName) == 0) {
                    if (!halls[j].isSeatValid(row, col)) {
                        cout << "Invalid seat.\n";
                        return false;
                    }
                    if (halls[j].isSeatTaken(row, col)) {
                        cout << "Seat already taken.\n";
                        return false;
                    }

                    double price = movies[i]->calculateTicketPrice();
                    if (!currentUser->buyTicket(movieId, hallName, movies[i]->getDate(),
                        movies[i]->getStartTime(), row, col, price)) {
                        cout << "Insufficient funds.\n";
                        return false;
                    }

                    halls[j].reserveSeat(row, col);
                    cout << "Ticket purchased successfully.\n";
                    return true;
                }
            }
        }
    }
    cout << "Movie or hall not found.\n";
    return false;
}

// List all movies
void CinemaSystem::listMovies() const {
    cout << "\n-- Movies List --\n";
    for (int i = 0; i < movieCount; ++i) {
        cout << movies[i]->getId() << " - " << movies[i]->getTitle()
            << " (" << movies[i]->getDate() << " @ " << movies[i]->getStartTime()
            << ") in hall " << movies[i]->getHall() << endl;
    }
}

// List current user's tickets
void CinemaSystem::listUserHistory() const {
    if (!currentUser) return;
    currentUser->printTickets();
}
