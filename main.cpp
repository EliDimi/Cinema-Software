#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "CinemaSystem.h"

using namespace std;

int main() {
    // Create system instance
    CinemaSystem system;

    // Load saved data
    system.loadUsersFromFile("users.dat");
    system.loadMoviesFromFile("movies.dat");
    system.loadHallsFromFile("halls.dat");

    cout << "Cinema System Started.\n";

    // Ensure default admin exists
    if (!system.login("admin", "123")) {
        system.registerUser("admin", "123", true);
        system.logout();
    }

    while (true) {
        cout << "\n1. Register\n2. Login\n3. Exit\n> ";
        int choice;

        if (!(cin >> choice)) {
            cin.clear();                 
            cin.ignore(10000, '\n');   
            cout << "Invalid input. Please enter 1, 2, or 3.\n";
            continue;
        }

        if (choice == 1) {
            char username[30], password[30];
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (system.registerUser(username, password)) {
                cout << "Registered successfully.\n";
            }
            else {
                cout << "Username already exists.\n";
            }
        }
        else if (choice == 2) {
            char username[30], password[30];
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            if (system.login(username, password)) {
                cout << "Login successful.\n";
                User* current = system.getCurrentUser();

                if (current->getIsAdmin()) {
                    // ----- ADMIN MENU -----
                    int option;
                    do {
                        cout << "\n-- ADMIN MENU --\n";
                        cout << "1. Add Movie\n";
                        cout << "2. Add Hall\n";
                        cout << "3. Remove Movie\n";
                        cout << "4. Remove Hall\n";
                        cout << "5. Change Movie Title\n";
                        cout << "6. List Users\n";
                        cout << "7. List User Tickets\n";
                        cout << "8. Remove User\n";
                        cout << "9. Close Hall\n";
                        cout << "10. List User History\n";
                        cout << "11. Rate Movie\n";
                        cout << "12. Change Movie Hall\n";
                        cout << "13. Change Movie Date\n";
                        cout << "14. Change Movie Start/End Time\n";
                        cout << "15. View Halls\n";
                        cout << "16. Logout\n> ";
                        cin >> option;

                        if (option == 1) {
                            int genre;
                            cout << "Genre (1=Action, 2=Drama, 3=Documentary): ";
                            cin >> genre;

                            char id[20], hallName[20], date[11], start[6], end[6];
                            char title[100];
                            double rating;
                            int duration, year;

                            cout << "ID: "; cin >> id;
                            cout << "Title: "; cin.ignore(); cin.getline(title, 100);
                            cout << "Rating: "; cin >> rating;
                            cout << "Duration: "; cin >> duration;
                            cout << "Year: "; cin >> year;
                            cout << "Hall: "; cin >> hallName;
                            cout << "Date (YYYY-MM-DD): "; cin >> date;
                            cout << "Start Time (HH:MM): "; cin >> start;
                            cout << "End Time (HH:MM): "; cin >> end;

                            // Date format validation
                            auto validDate = [&](const char* d) {
                                int y, mo, da;
                                if (sscanf(d, "%4d-%2d-%2d", &y, &mo, &da) != 3) return false;
                                if (mo < 1 || mo > 12 || da < 1 || da > 31) return false;
                                return true;
                                };
                            if (!validDate(date)) {
                                cout << "Error: date must be YYYY-MM-DD\n";
                                break;
                            }
                            // Time format + start<end
                            auto validTime = [&](const char* t) {
                                int h, m;
                                if (sscanf(t, "%2d:%2d", &h, &m) != 2) return false;
                                return (h >= 0 && h < 24 && m >= 0 && m < 60);
                                };
                            if (!validTime(start) || !validTime(end)) {
                                cout << "Error: time must be HH:MM  (00:00–23:59)\n";
                                break;
                            }
                            int sMin = CinemaSystem::toMinutes(start);
                            int eMin = CinemaSystem::toMinutes(end);
                            if (sMin < 0 || eMin < 0) {
                                cout << "Error parsing time\n";
                                break;
                            }
                            if (!system.isTimeSlotFree(hallName, date, start, end)) {
                                cout << "Error: time slot overlaps an existing showing in that hall\n";
                                break;
                            }

                            Movie* m = nullptr;
                            if (genre == 1) {
                                int intensity;
                                cout << "Action intensity (0–20): ";
                                cin >> intensity;
                                m = new ActionMovie(id, title, rating, duration, year,
                                    hallName, date, start, end, intensity);
                            }
                            else if (genre == 2) {
                                bool hasComedy;
                                cout << "Has comedy elements (1=yes,0=no): ";
                                cin >> hasComedy;
                                m = new DramaMovie(id, title, rating, duration, year,
                                    hallName, date, start, end, hasComedy);
                            }
                            else if (genre == 3) {
                                char theme[50];
                                bool based;
                                cout << "Theme: "; cin.ignore(); cin.getline(theme, 50);
                                cout << "Based on true events (1=yes,0=no): ";
                                cin >> based;
                                m = new DocumentaryMovie(id, title, rating, duration, year,
                                    hallName, date, start, end, theme, based);
                            }
                            else {
                                cout << "Error: invalid genre selection\n";
                                break;
                            }

                            system.addMovie(m);
                            system.saveMoviesToFile("movies.dat");
                            cout << "Movie added successfully.\n";
                        }

                        else if (option == 2) {
                            char name[20];
                            int rows, cols;
                            cout << "Hall name: ";
                            cin >> name;
                            cout << "Rows: ";
                            cin >> rows;
                            cout << "Columns: ";
                            cin >> cols;
                            Hall h(name, rows, cols);
                            system.addHall(h);
                            cout << "Hall added.\n";
                        }

                        else if (option == 3) {
                            char id[20];
                            cout << "Movie ID to remove: ";
                            cin >> id;
                            if (system.removeMovieById(id)) {
                                cout << "Removed.\n";
                                system.saveMoviesToFile("movies.dat");
                                system.saveUsersToFile("users.dat");
                            }
                            else {
                                cout << "Not found.\n";
                            }
                        }

                        else if (option == 4) {
                            char name[20];
                            cout << "Hall name to remove: ";
                            cin >> name;
                            if (system.removeHallByName(name)) {
                                cout << "Removed.\n";
                                system.saveHallsToFile("halls.dat");
                            }
                            else {
                                cout << "Not found.\n";
                            }
                        }

                        else if (option == 5) {
                            char id[20], newTitle[100];
                            cout << "Movie ID: ";
                            cin >> id;
                            cout << "New Title: ";
                            cin.ignore(); cin.getline(newTitle, 100);
                            if (system.changeMovieTitle(id, newTitle)) {
                                cout << "Updated.\n";
                                system.saveMoviesToFile("movies.dat");
                            }
                            else {
                                cout << "Not found.\n";
                            }
                        }

                        else if (option == 6) {
                            system.listUsers();
                        }

                        else if (option == 7) {
                            char name[30];
                            cout << "Username: ";
                            cin >> name;
                            system.listUserTickets(name);
                        }

                        else if (option == 8) {
                            char name[30];
                            cout << "Username to remove: ";
                            cin >> name;
                            if (system.removeUser(name)) {
                                cout << "User removed.\n";
                                system.saveUsersToFile("users.dat");
                            }
                            else {
                                cout << "User not found.\n";
                            }
                        }

                        else if (option == 9) {
                            char name[20];
                            cout << "Hall to close: ";
                            cin >> name;
                            system.closeHallByName(name);
                            system.saveMoviesToFile("movies.dat");
                            system.saveHallsToFile("halls.dat");
                        }

                        else if (option == 10) {
                            char name[30];
                            cout << "User for history: ";
                            cin >> name;
                            system.listUserHistory(name);
                        }

                        else if (option == 11) {
                            char id[20];
                            double rating;
                            cout << "Movie ID: ";
                            cin >> id;
                            cout << "New rating: ";
                            cin >> rating;
                            system.rateMovie(id, rating);
                        }

                        else if (option == 12) {
                            char id[20], newHall[20];
                            cout << "Movie ID: ";
                            cin >> id;
                            cout << "New Hall: ";
                            cin >> newHall;
                            if (system.changeMovieHallIfPossible(id, newHall)) {
                                cout << "Updated.\n";
                                system.saveMoviesToFile("movies.dat");
                            }
                            else {
                                cout << "Not updated.\n";
                            }
                        }

                        else if (option == 13) {
                            char id[20], newDate[11];
                            cout << "Movie ID: ";
                            cin >> id;
                            cout << "New Date (YYYY-MM-DD): ";
                            cin >> newDate;
                            if (system.changeMovieDateIfPossible(id, newDate)) {
                                cout << "Date updated.\n";
                                system.saveMoviesToFile("movies.dat");
                            }
                            else {
                                cout << "Not updated.\n";
                            }
                        }

                        else if (option == 14) {
                            char id[20], newStart[6], newEnd[6];
                            cout << "Movie ID: ";
                            cin >> id;
                            cout << "New Start Time (HH:MM): ";
                            cin >> newStart;
                            cout << "New End Time (HH:MM): ";
                            cin >> newEnd;
                            if (system.changeMovieTimeIfPossible(id, newStart, newEnd)) {
                                cout << "Time updated.\n";
                                system.saveMoviesToFile("movies.dat");
                            }
                            else {
                                cout << "Not updated.\n";
                            }
                        }

                        else if (option == 15) {
                            system.listHalls();
                        }

                    } while (option != 16);
                }
                else {
                    // ----- USER MENU -----
                    int opt;
                    do {
                        cout << "\n-- USER MENU --\n";
                        cout << "1. View Tickets\n";
                        cout << "2. List Movies\n";
                        cout << "3. Buy Ticket\n";
                        cout << "4. Add Funds\n";
                        cout << "5. View Balance\n";
                        cout << "6. List History\n";
                        cout << "7. Logout\n> ";
                        cin >> opt;

                        if (opt == 1) {
                            current->printTickets();
                        }

                        else if (opt == 2) {
                            system.listMovies();
                        }

                        else if (opt == 3) {
                            char movieId[20];
                            int row, col;
                            cout << "Movie ID: ";
                            cin >> movieId;
                            cout << "Row: ";
                            cin >> row;
                            cout << "Column: ";
                            cin >> col;
                            if (system.buyTicket(movieId, row, col)) {
                                system.saveUsersToFile("users.dat");
                            }
                        }

                        else if (opt == 4) {
                            double amount;
                            cout << "Amount to add: ";
                            cin >> amount;
                            current->addFunds(amount);
                            system.saveUsersToFile("users.dat");
                        }

                        else if (opt == 5) {
                            cout << "Current balance: " << current->getBalance() << " BGN\n";
                        }

                        else if (opt == 6) {
                            system.listUserHistory();
                        }

                    } while (opt != 7);
                }

                system.logout();
            }
            else {
                cout << "Login failed.\n";
            }
        }
        else if (choice == 3) {
            break;
        }
        else {
            cout << "Invalid option.\n";
        }
    }

    // Save everything
    system.saveUsersToFile("users.dat");
    system.saveMoviesToFile("movies.dat");
    system.saveHallsToFile("halls.dat");

    cout << "System data saved. Goodbye!\n";
    return 0;
}
