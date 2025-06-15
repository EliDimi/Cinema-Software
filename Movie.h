#pragma once
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

// Enum to represent movie genres
enum Genre {
    ACTION,
    DRAMA,
    DOCUMENTARY
};

// Abstract base class for all types of movies
class Movie {
protected:
    char id[20];           // Unique movie ID
    char title[100];       // Movie title
    double rating;         // User rating
    int duration;          // Duration in minutes
    int releaseYear;       // Year of release
    char hall[20];         // Hall name
    char date[11];         // YYYY-MM-DD
    char startTime[6];     // HH:MM
    char endTime[6];       // HH:MM

public:
    // Default constructor
    Movie();

    // Parameterized constructor
    Movie(const char* id, const char* title, double rating, int duration,
        int releaseYear, const char* hall, const char* date,
        const char* startTime, const char* endTime);

    void setDate(const char* newDate);
    void setStartTime(const char* newStartTime);
    void setEndTime(const char* newEndTime);

    virtual ~Movie() {}

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
    void setRating(double newRating);  // NEW

    // Calculates ticket price (must be implemented by subclasses)
    virtual double calculateTicketPrice() const = 0;

    // Get genre type (for saving)
    virtual Genre getGenre() const = 0;

    // Save and load
    virtual void saveToStream(ofstream& out) const;
    virtual void loadFromStream(ifstream& in);
};

// -----------------------------
// ActionMovie
// -----------------------------
class ActionMovie : public Movie {
private:
    int actionIntensity;

public:
    ActionMovie();
    ActionMovie(const char* id, const char* title, double rating, int duration,
        int releaseYear, const char* hall, const char* date,
        const char* startTime, const char* endTime, int actionIntensity);

    double calculateTicketPrice() const override;
    Genre getGenre() const override;

    void saveToStream(ofstream& out) const override;
    void loadFromStream(ifstream& in) override;
};

// -----------------------------
// DramaMovie
// -----------------------------
class DramaMovie : public Movie {
private:
    bool hasComedyElements;

public:
    DramaMovie();
    DramaMovie(const char* id, const char* title, double rating, int duration,
        int releaseYear, const char* hall, const char* date,
        const char* startTime, const char* endTime, bool hasComedyElements);

    double calculateTicketPrice() const override;
    Genre getGenre() const override;

    void saveToStream(ofstream& out) const override;
    void loadFromStream(ifstream& in) override;
};

// -----------------------------
// DocumentaryMovie
// -----------------------------
class DocumentaryMovie : public Movie {
private:
    char theme[50];
    bool isBasedOnTrueEvents;

public:
    DocumentaryMovie();
    DocumentaryMovie(const char* id, const char* title, double rating, int duration,
        int releaseYear, const char* hall, const char* date,
        const char* startTime, const char* endTime,
        const char* theme, bool isBasedOnTrueEvents);

    double calculateTicketPrice() const override;
    Genre getGenre() const override;

    void saveToStream(ofstream& out) const override;
    void loadFromStream(ifstream& in) override;
};
