#include<iostream>
#include<fstream>
#include<string>
#include"Validation.h"
#include"adminDashboard.h"
#include"userDashboard.h"

using namespace std;


class Registration
{
    private:
    string userFileName = "UserFile.txt";
    string adminFileName = "AdminFile.txt";
    Validation validator;

    public:

    void showMenu(){ // Creating the Menu which options
        cout << "===== Welcome to our Car Rental System Program =====" << endl;
        cout << "\n1. Admin\n2. User \n3. Exit" << endl;
        int choice;
        cin >> choice;
        do{
            switch(choice){
                case 1:
                AdminOption();
                break;
                case 2:
                UserOption();
                break;
                case 3:
                cout << "Thanks to using our program!" << endl;
                cout << "Exiting....." << endl;
                exit(0);
                break;
                default:
                cout << "Invalid choice! Please try again.." << endl;
                break;
            }
        } while (choice != 3); 
    } //closing the Menu function

    void AdminOption(){
        cout << "===== Admin =====" << endl;
        cout << "\n1. Registration \n2. Login \n3. Exit" << endl;
        int choiceA;
        cin >> choiceA;
        do{
            switch(choiceA){
                case 1:
                personRegister(true);
                break;
                case 2:
                personLogin(true);
                break;
                case 3:
                cout << "Going back to menu....."<<endl;
                showMenu();
                break;
                default:
                cout << "Invalid choice! Please try again..." << endl;
            }
    } while (choiceA != 3);
    }

    void UserOption(){
        cout << "===== User =====" << endl;
        cout << "\n1. Registration \n2. Login \n3. Exit" << endl;
    int choiceU;
    cin >> choiceU;
    do{
        switch(choiceU){
            case 1:
            personRegister(false);
            break;
            case 2:
            personLogin(false);
            break;
            case 3:
            cout << "Going to back menu..." << endl;
            showMenu();
            break;
            default:
            cout << "Invalid choice! PLease try again..." << endl;
        }
    } while (choiceU != 3);
}

void personRegister(bool isAdmin){
    string username, password;
    string userType = isAdmin ? "Admin" : "User";
    string* fileName = isAdmin ? &adminFileName : &userFileName;
    cin.ignore();

    cout << "===== Registration of " << userType << " =====" << endl;
    cout << "Enter username: ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);

    if (username.empty() || password.empty()) {
        cout << "Username and password cannot be empty!" << endl;
        return;
    }
    if (validator.usernameExists(username, *fileName)) {
        cout << "Username already exists! Please try again." << endl;
        return;
    }
    ofstream file(*fileName, ios::app);
    if (!file) {
        cout << "Error opening file!" << endl;
        return;
    }
    file << username << " " << password << endl;    
    file.close();
    cout << "Registration successful!" << endl;
    cout << "You can now login!" << endl;
    cout << "1. Back to " << userType << " menu" << endl;
    int choice;
    cin >> choice;
    do{

        switch(choice){
            case 1:
            cout << "Going back to " << userType << " menu..." << endl;
            if (isAdmin) {
                AdminOption();
            } else {
                UserOption();
            }
            break;
            default:
            cout << "Invalid choice! Please try again..." << endl;
        }
    } while (choice != 1);
}

bool personLogin(bool isAdmin){
    string username, password;
    string userType = isAdmin ? "Admin" : "User";
    string* fileName = isAdmin ? &adminFileName : &userFileName;
    cin.ignore();

    cout << "===== Login of " << userType << " =====" << endl;
    cout << "Enter username: ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);

    if (validator.validateCredentials(username, password, *fileName)) {
        cout << "Login successful!" << endl;
        
    int choice;
    do{
        cout << "1. Go to " << userType << " dashboard" << endl;
        cout << "2. Back to " << userType << " menu" << endl;
        cin >> choice;
        switch(choice){
            case 1:
            cout << "Going to " << userType << " dashboard..." << endl;
            if (isAdmin) {
                Adashboard admin;
                admin.showAdminDashboard();
            } else {
                Udashboard user;
                user.showUserDashboard();
            }
            break;
            case 2:
            cout << "Going back to " << userType << " menu..." << endl;
            if (isAdmin) {
                AdminOption();
            } else {
                UserOption();
            }
            break;
            default:
            cout << "Invalid choice! Please try again..." << endl;
        }
    } while (choice != 1 && choice != 2);
        return true;
    } else {
        cout << "Invalid username or password!" << endl;
        return false;
    }  
    

    return false; // Ensure all code paths return a value
} // Closing the Registration class
};