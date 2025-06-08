#include "Movie.h"


// ------------ Movie Base Class ------------
Movie::Movie(const char* id, const char* title, double rating, int duration,
    int releaseYear, const char* hall, const char* date,
    const char* startTime, const char* endTime)
    : rating(rating), duration(duration), releaseYear(releaseYear) {
    strncpy(this->id, id, sizeof(this->id));
    strncpy(this->title, title, sizeof(this->title));
    strncpy(this->hall, hall, sizeof(this->hall));
    strncpy(this->date, date, sizeof(this->date));
    strncpy(this->startTime, startTime, sizeof(this->startTime));
    strncpy(this->endTime, endTime, sizeof(this->endTime));
}

Movie::~Movie() {}

const char* Movie::getId() const { return id; }
const char* Movie::getTitle() const { return title; }
const char* Movie::getHall() const { return hall; }
double Movie::getRating() const { return rating; }
int Movie::getDuration() const { return duration; }
int Movie::getReleaseYear() const { return releaseYear; }
const char* Movie::getDate() const { return date; }
const char* Movie::getStartTime() const { return startTime; }
const char* Movie::getEndTime() const { return endTime; }

void Movie::setTitle(const char* newTitle) {
    strncpy(title, newTitle, sizeof(title));
}

void Movie::setHall(const char* newHall) {
    strncpy(hall, newHall, sizeof(hall));
}

// ------------ ActionMovie ------------
ActionMovie::ActionMovie(const char* id, const char* title, double rating, int duration,
    int releaseYear, const char* hall, const char* date,
    const char* startTime, const char* endTime,
    int actionIntensity)
    : Movie(id, title, rating, duration, releaseYear, hall, date, startTime, endTime),
    actionIntensity(actionIntensity) {
}

double ActionMovie::calculateTicketPrice() const {
    return 9.0 + actionIntensity * 1.5;
}

// ------------ DramaMovie ------------
DramaMovie::DramaMovie(const char* id, const char* title, double rating, int duration,
    int releaseYear, const char* hall, const char* date,
    const char* startTime, const char* endTime,
    bool hasComedyElements)
    : Movie(id, title, rating, duration, releaseYear, hall, date, startTime, endTime),
    hasComedyElements(hasComedyElements) {
}

double DramaMovie::calculateTicketPrice() const {
    return 7.0 + (hasComedyElements ? 2.0 : 0.0);
}

// ------------ DocumentaryMovie ------------
DocumentaryMovie::DocumentaryMovie(const char* id, const char* title, double rating, int duration,
    int releaseYear, const char* hall, const char* date,
    const char* startTime, const char* endTime,
    const char* theme, bool isBasedOnTrueEvents)
    : Movie(id, title, rating, duration, releaseYear, hall, date, startTime, endTime),
    isBasedOnTrueEvents(isBasedOnTrueEvents) {
    strncpy(this->theme, theme, sizeof(this->theme));
}

double DocumentaryMovie::calculateTicketPrice() const {
    return 5.0 + (isBasedOnTrueEvents ? 3.0 : 0.0);
}