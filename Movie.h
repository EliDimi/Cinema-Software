#pragma once
#include <cstring>
using namespace std;

// Enum to represent movie genres
enum Genre {
    ACTION,
    DRAMA,
    DOCUMENTARY
};

// Abstract base class for all types of movies
class Movie {
private:
    char id[20];           // Unique movie ID
    char title[100];       // Movie title
    double rating;         // User rating (e.g., from 1 to 10)
    int duration;          // Duration in minutes
    int releaseYear;       // Year of release
    char hall[20];         // Hall where the movie is shown
    char date[11];         // Screening date (YYYY-MM-DD)
    char startTime[6];     // Start time (HH:MM)
    char endTime[6];       // End time (HH:MM)

public:
    // Constructor
    Movie(const char* id, const char* title, double rating, int duration,
        int releaseYear, const char* hall, const char* date,
        const char* startTime, const char* endTime);

    // Virtual destructor
    virtual ~Movie();

    // Pure virtual function to calculate ticket price
    virtual double calculateTicketPrice() const = 0;

    // Getters
    const char* getId() const;
    const char* getTitle() const;
    const char* getHall() const;
    double getRating() const;
    int getDuration() const;
    int getReleaseYear() const;
    const char* getDate() const;
    const char* getStartTime() const;
    const char* getEndTime() const;

    // Setters
    void setTitle(const char* newTitle);
    void setHall(const char* newHall);
};

// Derived class for action movies
class ActionMovie : public Movie {
private:
    int actionIntensity;   // Value from 0 to 20

public:
    // Constructor
    ActionMovie(const char* id, const char* title, double rating, int duration,
        int releaseYear, const char* hall, const char* date,
        const char* startTime, const char* endTime, int actionIntensity);

    // Ticket price calculation for action movies
    double calculateTicketPrice() const override;
};

// Derived class for drama movies
class DramaMovie : public Movie {
private:
    bool hasComedyElements;   // True if contains comedy elements

public:
    // Constructor
    DramaMovie(const char* id, const char* title, double rating, int duration,
        int releaseYear, const char* hall, const char* date,
        const char* startTime, const char* endTime, bool hasComedyElements);

    // Ticket price calculation for drama movies
    double calculateTicketPrice() const override;
};

// Derived class for documentaries
class DocumentaryMovie : public Movie {
private:
    char theme[50];               // Documentary theme
    bool isBasedOnTrueEvents;     // True if based on true events

public:
    // Constructor
    DocumentaryMovie(const char* id, const char* title, double rating, int duration,
        int releaseYear, const char* hall, const char* date,
        const char* startTime, const char* endTime,
        const char* theme, bool isBasedOnTrueEvents);

    // Ticket price calculation for documentaries
    double calculateTicketPrice() const override;
};