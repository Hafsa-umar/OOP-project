#include "appointment.h"
#include <string>
#include <iostream>
using namespace std;

Appointment::Appointment() {
    Appointmentid = 0;
    patientId = 0;
    doctorId = 0;
    date= " ";
    time= " ";
    status= "pending";
}
int Appointment::getDoctorId() const {
    return doctorId;
}
int Appointment::getpatientId()const {
    return patientId;
}
const std::string Appointment::getDate() const {
    return date;
}
const std::string Appointment::getTime() const {
    return time; 
}
const std::string Appointment::getstatus() const {
    return status; 
}
int Appointment:: getAppointmentId()const {
    return Appointmentid;
}

void Appointment::setDate(const std::string d) {
    date = d;

}
void Appointment::setTime(const std::string t) {
    time = t;
}
void Appointment::setpatientid(int pi) { ; 
patientId = pi;
}

void Appointment::setdoctorID(int di) {
    doctorId = di;

}

void Appointment::setAppointmentId(int Ad) {
    Appointmentid=Ad;
}


void Appointment::setstatus(const std::string s) {
    status=s;
}

bool Appointment::operator==(const Appointment& a) const {
    return (doctorId == a.doctorId &&
        (date== a.date) &&
        (time == a.time) &&
        (status== "cancelled") != 0 &&
        (a.status== "cancelled") != 0);
}
