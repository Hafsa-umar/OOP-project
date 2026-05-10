#include "doctor.h"
#include <iostream>
#include <string>
using namespace std;

Doctor::Doctor() : Person() {
    specialization= " ";
    fee = 0;
}
Doctor::Doctor(int ID, std::string n, std::string pass, std::string cont, std::string special, float FEE) 
    :Person(ID, n, pass, cont) {
    specialization = special;
    fee = FEE;
}
float Doctor::getFee() const {
    return fee;
}

const std::string Doctor::getSpecialization() const {
    return specialization;
}

void Doctor:: setspecialization(const std::string special) {
    specialization = special;
}
void Doctor::setfee(const float f) {
    fee = f;
}

bool Doctor::operator==(const Doctor& d) const {
    return this->id == d.id;
}

ostream& operator<<(ostream& out, const Doctor& d) {
    out << d.id << endl << d.name << endl << d.specialization << endl << d.fee;
    return out;
}

void Doctor::displayProfile() const {
    cout << "Doctor Name: "<<"Doctor " << name << endl;
    cout << "Doctor ID: " << id << endl;
    cout << "Doctor's Specialization: " << specialization << endl;
    cout << "Doctor Fee: " << fee << endl;
    cout << "Doctor ContactNO: " << contact << endl;
}
void Doctor::displayMenu()const {
    cout << "1. View today's Appointment" << endl;
    cout << "2. Mark Appointment complete" << endl;
    cout << "3. Mark Appointment No-Show" << endl;
    cout << "4. Write Prescripton" << endl;
    cout << "5. View Medical Patient'sMedical History" << endl;
    cout << "6. Logout" << endl;
}

bool Doctor::login(const std::string& pass) const {
    return password== pass;
}