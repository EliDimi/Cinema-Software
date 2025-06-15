#define _CRT_SECURE_NO_WARNINGS
#include "Movie.h"
#include <cstring>

// -----------------------------
// Movie (base class)
// -----------------------------

Movie::Movie() : rating(0), duration(0), releaseYear(0) {
    id[0] = '\0';
    title[0] = '\0';
    hall[0] = '\0';
    date[0] = '\0';
    startTime[0] = '\0';
    endTime[0] = '\0';
}

Movie::Movie(const char* id, const char* title, double rating, int duration,
    int releaseYear, const char* hall, const char* date,
    const char* startTime, const char* endTime)
    : rating(rating), duration(duration), releaseYear(releaseYear) {
    strcpy(this->id, id);
    strcpy(this->title, title);
    strcpy(this->hall, hall);
    strcpy(this->date, date);
    strcpy(this->startTime, startTime);
    strcpy(this->endTime, endTime);
}

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
    strcpy(title, newTitle);
}

void Movie::setHall(const char* newHall) {
    strcpy(hall, newHall);
}

void Movie::setRating(double newRating) {
    if (newRating >= 0 && newRating <= 10)
        rating = newRating;
}

void Movie::saveToStream(ofstream& out) const {
    out.write(id, sizeof(id));
    out.write(title, sizeof(title));
    out.write(reinterpret_cast<const char*>(&rating), sizeof(rating));
    out.write(reinterpret_cast<const char*>(&duration), sizeof(duration));
    out.write(reinterpret_cast<const char*>(&releaseYear), sizeof(releaseYear));
    out.write(hall, sizeof(hall));
    out.write(date, sizeof(date));
    out.write(startTime, sizeof(startTime));
    out.write(endTime, sizeof(endTime));
}

void Movie::loadFromStream(ifstream& in) {
    in.read(id, sizeof(id));
    in.read(title, sizeof(title));
    in.read(reinterpret_cast<char*>(&rating), sizeof(rating));
    in.read(reinterpret_cast<char*>(&duration), sizeof(duration));
    in.read(reinterpret_cast<char*>(&releaseYear), sizeof(releaseYear));
    in.read(hall, sizeof(hall));
    in.read(date, sizeof(date));
    in.read(startTime, sizeof(startTime));
    in.read(endTime, sizeof(endTime));
}

// -----------------------------
// ActionMovie
// -----------------------------

ActionMovie::ActionMovie() : actionIntensity(0) {}

ActionMovie::ActionMovie(const char* id, const char* title, double rating, int duration,
    int releaseYear, const char* hall, const char* date,
    const char* startTime, const char* endTime, int intensity)
    : Movie(id, title, rating, duration, releaseYear, hall, date, startTime, endTime),
    actionIntensity(intensity) {
}


Genre ActionMovie::getGenre() const {
    return ACTION;
}

void ActionMovie::saveToStream(ofstream& out) const {
    char type = 'A';
    out.write(&type, sizeof(type));
    Movie::saveToStream(out);
    out.write(reinterpret_cast<const char*>(&actionIntensity), sizeof(actionIntensity));
}

void ActionMovie::loadFromStream(ifstream& in) {
    Movie::loadFromStream(in);
    in.read(reinterpret_cast<char*>(&actionIntensity), sizeof(actionIntensity));
}

// -----------------------------
// DramaMovie
// -----------------------------

DramaMovie::DramaMovie() : hasComedyElements(false) {}

DramaMovie::DramaMovie(const char* id, const char* title, double rating, int duration,
    int releaseYear, const char* hall, const char* date,
    const char* startTime, const char* endTime, bool hasComedy)
    : Movie(id, title, rating, duration, releaseYear, hall, date, startTime, endTime),
    hasComedyElements(hasComedy) {
}


Genre DramaMovie::getGenre() const {
    return DRAMA;
}

void DramaMovie::saveToStream(ofstream& out) const {
    char type = 'D';
    out.write(&type, sizeof(type));
    Movie::saveToStream(out);
    out.write(reinterpret_cast<const char*>(&hasComedyElements), sizeof(hasComedyElements));
}

void DramaMovie::loadFromStream(ifstream& in) {
    Movie::loadFromStream(in);
    in.read(reinterpret_cast<char*>(&hasComedyElements), sizeof(hasComedyElements));
}

// -----------------------------
// DocumentaryMovie
// -----------------------------

DocumentaryMovie::DocumentaryMovie() : isBasedOnTrueEvents(false) {
    theme[0] = '\0';
}

DocumentaryMovie::DocumentaryMovie(const char* id, const char* title, double rating, int duration,
    int releaseYear, const char* hall, const char* date,
    const char* startTime, const char* endTime,
    const char* theme, bool isBasedOnTrueEvents)
    : Movie(id, title, rating, duration, releaseYear, hall, date, startTime, endTime),
    isBasedOnTrueEvents(isBasedOnTrueEvents) {
    strcpy(this->theme, theme);
}


Genre DocumentaryMovie::getGenre() const {
    return DOCUMENTARY;
}

void DocumentaryMovie::saveToStream(ofstream& out) const {
    char type = 'C';
    out.write(&type, sizeof(type));
    Movie::saveToStream(out);
    out.write(theme, sizeof(theme));
    out.write(reinterpret_cast<const char*>(&isBasedOnTrueEvents), sizeof(isBasedOnTrueEvents));
}

void DocumentaryMovie::loadFromStream(ifstream& in) {
    Movie::loadFromStream(in);
    in.read(theme, sizeof(theme));
    in.read(reinterpret_cast<char*>(&isBasedOnTrueEvents), sizeof(isBasedOnTrueEvents));
}

void Movie::setDate(const char* newDate) { strcpy(date, newDate); }
void Movie::setStartTime(const char* t) { strcpy(startTime, t); }
void Movie::setEndTime(const char* t) { strcpy(endTime, t); }

double ActionMovie::calculateTicketPrice() const {
    return 9.0 + actionIntensity * 1.5;
}

double DramaMovie::calculateTicketPrice() const {
    return 7.0 + (hasComedyElements ? 2.0 : 0.0);
}

double DocumentaryMovie::calculateTicketPrice() const {
    return 5.0 + (isBasedOnTrueEvents ? 3.0 : 0.0);
}