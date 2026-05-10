#include "person.h"
#include <string>
#include <iostream>
using namespace std;

Person::Person() {
    id = 0;
    name= " ";
    password= " ";
    contact = "00000000000";
}

Person::Person(int ID, const std::string n, const std::string pass,std::string cont) {
    id = ID;
    name= n;
    password= pass;
    contact = cont;
}

int Person::getId() const {
    return id;
}

const std::string Person::getName() const {
    return name;
}
const std::string Person:: getpassword() const {
    return  password;
}
const std::string Person::getContact() const {
    return contact;

}
void Person::setname (std::string n) {
    name = n;
}
void Person::setpassword(std::string pass) {
    password = pass;
}
void Person::setContact(std::string cont) {
    contact = cont;
}
void Person::setID(int id) {
    this->id = id;
}

Person::~Person() {
    
}