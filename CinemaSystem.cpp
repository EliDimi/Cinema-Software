#define _CRT_SECURE_NO_WARNINGS
#include "CinemaSystem.h"
#include <fstream>
#include <cstring>
#include <ctime>
#include <iostream>
using namespace std;

// Constructor – initializes user and movie arrays
CinemaSystem::CinemaSystem() {
    for (int i = 0; i < 100; ++i) {
        users[i] = nullptr;
        movies[i] = nullptr;
    }
}

// Destructor – deallocates all users and movies
CinemaSystem::~CinemaSystem() {
    for (int i = 0; i < userCount; ++i) delete users[i];
    for (int i = 0; i < movieCount; ++i) delete movies[i];
}

// Register a new user if username is unique
bool CinemaSystem::registerUser(const char* username, const char* password, bool isAdmin) {
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i]->getUsername(), username) == 0)
            return false;
    }
    users[userCount++] = new User(username, password, isAdmin);
    return true;
}

// Login a user and load their valid tickets
bool CinemaSystem::login(const char* username, const char* password) {
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i]->getUsername(), username) == 0 &&
            strcmp(users[i]->getPassword(), password) == 0) {
            currentUser = users[i];
            currentUser->expirePastTickets();
            return true;
        }
    }
    return false;
}

// Logout current user
void CinemaSystem::logout() { currentUser = nullptr; }

// Get the currently logged-in user
User* CinemaSystem::getCurrentUser() const { return currentUser; }

// Add a new movie to the system
void CinemaSystem::addMovie(Movie* movie) {
    if (movieCount < 100)
        movies[movieCount++] = movie;
}

// Remove a movie and refund all related tickets
bool CinemaSystem::removeMovieById(const char* movieId) {
    for (int u = 0; u < userCount; ++u) {
        users[u]->removeTicketsForMovie(movieId);
        users[u]->removeHistoryForMovie(movieId);
    }

    for (int i = 0; i < movieCount; ++i) {
        if (std::strcmp(movies[i]->getId(), movieId) == 0) {
            delete movies[i];
            for (int j = i; j < movieCount - 1; ++j) {
                movies[j] = movies[j + 1];
            }
            --movieCount;

            std::cout << "Movie and all related tickets removed; refunds issued.\n";

            saveMoviesToFile("movies.dat");
            saveUsersToFile("users.dat");

            return true;
        }
    }
    return false;
}

// Change the title of a movie
bool CinemaSystem::changeMovieTitle(const char* movieId, const char* newTitle) {
    for (int i = 0; i < movieCount; ++i) {
        if (strcmp(movies[i]->getId(), movieId) == 0) {
            movies[i]->setTitle(newTitle);
            return true;
        }
    }
    return false;
}

