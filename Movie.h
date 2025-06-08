#pragma once
#include <cstring>
using namespace std;

enum Genre {
    ACTION,
    DRAMA,
    DOCUMENTARY
};

class Movie {
private:
    char id[20];
    char title[100];
    double rating;
    int duration;
    int releaseYear;
    char hall[20];
    char date[11];       // YYYY-MM-DD
    char startTime[6];   // HH:MM
    char endTime[6];     // HH:MM

public:
    Movie(const char* id,
        const char* title,
        double rating,
        int duration,
        int releaseYear,
        const char* hall,
        const char* date,
        const char* startTime,
        const char* endTime);

    virtual ~Movie();

    virtual double calculateTicketPrice() const = 0;

    const char* getId() const;
    const char* getTitle() const;
    const char* getHall() const;
    double getRating() const;
    int getDuration() const;
    int getReleaseYear() const;
    const char* getDate() const;
    const char* getStartTime() const;
    const char* getEndTime() const;

    void setTitle(const char* newTitle);
    void setHall(const char* newHall);
};

class ActionMovie : public Movie {
private:
    int actionIntensity;
public:
    ActionMovie(const char* id, const char* title, double rating, int duration,
        int releaseYear, const char* hall, const char* date,
        const char* startTime, const char* endTime,
        int actionIntensity);

    double calculateTicketPrice() const override;
};

class DramaMovie : public Movie {
private:
    bool hasComedyElements;
public:
    DramaMovie(const char* id, const char* title, double rating, int duration,
        int releaseYear, const char* hall, const char* date,
        const char* startTime, const char* endTime,
        bool hasComedyElements);

    double calculateTicketPrice() const override;
};

class DocumentaryMovie : public Movie {
private:
    char theme[50];
    bool isBasedOnTrueEvents;
public:
    DocumentaryMovie(const char* id, const char* title, double rating, int duration,
        int releaseYear, const char* hall, const char* date,
        const char* startTime, const char* endTime,
        const char* theme, bool isBasedOnTrueEvents);

    double calculateTicketPrice() const override;
};
