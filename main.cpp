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
    if (!system.login("admin", "admin123")) {
        system.registerUser("admin", "admin123", true);
        system.logout();
    }

    while (true) {
        cout << "\n1. Register\n2. Login\n3. Exit\n> ";
        int choice;
        cin >> choice;

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
                        cout << "6. Change Movie Hall\n";
                        cout << "7. List Users\n";
                        cout << "8. List User Tickets\n";
                        cout << "9. Remove User\n";
                        cout << "10. Close Hall\n";
                        cout << "11. List User History\n";
                        cout << "12. Rate Movie\n";
                        cout << "13. Logout\n> ";
                        cin >> option;

                        if (option == 1) {
                            // Add movie
                            int genre;
                            cout << "Genre (1=Action, 2=Drama, 3=Documentary): ";
                            cin >> genre;

                            char id[20], title[100], hall[20], date[11], start[6], end[6];
                            double rating;
                            int duration, year;

                            cout << "ID: "; cin >> id;
                            cout << "Title: "; cin.ignore(); cin.getline(title, 100);
                            cout << "Rating: "; cin >> rating;
                            cout << "Duration: "; cin >> duration;
                            cout << "Year: "; cin >> year;
                            cout << "Hall: "; cin >> hall;
                            cout << "Date (YYYY-MM-DD): "; cin >> date;
                            cout << "Start Time (HH:MM): "; cin >> start;
                            cout << "End Time (HH:MM): "; cin >> end;

                            Movie* m = nullptr;
                            if (genre == 1) {
                                int intensity;
                                cout << "Action Intensity (0–20): ";
                                cin >> intensity;
                                m = new ActionMovie(id, title, rating, duration, year, hall, date, start, end, intensity);
                            }
                            else if (genre == 2) {
                                bool comedy;
                                cout << "Has comedy (1 = yes, 0 = no): ";
                                cin >> comedy;
                                m = new DramaMovie(id, title, rating, duration, year, hall, date, start, end, comedy);
                            }
                            else if (genre == 3) {
                                char theme[50];
                                bool real;
                                cout << "Theme: ";
                                cin.ignore(); cin.getline(theme, 50);
                                cout << "Based on real events (1 = yes, 0 = no): ";
                                cin >> real;
                                m = new DocumentaryMovie(id, title, rating, duration, year, hall, date, start, end, theme, real);
                            }

                            if (m != nullptr) {
                                system.addMovie(m);
                                cout << "Movie added.\n";
                            }
                        }

                        else if (option == 2) {
                            // Add hall
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
                            // Remove movie
                            char id[20];
                            cout << "Movie ID to remove: ";
                            cin >> id;
                            if (system.removeMovieById(id)) cout << "Removed.\n";
                            else cout << "Not found.\n";
                        }

                        else if (option == 4) {
                            // Remove hall
                            char name[20];
                            cout << "Hall name to remove: ";
                            cin >> name;
                            if (system.removeHallByName(name)) cout << "Removed.\n";
                            else cout << "Not found.\n";
                        }

                        else if (option == 5) {
                            // Change movie title
                            char id[20], newTitle[100];
                            cout << "Movie ID: ";
                            cin >> id;
                            cout << "New Title: ";
                            cin.ignore(); cin.getline(newTitle, 100);
                            if (system.changeMovieTitle(id, newTitle)) cout << "Updated.\n";
                            else cout << "Not found.\n";
                        }

                        else if (option == 6) {
                            // Change movie hall
                            char id[20], newHall[20];
                            cout << "Movie ID: ";
                            cin >> id;
                            cout << "New Hall: ";
                            cin >> newHall;
                            if (system.changeMovieHall(id, newHall)) cout << "Updated.\n";
                            else cout << "Not found.\n";
                        }

                        else if (option == 7) {
                            system.listUsers();
                        }

                        else if (option == 8) {
                            char name[30];
                            cout << "Username: ";
                            cin >> name;
                            system.listUserTickets(name);
                        }

                        else if (option == 9) {
                            char name[30];
                            cout << "Username to remove: ";
                            cin >> name;
                            if (system.removeUser(name)) cout << "User removed.\n";
                            else cout << "User not found.\n";
                        }

                        else if (option == 10) {
                            char name[20];
                            cout << "Hall to close: ";
                            cin >> name;
                            system.closeHallByName(name);
                        }

                        else if (option == 11) {
                            char name[30];
                            cout << "User for history: ";
                            cin >> name;
                            system.listUserHistory(name);
                        }

                        else if (option == 12) {
                            char id[20];
                            double rating;
                            cout << "Movie ID: ";
                            cin >> id;
                            cout << "New rating: ";
                            cin >> rating;
                            system.rateMovie(id, rating);
                        }

                    } while (option != 13);
                }

                else {
                    // ----- USER MENU -----
                    int opt;
                    do {
                        cout << "\n-- USER MENU --\n";
                        cout << "1. View Tickets\n";
                        cout << "2. List Movies\n";
                        cout << "3. Buy Ticket\n";
                        cout << "4. List History\n";
                        cout << "5. Logout\n> ";
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
                            system.buyTicket(movieId, row, col);
                        }

                        else if (opt == 4) {
                            system.listUserHistory();
                        }

                    } while (opt != 5);
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