// Change the hall of a movie (with date & slot check)
bool CinemaSystem::changeMovieHallIfPossible(const char* movieId, const char* newHallName) {
    for (int i = 0; i < movieCount; ++i) {
        if (strcmp(movies[i]->getId(), movieId) == 0) {
            // Check if date is in the future
            time_t now = time(nullptr);
            tm* lt = localtime(&now);
            char today[11];
            sprintf(today, "%04d-%02d-%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);
            if (strcmp(movies[i]->getDate(), today) < 0) {
                cout << "Error: Projection already in the past.\n";
                return false;
            }
            // Check if the slot is free
            if (!isTimeSlotFree(newHallName, movies[i]->getDate(), movies[i]->getStartTime(), movies[i]->getEndTime())) {
                cout << "Error: The slot is busy in new hall.\n";
                return false;
            }
            movies[i]->setHall(newHallName);
            return true;
        }
    }
    return false;
}

// Change the hall of a movie without any checks
bool CinemaSystem::changeMovieHall(const char* movieId, const char* newHallName) {
    for (int i = 0; i < movieCount; ++i) {
        if (strcmp(movies[i]->getId(), movieId) == 0) {
            movies[i]->setHall(newHallName);
            return true;
        }
    }
    return false;
}

// Add a new hall to the system
void CinemaSystem::addHall(const Hall& hall) {
    if (hallCount < 20)
        halls[hallCount++] = hall;
}

// Remove a hall by its name
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

// Close a hall (mark all its movies as CLOSED)
void CinemaSystem::closeHallByName(const char* hallName) {
    for (int i = 0; i < movieCount; ++i) {
        if (strcmp(movies[i]->getHall(), hallName) == 0)
            movies[i]->setHall("CLOSED");
    }
    cout << "All movies in hall '" << hallName << "' are now CLOSED.\n";
}

// Save all users (and their history) to binary file
void CinemaSystem::saveUsersToFile(const char* filename) {
    ofstream out(filename, ios::binary);
    if (!out) return;
    out.write(reinterpret_cast<const char*>(&userCount), sizeof(userCount));
    for (int i = 0; i < userCount; ++i) {
        users[i]->saveToStream(out);
        users[i]->saveHistoryToStream(out);
    }
    out.close();
}

// Load users from binary file
void CinemaSystem::loadUsersFromFile(const char* filename) {
    for (int i = 0; i < userCount; ++i) delete users[i];
    userCount = 0;

    ifstream in(filename, ios::binary);
    if (!in) return;
    in.read(reinterpret_cast<char*>(&userCount), sizeof(userCount));
    for (int i = 0; i < userCount; ++i) {
        users[i] = new User();
        users[i]->loadFromStream(in);
        users[i]->loadHistoryFromStream(in);
    }
    in.close();
}

// Save all movies to file
void CinemaSystem::saveMoviesToFile(const char* filename) {
    ofstream out(filename, ios::binary);
    if (!out) return;
    out.write(reinterpret_cast<const char*>(&movieCount), sizeof(movieCount));
    for (int i = 0; i < movieCount; ++i)
        movies[i]->saveToStream(out);
    out.close();
}

// Load movies from file and recreate correct subclass
void CinemaSystem::loadMoviesFromFile(const char* filename) {
    for (int i = 0; i < movieCount; ++i) delete movies[i];
    movieCount = 0;

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

// Save halls to file
void CinemaSystem::saveHallsToFile(const char* filename) {
    ofstream out(filename, ios::binary);
    if (!out) return;
    out.write(reinterpret_cast<const char*>(&hallCount), sizeof(hallCount));
    for (int i = 0; i < hallCount; ++i)
        halls[i].saveToStream(out);
    out.close();
}

// Load halls from file
void CinemaSystem::loadHallsFromFile(const char* filename) {
    hallCount = 0;
    ifstream in(filename, ios::binary);
    if (!in) return;
    in.read(reinterpret_cast<char*>(&hallCount), sizeof(hallCount));
    for (int i = 0; i < hallCount; ++i)
        halls[i].loadFromStream(in);
    in.close();
}

// List all users and their admin status
void CinemaSystem::listUsers() const {
    cout << "\n-- Users List --\n";
    for (int i = 0; i < userCount; ++i) {
        cout << users[i]->getUsername();
        if (users[i]->getIsAdmin()) cout << " (admin)";
        cout << endl;
    }
}

// Show tickets of a specific user
void CinemaSystem::listUserTickets(const char* username) {
    for (int i = 0; i < userCount; ++i)
        if (strcmp(users[i]->getUsername(), username) == 0) {
            users[i]->printTickets();
            return;
        }
    cout << "User not found.\n";
}

// Show history of a specific user
void CinemaSystem::listUserHistory(const char* username) {
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i]->getUsername(), username) == 0) {
            users[i]->printHistory();
            return;
        }
    }
    cout << "User not found.\n";
}

// Show history of the currently logged-in user
void CinemaSystem::listUserHistory() const {
    if (!currentUser) return;
    currentUser->printHistory();
}

// Remove a user from the system
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

// Update the rating of a movie
void CinemaSystem::rateMovie(const char* movieId, double newRating) {
    for (int i = 0; i < movieCount; ++i)
        if (strcmp(movies[i]->getId(), movieId) == 0) {
            movies[i]->setRating(newRating);
            cout << "Rating updated.\n";
            return;
        }
    cout << "Movie not found.\n";
}

// Attempt to buy a ticket for a movie
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

