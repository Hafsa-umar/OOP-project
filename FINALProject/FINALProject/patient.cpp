#include "Patient.h"
#include <iostream>
#include <string>
using namespace std;

Patient::Patient() : Person() {
    age = 0;
    gender = 'M';
    balance = 0;
}
Patient::Patient(int ID, std::string n, std::string pass, std::string cont, int Age, std::string gen, float bal)
    :Person(ID, n, pass, cont) {
    age = Age;
    gender = gen;
    balance = bal;
}

float Patient::getBalance() const {
    return balance;
}
int Patient::getage()const {
    return age;
}
std::string Patient::getGender()const {
    return gender;
}
void Patient::setAge(int Age) {
    age = Age;
}

void Patient::setBalance(float b) {
    balance = b;
}
void Patient::setgender(std::string gen) {
    gender = gen;
}

void Patient::operator+=(double amount) {
    balance += amount;
}

void Patient::operator-=(double amount) {
    balance -= amount;
}
bool Patient::operator>=(const Patient& obj) {
    return balance >= obj.balance;
}

bool Patient::operator==(const Patient& p) const {
    return this->id == p.id;
}

ostream& operator<<(ostream& out, const Patient& p) {
    out << p.id << endl; 
    out << p.name << endl;
    out<< p.balance;
    return out;
}

void Patient::displayProfile() const {
    cout << "patient id:"<<id << endl;
    cout <<"patient name:"<<name << endl;
    cout << "Balance: " <<balance<<endl;
}
void Patient::displayMenu()const {
    cout << "Welcome," << name << "!" << endl <<
        "Balance: " << "PKR" << balance << endl <<
        "========================" << endl;
    cout << "1. Book Appointment" << endl;
    cout << "2. Cancel Appointment" << endl;
    cout << "3. View My Appointments" << endl;
    cout << "4. View My Medical Records" << endl;
    cout << "5. View My Bills" << endl;
    cout << "6. Pay Bill" << endl;
    cout << "7. Top Up Balance "<< endl;
    cout << "8. Logout"<<endl;
}

bool Patient::login(const std::string& pass) const{
    return password==pass;
}