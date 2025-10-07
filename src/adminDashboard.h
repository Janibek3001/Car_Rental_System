#include <iostream>
#include <fstream>
#include <string>
#include <exception>

using namespace std;

// Custom exceptions
class FileOpenException : public exception {
public:
    const char* what() const throw() {
        return "Error: Unable to open file!";
    }
};

class CarNotFoundException : public exception {
public:
    const char* what() const throw() {
        return "Error: Car not found!";
    }
};

class CarAddException : public exception {
public:
    const char* what() const throw() {
        return "Error: Unable to add car!";
    }
};

class CarDeleteException : public exception {
public:
    const char* what() const throw() {
        return "Error: Unable to delete car!";
    }
};

class Adashboard {
private:
    string carsFile = "cars.txt";
    string rentalHistoryFile = "rental_history.txt";
    
    // Display a single car entry
    void displayCar(int id, string brand, string model, int year, double price, bool available) {
        cout << "ID: " << id
             << " | Brand: " << brand
             << " | Model: " << model
             << " | Year: " << year
             << " | Price per day: $" << price
             << " | Available: " << (available ? "Yes" : "No") << endl;
    }

    // Find the next available car ID
    int getNextCarId() {
        ifstream file(carsFile);
        if (!file) {
            throw FileOpenException();
        }

        int maxId = 0;
        int id;
        string brand, model;
        int year;
        double price;
        bool available;

        while (file >> id >> brand >> model >> year >> price >> available) {
            if (id > maxId) {
                maxId = id;
            }
        }

        file.close();
        return maxId + 1;
    }

    // Check if a car with given ID exists
    bool carExists(int id) {
        ifstream file(carsFile);
        if (!file) {
            throw FileOpenException();
        }

        int carId;
        string brand, model;
        int year;
        double price;
        bool available;

        while (file >> carId >> brand >> model >> year >> price >> available) {
            if (carId == id) {
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

public:
    Adashboard() {
        // Constructor
    }

    void showAdminDashboard() {
        int choice;
        do {
            cout << "\n===== Admin Dashboard =====" << endl;
            cout << "1. Add car" << endl;
            cout << "2. Delete car" << endl;
            cout << "3. View history" << endl;
            cout << "4. View all cars" << endl;
            cout << "5. Logout" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addCar();
                    break;
                case 2:
                    deleteCar();
                    break;
                case 3:
                    viewHistory();
                    break;
                case 4:
                    viewAllCars();
                    break;
                case 5:
                    cout << "Logging out from Admin Dashboard..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
            }
        } while (choice != 5);
    }

    void addCar() {
        try {
            string brand, model;
            int year;
            double price;
            
            cout << "\n===== Add a New Car =====" << endl;
            
            // Clear input buffer
            cin.ignore();
            
            cout << "Enter car brand: ";
            getline(cin, brand);
            
            cout << "Enter car model: ";
            getline(cin, model);
            
            cout << "Enter car year: ";
            cin >> year;
            
            // Input validation for year
            if (cin.fail() || year < 1900 || year > 2030) {
                cin.clear();
                cin.ignore(10000, '\n');
                throw invalid_argument("Invalid year. Must be between 1900 and 2030.");
            }
            
            cout << "Enter price per day: $";
            cin >> price;
            
            // Input validation for price
            if (cin.fail() || price <= 0) {
                cin.clear();
                cin.ignore(10000, '\n');
                throw invalid_argument("Invalid price. Must be a positive number.");
            }
            
            int id = getNextCarId();
            bool available = true;
            
            // Add car to file
            ofstream file(carsFile, ios::app);
            if (!file) {
                throw FileOpenException();
            }
            
            file << id << " " << brand << " " << model << " " << year << " " << price << " " << available << endl;
            file.close();
            
            cout << "Car added successfully with ID: " << id << endl;
        } catch (const invalid_argument& e) {
            cout << e.what() << endl;
        } catch (const FileOpenException& e) {
            cout << e.what() << endl;
        } catch (const exception& e) {
            cout << "An error occurred: " << e.what() << endl;
        }
    }

    void deleteCar() {
 try {
    viewAllCars();
    int id;
    cout << "\nEnter the ID of the car you want to delete: ";
    cin >> id;
    // Input validation
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        throw invalid_argument("Invalid ID. Must be a number.");
    }
    if (!carExists(id)) {
        throw CarNotFoundException();
    }
    ifstream inFile(carsFile);
    ofstream outFile("temp.txt");
    if (!inFile || !outFile) {
        throw FileOpenException();
    }
    int carId;
    string brand, model;
    int year;
    double price;
    bool available;
    bool deleted = false;
    while (inFile >> carId >> brand >> model >> year >> price >> available) {
        if (carId != id) {
            outFile << carId << " " << brand << " " << model << " " << year << " " << price << " " << available << endl;
        } else {
            deleted = true;
        }
    }
    inFile.close();
    outFile.close();
    
    ifstream tempFile("temp.txt");
    ofstream mainFile(carsFile, ios::trunc); // Open with truncate flag to clear contents
    if (!tempFile || !mainFile) {
        throw FileOpenException();
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
    
    if (deleted) {
        cout << "Car with ID " << id << " has been deleted." << endl;
    } else {
        throw CarDeleteException();
    }
 } catch (const invalid_argument& e) {
    cout << e.what() << endl;
 } catch (const CarNotFoundException& e) {
    cout << e.what() << endl;
 } catch (const FileOpenException& e) {
    cout << e.what() << endl;
 } catch (const CarDeleteException& e) {
    cout << e.what() << endl;
 } catch (const exception& e) {
    cout << "An error occurred: " << e.what() << endl;
 }
}

    void viewAllCars() {
        try {
            ifstream file(carsFile);
            if (!file) {
                throw FileOpenException();
            }
        
        cout << "\n===== All Cars =====" << endl;
        
        int id, year;
        string brand, model;
        double price;
        bool available;
        bool found = false;
        
        while (file >> id >> brand >> model >> year >> price >> available) {
            displayCar(id, brand, model, year, price, available);
            found = true;
        }
        
        if (!found) {
            cout << "No cars in the system." << endl;
        }
        
        file.close();
        } catch (const FileOpenException& e) {
            cout << e.what() << endl;
        } catch (const exception& e) {
            cout << "An error occurred: " << e.what() << endl;
        }
    }

    void viewHistory() {
        try {
            ifstream file(rentalHistoryFile);
            if (!file) {
                throw FileOpenException();
            }
        
        cout << "\n===== Complete Rental History =====" << endl;
        
        string date, username, action;
        int carId;
        bool found = false;
        
        while (file >> date >> username >> carId >> action) {
            cout << "Date: " << date
                 << " | User: " << username
                 << " | Car ID: " << carId
                 << " | Action: " << action << endl;
            found = true;
        }
        
        if (!found) {
            cout << "No rental history found." << endl;
        }
        
        file.close();
        } catch (const FileOpenException& e) {
            cout << e.what() << endl;
        } catch (const exception& e) {
            cout << "An error occurred: " << e.what() << endl;
        }
    }
};