// Print list of all movies
void CinemaSystem::listMovies() const {
    cout << "\n-- Movies List --\n";
    for (int i = 0; i < movieCount; ++i) {
        cout << movies[i]->getId() << " - " << movies[i]->getTitle()
            << " (" << movies[i]->getDate() << " @ " << movies[i]->getStartTime()
            << ") in hall " << movies[i]->getHall() << endl;
    }
}

// Convert time in "HH:MM" format to total minutes
int CinemaSystem::toMinutes(const char* hhmm) {
    int h, m;
    if (sscanf(hhmm, "%d:%d", &h, &m) != 2) return -1;
    return h * 60 + m;
}

// Check if a time slot is free in a hall for given date
bool CinemaSystem::isTimeSlotFree(const char* hallName, const char* date,
    const char* startTime, const char* endTime) const {
    int newS = toMinutes(startTime);
    int newE = toMinutes(endTime);
    if (newS < 0 || newE < 0) return false;

    for (int i = 0; i < movieCount; ++i) {
        Movie* m = movies[i];
        if (strcmp(m->getHall(), hallName) != 0) continue;
        if (strcmp(m->getDate(), date) != 0) continue;

        int oldS = toMinutes(m->getStartTime());
        int oldE = toMinutes(m->getEndTime());
        if (newS < oldE && oldS < newE)
            return false;
    }
    return true;
}

// Print all available halls
void CinemaSystem::listHalls() const {
    cout << "\n-- Halls List --\n";
    if (hallCount == 0) {
        cout << "No halls available.\n\n";
        return;
    }
    for (int i = 0; i < hallCount; ++i) {
        halls[i].printHall();
    }
}

// Change the date of a movie (with slot validation)
bool CinemaSystem::changeMovieDateIfPossible(
    const char* movieId,
    const char* newDate)
{
    for (int i = 0; i < movieCount; ++i) {
        if (std::strcmp(movies[i]->getId(), movieId) == 0) {
            time_t now = std::time(nullptr);
            std::tm lt;
            localtime_s(&lt, &now);
            char today[11];
            std::sprintf(today, "%04d-%02d-%02d",
                lt.tm_year + 1900,
                lt.tm_mon + 1,
                lt.tm_mday);

            if (std::strcmp(movies[i]->getDate(), today) < 0) {
                std::cout << "Error: Projection already in the past.\n";
                return false;
            }

            if (!isTimeSlotFree(
                movies[i]->getHall(),
                newDate,
                movies[i]->getStartTime(),
                movies[i]->getEndTime()))
            {
                std::cout << "Error: Slot is busy for the new date.\n";
                return false;
            }

            movies[i]->setDate(newDate);

            // Propagate date change to tickets
            for (int u = 0; u < userCount; ++u) {
                users[u]->updateTicketsForMovie(
                    movieId,
                    newDate,
                    /*newTime=*/nullptr);
            }

            return true;
        }
    }
    return false;
}

// Change movie time (start & end) with validation
bool CinemaSystem::changeMovieTimeIfPossible(
    const char* movieId,
    const char* newStartTime,
    const char* newEndTime)
{
    for (int i = 0; i < movieCount; ++i) {
        if (std::strcmp(movies[i]->getId(), movieId) == 0) {
            // Build today's date string "YYYY-MM-DD"
            time_t now = std::time(nullptr);
            std::tm lt;
            localtime_s(&lt, &now);
            char today[11];
            std::sprintf(today, "%04d-%02d-%02d",
                lt.tm_year + 1900,
                lt.tm_mon + 1,
                lt.tm_mday);

            // Can't change past projections
            if (std::strcmp(movies[i]->getDate(), today) < 0) {
                std::cout << "Error: Projection already in the past.\n";
                return false;
            }

            // Slot conflict check
            if (!isTimeSlotFree(
                movies[i]->getHall(),
                movies[i]->getDate(),
                newStartTime,
                newEndTime))
            {
                std::cout << "Error: Slot is busy for new times.\n";
                return false;
            }

            // 1) Update the movie
            movies[i]->setStartTime(newStartTime);
            movies[i]->setEndTime(newEndTime);

            // 2) Propagate change to every user
            for (int u = 0; u < userCount; ++u) {
                users[u]->updateTicketsForMovie(
                    movieId,
                    /*newDate=*/nullptr,
                    newStartTime);
            }

            return true;
        }
    }
    return false;
}