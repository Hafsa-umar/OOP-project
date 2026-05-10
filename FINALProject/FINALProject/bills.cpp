#include "bills.h"
#include <string>
#include <iostream>
using namespace std;

Bill::Bill() {
    billid = 0;
    patientId = 0;
    AppointmentId = 0;
    amount = 0;
    status= "unpaid";
    date=" ";
}
Bill::Bill(int ID, int PI, int Ai, float a, std::string s, std::string d) {
    billid = ID;
    patientId = PI;
    AppointmentId = Ai;
    amount = a;
    status = s;
    date = d;
}


int Bill::getbillId() const {
    return billid;
}
float Bill::getAmount() const {
    return amount; 
}
const std::string Bill::getStatus() const { 
    return status; 
}
const std::string Bill::GetDate()const {
    return date;
}
int Bill::getpatientId()const {
    return patientId;
}
int Bill::getAppointmentId()const {
    return AppointmentId;
}
void Bill::setdate(const std::string d) {
    date = d;
}

void Bill::setpatientid(int pi) {
    patientId = pi;
}
void Bill::setStatus(const std::string s) {
    status= s;
}

void Bill::SetAmount(float a) {
    amount = a;
}
void Bill::setbillid(int Id) {
    billid = Id;
}
void Bill::setAppointmentId(int id) {
    AppointmentId = id;
}