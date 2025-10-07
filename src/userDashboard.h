#include<iostream>
#include<fstream>
#include<string>

using namespace std;

class Udashboard 
{
private:
    string carsFile = "cars.txt";
    string rentalHistoryFile = "rental_history.txt";

    void displayCar(int id, string brand, string model, int year, double price, bool available) {
        cout << "ID: " << id << "| Brand: " << brand << "| Model: " << model << "| Year: " << year 
        << "| Price per day: $" << price << "| Available: " << (available ? "Yes" : "No") << endl;
    }

    // Find a car by ID
    bool findCar(int id, string& brand, string& model, int& year, double& price, bool& available) {
        ifstream file(carsFile);
        if (!file) {
            cout << "Error opening cars file!" << endl;
            return false;
        }

        int carId;
        string carBrand, carModel;
        int carYear;
        double carPrice;
        bool carAvailable;

        while (file >> carId >> carBrand >> carModel >> carYear >> carPrice >> carAvailable) {
            if (carId == id) {
                brand = carBrand;
                model = carModel;
                year = carYear;
                price = carPrice;
                available = carAvailable;
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }
    
    // Update car availability
    void updateCarAvailability(int id, bool available) {
    ifstream inFile(carsFile);
    ofstream outFile("temp.txt");
    if (!inFile || !outFile) {
        cout << "Error opening files!" << endl;
        return;
    }
    int carId;
    string carBrand, carModel;
    int carYear;
    double carPrice;
    bool carAvailable;
    while (inFile >> carId >> carBrand >> carModel >> carYear >> carPrice >> carAvailable) {
        if (carId == id) {
            outFile << carId << " " << carBrand << " " << carModel << " " << carYear << " " << carPrice << " " << available << endl;
        } else {
            outFile << carId << " " << carBrand << " " << carModel << " " << carYear << " " << carPrice << " " << carAvailable << endl;
        }
    }
    inFile.close();
    outFile.close();
    
    ifstream tempFile("temp.txt");
    ofstream mainFile(carsFile, ios::trunc); // Open with truncate flag to clear contents
    if (!tempFile || !mainFile) {
        cout << "Error opening files for update!" << endl;
        return;
    }
    
    // Copy content from temp file to main file
    string line;
    while (getline(tempFile, line)) {
        mainFile << line << endl;
    }
    
    tempFile.close();
    mainFile.close();
    
    // Remove the temporary file
    remove("temp.txt");
}

    // Record rental in history
    void recordRental(int carId, string username, string action) {
        ofstream file(rentalHistoryFile, ios::app);
        if (!file) {
            cout << "Error opening rental history file!" << endl;
            return;
        }

        // Get current date (simplified for this example)
        string date = "2025-05-11"; // Placeholder for current date

        file << date << " " << username << " " << carId << " " << action << endl;
        file.close();
    }

public:
    Udashboard() {
        // Constructor
    }

    void showUserDashboard() {
        int choice;
        do {
            cout << "\n===== User Dashboard =====" << endl;
            cout << "1. Rent a car" << endl;
            cout << "2. View available cars" << endl;
            cout << "3. View history" << endl;
            cout << "4. Return a car" << endl;
            cout << "5. Logout" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            // Handle invalid input
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Please enter a number." << endl;
                continue;
            }

            switch (choice) {
                case 1:
                    rentCar();
                    break;
                case 2:
                    viewAvailableCars();
                    break;
                case 3:
                    viewHistory();
                    break;
                case 4:
                    returnCar();
                    break;
                case 5:
                    cout << "Logging out from User Dashboard..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
            }
        } while (choice != 5);
    }

    void viewAvailableCars() {
        ifstream file(carsFile);
        if (!file) {
            cout << "Error opening cars file!" << endl;
            return;
        }

        cout << "\n===== Available Cars =====" << endl;
        
        int id, year;
        string brand, model;
        double price;
        bool available;
        bool found = false;

        while (file >> id >> brand >> model >> year >> price >> available) {
            if (available) {
                displayCar(id, brand, model, year, price, available);
                found = true;
            }
        }

        if (!found) {
            cout << "No available cars at the moment." << endl;
        }

        file.close();
    }

    void rentCar() {
        viewAvailableCars();
        
        int carId;
        string username;
        
        cout << "\nEnter your username: ";
        cin >> username;
        cout << "Enter the ID of the car you want to rent: ";
        cin >> carId;
        
        // Handle invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid ID. Please enter a number." << endl;
            return;
        }

        string brand, model;
        int year;
        double price;
        bool available;

        if (findCar(carId, brand, model, year, price, available)) {
            if (available) {
                updateCarAvailability(carId, false);
                recordRental(carId, username, "rented");
                cout << "You have successfully rented the " << brand << " " << model << "." << endl;
            } else {
                cout << "This car is not available for rent." << endl;
            }
        } else {
            cout << "Car with ID " << carId << " not found." << endl;
        }
    }

    void returnCar() {
        string username;
        int carId;
        
        cout << "\nEnter your username: ";
        cin >> username;
        cout << "Enter the ID of the car you want to return: ";
        cin >> carId;
        
        // Handle invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid ID. Please enter a number." << endl;
            return;
        }

        string brand, model;
        int year;
        double price;
        bool available;

        if (findCar(carId, brand, model, year, price, available)) {
            if (!available) {
                updateCarAvailability(carId, true);
                recordRental(carId, username, "returned");
                cout << "You have successfully returned the " << brand << " " << model << "." << endl;
            } else {
                cout << "This car was not rented." << endl;
            }
        } else {
            cout << "Car with ID " << carId << " not found." << endl;
        }
    }

    void viewHistory() {
        string username;
        cout << "\nEnter your username to view your rental history: ";
        cin >> username;
        
        ifstream file(rentalHistoryFile);
        if (!file) {
            cout << "Error opening rental history file or the file is empty!" << endl;
            return;
        }

        cout << "\n===== Rental History for " << username << " =====" << endl;
        
        string date, user, action;
        int carId;
        bool found = false;

        while (file >> date >> user >> carId >> action) {
            if (user == username) {
                string brand, model;
                int year;
                double price;
                bool available;
                
                if (findCar(carId, brand, model, year, price, available)) {
                    cout << "Date: " << date << " | Car: " << brand << " " << model 
                         << " | Action: " << action << endl;
                    found = true;
                } else {
                    cout << "Date: " << date << " | Car ID: " << carId 
                         << " | Action: " << action << " (Car details not found)" << endl;
                    found = true;
                }
            }
        }

        if (!found) {
            cout << "No rental history found for " << username << "." << endl;
        }

        file.close();
    }


};