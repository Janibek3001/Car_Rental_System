// This is our Header file that we must to use
// Try to make comments

#include<iostream>
using namespace std;

class Car //The class of Car (Written by Janibek)
{
protected:      //Protected members
    string brand;
    string model;
    double Price;

public:
    Car(string b, string m, double p):brand(b), model(m), Price(p){}; // Constructor

    virtual void DisplayCarDetails(){ //Details Display function
        cout << "Brand: " << brand << endl;
        cout << "Model: " << model << endl;
        cout << "Price: " << Price << endl;
    }

    virtual ~Car(){}; // Destructor
};
//sagksdfgka