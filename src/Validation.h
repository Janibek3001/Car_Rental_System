#include<iostream>
#include<fstream>
#include<string>

using namespace std;

class Validation
{
    public:
    Validation(){}; //Constructor

    bool usernameExists(const string& username, const string& filename) {
        ifstream file(filename);
        string fileuser;
        while (file >> fileuser) {
            if (fileuser == username) {
                file.close();
                return true; //Username Found
            }
        }
        file.close();
        return false;
    }

    bool validateCredentials(const string& username, const string& password, const string& filename) {
        ifstream file(filename);
        string fileName, filePassword;
        while (file >> fileName >> filePassword) {
            if (fileName == username && filePassword == password) {
                file.close();
                return true; //Credential match
            }
        }
        file.close();
        return false;
    }
};